// KliveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "kp2p_check_tool_win32.h"
#include "kp2p_check_tool_win32Dlg.h"
#include "KliveDlg.h"
#include "afxdialogex.h"
#include "Util.h"


// CKliveDlg 对话框

IMPLEMENT_DYNAMIC(CKliveDlg, CDialogEx)

CKliveDlg::CKliveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LIVE_DIALOG, pParent)
{

}

CKliveDlg::~CKliveDlg()
{
}

void CKliveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CKliveDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CKliveDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CKliveDlg 消息处理程序
void CKliveDlg::SetParentThis(void *parent)
{
	m_Parent = (Ckp2p_check_tool_win32Dlg*)parent;
}

void CKliveDlg::StartLive(PVOID arg, int channel)
{
	kp2p_handle_t handle = (kp2p_handle_t)arg;
	// live
	//kp2p_open_stream(handle, channel, 1);
	
	//msleep_c(8 * 1000);
	
	//kp2p_close_stream(handle, channel, 1);
	

	//msleep_c(5 * 1000);

	//云台控制
	//kp2p_ptz_ctrl(handle, channel, KP2P_PTZ_CONTROL_ACTION_STOP, 0, 0);
	//远程设置
	//kp2p_remote_setup(handle, "", strlen(""));
}


void CKliveDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
