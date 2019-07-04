
// kp2p_check_tool_win32Dlg.cpp : 
//

#include "stdafx.h"
#include "kp2p_check_tool_win32.h"
#include "kp2p_check_tool_win32Dlg.h"
#include "afxdialogex.h"
#include "Kp2pEnv.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL Ckp2p_check_tool_win32Dlg::m_bShellLogin = FALSE;
BOOL Ckp2p_check_tool_win32Dlg::m_bInitInfoPage0 = FALSE;
BOOL Ckp2p_check_tool_win32Dlg::m_bInitInfoPage1 = FALSE;
int Ckp2p_check_tool_win32Dlg::m_Ecode = 0;
Ckp2p_check_tool_win32Dlg* Ckp2p_check_tool_win32Dlg::m_This = NULL;
CRITICAL_SECTION Ckp2p_check_tool_win32Dlg::m_OnDataFlagLock = { 0 };
HANDLE Ckp2p_check_tool_win32Dlg::m_OnDataFlagEvent = NULL;
volatile LONG Ckp2p_check_tool_win32Dlg::m_OnDataFlagCountLock = 0;
volatile LONG Ckp2p_check_tool_win32Dlg::m_FstCurDevConfigInfoCountLock = 0;
INT Ckp2p_check_tool_win32Dlg::m_nConfigItemCount = 0;
INT Ckp2p_check_tool_win32Dlg::m_nStatusItemCount = 0;
//HANDLE Ckp2p_check_tool_win32Dlg::m_EndRunCheck = NULL;

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
	DDX_Control(pDX, IDC_UNTEST_ITEM_LIST, m_ListBoxTestItem);
	DDX_Control(pDX, IDC_STATUS_LIST, m_ListBoxStatusShow);

	DDX_Control(pDX, IDC_TAB1, m_TabCtrlInfo);

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
	ON_BN_CLICKED(IDC_RESTART_IOTDAEMON_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedRestartIotdaemonButton)
	ON_BN_CLICKED(IDC_RESTART_APP_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedRestartAppButton)
	ON_BN_CLICKED(IDC_RESTART_DEV_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedRestartDevButton)
	ON_BN_CLICKED(IDC_REMOTE_CONFIG_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedRemoteConfigButton)
	ON_BN_CLICKED(IDC_EXPORT_DATA_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedExportDataButton)
	ON_BN_CLICKED(IDC_DEL_TEST_ITEM_BUTTON, &Ckp2p_check_tool_win32Dlg::OnBnClickedDelTestItemButton)
	ON_WM_TIMER()
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

	CRect tabRect;
	m_TabCtrlInfo.InsertItem(0, _T("设备配置信息"));
	m_TabCtrlInfo.InsertItem(1, _T("设备状态信息"));
	m_DevConfigDlg.Create(IDD_DEV_CONFIG_DIALOG, &m_TabCtrlInfo);
	m_DevStatusDlg.Create(IDD_DEV_STATUS_DIALOG, &m_TabCtrlInfo);
	m_TabCtrlInfo.GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;
	m_DevConfigDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	m_DevStatusDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	m_TabCtrlInfo.SetCurSel(0);

	CTime time;
	time = time.GetCurrentTime();
	CString stime;
	stime = time.Format("%y-%m-%d %H:%M:%S");
	
	UINT array[3] = { 12301,12302, 12303 };
	m_Statusbar.Create(this);
	m_Statusbar.SetIndicators(array, sizeof(array) / sizeof(UINT));

	CRect rect;
	GetWindowRect(rect);
	m_Statusbar.SetPaneInfo(0, array[0], 0, rect.Width() / 3);
	m_Statusbar.SetPaneInfo(1, array[1], 0, 8 * 8);
	m_Statusbar.SetPaneInfo(2, array[2], 0, rect.Width() / 3);
	m_Statusbar.SetPaneText(0, stime);
	m_Statusbar.SetPaneText(1, _T("当前状态："));
	m_Statusbar.SetPaneText(2, _T(" "));
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	SetTimer(1, 1000, NULL);

	m_comboBoxTestItem.AddString(_T("MTU"));
	m_comboBoxTestItem.AddString(_T("网关"));
	m_comboBoxTestItem.AddString(_T("DNS"));
	m_comboBoxTestItem.AddString(_T("信号强度"));
	m_comboBoxTestItem.AddString(_T("CPU占用"));
	m_comboBoxTestItem.AddString(_T("内存占用"));
	m_comboBoxTestItem.AddString(_T("进程"));
	m_comboBoxTestItem.AddString(_T("线程"));

	m_comboBoxTestItem.AddString(_T("PING检测"));
	m_comboBoxTestItem.AddString(_T("网络连接状况检测"));
	m_comboBoxTestItem.AddString(_T("带宽检测"));
	m_comboBoxTestItem.AddString(_T("路由表检测"));
	m_comboBoxTestItem.AddString(_T("日志数据获取检测"));
	//m_comboBoxTestItem.InsertString(1, _T("DNS"));
  
	m_comboBoxTestItem.SetCurSel(0);
	m_ListBoxTestItem.SetCurSel(0);

	m_BtnStartTest = NULL;
	m_BtnStartTest = (CButton *)GetDlgItem(IDC_START_TEST_BUTTON);
	m_BtnStartTest->EnableWindow(FALSE);

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

	m_BtnRestartIOTRdaemon = NULL;
	m_BtnRestartIOTRdaemon = (CButton *)GetDlgItem(IDC_RESTART_IOTDAEMON_BUTTON);
	m_BtnRestartIOTRdaemon->EnableWindow(FALSE);

	m_BtnRestartAPP = NULL;
	m_BtnRestartAPP = (CButton *)GetDlgItem(IDC_RESTART_APP_BUTTON);
	m_BtnRestartAPP->EnableWindow(FALSE);

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

	m_EditDevID.SetString(L"1606129063"/*L"924957972"*/);
	m_EditDevUser.SetString(L"admin");
	m_EditSvrUser.SetString(L"test0");
	m_EditSvrPassword.SetString(L"123456");
	UpdateData(FALSE);

	return TRUE; 
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
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	if (m_bDevConnectStatusFlag) {
		if (m_bShellLogin) {
			if (m_Session) {
				IOT_SHELL_Logout(&m_Session);
			}
			/*if (m_Shell) {
				IOT_SHELL_Deinit(&m_Shell);
			}*/
		}
		kp2p_exit();
	}

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
		m_DevConfigDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_DevStatusDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
		 
	case 1:
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
	//退出测试工作线程
	SetEvent(m_ExcuteCmdThreadExitEvent);
	SetEvent(m_ModifyDevConfigThreadExitEvent);

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

	if (m_bShellLogin) {
		if (m_Session) {
			IOT_SHELL_Logout(&m_Session);
			SHOW_STATUS_INFO_A("IOT_SHELL_Logout退出登录");
		}
		if (m_Shell) {
			//IOT_SHELL_Deinit(&m_Shell)/*;*/
		}
		
	}
	
	//kp2p_close(m_Handle);
	kp2p_exit();
	SHOW_STATUS_INFO_A("断开连接，结束测试");
	m_bDevConnectStatusFlag = FALSE;

	m_BtnDisconnect->EnableWindow(FALSE);
	m_BtnDevCfgModify->EnableWindow(FALSE);
	m_BtnVideo->EnableWindow(FALSE);
	m_BtnLive->EnableWindow(FALSE);
	m_BtnRestartIOTRdaemon->EnableWindow(FALSE);
	m_BtnRestartAPP->EnableWindow(FALSE);
	m_BtnRestartDev->EnableWindow(FALSE);
	m_BtnRemoteSetting->EnableWindow(FALSE);
	//m_BtnExportData->EnableWindow(FALSE);

	m_bReadyStartFlag = FALSE;
	m_BtnStartTest->EnableWindow(FALSE);

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
			if (::InterlockedDecrement(&m_FstCurDevConfigInfoCountLock) == 0) {
				//TODO:
			}
			else {
				temp = m_This->m_sdns;
				SHOW_STATUS_INFO_S_W(temp.GetString());
				m_This->m_bOnDataFlag = TRUE;
			}
			SetEvent(m_This->m_ThreadNotifyCmdEvent);
			break;
		case ERROR_AUTH_USR:
			SHOW_STATUS_INFO_S_A("服务器鉴权失败，用户名不存在");
			break;
		case ERROR_AUTH_PWD:
			SHOW_STATUS_INFO_S_A("服务器鉴权失败，密码不正确");
			break;
		case ERROR_AUTH_MAC:
			SHOW_STATUS_INFO_S_A("服务器鉴权失败，MAC地址错误");
			break;
		case ERROR_AUTH_VERSION:
			SHOW_STATUS_INFO_S_A("服务器鉴权失败，版本不正确");
			break;
		case ERROR_AUTH_ADD:
			SHOW_STATUS_INFO_S_A("服务器鉴权失败，ERROR_AUTH_ADD");
			break;
		case ERROR_AUTH_UNKNOW:
			SHOW_STATUS_INFO_S_A("服务器鉴权失败，ERROR_AUTH_UNKNOW");
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
	info.Format(L"[OnConnect]Handle:%p, type:%s\n", p2p_handle, conn_type);
	//SHOW_STATUS_INFO_S_W(info.GetString());
}


