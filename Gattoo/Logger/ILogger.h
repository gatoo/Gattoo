#pragma once

#include "ILogPresenter.h"

__interface ILogger
{
	enum LogLevel
	{
		enInformation,
		enWarning,
		enError
	};

	virtual void SetPresenter(ILogPresenter* pPresenter) = 0;
	virtual void LogMsg(LogLevel level, char const * lpctMsg) = 0;
};