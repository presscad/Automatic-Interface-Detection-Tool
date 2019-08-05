
// kp2p_check_tool_win32Dlg.cpp : 
//

#include "stdafx.h"
#include "kp2p_check_tool_win32.h"
#include "kp2p_check_tool_win32Dlg.h"
#include "afxdialogex.h"
#include "Kp2pEnv.h"
#include "Util.h"
//#include "ParseJson.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL Ckp2p_check_tool_win32Dlg::m_bShellLogin = FALSE;
BOOL Ckp2p_check_tool_win32Dlg::m_bInitInfoPage0 = FALSE;
BOOL Ckp2p_check_tool_win32Dlg::m_bInitInfoPage1 = FALSE;
int  Ckp2p_check_tool_win32Dlg::m_Ecode = 0;
Ckp2p_check_tool_win32Dlg* Ckp2p_check_tool_win32Dlg::m_This = NULL;
CRITICAL_SECTION Ckp2p_check_tool_win32Dlg::m_OnDataFlagLock = { 0 };
HANDLE Ckp2p_check_tool_win32Dlg::m_OnDataFlagEvent = NULL;
volatile LONG Ckp2p_check_tool_win32Dlg::m_OnDataFlagCountLock = 0;
volatile LONG Ckp2p_check_tool_win32Dlg::m_FstCurDevConfigInfoCountLock = 0;
volatile LONG Ckp2p_check_tool_win32Dlg::m_OnSaveCacheCountLock = 0;
volatile LONG Ckp2p_check_tool_win32Dlg::m_OnOperateControlCountLock = 0;

INT Ckp2p_check_tool_win32Dlg::m_nConfigItemCount = 0;
INT Ckp2p_check_tool_win32Dlg::m_nStatusItemCount = 0;
//HANDLE Ckp2p_check_tool_win32Dlg::m_EndRunCheck = NULL;
BOOL Ckp2p_check_tool_win32Dlg::m_bFstCurDevConfigInfoFlag = FALSE;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


Ckp2p_check_tool_win32Dlg::Ckp2p_check_tool_win32Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_KP2P_CHECK_TOOL_WIN32_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_ExecHandleThr = NULL;
	m_ModHandleThr = NULL;
	m_CacheHandleThr = NULL;
	m_LoginHistoryCacheHandleThr = NULL;
	m_OperateControlHandleThr = NULL;
	m_CheckDevOfflineStatusHandleThr = NULL;
	m_nReShellLoginCount = 0;
}

void Ckp2p_check_tool_win32Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_ID_DEV_EDIT, m_EditDevID);
	DDX_Text(pDX, IDC_USER_DEV_EDIT, m_EditDevUser);
	DDX_Text(pDX, IDC_PASSD_DEV_EDIT, m_EditDevPassword);
	DDX_Text(pDX, IDC_USER_SVR_EDIT, m_EditSvrUser);
	DDX_Text(pDX, IDC_PASSD_SVR_EDIT, m_EditSvrPassword);

	//DDX_Control(pDX, IDC_LIST3, m_listData);
	DDX_Control(pDX, IDC_COMBO1, m_comboBoxTestItem);
	DDX_Control(pDX, IDC_ID_DEV_COMBO, m_ComboBoxDevIDItem);
	DDX_Control(pDX, IDC_USER_DEV_COMBO, m_ComboBoxDevUserItem);
	DDX_Control(pDX, IDC_USER_SVR_COMBO, m_ComboBoxSvrUserItem);
	
	//DDX_Control(pDX, IDC_UNTEST_ITEM_LIST, m_CheckListBoxTestItem);
	DDX_Control(pDX, IDC_STATUS_LIST, m_ListBoxStatusShow);

	DDX_Control(pDX, IDC_TAB1, m_TabCtrlInfo);
	DDX_Control(pDX, IDC_CUR_MAC_SVR_STATIC, m_LableCurMAC);

}

BEGIN_MESSAGE_MAP(Ckp2p_check_tool_win32Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCHITTEST()
	ON_WM_TIMER()
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &Ckp2p_check_tool_win32Dlg::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDCANCEL, &Ckp2p_check_tool_win32Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &Ckp2p_check_tool_win32Dlg::OnBnClickedOk)
	//ON_EN_CHANGE(IDC_EDIT_PARA_USER, &Ckp2p_check_tool_win32Dlg::OnEnChangeEditParaUser)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &Ckp2p_check_tool_win32Dlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedConnectButton)
	ON_BN_CLICKED(IDC_DISCONNECT_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedDisconnectButton)
	ON_BN_CLICKED(IDC_CANNEL_TEST_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedCannelTestButton)
	ON_BN_CLICKED(IDC_START_TEST_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedStartTestButton)
	ON_BN_CLICKED(IDC_LOAD_TEST_ITEM_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedLoadTestItemButton)
	ON_BN_CLICKED(IDC_VEDIO_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedVedioButton)
	ON_BN_CLICKED(IDC_LIVE_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedLiveButton)
	ON_BN_CLICKED(IDC_MOD_CONFIG_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedModConfigButton)
	ON_BN_CLICKED(IDC_RESTART_DEV_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedRestartDevButton)
	ON_BN_CLICKED(IDC_REMOTE_CONFIG_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedRemoteConfigButton)
	ON_BN_CLICKED(IDC_EXPORT_DATA_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedExportDataButton)
	ON_BN_CLICKED(IDC_DEL_TEST_ITEM_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedDelTestItemButton)
	//ON_WM_TIMER()
	ON_BN_CLICKED(IDC_START_CHECK_MAIN_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedStartCheckMainButton)
	ON_BN_CLICKED(IDC_CONFIG_SETTING_MAIN_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedConfigSettingMainButton)
	ON_BN_CLICKED(IDC_STOP_CHECK_MAIN_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedStopCheckMainButton)
	ON_BN_CLICKED(IDC_INFO_STATUS_DEV_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedInfoStatusDevButton)
	ON_BN_CLICKED(IDC_MOD_TIME_DEV_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedModTimeDevButton)
	ON_CBN_EDITCHANGE(IDC_ID_DEV_COMBO, &Ckp2p_check_tool_win32Dlg::OnCbnEditchangeIdDevCombo)
	ON_LBN_SELCHANGE(IDC_UNTEST_ITEM_LIST, &Ckp2p_check_tool_win32Dlg::OnLbnSelchangeUntestItemList)
END_MESSAGE_MAP()

BOOL Ckp2p_check_tool_win32Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

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
	}


	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		

	InitializeCriticalSection(&m_OnDataFlagLock);
	m_OnDataFlagEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_ExcuteCmdThreadExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_ModifyDevConfigThreadExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_ThreadNotifyCmdEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_ExecThreadStartCmdEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_ModThreadStartCmdEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_ReadyExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	//m_EndRunCheck = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_LoginInfoHistoryCacheThreadUpdateEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_QueryDevInfoHistoryCacheThreadUpdateEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_LoginInfoHistoryCacheThreadExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_OperateControlThreadStartEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_OperateControlThreadExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_CheckDevOfflineStatusThreadStartEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_CheckDevOfflineStatusThreadExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	//m_RestartDevThreadNotifyFailEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	//m_RestartDevThreadNotifySucessEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_DevOfflineNotifyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_TerminateCheckNotifyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_ListBoxStatusShow.SetHorizontalExtent(1500);

	CRect tabRect;
	m_TabCtrlInfo.InsertItem(0, _T("设备信息"));
	m_TabCtrlInfo.InsertItem(1, _T("设备配置测试信息"));
	m_TabCtrlInfo.InsertItem(2, _T("设备状态测试信息"));
	m_QueryDevInfoDlg.Create(IDD_QUERY_DEV_INFO_DIALOG, &m_TabCtrlInfo);
	m_DevConfigDlg.Create(IDD_DEV_CONFIG_DIALOG, &m_TabCtrlInfo);
	m_DevStatusDlg.Create(IDD_DEV_STATUS_DIALOG, &m_TabCtrlInfo);
	m_TabCtrlInfo.GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;
	m_QueryDevInfoDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	m_DevConfigDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	m_DevStatusDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	m_TabCtrlInfo.SetCurSel(0);

	CTime time;
	time = time.GetCurrentTime();
	CString stime;
	stime = time.Format("%y-%m-%d %H:%M:%S");
	
	m_This = this;
	UINT array[5] = { 12301,12302, 12303/*, 12304, 12305*/ };
	m_Statusbar.Create(this);
	m_Statusbar.SetIndicators(array, sizeof(array) / sizeof(UINT));

	CRect rect;
	GetWindowRect(rect);
	m_Statusbar.SetPaneInfo(0, array[0], 0, rect.Width() / 3);
	m_Statusbar.SetPaneInfo(1, array[1], 0, 8 * 8);
	m_Statusbar.SetPaneInfo(2, array[2], 0, rect.Width() / 3);
	/*m_Statusbar.SetPaneInfo(3, array[3], 0, 9 * 9);
	m_Statusbar.SetPaneInfo(4, array[4], 0, rect.Width() / 5);*/
	m_Statusbar.SetPaneText(0, stime);
	m_Statusbar.SetPaneText(1, _T("当前状态："));
	m_Statusbar.SetPaneText(2, _T(""));
	/*m_Statusbar.SetPaneText(3, _T("设备系统时间："));
	m_Statusbar.SetPaneText(4, _T("123456"));*/
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	SetTimer(1, 1000, NULL);

	m_comboBoxTestItem.AddString(_T("MTU"));
	m_comboBoxTestItem.AddString(_T("网关"));
	m_comboBoxTestItem.AddString(_T("DNS"));
	m_comboBoxTestItem.AddString(_T("信号强度"));
	m_comboBoxTestItem.AddString(_T("内存占用"));
	m_comboBoxTestItem.AddString(_T("进程"));
	m_comboBoxTestItem.AddString(_T("线程"));
	m_comboBoxTestItem.AddString(_T("CPU占用"));

	m_comboBoxTestItem.AddString(_T("PING检测"));
	m_comboBoxTestItem.AddString(_T("网络连接状况检测"));
	m_comboBoxTestItem.AddString(_T("带宽检测"));
	m_comboBoxTestItem.AddString(_T("路由表检测"));
	m_comboBoxTestItem.AddString(_T("日志数据获取检测"));
	//m_comboBoxTestItem.InsertString(1, _T("DNS"));
  
	m_comboBoxTestItem.SetCurSel(1);

	m_CheckListBoxTestItem.SubclassDlgItem(IDC_UNTEST_ITEM_LIST, this);
	m_CheckListBoxTestItem.AddString(_T("PING检测"));
	m_CheckListBoxTestItem.AddString(_T("信号强度"));
	m_CheckListBoxTestItem.AddString(_T("设备系统时间"));
	m_CheckListBoxTestItem.AddString(_T("MTU"));
	m_CheckListBoxTestItem.AddString(_T("网关"));
	m_CheckListBoxTestItem.AddString(_T("DNS"));	
	//m_CheckListBoxTestItem.AddString(_T("test"));
	/*m_CheckListBoxTestItem.AddString(_T("CPU占用"));
	m_CheckListBoxTestItem.AddString(_T("内存占用"));
	m_CheckListBoxTestItem.AddString(_T("进程"));
	m_CheckListBoxTestItem.AddString(_T("线程"));
	m_CheckListBoxTestItem.AddString(_T("路由表检测"));
	m_CheckListBoxTestItem.AddString(_T("网络连接状况检测"));
	m_CheckListBoxTestItem.AddString(_T("带宽检测"));*/
	m_CheckListBoxTestItem.SetCheck(0, 1);
	m_CheckListBoxTestItem.SetCheck(1, 1);
	m_CheckListBoxTestItem.SetCheck(2, 1);
	m_CheckListBoxTestItem.SetCheck(3, 1);
	m_CheckListBoxTestItem.SetCheck(4, 1);
	m_CheckListBoxTestItem.SetCheck(5, 1);
	//m_CheckListBoxTestItem.SetCheck(6, 0);

	//m_CheckListBoxTestItem.SetCurSel(0);

	m_BtnStartCheck = NULL;
	m_BtnStartCheck = (CButton *)GetDlgItem(IDC_START_CHECK_MAIN_BUTTON);
	m_BtnStartCheck->EnableWindow(FALSE);

	m_BtnStopCheck = NULL;
	m_BtnStopCheck = (CButton *)GetDlgItem(IDC_STOP_CHECK_MAIN_BUTTON);
	m_BtnStopCheck->EnableWindow(FALSE);

	/*m_BtnStartTest = NULL;
	m_BtnStartTest = (CButton *)GetDlgItem(IDC_START_TEST_BUTTON);
	m_BtnStartTest->EnableWindow(FALSE);*/

	m_BtnConnect = NULL;
	m_BtnConnect = (CButton *)GetDlgItem(IDC_CONNECT_BUTTON);
	//m_BtnConnect->EnableWindow(FALSE);

	m_BtnDisconnect = NULL;
	m_BtnDisconnect = (CButton *)GetDlgItem(IDC_DISCONNECT_BUTTON);
	m_BtnDisconnect->EnableWindow(FALSE);

	m_BtnDevCfgModify = NULL;
	m_BtnDevCfgModify = (CButton *)GetDlgItem(IDC_MOD_CONFIG_BUTTON);
	m_BtnDevCfgModify->EnableWindow(FALSE);

	m_BtnVideo = NULL;
	m_BtnVideo = (CButton *)GetDlgItem(IDC_VEDIO_BUTTON);
	m_BtnVideo->EnableWindow(FALSE);

	m_BtnLive = NULL;
	m_BtnLive = (CButton *)GetDlgItem(IDC_LIVE_BUTTON);
	m_BtnLive->EnableWindow(FALSE);

	m_BtnGetDEVStatusInfo = NULL;
	m_BtnGetDEVStatusInfo = (CButton *)GetDlgItem(IDC_INFO_STATUS_DEV_BUTTON);
	m_BtnGetDEVStatusInfo->EnableWindow(TRUE);

	m_BtnDevSysTimeModify = NULL;
	m_BtnDevSysTimeModify = (CButton *)GetDlgItem(IDC_MOD_TIME_DEV_BUTTON);
	m_BtnDevSysTimeModify->EnableWindow(FALSE);

	m_BtnRestartDev = NULL;
	m_BtnRestartDev = (CButton *)GetDlgItem(IDC_RESTART_DEV_BUTTON);
	m_BtnRestartDev->EnableWindow(FALSE);

	m_BtnRemoteSetting = NULL;
	m_BtnRemoteSetting = (CButton *)GetDlgItem(IDC_REMOTE_CONFIG_BUTTON);
	m_BtnRemoteSetting->EnableWindow(FALSE);

	m_BtnExportData = NULL;
	m_BtnExportData = (CButton *)GetDlgItem(IDC_EXPORT_DATA_BUTTON);
	m_BtnExportData->EnableWindow(FALSE);

	m_Session = NULL;
	m_bOnDataFlag = FALSE;
	m_bReadyStartFlag = FALSE;
	m_bReadyLoadFlag = FALSE;
	m_bDevConnectStatusFlag = FALSE;
	m_bShellSendReqFlag = FALSE;
	m_Shell = NULL;
	m_OnOperateControlCountLock = 0;

	
	(CButton *)GetDlgItem(IDC_ID_DEV_COMBO)->SetFocus();
	

#if (T_DEBUG == 0)
	//m_EditDevID.SetString(/*L"1606129063"*/L"924957972");
#else
	m_EditDevID.SetString(L"1606129063"/*L"924957972"*/);
#endif
	//m_EditDevUser.SetString(L"admin");
	/*m_EditSvrUser.SetString(L"test0");
	m_EditSvrPassword.SetString(L"123456");*/
	//UpdateData(FALSE);

	//获取本机所有mac,IP等网卡信息
	get_mac_info_init();

	//开启工作线程

	try {
		start_work_thread();
	}
	catch (exception &e) {
		CString err;
		err = e.what();
		SHOW_STATUS_INFO_W(err.GetString());
	}


	return FALSE;
	//return TRUE; 
}

BOOL Ckp2p_check_tool_win32Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	//if (WM_KEYDOWN == pMsg->message)
	//{
	//	CWnd *pFocus = GetFocus();
	//	if (pFocus)
	//	{
	//		if (IDC_CONNECT_BUTTON == pFocus->GetDlgCtrlID())
	//		{
	//			(CButton *)(GetDlgItem(IDC_CONNECT_BUTTON))->SetFocus();
	//		}
	//		/*else if (IDC_BUTTON2 == pFocus->GetDlgCtrlID())
	//		{
	//			(CButton *)(GetDlgItem(IDC_BUTTON3))->SetFocus();
	//		}
	//		else if (IDC_BUTTON3 == pFocus->GetDlgCtrlID())
	//		{
	//			(CButton *)(GetDlgItem(IDC_BUTTON1))->SetFocus();
	//		}*/
	//		return FALSE;
	//	}
	//}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void Ckp2p_check_tool_win32Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Ckp2p_check_tool_win32Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ÓÃÓÚ»æÖÆµÄÉè±¸ÉÏÏÂÎÄ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ê¹Í¼±êÔÚ¹¤×÷Çø¾ØÐÎÖÐ¾ÓÖÐ
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// »æÖÆÍ¼±ê
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

LRESULT Ckp2p_check_tool_win32Dlg::OnNcHitTest(CPoint point)
{
	int ret = CDialogEx::OnNcHitTest(point);

	//ifÓï¾äµÄÇ°Á½ÐÐÊÇÓÃÀ´½ûÖ¹¸Ä±ä´óÐ¡µÄ£¬×îºóÒ»ÐÐÊÇÓÃÀ´½ûÖ¹ÒÆ¶¯µÄ
	if (HTTOP == ret || HTBOTTOM == ret || HTLEFT == ret || HTRIGHT == ret
		|| HTBOTTOMLEFT == ret || HTBOTTOMRIGHT == ret || HTTOPLEFT == ret || HTTOPRIGHT == ret || HTCAPTION == ret)
		return HTCLIENT;

	return ret;
}

HCURSOR Ckp2p_check_tool_win32Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//void Ckp2p_check_tool_win32Dlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
//	*pResult = 0;
//}


void Ckp2p_check_tool_win32Dlg::OnBnClickedCancel()
{
	// TODO:
	INT_PTR nRes;


	::InterlockedIncrement(&m_OnDataFlagCountLock);
	if (::InterlockedDecrement(&m_OnDataFlagCountLock) != 0) {
		//SHOW_STATUS_INFO_A("正在测试，请等待");
		nRes = MessageBox(_T("正在测试,是否要中止并断开？"), _T("信息提示"), MB_YESNO);
		if (IDNO == nRes) {
			return;
		}
		
	}
	m_bDevConnectStatusFlag = FALSE;
	m_bReadyStartFlag = FALSE;
	SetEvent(m_TerminateCheckNotifyEvent);
	
	if (m_bDevConnectStatusFlag) {
		if (m_bShellLogin) {
			if (m_Session) {
				IOT_SHELL_Logout(&m_Session);
			}
			/*if (m_Shell) {
				IOT_SHELL_Deinit(&m_Shell);
			}*/
		}
		kp2p_close(m_Handle);
		kp2p_exit();
	}

	//退出测试工作线程
	SetEvent(m_ExcuteCmdThreadExitEvent);
	SetEvent(m_ModifyDevConfigThreadExitEvent);
	//SetEvent(m_SaveCacheThreadExitEvent);
	SetEvent(m_LoginInfoHistoryCacheThreadExitEvent);
	SetEvent(m_OperateControlThreadExitEvent);
	SetEvent(m_CheckDevOfflineStatusThreadExitEvent);

	HANDLE hThread[5];
	hThread[0] = m_ExecHandleThr;
	hThread[1] = m_ModHandleThr;
	hThread[2] = m_LoginHistoryCacheHandleThr;
	hThread[3] = m_OperateControlHandleThr;
	hThread[4] = m_CheckDevOfflineStatusHandleThr;

	WaitForMultipleObjects(5, hThread, TRUE, INFINITE);

	for (int i = 0; i < (sizeof(hThread) / sizeof(hThread[0])); i++) {
		CloseHandle(hThread[i]);
	}


	KillTimer(1);

	CDialogEx::OnCancel();
}

void Ckp2p_check_tool_win32Dlg::OnBnClickedOk()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	CDialogEx::OnOK();
}


//void Ckp2p_check_tool_win32Dlg::OnEnChangeEditParaUser()
//{
//	// TODO:  Èç¹û¸Ã¿Ø¼þÊÇ RICHEDIT ¿Ø¼þ£¬Ëü½«²»
//	// ·¢ËÍ´ËÍ¨Öª£¬³ý·ÇÖØÐ´ CDialogEx::OnInitDialog()
//	// º¯Êý²¢µ÷ÓÃ CRichEditCtrl().SetEventMask()£¬
//	// Í¬Ê±½« ENM_CHANGE ±êÖ¾¡°»ò¡±ÔËËãµ½ÑÚÂëÖÐ¡£
//
//	// TODO:  ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
//}

