// CCapScreenDlg.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "CapScreenDlg.h"
#include "afxdialogex.h"

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
}


BEGIN_MESSAGE_MAP(CCapScreenDlg, CDialogEx)
END_MESSAGE_MAP()


// CCapScreenDlg message handlers


BOOL CCapScreenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	// get client rectangle for picture box
	CStatic* pictureBox = (CStatic*)(GetDlgItem(IDC_IMAGE));
	CRect rect;
	pictureBox->GetClientRect(rect);

	// create CClientDC for picture box
	CClientDC dc(pictureBox);

	// load image to CImage class
	CImage image;
	image.Load("772.bmp");
	AfxMessageBox(std::to_string(image.GetHeight()).c_str());

	// load image to CBitmap class
	CBitmap bitmap;
	bitmap.Attach(image.Detach());

	// create CDC to load bitmap
	CDC memoryDC;
	memoryDC.CreateCompatibleDC(&dc);

	// add bitmap to memoryDC object
	memoryDC.SelectObject(&bitmap);

	// get bitmap dimension
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);

	// set stretch build mode to color on color
	dc.SetStretchBltMode(COLORONCOLOR);
	dc.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memoryDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);




	//CBitmap m_image; // needed to get height/width
	//HBITMAP hBmp; // handle to your bitmap
	//int height; // height of the bitmap
	//int width; // width of the bitmap

	//hBmp = (HBITMAP) ::LoadImage(
	//	AfxGetInstanceHandle(),
	//	"772.bmp", IMAGE_BITMAP, 0, 0,
	//	LR_LOADFROMFILE |
	//	LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	//// =============================================
	//if (hBmp)
	//	m_image.Attach(hBmp);

	//BITMAP bm;
	//m_image.GetBitmap(&bm);
	//height = bm.bmHeight;
	//width = bm.bmWidth;

	//CStatic* pictureBox = (CStatic*)(GetDlgItem(IDC_IMAGE));
	//pictureBox->SetBitmap(hBmp); // to display the
	//// bitmap
	return TRUE;
}
