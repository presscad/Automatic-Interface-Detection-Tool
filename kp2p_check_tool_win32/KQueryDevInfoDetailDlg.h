#pragma once


// CKQueryDevInfoDetailDlg 对话框

class CKQueryDevInfoDetailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKQueryDevInfoDetailDlg)

public:
	CKQueryDevInfoDetailDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKQueryDevInfoDetailDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QUERY_DEV_INFO_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl         m_ListCtrlQueryInfo;
};
