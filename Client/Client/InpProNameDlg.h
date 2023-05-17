#pragma once
#include "afxdialogex.h"


// CInpProNameDlg dialog

class CInpProNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInpProNameDlg)

public:
	CInpProNameDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CInpProNameDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INP_PRO_NAME_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnEnChangeInpNamePro();

};