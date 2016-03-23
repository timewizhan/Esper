// JoinDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EsperClient.h"
#include "JoinDlg.h"
#include "afxdialogex.h"
#include "Communication.h"


// CJoinDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CJoinDlg, CDialogEx)

CJoinDlg::CJoinDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_JOIN, pParent),m_idCheck(false),m_emailCheck(false),m_authcodecheck(false)
{
	
}

CJoinDlg::~CJoinDlg()
{

}

void CJoinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CJoinDlg, CDialogEx)
	ON_BN_CLICKED(IDC_JOIN_BTT_IDCHECK, &CJoinDlg::OnBnClickedJoinBttIdcheck)
	ON_BN_CLICKED(IDC_JOIN_BTT_EMAILCHECK, &CJoinDlg::OnBnClickedJoinBttEmailcheck)
	ON_BN_CLICKED(IDC_JOIN_BTT_EMAILAUTH, &CJoinDlg::OnBnClickedJoinBttEmailauth)
	ON_BN_CLICKED(IDC_JOIN_BTT_EMAILAUTHCHECK, &CJoinDlg::OnBnClickedJoinBttEmailauthcheck)
	ON_BN_CLICKED(ID_JOIN_OK, &CJoinDlg::OnBnClickedJoinOk)
	ON_BN_CLICKED(ID_JOIN_CANCLE, &CJoinDlg::OnBnClickedJoinCancle)
END_MESSAGE_MAP()


// CJoinDlg 메시지 처리기입니다.


void CJoinDlg::OnBnClickedJoinBttIdcheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strId;
	GetDlgItemTextW(IDC_JOIN_EDIT_ID,strId);
	
	// ID Check 통신

	Items item;
	CT2CA converter(strId);
	item.Id = converter;
	char* str;

	SOCKET s = socketCreate();
	if (s == SOCKET_ERROR) AfxMessageBox(_T("socket error!"), MB_OK);
	/*
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(4500);
	addr.sin_addr.s_addr = inet_addr("165.132.144.106");
	//if (connect(s, (SOCKADDR*)&addr, sizeof(addr)) == -1) {
	//	AfxMessageBox(_T("connection(dir) error!"), MB_OK);
	//}*/
	
	if (sockSetting(s) == -1)
		AfxMessageBox(_T("connection error!"), MB_OK);
	
	else {
		socket_send(s, "checkID", item);
		socket_recv(s, &str);

		//AfxMessageBox((LPCTSTR)str.c_str(), MB_OK);
		AfxMessageBox(*str, MB_OK);
		AfxMessageBox((LPCTSTR)resultpacketbuffer1.c_str(), MB_OK);
		AfxMessageBox((LPCTSTR)resultpacketbuffer2.c_str(), MB_OK);

		if (resultpacketbuffer2 == "refusal") {
			m_idCheck = false;
			AfxMessageBox(TEXT("중복된 ID 입니다."), MB_OK);
		}
			
		else {
			AfxMessageBox(_T("사용 가능한 ID 입니다."), MB_OK);
			m_idCheck = true;
		}
	}
	closesocket(s);
	WSACleanup();
	// if 가능하면
	


}


void CJoinDlg::OnBnClickedJoinBttEmailcheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strEmail;
	GetDlgItemTextW(IDC_JOIN_EDIT_EMAIL, strEmail);
	// Email check 통신
	Items item;
	CT2CA converter(strEmail);
	item.Email = converter;
	char* str;

	SOCKET s = socketCreate();
	if (s == SOCKET_ERROR) AfxMessageBox(_T("socket error!"), MB_OK);
	/*
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(4500);
	addr.sin_addr.s_addr = inet_addr("165.132.144.106");
	//if (connect(s, (SOCKADDR*)&addr, sizeof(addr)) == -1) {
	//	AfxMessageBox(_T("connection(dir) error!"), MB_OK);
	//}*/

	if (sockSetting(s) == -1)
		AfxMessageBox(_T("connection error!"), MB_OK);

	else {
		socket_send(s, "checkID", item);
		socket_recv(s, &str);
		AfxMessageBox(*str, MB_OK);
		AfxMessageBox((LPCTSTR)resultpacketbuffer1.c_str(), MB_OK);
		AfxMessageBox((LPCTSTR)resultpacketbuffer2.c_str(), MB_OK);
		if (resultpacketbuffer2 == "refusal") {
			AfxMessageBox(_T("중복된 Email 입니다."), MB_OK);
			m_emailCheck = false;
		}
		else {
			AfxMessageBox(_T("사용 가능한 Email 입니다."), MB_OK);
			m_emailCheck = true;
		}
	}		

}


