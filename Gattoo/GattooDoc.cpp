
// GattooDoc.cpp : implementation of the CGattooDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Gattoo.h"
#endif

#include "GattooDoc.h"
#include "GlobalSettings.h"

#include <propkey.h>
#include <sstream>
#include <iomanip>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGattooDoc

IMPLEMENT_DYNCREATE(CGattooDoc, CDocument)

BEGIN_MESSAGE_MAP(CGattooDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, &CGattooDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_RAW, &CGattooDoc::OnFileSaveRaw)
END_MESSAGE_MAP()


// CGattooDoc construction/destruction

CGattooDoc::CGattooDoc() : m_pCurrImg(nullptr)
{
	// TODO: add one-time construction code here

}

CGattooDoc::~CGattooDoc()
{
}

// CGattooDoc serialization

void CGattooDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CGattooDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CGattooDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGattooDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGattooDoc diagnostics

#ifdef _DEBUG
void CGattooDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGattooDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGattooDoc commands
BOOL CGattooDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	if (!m_ImgOrig.Load(lpszPathName)) return FALSE;
	if (!m_ImgForPrint.Load(lpszPathName)) return FALSE;

	if (!m_ImgOrig.IsBinary())
		m_ImgForPrint.doHalfTone();

	m_pCurrImg = &m_ImgOrig;

	SetPathName(lpszPathName);
	SendMessage(AfxGetMainWnd()->GetSafeHwnd(), IDM_USER_IMG_LOADED, 0, 0);

	UpdateAllViews(nullptr);

	return TRUE;
}

CSize CGattooDoc::getImgSize() const
{
	return m_pCurrImg ? m_pCurrImg->getImgSize() : CSize(0,0);
}

void const * CGattooDoc::getImgData() const
{
	return m_pCurrImg ? m_pCurrImg->getImgData() : nullptr;
}

void CGattooDoc::OnFileSave()
{
	m_ImgForPrint.saveToSD();
}

void CGattooDoc::OnFileSaveRaw()
{
	static TCHAR szFilter[] = _T("Raw print files (*.pix)|*.pix||");
	CFileDialog dlg(FALSE, _T("pix"), nullptr, OFN_ENABLESIZING | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, szFilter, AfxGetMainWnd(), sizeof(OPENFILENAME), TRUE);

	if (IDOK == dlg.DoModal())
		m_ImgForPrint.saveToFile(dlg.GetPathName());
}

void CGattooDoc::PerformDrawing(CBitmap &bmpOut, CPoint const &ptStart)
{
	if (m_pCurrImg)
		m_pCurrImg->Draw(bmpOut, ptStart);
}

std::string const CGattooDoc::getImgDimension() const
{
	std::stringstream str;

	CSize sz = m_ImgOrig.getImgSize();
	str << sz.cx << "x" << sz.cy;

	return str.str();
}

std::string const CGattooDoc::getImgPrintMMDimension() const
{
	std::stringstream str;

	CSize sz = m_ImgForPrint.getImgSize();
	str << sz.cx * CStaticSettings::HZ_SIZE_SCALE << "x" << sz.cy * CStaticSettings::VT_SIZE_SCALE;

	return str.str();
}

std::string const CGattooDoc::getImgPrintPXDimension() const
{
	std::stringstream str;

	CSize sz = m_ImgForPrint.getImgSize();
	str << sz.cx << "x" << sz.cy;

	return str.str();
}

std::string const CGattooDoc::getImgModification() const
{
	WIN32_FILE_ATTRIBUTE_DATA fileAttrData = {0};
	GetFileAttributesEx(GetPathName(), GetFileExInfoStandard, &fileAttrData);

	SYSTEMTIME stLocal = {0};
	FILETIME ftLastModifiedDate = fileAttrData.ftLastWriteTime;

	FileTimeToLocalFileTime(&ftLastModifiedDate, &ftLastModifiedDate);
	FileTimeToSystemTime(&ftLastModifiedDate, &stLocal);

	std::stringstream str;

	str << std::setw(2) << std::setfill('0');
	str << stLocal.wHour << ":";
	str << std::setw(2) << std::setfill('0');
	str << stLocal.wMinute << ":";
	str << std::setw(2) << std::setfill('0');
	str << stLocal.wSecond;
	str << " ";
	str << std::setw(2) << std::setfill('0');
	str << stLocal.wDay << ".";
	str << std::setw(2) << std::setfill('0');
	str << stLocal.wMonth << ".";
	str << std::setw(2) << std::setfill('0');
	str << stLocal.wYear;

	return str.str();
}

std::string const CGattooDoc::getImgDepth() const
{
	std::stringstream str;
	str << (int) m_ImgOrig.getImageDepth();
	return str.str();
}

void CGattooDoc::SwitchToOriginal(BOOL bOrig)
{
	m_pCurrImg = bOrig ? &m_ImgOrig : &m_ImgForPrint;
}

CGattooImg::EImageState CGattooDoc::GetDocumentState()
{
	return m_pCurrImg->getState();
}

void CGattooDoc::doInverse()
{
	m_pCurrImg->doInverse();
}

bool CGattooDoc::EraseRect(CRect &rcErase)
{
	return m_pCurrImg->EraseRect(rcErase);
}

void CGattooDoc::CropImage(CRect &rc)
{
	m_pCurrImg->CropImage(rc);
	SendMessage(AfxGetMainWnd()->GetSafeHwnd(), IDM_USER_IMG_LOADED, 0, 0);
}

bool CGattooDoc::resizeImage(CSize &szNewSize)
{
	CGattooImg imgTemp(m_ImgOrig);

	imgTemp.Resize(szNewSize);

	if (!imgTemp.IsBinary())
		imgTemp.doHalfTone();

	m_ImgForPrint = imgTemp;
	m_pCurrImg = &m_ImgForPrint;

	SendMessage(AfxGetMainWnd()->GetSafeHwnd(), IDM_USER_IMG_LOADED, 0, 0);

	return true;
}