void Ckp2p_check_tool_win32Dlg::OnDisconnect(kp2p_handle_t p2p_handle, void *context, int ret)
{
	//   printf("[OnDisconnect]Handle:%p disconnected,reason:%d\n", p2p_handle, ret);
	CString info;
	info.Format(L"[OnDisconnect]Handle:%p disconnected,reason:%d\n", p2p_handle, ret);
	//SHOW_STATUS_INFO_S_W(info.GetString());
}


void Ckp2p_check_tool_win32Dlg::OnOpenStream(kp2p_handle_t p2p_handle, void *context, uint32_t channel, uint32_t stream, int err, const char *cam_des)
{
	//  printf("OnOpenStream: %s\n", cam_des);
}


void Ckp2p_check_tool_win32Dlg::OnCloseStream(kp2p_handle_t p2p_handle, void *context, uint32_t channel, uint32_t stream, int err)
{
	//    printf("OnCloseStream: %d %d\n", channel, stream);
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
	//unsigned char *p = (unsigned char *)frame;
	//stream_bytes += frame_size;
	//if (stream_times == 0) { stream_times = iclock(); }
	//if (frame_type == 1) { stream_frame_times_end = iclock(); }

	//if (KP2P_FRAME_TYPE_AUDIO == frame_type) {
	//	//        printf("OnRecvFreamEx: channel(%u) enc(%s) size(%lu) ts("fllu") samplerate(%u) samplewidth(%u) channles(%u)\n",
	//	//               channel, enc, frame_size, time_stamp, sample_rate, sample_width, sample_channels);

	//}
	//else if (KP2P_FRAME_TYPE_IFRAME == frame_type || KP2P_FRAME_TYPE_PFRAME == frame_type) {
	//	//     printf("OnRecvFrameEx: channel(%u) enc(%s) size(%lu) type(%u) ts("fllu") width(%u) height(%u) %02x%02x%02x%02x %02x%02x%02x%02x\n",
	//	//           channel, enc, frame_size, frame_type, time_stamp, width, height, p[0], p[1], p[2],
	//	//           p[3], p[4], p[5], p[6], p[7]);
	//	//        OnDumpVideo(channel, enc, frame, frame_size);
	//}
	//else if (KP2P_FRAME_TYPE_OOB == frame_type) {
	//	//    printf("recvFrame OOB: size:%lu\n", frame_size);
	//}
	//if (record_f != NULL)
	//{
	//	fwrite(frame, 1, frame_size, record_f);
	//}
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
	//if (frame_type == 1) { rreplay_frame_times_end = iclock(); }
	//if (record_f == NULL)
	//{
	//	replay_stream_times_end = replay_stream_times_end == 0 ? iclock() : replay_stream_times_end;


	//	if (KP2P_FRAME_TYPE_AUDIO == frame_type) {
	//		//    printf("OnRecvRecFrame: enc(%s) size(%lu) ts("fllu") samplerate(%u) samplewidth(%u) channles(%u) genTime(%u)\n",
	//		//        enc, frame_size, time_stamp, sample_rate, sample_width, sample_channels, gen_time);

	//	}
	//	else if (KP2P_FRAME_TYPE_IFRAME == frame_type || KP2P_FRAME_TYPE_PFRAME == frame_type) {
	//		//      printf("OnRecvRecFrame: enc(%s) size(%lu) type(%u) ts("fllu") width(%u) height(%u) genTime(%u)\n",
	//		//          enc, frame_size, frame_type, time_stamp, width, height, gen_time);
	//	}
	//}
	//else
	//{
	//	if (KP2P_FRAME_TYPE_IFRAME == frame_type || KP2P_FRAME_TYPE_PFRAME == frame_type) {
	//		//               printf("OnRecvRecFrame: enc(%s) size(%lu) type(%u) ts("fllu") width(%u) height(%u) genTime(%u)\n",
	//		//                  enc, frame_size, frame_type, time_stamp, width, height, gen_time);
	//	}
	//	fwrite(frame, 1, frame_size, record_f);
	//}
}


