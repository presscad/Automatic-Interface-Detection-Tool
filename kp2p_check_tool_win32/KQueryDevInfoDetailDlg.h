#pragma once


// CKQueryDevInfoDetailDlg �Ի���

class CKQueryDevInfoDetailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKQueryDevInfoDetailDlg)

public:
	CKQueryDevInfoDetailDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CKQueryDevInfoDetailDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QUERY_DEV_INFO_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl         m_ListCtrlQueryInfo;
};
