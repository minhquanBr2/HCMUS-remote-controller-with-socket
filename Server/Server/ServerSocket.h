#pragma once

// CServerSocket command target

class CServerSocket : public CSocket
{
public:
	CServerSocket();
	virtual ~CServerSocket();
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};


