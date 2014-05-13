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
	ON_UPDATE_COMMAND_UI_RANGE(ID_TOOLS_ERASER, ID_TOOLS_INVERT, &COrigGattooView::OnUpdateTools)
	ON_WM_ERASEBKGND()
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
	pDC = &memdc.GetDC();

	pDC->FillSolidRect(&rc, GetSysColor(COLOR_APPWORKSPACE));
	pDoc->PerformDrawing(pDC);
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
