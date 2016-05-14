// RemindIdDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EsperClient.h"
#include "RemindIdDlg.h"
#include "afxdialogex.h"
#include "Communication.h"
#include <string>



// CRemindIdDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRemindIdDlg, CDialogEx)

CRemindIdDlg::CRemindIdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_REMINDID, pParent)
{

}

CRemindIdDlg::~CRemindIdDlg()
{
}

void CRemindIdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRemindIdDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRemindIdDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRemindIdDlg 메시지 처리기입니다.


void CRemindIdDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CString cstrName, cstrEmail;
	
	GetDlgItemText(IDC_REMINDID_EDIT_NAME, cstrName);
	GetDlgItemText(IDC_REMINDID_EDIT_EMAIL, cstrEmail);
	std::string m_username(cstrName), m_email(cstrEmail);
	
	Items item;
	std::string str;
	item.Name = m_username;
	item.setEmail(m_email);

	//통신 목표 설정
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
			AfxMessageBox(TEXT(" 해당하는 ID가 존재하지 않습니다."), MB_OK);
		else if (resultpacketbuffer3 == "approval") {
			std::string message = "아이디는 입니다.";
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
