#pragma once


// CRemindIdDlg ��ȭ �����Դϴ�.

class CRemindIdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRemindIdDlg)

public:
	CRemindIdDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CRemindIdDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REMINDID };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
