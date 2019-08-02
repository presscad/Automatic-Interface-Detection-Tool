// Kvideo.cpp : 实现文件
//

#include "stdafx.h"
#include "kp2p_check_tool_win32.h"
#include "kp2p_check_tool_win32Dlg.h"
#include "KvideoDlg.h"
#include "afxdialogex.h"
#include "Util.h"


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
	
	DDX_Control(pDX, IDC_REPLAY_PROGRESS_VEDIO_SLIDER, m_ReplayProgressSliderCtrl);

	DDX_Control(pDX, IDC_DATE_VEDIO_DATETIMEPICKER, m_QueryDateTimeCtrl);
	DDX_Control(pDX, IDC_START_TIME_VEDIO_DATETIMEPICKER, m_QueryStartTimeCtrl);
	DDX_Control(pDX, IDC_END_TIME_VEDIO_DATETIMEPICKER, m_QueryEndTimeCtrl);
	DDX_Control(pDX, IDC_PLAY_START_VEDIO_DATETIMEPICKER, m_ReplayStartTimeCtrl);
	DDX_Control(pDX, IDC_PLAY_END_VEDIO_DATETIMEPICKER, m_ReplayEndTimeCtrl);

}


BEGIN_MESSAGE_MAP(CKvideoDlg, CDialogEx)
	ON_WM_TIMER()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER3, &CKvideoDlg::OnDtnDatetimechangeDatetimepicker3)
	ON_BN_CLICKED(IDC_QUERY_VEDIO_BUTTON, &CKvideoDlg::OnBnClickedQueryVedioButton)
	ON_BN_CLICKED(IDOK, &CKvideoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CKvideoDlg::OnBnClickedCancel)
	ON_NOTIFY(NM_DBLCLK, IDC_VEDIO_FILE_LIST, &CKvideoDlg::OnNMDblclkVedioFileList)
	ON_BN_CLICKED(IDC_PLAY_VEDIO_BUTTON, &CKvideoDlg::OnBnClickedPlayVedioButton)
	ON_BN_CLICKED(IDC_STOP_VEDIOBUTTON, &CKvideoDlg::OnBnClickedStopVediobutton)
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

	m_PlayRecBtn = NULL;
	m_PlayRecBtn = (CButton *)GetDlgItem(IDC_PLAY_VEDIO_BUTTON);
	m_PlayRecBtn->EnableWindow(FALSE);

	m_StopRecBtn = NULL;
	m_StopRecBtn = (CButton *)GetDlgItem(IDC_STOP_VEDIOBUTTON);
	m_StopRecBtn->EnableWindow(FALSE);

	pReplayDurationEdit = NULL;
	pReplayDurationEdit = (CEdit *)GetDlgItem(IDC_PLAY_KEEP_TIME_VEDIO_EDIT);

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

void CKvideoDlg::OnDtnDatetimechangeDatetimepicker3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CKvideoDlg::OnBnClickedQueryVedioButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	INT nRes;

	//查找
	//FindRecFile(m_Parent->m_Handle);
	//检索
	nRes = SearchRecFile(m_Parent->m_Handle);
	if (nRes == 0) {
		m_PlayRecBtn->EnableWindow(TRUE);
		std::map<uint32_t, CString>::iterator it = m_RecChannelMap.begin();
		for (; it != m_RecChannelMap.end(); it++) {
			CString ch;
			ch.Format(_T("%u"), it->first);
			m_CurRecChannelComboBox.AddString(ch.GetString());
			m_CurRecChannelComboBox.SetCurSel(0);
		}
	}

}


void CKvideoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	HANDLE hThread[1];
	hThread[0] = m_ReplayRecTimerHandleThr;

	SetEvent(m_ReplayRecTimerExitHandle);

	WaitForMultipleObjects(1, hThread, TRUE, INFINITE);
	for (int i = 0; i < (sizeof(hThread) / sizeof(hThread[0])); i++) {
		CloseHandle(hThread[i]);
	}

	CDialogEx::OnOK();
}

void CKvideoDlg::SetParent(PVOID arg)
{
	m_Parent = (Ckp2p_check_tool_win32Dlg*)arg;
}

INT CKvideoDlg::SearchRecFile(PVOID arg)
{
	kp2p_handle_t			    handle = (kp2p_handle_t)arg;
	kp2p_rec_search_handle_t    search_h;
	kp2p_rec_files_t            *pRecords = NULL;
	//kp2p_rec_playback_handle_t  play_h;
	//kp2p_rec_file_t             play_file_r = { 0 };
	
	uint8_t chn[1] = { 0 };
	//uint8_t chn[8] = { 0, 1, 2, 3, 4, 6, 127, 89 };
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
		return -1;
	}

	uint32_t i = 0;
	search_h = kp2p_rec_find_file_start(handle, sizeof(chn) / sizeof(chn[0]), chn, start_time, end_time, type);
	while ((pRecords = kp2p_rec_find_file_next(search_h)) && pRecords->records_cnt > 0) {

		/*play_file_r.begin_time = pRecords->records[pRecords->records_cnt - 1].begin_time;
		play_file_r.end_time = pRecords->records[pRecords->records_cnt - 1].end_time;
		play_file_r.channel = pRecords->records[pRecords->records_cnt - 1].channel;
		play_file_r.type = pRecords->records[pRecords->records_cnt - 1].type;*/

		ssize_t rec_count = kp2p_rec_find_file_count(search_h);

		break;
	}
	//kp2p_rec_find_file_stop(search_h);

	if (!pRecords) {
		MessageBox(_T("找不到录像文件"), _T("信息提示"), MB_OK);
		kp2p_rec_find_file_stop(search_h);
		return -1;
	}

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
	CDialogEx::OnCancel();
}


