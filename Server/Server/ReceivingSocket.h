#pragma once
#include <afxwin.h>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <Windows.h>
#include <TlHelp32.h>

#include "UtilScreenCapture.h"
#include "UtilKeystroke.h"
#include "UtilBrowseDirectory.h"
#include "UtilShowProcess.h"

// CReceivingSocket command target

class CReceivingSocket : public CSocket
{
public:
	CReceivingSocket();
	virtual ~CReceivingSocket();
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	BOOL OnReceiveCapScreen(int nErrorCode);
	void OnReceiveKeystroke(int nErrorCode);
	BOOL OnReceiveBrowseDisk(int nErrorCode);
	void OnReceiveBrowseDir(int nErrorCode, CStringW path);
	void OnReceiveShowPro(int nErrorCode);
	bool OnReceiveShowPro_Kill(int nErrorCode, int Pid);
private:
	UINT_PTR m_timerId = NULL;
};


