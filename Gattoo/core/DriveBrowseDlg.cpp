#include "StdAfx.h"

#include <atltrace.h>

#include "../resource.h"
#include "DriveBrowseDlg.h"

#include <Commctrl.h>
#include <vector>

CDriveBrowseDlg::CDriveBrowseDlg(void)
	: m_hWnd(NULL)
	, m_chDrive(0)
{
}


CDriveBrowseDlg::~CDriveBrowseDlg(void)
{
}

bool CDriveBrowseDlg::browseDrive(TCHAR &chDrive)
{
	// Initialize the common control library.
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_TREEVIEW_CLASSES;

	InitCommonControlsEx(&icc);

	m_hWnd = NULL;
	m_chDrive = 0;

	INT_PTR res = DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_DRIVES), NULL, DialogProc, (LPARAM) this);

	if (IDOK == res)
		chDrive = m_chDrive;

	return m_chDrive != 0;
}

INT_PTR CALLBACK CDriveBrowseDlg::DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CDriveBrowseDlg* pDlg = NULL;

	if ((uMsg == WM_INITDIALOG) && (NULL != lParam))
	{
		SetWindowLongPtr(hwnd, DWLP_USER, (LONG_PTR)lParam);

		pDlg = (CDriveBrowseDlg*)lParam;
		pDlg->m_hWnd = hwnd;

		pDlg->FillDrivesTree();

		return FALSE;
	}

	pDlg = (CDriveBrowseDlg*) GetWindowLongPtr(hwnd, DWLP_USER);

	if (NULL == pDlg) return FALSE;

	switch(uMsg)
	{
	case WM_COMMAND:
		{
			pDlg->m_chDrive = 0;

			if (LOWORD(wParam) == 1)
			{

				HWND hTree = GetDlgItem(pDlg->m_hWnd, IDC_TREE_DRIVES);
				HTREEITEM hItem = TreeView_GetSelection(hTree);
				HTREEITEM hRoot = TreeView_GetRoot(hTree);

				if (hItem != hRoot)
				{
					TVITEM item;
					ZeroMemory(&item, sizeof(item));
					item.hItem = hItem;
					item.mask = TVIF_PARAM;
					
					BOOL b = TreeView_GetItem(hTree, &item);

					pDlg->m_chDrive = ((LPCTSTR) item.lParam)[0];
				}
				else
					pDlg->m_chDrive = 0;
			}
			
			EndDialog(pDlg->m_hWnd, LOWORD(wParam));				
		}
		break;
	
	case WM_NOTIFY:
		{
			HWND hTree = GetDlgItem(pDlg->m_hWnd, IDC_TREE_DRIVES);
			if (reinterpret_cast<LPNMHDR>(lParam)->hwndFrom == hTree)
			{
				if (reinterpret_cast<LPNMHDR>(lParam)->code == NM_DBLCLK)
				{
					int y= 0;// double-click
				}
			}
		}
		
		break;
	}	
	
	return 0;
}

bool CDriveBrowseDlg::FillDrivesList(CDrivesList &listDrives)
{
	std::basic_string<TCHAR> strName;
	std::vector<BYTE> arr;

	DWORD dwSize = 0;
	dwSize = GetLogicalDriveStrings(dwSize, NULL);

	arr.resize((dwSize+1)*sizeof(TCHAR));

	if (0 == GetLogicalDriveStrings(dwSize, (LPTSTR) &arr[0])) return false;

	LPCTSTR pDrive = (LPCTSTR) &arr[0];

	do 
	{
		if (DRIVE_REMOVABLE == GetDriveType(pDrive))
			listDrives.push_back(pDrive);

		pDrive = pDrive + _tcslen(pDrive);

	} while (*(++pDrive));

#ifdef _DEBUG

	TCHAR szDrive[] = _T("H:\\");

	for (int i=0; i<0; ++i)
	{
		szDrive[0]++;
		listDrives.push_back(szDrive);
	}

#endif

	return true;
}

void CDriveBrowseDlg::FillDrivesTree()
{
	FillDrivesList(m_listDrives);
	
	HWND hTree = GetDlgItem(m_hWnd, IDC_TREE_DRIVES);
	CDrivesList::iterator iter = m_listDrives.begin();
	
	TVITEM item;
	TVINSERTSTRUCT insItem;
	
	ZeroMemory(&insItem, sizeof(insItem));
	ZeroMemory(&item, sizeof(item));

	item.mask = TVIF_TEXT|TVIF_PARAM;
	item.pszText = _T("Removable media volumes");

	insItem.item = item;
	insItem.hParent = NULL;

	HTREEITEM hRoot = (HTREEITEM)SendMessage(hTree, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&insItem); 

	insItem.hParent = hRoot;
	insItem.hInsertAfter = TVI_SORT;

	for(iter; iter != m_listDrives.end(); ++iter)
	{
		insItem.item.pszText = const_cast<LPTSTR>((*iter).c_str());
		insItem.item.lParam = (LPARAM) ((*iter).c_str());
		SendMessage(hTree, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&insItem);
	}

	TreeView_Expand(hTree, hRoot, TVE_EXPAND);
}
