// BrowseDirDlg.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "BrowseDirDlg.h"
#include "afxdialogex.h"


// CBrowseDirDlg dialog

IMPLEMENT_DYNAMIC(CBrowseDirDlg, CDialogEx)

CBrowseDirDlg::CBrowseDirDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CBrowseDirDlg, pParent)
{

}

CBrowseDirDlg::~CBrowseDirDlg()
{
}

void CBrowseDirDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DIR, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CBrowseDirDlg, CDialogEx)
END_MESSAGE_MAP()


// CBrowseDirDlg message handlers


BOOL CBrowseDirDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

	// TODO: Add extra initialization here
    // Add columns to the CListView control
    m_listCtrl.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 300);
    m_listCtrl.InsertColumn(1, _T("Date Modified"), LVCFMT_LEFT, 100);
    m_listCtrl.InsertColumn(2, _T("Size"), LVCFMT_LEFT, 100);

    // Add items to the CListView control
    int nIndex = m_listCtrl.InsertItem(0, _T("Sandra C. Anschwitz"));
    m_listCtrl.SetItemText(nIndex, 1, _T("Singer"));
    m_listCtrl.SetItemText(nIndex, 2, _T("Handball"));

    nIndex = m_listCtrl.InsertItem(1, _T("Roger A. Miller"));
    m_listCtrl.SetItemText(nIndex, 1, _T("Footballer"));
    m_listCtrl.SetItemText(nIndex, 2, _T("Tennis"));

    return TRUE;

    return TRUE;
}
