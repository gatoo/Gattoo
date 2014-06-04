
// GattooView.h : interface of the CGattooView class
//

#pragma once

#include "GattooDoc.h"
#include "BaseImgView.h"

class CPrintGattooView : public CBaseImgView
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
		enResize,
		enInverse,
		enZoomIn,
		enZoomOut
	};

	ETool m_enCurrentTool;

	bool m_bInClient;

	bool m_bToolStarted;

	CPoint m_ptStart;
	CPoint m_ptEnd;
	CPoint m_ptMoveStart;

	CDC m_memDC;

	static int const ERASER_BLOCK_SIZE;

	CRect m_rcEraser;

	float m_fZoomFactor;

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	
	void DrawEraser(CDC* pDC);
	void DoErase(CPoint const &pt);

	CPoint GetDrawOrigin();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);	
	afx_msg void OnToolsCrop();

	afx_msg void OnUpdateToolsCrop(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolsEraser(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolsZoomIn(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolsZoomOut(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolsInverse(CCmdUI *pCmdUI);

	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	afx_msg void OnToolInverse();
	afx_msg void OnToolsEraser();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnUpdateFileSaveRaw(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolsResize(CCmdUI *pCmdUI);
	afx_msg void OnToolsResize();
	afx_msg void OnToolsZoomIn();
	afx_msg void OnToolsZoomOut();

	void DrawCropFrame(CDC* pDC);
	virtual void OnDocumentLoad();

};

#ifndef _DEBUG  // debug version in GattooView.cpp
inline CGattooDoc* CPrintGattooView::GetDocument() const
   { return reinterpret_cast<CGattooDoc*>(m_pDocument); }
#endif

