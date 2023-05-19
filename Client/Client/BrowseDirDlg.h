#pragma once
#include <vector>
#include <string>

typedef std::tuple<CString, CString> tuple2str;
typedef std::tuple<CString, CString, CString> tuple3str;


// CBrowseDirDlg dialog

class CBrowseDirDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBrowseDirDlg)

public:
	CBrowseDirDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CBrowseDirDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BDIR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listCtrl;
	afx_msg void OnListCtrlClick(NMHDR* pNMHDR, LRESULT* pResult);
	CString m_strPrevPathMB = "";
	CString m_strPathMB;
	CEdit m_strDisplay;
	std::vector<CStringA> m_msgArr;
	afx_msg void OnBnClickedBtnBack();
	BOOL ReceiveBrowseDisk(std::vector<CStringA>& msgArr);
	BOOL ReceiveBrowseDir(std::vector<CStringA>& msgArr);
};
