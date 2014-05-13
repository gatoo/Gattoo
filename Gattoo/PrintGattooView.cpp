
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

	ON_UPDATE_COMMAND_UI(ID_TOOLS_CROP, &CPrintGattooView::OnUpdateToolsCrop)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ERASER, &CPrintGattooView::OnUpdateToolsEraser)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM_IN, &CPrintGattooView::OnUpdateToolsZoomIn)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM_OUT, &CPrintGattooView::OnUpdateToolsZoomOut)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_INVERT, &CPrintGattooView::OnUpdateToolsInverse)

	ON_COMMAND(ID_TOOLS_CROP, &CPrintGattooView::OnToolsCrop)
	ON_COMMAND(ID_TOOLS_INVERT, &CPrintGattooView::OnToolInverse)

	ON_COMMAND(ID_TOOLS_ERASER, &CPrintGattooView::OnToolsEraser)
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_NCMOUSELEAVE()
	ON_WM_MOUSELEAVE()
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

	CMemDC memDC(*pDC, this);
	pDC = &memDC.GetDC();

	pDC->FillSolidRect(&rc, GetSysColor(COLOR_APPWORKSPACE));
	pDoc->PerformDrawing(pDC);

	if (m_enCurrentTool == enErase && m_bInClient)
	{
		CRect rec;
		POINT pt;
		GetCursorPos(&pt);

		ScreenToClient(&pt);

		rec.SetRect(pt, CPoint(pt.x+10, pt.y + 10));

		pDC->FillSolidRect(rec,  RGB(0xFF, 0, 0));
	}
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

void CPrintGattooView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CGattooDoc* pDoc = GetDocument();
	pDoc->SwitchToOriginal(FALSE);

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	Invalidate();
}

void CPrintGattooView::OnUpdateToolsCrop(CCmdUI *pCmdUI)
{
	CGattooDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->GetDocumentState() != CGattooImg::enUnknown);
}

void CPrintGattooView::OnUpdateToolsEraser(CCmdUI *pCmdUI)
{
	CGattooDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->GetDocumentState() != CGattooImg::enUnknown);
}

void CPrintGattooView::OnUpdateToolsZoomIn(CCmdUI *pCmdUI)
{
	CGattooDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->GetDocumentState() != CGattooImg::enUnknown);
}

void CPrintGattooView::OnUpdateToolsZoomOut(CCmdUI *pCmdUI)
{
	CGattooDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->GetDocumentState() != CGattooImg::enUnknown);
}

void CPrintGattooView::OnUpdateToolsInverse(CCmdUI *pCmdUI)
{
	CGattooDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->GetDocumentState() != CGattooImg::enUnknown);
}

void CPrintGattooView::OnToolsCrop()
{
	m_enCurrentTool = enCrop;
}

void CPrintGattooView::OnToolInverse()
{
	GetDocument()->doInverse();
	Invalidate();
}

void CPrintGattooView::OnToolsEraser()
{
	m_enCurrentTool = enErase;
}

BOOL CPrintGattooView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	m_bInClient = true;

	if (m_enCurrentTool == enErase)
	{
		SetCursor(nullptr);

		TRACKMOUSEEVENT ev;

		ev.cbSize = sizeof(TRACKMOUSEEVENT);
		ev.dwFlags = TME_LEAVE;
		ev.hwndTrack = m_hWnd;

		TrackMouseEvent(&ev);
		return TRUE;
	}

	return CView::OnSetCursor(pWnd, nHitTest, message);
}


void CPrintGattooView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_enCurrentTool == enErase && m_bInClient) Invalidate(FALSE);

	CView::OnMouseMove(nFlags, point);
}


void CPrintGattooView::OnNcMouseLeave()
{
	// This feature requires Windows 2000 or greater.
	// The symbols _WIN32_WINNT and WINVER must be >= 0x0500.
	// TODO: Add your message handler code here and/or call default
	TRACE("OnNcMouseLeave\n");
	CView::OnNcMouseLeave();
}


void CPrintGattooView::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default

	TRACE("OnMouseLeave\n");

	m_bInClient = false;
	Invalidate(FALSE);

	TRACKMOUSEEVENT ev;

	ev.cbSize = sizeof(TRACKMOUSEEVENT);
	ev.dwFlags = TME_CANCEL;
	ev.hwndTrack = m_hWnd;

	TrackMouseEvent(&ev);

	CView::OnMouseLeave();
}
