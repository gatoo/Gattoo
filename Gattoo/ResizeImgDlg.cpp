// ResizeImgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Gattoo.h"
#include "ResizeImgDlg.h"
#include "afxdialogex.h"


// CResizeImgDlg dialog

IMPLEMENT_DYNAMIC(CResizeImgDlg, CDialogEx)

CResizeImgDlg::CResizeImgDlg(CSize szPexels, double dScale /*= 2.5*/, CWnd* pParent /*=NULL*/)
	: CDialogEx(CResizeImgDlg::IDD, pParent)
{
	PrepareDlg(szPexels, dScale);
}

CResizeImgDlg::~CResizeImgDlg()
{
}

void CResizeImgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_SCALE, m_bKeepScale);
	DDX_Text(pDX, IDC_EDIT_HEIGHT_MM, m_dwHeightMM);
	DDV_MinMaxUInt(pDX, m_dwHeightMM, 0, 1500);
	DDX_Text(pDX, IDC_EDIT_HEIGHT_PX, m_dwHeightPX);
	DDV_MinMaxUInt(pDX, m_dwHeightPX, 0, 3750);
	DDX_Text(pDX, IDC_EDIT_WIDTH_MM, m_dwWidthMM);
	DDV_MinMaxUInt(pDX, m_dwWidthMM, 0, 1500);
	DDX_Text(pDX, IDC_EDIT_WIDTH_PX, m_dwWidthPX);
	DDV_MinMaxUInt(pDX, m_dwWidthPX, 0, 3750);
}

BEGIN_MESSAGE_MAP(CResizeImgDlg, CDialogEx)
END_MESSAGE_MAP()

// CResizeImgDlg message handlers

void CResizeImgDlg::PrepareDlg(CSize szPexels, double dScale)
{
	m_dwWidthPX = szPexels.cx;
	m_dwHeightPX = szPexels.cy;

	m_dwWidthMM = (DWORD)(m_dwWidthPX * dScale);
	m_dwHeightMM = (DWORD)(m_dwHeightPX * dScale);

	m_dScale = m_dwWidthPX / m_dwHeightPX;

	m_bKeepScale = true;
}

BOOL CResizeImgDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == EN_UPDATE)
	{
		if (LOWORD(wParam) >= IDC_EDIT_WIDTH_PX && LOWORD(wParam) <= IDC_EDIT_HEIGHT_MM)
		{
			int iCurPos = 0;
			CString strData;
			CEdit* pEdit = (CEdit*)FromHandle((HWND)lParam);
			
			pEdit->GetWindowText(strData);
			iCurPos =LOWORD(pEdit->GetSel());

			// Check for validness
			if (!CheckForNumber(strData, iCurPos, IsFloatAccepted(LOWORD(wParam))))
			{
				// Update with removed char
				-- iCurPos;
				pEdit->SetWindowText(strData);
				pEdit->SetSel(iCurPos, iCurPos);
				return 0;
			}

			BOOL bScale = (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_SCALE))->GetCheck());
// 
// 			
// 
// 			double dCurr = _tstof(strData);
// 
// 			if (bScale)
// 			{
// 				switch (LOWORD(wParam))
// 				{
// 				case IDC_EDIT_HEIGHT_MM:
// 					break;
// 				case IDC_EDIT_HEIGHT_PX:
// 					break;
// 				case IDC_EDIT_WIDTH_MM:
// 					break;
// 				case IDC_EDIT_WIDTH_PX:
// 					break;
// 				}
// 			}
// 			else
// 			{
// 				strData.Format(_T("%.2f"), dCurr);
// 				pWnd->SetWindowText(strData);
// 			}
// 
// 			return 0;
		}
	}

	return CDialogEx::OnCommand(wParam, lParam);
}

BOOL CResizeImgDlg::CheckForNumber(CString& strData, int iPos, BOOL bFloat) const
{
	if (0 == iPos) return true;

	iPos --;

	char const chCurSymbol = strData.GetAt(iPos);

	// Check if is digit
	BOOL bValid = isdigit(chCurSymbol);
	
	// If it invalid, may be this is dot
	if (!bValid && bFloat && chCurSymbol == _T('.'))
	{
		int iDotCount = 0;
		int iFindPos = 0;
		
		do
		{
			iFindPos = strData.Find(_T('.'), iFindPos);
			if (iFindPos != -1)
			{
				iDotCount ++;
				iFindPos++;
			}

		}while(iDotCount < 3 && iFindPos != -1);

		bValid = (iDotCount == 1);
	}

	if (!bValid)
		strData.Delete(iPos);

	return bValid;
}

BOOL CResizeImgDlg::IsFloatAccepted(int iID)
{
	BOOL bAllowed = FALSE;

	switch (iID)
	{
	case IDC_EDIT_HEIGHT_MM:
	case IDC_EDIT_WIDTH_MM:
		bAllowed = TRUE;
		break;
	}

	return bAllowed;
}
