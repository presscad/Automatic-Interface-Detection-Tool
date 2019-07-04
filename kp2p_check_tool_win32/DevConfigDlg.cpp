// DevConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "kp2p_check_tool_win32.h"
#include "DevConfigDlg.h"
#include "afxdialogex.h"


// CDevConfigDlg 对话框

IMPLEMENT_DYNAMIC(CDevConfigDlg, CDialogEx)

CDevConfigDlg::CDevConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEV_CONFIG_DIALOG, pParent)
{

}

CDevConfigDlg::~CDevConfigDlg()
{
}

void CDevConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CONFIG_LIST, m_ListCtrlConfig);
}


BEGIN_MESSAGE_MAP(CDevConfigDlg, CDialogEx)
END_MESSAGE_MAP()


// CDevConfigDlg 消息处理程序

BOOL CDevConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ListCtrlConfig.InsertColumn(0, TEXT("功能项"), 0, 100);
	m_ListCtrlConfig.InsertColumn(1, TEXT("信息"), 0, 300);

	/*m_ListCtrlConfig.InsertItem(0, _T("MTU"));
	m_ListCtrlConfig.InsertItem(1, _T("网关"));
	m_ListCtrlConfig.InsertItem(2, _T("DNS"));
	m_ListCtrlConfig.InsertItem(3, _T("信号强度"));
	m_ListCtrlConfig.InsertItem(4, _T("CPU占用"));
	m_ListCtrlConfig.InsertItem(5, _T("内存占用"));
	m_ListCtrlConfig.InsertItem(6, _T("进程"));
	m_ListCtrlConfig.InsertItem(7, _T("线程"));*/

	return TRUE;
}

void CDevConfigDlg::PutDevConfigInfoOnList()
{
	/*m_ListCtrlConfig.SetItemText(0, 1, _T("OK"));
	m_ListCtrlConfig.InsertItem(1, _T("3"));
	m_ListCtrlConfig.SetItemText(1, 1, _T("FAIL"));*/
}
