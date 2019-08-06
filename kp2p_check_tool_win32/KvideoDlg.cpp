// Kvideo.cpp : 实现文件
//

#include "stdafx.h"
#include "kp2p_check_tool_win32.h"
#include "kp2p_check_tool_win32Dlg.h"
#include "KvideoDlg.h"
#include "afxdialogex.h"
#include "Util.h"

volatile LONG CKvideoDlg::m_OnReplayRecCountLock = 0;
volatile LONG CKvideoDlg::m_OnReplayVedioCountLock = 0;
volatile LONG CKvideoDlg::m_OnReplayAudioCountLock = 0;
uint64_t      CKvideoDlg::m_FrameCountTotal = 0;
time_t        CKvideoDlg::m_GetIFrameStartTime = 0;
time_t        CKvideoDlg::m_GetIFrameEndTime= 0;
LONG          CKvideoDlg::m_VedioFrameNum = 0;
LONG          CKvideoDlg::m_AudioFrameNum = 0;
LONG          CKvideoDlg::m_CurReplayRecDuraTime = 0;

// CKvideoDlg 对话框

IMPLEMENT_DYNAMIC(CKvideoDlg, CDialogEx)

CKvideoDlg::CKvideoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VEDIO_DIALOG, pParent)
{

}

CKvideoDlg::~CKvideoDlg()
{
}

void CKvideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_PLAY_KEEP_TIME_VEDIO_EDIT, m_ReplayDurationTimeEdit);

	DDX_Control(pDX, IDC_VEDIO_FILE_LIST, m_RecFileInfoListCtrl);
	DDX_Control(pDX, IDC_SHOW_REALTIME_INFO_VEDIO_LIST, m_ReplayRecInfoShowListCtrl);
	DDX_Control(pDX, IDC_CUR_REC_CHANNEL_VEDIO_COMBO, m_CurRecChannelComboBox);
	DDX_Control(pDX, IDC_QUERY_REC_CHANNEL_VEDIO_COMBO, m_QueryRecChannelComboBox);
	
	DDX_Control(pDX, IDC_REPLAY_PROGRESS_VEDIO_SLIDER, m_ReplayProgressSliderCtrl);

	DDX_Control(pDX, IDC_DATE_VEDIO_DATETIMEPICKER, m_QueryDateTimeCtrl);
	DDX_Control(pDX, IDC_START_TIME_VEDIO_DATETIMEPICKER, m_QueryStartTimeCtrl);
	DDX_Control(pDX, IDC_END_TIME_VEDIO_DATETIMEPICKER, m_QueryEndTimeCtrl);
	DDX_Control(pDX, IDC_PLAY_START_VEDIO_DATETIMEPICKER, m_ReplayStartTimeCtrl);
	DDX_Control(pDX, IDC_PLAY_END_VEDIO_DATETIMEPICKER, m_ReplayEndTimeCtrl);

}


BEGIN_MESSAGE_MAP(CKvideoDlg, CDialogEx)
	ON_WM_TIMER()
	//ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER3, &CKvideoDlg::OnDtnDatetimechangeDatetimepicker3)
	ON_BN_CLICKED(IDC_QUERY_VEDIO_BUTTON, &CKvideoDlg::OnBnClickedQueryVedioButton)
	ON_BN_CLICKED(IDOK, &CKvideoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CKvideoDlg::OnBnClickedCancel)
	ON_NOTIFY(NM_DBLCLK, IDC_VEDIO_FILE_LIST, &CKvideoDlg::OnNMDblclkVedioFileList)
	ON_BN_CLICKED(IDC_PLAY_VEDIO_BUTTON, &CKvideoDlg::OnBnClickedPlayVedioButton)
	ON_BN_CLICKED(IDC_STOP_VEDIO_BUTTON, &CKvideoDlg::OnBnClickedStopVediobutton)
END_MESSAGE_MAP()


// CKvideo 消息处理程序


