
// TPArticleBroadCast.h : main header file for the TPArticleBroadCast application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTPArticleBroadCastApp:
// See TPArticleBroadCast.cpp for the implementation of this class
//

class CTPArticleBroadCastApp : public CWinAppEx
{
public:
	CTPArticleBroadCastApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CTPArticleBroadCastApp theApp;
