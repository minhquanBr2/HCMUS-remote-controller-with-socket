// ClientSocket.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "ClientSocket.h"


// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}


// CClientSocket member functions


void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	char strRec[1024] = "";
	Receive(strRec, 1024);
	((CClientApp*)AfxGetApp())->m_pClientView->AddMsg(strRec);
	CSocket::OnReceive(nErrorCode);
}
