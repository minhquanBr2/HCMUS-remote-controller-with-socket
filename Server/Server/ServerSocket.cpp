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
	//Accept(m_ReceivingSocket);


    if (Accept(m_ReceivingSocket))
    {
        // Client connected successfully
        CString clientAddress;
        UINT clientPort;

        // Get the client's IP address and port
        m_ReceivingSocket.GetPeerName(clientAddress, clientPort);

        // Convert the IP address to string
        CString ipAddress;
        if (clientAddress.GetLength() == sizeof(SOCKADDR_IN))
        {
            sockaddr_in* pSockAddr = reinterpret_cast<sockaddr_in*>(clientAddress.GetBuffer());
            ipAddress = inet_ntoa(pSockAddr->sin_addr);
        }

        // Output the client's IP address and port
        std::string connected_msg = "Client connected from " + std::string(clientAddress) + ":" + std::to_string(clientPort);
        AfxMessageBox(connected_msg.c_str());
        //TRACE("Client connected from %s:%d\n", ipAddress, clientPort);
    }
	CSocket::OnAccept(nErrorCode);
}


void CServerSocket::OnReceive(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	// Receiving data
	AfxMessageBox("Data received!");
	CSocket::OnReceive(nErrorCode);
}