void Ckp2p_check_tool_win32Dlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	CRect tabRect;    
			 
	m_TabCtrlInfo.GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;

	switch (m_TabCtrlInfo.GetCurSel())
	{
		
	case 0:
		m_QueryDevInfoDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_DevConfigDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_DevStatusDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
		 
	case 1:
		m_QueryDevInfoDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_DevConfigDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_DevStatusDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 2:
		m_QueryDevInfoDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_DevConfigDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_DevStatusDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		break;
	default:
		break;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void Ckp2p_check_tool_win32Dlg::OnBnClickedDisconnectButton()
{
	INT_PTR nRes;

	::InterlockedIncrement(&m_OnDataFlagCountLock);
	if (::InterlockedDecrement(&m_OnDataFlagCountLock) != 0) {
		//SHOW_STATUS_INFO_A("正在测试，请等待");
		nRes = MessageBox(_T("正在测试,是否要中止并断开？"), _T("信息提示"), MB_YESNO);
		if (IDNO == nRes) {
			return;
		}
		SetEvent(m_TerminateCheckNotifyEvent);
	}
#if 0
	////退出测试工作线程
	//SetEvent(m_ExcuteCmdThreadExitEvent);
	//SetEvent(m_ModifyDevConfigThreadExitEvent);
	////SetEvent(m_SaveCacheThreadExitEvent);
	//SetEvent(m_LoginInfoHistoryCacheThreadExitEvent);
	//SetEvent(m_OperateControlThreadExitEvent);
	//SetEvent(m_CheckDevOfflineStatusThreadExitEvent);

	/*DWORD nRet;
	if ((nRet = WaitForSingleObject(m_ReadyExitEvent, 30000)) != WAIT_OBJECT_0) {
		msleep_c(50);
		switch (nRet)
		{
		case WAIT_TIMEOUT:
			SHOW_STATUS_INFO_S_A("超过等待30秒时间，强制退出");
			break;
		case WAIT_FAILED:
			SHOW_STATUS_INFO_S_A("WaitForSingleObject error");
			break;
		default:
			break;
		}
	}*/
#endif

	if (m_bShellLogin) {
		if (m_Session) {
			IOT_SHELL_Logout(&m_Session);
			SHOW_STATUS_INFO_A("IOT_SHELL_Logout退出登录");
		}
		if (m_Shell) {
			//IOT_SHELL_Deinit(&m_Shell);
		}	
	}
	
	kp2p_close(m_Handle);
	kp2p_exit();

	//SetEvent(m_CheckDevOfflineStatusThreadStartEvent);

	m_bDevConnectStatusFlag = FALSE;
	m_bReadyStartFlag = FALSE;
#if 0
	m_BtnDisconnect->EnableWindow(FALSE);
	m_BtnDevCfgModify->EnableWindow(FALSE);
	m_BtnVideo->EnableWindow(FALSE);
	m_BtnLive->EnableWindow(FALSE);
	m_BtnDevSysTimeModify->EnableWindow(FALSE);
	m_BtnRestartDev->EnableWindow(FALSE);
	m_BtnRemoteSetting->EnableWindow(FALSE);
	//m_BtnExportData->EnableWindow(FALSE);
	//m_BtnStartTest->EnableWindow(FALSE);
	m_BtnStartCheck->EnableWindow(FALSE);
	m_BtnStopCheck->EnableWindow(FALSE);
	m_BtnConnect->EnableWindow(TRUE);

	m_ComboBoxDevIDItem.EnableWindow(TRUE);
	m_ComboBoxDevUserItem.EnableWindow(TRUE);
	(CEdit *)GetDlgItem(IDC_PASSD_DEV_EDIT)->EnableWindow(TRUE);
#endif
	if (!m_TestCmdDlg.m_EditCmd.IsEmpty()) {
		m_TestCmdDlg.m_EditCmd.Empty();
	}

	SHOW_CURRENT_STATUS_INFO_A("退出登陆");

	return;
}

void Ckp2p_check_tool_win32Dlg::test_shell_OnShellData(void *ctx, void *session, const void *data, int datasz, int ecode)
{
	CString temp;
	if (0 != datasz) {
		m_Ecode = ecode;
		switch (ecode) {
		case ERROR_AUTH_SUCCESS:
			memset(m_This->m_sdns, 0, 1024 * 4);
			if (datasz < 1024 * 4)
				memcpy(m_This->m_sdns, data, datasz);
			::InterlockedIncrement(&m_FstCurDevConfigInfoCountLock);
			if (::InterlockedDecrement(&m_FstCurDevConfigInfoCountLock) == 1) {
				//TODO:
				if (m_This->m_bQueryModDevConfigInfoFlag) {
					temp = m_This->m_sdns;
					SHOW_STATUS_INFO_S_W(temp.GetString());
				} else 
					SHOW_STATUS_INFO_S_A("获取信息");
			}
			else {
				temp = m_This->m_sdns;
				SHOW_STATUS_INFO_S_A("获取信息");
				SHOW_STATUS_INFO_S_W(temp.GetString());
				//m_This->m_bOnDataFlag = TRUE;
			}
			m_This->m_bOnDataFlag = TRUE;
			SetEvent(m_This->m_ThreadNotifyCmdEvent);
			break;
		case ERROR_AUTH_USR:
			::InterlockedIncrement(&m_FstCurDevConfigInfoCountLock);
			if (::InterlockedDecrement(&m_FstCurDevConfigInfoCountLock) == 1) {
				//TODO:
				if (m_This->m_EditSvrUser.IsEmpty()) {
					SHOW_STATUS_INFO_S_A("服务器鉴权失败，请输入用户名");
				}
				else {
					SHOW_STATUS_INFO_S_A("服务器鉴权失败，用户名不存在");
				}
			}
			else {
				SHOW_STATUS_INFO_S_A("服务器鉴权失败，用户名不存在");
			}
			SHOW_STATUS_INFO_S_A("获取失败");
			m_This->m_bShellSendReqFlag = FALSE;
			m_This->m_bOnDataFlag = FALSE;
			SetEvent(m_This->m_ThreadNotifyCmdEvent);
			break;
		case ERROR_AUTH_PWD:
			::InterlockedIncrement(&m_FstCurDevConfigInfoCountLock);
			if (::InterlockedDecrement(&m_FstCurDevConfigInfoCountLock) == 1) {
				//TODO:
				if (m_This->m_EditSvrPassword.IsEmpty()) {
					SHOW_STATUS_INFO_S_A("服务器鉴权失败，请输入密码");
				}
				else {
					SHOW_STATUS_INFO_S_A("服务器鉴权失败，密码不正确");
				}
			}
			else {
				SHOW_STATUS_INFO_S_A("服务器鉴权失败，密码不正确");
			}
			SHOW_STATUS_INFO_S_A("获取失败");
			m_This->m_bShellSendReqFlag = FALSE;
			m_This->m_bOnDataFlag = FALSE;
			SetEvent(m_This->m_ThreadNotifyCmdEvent);
			break;
		case ERROR_AUTH_MAC:
			SHOW_STATUS_INFO_S_A("服务器鉴权失败，MAC地址错误");
			SHOW_STATUS_INFO_S_A("获取失败");
			m_This->m_bShellSendReqFlag = FALSE;
			m_This->m_bOnDataFlag = FALSE;
			SetEvent(m_This->m_ThreadNotifyCmdEvent);
			break;
		case ERROR_AUTH_VERSION:
			SHOW_STATUS_INFO_S_A("服务器鉴权失败，版本不正确");
			SHOW_STATUS_INFO_S_A("获取失败");
			m_This->m_bShellSendReqFlag = FALSE;
			m_This->m_bOnDataFlag = FALSE;
			SetEvent(m_This->m_ThreadNotifyCmdEvent);
			break;
		case ERROR_AUTH_ADD:
			SHOW_STATUS_INFO_S_A("服务器鉴权失败，ERROR_AUTH_ADD");
			SHOW_STATUS_INFO_S_A("获取失败");
			m_This->m_bShellSendReqFlag = FALSE;
			m_This->m_bOnDataFlag = FALSE;
			SetEvent(m_This->m_ThreadNotifyCmdEvent);
			break;
		case ERROR_AUTH_UNKNOW:
			SHOW_STATUS_INFO_S_A("服务器鉴权失败，ERROR_AUTH_UNKNOW");
			SHOW_STATUS_INFO_S_A("获取失败");
			m_This->m_bShellSendReqFlag = FALSE;
			m_This->m_bOnDataFlag = FALSE;
			SetEvent(m_This->m_ThreadNotifyCmdEvent);
			break;
		default:
			break;
		}

		/*::InterlockedIncrement(&m_OnDataFlagCountLock);
		if (::InterlockedDecrement(&m_OnDataFlagCountLock) == 0) {
			m_This->m_bOnDataFlag = true;
			SetEvent(m_OnDataFlagEvent);
		}*/
		//SetEvent(m_OnDataFlagEvent);
		/*EnterCriticalSection(&m_OnDataFlagLock);
		m_This->m_bOnDataFlag = true;
		LeaveCriticalSection(&m_OnDataFlagLock);*/
	}
}

void Ckp2p_check_tool_win32Dlg::OnConnect(kp2p_handle_t p2p_handle, void *context, const char *conn_type)
{
	    //printf("[OnConnect]Handle:%p, type:%s\n", p2p_handle, conn_type);
	//conn_times = iclock();
	//conn_status = 1;
	CString info;
	info.Format(L"[OnConnect]Handle:%p, type:", p2p_handle);
	info += conn_type;
	SHOW_STATUS_INFO_S_W(info.GetString());
	SHOW_STATUS_INFO_S_A("kp2p_connect连接成功");
}


void Ckp2p_check_tool_win32Dlg::OnDisconnect(kp2p_handle_t p2p_handle, void *context, int ret)
{
	//   printf("[OnDisconnect]Handle:%p disconnected,reason:%d\n", p2p_handle, ret);
	CString info;
	info.Format(L"[OnDisconnect]Handle:%p disconnected,reason:%d\n", p2p_handle, ret);
	SHOW_STATUS_INFO_S_W(info.GetString());
	SHOW_STATUS_INFO_S_A("设备连接已断开");
	//m_This->m_Statusbar.SetPaneText(2, _T("断开设备连接"));
	/*::InterlockedIncrement(&m_This->m_OnOperateControlCountLock);
	if (::InterlockedDecrement(&m_This->m_OnOperateControlCountLock) != 0) {
		SetEvent(m_This->m_RestartDevThreadNotifySucessEvent);
	}*/

	::InterlockedIncrement(&m_OnDataFlagCountLock);
	if (::InterlockedDecrement(&m_OnDataFlagCountLock) == 0) {
		::InterlockedIncrement(&m_OnOperateControlCountLock);
		if (::InterlockedDecrement(&m_OnOperateControlCountLock) != 0) {
			SetEvent(m_This->m_DevOfflineNotifyEvent);
		}
		else {
			if (m_This->m_bReadyStartFlag)
				SetEvent(m_This->m_CheckDevOfflineStatusThreadStartEvent);
		}
	}
	else {
		SetEvent(m_This->m_DevOfflineNotifyEvent);
	}
}


void Ckp2p_check_tool_win32Dlg::OnOpenStream(kp2p_handle_t p2p_handle, void *context, uint32_t channel, uint32_t stream, int err, const char *cam_des)
{
	  printf("OnOpenStream: %d %s %d\n", channel, cam_des, err);
}


void Ckp2p_check_tool_win32Dlg::OnCloseStream(kp2p_handle_t p2p_handle, void *context, uint32_t channel, uint32_t stream, int err)
{
	    printf("OnCloseStream: %d %d %d\n", channel, stream, err);
}


void Ckp2p_check_tool_win32Dlg::dumpH264(const char *path, void *frame, size_t frame_len)
{
	static FILE *fp264 = NULL;
	if (!fp264) {
		fp264 = fopen(path, "w+");
	}
	else {
		fwrite(frame, 1, frame_len, fp264);
	}
}


FILE* Ckp2p_check_tool_win32Dlg::dumpFp[64] = { 0 };

void Ckp2p_check_tool_win32Dlg::OnDumpVideo(int ch, const char *enc, void *frame, size_t frame_size)
{
	char dumpname[64] = { 0 };
	if (!dumpFp[ch]) {
		snprintf(dumpname, sizeof(dumpname), "ch%d.%s", ch, enc);
		dumpFp[ch] = fopen(dumpname, "w+");
		//     printf("OnDumpVideo: open %s %p\n", dumpname, dumpFp[ch]);
	}

	if (dumpFp[ch]) {
		fwrite(frame, frame_size, 1, dumpFp[ch]);
	}
}

void Ckp2p_check_tool_win32Dlg::OnRecvFrameEx(kp2p_handle_t p2p_handle, void *context,
	uint32_t frame_type,
	void *frame, size_t frame_size,
	uint32_t channel,
	uint64_t time_stamp,
	uint32_t width,
	uint32_t height,
	uint32_t sample_rate,
	uint32_t sample_width,
	uint32_t sample_channels,
	const char *enc) 
{

	if (KP2P_FRAME_TYPE_AUDIO == frame_type) {
		CKliveDlg::m_AudioFrameLiveNum += 1;
		::InterlockedIncrement(&CKliveDlg::m_OnPlayAudioLiveCountLock);
		if (::InterlockedDecrement(&CKliveDlg::m_OnPlayAudioLiveCountLock) == 1) {
			CString temp;
			temp.Format(_T("%u"), sample_rate);
			m_This->m_LiveDlg.m_PlayLiveInfoShowListCtrl.SetItemText(5, 1, temp.GetString());
			temp.Format(_T("%u"), sample_width);
			m_This->m_LiveDlg.m_PlayLiveInfoShowListCtrl.SetItemText(6, 1, temp.GetString());
			temp.Format(_T("%u"), sample_channels);
			m_This->m_LiveDlg.m_PlayLiveInfoShowListCtrl.SetItemText(7, 1, temp.GetString());
			temp.Format(_T("%u bps"), sample_rate * sample_width *sample_channels);
			m_This->m_LiveDlg.m_PlayLiveInfoShowListCtrl.SetItemText(8, 1, temp.GetString());
			if (strcmp(enc, "G711A") == 0) {
				temp.SetString(_T("音频编码(G711A)"));
			}
			else {
				temp.SetString(_T("未知类型"));
			}
			m_This->m_LiveDlg.m_PlayLiveInfoShowListCtrl.SetItemText(9, 1, temp.GetString());
			//m_This->m_LiveDlg.m_PlayLiveInfoShowListCtrl.RedrawWindow();

			::InterlockedDecrement(&CKliveDlg::m_OnPlayAudioLiveCountLock);
		}
		CKliveDlg::m_FrameLiveCountTotal += frame_size;
		/*printf("OnRecvRecFrame: enc(%s) size(%lu) ts(%llu) samplerate(%u) samplewidth(%u) channles(%u) genTime(%u)\n",
		enc, frame_size, time_stamp, sample_rate, sample_width, sample_channels, gen_time);*/

}
	else if (KP2P_FRAME_TYPE_IFRAME == frame_type || KP2P_FRAME_TYPE_PFRAME == frame_type) {
		CKliveDlg::m_VedioFrameLiveNum += 1;
		::InterlockedIncrement(&CKliveDlg::m_OnPlayVedioLiveCountLock);
		if (::InterlockedDecrement(&CKliveDlg::m_OnPlayVedioLiveCountLock) == 1) {
			CString temp;
			CKliveDlg::m_GetIFrameLiveEndTime = GetTickCount();
			temp.Format(_T("%u 毫秒"), CKliveDlg::m_GetIFrameLiveEndTime - CKliveDlg::m_GetIFrameLiveStartTime);
			m_This->m_LiveDlg.m_PlayLiveInfoShowListCtrl.SetItemText(12, 1, temp.GetString());

			temp.Format(_T("%u * %u"), width, height);
			m_This->m_LiveDlg.m_PlayLiveInfoShowListCtrl.SetItemText(1, 1, temp.GetString());

			if (strcmp(enc, "H265") == 0) {
				temp.SetString(_T("视频编码(H265)"));
			}
			else if (strcmp(enc, "H264") == 0) {
				temp.SetString(_T("视频编码-H264"));
			}
			else {
				temp.SetString(_T("未知类型"));
			}
			m_This->m_LiveDlg.m_PlayLiveInfoShowListCtrl.SetItemText(2, 1, temp.GetString());
			//m_This->m_LiveDlg.m_PlayLiveInfoShowListCtrl.RedrawWindow();

			::InterlockedDecrement(&CKliveDlg::m_OnPlayVedioLiveCountLock);
		}
		CKliveDlg::m_FrameLiveCountTotal += frame_size;
		/*printf("OnRecvRecFrame: enc(%s) size(%lu) type(%u) ts(%llu) width(%u) height(%u) genTime(%u)\n",
		enc, frame_size, frame_type, time_stamp, width, height, gen_time);*/
	}
	else if (KP2P_FRAME_TYPE_OOB == frame_type) {
		/*printf("OnRecvRecFrame: enc(%s) size(%lu) type(%u) ts(%llu) width(%u) height(%u) genTime(%u)\n",
			enc, frame_size, frame_type, time_stamp, width, height, gen_time);*/
	}

#if 0
	unsigned char *p = (unsigned char *)frame;
	stream_bytes += frame_size;
	if (stream_times == 0) { stream_times = iclock(); }
	if (frame_type == 1) { stream_frame_times_end = iclock(); }

	if (KP2P_FRAME_TYPE_AUDIO == frame_type) {
		//        printf("OnRecvFreamEx: channel(%u) enc(%s) size(%lu) ts("fllu") samplerate(%u) samplewidth(%u) channles(%u)\n",
		//               channel, enc, frame_size, time_stamp, sample_rate, sample_width, sample_channels);

	}
	else if (KP2P_FRAME_TYPE_IFRAME == frame_type || KP2P_FRAME_TYPE_PFRAME == frame_type) {
		//     printf("OnRecvFrameEx: channel(%u) enc(%s) size(%lu) type(%u) ts("fllu") width(%u) height(%u) %02x%02x%02x%02x %02x%02x%02x%02x\n",
		//           channel, enc, frame_size, frame_type, time_stamp, width, height, p[0], p[1], p[2],
		//           p[3], p[4], p[5], p[6], p[7]);
		//        OnDumpVideo(channel, enc, frame, frame_size);
	}
	else if (KP2P_FRAME_TYPE_OOB == frame_type) {
		//    printf("recvFrame OOB: size:%lu\n", frame_size);
	}
	if (record_f != NULL)
	{
		fwrite(frame, 1, frame_size, record_f);
	}
#endif
}

void Ckp2p_check_tool_win32Dlg::OnVOP2PCallResult(kp2p_handle_t p2p_handle, void *context, int result)
{
	//   printf("OnVOP2PCallResult: %p %d\n", p2p_handle, result);
}

void Ckp2p_check_tool_win32Dlg::OnVOP2PHangupResult(kp2p_handle_t p2p_handle, void *context, int result)
{
	//  printf("OnVOP2PHangupResult: %p %d\n", p2p_handle, result);
}

void Ckp2p_check_tool_win32Dlg::OnRecvRecFrame(kp2p_handle_t p2p_handle, void *context,
	uint32_t frame_type,
	void *frame, size_t frame_size, uint32_t channel, uint64_t time_stamp,
	uint32_t width, uint32_t height,
	uint32_t sample_rate, uint32_t sample_width, uint32_t sample_channels,
	uint32_t gen_time,
	const char *enc) 
{

	if (KP2P_FRAME_TYPE_AUDIO == frame_type) {
		CKvideoDlg::m_AudioFrameNum += 1;
		::InterlockedIncrement(&CKvideoDlg::m_OnReplayAudioCountLock);
		if (::InterlockedDecrement(&CKvideoDlg::m_OnReplayAudioCountLock) == 1) {
			CString temp;
			temp.Format(_T("%u"), sample_rate);
			m_This->m_VideoDlg.m_ReplayRecInfoShowListCtrl.SetItemText(5, 1, temp.GetString());
			temp.Format(_T("%u"), sample_width);
			m_This->m_VideoDlg.m_ReplayRecInfoShowListCtrl.SetItemText(6, 1, temp.GetString());
			temp.Format(_T("%u"), sample_channels);
			m_This->m_VideoDlg.m_ReplayRecInfoShowListCtrl.SetItemText(7, 1, temp.GetString());
			temp.Format(_T("%u bps"), sample_rate * sample_width *sample_channels);
			m_This->m_VideoDlg.m_ReplayRecInfoShowListCtrl.SetItemText(8, 1, temp.GetString());
			if (strcmp(enc, "G711A") == 0) {
				temp.SetString(_T("音频编码(G711A)"));
			}
			else {
				temp.SetString(_T("未知类型"));
			}
			m_This->m_VideoDlg.m_ReplayRecInfoShowListCtrl.SetItemText(9, 1, temp.GetString());
			m_This->m_VideoDlg.m_ReplayRecInfoShowListCtrl.RedrawWindow();

			::InterlockedDecrement(&CKvideoDlg::m_OnReplayAudioCountLock);
		}
		CKvideoDlg::m_FrameCountTotal += frame_size;
			/*printf("OnRecvRecFrame: enc(%s) size(%lu) ts(%llu) samplerate(%u) samplewidth(%u) channles(%u) genTime(%u)\n",
			    enc, frame_size, time_stamp, sample_rate, sample_width, sample_channels, gen_time);*/

	}
	else if (KP2P_FRAME_TYPE_IFRAME == frame_type || KP2P_FRAME_TYPE_PFRAME == frame_type) {
		CKvideoDlg::m_VedioFrameNum += 1;
		::InterlockedIncrement(&CKvideoDlg::m_OnReplayVedioCountLock);
		if (::InterlockedDecrement(&CKvideoDlg::m_OnReplayVedioCountLock) == 1) {
			CString temp;
			CKvideoDlg::m_GetIFrameEndTime = GetTickCount();
			temp.Format(_T("%u 毫秒"), CKvideoDlg::m_GetIFrameEndTime - CKvideoDlg::m_GetIFrameStartTime);
			m_This->m_VideoDlg.m_ReplayRecInfoShowListCtrl.SetItemText(12, 1, temp.GetString());

			temp.Format(_T("%u * %u"), width, height);
			m_This->m_VideoDlg.m_ReplayRecInfoShowListCtrl.SetItemText(1, 1, temp.GetString());

			if (strcmp(enc, "H265") == 0) {
				temp.SetString(_T("视频编码(H265)"));
			}
			else if (strcmp(enc, "H264") == 0) {
				temp.SetString(_T("视频编码-H264"));
			}
			else {
				temp.SetString(_T("未知类型"));
			}
			m_This->m_VideoDlg.m_ReplayRecInfoShowListCtrl.SetItemText(2, 1, temp.GetString());
			m_This->m_VideoDlg.m_ReplayRecInfoShowListCtrl.RedrawWindow();

			::InterlockedDecrement(&CKvideoDlg::m_OnReplayVedioCountLock);
		}
		CKvideoDlg::m_FrameCountTotal += frame_size;
			    /*printf("OnRecvRecFrame: enc(%s) size(%lu) type(%u) ts(%llu) width(%u) height(%u) genTime(%u)\n", 
					enc, frame_size, frame_type, time_stamp, width, height, gen_time);*/
	}
	else if (KP2P_FRAME_TYPE_OOB == frame_type) {
		printf("OnRecvRecFrame: enc(%s) size(%lu) type(%u) ts(%llu) width(%u) height(%u) genTime(%u)\n",
		enc, frame_size, frame_type, time_stamp, width, height, gen_time);
	}


#if 0

	if (frame_type == 1) { rreplay_frame_times_end = iclock(); }
	if (record_f == NULL)
	{
		replay_stream_times_end = replay_stream_times_end == 0 ? iclock() : replay_stream_times_end;


		if (KP2P_FRAME_TYPE_AUDIO == frame_type) {
			//    printf("OnRecvRecFrame: enc(%s) size(%lu) ts("fllu") samplerate(%u) samplewidth(%u) channles(%u) genTime(%u)\n",
			//        enc, frame_size, time_stamp, sample_rate, sample_width, sample_channels, gen_time);

		}
		else if (KP2P_FRAME_TYPE_IFRAME == frame_type || KP2P_FRAME_TYPE_PFRAME == frame_type) {
			//      printf("OnRecvRecFrame: enc(%s) size(%lu) type(%u) ts("fllu") width(%u) height(%u) genTime(%u)\n",
			//          enc, frame_size, frame_type, time_stamp, width, height, gen_time);
		}
	}
	else
	{
		if (KP2P_FRAME_TYPE_IFRAME == frame_type || KP2P_FRAME_TYPE_PFRAME == frame_type) {
			//               printf("OnRecvRecFrame: enc(%s) size(%lu) type(%u) ts("fllu") width(%u) height(%u) genTime(%u)\n",
			//                  enc, frame_size, frame_type, time_stamp, width, height, gen_time);
		}
		fwrite(frame, 1, frame_size, record_f);
	}

#endif
}


void Ckp2p_check_tool_win32Dlg::OnLoginResult(kp2p_handle_t p2p_handle, void *context, int ret)
{
	    //printf("OnLoginResult Handle:%p ret:%d\n", p2p_handle, ret);
	//login_times = iclock();
	CString info;
	info.Format(L"[OnLoginResult]Handle:%p, ret:%d", p2p_handle, ret);
	//SHOW_STATUS_INFO_S_W(info.GetString());
	//SHOW_STATUS_INFO_S_A("kp2p_login登录成功");
}


void Ckp2p_check_tool_win32Dlg::OnPtzResult(kp2p_handle_t p2p_handle, void *context, int ret)
{
	    //printf("OnPtzResult Handle:%p ret:%d\n", p2p_handle, ret);
}


void Ckp2p_check_tool_win32Dlg::OnVconRecv(kp2p_handle_t p2p_handle, void *context, uint32_t channel, const void *buf, size_t len)
{
	//    printf("OnVconRecv(p2p_handle=%p, channel=%u, buf=%s, data_size=%lu\n", p2p_handle, channel,
	//           (const char *) buf, len);
}


void Ckp2p_check_tool_win32Dlg::OnRemoteSetup(kp2p_handle_t p2p_handle, void *context, const void *data, size_t data_size)
{
	    //printf("%s %d | %s\n", __FUNCTION__, __LINE__, (const char *) data);
	//setup_res_times = iclock();
}


void Ckp2p_check_tool_win32Dlg::OnLogUpload(kp2p_handle_t p2p_handle, void *context, const char *log, size_t log_len)
{
	//    printf("OnLogUpload: %s\n", log);
	CString info;
	info.Format(L"[OnLogUpload]Handle:%p, log:%s\n", p2p_handle, log);
}

void Ckp2p_check_tool_win32Dlg::OnP2PError(kp2p_handle_t p2p_handle, void *context, const char *msg, size_t msg_len)
{
	CString info;
	info.Format(L"[OnP2PError]Handle:%p, type:%s\n", p2p_handle, msg);
	//SHOW_STATUS_INFO_S_W(info.GetString());
}

void Ckp2p_check_tool_win32Dlg::OnBnClickedConnectButton()
{
	UpdateData(TRUE);
	
	INT_PTR         nRes;

#if 0
	CEdit           *pEditDevID = NULL;
	CEdit           *pEditDevUser = NULL;

	if (m_EditDevID.IsEmpty() || m_EditDevUser.IsEmpty()) {	
		nRes = MessageBox(_T("设备ID或用户名不能为空"), _T("信息提示"), MB_OK);
		if (IDOK == nRes) {
			if (m_EditDevID.IsEmpty()) {
				pEditDevID = (CEdit *)GetDlgItem(IDC_ID_DEV_EDIT);
				pEditDevID->SetFocus();
			
			}
			else {
				pEditDevUser = (CEdit *)GetDlgItem(IDC_USER_DEV_EDIT);
				pEditDevUser->SetFocus();
			}
		}
		return;
	}

	if (m_EditDevID.GetLength() < 9) {
		nRes = MessageBox(_T("请输入正确的设备ID"), _T("信息提示"), MB_OK);
		if (IDOK == nRes) {
			m_EditDevID.Empty();
			UpdateData(FALSE);
			pEditDevID = (CEdit *)GetDlgItem(IDC_ID_DEV_EDIT);
			pEditDevID->SetFocus();
		}
		return;
	}
#endif

	m_ComboBoxDevIDItem.GetWindowText(m_EditDevID);
	m_ComboBoxDevUserItem.GetWindowText(m_EditDevUser);
	m_ComboBoxSvrUserItem.GetWindowText(m_EditSvrUser);

	if (m_nCurRestartDevIDCount == 1) {
		if (m_EditDevID.Compare(m_sCurRestartDevID) == 0) {
			::InterlockedIncrement(&m_OnOperateControlCountLock);
			if (::InterlockedDecrement(&m_OnOperateControlCountLock) != 0) {
				SHOW_STATUS_INFO_A("该设备正在重新启动中，请等待");
				return;
			}
			else {
				m_nCurRestartDevIDCount = 0;
			}
		}
	}

	if (m_EditDevID.IsEmpty() || m_EditDevUser.IsEmpty()) {
		nRes = MessageBox(_T("设备ID或用户名不能为空"), _T("信息提示"), MB_OK);
		if (IDOK == nRes) {
			if (m_EditDevID.IsEmpty()) {
				m_ComboBoxDevIDItem.SetFocus();

			}
			else {
				m_ComboBoxDevUserItem.SetFocus();
			}
		}
		return;
	}

	if (m_EditDevID.GetLength() < 9) {
		nRes = MessageBox(_T("请输入正确的设备ID"), _T("信息提示"), MB_OK);
		if (IDOK == nRes) {
			m_ComboBoxDevIDItem.SetFocus();
		}
		return;
	}
	m_QueryDevInfoDlg.m_ListCtrlQueryInfo.DeleteAllItems();
	m_QueryDevInfoDlg.m_ListCtrlQueryInfo.RedrawWindow();

	SHOW_STATUS_INFO_A("获取设备信息：");
	SHOW_CURRENT_STATUS_INFO_A("获取设备信息中...");
	//查询当前设备信息
	INT nErr = check_cur_devid_status_info_once();
	if (nErr == -1) {
		SHOW_STATUS_INFO_A("设备离线");
		SHOW_STATUS_INFO_A("获取设备信息结束");	
		SHOW_STATUS_INFO_A(" ");
		SHOW_STATUS_INFO_A("设备离线，无法连接");
		return;
	}
	else if (nErr == -4) {
		SHOW_STATUS_INFO_A("获取失败");
		SHOW_STATUS_INFO_A("获取设备信息结束");
		SHOW_CURRENT_STATUS_INFO_A("获取设备信息失败");
		return;
	}
	SHOW_STATUS_INFO_A("设备在线");
	SHOW_STATUS_INFO_A("获取设备信息结束");
	SHOW_STATUS_INFO_A(" ");
	SHOW_STATUS_INFO_A("设备在线，开始连接");
	SHOW_CURRENT_STATUS_INFO_A("设备连接中...");
	SHOW_STATUS_INFO_W(CA2W(kp2p_get_version()));

	kp2p_set_log(KP2P_LOG_ALL, "./", 1000 * 1000);

	m_This = this;
	m_bReadyStartFlag = FALSE;

	//设备连接初始化
	if (!p2p_context_init()) {

		SHOW_STATUS_INFO_A("连接初始化失败");
		SHOW_CURRENT_STATUS_INFO_A("连接设备失败");
		return;
	}
	SHOW_CURRENT_STATUS_INFO_A("设备连接成功");
	SHOW_STATUS_INFO_A("设备连接初始化成功");
	m_bDevConnectStatusFlag = TRUE;

	m_ComboBoxDevIDItem.EnableWindow(FALSE);
	m_ComboBoxDevUserItem.EnableWindow(FALSE);
	(CEdit *)GetDlgItem(IDC_PASSD_DEV_EDIT)->EnableWindow(FALSE);

	//获取本机所有tcp网络连接信息
	get_tcp_network_info_init(TURN_SERVER_IP);
	//获取当前鉴权mac地址
	get_cur_mac_init();

	vector<CString>::iterator it;
	if (!m_DevIDHistoryVec.empty()) {
		it = find(m_DevIDHistoryVec.begin(), m_DevIDHistoryVec.end(), m_EditDevID);
		if (it == m_DevIDHistoryVec.end()) {
			//m_DevIDHistoryVec.push_back(m_EditDevID);
			//m_ComboBoxDevIDItem.AddString(m_EditDevID.GetString());
			m_DevIDHistoryVec.insert(m_DevIDHistoryVec.begin(), m_EditDevID);
			if (m_DevIDHistoryVec.size() >= 10) {
				m_LoginHistoryCmd = 1;
				SetEvent(m_LoginInfoHistoryCacheThreadUpdateEvent);
			}
			else {
				m_DevIDHistoryVec.push_back(m_EditDevID);
				m_ComboBoxDevIDItem.AddString(m_EditDevID.GetString());
			}
		}
	}
	else {
		m_DevIDHistoryVec.push_back(m_EditDevID);
		m_ComboBoxDevIDItem.AddString(m_EditDevID.GetString());
	}

	if (!m_DevUserHistoryVec.empty()) {
		it = find(m_DevUserHistoryVec.begin(), m_DevUserHistoryVec.end(), m_EditDevUser);
		if (it == m_DevUserHistoryVec.end()) {
			//m_DevUserHistoryVec.push_back(m_EditDevUser);
			//m_ComboBoxDevUserItem.AddString(m_EditDevUser.GetString());
			m_DevUserHistoryVec.insert(m_DevUserHistoryVec.begin(), m_EditDevUser);
			if (m_DevUserHistoryVec.size() >= 10) {
				m_LoginHistoryCmd = 2;
				SetEvent(m_LoginInfoHistoryCacheThreadUpdateEvent);
			}
			else {
				m_DevUserHistoryVec.push_back(m_EditDevUser);
				m_ComboBoxDevUserItem.AddString(m_EditDevUser.GetString());
			}
		}
	}
	else {
		m_DevUserHistoryVec.push_back(m_EditDevUser);
		m_ComboBoxDevUserItem.AddString(m_EditDevUser.GetString());
	}


#if 0
	//UpdateData(FALSE);

	/*if (!m_SvrUserHistoryVec.empty()) {
		it = m_SvrUserHistoryVec.begin();
		for (; it != m_DevIDHistoryVec.end(); it++) {
			m_ComboBoxSvrUserItem.AddString((*it).GetString());
		}
	}*/
#endif
	//保存设备配置信息结构初始化
	config_info_init();
	//保存设备状态信息结构初始化
	status_info_init();

	(CButton *)GetDlgItem(IDC_DISCONNECT_BUTTON)->EnableWindow(TRUE);
	//m_BtnDisconnect->EnableWindow(TRUE);
	(CButton *)GetDlgItem(IDC_MOD_CONFIG_BUTTON)->EnableWindow(TRUE);
	//m_BtnDevCfgModify->EnableWindow(TRUE);
	(CButton *)GetDlgItem(IDC_VEDIO_BUTTON)->EnableWindow(TRUE);
	//m_BtnVideo->EnableWindow(TRUE);
	(CButton *)GetDlgItem(IDC_LIVE_BUTTON)->EnableWindow(TRUE);
	//m_BtnLive->EnableWindow(TRUE);
	//m_BtnGetDEVStatusInfo->EnableWindow(TRUE);
	//m_BtnDevSysTimeModify->EnableWindow(TRUE);
	(CButton *)GetDlgItem(IDC_RESTART_DEV_BUTTON)->EnableWindow(TRUE);
	//m_BtnRestartDev->EnableWindow(TRUE);
	//m_BtnRemoteSetting->EnableWindow(TRUE);
	//m_BtnExportData->EnableWindow(TRUE);

	m_bReadyStartFlag = TRUE;
	m_bReadyLoadFlag = TRUE;
	if (m_bReadyStartFlag && m_bReadyLoadFlag) {
		//m_BtnStartTest->EnableWindow(TRUE);
		(CButton *)GetDlgItem(IDC_START_CHECK_MAIN_BUTTON)->EnableWindow(TRUE);
		//m_BtnStartCheck->EnableWindow(TRUE);
		(CButton *)GetDlgItem(IDC_STOP_CHECK_MAIN_BUTTON)->EnableWindow(TRUE);
		//m_BtnStopCheck->EnableWindow(TRUE);
	}
	else {
		//m_BtnStartTest->EnableWindow(FALSE);
		(CButton *)GetDlgItem(IDC_START_CHECK_MAIN_BUTTON)->EnableWindow(FALSE);
		//m_BtnStartCheck->EnableWindow(FALSE);
	}

	m_BtnConnect->EnableWindow(FALSE);
#if 0
	//m_This = this;

	/*try {
		start_work_thread();
	}
	catch (exception &e) {
		CString err;
		err = e.what();
		SHOW_STATUS_INFO_W(err.GetString());
	}*/
#endif
	return;
}

void Ckp2p_check_tool_win32Dlg::start_work_thread()
{
	if (m_ExecHandleThr == NULL) {
		m_ExecHandleThr = (HANDLE)_beginthreadex(NULL, 0, Ckp2p_check_tool_win32Dlg::ExcuteCmdWorkThreadProc, this, CREATE_SUSPENDED, NULL);
		if (m_ExecHandleThr == 0) {
			//printlog("CreateThread failed. LastError: %u\n", GetLastError());
			//free(t);
			throw MyException();
		}
		::ResumeThread(m_ExecHandleThr);
	}

	if (m_ModHandleThr == NULL) {
		m_ModHandleThr = (HANDLE)_beginthreadex(NULL, 0, Ckp2p_check_tool_win32Dlg::ModifyDevConfigWorkThreadProc, this, CREATE_SUSPENDED, NULL);
		if (m_ModHandleThr == 0) {
			//printlog("CreateThread failed. LastError: %u\n", GetLastError());
			throw MyException();
		}
		::ResumeThread(m_ModHandleThr);
	}
#if 0
	/*m_CacheHandleThr = (HANDLE)_beginthreadex(NULL, 0, Ckp2p_check_tool_win32Dlg::SaveCacheWorkThreadProc, this, CREATE_SUSPENDED, NULL);
	if (m_ModHandleThr == 0) {
		throw MyException();
	}
	::ResumeThread(m_CacheHandleThr);*/
#endif

	if (m_LoginHistoryCacheHandleThr == NULL) {
		m_LoginHistoryCacheHandleThr = (HANDLE)_beginthreadex(NULL, 0, Ckp2p_check_tool_win32Dlg::LoginInfoHistoryCacheWorkThreadProc, this, CREATE_SUSPENDED, NULL);
		if (m_ModHandleThr == 0) {
			throw MyException();
		}
		::ResumeThread(m_LoginHistoryCacheHandleThr);
	}

	if (m_OperateControlHandleThr == NULL) {
		m_OperateControlHandleThr = (HANDLE)_beginthreadex(NULL, 0, Ckp2p_check_tool_win32Dlg::OperateControlWorkThreadProc, this, CREATE_SUSPENDED, NULL);
		if (m_ModHandleThr == 0) {
			throw MyException();
		}
		::ResumeThread(m_OperateControlHandleThr);
	}
	
	if (m_CheckDevOfflineStatusHandleThr == NULL) {
		m_CheckDevOfflineStatusHandleThr = (HANDLE)_beginthreadex(NULL, 0, Ckp2p_check_tool_win32Dlg::CheckDevOfflineStatusWorkThreadProc, this, CREATE_SUSPENDED, NULL);
		if (m_ModHandleThr == 0) {
			throw MyException();
		}
		::ResumeThread(m_CheckDevOfflineStatusHandleThr);
	}
}

BOOL Ckp2p_check_tool_win32Dlg::p2p_context_init()
{
	kp2p_callback_t cb = { 0 };

	int            result = 0;
	int            login_result = -1;
	int            channel = 0;
	uint32_t       sessionid = 0;

	time_t shell_end_times;
	time_t shell_start_times;
	CString info;

	cb.OnConnect = OnConnect;
	cb.OnDisconnect = OnDisconnect;
	cb.OnLoginResult = OnLoginResult;
	cb.OnPtzResult = OnPtzResult;
	cb.OnOpenStream = OnOpenStream;
	cb.OnCloseStream = OnCloseStream;
	cb.OnRecvFrameEx = OnRecvFrameEx;
	cb.OnVOP2PCallResult = OnVOP2PCallResult;
	cb.OnVOP2PHangupResult = OnVOP2PHangupResult;
	cb.OnRecvRecFrame = OnRecvRecFrame;
	cb.OnRemoteSetup = OnRemoteSetup;
	cb.OnVconRecv = OnVconRecv;
	cb.OnP2PError = OnP2PError;
	cb.OnLogUpload = OnLogUpload;

	SHOW_STATUS_INFO_A("初始化中，请等待");
	int nRet = kp2p_init(&cb);
	if (nRet != 0) {
		SHOW_STATUS_INFO_A("kp2p_init初始化失败");
		//kp2p_exit();
		return FALSE;
	}
	SHOW_STATUS_INFO_A("kp2p_init初始化成功");

	channel = m_Channel;
	BOOL bRet = check_p2p_ex(CW2A(m_EditDevID.GetString()), "", CW2A(m_EditDevUser.GetString()) , CW2A(m_EditDevPassword.GetString()), 0, channel);
	if (!bRet) {
		return FALSE;
	}

	m_nReShellLoginCount = 0;

SHELL_LOGIN:

	/*bRet = init_from_config_ini();
	if (!bRet) {
		SHOW_STATUS_INFO_A("init_from_config_ini获取config.ini配置信息失败");
	}*/

	if (m_Shell == NULL) {
		m_Shell = IOT_SHELL_Init();
		if (!m_Shell) {
			SHOW_STATUS_INFO_A("IOT_SHELL_Init初始化失败");
			return FALSE;
		}
		SHOW_STATUS_INFO_A("IOT_SHELL_Init初始化成功");
	}
	else {
		SHOW_STATUS_INFO_A("IOT_SHELL_Init已初始化");
	}
	//kp2p_set_turn_server("118.190.84.189:19999");
	/*CString turnsvr_address;
	turnsvr_address.Format(L"%s:%s", m_TurnServerIp.GetString(), m_TurnServerPort.GetString());
	IOT_SHELL_SetTurnServer(CW2A(turnsvr_address.GetString()));*/
	IOT_SHELL_SetTurnServer("118.190.84.189:19999");

	IOT_LINK_ShellHooks sh;
	sh.OnShellData = test_shell_OnShellData;
	/*IOT_LINK_ShellHooks sh = {
	.OnShellData = test_shell_OnShellData
	};*/

	shell_start_times = GetTickCount();
	m_Session = IOT_SHELL_Login(m_Shell, CW2A(m_EditDevID.GetString()), "", &sh, 10 * 1000, NULL, &result);

	if (!m_Session)
	{
		if (m_Shell) {
			IOT_SHELL_Deinit(&m_Shell);
		}        
		SHOW_STATUS_INFO_A("IOT_SHELL_Login登录失败");
		CString temp;
		temp.Format(_T("正在尝试重新登录，第%d次"), m_nReShellLoginCount + 1);
		SHOW_STATUS_INFO_W(temp.GetString());
		m_nReShellLoginCount++;
		if (m_nReShellLoginCount < 3)
			goto SHELL_LOGIN;
		return FALSE;
	}
	SHOW_STATUS_INFO_A("IOT_SHELL_Login登录成功");
	shell_end_times = GetTickCount();
	info.Empty();
	info.Format(L"IOT_SHELL_Login连接耗时: %d 毫秒", shell_end_times - shell_start_times);
	SHOW_STATUS_INFO_W(info.GetString());

	m_bShellLogin = TRUE;
	return TRUE;

//FAIL_END:
//
//	if (m_Handle) {
//		kp2p_close(m_Handle);
//		kp2p_exit();
//	}

	return FALSE;
}

void Ckp2p_check_tool_win32Dlg::config_info_init()
{
	if (!m_DevConfigInfoMap.empty()) {
		map<LONG64, pkp2p_dev_config_t>::iterator pos;
		pos = m_DevConfigInfoMap.find(atoll(CW2A(m_EditDevID.GetString())));
		if (pos != m_DevConfigInfoMap.end()) {
			return;
		}
	}

	kp2p_dev_config_t *pcfg = (kp2p_dev_config_t *)malloc(sizeof(kp2p_dev_config_t));
	memset(pcfg->id, 0, 20);
	memset(pcfg->user, 0, 20);
	memset(pcfg->password, 0, 20);
	memset(pcfg->mtu, 0, 10);
	memset(pcfg->gateway, 0, 20);
	memset(pcfg->dns, 0, 20);
	memset(pcfg->signal, 0, 50);
	pcfg->mem_use = { 0 };
	pcfg->cpu_use = { 0 };
	pcfg->process = { 0 };
	pcfg->thread = { 0 };
	memcpy(pcfg->id, CW2A(m_EditDevID.GetString()), m_EditDevID.GetLength());
	memcpy(pcfg->user, CW2A(m_EditDevUser.GetString()), m_EditDevUser.GetLength());
	memcpy(pcfg->password, CW2A(m_EditDevPassword.GetString()), m_EditDevPassword.GetLength());
	push_info_in_queue(DEV_CONFIG, atoll(CW2A(m_EditDevID.GetString())), pcfg);

}

void Ckp2p_check_tool_win32Dlg::status_info_init()
{
	if (!m_DevStatusInfoMap.empty()) {
		map<LONG64, pkp2p_dev_status_t>::iterator pos;
		pos = m_DevStatusInfoMap.find(atoll(CW2A(m_EditDevID.GetString())));
		if (pos != m_DevStatusInfoMap.end()) {
			return;
		}
	}

	kp2p_dev_status_t *pstatus = (kp2p_dev_status_t*)malloc(sizeof(kp2p_dev_status_t));
	memset(pstatus->id, 0, 20);
	memset(pstatus->user, 0, 20);
	memset(pstatus->password, 0, 20);
	pstatus->bandwidth_info = { 0 };
	pstatus->network_info = { 0 };
	pstatus->ping_info = { 0 };
	pstatus->route_info = { 0 };
	memcpy(pstatus->id, CW2A(m_EditDevID.GetString()), m_EditDevID.GetLength());
	memcpy(pstatus->user, CW2A(m_EditDevUser.GetString()), m_EditDevUser.GetLength());
	memcpy(pstatus->password, CW2A(m_EditDevPassword.GetString()), m_EditDevPassword.GetLength());
	push_info_in_queue(DEV_STATUS, atoll(CW2A(m_EditDevID.GetString())), pstatus);
	
}

void Ckp2p_check_tool_win32Dlg::info_deinit()
{

}

BOOL Ckp2p_check_tool_win32Dlg::Run_Check()
{
	unsigned char decrypt[16];
	char localmac[32];
	memset(localmac, 0x00, 32);
	memset(decrypt, 0x00, 16);
	//memset(&m_ReqData, 0x00, sizeof(auth_psd_req_data_t));

	if (!m_TestItemVec.empty() || !m_TestStatusItemVec.empty()) {
		m_bShellSendReqFlag = FALSE;
		if (!m_bShellSendReqFlag) {
			memset(&m_ReqData, 0x00, sizeof(auth_psd_req_data_t));
			MD5Init(&m_Md5);
			MD5Update(&m_Md5, (unsigned char*)((char*)(CW2A(m_EditSvrPassword.GetString()))), m_EditSvrPassword.GetLength());
			MD5Final(&m_Md5, decrypt);

			get_mac(localmac);
			memcpy(m_ReqData.uid, CW2A(m_EditDevID.GetString()), m_EditDevID.GetLength());
			memcpy(m_ReqData.usr, CW2A(m_EditSvrUser.GetString()), m_EditSvrUser.GetLength());
			memcpy(m_ReqData.pwd, decrypt, 16);
			memcpy(m_ReqData.ver, "1", strlen("1"));
			memcpy(m_ReqData.mac, localmac, strlen(localmac));
			m_bShellSendReqFlag = TRUE;
		}

		SetEvent(m_This->m_ExecThreadStartCmdEvent);

	}

	return TRUE;
}

LPCTSTR Ckp2p_check_tool_win32Dlg::int_to_string(int arg, LPCTSTR input)
{
	//char * putout = NULL;
	//TCHAR * putout2 = NULL;
	CString info;

	//info = input;
	info.Format(L"%s: %d", input, arg);
	/*_itoa(arg, putout, 10);
	int iLength = MultiByteToWideChar(CP_ACP, 0, putout, strlen(putout) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, putout, strlen(putout) + 1, putout2, iLength);
	info += putout2;*/
	return info.GetBuffer(0);
}

BOOL Ckp2p_check_tool_win32Dlg::check_p2p_ex(const char *id, const char *ip, const char *user, const char *passwd, int func, int channel)
{
	int     ret = -1;
	int     login_result = -1;
	CString info;

	m_Handle = NULL;
	m_Handle = kp2p_create(NULL);
	if (!m_Handle) {
		SHOW_STATUS_INFO_A("kp2p_create创建句柄失败");
		m_Handle = NULL;
		return FALSE;
	}
	SHOW_STATUS_INFO_A("kp2p_create创建句柄成功");

	//IOT_SHELL_SetTurnServer("118.190.84.189:19999");

	uint32_t start_times = GetTickCount();
	uint32_t end_times;
	//login_result = kp2p_connect_v2(m_Handle, id, ip, 10000);
	login_result = kp2p_connect(m_Handle, id, ip, 10000);
	if (login_result != 0) {
		info.Empty();
		info.Format(L"kp2p_connect连接失败 返回值:%d", login_result);
		SHOW_STATUS_INFO_W(info.GetString());
		if (login_result == -12) {
			SHOW_STATUS_INFO_A("设备鉴权失败，请检查设备ID，用户名或密码是否正确");
		}
	
		end_times = GetTickCount();
		info.Empty();
		info.Format(L"kp2p_connect连接耗时: %d 毫秒", end_times - start_times);
		SHOW_STATUS_INFO_W(info.GetString());
		goto FAIL_END;
	}
	end_times = GetTickCount();
	info.Empty();
	info.Format(L"kp2p_connect连接耗时: %d 毫秒", end_times - start_times);
	SHOW_STATUS_INFO_W(info.GetString());

	//kp2p_getsessionid(handle, &sessionid);
	//SHOW_STATUS_INFO(int_to_string(sessionid, L"会话ID："));

	uint32_t login_start_times = GetTickCount();
	uint32_t login_end_times;
	ret = kp2p_login(m_Handle, user, passwd);
	if (0 != ret) {
		info.Empty();
		info.Format(L"kp2p_login登录失败 返回值：%d", ret);
		SHOW_STATUS_INFO_W(info.GetString());
		if (ret == -20) {
			SHOW_STATUS_INFO_A("设备鉴权失败，用户名错误");
		}

		login_end_times = GetTickCount();
		info.Empty();
		info.Format(L"kp2p_login连接耗时: %d 毫秒", login_end_times - login_start_times);
		SHOW_STATUS_INFO_W(info.GetString());

		goto FAIL_END;
	}
	login_end_times = GetTickCount();
	SHOW_STATUS_INFO_A("kp2p_login登录成功");
	info.Empty();
	info.Format(L"kp2p_login连接耗时: %d 毫秒", login_end_times - login_start_times);
	SHOW_STATUS_INFO_W(info.GetString());

	return TRUE;

FAIL_END:

	if (m_Handle) {
		kp2p_close(m_Handle);
		kp2p_exit();
	}

	return FALSE;
}


void Ckp2p_check_tool_win32Dlg::OnBnClickedCannelTestButton()
{
	// TODO: 在此添加控件通知处理程序代码
}

INT Ckp2p_check_tool_win32Dlg::get_tcp_network_info_init(LPCWSTR item)
{
	// Declare and initialize variables  
	//PMIB_TCPTABLE_OWNER_PID m_pTcpTable;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;
	char szLogBuffer[512] = { 0 };
	char szLocalAddr[128];
	char szRemoteAddr[128];
	struct in_addr IpAddr;
	int i, type_cmd = -1;

	if (lstrcmp(item, _T("all")) == 0) {
		type_cmd = 0;
	}
	else if (lstrcmp(item, TURN_SERVER_IP) == 0) {
		type_cmd = 1;
	}

	m_pTcpTable = (MIB_TCPTABLE_OWNER_PID *)MALLOC(sizeof(MIB_TCPTABLE_OWNER_PID));
	if (m_pTcpTable == NULL) {
		//printf("Error allocating memory\n");
		return -1;
	}

	dwSize = sizeof(MIB_TCPTABLE_OWNER_PID);
	// Make an initial call to GetTcpTable to  
	// get the necessary size into the dwSize variable  
	if ((dwRetVal = GetExtendedTcpTable(m_pTcpTable, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0)) ==
		ERROR_INSUFFICIENT_BUFFER) {
		FREE(m_pTcpTable);
		m_pTcpTable = (MIB_TCPTABLE_OWNER_PID *)MALLOC(dwSize);
		if (m_pTcpTable == NULL) {
			//printf("Error allocating memory\n");
			return -1;
		}
	}
	// Make a second call to GetTcpTable to get  
	// the actual data we require  
	if ((dwRetVal = GetExtendedTcpTable(m_pTcpTable, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0)) == NO_ERROR) {
		switch (type_cmd) 
		{
		case 0:
			break;
		case 1:
			for (i = 0; i < (int)m_pTcpTable->dwNumEntries; i++) {	

				IpAddr.S_un.S_addr = (u_long)m_pTcpTable->table[i].dwLocalAddr;
				strcpy_s(szLocalAddr, sizeof(szLocalAddr), inet_ntoa(IpAddr));
				//printf("\tTCP[%d] Local Addr: %s\n", i, szLocalAddr);
				//printf("\tTCP[%d] Local Port: %d \n", i, ntohs((u_short)m_pTcpTable->table[i].dwLocalPort));

				IpAddr.S_un.S_addr = (u_long)m_pTcpTable->table[i].dwRemoteAddr;
				strcpy_s(szRemoteAddr, sizeof(szRemoteAddr), inet_ntoa(IpAddr));

				int widesize = MultiByteToWideChar(CP_ACP, 0, szRemoteAddr, -1, NULL, 0);
				CString temp_str;
				int count = MultiByteToWideChar(CP_ACP, 0, szRemoteAddr, -1, temp_str.GetBuffer(MAX_PATH), widesize);

				//printf("\tTCP[%d] Remote Addr: %s\n", i, szRemoteAddr);
				//printf("\tTCP[%d] Remote Port: %d\n", i, ntohs((u_short)m_pTcpTable->table[i].dwRemotePort));
				//printf("\tTCP[%d] Pid: %d\n", i, ntohs((u_short)m_pTcpTable->table[i].dwOwningPid));
				if (temp_str.Compare(item) == 0) {
					m_sCurLocalIP = szLocalAddr;
					break;
				}
			}
			break;
		default:
			break;
		}
		
	}
	else {
		//printf("\tGetTcpTable failed with %d\n", dwRetVal);
		FREE(m_pTcpTable);
		return -1;
	}

	/*if (pTcpTable != NULL) {
		FREE(pTcpTable);
		pTcpTable = NULL;
	}*/

	return 0;
}

INT Ckp2p_check_tool_win32Dlg::get_mac_info_init()
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(sizeof(IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL)
	{
		printf("Error allocating memory needed to call GetAdaptersinfo\n");
		return -1;
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL)
		{
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
			return -1;    //    error data return
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
	{
		pAdapter = pAdapterInfo;
		while (pAdapter)
		{
			MyAdpterInfo info;
			info.Name = std::string(pAdapter->AdapterName);
			info.Description = std::string(pAdapter->Description);
			info.Type = pAdapter->Type;
			char buffer[20];
			sprintf_s(buffer, "%02x:%02x:%02x:%02x:%02x:%02x", pAdapter->Address[0],
				pAdapter->Address[1], pAdapter->Address[2], pAdapter->Address[3],
				pAdapter->Address[4], pAdapter->Address[5]);
			info.MacAddress = std::string(buffer);
			IP_ADDR_STRING *pIpAddrString = &(pAdapter->IpAddressList);
			do
			{
				info.Ip.push_back(std::string(pIpAddrString->IpAddress.String));
				pIpAddrString = pIpAddrString->Next;
			} while (pIpAddrString);
			pAdapter = pAdapter->Next;
			m_AdpterInfo.push_back(info);
		}
		if (pAdapterInfo)
			FREE(pAdapterInfo);

		//FILE *file;
		//char *temp = NULL;
		//std::string ip_info;
		//char ptr[1024] = { 0 };
		//char cmd[1024] = { 0 };
		//strcat(ptr, "netstat -na | findstr 118.190.84.189");

		//if ((file = _popen(ptr, "r")) != NULL)
		//{
		//	while ((temp = fgets(cmd, 1024, file)) != NULL) {    
		//	}
		//	_pclose(file);
		//}
		//ip_info = ip_info;

		//return 0;    // normal return
	}
	else
	{
		if (pAdapterInfo)
			FREE(pAdapterInfo);
		//printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);
		return -1;    //    null data return
	}

	return 0;
}

void Ckp2p_check_tool_win32Dlg::get_cur_mac_init()
{
	for (size_t i = 0; i < m_AdpterInfo.size(); ++i)
	{
		for (size_t j = 0; j < m_AdpterInfo[i].Ip.size(); ++j)
		{
			if (j != 0)
			{
				//std::cout << ", ";
			}

			int widesize = MultiByteToWideChar(CP_ACP, 0, m_AdpterInfo[i].Ip[j].c_str(), -1, NULL, 0);
			CString temp_str;
			MultiByteToWideChar(CP_ACP, 0, m_AdpterInfo[i].Ip[j].c_str(), -1, temp_str.GetBuffer(MAX_PATH), widesize);
			if (m_sCurLocalIP.Compare(temp_str) == 0) {
				//memcpy(mac, m_AdpterInfo[i].MacAddress.c_str(), m_AdpterInfo[i].MacAddress.size());
				widesize = MultiByteToWideChar(CP_ACP, 0, m_AdpterInfo[i].MacAddress.c_str(), -1, NULL, 0);
				CString mac_str;
				MultiByteToWideChar(CP_ACP, 0, m_AdpterInfo[i].MacAddress.c_str(), -1, temp_str.GetBuffer(MAX_PATH), widesize);
				m_LableCurMAC.SetWindowTextW(temp_str.GetString());
				mac_str.Format(_T("%s:%s"), _T("当前鉴权mac地址"), temp_str.GetString());
				m_LableCurMAC.SetWindowTextW(mac_str.GetString());
				goto END_MAC;
			}

		}

	}



END_MAC:

	return ;
}

int Ckp2p_check_tool_win32Dlg::get_mac(char* mac)
{
	//char mac_addr[30];
#ifndef _WIN32
	char mac_addr[30];
	int sockfd;
	struct ifreq tmp;


	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		//   perror("create socket fail\n");
		return -1;
	}
	////////////////////

	int interfaceNum = 0;
	struct ifreq buf[16];
	struct ifconf ifc;
	//    struct ifreq ifrcopy;

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = (caddr_t)buf;
	if (!ioctl(sockfd, SIOCGIFCONF, (char *)&ifc))
	{
		interfaceNum = ifc.ifc_len / sizeof(struct ifreq);
		//     printf("interface num = %dn", interfaceNum);
		if (interfaceNum-- > 0)
		{
			//       printf("ndevice name: %sn", buf[interfaceNum].ifr_name);


			/////////////////


			memset(&tmp, 0, sizeof(struct ifreq));
			//    strncpy(tmp.ifr_name,"ens33",sizeof(tmp.ifr_name)-1);
			strcpy(tmp.ifr_name, buf[interfaceNum].ifr_name);
			if ((ioctl(sockfd, SIOCGIFHWADDR, &tmp)) < 0)
			{
				//    printf("mac ioctl error\n");
				return -1;
			}

			sprintf(mac_addr, "%02x:%02x:%02x:%02x:%02x:%02x",
				(unsigned char)tmp.ifr_hwaddr.sa_data[0],
				(unsigned char)tmp.ifr_hwaddr.sa_data[1],
				(unsigned char)tmp.ifr_hwaddr.sa_data[2],
				(unsigned char)tmp.ifr_hwaddr.sa_data[3],
				(unsigned char)tmp.ifr_hwaddr.sa_data[4],
				(unsigned char)tmp.ifr_hwaddr.sa_data[5]
			);
			//  printf("local mac11111111111111111111111111:%s\n", mac_addr);
			close(sockfd);
			memcpy(mac, mac_addr, strlen(mac_addr));
		}
	}
#else
	/*IP_ADAPTER_INFO AdapterInfo[16];
	DWORD dwBuflen = sizeof(AdapterInfo);

	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBuflen);

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
	if (NULL == pAdapterInfo)
		return -1;
	BYTE* MACData = pAdapterInfo->Address;
	sprintf(mac_addr, "%02x:%02x:%02x:%02x:%02x:%02x",
		MACData[0], MACData[1], MACData[2], MACData[3], MACData[4], MACData[5]);
	memcpy(mac, mac_addr, strlen(mac_addr));*/

		for (size_t i = 0; i < m_AdpterInfo.size(); ++i)
		{
			for (size_t j = 0; j < m_AdpterInfo[i].Ip.size(); ++j)
			{
				if (j != 0)
				{
					//std::cout << ", ";
				}
				
				int widesize = MultiByteToWideChar(CP_ACP, 0, m_AdpterInfo[i].Ip[j].c_str(), -1, NULL, 0);
				CString temp_str;
				MultiByteToWideChar(CP_ACP, 0, m_AdpterInfo[i].Ip[j].c_str(), -1, temp_str.GetBuffer(MAX_PATH), widesize);
				if (m_sCurLocalIP.Compare(temp_str) == 0) {
					memcpy(mac, m_AdpterInfo[i].MacAddress.c_str(), m_AdpterInfo[i].MacAddress.size());
					widesize = MultiByteToWideChar(CP_ACP, 0, m_AdpterInfo[i].MacAddress.c_str(), -1, NULL, 0);
					CString mac_str;
					MultiByteToWideChar(CP_ACP, 0, m_AdpterInfo[i].MacAddress.c_str(), -1, temp_str.GetBuffer(MAX_PATH), widesize);
					m_LableCurMAC.SetWindowTextW(temp_str.GetString());
					mac_str.Format(_T("%s:%s"), _T("当前鉴权mac地址"), temp_str.GetString());
					m_LableCurMAC.SetWindowTextW(mac_str.GetString());
					goto END_MAC;
				}

			}

		}

#endif

END_MAC:

	return 0;
}

size_t Ckp2p_check_tool_win32Dlg::WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t realsize = size * nmemb;
	MemoryStruct_t *mem = (pMemoryStruct_t)data;

	mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory)
	{
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
	}
	return realsize;
}

