#pragma once
#include "afxdialogex.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "InpAppNameDlg.h"
#include <algorithm>

// CShowAppDlg dialog

class CShowAppDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShowAppDlg)

public:
	CShowAppDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CShowAppDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHOW_APP_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedKillBtn();
	afx_msg void OnBnClickedStartApp();
	afx_msg void OnLvnItemchangedListApp(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();

public:
	CListCtrl m_ListControl;
	CString cur_Name;

public:
	CInpAppNameDlg m_dlgINP;
};