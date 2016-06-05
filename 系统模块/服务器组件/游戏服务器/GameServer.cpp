#include "Stdafx.h"
#include "GameServer.h"
#include "GameServerDlg.h"

#ifndef _DEBUG
#include "../../../开发库/crash/CrashRpt.h"
#pragma comment(lib, "../../../开发库/crash/CrashRpt.lib")
BOOL WINAPI crash(PEXCEPTION_POINTERS Exception)
{
	char szFileName[260];

	//获取时间
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);

	_snprintf(szFileName,sizeof(szFileName),"GameServer%04d-%02d-%02d_%02d-%02d-%02d.dmp",SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);

	HANDLE lHandle=CreateFile(szFileName,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
	CloseHandle(lHandle);

	GenerateMiniDumpA(Exception,szFileName);

	return EXCEPTION_EXECUTE_HANDLER;
}
#endif

//////////////////////////////////////////////////////////////////////////////////

//程序对象
CGameServerApp theApp;

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameServerApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameServerApp::CGameServerApp()
{
#ifndef _DEBUG
	InitializeCrashRptEx(crash);
#endif
}

//启动函数
BOOL CGameServerApp::InitInstance()
{
	__super::InitInstance();

	//设置组件
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//设置注册表
	SetRegistryKey(szProduct);

	//显示窗口
	CGameServerDlg GameServerDlg;
	m_pMainWnd=&GameServerDlg;
	GameServerDlg.DoModal();

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////
