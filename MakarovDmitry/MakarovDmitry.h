// MakarovDmitry.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMakarovDmitryApp:
// See MakarovDmitry.cpp for the implementation of this class
//

class CMakarovDmitryApp : public CWinApp
{
public:
	CMakarovDmitryApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMakarovDmitryApp theApp;