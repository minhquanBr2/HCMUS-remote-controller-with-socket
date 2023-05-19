
// ClientView.h : interface of the CClientView class
//

#pragma once

#define CONN 0
#define SAPP 1
#define SPRO 2
#define CSCR 3
#define KSTR 4
#define BDIR 5

#define PRE_AGREED_PORT     8686
#define RECV_BUFFER_SIZE    4096

#include "ClientDoc.h"
#include "ClientView.h"
#include "MainFrm.h"

#include "ClientDlg.h"
#include "CapScreenDlg.h"
#include "KeystrokeDlg.h"
#include "BrowseDirDlg.h"
#include "ShowProcessDlg.h"
#include "ShowAppDlg.h"

#include <string>
#include <vector>



class CClientView : public CView
{
protected: // create from serialization only
	CClientView() noexcept;
	DECLARE_DYNCREATE(CClientView)
	CStringArray m_MsgArray;

// Attributes
public:
	CButton m_button[6];
	CClientDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonConnectClicked();
	afx_msg void OnButtonShowAppClicked();
	afx_msg void OnButtonShowProcessClicked();
	afx_msg void OnButtonCapScreenClicked();
	afx_msg void OnButtonKeystrokeClicked();
	afx_msg void OnButtonBrowseDirClicked();
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnConnectConnectToServer();
	void AddMsg(CString strMessage);
	void InitButtons();
	void UpdateButtons();	
public:
	// for Connect (0)
	CClientDlg m_dlgCONN;
	// for Show App (1)
	CShowAppDlg m_dlgSAPP;
	std::string m_strApp;
	// for Show Process (2)
	CShowProcessDlg m_dlgSPRO;
	std::string m_strProcess;
	// for Capture Screen (3)
	CCapScreenDlg m_dlgCSCR;
	BOOL ReceiveFile();
	void ShowImageDialog();
	// for Keystroke (4)
	CKeystrokeDlg m_dlgKSTR;
	// for Browse Directory (5)
	CBrowseDirDlg m_dlgBDIR;
};

#ifndef _DEBUG  // debug version in ClientView.cpp
inline CClientDoc* CClientView::GetDocument() const
   { return reinterpret_cast<CClientDoc*>(m_pDocument); }
#endif

