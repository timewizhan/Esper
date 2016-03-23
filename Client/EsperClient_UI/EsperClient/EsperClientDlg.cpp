
// EsperClientDlg.cpp : 구현 파일
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



// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CEsperClientDlg 대화 상자

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




// CEsperClientDlg 메시지 처리기

BOOL CEsperClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	m_hIcon = NULL;
	//SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	//SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

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
		CA2W("굴림")); // lpszFacename 
	GetDlgItem(IDC_STATIC)->SetFont(&m_font);
	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//TREE 만들기
	
	m_hRoot[0] = m_Tree.InsertItem(_T("파일명1"),0,1);
	m_hRoot[1] = m_Tree.InsertItem(_T("파일명1"), 0, 1);
	//m_Tree.SetFont(&m_font,1);

	m_hKind[0] = m_Tree.InsertItem(_T("사람1"), 2, 2, m_hRoot[0], TVI_LAST);
	m_hKind[2] = m_Tree.InsertItem(_T("사람2"), 2, 2, m_hRoot[0], TVI_LAST);
	m_hKind[3] = m_Tree.InsertItem(_T("사람3"), 2, 2, m_hRoot[0], TVI_LAST);
	m_hKind[4] = m_Tree.InsertItem(_T("사람4"), 2, 2, m_hRoot[0], TVI_LAST);

	m_hKind[0] = m_Tree.InsertItem(_T("사람1"), 2, 2, m_hRoot[1], TVI_LAST);
	m_hKind[2] = m_Tree.InsertItem(_T("사람2"), 2, 2, m_hRoot[1], TVI_LAST);
	m_hKind[3] = m_Tree.InsertItem(_T("사람3"), 2, 2, m_hRoot[1], TVI_LAST);
	m_hKind[4] = m_Tree.InsertItem(_T("사람4"), 2, 2, m_hRoot[1], TVI_LAST);

	//m_Tree.Expand(m_hRoot[0], TVE_EXPAND);
	//m_Tree.Expand(m_hRoot[1], TVE_EXPAND);
	

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CEsperClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
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
	
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEsperClientDlg::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEsperClientDlg::OnEnChangeEdit3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEsperClientDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CEsperClientDlg::OnMenuWrapping()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 랩핑 메뉴

	CFileDialog dialog((BOOL)TRUE, _T("hwp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("한글파일(*.hwp)|*.hwp|모든파일(*.*)|*.*||"));


	dialog.m_ofn.lpstrTitle = _T("파일 랩핑하기");
	dialog.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;
	
	
	if (IDOK == dialog.DoModal()) {
		CWrapDlg wDialog(dialog.GetFileName(),dialog.GetPathName());
		wDialog.SetId(m_userid);
		wDialog.DoModal();
	}

}



void CEsperClientDlg::OnSetting()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 환경설정
	CSettingDlg dialog1;
	dialog1.SetId(m_userid);
	dialog1.DoModal();
}


void CEsperClientDlg::OnTrayOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_isOpen)
		m_isOpen = false;
	else
		m_isOpen = true;
}


void CEsperClientDlg::OnTraySettings()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSettingDlg dialog1;
	dialog1.SetId(m_userid);
	dialog1.DoModal();
}


void CEsperClientDlg::OnTrayInfo()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CEsperClientDlg::OnTrayLogout()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CEsperClientDlg::OnTrayExit()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString selId;
	HTREEITEM hItem;
	TV_HITTESTINFO hit_info;

	// 화면상에서 마우스의 위치를 얻는다.
	::GetCursorPos(&hit_info.pt);

	// 얻은 마우스 좌표를 트리컨트롤 기준의 좌표로 변경한다.
	::ScreenToClient(m_Tree.m_hWnd, &hit_info.pt);

	// 현재 마우스 좌표가 위치한 항목 정보를 얻는다.
	HTREEITEM current_item = m_Tree.HitTest(&hit_info);
	if (current_item != NULL) {
		// 마우스가 위치한 항목을 찾았다면 해당 항목을 선택한다.
		m_Tree.Select(current_item, TVGN_CARET);

		selId = m_Tree.GetItemText(current_item);
		hItem = m_Tree.GetSelectedItem();

		//if (hItem == m_hRoot[0] or hItem == m_hRoot[1])
		{
			CPoint p;
			GetCursorPos(&p);
			CMenu menu;
			menu.LoadMenu(IDR_TREESELECT_MENU);
			CMenu* Dmenu = menu.GetSubMenu(0); //맨 앞에 메뉴를 가져옴
			Dmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, this);
		}

	}

	

	*pResult = 0;
}
