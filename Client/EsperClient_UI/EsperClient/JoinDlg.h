#pragma once


// CJoinDlg ��ȭ �����Դϴ�.

class CJoinDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CJoinDlg)

public:
	CJoinDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CJoinDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JOIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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
