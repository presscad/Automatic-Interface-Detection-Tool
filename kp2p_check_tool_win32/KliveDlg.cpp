// KliveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "kp2p_check_tool_win32.h"
#include "kp2p_check_tool_win32Dlg.h"
#include "KliveDlg.h"
#include "afxdialogex.h"
#include "Util.h"

volatile LONG      CKliveDlg::m_OnPlayLiveCountLock = 0;
volatile LONG      CKliveDlg::m_OnPlayVedioLiveCountLock = 0;
volatile LONG      CKliveDlg::m_OnPlayAudioLiveCountLock = 0;
uint64_t           CKliveDlg::m_FrameLiveCountTotal = 0;
time_t             CKliveDlg::m_GetIFrameLiveStartTime = 0;
time_t             CKliveDlg::m_GetIFrameLiveEndTime = 0;
LONG               CKliveDlg::m_VedioFrameLiveNum = 0;
LONG               CKliveDlg::m_AudioFrameLiveNum = 0;
LONG               CKliveDlg::m_CurPlayLiveDuraTime = 0;

// CKliveDlg 对话框

IMPLEMENT_DYNAMIC(CKliveDlg, CDialogEx)

CKliveDlg::CKliveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LIVE_DIALOG, pParent)
{

}

CKliveDlg::~CKliveDlg()
{
}

void CKliveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_PLAY_KEEP_TIME_LIVE_EDIT, m_PlayDurationTimeEdit);
	DDX_Control(pDX, IDC_CUR_PLAY_CHANNEL_LIVE_COMBO, m_CurLiveChannelComboBox);
	DDX_Control(pDX, IDC_PLAY_PROGRESS_LIVE_SLIDER, m_PlayProgressSliderCtrl);
	DDX_Control(pDX, IDC_LIVE_LIST, m_PlayLiveInfoShowListCtrl);
}


