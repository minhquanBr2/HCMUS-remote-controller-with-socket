
// ClientView.cpp : implementation of the CClientView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Client.h"
#endif


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
		top += 30;
		bottom += 30;
	}

	if (!pDoc)
		return;

	CRect rectClient;
	GetClientRect(rectClient);
	pDC->FillSolidRect(rectClient, RGB(0, 65, 100));
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


void CClientView::AddMsg(CString strMessage)
{
	m_MsgArray.Add(strMessage);
	Invalidate();
}

int CClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//CFont font;
	//font.CreatePointFont(30, _T("Arial Black"));

	//LOGFONT lf;
	//// clear out structure
	//memset(&lf, 0, sizeof(LOGFONT));
	//// request a 12-pixel-height font
	//lf.lfHeight = 12;
	//// request a face name "Arial"
	//_tcsncpy_s(lf.lfFaceName, LF_FACESIZE, _T("Arial"), 50);

	//CFont font1;
	//font1.CreateFontIndirect(&lf);  // create the font

	m_button[0].Create("CONNECT", BS_DEFPUSHBUTTON | BS_MULTILINE,
		CRect(50, 50, 200, 150), this, IDC_BN_CONN); 
	m_button[1].Create("SHOW APPLICATIONS", BS_DEFPUSHBUTTON | BS_MULTILINE,
		CRect(250, 50, 400, 150), this, IDC_BN_SAPP);  
	m_button[2].Create("SHOW PROCESSES", BS_DEFPUSHBUTTON | BS_MULTILINE,
		CRect(450, 50, 600, 150), this, IDC_BN_SPRO);
	m_button[3].Create("CAPTURE SCREEN", BS_DEFPUSHBUTTON | BS_MULTILINE,
		CRect(50, 200, 200, 300), this, IDC_BN_CSCR);
	m_button[4].Create("KEYSTROKE", BS_DEFPUSHBUTTON | BS_MULTILINE,
		CRect(250, 200, 400, 300), this, IDC_BN_KSTR);
	m_button[5].Create("BROWSE DIRECTORY", BS_DEFPUSHBUTTON | BS_MULTILINE,
		CRect(450, 200, 600, 300), this, IDC_BN_BDIR);

	for (int i = 0; i < 6; i++) {
		m_button[i].ShowWindow(SW_SHOW);
	}
	return 0;
}

// extra functions
std::string getFirstWord(char* input) {
	// Find the length of the input string
	size_t length = strlen(input);

	// Skip leading whitespace characters
	size_t i = 0;
	while (i < length && input[i] == ' ')
		i++;

	// Find the end of the first word
	size_t wordEnd = i;
	while (wordEnd < length && input[wordEnd] != ' ')
		wordEnd++;

	// Extract the first word
	std::string firstWord(input + i, wordEnd - i);

	return firstWord;
}

void CClientView::OnButtonConnectClicked()
{
	if (((CClientApp*)AfxGetApp())->m_isConnected == FALSE)
	{
		m_dlgCONN.DoModal();
	}
	else
	{
		((CClientApp*)AfxGetApp())->m_isConnected = FALSE;
		this->m_button[CONN].SetWindowText("CONNECT");
		this->UpdateButtons();
		((CClientApp*)AfxGetApp())->m_ClientSocket.Close();
		//AfxMessageBox("Disconnected from the server!");
	}

	Invalidate();
	UpdateWindow();
}

void CClientView::OnButtonShowAppClicked()
{
	BOOL isConnected = FALSE;
	SOCKADDR_IN sockAddr;
	int len = sizeof(sockAddr);
	if (((CClientApp*)AfxGetApp())->m_ClientSocket.GetSockName((SOCKADDR*)&sockAddr, &len))
	{
		isConnected = TRUE;
	}

	if (!isConnected)
	{
		((CClientApp*)AfxGetApp())->m_ClientSocket.Create();
		CString IPAddress = ((CClientApp*)AfxGetApp())->m_ClientDlg.m_strIPAddress;
		UINT Port = ((CClientApp*)AfxGetApp())->m_ClientDlg.m_iPort;
		((CClientApp*)AfxGetApp())->m_ClientSocket.Connect(IPAddress, Port);
	}

	CString msg(std::string("REQ_SAPP").c_str());
	((CClientApp*)AfxGetApp())->m_ClientSocket.Send(msg.GetBuffer(msg.GetLength()), msg.GetLength());

	char buffer[50000] = "";
	int nBytesReceived = ((CClientApp*)AfxGetApp())->m_ClientSocket.Receive(buffer, 50000, 0);

	if (getFirstWord(buffer) == "Failed")
		return;

	m_strApp = std::string(buffer);

	if (nBytesReceived > 0) {
		m_dlgSAPP.DoModal();

		Invalidate();
		UpdateWindow();
	}
}

