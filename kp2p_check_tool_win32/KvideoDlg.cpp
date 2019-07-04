// Kvideo.cpp : 实现文件
//

#include "stdafx.h"
#include "kp2p_check_tool_win32.h"
#include "kp2p_check_tool_win32Dlg.h"
#include "KvideoDlg.h"
#include "afxdialogex.h"
#include "Util.h"


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

	DDX_Control(pDX, IDC_VEDIO_FILE_LIST, m_RecFileInfoListCtrl);

	DDX_Control(pDX, IDC_DATE_VEDIO_DATETIMEPICKER, m_QueryDateTimeCtrl);
	DDX_Control(pDX, IDC_START_TIME_VEDIO_DATETIMEPICKER, m_QueryStartTimeCtrl);
	DDX_Control(pDX, IDC_END_TIME_VEDIO_DATETIMEPICKER, m_QueryEndTimeCtrl);

	//DDX_Text(pDX, IDC_START_TIME_VEDIO_DATETIMEPICKER, m_QueryStartTimeText);
	//DDX_Text(pDX, IDC_START_TIME_VEDIO_DATETIMEPICKER, m_QueryEndTimeText);

}


BEGIN_MESSAGE_MAP(CKvideoDlg, CDialogEx)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER3, &CKvideoDlg::OnDtnDatetimechangeDatetimepicker3)
	ON_BN_CLICKED(IDC_QUERY_VEDIO_BUTTON, &CKvideoDlg::OnBnClickedQueryVedioButton)
	ON_BN_CLICKED(IDC_RESET_VIDEO_BUTTON, &CKvideoDlg::OnBnClickedResetVideoButton)
	ON_BN_CLICKED(IDOK, &CKvideoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_PLAY_VEDIO_BUTTON, &CKvideoDlg::OnBnClickedPlayVedioButton)
END_MESSAGE_MAP()


// CKvideo 消息处理程序


BOOL CKvideoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//CString  mTime;
	//mTime = "0:0:0";        //自设时间
	//COleDateTime        dt;
	//dt.ParseDateTime(mTime);        //将mTime的时间值存到dt中
	//SYSTEMTIME        st;        //定义一个系统时间类型的变量
	//dt.GetAsSystemTime(st);        //将dt中的时间按系统时间格式化

	//CTime        tm(st);        //定义CTime对象并将st赋值给它
	//tm.Format("%H%H:%M%M:%S%S");
	//m_QueryStartTimeCtrl.SetTime(&tm);        //控件会自动获取其需要的时间部分

	m_RecFileInfoListCtrl.InsertColumn(0, TEXT("ID"), 0, 50);
	m_RecFileInfoListCtrl.InsertColumn(1, TEXT("channel"), 0, 50);
	m_RecFileInfoListCtrl.InsertColumn(2, TEXT("type"), 0, 50);
	m_RecFileInfoListCtrl.InsertColumn(3, TEXT("begin_time"), 0, 150);
	m_RecFileInfoListCtrl.InsertColumn(4, TEXT("end_time"), 0, 150);
	m_RecFileInfoListCtrl.InsertColumn(5, TEXT("quality"), 0, 50);
	/*m_RecFileInfoListCtrl.InsertItem(0, _T("2"));
	m_RecFileInfoListCtrl.SetItemText(0, 1, _T("OK"));
	m_RecFileInfoListCtrl.InsertItem(1, _T("3"));
	m_RecFileInfoListCtrl.SetItemText(1, 1, _T("FAIL"));*/

	m_RecFileInfoListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);

	/*int m_nCurrentSel = m_RecFileInfoListCtrl.GetSelectedCount();
	if (m_nCurrentSel >0)*/

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

	//查找
	//FindRecFile(m_Parent->m_Handle);
	//检索
	SearchRecFile(m_Parent->m_Handle);

}


void CKvideoDlg::OnBnClickedResetVideoButton()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CKvideoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

void CKvideoDlg::SetParent(PVOID arg)
{
	m_Parent = (Ckp2p_check_tool_win32Dlg*)arg;
}

