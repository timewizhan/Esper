#pragma once


// CRemindPwDlg ��ȭ �����Դϴ�.

class CRemindPwDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRemindPwDlg)

public:
	CRemindPwDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CRemindPwDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REMINDPW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
