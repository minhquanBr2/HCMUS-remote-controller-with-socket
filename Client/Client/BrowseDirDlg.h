#pragma once


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
};
