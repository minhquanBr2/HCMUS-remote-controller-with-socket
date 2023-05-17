// InpProName.cpp : implementation file
//

#include "pch.h"
#include "Server.h"
#include "afxdialogex.h"
#include "InpProName.h"


// CInpProName dialog

IMPLEMENT_DYNAMIC(CInpProName, CDialogEx)

CInpProName::CInpProName(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CInpProName, pParent)
{

}

CInpProName::~CInpProName()
{
}

void CInpProName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInpProName, CDialogEx)
END_MESSAGE_MAP()


// CInpProName message handlers