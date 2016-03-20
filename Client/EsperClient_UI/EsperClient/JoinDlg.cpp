// JoinDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EsperClient.h"
#include "JoinDlg.h"
#include "afxdialogex.h"


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
	// if 가능하면
	AfxMessageBox(_T("사용 가능한 ID 입니다."),MB_OK);
	m_idCheck = true;
	// else
	//AfxMessageBox(_T("중복된 ID 입니다."),MB_OK);
	//m_idCheck=false;

}


void CJoinDlg::OnBnClickedJoinBttEmailcheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strEmail;
	GetDlgItemTextW(IDC_JOIN_EDIT_EMAIL, strEmail);
	// Email check 통신
	// if 가능하면
	AfxMessageBox(_T("사용 가능한 Email 입니다."), MB_OK);
	m_emailCheck = true;
	//else
	//AfxMessageBox(_T("중복된 Email 입니다."),MB_OK);
	//m_emailCheck = false;

}


void CJoinDlg::OnBnClickedJoinBttEmailauth()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!m_emailCheck)
		AfxMessageBox(_T("Email을 확인해 주세요."), MB_OK);
	
	else {
		//인증 메일 발송 통신
		AfxMessageBox(_T("인증 메일이 전송되었습니다."), MB_OK);
	}
}


void CJoinDlg::OnBnClickedJoinBttEmailauthcheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strCode;
	GetDlgItemTextW(IDC_JOIN_EMAILAUTH, strCode);
	//인증 코드 통신
	//if 맞으면
	AfxMessageBox(_T("인증이 완료되었습니다."), MB_OK);
	m_authcodecheck = true;
	//else
	//AfxMessageBox(_T("인증에 실패했습니다."), MB_OK);
	//m_authcodecheck = false;
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
		CDialogEx::OnOK();
	}
	
	
}


void CJoinDlg::OnBnClickedJoinCancle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}