void Ckp2p_check_tool_win32Dlg::format_dev_info_and_show(UINT n, void *arg)
{
	if (n == 0) {
		CDevSysInfoObject *info = (CDevSysInfoObject*)arg;
		CString temp;
		temp = info->id.c_str();
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.InsertItem(0, _T(""));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(0, 0, _T("设备id:"));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(0, 1, temp.GetString());
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.RedrawWindow();
		temp = info->server.c_str();
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.InsertItem(1, _T(""));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(1, 0, _T("所在服务器:"));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(1, 1, temp.GetString());
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.RedrawWindow();
		temp = info->last_time.c_str();
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.InsertItem(2, _T(""));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(2, 0, _T("最后上线时间:"));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(2, 1, temp.GetString());
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.RedrawWindow();
		temp = info->first_time.c_str();
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.InsertItem(3, _T(""));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(3, 0, _T("最初上线时间:"));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(3, 1, temp.GetString());
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.RedrawWindow();
		temp = info->ch_count.c_str();
		m_Channel = atoi(info->ch_count.c_str());
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.InsertItem(4, _T(""));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(4, 0, _T("设备通道数:"));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(4, 1, temp.GetString());
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.RedrawWindow();
		temp = info->hw_code.c_str();
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.InsertItem(5, _T(""));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(5, 0, _T("硬件代码:"));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(5, 1, temp.GetString());
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.RedrawWindow();
		temp = info->fw_version.c_str();
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.InsertItem(6, _T(""));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(6, 0, _T("固件版本:"));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(6, 1, temp.GetString());
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.RedrawWindow();
		temp = info->version.c_str();
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.InsertItem(7, _T(""));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(7, 0, _T("P2P版本:"));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(7, 1, temp.GetString());
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.RedrawWindow();
	
#if 0
		temp = _T("设备id:");
		temp += info->id.c_str();
		SHOW_STATUS_INFO_W(temp.GetString());
		temp = _T("所在服务器:");
		temp += info->server.c_str();
		SHOW_STATUS_INFO_W(temp.GetString());
		temp = _T("最后上线时间:");
		temp += info->last_time.c_str();
		SHOW_STATUS_INFO_W(temp.GetString());
		temp = _T("最初上线时间:");
		temp += info->first_time.c_str();
		SHOW_STATUS_INFO_W(temp.GetString());
		temp = _T("设备通道数:");
		temp += info->ch_count.c_str();
		SHOW_STATUS_INFO_W(temp.GetString());
		temp = _T("硬件代码:");
		temp += info->hw_code.c_str();
		SHOW_STATUS_INFO_W(temp.GetString());
		temp = _T("固件版本:");
		temp += info->fw_version.c_str();
		SHOW_STATUS_INFO_W(temp.GetString());
		temp = _T("P2P版本:");
		temp += info->version.c_str();
		SHOW_STATUS_INFO_W(temp.GetString());
		temp = _T("odm:");
		temp += info->odm.c_str();
		SHOW_STATUS_INFO_W(temp.GetString());
#endif

	}
	else if (n == 1) {
		CDevStatusInfoObject *info = (CDevStatusInfoObject*)arg;
		CString temp;
		temp = info->status.c_str();
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.InsertItem(8, _T(""));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(8, 0, _T("在线状态:"));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(8, 1, temp.GetString());
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.RedrawWindow();
		temp = info->net_type.c_str();
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.InsertItem(9, _T(""));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(9, 0, _T("网络类型:"));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(9, 1, temp.GetString());
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.RedrawWindow();
		temp = info->last_time.c_str();
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.InsertItem(10, _T(""));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(10, 0, _T("最后在线时间:"));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(10, 1, temp.GetString());
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.RedrawWindow();
		temp = info->ip.c_str();
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.InsertItem(11, _T(""));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(11, 0, _T("设备IP地址:"));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(11, 1, temp.GetString());
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.RedrawWindow();
		temp = info->port.c_str();
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.InsertItem(12, _T(""));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(12, 0, _T("设备上线端口:"));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(12, 1, temp.GetString());
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.RedrawWindow();
		temp = info->proto.c_str();
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.InsertItem(13, _T(""));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(13, 0, _T("协议类型:"));
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.SetItemText(13, 1, temp.GetString());
		m_QueryDevInfoDlg.m_ListCtrlQueryInfo.RedrawWindow();

#if 0
		temp = _T("在线状态:");
		temp += info->status.c_str();
		SHOW_STATUS_INFO_W(temp.GetString());
		temp = _T("网络类型:");
		temp += info->net_type.c_str();
		SHOW_STATUS_INFO_W(temp.GetString());
		temp = _T("最后在线时间:");
		temp += info->last_time.c_str();
		SHOW_STATUS_INFO_W(temp.GetString());
		temp = _T("设备IP地址:");
		temp += info->ip.c_str();
		SHOW_STATUS_INFO_W(temp.GetString());
		temp = _T("设备上线端口:");
		temp += info->port.c_str();
		SHOW_STATUS_INFO_W(temp.GetString());
		temp = _T("协议类型:");
		temp += info->proto.c_str();
		SHOW_STATUS_INFO_W(temp.GetString());
		/*temp = _T("count:");
		temp += info->count.c_str();
		SHOW_STATUS_INFO_W(temp.GetString());*/
#endif
	}
}

