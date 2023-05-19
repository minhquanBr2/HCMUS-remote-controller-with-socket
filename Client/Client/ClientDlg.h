#pragma once
#include "ClientSocket.h"


// CClientDlg dialog

class CClientDlg : public CDialog
{
	DECLARE_DYNAMIC(CClientDlg)

public:
	CClientDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CClientDlg();

	

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONNECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strIPAddress;
	UINT m_iPort;
	afx_msg void OnBnClickedConnect();
};
