
// GattooView.h : interface of the CGattooView class
//

#pragma once

#include "GattooDoc.h"

class CPrintGattooView : public CView
{
protected: // create from serialization only
	CPrintGattooView();
	DECLARE_DYNCREATE(CPrintGattooView)

// Attributes
public:
	CGattooDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CPrintGattooView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	enum ETool
	{
		enNone,
		enCrop,
		enErase,
		enZoomIn,
		enZoomOut
	};

	ETool m_enCurrentTool;

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnToolsHalftone();
	afx_msg void OnToolsSaveToSD();	
	afx_msg void OnToolsCrop();
	afx_msg void OnUpdateToolsCrop(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in GattooView.cpp
inline CGattooDoc* CPrintGattooView::GetDocument() const
   { return reinterpret_cast<CGattooDoc*>(m_pDocument); }
#endif