INT Ckp2p_check_tool_win32Dlg::check_cur_devid_status_info_once(LPCWSTR item, BOOL flag)
{
	if (item == NULL) {
		goto CHECK_CURL_GET;
	}

	m_EditDevID.SetString(item);

#if 0
	if (!m_TestStatusItemVec.empty()) {
		m_TestStatusItemVec.clear();
	}

	if (!m_TestItemVec.empty()) {
		m_TestItemVec.clear();
	}
	
	m_TestItemVec.push_back(CString("test"));
	
	//m_bFstCurDevConfigInfoFlag = TRUE;
	m_bQueryModDevConfigInfoFlag = TRUE;

	unsigned char decrypt[16];
	char localmac[32];
	memset(localmac, 0x00, 32);
	memset(decrypt, 0x00, 16);
	m_bShellSendReqFlag = FALSE;
	if (!m_bShellSendReqFlag) {
		memset(&m_ReqData, 0x00, sizeof(auth_psd_req_data_t));
		MD5Init(&m_Md5);
		MD5Update(&m_Md5, (unsigned char*)((char*)(CW2A(m_EditSvrPassword.GetString()))), strlen((char *)(CW2A(m_EditSvrPassword.GetString()))));
		MD5Final(&m_Md5, decrypt);

		get_mac(localmac);
		memcpy(m_ReqData.uid, CW2A(m_EditDevID.GetString()), m_EditDevID.GetLength());
		memcpy(m_ReqData.usr, CW2A(m_EditSvrUser.GetString()), m_EditSvrUser.GetLength());
		memcpy(m_ReqData.pwd, decrypt, 16);
		memcpy(m_ReqData.ver, "1", strlen("1"));
		memcpy(m_ReqData.mac, localmac, strlen(localmac));
		m_bShellSendReqFlag = TRUE;
	}

	::InterlockedIncrement(&m_FstCurDevConfigInfoCountLock);
	SetEvent(m_ExecThreadStartCmdEvent);

	return 0;
#endif

CHECK_CURL_GET:
	
	CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
    if(CURLE_OK != res)
    {
        //cout<<"curl init failed"<<endl;
		SHOW_STATUS_INFO_A("curl_global_init failed");
        return -2;
    }

    CURL *pCurl = NULL;
    pCurl = curl_easy_init();

    if( NULL == pCurl)
    {
        //cout<<"Init CURL failed..."<<endl;
		SHOW_STATUS_INFO_A("curl_easy_init failed");
        return -3;
    }

    string url = "http://ngw.dvr163.com/ct/idinfo?id="; 
	url += CW2A(m_EditDevID.GetString());
	
    //string filename = "dfshealth.html"; 

    curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 10L);//请求超时时长
    curl_easy_setopt(pCurl, CURLOPT_CONNECTTIMEOUT, 10L); //连接超时时长 
    curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1L);//允许重定向
    curl_easy_setopt(pCurl, CURLOPT_HEADER, 0L);  //若启用，会将头文件的信息作为数据流输出
    curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);  //得到请求结果后的回调函数

	MemoryStruct_t oDataChunk;  //请求结果的保存格式
    curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &oDataChunk);

    curl_easy_setopt(pCurl, CURLOPT_NOSIGNAL, 1L); //关闭中断信号响应
    curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 0L); //启用时会汇报所有的信息
    curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str() );

    curl_slist *pList = NULL;
    pList = curl_slist_append(pList,"Accept-Encoding:gzip, deflate, sdch"); 
    pList = curl_slist_append(pList,"Accept-Language:zh-CN,zh;q=0.8"); 
    pList = curl_slist_append(pList,"Connection:keep-alive");
    curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, pList); 

    res = curl_easy_perform(pCurl);  

    long res_code=0;
	//CDevSysInfoObject sys_info;
    res=curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &res_code);

    if(( res == CURLE_OK ) && (res_code == 200 || res_code == 201))
    {
        //FILE* fTmpMem = (FILE*)oDataChunk.memory;
		char* fTmpMem = (char*)oDataChunk.memory;
        if (fTmpMem) {
			//CDevSysInfoObject info;
			m_CurSysInfo.parse_sys_info(fTmpMem);
			if (!flag)
				format_dev_info_and_show(0, &m_CurSysInfo);
        }
	}
	else {
		CString temp;
		temp.Format(_T("curl failed, http res_code:%ld"), res_code);
		SHOW_STATUS_INFO_W(temp.GetString());
		SHOW_STATUS_INFO_A("请检查网络连接状况");
		return -4;
	}

	//CDevStatusInfoObject status_info;
	url.clear();
	std::ostringstream ostr;
	ostr << "http://" << m_CurSysInfo.server.c_str() << "/api/dev/fullinfo?ids=[\"" << CW2A(m_EditDevID.GetString()) << "\"]";
	url = ostr.str();
	curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str()); 
	MemoryStruct_t oDataChunk1;  
	curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &oDataChunk1);
	res = curl_easy_perform(pCurl);  
	res_code = 0;
	res = curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &res_code);
	if ((res == CURLE_OK) && (res_code == 200 || res_code == 201))
	{
		//FILE* fTmpMem = (FILE*)oDataChunk.memory;
		char* fTmpMem = (char*)oDataChunk1.memory;
		if (fTmpMem) {
			//CDevStatusInfoObject info;
			if (m_CurStatusInfo.parse_status_info(fTmpMem) == -1) {
				if (!flag)
					format_dev_info_and_show(1, &m_CurStatusInfo);
				goto CURL_DEV_OFF_LINE;
			}
			if (!flag)
				format_dev_info_and_show(1, &m_CurStatusInfo);
		}
	}
	else {
		CString temp;
		temp.Format(_T("curl failed, http res_code:%ld"), res_code);
		SHOW_STATUS_INFO_W(temp.GetString());
		return -4;
	}

	curl_slist_free_all(pList);
	curl_easy_cleanup(pCurl);
	curl_global_cleanup();

	return 0;

CURL_DEV_OFF_LINE:

    curl_slist_free_all(pList); 
    curl_easy_cleanup(pCurl);
    curl_global_cleanup();

    return -1;

}

unsigned int __stdcall Ckp2p_check_tool_win32Dlg::ModifyDevConfigWorkThreadProc(PVOID arg)
{
	BOOL bExit = FALSE;
	DWORD nRet;
	CString localcmd/*, temp*/;
	char localcmd2[256];
	memset(localcmd2, 0x00, 256);
	int ncmds = 0;
	
	Ckp2p_check_tool_win32Dlg *p = (Ckp2p_check_tool_win32Dlg*)arg;

	while (!bExit)
	{
		/*if (WAIT_OBJECT_0 == WaitForSingleObject(p->m_ModifyDevConfigThreadExitEvent, 0))
			bExit = TRUE;*/

		DWORD nRes = WaitForSingleObject(p->m_ModifyDevConfigThreadExitEvent, 0);
		if (nRes == WAIT_OBJECT_0) {
			bExit = TRUE;
		}


		if ((nRet = WaitForSingleObject(p->m_ModThreadStartCmdEvent, 0)) != WAIT_OBJECT_0) {
			/*if (bExit)
				SetEvent(p->m_ReadyExitEvent);*/
			msleep_c(50);
			continue;
		}

		while (p->m_DevSettingDlg.m_nItemCount > 0) {
			if (p->m_DevSettingDlg.m_bGateWay) {
				localcmd.SetString(L"route | grep default | awk '{print $2}'");
				p->m_DevSettingDlg.m_bGateWay = FALSE;
				ncmds = 1;
				SHOW_STATUS_INFO_S_A("修改网关前：");
				goto MSCONFIG_END;
			}
			if (p->m_DevSettingDlg.m_bDNS) {
				localcmd.SetString(L"cat /etc/resolv.conf  | awk '{print $2}'");
				p->m_DevSettingDlg.m_bDNS = FALSE;
				ncmds = 2;
				SHOW_STATUS_INFO_S_A("修改DNS前：");
				goto MSCONFIG_END;
			}
			if (p->m_DevSettingDlg.m_bMTU) {
				localcmd.SetString(L"cat /sys/class/net/eth0/mtu");
				p->m_DevSettingDlg.m_bMTU = FALSE;
				ncmds = 3;
				SHOW_STATUS_INFO_S_A("修改MTU前：");
				goto MSCONFIG_END;
			}

		MSCONFIG_END:

			m_Ecode = 0;
			int send_size = IOT_SHELL_Send(p->m_Session, CW2A(localcmd.GetString()), localcmd.GetLength() + 1, &p->m_ReqData);
			if ((nRet = WaitForSingleObject(p->m_ThreadNotifyCmdEvent, 10000)) != WAIT_OBJECT_0) {
				msleep_c(50);
				switch (nRet)
				{
				case WAIT_TIMEOUT:
					SHOW_STATUS_INFO_S_A("获取数据时间超过10秒");
					break;
				case WAIT_FAILED:
					SHOW_STATUS_INFO_S_A("WaitForSingleObject error");
					break;
				default:
					break;
				}
				//continue;
			}

			//localcmd.Empty();
			if ((m_Ecode == 0) && p->m_bOnDataFlag)
			{
				switch (ncmds)
				{
				case 1:
					if (p->m_DevSettingDlg.m_EditGateWay.GetLength() >= 32) {
						continue;
					}

					if (strlen(p->m_sdns) < 32) {
						p->m_sdns[strlen(p->m_sdns) - 1] = '\0';

						//localcmd.Format(L"sed 's/%s/%s/g' /etc/resolv.conf", p->m_sdns, CW2A(p->m_DevSettingDlg.m_EditGateWay.GetString()));
						//sprintf(localcmd2, "sed 's/%s/%s/g' /etc/resolv.conf", p->m_sdns, (char*)(CW2A(p->m_DevSettingDlg.m_EditGateWay.GetString())));
						sprintf(localcmd2, "route add default gw %s", (char*)(CW2A(p->m_DevSettingDlg.m_EditGateWay.GetString())));
						//sprintf(localcmd2, "echo \"nameserver %s\" > /etc/resolv.conf", (char*)(CW2A(p->m_DevSettingDlg.m_EditGateWay.GetString())));
					}
					else
					{
						/*printf("the dns return is incorrect,can't change it!\n");
						if (m_Session) {
						IOT_SHELL_Logout(&session);
						}
						if (shell) {
						IOT_SHELL_Deinit(&shell);
						}
						goto CHECK_END;*/
					}
					break;
				case 2:
					if (p->m_DevSettingDlg.m_EditDNS.GetLength() >= 32) {
						continue;
					}

					if (strlen(p->m_sdns) < 32) {
						//localcmd.Format(L"route add default gw %s", CW2A(p->m_DevSettingDlg.m_EditDNS.GetString()));
						sprintf(localcmd2, "echo \"nameserver %s\" > /etc/resolv.conf", (char*)(CW2A(p->m_DevSettingDlg.m_EditDNS.GetString())));
						//sprintf(localcmd2, "route add default gw %s", (char*)(CW2A(p->m_DevSettingDlg.m_EditDNS.GetString())));
					}
					else
					{
						/*printf("the gate way return is incorrect,can't change it!\n");
						if (session) {
						IOT_SHELL_Logout(&session);
						}
						if (shell) {
						IOT_SHELL_Deinit(&shell);
						}
						goto CHECK_END;*/
					}
					break;
				case 3:
					if (p->m_DevSettingDlg.m_EditMTU.GetLength() >= 32) {
						continue;
					}

					if (strlen(p->m_sdns) < 32) {
						p->m_sdns[strlen(p->m_sdns) - 1] = '\0';
						//localcmd.Format(L"sed 's/%s/%s/g' /sys/class/net/eth0/mtu", p->m_sdns, CW2A(p->m_DevSettingDlg.m_EditMTU.GetString()));
						//sprintf(localcmd2, "sed 's/%s/%s/g' /sys/class/net/eth0/mtu", p->m_sdns, (char*)(CW2A(p->m_DevSettingDlg.m_EditMTU.GetString())));
						sprintf(localcmd2, "echo \"%s\" > /sys/class/net/eth0/mtu", (char*)(CW2A(p->m_DevSettingDlg.m_EditMTU.GetString())));

					}
					else
					{
						/*printf("the mtu return is incorrect,can't change it!\n");
						if (session) {
						IOT_SHELL_Logout(&session);
						}
						if (shell) {
						IOT_SHELL_Deinit(&shell);
						}
						goto CHECK_END;*/
					}
					break;
				default:
					break;
				}

				m_Ecode = 0;
				p->m_bOnDataFlag = FALSE;
				SHOW_STATUS_INFO_S_A("修改后：");
				//send_size = IOT_SHELL_Send(p->m_Session, CW2A(localcmd.GetString()), localcmd.GetLength() + 1, &p->m_ReqData);
				send_size = IOT_SHELL_Send(p->m_Session, localcmd2, strlen(localcmd2) + 1, &p->m_ReqData);
				Sleep(500);

				switch (ncmds)
				{
				case 1:
					p->check_current_config_info_once(TEST_GEATEWAY_ITEM);
					break;
				case 2:
					p->check_current_config_info_once(TEST_DNS_ITEM);
					break;
				case 3:
					p->check_current_config_info_once(TEST_MTU_ITEM);
					break;
				}

				if ((nRet = WaitForSingleObject(p->m_ModThreadStartCmdEvent, 30000)) == WAIT_OBJECT_0) {
					/*switch (nRet)
					{
					case WAIT_TIMEOUT:
						SHOW_STATUS_INFO_S_A("获取数据时间超过10秒");
						break;
					case WAIT_FAILED:
						SHOW_STATUS_INFO_S_A("WaitForSingleObject error");
						break;
					default:
						break;
					}*/

					if (p->m_DevSettingDlg.m_nItemCount == 2) {
						p->m_bQueryModDevConfigInfoFlag = TRUE;
					}

				}

				//switch (ncmds) {
				//case 1:
				//	//p->m_LableCurGateWay = p->m_sdns;
				//	break;
				//case 2:
				//	//p->m_LableCurDNS = p->m_sdns;
				//	break;
				//case 3:
				//	//p->m_LableCurMTU = p->m_sdns;
				//	break;
				//}

			}
			p->m_DevSettingDlg.m_nItemCount--;
			
		}
		//p->m_Statusbar.SetPaneText(2, _T("修改设备配置结束"));
	}


	return 0;
}

