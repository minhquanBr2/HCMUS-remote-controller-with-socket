// CCapScreenDlg.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "CapScreenDlg.h"
#include "afxdialogex.h"
#include <afxwin.h>
#include <afxcmn.h>

#include <string>


// CCapScreenDlg dialog

IMPLEMENT_DYNAMIC(CCapScreenDlg, CDialogEx)

CCapScreenDlg::CCapScreenDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CSCR, pParent)
{

}

CCapScreenDlg::~CCapScreenDlg()
{
}

void CCapScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE, m_staticImg);
}


BEGIN_MESSAGE_MAP(CCapScreenDlg, CDialogEx)
	ON_STN_CLICKED(IDC_IMAGE, &CCapScreenDlg::OnStnClickedImage)
	ON_BN_CLICKED(IDOK, &CCapScreenDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCapScreenDlg message handlers


BOOL CCapScreenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	// Load the bitmap image into the CBitmap object

	// Lưu ý:
	// 1. m_bitmap phải được tạo trước, là member variable của lớp CCapScreenDlg
	// 2. đường dẫn có thể là tương đối hoặc tuyệt đối
	// 3. đường dẫn dùng / hay \\ đều được
	// 4. đường dẫn có thể dùng _T() để chuyển kiểu dữ liệu hoặc không
	CImage img;
	img.Load("screenshot.bmp");
	m_bitmap.Attach(img.Detach());

	// Set the bitmap image in the image control
	CStatic* pImageControl = (CStatic*)GetDlgItem(IDC_IMAGE);
	pImageControl->SetBitmap(m_bitmap);

	return TRUE;
}


void CCapScreenDlg::OnStnClickedImage()
{
	// TODO: Add your control notification handler code here
}


void CCapScreenDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
