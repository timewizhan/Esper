#pragma once


// CRemindPwDlg 대화 상자입니다.

class CRemindPwDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRemindPwDlg)

public:
	CRemindPwDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRemindPwDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REMINDPW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
