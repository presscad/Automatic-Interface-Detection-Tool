#pragma once

#include <time.h>
#include <map>

// CKvideo 对话框

class Ckp2p_check_tool_win32Dlg;

class CKvideoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKvideoDlg)

public:
	CKvideoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKvideoDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VEDIO_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDtnDatetimechangeDatetimepicker3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedQueryVedioButton();
	afx_msg void OnBnClickedOk();

public:
	void SetParent(PVOID arg);

private:
	void FindRecFile(PVOID arg);
	INT SearchRecFile(PVOID arg);

public:
	CString       m_ReplayDurationTimeEdit;
	time_t        m_ReplayStartTime;
	time_t        m_ReplayEndTime;

	CDateTimeCtrl m_QueryDateTimeCtrl;
	CDateTimeCtrl m_QueryStartTimeCtrl;
	CDateTimeCtrl m_QueryEndTimeCtrl;
	CDateTimeCtrl m_ReplayStartTimeCtrl;
	CDateTimeCtrl m_ReplayEndTimeCtrl;

	CEdit         *pReplayDurationEdit;
	CButton       *m_QueryRecBtn;
	CButton       *m_PlayRecBtn;
	CButton       *m_StopRecBtn;
	CComboBox     m_CurRecChannelComboBox;
	CSliderCtrl   m_ReplayProgressSliderCtrl;

	CListCtrl     m_RecFileInfoListCtrl;
	CListCtrl     m_ReplayRecInfoShowListCtrl;

	std::map<uint32_t, CString>   m_RecChannelMap;

	Ckp2p_check_tool_win32Dlg     *m_Parent;

	kp2p_rec_playback_handle_t    play_h;
	HANDLE                        m_ReplayProgressStartHandle;
	HANDLE                        m_ReplayProgressExitHandle;
	HANDLE                        m_ReplayRecTimerStartHandle;
	HANDLE                        m_ReplayRecTimerExitHandle;
	HANDLE                        m_ReplayRecTimeOutNotifyHandle;
	HANDLE                        m_ReplayRecTimerQuitNotifyHandle;

	HANDLE                        m_ReplayRecTimerHandleThr;

	static volatile LONG          m_OnReplayRecCountLock;
	static volatile LONG          m_OnReplayVedioCountLock;
	static volatile LONG          m_OnReplayAudioCountLock;
	static uint64_t               m_FrameCountTotal;
	static time_t                 m_GetIFrameStartTime;
	static time_t                 m_GetIFrameEndTime;
	static LONG                   m_VedioFrameNum;
	static LONG                   m_AudioFrameNum;
	static LONG                   m_CurReplayRecDuraTime;

public:
	int replay_maketime(time_t *start_time, time_t *end_time);
	static unsigned int __stdcall ReplayProgressWorkThreadProc(PVOID arg);
	static unsigned int __stdcall ReplayRecTimerWorkThreadProc(PVOID arg);

	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMDblclkVedioFileList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedPlayVedioButton();
	afx_msg void OnBnClickedStopVediobutton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

};
