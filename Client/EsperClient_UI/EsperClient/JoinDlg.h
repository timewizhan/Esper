#pragma once


// CJoinDlg 대화 상자입니다.

class CJoinDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CJoinDlg)

public:
	CJoinDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CJoinDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JOIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	bool m_idCheck, m_emailCheck, m_authcodecheck;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedJoinBttIdcheck();
	afx_msg void OnBnClickedJoinBttEmailcheck();
	afx_msg void OnBnClickedJoinBttEmailauth();
	afx_msg void OnBnClickedJoinBttEmailauthcheck();
	afx_msg void OnBnClickedJoinOk();
	afx_msg void OnBnClickedJoinCancle();


};
