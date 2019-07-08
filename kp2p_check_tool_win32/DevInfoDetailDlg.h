#pragma once


// CDevInfoDetailDlg 对话框
class kp2p_check_tool_win32Dlg;
class CDevInfoDetailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDevInfoDetailDlg)

public:
	CDevInfoDetailDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDevInfoDetailDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEV_INFO_DETAIL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	kp2p_check_tool_win32Dlg *m_Parent;

	CListCtrl m_DevInfoDetailListCtrl;

public:
	void SetParent(PVOID arg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
};
