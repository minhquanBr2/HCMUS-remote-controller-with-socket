#pragma once


// CKeystrokeDlg dialog

class CKeystrokeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKeystrokeDlg)

public:
	CKeystrokeDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CKeystrokeDlg();
public:
	BOOL m_isHooked = FALSE;
	CEdit m_strDisplay;
	CString m_strAllKeystroke;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KSTR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	//CClientApp* m_pMainWnd;
	afx_msg void OnBnClickedBnKstrHook();
	afx_msg void OnBnClickedBnKstrUnhk();
	afx_msg void OnEnChangeEditKeystroke();
	virtual BOOL OnInitDialog();
};
