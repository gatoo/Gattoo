
// VisualizerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Visualizer.h"
#include "VisualizerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVisualizerDlg dialog



CVisualizerDlg::CVisualizerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVisualizerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVisualizerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVisualizerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CVisualizerDlg::OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_VISUALIZE, &CVisualizerDlg::OnBnClickedButtonVisualize)
END_MESSAGE_MAP()


// CVisualizerDlg message handlers

BOOL CVisualizerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVisualizerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVisualizerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVisualizerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#include <opencv2/opencv.hpp>

struct SImgHeader
{
	char szImgName[16];
	char szImgSize[16];

	short shXSize;
	short shYSize;

	unsigned int uiDotsCount;

	char padding[24];
};

#define MAKE_LE(shY)  (((shY >> 8) & 0xFF) | ((shY << 8) & 0xFF00 ))
#define MAKE_DWORD_LE(dwY)  (((dwY >> 24) & 0x000000FF) | ((dwY >> 8) & 0x0000FF00) | ((dwY << 24) & 0xFF000000) | ((dwY << 8) & 0x00FF0000))

unsigned char CVisualizerDlg::getColor(unsigned char chLevel)
{
	unsigned char chRes = 0;

	switch (chLevel)
	{
	case 7: chRes = 0xFF; break;
	case 6: chRes = 0xD8; break;
	case 5: chRes = 0xB4; break;
	case 4: chRes = 0x90; break;
	case 3: chRes = 0x6C; break;
	case 2: chRes = 0x48; break;
	case 1: chRes = 0x24; break;
	case 0: chRes = 0x00; break;

	default: assert(false); break;
	}

	return chRes;
}

void CVisualizerDlg::OnBnClickedButtonBrowse()
{
	CFileDialog dlg(TRUE);

	if (IDOK != dlg.DoModal()) return;

	m_strFile = dlg.GetPathName();
}


void CVisualizerDlg::OnBnClickedButtonVisualize()
{
	cv::Mat srcImg;
	cv::Mat zoomedImg;
	size_t readData = 0;
	SImgHeader header;

	const int BLOCK_SIZE = 64;

	char szDataBuf[BLOCK_SIZE];

	FILE *f = fopen(m_strFile.GetBuffer(), "rb");
	if (NULL == f) return;

	readData = fread(&header, 1, sizeof(SImgHeader), f);

	if (sizeof(SImgHeader) != readData) return;

	srcImg.create(MAKE_LE(header.shYSize), MAKE_LE(header.shXSize), CV_8UC1);
	srcImg = 0x5F;

	readData = fread(szDataBuf, 1, BLOCK_SIZE, f);

	if (EOF == readData || readData != BLOCK_SIZE) return;

	unsigned short iY =0;
	unsigned short iX =0;
	unsigned char uColor = 0;
	unsigned short iData = 0;

	short *pShortData = (short*) szDataBuf;

	cv::resize(srcImg, zoomedImg, cv::Size(srcImg.cols * 5, srcImg.rows*5), 0, 0, cv::INTER_NEAREST);
	cv::namedWindow("Vizualizer", cv::WINDOW_NORMAL);
	cv::imshow("Vizualizer", zoomedImg);

	do 
	{
		for (int i=0; i<BLOCK_SIZE/sizeof(iData); ++i)
		{

			iData = MAKE_LE(pShortData[i]);

			if (iData & 0x8000)
			{
				iY = iData & 0xFFF;
				continue;
			}
			else
			{
				uColor = getColor((iData & 0x7000) >> 12);
				iX = iData & 0xFFF;
			}

			srcImg.at<unsigned char>(iY, iX) = uColor;

			cv::waitKey(200);

			cv::resize(srcImg, zoomedImg, cv::Size(srcImg.cols * 5, srcImg.rows*5), 0, 0, cv::INTER_NEAREST);
			cv::imshow("Vizualizer", zoomedImg);
		}

		readData = fread(szDataBuf, 1, BLOCK_SIZE, f);		

	} while (EOF != readData && readData == BLOCK_SIZE);
}