unsigned int __stdcall Ckp2p_check_tool_win32Dlg::ExcuteCmdWorkThreadProc(PVOID arg)
{
	BOOL bExit = FALSE;
	DWORD nRet, nTimeCount;
	//time_t send_cmd_start;
	Ckp2p_check_tool_win32Dlg *p = (Ckp2p_check_tool_win32Dlg*)arg;
	char localcmd[256];
	memset(localcmd, 0x00, 256);
	INT recv_count = -1, type_cmd = -1;
	INT dns_count = -1, gateway_count = -1;
	HANDLE h[3];
	
	h[0] = p->m_ThreadNotifyCmdEvent;
	h[1] = p->m_DevOfflineNotifyEvent;
	h[2] = p->m_TerminateCheckNotifyEvent;

	//LONG64 cur_id = atoll(CW2A(p->m_EditDevID.GetString()));
	//map<LONG64, pkp2p_dev_config_t>::iterator c_pos;
	//map<LONG64, pkp2p_dev_status_t>::iterator s_pos;

	while (!bExit)
	{
		/*if (WAIT_OBJECT_0 == WaitForSingleObject(p->m_ExcuteCmdThreadExitEvent, 0))
			bExit = TRUE;*/

		DWORD nRes = WaitForSingleObject(p->m_ExcuteCmdThreadExitEvent, 0);
		if (nRes == WAIT_OBJECT_0) {
			bExit = TRUE;
		}

		if ((nRet = WaitForSingleObject(p->m_ExecThreadStartCmdEvent, 0)) != WAIT_OBJECT_0) {
			/*if (bExit)
				SetEvent(p->m_ReadyExitEvent);*/
			msleep_c(50);
			continue;
		}
		(CEdit *)p->GetDlgItem(IDC_PASSD_SVR_EDIT)->EnableWindow(FALSE);;
		p->m_ComboBoxSvrUserItem.EnableWindow(FALSE);

		::InterlockedIncrement(&m_OnDataFlagCountLock);

		//p->m_DevStatusDlg.m_ListCtrlStatus.DeleteAllItems();
		int i = 0;
		recv_count = -1;
		int sum = p->m_TestItemVec.size();
		vector<CString>::iterator it = p->m_TestItemVec.begin();
		for (; it != p->m_TestItemVec.end(); it++) 
		{
			if ((*it).Compare(TEST_MTU_ITEM) == 0) {
				type_cmd = 0;
				nTimeCount = 5000;
				strcpy(localcmd, "cat /sys/class/net/eth0/mtu");
				//SHOW_STATUS_INFO_S_A("MTU获取，执行命令：cat /sys/class/net/eth0/mtu");
				SHOW_STATUS_INFO_S_A("发送获取MTU指令");
			}
			else if ((*it).Compare(TEST_GEATEWAY_ITEM) == 0) {
				type_cmd = 1;
				nTimeCount = 10000;
				strcpy(localcmd, "route | grep default | awk '{print $2}'");
				//SHOW_STATUS_INFO_S_A("网关配置获取，执行命令：route | grep default | awk '{print $2}'");
				SHOW_STATUS_INFO_S_A("发送获取网关指令");
				//recv_count = 1;
			}
			else if ((*it).Compare(TEST_DNS_ITEM) == 0) {
				type_cmd = 2;
				nTimeCount = 10000;
				strcpy(localcmd, "cat /etc/resolv.conf  | awk '{print $2}'");
				//SHOW_STATUS_INFO_S_A("DNS配置获取，执行命令：cat /etc/resolv.conf  | awk '{print $2}'");
				SHOW_STATUS_INFO_S_A("发送获取DNS指令");
				recv_count = 2;
				dns_count = 2;
			}
			else if ((*it).Compare(TEST_SIGNAL_ITEM) == 0) {
				type_cmd = 3;
				nTimeCount = 10000;
				strcpy(localcmd, "cat /proc/net/rtl8188fu/wlan0/rx_signal");
				//SHOW_STATUS_INFO_S_A("信号强度获取，执行命令：cat /proc/net/rtl8188fu/wlan0/rx_signal");
				SHOW_STATUS_INFO_S_A("发送获取信号强度指令");
			}
			else if ((*it).Compare(TEST_CPU_ITEM) == 0) {
				type_cmd = 4;
				nTimeCount = 15000;
				strcpy(localcmd, "top -n 1");
				//strcpy(localcmd, "top -b -n 1 | grep 'top -' -A 3");
				//SHOW_STATUS_INFO_S_A("CPU占用获取，执行命令：top -b -n 1 | grep 'top -' -A 3");
				SHOW_STATUS_INFO_S_A("发送获取CPU占用信息指令");
			}
			else if ((*it).Compare(TEST_MEMORY_ITEM) == 0) {
				type_cmd = 5;
				nTimeCount = 15000;
				strcpy(localcmd, "top -b -n 1 | grep 'top -' -A 5 | grep 'KiB Mem'");
				//SHOW_STATUS_INFO_S_A("内存占用获取，执行命令：top -b -n 1 | grep 'top -' -A 5 | grep 'KiB Mem'");
				SHOW_STATUS_INFO_S_A("发送获取内存占用信息指令");
			}
			else if ((*it).Compare(TEST_PROCESS_ITEM) == 0) {
				type_cmd = 6;
				nTimeCount = 15000;
				strcpy(localcmd, "ps");
				//SHOW_STATUS_INFO_S_A("进程信息获取，执行命令：ps");
				SHOW_STATUS_INFO_S_A("发送获取指令");
			}
			else if ((*it).Compare(TEST_THREAD_ITEM) == 0) {
				type_cmd = 7;
				nTimeCount = 15000;
				strcpy(localcmd, "ps -T");
				//SHOW_STATUS_INFO_S_A("线程信息获取，执行命令：ps -T");
				SHOW_STATUS_INFO_S_A("发送获取指令");
			}
			else if ((*it).Compare(TEST_DEV_SYS_TIME_ITEM) == 0) {
				type_cmd = 8;
				nTimeCount = 10000;
				strcpy(localcmd, "date +\"%Y-%m-%d  %H:%M:%S\"");
				//SHOW_STATUS_INFO_S_A("线程信息获取，执行命令：ps -T");
				SHOW_STATUS_INFO_S_A("发送获取设备系统时间指令");
			}
			else if ((*it).Compare(_T("test")) == 0) {
				type_cmd = 9;
				nTimeCount = 10000;
				if (!p->m_TestCmdDlg.m_EditCmd.IsEmpty()) {
					strcpy(localcmd, CW2A(p->m_TestCmdDlg.m_EditCmd.GetString()));
				}
				else {
					CString temp;
					temp.SetString(L"wget -q -O - http://ngw.dvr163.com/ct/idinfo?id=");
					temp += p->m_EditDevID;
					strcpy(localcmd, CW2A(temp.GetString()));
				}
				//SHOW_STATUS_INFO_S_A("线程信息获取，执行命令：ps -T");
				SHOW_STATUS_INFO_S_A("发送测试指令");
			}
			
			m_Ecode = 0;

			int send_size = IOT_SHELL_Send(p->m_Session, localcmd, strlen(localcmd) + 1, &p->m_ReqData);

		MULTI_CONFIG_RECV:

			nRet = -1;
			CString temp, sub_temp;
			nRet = WaitForMultipleObjects(3, h, FALSE, nTimeCount);
			switch (nRet)
			{
			case WAIT_OBJECT_0 + 0:
				//CString temp, sub_temp;
				int start_pos, end_pos, nCount;
				//vector<CString>::iterator it;
				if (!p->m_bOnDataFlag) {
					::InterlockedIncrement(&m_FstCurDevConfigInfoCountLock);
					if (::InterlockedDecrement(&m_FstCurDevConfigInfoCountLock) == 1) {
						p->m_DevSettingDlg.m_LableCurGateWay.SetWindowTextW(_T("查询失败"));
						p->m_DevSettingDlg.m_LableCurDNS.SetWindowTextW(_T("查询失败"));
						p->m_DevSettingDlg.m_LableCurMTU.SetWindowTextW(_T("查询失败"));

					}
					else {
						p->m_DevConfigDlg.m_ListCtrlConfig.SetItemText(i, 2, _T("失败"));
					}
					goto CONFIG_AGAIN;
				}

				temp = p->m_sdns;
				::InterlockedIncrement(&m_FstCurDevConfigInfoCountLock);
				if (::InterlockedDecrement(&m_FstCurDevConfigInfoCountLock) == 1) {
					switch (type_cmd) {
					case 0:
						if (!p->m_bQueryModDevConfigInfoFlag) {
							p->m_LableCurMTU = p->m_sdns;
							p->m_DevSettingDlg.m_LableCurMTU.SetWindowText(temp.GetString());
						}
						else {
							p->m_bQueryModDevConfigInfoFlag = FALSE;
						}
						break;
					case 1:
						if (!p->m_bQueryModDevConfigInfoFlag) {
							p->m_LableCurGateWay = p->m_sdns;
							p->m_DevSettingDlg.m_LableCurGateWay.SetWindowText(temp.GetString());
						}
						else {
							p->m_bQueryModDevConfigInfoFlag = FALSE;
						}
						break;
					case 2:
						if (!p->m_bQueryModDevConfigInfoFlag) {
							p->m_LableCurDNS = p->m_sdns;
							CString beforeStr;
							p->m_DevSettingDlg.m_LableCurDNS.GetWindowText(beforeStr);
							if (beforeStr.Find(_T("正在查询中")) != -1)
								beforeStr.Empty();
							beforeStr += temp;
							beforeStr += _T(" ");
							p->m_DevSettingDlg.m_LableCurDNS.SetWindowText(beforeStr.GetString());
							//p->m_DevSettingDlg.m_LableCurDNS.SetWindowText(temp.GetString());
						}
						else {
							p->m_bQueryModDevConfigInfoFlag = FALSE;
						}
						break;
					}
				}
				else {
					switch (type_cmd) {
					case 3:
						start_pos = temp.Find(_T("signal_strength"));
						if (start_pos == -1) {
							p->m_DevConfigDlg.m_ListCtrlConfig.SetItemText(i, 2, _T("失败"));
						}
						else {
							end_pos = temp.Find(_T("signal_qual"));
							nCount = end_pos - (start_pos + 16);
							sub_temp = temp.Mid(start_pos + 16, nCount);
							p->m_DevConfigDlg.m_ListCtrlConfig.SetItemText(i, 1, sub_temp.GetString());
							p->m_DevConfigDlg.m_ListCtrlConfig.SetItemText(i, 2, _T("成功"));
						}
						break;
					default:
						if (recv_count > 0) {
							CString beforeStr = p->m_DevConfigDlg.m_ListCtrlConfig.GetItemText(i, 1);
							beforeStr += temp;
							beforeStr += _T("   ");
							//temp = beforeStr;
							p->m_DevConfigDlg.m_ListCtrlConfig.SetItemText(i, 1, beforeStr.GetString());
						}
						else {
							p->m_DevConfigDlg.m_ListCtrlConfig.SetItemText(i, 1, temp.GetString());
						}
						p->m_DevConfigDlg.m_ListCtrlConfig.SetItemText(i, 2, _T("成功"));
						break;

					}
				}
				break;
			case WAIT_OBJECT_0 + 1:
				goto DEV_OFF_LINE_TEST;
				break;
			case WAIT_OBJECT_0 + 2:
				goto TERMIMATE_CHECK;
				break;
			case WAIT_FAILED:
				SHOW_STATUS_INFO_S_A("WaitForSingleObject error");
				break;
			case WAIT_TIMEOUT:
				if (recv_count > 0 && recv_count == dns_count) {
					recv_count = -2;
				}
				else if (recv_count > 0 && recv_count == gateway_count) {
					recv_count = -2;
				}
				else {
					SHOW_STATUS_INFO_S_A("获取数据时间超时");
					recv_count = -1;
				}

				::InterlockedIncrement(&m_FstCurDevConfigInfoCountLock);
				if (::InterlockedDecrement(&m_FstCurDevConfigInfoCountLock) == 1) {
					switch (type_cmd) {
					case 0:
						if (!p->m_bQueryModDevConfigInfoFlag) {
							//p->m_LableCurMTU = p->m_sdns;
							p->m_DevSettingDlg.m_LableCurMTU.SetWindowText(_T("查询超时"));

						}
						else {
							p->m_bQueryModDevConfigInfoFlag = FALSE;
						}
						break;
					case 1:
						if (!p->m_bQueryModDevConfigInfoFlag) {
							//p->m_LableCurGateWay = p->m_sdns;
							if (recv_count == -2) {
								p->m_DevSettingDlg.m_LableCurGateWay.SetWindowText(_T("查询超时"));
							}
						}
						else {
							p->m_bQueryModDevConfigInfoFlag = FALSE;
						}
						break;
					case 2:
						if (!p->m_bQueryModDevConfigInfoFlag) {
							//p->m_LableCurDNS = p->m_sdns;
							if (recv_count == -2) {
								p->m_DevSettingDlg.m_LableCurDNS.SetWindowText(_T("查询超时"));
							}
						}
						else {
							p->m_bQueryModDevConfigInfoFlag = FALSE;
						}
						break;
					}
				}
				break;
			}

#if 0
	//		if ((nRet = WaitForSingleObject(p->m_ThreadNotifyCmdEvent, nTimeCount)) == WAIT_OBJECT_0) {
	//			CString temp, sub_temp;
	//			int start_pos, end_pos, nCount;
	//			vector<CString>::iterator it;
	//			if (!p->m_bOnDataFlag) {
	//				::InterlockedIncrement(&m_FstCurDevConfigInfoCountLock);
	//				if (::InterlockedDecrement(&m_FstCurDevConfigInfoCountLock) == 1) {
	//					p->m_DevSettingDlg.m_LableCurGateWay.SetWindowTextW(_T("查询失败"));
	//					p->m_DevSettingDlg.m_LableCurDNS.SetWindowTextW(_T("查询失败"));
	//					p->m_DevSettingDlg.m_LableCurMTU.SetWindowTextW(_T("查询失败"));
	//					
	//				}
	//				else {
	//					p->m_DevConfigDlg.m_ListCtrlConfig.SetItemText(i, 2, _T("失败"));
	//				}
	//				goto CONFIG_AGAIN;
	//			}

	//			//if (!p->m_SvrUserHistoryVec.empty()) {
	//			//	it = find(p->m_SvrUserHistoryVec.begin(), p->m_SvrUserHistoryVec.end(), p->m_EditSvrUser);
	//			//	if (it == p->m_SvrUserHistoryVec.end()) {
	//			//		p->m_SvrUserHistoryVec.insert(p->m_SvrUserHistoryVec.begin(), p->m_EditSvrUser);
	//			//		//p->m_SvrUserHistoryVec.push_back(p->m_EditSvrUser);
	//			//		//p->m_ComboBoxSvrUserItem.AddString(p->m_EditSvrUser.GetString());
	//			//		//p->m_ComboBoxSvrUserItem.clear
	//			//		if (p->m_SvrUserHistoryVec.size() >= 10) {
	//			//			p->m_LoginHistoryCmd = 3;
	//			//			SetEvent(p->m_LoginInfoHistoryCacheThreadUpdateEvent);
	//			//		}
	//			//		else {
	//			//			p->m_SvrUserHistoryVec.push_back(p->m_EditSvrUser);
	//			//			p->m_ComboBoxSvrUserItem.AddString(p->m_EditSvrUser.GetString());
	//			//		}
	//			//	}
	//			//	
	//			//}
	//			//else {
	//			//	p->m_SvrUserHistoryVec.push_back(p->m_EditSvrUser);
	//			//	p->m_ComboBoxSvrUserItem.AddString(p->m_EditSvrUser.GetString());
	//			//}
	//		
	//			temp = p->m_sdns;
	//			::InterlockedIncrement(&m_FstCurDevConfigInfoCountLock);
	//			if (::InterlockedDecrement(&m_FstCurDevConfigInfoCountLock) == 1) {
	//				switch (type_cmd) {
	//				case 0:
	//					if (!p->m_bQueryModDevConfigInfoFlag) {
	//						p->m_LableCurMTU = p->m_sdns;
	//						p->m_DevSettingDlg.m_LableCurMTU.SetWindowText(temp.GetString());
	//					}
	//					else {
	//						p->m_bQueryModDevConfigInfoFlag = FALSE;
	//					}
	//					break;
	//				case 1:
	//					if (!p->m_bQueryModDevConfigInfoFlag) {
	//						p->m_LableCurGateWay = p->m_sdns;						
	//						p->m_DevSettingDlg.m_LableCurGateWay.SetWindowText(temp.GetString());
	//					}
	//					else {
	//						p->m_bQueryModDevConfigInfoFlag = FALSE;
	//					}
	//					break;
	//				case 2:
	//					if (!p->m_bQueryModDevConfigInfoFlag) {
	//						p->m_LableCurDNS = p->m_sdns;
	//						CString beforeStr;
	//						p->m_DevSettingDlg.m_LableCurDNS.GetWindowText(beforeStr);
	//						if (beforeStr.Find(_T("正在查询中")) != -1)
	//							beforeStr.Empty();
	//						beforeStr += temp;
	//						beforeStr += _T(" ");
	//						p->m_DevSettingDlg.m_LableCurDNS.SetWindowText(beforeStr.GetString());
	//						//p->m_DevSettingDlg.m_LableCurDNS.SetWindowText(temp.GetString());
	//					}
	//					else {
	//						p->m_bQueryModDevConfigInfoFlag = FALSE;
	//					}
	//					break;
	//				}
	//			}
	//			else {
	//				switch (type_cmd) {
	//				case 3:
	//					start_pos = temp.Find(_T("signal_strength"));
	//					if (start_pos == -1) {
	//						p->m_DevConfigDlg.m_ListCtrlConfig.SetItemText(i, 2, _T("失败"));
	//					}
	//					else {
	//						end_pos = temp.Find(_T("signal_qual"));
	//						nCount = end_pos - (start_pos + 16);
	//						sub_temp = temp.Mid(start_pos + 16, nCount);
	//						p->m_DevConfigDlg.m_ListCtrlConfig.SetItemText(i, 1, sub_temp.GetString());
	//						p->m_DevConfigDlg.m_ListCtrlConfig.SetItemText(i, 2, _T("成功"));
	//					}
	//					break;
	//				default:
	//					if (recv_count > 0) {
	//						CString beforeStr = p->m_DevConfigDlg.m_ListCtrlConfig.GetItemText(i, 1);
	//						beforeStr += temp;
	//						beforeStr += _T("   ");
	//						//temp = beforeStr;
	//						p->m_DevConfigDlg.m_ListCtrlConfig.SetItemText(i, 1, beforeStr.GetString());
	//					}
	//					else {
	//						p->m_DevConfigDlg.m_ListCtrlConfig.SetItemText(i, 1, temp.GetString());
	//					}
	//					p->m_DevConfigDlg.m_ListCtrlConfig.SetItemText(i, 2, _T("成功"));
	//					break;
	//			
	//				}
	//				//p->m_DevConfigDlg.m_ListCtrlConfig.SetItemText(i, 1, temp.GetString());
	//				//p->m_DevConfigDlg.m_ListCtrlConfig.SetItemText(i, 2, _T("成功"));
	//			}

	//			/*c_pos = p->m_DevConfigInfoMap.find(cur_id);
	//			if (c_pos == p->m_DevConfigInfoMap.end()) {
	//			}

	//			switch (type_cmd) {
	//			case 0:
	//				if (strlen(c_pos->second->mtu) == 0 || strcmp(c_pos->second->mtu, p->m_sdns) == 0) {
	//					goto CONFIG_AGAIN;
	//				}
	//				memcpy(c_pos->second->mtu, p->m_sdns, temp.GetLength());				
	//				break;
	//			case 1:
	//				if (strlen(c_pos->second->gateway) == 0 || strcmp(c_pos->second->gateway, p->m_sdns) == 0) {
	//					goto CONFIG_AGAIN;
	//				}
	//				memcpy(c_pos->second->gateway, p->m_sdns, temp.GetLength());
	//				break;
	//			case 2:
	//				if (strlen(c_pos->second->dns) == 0 || strcmp(c_pos->second->dns, p->m_sdns) == 0) {
	//					goto CONFIG_AGAIN;
	//				}
	//				memcpy(c_pos->second->dns, p->m_sdns, temp.GetLength());
	//				break;
	//			case 3:
	//				if (strlen(c_pos->second->signal) == 0 || strcmp(c_pos->second->signal, p->m_sdns) == 0) {
	//					goto CONFIG_AGAIN;
	//				}
	//				memcpy(c_pos->second->signal, p->m_sdns, temp.GetLength());
	//				break;
	//			case 4:
	//		
	//				break;
	//			case 5:
	//
	//				break;
	//			case 6:
	//
	//				break;
	//			case 7:
	//	
	//				break;
	//			default:
	//				break;
	//			}*/
	//		}
	//		else {
	//			//msleep_c(50);
	//			switch (nRet)
	//			{
	//			case WAIT_TIMEOUT:
	//				if (recv_count > 0) {
	//					recv_count = -2;
	//				}
	//				else {
	//					SHOW_STATUS_INFO_S_A("获取数据时间超时");
	//				}
	//
	//				::InterlockedIncrement(&m_FstCurDevConfigInfoCountLock);
	//				if (::InterlockedDecrement(&m_FstCurDevConfigInfoCountLock) == 1) {
	//					switch (type_cmd) {
	//					case 0:
	//						if (!p->m_bQueryModDevConfigInfoFlag) {
	//							//p->m_LableCurMTU = p->m_sdns;
	//							p->m_DevSettingDlg.m_LableCurMTU.SetWindowText(_T("查询超时"));
	//	
	//						}
	//						else {
	//							p->m_bQueryModDevConfigInfoFlag = FALSE;
	//						}
	//						break;
	//					case 1:
	//						if (!p->m_bQueryModDevConfigInfoFlag) {
	//							//p->m_LableCurGateWay = p->m_sdns;
	//							if (recv_count != -2) {
	//								p->m_DevSettingDlg.m_LableCurGateWay.SetWindowText(_T("查询超时"));
	//							}
	//						}
	//						else {
	//							p->m_bQueryModDevConfigInfoFlag = FALSE;
	//						}
	//						break;
	//					case 2:
	//						if (!p->m_bQueryModDevConfigInfoFlag) {
	//							//p->m_LableCurDNS = p->m_sdns;
	//							if (recv_count != -2) {
	//								p->m_DevSettingDlg.m_LableCurDNS.SetWindowText(_T("查询超时"));
	//							}
	//						}
	//						else {
	//							p->m_bQueryModDevConfigInfoFlag = FALSE;
	//						}
	//						break;
	//					}
	//				}
	//				break;
	//			case WAIT_FAILED:
	//				SHOW_STATUS_INFO_S_A("WaitForSingleObject error");
	//				break;
	//			default:
	//				break;
	//			}
	//			//continue;
	//		}
#endif

			if (recv_count > 0) {
				recv_count--;
				nTimeCount -= 5000;
				goto MULTI_CONFIG_RECV;
			}

		CONFIG_AGAIN:

			memset(localcmd, 0x00, 256);
			i++;
		}

		//设备参数修改查询一次结束
		if (m_bFstCurDevConfigInfoFlag) {
			//if (p->m_DevSettingDlg.m_nItemCount <= 0) {
				::InterlockedDecrement(&m_FstCurDevConfigInfoCountLock);
				m_bFstCurDevConfigInfoFlag = FALSE;
				SetEvent(p->m_ModThreadStartCmdEvent);
			//}
			/*::InterlockedDecrement(&m_FstCurDevConfigInfoCountLock);
			m_bFstCurDevConfigInfoFlag = FALSE;*/
		}

		//p->m_DevStatusDlg.m_ListCtrlStatus.DeleteAllItems();
		i = 0;
		it = p->m_TestStatusItemVec.begin();
		for (; it != p->m_TestStatusItemVec.end(); it++) {
			if ((*it).Compare(TEST_PING_ITEM) == 0) {
				CString pcmd = L"ping -c ";
				pcmd += p->m_PingSettingDlg.m_PNum;
				pcmd += L" -s ";
				pcmd += p->m_PingSettingDlg.m_PpkgSize;
				pcmd += L" ";
				pcmd += p->m_PingSettingDlg.m_PwebSite;
				strcpy(localcmd, CW2A(pcmd.GetString()));
				SHOW_STATUS_INFO_S_A("发送ping指令");
				/*SHOW_STATUS_INFO_S_A("PING，执行命令：");
				SHOW_STATUS_INFO_S_W(pcmd.GetString());*/
				recv_count = 1;
				nTimeCount = 30000;
				
			}
			else if ((*it).Compare(TEST_NETWORK_ITEM) == 0) {
				strcpy(localcmd, "netstat | grep ip | awk '{print $6}'");
				//SHOW_STATUS_INFO_S_A("网络连接信息获取，执行命令：netstat | grep ip | awk '{print $6}'");
				SHOW_STATUS_INFO_S_A("发送获取网络连接信息指令");
			}
			else if ((*it).Compare(TEST_ROUTE_ITEM) == 0) {
				strcpy(localcmd, "route");
				//SHOW_STATUS_INFO_S_A("路由表信息获取，执行命令：route");
				SHOW_STATUS_INFO_S_A("发送获取路由表信息指令");
			}
			else if ((*it).Compare(TEST_BANDWIDTH) == 0) {
				//strcpy(localcmd, "cat /proc/net/rtl8188fu/wlan0/rx_signal");
				//SHOW_STATUS_INFO_S_A("带宽信息获取，执行命令：cat /proc/net/rtl8188fu/wlan0/rx_signal");
			}
			else if ((*it).Compare(TEST_LOG_ITEM) == 0) {
				//strcpy(localcmd, "netstat -anpt");
				//SHOW_STATUS_INFO_S_A("日志数据获取，执行命令：netstat -anpt");
			}

			//send_cmd_start = time(NULL);
			m_Ecode = 0;
			IOT_SHELL_Send(p->m_Session, localcmd, strlen(localcmd) + 1, &p->m_ReqData);

		MULTI_STATUS_RECV:
			
			nRet = -1;
			CString temp;
			nRet = WaitForMultipleObjects(3, h, FALSE, nTimeCount);
			switch (nRet)
			{		
			case WAIT_OBJECT_0 + 0:
				if (!p->m_bOnDataFlag) {
					p->m_DevStatusDlg.m_ListCtrlStatus.SetItemText(i, 2, _T("失败"));
					goto STATUS_AGAIN;
				}

				//CString temp;
				temp = p->m_sdns;
				if (recv_count > 0) {
					CString beforeStr = p->m_DevStatusDlg.m_ListCtrlStatus.GetItemText(i, 1);
					beforeStr += temp;
					//temp = beforeStr;
					p->m_DevStatusDlg.m_ListCtrlStatus.SetItemText(i, 1, beforeStr.GetString());
					//p->m_DevStatusDlg.m_ListCtrlStatus.SetItemText(i, 1, temp.GetString());
					INT nRet = beforeStr.Find(_T("bad address"));
					if (nRet != -1) {
						p->m_DevStatusDlg.m_ListCtrlStatus.SetItemText(i, 2, _T("fail"));
						recv_count = 0;
					}

					nRet = beforeStr.Find(_T("unknown host"));
					if (nRet != -1) {
						p->m_DevStatusDlg.m_ListCtrlStatus.SetItemText(i, 2, _T("fail"));
						recv_count = 0;
					}

					nRet = beforeStr.Find(_T("packet loss"));
					if (nRet != -1) {
						nRet = beforeStr.Find(_T("100% packet loss"));
						if (nRet == -1) {
							p->m_DevStatusDlg.m_ListCtrlStatus.SetItemText(i, 2, _T("sucess"));
						}
						else {
							p->m_DevStatusDlg.m_ListCtrlStatus.SetItemText(i, 2, _T("fail"));
						}
						recv_count = 0;
					}
				}
				else {
					p->m_DevStatusDlg.m_ListCtrlStatus.SetItemText(i, 1, temp.GetString());
				}
				//memset(p->m_sdns, 0, 1024 * 4);
				break;
			case WAIT_OBJECT_0 + 1:
				goto DEV_OFF_LINE_TEST;
				break;
			case WAIT_OBJECT_0 + 2:
				goto TERMIMATE_CHECK;
				break;
			case WAIT_FAILED:
				SHOW_STATUS_INFO_S_A("WaitForSingleObject error");
				break;
			case WAIT_TIMEOUT:
				recv_count = 0;
				SHOW_STATUS_INFO_S_A("获取数据时间超过10秒");
				p->m_DevStatusDlg.m_ListCtrlStatus.SetItemText(i, 2, _T("fail"));
				break;
			}

#if 0
			//if ((nRet = WaitForSingleObject(p->m_ThreadNotifyCmdEvent, 10000)) == WAIT_OBJECT_0) {
			//	if (!p->m_bOnDataFlag) {
			//		p->m_DevStatusDlg.m_ListCtrlStatus.SetItemText(i, 2, _T("失败"));
			//		goto STATUS_AGAIN;
			//	}

			//	//vector<CString>::iterator it;
			//	//if (!p->m_SvrUserHistoryVec.empty()) {
			//	//	it = find(p->m_SvrUserHistoryVec.begin(), p->m_SvrUserHistoryVec.end(), p->m_EditSvrUser);
			//	//	if (it == p->m_SvrUserHistoryVec.end()) {
			//	//		//p->m_SvrUserHistoryVec.push_back(p->m_EditSvrUser);
			//	//		p->m_SvrUserHistoryVec.insert(p->m_SvrUserHistoryVec.begin(), p->m_EditSvrUser);
			//	//		//p->m_ComboBoxSvrUserItem.AddString(p->m_EditSvrUser.GetString());
			//	//		if (p->m_SvrUserHistoryVec.size() >= 10)
			//	//			SetEvent(p->m_LoginInfoHistoryCacheThreadUpdateEvent);
			//	//	}

			//	//}
			//	//else {
			//	//	p->m_SvrUserHistoryVec.push_back(p->m_EditSvrUser);
			//	//	p->m_ComboBoxSvrUserItem.AddString(p->m_EditSvrUser.GetString());
			//	//}

			//	CString temp;
			//	temp = p->m_sdns;
			//	if (recv_count > 0) {
			//		CString beforeStr = p->m_DevStatusDlg.m_ListCtrlStatus.GetItemText(i, 1);
			//		beforeStr += temp;
			//		//temp = beforeStr;
			//		p->m_DevStatusDlg.m_ListCtrlStatus.SetItemText(i, 1, beforeStr.GetString());
			//		//p->m_DevStatusDlg.m_ListCtrlStatus.SetItemText(i, 1, temp.GetString());
			//		INT nRet = beforeStr.Find(_T("bad address"));
			//		if (nRet != -1) {
			//			p->m_DevStatusDlg.m_ListCtrlStatus.SetItemText(i, 2, _T("fail"));
			//			recv_count = 0;
			//		}

			//		nRet = beforeStr.Find(_T("unknown host"));
			//		if (nRet != -1) {
			//			p->m_DevStatusDlg.m_ListCtrlStatus.SetItemText(i, 2, _T("fail"));
			//			recv_count = 0;
			//		}

			//		nRet = beforeStr.Find(_T("packet loss"));
			//		if (nRet != -1) {
			//			nRet = beforeStr.Find(_T("100% packet loss"));
			//			if (nRet == -1) {
			//				p->m_DevStatusDlg.m_ListCtrlStatus.SetItemText(i, 2, _T("sucess"));
			//			}
			//			else {
			//				p->m_DevStatusDlg.m_ListCtrlStatus.SetItemText(i, 2, _T("fail"));
			//			}
			//			recv_count = 0;
			//		}
			//	}
			//	else {
			//		p->m_DevStatusDlg.m_ListCtrlStatus.SetItemText(i, 1, temp.GetString());
			//	}
			//	//memset(p->m_sdns, 0, 1024 * 4);
			//}
			//else {
			//	//msleep_c(50);
			//	switch (nRet)
			//	{
			//	case WAIT_TIMEOUT:
			//		SHOW_STATUS_INFO_S_A("获取数据时间超过10秒");
			//		break;
			//	case WAIT_FAILED:
			//		SHOW_STATUS_INFO_S_A("WaitForSingleObject error");
			//		break;
			//	default:
			//		break;
			//	}
			//	//continue;
			//
			//}
#endif

			if (recv_count > 0) {
				goto MULTI_STATUS_RECV;
			}

	STATUS_AGAIN:

			memset(localcmd, 0x00, 256);
			i++;
		}
		
		::InterlockedDecrement(&m_OnDataFlagCountLock);
		(CEdit *)p->GetDlgItem(IDC_PASSD_SVR_EDIT)->EnableWindow(TRUE);;
		p->m_ComboBoxSvrUserItem.EnableWindow(TRUE);
		//SetEvent(m_EndRunCheck);
		//p->m_Statusbar.SetPaneText(2, _T("测试结束"));
		SHOW_STATUS_INFO_S_A("测试结束"); 
		continue;

	DEV_OFF_LINE_TEST:

		::InterlockedDecrement(&m_OnDataFlagCountLock);
		(CEdit *)p->GetDlgItem(IDC_PASSD_SVR_EDIT)->EnableWindow(TRUE);;
		p->m_ComboBoxSvrUserItem.EnableWindow(TRUE);
		if (m_bFstCurDevConfigInfoFlag) {
			//if (p->m_DevSettingDlg.m_nItemCount <= 0) {
			::InterlockedDecrement(&m_FstCurDevConfigInfoCountLock);
			m_bFstCurDevConfigInfoFlag = FALSE;
			SetEvent(p->m_ModThreadStartCmdEvent);
			//}
			/*::InterlockedDecrement(&m_FstCurDevConfigInfoCountLock);
			m_bFstCurDevConfigInfoFlag = FALSE;*/
		}

		p->m_bDevConnectStatusFlag = FALSE;
		p->m_bReadyStartFlag = FALSE;
		//p->m_BtnStartTest->EnableWindow(FALSE);
		p->m_BtnStartCheck->EnableWindow(FALSE);
		p->m_BtnStopCheck->EnableWindow(FALSE);
		p->m_BtnDisconnect->EnableWindow(FALSE);
		p->m_BtnDevCfgModify->EnableWindow(FALSE);
		p->m_BtnRestartDev->EnableWindow(FALSE);
		p->m_BtnVideo->EnableWindow(FALSE);
		p->m_BtnLive->EnableWindow(FALSE);
		//p->m_BtnConnect = NULL;
		//p->m_BtnConnect = (CButton *)p->GetDlgItem(IDC_CONNECT_BUTTON);
		p->m_BtnConnect->EnableWindow(TRUE);
		p->m_ComboBoxDevIDItem.EnableWindow(TRUE);
		p->m_ComboBoxDevUserItem.EnableWindow(TRUE);

		p->m_Statusbar.SetPaneText(2, _T("设备连接已断开"));

		SHOW_STATUS_INFO_S_A("中止测试");
		continue;

	TERMIMATE_CHECK:

		::InterlockedDecrement(&m_OnDataFlagCountLock);
		(CEdit *)p->GetDlgItem(IDC_PASSD_SVR_EDIT)->EnableWindow(TRUE);;
		p->m_ComboBoxSvrUserItem.EnableWindow(TRUE);
		if (m_bFstCurDevConfigInfoFlag) {
			//if (p->m_DevSettingDlg.m_nItemCount <= 0) {
			::InterlockedDecrement(&m_FstCurDevConfigInfoCountLock);
			m_bFstCurDevConfigInfoFlag = FALSE;
			SetEvent(p->m_ModThreadStartCmdEvent);
			//}
			/*::InterlockedDecrement(&m_FstCurDevConfigInfoCountLock);
			m_bFstCurDevConfigInfoFlag = FALSE;*/
		}
		p->m_Statusbar.SetPaneText(2, _T("测试中止"));
		SHOW_STATUS_INFO_S_A("中止测试");	

	}
	return 0;
}