void Ckp2p_check_tool_win32Dlg::OnLoginResult(kp2p_handle_t p2p_handle, void *context, int ret)
{
	    printf("OnLoginResult Handle:%p ret:%d\n", p2p_handle, ret);
	//login_times = iclock();
}


void Ckp2p_check_tool_win32Dlg::OnPtzResult(kp2p_handle_t p2p_handle, void *context, int ret)
{
	    printf("OnPtzResult Handle:%p ret:%d\n", p2p_handle, ret);
}


void Ckp2p_check_tool_win32Dlg::OnVconRecv(kp2p_handle_t p2p_handle, void *context, uint32_t channel, const void *buf, size_t len)
{
	//    printf("OnVconRecv(p2p_handle=%p, channel=%u, buf=%s, data_size=%lu\n", p2p_handle, channel,
	//           (const char *) buf, len);
}


void Ckp2p_check_tool_win32Dlg::OnRemoteSetup(kp2p_handle_t p2p_handle, void *context, const void *data, size_t data_size)
{
	    printf("%s %d | %s\n", __FUNCTION__, __LINE__, (const char *) data);
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
	SHOW_CURRENT_STATUS_INFO_A("连接中");
	SHOW_STATUS_INFO_W(CA2W(kp2p_get_version()));

	kp2p_set_log(KP2P_LOG_ALL, "./", 1000 * 1000);

	if (!p2p_context_init()) {

		SHOW_STATUS_INFO_A("连接初始化失败");
		return;
	}
	SHOW_CURRENT_STATUS_INFO_A("设备连接成功");
	SHOW_STATUS_INFO_A("设备连接初始化成功");
	m_bDevConnectStatusFlag = TRUE;

	//保存信息结构初始化
	config_info_init();
	
	m_BtnDisconnect->EnableWindow(TRUE);
	m_BtnDevCfgModify->EnableWindow(TRUE);
	m_BtnVideo->EnableWindow(TRUE);
	m_BtnLive->EnableWindow(TRUE);
	m_BtnRestartIOTRdaemon->EnableWindow(TRUE);
	m_BtnRestartAPP->EnableWindow(TRUE);
	m_BtnRestartDev->EnableWindow(TRUE);
	m_BtnRemoteSetting->EnableWindow(TRUE);
	m_BtnExportData->EnableWindow(TRUE);

	m_bReadyStartFlag = TRUE;
	if (m_bReadyStartFlag && m_bReadyLoadFlag) {
		m_BtnStartTest->EnableWindow(TRUE);
	}
	else {
		m_BtnStartTest->EnableWindow(FALSE);
	}

	m_This = this;

	try {
		start_work_thread();
	}
	catch (exception &e) {
		SHOW_STATUS_INFO_A("CreateThread failed");
	}

	return;
}

