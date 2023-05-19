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

BOOL CCapScreenDlg::ReceiveFile()
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


