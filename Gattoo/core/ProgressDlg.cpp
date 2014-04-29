// D:\Programming\Granit\Gattoo\Gattoo\core\ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProgressDlg.h"
#include "afxdialogex.h"
#include "StorageRoutine/CVolumeAccess.h"


// CProgressDlg dialog

IMPLEMENT_DYNAMIC(CProgressDlg, CDialogEx)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProgressDlg::IDD, pParent)
	, m_iMaxValue(100)
	, m_pProcessor(NULL)
	, m_dwStartSector(70000)
{

}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progressCtrl);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CProgressDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

bool CProgressDlg::doStep()
{
	m_progressCtrl.StepIt();
	return true;
}

bool CProgressDlg::init(int iMax, int iStep)
{
	m_progressCtrl.SetRange(0, iMax);
	m_progressCtrl.SetStep(iStep);
	return true;
}


// CProgressDlg message handlers


void CProgressDlg::OnBnClickedCancel()
{
	if (m_pProcessor) m_pProcessor->stop();
	EndDialog(IDOK);
}

void CProgressDlg::startSave(std::string &strPath)
{
	m_hProcessor = CreateThread(NULL, 0, ThreadProc, this, CREATE_SUSPENDED, NULL);
	m_strPath = strPath;
	if (IDOK == DoModal())
	{
		MessageBox(_T("The file was successfully saved to SD-card"), _T(""), MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(_T("Failed to save file to SD-card"), _T(""), MB_OK|MB_ICONERROR);
	}
}

DWORD WINAPI CProgressDlg::ThreadProc(LPVOID lpParameter)
{
	CProgressDlg* pDlg = (CProgressDlg*) lpParameter;
	CVolumeAccess *vol = CVolumeAccess::getInstance();

	if (!vol->saveDataToSector(pDlg->m_strPath.c_str(), pDlg->m_dwStartSector, pDlg))
	{
		pDlg->EndDialog(IDCANCEL);
		return -1;
	}

	pDlg->EndDialog(IDOK);

	return 0;
}


BOOL CProgressDlg::OnInitDialog()
{
	__super::OnInitDialog();

	if (m_hProcessor)
		ResumeThread(m_hProcessor);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
