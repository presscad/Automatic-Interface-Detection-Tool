#pragma once


// CKvideo 对话框

class Ckp2p_check_tool_win32Dlg;

class CKvideoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKvideoDlg)

public:
	CKvideoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKvideoDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VEDIO_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDtnDatetimechangeDatetimepicker3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedQueryVedioButton();
	afx_msg void OnBnClickedResetVideoButton();
	afx_msg void OnBnClickedOk();

public:
	void SetParent(PVOID arg);

private:
	void FindRecFile(PVOID arg);
	void SearchRecFile(PVOID arg);

public:
	CDateTimeCtrl m_QueryDateTimeCtrl;
	CDateTimeCtrl m_QueryStartTimeCtrl;
	CDateTimeCtrl m_QueryEndTimeCtrl;

	CListCtrl     m_RecFileInfoListCtrl;

	//CString       m_QueryStartTimeText;
	//CString       m_QueryEndTimeText;

	Ckp2p_check_tool_win32Dlg *m_Parent;
	afx_msg void OnBnClickedPlayVedioButton();
};
