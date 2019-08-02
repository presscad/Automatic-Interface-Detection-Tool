#pragma once


// CKdevSettingDlg 对话框

class Ckp2p_check_tool_win32Dlg;

class CKdevSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKdevSettingDlg)

public:
	CKdevSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKdevSettingDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEV_SETTING_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGatewayModifyButton();
	afx_msg void OnBnClickedDnsModifyButton();
	afx_msg void OnBnClickedMtuModifyButton();

public:
	void Init(PVOID arg);
	void SetMainParent(PVOID arg);
	BOOL InitCurConfigInfo();

public:
	CString m_EditGateWay;
	CString m_EditMTU;
	CString m_EditDNS;
	CString m_CurGateWayInfo;
	CString m_CurMTUInfo;
	CString m_CurDNSInfo;

	CStatic m_LableCurGateWay;
	CStatic m_LableCurMTU;
	CStatic m_LableCurDNS;
	CIPAddressCtrl m_IPCtrlDns;

	BOOL						m_bGateWay;
	BOOL						m_bMTU;
	BOOL						m_bDNS;
	Ckp2p_check_tool_win32Dlg   *m_ParentThis;
	UINT						m_nItemCount;
	
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedConfirmModifyButton();
	//afx_msg void OnIpnFieldchangedDnsDevIpaddress(NMHDR *pNMHDR, LRESULT *pResult);
};
