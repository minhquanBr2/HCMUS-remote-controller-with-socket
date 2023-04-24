#pragma once

// CReceivingSocket command target

class CReceivingSocket : public CSocket
{
public:
	CReceivingSocket();
	virtual ~CReceivingSocket();
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


