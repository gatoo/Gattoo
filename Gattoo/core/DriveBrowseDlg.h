#pragma once

#include <list>
#include <string>

class CDriveBrowseDlg
{
public:
	
	CDriveBrowseDlg(HWND hParent = nullptr);
	~CDriveBrowseDlg(void);

	bool browseDrive(TCHAR &chDrive);

protected:

	static INT_PTR CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		
	bool FillDrivesList(std::list<std::basic_string<TCHAR> > &listDrives);
	void FillDrivesTree();

	char GetDriveLetter();
		
	typedef std::list<std::basic_string<TCHAR> > CDrivesList;
	
	HWND m_hWnd;
	TCHAR m_chDrive;
	CDrivesList m_listDrives;

	
};

