// ShowAppDlg.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "afxdialogex.h"
#include "ShowAppDlg.h"


// CShowAppDlg dialog

IMPLEMENT_DYNAMIC(CShowAppDlg, CDialogEx)

CShowAppDlg::CShowAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHOW_APP_DLG, pParent)
{

}

CShowAppDlg::~CShowAppDlg()
{
}

void CShowAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_APP, m_ListControl);
}

BEGIN_MESSAGE_MAP(CShowAppDlg, CDialogEx)
	ON_BN_CLICKED(ID_KILL_APP_BTN, &CShowAppDlg::OnBnClickedKillBtn)
	ON_BN_CLICKED(ID_START_APP_BTN, &CShowAppDlg::OnBnClickedStartApp)
	ON_NOTIFY(NM_CLICK, IDC_LIST_APP, &CShowAppDlg::OnLvnItemchangedListApp)
END_MESSAGE_MAP()


// CShowAppDlg message handlers

std::vector<std::string> splitString(const std::string& input)
{
	std::vector<std::string> result;
	std::istringstream iss(input);
	std::string token;

	while (std::getline(iss, token, '/'))
	{
		result.push_back(token);
	}

	return result;
}

void CShowAppDlg::OnBnClickedKillBtn()
{
	// TODO: Add your control notification handler code here
	CString msg = CString("REQ_SAPP_KILL");
	msg = msg + " " + cur_Name;
	((CClientApp*)AfxGetApp())->m_ClientSocket.Send(msg.GetBuffer(msg.GetLength()), msg.GetLength());

	char buffer[1000] = "";
	int nBytesReceived = ((CClientApp*)AfxGetApp())->m_ClientSocket.Receive(buffer, 100000, 0);

	MessageBox(buffer);
	if (strcmp(buffer, "Failed!") == 0) {
		return;
	}

	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_APP);

	// Get the selected item index
	int nIndex = pListCtrl->GetSelectionMark();

	if (nIndex >= 0)
	{
		pListCtrl->DeleteItem(nIndex);
	}
}

void CShowAppDlg::OnBnClickedStartApp()
{
	// TODO: Add your control notification handler code here
	m_dlgINP.DoModal();
}

BOOL CShowAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	m_ListControl.SubclassDlgItem(IDD_SHOW_APP_DLG, this);
	m_ListControl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

	m_ListControl.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 100);
	m_ListControl.InsertColumn(1, _T("Application Name"), LVCFMT_LEFT, 300);

	std::string process = ((CClientApp*)AfxGetApp())->m_pClientView->m_strApp;

	std::vector<std::string> lst = splitString(process);

	sort(lst.begin(), lst.end());

	int cnt = 0;
	for (std::string x : lst) {
		++cnt;
		std::string ID = std::to_string(cnt);
		int nIndex = m_ListControl.InsertItem(cnt - 1, _T(ID.c_str()));
		m_ListControl.SetItemText(nIndex, 1, _T(x.c_str()));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CShowAppDlg::OnLvnItemchangedListApp(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_APP);
	int nIndex = pListCtrl->GetSelectionMark();

	if (nIndex >= 0)
	{
		cur_Name = pListCtrl->GetItemText(nIndex, 1);
	}
}