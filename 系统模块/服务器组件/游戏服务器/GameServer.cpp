#include "Stdafx.h"
#include "GameServer.h"
#include "GameServerDlg.h"

#ifndef _DEBUG
#include "../../../������/crash/CrashRpt.h"
#pragma comment(lib, "../../../������/crash/CrashRpt.lib")
BOOL WINAPI crash(PEXCEPTION_POINTERS Exception)
{
	char szFileName[260];

	//��ȡʱ��
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

//�������
CGameServerApp theApp;

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameServerApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameServerApp::CGameServerApp()
{
#ifndef _DEBUG
	InitializeCrashRptEx(crash);
#endif
}

//��������
BOOL CGameServerApp::InitInstance()
{
	__super::InitInstance();

	//�������
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//����ע���
	SetRegistryKey(szProduct);

	//��ʾ����
	CGameServerDlg GameServerDlg;
	m_pMainWnd=&GameServerDlg;
	GameServerDlg.DoModal();

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////
