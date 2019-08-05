#pragma once


class Ckp2p_check_tool_win32Dlg;
// CKliveDlg 对话框

class CKliveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKliveDlg)

public:
	CKliveDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKliveDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIVE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	Ckp2p_check_tool_win32Dlg *m_Parent;

	HANDLE                    m_PlayLiveTimerStartHandle;
	HANDLE                    m_PlayLiveTimerExitHandle;
	HANDLE                    m_PlayLiveTimeOutNotifyHandle;
	HANDLE                    m_PlayLiveQuitNotifyHandle;

	HANDLE                    m_PlayLiveTimerHandleThr;

	CString                   m_PlayDurationTimeEdit;
	CEdit                     *pPlayDurationEdit;
	CButton                   *m_PlayLiveBtn;
	CButton                   *m_StopLiveBtn;
	CComboBox                 m_CurLiveChannelComboBox;
	CSliderCtrl               m_PlayProgressSliderCtrl;
	CListCtrl                 m_PlayLiveInfoShowListCtrl;

	int                       m_LiveChannel;
	int                       m_LiveStreamID;
	static volatile LONG      m_OnPlayLiveCountLock;
	static volatile LONG      m_OnPlayVedioLiveCountLock;
	static volatile LONG      m_OnPlayAudioLiveCountLock;
	static uint64_t           m_FrameLiveCountTotal;
	static time_t             m_GetIFrameLiveStartTime;
	static time_t             m_GetIFrameLiveEndTime;
	static LONG               m_VedioFrameLiveNum;
	static LONG               m_AudioFrameLiveNum;
	static LONG               m_CurPlayLiveDuraTime;
	BOOL                      m_bLiveQuit;

public:
	void SetParentThis(void *parent);
	void SetLiveChannel(int channel);
	static unsigned int __stdcall PlayLiveTimerWorkThreadProc(PVOID arg);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedPlayButton();
	afx_msg void OnBnClickedStopButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
};
