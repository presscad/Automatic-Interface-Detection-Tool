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
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	Ckp2p_check_tool_win32Dlg *m_Parent;

	CString       m_SavePahtEdit;
	CDateTimeCtrl m_ChooseDateTimeCtrl;
	CComboBox     m_ChooseDataTypeComboBox;
	CListBox      m_UnChooseItemListBox;
	CListBox      m_ChoosedItemListBox;

	BOOL          m_bOnceAgain;

public:
	void SetParent(PVOID arg);
	afx_msg void OnBnClickedAddItemSaveButton();
	afx_msg void OnBnClickedDelItemSaveButton();
	afx_msg void OnBnClickedClearItemSaveButton();
	afx_msg void OnBnClickedBrowseFileSaveButton();
	afx_msg void OnBnClickedSaveFileSaveButton();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedLoadDevIdSaveButton();
	afx_msg void OnBnClickedCheckItemSaveButton();
	afx_msg void OnBnClickedAddAllItemSaveButton();
};
