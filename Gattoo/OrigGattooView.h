#pragma once

// COrigGattooView view

#include "GattooDoc.h"

class COrigGattooView : public CView
{

public:

	CGattooDoc* GetDocument() const;

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSaveRaw(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTools(CCmdUI *pCmdUI);

	virtual void OnDraw(CDC* pDC);

	virtual ~COrigGattooView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(COrigGattooView)

	COrigGattooView();


	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
};

#ifndef _DEBUG  // debug version in GattooView.cpp
inline CGattooDoc* COrigGattooView::GetDocument() const
{ return reinterpret_cast<CGattooDoc*>(m_pDocument); }
#endif