void Ckp2p_check_tool_win32Dlg::start_work_thread()
{
	m_ExecHandleThr = (HANDLE)_beginthreadex(NULL, 0, Ckp2p_check_tool_win32Dlg::ExcuteCmdThreadProc, this, CREATE_SUSPENDED, NULL);
	if (m_ExecHandleThr == 0) {
		//printlog("CreateThread failed. LastError: %u\n", GetLastError());
		//free(t);
		throw exception();
	}
	::ResumeThread(m_ExecHandleThr);

	m_ModHandleThr = (HANDLE)_beginthreadex(NULL, 0, Ckp2p_check_tool_win32Dlg::ModifyDevConfigThreadProc, this, CREATE_SUSPENDED, NULL);
	if (m_ModHandleThr == 0) {
		//printlog("CreateThread failed. LastError: %u\n", GetLastError());
		throw exception();
	}
	::ResumeThread(m_ModHandleThr);
}

BOOL Ckp2p_check_tool_win32Dlg::p2p_context_init()
{
	kp2p_callback_t cb = { 0 };

	int            result = 0;

	char            user[32] = "admin";
	char            passwd[128] = "";
	int             channel = 0;

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
		kp2p_exit();
		return FALSE;
	}
	SHOW_STATUS_INFO_A("kp2p_init初始化成功");

	BOOL bRet = check_p2p_ex(CW2A(m_EditDevID.GetString()), "", user, passwd, 0, channel);
	if (!bRet) {
		return FALSE;
	}

	time_t shell_end_times;
	time_t shell_start_times;
	CString info;

SHELL_LOGIN:

	m_Shell = IOT_SHELL_Init();
	if (!m_Shell) {
		SHOW_STATUS_INFO_A("IOT_SHELL_Init初始化失败");
		return FALSE;
	}
	SHOW_STATUS_INFO_A("IOT_SHELL_Init初始化成功");
	//	IOT_SHELL_SetTurnServer("192.168.23.180:8000");
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
		SHOW_STATUS_INFO_A("正在重新登录");
		msleep_c(2000);
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
}

void Ckp2p_check_tool_win32Dlg::config_info_init()
{
	kp2p_dev_config_t *pcfg = (kp2p_dev_config_t *)malloc(sizeof(kp2p_dev_config_t));
	memset(pcfg->id, 0, 20);
	memset(pcfg->user, 0, 20);
	memset(pcfg->password, 0, 20);
	memset(pcfg->mtu, 0, 10);
	memset(pcfg->gateway, 0, 20);
	memset(pcfg->signal, 0, 50);
	pcfg->mem_use = { 0 };
	pcfg->cpu_use = { 0 };
	pcfg->process = { 0 };
	pcfg->thread = { 0 };
	memcpy(pcfg->id, CW2A(m_EditDevID.GetString()), m_EditDevID.GetLength());
	memcpy(pcfg->user, CW2A(m_EditDevUser.GetString()), m_EditDevUser.GetLength());
	memcpy(pcfg->password, CW2A(m_EditDevPassword.GetString()), m_EditDevPassword.GetLength());

}

