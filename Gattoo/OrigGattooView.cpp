// OrigGattooView.cpp : implementation file
//

#include "stdafx.h"
#include "Gattoo.h"
#include "OrigGattooView.h"
#include "GattooDoc.h"

#include <GlobalSettings.h>

// COrigGattooView

IMPLEMENT_DYNCREATE(COrigGattooView, CView)

COrigGattooView::COrigGattooView()
{
}

COrigGattooView::~COrigGattooView()
{
}

BEGIN_MESSAGE_MAP(COrigGattooView, CView)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TOOLS_ERASER, ID_TOOLS_ZOOM_OUT, &COrigGattooView::OnUpdateTools)
	ON_WM_ERASEBKGND()
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &COrigGattooView::OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_RAW, &COrigGattooView::OnUpdateFileSaveRaw)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// COrigGattooView drawing

void COrigGattooView::OnDraw(CDC* pDC)
{
	CGattooDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	CRect rc;
	GetClientRect(&rc);

	CMemDC memdc(*pDC, this);

	memdc.GetDC().FillSolidRect(&rc, GetSysColor(COLOR_APPWORKSPACE));

	CSize imSize = GetDocument()->getImgSize();
	if (imSize.cx == 0 || imSize.cy == 0)
		return;

	CBitmap *bmp = memdc.GetDC().GetCurrentBitmap();

	if (bmp)
	{
		CDC dcCache;
		CBitmap bmpCache;

		int iWidth  = std::min<int>(rc.Width(), imSize.cx);
		int iHeight = std::min<int>(rc.Height(), imSize.cy);

		bmpCache.CreateCompatibleBitmap(pDC, iWidth, iHeight);
		dcCache.CreateCompatibleDC(pDC);
		dcCache.SelectObject(bmpCache);

		CPoint ptStart;
		
		ptStart.x = (rc.Width() > imSize.cx) ? (rc.Width() - imSize.cx) / 2 : 0;
		ptStart.y = (rc.Height() > imSize.cy) ? (rc.Height() - imSize.cy) / 2 : 0;

		if (imSize.cx - m_ptViewPoint.x < rc.Width())
			m_ptViewPoint.x = std::max<int>(imSize.cx - rc.Width(), 0);

		if (imSize.cy - m_ptViewPoint.y < rc.Height())
			m_ptViewPoint.y = std::max<int>(imSize.cy - rc.Height(), 0);

		pDoc->PerformDrawing(bmpCache, m_ptViewPoint);

		memdc.GetDC().BitBlt(ptStart.x, ptStart.y, iWidth, iHeight, &dcCache, 0, 0, SRCCOPY);
	}
}


// COrigGattooView diagnostics

#ifdef _DEBUG
void COrigGattooView::AssertValid() const
{
	CView::AssertValid();
}

void COrigGattooView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGattooDoc* COrigGattooView::GetDocument() const // non-debug version is inline
{
	if (!m_pDocument) return nullptr;

	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGattooDoc)));
	return (CGattooDoc*)m_pDocument;
}
#endif //_DEBUG

// COrigGattooView message handlers
void COrigGattooView::OnUpdateTools(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void COrigGattooView::OnUpdateFileSaveRaw(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void COrigGattooView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CGattooDoc* pDoc = GetDocument();
	pDoc->SwitchToOriginal(TRUE);

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	Invalidate();
}

BOOL COrigGattooView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return 0;

	return CView::OnEraseBkgnd(pDC);
}

void COrigGattooView::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void COrigGattooView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	UpdateScrolls();
	Invalidate();
}


void COrigGattooView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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


void COrigGattooView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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

void COrigGattooView::OnSize(UINT nType, int cx, int cy)
{
	UpdateScrolls();
	CView::OnSize(nType, cx, cy);
}

void COrigGattooView::UpdateScrolls()
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
