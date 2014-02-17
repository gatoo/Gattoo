#pragma once

#include <string>

class CCommonHelpers
{
public:

	static bool getAppFolder(std::string& strPath);
	static bool getTempFilePath(std::string& strPath);

private:
	CCommonHelpers(void);
	~CCommonHelpers(void);
};

