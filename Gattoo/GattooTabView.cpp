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


// CGattooTabView message handlers


//BOOL CGattooTabView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	return CTabView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
//}


int CGattooTabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;

	AddView (RUNTIME_CLASS (COrigGattooView), _T("Original"), -1);
	AddView (RUNTIME_CLASS (CPrintGattooView), _T("Print"), -1);	

	return 0;
}
