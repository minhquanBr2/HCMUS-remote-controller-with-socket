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


// CClientDlg dialog

IMPLEMENT_DYNAMIC(CClientDlg, CDialog)

CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CONNECT, pParent)
	, m_strIPAddress(_T(""))
	, m_iPort(0)
	, m_strMessage(_T(""))
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
	DDX_Text(pDX, IDC_MESSAGE, m_strMessage);
}


BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	ON_EN_CHANGE(IDC_MESSAGE, &CClientDlg::OnEnChangeMessage)
	ON_BN_CLICKED(IDC_CONNECT, &CClientDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDCANCEL, &CClientDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SEND, &CClientDlg::OnBnClickedSend)
END_MESSAGE_MAP()


// CClientDlg message handlers


void CClientDlg::OnEnChangeMessage()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CClientDlg::OnBnClickedConnect()
{
	// TODO: Add your control notification handler code here
	// Connecting the client to server
	UpdateData(TRUE);
	if (((CClientApp*)AfxGetApp())->m_ClientSocket.Connect(m_strIPAddress, m_iPort))
	{
		AfxMessageBox("Successfully connected to server!");
		((CClientApp*)AfxGetApp())->m_isConnected = TRUE;
		((CClientApp*)AfxGetApp())->m_pClientView->m_button[0].SetWindowText("DISCONNECT");
		CClientDlg::OnCancel();
	}
	else
	{
		AfxMessageBox("Failed to connected to server!");
	}
	DWORD error = GetLastError();
}


void CClientDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}


void CClientDlg::OnBnClickedSend()
{
	// TODO: Add your control notification handler code here
	// Sending the client to server
	UpdateData(TRUE);
	if (((CClientApp*)AfxGetApp())->m_ClientSocket.Send(m_strMessage.GetBuffer(m_strMessage.GetLength()), m_strMessage.GetLength()))
	{
		AfxMessageBox("Message sent successfully!");
	}
	else
	{
		AfxMessageBox("Failed to send message!");
	}
}
