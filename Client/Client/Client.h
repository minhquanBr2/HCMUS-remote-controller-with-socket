
// Client.h : main header file for the Client application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "ClientView.h"
#include "ClientDlg.h"
#include "MainFrm.h"


// CClientApp:
// See Client.cpp for the implementation of this class
//

class CClientApp : public CWinAppEx
{
public:
	CClientApp() noexcept;
	CClientView* m_pClientView;
	CClientSocket m_ClientSocket;
	CClientDlg m_ClientDlg;
	bool m_isConnected;


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

extern CClientApp theApp;
