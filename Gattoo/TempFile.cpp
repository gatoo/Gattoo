#include "StdAfx.h"

#include "TempFile.h"
#include "CommonHelpers.h"

CTempFile::CTempFile(void)
	: m_File(nullptr)
{
}

CTempFile::~CTempFile(void)
{
	Close();
}

CTempFile::operator FILE*() const
{
	return m_File;
}

bool CTempFile::Create(LPCTSTR lpszPath /*= nullptr*/)
{
	if (nullptr == lpszPath)
		CCommonHelpers::getTempFilePath(m_strFilePath);
	else
		m_strFilePath.assign(lpszPath);

	m_File = fopen(m_strFilePath.c_str(), "wb");

	return m_File != nullptr;
}

bool CTempFile::Close(bool bDelete /*= true*/)
{
	if (!m_File) return false;

	fclose(m_File);
	m_File = nullptr;

	return bDelete ? 0 != DeleteFile(m_strFilePath.c_str()) : true;
}