void Ckp2p_check_tool_win32Dlg::status_info_init()
{

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
	memset(&m_ReqData, 0x00, sizeof(auth_psd_req_data_t));

	if (!m_TestItemVec.empty() || !m_TestStatusItemVec.empty()) {
		if (!m_bShellSendReqFlag) {
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
		
		/*while (1) {
			::InterlockedIncrement(&m_OnDataFlagCountLock);
			if (::InterlockedDecrement(&m_OnDataFlagCountLock) != 0) {
				SHOW_STATUS_INFO_A("正在测试，请等待");
				return;
			}
			SHOW_STATUS_INFO_A("开始测试");
			m_Statusbar.SetPaneText(2, _T("测试中"));
		}*/
	}

	//msleep_c(3000);
	//WaitForSingleObject(m_EndRunCheck, INFINITE);
	//m_Statusbar.SetPaneText(2, _T("测试结束"));

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
	//kp2p_handle_t              handle;
	int                        ret = -1;
	int                        login_result = -1;
	/*kp2p_rec_search_handle_t   search_h;
	kp2p_rec_files_t           *pRecords = NULL;
	kp2p_rec_playback_handle_t play_h;
	kp2p_rec_file_t            play_file_r;*/
	//  check_context_t  mchecktext;

	//uint32_t sessionid = 0;
	//time_t    rawtime;
	struct tm *ptminfo = NULL;
	int conn_time = 0;
	int login_time = 0;
	int stream_time = 0;
	int total_time = 0;
	int setup_time = 0;
	char tt[30] = { 0 };
	int replay_search_time = 0;
	int replay_search_count = 0;
	int replay_stream_time = 0;
	int replay_frame_times = 0;
	int stream_frame_times = 0;

	//char * putout;
	//TCHAR * putout2;
	CString info;

	m_Handle = NULL;
	m_Handle = kp2p_create(NULL);
	if (!m_Handle) {
		SHOW_STATUS_INFO_A("kp2p_create创建句柄失败");
		m_Handle = NULL;
		return FALSE;
	}
	SHOW_STATUS_INFO_A("kp2p_create创建句柄成功");

	uint32_t start_times = GetTickCount();
	uint32_t end_times;
	login_result = kp2p_connect_v2(m_Handle, id, ip, 10000);
	if (login_result != 0) {
		info.Empty();
		info.Format(L"kp2p_connect连接失败 返回值:%d", login_result);
		SHOW_STATUS_INFO_W(info.GetString());
	
		end_times = GetTickCount();
		info.Empty();
		info.Format(L"kp2p_connect连接耗时: %d 毫秒", end_times - start_times);
		SHOW_STATUS_INFO_W(info.GetString());
		goto FAIL_END;
	}
	end_times = GetTickCount();
	info.Empty();
	info.Format(L"kp2p_login连接耗时: %d 毫秒", end_times - start_times);
	SHOW_STATUS_INFO_W(info.GetString());
	SHOW_STATUS_INFO_A("kp2p_connect连接成功");

	//kp2p_getsessionid(handle, &sessionid);
	//SHOW_STATUS_INFO(int_to_string(sessionid, L"会话ID："));

	uint32_t login_start_times = GetTickCount();
	uint32_t login_end_times;
	ret = kp2p_login(m_Handle, user, passwd);
	if (0 != ret) {
		info.Empty();
		info.Format(L"kp2p_login登录失败 返回值：%d", ret);
		SHOW_STATUS_INFO_W(info.GetString());

		login_end_times = GetTickCount();
		info.Empty();
		info.Format(L"kp2p_login连接耗时: %d 毫秒", login_end_times - login_start_times);
		SHOW_STATUS_INFO_W(info.GetString());

		goto FAIL_END;
	}
	login_end_times = GetTickCount();
	info.Empty();
	info.Format(L"kp2p_login连接耗时: %d 毫秒", login_end_times - login_start_times);
	SHOW_STATUS_INFO_W(info.GetString());
	SHOW_STATUS_INFO_A("kp2p_login登录成功");

	return TRUE;

FAIL_END:

	if (m_Handle)
		kp2p_close(m_Handle);

	return FALSE;
}


void Ckp2p_check_tool_win32Dlg::OnBnClickedCannelTestButton()
{
	// TODO: 在此添加控件通知处理程序代码
}

int Ckp2p_check_tool_win32Dlg::get_mac(char* mac)
{
	char mac_addr[30];
#ifndef _WIN32
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
	IP_ADAPTER_INFO AdapterInfo[16];
	DWORD dwBuflen = sizeof(AdapterInfo);

	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBuflen);

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
	if (NULL == pAdapterInfo)
		return -1;
	BYTE* MACData = pAdapterInfo->Address;
	sprintf(mac_addr, "%02x:%02x:%02x:%02x:%02x:%02x",
		MACData[0], MACData[1], MACData[2], MACData[3], MACData[4], MACData[5]);
	memcpy(mac, mac_addr, strlen(mac_addr));
#endif
	return 0;
}

