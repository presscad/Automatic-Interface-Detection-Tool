// Kping.cpp : 实现文件
//

#include "stdafx.h"
#include "kp2p_check_tool_win32.h"
#include "KpingDlg.h"
#include "afxdialogex.h"


// CKping 对话框

IMPLEMENT_DYNAMIC(CKpingDlg, CDialogEx)

CKpingDlg::CKpingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PING_SETTING_DIALOG, pParent)
{

}

CKpingDlg::~CKpingDlg()
{
}

void CKpingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_PING_NUM_EDIT, m_PNum);
	DDX_Text(pDX, IDC_PING_PKG_SIZE_EDIT, m_PpkgSize);
	DDX_Control(pDX, IDC_PING_WEBSITE_COMBO, m_PcomboBoxWebSiteItem);
}


BEGIN_MESSAGE_MAP(CKpingDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CKpingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CKping 消息处理程序


void CKpingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int index;
	if (m_PNum.IsEmpty() || m_PpkgSize.IsEmpty() || (index = m_PcomboBoxWebSiteItem.GetCurSel()) == -1) {
		MessageBox(_T("设置次数或包大小不能为空，请填写"), _T("信息提示"), MB_OK);
		return;
	}

	//m_PcomboBoxWebSiteItem.GetWindowText(m_PwebSite);
	switch (m_PcomboBoxWebSiteItem.GetCurSel())
	{
	case 0:
		m_PwebSite.SetString(L"www.baidu.com");
		break;
	case 1:
		m_PwebSite.SetString(L"www.baidu.com");
		break;
	case 2:
		m_PwebSite.SetString(L"www.baidu.com");
		break;
	default:
		break;
	}

	CDialogEx::OnOK();
}

BOOL CKpingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_PNum.SetString(_T("1"));
	m_PpkgSize.SetString(_T("1000"));
	m_PcomboBoxWebSiteItem.AddString(_T("百度"));
	m_PcomboBoxWebSiteItem.AddString(_T("谷歌"));
	m_PcomboBoxWebSiteItem.AddString(_T("易视网"));
	m_PcomboBoxWebSiteItem.SetCurSel(0);

	UpdateData(FALSE);

	return TRUE;
}
