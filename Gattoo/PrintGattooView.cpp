
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

int const CPrintGattooView::ERASER_BLOCK_SIZE = 20;


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
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CGattooView construction/destruction

CPrintGattooView::CPrintGattooView()
	: m_enCurrentTool(enNone)
	, m_bToolStarted(false)
{
	// TODO: add construction code here
	m_rcEraser.SetRect(0, 0, ERASER_BLOCK_SIZE, ERASER_BLOCK_SIZE);
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
		DrawEraser(pDC);
	else if (m_enCurrentTool == enCrop && m_ptEnd != m_ptStart)
		DrawCropFrame(pDC);
}

void CPrintGattooView::DrawEraser(CDC* pDC)
{
	POINT pt;
	CPen pen;
	CRect const rcFrame(1, 1, 1, 1);
	
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	m_rcEraser.MoveToXY(pt);

	pen.CreatePen(PS_SOLID, 1, RGB(0xFF, 0xFF, 0xFF));

	pDC->SelectObject(pen);
	pDC->Rectangle(m_rcEraser);
	pDC->FillSolidRect(m_rcEraser - rcFrame, RGB(0, 0, 0));
}

void CPrintGattooView::DrawCropFrame(CDC* pDC)
{
	CPen pen;

	CRect const rcFrame(m_ptStart, m_ptEnd);

	pen.CreatePen(PS_SOLID, 1, RGB(0x00, 0xFF, 0x00));

	pDC->SelectObject(pen);
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(rcFrame);
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
	pCmdUI->SetCheck(m_enCurrentTool == enCrop);
}

void CPrintGattooView::OnUpdateToolsEraser(CCmdUI *pCmdUI)
{
	CGattooDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->GetDocumentState() != CGattooImg::enUnknown);
	pCmdUI->SetCheck(m_enCurrentTool == enErase);
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
	m_enCurrentTool = (m_enCurrentTool != enCrop) ? enCrop : enNone;
}

void CPrintGattooView::OnToolInverse()
{
	GetDocument()->doInverse();
	m_enCurrentTool = enNone;
	Invalidate();
}

void CPrintGattooView::OnToolsEraser()
{
	m_enCurrentTool = (m_enCurrentTool != enErase) ? enErase : enNone;
}

BOOL CPrintGattooView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	m_bInClient = true;

	//TRACE("OnSetCursor\n");

	ASSERT(HTCLIENT & nHitTest);

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
	if(m_enCurrentTool == enErase && m_bInClient)
	{
		if (nFlags & MK_LBUTTON)
		{
			CGattooDoc* pDoc = GetDocument();
			CRect rcErase(m_rcEraser);

			rcErase.MoveToXY(point);

			CSize size = pDoc->getImgSize();
			CRect imgRect(0, 0, size.cx, size.cy);
			rcErase.IntersectRect(imgRect, rcErase);

			pDoc->EraseRect(rcErase);
		}

		Invalidate(FALSE);

		//TRACE("OnMouseMove\n");
	}
	else if (m_enCurrentTool == enCrop)
	{
		if (m_bToolStarted)
		{
			m_ptEnd = point;
		}
		else if(nFlags & MK_LBUTTON)
		{
			m_ptStart -= m_ptMoveStart - point;
			m_ptEnd -= m_ptMoveStart - point;

			m_ptMoveStart = point;
		}

		Invalidate(FALSE);
	}

	CView::OnMouseMove(nFlags, point);
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

void CPrintGattooView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bInClient)
	{
		if(m_enCurrentTool == enErase)
		{
			CGattooDoc* pDoc = GetDocument();
			CRect rcErase(m_rcEraser);

			rcErase.MoveToXY(point);

			CSize size = pDoc->getImgSize();
			CRect imgRect(0, 0, size.cx, size.cy);
			rcErase.IntersectRect(imgRect, rcErase);

			pDoc->EraseRect(rcErase);

			SetCapture();

		} else if (m_enCurrentTool == enCrop)
		{
			CRect rcFrame(m_ptStart, m_ptEnd);
			rcFrame.NormalizeRect();

			if (!rcFrame.PtInRect(point))
			{
				m_bToolStarted = true;
				m_ptStart = m_ptEnd = point;
			}
			else
				m_ptMoveStart = point;
		}
	}

	CView::OnLButtonDown(nFlags, point);
}

void CPrintGattooView::OnLButtonUp(UINT nFlags, CPoint point)
{
	TRACE("OnLButtonUp %d\n", m_bInClient);
	if (m_bInClient)
	{
		if (m_enCurrentTool == enCrop && m_bToolStarted)
		{
			m_bToolStarted = false;
			Invalidate(0);
		}
	}
	else
	{
		m_bToolStarted = false;
	}

	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}


void CPrintGattooView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_bInClient)
	{
		CRect rcCropFrame(m_ptStart, m_ptEnd);
		rcCropFrame.NormalizeRect();

		if (rcCropFrame.PtInRect(point))
		{
			CGattooDoc* pDoc = GetDocument();

			CSize size = pDoc->getImgSize();

			CRect imgRect(0, 0, size.cx-1, size.cy-1);

			rcCropFrame.IntersectRect(imgRect, rcCropFrame);

			pDoc->CropImage(rcCropFrame);
			m_ptStart = m_ptEnd = 0;
			Invalidate(FALSE);
		}
	}

	CView::OnLButtonDblClk(nFlags, point);
}
