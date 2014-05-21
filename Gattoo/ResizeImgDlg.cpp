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

	m_dwWidthMM = m_dwWidthPX * dScale;
	m_dwHeightMM = m_dwHeightPX * dScale;

	m_bKeepScale = true;
}
