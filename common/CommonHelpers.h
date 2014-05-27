#pragma once

#include <string>

class CCommonHelpers
{
public:

	static bool getAppFolder(std::string& strPath);
	static bool getTempFilePath(std::string& strPath);
	static DWORD Round(double dInput);

private:
	CCommonHelpers(void);
	~CCommonHelpers(void);
};

