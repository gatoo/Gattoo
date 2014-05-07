// OrigGattooView.cpp : implementation file
//

#include "stdafx.h"
#include "Gattoo.h"
#include "OrigGattooView.h"


// COrigGattooView

IMPLEMENT_DYNCREATE(COrigGattooView, CView)

COrigGattooView::COrigGattooView()
{

}

COrigGattooView::~COrigGattooView()
{
}

BEGIN_MESSAGE_MAP(COrigGattooView, CView)
END_MESSAGE_MAP()


// COrigGattooView drawing

void COrigGattooView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// COrigGattooView diagnostics

#ifdef _DEBUG
void COrigGattooView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void COrigGattooView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// COrigGattooView message handlers
