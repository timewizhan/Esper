#pragma once


// CPW_REASK 대화 상자입니다.

class CPW_REASK : public CDialog
{
	DECLARE_DYNAMIC(CPW_REASK)

public:
	CPW_REASK(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPW_REASK();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PW_REASK_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
