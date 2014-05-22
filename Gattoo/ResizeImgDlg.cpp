// ResizeImgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Gattoo.h"
#include "ResizeImgDlg.h"
#include "afxdialogex.h"
#include "CommonHelpers.h"

// CResizeImgDlg dialog

IMPLEMENT_DYNAMIC(CResizeImgDlg, CDialogEx)

CResizeImgDlg::CResizeImgDlg(CSize szPexels, double dHZScale, double dVTScale, CWnd* pParent /*=NULL*/)
	: CDialogEx(CResizeImgDlg::IDD, pParent)
	, m_dHZScale(dHZScale)
	, m_dVTScale(dVTScale)
{
	PrepareDlg(szPexels);
}

CResizeImgDlg::~CResizeImgDlg()
{
}

void CResizeImgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_SCALE, m_bKeepScale);
	DDX_Text(pDX, IDC_EDIT_HEIGHT_MM, m_dHeightMM);
	DDV_MinMaxDouble(pDX, m_dHeightMM, 0, 1500);
	DDX_Text(pDX, IDC_EDIT_HEIGHT_PX, m_dwHeightPX);
	DDV_MinMaxUInt(pDX, m_dwHeightPX, 0, 3750);
	DDX_Text(pDX, IDC_EDIT_WIDTH_MM, m_dWidthMM);
	DDV_MinMaxDouble(pDX, m_dWidthMM, 0, 1500);
	DDX_Text(pDX, IDC_EDIT_WIDTH_PX, m_dwWidthPX);
	DDV_MinMaxUInt(pDX, m_dwWidthPX, 0, 3750);
}

BEGIN_MESSAGE_MAP(CResizeImgDlg, CDialogEx)
END_MESSAGE_MAP()

// CResizeImgDlg message handlers

void CResizeImgDlg::PrepareDlg(CSize szPexels)
{
	m_dwWidthPX = szPexels.cx;
	m_dwHeightPX = szPexels.cy;

	m_dWidthMM = m_dHZScale * m_dwWidthPX;
	m_dHeightMM = m_dVTScale * m_dwHeightPX;

	m_dWidthToHeightScale = (double)m_dwWidthPX / m_dwHeightPX;

	m_bKeepScale = true;
}

BOOL CResizeImgDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	static bool bOk = true;

	if (HIWORD(wParam) == EN_UPDATE && bOk)
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

			double dCurr = _tstof(strData);

			switch (LOWORD(wParam))
			{
			case IDC_EDIT_HEIGHT_MM:
				m_dHeightMM = dCurr;
				m_dwHeightPX = CCommonHelpers::Round(dCurr/m_dVTScale);
				if (bScale)
				{
					m_dWidthMM = dCurr * m_dWidthToHeightScale;
					m_dwWidthPX = CCommonHelpers::Round(m_dWidthMM/m_dHZScale);
				}
				break;
			case IDC_EDIT_HEIGHT_PX:
				m_dwHeightPX = CCommonHelpers::Round(dCurr);
				m_dHeightMM = dCurr*m_dVTScale;
				if (bScale)
				{
					m_dWidthMM = m_dHeightMM * m_dWidthToHeightScale;
					m_dwWidthPX = CCommonHelpers::Round(m_dWidthMM/m_dHZScale);
				}
				break;
			case IDC_EDIT_WIDTH_MM:
				m_dWidthMM = dCurr;
				m_dwWidthPX = CCommonHelpers::Round(dCurr/m_dHZScale);
				if (bScale)
				{
					m_dHeightMM = dCurr / m_dWidthToHeightScale;
					m_dwHeightPX = CCommonHelpers::Round(m_dHeightMM/m_dVTScale);
				}
				break;
			case IDC_EDIT_WIDTH_PX:
				m_dwWidthPX = CCommonHelpers::Round(dCurr);
				m_dWidthMM = dCurr*m_dHZScale;
				if (bScale)
				{
					m_dHeightMM = m_dWidthMM / m_dWidthToHeightScale;
					m_dwHeightPX = CCommonHelpers::Round(m_dHeightMM/m_dVTScale);
				}
				break;
			}

			bOk = false;
			LPCTSTR lpszFormatFraction = _T("%.2f");
			LPCTSTR lpszFormat = _T("%.0f");
			LPCTSTR lpszFormatCurr = nullptr;
			double intPart;

			lpszFormatCurr = modf(m_dHeightMM, &intPart) ? lpszFormatFraction : lpszFormat;
			strData.Format(lpszFormatCurr, m_dHeightMM);
			SetDlgItemText(IDC_EDIT_HEIGHT_MM, strData);

			lpszFormatCurr = modf(m_dWidthMM, &intPart) ? lpszFormatFraction : lpszFormat;
			strData.Format(lpszFormatCurr, m_dWidthMM);
			SetDlgItemText(IDC_EDIT_WIDTH_MM, strData);

			SetDlgItemInt(IDC_EDIT_HEIGHT_PX, m_dwHeightPX);
			SetDlgItemInt(IDC_EDIT_WIDTH_PX, m_dwWidthPX);

			bOk = true;

			pEdit->SetSel(iCurPos, iCurPos);

			return 0;
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