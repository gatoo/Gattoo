// ImgSaver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <crtdbg.h>
#include <malloc.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <Shlobj.h>

#include "StorageRoutine/CVolumeAccess.h"
#include "DriveBrowseDlg.h"

bool getFileToSave(std::basic_string<TCHAR> &strFile)
{
	std::vector<TCHAR> vecFileBuf(MAX_PATH);
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hInstance = GetModuleHandle(NULL);
	ofn.lpstrFile = &vecFileBuf[0];
	ofn.nMaxFile = vecFileBuf.size();
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_FORCESHOWHIDDEN | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;

	if (GetOpenFileName(&ofn))
	{
		strFile = ofn.lpstrFile;
	}

	return true;
}

bool getDriveToSave(std::basic_string<TCHAR> &strDrive)
{
	TCHAR chDrive = 0;
	
	CDriveBrowseDlg dlg;
	if (dlg.browseDrive(chDrive))
		strDrive = chDrive;

	return chDrive != 0;
}

bool formatDrive(TCHAR const chDrive)
{
	std::basic_stringstream<TCHAR> str;
	
	str << _T("FORMAT ");
	str << chDrive;
	str << _T(": /FS:FAT32 /Q /V:Granit");

	//_tsystem(_T("format I:"));

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	LPTSTR szCmd =_tcsdup(TEXT("c:\\windows\\system32\\format.com I:"));
	CreateProcess(NULL, szCmd, NULL, NULL, true, 0, NULL, NULL, &si, &pi);

	WaitForSingleObject(pi.hProcess, INFINITE);
	//DWORD dw = SHFormatDrive(::GetDesktopWindow(), 3, SHFMT_ID_DEFAULT, SHFMT_OPT_FULL);

	return true;
}