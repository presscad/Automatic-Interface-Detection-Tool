// KTestCmdDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "kp2p_check_tool_win32.h"
#include "kp2p_check_tool_win32Dlg.h"
#include "KTestCmdDlg.h"
#include "afxdialogex.h"


// CKTestCmdDlg 对话框

IMPLEMENT_DYNAMIC(CKTestCmdDlg, CDialogEx)

CKTestCmdDlg::CKTestCmdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEST_CMD_DIALOG, pParent)
{

}

CKTestCmdDlg::~CKTestCmdDlg()
{
}

void CKTestCmdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_TEST_CMD_CONTENT_EDIT, m_EditCmd);
}


BEGIN_MESSAGE_MAP(CKTestCmdDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CKTestCmdDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CKTestCmdDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CKTestCmdDlg 消息处理程序


void CKTestCmdDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;

	UpdateData(TRUE);
	if (m_EditCmd.IsEmpty()) {
		nRes = MessageBox(_T("输入命令内容为空，请输入命令内容"), _T("信息提示"), MB_OK);
		if (IDOK == nRes) {
			return;
		}
	}

	CDialogEx::OnOK();
}


void CKTestCmdDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_EditCmd.Empty();
	//UpdateData(FALSE);
	CDialogEx::OnCancel();
}
