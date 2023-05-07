// ShowProcessDlg.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "ShowProcessDlg.h"
#include "afxdialogex.h"


// CShowProcessDlg dialog

IMPLEMENT_DYNAMIC(CShowProcessDlg, CDialogEx)

CShowProcessDlg::CShowProcessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SPRO1, pParent)
{

}

CShowProcessDlg::~CShowProcessDlg()
{
}

void CShowProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRO, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CShowProcessDlg, CDialogEx)
	ON_BN_CLICKED(ID_KILL_BTN, &CShowProcessDlg::OnBnClickedKillBtn)
	ON_BN_CLICKED(ID_START_PRO, &CShowProcessDlg::OnBnClickedStartPro)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PRO, &CShowProcessDlg::OnLvnItemchangedListPro)
END_MESSAGE_MAP()


// CShowProcessDlg message handlers
std::vector<std::pair<std::string, std::string>> splitString(const std::string& str)
{
	std::vector<std::string> substrings;
	std::string temp;

	for (char c : str)
	{
		if (c == '/') // Use '/' as the delimiter instead of '.'
		{
			substrings.push_back(temp);
			temp.clear();
		}
		else
		{
			temp += c;
		}
	}

	if (!temp.empty())
	{
		substrings.push_back(temp);
	}

	std::vector<std::pair<std::string, std::string>> result;

	for (int i = 0; i < substrings.size() - 1; i += 2)
	{
		result.push_back(std::make_pair(substrings[i], substrings[i + 1]));
	}

	return result;
}


void CShowProcessDlg::OnBnClickedKillBtn()
{
	CString msg = CString("REQ_SPRO_KILL");
	msg = msg + " " + cur_PID;
	((CClientApp*)AfxGetApp())->m_ClientSocket.Send(msg.GetBuffer(msg.GetLength()), msg.GetLength());

	//MessageBox((LPCTSTR)msg, "Client");

	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_PRO);

	// Get the selected item index
	int nIndex = pListCtrl->GetSelectionMark();

	if (nIndex >= 0)
	{
		// Delete the selected item
		pListCtrl->DeleteItem(nIndex);
	}
}


void CShowProcessDlg::OnBnClickedStartPro()
{
	// TODO: Add your control notification handler code here
}


void CShowProcessDlg::OnLvnItemchangedListPro(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_PRO);
	int nIndex = pListCtrl->GetSelectionMark();

	if (nIndex >= 0)
	{
		cur_PID = pListCtrl->GetItemText(nIndex, 1);
	}
}


BOOL CShowProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_listCtrl.SubclassDlgItem(IDD_SPRO, this);

	m_listCtrl.InsertColumn(0, _T("Process Name"), LVCFMT_LEFT, 300);
	m_listCtrl.InsertColumn(1, _T("Process ID"), LVCFMT_LEFT, 200);

	// xu ly process

	std::string process = ((CClientApp*)AfxGetApp())->m_pClientView->m_strProcess;


	//MessageBox(process.c_str());

	//std::stringstream ss(process);
	//std::string name, id;
	//while (ss >> name >> id) {
	//	int nIndex = m_listCtrl.InsertItem(0, _T(name.c_str()));
	//	m_listCtrl.SetItemText(nIndex, 1, _T(id.c_str()));
	//	/*MessageBox(name.c_str());
	//	MessageBox(id.c_str());*/
	//}

	std::vector<std::pair<std::string, std::string>> lst = splitString(process);
	for (std::pair<std::string, std::string> x : lst) {
		int nIndex = m_listCtrl.InsertItem(0, _T(x.first.c_str()));
		m_listCtrl.SetItemText(nIndex, 1, _T(x.second.c_str()));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
