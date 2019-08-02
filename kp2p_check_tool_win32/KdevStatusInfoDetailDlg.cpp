// KdevStatusInfoDetailDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "kp2p_check_tool_win32.h"
#include "kp2p_check_tool_win32Dlg.h"
#include "KdevStatusInfoDetailDlg.h"
#include "afxdialogex.h"


int CKdevStatusInfoDetailDlg::index = 0;

// CKdevStatusInfoDetailDlg 对话框

IMPLEMENT_DYNAMIC(CKdevStatusInfoDetailDlg, CDialogEx)

CKdevStatusInfoDetailDlg::CKdevStatusInfoDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEV_STATUS_INFO_DETAIL_DIALOG, pParent)
{

}

CKdevStatusInfoDetailDlg::~CKdevStatusInfoDetailDlg()
{
}

void CKdevStatusInfoDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DEV_STATUS_INFO_DETAIL_LIST, m_DevStatusInfoDetailListCtrl);
	DDX_Control(pDX, IDC_DEV_ID_SETTING_COMBO, m_cDevIDComboBox);
	DDX_Control(pDX, IDC_QUERY_DEV_INFO_STATUS_STATIC, m_CurQueryStatusLable);
	DDX_Control(pDX, IDC_QUERY_STATUS_PICTURE_STATIC, m_PicCtrlQueryStatus);
}


