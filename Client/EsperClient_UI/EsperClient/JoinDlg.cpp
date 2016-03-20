// JoinDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EsperClient.h"
#include "JoinDlg.h"
#include "afxdialogex.h"


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
	// if �����ϸ�
	AfxMessageBox(_T("��� ������ ID �Դϴ�."),MB_OK);
	m_idCheck = true;
	// else
	//AfxMessageBox(_T("�ߺ��� ID �Դϴ�."),MB_OK);
	//m_idCheck=false;

}


void CJoinDlg::OnBnClickedJoinBttEmailcheck()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strEmail;
	GetDlgItemTextW(IDC_JOIN_EDIT_EMAIL, strEmail);
	// Email check ���
	// if �����ϸ�
	AfxMessageBox(_T("��� ������ Email �Դϴ�."), MB_OK);
	m_emailCheck = true;
	//else
	//AfxMessageBox(_T("�ߺ��� Email �Դϴ�."),MB_OK);
	//m_emailCheck = false;

}


void CJoinDlg::OnBnClickedJoinBttEmailauth()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!m_emailCheck)
		AfxMessageBox(_T("Email�� Ȯ���� �ּ���."), MB_OK);
	
	else {
		//���� ���� �߼� ���
		AfxMessageBox(_T("���� ������ ���۵Ǿ����ϴ�."), MB_OK);
	}
}


void CJoinDlg::OnBnClickedJoinBttEmailauthcheck()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strCode;
	GetDlgItemTextW(IDC_JOIN_EMAILAUTH, strCode);
	//���� �ڵ� ���
	//if ������
	AfxMessageBox(_T("������ �Ϸ�Ǿ����ϴ�."), MB_OK);
	m_authcodecheck = true;
	//else
	//AfxMessageBox(_T("������ �����߽��ϴ�."), MB_OK);
	//m_authcodecheck = false;
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
		CDialogEx::OnOK();
	}
	
	
}


void CJoinDlg::OnBnClickedJoinCancle()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnOK();
}

