
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

#define BUFFER_SIZE 65536


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
		top += 30;
		bottom += 30;
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
	if (((CClientApp*)AfxGetApp())->m_isConnected == FALSE)
	{
		CClientDlg dlgConnect;
		dlgConnect.DoModal();
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
	if (((CClientApp*)AfxGetApp())->m_isConnected == FALSE) {
		AfxMessageBox("Client hasn't been connected to server!");
	}
	else {
		CString msg(std::string("REQ_SPRO").c_str());
		((CClientApp*)AfxGetApp())->m_ClientSocket.Send(msg.GetBuffer(msg.GetLength()), msg.GetLength());

		char buffer[100000] = "";
		int nBytesReceived = ((CClientApp*)AfxGetApp())->m_ClientSocket.Receive(buffer, 100000, 0);

		m_strProcess = std::string(buffer);

		if (nBytesReceived > 0) {
			//MessageBox(m_strProcess.c_str(), "Process ne");
			m_dlgSPRO.DoModal();
		}
		else {
			MessageBox(std::to_string(nBytesReceived).c_str(), "ERROR");
		}
	}

	Invalidate();
	UpdateWindow();
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
	this->ReceiveFile();
	this->ShowImageDialog();

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
		CString msg(std::string("REQ_KSTR").c_str());
		((CClientApp*)AfxGetApp())->m_ClientSocket.Send(msg.GetBuffer(msg.GetLength()), msg.GetLength());

		char strRec[256] = "";
		int bytesRead;
		int nErrorCode = 0;

		m_dlgKSTR.m_strAllKeystroke = "";
		m_dlgKSTR.DoModal();
		
		// Receive data from server and store it in a buffer
		//while (bytesRead = ((CClientApp*)AfxGetApp())->m_ClientSocket.Receive(strRec, 256))
		//{
		//	// Print data to the edit control
		//	CString str(strRec, bytesRead);
		//	AfxMessageBox(str);								// it didnt't work
		//	//dlg.m_strKeystroke.ReplaceSel(str + "\r\n");
		//	
		//	// Continue listening for incoming data
		//	((CClientApp*)AfxGetApp())->m_ClientSocket.AsyncSelect(FD_READ);

		//}

		//((CClientApp*)AfxGetApp())->m_ClientSocket.OnReceive(nErrorCode);

		//	// Print data to the edit control
		//	AfxMessageBox("view");								// it didnt't work
		//	//dlg.m_strKeystroke.ReplaceSel(str + "\r\n");

		//	// Continue listening for incoming data
		//	((CClientApp*)AfxGetApp())->m_ClientSocket.AsyncSelect(FD_READ);
	}
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
	this->ReceiveBrowseDisk(m_dlgBDIR.m_msgArr);
	
	
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
BOOL CClientView::ReceiveFile()
{
	// local variables used in file transfer (declared here to avoid
	// "goto skips definition"-style compiler errors)

	BOOL bRet = TRUE; // return value
	// used to monitor the progress of a receive operation
	int dataLength, cbBytesRet, cbLeftToReceive;
	// pointer to buffer for receiving data
	// (memory is allocated after obtaining file size)
	BYTE* recdData = NULL;

	CFile destFile;
	CFileException fe;
	BOOL bFileIsOpen = FALSE;

	// open/create target file that receives the transferred data

	if (!(bFileIsOpen = destFile.Open("screenshot.bmp", CFile::modeCreate |
		CFile::modeWrite | CFile::typeBinary, &fe)))
	{
		TCHAR strCause[256];
		fe.GetErrorMessage(strCause, 255);
		TRACE("GetFileFromRemoteSender encountered an error while opening the local file\n"
			"\tFile name = %s\n\tCause = %s\n\tm_cause = %d\n\tm_IOsError = %d\n",
			fe.m_strFileName, strCause, fe.m_cause, fe.m_lOsError);

		/* you should handle the error here */

		bRet = FALSE;
		goto PreReturnCleanup;
	}


	// get the file's size first
	cbLeftToReceive = sizeof(dataLength);

	do
	{
		BYTE* bp = (BYTE*)(&dataLength) + sizeof(dataLength) - cbLeftToReceive;
		cbBytesRet = ((CClientApp*)AfxGetApp())->m_ClientSocket.Receive(bp, cbLeftToReceive);

		// test for errors and get out if they occurred
		if (cbBytesRet == SOCKET_ERROR || cbBytesRet == 0)
		{
			int iErr = ::GetLastError();
			TRACE("GetFileFromRemoteSite returned a socket error while getting file length\n"
				"\tNumber of Bytes received (zero means connection was closed) = %d\n"
				"\tGetLastError = %d\n", cbBytesRet, iErr);

			/* you should handle the error here */

			bRet = FALSE;
			goto PreReturnCleanup;
		}

		// good data was retrieved, so accumulate
		// it with already-received data
		cbLeftToReceive -= cbBytesRet;

	} while (cbLeftToReceive > 0);

	dataLength = ntohl(dataLength);

	// now get the file in RECV_BUFFER_SIZE chunks at a time

	recdData = new byte[RECV_BUFFER_SIZE];
	cbLeftToReceive = dataLength;

	do
	{
		int iiGet, iiRecd;

		iiGet = (cbLeftToReceive < RECV_BUFFER_SIZE) ?
			cbLeftToReceive : RECV_BUFFER_SIZE;
		iiRecd = ((CClientApp*)AfxGetApp())->m_ClientSocket.Receive(recdData, iiGet);
		AfxMessageBox(std::to_string(iiRecd).c_str());

		// test for errors and get out if they occurred
		if (iiRecd == SOCKET_ERROR || iiRecd == 0)
		{
			int iErr = ::GetLastError();
			TRACE("GetFileFromRemoteSite returned a socket error while getting chunked file data\n"
				"\tNumber of Bytes received (zero means connection was closed) = %d\n"
				"\tGetLastError = %d\n", iiRecd, iErr);

			/* you should handle the error here */

			bRet = FALSE;
			goto PreReturnCleanup;
		}

		// good data was retrieved, so accumulate
		// it with already-received data

		destFile.Write(recdData, iiRecd); // Write it
		cbLeftToReceive -= iiRecd;

	} while (cbLeftToReceive > 0);

PreReturnCleanup: // labelled "goto" destination

	// free allocated memory
	// if we got here from a goto that skipped allocation,
	// delete of NULL pointer
	// is permissible under C++ standard and is harmless
	delete[] recdData;

	if (bFileIsOpen)
		destFile.Close();
	// only close file if it's open (open might have failed above)

	((CClientApp*)AfxGetApp())->m_ClientSocket.Close();

	return bRet;
}
void CClientView::ShowImageDialog()
{
	m_dlgCSCR.DoModal();
}

