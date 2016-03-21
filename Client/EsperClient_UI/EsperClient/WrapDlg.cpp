// WrapDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EsperClient.h"
#include "WrapDlg.h"
#include "afxdialogex.h"
#include "FileLayer.h"
#include "Communication.h"

// CWrapDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CWrapDlg, CDialog)

CWrapDlg::CWrapDlg (CString filename, CString filepath, CWnd* pParent /*=NULL*/)
	: CDialog(IDD_WrapDlg, pParent), Filename(filename),Filepath(filepath)
{

} 

CWrapDlg::~CWrapDlg()
{
}

void CWrapDlg::SetId(std::string param)
{
	m_userid = param;
}

void CWrapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_authlistctrl);
}


BEGIN_MESSAGE_MAP(CWrapDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CWrapDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CWrapDlg::OnBnClickedOk)
	ON_STN_CLICKED(IDC_WRAP_FILENAME, &CWrapDlg::OnStnClickedWrapFilename)
END_MESSAGE_MAP()


// CWrapDlg 메시지 처리기입니다.


BOOL CWrapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(_T("Wrap"));
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetDlgItemText(IDC_WRAP_FILENAME, Filename);
	SetDlgItemText(IDC_WRAP_FILEPATH, Filepath);
	CString  m_userid2(m_userid.c_str());
	SetDlgItemText(IDC_WRAP_USER,m_userid2);
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


void CWrapDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();

	CString SubFilename = Filename.Mid(0, (Filename.GetLength() - 4));
	CFileDialog dialog((BOOL)FALSE, _T("esp"), SubFilename+_T(".esp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("에스퍼(*.esp)|*.esp|모든파일(*.*)|*.*||"));
	
	dialog.m_ofn.lpstrTitle = _T("파일 랩핑하기");
	dialog.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;
	dialog.DoModal();

	ST_FILE_LAYER_HEADER stFileLayerHeader;
	stFileLayerHeader.dwServerId = 1;
	stFileLayerHeader.dwUserId = 1;

	Items item;

	CT2CA pszConvertedAnsiString(Filepath), pszConvertedAnsiString2(dialog.GetFolderPath());
	std::string strInputFile(pszConvertedAnsiString);
	//std::string strInputFile("C:\\Users\\wooPC\\Desktop\\hello.hwp");
	std::string strOutputFile(pszConvertedAnsiString2);
	//std::string strOutputFile("C:\\Users\\wooPC\\Desktop");
	DWORD dwRet = EncryptFileLayer(stFileLayerHeader, strInputFile, strOutputFile);

}


void CWrapDlg::OnStnClickedWrapFilename()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