void CKvideoDlg::OnNMDblclkVedioFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//CString idStr, channelStr, typeStr, begin_timeStr, end_timeStr, qualityStr;
	//int m_nCurrentSel = m_RecFileInfoListCtrl.GetSelectedCount();
	////m_RecFileInfoListCtrl.SetItemState(m_nCurrentSel, LVIS_SELECTED | LVIS_FOCUSED,  LVIS_SELECTED | LVIS_FOCUSED);
	//if (m_nCurrentSel >= 0) {
	//	idStr = m_RecFileInfoListCtrl.GetItemText(m_nCurrentSel, 0);
	//	channelStr = m_RecFileInfoListCtrl.GetItemText(m_nCurrentSel, 1);
	//	typeStr = m_RecFileInfoListCtrl.GetItemText(m_nCurrentSel, 2);
	//	begin_timeStr = m_RecFileInfoListCtrl.GetItemText(m_nCurrentSel, 3);
	//	end_timeStr = m_RecFileInfoListCtrl.GetItemText(m_nCurrentSel, 4);
	//	qualityStr = m_RecFileInfoListCtrl.GetItemText(m_nCurrentSel, 5);
	//}

	POSITION p = m_RecFileInfoListCtrl.GetFirstSelectedItemPosition();
	if (p == NULL)
	{
		TRACE(_T("没有行被选中!"));
		*pResult = 0;
		return;
	}

	int index = m_RecFileInfoListCtrl.GetNextSelectedItem(p);
	CString channelStr = m_RecFileInfoListCtrl.GetItemText(index, 1);
	CString begin_timeStr = m_RecFileInfoListCtrl.GetItemText(index, 3);
	CString end_timeStr = m_RecFileInfoListCtrl.GetItemText(index, 4);

	/*INT_PTR nRes = m_DevInfoDetailDlg.DoModal();
	if (nRes == IDOK) {

	}*/

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

	//uint8_t chn[8] = { 0, 1, 2, 3, 4, 6, 127, 89 };
	uint8_t chn[1] = { 0 };
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
		return;
	}

	if (atol(CW2A(m_ReplayDurationTimeEdit.GetString())) > 1000) {
		MessageBox(_T("播放时长大于1000秒，请重新设置"), _T("信息提示"), MB_OK);
		pReplayDurationEdit->SetFocus();
		return;
	}

	m_ReplayRecInfoShowListCtrl.DeleteAllItems();
	m_ReplayRecInfoShowListCtrl.RedrawWindow();

	search_h = kp2p_rec_find_file_start(m_Parent->m_Handle, sizeof(chn) / sizeof(chn[0]), chn, m_ReplayStartTime, m_ReplayEndTime, type);
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

	if (!pRecords && 0 == play_file_r.begin_time) {
		MessageBox(_T("找不到录像文件"), _T("信息提示"), MB_OK);
		return;
	}
	double cost = difftime(play_file_r.end_time, play_file_r.begin_time);
	CString time_duration;
	time_duration.Format(_T("%.0f"), cost);
	long rec_time = atol(CW2A(m_ReplayDurationTimeEdit.GetString()));
	if (rec_time > (long)cost) {
		m_ReplayDurationTimeEdit = time_duration;
		UpdateData(FALSE);
	}

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
	::InterlockedIncrement(&m_OnReplayVedioCountLock);
	::InterlockedIncrement(&m_OnReplayAudioCountLock);

	
	chn[0] = play_file_r.channel;
	m_GetIFrameStartTime = GetTickCount();
	play_h = kp2p_rec_play_start(m_Parent->m_Handle, 1, chn, play_file_r.begin_time, play_file_r.end_time, play_file_r.type);
	SetTimer(1, 990, NULL);
	SetEvent(m_ReplayRecTimerStartHandle);
	
	m_StopRecBtn->EnableWindow(TRUE);
	//Sleep(10000);

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
	HANDLE h[2];

	CKvideoDlg *p = (CKvideoDlg*)arg;

	h[0] = p->m_Parent->m_CheckDevOfflineStatusThreadStartEvent;
	h[1] = p->m_ReplayRecTimeOutNotifyHandle;

	while (!bExit)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(p->m_ReplayRecTimerExitHandle, 0))
			bExit = TRUE;

		if ((nRet = WaitForSingleObject(p->m_ReplayRecTimerStartHandle, 0)) != WAIT_OBJECT_0) {
			msleep_c(50);
			continue;
		}
		DWORD tm = atol(CW2A(p->m_ReplayDurationTimeEdit.GetString())) * 1000;
		nRet = WaitForMultipleObjects(2, h, FALSE, tm);
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
			p->m_StopRecBtn->EnableWindow(FALSE);
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
			p->m_StopRecBtn->EnableWindow(FALSE);
			p->KillTimer(1);
			m_FrameCountTotal = 0;
			m_VedioFrameNum = 0;
			m_AudioFrameNum = 0;
			m_CurReplayRecDuraTime = 0;
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
			p->m_StopRecBtn->EnableWindow(FALSE);
			p->KillTimer(1);
			m_FrameCountTotal = 0;
			m_VedioFrameNum = 0;
			m_AudioFrameNum = 0;
			m_CurReplayRecDuraTime = 0;
			break;
		}

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
		temp.Format(_T("%ld fps"), (m_AudioFrameNum + m_VedioFrameNum) / m_CurReplayRecDuraTime);
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