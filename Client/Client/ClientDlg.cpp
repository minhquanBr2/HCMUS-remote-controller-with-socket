// ClientDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "ClientDlg.h"
#include "ClientSocket.h"
#include "ClientView.h"
#include "afxwin.h"
#include "afxdialogex.h"

#include <string>


// CClientDlg dialog

IMPLEMENT_DYNAMIC(CClientDlg, CDialog)

CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CONNECT, pParent)
	, m_strIPAddress(_T(""))
	, m_iPort(0)
{

}

CClientDlg::~CClientDlg()
{
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IPADDRESS, m_strIPAddress);
	DDX_Text(pDX, IDC_PORT, m_iPort);
}


BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	ON_BN_CLICKED(IDC_CONNECT, &CClientDlg::OnBnClickedConnect)
END_MESSAGE_MAP()

void CClientDlg::OnBnClickedConnect()
{
	UpdateData(TRUE);
	// Check if the socket has been created or not, since Created() method can only be called once for every CSocket object
	if (((CClientApp*)AfxGetApp())->m_ClientSocket.m_hSocket == INVALID_SOCKET)
		((CClientApp*)AfxGetApp())->m_ClientSocket.Create();

	if (((CClientApp*)AfxGetApp())->m_ClientSocket.Connect(m_strIPAddress, m_iPort))
	{
		AfxMessageBox("Successfully connected to server!");
		((CClientApp*)AfxGetApp())->m_isConnected = TRUE;
		((CClientApp*)AfxGetApp())->m_pClientView->m_button[CONN].SetWindowText("DISCONNECT");
		((CClientApp*)AfxGetApp())->m_pClientView->UpdateButtons();
		CClientDlg::OnCancel();
	}
	else
	{
		AfxMessageBox("Failed to connected to server!");
	}
	DWORD error = GetLastError();
}