BOOL CKvideoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ReplayProgressExitHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_ReplayProgressStartHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_ReplayRecTimerStartHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_ReplayRecTimerExitHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_ReplayRecTimeOutNotifyHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_ReplayRecTimerQuitNotifyHandle = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_RecFileInfoListCtrl.SetExtendedStyle(m_RecFileInfoListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_RecFileInfoListCtrl.InsertColumn(0, TEXT("ID"), 0, 35);
	m_RecFileInfoListCtrl.InsertColumn(1, TEXT("channel"), 0, 55);
	m_RecFileInfoListCtrl.InsertColumn(2, TEXT("录像类型"), 0, 90);
	m_RecFileInfoListCtrl.InsertColumn(3, TEXT("录像开始时间"), 0, 150);
	m_RecFileInfoListCtrl.InsertColumn(4, TEXT("录像结束时间"), 0, 150);
	m_RecFileInfoListCtrl.InsertColumn(5, TEXT("录像时长(秒)"), 0, 150);

	m_ReplayRecInfoShowListCtrl.SetExtendedStyle(m_ReplayRecInfoShowListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ReplayRecInfoShowListCtrl.InsertColumn(0, TEXT("名称"), 0, 110);
	m_ReplayRecInfoShowListCtrl.InsertColumn(1, TEXT("数据"), 0, 150);

	m_QueryRecBtn = NULL;
	m_QueryRecBtn = (CButton *)GetDlgItem(IDC_QUERY_VEDIO_BUTTON);

	m_PlayRecBtn = NULL;
	m_PlayRecBtn = (CButton *)GetDlgItem(IDC_PLAY_VEDIO_BUTTON);
	m_PlayRecBtn->EnableWindow(FALSE);

	m_StopRecBtn = NULL;
	m_StopRecBtn = (CButton *)GetDlgItem(IDC_STOP_VEDIO_BUTTON);
	m_StopRecBtn->EnableWindow(FALSE);

	pReplayDurationEdit = NULL;
	pReplayDurationEdit = (CEdit *)GetDlgItem(IDC_PLAY_KEEP_TIME_VEDIO_EDIT);

	CString temp;
	for (int i = 0; i < m_VedioChannel; i++) {
		temp.Format(_T("%d"), i);
		m_QueryRecChannelComboBox.AddString(temp.GetString());
		m_CurRecChannelComboBox.AddString(temp.GetString());
	}
	
	m_CurRecChannelComboBox.SetCurSel(0);
	m_QueryRecChannelComboBox.SetCurSel(0);
	m_ReplayDurationTimeEdit.SetString(L"30");
	UpdateData(FALSE);

	m_ReplayRecTimerHandleThr = (HANDLE)_beginthreadex(NULL, 0, CKvideoDlg::ReplayRecTimerWorkThreadProc, this, CREATE_SUSPENDED, NULL);
	if (m_ReplayRecTimerHandleThr == 0) {
		//printlog("CreateThread failed. LastError: %u\n", GetLastError());
		//free(t);
		//throw MyException();
	}
	::ResumeThread(m_ReplayRecTimerHandleThr);

	CTime time1 = CTime::GetCurrentTime();
	CTime query_start(time1.GetYear(), time1.GetMonth(), time1.GetDay(), 00, 00, 00);
	m_QueryStartTimeCtrl.SetTime(&query_start);
	UpdateData(FALSE);

	CTime query_end(time1.GetYear(), time1.GetMonth(), time1.GetDay(), 23, 59, 59);
	m_QueryEndTimeCtrl.SetTime(&query_end);
	UpdateData(FALSE);

	CTime replay_start(time1.GetYear(), time1.GetMonth(), time1.GetDay(), 0, 0, 0);
	m_ReplayStartTimeCtrl.SetTime(&replay_start);
	UpdateData(FALSE);

	CTime replay_end(time1.GetYear(), time1.GetMonth(), time1.GetDay(), 23, 59, 59);
	m_ReplayEndTimeCtrl.SetTime(&replay_end);
	UpdateData(FALSE);

#if 0
	//CString  mTime;
	//mTime = "10:00:00";        //自设时间
	//COleDateTime        dt;
	//dt.ParseDateTime(mTime);        //将mTime的时间值存到dt中
	//SYSTEMTIME        st;        //定义一个系统时间类型的变量
	//dt.GetAsSystemTime(st);        //将dt中的时间按系统时间格式化

	//CTime        tm(st);        //定义CTime对象并将st赋值给它
	//tm.Format("%H%H:%M%M:%S%S");
	//m_QueryStartTimeCtrl.SetTime(&tm);        //控件会自动获取其需要的时间部分
#endif

	return TRUE;
}

//void CKvideoDlg::OnDtnDatetimechangeDatetimepicker3(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}

void CKvideoDlg::OnBnClickedQueryVedioButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	INT nRes;

	m_QueryStartTimeCtrl.EnableWindow(FALSE);
	m_QueryEndTimeCtrl.EnableWindow(FALSE);
	m_QueryRecChannelComboBox.EnableWindow(FALSE);

	//查找
	//FindRecFile(m_Parent->m_Handle);
	//检索
	nRes = SearchRecFile(m_Parent->m_Handle);
	if (nRes == 0) {
		m_PlayRecBtn->EnableWindow(TRUE);
		/*std::map<uint32_t, CString>::iterator it = m_RecChannelMap.begin();
		for (; it != m_RecChannelMap.end(); it++) {
			CString ch;
			ch.Format(_T("%u"), it->first);
			m_CurRecChannelComboBox.AddString(ch.GetString());
			m_CurRecChannelComboBox.SetCurSel(0);
		}*/
	}
	m_QueryStartTimeCtrl.EnableWindow(TRUE);
	m_QueryEndTimeCtrl.EnableWindow(TRUE);
	m_QueryRecChannelComboBox.EnableWindow(TRUE);
}


void CKvideoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;
	HANDLE hThread[1];
	hThread[0] = m_ReplayRecTimerHandleThr;

	::InterlockedIncrement(&m_OnReplayRecCountLock);
	if (::InterlockedDecrement(&m_OnReplayRecCountLock) != 0) {
		nRes = MessageBox(_T("正在直播中，确认是否要中止并退出？"), _T("信息提示"), MB_YESNO);
		if (nRes == IDNO) {
			return;
		}
		SetEvent(m_ReplayRecTimerQuitNotifyHandle);
	}

	SetEvent(m_ReplayRecTimerExitHandle);

	WaitForMultipleObjects(1, hThread, TRUE, INFINITE);
	for (int i = 0; i < (sizeof(hThread) / sizeof(hThread[0])); i++) {
		CloseHandle(hThread[i]);
	}

	/*HANDLE hThread[1];
	hThread[0] = m_ReplayRecTimerHandleThr;

	SetEvent(m_ReplayRecTimerExitHandle);

	WaitForMultipleObjects(1, hThread, TRUE, INFINITE);
	for (int i = 0; i < (sizeof(hThread) / sizeof(hThread[0])); i++) {
		CloseHandle(hThread[i]);
	}*/

	CDialogEx::OnOK();
}

void CKvideoDlg::SetParent(PVOID arg)
{
	m_Parent = (Ckp2p_check_tool_win32Dlg*)arg;
}

void CKvideoDlg::SetVedioChannel(int channel)
{
	m_VedioChannel = channel;
}

