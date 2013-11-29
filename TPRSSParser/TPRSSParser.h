// TPRSSParser.h : main header file for the TPRSSParser DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTPRSSParserApp
// See TPRSSParser.cpp for the implementation of this class
//

class CTPRSSParserApp : public CWinApp
{
public:
	CTPRSSParserApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
