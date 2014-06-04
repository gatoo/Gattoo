#include "stdafx.h"
#include "GattooImg.h"

#include "../../Common/CommonHelpers.h"
#include "../TempFile.h"

#include "Filters/IImgFilter.h"
#include "Filters/EqulizeFilter.h"
#include "Filters/HalfToneFilter.h"

#include <IImgConverter.h>
#include "StorageRoutine/CVolumeAccess.h"
#include "DriveBrowseDlg.h"

#include "../resource.h"

#include <sstream>

CGattooImg::CGattooImg(void)
	: m_enState(enUnknown)
{
}

CGattooImg::CGattooImg(CGattooImg& src)
{
	*this = src;
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
	{
		bResult = filters[i]->Apply(m_Img);
		delete filters[i];
	}

	cv::cvtColor(m_Img, m_Img, CV_GRAY2RGB);

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

	assert(CV_8U == m_Img.depth());

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
/*	if (m_Img.empty()) return true;

	IImgConverter* pConverter = createImgConverter(IImgConverter::enDefault);

	std::string strPath;
	CCommonHelpers::getTempFilePath(strPath);

	cv::cvtColor(m_Img, m_Img, CV_RGB2GRAY);

	CTempFile tmpFile;

	tmpFile.Create();

	strPath.append("pix");
	pConverter->Convert(m_Img, tmpFile);
//	converter.CreateBitmap(strPath.c_str());

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

		//return true;
	}

	CUPDialog progress(AfxGetMainWnd()->GetSafeHwnd(), ThreadProc, (LPVOID)strPath.c_str());

	progress.SetDialogTemplate(nullptr, MAKEINTRESOURCE(IDD_DIALOG_SAVE_PROGRESS), IDC_STATIC_PROGRESS, IDC_PROGRESS, IDC_BUTTON_CANCEL);

	if (IDOK == progress.DoModal())
		MessageBox(AfxGetMainWnd()->GetSafeHwnd(), _T("Image was saved to SD card successfully."), _T("Information"), MB_OK|MB_ICONINFORMATION);
	else
		MessageBox(AfxGetMainWnd()->GetSafeHwnd(), _T("Image saving to SD card was interrupted."), _T("Warning"), MB_OK|MB_ICONEXCLAMATION);
	//startSave(strPath);

	CVolumeAccess::cleanResources();

	pConverter->Destroy();
*/
	return true;
}

bool CGattooImg::ThreadProc(const CUPDUPDATA* pCUPDUPData)
{
	CVolumeAccess *vol = CVolumeAccess::getInstance();

	LPCTSTR szFilePath = (LPCTSTR) pCUPDUPData->GetAppData();
	if (nullptr == szFilePath) return false;

	FILE* pFile = NULL;

	_tfopen_s(&pFile, szFilePath, _T("rb"));
	if (nullptr == pFile) return false;

	fseek(pFile, 0, SEEK_END);
	long lTotal = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	DWORD dwStartSector = 70000;
	size_t readData = 0;
	const size_t iBufSize = vol->getSectorSize();
	std::vector<BYTE> vecData(iBufSize);

	readData = fread(&vecData[0], sizeof(BYTE), iBufSize, pFile);

	const double iProgressStep = 100.0 / ceil((double)lTotal/iBufSize);
	double dCurProgress = iProgressStep;

	std::stringstream strInfo;
	
	strInfo << _T("Completed ");

	std::streampos const iUpdtPos = strInfo.tellp();
	strInfo << "0%";

 	dwStartSector -= vol->getResrvdSctCount();
	while(readData == iBufSize && !pCUPDUPData->ShouldTerminate())
	{
		vol->writeBytesToDeviceSector(&vecData[0], iBufSize, dwStartSector++);
		readData = fread(&vecData[0], sizeof(BYTE), iBufSize, pFile);


		pCUPDUPData->SetProgress(strInfo.str().c_str(), (int)dCurProgress);
		dCurProgress += iProgressStep;

		strInfo.seekp(iUpdtPos);
		strInfo << (int)dCurProgress << _T("%");

#ifdef _DEBUG
		Sleep(50);
#else
		Sleep(0);
#endif

	}

	if (readData)
	{
		memset(&vecData[0] + readData, 0, iBufSize - readData);
		vol->writeBytesToDeviceSector(&vecData[0], iBufSize, dwStartSector);
		pCUPDUPData->SetProgress((int)dCurProgress);
	}

	fclose(pFile);

	return true;
}

