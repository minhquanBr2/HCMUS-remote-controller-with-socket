#pragma once

// CServerSocket command target
#include "ReceivingSocket.h"

class CServerSocket : public CSocket
{
public:
	CReceivingSocket m_ReceivingSocket;
	CServerSocket();
	virtual ~CServerSocket();
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};


