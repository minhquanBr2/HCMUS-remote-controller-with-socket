#pragma once
#include "afxdialogex.h"


// CInpAppNameDlg dialog

class CInpAppNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInpAppNameDlg)

public:
	CInpAppNameDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CInpAppNameDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INP_APP_NAME_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeInpNameApp();
	afx_msg void OnBnClickedBtnStart();
};