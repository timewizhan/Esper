
// EsperClientDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"


// CEsperClientDlg ��ȭ ����
class CEsperClientDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CEsperClientDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�
	HTREEITEM m_hRoot;
	HTREEITEM m_hKind[4];

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ESPERCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
