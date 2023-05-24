// BrowseDirDlg.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "BrowseDirDlg.h"
#include "afxdialogex.h"

#define BUFFER_SIZE 65536



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
    ON_BN_CLICKED(IDC_BTN_BACK, &CBrowseDirDlg::OnBnClickedBtnBack)
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
    for (CStringA disk : m_msgArr) {
        rcv_msg += disk;
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

// Convert a CStringW string to a CStringA string
CStringA ConvertCStringWToCStringA(const CStringW& strW)
{
    int lenA = WideCharToMultiByte(CP_UTF8, 0, strW, -1, NULL, 0, NULL, NULL);
    char* ptrA = new char[lenA];
    WideCharToMultiByte(CP_UTF8, 0, strW, -1, ptrA, lenA, NULL, NULL);
    CStringA strA(ptrA, lenA);
    delete[] ptrA;
    return strA;
}

void CBrowseDirDlg::OnListCtrlClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

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
        BOOL browsable = this->ReceiveBrowseDir(m_msgArr);

        // Process and display
        if (!browsable)
        {
            m_strPathMB = m_strPrevPathMB;
            m_strDisplay.SetWindowText(m_strPathMB);
            AfxMessageBox("Unable to browse more!");
        }
        else
        {
            CStringW rcv_msg_str = L"";
            for (CStringA dir : m_msgArr)
            {
                int length = dir.GetLength() + 1; // Get the length of the string, including the null terminator
                int wLength = MultiByteToWideChar(CP_UTF8, 0, dir, length, NULL, 0); // Get the required size of the buffer for the wide char string
                CStringW wdir; // Create a CStringW to store the wide char string
                wchar_t* buffer = wdir.GetBuffer(wLength); // Get a buffer for the wide char string
                MultiByteToWideChar(CP_UTF8, 0, dir, length, buffer, wLength); // Convert the multibyte string to a wide char string
                wdir.ReleaseBuffer(); // Release the buffer

                rcv_msg_str += wdir;
            }

            m_listCtrl.DeleteAllItems();

            CStringW delimiterOut(L"\n"); 
            CStringW tokenOut; 
            int startOut = 0; 
            int nIndex = 0;
            int nDir = 0;
            while ((tokenOut = rcv_msg_str.Tokenize(delimiterOut, startOut)) != L"") {

                CStringW delimiterIn(L"\t");
                CStringW tokenIn;
                int startIn = 0;
                std::vector<CStringW> dir_info;
                while ((tokenIn = tokenOut.Tokenize(delimiterIn, startIn)) != L"")
                {
                    dir_info.push_back(tokenIn);
                }
                nIndex = m_listCtrl.InsertItem(nDir, ConvertCStringWToCStringA(dir_info[0]));           // dir_name
                if (dir_info.size() > 1) m_listCtrl.SetItemText(nIndex, 1, ConvertCStringWToCStringA(dir_info[1]));              // dir_date
                if (dir_info.size() > 2) m_listCtrl.SetItemText(nIndex, 2, ConvertCStringWToCStringA(dir_info[2]));              // dir_size
                nDir++;
            }
        }
    }

    *pResult = 0;
}