void CGattooImg::Draw(CBitmap &bmpOut, CPoint const &ptStart)
{
	BITMAP bmp;

	if (!bmpOut.GetBitmap(&bmp))
	{
		assert(false);
		return;
	}

	int iWidth  = bmp.bmWidth;
	int iHeight = bmp.bmHeight;

	BITMAPINFO bi;
	ZeroMemory(&bi, sizeof(bi));
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biBitCount = 24;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biWidth = iWidth;
	bi.bmiHeader.biHeight = -iHeight;

	int iCurLine = 0;
	unsigned char * lpData = nullptr;

	lpData = m_Img.data+ (ptStart.x*3) + ptStart.y * m_Img.cols * 3;

	for(int i=0; i<iHeight; i++)
	{
		iCurLine = iHeight - i - 1;
		SetDIBits(nullptr, bmpOut, iCurLine, 1, (LPVOID)lpData, &bi, DIB_RGB_COLORS);
		lpData = lpData + m_Img.cols*3;
	}
}

void CGattooImg::DrawPreview(CDC * pDC)
{
// 	if (nullptr == m_previewDC.GetSafeHdc())
// 	{
// 		m_previewDC.CreateCompatibleDC(pDC);
// 	}
}

CGattooImg::EImageState CGattooImg::getState() const
{
	return m_enState;
}

std::string const & CGattooImg::getImagePath() const
{
	return m_strImgPath;
}

char CGattooImg::getImageDepth() const
{
	return (m_Img.empty()) ? 0 : m_Img.channels() * 8;
}

bool CGattooImg::doInverse()
{
	cv::bitwise_not(m_Img, m_Img);
	return true;
}

bool CGattooImg::EraseRect(CRect &rcErase)
{
	cv::Mat t = m_Img(cv::Range(rcErase.top, rcErase.bottom), cv::Range(rcErase.left, rcErase.right));
	t = 0;
	return true;
}

void CGattooImg::CropImage(CRect & rc)
{
	cv::Mat cropped = m_Img(cv::Rect(rc.left, rc.top, rc.Width(), rc.Height()));
	cropped.copyTo(m_Img);
}

bool CGattooImg::saveToFile(LPCTSTR lpszPath)
{
/*	IImgConverter* pConverter = createImgConverter(IImgConverter::enDefault);
	
	FILE* fOut = fopen(lpszPath, "wb");
	
	if (fOut == nullptr) return false;

	cv::cvtColor(m_Img, m_Img, CV_RGB2GRAY);

	pConverter->Convert(m_Img, fOut);
	pConverter->Destroy();

	fclose(fOut);

	cv::cvtColor(m_Img, m_Img, CV_GRAY2RGB);
*/
	return true;
}

bool CGattooImg::Resize(CSize & szNewSize)
{
	cv::Mat dst;

	cv::resize(m_Img, dst, cv::Size(szNewSize.cx, szNewSize.cy), 0, 0, cv::INTER_AREA);

	m_Img = dst;

	return true;
}

bool CGattooImg::IsBinary() const
{
	std::vector<cv::Mat> bgr_planes(3);
	
	cv::split(m_Img, &bgr_planes[0]);

	/// Establish the number of bins
	int histSize = 256;
	
	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 } ;
	const float* histRange = { range };

	cv::Mat b_hist, g_hist, r_hist;

	/// Compute the histograms:
	calcHist( &bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange);
	calcHist( &bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange);
	calcHist( &bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange);

	bool bRes = true;
	int const iImgArea = m_Img.cols * m_Img.rows;
	
	bRes &= (b_hist.at<float>(0) + b_hist.at<float>(255)) == iImgArea;
	bRes &= (g_hist.at<float>(0) + g_hist.at<float>(255)) == iImgArea;
	bRes &= (b_hist.at<float>(0) + b_hist.at<float>(255)) == iImgArea;

	return bRes;
}

CGattooImg & CGattooImg::operator=(const CGattooImg & rhs)
{
	if(this == &rhs) return *this;
	
	m_Img = rhs.m_Img.clone();
	m_enState = rhs.m_enState;

	return *this;
}
