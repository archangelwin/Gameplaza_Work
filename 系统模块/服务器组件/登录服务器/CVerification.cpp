#include "StdAfx.h"
#include <vector>
#include "CVerification.h"
#include "AttemperEngineSink.h"


CVerification* CVerification::m_pInstance = 0;
int CVerification::m_iThreadRequestNum = 0;
int CVerification::m_iThreadSendNum = 0;
std::vector<tagMBVerifInfoThread> CVerification::G_vThreadRequestMobileNum;


CVerification::CVerification()
{

}

void CVerification::StartThread(LPVOID lpParameter)
{
	strThreadLogDir = GetFileDialogPath()+"\\log";
	m_iThreadRequestNum = 0;
	m_iThreadSendNum = 0;
	G_vThreadRequestMobileNum.clear();
	hSendhandle = (HANDLE)_beginthreadex(NULL, 0, SendThreadFunProc, lpParameter, 0, NULL); 
}

//发送验证码消息协议线程函数
unsigned __stdcall CVerification::SendThreadFunProc(LPVOID lpParameter)
{  
	Sleep(1000);
	tagMBVerifInfoThread sFirstValue;
	SMSInfo p;
	const char *data[2];
	data[1]="10";
	while(TRUE)
	{

		if (lpParameter == NULL)
		{
			return 0;
		}
		if (m_iThreadRequestNum <= m_iThreadSendNum)
		{
			Sleep(200);
			//CTraceService::TraceString(TEXT("线程1没有数据"),TraceLevel_Warning);
			continue;
		}
		sFirstValue = G_vThreadRequestMobileNum[m_iThreadSendNum];
		CAttemperEngineSink* pAttempEngineSink = (CAttemperEngineSink*)lpParameter;
		BYTE cbDataBuffer[SOCKET_TCP_PACKET];
		CMD_GPO_MBVerificationCode * pCMDMBVerifyRes=(CMD_GPO_MBVerificationCode *)cbDataBuffer;

		//设置变量
		ZeroMemory(pCMDMBVerifyRes,sizeof(CMD_GPO_MBVerificationCode));
		TCHAR c_szMobilePhone[LEN_MOBILE_PHONE];
		lstrcpyn(c_szMobilePhone,sFirstValue.szMobilePhone,CountArray(c_szMobilePhone));
#ifdef _UNICODE
		char c_szMobilePhone[LEN_MOBILE_PHONE];
		wcstombs(c_szMobilePhone, sFirstValue.szMobilePhone, wcslen(c_wText) + 1);
		data[0]=ConvertLPWSTRToLPSTR(sFirstValue.szVerificationCode);
#else
		data[0]=sFirstValue.szVerificationCode;
#endif

		int ret=sendTemplateSMS(&p,c_szMobilePhone,pAttempEngineSink->m_SMSVerifyTemplateID,data,2);
		if (ret == 0)
		{
			pCMDMBVerifyRes->bIsVerificationCodeSend = true;
		}
		else
		{
			pCMDMBVerifyRes->bIsVerificationCodeSend = false;
			lstrcpyn(pCMDMBVerifyRes->szDescribeString,p.reason.statusMsg,CountArray(pCMDMBVerifyRes->szDescribeString));
		}
		WORD wSendSize=sizeof(CMD_GPO_MBVerificationCode);
		DWORD dwContextID = sFirstValue.dSocketId;
		pAttempEngineSink->m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GPO_MBCODE_VERIFY,cbDataBuffer,wSendSize);
		m_iThreadSendNum++;
		//SendThreadMsgRecord(c_szMobilePhone,p);
	}
	return 0;  
}


void CVerification::SendThreadMsgRecord(char * cMobileNum,SMSInfo &p)
{
	CString strName = strThreadLogDir+"\\ThreadSendMsg.log";
	SYSTEMTIME sys; 
	GetLocalTime(&sys);
	char szTime[100];
	sprintf( szTime, "第%d个发送手机号%s:%d:%d:%s错误码:%d\r\n",m_iThreadSendNum,cMobileNum,sys.wMinute,sys.wSecond,p.reason.statusMsg,p.reason.statusCode);
	CString strMsg(szTime);
	CFile fLog;
	if(fLog.Open( strName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite ))
	{

		fLog.SeekToEnd(); 	
		int strLength=strMsg.GetLength();
#ifdef _UNICODE
		BYTE bom[2] = {0xff, 0xfe};
		fLog.Write(bom,sizeof(BYTE)*2);
		strLength*=2;
#endif
		fLog.Write((LPCTSTR)strMsg,strLength);
		fLog.Close();
	}
}

