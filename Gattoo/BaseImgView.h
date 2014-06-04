#pragma once

#include "GattooDoc.h"

// CBaseImgView view
class CBaseImgView : public CView
{

public:

	//DECLARE_DYNCREATE(CBaseImgView)

	virtual ~CBaseImgView();

	CGattooDoc* GetDocument() const;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	DECLARE_MESSAGE_MAP()

	CBaseImgView();

	void UpdateScrolls();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual void OnUpdate(CBaseImgView* pSender, LPARAM lHint, CObject* pHint);
	

	virtual void OnDocumentLoad();

	virtual void OnDraw(CDC* pDC);

	CPoint m_ptViewPoint;

	int m_iMaxXScroll;
	int m_iMaxYScroll;

};

#ifndef _DEBUG  // debug version in GattooView.cpp
inline CGattooDoc* CBaseImgView::GetDocument() const
{ return reinterpret_cast<CGattooDoc*>(m_pDocument); }
#endif


