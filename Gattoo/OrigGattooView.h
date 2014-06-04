#pragma once

#include "GattooDoc.h"
#include "BaseImgView.h"

class COrigGattooView : public CBaseImgView
{

public:

	virtual ~COrigGattooView();

protected:

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(COrigGattooView)

	COrigGattooView();

	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSaveRaw(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTools(CCmdUI *pCmdUI);

	virtual void OnDraw(CDC* pDC);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

	virtual void OnDocumentLoad();

	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

#ifndef _DEBUG  // debug version in GattooView.cpp
inline CGattooDoc* COrigGattooView::GetDocument() const
{ return reinterpret_cast<CGattooDoc*>(m_pDocument); }
#endif


