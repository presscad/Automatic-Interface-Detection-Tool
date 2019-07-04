#pragma once


// CKping 对话框

class CKpingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKpingDlg)

public:
	CKpingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKpingDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PING_SETTING_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

public:
	CString   m_PNum;
	CString   m_PpkgSize;
	CString   m_PwebSite;
	CComboBox m_PcomboBoxWebSiteItem;
};
