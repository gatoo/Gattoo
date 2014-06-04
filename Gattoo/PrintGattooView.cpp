
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
#include "GlobalSettings.h"
#include "ResizeImgDlg.h"

#include "BaseImgView.h"

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
	ON_UPDATE_COMMAND_UI(ID_TOOLS_RESIZE, &CPrintGattooView::OnUpdateToolsResize)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM_IN, &CPrintGattooView::OnUpdateToolsZoomIn)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM_OUT, &CPrintGattooView::OnUpdateToolsZoomOut)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_INVERT, &CPrintGattooView::OnUpdateToolsInverse)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_RAW, &CPrintGattooView::OnUpdateFileSaveRaw)

	ON_COMMAND(ID_TOOLS_CROP, &CPrintGattooView::OnToolsCrop)
	ON_COMMAND(ID_TOOLS_RESIZE, &CPrintGattooView::OnToolsResize)
	ON_COMMAND(ID_TOOLS_INVERT, &CPrintGattooView::OnToolInverse)
	ON_COMMAND(ID_TOOLS_ERASER, &CPrintGattooView::OnToolsEraser)

	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()

	ON_COMMAND(ID_TOOLS_ZOOM_IN, &CPrintGattooView::OnToolsZoomIn)
	ON_COMMAND(ID_TOOLS_ZOOM_OUT, &CPrintGattooView::OnToolsZoomOut)
END_MESSAGE_MAP()

// CGattooView construction/destruction

CPrintGattooView::CPrintGattooView()
	: m_enCurrentTool(enNone)
	, m_bToolStarted(false)
	, m_fZoomFactor(1)
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

	return CBaseImgView::PreCreateWindow(cs);
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
	
	CSize imSize = GetDocument()->getImgSize();
	if (imSize.cx == 0 || imSize.cy == 0)
		return;

	CBitmap *bmp = pDC->GetCurrentBitmap();

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

// 		if (imSize.cx - m_ptViewPoint.x < rc.Width())
// 			m_ptViewPoint.x = std::max<int>(imSize.cx - rc.Width(), 0);
// 
// 		if (imSize.cy - m_ptViewPoint.y < rc.Height())
// 			m_ptViewPoint.y = std::max<int>(imSize.cy - rc.Height(), 0);

		pDoc->PerformDrawing(bmpCache, CPoint(0,0));

		pDC->StretchBlt(ptStart.x, ptStart.y, iWidth*m_fZoomFactor, iHeight*m_fZoomFactor, &dcCache, 0, 0, iWidth, iHeight, SRCCOPY);
	}
	//pDoc->PerformDrawing(pDC, GetDrawOrigin());

// 	CPoint pt = GetDrawOrigin();
// 	pDC->StretchBlt(0, 0, 382, 400, pDC, pt.x, pt.y, 191, 200, SRCCOPY);

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
	CBaseImgView::AssertValid();
}

void CPrintGattooView::Dump(CDumpContext& dc) const
{
	CBaseImgView::Dump(dc);
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

	return CBaseImgView::OnEraseBkgnd(pDC);
}

void CPrintGattooView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CGattooDoc* pDoc = GetDocument();
	pDoc->SwitchToOriginal(FALSE);

	CBaseImgView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	Invalidate();
}

void CPrintGattooView::OnUpdateFileSaveRaw(CCmdUI *pCmdUI)
{
	CGattooDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->GetDocumentState() != CGattooImg::enUnknown);
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

void CPrintGattooView::OnUpdateToolsResize(CCmdUI *pCmdUI)
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
	m_enCurrentTool = enInverse;
	
	GetDocument()->doInverse();
	Invalidate();
}

void CPrintGattooView::OnToolsEraser()
{
	m_enCurrentTool = (m_enCurrentTool != enErase) ? enErase : enNone;
}

void CPrintGattooView::OnToolsResize()
{
	m_enCurrentTool = enResize;

	CResizeImgDlg dlg(GetDocument()->getImgSize(), CStaticSettings::HZ_SIZE_SCALE, CStaticSettings::VT_SIZE_SCALE);

	LPCTSTR lpszMsg = _T("All changes will be lost after resizing. Image will be restored to original view.");
	
	if (IDOK == dlg.DoModal() && IDYES == MessageBox(lpszMsg, nullptr, MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2))
	{
		GetDocument()->resizeImage(dlg.GetNewSize());
		Invalidate(FALSE);
	}
}

void CPrintGattooView::OnToolsZoomIn()
{
	m_enCurrentTool = enZoomIn;
	m_fZoomFactor *= CStaticSettings::IMG_ZOOM_FACTOR_MULTIPLIER;
	Invalidate(FALSE);
}

void CPrintGattooView::OnToolsZoomOut()
{
	m_enCurrentTool = enZoomOut;
	m_fZoomFactor /= CStaticSettings::IMG_ZOOM_FACTOR_MULTIPLIER;
	Invalidate(FALSE);
}

BOOL CPrintGattooView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	m_bInClient = true;

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

	return CBaseImgView::OnSetCursor(pWnd, nHitTest, message);
}

void CPrintGattooView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_enCurrentTool == enErase && m_bInClient)
	{
		if (nFlags & MK_LBUTTON)
			DoErase(point);

		Invalidate(FALSE);
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

	CBaseImgView::OnMouseMove(nFlags, point);
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

	CBaseImgView::OnMouseLeave();
}

void CPrintGattooView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bInClient)
	{
		if(m_enCurrentTool == enErase)
		{
			DoErase(point);
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

	CBaseImgView::OnLButtonDown(nFlags, point);
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
	CBaseImgView::OnLButtonUp(nFlags, point);
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

			rcCropFrame -= GetDrawOrigin();
			rcCropFrame.IntersectRect(imgRect, rcCropFrame);

			pDoc->CropImage(rcCropFrame);
			m_ptStart = m_ptEnd = 0;
			Invalidate(FALSE);
		}
	}

	CBaseImgView::OnLButtonDblClk(nFlags, point);
}

void CPrintGattooView::DoErase(CPoint const &pt)
{
	CGattooDoc* pDoc = GetDocument();
	CRect rcErase(m_rcEraser);

	rcErase.MoveToXY(pt);
	rcErase -= GetDrawOrigin();

	CSize size = pDoc->getImgSize();
	CRect imgRect(0, 0, size.cx, size.cy);
	rcErase.IntersectRect(imgRect, rcErase);

	pDoc->EraseRect(rcErase);
}

CPoint CPrintGattooView::GetDrawOrigin()
{
	CRect rc;
	CSize imSize;

	imSize = GetDocument()->getImgSize();
	GetClientRect(&rc);

	int const iXDest = std::max<int>(0, (rc.Width() - imSize.cx)/2);
	int const iYDest = std::max<int>(0, (rc.Height() - imSize.cy)/2);

	return CPoint(iXDest, iYDest);
}

void CPrintGattooView::OnDocumentLoad()
{
	m_fZoomFactor = 1;
}
