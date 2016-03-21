// WrapDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EsperClient.h"
#include "WrapDlg.h"
#include "afxdialogex.h"
#include "FileLayer.h"
#include "Communication.h"

// CWrapDlg ��ȭ �����Դϴ�.

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


// CWrapDlg �޽��� ó�����Դϴ�.


BOOL CWrapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(_T("Wrap"));
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	SetDlgItemText(IDC_WRAP_FILENAME, Filename);
	SetDlgItemText(IDC_WRAP_FILEPATH, Filepath);
	CString  m_userid2(m_userid.c_str());
	SetDlgItemText(IDC_WRAP_USER,m_userid2);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}




void CWrapDlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	
	m_authlistctrl.AddString(str);
	UpdateData(TRUE);
}


void CWrapDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialog::OnOK();

	CString SubFilename = Filename.Mid(0, (Filename.GetLength() - 4));
	CFileDialog dialog((BOOL)FALSE, _T("esp"), SubFilename+_T(".esp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("������(*.esp)|*.esp|�������(*.*)|*.*||"));
	
	dialog.m_ofn.lpstrTitle = _T("���� �����ϱ�");
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
