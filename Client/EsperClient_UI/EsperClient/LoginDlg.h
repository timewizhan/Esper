 #pragma once
#include <string>
// LoginDlg ��ȭ �����Դϴ�.
class LoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LoginDlg)

public:
	LoginDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~LoginDlg();
	virtual BOOL OnInitDialog();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
#endif

protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()


public:
	afx_msg void OnBnClickedOk();
	void SetId(std::string param);

private:
	std::string m_userid;
	
public:
	afx_msg void OnBnClickedRegist();
};
