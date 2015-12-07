// SettingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EsperClient.h"
#include "SettingDlg.h"
#include "afxdialogex.h"
#include "PW_REASK.h"

// CSettingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSettingDlg, CDialog)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SETTING, pParent)
{

}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CSettingDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSettingDlg 메시지 처리기입니다.


BOOL CSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowText(_T("Setting"));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSettingDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CPW_REASK dialog;
	dialog.DoModal();
}
