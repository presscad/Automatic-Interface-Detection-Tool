// KdevSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "kp2p_check_tool_win32.h"
#include "kp2p_check_tool_win32Dlg.h"
#include "KdevSettingDlg.h"
#include "afxdialogex.h"


// CKdevSettingDlg 对话框

IMPLEMENT_DYNAMIC(CKdevSettingDlg, CDialogEx)

CKdevSettingDlg::CKdevSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEV_SETTING_DIALOG, pParent)
{
	m_nItemCount = 0;
	m_bGateWay = FALSE;
	m_bMTU = FALSE;
	m_bDNS = FALSE;
	m_ParentThis = NULL;
}

CKdevSettingDlg::~CKdevSettingDlg()
{
}

void CKdevSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_GATEWAY_DEV_EDIT, m_EditGateWay);
	DDX_Text(pDX, IDC_MTU_DEV_EDIT, m_EditMTU);
	DDX_Text(pDX, IDC_DNS_DEV_EDIT, m_EditDNS);

	DDX_Control(pDX, IDC_GATEWAY_DEV_CUR_STATIC, m_LableCurGateWay);
	DDX_Control(pDX, IDC_DNS_DEV_CUR_STATIC, m_LableCurDNS);
	DDX_Control(pDX, IDC_MTU_DEV_CUR_STATIC, m_LableCurMTU);
}


BEGIN_MESSAGE_MAP(CKdevSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_GATEWAY_MODIFY_BUTTON, &CKdevSettingDlg::OnBnClickedGatewayModifyButton)
	ON_BN_CLICKED(IDC_DNS_MODIFY_BUTTON, &CKdevSettingDlg::OnBnClickedDnsModifyButton)
	ON_BN_CLICKED(IDC_MTU_MODIFY_BUTTON, &CKdevSettingDlg::OnBnClickedMtuModifyButton)
	ON_BN_CLICKED(IDCANCEL, &CKdevSettingDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CONFIRM_MODIFY_BUTTON, &CKdevSettingDlg::OnBnClickedConfirmModifyButton)
END_MESSAGE_MAP()


void CKdevSettingDlg::Init(PVOID arg)
{
	m_ParentThis = (Ckp2p_check_tool_win32Dlg*)arg;
	m_nItemCount = 0;
	m_bGateWay = FALSE;
	m_bMTU = FALSE;
	m_bDNS = FALSE;
}

void CKdevSettingDlg::SetParent(PVOID arg)
{
	m_ParentThis = (Ckp2p_check_tool_win32Dlg*)arg;
}

// CKdevSettingDlg 消息处理程序

BOOL CKdevSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_EditGateWay.Empty();
	m_EditMTU.Empty();
	m_EditDNS.Empty();
	m_LableCurGateWay.SetWindowTextW(_T("正在查询中..."));
	m_LableCurDNS.SetWindowTextW(_T("正在查询中..."));
	m_LableCurMTU.SetWindowTextW(_T("正在查询中..."));
	/*m_LableCurGateWay.SetWindowTextW(m_ParentThis->m_LableCurGateWay.GetString());
	m_LableCurDNS.SetWindowTextW(m_ParentThis->m_LableCurDNS.GetString());
	m_LableCurMTU.SetWindowTextW(m_ParentThis->m_LableCurMTU.GetString());
*/
	UpdateData(FALSE);
	return TRUE;
}


void CKdevSettingDlg::OnBnClickedGatewayModifyButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	INT_PTR nRes = 0;
	CEdit* pEditGateway = NULL;
	if (m_EditGateWay.IsEmpty()) {
		nRes = MessageBox(_T("请输入需要修改的数据"), _T("信息提示"), MB_OK);
		if (IDOK == nRes) {
			pEditGateway = (CEdit *)GetDlgItem(IDC_GATEWAY_DEV_EDIT);
			pEditGateway->SetFocus();
		}
		return;
	}
	m_bGateWay = TRUE;
	m_nItemCount++;
	MessageBox(_T("输入网关数据导入成功"), _T("信息提示"), MB_OK);
}


void CKdevSettingDlg::OnBnClickedDnsModifyButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	INT_PTR nRes = 0;
	CEdit* pEditDNS = NULL;
	if (m_EditDNS.IsEmpty()) {
		nRes = MessageBox(_T("请输入需要修改的数据"), _T("信息提示"), MB_OK);
		if (IDOK == nRes) {
			pEditDNS = (CEdit *)GetDlgItem(IDC_DNS_DEV_EDIT);
			pEditDNS->SetFocus();
		}
		return;
	}
	m_bDNS = TRUE;
	m_nItemCount++;
	MessageBox(_T("输入DNS数据导入成功"), _T("信息提示"), MB_OK);
}


