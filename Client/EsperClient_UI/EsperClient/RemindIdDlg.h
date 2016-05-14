#pragma once


// CRemindIdDlg 대화 상자입니다.

class CRemindIdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRemindIdDlg)

public:
	CRemindIdDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRemindIdDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REMINDID };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
