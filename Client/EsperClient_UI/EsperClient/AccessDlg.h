#pragma once
#include <string>
#include "Communication.h"

// CAccessDlg ��ȭ �����Դϴ�.

class CAccessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAccessDlg)

public:
	CAccessDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAccessDlg();
	virtual BOOL OnInitDialog();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ACCESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	Items item;
	afx_msg void OnBnClickedAccessBttAdd();
	CListBox m_authlistctrl;
	std::string m_userid, m_sessionkey, m_filename;
	void setFilename(std::string file) { m_filename = file; }
	afx_msg void OnBnClickedOk();
};
