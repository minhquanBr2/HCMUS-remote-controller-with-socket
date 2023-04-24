
// ClientView.cpp : implementation of the CClientView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Client.h"
#endif

#include "ClientDoc.h"
#include "ClientView.h"
#include "ClientDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientView

IMPLEMENT_DYNCREATE(CClientView, CView)

BEGIN_MESSAGE_MAP(CClientView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CClientView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BN_CONN, OnButtonConnectClicked)
	ON_BN_CLICKED(IDC_BN_SAPP, OnButtonShowAppClicked)
	ON_BN_CLICKED(IDC_BN_SPRO, OnButtonShowProcessClicked)
	ON_BN_CLICKED(IDC_BN_CSCR, OnButtonCapScreenClicked)
	ON_BN_CLICKED(IDC_BN_KSTR, OnButtonKeystrokeClicked)
	ON_BN_CLICKED(IDC_BN_BDIR, OnButtonBrowseDirClicked)
END_MESSAGE_MAP()

// CClientView construction/destruction

CClientView::CClientView() noexcept
{
	// TODO: add construction code here
	((CClientApp*)AfxGetApp())->m_pClientView = this;
}

CClientView::~CClientView()
{
}

BOOL CClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CClientView drawing

void CClientView::OnDraw(CDC* pDC)
{
	CClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// Displaying the messages received from the client
	int left = 100, top = 100, right = 1000, bottom = 5000;
	for (int nIndex(0); nIndex < m_MsgArray.GetSize(); nIndex++)
	{
		CRect rect(left, top, right, bottom); // specify the bounding rectangle
		pDC->DrawText(m_MsgArray.GetAt(nIndex), &rect, DT_WORDBREAK | DT_LEFT); // draw the text with word wrapping and left alignment
		top += 300;
		bottom += 300;
	}

	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CClientView printing


void CClientView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CClientView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CClientView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CClientView diagnostics

#ifdef _DEBUG
void CClientView::AssertValid() const
{
	CView::AssertValid();
}

void CClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CClientDoc* CClientView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CClientDoc)));
	return (CClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CClientView message handlers


//void CClientView::OnConnectConnectToServer()
//{
//	// TODO: Add your command handler code here
//	CClientDlg dlgConnect;
//	dlgConnect.DoModal();
//
//}

void CClientView::AddMsg(CString strMessage)
{
	// TODO: Add your implementation code here.
	m_MsgArray.Add(strMessage);
	Invalidate();
}



int CClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_button[0].Create("CONNECT", BS_PUSHBUTTON,
		CRect(300, 100, 580, 150), this, IDC_BN_CONN);
	m_button[1].Create("SHOW APPLICATIONS", BS_PUSHBUTTON,
		CRect(600, 100, 880, 150), this, IDC_BN_SAPP);
	m_button[2].Create("SHOW PROCESSES", BS_PUSHBUTTON,
		CRect(900, 100, 1180, 150), this, IDC_BN_SPRO);
	m_button[3].Create("CAPTURE SCREEN", BS_PUSHBUTTON,
		CRect(300, 300, 580, 350), this, IDC_BN_CSCR);
	m_button[4].Create("KEYSTROKE", BS_PUSHBUTTON,
		CRect(600, 300, 880, 350), this, IDC_BN_KSTR);
	m_button[5].Create("BROWSE DIRECTORY", BS_PUSHBUTTON,
		CRect(900, 300, 1180, 350), this, IDC_BN_BDIR);

	for (int i = 0; i < 6; i++)
		m_button[i].ShowWindow(SW_SHOW);
	return 0;
}


void CClientView::OnButtonConnectClicked()
{
	//AfxMessageBox("Connect.");
	if (((CClientApp*)AfxGetApp())->m_isConnected == FALSE)
	{
		CClientDlg dlgConnect;
		//CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		dlgConnect.DoModal();
	}
	else
	{
		((CClientApp*)AfxGetApp())->m_isConnected = FALSE;
		this->m_button[CONN].SetWindowText("CONNECT");
		this->UpdateButtons();
		((CClientApp*)AfxGetApp())->m_ClientSocket.Close();
		AfxMessageBox("Disconnected from the server!");
	}

		

	Invalidate();
	UpdateWindow();
}

void CClientView::OnButtonShowAppClicked()
{
	if (((CClientApp*)AfxGetApp())->m_isConnected == FALSE)
	{
		AfxMessageBox("Client hasn't been connected to server!");
	}
	else
	{

	}
	Invalidate();
	UpdateWindow();
}

void CClientView::OnButtonShowProcessClicked()
{
	if (((CClientApp*)AfxGetApp())->m_isConnected == FALSE)
	{
		AfxMessageBox("Client hasn't been connected to server!");
	}
	else
	{

	}
	Invalidate();
	UpdateWindow();
}

void CClientView::OnButtonCapScreenClicked()
{
	if (((CClientApp*)AfxGetApp())->m_isConnected == FALSE)
	{
		AfxMessageBox("Client hasn't been connected to server!");
	}
	else
	{

	}
	Invalidate();
	UpdateWindow();
}

void CClientView::OnButtonKeystrokeClicked()
{
	if (((CClientApp*)AfxGetApp())->m_isConnected == FALSE)
	{
		AfxMessageBox("Client hasn't been connected to server!");
	}
	else
	{

	}
	Invalidate();
	UpdateWindow();
}

void CClientView::OnButtonBrowseDirClicked()
{
	if (((CClientApp*)AfxGetApp())->m_isConnected == FALSE)
	{
		AfxMessageBox("Client hasn't been connected to server!");
	}
	else
	{

	}
	Invalidate();
	UpdateWindow();
}

void CClientView::InitButtons()
{
	for (int i = SAPP; i <= BDIR; i++)
		m_button[i].EnableWindow(FALSE);
}

void CClientView::UpdateButtons()
{
	if (((CClientApp*)AfxGetApp())->m_isConnected == TRUE)
		for (int i = SAPP; i <= BDIR; i++)
			m_button[i].EnableWindow(TRUE);
	else
		for (int i = SAPP; i <= BDIR; i++)
			m_button[i].EnableWindow(FALSE);
}