#pragma once


// CDevStatusDlg 对话框

class CDevStatusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDevStatusDlg)

public:
	CDevStatusDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDevStatusDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEV_STATUS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	void PutDevStatusInfoOnList();

public:
	CListCtrl m_ListCtrlStatus;
};
