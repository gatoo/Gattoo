
// GattooView.h : interface of the CGattooView class
//

#pragma once


class CGattooView : public CView
{
protected: // create from serialization only
	CGattooView();
	DECLARE_DYNCREATE(CGattooView)

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
	virtual ~CGattooView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GattooView.cpp
inline CGattooDoc* CGattooView::GetDocument() const
   { return reinterpret_cast<CGattooDoc*>(m_pDocument); }
#endif

