#include "stdafx.h"
#include "GattooImg.h"

#include "../CommonHelpers.h"

#include "Filters/IImgFilter.h"
#include "Filters/EqulizeFilter.h"
#include "Filters/HalfToneFilter.h"

#include "ImgConverters/ImgConverter.h"
#include "StorageRoutine/CVolumeAccess.h"
#include "DriveBrowseDlg.h"

#include "ProgressDlg.h"

CGattooImg::CGattooImg(void)
	: m_bIsChanged(false)
	, m_BMPBuff(nullptr)
	, m_ZoomFactor(1)
	, m_enState(enUnknown)
{
}

CGattooImg::~CGattooImg(void)
{
}

unsigned char CGattooImg::getColor(unsigned char chLevel)
{
	unsigned char chRes = 0;

	switch (chLevel)
	{
	case 7: chRes = 0xFF; break;
	case 6: chRes = 0xD8; break;
	case 5: chRes = 0xB4; break;
	case 4: chRes = 0x90; break;
	case 3: chRes = 0x6C; break;
	case 2: chRes = 0x48; break;
	case 1: chRes = 0x24; break;
	case 0: chRes = 0x00; break;
	
	default: assert(false); break;
	}

	return chRes;
}

char CGattooImg::getLevel(uchar intencity)
{
	if (intencity < 0x20)
		return 0;
	else if (intencity < 0x40)
		return 1;
	else if (intencity < 0x60)
		return 2;
	else if (intencity < 0x80)
		return 3;
	else if (intencity < 0xA0)
		return 4;
	else if (intencity < 0xC0)
		return 5;
	else if (intencity < 0xE0)
		return 6;
	else
		return 7;
}

bool CGattooImg::doHalfTone()
{
	bool bResult = true;
	std::vector<IImgFilter*> filters;

	if (m_Img.empty())
		return false;

	filters.push_back(new CEqulizeFilter());
	filters.push_back(new CHalfToneFilter());

	cv::cvtColor(m_Img, m_Img, CV_RGB2GRAY);

	for(size_t i=0; i<filters.size() && bResult; ++i)
		bResult = filters[i]->Apply(m_Img);

	cv::cvtColor(m_Img, m_Img, CV_GRAY2RGB);

	m_bIsChanged = bResult;
	m_enState = enHalftone;

	return bResult;
}

CSize CGattooImg::getImgSize() const
{
	return CSize(m_Img.cols, m_Img.rows);
}

void const * CGattooImg::getImgData() const
{
	return m_Img.data;
}

bool CGattooImg::Load(LPCSTR lpszFilePath)
{
	m_Img = cv::imread(lpszFilePath);
	m_bIsChanged = true;

	m_strImgPath = lpszFilePath;

	m_enState = enInitial;

	return !m_Img.empty();
}

bool CGattooImg::getDriveToSave(std::basic_string<TCHAR> &strDrive)
{
	TCHAR chDrive = 0;

	// TODO: Make it modal
	CDriveBrowseDlg dlg;
	if (dlg.browseDrive(chDrive))
		strDrive = chDrive;

	return chDrive != 0;
}

bool CGattooImg::saveToSD()
{
	if (m_Img.empty()) return true;

	CImgConverter converter;

	std::string strPath;
	CCommonHelpers::getTempFilePath(strPath);

	cv::cvtColor(m_Img, m_Img, CV_RGB2GRAY);

	strPath.append("pix");
 	converter.Convert(m_Img, strPath.c_str());
 	converter.CreateBitmap(strPath.c_str());

	cv::cvtColor(m_Img, m_Img, CV_GRAY2RGB);

	std::string strDrive;
	getDriveToSave(strDrive);
	if (0 == strDrive.size()) return false;

	CVolumeAccess::setWorkingDriveLetter(strDrive[0]);
	CVolumeAccess *vol = CVolumeAccess::getInstance();

	if (!vol->checkVolumeParams())
	{
		if (IDYES != MessageBox(nullptr, _T("Volume parameters are not valid. Reformat drive (all data will be lost)?"), _T("Warning"), MB_YESNO|MB_ICONWARNING))
			return false;

		//formatDrive(strDrive[0]);

		return true;
	}

	CProgressDlg progress;
	progress.startSave(strPath);

	DeleteFile(strPath.c_str());

	CVolumeAccess::cleanResources();

	return true;
}

void CGattooImg::Draw(CDC* pDC, CRect const &rc)
{
	if (NULL == m_memDC.GetSafeHdc())
		m_memDC.CreateCompatibleDC(pDC);

	BITMAP memBitMap;
	CBitmap* memBMP = m_memDC.GetCurrentBitmap();

	HBITMAP bmp = (HBITMAP) memBMP->GetSafeHandle();
	
	memBMP->GetBitmap(&memBitMap);

	if (memBitMap.bmWidth != rc.Width() || memBitMap.bmHeight != rc.Height())
	{
		BITMAPINFO bi;
		ZeroMemory(&bi, sizeof(bi));
		bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bi.bmiHeader.biCompression = BI_RGB;
		bi.bmiHeader.biBitCount = 24;
		bi.bmiHeader.biPlanes = 1;
		bi.bmiHeader.biWidth = rc.Width();
		bi.bmiHeader.biHeight = -rc.Height();
		bi.bmiHeader.biSizeImage = 0;
		
		bmp = ::CreateDIBSection(NULL, &bi, DIB_RGB_COLORS, (void**) &m_BMPBuff, NULL, 0);
		m_memDC.SelectObject(bmp);

		m_bIsChanged = true;
	}

	if (m_bIsChanged)
	{
		int iPadding = 4 -((rc.Width()*3) % 4);
		iPadding = (iPadding == 4) ? 0 : iPadding;

		int iSrcLineLen = m_Img.cols*3;
		int iDstLineLen = rc.Width()*3 + iPadding;		

		int iXStart = (rc.Width() - m_Img.cols)  / 2;
		int iYStart = (rc.Height() - m_Img.rows) / 2;

		BYTE* pDstBuf = m_BMPBuff + iYStart*iDstLineLen + iXStart*3;
		BYTE* pSrcBuf = m_Img.data;

		m_memDC.FillSolidRect(&rc, GetSysColor(COLOR_APPWORKSPACE));

		for(int i=0; i<m_Img.rows; i++)
		{
			memcpy(pDstBuf, pSrcBuf, iSrcLineLen);
			pDstBuf += iDstLineLen;
			pSrcBuf += iSrcLineLen;
		}
	}

	pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &m_memDC, 0, 0, SRCCOPY);
}

CGattooImg::EImageState CGattooImg::getState() const
{
	return m_enState;
}

std::string const & CGattooImg::getImagePath() const
{
	return m_strImgPath;
}
