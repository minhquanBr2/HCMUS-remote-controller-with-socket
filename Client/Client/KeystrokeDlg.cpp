// KeystrokeDlg.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "KeystrokeDlg.h"
#include "afxdialogex.h"



// CKeystrokeDlg dialog

IMPLEMENT_DYNAMIC(CKeystrokeDlg, CDialogEx)

CKeystrokeDlg::CKeystrokeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KSTR, pParent)
{
	m_strAllKeystroke = "";
}

CKeystrokeDlg::~CKeystrokeDlg()
{
}

void CKeystrokeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_KEYSTROKE, m_strDisplay);
}


BEGIN_MESSAGE_MAP(CKeystrokeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BN_KSTR_HOOK, &CKeystrokeDlg::OnBnClickedBnKstrHook)
	ON_BN_CLICKED(IDC_BN_KSTR_UNHK, &CKeystrokeDlg::OnBnClickedBnKstrUnhk)
	ON_EN_CHANGE(IDC_EDIT_KEYSTROKE, &CKeystrokeDlg::OnEnChangeEditKeystroke)
END_MESSAGE_MAP()


// CKeystrokeDlg message handlers



void CKeystrokeDlg::OnBnClickedBnKstrHook()
{
	CString msg(std::string("REQ_KSTR_HOOK").c_str());
	((CClientApp*)AfxGetApp())->m_ClientSocket.Send(msg.GetBuffer(msg.GetLength()), msg.GetLength());		
}


void CKeystrokeDlg::OnBnClickedBnKstrUnhk()
{
	CString msg(std::string("REQ_KSTR_UNHOOK").c_str());
	((CClientApp*)AfxGetApp())->m_ClientSocket.Send(msg.GetBuffer(msg.GetLength()), msg.GetLength());
}

BOOL CKeystrokeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_isHooked = TRUE;	

	// Get the current font object of the edit control
	CFont* pFont = m_strDisplay.GetFont();

	// Get the LOGFONT structure of the current font object
	LOGFONT lf;
	pFont->GetLogFont(&lf);

	// Modify the point size of the font
	lf.lfHeight = 16;

	// Create a new font object with the modified LOGFONT structure
	CFont font;
	font.CreateFontIndirect(&lf);

	// Set the new font for the edit control
	m_strDisplay.SetFont(&font);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CKeystrokeDlg::OnCancel()
{
	CString msg(std::string("REQ_KSTR_UNHOOK").c_str());
	((CClientApp*)AfxGetApp())->m_ClientSocket.Send(msg.GetBuffer(msg.GetLength()), msg.GetLength());
	m_isHooked = FALSE;
	CDialogEx::OnCancel();
}


void CKeystrokeDlg::OnOK()
{
	CString msg(std::string("REQ_KSTR_UNHOOK").c_str());
	((CClientApp*)AfxGetApp())->m_ClientSocket.Send(msg.GetBuffer(msg.GetLength()), msg.GetLength());
	m_isHooked = FALSE;
	CDialogEx::OnCancel();
}


void CKeystrokeDlg::OnEnChangeEditKeystroke()
{
	int numLines = m_strDisplay.GetLineCount();
	m_strDisplay.LineScroll(numLines);
}
