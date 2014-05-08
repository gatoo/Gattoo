// GattooTabView.cpp : implementation file
//

#include "stdafx.h"
#include "Gattoo.h"
#include "GattooTabView.h"

#include "PrintGattooView.h"
#include "OrigGattooView.h"

// CGattooTabView

IMPLEMENT_DYNCREATE(CGattooTabView, CTabView)

CGattooTabView::CGattooTabView()
{
}

CGattooTabView::~CGattooTabView()
{
}

BEGIN_MESSAGE_MAP(CGattooTabView, CTabView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CGattooTabView diagnostics

#ifdef _DEBUG
void CGattooTabView::AssertValid() const
{
	CTabView::AssertValid();
}

#ifndef _WIN32_WCE
void CGattooTabView::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif
#endif //_DEBUG

int CGattooTabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;

	AddView (RUNTIME_CLASS (COrigGattooView), _T("Original"), -1);
	AddView (RUNTIME_CLASS (CPrintGattooView), _T("Print"), -1);

	return 0;
}