unsigned int __stdcall Ckp2p_check_tool_win32Dlg::ModifyDevConfigThreadProc(PVOID arg)
{
	BOOL bExit = FALSE;
	DWORD nRet;
	CString localcmd/*, temp*/;
	char localcmd2[256];
	memset(localcmd2, 0x00, 256);
	int ncmds = 0;
	
	Ckp2p_check_tool_win32Dlg *p = (Ckp2p_check_tool_win32Dlg*)arg;

	unsigned char decrypt[16];
	char localmac[32];
	memset(localmac, 0x00, 32);
	memset(decrypt, 0x00, 16);
	if (!p->m_bShellSendReqFlag) {
		memset(&p->m_ReqData, 0x00, sizeof(auth_psd_req_data_t));
		MD5Init(&p->m_Md5);
		MD5Update(&p->m_Md5, (unsigned char*)((char*)(CW2A(p->m_EditSvrPassword.GetString()))), strlen((char *)(CW2A(p->m_EditSvrPassword.GetString()))));
		MD5Final(&p->m_Md5, decrypt);

		get_mac(localmac);
		memcpy(p->m_ReqData.uid, CW2A(p->m_EditDevID.GetString()), p->m_EditDevID.GetLength());
		memcpy(p->m_ReqData.usr, CW2A(p->m_EditSvrUser.GetString()), p->m_EditSvrUser.GetLength());
		memcpy(p->m_ReqData.pwd, decrypt, 16);
		memcpy(p->m_ReqData.ver, "1", strlen("1"));
		memcpy(p->m_ReqData.mac, localmac, strlen(localmac));
		p->m_bShellSendReqFlag = TRUE;
	}

	while (p->m_DevSettingDlg.m_nItemCount > 0) {
		if (p->m_DevSettingDlg.m_bGateWay) {
			localcmd.SetString(L"route | grep default | awk '{print $2}'");
			p->m_DevSettingDlg.m_bGateWay = FALSE;
			ncmds = 1;
			goto CUR_MSCONFIG_END;
		}
		if (p->m_DevSettingDlg.m_bDNS) {
			localcmd.SetString(L"cat /etc/resolv.conf  | awk '{print $2}'");
			p->m_DevSettingDlg.m_bDNS = FALSE;
			ncmds = 2;
			goto CUR_MSCONFIG_END;
		}
		if (p->m_DevSettingDlg.m_bMTU) {
			localcmd.SetString(L"cat /sys/class/net/eth0/mtu");
			p->m_DevSettingDlg.m_bMTU = FALSE;
			ncmds = 3;
			goto CUR_MSCONFIG_END;
		}

	CUR_MSCONFIG_END:

		int send_size = IOT_SHELL_Send(p->m_Session, CW2A(localcmd.GetString()), localcmd.GetLength() + 1, &p->m_ReqData);
		if ((nRet = WaitForSingleObject(p->m_ThreadNotifyCmdEvent, 30000)) != WAIT_OBJECT_0) {
			Sleep(50);
			switch (nRet)
			{
			case WAIT_TIMEOUT:
				//MessageBox(_T("获取数据时间超过30秒"), _T("信息提示"), MB_OK);
				break;
			case WAIT_FAILED:
				//MessageBox(_T("WaitForSingleObject error"), _T("信息提示"), MB_OK);
				break;
			default:
				break;
			}
		}
		//temp = p->m_sdns;
		switch (ncmds) {
		case 1:
			p->m_LableCurGateWay = p->m_sdns;
			//p->m_DevSettingDlg.m_LableCurGateWay.SetWindowText(temp.GetString());
			break;
		case 2:
			p->m_LableCurDNS = p->m_sdns;
			//p->m_DevSettingDlg.m_LableCurDNS.SetWindowText(temp.GetString());
			break;
		case 3:
			p->m_LableCurMTU = p->m_sdns;
			//p->m_DevSettingDlg.m_LableCurMTU.SetWindowText(temp.GetString());
			break;
		default:
			break;
		}

		p->m_DevSettingDlg.m_nItemCount--;
		localcmd.Empty();
		//temp.Empty();

	}

	::InterlockedIncrement(&m_FstCurDevConfigInfoCountLock);

	while (!bExit)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(p->m_ModifyDevConfigThreadExitEvent, 0))
			bExit = TRUE;


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
			if ((nRet = WaitForSingleObject(p->m_ThreadNotifyCmdEvent, 30000)) != WAIT_OBJECT_0) {
				msleep_c(50);
				switch (nRet)
				{
				case WAIT_TIMEOUT:
					SHOW_STATUS_INFO_S_A("获取数据时间超过30秒");
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
						sprintf(localcmd2, "sed 's/%s/%s/g' /etc/resolv.conf", p->m_sdns, (char*)(CW2A(p->m_DevSettingDlg.m_EditGateWay.GetString())));
						
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
						sprintf(localcmd2, "route add default gw %s", (char*)(CW2A(p->m_DevSettingDlg.m_EditDNS.GetString())));
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
						sprintf(localcmd2, "sed 's/%s/%s/g' /sys/class/net/eth0/mtu", p->m_sdns, (char*)(CW2A(p->m_DevSettingDlg.m_EditMTU.GetString())));
						//	printf("dddddd--%s\n",localcmd);
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
				if ((nRet = WaitForSingleObject(p->m_ThreadNotifyCmdEvent, 30000)) != WAIT_OBJECT_0) {
					msleep_c(50);
					switch (nRet)
					{
					case WAIT_TIMEOUT:
						SHOW_STATUS_INFO_S_A("获取数据时间超过30秒");
						break;
					case WAIT_FAILED:
						SHOW_STATUS_INFO_S_A("WaitForSingleObject error");
						break;
					default:
						break;
					}
					continue;
				}

				switch (ncmds) {
				case 1:
					p->m_LableCurGateWay = p->m_sdns;
					break;
				case 2:
					p->m_LableCurDNS = p->m_sdns;
					break;
				case 3:
					p->m_LableCurMTU = p->m_sdns;
					break;
				}

			}
			p->m_DevSettingDlg.m_nItemCount--;
			
		}
	}


	return 0;
}