BEGIN_MESSAGE_MAP(CKdevStatusInfoDetailDlg, CDialogEx)
	ON_CBN_EDITCHANGE(IDC_DEV_ID_SETTING_COMBO, &CKdevStatusInfoDetailDlg::OnCbnEditchangeDevIdSettingCombo)
	ON_CBN_EDITUPDATE(IDC_DEV_ID_SETTING_COMBO, &CKdevStatusInfoDetailDlg::OnCbnEditupdateDevIdSettingCombo)
	ON_BN_CLICKED(IDC_QUERY_DEV_INFO_SETTING_BUTTON, &CKdevStatusInfoDetailDlg::OnBnClickedQueryDevInfoSettingButton)
	ON_BN_CLICKED(IDOK, &CKdevStatusInfoDetailDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CKdevStatusInfoDetailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_DevStatusInfoDetailListCtrl.SetExtendedStyle(m_DevStatusInfoDetailListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_DevStatusInfoDetailListCtrl.InsertColumn(0, TEXT("设备ID"), 0, 70);
	m_DevStatusInfoDetailListCtrl.InsertColumn(1, TEXT("所在服务器地址"), 0, 100);
	m_DevStatusInfoDetailListCtrl.InsertColumn(2, TEXT("最后上线时间"), 0, 115);
	m_DevStatusInfoDetailListCtrl.InsertColumn(3, TEXT("最初上线时间"), 0, 115);
	m_DevStatusInfoDetailListCtrl.InsertColumn(4, TEXT("设备通道数"), 0, 75);
	m_DevStatusInfoDetailListCtrl.InsertColumn(5, TEXT("硬件代码"), 0, 80);
	m_DevStatusInfoDetailListCtrl.InsertColumn(6, TEXT("固件版本"), 0, 60);
	m_DevStatusInfoDetailListCtrl.InsertColumn(7, TEXT("P2P版本"), 0, 60);

	m_DevStatusInfoDetailListCtrl.InsertColumn(8, TEXT("在线状态"), 0, 70);
	m_DevStatusInfoDetailListCtrl.InsertColumn(9, TEXT("网络类型"), 0, 90);
	m_DevStatusInfoDetailListCtrl.InsertColumn(10, TEXT("最后在线时间"), 0, 115);
	m_DevStatusInfoDetailListCtrl.InsertColumn(11, TEXT("设备IP"), 0, 90);
	m_DevStatusInfoDetailListCtrl.InsertColumn(12, TEXT("设备上线端口"), 0, 90);
	m_DevStatusInfoDetailListCtrl.InsertColumn(13, TEXT("协议类型"), 0, 70);
	m_DevStatusInfoDetailListCtrl.InsertItem(0, _T(""));
	//m_DevStatusInfoDetailListCtrl.RedrawWindow();

#if 0
	if (!m_Parent->m_EditDevID.IsEmpty()) {
		m_Parent->check_cur_devid_status_info_once(NULL, TRUE);
		m_DevSysInfoDetailListCtrl.SetItemText(0, 0, CA2W(m_Parent->m_CurSysInfo.id.c_str()));
		m_DevSysInfoDetailListCtrl.SetItemText(0, 1, CA2W(m_Parent->m_CurSysInfo.server.c_str()));
		m_DevSysInfoDetailListCtrl.SetItemText(0, 2, CA2W(m_Parent->m_CurSysInfo.last_time.c_str()));
		m_DevSysInfoDetailListCtrl.SetItemText(0, 3, CA2W(m_Parent->m_CurSysInfo.first_time.c_str()));
		m_DevSysInfoDetailListCtrl.SetItemText(0, 4, CA2W(m_Parent->m_CurSysInfo.ch_count.c_str()));
		m_DevSysInfoDetailListCtrl.SetItemText(0, 5, CA2W(m_Parent->m_CurSysInfo.hw_code.c_str()));
		m_DevSysInfoDetailListCtrl.SetItemText(0, 6, CA2W(m_Parent->m_CurSysInfo.fw_version.c_str()));
		m_DevSysInfoDetailListCtrl.SetItemText(0, 7, CA2W(m_Parent->m_CurSysInfo.version.c_str()));
		m_DevSysInfoDetailListCtrl.SetItemText(0, 8, CA2W(m_Parent->m_CurStatusInfo.status.c_str()));
		m_DevSysInfoDetailListCtrl.SetItemText(0, 9, CA2W(m_Parent->m_CurStatusInfo.net_type.c_str()));
		m_DevSysInfoDetailListCtrl.SetItemText(0, 10, CA2W(m_Parent->m_CurStatusInfo.last_time.c_str()));
		m_DevSysInfoDetailListCtrl.SetItemText(0, 11, CA2W(m_Parent->m_CurStatusInfo.ip.c_str()));
		m_DevSysInfoDetailListCtrl.SetItemText(0, 12, CA2W(m_Parent->m_CurStatusInfo.port.c_str()));
		m_DevSysInfoDetailListCtrl.SetItemText(0, 13, CA2W(m_Parent->m_CurStatusInfo.proto.c_str()));
	}
#endif

	return TRUE;
}

// CKdevStatusInfoDetailDlg 消息处理程序


void CKdevStatusInfoDetailDlg::OnCbnEditchangeDevIdSettingCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;

	m_CurQueryStatusLable.SetWindowTextW(_T(""));
	BOOL bRes = m_bitmap.DeleteObject();
	if (bRes) {
		m_hBmp = (HBITMAP)m_bitmap.GetSafeHandle();
		m_PicCtrlQueryStatus.SetBitmap(m_hBmp);
	}

	CString idStr = TEXT("");
	m_cDevIDComboBox.GetWindowTextW(idStr);
	if (idStr.IsEmpty()) {
		m_cDevIDComboBox.SetFocus();
		return;
	}

	for (int i = 0; i < idStr.GetLength() - 1; ++i) {
		wchar_t ch = idStr.GetAt(i);
		if (!isdigit(ch)) {
			nRes = MessageBox(_T("输入内容含有非数字信息，请输入数字"), _T("信息提示"), MB_OK);
			if (IDOK == nRes) {
				break;
			}
		}
	}
}


void CKdevStatusInfoDetailDlg::OnCbnEditupdateDevIdSettingCombo()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CKdevStatusInfoDetailDlg::SetMainParent(void* arg)
{
	m_Parent = (Ckp2p_check_tool_win32Dlg *)arg;
}


