
// EsperClientDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "EsperClient.h"
#include "EsperClientDlg.h"
#include "afxdialogex.h"
#include "WrapDlg.h"
#include "SettingDlg.h"
#include "strsafe.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDH_TRAYICON 0x01
#define YM_FROMTRAY WM_USER+1



// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void On32775();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CAboutDlg::On32775()
{
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_COMMAND(ID_32775, &CAboutDlg::On32775)
END_MESSAGE_MAP()


// CEsperClientDlg ��ȭ ����

void CEsperClientDlg::SetId(std::string param_id,std::string param_sessionkey)
{
	m_userid = param_id;
	m_sessionkey = param_sessionkey;
}

CEsperClientDlg::CEsperClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ESPERCLIENT_DIALOG, pParent), m_strEdit(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_oldpos.x = m_oldpos.y = 0;
	m_isOpen = true;
}

void CEsperClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	//DDX_Control(pDX, IDC_EDIT1, m_ebMsg);
	//DDX_Text(pDX, IDC_EDIT1, m_strEdit);
}

BEGIN_MESSAGE_MAP(CEsperClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(YM_FROMTRAY, OnMsgFromTray)
//	ON_EN_CHANGE(IDC_EDIT1, &CEsperClientDlg::OnEnChangeEdit1)
//ON_LBN_SELCHANGE(IDC_LIST1, &CEsperClientDlg::OnLbnSelchangeList1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CEsperClientDlg::OnTvnSelchangedTree1)
	ON_COMMAND(ID_MENU_WRAPPING, &CEsperClientDlg::OnMenuWrapping)
	ON_COMMAND(ID_SETTING, &CEsperClientDlg::OnSetting)
	ON_COMMAND(ID_TRAY_OPEN, &CEsperClientDlg::OnTrayOpen)
	ON_COMMAND(ID_TRAY_SETTINGS, &CEsperClientDlg::OnTraySettings)
	ON_COMMAND(ID_TRAY_INFO, &CEsperClientDlg::OnTrayInfo)
	ON_COMMAND(ID_TRAY_LOGOUT, &CEsperClientDlg::OnTrayLogout)
	ON_COMMAND(ID_TRAY_EXIT, &CEsperClientDlg::OnTrayExit)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()

	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CEsperClientDlg::OnNMRClickTree1)
END_MESSAGE_MAP()




// CEsperClientDlg �޽��� ó����

BOOL CEsperClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);
	/*
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}*/

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	m_hIcon = NULL;
	//SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	//SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	NOTIFYICONDATA st_taskbaricon = { 0, };
	st_taskbaricon.cbSize = sizeof(NOTIFYICONDATA);
	st_taskbaricon.hWnd = this->GetSafeHwnd();
	st_taskbaricon.uID = IDH_TRAYICON;
	st_taskbaricon.uFlags = NIF_ICON | NIF_TIP | NIF_INFO | NIF_MESSAGE;
	st_taskbaricon.hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	st_taskbaricon.uCallbackMessage = YM_FROMTRAY;

	wcscpy(st_taskbaricon.szTip, TEXT("ESPER Client - developing version"));
	wcscpy(st_taskbaricon.szInfoTitle, TEXT("ESPER"));
	wcscpy(st_taskbaricon.szInfo, TEXT("ESPER Client - developing version"));

	st_taskbaricon.uVersion = NOTIFYICON_VERSION;
	st_taskbaricon.dwInfoFlags = NIIF_USER;

	::Shell_NotifyIcon(NIM_ADD, &st_taskbaricon);

	CFont m_font;

	m_font.CreateFont(40, // nHeight 
		30, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		30, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		DEFAULT_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		CA2W("����")); // lpszFacename 
	GetDlgItem(IDC_STATIC)->SetFont(&m_font);
	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	//TREE �����
	
	m_hRoot[0] = m_Tree.InsertItem(_T("���ϸ�1"),0,1);
	m_hRoot[1] = m_Tree.InsertItem(_T("���ϸ�1"), 0, 1);
	//m_Tree.SetFont(&m_font,1);

	m_hKind[0] = m_Tree.InsertItem(_T("���1"), 2, 2, m_hRoot[0], TVI_LAST);
	m_hKind[2] = m_Tree.InsertItem(_T("���2"), 2, 2, m_hRoot[0], TVI_LAST);
	m_hKind[3] = m_Tree.InsertItem(_T("���3"), 2, 2, m_hRoot[0], TVI_LAST);
	m_hKind[4] = m_Tree.InsertItem(_T("���4"), 2, 2, m_hRoot[0], TVI_LAST);

	m_hKind[0] = m_Tree.InsertItem(_T("���1"), 2, 2, m_hRoot[1], TVI_LAST);
	m_hKind[2] = m_Tree.InsertItem(_T("���2"), 2, 2, m_hRoot[1], TVI_LAST);
	m_hKind[3] = m_Tree.InsertItem(_T("���3"), 2, 2, m_hRoot[1], TVI_LAST);
	m_hKind[4] = m_Tree.InsertItem(_T("���4"), 2, 2, m_hRoot[1], TVI_LAST);

	//m_Tree.Expand(m_hRoot[0], TVE_EXPAND);
	//m_Tree.Expand(m_hRoot[1], TVE_EXPAND);
	

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CEsperClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();

	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CEsperClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CEsperClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CEsperClientDlg::OnMsgFromTray(WPARAM wParam, LPARAM lParam)
{
	/* CString str;
	str.Format(_T("YM_FROMTRAY	%04x	%04x\r\n"), wParam, lParam);
	UpdateData(TRUE);
	m_strEdit += str;
	UpdateData(FALSE);
	m_ebMsg.LineScroll(m_ebMsg.GetLineCount());*/

	if (wParam == IDH_TRAYICON)
		if (lParam == WM_LBUTTONDOWN || lParam == WM_RBUTTONDOWN)
		{
			POINT pos;
			CMenu m_menu2;
			m_menu2.LoadMenu(IDR_TRAY);
			::GetCursorPos(&pos);
			::SetForegroundWindow(this->GetSafeHwnd());
			m_menu2.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this);
			
			::SetForegroundWindow(this->GetSafeHwnd ());

		}
		else if (lParam == WM_LBUTTONDBLCLK)
		{
			ShowWindow(SW_SHOW);
			//SetFocus();
		}

	if (m_isOpen) {
		ShowWindow(SW_SHOW);
	}
	else
		ShowWindow(SW_HIDE);

	return 0;
}