unsigned int __stdcall Ckp2p_check_tool_win32Dlg::ExcuteCmdThreadProc(PVOID arg)
{
	BOOL bExit = FALSE;
	DWORD nRet;
	//time_t send_cmd_start;
	Ckp2p_check_tool_win32Dlg *p = (Ckp2p_check_tool_win32Dlg*)arg;
	char localcmd[256];
	memset(localcmd, 0x00, 256);
	INT recv_count = -1;

	while (!bExit)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(p->m_ExcuteCmdThreadExitEvent, 0))
			bExit = TRUE;

		if ((nRet = WaitForSingleObject(p->m_ExecThreadStartCmdEvent, 0)) != WAIT_OBJECT_0) {
			/*if (bExit)
				SetEvent(p->m_ReadyExitEvent);*/
			msleep_c(50);
			continue;
		}
		::InterlockedIncrement(&m_OnDataFlagCountLock);

		//p->m_DevStatusDlg.m_ListCtrlStatus.DeleteAllItems();
		int i = 0;
		vector<CString>::iterator it = p->m_TestItemVec.begin();
		for (; it != p->m_TestItemVec.end(); it++) {
			if ((*it).Compare(TEST_MTU_ITEM) == 0) {
				strcpy(localcmd, "cat /sys/class/net/eth0/mtu");
				SHOW_STATUS_INFO_S_A("MTU获取，执行命令：cat /sys/class/net/eth0/mtu");
			}
			else if ((*it).Compare(TEST_GEATEWAY_ITEM) == 0) {
				strcpy(localcmd, "route | grep default | awk '{print $2}'");
				SHOW_STATUS_INFO_S_A("网关配置获取，执行命令：route | grep default | awk '{print $2}'");
			}
			else if ((*it).Compare(TEST_DNS_ITEM) == 0) {
				strcpy(localcmd, "cat /etc/resolv.conf  | awk '{print $2}'");
				SHOW_STATUS_INFO_S_A("DNS配置获取，执行命令：cat /etc/resolv.conf  | awk '{print $2}'");
			}
			else if ((*it).Compare(TEST_SIGNAL_ITEM) == 0) {
				strcpy(localcmd, "cat /proc/net/rtl8188fu/wlan0/rx_signal");
				SHOW_STATUS_INFO_S_A("信号强度获取，执行命令：cat /proc/net/rtl8188fu/wlan0/rx_signal");
			}
			else if ((*it).Compare(TEST_CPU_ITEM) == 0) {
				strcpy(localcmd, "top -b -n 1 | grep 'top -' -A 3");
				SHOW_STATUS_INFO_S_A("CPU占用获取，执行命令：top -b -n 1 | grep 'top -' -A 3");
			}
			else if ((*it).Compare(TEST_MEMORY_ITEM) == 0) {
				strcpy(localcmd, "top -b -n 1 | grep 'top -' -A 5 | grep 'KiB Mem'");
				SHOW_STATUS_INFO_S_A("内存占用获取，执行命令：top -b -n 1 | grep 'top -' -A 5 | grep 'KiB Mem'");
			}
			else if ((*it).Compare(TEST_PROCESS_ITEM) == 0) {
				strcpy(localcmd, "ps");
				SHOW_STATUS_INFO_S_A("进程信息获取，执行命令：ps");
			}
			else if ((*it).Compare(TEST_THREAD_ITEM) == 0) {
				strcpy(localcmd, "ps -T");
				SHOW_STATUS_INFO_S_A("线程信息获取，执行命令：ps -T");
			}
			
			m_Ecode = 0;

			int send_size = IOT_SHELL_Send(p->m_Session, localcmd, strlen(localcmd) + 1, &p->m_ReqData);

		//MULTI_CONFIG_RECV:

			nRet = -1;
			if ((nRet = WaitForSingleObject(p->m_ThreadNotifyCmdEvent, 30000)) == WAIT_OBJECT_0) {
				CString temp;
				temp = p->m_sdns;
				p->m_DevConfigDlg.m_ListCtrlConfig.SetItemText(i, 1, temp.GetString());
				memset(p->m_sdns, 0, 1024 * 4);
				//UpdateData(FALSE);
			}
			else {
				//msleep_c(50);
				switch (nRet)
				{
				case WAIT_TIMEOUT:
					SHOW_STATUS_INFO_S_A("获取数据时间超过30秒");
					break;
				case WAIT_FAILED:
					SHOW_STATUS_INFO_S_A("WaitForSingleObject error");
					break;
				default:
					break;
				}
				//continue;
			}
			/*while () {
				goto MULTI_CONFIG_RECV;
			}*/

			memset(localcmd, 0x00, 256);
			i++;
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
				SHOW_STATUS_INFO_S_A("PING，执行命令：");
				SHOW_STATUS_INFO_S_W(pcmd.GetString());
				recv_count = 1;
				
			}
			else if ((*it).Compare(TEST_NETWORK_ITEM) == 0) {
				strcpy(localcmd, "netstat | grep ip | awk '{print $6}'");
				SHOW_STATUS_INFO_S_A("网络连接信息获取，执行命令：netstat | grep ip | awk '{print $6}'");
			}
			else if ((*it).Compare(TEST_ROUTE_ITEM) == 0) {
				strcpy(localcmd, "route");
				SHOW_STATUS_INFO_S_A("路由表信息获取，执行命令：route");
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
			if ((nRet = WaitForSingleObject(p->m_ThreadNotifyCmdEvent, 30000)) == WAIT_OBJECT_0) {
				CString temp;
				temp = p->m_sdns;
				p->m_DevStatusDlg.m_ListCtrlStatus.SetItemText(i, 1, temp.GetString());
				memset(p->m_sdns, 0, 1024 * 4);
				//p->UpdateData(FALSE);
			}
			else {
				//msleep_c(50);
				switch (nRet)
				{
				case WAIT_TIMEOUT:
					SHOW_STATUS_INFO_S_A("获取数据时间超过30秒");
					break;
				case WAIT_FAILED:
					SHOW_STATUS_INFO_S_A("WaitForSingleObject error");
					break;
				default:
					break;
				}
				//continue;
			}
			while (recv_count > 0) {
				recv_count--;
				goto MULTI_STATUS_RECV;
			}

			memset(localcmd, 0x00, 256);
			i++;
		}
		::InterlockedDecrement(&m_OnDataFlagCountLock);
		//SetEvent(m_EndRunCheck);
		SHOW_STATUS_INFO_S_A("测试结束");
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
	//send_cmd_start = 0;
	//unsigned char decrypt[16];
	//char localmac[32];
	//memset(localmac, 0x00, 32);
	//memset(decrypt, 0x00, 16);
	////memset(&m_ReqData, 0x00, sizeof(auth_psd_req_data_t));
	//char localcmd[256];
	//memset(localcmd, 0x00, 256);

	//if (!m_TestStatusItemVec.empty()) {
	//	MD5Init(&m_Md5);
	//	MD5Update(&m_Md5, (unsigned char*)((char*)(CW2A(m_EditSvrPassword.GetString()))), strlen((char *)(CW2A(m_EditSvrPassword.GetString()))));
	//	MD5Final(&m_Md5, decrypt);

	//	get_mac(localmac);
	//	memcpy(m_ReqData.uid, CW2A(m_EditDevID.GetString()), m_EditDevID.GetLength());
	//	memcpy(m_ReqData.usr, CW2A(m_EditSvrUser.GetString()), m_EditSvrUser.GetLength());
	//	memcpy(m_ReqData.pwd, decrypt, 16);
	//	memcpy(m_ReqData.ver, "1", strlen("1"));
	//	memcpy(m_ReqData.mac, localmac, strlen(localmac));

	//	vector<CString>::iterator it = m_TestStatusItemVec.begin();
	//	for (; it != m_TestStatusItemVec.end(); it++) {
	//		if ((*it).Compare(TEST_PING_ITEM) == 0) {
	//			CString pcmd = L"ping -c ";
	//			pcmd += m_PingSettingDlg.m_PNum;
	//			pcmd += L" -s ";
	//			pcmd += m_PingSettingDlg.m_PpkgSize;
	//			pcmd += L" ";
	//			pcmd += m_PingSettingDlg.m_PwebSite;
	//			strcpy(localcmd, CW2A(pcmd.GetString()));
	//			SHOW_STATUS_INFO_A("MTU获取，执行命令：");
	//			SHOW_STATUS_INFO_W(pcmd.GetString());
	//		}
	//		else if ((*it).Compare(TEST_NETWORK_ITEM) == 0) {
	//			strcpy(localcmd, "netstat | grep ip | awk '{print $6}'");
	//			SHOW_STATUS_INFO_A("网络连接信息获取，执行命令：netstat | grep ip | awk '{print $6}'");
	//		}
	//		else if ((*it).Compare(TEST_ROUTE_ITEM) == 0) {
	//			strcpy(localcmd, "route'");
	//			SHOW_STATUS_INFO_A("路由表信息获取，执行命令：route");
	//		}
	//		else if ((*it).Compare(TEST_BANDWIDTH) == 0) {
	//			//strcpy(localcmd, "cat /proc/net/rtl8188fu/wlan0/rx_signal");
	//			//SHOW_STATUS_INFO_A("带宽信息获取，执行命令：cat /proc/net/rtl8188fu/wlan0/rx_signal");
	//		}
	//		else if ((*it).Compare(TEST_LOG_ITEM) == 0) {
	//			//strcpy(localcmd, "netstat -anpt");
	//			//SHOW_STATUS_INFO_A("日志数据获取，执行命令：netstat -anpt");
	//		}

	//		send_cmd_start = time(NULL);
	//		m_bOnDataFlag = FALSE;
	//		m_Ecode = 0;

	//		IOT_SHELL_Send(m_Session, localcmd, strlen(localcmd) + 1, &m_ReqData);
	//		while (!m_bOnDataFlag) {
	//			msleep_c(1000);
	//			if (time(NULL) - send_cmd_start > 30) {
	//				SHOW_STATUS_INFO_A("get data time out 30 sec!");
	//				continue;
	//			}
	//		}
	//	}
	//}
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

