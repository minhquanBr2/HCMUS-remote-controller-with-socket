
// Server.h : main header file for the Server application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "ServerSocket.h"
#include "ServerView.h"


// CServerApp:
// See Server.cpp for the implementation of this class
//

class CServerApp : public CWinAppEx
{
public:
	CServerApp() noexcept;
	CServerSocket m_ServerSocket;
	CServerView *m_pServerView;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CServerApp theApp;
