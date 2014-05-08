
// GattooDoc.cpp : implementation of the CGattooDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Gattoo.h"
#endif

#include "GattooDoc.h"

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
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CROP, &CGattooDoc::OnUpdateToolsCrop)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ERASER, &CGattooDoc::OnUpdateToolsEraser)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM_IN, &CGattooDoc::OnUpdateToolsZoomIn)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM_OUT, &CGattooDoc::OnUpdateToolsZoomOut)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_HALFTONE, &CGattooDoc::OnUpdateToolsHalftone)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SAVETOSD, &CGattooDoc::OnUpdateToolsSavetosd)
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
	 

	m_ImgForPrint.doHalfTone();

	m_pCurrImg = &m_ImgOrig;

	SetPathName(lpszPathName);

	SendMessage(AfxGetMainWnd()->GetSafeHwnd(), IDM_USER_IMG_LOADED, 0, 0);

	return TRUE;
}

CSize CGattooDoc::getImgSize() const
{
	return m_ImgOrig.getImgSize();
}

void const * CGattooDoc::getImgData() const
{
	return m_ImgOrig.getImgData();
}

void CGattooDoc::OnFileSave()
{
	m_ImgOrig.saveToSD();
}

void CGattooDoc::PerformDrawing(CDC* pDC, CRect const &rc)
{
	if (m_pCurrImg)
		m_pCurrImg->Draw(pDC, rc);
}

void CGattooDoc::OnToolsHalftone()
{
	m_ImgOrig.doHalfTone();
}

void CGattooDoc::OnToolsSaveToSD()
{
	m_ImgOrig.saveToSD();
}

void CGattooDoc::OnUpdateToolsCrop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_ImgOrig.getState() != CGattooImg::enUnknown);
}

void CGattooDoc::OnUpdateToolsEraser(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_ImgOrig.getState() != CGattooImg::enUnknown);
}

void CGattooDoc::OnUpdateToolsZoomIn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_ImgOrig.getState() != CGattooImg::enUnknown);
}

void CGattooDoc::OnUpdateToolsZoomOut(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_ImgOrig.getState() != CGattooImg::enUnknown);
}

void CGattooDoc::OnUpdateToolsHalftone(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_ImgOrig.getState() == CGattooImg::enInitial);
}

void CGattooDoc::OnUpdateToolsSavetosd(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_ImgOrig.getState() == CGattooImg::enHalftone);
}

std::string const CGattooDoc::getImgDimension() const
{
	std::stringstream str;

	CSize sz = getImgSize();
	str << sz.cx << "x" << sz.cy;

	return str.str();
}

std::string const CGattooDoc::getImgPrintDimension() const
{
	const double dScale = 0.25;
	std::stringstream str;

	CSize sz = getImgSize();
	str << sz.cx * dScale << "x" << sz.cy * dScale;

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
