// BaseImgView.cpp : implementation file
//
#include "stdafx.h"
#include "Gattoo.h"
#include "BaseImgView.h"

#include <GlobalSettings.h>

// CBaseImgView

//IMPLEMENT_DYNCREATE(CBaseImgView, CView)

CBaseImgView::CBaseImgView()
{
}

CBaseImgView::~CBaseImgView()
{
}

BEGIN_MESSAGE_MAP(CBaseImgView, CView)
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
END_MESSAGE_MAP()

#ifdef _DEBUG
void CBaseImgView::AssertValid() const
{
	CView::AssertValid();
}

void CBaseImgView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGattooDoc* CBaseImgView::GetDocument() const // non-debug version is inline
{
	if (!m_pDocument) return nullptr;

	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGattooDoc)));
	return (CGattooDoc*)m_pDocument;
}
#endif //_DEBUG

BOOL CBaseImgView::OnEraseBkgnd(CDC* pDC)
{
	return 0; //CView::OnEraseBkgnd(pDC);
}

void CBaseImgView::OnUpdate(CBaseImgView* pSender, LPARAM lHint, CObject* pHint)
{
	if (lHint == CGattooDoc::IMAGE_LOAD_EVENT)
		OnDocumentLoad();
}

void CBaseImgView::UpdateScrolls()
{
	CGattooDoc* pDoc = GetDocument();

	if (pDoc)
	{
		CRect rcClient;
		CSize sizeImg = pDoc->getImgSize();

		GetClientRect(&rcClient);

		SCROLLINFO sinfo;

		sinfo.cbSize = sizeof(SCROLLINFO);
		sinfo.fMask = SIF_PAGE | SIF_RANGE;
		sinfo.nMin = 0;

		if(rcClient.Width() < sizeImg.cx)
		{
			m_iMaxXScroll = sizeImg.cx - rcClient.Width();
			sinfo.nMax = sizeImg.cx;
			sinfo.nPage = rcClient.Width();
			SetScrollInfo(SB_HORZ, &sinfo);
		}
		else
			SetScrollRange(SB_HORZ, 0, 0);

		if(rcClient.Height() < sizeImg.cy)
		{
			m_iMaxYScroll = sizeImg.cy - rcClient.Height();
			sinfo.nMax = sizeImg.cy;
			sinfo.nPage = rcClient.Height();

			SetScrollInfo(SB_VERT, &sinfo);
		}
		else
			SetScrollRange(SB_VERT, 0, 0);
	}
}

void CBaseImgView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int uiNewPos = 0;

	SCROLLINFO scrlInfo;

	switch (nSBCode) 
	{ 
		// User clicked the scroll bar shaft left of the scroll box. 
	case SB_PAGEUP:
		GetScrollInfo(SB_HORZ, &scrlInfo, SIF_PAGE);
		uiNewPos = m_ptViewPoint.x - scrlInfo.nPage;
		break; 

		// User clicked the scroll bar shaft right of the scroll box. 
	case SB_PAGEDOWN:
		GetScrollInfo(SB_HORZ, &scrlInfo, SIF_PAGE);
		uiNewPos = m_ptViewPoint.x + scrlInfo.nPage; 
		break; 

		// User clicked the left arrow. 
	case SB_LINEUP: 
		uiNewPos = m_ptViewPoint.x - CStaticSettings::HZ_SCROLL_STEP; 
		break; 

		// User clicked the right arrow. 
	case SB_LINEDOWN: 
		uiNewPos = m_ptViewPoint.x + CStaticSettings::HZ_SCROLL_STEP; 
		break; 

		// User dragged the scroll box. 
	case SB_THUMBPOSITION: 
		uiNewPos = nPos; 
		break; 

	default: 
		uiNewPos = m_ptViewPoint.x; 
	} 

	// New position must be between 0 and the screen width. 
	uiNewPos = std::max<int>(0, uiNewPos); 
	uiNewPos = std::min<int>(m_iMaxXScroll, uiNewPos); 

	SetScrollPos(SB_HORZ, uiNewPos);
	m_ptViewPoint.x = uiNewPos;
	Invalidate();

	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CBaseImgView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int uiNewPos = 0;

	SCROLLINFO scrlInfo;

	switch (nSBCode) 
	{ 
		// User clicked the scroll bar shaft left of the scroll box. 
	case SB_PAGEUP:
		GetScrollInfo(SB_VERT, &scrlInfo, SIF_PAGE);
		uiNewPos = m_ptViewPoint.y - scrlInfo.nPage;
		break; 

		// User clicked the scroll bar shaft right of the scroll box. 
	case SB_PAGEDOWN:
		GetScrollInfo(SB_VERT, &scrlInfo, SIF_PAGE);
		uiNewPos = m_ptViewPoint.y + scrlInfo.nPage; 
		break; 

		// User clicked the left arrow. 
	case SB_LINEUP: 
		uiNewPos = m_ptViewPoint.y - CStaticSettings::VT_SCROLL_STEP; 
		break; 

		// User clicked the right arrow. 
	case SB_LINEDOWN: 
		uiNewPos = m_ptViewPoint.y + CStaticSettings::VT_SCROLL_STEP; 
		break; 

		// User dragged the scroll box. 
	case SB_THUMBPOSITION: 
		uiNewPos = nPos; 
		break; 

	default: 
		uiNewPos = m_ptViewPoint.y; 
	} 

	// New position must be between 0 and the screen width. 
	uiNewPos = std::max<int>(0, uiNewPos); 
	uiNewPos = std::min<int>(m_iMaxYScroll, uiNewPos); 

	SetScrollPos(SB_VERT, uiNewPos);
	m_ptViewPoint.y = uiNewPos;
	Invalidate();

	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CBaseImgView::OnSize(UINT nType, int cx, int cy)
{
	UpdateScrolls();
	CView::OnSize(nType, cx, cy);
}

void CBaseImgView::OnDraw(CDC* pDC)
{
	throw std::exception("The method or operation is not implemented.");
}

void CBaseImgView::OnDocumentLoad()
{
	throw std::exception("The method or operation is not implemented.");
}
