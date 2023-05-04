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
	// TODO: Add your control notification handler code here
	if (m_isHooked == FALSE)
	{
		m_isHooked = TRUE;
	}
		
}


void CKeystrokeDlg::OnBnClickedBnKstrUnhk()
{
	// TODO: Add your control notification handler code here
	if (m_isHooked == TRUE)
	{
		m_isHooked = FALSE;
		//m_strAllKeystroke = "";
	}
		
}



void CKeystrokeDlg::OnEnChangeEditKeystroke()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


BOOL CKeystrokeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	// Get a pointer to the edit control
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_KEYSTROKE);

	// Set the edit control to be a multiline edit control
	pEdit->ModifyStyle(0, ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_MULTILINE);

	// Select the entire text in the edit control
	pEdit->SetSel(0, -1);

	// Scroll the text into view
	pEdit->LineScroll(pEdit->GetFirstVisibleLine());

	// Get the current font object of the edit control
	CFont* pFont = pEdit->GetFont();

	// Get the LOGFONT structure of the current font object
	LOGFONT lf;
	pFont->GetLogFont(&lf);

	// Modify the point size of the font
	lf.lfHeight = 16;

	// Create a new font object with the modified LOGFONT structure
	CFont font;
	font.CreateFontIndirect(&lf);

	// Set the new font for the edit control
	pEdit->SetFont(&font);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
