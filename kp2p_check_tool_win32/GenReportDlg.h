#pragma once


// CGenReprotDlg 对话框

class Ckp2p_check_tool_win32Dlg;

class CGenReportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGenReportDlg)

public:
	CGenReportDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGenReportDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPORT_DATA_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	Ckp2p_check_tool_win32Dlg *m_Parent;

public:
	void SetParent(PVOID arg);
};
