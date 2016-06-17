#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"
//////////////////////////////////////////////////////////////////////////////////


CString GetFileDialogPath2();
int GloginProcCost=0;
//构造函数
CDataBaseEngineSink::CDataBaseEngineSink()
{
	//成员变量
	m_uiRandCodeCnt=0;

	//组件变量
	m_pInitParameter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return;
}

//析构函数
CDataBaseEngineSink::~CDataBaseEngineSink()
{
}

//接口查询
VOID * CDataBaseEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngineSink,Guid,dwQueryVer);
	return NULL;
}

//启动事件
bool CDataBaseEngineSink::OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)
{
	//创建对象
	if ((m_AccountsDBModule.GetInterface()==NULL)&&(m_AccountsDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//创建对象
	if ((m_TreasureDBModule.GetInterface()==NULL)&&(m_TreasureDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//创建对象
	if ((m_PlatformDBModule.GetInterface()==NULL)&&(m_PlatformDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//创建对象
	//if ((m_LogServerDBModule.GetInterface()==NULL)&&(m_LogServerDBModule.CreateInstance()==false))
	//{
	//	CTraceService::TraceString(TEXT("LogServer 创建失败"),TraceLevel_Exception);	}
	//else
	//{
	//	try
	//	{
	//		TCHAR szPath[MAX_PATH]=TEXT("");
	//		TCHAR szConfigFileName[MAX_PATH] = TEXT("");
	//		TCHAR szServerName[50] = TEXT("");
	//		TCHAR szDBName[50] = TEXT("");
	//		TCHAR szLogServerIPAddr[50] = TEXT("");
	//		TCHAR szLogServerUserName[50] = TEXT("");
	//		TCHAR szLogserverUserPwd[50] = TEXT("");
	//		GetCurrentDirectory(sizeof(szPath),szPath);
	//		_sntprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\LogServerConfig.ini"),szPath);

	//		WORD wServerType = GetPrivateProfileInt(TEXT("Config"),TEXT("LogServerType"),1,szConfigFileName);
	//		WORD wLogServerPort = 0;
	//		if (wServerType == 1)
	//		{
	//			lstrcpyn(szServerName,TEXT("ProductServer"),CountArray(szServerName));
	//		} 
	//		else
	//		{
	//			lstrcpyn(szServerName,TEXT("TestServer"),CountArray(szServerName));
	//		}

	//		// Read the content of the log server
	//		GetPrivateProfileString(szServerName,TEXT("IPAddress"),TEXT(""),szLogServerIPAddr,CountArray(szLogServerIPAddr),szConfigFileName);
	//		GetPrivateProfileString(szServerName,TEXT("DBName"),TEXT(""),szDBName,CountArray(szDBName),szConfigFileName);
	//		GetPrivateProfileString(szServerName,TEXT("UserName"),TEXT(""),szLogServerUserName,CountArray(szLogServerUserName),szConfigFileName);
	//		GetPrivateProfileString(szServerName,TEXT("Password"),TEXT(""),szLogserverUserPwd,CountArray(szLogserverUserPwd),szConfigFileName);

	//		wLogServerPort = GetPrivateProfileInt(szServerName,TEXT("PORT"),22588,szConfigFileName);

	//		m_LogServerDBModule->SetConnectionInfo(szLogServerIPAddr,wLogServerPort,
	//			szDBName,szLogServerUserName,szLogserverUserPwd,
	//			"登录服务器","LogServerDBParameter");

	//		//发起连接
	//		m_LogServerDBModule->OpenConnection();
	//		m_LogServerDBAide.SetDataBase(m_LogServerDBModule.GetInterface());

	//		// 显示连接日志服务器的类型和地址
	//		TCHAR pszDescribe[MAX_PATH]=TEXT("");
	//		_sntprintf(pszDescribe,CountArray(pszDescribe),TEXT("ServerType : %s Link success, IPAddress : %s, PORT : %ld"),szServerName, szLogServerIPAddr, wLogServerPort);
	//		CTraceService::TraceString(pszDescribe,TraceLevel_Normal);
	//	}
	//	catch (IDataBaseException * pIException)
	//	{
	//		//错误信息
	//		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
	//		CTraceService::TraceString(TEXT("logserver error"),TraceLevel_Exception);
	//		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	//	}
	//}

	try
	{
		//连接信息
		tagDataBaseParameter * pAccountsDBParameter=&m_pInitParameter->m_AccountsDBParameter;
		tagDataBaseParameter * pTreasureDBParameter=&m_pInitParameter->m_TreasureDBParameter;
		tagDataBaseParameter * pPlatformDBParameter=&m_pInitParameter->m_PlatformDBParameter;

		//设置连接
		m_AccountsDBModule->SetConnectionInfo(pAccountsDBParameter->szDataBaseAddr,pAccountsDBParameter->wDataBasePort,
			pAccountsDBParameter->szDataBaseName,pAccountsDBParameter->szDataBaseUser,pAccountsDBParameter->szDataBasePass,
			"协调服务器","AccountsDBModule");
		m_TreasureDBModule->SetConnectionInfo(pTreasureDBParameter->szDataBaseAddr,pTreasureDBParameter->wDataBasePort,
			pTreasureDBParameter->szDataBaseName,pTreasureDBParameter->szDataBaseUser,pTreasureDBParameter->szDataBasePass,
			"协调服务器","TreasureDBModule");
		m_PlatformDBModule->SetConnectionInfo(pPlatformDBParameter->szDataBaseAddr,pPlatformDBParameter->wDataBasePort,
			pPlatformDBParameter->szDataBaseName,pPlatformDBParameter->szDataBaseUser,pPlatformDBParameter->szDataBasePass,
			"协调服务器","PlatformDBModule");

		//发起连接
		m_AccountsDBModule->OpenConnection();
		m_AccountsDBAide.SetDataBase(m_AccountsDBModule.GetInterface());

		//发起连接
		m_TreasureDBModule->OpenConnection();
		m_TreasureDBAide.SetDataBase(m_TreasureDBModule.GetInterface());

		//发起连接
		m_PlatformDBModule->OpenConnection();
		m_PlatformDBAide.SetDataBase(m_PlatformDBModule.GetInterface());



		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		return false;
	}



	return true;
}

//停止事件
bool CDataBaseEngineSink::OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx)
{
	//设置对象
	m_AccountsDBAide.SetDataBase(NULL);
	m_TreasureDBAide.SetDataBase(NULL);
	m_PlatformDBAide.SetDataBase(NULL);

	//关闭连接
	if (m_AccountsDBModule.GetInterface()!=NULL)
	{
		m_AccountsDBModule->CloseConnection();
		m_AccountsDBModule.CloseInstance();
	}

	//关闭连接
	if (m_TreasureDBModule.GetInterface()!=NULL)
	{
		m_TreasureDBModule->CloseConnection();
		m_TreasureDBModule.CloseInstance();
	}

	//关闭连接
	if (m_PlatformDBModule.GetInterface()!=NULL)
	{
		m_PlatformDBModule->CloseConnection();
		m_PlatformDBModule.CloseInstance();
	}

	//组件变量
	m_pInitParameter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return true;
}

//时间事件
bool CDataBaseEngineSink::OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
	return false;
}

//控制事件
bool CDataBaseEngineSink::OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize)
{
	return false;
}

//请求事件
//请求事件
bool CDataBaseEngineSink::OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBR_GC_INSERT_MATCH_SIGNUP_USER:			//标识登录
		{
			return OnRequestInsertMatchSignUpUser(dwContextID,pData,wDataSize);
		}
	case DBR_GR_LOAD_MATCH_CONFIG:   //加载比赛配置信息
		{
			return OnRequestLoadMatchConfig(dwContextID,pData,wDataSize);
		}
	case DBR_GR_SAVE_MATCH_RESULT:
		{
			return OnRequestSaveMatchResult(dwContextID,pData,wDataSize);
		}
	case DBR_GR_GET_SQL_TIME:
		{
			return OnRequestGetSqlTime(dwContextID,pData,wDataSize);
		}
	}

	return false;
}

//插入比赛信息 
bool CDataBaseEngineSink::OnRequestInsertMatchSignUpUser(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GC_InsertMatchSignUpUser));
	if (wDataSize!=sizeof(DBR_GC_InsertMatchSignUpUser)) return false;

	//执行查询
	DBR_GC_InsertMatchSignUpUser * pInsertMatchSignUpUser=(DBR_GC_InsertMatchSignUpUser *)pData;

	try
	{
		//构造参数
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pInsertMatchSignUpUser->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@MatchNum"),pInsertMatchSignUpUser->nMatchID);
		m_TreasureDBAide.AddParameter(TEXT("@bIsInGame"),pInsertMatchSignUpUser->bIsInGame);
		m_TreasureDBAide.AddParameter(TEXT("@llUserScoreInGame"),pInsertMatchSignUpUser->llUserScore);
		time_t rawtime;
		struct tm * timeinfo;
		TCHAR buffer[80];
		time (&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer,80,"%Y-%m-%d",timeinfo);
		m_TreasureDBAide.AddParameter(TEXT("@MatchDay"),buffer);
		//m_TreasureDBAide.AddParameter(TEXT("@MatchType"),pInsertMatchSignUpUser->nMatchID);
		m_TreasureDBAide.AddParameter(TEXT("@GroupID"),pInsertMatchSignUpUser->wGroupID);

		//输出参数
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//执行查询
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_INSERT_MATCH_SIGNUP_USER"),true);

		// Get the return value 
		DBO_GC_Insert_Match_SignUp_User stDBOInsertMatchSignUPUser = {0};
		stDBOInsertMatchSignUPUser.dwUserID = m_TreasureDBAide.GetValue_LONG(TEXT("UserID"));
		stDBOInsertMatchSignUPUser.wServerID = pInsertMatchSignUpUser->wServerID;
		stDBOInsertMatchSignUPUser.nMatchID = m_TreasureDBAide.GetValue_LONG(TEXT("MatchID"));
		stDBOInsertMatchSignUPUser.bMatchStatus = m_TreasureDBAide.GetValue_LONG(TEXT("MatchStatus"));
		stDBOInsertMatchSignUPUser.bIsInGame = pInsertMatchSignUpUser->bIsInGame;
		stDBOInsertMatchSignUPUser.wGroupID = pInsertMatchSignUpUser->wGroupID;
		m_TreasureDBAide.GetValue_String(TEXT("NickName"),stDBOInsertMatchSignUPUser.szNickName,CountArray(stDBOInsertMatchSignUPUser.szNickName));
		stDBOInsertMatchSignUPUser.nMatchType = m_TreasureDBAide.GetValue_LONG(TEXT("MatchType"));
		stDBOInsertMatchSignUPUser.wEnrollmentFee = m_TreasureDBAide.GetValue_LONG(TEXT("EnrollmentFee"));
		//结果处理
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		lstrcpyn(stDBOInsertMatchSignUPUser.szDescription,CW2CT(DBVarValue.bstrVal),CountArray(stDBOInsertMatchSignUPUser.szDescription));

		//检测用户身上的分数是否够报名费, 并且没有报名
		//if (stDBOInsertMatchSignUPUser.wEnrollmentFee > pInsertMatchSignUpUser->llUserScore && !stDBOInsertMatchSignUPUser.bMatchStatus)
		//{
		//	lResultCode = DB_ERROR;	
		//	lstrcpyn(stDBOInsertMatchSignUPUser.szDescription,TEXT("报名费不足, 请充值"),CountArray(stDBOInsertMatchSignUPUser.szDescription));
		//}

		if (lResultCode == DB_SUCCESS)
		{
			stDBOInsertMatchSignUPUser.bIsSignUpSuc = true;
			char szMsg[100];
			sprintf( szMsg, "用户Id=%ld报名成功",pInsertMatchSignUpUser->dwUserID);
			CString strTime(szMsg);
			OnRecord(strTime);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GC_INSERT_MATCH_SIGNUP_USER,dwContextID,
				&stDBOInsertMatchSignUPUser,sizeof(DBO_GC_Insert_Match_SignUp_User));
		}
		else
		{
			stDBOInsertMatchSignUPUser.bIsSignUpSuc = false;
			char szMsg[100];
			sprintf( szMsg, "用户Id=%ld报名失败",pInsertMatchSignUpUser->dwUserID);
			CString strTime(szMsg);
			OnRecord(strTime);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GC_INSERT_MATCH_SIGNUP_USER,dwContextID,
				&stDBOInsertMatchSignUPUser,sizeof(DBO_GC_Insert_Match_SignUp_User));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(TEXT("GSP_GP_INSERT_MATCH_SIGNUP_USER"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		DBO_GC_Insert_Match_SignUp_User stDBOInsertMatchSignUPUser = {0};
		stDBOInsertMatchSignUPUser.dwUserID = pInsertMatchSignUpUser->dwUserID;
		stDBOInsertMatchSignUPUser.wServerID = pInsertMatchSignUpUser->wServerID;
		stDBOInsertMatchSignUPUser.bIsSignUpSuc = false;
		lstrcpyn(stDBOInsertMatchSignUPUser.szDescription,TEXT("数据库异常, 请稍后重试"),CountArray(stDBOInsertMatchSignUPUser.szDescription));

		printf("Sign Up failed\n");
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GC_INSERT_MATCH_SIGNUP_USER,dwContextID,
				&stDBOInsertMatchSignUPUser,sizeof(DBO_GC_Insert_Match_SignUp_User));

		return false;
	}

	return true;
}