// For Browse Directory (6)
BOOL CClientView::ReceiveBrowseDisk(std::vector<CStringW>& msgArr)
{
	if (!msgArr.empty())
		msgArr.clear();

	BOOL bRet = TRUE; 
	int cbBytesRet;

	char buffer[1024] = "";
	cbBytesRet = ((CClientApp*)AfxGetApp())->m_ClientSocket.Receive(buffer, 1024);

	// Convert the received data to a Unicode string
	CStringA strMsg(buffer);
	int len = MultiByteToWideChar(CP_UTF8, 0, strMsg, -1, NULL, 0);
	CStringW wstrMsg;
	int wideLen = MultiByteToWideChar(CP_UTF8, 0, strMsg, -1, wstrMsg.GetBuffer(len), len);
	wstrMsg.ReleaseBuffer(wideLen); 

	// test for errors and get out if they occurred
	if (cbBytesRet == SOCKET_ERROR || cbBytesRet == 0)
	{
		int iErr = ::GetLastError();
		TRACE("GetFileFromRemoteSite returned a socket error while getting file length\n"
			"\tNumber of Bytes received (zero means connection was closed) = %d\n"
			"\tGetLastError = %d\n", cbBytesRet, iErr);

		bRet = FALSE;
		return bRet;
	}

	msgArr.push_back(wstrMsg);

	//((CClientApp*)AfxGetApp())->m_ClientSocket.Close();
	return bRet;
}
BOOL CClientView::ReceiveBrowseDir(std::vector<CStringW>& msgArr)
{
	if (!msgArr.empty())
		msgArr.clear();

	BOOL bRet = TRUE;
	int cbBytesRet;
	wchar_t* buffer = new wchar_t[BUFFER_SIZE];

	int timeout = 100; // 1 second timeout
	((CClientApp*)AfxGetApp())->m_ClientSocket.SetSockOpt(SO_RCVTIMEO, &timeout, sizeof(timeout));

	cbBytesRet = ((CClientApp*)AfxGetApp())->m_ClientSocket.Receive(buffer, (BUFFER_SIZE - 1) * sizeof(wchar_t));
	buffer[cbBytesRet] = '\0';

	// test for errors and get out if they occurred
	if (cbBytesRet == 0 || cbBytesRet == SOCKET_ERROR || std::wstring(buffer) == L"Invalid path!")
	{
		int iErr = ::GetLastError();
		TRACE("GetFileFromRemoteSite returned a socket error while getting file length\n"
			"\tNumber of Bytes received (zero means connection was closed) = %d\n"
			"\tGetLastError = %d\n", cbBytesRet, iErr);

		bRet = FALSE;
		delete[] buffer;
		AfxMessageBox(std::to_string(cbBytesRet).c_str());
		return bRet;
	}

	AfxMessageBox(std::to_string(cbBytesRet).c_str());
	msgArr.push_back(std::wstring(buffer).c_str());
	
	delete[] buffer;
	return bRet;
}