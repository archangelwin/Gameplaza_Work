#include "Stdafx.h"
#include "HelpServer.h"
#include "HelpServerDlg.h"

//////////////////////////////////////////////////////////////////////////////////

//�������
CHelpServerApp theApp;

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CHelpServerApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CHelpServerApp::CHelpServerApp()
{
}

//��������
BOOL CHelpServerApp::InitInstance()
{
	__super::InitInstance();

	//�������
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//����ע���
	SetRegistryKey(szProduct);

	//��ʾ����
	CHelpServerDlg HelpServerDlg;
	m_pMainWnd=&HelpServerDlg;
	HelpServerDlg.DoModal();

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////
