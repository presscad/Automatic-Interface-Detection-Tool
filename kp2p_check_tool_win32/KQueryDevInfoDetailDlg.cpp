// KQueryDevInfoDetailDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "kp2p_check_tool_win32.h"
#include "KQueryDevInfoDetailDlg.h"
#include "afxdialogex.h"


// CKQueryDevInfoDetailDlg �Ի���

IMPLEMENT_DYNAMIC(CKQueryDevInfoDetailDlg, CDialogEx)

CKQueryDevInfoDetailDlg::CKQueryDevInfoDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_QUERY_DEV_INFO_DIALOG, pParent)
{

}

CKQueryDevInfoDetailDlg::~CKQueryDevInfoDetailDlg()
{
}

void CKQueryDevInfoDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_QUERY_DEV_INFO_LIST, m_ListCtrlQueryInfo);
}


BEGIN_MESSAGE_MAP(CKQueryDevInfoDetailDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CKQueryDevInfoDetailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ListCtrlQueryInfo.SetExtendedStyle(m_ListCtrlQueryInfo.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ListCtrlQueryInfo.InsertColumn(0, TEXT("������"), 0, 100);
	m_ListCtrlQueryInfo.InsertColumn(1, TEXT("��Ϣ"), 0, 300);
	//m_ListCtrlQueryInfo.InsertItem(0, _T(""));
	/*m_ListCtrlQueryInfo.SetItemText(0, 0, TEXT("�豸ID"));
	m_ListCtrlQueryInfo.InsertItem(1, _T(""));
	m_ListCtrlQueryInfo.SetItemText(1, 0, TEXT("���ڷ�������ַ"));
	m_ListCtrlQueryInfo.InsertItem(2, _T(""));
	m_ListCtrlQueryInfo.SetItemText(2, 0, TEXT("�������ʱ��"));
	m_ListCtrlQueryInfo.InsertItem(3, _T(""));
	m_ListCtrlQueryInfo.SetItemText(3, 0, TEXT("�������ʱ��"));
	m_ListCtrlQueryInfo.InsertItem(4, _T(""));
	m_ListCtrlQueryInfo.SetItemText(4, 0, TEXT("�豸ͨ����"));
	m_ListCtrlQueryInfo.InsertItem(5, _T(""));
	m_ListCtrlQueryInfo.SetItemText(5, 0, TEXT("Ӳ������"));
	m_ListCtrlQueryInfo.InsertItem(6, _T(""));
	m_ListCtrlQueryInfo.SetItemText(6, 0, TEXT("�̼��汾"));
	m_ListCtrlQueryInfo.InsertItem(7, _T(""));
	m_ListCtrlQueryInfo.SetItemText(7, 0, TEXT("P2P�汾"));
	m_ListCtrlQueryInfo.InsertItem(8, _T(""));
	m_ListCtrlQueryInfo.SetItemText(8, 0, TEXT("����״̬"));
	m_ListCtrlQueryInfo.InsertItem(9, _T(""));
	m_ListCtrlQueryInfo.SetItemText(9, 0, TEXT("��������"));
	m_ListCtrlQueryInfo.InsertItem(10, _T(""));
	m_ListCtrlQueryInfo.SetItemText(10, 0, TEXT("�������ʱ��"));
	m_ListCtrlQueryInfo.InsertItem(11, _T(""));
	m_ListCtrlQueryInfo.SetItemText(11, 0, TEXT("�豸IP"));
	m_ListCtrlQueryInfo.InsertItem(12, _T(""));
	m_ListCtrlQueryInfo.SetItemText(12, 0, TEXT("�豸���߶˿�"));
	m_ListCtrlQueryInfo.InsertItem(13, _T(""));
	m_ListCtrlQueryInfo.SetItemText(13, 0, TEXT("Э������"));*/
	//UpdateData(FALSE);

#if 0
	m_ListCtrlQueryInfo.InsertColumn(0, TEXT("�豸ID"), 0, 100);
	m_ListCtrlQueryInfo.InsertColumn(1, TEXT("���ڷ�������ַ"), 0, 100);
	m_ListCtrlQueryInfo.InsertColumn(2, TEXT("�������ʱ��"), 0, 100);
	m_ListCtrlQueryInfo.InsertColumn(3, TEXT("�������ʱ��"), 0, 100);
	m_ListCtrlQueryInfo.InsertColumn(4, TEXT("�豸ͨ����"), 0, 100);
	m_ListCtrlQueryInfo.InsertColumn(5, TEXT("Ӳ������"), 0, 100);
	m_ListCtrlQueryInfo.InsertColumn(6, TEXT("�̼��汾"), 0, 100);
	m_ListCtrlQueryInfo.InsertColumn(7, TEXT("P2P�汾"), 0, 100);

	m_ListCtrlQueryInfo.InsertColumn(8, TEXT("����״̬"), 0, 100);
	m_ListCtrlQueryInfo.InsertColumn(9, TEXT("��������"), 0, 100);
	m_ListCtrlQueryInfo.InsertColumn(10, TEXT("�������ʱ��"), 0, 100);
	m_ListCtrlQueryInfo.InsertColumn(11, TEXT("�豸IP��ַ"), 0, 100);
	m_ListCtrlQueryInfo.InsertColumn(12, TEXT("�豸���߶˿�"), 0, 100);
	m_ListCtrlQueryInfo.InsertColumn(13, TEXT("Э������"), 0, 100);
	m_ListCtrlQueryInfo.InsertItem(0, _T(""));
#endif

	return TRUE;
}


// CKQueryDevInfoDetailDlg ��Ϣ�������
