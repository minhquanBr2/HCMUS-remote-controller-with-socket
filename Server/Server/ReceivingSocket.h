#pragma once
#include <afxwin.h>

// CReceivingSocket command target

class CReceivingSocket : public CSocket
{
public:
	CReceivingSocket();
	virtual ~CReceivingSocket();
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	BOOL OnReceiveCapScreen(int nErrorCode);
	void OnReceiveKeystroke(int nErrorCode);
	void OnReceiveBrowseDir(int nErrorCode);
private:
	UINT_PTR m_timerId = NULL;
};


