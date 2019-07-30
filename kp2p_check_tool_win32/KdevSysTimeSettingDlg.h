#pragma once


// CKdevSysTimeSettingDlg 对话框

class CKdevSysTimeSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKdevSysTimeSettingDlg)

public:
	CKdevSysTimeSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKdevSysTimeSettingDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEV_SYS_TIME_SETTING_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