INT CKvideoDlg::SearchRecFile(PVOID arg)
{
	kp2p_handle_t			    handle = (kp2p_handle_t)arg;
	kp2p_rec_search_handle_t    search_h;
	kp2p_rec_files_t            *pRecords = NULL;
	//kp2p_rec_playback_handle_t  play_h;
	kp2p_rec_file_t             play_file_r = { 0 };
	
	/*uint8_t *chn = (uint8_t*)malloc((m_Parent->m_Channel) * sizeof(uint8_t));
	for (int i = 0; i < m_Parent->m_Channel; i++) {
		chn[i] = i;
	}*/

	CString item;
	m_QueryRecChannelComboBox.GetWindowText(item);
	//memcpy(chn, CW2A(item.GetString()), item.GetLength());
	uint8_t chn[1] = { 0 };
	uint32_t num = atoi(CW2A(item.GetString()));
	chn[0] = num;

	time_t  start_time, end_time;
	//replay_maketime(&starttime, &endtime);
	int type = 15;

	UpdateData(TRUE);
	CTime query_date, query_start_time, query_end_time;
	//COleDateTime query_date, query_start_time, query_end_time;
	m_QueryDateTimeCtrl.GetTime(query_date);
	m_QueryStartTimeCtrl.GetTime(query_start_time);
	m_QueryEndTimeCtrl.GetTime(query_end_time);
	start_time = query_start_time.GetTime();
	end_time = query_end_time.GetTime();

	if (start_time == end_time) {
		MessageBox(_T("开始时间与结束时间相同，请重新设置"), _T("信息提示"), MB_OK);
		m_QueryStartTimeCtrl.SetFocus();
		/*free(chn);
		chn = NULL;*/
		return -1;
	}

	uint32_t i = 0;
	search_h = kp2p_rec_find_file_start(handle, sizeof(chn) / sizeof(chn[0]), chn, start_time, end_time, type);
	if (NULL == search_h) {
		MessageBox(_T("找不到录像文件"), _T("信息提示"), MB_OK);
		//kp2p_rec_find_file_stop(search_h);
		/*free(chn);
		chn = NULL;*/
		return -1;
	}
#if 0
	while ((pRecords = kp2p_rec_find_file_next(search_h))) {
		/*play_file_r.begin_time = pRecords->records[0].begin_time;
		play_file_r.end_time = pRecords->records[0].end_time;
		play_file_r.channel = pRecords->records[0].channel;
		play_file_r.type = pRecords->records[0].type;*/
		play_file_r.begin_time = pRecords->records[0].begin_time;
		if (pRecords && pRecords->records_cnt <= 0) {
			break;
		}
		/*int i = 0;
		for (i = 0; i < pRecords->records_cnt; i++) {
		printf("kp2p_rec_find_file[%d] ch:%d type:%d %d - %d\n", i,
		pRecords->records[i].channel, pRecords->records[i].type,
		pRecords->records[i].begin_time, pRecords->records[i].end_time);
		}*/
	}
#endif

#if 1
	ssize_t rec_count;
	while ((pRecords = kp2p_rec_find_file_next(search_h)) && pRecords->records_cnt > 0) {

		/*play_file_r.begin_time = pRecords->records[pRecords->records_cnt - 1].begin_time;
		play_file_r.end_time = pRecords->records[pRecords->records_cnt - 1].end_time;
		play_file_r.channel = pRecords->records[pRecords->records_cnt - 1].channel;
		play_file_r.type = pRecords->records[pRecords->records_cnt - 1].type;*/

		rec_count = kp2p_rec_find_file_count(search_h);

		break;
	}
#endif
	//if (!pRecords && (0 == play_file_r.begin_time)) 
	if (!pRecords && rec_count > 0)
	{
		MessageBox(_T("找不到录像文件"), _T("信息提示"), MB_OK);
		kp2p_rec_find_file_stop(search_h);
		/*free(chn);
		chn = NULL;*/
		return -1;
	}

	m_RecFileInfoListCtrl.DeleteAllItems();

	i = 0;
	char temp[20] = { 0 };
	struct tm p;
	time_t tm_start, tm_end;
	CString idStr, channelStr, typeStr, begin_timeStr, end_timeStr, time_count/*qualityStr*/;
	for (i = 0; i < pRecords->records_cnt; i++) {
		idStr.Format(L"%d", i + 1);
		m_RecFileInfoListCtrl.InsertItem(i, idStr.GetString());
		channelStr.Format(L"%d", pRecords->records[i].channel);
		m_RecFileInfoListCtrl.SetItemText(i, 1, channelStr.GetString());
		m_RecChannelMap.insert(std::map<uint32_t, CString>::value_type(pRecords->records[i].channel, CString("1")));

		typeStr.Format(L"%d", pRecords->records[i].type);
		switch (pRecords->records[i].type) {
		case 1:
			m_RecFileInfoListCtrl.SetItemText(i, 2, _T("日程录像"));
			break;
		case 2:
			m_RecFileInfoListCtrl.SetItemText(i, 2, _T("移动侦测录像"));
			break;
		}

		memset(temp, 0, 20);
		tm_start = (long)(pRecords->records[i].begin_time);
		p = *localtime(&tm_start);
		strftime(temp, 1000, "%Y-%m-%d %H:%M:%S", &p);
		begin_timeStr = temp;
		m_RecFileInfoListCtrl.SetItemText(i, 3, begin_timeStr.GetString());
		//m_RecFileInfoListCtrl.RedrawWindow();

		memset(temp, 0, 20);
		tm_end = (long)(pRecords->records[i].end_time);
		p = *localtime(&tm_end);
		strftime(temp, 1000, "%Y-%m-%d %H:%M:%S", &p);
		end_timeStr = temp;
		m_RecFileInfoListCtrl.SetItemText(i, 4, end_timeStr.GetString());
		//m_RecFileInfoListCtrl.RedrawWindow();

		double cost = difftime(tm_end, tm_start);
		time_count.Format(_T("%.2f"), cost);
		m_RecFileInfoListCtrl.SetItemText(i, 5, time_count.GetString());
		m_RecFileInfoListCtrl.RedrawWindow();

		/*printf("kp2p_rec_find_file[%d] ch:%d type:%d %d - %d\n", i,
			pRecords->records[i].channel, pRecords->records[i].type,
			pRecords->records[i].begin_time, pRecords->records[i].end_time);*/
	}
	kp2p_rec_find_file_stop(search_h);
	/*free(chn);
	chn = NULL;*/

	return 0;
}

void CKvideoDlg::FindRecFile(PVOID arg)
{
	kp2p_handle_t           handle = (kp2p_handle_t)arg;
	kp2p_find_file_cond_t   cond;
	kp2p_find_file_handle_t find_handle;
	kp2p_find_file_data_t   find_data;


	CTime query_date, query_start_time, query_end_time;
	m_QueryDateTimeCtrl.GetTime(query_date);
	m_QueryStartTimeCtrl.GetTime(query_start_time);
	m_QueryEndTimeCtrl.GetTime(query_end_time);

	cond.channel = 0;
	cond.start_time.year = query_date.GetYear();
	cond.start_time.month = query_date.GetMonth();
	cond.start_time.day = query_date.GetDay();
	cond.start_time.hour = 16;
	cond.start_time.minute = 0;
	cond.start_time.second = 0;
	/*cond.start_time.hour = query_start_time.GetHour();
	cond.start_time.minute = query_start_time.GetMinute();
	cond.start_time.second = query_start_time.GetSecond();*/
	cond.stop_time.year = query_date.GetYear();
	cond.stop_time.month = query_date.GetMonth();
	cond.stop_time.day = query_date.GetDay();
	cond.stop_time.hour = 17;
	cond.stop_time.minute = 0;
	cond.stop_time.second = 0;
	/*cond.stop_time.hour = query_end_time.GetHour();
	cond.stop_time.minute = query_end_time.GetMinute();
	cond.stop_time.second = query_end_time.GetSecond();*/
	cond.type = 2;
	find_handle = kp2p_find_file(handle, &cond, 5);
	if (!find_handle) {
		return;
	}

	while (kp2p_find_next_file(find_handle, &find_data, 5) != -1) {
		printf("file_size(%d)\n", find_data.file_size);
	}

	kp2p_find_stop(find_handle, 5);
	return;
}


void CKvideoDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;
	HANDLE hThread[1];
	hThread[0] = m_ReplayRecTimerHandleThr;

	::InterlockedIncrement(&m_OnReplayRecCountLock);
	if (::InterlockedDecrement(&m_OnReplayRecCountLock) != 0) {
		nRes = MessageBox(_T("正在播放中，确认是否要中止并退出？"), _T("信息提示"), MB_YESNO);
		if (nRes == IDNO) {
			return;
		}
		SetEvent(m_ReplayRecTimerQuitNotifyHandle);
	}

	SetEvent(m_ReplayRecTimerExitHandle);

	WaitForMultipleObjects(1, hThread, TRUE, INFINITE);
	for (int i = 0; i < (sizeof(hThread) / sizeof(hThread[0])); i++) {
		CloseHandle(hThread[i]);
	}

	CDialogEx::OnCancel();
}


void CKvideoDlg::OnNMDblclkVedioFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	kp2p_rec_search_handle_t   search_h;
	kp2p_rec_files_t           *pRecords;
	//kp2p_rec_playback_handle_t play_h;
	kp2p_rec_file_t            play_file_r = { 0 };


	POSITION p = m_RecFileInfoListCtrl.GetFirstSelectedItemPosition();
	if (p == NULL)
	{
		TRACE(_T("没有行被选中!"));
		*pResult = 0;
		return;
	}

	::InterlockedIncrement(&m_OnReplayRecCountLock);
	if (::InterlockedDecrement(&m_OnReplayRecCountLock) != 0) {
		INT_PTR nRes = MessageBox(_T("正在播放中，确认是否要中止并退出？"), _T("信息提示"), MB_YESNO);
		if (nRes == IDNO) {
			return;
		}
		SetEvent(m_ReplayRecTimerQuitNotifyHandle);
	}
	Sleep(500);
	int index = m_RecFileInfoListCtrl.GetNextSelectedItem(p);
	CString channelStr = m_RecFileInfoListCtrl.GetItemText(index, 1);
	CString begin_timeStr = m_RecFileInfoListCtrl.GetItemText(index, 3);
	CString end_timeStr = m_RecFileInfoListCtrl.GetItemText(index, 4);
	CString duration_timeStr = m_RecFileInfoListCtrl.GetItemText(index, 5);

	//m_CurRecChannelComboBox.SetWindowText(channelStr.GetString());
	int sel_num = m_CurRecChannelComboBox.GetCount();
	int i;
	for (i = 0; i < sel_num; i++) {
		CString temp;
		m_CurRecChannelComboBox.GetLBText(i, temp);
		if (temp.Compare(channelStr) == 0) {
			m_CurRecChannelComboBox.SetCurSel(i);
			break;
		}
	}

	UpdateData(FALSE);

	struct tm sTime_begin;
	swscanf(begin_timeStr.GetString(), L"%d-%d-%d %d:%d:%d", &sTime_begin.tm_year, &sTime_begin.tm_mon, &sTime_begin.tm_mday, &sTime_begin.tm_hour, &sTime_begin.tm_min, &sTime_begin.tm_sec);
	//sTime_begin.tm_year -= 1900;
	//sTime_begin.tm_mon -= 1;
	//time_t begin_timestamp = mktime(&sTime_begin);
	CTime query_start(sTime_begin.tm_year, sTime_begin.tm_mon, sTime_begin.tm_mday, sTime_begin.tm_hour, sTime_begin.tm_min, sTime_begin.tm_sec);
	m_ReplayStartTimeCtrl.DeleteTempMap();
	m_ReplayStartTimeCtrl.SetTime(&query_start);
	UpdateData(FALSE);

	struct tm sTime_end;
	swscanf(end_timeStr.GetString(), L"%d-%d-%d %d:%d:%d", &sTime_end.tm_year, &sTime_end.tm_mon, &sTime_end.tm_mday, &sTime_end.tm_hour, &sTime_end.tm_min, &sTime_end.tm_sec);
	//sTime_end.tm_year -= 1900;
	//sTime_end.tm_mon -= 1;
	//time_t end_timestamp = mktime(&sTime_end);
	CTime replay_end(sTime_end.tm_year, sTime_end.tm_mon, sTime_end.tm_mday, sTime_end.tm_hour, sTime_end.tm_min, sTime_end.tm_sec);
	m_ReplayEndTimeCtrl.SetTime(&replay_end);
	UpdateData(FALSE);

	m_ReplayDurationTimeEdit = duration_timeStr;
	UpdateData(FALSE);

	CString item;
	m_CurRecChannelComboBox.GetWindowText(item);
	uint8_t chn[1] = { 0 };
	uint32_t num = atoi(CW2A(item.GetString()));
	chn[0] = num;
	int type = 15;

	UpdateData(TRUE);
	CTime replay_start_time, replay_end_time;

	m_ReplayStartTimeCtrl.GetTime(replay_start_time);
	m_ReplayEndTimeCtrl.GetTime(replay_end_time);

	m_ReplayStartTime = replay_start_time.GetTime();
	m_ReplayEndTime = replay_end_time.GetTime();

	m_ReplayRecInfoShowListCtrl.DeleteAllItems();
	m_ReplayRecInfoShowListCtrl.RedrawWindow();

	search_h = kp2p_rec_find_file_start(m_Parent->m_Handle, sizeof(chn) / sizeof(chn[0]), chn, m_ReplayStartTime, m_ReplayEndTime, type);
	if (NULL == search_h) {
		MessageBox(_T("播放录像失败，找不到录像文件"), _T("信息提示"), MB_OK);
		//kp2p_rec_find_file_stop(search_h);
		return;
	}
	while ((pRecords = kp2p_rec_find_file_next(search_h)) && pRecords->records_cnt > 0) {

		/*play_file_r.begin_time = pRecords->records[pRecords->records_cnt - 1].begin_time;
		play_file_r.end_time = pRecords->records[pRecords->records_cnt - 1].end_time;
		play_file_r.channel = pRecords->records[pRecords->records_cnt - 1].channel;
		play_file_r.type = pRecords->records[pRecords->records_cnt - 1].type;*/

		play_file_r.begin_time = pRecords->records[0].begin_time;
		play_file_r.end_time = pRecords->records[0].end_time;
		play_file_r.channel = pRecords->records[0].channel;
		play_file_r.type = pRecords->records[0].type;

		ssize_t rec_count = kp2p_rec_find_file_count(search_h);

		break;
	}

	if (!pRecords) {
		MessageBox(_T("找不到录像文件"), _T("信息提示"), MB_OK);
		kp2p_rec_find_file_stop(search_h);
		return;
	}
	kp2p_rec_find_file_stop(search_h);


