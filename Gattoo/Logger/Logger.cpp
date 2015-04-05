#include "stdafx.h"
#include "Logger.h"


CLogger::CLogger() : m_pPresenter(nullptr)
{
}


CLogger::~CLogger()
{
}

void CLogger::SetPresenter(ILogPresenter* pPresenter)
{
	m_pPresenter = pPresenter;
}

void CLogger::LogMsg(LogLevel level, char const * lpctMsg)
{
	if (m_pPresenter)
		m_pPresenter->DisplayLogMsg(lpctMsg);
}
