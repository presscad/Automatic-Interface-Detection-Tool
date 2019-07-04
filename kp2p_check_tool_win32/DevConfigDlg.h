#pragma once


// CDevConfigDlg 对话框

class CDevConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDevConfigDlg)

public:
	CDevConfigDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDevConfigDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEV_CONFIG_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	void PutDevConfigInfoOnList();

public:
	CListCtrl m_ListCtrlConfig;
};
