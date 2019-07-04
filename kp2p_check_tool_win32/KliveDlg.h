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

	DECLARE_MESSAGE_MAP()

public:
	Ckp2p_check_tool_win32Dlg *m_Parent;

public:
	void SetParentThis(void *parent);
	void StartLive(PVOID arg, int channel);
	afx_msg void OnBnClickedOk();
};
