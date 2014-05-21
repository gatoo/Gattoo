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
			
			double const dScale = 0.25;

			double dCurr = _tstof(strData);

			switch (LOWORD(wParam))
			{
			case IDC_EDIT_HEIGHT_MM:
				m_dwHeightMM = dCurr;
				m_dwHeightPX = dCurr/dScale;
				if (bScale)
				{
					m_dwWidthMM = dCurr * m_dScale;
					m_dwWidthPX = m_dwWidthMM/dScale;
				}
				break;
			case IDC_EDIT_HEIGHT_PX:
				m_dwHeightPX = dCurr;
				m_dwHeightMM = dCurr*dScale;
				if (bScale)
				{
					m_dwWidthMM = m_dwHeightMM * m_dScale;
					m_dwWidthPX = m_dwWidthMM / dScale;
				}
				break;
			case IDC_EDIT_WIDTH_MM:
				m_dwWidthMM = dCurr;
				m_dwWidthPX = dCurr/dScale;
				if (bScale)
				{
					m_dwHeightMM = dCurr / m_dScale;
					m_dwHeightPX = m_dwHeightMM / dScale;						
				}
				break;
			case IDC_EDIT_WIDTH_PX:
				m_dwWidthPX = dCurr;
				m_dwWidthMM = dCurr*dScale;
				if (bScale)
				{
					m_dwHeightMM = m_dwWidthMM / m_dScale;
					m_dwHeightPX = m_dwHeightMM / dScale;						
				}
				break;
			}

			bOk = false;

			SetDlgItemInt(IDC_EDIT_HEIGHT_MM, m_dwHeightMM);
			SetDlgItemInt(IDC_EDIT_WIDTH_MM, m_dwWidthMM);

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

/*
// ==UserScript==
// @name        Check
// @namespace   swe
// @include     https://visaservices.co.in/Sweden-Ukraine-Appointment/AppSchedulingEmb/AppSchedulingGetInfo.aspx?P=ri7FHohe3VirNKmyLaRu36t9%2fpEItw3gfYXFtDFlxVY%3d
// @version     1
// @grant       none
// ==/UserScript==

function checkData()
{
var dataElem = document.getElementById('ctl00_plhMain_lblMsg');

if (dataElem == null)
alert('Error 1');

var dataStr = dataElem.innerHTML;

var re = new RegExp("\.\./\.\.\./2014", "gi");

var results = re.exec(dataStr);

//alert(dataStr);

if (results)
{
var parts = results[0].split('/');
var day = parseInt(parts[0], 10);

if (day < 30 && parts[1] == 'May')
alert('Urha!!!');
}
else
alert('Error 2' + results);

__doPostBack('ctl00$plhMain$cboVisaCategory','');
}

setTimeout(checkData, 300000);
*/