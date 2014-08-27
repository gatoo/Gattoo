#pragma once

#include <string>

class CTempFile
{
public:
	CTempFile(void);
	virtual ~CTempFile(void);

	bool Create(LPCTSTR lpszPath = nullptr);

	bool Close(bool bDelete = true);

	LPCTSTR getName();

	operator FILE* () const;

protected:

	FILE* m_File;
	std::string m_strFilePath;
};