//读取比赛配置信息
bool CDataBaseEngineSink::OnRequestLoadMatchConfig(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		WORD wPacketSize=0;
		BYTE cbBuffer[MAX_ASYNCHRONISM_DATA*10];
		//构造参数
		m_TreasureDBAide.ResetParameter();
		SYSTEMTIME st;
		GetLocalTime(&st);
		char szTime[20];
		sprintf(szTime,"%d-%d-%d",st.wYear,st.wMonth,st.wDay);
		m_TreasureDBAide.AddParameter(TEXT("@MachDay"),szTime);
		//执行查询
		LONG lReturnValue=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_LoadMatchConfig"),true);
		DBO_Load_Match_Config_Item * pMatchConfigItem=NULL;
		//结果处理
		if(lReturnValue==0)
		{
			//读取信息
			while(true)
			{
				if (m_TreasureDBModule->IsRecordsetEnd()==true) break;
				pMatchConfigItem=(DBO_Load_Match_Config_Item *)(cbBuffer+wPacketSize);
				//读取数据
				pMatchConfigItem->nStartTime=m_TreasureDBAide.GetValue_LONG(TEXT("StartTime"));
				pMatchConfigItem->nEndTime=m_TreasureDBAide.GetValue_LONG(TEXT("EndTime"));
				pMatchConfigItem->nMatchNum=m_TreasureDBAide.GetValue_LONG(TEXT("MachNum"));
				pMatchConfigItem->nApplyCost=m_TreasureDBAide.GetValue_LONG(TEXT("ApplyCost"));
				pMatchConfigItem->nMatchUserCount=m_TreasureDBAide.GetValue_LONG(TEXT("MachUserCount"));
				pMatchConfigItem->nGameTime=m_TreasureDBAide.GetValue_LONG(TEXT("GameTimes"));
				pMatchConfigItem->nMatchType=m_TreasureDBAide.GetValue_LONG(TEXT("MachType"));
				m_TreasureDBAide.GetValue_String(TEXT("MachName"),pMatchConfigItem->szMatchName,CountArray(pMatchConfigItem->szMatchName));
				//设置位移
				wPacketSize+=sizeof(DBO_Load_Match_Config_Item);
				//移动记录
				m_TreasureDBModule->MoveToNext();
			}
			if (wPacketSize>0)m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_LOAD_MATCH_CONFIG,dwContextID,cbBuffer,wPacketSize);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(TEXT("GSP_GR_LoadMatchConfig"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestSaveMatchResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	WORD wDataSizeTmp = sizeof(stMatchResult);
	//数据包大小判断
	if (wDataSize%wDataSizeTmp!=0) return true;

	//解析数组
	CString cstrSQL=TEXT("");
	for (unsigned int i = 0; i < wDataSize/wDataSizeTmp; i++)
	{
		CString strSQLTmp;
		stMatchResult *stTmp = (stMatchResult*)pData+i;
		strSQLTmp.Format(TEXT("exec GSP_GP_INSERT_MATCH_RESULT %ld,%lld,%d,\"%s\",\"%s\",%d,%d,%d,%d,%d,%d,%d;"),
			stTmp->dwUserID,
			stTmp->lUserScore,
			stTmp->nMatchRank,
			stTmp->szBeginTime,
			stTmp->szEndTime,
			stTmp->nMatchNum,
			stTmp->nServerId,
			stTmp->nIsComplete,
			stTmp->nIsRobot,
			stTmp->nMatchType,
			stTmp->nGroupId,
			stTmp->nIsGet
			);

		if (cstrSQL.GetLength()>=1023 - 180)
		{
			try
			{
				TCHAR * szSQL = (LPTSTR)(LPCTSTR)cstrSQL;
				m_TreasureDBAide.ExecuteSentence(szSQL,true);
			}
			catch (IDataBaseException * pIException)
			{
				//错误信息
				LPCTSTR pszDescribeDBOError=pIException->GetExceptionDescribe();
				CTraceService::TraceString(pszDescribeDBOError,TraceLevel_Exception);
			}
			cstrSQL=TEXT("");
		}

		cstrSQL+=strSQLTmp;

		TCHAR szsqltmp2[2000];
		_tcsncpy(szsqltmp2,cstrSQL,CountArray(szsqltmp2));
	}

	try
	{
		if (cstrSQL!=TEXT(""))
		{
			TCHAR * szSQLExtra = (LPTSTR)(LPCTSTR)cstrSQL;
			m_TreasureDBAide.ExecuteSentence(szSQLExtra,true);
		}
	}
	catch  (IDataBaseException * pIException)
	{
		//错误信息
		TCHAR	szSQL1[1024];
		_tcsncpy(szSQL1,cstrSQL,CountArray(szSQL1));
		CTraceService::TraceString(szSQL1,TraceLevel_Exception);

		LPCTSTR pszDescribeDBOError2=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribeDBOError2,TraceLevel_Exception);
	}



	return true;
}


bool CDataBaseEngineSink::OnRequestGetSqlTime(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		WORD wPacketSize=0;
		BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];
		//构造参数
		m_TreasureDBAide.ResetParameter();
		//执行查询
		LONG lReturnValue=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_Get_Sql_Time"),true);
		DBO_Get_Sql_Time pSqlTime={0};
		//结果处理
		if(lReturnValue==0)
		{

			m_TreasureDBAide.GetValue_String(TEXT("SqlTime"),pSqlTime.szTime,CountArray(pSqlTime.szTime));
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GET_SQL_TIME,dwContextID,&pSqlTime,sizeof(DBO_Get_Sql_Time));

		}
		

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		CTraceService::TraceString(TEXT("GSP_GP_Get_Sql_Time"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

void CDataBaseEngineSink::OnRecord(CString strMsg)
{
	CString strFileDlgPath;
	TCHAR szModuleDirectory[MAX_PATH];	//模块目录
	GetModuleFileName(AfxGetInstanceHandle(),szModuleDirectory,sizeof(szModuleDirectory));
	int nModuleLen=lstrlen(szModuleDirectory);
	int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".EXE")) + 1;
	if (nModuleLen<=nProcessLen) 
		return ;
	szModuleDirectory[nModuleLen-nProcessLen]=0;
	strFileDlgPath = szModuleDirectory;
	CString strThreadLogDir  = strFileDlgPath+"\\Record.log";
	SYSTEMTIME sys; 
	GetLocalTime(&sys);
	char szTime[100];
	sprintf( szTime, "日期::%d:%d:%d:%d\r\n",sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
	CString strTime(szTime);
	strMsg = strMsg + strTime;
	CFile fLog;
	if(fLog.Open( strThreadLogDir, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite ))
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
//////////////////////////////////////////////////////////////////////////////////
