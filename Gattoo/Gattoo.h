
// Gattoo.h : main header file for the Gattoo application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Logger/ILogger.h"

// CGattooApp:
// See Gattoo.cpp for the implementation of this class
//

class CGattooApp : public CWinAppEx
{
public:

	CGattooApp();

	void AddLogMsg(char const * msg, char const * szFile, int iLine, ...);

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:

	ILogger * m_pLogger;
};

extern CGattooApp theApp;
