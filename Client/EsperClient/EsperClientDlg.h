
// EsperClientDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"


// CEsperClientDlg 대화 상자
class CEsperClientDlg : public CDialogEx
{
// 생성입니다.
public:
	CEsperClientDlg(CWnd* pParent = NULL);	// 표준 생성자입니다
	HTREEITEM m_hRoot;
	HTREEITEM m_hKind[4];

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ESPERCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	CTreeCtrl m_Tree;
	afx_msg void OnMenuWrapping();
	afx_msg void OnSetting();
};