unsigned int __stdcall Ckp2p_check_tool_win32Dlg::SaveCacheWorkThreadProc(PVOID arg)
{
	BOOL bExit = FALSE;
	DWORD nRet;

	Ckp2p_check_tool_win32Dlg *p = (Ckp2p_check_tool_win32Dlg*)arg;
	
	while (!bExit)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(p->m_SaveCacheThreadExitEvent, 0))
			bExit = TRUE;

		if ((nRet = WaitForSingleObject(p->m_SaveCacheThreadStartCmdEvent, 0)) != WAIT_OBJECT_0) {
			msleep_c(50);
			continue;
		}
		::InterlockedIncrement(&m_OnSaveCacheCountLock);

		//TODO:

		::InterlockedDecrement(&m_OnSaveCacheCountLock);
	}

	return 0;
}

unsigned int __stdcall Ckp2p_check_tool_win32Dlg::LoadCacheWorkThreadProc(PVOID arg)
{
	/*BOOL bExit = FALSE;
	DWORD nRet;

	Ckp2p_check_tool_win32Dlg *p = (Ckp2p_check_tool_win32Dlg*)arg;

	while (!bExit)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(p->m_ExcuteCmdThreadExitEvent, 0))
			bExit = TRUE;

		if ((nRet = WaitForSingleObject(p->m_ExecThreadStartCmdEvent, 0)) != WAIT_OBJECT_0) {
			msleep_c(50);
			continue;
		}
		::InterlockedIncrement(&m_OnDataFlagCountLock);
	}*/

	return 0;
}

unsigned int __stdcall Ckp2p_check_tool_win32Dlg::ShowInfoWorkThreadProc(PVOID arg)
{
	BOOL bExit = FALSE;
	DWORD nRet;

	Ckp2p_check_tool_win32Dlg *p = (Ckp2p_check_tool_win32Dlg*)arg;

	while (!bExit)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(p->m_SaveCacheThreadExitEvent, 0))
			bExit = TRUE;

		if ((nRet = WaitForSingleObject(p->m_SaveCacheThreadStartCmdEvent, 0)) != WAIT_OBJECT_0) {
			msleep_c(50);
			continue;
		}
		::InterlockedIncrement(&m_OnSaveCacheCountLock);

		//TODO:

		::InterlockedDecrement(&m_OnSaveCacheCountLock);
	}

	return 0;
}

unsigned int __stdcall Ckp2p_check_tool_win32Dlg::LoginInfoHistoryCacheWorkThreadProc(PVOID arg)
{
	BOOL bExit = FALSE;
	DWORD nRet;
	HANDLE h[2];

	Ckp2p_check_tool_win32Dlg *p = (Ckp2p_check_tool_win32Dlg*)arg;
	h[0] = p->m_LoginInfoHistoryCacheThreadUpdateEvent;
	h[1] = p->m_QueryDevInfoHistoryCacheThreadUpdateEvent;


	while (!bExit)
	{
		/*if (WAIT_OBJECT_0 == WaitForSingleObject(p->m_LoginInfoHistoryCacheThreadExitEvent, 0))
			bExit = TRUE;*/

		DWORD nRes = WaitForSingleObject(p->m_LoginInfoHistoryCacheThreadExitEvent, 0);
		if (nRes == WAIT_OBJECT_0) {
			bExit = TRUE;
		}


		nRet = WaitForMultipleObjects(2, h, FALSE, 0);
		if (nRet != WAIT_OBJECT_0 + 0 && nRet != WAIT_OBJECT_0 + 1) {
			msleep_c(50);
			continue;
		}

		/*if ((nRet = WaitForSingleObject(p->m_LoginInfoHistoryCacheThreadUpdateEvent, 0)) != WAIT_OBJECT_0) {
			msleep_c(50);
			continue;
		}*/
		//::InterlockedIncrement(&m_OnSaveCacheCountLock);

		switch (nRet) {
		case WAIT_OBJECT_0 + 0:
			switch (p->m_LoginHistoryCmd)
			{
			case 1:
				p->m_DevIDHistoryVec.pop_back();
				p->m_ComboBoxDevIDItem.Clear();
				for (vector<CString>::iterator it = p->m_DevIDHistoryVec.begin(); it != p->m_DevIDHistoryVec.begin(); it++) {
					p->m_ComboBoxDevIDItem.AddString((*it).GetString());
				}
				break;
			case 2:
				p->m_DevUserHistoryVec.pop_back();
				p->m_ComboBoxDevUserItem.Clear();
				for (vector<CString>::iterator it = p->m_DevUserHistoryVec.begin(); it != p->m_DevUserHistoryVec.begin(); it++) {
					p->m_ComboBoxDevUserItem.AddString((*it).GetString());
				}
				break;
			case 3:
				p->m_SvrUserHistoryVec.pop_back();
				p->m_ComboBoxSvrUserItem.Clear();
				for (vector<CString>::iterator it = p->m_SvrUserHistoryVec.begin(); it != p->m_SvrUserHistoryVec.begin(); it++) {
					p->m_ComboBoxSvrUserItem.AddString((*it).GetString());
				}
				break;
			default:
				break;
			}
			break;
		case WAIT_OBJECT_0 + 1:
			p->m_DevStatusInfoDetailDlg.m_CurDevIDHistoryVec.pop_back();
			p->m_DevStatusInfoDetailDlg.m_cDevIDComboBox.Clear();
			for (vector<CString>::iterator it = p->m_DevStatusInfoDetailDlg.m_CurDevIDHistoryVec.begin(); it != p->m_DevStatusInfoDetailDlg.m_CurDevIDHistoryVec.begin(); it++) {
				p->m_DevStatusInfoDetailDlg.m_cDevIDComboBox.AddString((*it).GetString());
			}
			break;
		}

		//::InterlockedDecrement(&m_OnSaveCacheCountLock);
	}

	return 0;
}

unsigned int __stdcall Ckp2p_check_tool_win32Dlg::OperateControlWorkThreadProc(PVOID arg)
{
	BOOL bExit = FALSE;
	DWORD nRet;
	char localcmd[256];
	memset(localcmd, 0x00, 256);
	HANDLE h[2];
	
	Ckp2p_check_tool_win32Dlg *p = (Ckp2p_check_tool_win32Dlg*)arg;
	h[0] = p->m_DevOfflineNotifyEvent;
	h[1] = p->m_ThreadNotifyCmdEvent;

	while (!bExit)
	{
		/*if (WAIT_OBJECT_0 == WaitForSingleObject(p->m_OperateControlThreadExitEvent, 0))
			bExit = TRUE;*/

		DWORD nRes = WaitForSingleObject(p->m_OperateControlThreadExitEvent, 0);
		if (nRes == WAIT_OBJECT_0) {
			bExit = TRUE;
		}

		if ((nRet = WaitForSingleObject(p->m_OperateControlThreadStartEvent, 0)) != WAIT_OBJECT_0) {
			msleep_c(50);
			continue;
		}

		::InterlockedIncrement(&m_OnOperateControlCountLock);

		vector<CString>::iterator it = p->m_TestFuncItemVec.begin();
		for (; it != p->m_TestFuncItemVec.end(); it++) {
			if ((*it).Compare(TEST_RESTART_DEV) == 0) {
				strcpy(localcmd, "reboot");
				SHOW_STATUS_INFO_S_A("发送重启设备指令");
			}
			else if ((*it).Compare(TEST_RESTART_APP) == 0) {

			}
			else if ((*it).Compare(TEST_RESTART_IOTDAEMON) == 0) {

			} 

			int send_size = IOT_SHELL_Send(p->m_Session, localcmd, strlen(localcmd) + 1, &p->m_ReqData);
				
			nRet = WaitForMultipleObjects(2, h, FALSE, 30000);
			switch (nRet)
			{
			case WAIT_FAILED:
				SHOW_STATUS_INFO_S_A("WaitForSingleObject error");
				break;
			case WAIT_TIMEOUT:
				SHOW_STATUS_INFO_S_A("重启设备失败");
				p->m_nCurRestartDevIDCount = 0;
				break;
			case WAIT_OBJECT_0 + 0:
				p->m_bDevConnectStatusFlag = FALSE;
				p->m_bReadyStartFlag = FALSE;
		
				//p->m_BtnStartTest->EnableWindow(FALSE);
				p->m_BtnStartCheck->EnableWindow(FALSE);
				p->m_BtnStopCheck->EnableWindow(FALSE);
				p->m_BtnDisconnect->EnableWindow(FALSE);
				p->m_BtnDevCfgModify->EnableWindow(FALSE);
				p->m_BtnRestartDev->EnableWindow(FALSE);
				//p->m_BtnConnect = NULL;
				//p->m_BtnConnect = (CButton *)p->GetDlgItem(IDC_CONNECT_BUTTON);
				p->m_BtnVideo->EnableWindow(FALSE);
				p->m_BtnLive->EnableWindow(FALSE);
				p->m_BtnConnect->EnableWindow(TRUE);
				p->m_BtnDevSysTimeModify->EnableWindow(FALSE);

				p->m_ComboBoxDevIDItem.EnableWindow(TRUE);
				p->m_ComboBoxDevUserItem.EnableWindow(TRUE);
				(CEdit *)p->GetDlgItem(IDC_PASSD_DEV_EDIT)->EnableWindow(TRUE);
				p->m_Statusbar.SetPaneText(2, _T("设备重启"));
				SHOW_STATUS_INFO_S_A("重启设备成功，设备正在重启");
				Sleep(45000);
				break;
			case WAIT_OBJECT_0 + 1:
				if (!p->m_bOnDataFlag) {
					SHOW_STATUS_INFO_S_A("重启设备失败");
					p->m_nCurRestartDevIDCount = 0;
				}
				break;
			}			
														
		}

		::InterlockedDecrement(&m_OnOperateControlCountLock);
	}

	return 0;
}