void CBrowseDirDlg::OnBnClickedBtnBack()
{
    // Change UI 
    if (m_strPathMB.Right(1) == ":")
    {
        // Initial path is ""
        m_strPathMB = "";
        m_strDisplay.SetWindowText(m_strPathMB);

        // Add columns to the CListView control
        m_listCtrl.DeleteAllItems();
        m_listCtrl.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 300);
        m_listCtrl.InsertColumn(1, _T("Date Modified"), LVCFMT_LEFT, 200);
        m_listCtrl.InsertColumn(2, _T("Size"), LVCFMT_LEFT, 100);

        // browse disks
        const char* msg = "REQ_BDIR";
        CStringA strMsg(msg);
        ((CClientApp*)AfxGetApp())->m_ClientSocket.Send(strMsg.GetBuffer(strMsg.GetLength()), strMsg.GetLength());
        this->ReceiveBrowseDisk(m_msgArr);

        // Concatenate all disk names for displaying
        int nDisk = 0;
        CStringW rcv_msg = L"";
        for (CStringA disk : m_msgArr) {
            rcv_msg += disk;
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
    }



    else
    {
        // Change UI
        m_strPathMB = m_strPrevPathMB;
        m_strPrevPathMB = m_strPrevPathMB.Left(m_strPrevPathMB.ReverseFind('\\'));
        m_strDisplay.SetWindowText(m_strPathMB);

        // Prepare for sending message
        CStringA msg = "REQ_BDIR" + m_strPathMB;
        if (!m_msgArr.empty())
            m_msgArr.clear();

        // Send
        ((CClientApp*)AfxGetApp())->m_ClientSocket.Send(msg.GetBuffer(msg.GetLength()), msg.GetLength());

        // Receive
        BOOL browsable = this->ReceiveBrowseDir(m_msgArr);

        // Process and display
        if (!browsable)
        {
            m_strPathMB = m_strPrevPathMB;
            m_strDisplay.SetWindowText(m_strPathMB);
            AfxMessageBox("Invalid path!");
        }
        else
        {
            CStringW rcv_msg_str = L"";
            for (CStringA dir : m_msgArr)
            {
                int length = dir.GetLength() + 1; // Get the length of the string, including the null terminator
                int wLength = MultiByteToWideChar(CP_UTF8, 0, dir, length, NULL, 0); // Get the required size of the buffer for the wide char string
                CStringW wdir; // Create a CStringW to store the wide char string
                wchar_t* buffer = wdir.GetBuffer(wLength); // Get a buffer for the wide char string
                MultiByteToWideChar(CP_UTF8, 0, dir, length, buffer, wLength); // Convert the multibyte string to a wide char string
                wdir.ReleaseBuffer(); // Release the buffer

                rcv_msg_str += wdir;
            }

            m_listCtrl.DeleteAllItems();


            CStringW delimiterOut(L"\n");
            CStringW tokenOut;
            int startOut = 0;
            int nIndex = 0;
            int nDir = 0;
            while ((tokenOut = rcv_msg_str.Tokenize(delimiterOut, startOut)) != L"") {

                CStringW delimiterIn(L"\t");
                CStringW tokenIn;
                int startIn = 0;
                std::vector<CStringW> dir_info;
                while ((tokenIn = tokenOut.Tokenize(delimiterIn, startIn)) != L"")
                    dir_info.push_back(tokenIn);
                nIndex = m_listCtrl.InsertItem(nDir, ConvertCStringWToCStringA(dir_info[0]));           // dir_name
                if (dir_info.size() > 1) m_listCtrl.SetItemText(nIndex, 1, ConvertCStringWToCStringA(dir_info[1]));              // dir_date
                if (dir_info.size() > 2) m_listCtrl.SetItemText(nIndex, 2, ConvertCStringWToCStringA(dir_info[2]));              // dir_size
                nDir++;
            }
        }
    }
    
}

BOOL CBrowseDirDlg::ReceiveBrowseDisk(std::vector<CStringA>& msgArr)
{
    if (!msgArr.empty())
        msgArr.clear();

    BOOL bRet = TRUE;
    int cbBytesRet;

    char buffer[1024] = "";
    cbBytesRet = ((CClientApp*)AfxGetApp())->m_ClientSocket.Receive(buffer, 1024);
    buffer[cbBytesRet] = '\0';

    CStringA strMsg(buffer);

    // test for errors and get out if they occurred
    if (cbBytesRet == SOCKET_ERROR || cbBytesRet == 0)
    {
        int iErr = ::GetLastError();
        TRACE("GetFileFromRemoteSite returned a socket error while getting file length\n"
            "\tNumber of Bytes received (zero means connection was closed) = %d\n"
            "\tGetLastError = %d\n", cbBytesRet, iErr);

        bRet = FALSE;
        return bRet;
    }

    msgArr.push_back(strMsg);

    //((CClientApp*)AfxGetApp())->m_ClientSocket.Close();
    return bRet;
}
BOOL CBrowseDirDlg::ReceiveBrowseDir(std::vector<CStringA>& msgArr)
{
    if (!msgArr.empty())
        msgArr.clear();

    BOOL bRet = TRUE;
    int cbBytesRet;
    char* buffer = new char[BUFFER_SIZE];

    int timeout = 100; // 1 second timeout
    ((CClientApp*)AfxGetApp())->m_ClientSocket.SetSockOpt(SO_RCVTIMEO, &timeout, sizeof(timeout));

    cbBytesRet = ((CClientApp*)AfxGetApp())->m_ClientSocket.Receive(buffer, (BUFFER_SIZE - 1));
    buffer[cbBytesRet] = '\0';

    CStringA strMsg(buffer);

    // test for errors and get out if they occurred
    if (cbBytesRet == 0 || cbBytesRet == SOCKET_ERROR || std::string(buffer) == "Invalid path!")
    {
        int iErr = ::GetLastError();
        TRACE("GetFileFromRemoteSite returned a socket error while getting file length\n"
            "\tNumber of Bytes received (zero means connection was closed) = %d\n"
            "\tGetLastError = %d\n", cbBytesRet, iErr);

        bRet = FALSE;
        delete[] buffer;
        return bRet;
    }

    msgArr.push_back(strMsg);

    delete[] buffer;
    return bRet;
}