void CKdevStatusInfoDetailDlg::OnBnClickedQueryDevInfoSettingButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	INT i;
	INT_PTR nRes;
	CString temp;
	//CBitmap bitmap;  // CBitmap对象，用于加载位图   
	//HBITMAP hBmp;    // 保存CBitmap加载的位图的句柄 

	m_CurQueryStatusLable.SetWindowTextW(_T(""));
	BOOL bRes = m_bitmap.DeleteObject();
	if (bRes) {
		m_hBmp = (HBITMAP)m_bitmap.GetSafeHandle();
		m_PicCtrlQueryStatus.SetBitmap(m_hBmp);
	}

	m_cDevIDComboBox.GetWindowText(temp);
	if (temp.IsEmpty()) {
		nRes = MessageBox(_T("输入设备ID信息为空"), _T("信息提示"), MB_OK);
		if (IDOK == nRes) {
			m_cDevIDComboBox.SetFocus();
			return;
		}
	}

	INT nRet = m_Parent->check_cur_devid_status_info_once(temp.GetString(), TRUE);
	if ((nRet != 0 && nRet != -1) || (m_Parent->m_CurSysInfo.server.empty())) {
		m_CurQueryStatusLable.SetWindowTextW(_T("失败"));
		m_bitmap.LoadBitmap(IDB_QUERY_FAIL_BITMAP);  // 将位图IDB_BITMAP1加载到bitmap   
		m_hBmp = (HBITMAP)m_bitmap.GetSafeHandle();  // 获取bitmap加载位图的句柄   
		m_PicCtrlQueryStatus.SetBitmap(m_hBmp);
		return;
	}

	std::vector<CString>::iterator it;
	if (!m_CurDevIDHistoryVec.empty()) {
		it = find(m_CurDevIDHistoryVec.begin(), m_CurDevIDHistoryVec.end(), temp);
		if (it == m_CurDevIDHistoryVec.end()) {
			m_CurDevIDHistoryVec.insert(m_CurDevIDHistoryVec.begin(), temp);
			if (m_CurDevIDHistoryVec.size() >= 10) {

				SetEvent(m_Parent->m_QueryDevInfoHistoryCacheThreadUpdateEvent);
			}
			else {
				m_CurDevIDHistoryVec.push_back(temp);
				m_cDevIDComboBox.AddString(temp.GetString());
			}
		}
	}
	else {
		m_CurDevIDHistoryVec.push_back(temp);
		m_cDevIDComboBox.AddString(temp.GetString());
	}
	m_CurQueryStatusLable.SetWindowTextW(_T("成功"));

	m_bitmap.LoadBitmap(IDB_QUERY_SUCESS_BITMAP);  // 将位图IDB_BITMAP1加载到bitmap   
	m_hBmp = (HBITMAP)m_bitmap.GetSafeHandle();  // 获取bitmap加载位图的句柄   
	m_PicCtrlQueryStatus.SetBitmap(m_hBmp);

#if 0
	//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
	/*m_DevStatusInfoDetailListCtrl.SetItemText(0, 0, CA2W(m_Parent->m_CurSysInfo.id.c_str()));
	m_DevStatusInfoDetailListCtrl.SetItemText(0, 1, CA2W(m_Parent->m_CurSysInfo.server.c_str()));
	m_DevStatusInfoDetailListCtrl.SetItemText(0, 2, CA2W(m_Parent->m_CurSysInfo.last_time.c_str()));
	m_DevStatusInfoDetailListCtrl.SetItemText(0, 3, CA2W(m_Parent->m_CurSysInfo.first_time.c_str()));
	m_DevStatusInfoDetailListCtrl.SetItemText(0, 4, CA2W(m_Parent->m_CurSysInfo.ch_count.c_str()));
	m_DevStatusInfoDetailListCtrl.SetItemText(0, 5, CA2W(m_Parent->m_CurSysInfo.hw_code.c_str()));
	m_DevStatusInfoDetailListCtrl.SetItemText(0, 6, CA2W(m_Parent->m_CurSysInfo.fw_version.c_str()));
	m_DevStatusInfoDetailListCtrl.SetItemText(0, 7, CA2W(m_Parent->m_CurSysInfo.version.c_str()));
	m_DevStatusInfoDetailListCtrl.SetItemText(0, 8, CA2W(m_Parent->m_CurStatusInfo.status.c_str()));
	m_DevStatusInfoDetailListCtrl.SetItemText(0, 9, CA2W(m_Parent->m_CurStatusInfo.net_type.c_str()));
	m_DevStatusInfoDetailListCtrl.SetItemText(0, 10, CA2W(m_Parent->m_CurStatusInfo.last_time.c_str()));
	m_DevStatusInfoDetailListCtrl.SetItemText(0, 11, CA2W(m_Parent->m_CurStatusInfo.ip.c_str()));
	m_DevStatusInfoDetailListCtrl.SetItemText(0, 12, CA2W(m_Parent->m_CurStatusInfo.port.c_str()));
	m_DevStatusInfoDetailListCtrl.SetItemText(0, 13, CA2W(m_Parent->m_CurStatusInfo.proto.c_str()));*/
