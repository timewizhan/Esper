// RemindPwDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EsperClient.h"
#include "RemindPwDlg.h"
#include "afxdialogex.h"
#include "Communication.h"
#include <string>


// CRemindPwDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CRemindPwDlg, CDialogEx)

CRemindPwDlg::CRemindPwDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_REMINDPW, pParent)
{

}

CRemindPwDlg::~CRemindPwDlg()
{
}

void CRemindPwDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRemindPwDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRemindPwDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRemindPwDlg �޽��� ó�����Դϴ�.


void CRemindPwDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	CString cstrId, cstrEmail;

	GetDlgItemText(IDC_REMINDPW_EDIT_ID, cstrId);
	GetDlgItemText(IDC_REMINDPW_EDIT_EMAIL, cstrEmail);
	std::string m_userid(cstrId), m_email(cstrEmail);

	Items item;
	std::string str;
	item.Id = m_userid;
	item.setEmail(m_email);

	//��� ��ǥ ����
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
		socket_send(s, "findID", item);
		resultpacketbuffer1 = "";
		resultpacketbuffer2 = "";
		resultpacketbuffer3 = "";
		if (socket_recv(s, &str)<0) AfxMessageBox(_T("Receive Error"), MB_OK);

		CString cstr = str.c_str();


		//AfxMessageBox((LPCTSTR)str.c_str(), MB_OK);
		//AfxMessageBox((LPCTSTR)str, MB_OK);
		//AfxMessageBox(str.c_str(), MB_OK);
		//AfxMessageBox(resultpacketbuffer1.c_str(), MB_OK);
		//AfxMessageBox(resultpacketbuffer2.c_str(), MB_OK);

		if (resultpacketbuffer3 == "refusal")
			AfxMessageBox(TEXT(" �ش��ϴ� ID�� �������� �ʽ��ϴ�."), MB_OK);
		else if (resultpacketbuffer3 == "approval") {
			std::string message = "��й�ȣ�� �Դϴ�.";
		}
		else
		{
			AfxMessageBox(TEXT("Error"), MB_OK);
			closesocket(s);
			::SendMessage(GetSafeHwnd(), WM_CLOSE, NULL, NULL);
		}
	}
	closesocket(s);
	WSACleanup();


	CDialogEx::OnOK();
}
