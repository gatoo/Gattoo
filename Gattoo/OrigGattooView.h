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

	CPoint m_ptViewPoint;

	int m_iMaxXScroll;
	int m_iMaxYScroll;

	void UpdateScrolls();

	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
};

#ifndef _DEBUG  // debug version in GattooView.cpp
inline CGattooDoc* COrigGattooView::GetDocument() const
{ return reinterpret_cast<CGattooDoc*>(m_pDocument); }
#endif


