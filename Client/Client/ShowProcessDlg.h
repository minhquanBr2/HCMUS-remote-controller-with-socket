#pragma once
#include "afxdialogex.h"
#include <string>
#include <vector>
#include <iostream>
#include <vector>
#include <utility>
#include "InpProNameDlg.h"
#include <algorithm>

// CShowProcessDlg dialog

class CShowProcessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShowProcessDlg)

public:
	CShowProcessDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CShowProcessDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPRO1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedKillBtn();
	afx_msg void OnBnClickedStartPro();
	afx_msg void OnLvnItemchangedListPro(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	CListCtrl m_listCtrl;
	CString cur_PID;
	CInpProNameDlg m_dlgINP;
	virtual void OnOK();
	virtual void OnCancel();
};