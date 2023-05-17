// ServerSocket.cpp : implementation file
//

#include "pch.h"
#include "Server.h"
#include "ServerSocket.h"
#include "ReceivingSocket.h"
#include <afxwin.h>					// include the MFC header file
#include <string>


// CServerSocket

CServerSocket::CServerSocket()
{
}

CServerSocket::~CServerSocket()
{
}


// CServerSocket member functions
void CServerSocket::OnAccept(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	// Accepting incoming connections	
	// AfxMessageBox("Client has successfully connected!");
	Accept(m_ReceivingSocket);
	CSocket::OnAccept(nErrorCode);
}


void CServerSocket::OnReceive(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	// Receiving data
	AfxMessageBox("Data received!");
	CSocket::OnReceive(nErrorCode);
}
