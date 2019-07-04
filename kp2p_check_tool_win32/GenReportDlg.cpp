// GenReprotDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "kp2p_check_tool_win32.h"
#include "kp2p_check_tool_win32Dlg.h"
#include "GenReportDlg.h"
#include "afxdialogex.h"


// CGenReprotDlg 对话框

IMPLEMENT_DYNAMIC(CGenReportDlg, CDialogEx)

CGenReportDlg::CGenReportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EXPORT_DATA_DIALOG, pParent)
{

}

CGenReportDlg::~CGenReportDlg()
{
}

void CGenReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGenReportDlg, CDialogEx)
END_MESSAGE_MAP()


// CGenReprotDlg 消息处理程序

void CGenReportDlg::SetParent(PVOID arg)
{
	m_Parent = (Ckp2p_check_tool_win32Dlg*)arg;
}
