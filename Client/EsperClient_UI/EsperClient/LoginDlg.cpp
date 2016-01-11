// LoginDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EsperClient.h"
#include "LoginDlg.h"
#include "afxdialogex.h"

#include "EsperClientDlg.h"


// LoginDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(LoginDlg, CDialogEx)

CFont m_font;

LoginDlg::LoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOGIN, pParent)
{

}

LoginDlg::~LoginDlg()
{

}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_OK, &LoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// LoginDlg �޽��� ó�����Դϴ�.


BOOL LoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(_T("Login"));
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_font.CreateFont(25, // nHeight 
		12, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		10, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		DEFAULT_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		CA2W("����")); // lpszFacename 
	GetDlgItem(IDC_STATIC_LOGIN)->SetFont(&m_font);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}




void LoginDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CEsperClientDlg dlg;
	ShowWindow(SW_HIDE);
	dlg.DoModal();
}
