
// EsperClientDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include <string>


// CEsperClientDlg 대화 상자
class CEsperClientDlg : public CDialogEx
{
// 생성입니다.
public:
	CEsperClientDlg(CWnd* pParent = NULL);	// 표준 생성자입니다
	HTREEITEM m_hRoot[4];
	HTREEITEM m_hKind[4];
	POINT m_oldpos;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ESPERCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	bool m_isOpen;

	CEdit m_ebMsg;
	CString m_strEdit;

	std::string m_userid;
	std::string m_sessionkey;
	

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg LRESULT OnMsgFromTray(WPARAM, LPARAM);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	void SetId(std::string param_id);

	afx_msg void OnEnChangeEdit1();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	CTreeCtrl m_Tree;
	afx_msg void OnMenuWrapping();
	afx_msg void OnSetting();
	afx_msg void OnTrayOpen();
	afx_msg void OnTraySettings();
	afx_msg void OnTrayInfo();
	afx_msg void OnTrayLogout();
	afx_msg void OnTrayExit();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);


	afx_msg void OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTreeselectAccess();
	afx_msg void OnTreeselectDelete();
};
