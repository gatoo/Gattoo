#pragma once

__interface ILogPresenter
{
	virtual void DisplayLogMsg(char const * lpctMsg) = 0;
};