void CKvideoDlg::SearchRecFile(PVOID arg)
{
	kp2p_handle_t			    handle = (kp2p_handle_t)arg;
	kp2p_rec_search_handle_t    search_h;
	kp2p_rec_files_t            *pRecords = NULL;
	//kp2p_rec_playback_handle_t  play_h;
	//kp2p_rec_file_t             play_file_r = { 0 };
	
	uint8_t chn[1] = { 0 };
	time_t  start_time, end_time;
	//replay_maketime(&starttime, &endtime);
	int type = 15;

	CTime query_date, query_start_time, query_end_time;
	m_QueryDateTimeCtrl.GetTime(query_date);
	m_QueryStartTimeCtrl.GetTime(query_start_time);
	m_QueryEndTimeCtrl.GetTime(query_end_time);

	start_time = query_start_time.GetTime();
	end_time = query_end_time.GetTime();

	uint32_t i = 0;
	search_h = kp2p_rec_find_file_start(m_Parent->m_Handle, sizeof(chn) / sizeof(chn[0]), chn, start_time, end_time, type);
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
		return;
	}

	i = 0;
	CString idStr, channelStr, typeStr, begin_timeStr, end_timeStr, qualityStr;
	for (i = 0; i < pRecords->records_cnt; i++) {
		idStr.Format(L"%d", i + 1);
		m_RecFileInfoListCtrl.InsertItem(i, idStr.GetString());
		channelStr.Format(L"%d", pRecords->records[i].channel);
		m_RecFileInfoListCtrl.SetItemText(i, 1, channelStr.GetString());
		typeStr.Format(L"%d", pRecords->records[i].type);
		m_RecFileInfoListCtrl.SetItemText(i, 2, typeStr.GetString());
		begin_timeStr.Format(L"%d", pRecords->records[i].begin_time);
		m_RecFileInfoListCtrl.SetItemText(i, 3, begin_timeStr.GetString());
		end_timeStr.Format(L"%d", pRecords->records[i].end_time);
		m_RecFileInfoListCtrl.SetItemText(i, 4, end_timeStr.GetString());
		qualityStr.Format(L"%d", pRecords->records[i].quality);
		m_RecFileInfoListCtrl.SetItemText(i, 5, qualityStr.GetString());

		/*printf("kp2p_rec_find_file[%d] ch:%d type:%d %d - %d\n", i,
			pRecords->records[i].channel, pRecords->records[i].type,
			pRecords->records[i].begin_time, pRecords->records[i].end_time);*/
	}
	kp2p_rec_find_file_stop(search_h);

	/*chn[0] = play_file_r.channel;
	play_h = kp2p_rec_play_start(handle, 1, chn, play_file_r.begin_time, play_file_r.end_time,
		play_file_r.type);
	msleep_c(8 * 1000);

	kp2p_rec_play_stop(play_h);*/

	return;
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
	cond.start_time.hour = query_start_time.GetHour();
	cond.start_time.minute = query_start_time.GetMinute();
	cond.start_time.second = query_start_time.GetSecond();
	cond.stop_time.year = query_date.GetYear();
	cond.stop_time.month = query_date.GetMonth();
	cond.stop_time.day = query_date.GetDay();
	cond.stop_time.hour = query_end_time.GetHour();
	cond.stop_time.minute = query_end_time.GetMinute();
	cond.stop_time.second = query_end_time.GetSecond();
	cond.type = 0;
	find_handle = kp2p_find_file(handle, &cond, 5);
	if (!find_handle) {
		return;
	}

	while (kp2p_find_next_file(find_handle, &find_data, 5) != -1) {
		//printf("file_size(%d)\n", find_data.file_size);
	}

	kp2p_find_stop(find_handle, 5);
	return;
}



void CKvideoDlg::OnBnClickedPlayVedioButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CString idStr, channelStr, typeStr, begin_timeStr, end_timeStr, qualityStr;
	int m_nCurrentSel = m_RecFileInfoListCtrl.GetSelectedCount();
	//m_RecFileInfoListCtrl.SetItemState(m_nCurrentSel, LVIS_SELECTED | LVIS_FOCUSED,  LVIS_SELECTED | LVIS_FOCUSED);
	if (m_nCurrentSel >= 0) {
		idStr = m_RecFileInfoListCtrl.GetItemText(m_nCurrentSel, 0);
		channelStr = m_RecFileInfoListCtrl.GetItemText(m_nCurrentSel, 1);
		typeStr = m_RecFileInfoListCtrl.GetItemText(m_nCurrentSel, 2);
		begin_timeStr = m_RecFileInfoListCtrl.GetItemText(m_nCurrentSel, 3);
		end_timeStr = m_RecFileInfoListCtrl.GetItemText(m_nCurrentSel, 4);
		qualityStr = m_RecFileInfoListCtrl.GetItemText(m_nCurrentSel, 5);
	}

}
