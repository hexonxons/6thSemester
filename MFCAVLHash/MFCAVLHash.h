// MFCAVLHash.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFCAVLHashApp:
// See MFCAVLHash.cpp for the implementation of this class
//

class CMFCAVLHashApp : public CWinApp
{
public:
	CMFCAVLHashApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFCAVLHashApp theApp;