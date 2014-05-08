
// GattooView.cpp : implementation of the CGattooView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Gattoo.h"
#endif

#include "GattooDoc.h"
#include "PrintGattooView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGattooView

IMPLEMENT_DYNCREATE(CPrintGattooView, CView)

BEGIN_MESSAGE_MAP(CPrintGattooView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_TOOLS_HALFTONE, &CPrintGattooView::OnToolsHalftone)
	ON_COMMAND(ID_TOOLS_SAVETOSD, &CPrintGattooView::OnToolsSaveToSD)
	ON_COMMAND(ID_TOOLS_CROP, &CPrintGattooView::OnToolsCrop)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CROP, &CPrintGattooView::OnUpdateToolsCrop)
END_MESSAGE_MAP()

// CGattooView construction/destruction

CPrintGattooView::CPrintGattooView()
	: m_enCurrentTool(enNone)
{
	// TODO: add construction code here

}

CPrintGattooView::~CPrintGattooView()
{
}

BOOL CPrintGattooView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGattooView drawing

void CPrintGattooView::OnDraw(CDC* pDC)
{
	CGattooDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	CRect rc;
	GetClientRect(&rc);

	pDoc->PerformDrawing(pDC, rc);
}

void CPrintGattooView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPrintGattooView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGattooView diagnostics

#ifdef _DEBUG
void CPrintGattooView::AssertValid() const
{
	CView::AssertValid();
}

void CPrintGattooView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGattooDoc* CPrintGattooView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGattooDoc)));
	return (CGattooDoc*)m_pDocument;
}
#endif //_DEBUG


// CGattooView message handlers


BOOL CPrintGattooView::OnEraseBkgnd(CDC* pDC)
{
	return 0;// TODO: Add your message handler code here and/or call default

	return CView::OnEraseBkgnd(pDC);
}


void CPrintGattooView::OnToolsHalftone()
{
	CGattooDoc* pDoc = GetDocument();
	pDoc->OnToolsHalftone();
	Invalidate();
}

void CPrintGattooView::OnToolsSaveToSD()
{
	CGattooDoc* pDoc = GetDocument();
	pDoc->OnToolsSaveToSD();
	Invalidate();
}


void CPrintGattooView::OnToolsCrop()
{
	m_enCurrentTool = enCrop;
}


void CPrintGattooView::OnUpdateToolsCrop(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(enCrop == m_enCurrentTool);
}


void CPrintGattooView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CGattooDoc* pDoc = GetDocument();
	pDoc->SwitchToOriginal(FALSE);

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