BEGIN_MESSAGE_MAP(CKliveDlg, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CKliveDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_PLAY_LIVE_BUTTON, &CKliveDlg::OnBnClickedPlayButton)
	ON_BN_CLICKED(IDC_STOP_LIVE_BUTTON, &CKliveDlg::OnBnClickedStopButton)
	ON_BN_CLICKED(IDCANCEL, &CKliveDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CKliveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_PlayLiveTimerStartHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_PlayLiveTimerExitHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_PlayLiveTimeOutNotifyHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_PlayLiveQuitNotifyHandle = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_PlayLiveInfoShowListCtrl.SetExtendedStyle(m_PlayLiveInfoShowListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_PlayLiveInfoShowListCtrl.InsertColumn(0, TEXT("名称"), 0, 110);
	m_PlayLiveInfoShowListCtrl.InsertColumn(1, TEXT("数据"), 0, 150);

	m_PlayLiveBtn = NULL;
	m_PlayLiveBtn = (CButton *)GetDlgItem(IDC_PLAY_LIVE_BUTTON);
	//m_PlayLiveBtn->EnableWindow(FALSE);

	m_StopLiveBtn = NULL;
	m_StopLiveBtn = (CButton *)GetDlgItem(IDC_STOP_LIVE_BUTTON);
	m_StopLiveBtn->EnableWindow(FALSE);

	pPlayDurationEdit = NULL;
	pPlayDurationEdit = (CEdit *)GetDlgItem(IDC_PLAY_KEEP_TIME_LIVE_EDIT);

	m_LiveStreamID = 1;
	CString temp;
	for (int i = 0; i < m_LiveChannel; i++) {
		temp.Format(_T("%d"), i);
		m_CurLiveChannelComboBox.AddString(temp.GetString());
	}
	
	m_CurLiveChannelComboBox.SetCurSel(0);
	m_PlayDurationTimeEdit.SetString(L"30");
	UpdateData(FALSE);

	/*m_PlayProgressSliderCtrl.SetRange(0, atoi(CW2A(m_PlayDurationTimeEdit.GetString())));
	m_PlayProgressSliderCtrl.SetPos(0);*/

	m_PlayLiveTimerHandleThr = (HANDLE)_beginthreadex(NULL, 0, CKliveDlg::PlayLiveTimerWorkThreadProc, this, CREATE_SUSPENDED, NULL);
	if (m_PlayLiveTimerHandleThr == 0) {
		//throw MyException();
	}
	::ResumeThread(m_PlayLiveTimerHandleThr);


	return TRUE;
}

// CKliveDlg 消息处理程序
void CKliveDlg::SetParentThis(void *parent)
{
	m_Parent = (Ckp2p_check_tool_win32Dlg*)parent;
}

void CKliveDlg::SetLiveChannel(int channel)
{
	m_LiveChannel = channel;
}


void CKliveDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

void CKliveDlg::OnBnClickedPlayButton()
{
	// TODO: 在此添加控件通知处理程序代码
	m_PlayLiveInfoShowListCtrl.DeleteAllItems();
	m_PlayLiveInfoShowListCtrl.RedrawWindow();

	UpdateData(TRUE);

	::InterlockedIncrement(&m_OnPlayVedioLiveCountLock);
	::InterlockedIncrement(&m_OnPlayAudioLiveCountLock);

	CString item;
	m_CurLiveChannelComboBox.GetWindowText(item);
	m_LiveChannel = atoi(CW2A(item.GetString()));

	int nRes = kp2p_open_stream(m_Parent->m_Handle, m_LiveChannel, m_LiveStreamID);
	if (nRes != 0) {
		MessageBox(_T("直播开始播放失败"), _T("信息提示"), MB_OK);
	}

	m_PlayLiveInfoShowListCtrl.InsertItem(0, _T(""));
	m_PlayLiveInfoShowListCtrl.SetItemText(0, 0, _T("视频信息："));
	m_PlayLiveInfoShowListCtrl.InsertItem(1, _T(""));
	m_PlayLiveInfoShowListCtrl.SetItemText(1, 0, _T("分辨率"));
	m_PlayLiveInfoShowListCtrl.InsertItem(2, _T(""));
	m_PlayLiveInfoShowListCtrl.SetItemText(2, 0, _T("编码类型"));

	m_PlayLiveInfoShowListCtrl.InsertItem(3, _T(""));
	m_PlayLiveInfoShowListCtrl.SetItemText(3, 0, _T(""));
	m_PlayLiveInfoShowListCtrl.InsertItem(4, _T(""));
	m_PlayLiveInfoShowListCtrl.SetItemText(4, 0, _T("音频信息："));
	m_PlayLiveInfoShowListCtrl.InsertItem(5, _T(""));
	m_PlayLiveInfoShowListCtrl.SetItemText(5, 0, _T("音频采样率"));
	m_PlayLiveInfoShowListCtrl.InsertItem(6, _T(""));
	m_PlayLiveInfoShowListCtrl.SetItemText(6, 0, _T("音频采样位数"));
	m_PlayLiveInfoShowListCtrl.InsertItem(7, _T(""));
	m_PlayLiveInfoShowListCtrl.SetItemText(7, 0, _T("音频采样声道数"));
	m_PlayLiveInfoShowListCtrl.InsertItem(8, _T(""));
	m_PlayLiveInfoShowListCtrl.SetItemText(8, 0, _T("比特率"));
	m_PlayLiveInfoShowListCtrl.InsertItem(9, _T(""));
	m_PlayLiveInfoShowListCtrl.SetItemText(9, 0, _T("编码类型"));

	m_PlayLiveInfoShowListCtrl.InsertItem(10, _T(""));
	m_PlayLiveInfoShowListCtrl.SetItemText(10, 0, _T(""));
	m_PlayLiveInfoShowListCtrl.InsertItem(11, _T(""));
	m_PlayLiveInfoShowListCtrl.SetItemText(11, 0, _T("统计信息:"));
	m_PlayLiveInfoShowListCtrl.InsertItem(12, _T(""));
	m_PlayLiveInfoShowListCtrl.SetItemText(12, 0, _T("第一个I帧抵达时间"));
	m_PlayLiveInfoShowListCtrl.InsertItem(13, _T(""));
	m_PlayLiveInfoShowListCtrl.SetItemText(13, 0, _T("码流字节数"));
	m_PlayLiveInfoShowListCtrl.InsertItem(14, _T(""));
	m_PlayLiveInfoShowListCtrl.SetItemText(14, 0, _T("音频帧数"));
	m_PlayLiveInfoShowListCtrl.InsertItem(15, _T(""));
	m_PlayLiveInfoShowListCtrl.SetItemText(15, 0, _T("视频帧数"));
	m_PlayLiveInfoShowListCtrl.InsertItem(16, _T(""));
	m_PlayLiveInfoShowListCtrl.SetItemText(16, 0, _T("帧率"));
	m_PlayLiveInfoShowListCtrl.InsertItem(17, _T(""));
	m_PlayLiveInfoShowListCtrl.SetItemText(17, 0, _T("平均带宽"));
	m_PlayLiveInfoShowListCtrl.InsertItem(18, _T(""));
	m_PlayLiveInfoShowListCtrl.SetItemText(18, 0, _T("播放时长"));

	m_GetIFrameLiveStartTime = GetTickCount();
	SetTimer(1, 990, NULL);
	SetEvent(m_PlayLiveTimerStartHandle);

	m_PlayLiveBtn->EnableWindow(FALSE);
	m_StopLiveBtn->EnableWindow(TRUE);
	pPlayDurationEdit->EnableWindow(FALSE);
	m_CurLiveChannelComboBox.EnableWindow(FALSE);

	m_PlayProgressSliderCtrl.SetRange(0, atoi(CW2A(m_PlayDurationTimeEdit.GetString())));
	m_PlayProgressSliderCtrl.SetPos(0);
}


void CKliveDlg::OnBnClickedStopButton()
{
	// TODO: 在此添加控件通知处理程序代码

	SetEvent(m_PlayLiveTimeOutNotifyHandle);

	/*int nRes = kp2p_close_stream(m_Parent->m_Handle, m_LiveChannel, m_LiveStreamID);
	if (nRes != 0) {
		MessageBox(_T("直播停止播放失败"), _T("信息提示"), MB_OK);
	}*/
}

unsigned int __stdcall CKliveDlg::PlayLiveTimerWorkThreadProc(PVOID arg)
{
	BOOL bExit = FALSE;
	DWORD nRet, nRes;
	HANDLE h[3];

	CKliveDlg *p = (CKliveDlg*)arg;

	h[0] = p->m_Parent->m_DevOfflineNotifyEvent;
	h[1] = p->m_PlayLiveTimeOutNotifyHandle;
	h[2] = p->m_PlayLiveQuitNotifyHandle;

	while (!bExit)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(p->m_PlayLiveTimerExitHandle, 0))
			bExit = TRUE;

		if ((nRet = WaitForSingleObject(p->m_PlayLiveTimerStartHandle, 0)) != WAIT_OBJECT_0) {
			msleep_c(50);
			continue;
		}
		::InterlockedIncrement(&m_OnPlayLiveCountLock);

		DWORD tm = atol(CW2A(p->m_PlayDurationTimeEdit.GetString())) * 1000;
		nRes = WaitForMultipleObjects(3, h, FALSE, tm);
		if (nRes == WAIT_OBJECT_0 + 0) {
			kp2p_close_stream(p->m_Parent->m_Handle, p->m_LiveChannel, p->m_LiveStreamID);

			p->m_PlayLiveInfoShowListCtrl.InsertItem(19, _T(""));
			p->m_PlayLiveInfoShowListCtrl.SetItemText(19, 0, _T(""));
			p->m_PlayLiveInfoShowListCtrl.InsertItem(20, _T(""));
			p->m_PlayLiveInfoShowListCtrl.SetItemText(20, 0, _T(""));
			p->m_PlayLiveInfoShowListCtrl.InsertItem(21, _T(""));
			p->m_PlayLiveInfoShowListCtrl.SetItemText(21, 0, _T("播放结束"));

			p->pPlayDurationEdit->EnableWindow(TRUE);
			p->m_StopLiveBtn->EnableWindow(FALSE);
			p->m_PlayLiveBtn->EnableWindow(FALSE);
			p->KillTimer(1);
			m_FrameLiveCountTotal = 0;
			m_VedioFrameLiveNum = 0;
			m_AudioFrameLiveNum = 0;
			m_CurPlayLiveDuraTime = 0;
			p->MessageBox(_T("设备连接断开，直播测试中止"), _T("信息提示"), MB_OK);
		}
		else if (nRes == WAIT_OBJECT_0 + 1) {
			kp2p_close_stream(p->m_Parent->m_Handle, p->m_LiveChannel, p->m_LiveStreamID);

			p->m_PlayLiveInfoShowListCtrl.InsertItem(19, _T(""));
			p->m_PlayLiveInfoShowListCtrl.SetItemText(19, 0, _T(""));
			p->m_PlayLiveInfoShowListCtrl.InsertItem(20, _T(""));
			p->m_PlayLiveInfoShowListCtrl.SetItemText(20, 0, _T(""));
			p->m_PlayLiveInfoShowListCtrl.InsertItem(21, _T(""));
			p->m_PlayLiveInfoShowListCtrl.SetItemText(21, 0, _T("播放结束"));

			p->pPlayDurationEdit->EnableWindow(TRUE);
			p->m_StopLiveBtn->EnableWindow(FALSE);
			p->m_PlayLiveBtn->EnableWindow(TRUE);
			p->m_CurLiveChannelComboBox.EnableWindow(TRUE);
			p->KillTimer(1);
			m_FrameLiveCountTotal = 0;
			m_VedioFrameLiveNum = 0;
			m_AudioFrameLiveNum = 0;
			m_CurPlayLiveDuraTime = 0;
		}
		else if (nRes == WAIT_OBJECT_0 + 2) {
			kp2p_close_stream(p->m_Parent->m_Handle, p->m_LiveChannel, p->m_LiveStreamID);

			p->KillTimer(1);
			m_FrameLiveCountTotal = 0;
			m_VedioFrameLiveNum = 0;
			m_AudioFrameLiveNum = 0;
			m_CurPlayLiveDuraTime = 0;
		}
		else if (nRes == WAIT_TIMEOUT) {
			kp2p_close_stream(p->m_Parent->m_Handle, p->m_LiveChannel, p->m_LiveStreamID);

			p->m_PlayLiveInfoShowListCtrl.InsertItem(19, _T(""));
			p->m_PlayLiveInfoShowListCtrl.SetItemText(19, 0, _T(""));
			p->m_PlayLiveInfoShowListCtrl.InsertItem(20, _T(""));
			p->m_PlayLiveInfoShowListCtrl.SetItemText(20, 0, _T(""));
			p->m_PlayLiveInfoShowListCtrl.InsertItem(21, _T(""));
			p->m_PlayLiveInfoShowListCtrl.SetItemText(21, 0, _T("播放结束"));

			p->pPlayDurationEdit->EnableWindow(TRUE);
			p->m_StopLiveBtn->EnableWindow(FALSE);
			p->m_PlayLiveBtn->EnableWindow(TRUE);
			p->m_CurLiveChannelComboBox.EnableWindow(TRUE);
			p->KillTimer(1);
			m_FrameLiveCountTotal = 0;
			m_VedioFrameLiveNum = 0;
			m_AudioFrameLiveNum = 0;
			m_CurPlayLiveDuraTime = 0;
		}
#if 0
		/*switch (nRes)
		{
		case WAIT_OBJECT_0 + 0:
			kp2p_close_stream(p->m_Parent->m_Handle, p->m_LiveChannel, p->m_LiveStreamID);

			p->m_PlayLiveInfoShowListCtrl.InsertItem(19, _T(""));
			p->m_PlayLiveInfoShowListCtrl.SetItemText(19, 0, _T(""));
			p->m_PlayLiveInfoShowListCtrl.InsertItem(20, _T(""));
			p->m_PlayLiveInfoShowListCtrl.SetItemText(20, 0, _T(""));
			p->m_PlayLiveInfoShowListCtrl.InsertItem(21, _T(""));
			p->m_PlayLiveInfoShowListCtrl.SetItemText(21, 0, _T("播放结束"));

			p->pPlayDurationEdit->EnableWindow(TRUE);
			p->m_StopLiveBtn->EnableWindow(FALSE);
			p->m_PlayLiveBtn->EnableWindow(FALSE);
			p->KillTimer(1);
			m_FrameLiveCountTotal = 0;
			m_VedioFrameLiveNum = 0;
			m_AudioFrameLiveNum = 0;
			m_CurPlayLiveDuraTime = 0;
			p->MessageBox(_T("设备连接断开，直播测试中止"), _T("信息提示"), MB_OK);
			break;
		case WAIT_OBJECT_0 + 1:
			kp2p_close_stream(p->m_Parent->m_Handle, p->m_LiveChannel, p->m_LiveStreamID);

			p->m_PlayLiveInfoShowListCtrl.InsertItem(19, _T(""));
			p->m_PlayLiveInfoShowListCtrl.SetItemText(19, 0, _T(""));
			p->m_PlayLiveInfoShowListCtrl.InsertItem(20, _T(""));
			p->m_PlayLiveInfoShowListCtrl.SetItemText(20, 0, _T(""));
			p->m_PlayLiveInfoShowListCtrl.InsertItem(21, _T(""));
			p->m_PlayLiveInfoShowListCtrl.SetItemText(21, 0, _T("播放结束"));

			p->pPlayDurationEdit->EnableWindow(TRUE);
			p->m_StopLiveBtn->EnableWindow(FALSE);
			p->m_PlayLiveBtn->EnableWindow(TRUE);
			p->KillTimer(1);
			m_FrameLiveCountTotal = 0;
			m_VedioFrameLiveNum = 0;
			m_AudioFrameLiveNum = 0;
			m_CurPlayLiveDuraTime = 0;
			break;
		case WAIT_TIMEOUT:
			kp2p_close_stream(p->m_Parent->m_Handle, p->m_LiveChannel, p->m_LiveStreamID);

			p->m_PlayLiveInfoShowListCtrl.InsertItem(19, _T(""));
			p->m_PlayLiveInfoShowListCtrl.SetItemText(19, 0, _T(""));
			p->m_PlayLiveInfoShowListCtrl.InsertItem(20, _T(""));
			p->m_PlayLiveInfoShowListCtrl.SetItemText(20, 0, _T(""));
			p->m_PlayLiveInfoShowListCtrl.InsertItem(21, _T(""));
			p->m_PlayLiveInfoShowListCtrl.SetItemText(21, 0, _T("播放结束"));

			p->pPlayDurationEdit->EnableWindow(TRUE);
			p->m_StopLiveBtn->EnableWindow(FALSE);
			p->m_PlayLiveBtn->EnableWindow(TRUE);
			p->KillTimer(1);
			m_FrameLiveCountTotal = 0;
			m_VedioFrameLiveNum = 0;
			m_AudioFrameLiveNum = 0;
			m_CurPlayLiveDuraTime = 0;
			break;
		case WAIT_FAILED:
			break;
		
		}*/
#endif
		::InterlockedDecrement(&m_OnPlayLiveCountLock);

	}

	return 0;
}


void CKliveDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString temp;
	switch (nIDEvent) {
	case 1:
		m_CurPlayLiveDuraTime += 1;
		temp.Format(_T("%ld"), m_FrameLiveCountTotal);
		m_PlayLiveInfoShowListCtrl.SetItemText(13, 1, temp.GetString());
		temp.Format(_T("%ld"), m_AudioFrameLiveNum);
		m_PlayLiveInfoShowListCtrl.SetItemText(14, 1, temp.GetString());
		temp.Format(_T("%ld"), m_VedioFrameLiveNum);
		m_PlayLiveInfoShowListCtrl.SetItemText(15, 1, temp.GetString());
		temp.Format(_T("%ld fps"), m_VedioFrameLiveNum / m_CurPlayLiveDuraTime);
		m_PlayLiveInfoShowListCtrl.SetItemText(16, 1, temp.GetString());
		temp.Format(_T("%ld byte/s"), m_FrameLiveCountTotal / m_CurPlayLiveDuraTime);
		m_PlayLiveInfoShowListCtrl.SetItemText(17, 1, temp.GetString());
		temp.Format(_T("%ld 秒"), m_CurPlayLiveDuraTime);
		m_PlayLiveInfoShowListCtrl.SetItemText(18, 1, temp.GetString());
		break;
	case 2:
		break;
	}

	m_PlayProgressSliderCtrl.SetPos(m_CurPlayLiveDuraTime);

	CDialogEx::OnTimer(nIDEvent);
}


void CKliveDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	INT_PTR nRes;
	HANDLE hThread[1];
	hThread[0] = m_PlayLiveTimerHandleThr;

	::InterlockedIncrement(&m_OnPlayLiveCountLock);
	if (::InterlockedDecrement(&m_OnPlayLiveCountLock) != 0) {
		nRes = MessageBox(_T("正在播放中，确认是否要中止并退出？"), _T("信息提示"), MB_YESNO);
		if (nRes == IDNO) {
			return;
		}
		SetEvent(m_PlayLiveQuitNotifyHandle);
	}

	SetEvent(m_PlayLiveTimerExitHandle);

	WaitForMultipleObjects(1, hThread, TRUE, INFINITE);
	for (int i = 0; i < (sizeof(hThread) / sizeof(hThread[0])); i++) {
		CloseHandle(hThread[i]);
	}

	//CDialogEx::OnOK();
	CDialogEx::OnCancel();
}
