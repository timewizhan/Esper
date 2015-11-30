// WrapDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EsperClient.h"
#include "WrapDlg.h"
#include "afxdialogex.h"


// CWrapDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CWrapDlg, CDialog)

CWrapDlg::CWrapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_WrapDlg, pParent)
{

}

CWrapDlg::~CWrapDlg()
{
}

void CWrapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_authlistctrl);
}


BEGIN_MESSAGE_MAP(CWrapDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CWrapDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CWrapDlg 메시지 처리기입니다.


BOOL CWrapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(_T("Wrap"));
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}




void CWrapDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	
	m_authlistctrl.AddString(str);
	UpdateData(TRUE);
}
