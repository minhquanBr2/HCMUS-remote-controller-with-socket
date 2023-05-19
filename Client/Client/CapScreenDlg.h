#pragma once


// CCapScreenDlg dialog

class CCapScreenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCapScreenDlg)

public:
	CCapScreenDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCapScreenDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CSCR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_staticImg;
	CBitmap m_bitmap;
	BOOL ReceiveFile();
};
