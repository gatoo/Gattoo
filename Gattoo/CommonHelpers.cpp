#include "StdAfx.h"
#include "CommonHelpers.h"


CCommonHelpers::CCommonHelpers(void)
{
}


CCommonHelpers::~CCommonHelpers(void)
{
}

bool CCommonHelpers::getAppFolder(std::string& strPath)
{
	DWORD dwSize = MAX_PATH;
	TCHAR lpszAppPath[MAX_PATH] = {0};

	if (!GetModuleFileName(NULL, lpszAppPath, dwSize)) return false;

	LPTSTR lp = _tcsrchr(lpszAppPath, _T('\\'));
	if (!lp) return false;

	*(++lp) = _T('\0');

	strPath.assign(lpszAppPath);

	return true;
}

bool CCommonHelpers::getTempFilePath(std::string& strPath)
{
	if (!getAppFolder(strPath))
	{
		strPath.clear();
		return false;
	}

	char buffer[L_tmpnam];
	tmpnam(buffer);

	strPath.append(buffer);

	return true;
}