#if 0
	search_h = kp2p_rec_find_file_start(m_Parent->m_Handle, sizeof(chn) / sizeof(chn[0]), chn, /*starttime, endtime*/m_ReplayStartTime, m_ReplayEndTime, type);
	while ((pRecords = kp2p_rec_find_file_next(search_h))) {
		play_file_r.begin_time = pRecords->records[0].begin_time;
		play_file_r.end_time = pRecords->records[0].end_time;
		play_file_r.channel = pRecords->records[0].channel;
		play_file_r.type = pRecords->records[0].type;

		if (pRecords && pRecords->records_cnt <= 0) {
			break;
		}
		/*int i = 0;
		for (i = 0; i < pRecords->records_cnt; i++) {
		printf("kp2p_rec_find_file[%d] ch:%d type:%d %d - %d\n", i,
		pRecords->records[i].channel, pRecords->records[i].type,
		pRecords->records[i].begin_time, pRecords->records[i].end_time);
		}*/
	}
	kp2p_rec_find_file_stop(search_h);
#endif

	pReplayDurationEdit->EnableWindow(FALSE);

	m_ReplayProgressSliderCtrl.SetRange(0, atoi(CW2A(m_ReplayDurationTimeEdit.GetString())));
	m_ReplayProgressSliderCtrl.SetPos(0);

	m_ReplayRecInfoShowListCtrl.InsertItem(0, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(0, 0, _T("视频信息："));
	m_ReplayRecInfoShowListCtrl.InsertItem(1, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(1, 0, _T("分辨率"));
	m_ReplayRecInfoShowListCtrl.InsertItem(2, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(2, 0, _T("编码类型"));

	m_ReplayRecInfoShowListCtrl.InsertItem(3, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(3, 0, _T(""));
	m_ReplayRecInfoShowListCtrl.InsertItem(4, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(4, 0, _T("音频信息："));
	m_ReplayRecInfoShowListCtrl.InsertItem(5, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(5, 0, _T("音频采样率"));
	m_ReplayRecInfoShowListCtrl.InsertItem(6, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(6, 0, _T("音频采样位数"));
	m_ReplayRecInfoShowListCtrl.InsertItem(7, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(7, 0, _T("音频采样声道数"));
	m_ReplayRecInfoShowListCtrl.InsertItem(8, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(8, 0, _T("比特率"));
	m_ReplayRecInfoShowListCtrl.InsertItem(9, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(9, 0, _T("编码类型"));

	m_ReplayRecInfoShowListCtrl.InsertItem(10, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(10, 0, _T(""));
	m_ReplayRecInfoShowListCtrl.InsertItem(11, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(11, 0, _T("统计信息:"));
	m_ReplayRecInfoShowListCtrl.InsertItem(12, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(12, 0, _T("第一个I帧抵达时间"));
	m_ReplayRecInfoShowListCtrl.InsertItem(13, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(13, 0, _T("码流字节数"));
	m_ReplayRecInfoShowListCtrl.InsertItem(14, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(14, 0, _T("音频帧数"));
	m_ReplayRecInfoShowListCtrl.InsertItem(15, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(15, 0, _T("视频帧数"));
	m_ReplayRecInfoShowListCtrl.InsertItem(16, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(16, 0, _T("帧率"));
	m_ReplayRecInfoShowListCtrl.InsertItem(17, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(17, 0, _T("平均带宽"));
	m_ReplayRecInfoShowListCtrl.InsertItem(18, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(18, 0, _T("播放时长"));

	//m_ReplayRecInfoShowListCtrl.RedrawWindow();
	m_OnReplayVedioCountLock = 0;
	m_OnReplayAudioCountLock = 0;
	::InterlockedIncrement(&m_OnReplayVedioCountLock);
	::InterlockedIncrement(&m_OnReplayAudioCountLock);


	chn[0] = play_file_r.channel;
	m_GetIFrameStartTime = GetTickCount();
	play_h = kp2p_rec_play_start(m_Parent->m_Handle, 1, chn, play_file_r.begin_time, play_file_r.end_time, play_file_r.type);
	SetTimer(1, 990, NULL);
	SetEvent(m_ReplayRecTimerStartHandle);

	
	(CButton *)GetDlgItem(IDC_STOP_VEDIO_BUTTON)->EnableWindow(TRUE);
	//m_StopRecBtn->EnableWindow(TRUE);
	(CButton *)GetDlgItem(IDC_PLAY_VEDIO_BUTTON)->EnableWindow(FALSE);
	//m_PlayRecBtn->EnableWindow(FALSE);
	(CEdit *)GetDlgItem(IDC_PLAY_KEEP_TIME_VEDIO_EDIT)->EnableWindow(FALSE);
	(CDateTimeCtrl*)GetDlgItem(IDC_PLAY_START_VEDIO_DATETIMEPICKER)->EnableWindow(FALSE);
	(CDateTimeCtrl*)GetDlgItem(IDC_PLAY_END_VEDIO_DATETIMEPICKER)->EnableWindow(FALSE);
	m_CurRecChannelComboBox.EnableWindow(FALSE);

	*pResult = 0;
}


int CKvideoDlg::replay_maketime(time_t *start_time, time_t *end_time)
{
	struct tm tmpTm;
	time_t    tmpTime = time(NULL);

	gmtime_r(&tmpTm, &tmpTime);

	tmpTm.tm_hour = 0;
	tmpTm.tm_min = 0;
	tmpTm.tm_sec = 0;
	*start_time = timegm(&tmpTm);

	tmpTm.tm_hour = 23;
	tmpTm.tm_min = 59;
	tmpTm.tm_sec = 59;
	*end_time = timegm(&tmpTm);
	return 0;
}


void CKvideoDlg::OnBnClickedPlayVedioButton()
{
	// TODO: 在此添加控件通知处理程序代码

	kp2p_rec_search_handle_t   search_h;
	kp2p_rec_files_t           *pRecords;
	//kp2p_rec_playback_handle_t play_h;
	kp2p_rec_file_t            play_file_r = { 0 };

	//uint8_t *chn = (uint8_t*)malloc(1 * sizeof(uint8_t));
	CString item;
	m_CurRecChannelComboBox.GetWindowText(item);
	//memcpy(chn, CW2A(item.GetString()), item.GetLength());
	uint8_t chn[1] = { 0 };
	uint32_t num = atoi(CW2A(item.GetString()));
	chn[0] = num;

	

	/*time_t  starttime, endtime;
	replay_maketime(&starttime, &endtime);*/
	int type = 15;

	UpdateData(TRUE);
	CTime replay_start_time, replay_end_time;

	m_ReplayStartTimeCtrl.GetTime(replay_start_time);
	m_ReplayEndTimeCtrl.GetTime(replay_end_time);

	m_ReplayStartTime = replay_start_time.GetTime();
	m_ReplayEndTime = replay_end_time.GetTime();

	if (m_ReplayStartTime == m_ReplayEndTime) {
		MessageBox(_T("开始时间与结束时间相同，请重新设置"), _T("信息提示"), MB_OK);
		m_ReplayStartTimeCtrl.SetFocus();
		/*free(chn);
		chn = NULL;*/
		return;
	}

	if (atol(CW2A(m_ReplayDurationTimeEdit.GetString())) > 1000) {
		MessageBox(_T("播放时长大于1000秒，请重新设置"), _T("信息提示"), MB_OK);
		pReplayDurationEdit->SetFocus();
		/*free(chn);
		chn = NULL;*/
		return;
	}

	m_ReplayRecInfoShowListCtrl.DeleteAllItems();
	m_ReplayRecInfoShowListCtrl.RedrawWindow();

	search_h = kp2p_rec_find_file_start(m_Parent->m_Handle, sizeof(chn) / sizeof(chn[0]), chn, m_ReplayStartTime, m_ReplayEndTime, type);
	if (NULL == search_h) {
		MessageBox(_T("播放录像失败，找不到录像文件"), _T("信息提示"), MB_OK);
		//kp2p_rec_find_file_stop(search_h);
		return;
	}
	while ((pRecords = kp2p_rec_find_file_next(search_h)) && pRecords->records_cnt > 0) {

		/*play_file_r.begin_time = pRecords->records[pRecords->records_cnt - 1].begin_time;
		play_file_r.end_time = pRecords->records[pRecords->records_cnt - 1].end_time;
		play_file_r.channel = pRecords->records[pRecords->records_cnt - 1].channel;
		play_file_r.type = pRecords->records[pRecords->records_cnt - 1].type;*/

		play_file_r.begin_time = pRecords->records[0].begin_time;
		play_file_r.end_time = pRecords->records[0].end_time;
		play_file_r.channel = pRecords->records[0].channel;
		play_file_r.type = pRecords->records[0].type;

		ssize_t rec_count = kp2p_rec_find_file_count(search_h);

		break;
	}
	//kp2p_rec_find_file_stop(search_h);

	if (!pRecords || ((0 == play_file_r.begin_time) && (0 == play_file_r.end_time))) {
		MessageBox(_T("播放录像失败，找不到录像文件"), _T("信息提示"), MB_OK);
		kp2p_rec_find_file_stop(search_h);
		return;
	}

	//if (!pRecords) {
	//	MessageBox(_T("找不到录像文件"), _T("信息提示"), MB_OK);
	//	kp2p_rec_find_file_stop(search_h);
	//	/*free(chn);
	//	chn = NULL;*/
	//	return;
	//}
	kp2p_rec_find_file_stop(search_h);


#if 0
	search_h = kp2p_rec_find_file_start(m_Parent->m_Handle, sizeof(chn) / sizeof(chn[0]), chn, /*starttime, endtime*/m_ReplayStartTime, m_ReplayEndTime, type);
	while ((pRecords = kp2p_rec_find_file_next(search_h))) {
		play_file_r.begin_time = pRecords->records[0].begin_time;
		play_file_r.end_time = pRecords->records[0].end_time;
		play_file_r.channel = pRecords->records[0].channel;
		play_file_r.type = pRecords->records[0].type;

		if (pRecords && pRecords->records_cnt <= 0) {
			break;
		}
		/*int i = 0;
		for (i = 0; i < pRecords->records_cnt; i++) {
			printf("kp2p_rec_find_file[%d] ch:%d type:%d %d - %d\n", i,
				pRecords->records[i].channel, pRecords->records[i].type,
				pRecords->records[i].begin_time, pRecords->records[i].end_time);
		}*/
	}
	kp2p_rec_find_file_stop(search_h);
#endif

	double cost = difftime(play_file_r.end_time, play_file_r.begin_time);
	CString time_duration;
	time_duration.Format(_T("%.0f"), cost);
	long rec_time = atol(CW2A(m_ReplayDurationTimeEdit.GetString()));
	if (rec_time > (long)cost) {
		m_ReplayDurationTimeEdit = time_duration;
		UpdateData(FALSE);
	}

	//pReplayDurationEdit->EnableWindow(FALSE);

	m_ReplayProgressSliderCtrl.SetRange(0, atoi(CW2A(m_ReplayDurationTimeEdit.GetString())));
	m_ReplayProgressSliderCtrl.SetPos(0);

	m_ReplayRecInfoShowListCtrl.InsertItem(0, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(0, 0, _T("视频信息："));
	m_ReplayRecInfoShowListCtrl.InsertItem(1, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(1, 0, _T("分辨率"));
	m_ReplayRecInfoShowListCtrl.InsertItem(2, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(2, 0, _T("编码类型"));

	m_ReplayRecInfoShowListCtrl.InsertItem(3, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(3, 0, _T(""));
	m_ReplayRecInfoShowListCtrl.InsertItem(4, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(4, 0, _T("音频信息："));
	m_ReplayRecInfoShowListCtrl.InsertItem(5, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(5, 0, _T("音频采样率"));
	m_ReplayRecInfoShowListCtrl.InsertItem(6, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(6, 0, _T("音频采样位数"));
	m_ReplayRecInfoShowListCtrl.InsertItem(7, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(7, 0, _T("音频采样声道数"));
	m_ReplayRecInfoShowListCtrl.InsertItem(8, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(8, 0, _T("比特率"));
	m_ReplayRecInfoShowListCtrl.InsertItem(9, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(9, 0, _T("编码类型"));

	m_ReplayRecInfoShowListCtrl.InsertItem(10, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(10, 0, _T(""));
	m_ReplayRecInfoShowListCtrl.InsertItem(11, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(11, 0, _T("统计信息:"));
	m_ReplayRecInfoShowListCtrl.InsertItem(12, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(12, 0, _T("第一个I帧抵达时间"));
	m_ReplayRecInfoShowListCtrl.InsertItem(13, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(13, 0, _T("码流字节数"));
	m_ReplayRecInfoShowListCtrl.InsertItem(14, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(14, 0, _T("音频帧数"));
	m_ReplayRecInfoShowListCtrl.InsertItem(15, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(15, 0, _T("视频帧数"));
	m_ReplayRecInfoShowListCtrl.InsertItem(16, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(16, 0, _T("帧率"));
	m_ReplayRecInfoShowListCtrl.InsertItem(17, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(17, 0, _T("平均带宽"));
	m_ReplayRecInfoShowListCtrl.InsertItem(18, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(18, 0, _T("播放时长"));

	//m_ReplayRecInfoShowListCtrl.RedrawWindow();
	m_OnReplayVedioCountLock = 0;
	m_OnReplayAudioCountLock = 0;
	::InterlockedIncrement(&m_OnReplayVedioCountLock);
	::InterlockedIncrement(&m_OnReplayAudioCountLock);

	
	chn[0] = play_file_r.channel;
	m_GetIFrameStartTime = GetTickCount();
	play_h = kp2p_rec_play_start(m_Parent->m_Handle, 1, chn, play_file_r.begin_time, play_file_r.end_time, play_file_r.type);
	SetTimer(1, 990, NULL);
	SetEvent(m_ReplayRecTimerStartHandle);
	
	(CEdit *)GetDlgItem(IDC_PLAY_KEEP_TIME_VEDIO_EDIT)->EnableWindow(FALSE);
	(CButton *)GetDlgItem(IDC_STOP_VEDIO_BUTTON)->EnableWindow(TRUE);
	//m_StopRecBtn->EnableWindow(TRUE);
	(CButton *)GetDlgItem(IDC_PLAY_VEDIO_BUTTON)->EnableWindow(FALSE);
	//m_PlayRecBtn->EnableWindow(FALSE);
	m_CurRecChannelComboBox.EnableWindow(FALSE);
	(CDateTimeCtrl*)GetDlgItem(IDC_PLAY_START_VEDIO_DATETIMEPICKER)->EnableWindow(FALSE);
	(CDateTimeCtrl*)GetDlgItem(IDC_PLAY_END_VEDIO_DATETIMEPICKER)->EnableWindow(FALSE);

}


void CKvideoDlg::OnBnClickedStopVediobutton()
{
	// TODO: 在此添加控件通知处理程序代码

	SetEvent(m_ReplayRecTimeOutNotifyHandle);
#if 0
	/*kp2p_rec_play_stop(play_h);

	m_ReplayRecInfoShowListCtrl.InsertItem(17, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(17, 0, _T(""));
	m_ReplayRecInfoShowListCtrl.InsertItem(18, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(18, 0, _T("统计信息："));
	m_ReplayRecInfoShowListCtrl.InsertItem(19, _T(""));
	m_ReplayRecInfoShowListCtrl.SetItemText(19, 0, _T("播放结束"));

	pReplayDurationEdit->EnableWindow(TRUE);
	m_StopRecBtn->EnableWindow(FALSE);*/
#endif
}

unsigned int __stdcall CKvideoDlg::ReplayProgressWorkThreadProc(PVOID arg)
{
	BOOL bExit = FALSE;
	DWORD nRet;

	CKvideoDlg *p = (CKvideoDlg*)arg;

	while (!bExit)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(p->m_ReplayProgressExitHandle, 0))
			bExit = TRUE;

		if ((nRet = WaitForSingleObject(p->m_ReplayProgressStartHandle, 0)) != WAIT_OBJECT_0) {
			msleep_c(50);
			continue;
		}

	}

	return 0;
}

unsigned int __stdcall CKvideoDlg::ReplayRecTimerWorkThreadProc(PVOID arg)
{
	BOOL bExit = FALSE;
	DWORD nRet;
	HANDLE h[3];

	CKvideoDlg *p = (CKvideoDlg*)arg;

	h[0] = p->m_Parent->m_DevOfflineNotifyEvent;
	h[1] = p->m_ReplayRecTimeOutNotifyHandle;
	h[2] = p->m_ReplayRecTimerQuitNotifyHandle;

	while (!bExit)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(p->m_ReplayRecTimerExitHandle, 0))
			bExit = TRUE;

		if ((nRet = WaitForSingleObject(p->m_ReplayRecTimerStartHandle, 0)) != WAIT_OBJECT_0) {
			msleep_c(50);
			continue;
		}

		::InterlockedIncrement(&m_OnReplayRecCountLock);

		DWORD tm = atol(CW2A(p->m_ReplayDurationTimeEdit.GetString())) * 1000;
		nRet = WaitForMultipleObjects(3, h, FALSE, tm);
		switch (nRet)
		{
		case WAIT_FAILED:
			break;
		case WAIT_TIMEOUT:
			kp2p_rec_play_stop(p->play_h);

			p->m_ReplayRecInfoShowListCtrl.InsertItem(19, _T(""));
			p->m_ReplayRecInfoShowListCtrl.SetItemText(19, 0, _T(""));
			p->m_ReplayRecInfoShowListCtrl.InsertItem(20, _T(""));
			p->m_ReplayRecInfoShowListCtrl.SetItemText(20, 0, _T(""));
			p->m_ReplayRecInfoShowListCtrl.InsertItem(21, _T(""));
			p->m_ReplayRecInfoShowListCtrl.SetItemText(21, 0, _T(""));
			p->m_ReplayRecInfoShowListCtrl.InsertItem(22, _T(""));
			p->m_ReplayRecInfoShowListCtrl.SetItemText(22, 0, _T(""));
			p->m_ReplayRecInfoShowListCtrl.InsertItem(23, _T(""));
			p->m_ReplayRecInfoShowListCtrl.SetItemText(23, 0, _T("播放结束"));

			p->pReplayDurationEdit->EnableWindow(TRUE);
			(CButton *)p->GetDlgItem(IDC_STOP_VEDIO_BUTTON)->EnableWindow(FALSE);
			//p->m_StopRecBtn->EnableWindow(FALSE);
			(CButton *)p->GetDlgItem(IDC_PLAY_VEDIO_BUTTON)->EnableWindow(TRUE);
			//p->m_PlayRecBtn->EnableWindow(TRUE);
			(CEdit *)p->GetDlgItem(IDC_PLAY_KEEP_TIME_VEDIO_EDIT)->EnableWindow(TRUE);
			(CDateTimeCtrl*)p->GetDlgItem(IDC_PLAY_START_VEDIO_DATETIMEPICKER)->EnableWindow(TRUE);
			(CDateTimeCtrl*)p->GetDlgItem(IDC_PLAY_END_VEDIO_DATETIMEPICKER)->EnableWindow(TRUE);
			p->m_CurRecChannelComboBox.EnableWindow(TRUE);
			p->KillTimer(1);
			m_FrameCountTotal = 0;
			m_VedioFrameNum = 0;
			m_AudioFrameNum = 0;
			m_CurReplayRecDuraTime = 0;
			break;
		case WAIT_OBJECT_0 + 0:
			kp2p_rec_play_stop(p->play_h);

			p->m_ReplayRecInfoShowListCtrl.InsertItem(19, _T(""));
			p->m_ReplayRecInfoShowListCtrl.SetItemText(19, 0, _T(""));
			p->m_ReplayRecInfoShowListCtrl.InsertItem(20, _T(""));
			p->m_ReplayRecInfoShowListCtrl.SetItemText(20, 0, _T(""));
			p->m_ReplayRecInfoShowListCtrl.InsertItem(21, _T(""));
			p->m_ReplayRecInfoShowListCtrl.SetItemText(21, 0, _T(""));
			p->m_ReplayRecInfoShowListCtrl.InsertItem(22, _T(""));
			p->m_ReplayRecInfoShowListCtrl.SetItemText(22, 0, _T(""));
			p->m_ReplayRecInfoShowListCtrl.InsertItem(23, _T(""));
			p->m_ReplayRecInfoShowListCtrl.SetItemText(23, 0, _T("播放结束"));

			p->pReplayDurationEdit->EnableWindow(TRUE);
			(CButton *)p->GetDlgItem(IDC_STOP_VEDIO_BUTTON)->EnableWindow(FALSE);
			//p->m_StopRecBtn->EnableWindow(FALSE);
			(CButton *)p->GetDlgItem(IDC_PLAY_VEDIO_BUTTON)->EnableWindow(FALSE);
			//p->m_PlayRecBtn->EnableWindow(FALSE);
			(CButton *)p->GetDlgItem(IDC_QUERY_VEDIO_BUTTON)->EnableWindow(FALSE);
			//p->m_QueryRecBtn->EnableWindow(FALSE);
			/*(CEdit *)p->GetDlgItem(IDC_PLAY_KEEP_TIME_VEDIO_EDIT)->EnableWindow(TRUE);
			(CDateTimeCtrl*)p->GetDlgItem(IDC_PLAY_START_VEDIO_DATETIMEPICKER)->EnableWindow(TRUE);
			(CDateTimeCtrl*)p->GetDlgItem(IDC_PLAY_END_VEDIO_DATETIMEPICKER)->EnableWindow(TRUE);*/
			p->KillTimer(1);
			m_FrameCountTotal = 0;
			m_VedioFrameNum = 0;
			m_AudioFrameNum = 0;
			m_CurReplayRecDuraTime = 0;
			p->MessageBox(_T("设备连接断开，录像回放测试中止"), _T("信息提示"), MB_OK);
			break;
		case WAIT_OBJECT_0 + 1:
			kp2p_rec_play_stop(p->play_h);

			p->m_ReplayRecInfoShowListCtrl.InsertItem(19, _T(""));
			p->m_ReplayRecInfoShowListCtrl.SetItemText(19, 0, _T(""));
			p->m_ReplayRecInfoShowListCtrl.InsertItem(20, _T(""));
			p->m_ReplayRecInfoShowListCtrl.SetItemText(20, 0, _T(""));
			p->m_ReplayRecInfoShowListCtrl.InsertItem(21, _T(""));
			p->m_ReplayRecInfoShowListCtrl.SetItemText(21, 0, _T(""));
			p->m_ReplayRecInfoShowListCtrl.InsertItem(22, _T(""));
			p->m_ReplayRecInfoShowListCtrl.SetItemText(22, 0, _T(""));
			p->m_ReplayRecInfoShowListCtrl.InsertItem(23, _T(""));
			p->m_ReplayRecInfoShowListCtrl.SetItemText(23, 0, _T("播放结束"));

			p->pReplayDurationEdit->EnableWindow(TRUE);
			(CButton *)p->GetDlgItem(IDC_STOP_VEDIO_BUTTON)->EnableWindow(FALSE);
			//p->m_StopRecBtn->EnableWindow(FALSE);
			(CButton *)p->GetDlgItem(IDC_PLAY_VEDIO_BUTTON)->EnableWindow(TRUE);
			//p->m_PlayRecBtn->EnableWindow(TRUE);
			(CEdit *)p->GetDlgItem(IDC_PLAY_KEEP_TIME_VEDIO_EDIT)->EnableWindow(TRUE);
			(CDateTimeCtrl*)p->GetDlgItem(IDC_PLAY_START_VEDIO_DATETIMEPICKER)->EnableWindow(TRUE);
			(CDateTimeCtrl*)p->GetDlgItem(IDC_PLAY_END_VEDIO_DATETIMEPICKER)->EnableWindow(TRUE);
			p->m_CurRecChannelComboBox.EnableWindow(TRUE);
			p->KillTimer(1);
			m_FrameCountTotal = 0;
			m_VedioFrameNum = 0;
			m_AudioFrameNum = 0;
			m_CurReplayRecDuraTime = 0;
			break;
		case WAIT_OBJECT_0 + 2:
			kp2p_rec_play_stop(p->play_h);

			p->KillTimer(1);
			m_FrameCountTotal = 0;
			m_VedioFrameNum = 0;
			m_AudioFrameNum = 0;
			m_CurReplayRecDuraTime = 0;
			break;
		}
		::InterlockedDecrement(&m_OnReplayRecCountLock);

	}

	return 0;
}

void CKvideoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString temp;
	switch (nIDEvent) {
	case 1:
		m_CurReplayRecDuraTime += 1;
		temp.Format(_T("%ld"), m_FrameCountTotal);
		m_ReplayRecInfoShowListCtrl.SetItemText(13, 1, temp.GetString());
		temp.Format(_T("%ld"), m_AudioFrameNum);
		m_ReplayRecInfoShowListCtrl.SetItemText(14, 1, temp.GetString());
		temp.Format(_T("%ld"), m_VedioFrameNum);
		m_ReplayRecInfoShowListCtrl.SetItemText(15, 1, temp.GetString());
		temp.Format(_T("%ld fps"), m_VedioFrameNum / m_CurReplayRecDuraTime);
		m_ReplayRecInfoShowListCtrl.SetItemText(16, 1, temp.GetString());
		temp.Format(_T("%ld byte/s"), m_FrameCountTotal / m_CurReplayRecDuraTime);
		m_ReplayRecInfoShowListCtrl.SetItemText(17, 1, temp.GetString());
		temp.Format(_T("%ld 秒"), m_CurReplayRecDuraTime);
		m_ReplayRecInfoShowListCtrl.SetItemText(18, 1, temp.GetString());
		break;
	case 2:
		break;
	}
	m_ReplayProgressSliderCtrl.SetPos(m_CurReplayRecDuraTime);
	CDialogEx::OnTimer(nIDEvent);
}