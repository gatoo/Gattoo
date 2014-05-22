#pragma once


// CResizeImgDlg dialog

class CResizeImgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CResizeImgDlg)

public:
	
	CResizeImgDlg(CSize szPexels, double dHZScale, double dVTScale, CWnd* pParent = NULL);   // standard constructor
	virtual ~CResizeImgDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_RESIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	
//	virtual BOOL OnInitDialog();

protected:

	void PrepareDlg(CSize szPexels);

	BOOL m_bKeepScale;

	double m_dWidthToHeightScale;

	double m_dHZScale;
	double m_dVTScale;

	double m_dHeightMM;
	double m_dWidthMM;

	DWORD m_dwHeightPX;
	DWORD m_dwWidthPX;
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	BOOL CheckForNumber(CString& strData, int iPos, BOOL bFloat) const;

	BOOL IsFloatAccepted(int iID);
};
