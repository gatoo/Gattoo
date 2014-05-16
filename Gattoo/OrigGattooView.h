#pragma once

// COrigGattooView view

#include "GattooDoc.h"

class COrigGattooView : public CView
{

protected:
	DECLARE_DYNCREATE(COrigGattooView)
	COrigGattooView();           // protected constructor used by dynamic creation

public:

	CGattooDoc* GetDocument() const;

	afx_msg void OnUpdateTools(CCmdUI *pCmdUI);
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view

	virtual ~COrigGattooView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

	CPoint GetDrawOrigin();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);

};

#ifndef _DEBUG  // debug version in GattooView.cpp
inline CGattooDoc* COrigGattooView::GetDocument() const
{ return reinterpret_cast<CGattooDoc*>(m_pDocument); }
#endif


