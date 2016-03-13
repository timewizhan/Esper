#pragma once
#include "afxwin.h"
#include <iostream>
#include <string>

// CWrapDlg ��ȭ �����Դϴ�.

class CWrapDlg : public CDialog
{
	DECLARE_DYNAMIC(CWrapDlg)

public:
	CWrapDlg( CString Filename = NULL, CString Filepath = NULL, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CWrapDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WrapDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	CString Filename, Filepath;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButton1();
	CListBox m_authlistctrl;
	afx_msg void OnBnClickedOk();
	afx_msg void OnStnClickedWrapFilename();
	void SetId(std::string param);
private : 
	std::string m_userid;

};
