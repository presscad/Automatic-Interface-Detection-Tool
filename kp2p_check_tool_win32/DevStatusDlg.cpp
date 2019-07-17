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
	ON_NOTIFY(NM_DBLCLK, IDC_STATUS_LIST, &CDevStatusDlg::OnNMDblclkStatusList)
END_MESSAGE_MAP()


// CDevStatusDlg 消息处理程序

BOOL CDevStatusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	/*CRect tabRect;
	GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 1;
	tabRect.bottom -= 1;
	m_ListCtrlStatus.SetWindowPos(NULL, tabRect.left, tabRect.top + 5, tabRect.Width(), tabRect.Height() + 5, SWP_SHOWWINDOW);*/

	m_ListCtrlStatus.SetExtendedStyle(m_ListCtrlStatus.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_ListCtrlStatus.InsertColumn(0, TEXT("功能项"), 0, 100);
	m_ListCtrlStatus.InsertColumn(1, TEXT("信息"), 0, 200);
	m_ListCtrlStatus.InsertColumn(2, TEXT("其他"), 0, 300);

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


void CDevStatusDlg::OnNMDblclkStatusList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取选中的的一个位置
	POSITION p = m_ListCtrlStatus.GetFirstSelectedItemPosition();
	if (p == NULL)
	{
		TRACE(_T("没有行被选中!"));
		*pResult = 0;
		return;
	}
	//获取刚选取的位置的下标(从0开始的)
	int index = m_ListCtrlStatus.GetNextSelectedItem(p);
	//获得选中的内容
	//得到第index行.第0列的内容(下标从0开始)
	CString FirstColumn = m_ListCtrlStatus.GetItemText(index, 0);
	//得到第index行,第1列的内容
	CString SecondColumn = m_ListCtrlStatus.GetItemText(index, 1);
	CString ThirdColumn = m_ListCtrlStatus.GetItemText(index, 2);

	//m_DevInfoDetailDlg.SetParent(this);

	//非模态
	CDevInfoDetailDlg *infoDlg = new CDevInfoDetailDlg();
	infoDlg->Create(IDD_DEV_INFO_DETAIL_DIALOG, NULL);
	infoDlg->SetParent(this);
	infoDlg->SetWindowText(FirstColumn.GetString());
	infoDlg->ShowWindow(SW_SHOWNORMAL);
	infoDlg->m_DevInfoDetailListCtrl.InsertColumn(0, TEXT("测试项"), 0, 100);
	infoDlg->m_DevInfoDetailListCtrl.InsertColumn(1, TEXT("信息"), 0, 500);
	infoDlg->m_DevInfoDetailListCtrl.InsertColumn(2, TEXT("其他"), 0, 500);
	infoDlg->m_DevInfoDetailListCtrl.InsertItem(0, _T(""));
	infoDlg->m_DevInfoDetailListCtrl.SetItemText(0, 0, FirstColumn.GetString());
	infoDlg->m_DevInfoDetailListCtrl.SetItemText(0, 1, SecondColumn.GetString());
	infoDlg->m_DevInfoDetailListCtrl.SetItemText(0, 2, ThirdColumn.GetString());

	//模态
	/*INT_PTR nRes = m_DevInfoDetailDlg.DoModal();
	if (nRes == IDOK) {

	}
	*/

	*pResult = 0;
}
