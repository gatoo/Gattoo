
// GattooView.cpp : implementation of the CGattooView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Gattoo.h"
#endif

#include "GattooDoc.h"
#include "GattooView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGattooView

IMPLEMENT_DYNCREATE(CGattooView, CView)

BEGIN_MESSAGE_MAP(CGattooView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CGattooView construction/destruction

CGattooView::CGattooView()
{
	// TODO: add construction code here

}

CGattooView::~CGattooView()
{
}

BOOL CGattooView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGattooView drawing

void CGattooView::OnDraw(CDC* pDC)
{
	CGattooDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	CRect rc;
	GetClientRect(&rc);

//  	CDC memDC;
// 	CBitmap bmp;
//  
//  	memDC.CreateCompatibleDC(pDC);
// 	bmp.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
// 	memDC.SelectObject(bmp);
// 	
// 	CSize size = pDoc->getImgSize();
// 	BITMAPINFO bmi;
// 
// 	memset(&bmi, 0, sizeof(bmi));
// 	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
// 	bmi.bmiHeader.biWidth       = size.cx;
// 	bmi.bmiHeader.biHeight      = -size.cy; // top-down image 
// 	bmi.bmiHeader.biPlanes      = 1;
// 	bmi.bmiHeader.biBitCount    = 24;
// 	bmi.bmiHeader.biCompression = BI_RGB;
// 	bmi.bmiHeader.biSizeImage   = 0;
// 
// 	memDC.FillSolidRect(&rc, RGB(0xFF, 0xFF, 0xFF));
// 
// 	int iXStart = 0, iYStart = 0;
// 
// 	iXStart = (rc.Width() - size.cx) / 2;
// 	iYStart = (rc.Height() - size.cy) / 2;
// 
// 	//bmp.SetBitmapBits(size.cx* size.cy *3, pDoc->getImgData());
// 
// 	SetDIBitsToDevice(memDC.GetSafeHdc(), iXStart, iYStart, size.cx,
// 		size.cy, 0, 0, 0, size.cy, pDoc->getImgData(), &bmi,
// 		DIB_RGB_COLORS);
// 
// 	pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);

	pDoc->PerformDrawing(pDC, rc);
}

void CGattooView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGattooView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGattooView diagnostics

#ifdef _DEBUG
void CGattooView::AssertValid() const
{
	CView::AssertValid();
}

void CGattooView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGattooDoc* CGattooView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGattooDoc)));
	return (CGattooDoc*)m_pDocument;
}
#endif //_DEBUG


// CGattooView message handlers


BOOL CGattooView::OnEraseBkgnd(CDC* pDC)
{
	return 0;// TODO: Add your message handler code here and/or call default

	return CView::OnEraseBkgnd(pDC);
}
