#include "Stdafx.h"
#include "HelpServer.h"
#include "HelpServerDlg.h"

//////////////////////////////////////////////////////////////////////////////////

//程序对象
CHelpServerApp theApp;

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CHelpServerApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CHelpServerApp::CHelpServerApp()
{
}

//启动函数
BOOL CHelpServerApp::InitInstance()
{
	__super::InitInstance();

	//设置组件
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//设置注册表
	SetRegistryKey(szProduct);

	//显示窗口
	CHelpServerDlg HelpServerDlg;
	m_pMainWnd=&HelpServerDlg;
	HelpServerDlg.DoModal();

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////
