// OrigGattooView.cpp : implementation file
//

#include "stdafx.h"
#include "Gattoo.h"
#include "OrigGattooView.h"
#include "GattooDoc.h"


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
	
	TRACE("Draw\n");

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

		pDoc->PerformDrawing(bmpCache, CPoint(0,0));

		memdc.GetDC().StretchBlt(0, 0, iWidth, iHeight, &dcCache, 0, 0, iWidth, iHeight, SRCCOPY);
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
			sinfo.nMax = sizeImg.cx/rcClient.Width() - 1;
			sinfo.nPage = rcClient.Width();
			SetScrollInfo(SB_HORZ, &sinfo);
		}
		else
			SetScrollRange(SB_HORZ, 0, 0);

		if(rcClient.Height() < sizeImg.cy)
		{
			sinfo.nMax = sizeImg.cy - rcClient.Height();
			sinfo.nPage = rcClient.Height();

			SetScrollInfo(SB_VERT, &sinfo);
		}
		else
			SetScrollRange(SB_VERT, 0, 0);
	}

	Invalidate();
}
