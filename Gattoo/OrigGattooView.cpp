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
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CROP, &COrigGattooView::OnUpdateToolsCrop)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ERASER, &COrigGattooView::OnUpdateToolsEraser)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM_IN, &COrigGattooView::OnUpdateToolsZoomIn)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM_OUT, &COrigGattooView::OnUpdateToolsZoomOut)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_HALFTONE, &COrigGattooView::OnUpdateToolsHalftone)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SAVETOSD, &COrigGattooView::OnUpdateToolsSavetosd)
END_MESSAGE_MAP()


// COrigGattooView drawing

void COrigGattooView::OnDraw(CDC* pDC)
{
	CGattooDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	CRect rc;
	GetClientRect(&rc);

	pDoc->PerformDrawing(pDC, rc);
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
void COrigGattooView::OnUpdateToolsCrop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void COrigGattooView::OnUpdateToolsEraser(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void COrigGattooView::OnUpdateToolsZoomIn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void COrigGattooView::OnUpdateToolsZoomOut(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void COrigGattooView::OnUpdateToolsHalftone(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void COrigGattooView::OnUpdateToolsSavetosd(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void COrigGattooView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CGattooDoc* pDoc = GetDocument();
	pDoc->SwitchToOriginal(TRUE);

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
