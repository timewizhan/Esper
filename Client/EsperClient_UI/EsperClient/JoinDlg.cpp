// JoinDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EsperClient.h"
#include "JoinDlg.h"
#include "afxdialogex.h"
#include "Communication.h"


// CJoinDlg ��ȭ �����Դϴ�.

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


// CJoinDlg �޽��� ó�����Դϴ�.


void CJoinDlg::OnBnClickedJoinBttIdcheck()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strId;
	GetDlgItemTextW(IDC_JOIN_EDIT_ID,strId);
	
	// ID Check ���

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
			AfxMessageBox(TEXT("�ߺ��� ID �Դϴ�."), MB_OK);
		}
			
		else {
			AfxMessageBox(_T("��� ������ ID �Դϴ�."), MB_OK);
			m_idCheck = true;
		}
	}
	closesocket(s);
	WSACleanup();
	// if �����ϸ�
	


}


void CJoinDlg::OnBnClickedJoinBttEmailcheck()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strEmail;
	GetDlgItemTextW(IDC_JOIN_EDIT_EMAIL, strEmail);
	// Email check ���
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
			AfxMessageBox(_T("�ߺ��� Email �Դϴ�."), MB_OK);
			m_emailCheck = false;
		}
		else {
			AfxMessageBox(_T("��� ������ Email �Դϴ�."), MB_OK);
			m_emailCheck = true;
		}
	}		

}


void CJoinDlg::OnBnClickedJoinBttEmailauth()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (!m_emailCheck)
		AfxMessageBox(_T("Email �ߺ��� Ȯ���� �ּ���."), MB_OK);
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

			//���� ���� �߼� ���
			Items item;
			char* str;
			socket_send(s, "checkID", item);
			socket_recv(s, &str);
			AfxMessageBox(*str, MB_OK);
			AfxMessageBox((LPCTSTR)resultpacketbuffer1.c_str(), MB_OK);
			AfxMessageBox((LPCTSTR)resultpacketbuffer2.c_str(), MB_OK);
			if(resultpacketbuffer2 == "refusal")
				AfxMessageBox(_T("���� ���� ���ۿ� �����߽��ϴ�."), MB_OK);
			else {
				AfxMessageBox(_T("���� ������ ���۵Ǿ����ϴ�."), MB_OK);
			}

		}
	}
}


void CJoinDlg::OnBnClickedJoinBttEmailauthcheck()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strCode;
	GetDlgItemTextW(IDC_JOIN_EMAILAUTH, strCode);
	//���� �ڵ� ���

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
		//if ������
		if (resultpacketbuffer2 == "refusal")
		{
			AfxMessageBox(_T("������ �����߽��ϴ�."), MB_OK);
			m_authcodecheck = false;
		}
		else {
			AfxMessageBox(_T("������ �Ϸ�Ǿ����ϴ�."), MB_OK);
			m_authcodecheck = true;
		}
	}
	
}


void CJoinDlg::OnBnClickedJoinOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!m_idCheck) AfxMessageBox(_T("ID �ߺ� üũ�� ���ּ���."), MB_OK);
	else if(!m_emailCheck) AfxMessageBox(_T("Email �ߺ� üũ�� ���ּ���."), MB_OK);
	else if(!m_authcodecheck) AfxMessageBox(_T("Email ������ ���ּ���."), MB_OK);
	else {
		CString Id, Pw, Name, Email;
		GetDlgItemTextW(IDC_JOIN_EDIT_ID, Id);
		GetDlgItemTextW(IDC_JOIN_EDIT_PW, Pw);
		GetDlgItemTextW(IDC_JOIN_EDIT_NAME, Name);
		GetDlgItemTextW(IDC_JOIN_EDIT_EMAIL, Email);
		//���
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
				AfxMessageBox(_T("ȸ�����Կ� �����߽��ϴ�."), MB_OK);
			}
			else {
				AfxMessageBox(_T("ȸ�� ������ �Ϸ�Ǿ����ϴ�."), MB_OK);
			}
		}
		CDialogEx::OnOK();
	}	
	
}


void CJoinDlg::OnBnClickedJoinCancle()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnOK();
}

