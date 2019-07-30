// KdevSysTimeSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "kp2p_check_tool_win32.h"
#include "kp2p_check_tool_win32Dlg.h"
#include "KdevSysTimeSettingDlg.h"
#include "afxdialogex.h"


// CKdevSysTimeSettingDlg 对话框

IMPLEMENT_DYNAMIC(CKdevSysTimeSettingDlg, CDialogEx)

CKdevSysTimeSettingDlg::CKdevSysTimeSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEV_SYS_TIME_SETTING_DIALOG, pParent)
{

}

CKdevSysTimeSettingDlg::~CKdevSysTimeSettingDlg()
{
}

void CKdevSysTimeSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CKdevSysTimeSettingDlg, CDialogEx)
END_MESSAGE_MAP()


// CKdevSysTimeSettingDlg 消息处理程序
