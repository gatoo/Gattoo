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

	afx_msg void OnUpdateToolsCrop(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolsEraser(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolsZoomIn(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolsZoomOut(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolsHalftone(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolsSavetosd(CCmdUI *pCmdUI);

	virtual void OnDraw(CDC* pDC);      // overridden to draw this view

	virtual ~COrigGattooView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
};

#ifndef _DEBUG  // debug version in GattooView.cpp
inline CGattooDoc* COrigGattooView::GetDocument() const
{ return reinterpret_cast<CGattooDoc*>(m_pDocument); }
#endif