void CJoinDlg::OnBnClickedJoinBttEmailauth()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_emailCheck)
		AfxMessageBox(_T("Email 중복을 확인해 주세요."), MB_OK);
	else {
		SOCKET s = socketCreate();
		if (s == SOCKET_ERROR) AfxMessageBox(_T("socket error!"), MB_OK);
		/*
		SOCKADDR_IN addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(4500);
		addr.sin_addr.s_addr = inet_addr("165.132.144.106");
		//if (connect(s, (SOCKADDR*)&addr, sizeof(addr)) == -1) {
		//	AfxMessageBox(_T("connection(dir) error!"), MB_OK);
		//}*/


		if (sockSetting(s) == -1)
			AfxMessageBox(_T("connection error!"), MB_OK);

		else {

			//인증 메일 발송 통신
			Items item;
			char* str;
			socket_send(s, "checkID", item);
			socket_recv(s, &str);
			AfxMessageBox(*str, MB_OK);
			AfxMessageBox((LPCTSTR)resultpacketbuffer1.c_str(), MB_OK);
			AfxMessageBox((LPCTSTR)resultpacketbuffer2.c_str(), MB_OK);
			if(resultpacketbuffer2 == "refusal")
				AfxMessageBox(_T("인증 메일 전송에 실패했습니다."), MB_OK);
			else {
				AfxMessageBox(_T("인증 메일이 전송되었습니다."), MB_OK);
			}

		}
	}
}


void CJoinDlg::OnBnClickedJoinBttEmailauthcheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strCode;
	GetDlgItemTextW(IDC_JOIN_EMAILAUTH, strCode);
	//인증 코드 통신

	SOCKET s = socketCreate();
	if (s == SOCKET_ERROR) AfxMessageBox(_T("socket error!"), MB_OK);
	/*
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(4500);
	addr.sin_addr.s_addr = inet_addr("165.132.144.106");
	//if (connect(s, (SOCKADDR*)&addr, sizeof(addr)) == -1) {
	//	AfxMessageBox(_T("connection(dir) error!"), MB_OK);
	//}*/

	if (sockSetting(s) == -1)
		AfxMessageBox(_T("connection error!"), MB_OK);

	else {

		Items item;
		CT2CA convert(strCode);
		item.EmailAuthCode = convert;
		char* str;
		socket_send(s, "checkAuthCode", item);
		socket_recv(s, &str);
		AfxMessageBox(*str, MB_OK);
		AfxMessageBox((LPCTSTR)resultpacketbuffer1.c_str(), MB_OK);
		AfxMessageBox((LPCTSTR)resultpacketbuffer2.c_str(), MB_OK);
		//if 맞으면
		if (resultpacketbuffer2 == "refusal")
		{
			AfxMessageBox(_T("인증에 실패했습니다."), MB_OK);
			m_authcodecheck = false;
		}
		else {
			AfxMessageBox(_T("인증이 완료되었습니다."), MB_OK);
			m_authcodecheck = true;
		}
	}
	
}


void CJoinDlg::OnBnClickedJoinOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!m_idCheck) AfxMessageBox(_T("ID 중복 체크를 해주세요."), MB_OK);
	else if(!m_emailCheck) AfxMessageBox(_T("Email 중복 체크를 해주세요."), MB_OK);
	else if(!m_authcodecheck) AfxMessageBox(_T("Email 인증을 해주세요."), MB_OK);
	else {
		CString Id, Pw, Name, Email;
		GetDlgItemTextW(IDC_JOIN_EDIT_ID, Id);
		GetDlgItemTextW(IDC_JOIN_EDIT_PW, Pw);
		GetDlgItemTextW(IDC_JOIN_EDIT_NAME, Name);
		GetDlgItemTextW(IDC_JOIN_EDIT_EMAIL, Email);
		//통신
		SOCKET s = socketCreate();
		if (s == SOCKET_ERROR) AfxMessageBox(_T("socket error!"), MB_OK);
		/*
		SOCKADDR_IN addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(4500);
		addr.sin_addr.s_addr = inet_addr("165.132.144.106");
		//if (connect(s, (SOCKADDR*)&addr, sizeof(addr)) == -1) {
		//	AfxMessageBox(_T("connection(dir) error!"), MB_OK);
		//}*/

		if (sockSetting(s) == -1)
			AfxMessageBox(_T("connection error!"), MB_OK);

		else {
			Items item;
			CT2CA sId(Id);
			CT2CA sPw(Pw);
			CT2CA sName(Name);
			CT2CA sEmail(Email);
			item.Id = sId;
			item.Pw = sPw;
			item.Name = sName;
			item.Email = sEmail;
			char* str;
			socket_send(s, "checkAuthCode", item);
			socket_recv(s, &str);
			AfxMessageBox(*str, MB_OK);
			if (resultpacketbuffer2 == "refusal")
			{
				AfxMessageBox(_T("회원가입에 실패했습니다."), MB_OK);
			}
			else {
				AfxMessageBox(_T("회원 가입이 완료되었습니다."), MB_OK);
			}
		}
		CDialogEx::OnOK();
	}	
	
}


void CJoinDlg::OnBnClickedJoinCancle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}

