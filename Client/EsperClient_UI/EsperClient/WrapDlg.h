#pragma once
#include "afxwin.h"


// CWrapDlg ��ȭ �����Դϴ�.

class CWrapDlg : public CDialog
{
	DECLARE_DYNAMIC(CWrapDlg)

public:
	CWrapDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CWrapDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WrapDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButton1();
	CListBox m_authlistctrl;
	afx_msg void OnBnClickedOk();
};