void CClientView::OnButtonShowProcessClicked()
{
	BOOL isConnected = FALSE;
	SOCKADDR_IN sockAddr;
	int len = sizeof(sockAddr);
	if (((CClientApp*)AfxGetApp())->m_ClientSocket.GetSockName((SOCKADDR*)&sockAddr, &len))
	{
		isConnected = TRUE;
	}

	if (!isConnected)
	{
		((CClientApp*)AfxGetApp())->m_ClientSocket.Create();
		CString IPAddress = ((CClientApp*)AfxGetApp())->m_ClientDlg.m_strIPAddress;
		UINT Port = ((CClientApp*)AfxGetApp())->m_ClientDlg.m_iPort;
		((CClientApp*)AfxGetApp())->m_ClientSocket.Connect(IPAddress, Port);
	}

	CString msg(std::string("REQ_SPRO").c_str());
	((CClientApp*)AfxGetApp())->m_ClientSocket.Send(msg.GetBuffer(msg.GetLength()), msg.GetLength());

	char buffer[50000] = "";
	int nBytesReceived = ((CClientApp*)AfxGetApp())->m_ClientSocket.Receive(buffer, 50000, 0);

	if (getFirstWord(buffer) == "Failed")
		return;

	m_strProcess = std::string(buffer);

	if (nBytesReceived > 0) {
		//MessageBox(m_strProcess.c_str(), "Process ne");
		m_dlgSPRO.DoModal();

		Invalidate();
		UpdateWindow();
	}
}

void CClientView::OnButtonCapScreenClicked()
{
	BOOL isConnected = FALSE;
	SOCKADDR_IN sockAddr;
	int len = sizeof(sockAddr);
	if (((CClientApp*)AfxGetApp())->m_ClientSocket.GetSockName((SOCKADDR*)&sockAddr, &len))
	{
		isConnected = TRUE;
	}

	if (!isConnected)
	{
		((CClientApp*)AfxGetApp())->m_ClientSocket.Create();
		CString IPAddress = ((CClientApp*)AfxGetApp())->m_ClientDlg.m_strIPAddress;
		UINT Port = ((CClientApp*)AfxGetApp())->m_ClientDlg.m_iPort;
		((CClientApp*)AfxGetApp())->m_ClientSocket.Connect(IPAddress, Port);
	}

	CString msg(std::string("REQ_CSCR").c_str());
	((CClientApp*)AfxGetApp())->m_ClientSocket.Send(msg.GetBuffer(msg.GetLength()), msg.GetLength());
	m_dlgCSCR.ReceiveFile();
	m_dlgCSCR.DoModal();

	Invalidate();
	UpdateWindow();
}

void CClientView::OnButtonKeystrokeClicked()
{
	BOOL isConnected = FALSE;
	SOCKADDR_IN sockAddr;
	int len = sizeof(sockAddr);
	if (((CClientApp*)AfxGetApp())->m_ClientSocket.GetSockName((SOCKADDR*)&sockAddr, &len))
	{
		isConnected = TRUE;
	}

	if (!isConnected)
	{
		((CClientApp*)AfxGetApp())->m_ClientSocket.Create();
		CString IPAddress = ((CClientApp*)AfxGetApp())->m_ClientDlg.m_strIPAddress;
		UINT Port = ((CClientApp*)AfxGetApp())->m_ClientDlg.m_iPort;
		((CClientApp*)AfxGetApp())->m_ClientSocket.Connect(IPAddress, Port);
	}
	
	int nErrorCode = 0;
	m_dlgKSTR.m_strAllKeystroke = "";
	m_dlgKSTR.DoModal();
	Invalidate();
	UpdateWindow();
}

void CClientView::OnButtonBrowseDirClicked()
{
	BOOL isConnected = FALSE;
	SOCKADDR_IN sockAddr;
	int len = sizeof(sockAddr);
	if (((CClientApp*)AfxGetApp())->m_ClientSocket.GetSockName((SOCKADDR*)&sockAddr, &len))
	{
		isConnected = TRUE;
	}

	if (!isConnected)
	{
		((CClientApp*)AfxGetApp())->m_ClientSocket.Create();
		CString IPAddress = ((CClientApp*)AfxGetApp())->m_ClientDlg.m_strIPAddress;
		UINT Port = ((CClientApp*)AfxGetApp())->m_ClientDlg.m_iPort;
		((CClientApp*)AfxGetApp())->m_ClientSocket.Connect(IPAddress, Port);
	}

	// create a UTF-8 encoded string
	const char* msg = "REQ_BDIR";

	// create a CString object from the UTF-8 encoded string
	CStringA strMsg(msg);
	((CClientApp*)AfxGetApp())->m_ClientSocket.Send(strMsg.GetBuffer(strMsg.GetLength()), strMsg.GetLength());
	m_dlgBDIR.ReceiveBrowseDisk(m_dlgBDIR.m_msgArr);
	m_dlgBDIR.DoModal();

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

// For Capture Screen (4)
