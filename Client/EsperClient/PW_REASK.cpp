// PW_REASK.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EsperClient.h"
#include "PW_REASK.h"
#include "afxdialogex.h"

CFont m_font_Name;
// CPW_REASK ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPW_REASK, CDialog)

CPW_REASK::CPW_REASK(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PW_REASK_DIALOG, pParent)
{

}

CPW_REASK::~CPW_REASK()
{
}

void CPW_REASK::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPW_REASK, CDialog)
END_MESSAGE_MAP()


// CPW_REASK �޽��� ó�����Դϴ�.


BOOL CPW_REASK::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	SetWindowText(_T("ESPER"));
	m_font_Name.CreateFont(25, // nHeight 
		12, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		10, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		DEFAULT_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		CA2W("����")); // lpszFacename 
	GetDlgItem(IDC_STATIC)->SetFont(&m_font_Name);
	GetDlgItem(IDC_STATIC2)->SetFont(&m_font_Name);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
