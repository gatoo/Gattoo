#pragma once
#include "ILogger.h"
#include "ILogPresenter.h"

class CLogger : public ILogger
{
public:
	CLogger();
	virtual ~CLogger();

	virtual void SetPresenter(ILogPresenter* pPresenter);
	virtual void LogMsg(LogLevel level, char const * lpctMsg);

private:
	ILogPresenter* m_pPresenter;
};

