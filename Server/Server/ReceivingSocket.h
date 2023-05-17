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
#include "UtilShowApp.h"

class CReceivingSocket : public CSocket
{
public:
	CReceivingSocket();
	virtual ~CReceivingSocket();
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);

// Chụp màn hình
public:
	BOOL OnReceiveCapScreen(int nErrorCode);

// Bắt phím nhấn
public:
	void OnReceiveKeystroke(int nErrorCode);
private:
	static HHOOK m_hook;
	CString m_strMsg;

// Duyệt cây thư mục
public:
	BOOL OnReceiveBrowseDisk(int nErrorCode);
	void OnReceiveBrowseDir(int nErrorCode, CStringW path);

// Khởi động và dừng ứng dụng
public:
	void OnReceiveShowApp(int ErrorCode);
	void OnReceiveShowApp_Kill(int nErrorCode, std::string appName);
	void OnReceiveShowApp_Start(int nErrorCode, std::string appName);

// Khởi động và dừng tiến trình
	void OnReceiveShowPro(int nErrorCode);
	void OnReceiveShowPro_Kill(int nErrorCode, int Pid);
	void OnReceiveShowPro_Start(int nErrorCode, std::string namePro);
private:
};

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);


