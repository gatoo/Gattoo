
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGattooDoc

IMPLEMENT_DYNCREATE(CGattooDoc, CDocument)

BEGIN_MESSAGE_MAP(CGattooDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, &CGattooDoc::OnFileSave)
END_MESSAGE_MAP()


// CGattooDoc construction/destruction

CGattooDoc::CGattooDoc()
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

	return m_Img.Load(lpszPathName);
}

CSize CGattooDoc::getImgSize() const
{
	return m_Img.getImgSize();
}

void const * CGattooDoc::getImgData() const
{
	return m_Img.getImgData();
}

void CGattooDoc::OnFileSave()
{
	//m_Img.saveToSD();
}

void CGattooDoc::PerformDrawing(CDC* pDC, CRect const &rc)
{
	m_Img.Draw(pDC, rc);
}

void CGattooDoc::OnToolsHalftone()
{
	m_Img.doHalfTone();
}

void CGattooDoc::OnToolsSaveToSD()
{
	m_Img.saveToSD();
}
