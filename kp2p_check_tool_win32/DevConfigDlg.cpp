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
	ON_NOTIFY(NM_DBLCLK, IDC_CONFIG_LIST, &CDevConfigDlg::OnNMDblclkConfigList)
END_MESSAGE_MAP()


// CDevConfigDlg 消息处理程序

BOOL CDevConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	/*CRect tabRect;
	GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 1;
	tabRect.bottom -= 1; 
	m_ListCtrlConfig.SetWindowPos(NULL, tabRect.left, tabRect.top + 5, tabRect.Width(), tabRect.Height() + 5, SWP_SHOWWINDOW);*/

	m_ListCtrlConfig.SetExtendedStyle(m_ListCtrlConfig.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ListCtrlConfig.InsertColumn(0, TEXT("功能项"), 0, 100);
	m_ListCtrlConfig.InsertColumn(1, TEXT("信息"), 0, 200);
	m_ListCtrlConfig.InsertColumn(2, TEXT("其他"), 0, 300);

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


void CDevConfigDlg::OnNMDblclkConfigList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取选中的的一个位置
	POSITION p = m_ListCtrlConfig.GetFirstSelectedItemPosition();
	if (p == NULL)
	{
		TRACE(_T("没有行被选中!"));
		*pResult = 0;
		return;
	}
	//获取刚选取的位置的下标(从0开始的)
	int index = m_ListCtrlConfig.GetNextSelectedItem(p);
	//获得选中的内容
	//得到第index行.第0列的内容(下标从0开始)
	CString FirstColumn = m_ListCtrlConfig.GetItemText(index, 0);
	//得到第index行,第1列的内容
	CString SecondColumn = m_ListCtrlConfig.GetItemText(index, 1);

	//m_DevInfoDetailDlg.SetParent(this);

	//非模态
	CDevInfoDetailDlg *infoDlg = new CDevInfoDetailDlg();
	infoDlg->Create(IDD_DEV_INFO_DETAIL_DIALOG, NULL);
	infoDlg->SetParent(this);
	infoDlg->SetWindowText(FirstColumn.GetString());
	infoDlg->ShowWindow(SW_SHOWNORMAL);
	infoDlg->m_DevInfoDetailListCtrl.InsertColumn(0, TEXT("测试项"), 0, 100);
	infoDlg->m_DevInfoDetailListCtrl.InsertColumn(1, TEXT("信息"), 0, 500);
	infoDlg->m_DevInfoDetailListCtrl.InsertItem(0, _T(""));
	infoDlg->m_DevInfoDetailListCtrl.SetItemText(0, 0, FirstColumn.GetString());
	infoDlg->m_DevInfoDetailListCtrl.SetItemText(0, 1, SecondColumn.GetString());	

	//模态
	/*INT_PTR nRes = m_DevInfoDetailDlg.DoModal();
	if (nRes == IDOK) {
		
	}
*/
	*pResult = 0;
}