#endif

	if (index != 0) {
		for (i = 0; i < index; i++) {
			CString sa = m_DevStatusInfoDetailListCtrl.GetItemText(i, 0);
			if (sa.Compare(temp.GetString()) == 0) {
				//m_DevStatusInfoDetailListCtrl.InsertItem(i, _T(""));
				m_DevStatusInfoDetailListCtrl.SetItemText(i, 0, CA2W(m_Parent->m_CurSysInfo.id.c_str()));
				//m_DevStatusInfoDetailListCtrl.InsertItem(i, _T(""));
				m_DevStatusInfoDetailListCtrl.SetItemText(i, 1, CA2W(m_Parent->m_CurSysInfo.server.c_str()));
				//m_DevStatusInfoDetailListCtrl.InsertItem(i, _T(""));
				m_DevStatusInfoDetailListCtrl.SetItemText(i, 2, CA2W(m_Parent->m_CurSysInfo.last_time.c_str()));
				//m_DevStatusInfoDetailListCtrl.InsertItem(i, _T(""));
				m_DevStatusInfoDetailListCtrl.SetItemText(i, 3, CA2W(m_Parent->m_CurSysInfo.first_time.c_str()));
				//m_DevStatusInfoDetailListCtrl.InsertItem(i, _T(""));
				m_DevStatusInfoDetailListCtrl.SetItemText(i, 4, CA2W(m_Parent->m_CurSysInfo.ch_count.c_str()));
				//m_DevStatusInfoDetailListCtrl.InsertItem(i, _T(""));
				m_DevStatusInfoDetailListCtrl.SetItemText(i, 5, CA2W(m_Parent->m_CurSysInfo.hw_code.c_str()));
				//m_DevStatusInfoDetailListCtrl.InsertItem(i, _T(""));
				m_DevStatusInfoDetailListCtrl.SetItemText(i, 6, CA2W(m_Parent->m_CurSysInfo.fw_version.c_str()));
				//m_DevStatusInfoDetailListCtrl.InsertItem(i, _T(""));
				m_DevStatusInfoDetailListCtrl.SetItemText(i, 7, CA2W(m_Parent->m_CurSysInfo.version.c_str()));
				//m_DevStatusInfoDetailListCtrl.InsertItem(i, _T(""));
				m_DevStatusInfoDetailListCtrl.SetItemText(i, 8, CA2W(m_Parent->m_CurStatusInfo.status.c_str()));
				//m_DevStatusInfoDetailListCtrl.InsertItem(i, _T(""));
				m_DevStatusInfoDetailListCtrl.SetItemText(i, 9, CA2W(m_Parent->m_CurStatusInfo.net_type.c_str()));
				//m_DevStatusInfoDetailListCtrl.InsertItem(i, _T(""));
				m_DevStatusInfoDetailListCtrl.SetItemText(i, 10, CA2W(m_Parent->m_CurStatusInfo.last_time.c_str()));
				//m_DevStatusInfoDetailListCtrl.InsertItem(i, _T(""));
				m_DevStatusInfoDetailListCtrl.SetItemText(i, 11, CA2W(m_Parent->m_CurStatusInfo.ip.c_str()));
				//m_DevStatusInfoDetailListCtrl.InsertItem(i, _T(""));
				m_DevStatusInfoDetailListCtrl.SetItemText(i, 12, CA2W(m_Parent->m_CurStatusInfo.port.c_str()));
				//m_DevStatusInfoDetailListCtrl.InsertItem(i, _T(""));
				m_DevStatusInfoDetailListCtrl.SetItemText(i, 13, CA2W(m_Parent->m_CurStatusInfo.proto.c_str()));
				m_DevStatusInfoDetailListCtrl.RedrawWindow();
				break;
			}
		}
		if (i == index ) {
			m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
			m_DevStatusInfoDetailListCtrl.SetItemText(index, 0, CA2W(m_Parent->m_CurSysInfo.id.c_str()));
			//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
			m_DevStatusInfoDetailListCtrl.SetItemText(index, 1, CA2W(m_Parent->m_CurSysInfo.server.c_str()));
			//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
			m_DevStatusInfoDetailListCtrl.SetItemText(index, 2, CA2W(m_Parent->m_CurSysInfo.last_time.c_str()));
			//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
			m_DevStatusInfoDetailListCtrl.SetItemText(index, 3, CA2W(m_Parent->m_CurSysInfo.first_time.c_str()));
			//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
			m_DevStatusInfoDetailListCtrl.SetItemText(index, 4, CA2W(m_Parent->m_CurSysInfo.ch_count.c_str()));
			//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
			m_DevStatusInfoDetailListCtrl.SetItemText(index, 5, CA2W(m_Parent->m_CurSysInfo.hw_code.c_str()));
			//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
			m_DevStatusInfoDetailListCtrl.SetItemText(index, 6, CA2W(m_Parent->m_CurSysInfo.fw_version.c_str()));
			//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
			m_DevStatusInfoDetailListCtrl.SetItemText(index, 7, CA2W(m_Parent->m_CurSysInfo.version.c_str()));
			//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
			m_DevStatusInfoDetailListCtrl.SetItemText(index, 8, CA2W(m_Parent->m_CurStatusInfo.status.c_str()));
			//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
			m_DevStatusInfoDetailListCtrl.SetItemText(index, 9, CA2W(m_Parent->m_CurStatusInfo.net_type.c_str()));
			//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
			m_DevStatusInfoDetailListCtrl.SetItemText(index, 10, CA2W(m_Parent->m_CurStatusInfo.last_time.c_str()));
			//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
			m_DevStatusInfoDetailListCtrl.SetItemText(index, 11, CA2W(m_Parent->m_CurStatusInfo.ip.c_str()));
			//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
			m_DevStatusInfoDetailListCtrl.SetItemText(index, 12, CA2W(m_Parent->m_CurStatusInfo.port.c_str()));
			//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
			m_DevStatusInfoDetailListCtrl.SetItemText(index, 13, CA2W(m_Parent->m_CurStatusInfo.proto.c_str()));
			index++;
			m_DevStatusInfoDetailListCtrl.RedrawWindow();
		}
	}
	else {
		//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
		m_DevStatusInfoDetailListCtrl.SetItemText(index, 0, CA2W(m_Parent->m_CurSysInfo.id.c_str()));
		//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
		m_DevStatusInfoDetailListCtrl.SetItemText(index, 1, CA2W(m_Parent->m_CurSysInfo.server.c_str()));
		//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
		m_DevStatusInfoDetailListCtrl.SetItemText(index, 2, CA2W(m_Parent->m_CurSysInfo.last_time.c_str()));
		//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
		m_DevStatusInfoDetailListCtrl.SetItemText(index, 3, CA2W(m_Parent->m_CurSysInfo.first_time.c_str()));
		//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
		m_DevStatusInfoDetailListCtrl.SetItemText(index, 4, CA2W(m_Parent->m_CurSysInfo.ch_count.c_str()));
		//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
		m_DevStatusInfoDetailListCtrl.SetItemText(index, 5, CA2W(m_Parent->m_CurSysInfo.hw_code.c_str()));
		//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
		m_DevStatusInfoDetailListCtrl.SetItemText(index, 6, CA2W(m_Parent->m_CurSysInfo.fw_version.c_str()));
		//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
		m_DevStatusInfoDetailListCtrl.SetItemText(index, 7, CA2W(m_Parent->m_CurSysInfo.version.c_str()));
		//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
		m_DevStatusInfoDetailListCtrl.SetItemText(index, 8, CA2W(m_Parent->m_CurStatusInfo.status.c_str()));
		//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
		m_DevStatusInfoDetailListCtrl.SetItemText(index, 9, CA2W(m_Parent->m_CurStatusInfo.net_type.c_str()));
		//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
		m_DevStatusInfoDetailListCtrl.SetItemText(index, 10, CA2W(m_Parent->m_CurStatusInfo.last_time.c_str()));
		//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
		m_DevStatusInfoDetailListCtrl.SetItemText(index, 11, CA2W(m_Parent->m_CurStatusInfo.ip.c_str()));
		//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
		m_DevStatusInfoDetailListCtrl.SetItemText(index, 12, CA2W(m_Parent->m_CurStatusInfo.port.c_str()));
		//m_DevStatusInfoDetailListCtrl.InsertItem(index, _T(""));
		m_DevStatusInfoDetailListCtrl.SetItemText(index, 13, CA2W(m_Parent->m_CurStatusInfo.proto.c_str()));
		index++;
		m_DevStatusInfoDetailListCtrl.RedrawWindow();
	}

}


void CKdevStatusInfoDetailDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_DevStatusInfoDetailListCtrl.DeleteAllItems();
	index = 0;
	CDialogEx::OnOK();
}
