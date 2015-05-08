// BaseImgView.cpp : implementation file
//
#include "stdafx.h"
#include "Gattoo.h"
#include "BaseImgView.h"

#include <GlobalSettings.h>

// CBaseImgView

//IMPLEMENT_DYNCREATE(CBaseImgView, CView)

CBaseImgView::CBaseImgView()
	: m_fZoomFactor(1)
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
//	ON_COMMAND(AFX_IDS_SCMAXIMIZE, &CBaseImgView::OnAfxIdsScmaximize)
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

void CBaseImgView::UpdateScrolls(int y)
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
		sinfo.nMax = 0;
		sinfo.nPage = 0;

		m_iMaxXScroll = 0;

		if(rcClient.Width() < sizeImg.cx * m_fZoomFactor)
		{
			m_iMaxXScroll = (int)(sizeImg.cx * m_fZoomFactor) - rcClient.Width();
			sinfo.nMax = (int)(sizeImg.cx * m_fZoomFactor);
			sinfo.nPage = rcClient.Width();

			SetScrollInfo(SB_HORZ, &sinfo);
		}
		else
		{
			ShowScrollBar(SB_HORZ, FALSE);
		}

		m_iMaxYScroll = 0;

		if(rcClient.Height() < sizeImg.cy * m_fZoomFactor)
		{
			m_iMaxYScroll = (int)(sizeImg.cy * m_fZoomFactor) - rcClient.Height();
			sinfo.nMax = (int)(sizeImg.cy * m_fZoomFactor);
			sinfo.nPage = rcClient.Height();

			SetScrollInfo(SB_VERT, &sinfo);
		}
		else
		{
			ShowScrollBar(SB_VERT, FALSE);
		}

		
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

void CBaseImgView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if (lHint == CGattooDoc::IMAGE_LOAD_EVENT)
		OnDocumentLoad();
}

void CBaseImgView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	UpdateScrolls(cy);
}