unsigned int __stdcall Ckp2p_check_tool_win32Dlg::CheckDevOfflineStatusWorkThreadProc(PVOID arg)
{
	BOOL bExit = FALSE;
	DWORD nRet;
	//HANDLE h[2];

	Ckp2p_check_tool_win32Dlg *p = (Ckp2p_check_tool_win32Dlg*)arg;
	//h[0] = p->m_DevOfflineNotifyEvent;
	//h[1] = p->m_ThreadNotifyCmdEvent;

	while (!bExit)
	{
		/*if (WAIT_OBJECT_0 == WaitForSingleObject(p->m_CheckDevOfflineStatusThreadExitEvent, 0))
			bExit = TRUE;*/

		DWORD nRes = WaitForSingleObject(p->m_CheckDevOfflineStatusThreadExitEvent, 0);
		if (nRes == WAIT_OBJECT_0) {
			bExit = TRUE;
		}

		if ((nRet = WaitForSingleObject(p->m_CheckDevOfflineStatusThreadStartEvent, 0)) != WAIT_OBJECT_0) {
			msleep_c(50);
			continue;
		}

		p->m_bDevConnectStatusFlag = FALSE;
		p->m_bReadyStartFlag = FALSE;
		(CButton *)p->GetDlgItem(IDC_START_CHECK_MAIN_BUTTON)->EnableWindow(FALSE);
		//p->m_BtnStartCheck->EnableWindow(FALSE);
		(CButton *)p->GetDlgItem(IDC_STOP_CHECK_MAIN_BUTTON)->EnableWindow(FALSE);
		//p->m_BtnStopCheck->EnableWindow(FALSE);
		(CButton *)p->GetDlgItem(IDC_DISCONNECT_BUTTON)->EnableWindow(FALSE);
		//p->m_BtnDisconnect->EnableWindow(FALSE);
		(CButton *)p->GetDlgItem(IDC_MOD_CONFIG_BUTTON)->EnableWindow(FALSE);
		//p->m_BtnDevCfgModify->EnableWindow(FALSE);
		(CButton *)p->GetDlgItem(IDC_RESTART_DEV_BUTTON)->EnableWindow(FALSE);
		//p->m_BtnRestartDev->EnableWindow(FALSE);
		(CButton *)p->GetDlgItem(IDC_VEDIO_BUTTON)->EnableWindow(FALSE);
		//p->m_BtnVideo->EnableWindow(FALSE);
		(CButton *)p->GetDlgItem(IDC_LIVE_BUTTON)->EnableWindow(FALSE);
		//p->m_BtnLive->EnableWindow(FALSE);
		(CButton *)p->GetDlgItem(IDC_CONNECT_BUTTON)->EnableWindow(TRUE);
		//p->m_BtnConnect->EnableWindow(TRUE);
		//(CButton *)p->GetDlgItem(IDC_MOD_TIME_DEV_BUTTON)->EnableWindow(FALSE);
		//p->m_BtnDevSysTimeModify->EnableWindow(FALSE);

		p->m_ComboBoxDevIDItem.EnableWindow(TRUE);
		p->m_ComboBoxDevUserItem.EnableWindow(TRUE);
		(CEdit *)p->GetDlgItem(IDC_PASSD_DEV_EDIT)->EnableWindow(TRUE);

		p->m_Statusbar.SetPaneText(2, _T("设备连接已断开"));

		//录像或直播视频播放中，通知录像或直播停止测试
		::InterlockedIncrement(&CKvideoDlg::m_OnReplayRecCountLock);
		::InterlockedIncrement(&CKliveDlg::m_OnPlayLiveCountLock);
		if ((::InterlockedDecrement(&CKvideoDlg::m_OnReplayRecCountLock) != 0)
			|| (::InterlockedDecrement(&CKliveDlg::m_OnPlayLiveCountLock) != 0)) {

			SetEvent(p->m_DevOfflineNotifyEvent);
		}

	}
	return 0;
}

BOOL Ckp2p_check_tool_win32Dlg::start_dev_config_test()
{
	unsigned char decrypt[16];
	char localmac[32];
	memset(localmac, 0x00, 32);
	memset(decrypt, 0x00, 16);
	memset(&m_ReqData, 0x00, sizeof(auth_psd_req_data_t));
	/*char localcmd[256];
	memset(localcmd, 0x00, 256);*/

	if (!m_TestItemVec.empty() || !m_TestStatusItemVec.empty()) {
		MD5Init(&m_Md5);
		MD5Update(&m_Md5, (unsigned char*)((char*)(CW2A(m_EditSvrPassword.GetString()))), m_EditSvrPassword.GetLength());
		MD5Final(&m_Md5, decrypt);

		get_mac(localmac);
		memcpy(m_ReqData.uid, CW2A(m_EditDevID.GetString()), m_EditDevID.GetLength());
		memcpy(m_ReqData.usr, CW2A(m_EditSvrUser.GetString()), m_EditSvrUser.GetLength());
		memcpy(m_ReqData.pwd, decrypt, 16);
		memcpy(m_ReqData.ver, "1", strlen("1"));
		memcpy(m_ReqData.mac, localmac, strlen(localmac));

		SetEvent(m_This->m_ExecThreadStartCmdEvent);
	}
	return TRUE;
}

BOOL Ckp2p_check_tool_win32Dlg::start_dev_status_test()
{
#if 0
	send_cmd_start = 0;
	unsigned char decrypt[16];
	char localmac[32];
	memset(localmac, 0x00, 32);
	memset(decrypt, 0x00, 16);
	//memset(&m_ReqData, 0x00, sizeof(auth_psd_req_data_t));
	char localcmd[256];
	memset(localcmd, 0x00, 256);

	if (!m_TestStatusItemVec.empty()) {
		MD5Init(&m_Md5);
		MD5Update(&m_Md5, (unsigned char*)((char*)(CW2A(m_EditSvrPassword.GetString()))), strlen((char *)(CW2A(m_EditSvrPassword.GetString()))));
		MD5Final(&m_Md5, decrypt);

		get_mac(localmac);
		memcpy(m_ReqData.uid, CW2A(m_EditDevID.GetString()), m_EditDevID.GetLength());
		memcpy(m_ReqData.usr, CW2A(m_EditSvrUser.GetString()), m_EditSvrUser.GetLength());
		memcpy(m_ReqData.pwd, decrypt, 16);
		memcpy(m_ReqData.ver, "1", strlen("1"));
		memcpy(m_ReqData.mac, localmac, strlen(localmac));

		vector<CString>::iterator it = m_TestStatusItemVec.begin();
		for (; it != m_TestStatusItemVec.end(); it++) {
			if ((*it).Compare(TEST_PING_ITEM) == 0) {
				CString pcmd = L"ping -c ";
				pcmd += m_PingSettingDlg.m_PNum;
				pcmd += L" -s ";
				pcmd += m_PingSettingDlg.m_PpkgSize;
				pcmd += L" ";
				pcmd += m_PingSettingDlg.m_PwebSite;
				strcpy(localcmd, CW2A(pcmd.GetString()));
				SHOW_STATUS_INFO_A("MTU获取，执行命令：");
				SHOW_STATUS_INFO_W(pcmd.GetString());
			}
			else if ((*it).Compare(TEST_NETWORK_ITEM) == 0) {
				strcpy(localcmd, "netstat | grep ip | awk '{print $6}'");
				SHOW_STATUS_INFO_A("网络连接信息获取，执行命令：netstat | grep ip | awk '{print $6}'");
			}
			else if ((*it).Compare(TEST_ROUTE_ITEM) == 0) {
				strcpy(localcmd, "route'");
				SHOW_STATUS_INFO_A("路由表信息获取，执行命令：route");
			}
			else if ((*it).Compare(TEST_BANDWIDTH) == 0) {
				//strcpy(localcmd, "cat /proc/net/rtl8188fu/wlan0/rx_signal");
				//SHOW_STATUS_INFO_A("带宽信息获取，执行命令：cat /proc/net/rtl8188fu/wlan0/rx_signal");
			}
			else if ((*it).Compare(TEST_LOG_ITEM) == 0) {
				//strcpy(localcmd, "netstat -anpt");
				//SHOW_STATUS_INFO_A("日志数据获取，执行命令：netstat -anpt");
			}

			send_cmd_start = time(NULL);
			m_bOnDataFlag = FALSE;
			m_Ecode = 0;

			IOT_SHELL_Send(m_Session, localcmd, strlen(localcmd) + 1, &m_ReqData);
			while (!m_bOnDataFlag) {
				msleep_c(1000);
				if (time(NULL) - send_cmd_start > 30) {
					SHOW_STATUS_INFO_A("get data time out 30 sec!");
					continue;
				}
			}
		}
	}
#endif
	return TRUE;
}

void Ckp2p_check_tool_win32Dlg::OnBnClickedStartTestButton()
{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	INT_PTR nRes = 0;
	CEdit* pEditSvrUser = NULL;
	CEdit* pEditSvrPassd = NULL;

	if (m_TestItemVec.empty() && m_TestStatusItemVec.empty()) {
		nRes = MessageBox(_T("请选择需要测试的项目并点击导入按钮"), _T("信息提示"), MB_OK);
		if (IDOK == nRes) 
			m_comboBoxTestItem.SetFocus();
		return;
	}
	
	if (m_EditSvrUser.IsEmpty() || m_EditSvrPassword.IsEmpty()) {
		nRes = MessageBox(_T("服务器用户名或密码不能为空"), _T("信息提示"), MB_OK);
		if (IDOK == nRes) {
			if (m_EditDevID.IsEmpty()) {
				pEditSvrUser = (CEdit *)GetDlgItem(IDC_USER_SVR_EDIT);
				pEditSvrUser->SetFocus();
			}
			else {
				pEditSvrPassd = (CEdit *)GetDlgItem(IDC_PASSD_SVR_EDIT);
				pEditSvrPassd->SetFocus();
			}
		}
		return;
	}
	m_This = this;
	
	::InterlockedIncrement(&m_OnDataFlagCountLock);
	if (::InterlockedDecrement(&m_OnDataFlagCountLock) != 0) {
		SHOW_STATUS_INFO_A("正在测试，请等待");
		return;
	}
	SHOW_STATUS_INFO_A("开始测试");
	m_Statusbar.SetPaneText(2, _T("测试中")); 

	if (m_bInitInfoPage0) {
		nRes = MessageBox(_T("已有选项测试过，是否再次测试？"), _T("信息提示"), MB_YESNO);
		if (IDYES == nRes) {
			m_DevConfigDlg.m_ListCtrlConfig.DeleteAllItems();
			m_DevStatusDlg.m_ListCtrlStatus.DeleteAllItems();
			UpdateData(FALSE);
			
		}
		else {
			SHOW_STATUS_INFO_A("测试结束");
			m_Statusbar.SetPaneText(2, _T("测试结束"));
			return;
		}
	}

	//初始化信息列表显示
	init_info_list();

	//启动测试线程
	//::ResumeThread(m_ExecHandleThr);
	//启动检测
	Run_Check();
	//设备配置参数测试
	//start_dev_config_test();
	//设备状态参数测试
	//start_dev_status_test();

	/*while (1) {
		Sleep(500);
		::InterlockedIncrement(&m_OnDataFlagCountLock);
		if (::InterlockedDecrement(&m_OnDataFlagCountLock) == 0) {
			break;
		}
		
	}*/
	m_Statusbar.SetPaneText(2, _T("测试结束"));

	m_bInitInfoPage0 = TRUE;
	UpdateData(FALSE);
}

void Ckp2p_check_tool_win32Dlg::OnBnClickedLoadTestItemButton()
{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	CString item;
	map<LONG64, pkp2p_dev_config_t>::iterator c_pos;
	map<LONG64, pkp2p_dev_status_t>::iterator s_pos;
	m_comboBoxTestItem.GetWindowText(item);

	if (item.Compare(TEST_MTU_ITEM) == 0)
		goto CONFIG_END;
	else if (item.Compare(TEST_GEATEWAY_ITEM) == 0)
		goto CONFIG_END;
	else if (item.Compare(TEST_DNS_ITEM) == 0)
		goto CONFIG_END;
	else if (item.Compare(TEST_SIGNAL_ITEM) == 0)
		goto CONFIG_END;
	else if (item.Compare(TEST_CPU_ITEM) == 0)
		goto CONFIG_END;
	else if (item.Compare(TEST_MEMORY_ITEM) == 0)
		goto CONFIG_END;
	else if (item.Compare(TEST_PROCESS_ITEM) == 0)
		goto CONFIG_END;
	else if (item.Compare(TEST_THREAD_ITEM) == 0)
		goto CONFIG_END;
	else if (item.Compare(TEST_PING_ITEM) == 0)
		goto STATUS_END;
	else if (item.Compare(TEST_NETWORK_ITEM) == 0)
		goto STATUS_END;
	else if (item.Compare(TEST_ROUTE_ITEM) == 0)
		goto STATUS_END;
	else if (item.Compare(TEST_BANDWIDTH) == 0)
		goto STATUS_END;
	else if (item.Compare(TEST_LOG_ITEM) == 0)
		goto STATUS_END;

	size_t item_num;
	int i;

STATUS_END:
	if (!m_TestStatusItemVec.empty()) {
		item_num = m_TestStatusItemVec.size();
		for (i = 0; i < (int)item_num; i++) {
			if (lstrcmp(m_TestStatusItemVec[i].GetString(), item.GetString()) == 0) {
				MessageBox(_T("已选择该测试项，请重新选择"), _T("信息提示"), MB_OK);
				return;
			}
		}
	}

	if (item.Compare(TEST_PING_ITEM) == 0) {
		m_PingSettingDlg.DoModal();
	}

	m_ListBoxTestItem.AddString(item.GetBuffer(0));
	m_TestStatusItemVec.push_back(item);

	m_nStatusItemCount++;

	m_bReadyLoadFlag = TRUE;
	if (m_bReadyStartFlag && m_bReadyLoadFlag) {
		m_BtnStartTest->EnableWindow(TRUE);
	}
	
	c_pos = m_DevConfigInfoMap.find(atoll(CW2A(m_EditDevID.GetString())));
	if (c_pos != m_DevConfigInfoMap.end()) {
		
	}

	//init_info_list(1);

	return;

CONFIG_END:
	if (!m_TestItemVec.empty()) {
		item_num = m_TestItemVec.size();
		for (i = 0; i < (int)item_num; i++) {
			if (lstrcmp(m_TestItemVec[i].GetString(), item.GetString()) == 0) {
				MessageBox(_T("已选择该测试项，请重新选择"), _T("信息提示"), MB_OK);
				return;
			}
		}
	}
	m_ListBoxTestItem.AddString(item.GetBuffer(0));
	m_TestItemVec.push_back(item);

	m_nConfigItemCount++;

	m_bReadyLoadFlag = TRUE;
	if (m_bReadyStartFlag && m_bReadyLoadFlag) {
		m_BtnStartTest->EnableWindow(TRUE);
	}

	//init_info_list(0);

	return;
}

void Ckp2p_check_tool_win32Dlg::init_info_list()
{
	int i, add_count;

	m_nCurConfigItemCount = m_DevConfigDlg.m_ListCtrlConfig.GetItemCount();

	if ((add_count = m_nConfigItemCount - m_nCurConfigItemCount) > 0) {

		for (i = 0; i < add_count; i++) {
				if (m_TestItemVec[m_nCurConfigItemCount + i].Compare(TEST_MTU_ITEM) == 0) {
					m_DevConfigDlg.m_ListCtrlConfig.InsertItem(m_nCurConfigItemCount + i, _T("MTU"));
				}
				else if (m_TestItemVec[m_nCurConfigItemCount + i].Compare(TEST_GEATEWAY_ITEM) == 0) {
					m_DevConfigDlg.m_ListCtrlConfig.InsertItem(m_nCurConfigItemCount + i, _T("网关"));
				}
				else if (m_TestItemVec[m_nCurConfigItemCount + i].Compare(TEST_DNS_ITEM) == 0) {
					m_DevConfigDlg.m_ListCtrlConfig.InsertItem(m_nCurConfigItemCount + i, _T("DNS"));
				}
				else if (m_TestItemVec[m_nCurConfigItemCount + i].Compare(TEST_SIGNAL_ITEM) == 0) {
					m_DevConfigDlg.m_ListCtrlConfig.InsertItem(m_nCurConfigItemCount + i, _T("信号强度"));
				}
				else if (m_TestItemVec[m_nCurConfigItemCount + i].Compare(TEST_CPU_ITEM) == 0) {
					m_DevConfigDlg.m_ListCtrlConfig.InsertItem(m_nCurConfigItemCount + i, _T("CPU占用"));
				}
				else if (m_TestItemVec[m_nCurConfigItemCount + i].Compare(TEST_MEMORY_ITEM) == 0) {
					m_DevConfigDlg.m_ListCtrlConfig.InsertItem(m_nCurConfigItemCount + i, _T("内存占用"));
				}
				else if (m_TestItemVec[m_nCurConfigItemCount + i].Compare(TEST_PROCESS_ITEM) == 0) {
					m_DevConfigDlg.m_ListCtrlConfig.InsertItem(m_nCurConfigItemCount + i, _T("进程"));
				}
				else if (m_TestItemVec[m_nCurConfigItemCount + i].Compare(TEST_THREAD_ITEM) == 0) {
					m_DevConfigDlg.m_ListCtrlConfig.InsertItem(m_nCurConfigItemCount + i, _T("线程"));
				}
				else if (m_TestItemVec[m_nCurConfigItemCount + i].Compare(TEST_DEV_SYS_TIME_ITEM) == 0) {
					m_DevConfigDlg.m_ListCtrlConfig.InsertItem(m_nCurConfigItemCount + i, _T("设备系统时间"));
				}
				else if (m_TestItemVec[m_nCurConfigItemCount + i].Compare(_T("test")) == 0) {
					m_DevConfigDlg.m_ListCtrlConfig.InsertItem(m_nCurConfigItemCount + i, _T("test"));
				}
		}
	}

	m_nCurStatusItemCount = m_DevStatusDlg.m_ListCtrlStatus.GetItemCount();

	if ((add_count = m_nStatusItemCount - m_nCurStatusItemCount) > 0) {
		for (i = 0; i < add_count; i++) {
			if (m_TestStatusItemVec[m_nCurStatusItemCount + i].Compare(TEST_PING_ITEM) == 0) {
				m_DevStatusDlg.m_ListCtrlStatus.InsertItem(m_nCurStatusItemCount + i, _T("PING检测"));
			}
			else if (m_TestStatusItemVec[m_nCurStatusItemCount + i].Compare(TEST_NETWORK_ITEM) == 0) {
				m_DevStatusDlg.m_ListCtrlStatus.InsertItem(m_nCurStatusItemCount + i, _T("网络连接状况检测"));
			}
			else if (m_TestStatusItemVec[m_nCurStatusItemCount + i].Compare(TEST_ROUTE_ITEM) == 0) {
				m_DevStatusDlg.m_ListCtrlStatus.InsertItem(m_nCurStatusItemCount + i, _T("路由表检测"));
			}
			else if (m_TestStatusItemVec[m_nCurStatusItemCount + i].Compare(TEST_BANDWIDTH) == 0) {
				m_DevStatusDlg.m_ListCtrlStatus.InsertItem(m_nCurStatusItemCount + i, _T("带宽检测"));
			}
			else if (m_TestStatusItemVec[m_nCurStatusItemCount + i].Compare(TEST_LOG_ITEM) == 0) {
				m_DevStatusDlg.m_ListCtrlStatus.InsertItem(m_nCurStatusItemCount + i, _T("日志数据获取检测"));
			}
		}
	}
	
}

void Ckp2p_check_tool_win32Dlg::check_current_config_info_once(LPCWSTR item)
{
	if (!m_TestStatusItemVec.empty()) {
		m_TestStatusItemVec.clear();
	}

	if (!m_TestItemVec.empty()) {
		m_TestItemVec.clear();
	}
	if (lstrcmp(item, _T("all")) == 0) {
		m_TestItemVec.push_back(CString("MTU"));
		m_TestItemVec.push_back(CString("DNS"));
		m_TestItemVec.push_back(CString(_T("网关")));
	}
	else if (lstrcmp(item, TEST_MTU_ITEM) == 0) {
		m_TestItemVec.push_back(CString("MTU"));
	}
	else if (lstrcmp(item, TEST_GEATEWAY_ITEM) == 0) {
		m_TestItemVec.push_back(CString(_T("网关")));
	}
	else if (lstrcmp(item, TEST_DNS_ITEM) == 0) {
		m_TestItemVec.push_back(CString("DNS"));
	}

	m_bFstCurDevConfigInfoFlag = TRUE;
	//UpdateData(TRUE);
	unsigned char decrypt[16];
	char localmac[32];
	memset(localmac, 0x00, 32);
	memset(decrypt, 0x00, 16);
	m_bShellSendReqFlag = FALSE;
	if (!m_bShellSendReqFlag) {
		memset(&m_ReqData, 0x00, sizeof(auth_psd_req_data_t));
		MD5Init(&m_Md5);
		MD5Update(&m_Md5, (unsigned char*)((char*)(CW2A(m_EditSvrPassword.GetString()))), strlen((char *)(CW2A(m_EditSvrPassword.GetString()))));
		MD5Final(&m_Md5, decrypt);

		get_mac(localmac);
		memcpy(m_ReqData.uid, CW2A(m_EditDevID.GetString()), m_EditDevID.GetLength());
		memcpy(m_ReqData.usr, CW2A(m_EditSvrUser.GetString()), m_EditSvrUser.GetLength());
		memcpy(m_ReqData.pwd, decrypt, 16);
		memcpy(m_ReqData.ver, "1", strlen("1"));
		memcpy(m_ReqData.mac, localmac, strlen(localmac));
		m_bShellSendReqFlag = TRUE;
	}

	::InterlockedIncrement(&m_FstCurDevConfigInfoCountLock);
	SetEvent(m_ExecThreadStartCmdEvent);
	
}

BOOL Ckp2p_check_tool_win32Dlg::init_from_config_ini()
{
	GetPrivateProfileString(L"TurnServer", L"ip", L"118.190.84.189", m_TurnServerIp.GetBuffer(MAX_PATH), MAX_PATH, L"config.ini");
	GetPrivateProfileString(L"TurnServer", L"port", L"19999", m_TurnServerPort.GetBuffer(MAX_PATH), MAX_PATH, L"config.ini");
	return TRUE;
}

#if 0
void Ckp2p_check_tool_win32Dlg::front_info_in_queue(TEST_DEV_ITEM_TYPE type, LONG64 id, PVOID arg)
{
	switch (type) {
	case DEV_CONFIG:
		kp2p_dev_config_t *p = (kp2p_dev_config_t *)arg;
		m_DevConfigInfoMap.insert(map<LONG64, kp2p_dev_config_t *>::value_type(1, p));
		break;
	case DEV_STATUS:
		kp2p_dev_status_t *p = (kp2p_dev_status_t *)arg;
		m_DevConfigInfoMap.insert(map<LONG64, kp2p_dev_config_t *>::value_type(1, p));
		break;
	default:
		break;
	}
}
#endif

void Ckp2p_check_tool_win32Dlg::push_info_in_queue(TEST_DEV_ITEM_TYPE type, LONG64 id, PVOID arg)
{
	map<LONG64, kp2p_dev_config_t *>::iterator c_pos;
	map<LONG64, kp2p_dev_status_t *>::iterator s_pos;

	switch (type) {
		case DEV_CONFIG:
			c_pos = m_DevConfigInfoMap.find(id);
			if (c_pos == m_DevConfigInfoMap.end()) {
				m_DevConfigInfoMap.insert(map<LONG64, kp2p_dev_config_t *>::value_type(id, (kp2p_dev_config_t *)arg));
			}			
			break;
		case DEV_STATUS:
			s_pos = m_DevStatusInfoMap.find(id);
			if (s_pos == m_DevStatusInfoMap.end()) {
				m_DevStatusInfoMap.insert(map<LONG64, kp2p_dev_status_t *>::value_type(id, (kp2p_dev_status_t *)arg));
			}
			break;
		default:
			break;
	}

}

void Ckp2p_check_tool_win32Dlg::del_info_out_queue(TEST_DEV_ITEM_TYPE type, LONG64 arg)
{
	switch (type) {
		case DEV_CONFIG:
		
			break;
		case DEV_STATUS:
	
			break;
		default:
			break;
	}
}

void Ckp2p_check_tool_win32Dlg::pop_info_out_queue(TEST_DEV_ITEM_TYPE type, LONG64 arg)
{

	switch (type) {
		case DEV_CONFIG:

			break;
		case DEV_STATUS:

			break;
		default:
			break;
	}
}

void Ckp2p_check_tool_win32Dlg::pop_info_out_queue(TEST_DEV_ITEM_TYPE type, PVOID arg)
{
	switch (type) {
		case DEV_CONFIG:
			break;
		case DEV_STATUS:
			break;
		default:
			break;
	}
}

void Ckp2p_check_tool_win32Dlg::clear_queue(TEST_DEV_ITEM_TYPE type)
{
	switch (type) {
		case DEV_CONFIG:
			break;
		case DEV_STATUS:
			break;
		default:
			break;
	}
}

void Ckp2p_check_tool_win32Dlg::OnBnClickedVedioButton()
{
	// TODO: 在此添加控件通知处理程序代码
	
	m_VideoDlg.SetParent(this);
	INT_PTR nRes = m_VideoDlg.DoModal();
	if (nRes == IDCANCEL) {
		/*if (dlg.m_pwd == password)
		{
		m_result = "ÃÜÂëÕýÈ·£¡";
		UpdateData(FALSE);
		}
		else {
		m_result = "ÃÜÂë´íÎó£¡";
		UpdateData(FALSE);
		}*/
	}
}