void CVerification::ThreadRecvMsgRecord(char * cMobileNum)
{
	CString strName = strThreadLogDir+"\\ThreadReceMsg.log";
	SYSTEMTIME sys; 
	GetLocalTime(&sys);
	char szTime[100];
	sprintf( szTime, "第%d个接受手机号:%s:%d:%d\r\n",m_iThreadRequestNum,cMobileNum,sys.wMinute,sys.wSecond);
	CString strMsg(szTime);
	CFile fLog;
	if(fLog.Open( strName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite ))
	{

		fLog.SeekToEnd(); 	
		int strLength=strMsg.GetLength();
#ifdef _UNICODE
		BYTE bom[2] = {0xff, 0xfe};
		fLog.Write(bom,sizeof(BYTE)*2);
		strLength*=2;
#endif
		fLog.Write((LPCTSTR)strMsg,strLength);
		fLog.Close();
	}
}

CString CVerification::GetFileDialogPath()
{
	CString strFileDlgPath;
	TCHAR szModuleDirectory[MAX_PATH];	//模块目录
	GetModuleFileName(AfxGetInstanceHandle(),szModuleDirectory,sizeof(szModuleDirectory));
	int nModuleLen=lstrlen(szModuleDirectory);
	int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".EXE")) + 1;
	if (nModuleLen<=nProcessLen) 
		return TEXT("");
	szModuleDirectory[nModuleLen-nProcessLen]=0;
	strFileDlgPath = szModuleDirectory;
	return strFileDlgPath;
}

void CVerification::EndThread()
{
	m_iThreadRequestNum = 0;
	m_iThreadSendNum = 0;
	G_vThreadRequestMobileNum.clear();
	TerminateThread(hSendhandle,0);
	CloseHandle(hSendhandle);
}

void CVerification::AddNewVerificationCode(DWORD dwSocketID,char * szVerificationCode,char * szMobilePhone)
{
	tagMBVerifInfoThread info;
	info.dSocketId = dwSocketID;
	lstrcpyn(info.szVerificationCode,szVerificationCode,CountArray(info.szVerificationCode));
	lstrcpyn(info.szMobilePhone,szMobilePhone,CountArray(info.szMobilePhone));
	if ((m_iThreadSendNum>=10000)&&(m_iThreadSendNum>=m_iThreadRequestNum))
	{
		G_vThreadRequestMobileNum.clear();
		m_iThreadRequestNum = 0;
		m_iThreadSendNum = 0;
	}
	G_vThreadRequestMobileNum.push_back(info);
	m_iThreadRequestNum++;
	ThreadRecvMsgRecord(info.szMobilePhone);

	return ;
}

void CVerification::OnRecordSendLoginNum(DWORD dwSendNum)
{
	CString strName = strThreadLogDir+"\\LoginQueueRecord.log";
	SYSTEMTIME sys; 
	GetLocalTime(&sys);
	char szTime[100];
	sprintf( szTime, "发送给数据库%d个登录请求:%d:%d:%d:%d:%d:%d\r\n",dwSendNum,sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
	CString strMsg(szTime);
	CFile fLog;
	if(fLog.Open( strName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite ))
	{

		fLog.SeekToEnd(); 	
		int strLength=strMsg.GetLength();
#ifdef _UNICODE
		BYTE bom[2] = {0xff, 0xfe};
		fLog.Write(bom,sizeof(BYTE)*2);
		strLength*=2;
#endif
		fLog.Write((LPCTSTR)strMsg,strLength);
		fLog.Close();
	}
}

void CVerification::OnRecordFinishLoginNum(DWORD dwSendNum)
{
	CString strName = strThreadLogDir+"\\LoginQueueRecord.log";
	SYSTEMTIME sys; 
	GetLocalTime(&sys);
	char szTime[100];
	sprintf( szTime, "完成一个登录信息后数据库还要处理%d个登录请求:%d:%d:%d\r\n",dwSendNum,sys.wHour,sys.wMinute,sys.wSecond);
	CString strMsg(szTime);
	CFile fLog;
	if(fLog.Open( strName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite ))
	{

		fLog.SeekToEnd(); 	
		int strLength=strMsg.GetLength();
#ifdef _UNICODE
		BYTE bom[2] = {0xff, 0xfe};
		fLog.Write(bom,sizeof(BYTE)*2);
		strLength*=2;
#endif
		fLog.Write((LPCTSTR)strMsg,strLength);
		fLog.Close();
	}
}
