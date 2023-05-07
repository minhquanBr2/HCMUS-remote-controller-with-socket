// BrowseDirDlg.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "BrowseDirDlg.h"
#include "afxdialogex.h"



// CBrowseDirDlg dialog

IMPLEMENT_DYNAMIC(CBrowseDirDlg, CDialogEx)

CBrowseDirDlg::CBrowseDirDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BDIR, pParent)
    , m_strPathMB(_T(""))
{

}

CBrowseDirDlg::~CBrowseDirDlg()
{
}

void CBrowseDirDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_DIR, m_listCtrl);
    DDX_Text(pDX, IDC_EDIT_PATH, m_strPathMB);
    DDX_Control(pDX, IDC_EDIT_PATH, m_strDisplay);
}


BEGIN_MESSAGE_MAP(CBrowseDirDlg, CDialogEx)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_DIR, &CBrowseDirDlg::OnListCtrlClick)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DIR, &CBrowseDirDlg::OnLvnItemchangedListDir)
END_MESSAGE_MAP()


// CBrowseDirDlg message handlers


BOOL CBrowseDirDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Attach the list control to the member variable just created
    m_listCtrl.SubclassDlgItem(IDD_BDIR, this);

    // Initial path is ""
    m_strPathMB = "";
    m_strDisplay.SetWindowText(m_strPathMB);

    // Add columns to the CListView control
    m_listCtrl.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 300);
    m_listCtrl.InsertColumn(1, _T("Date Modified"), LVCFMT_LEFT, 200);
    m_listCtrl.InsertColumn(2, _T("Size"), LVCFMT_LEFT, 100);

    // Concatenate all disk names for displaying
    int nDisk = 0;    
    CStringW rcv_msg = L"";
    for (CStringW disk : m_msgArr) {
        rcv_msg += disk;
        MessageBoxW(nullptr, rcv_msg, _T(L"Client3"), MB_OK);
    }

    // Displaying
    int curPos = 0;
    CStringW disk_name_W = rcv_msg.Tokenize(L"\n", curPos);
    while (!disk_name_W.IsEmpty())
    {
        // Convert Unicode string to MBCS string
        CStringA disk_name_MB;
        int nLen = WideCharToMultiByte(CP_ACP, 0, disk_name_W, -1, NULL, 0, NULL, NULL);
        if (nLen > 0)
        {
            char* szMB = disk_name_MB.GetBuffer(nLen);
            WideCharToMultiByte(CP_ACP, 0, disk_name_W, -1, szMB, nLen, NULL, NULL);
            disk_name_MB.ReleaseBuffer();
        }

        // Display to box
        m_listCtrl.InsertItem(nDisk, disk_name_MB);
        nDisk++;

        // Obtain next token
        disk_name_W = rcv_msg.Tokenize(L"\n", curPos);
    }
    return TRUE;
}


void CBrowseDirDlg::OnListCtrlClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    // Create message
    // Send
    // Receive
    // Display

    // Get the index of the selected item
    int nIndex = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);

    // If an item is selected, get its text and display it
    if (nIndex != -1)
    {
        // Change UI
        CStringA strTextMB = m_listCtrl.GetItemText(nIndex, 0); 
        m_strPrevPathMB = m_strPathMB;
        if (m_strPathMB != "") m_strPathMB += "\\";
        m_strPathMB = m_strPathMB + strTextMB;
        m_strDisplay.SetWindowText(m_strPathMB);

        // Prepare for sending message
        CStringA msg = "REQ_BDIR" + m_strPathMB;
        if (!m_msgArr.empty())
            m_msgArr.clear();

        // Send
        ((CClientApp*)AfxGetApp())->m_ClientSocket.Send(msg.GetBuffer(msg.GetLength()), msg.GetLength());

        // Receive
        BOOL browsable = ((CClientApp*)AfxGetApp())->m_pClientView->ReceiveBrowseDir(m_msgArr);

        // Process and display
        if (!browsable)
        {
            m_strPathMB = m_strPrevPathMB;
            m_strDisplay.SetWindowText(m_strPathMB);
        }
        else
        {
            int nDir = 0;
            int nIndex = 0;
            CStringW rcv_msg_str = L"";
            for (CStringW dir : m_msgArr) rcv_msg_str += dir;

            m_listCtrl.DeleteAllItems();

            std::vector<std::wstring> rcv_msg_vec = split(std::wstring(rcv_msg_str), L"\n");
            for (auto dir_str : rcv_msg_vec)
            {
                std::vector<std::wstring> dir_vec = split(dir_str, std::wstring(L"\t"));
                nIndex = m_listCtrl.InsertItem(nDir, (LPCTSTR)dir_vec[0].c_str());           // dir_name
                if (dir_vec.size() > 1) m_listCtrl.SetItemText(nIndex, 1, (LPCTSTR)dir_vec[1].c_str());              // dir_date
                if (dir_vec.size() > 2) m_listCtrl.SetItemText(nIndex, 2, (LPCTSTR)dir_vec[2].c_str());              // dir_size
                nDir++;
            }
        }
    }

    *pResult = 0;
}

std::vector<std::wstring> CBrowseDirDlg::split(std::wstring msg, std::wstring delimiter)
{
    std::vector<std::wstring> result;
    int startPos = 0;
    int curPos;

    while (1)
    {
        curPos = msg.find(delimiter, startPos);
        if (curPos == -1)
        {
            result.push_back(msg.substr(startPos, msg.length() - startPos));
            break;
        }
        else
        {
            result.push_back(msg.substr(startPos, curPos - startPos));
            startPos = curPos + 1;
        }
    }

    return result;
}



void CBrowseDirDlg::OnLvnItemchangedListDir(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;
}