void CEsperClientDlg::OnEnChangeEdit1()
{
	
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CEsperClientDlg::OnLbnSelchangeList1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CEsperClientDlg::OnEnChangeEdit3()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CEsperClientDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}


void CEsperClientDlg::OnMenuWrapping()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	// ���� �޴�

	CFileDialog dialog((BOOL)TRUE, _T("hwp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("�ѱ�����(*.hwp)|*.hwp|�������(*.*)|*.*||"));


	dialog.m_ofn.lpstrTitle = _T("���� �����ϱ�");
	dialog.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;
	
	
	if (IDOK == dialog.DoModal()) {
		CWrapDlg wDialog(dialog.GetFileName(),dialog.GetPathName());
		wDialog.SetId(m_userid);
		wDialog.DoModal();
	}

}



void CEsperClientDlg::OnSetting()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	// ȯ�漳��
	CSettingDlg dialog1;
	dialog1.SetId(m_userid);
	dialog1.DoModal();
}


void CEsperClientDlg::OnTrayOpen()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if (m_isOpen)
		m_isOpen = false;
	else
		m_isOpen = true;
}


void CEsperClientDlg::OnTraySettings()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CSettingDlg dialog1;
	dialog1.SetId(m_userid);
	dialog1.DoModal();
}


void CEsperClientDlg::OnTrayInfo()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CEsperClientDlg::OnTrayLogout()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}


void CEsperClientDlg::OnTrayExit()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	DestroyWindow();
	CDialog::OnClose();
	

}


void CEsperClientDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_oldpos = point;
	SetCapture();
	CDialog::OnLButtonDown(nFlags, point);
}

void CEsperClientDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default	
	if (nFlags == MK_LBUTTON)
	{
		RECT rect;
		POINT offset;

		this->GetWindowRect(&rect);
		offset.x = point.x - m_oldpos.x;
		offset.y = point.y - m_oldpos.y;

		rect.left += offset.x; rect.right += offset.x;
		rect.top += offset.y; rect.bottom += offset.y;

		MoveWindow(&rect, 1);
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CEsperClientDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture();
	CDialog::OnLButtonUp(nFlags, point);
}


void CEsperClientDlg::OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString selId;
	HTREEITEM hItem;
	TV_HITTESTINFO hit_info;

	// ȭ��󿡼� ���콺�� ��ġ�� ��´�.
	::GetCursorPos(&hit_info.pt);

	// ���� ���콺 ��ǥ�� Ʈ����Ʈ�� ������ ��ǥ�� �����Ѵ�.
	::ScreenToClient(m_Tree.m_hWnd, &hit_info.pt);

	// ���� ���콺 ��ǥ�� ��ġ�� �׸� ������ ��´�.
	HTREEITEM current_item = m_Tree.HitTest(&hit_info);
	if (current_item != NULL) {
		// ���콺�� ��ġ�� �׸��� ã�Ҵٸ� �ش� �׸��� �����Ѵ�.
		m_Tree.Select(current_item, TVGN_CARET);

		selId = m_Tree.GetItemText(current_item);
		hItem = m_Tree.GetSelectedItem();

		//if (hItem == m_hRoot[0] or hItem == m_hRoot[1])
		{
			CPoint p;
			GetCursorPos(&p);
			CMenu menu;
			menu.LoadMenu(IDR_TREESELECT_MENU);
			CMenu* Dmenu = menu.GetSubMenu(0); //�� �տ� �޴��� ������
			Dmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, this);
		}

	}

	

	*pResult = 0;
}
