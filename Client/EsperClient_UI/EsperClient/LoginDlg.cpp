// LoginDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EsperClient.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "JoinDlg.h"
#include "Communication.h"
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
void LoginDlg::SetId(std::string param)
{
	m_userid = param;
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_OK, &LoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_REGIST, &LoginDlg::OnBnClickedRegist)
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
	UpdateData(TRUE);
	CString cstrId,cstrPw;
	std::string m_pw;
	GetDlgItemText(IDC_LOGID, cstrId);
	GetDlgItemText(IDC_LOGPW, cstrPw);
	CT2CA pszConvertedAnsiString(cstrId);
	CT2CA pszConvertedAnsiString2(cstrPw);
	m_userid = pszConvertedAnsiString;
	m_pw = pszConvertedAnsiString2;
	Items item;
	char* str;
	item.setId(m_userid);
	item.setPw(m_pw);

	//��� ��ǥ ����
	SOCKET s=socketCreate();	
	if(s == SOCKET_ERROR) AfxMessageBox(_T("socket error!"), MB_OK);
	/*
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(4500);
	addr.sin_addr.s_addr = inet_addr("165.132.144.106");
	//if (connect(s, (SOCKADDR*)&addr, sizeof(addr)) == -1) {
	//	AfxMessageBox(_T("connection(dir) error!"), MB_OK);
	//}*/ 

	if (sockSetting(s) == -1)
		AfxMessageBox(_T("connection error!"),MB_OK );
	else {
		socket_send(s, "signIn", item);
		resultpacketbuffer1 = "";
		resultpacketbuffer2 = "";
		resultpacketbuffer3 = "";
		socket_recv(s, &str);

		//AfxMessageBox((LPCTSTR)str.c_str(), MB_OK);
		AfxMessageBox(*str, MB_OK);
		AfxMessageBox((LPCTSTR)resultpacketbuffer1.c_str(), MB_OK);
		AfxMessageBox((LPCTSTR)resultpacketbuffer2.c_str(), MB_OK);

		if (resultpacketbuffer3 == "refusal")
			AfxMessageBox(TEXT("Access denied"), MB_OK );
		else {
			CEsperClientDlg dlg;
			dlg.SetId(m_userid);
			ShowWindow(SW_HIDE);
			dlg.DoModal();
		}
	}
	closesocket(s);
	WSACleanup();

}


void LoginDlg::OnBnClickedRegist()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CJoinDlg dlg;
	dlg.DoModal();
}
