// AccessDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EsperClient.h"
#include "AccessDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include "Communication.h"


// CAccessDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAccessDlg, CDialogEx)

CAccessDlg::CAccessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ACCESS, pParent)
{

}

CAccessDlg::~CAccessDlg()
{
}

void CAccessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ACCESS_EDIT_USERLIST, m_authlistctrl);
}

BOOL CAccessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	std::ifstream fin;
	fin.open("../idsk.txt");
	fin >> m_userid >> m_sessionkey;

	SetDlgItemText(IDC_ACCESS_STATIC_USERNAME, m_userid.c_str());
	SetDlgItemText(IDC_ACCESS_STATIC_FILENAME, m_filename.c_str());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BEGIN_MESSAGE_MAP(CAccessDlg, CDialogEx)	
	ON_BN_CLICKED(IDC_ACCESS_BTT_ADD, &CAccessDlg::OnBnClickedAccessBttAdd)
	ON_BN_CLICKED(IDOK, &CAccessDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAccessDlg �޽��� ó�����Դϴ�.




void CAccessDlg::OnBnClickedAccessBttAdd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString str;
	GetDlgItem(IDC_ACCESS_EDIT_ADD)->GetWindowText(str);
	CT2CA temp(str);
	std::string tempstr(temp);
	item.AccessorID = tempstr;
	item.setId(m_userid);
	item.SessionKey = m_sessionkey;
	/*
	//��� ��ǥ ����
	SOCKET s = socketCreate();
	if (s == SOCKET_ERROR) AfxMessageBox(_T("socket error!"), MB_OK);
	/*
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(4500);
	addr.sin_addr.s_addr = inet_addr("165.132.144.106");
	*/

	//if (connect(s, (SOCKADDR*)&addr, sizeof(addr)) == -1) {
	//	AfxMessageBox(_T("connection(dir) error!"), MB_OK);
	//}
	/*
	if (sockSetting(s) == -1)
		AfxMessageBox(_T("connection error!"), MB_OK);
	else {
		socket_send(s, "accessorCheck", item);
		//closesocket(s);
		ShowWindow(SW_HIDE);
	}
	string strtemp = NULL;
	socket_recv(s, &strtemp);

	if (resultpacketbuffer1 == "succ")
	{*/
		m_authlistctrl.AddString(str);
		UpdateData(TRUE);
		item.Accessor.push_back(tempstr);
	/* }
	else if (resultpacketbuffer1 == "fail")
	{
		AfxMessageBox(_T("�߸��� ���̵��Դϴ�."));
	}
	else
	{
		AfxMessageBox(_T("������ ����� �����߽��ϴ�."));
	}

	closesocket(s);
	WSACleanup();*/
}


void CAccessDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnOK();
}
