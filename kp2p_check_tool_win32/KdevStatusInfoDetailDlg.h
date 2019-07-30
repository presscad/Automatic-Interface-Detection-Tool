#pragma once
#include <vector>

// CKdevStatusInfoDetailDlg 对话框

class CKdevStatusInfoDetailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKdevStatusInfoDetailDlg)

public:
	CKdevStatusInfoDetailDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKdevStatusInfoDetailDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEV_STATUS_INFO_DETAIL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl                  m_DevStatusInfoDetailListCtrl;
	CComboBox                  m_cDevIDComboBox;
	CStatic                    m_CurQueryStatusLable;

	std::vector<CString>       m_CurDevIDHistoryVec;

	Ckp2p_check_tool_win32Dlg  *m_Parent;

	static int                 index;

public:
	void SetMainParent(void* arg);
	afx_msg void OnCbnEditchangeDevIdSettingCombo();
	afx_msg void OnCbnEditupdateDevIdSettingCombo();
	afx_msg void OnBnClickedQueryDevInfoSettingButton();
	afx_msg void OnBnClickedOk();
};
