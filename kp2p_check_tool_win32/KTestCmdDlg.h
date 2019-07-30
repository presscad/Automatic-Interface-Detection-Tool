#pragma once


// CKTestCmdDlg 对话框

class CKTestCmdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKTestCmdDlg)

public:
	CKTestCmdDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKTestCmdDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST_CMD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_EditCmd;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
