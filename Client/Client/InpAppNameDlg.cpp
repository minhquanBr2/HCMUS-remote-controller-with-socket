// InpAppNameDlg.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "afxdialogex.h"
#include "InpAppNameDlg.h"


// CInpAppNameDlg dialog

IMPLEMENT_DYNAMIC(CInpAppNameDlg, CDialogEx)

CInpAppNameDlg::CInpAppNameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INP_APP_NAME_DLG, pParent)
{

}

CInpAppNameDlg::~CInpAppNameDlg()
{
}

void CInpAppNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInpAppNameDlg, CDialogEx)
	ON_EN_CHANGE(IDC_INP_NAME_APP, &CInpAppNameDlg::OnEnChangeInpNameApp)
	ON_BN_CLICKED(IDC_BTN_START, &CInpAppNameDlg::OnBnClickedBtnStart)
END_MESSAGE_MAP()


// CInpAppNameDlg message handlers


void CInpAppNameDlg::OnEnChangeInpNameApp()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CInpAppNameDlg::OnBnClickedBtnStart()
{
	// TODO: Add your control notification handler code here
	CString AppName;
	GetDlgItem(IDC_INP_NAME_APP)->GetWindowTextA(AppName);

	CString msg = CString("REQ_SAPP_START");
	msg = msg + " " + AppName;
	((CClientApp*)AfxGetApp())->m_ClientSocket.Send(msg.GetBuffer(msg.GetLength()), msg.GetLength());

	char buffer[1000] = "";
	int nBytesReceived = ((CClientApp*)AfxGetApp())->m_ClientSocket.Receive(buffer, 100000, 0);

	MessageBox(buffer);
}