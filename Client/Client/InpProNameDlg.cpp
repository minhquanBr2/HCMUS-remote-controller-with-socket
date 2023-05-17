// InpProNameDlg.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "afxdialogex.h"
#include "InpProNameDlg.h"


// CInpProNameDlg dialog

IMPLEMENT_DYNAMIC(CInpProNameDlg, CDialogEx)

CInpProNameDlg::CInpProNameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INP_PRO_NAME_DLG, pParent)
{

}
CInpProNameDlg::~CInpProNameDlg()
{
}
void CInpProNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInpProNameDlg, CDialogEx)
	ON_EN_CHANGE(IDC_INP_NAME_PRO, &CInpProNameDlg::OnEnChangeInpNamePro)
	ON_BN_CLICKED(IDC_BTN_START, &CInpProNameDlg::OnBnClickedBtnStart)
END_MESSAGE_MAP()

// CInpProNameDlg message handlers

void CInpProNameDlg::OnBnClickedBtnStart()
{
	// TODO: Add your control notification handler code here

	CString ProName;
	GetDlgItem(IDC_INP_NAME_PRO)->GetWindowTextA(ProName);

	CString msg = CString("REQ_SPRO_START");
	msg = msg + " " + ProName;
	((CClientApp*)AfxGetApp())->m_ClientSocket.Send(msg.GetBuffer(msg.GetLength()), msg.GetLength());

	char buffer[1000] = "";
	int nBytesReceived = ((CClientApp*)AfxGetApp())->m_ClientSocket.Receive(buffer, 100000, 0);

	MessageBox(buffer);
}

void CInpProNameDlg::OnEnChangeInpNamePro()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}