void Ckp2p_check_tool_win32Dlg::OnBnClickedLiveButton()
{
	// TODO: 在此添加控件通知处理程序代码
	
	m_LiveDlg.SetParentThis(this);
	m_LiveDlg.SetLiveChannel(m_Channel);

	INT_PTR nRes = m_LiveDlg.DoModal();
	if (nRes == IDCANCEL) {
		/*if (dlg.m_pwd == password)
		{
		m_result = "ÃÜÂëÕýÈ·£¡";
		UpdateData(FALSE);
		}
		else {
		m_result = "ÃÜÂë´íÎó£¡";
		UpdateData(FALSE);
		}*/
	}
}


void Ckp2p_check_tool_win32Dlg::OnBnClickedModConfigButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	INT_PTR nRes;
	time_t send_cmd_start = 0;
	unsigned char decrypt[16];
	char localmac[32];
	memset(localmac, 0x00, 32);
	memset(decrypt, 0x00, 16);

	::InterlockedIncrement(&m_OnDataFlagCountLock);
	if (::InterlockedDecrement(&m_OnDataFlagCountLock) != 0) {
		//MessageBox(_T("正在查询修改中，请等待"), _T("信息提示"), MB_OK);
		nRes = MessageBox(_T("正在测试中,是否要中止？"), _T("信息提示"), MB_YESNO);
		if (IDNO == nRes) {
			return;
		}
		SetEvent(m_TerminateCheckNotifyEvent);
	}

	m_ComboBoxSvrUserItem.GetWindowText(m_EditSvrUser);
	CEdit* pEditSvrPassd = NULL;
	if (m_EditSvrUser.IsEmpty() || m_EditSvrPassword.IsEmpty()) {
		nRes = MessageBox(_T("服务器用户名或密码不能为空"), _T("信息提示"), MB_OK);
		if (IDOK == nRes) {
			if (m_EditSvrUser.IsEmpty()) {
				m_ComboBoxSvrUserItem.SetFocus();
			}
			else {
				pEditSvrPassd = (CEdit *)GetDlgItem(IDC_PASSD_SVR_EDIT);
				pEditSvrPassd->SetFocus();
			}
		}
		return;
	}

	//m_This = this;
	//m_DevSettingDlg.Init(this);
	m_DevSettingDlg.SetMainParent(this);

	//启动设备配置修改线程
	//::ResumeThread(m_ModHandleThr);

	m_ComboBoxDevIDItem.GetWindowText(m_EditDevID);
	m_ComboBoxDevUserItem.GetWindowText(m_EditDevUser);
	m_ComboBoxSvrUserItem.GetWindowText(m_EditSvrUser);

	//查询当前设备配置参数
	check_current_config_info_once();
	
	nRes = m_DevSettingDlg.DoModal();
	//UpdateData(FALSE);
	if (nRes == IDOK) {
		
		/*if (m_DevSettingDlg.m_bGateWay)
			m_DevSettingDlg.m_nItemCount += 1;
		if (m_DevSettingDlg.m_bDNS)
			m_DevSettingDlg.m_nItemCount += 1;
		if (m_DevSettingDlg.m_bMTU)
			m_DevSettingDlg.m_nItemCount += 1;*/

		if (!m_bShellSendReqFlag) {
			memset(&m_ReqData, 0x00, sizeof(auth_psd_req_data_t));
			MD5Init(&m_Md5);
			MD5Update(&m_Md5, (unsigned char*)((char*)(CW2A(m_EditSvrPassword.GetString()))), strlen((char *)(CW2A(m_EditSvrPassword.GetString()))));
			MD5Final(&m_Md5, decrypt);

			get_mac(localmac);
			memcpy(m_ReqData.uid, CW2A(m_EditDevID.GetString()), m_EditDevID.GetLength());
			memcpy(m_ReqData.usr, CW2A(m_EditSvrUser.GetString()), m_EditSvrUser.GetLength());
			memcpy(m_ReqData.pwd, decrypt, 16);
			memcpy(m_ReqData.ver, "1", strlen("1"));
			memcpy(m_ReqData.mac, localmac, strlen(localmac));
			m_bShellSendReqFlag = TRUE;
		}

		SetEvent(m_ModThreadStartCmdEvent);

	}
}

void Ckp2p_check_tool_win32Dlg::OnBnClickedRestartDevButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	INT_PTR  nRes;
	unsigned char decrypt[16];
	char localmac[32];
	memset(localmac, 0x00, 32);
	memset(decrypt, 0x00, 16);

	::InterlockedIncrement(&m_OnDataFlagCountLock);
	if (::InterlockedDecrement(&m_OnDataFlagCountLock) != 0) {
		//MessageBox(_T("正在查询修改中，请等待"), _T("信息提示"), MB_OK);
		nRes = MessageBox(_T("正在测试中,是否要中止并重启设备？"), _T("信息提示"), MB_YESNO);
		if (IDNO == nRes) {
			return;
		}
		SetEvent(m_TerminateCheckNotifyEvent);
	}

	::InterlockedIncrement(&m_OnOperateControlCountLock);
	if (::InterlockedDecrement(&m_OnOperateControlCountLock) != 0) {
		SHOW_STATUS_INFO_A("正在进行，请等待");
		return;
	}

	m_ComboBoxSvrUserItem.GetWindowText(m_EditSvrUser);
	CEdit* pEditSvrPassd = NULL;
	if (m_EditSvrUser.IsEmpty() || m_EditSvrPassword.IsEmpty()) {
		nRes = MessageBox(_T("服务器用户名或密码不能为空"), _T("信息提示"), MB_OK);
		if (IDOK == nRes) {
			if (m_EditSvrUser.IsEmpty()) {
				m_ComboBoxSvrUserItem.SetFocus();
			}
			else {
				pEditSvrPassd = (CEdit *)GetDlgItem(IDC_PASSD_SVR_EDIT);
				pEditSvrPassd->SetFocus();
			}
		}
		return;
	}

	/*if (!m_bOnDataFlag) {
		MessageBox(_T("服务鉴权失败，无法修改"), _T("信息提示"), MB_OK);
		return;
	}*/

	nRes = MessageBox(_T("确认需要重启设备？"), _T("信息提示"), MB_YESNO);
	if (IDNO == nRes) {
		return;
	}

	if (!m_TestFuncItemVec.empty()) {
		m_TestFuncItemVec.clear();
	}

	m_bShellSendReqFlag = FALSE;
	if (!m_bShellSendReqFlag) {
		memset(&m_ReqData, 0x00, sizeof(auth_psd_req_data_t));
		MD5Init(&m_Md5);
		MD5Update(&m_Md5, (unsigned char*)((char*)(CW2A(m_EditSvrPassword.GetString()))), strlen((char *)(CW2A(m_EditSvrPassword.GetString()))));
		MD5Final(&m_Md5, decrypt);

		get_mac(localmac);
		memcpy(m_ReqData.uid, CW2A(m_EditDevID.GetString()), m_EditDevID.GetLength());
		memcpy(m_ReqData.usr, CW2A(m_EditSvrUser.GetString()), m_EditSvrUser.GetLength());
		memcpy(m_ReqData.pwd, decrypt, 16);
		memcpy(m_ReqData.ver, "1", strlen("1"));
		memcpy(m_ReqData.mac, localmac, strlen(localmac));
		m_bShellSendReqFlag = TRUE;
	}

	m_sCurRestartDevID = m_EditDevID;
	m_nCurRestartDevIDCount = 1;
	m_TestFuncItemVec.push_back(TEST_RESTART_DEV);
	SetEvent(m_OperateControlThreadStartEvent);
	//m_Statusbar.SetPaneText(2, _T("设备重启"));

}


void Ckp2p_check_tool_win32Dlg::OnBnClickedRemoteConfigButton()
{
	// TODO: 在此添加控件通知处理程序代码

	//char buffer[10 * 1024];

	//strcpy(buffer, REMOTE_SETUP_STR);
	//    kp2p_remote_setup(handle, REMOTE_SETUP_STR, 10 * 1024);
	kp2p_remote_setup(m_Handle, "hello", strlen("hello"));

}


void Ckp2p_check_tool_win32Dlg::OnBnClickedExportDataButton()
{
	// TODO: 在此添加控件通知处理程序代码

	m_GenReportDlg.SetParent(this);
	INT_PTR nRes = m_GenReportDlg.DoModal();
	if (nRes == IDOK) {
		/*if (dlg.m_pwd == password)
		{
		m_result = "ÃÜÂëÕýÈ·£¡";
		UpdateData(FALSE);
		}
		else {
		m_result = "ÃÜÂë´íÎó£¡";
		UpdateData(FALSE);
		}*/
	}
}


void Ckp2p_check_tool_win32Dlg::OnBnClickedDelTestItemButton()
{
	m_DevConfigDlg.m_ListCtrlConfig.DeleteAllItems();
	m_DevStatusDlg.m_ListCtrlStatus.DeleteAllItems();

	// TODO: 在此添加控件通知处理程序代码
	CString item;
	int item_index = m_ListBoxTestItem.GetCurSel();
	if (item_index == -1) {
		MessageBox(_T("请选中要删除的测试项，再点击删除按钮"), _T("信息提示"), MB_OK);
		return;
	}
	vector<CString>::iterator it;
	m_ListBoxTestItem.GetText(item_index, item);
	if (item.Compare(TEST_MTU_ITEM) == 0)
		goto D_CONFIG_END;
	else if (item.Compare(TEST_GEATEWAY_ITEM) == 0)
		goto D_CONFIG_END;
	else if (item.Compare(TEST_DNS_ITEM) == 0)
		goto D_CONFIG_END;
	else if (item.Compare(TEST_SIGNAL_ITEM) == 0)
		goto D_CONFIG_END;
	else if (item.Compare(TEST_CPU_ITEM) == 0)
		goto D_CONFIG_END;
	else if (item.Compare(TEST_MEMORY_ITEM) == 0)
		goto D_CONFIG_END;
	else if (item.Compare(TEST_PROCESS_ITEM) == 0)
		goto D_CONFIG_END;
	else if (item.Compare(TEST_THREAD_ITEM) == 0)
		goto D_CONFIG_END;
	else if (item.Compare(TEST_PING_ITEM) == 0)
		goto D_STATUS_END;
	else if (item.Compare(TEST_NETWORK_ITEM) == 0)
		goto D_STATUS_END;
	else if (item.Compare(TEST_ROUTE_ITEM) == 0)
		goto D_STATUS_END;
	else if (item.Compare(TEST_BANDWIDTH) == 0)
		goto D_STATUS_END;
	else if (item.Compare(TEST_LOG_ITEM) == 0)
		goto D_STATUS_END;

D_CONFIG_END:

	m_ListBoxTestItem.DeleteString(item_index);
	it = m_TestItemVec.begin();
	for ( ; it != m_TestItemVec.end(); ) {
		if (lstrcmp(item.GetString(), (*it).GetString()) == 0) {
			m_TestItemVec.erase(it);
			m_nConfigItemCount--;
			it = m_TestItemVec.begin();
		}
		else {
			it++;
		}
	}

	if (m_TestItemVec.empty() && m_TestStatusItemVec.empty()) {
		m_bInitInfoPage0 = FALSE;
		m_bReadyLoadFlag = FALSE;
		if (m_bReadyStartFlag && m_bReadyLoadFlag) {
			m_BtnStartTest->EnableWindow(TRUE);
		}
		else {
			m_BtnStartTest->EnableWindow(FALSE);
		}
	}

	return;

D_STATUS_END:

	m_ListBoxTestItem.DeleteString(item_index);
	it = m_TestStatusItemVec.begin();
	for (; it != m_TestStatusItemVec.end(); ) {
		if (lstrcmp(item.GetString(), (*it).GetString()) == 0) {
			m_TestStatusItemVec.erase(it);
			m_nStatusItemCount--;
			it = m_TestStatusItemVec.begin();
		}
		else {
			it++;
		}
	}

	if (m_TestItemVec.empty() && m_TestStatusItemVec.empty()) {
		m_bInitInfoPage0 = FALSE;
		m_bReadyLoadFlag = FALSE;
		if (m_bReadyStartFlag && m_bReadyLoadFlag) {
			m_BtnStartTest->EnableWindow(TRUE);
		}
		else {
			m_BtnStartTest->EnableWindow(FALSE);
		}
	}

	return;

}


void Ckp2p_check_tool_win32Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CTime time;
	time = time.GetCurrentTime();
	CString stime;
	stime = time.Format("%y-%m-%d %H:%M:%S");
	m_Statusbar.SetPaneText(0, stime);

	CDialogEx::OnTimer(nIDEvent);
}



void Ckp2p_check_tool_win32Dlg::OnBnClickedStartCheckMainButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int i;
	m_nStatusItemCount = 0;
	m_nConfigItemCount = 0;

	::InterlockedIncrement(&m_OnDataFlagCountLock);
	if (::InterlockedDecrement(&m_OnDataFlagCountLock) != 0) {
		SHOW_STATUS_INFO_A("正在测试，请等待");
		return;
	}

	m_DevConfigDlg.m_ListCtrlConfig.DeleteAllItems();
	m_DevConfigDlg.m_ListCtrlConfig.RedrawWindow();
	m_DevStatusDlg.m_ListCtrlStatus.DeleteAllItems();
	m_DevStatusDlg.m_ListCtrlStatus.RedrawWindow();

	if (!m_TestStatusItemVec.empty()) {
		m_TestStatusItemVec.clear();
	}

	if (!m_TestItemVec.empty()) {
		m_TestItemVec.clear();
	}

	int nCnt = m_CheckListBoxTestItem.GetCount();
	for (i = 0; i < nCnt; ++i)
	{
		if (1 == m_CheckListBoxTestItem.GetCheck(i)) {
			break;
		}
	}
	if (i == nCnt) {
		MessageBox(_T("未勾选测试项"), _T("信息提示"), MB_OK);
		return;
	}
	
	//map<LONG64, pkp2p_dev_config_t>::iterator c_pos;
	//map<LONG64, pkp2p_dev_status_t>::iterator s_pos;

	nCnt = m_CheckListBoxTestItem.GetCount();
	for (i = 0; i < nCnt; ++i)
	{
		if (1 == m_CheckListBoxTestItem.GetCheck(i))
		{
			CString item;
			m_CheckListBoxTestItem.GetText(i, item);

			if (item.Compare(TEST_MTU_ITEM) == 0)
				goto CONFIG_END;
			else if (item.Compare(TEST_GEATEWAY_ITEM) == 0)
				goto CONFIG_END;
			else if (item.Compare(TEST_DNS_ITEM) == 0)
				goto CONFIG_END;
			else if (item.Compare(TEST_SIGNAL_ITEM) == 0)
				goto CONFIG_END;
			else if (item.Compare(TEST_CPU_ITEM) == 0)
				goto CONFIG_END;
			else if (item.Compare(TEST_MEMORY_ITEM) == 0)
				goto CONFIG_END;
			else if (item.Compare(TEST_PROCESS_ITEM) == 0)
				goto CONFIG_END;
			else if (item.Compare(TEST_THREAD_ITEM) == 0)
				goto CONFIG_END;
			else if (item.Compare(TEST_DEV_SYS_TIME_ITEM) == 0)
				goto CONFIG_END;
			/*else if (item.Compare(_T("test")) == 0)
				goto CONFIG_END;*/
			else if (item.Compare(TEST_PING_ITEM) == 0)
				goto STATUS_END;
			else if (item.Compare(TEST_NETWORK_ITEM) == 0)
				goto STATUS_END;
			else if (item.Compare(TEST_ROUTE_ITEM) == 0)
				goto STATUS_END;
			else if (item.Compare(TEST_BANDWIDTH) == 0)
				goto STATUS_END;
			else if (item.Compare(TEST_LOG_ITEM) == 0)
				goto STATUS_END;

		STATUS_END:

			if (item.Compare(TEST_PING_ITEM) == 0) {
				if (m_PingSettingDlg.m_PNum.IsEmpty()) {
					m_PingSettingDlg.m_PNum.SetString(_T("1"));
				}
				if (m_PingSettingDlg.m_PpkgSize.IsEmpty()) {
					m_PingSettingDlg.m_PpkgSize.SetString(_T("100"));
				}
				if (m_PingSettingDlg.m_PwebSite.IsEmpty()) {
					m_PingSettingDlg.m_PwebSite.SetString(_T("www.baidu.com"));
				}
			}

			m_TestStatusItemVec.push_back(item);

			m_nStatusItemCount++;

			/*c_pos = m_DevConfigInfoMap.find(atoll(CW2A(m_EditDevID.GetString())));
			if (c_pos != m_DevConfigInfoMap.end()) {

			}*/

			continue;

		CONFIG_END:

			m_TestItemVec.push_back(item);

			m_nConfigItemCount++;
	
		}
	}

	INT_PTR nRes = 0;
#if 0
	CEdit* pEditSvrUser = NULL;
	CEdit* pEditSvrPassd = NULL;

	if (m_EditSvrUser.IsEmpty() || m_EditSvrPassword.IsEmpty()) {
		nRes = MessageBox(_T("服务器用户名或密码不能为空"), _T("信息提示"), MB_OK);
		if (IDOK == nRes) {
			if (m_EditDevID.IsEmpty()) {
				pEditSvrUser = (CEdit *)GetDlgItem(IDC_USER_SVR_EDIT);
				pEditSvrUser->SetFocus();
			}
			else {
				pEditSvrPassd = (CEdit *)GetDlgItem(IDC_PASSD_SVR_EDIT);
				pEditSvrPassd->SetFocus();
			}
		}
		return;
	}
#endif

	m_ComboBoxSvrUserItem.GetWindowText(m_EditSvrUser);
	CEdit* pEditSvrPassd = NULL;
	if (m_EditSvrUser.IsEmpty() || m_EditSvrPassword.IsEmpty()) {
		nRes = MessageBox(_T("服务器用户名或密码不能为空"), _T("信息提示"), MB_OK);
		if (IDOK == nRes) {
			if (m_EditSvrUser.IsEmpty()) {
				m_ComboBoxSvrUserItem.SetFocus();
			}
			else {
				pEditSvrPassd = (CEdit *)GetDlgItem(IDC_PASSD_SVR_EDIT);
				pEditSvrPassd->SetFocus();
			}
		}
		return;
	}

	m_This = this;

	/*::InterlockedIncrement(&m_OnDataFlagCountLock);
	if (::InterlockedDecrement(&m_OnDataFlagCountLock) != 0) {
		SHOW_STATUS_INFO_A("正在测试，请等待");
		return;
	}*/

	SHOW_STATUS_INFO_A("开始测试");
	m_Statusbar.SetPaneText(2, _T("测试中"));

	//初始化信息列表显示
	init_info_list();

	//启动测试线程
	//::ResumeThread(m_ExecHandleThr);
	//启动检测
	Run_Check();
	//设备配置参数测试
	//start_dev_config_test();
	//设备状态参数测试
	//start_dev_status_test();

	m_Statusbar.SetPaneText(2, _T("ready"));

	//UpdateData(FALSE);	
}


void Ckp2p_check_tool_win32Dlg::OnBnClickedConfigSettingMainButton()
{
	// TODO: 在此添加控件通知处理程序代码

	INT_PTR nRes = m_PingSettingDlg.DoModal();
	if (nRes == IDOK) {

	}
}


void Ckp2p_check_tool_win32Dlg::OnBnClickedStopCheckMainButton()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;

	::InterlockedIncrement(&m_OnDataFlagCountLock);
	if (::InterlockedDecrement(&m_OnDataFlagCountLock) != 0) {
		//SHOW_STATUS_INFO_A("正在测试，请等待");
		nRes = MessageBox(_T("正在测试,是否要中止测试？"), _T("信息提示"), MB_YESNO);
		if (IDNO == nRes) {
			return;
		}
		SetEvent(m_TerminateCheckNotifyEvent);
	}
	else {
		SHOW_STATUS_INFO_A("无测试进行中");
	}

	//SetEvent(m_DevOfflineNotifyEvent);
}


void Ckp2p_check_tool_win32Dlg::OnBnClickedInfoStatusDevButton()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;
	unsigned char decrypt[16];
	char localmac[32];

	memset(localmac, 0x00, 32);
	memset(decrypt, 0x00, 16);

	UpdateData(TRUE);
	m_ComboBoxDevIDItem.GetWindowText(m_EditDevID);
	m_DevStatusInfoDetailDlg.SetMainParent(this);

	nRes = m_DevStatusInfoDetailDlg.DoModal();
	if (nRes == IDOK) {
#if 0
		if (!m_bShellSendReqFlag) {
			memset(&m_ReqData, 0x00, sizeof(auth_psd_req_data_t));
			MD5Init(&m_Md5);
			MD5Update(&m_Md5, (unsigned char*)((char*)(CW2A(m_EditSvrPassword.GetString()))), strlen((char *)(CW2A(m_EditSvrPassword.GetString()))));
			MD5Final(&m_Md5, decrypt);

			get_mac(localmac);
			memcpy(m_ReqData.uid, CW2A(m_EditDevID.GetString()), m_EditDevID.GetLength());
			memcpy(m_ReqData.usr, CW2A(m_EditSvrUser.GetString()), m_EditSvrUser.GetLength());
			memcpy(m_ReqData.pwd, decrypt, 16);
			memcpy(m_ReqData.ver, "1", strlen("1"));
			memcpy(m_ReqData.mac, localmac, strlen(localmac));
			m_bShellSendReqFlag = TRUE;
		}

		//SetEvent(m_ModThreadStartCmdEvent);
#endif
	}

}


void Ckp2p_check_tool_win32Dlg::OnBnClickedModTimeDevButton()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;
	unsigned char decrypt[16];
	char localmac[32];

	memset(localmac, 0x00, 32);
	memset(decrypt, 0x00, 16);

	nRes = m_DevSysTimeSettingDlg.DoModal();
	if (nRes == IDOK) {

		if (!m_bShellSendReqFlag) {
			memset(&m_ReqData, 0x00, sizeof(auth_psd_req_data_t));
			MD5Init(&m_Md5);
			MD5Update(&m_Md5, (unsigned char*)((char*)(CW2A(m_EditSvrPassword.GetString()))), strlen((char *)(CW2A(m_EditSvrPassword.GetString()))));
			MD5Final(&m_Md5, decrypt);

			get_mac(localmac);
			memcpy(m_ReqData.uid, CW2A(m_EditDevID.GetString()), m_EditDevID.GetLength());
			memcpy(m_ReqData.usr, CW2A(m_EditSvrUser.GetString()), m_EditSvrUser.GetLength());
			memcpy(m_ReqData.pwd, decrypt, 16);
			memcpy(m_ReqData.ver, "1", strlen("1"));
			memcpy(m_ReqData.mac, localmac, strlen(localmac));
			m_bShellSendReqFlag = TRUE;
		}

		//SetEvent(m_ModThreadStartCmdEvent);

	}
}


void Ckp2p_check_tool_win32Dlg::OnCbnEditchangeIdDevCombo()
{
	// TODO: 在此添加控件通知处理程序代码
#if 0
	INT_PTR nRes;
	CString idStr = TEXT("");
	m_ComboBoxDevIDItem.GetWindowTextW(idStr);
	if (idStr.IsEmpty()) {
		m_ComboBoxDevIDItem.SetFocus();
		m_BtnConnect->EnableWindow(FALSE);
		return;
	}

	for (int i = 0; i < idStr.GetLength() - 1; ++i) {
		wchar_t ch = idStr.GetAt(i);
		if (!isdigit(ch)) {
			nRes = MessageBox(_T("输入内容含有非数字信息，请输入数字"), _T("信息提示"), MB_OK);
			if (IDOK == nRes) {
				m_BtnConnect->EnableWindow(FALSE);
				return;
			}
		}
	}
	m_BtnConnect->EnableWindow(TRUE);
#endif
}


void Ckp2p_check_tool_win32Dlg::OnLbnSelchangeUntestItemList()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;
	int i;
	static BOOL testFlag = FALSE;
	int nCnt = m_CheckListBoxTestItem.GetCount();
	for (i = 0; i < nCnt; ++i)
	{
		if (1 == m_CheckListBoxTestItem.GetCheck(i)) {	
			CString item;
			m_CheckListBoxTestItem.GetText(i, item);
			if (item.Compare(_T("test")) == 0) {
				if (!testFlag) {
					nRes = m_TestCmdDlg.DoModal();
					if (nRes == IDOK) {
						if (m_TestCmdDlg.m_EditCmd.IsEmpty()) {
							m_CheckListBoxTestItem.SetCheck(i, 0);
						}
						testFlag = TRUE;
					}
					else {
						m_CheckListBoxTestItem.SetCheck(i, 0);
					}
				}
				break;
			}	
		}
	}

	if (i == nCnt) {
		testFlag = FALSE;
	}

}
