// OrigGattooView.cpp : implementation file
//

#include "stdafx.h"
#include "Gattoo.h"
#include "OrigGattooView.h"
#include "GattooDoc.h"

// COrigGattooView

IMPLEMENT_DYNCREATE(COrigGattooView, CView)

COrigGattooView::COrigGattooView()
{
}

COrigGattooView::~COrigGattooView()
{
}

BEGIN_MESSAGE_MAP(COrigGattooView, CBaseImgView)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TOOLS_ERASER, ID_TOOLS_ZOOM_OUT, &COrigGattooView::OnUpdateTools)
	ON_WM_ERASEBKGND()
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &COrigGattooView::OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_RAW, &COrigGattooView::OnUpdateFileSaveRaw)
END_MESSAGE_MAP()


// COrigGattooView drawing
void COrigGattooView::OnDraw(CDC* pDC)
{
	CGattooDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	CRect rc;
	GetClientRect(&rc);

	CMemDC memdc(*pDC, this);

	memdc.GetDC().FillSolidRect(&rc, GetSysColor(COLOR_APPWORKSPACE));

	CSize imSize = GetDocument()->getImgSize();
	if (imSize.cx == 0 || imSize.cy == 0)
		return;

	CBitmap *bmp = memdc.GetDC().GetCurrentBitmap();

	if (bmp)
	{
		CDC dcCache;
		CBitmap bmpCache;

		int iWidth  = std::min<int>(rc.Width(), imSize.cx);
		int iHeight = std::min<int>(rc.Height(), imSize.cy);

		bmpCache.CreateCompatibleBitmap(pDC, iWidth, iHeight);
		dcCache.CreateCompatibleDC(pDC);
		HGDIOBJ pOldBmp = dcCache.SelectObject(bmpCache);

		CPoint ptStart;
		
		ptStart.x = (rc.Width() > imSize.cx) ? (rc.Width() - imSize.cx) / 2 : 0;
		ptStart.y = (rc.Height() > imSize.cy) ? (rc.Height() - imSize.cy) / 2 : 0;

		if (imSize.cx - m_ptViewPoint.x < rc.Width())
			m_ptViewPoint.x = std::max<int>(imSize.cx - rc.Width(), 0);

		if (imSize.cy - m_ptViewPoint.y < rc.Height())
			m_ptViewPoint.y = std::max<int>(imSize.cy - rc.Height(), 0);

		pDoc->PerformDrawing(bmpCache, m_ptViewPoint);

		memdc.GetDC().BitBlt(ptStart.x, ptStart.y, iWidth, iHeight, &dcCache, 0, 0, SRCCOPY);

		dcCache.SelectObject(pOldBmp);
	}
}

// COrigGattooView message handlers
void COrigGattooView::OnUpdateTools(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void COrigGattooView::OnUpdateFileSaveRaw(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void COrigGattooView::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void COrigGattooView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CGattooDoc* pDoc = GetDocument();
	pDoc->SwitchToOriginal(TRUE);

	CBaseImgView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	Invalidate(FALSE);
}

void COrigGattooView::OnDocumentLoad()
{
	UpdateScrolls();
	Invalidate(FALSE);
}

