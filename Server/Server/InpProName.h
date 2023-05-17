#pragma once
#include "afxdialogex.h"

// CInpProName dialog

class CInpProName : public CDialogEx
{
	DECLARE_DYNAMIC(CInpProName)

public:
	CInpProName(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CInpProName();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CInpProName };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};