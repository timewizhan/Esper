#pragma once
#include <string>
#include "Communication.h"

// CAccessDlg 대화 상자입니다.

class CAccessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAccessDlg)

public:
	CAccessDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAccessDlg();
	virtual BOOL OnInitDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ACCESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	Items item;
	afx_msg void OnBnClickedAccessBttAdd();
	CListBox m_authlistctrl;
	std::string m_userid, m_sessionkey, m_filename;
	void setFilename(std::string file) { m_filename = file; }
	afx_msg void OnBnClickedOk();
};
