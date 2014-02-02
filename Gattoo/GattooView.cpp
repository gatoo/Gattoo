
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

#include <opencv2/opencv.hpp>

// CGattooView

IMPLEMENT_DYNCREATE(CGattooView, CView)

BEGIN_MESSAGE_MAP(CGattooView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
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
	if (!pDoc)
		return;

// 	CDC memDC;
// 
// 	memDC.CreateCompatibleDC(pDC);

	cv::Mat img;

	img = cv::imread("C:\\temp\\test.bmp");

	BITMAPINFO bmi;

	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       = img.cols;
	bmi.bmiHeader.biHeight      = -img.rows; // top-down image 
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 24;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage   = 0;

	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(&rc, RGB(0x10, 0x10, 0x10));

	SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, img.cols,
		img.rows, 0, 0, 0, img.rows, img.data, &bmi,
		DIB_RGB_COLORS);
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
