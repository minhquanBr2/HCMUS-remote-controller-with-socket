#pragma once

// CClientSocket command target

class CClientSocket : public CSocket
{
public:
	char m_msg[1024] = "";
	CClientSocket();
	virtual ~CClientSocket();
	virtual void OnReceive(int nErrorCode);
};


