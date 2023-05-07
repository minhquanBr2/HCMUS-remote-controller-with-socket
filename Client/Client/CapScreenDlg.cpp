// CCapScreenDlg.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "CapScreenDlg.h"
#include "afxdialogex.h"
#include <afxwin.h>
#include <afxcmn.h>

#include <Windows.h>
#include <string>
#include <atlimage.h>  // for CImage
#include <gdiplus.h>   // for Gdiplus functions


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

    // Load the image from a file
    CString imagePath = _T("screenshot.bmp");  // replace with your image file path
    CImage m_image;
    m_image.Load(imagePath);

    // Get the dimensions of the picture control box
    CRect rect;
    GetDlgItem(IDC_IMAGE)->GetWindowRect(&rect);
    ScreenToClient(&rect);

    // Scale the image to fit the picture control box
    CImage scaledImage;
    double scaleX = (double)rect.Width() / (double)m_image.GetWidth();
    double scaleY = (double)rect.Height() / (double)m_image.GetHeight();
    double scale = min(scaleX, scaleY);
    int newWidth = (int)(scale * m_image.GetWidth());
    int newHeight = (int)(scale * m_image.GetHeight());
    scaledImage.Create(newWidth, newHeight, m_image.GetBPP());

    // Draw the original image onto the scaled image using GDI functions
    HDC hDC = scaledImage.GetDC();
    HDC hDCOrig = m_image.GetDC();
    SetStretchBltMode(hDC, HALFTONE);
    StretchBlt(hDC, 0, 0, newWidth, newHeight, hDCOrig, 0, 0, m_image.GetWidth(), m_image.GetHeight(), SRCCOPY);
    scaledImage.ReleaseDC();
    m_image.ReleaseDC();

    //// Create a Gdiplus::Bitmap from the scaled image
    //HBITMAP hBitmap = scaledImage.Detach();
    //Gdiplus::Bitmap bitmap(hBitmap, NULL);

    // Set the image in the picture control
    CStatic* pPictureCtrl = (CStatic*)GetDlgItem(IDC_IMAGE);
    pPictureCtrl->ModifyStyle(0, SS_BITMAP);  // set the picture control to display a bitmap
    pPictureCtrl->SetBitmap(scaledImage.Detach());  // detach the scaled image to set it as the bitmap

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
