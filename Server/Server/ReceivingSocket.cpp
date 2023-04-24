// ReceivingSocket.cpp : implementation file
//

#include "pch.h"
#include "Server.h"
#include "ReceivingSocket.h"
#include <filesystem>
#include <iostream>

#include "UtilScreenCapture.h"
#include "UtilListDirectories.h"

namespace fs = std::filesystem;


// CReceivingSocket

CReceivingSocket::CReceivingSocket()
{
}

CReceivingSocket::~CReceivingSocket()
{
}

void CReceivingSocket::OnReceive(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	char strRec[256] = "";
	Receive(strRec, 256);

	if (strcmp(strRec, "REQ_SCR_CAP") == 0)
	{
		screenCapture();
	}
	else if (strcmp(strRec, "REQ_LST_DIR") == 0)
	{
		CString dirs(listDirectories().c_str());
		Send(dirs.GetBuffer(dirs.GetLength()), dirs.GetLength());

	}
	((CServerApp*)AfxGetApp())->m_pServerView->AddMsg(strRec);
	CSocket::OnReceive(nErrorCode);
}


void CReceivingSocket::OnClose(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	AfxMessageBox("Connection Closed!");
	Close();
	CSocket::OnClose(nErrorCode);
}
