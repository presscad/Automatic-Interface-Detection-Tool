// DevInfoDetailDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "kp2p_check_tool_win32.h"
#include "kp2p_check_tool_win32Dlg.h"
#include "DevInfoDetailDlg.h"
#include "afxdialogex.h"


// CDevInfoDetailDlg 对话框

IMPLEMENT_DYNAMIC(CDevInfoDetailDlg, CDialogEx)

CDevInfoDetailDlg::CDevInfoDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEV_INFO_DETAIL_DIALOG, pParent)
{

}

CDevInfoDetailDlg::~CDevInfoDetailDlg()
{
}

void CDevInfoDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DEV_INFO_DETAIL_LIST, m_DevInfoDetailListCtrl);
}


BEGIN_MESSAGE_MAP(CDevInfoDetailDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CDevInfoDetailDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDevInfoDetailDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CDevInfoDetailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_DevInfoDetailListCtrl.SetExtendedStyle(m_DevInfoDetailListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	/*m_DevInfoDetailListCtrl.InsertColumn(0, TEXT("功能项"), 0, 100);
	m_DevInfoDetailListCtrl.InsertColumn(1, TEXT("信息"), 0, 500);

	m_DevInfoDetailListCtrl.InsertItem(0, _T(""));

	m_DevInfoDetailListCtrl.SetItemText(0, 0, _T("123"));
	m_DevInfoDetailListCtrl.SetItemText(0, 1, _T("456"));*/

	UpdateData(FALSE);

	return TRUE;
}

// CDevInfoDetailDlg 消息处理程序

void CDevInfoDetailDlg::SetParent(PVOID arg)
{
	m_Parent = (kp2p_check_tool_win32Dlg *)arg;
}

void CDevInfoDetailDlg::OnDestroy()
{
	CDialog::OnDestroy();
	delete this;
}

void CDevInfoDetailDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
	DestroyWindow();
}


void CDevInfoDetailDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
	DestroyWindow();
}
