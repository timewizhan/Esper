#pragma once


// CPW_REASK ��ȭ �����Դϴ�.

class CPW_REASK : public CDialog
{
	DECLARE_DYNAMIC(CPW_REASK)

public:
	CPW_REASK(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPW_REASK();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PW_REASK_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
