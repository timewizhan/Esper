 #pragma once
#include <string>
// LoginDlg 대화 상자입니다.
class LoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LoginDlg)

public:
	LoginDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~LoginDlg();
	virtual BOOL OnInitDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
#endif

protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	afx_msg void OnBnClickedOk();
	void SetId(std::string param);

private:
	std::string m_userid;
	
public:
	afx_msg void OnBnClickedRegist();
};
