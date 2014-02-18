
// VisualizerDlg.h : header file
//

#pragma once


// CVisualizerDlg dialog
class CVisualizerDlg : public CDialogEx
{
// Construction
public:
	CVisualizerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VISUALIZER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	CString m_strFile;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowse();
	unsigned char getColor(unsigned char chLevel);
	afx_msg void OnBnClickedButtonVisualize();
};
