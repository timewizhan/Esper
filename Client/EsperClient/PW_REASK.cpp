// PW_REASK.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EsperClient.h"
#include "PW_REASK.h"
#include "afxdialogex.h"

CFont m_font_Name;
// CPW_REASK 대화 상자입니다.

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


// CPW_REASK 메시지 처리기입니다.


BOOL CPW_REASK::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
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
		CA2W("굴림")); // lpszFacename 
	GetDlgItem(IDC_STATIC)->SetFont(&m_font_Name);
	GetDlgItem(IDC_STATIC2)->SetFont(&m_font_Name);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
