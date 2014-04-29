#pragma once


#include "IProgress.h"
#include "../resource.h"
#include "afxcmn.h"
#include <string>

// CProgressDlg dialog

class CProgressDlg : public CDialogEx, public IProgress
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProgressDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SAVE_PROGRESS };

	virtual bool doStep();

	virtual bool init(int iMax, int iStep);
	
protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()	

	int m_iMaxValue;
	CProgressCtrl m_progressCtrl;

	IProcessor *m_pProcessor;

	static DWORD WINAPI ThreadProc(LPVOID lpParameter);

	HANDLE m_hProcessor;

	const DWORD m_dwStartSector;
	std::string m_strPath;

public:
	afx_msg void OnBnClickedCancel();
	void startSave(std::string &strPath);
	virtual BOOL OnInitDialog();
};