void CKdevSettingDlg::OnBnClickedMtuModifyButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	INT_PTR nRes = 0;
	CEdit* pEditMTU = NULL;
	if (m_EditMTU.IsEmpty()) {
		nRes = MessageBox(_T("请输入需要修改的数据"), _T("信息提示"), MB_OK);
		if (IDOK == nRes) {
			pEditMTU = (CEdit *)GetDlgItem(IDC_MTU_DEV_EDIT);
			pEditMTU->SetFocus();
		}
		return;
	}
	m_bMTU = TRUE; 
	m_nItemCount++;
	MessageBox(_T("输入MTU数据导入成功"), _T("信息提示"), MB_OK);
}




void CKdevSettingDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	m_EditGateWay.Empty();
	m_EditDNS.Empty();
	m_EditMTU.Empty();

	CDialogEx::OnCancel();
}


void CKdevSettingDlg::OnBnClickedConfirmModifyButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	::InterlockedIncrement(&m_ParentThis->m_OnDataFlagCountLock);
	if (::InterlockedDecrement(&m_ParentThis->m_OnDataFlagCountLock) != 0) {
		MessageBox(_T("正在查询修改中，请等待"), _T("信息提示"), MB_OK);
		return;
	}
	if (/*m_EditGateWay.IsEmpty() && */m_EditMTU.IsEmpty() && m_EditDNS.IsEmpty()) {
		MessageBox(_T("请输入需要修改的数据"), _T("信息提示"), MB_OK);
		return;
	}
	if (!m_EditGateWay.IsEmpty()) {
		m_bGateWay = TRUE;
		m_nItemCount++;
	}
	if (!m_EditDNS.IsEmpty()) {
		m_bDNS = TRUE;
		m_nItemCount++;
	}
	if (!m_EditMTU.IsEmpty()) {
		m_bMTU = TRUE;
		m_nItemCount++;
	}

	INT_PTR nRes = MessageBox(_T("是否确认修改？"), _T("信息提示"), MB_YESNO);
	if (IDYES == nRes) {
		/*if ((!m_EditGateWay.IsEmpty() && !m_bGateWay)
			|| (m_EditDNS.IsEmpty() && !m_bDNS)
			|| (m_EditMTU.IsEmpty() && !m_bMTU)) {

			
		}
		else {
			MessageBox(_T("输入数据后请点击导入按钮"), _T("信息提示"), MB_OK);
			return;
		}*/

		m_ParentThis->m_bQueryModDevConfigInfoFlag = TRUE;
	}
	else {
		CDialogEx::OnCancel();
		return;
	}

	CDialogEx::OnOK();
}

BOOL CKdevSettingDlg::InitCurConfigInfo()
{
	CString localcmd;
	char localcmd2[256];
	memset(localcmd2, 0x00, 256);
	DWORD nRet;
	int ncmds = 0;

	m_nItemCount = 3;
	/*m_bGateWay = TRUE;
	m_bMTU = TRUE;
	m_bDNS = TRUE;*/

	while (m_nItemCount > 0) {
		if (m_bGateWay) {
			localcmd.SetString(L"route | grep default | awk '{print $2}'");
			m_bGateWay = FALSE;
			ncmds = 1;
			goto MSCONFIG_END;
		}
		if (m_bDNS) {
			localcmd.SetString(L"cat /etc/resolv.conf  | awk '{print $2}'");
			m_bDNS = FALSE;
			ncmds = 2;
			goto MSCONFIG_END;
		}
		if (m_bMTU) {
			localcmd.SetString(L"cat /sys/class/net/eth0/mtu");
			m_bMTU = FALSE;
			ncmds = 3;
			goto MSCONFIG_END;
		}

	MSCONFIG_END:

		int send_size = IOT_SHELL_Send(m_ParentThis->m_Session, CW2A(localcmd.GetString()), localcmd.GetLength() + 1, &m_ParentThis->m_ReqData);
		if ((nRet = WaitForSingleObject(m_ParentThis->m_ThreadNotifyCmdEvent, 30000)) != WAIT_OBJECT_0) {
			Sleep(50);
			switch (nRet)
			{
			case WAIT_TIMEOUT:
				MessageBox(_T("获取数据时间超过30秒"), _T("信息提示"), MB_OK);
				break;
			case WAIT_FAILED:
				MessageBox(_T("WaitForSingleObject error"), _T("信息提示"), MB_OK);
				break;
			default:
				break;
			}
		}

		switch (ncmds) {
		case 1:
			//m_LableCurGateWay = m_ParentThis->m_sdns;
			break;
		case 2:
			//m_LableCurDNS = m_ParentThis->m_sdns;
			break;
		case 3:
			//m_LableCurMTU = m_ParentThis->m_sdns;
			break;
		default:
			break;
		}
		UpdateData(FALSE);

		m_nItemCount--;
		localcmd.Empty();

	}

	return TRUE;
}
