#pragma once


// CGattooTabView view

class CGattooTabView : public CTabView
{
	DECLARE_DYNCREATE(CGattooTabView)

protected:
	CGattooTabView();           // protected constructor used by dynamic creation
	virtual ~CGattooTabView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
//	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


