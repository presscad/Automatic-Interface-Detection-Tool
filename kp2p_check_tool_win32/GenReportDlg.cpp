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

	DDX_Text(pDX, IDC_SAVE_PATH_SAVE_EDIT, m_SavePahtEdit);
	DDX_Control(pDX, IDC_CHOOSE_DATE_SAVE_DATETIMEPICKER, m_ChooseDateTimeCtrl);
	DDX_Control(pDX, IDC_CHOOSE_DATA_TYPE_SAVE_COMBO, m_ChooseDataTypeComboBox);
	DDX_Control(pDX, IDC_UNCHOOSE_DEV_ID_SAVE_LIST, m_UnChooseItemListBox);
	DDX_Control(pDX, IDC_CHOOSE_DEV_ID_SAVE_LIST, m_ChoosedItemListBox);
}


BEGIN_MESSAGE_MAP(CGenReportDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ADD_ITEM_SAVE_BUTTON, &CGenReportDlg::OnBnClickedAddItemSaveButton)
	ON_BN_CLICKED(IDC_DEL_ITEM_SAVE_BUTTON, &CGenReportDlg::OnBnClickedDelItemSaveButton)
	ON_BN_CLICKED(IDC_CLEAR_ITEM_SAVE_BUTTON, &CGenReportDlg::OnBnClickedClearItemSaveButton)
	ON_BN_CLICKED(IDC_BROWSE_FILE_SAVE_BUTTON, &CGenReportDlg::OnBnClickedBrowseFileSaveButton)
	ON_BN_CLICKED(IDC_SAVE_FILE_SAVE_BUTTON, &CGenReportDlg::OnBnClickedSaveFileSaveButton)
	ON_BN_CLICKED(IDOK, &CGenReportDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGenReportDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_LOAD_DEV_ID_SAVE_BUTTON, &CGenReportDlg::OnBnClickedLoadDevIdSaveButton)
	ON_BN_CLICKED(IDC_CHECK_ITEM_SAVE_BUTTON, &CGenReportDlg::OnBnClickedCheckItemSaveButton)
	ON_BN_CLICKED(IDC_ADD_ALL_ITEM_SAVE_BUTTON, &CGenReportDlg::OnBnClickedAddAllItemSaveButton)
END_MESSAGE_MAP()

BOOL CGenReportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ChooseDataTypeComboBox.InsertString(0, _T("设备ID"));
	m_ChooseDataTypeComboBox.InsertString(1, _T("用户名"));
	//m_ChooseDataTypeComboBox.AddString(_T("设备ID"));
	//m_ChooseDataTypeComboBox.AddString(_T("用户名"));
	m_ChooseDataTypeComboBox.SetCurSel(0);

	m_bOnceAgain = FALSE;

	return TRUE;
}

// CGenReprotDlg 消息处理程序

void CGenReportDlg::SetParent(PVOID arg)
{
	m_Parent = (Ckp2p_check_tool_win32Dlg*)arg;
}


void CGenReportDlg::OnBnClickedAddItemSaveButton()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CGenReportDlg::OnBnClickedDelItemSaveButton()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CGenReportDlg::OnBnClickedClearItemSaveButton()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CGenReportDlg::OnBnClickedBrowseFileSaveButton()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CGenReportDlg::OnBnClickedSaveFileSaveButton()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CGenReportDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CGenReportDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CGenReportDlg::OnBnClickedLoadDevIdSaveButton()
{
	// TODO: 在此添加控件通知处理程序代码
	m_UnChooseItemListBox.ResetContent();
	CString item;
	map<LONG64, pkp2p_dev_config_t>::iterator it = m_Parent->m_DevConfigInfoMap.begin();
	m_ChooseDataTypeComboBox.GetWindowText(item);

	if (item.Compare(_T("设备ID")) == 0) {
		goto CHOOSE_TYPE;
	}

CHOOSE_TYPE:

	for (; it != m_Parent->m_DevConfigInfoMap.end(); it++) {
		item.Empty();
		item.Format(_T("%lld"), (*it).first);
		m_UnChooseItemListBox.AddString(item.GetString());	
	}


}


void CGenReportDlg::OnBnClickedCheckItemSaveButton()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CGenReportDlg::OnBnClickedAddAllItemSaveButton()
{
	// TODO: 在此添加控件通知处理程序代码
}
