// LoginDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EsperClient.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "JoinDlg.h"
#include "Communication.h"
#include "EsperClientDlg.h"

// LoginDlg 대화 상자입니다.

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


// LoginDlg 메시지 처리기입니다.


BOOL LoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(_T("Login"));
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

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
		CA2W("굴림")); // lpszFacename 
	GetDlgItem(IDC_STATIC_LOGIN)->SetFont(&m_font);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}




void LoginDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	//통신 목표 설정
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CJoinDlg dlg;
	dlg.DoModal();
}