void Ckp2p_check_tool_win32Dlg::OnBnClickedVedioButton()
{
	// TODO: 在此添加控件通知处理程序代码
	
	m_VideoDlg.SetParent(this);
	INT_PTR nRes = m_VideoDlg.DoModal();
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


void Ckp2p_check_tool_win32Dlg::OnBnClickedLiveButton()
{
	// TODO: 在此添加控件通知处理程序代码
	
	m_LiveDlg.SetParentThis(this);
	m_LiveDlg.StartLive(m_Handle, 0);

	INT_PTR nRes = m_LiveDlg.DoModal();
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


void Ckp2p_check_tool_win32Dlg::OnBnClickedModConfigButton()
{
	// TODO: 在此添加控件通知处理程序代码

	time_t send_cmd_start = 0;
	unsigned char decrypt[16];
	char localmac[32];
	memset(localmac, 0x00, 32);
	memset(decrypt, 0x00, 16);

	//m_This = this;
	//m_DevSettingDlg.Init(this);
	m_DevSettingDlg.SetParent(this);

	//启动设备配置修改线程
	//::ResumeThread(m_ModHandleThr);
	
	INT_PTR nRes = m_DevSettingDlg.DoModal();
	//UpdateData(FALSE);
	if (nRes == IDOK) {
		
		if (m_DevSettingDlg.m_bGateWay)
			m_DevSettingDlg.m_nItemCount += 1;
		if (m_DevSettingDlg.m_bDNS)
			m_DevSettingDlg.m_nItemCount += 1;
		if (m_DevSettingDlg.m_bMTU)
			m_DevSettingDlg.m_nItemCount += 1;

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


void Ckp2p_check_tool_win32Dlg::OnBnClickedRestartIotdaemonButton()
{
	// TODO: 在此添加控件通知处理程序代码
}


void Ckp2p_check_tool_win32Dlg::OnBnClickedRestartAppButton()
{
	// TODO: 在此添加控件通知处理程序代码
}


void Ckp2p_check_tool_win32Dlg::OnBnClickedRestartDevButton()
{
	// TODO: 在此添加控件通知处理程序代码
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
