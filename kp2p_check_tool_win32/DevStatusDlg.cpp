// DevStatusDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "kp2p_check_tool_win32.h"
#include "DevStatusDlg.h"
#include "afxdialogex.h"


// CDevStatusDlg 对话框

IMPLEMENT_DYNAMIC(CDevStatusDlg, CDialogEx)

CDevStatusDlg::CDevStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEV_STATUS_DIALOG, pParent)
{
	
}

CDevStatusDlg::~CDevStatusDlg()
{
}

void CDevStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATUS_LIST, m_ListCtrlStatus);
}


BEGIN_MESSAGE_MAP(CDevStatusDlg, CDialogEx)
END_MESSAGE_MAP()


// CDevStatusDlg 消息处理程序

BOOL CDevStatusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ListCtrlStatus.InsertColumn(0, TEXT("功能项"), 0, 100);
	m_ListCtrlStatus.InsertColumn(1, TEXT("信息"), 0, 300);

	/*m_ListCtrlStatus.InsertItem(0, _T("PING检测"));
	m_ListCtrlStatus.InsertItem(1, _T("网络连接状况检测"));
	m_ListCtrlStatus.InsertItem(2, _T("路由表检测"));
	m_ListCtrlStatus.InsertItem(3, _T("带宽检测"));*/

	return TRUE;
}

void CDevStatusDlg::PutDevStatusInfoOnList()
{
	/*m_ListCtrlStatus.SetItemText(0, 1, _T("OK"));
	m_ListCtrlStatus.InsertItem(1, _T("3"));
	m_ListCtrlStatus.SetItemText(1, 1, _T("FAIL"));*/
}
