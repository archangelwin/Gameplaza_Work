#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDataBaseEngineSink::CDataBaseEngineSink()
{
	//���ñ���
	m_pGameParameter=NULL;
	m_pInitParameter=NULL;
	m_pDataBaseParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//�������
	m_pIDataBaseEngine=NULL;
	m_pIGameServiceManager=NULL;
	m_pIDataBaseEngineEvent=NULL;
	m_pIGameDataBaseEngineSink=NULL;
	m_pIDBCorrespondManager=NULL;

	//��������
	ZeroMemory(&m_LogonFailure,sizeof(m_LogonFailure));
	ZeroMemory(&m_LogonSuccess,sizeof(m_LogonSuccess));

	return;
}

//��������
CDataBaseEngineSink::~CDataBaseEngineSink()
{
	//�ͷŶ���
	SafeRelease(m_pIGameDataBaseEngineSink);

	return;
}

//�ӿڲ�ѯ
VOID * CDataBaseEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE(IGameDataBaseEngine,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngineSink,Guid,dwQueryVer);
	return NULL;
}

//��ȡ����
VOID * CDataBaseEngineSink::GetDataBase(REFGUID Guid, DWORD dwQueryVer)
{
	//Ч��״̬
	ASSERT(m_GameDBModule.GetInterface()!=NULL);
	if (m_GameDBModule.GetInterface()==NULL) return NULL;

	//��ѯ����
	IDataBase * pIDataBase=m_GameDBModule.GetInterface();
	VOID * pIQueryObject=pIDataBase->QueryInterface(Guid,dwQueryVer);

	return pIQueryObject;
}

//��ȡ����
VOID * CDataBaseEngineSink::GetDataBaseEngine(REFGUID Guid, DWORD dwQueryVer)
{
	//Ч��״̬
	ASSERT(m_pIDataBaseEngine!=NULL);
	if (m_pIDataBaseEngine==NULL) return NULL;

	//��ѯ����
	VOID * pIQueryObject=m_pIDataBaseEngine->QueryInterface(Guid,dwQueryVer);

	return pIQueryObject;
}

//Ͷ�ݽ��
bool CDataBaseEngineSink::PostGameDataBaseResult(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return true;
}

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)
{
	//��ѯ����
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IDataBaseEngine)!=NULL);
	m_pIDataBaseEngine=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IDataBaseEngine);

	//��������
	if ((m_GameDBModule.GetInterface()==NULL)&&(m_GameDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	if ((m_TreasureDBModule.GetInterface()==NULL)&&(m_TreasureDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	if ((m_PlatformDBModule.GetInterface()==NULL)&&(m_PlatformDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}
	if ((m_LogServerDBModule.GetInterface()==NULL)&&(m_LogServerDBModule.CreateInstance()==false))
	{
		CTraceService::TraceString(TEXT("LogServer ����ʧ��"),TraceLevel_Exception);	}
	else
	{
		try
		{
			TCHAR szPath[MAX_PATH]=TEXT("");
			TCHAR szConfigFileName[MAX_PATH] = TEXT("");
			TCHAR szServerName[50] = TEXT("");
			TCHAR szDBName[50] = TEXT("");
			TCHAR szLogServerIPAddr[50] = TEXT("");
			TCHAR szLogServerUserName[50] = TEXT("");
			TCHAR szLogserverUserPwd[50] = TEXT("");
			GetCurrentDirectory(sizeof(szPath),szPath);
			_sntprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\LogServerConfig.ini"),szPath);

			WORD wServerType = GetPrivateProfileInt(TEXT("Config"),TEXT("LogServerType"),1,szConfigFileName);
			WORD wLogServerPort = 0;
			if (wServerType == 1)
			{
				lstrcpyn(szServerName,TEXT("ProductServer"),CountArray(szServerName));
			} 
			else
			{
				lstrcpyn(szServerName,TEXT("TestServer"),CountArray(szServerName));
			}

			// Read the content of the log server
			GetPrivateProfileString(szServerName,TEXT("IPAddress"),TEXT(""),szLogServerIPAddr,CountArray(szLogServerIPAddr),szConfigFileName);
			GetPrivateProfileString(szServerName,TEXT("DBName"),TEXT(""),szDBName,CountArray(szDBName),szConfigFileName);
			GetPrivateProfileString(szServerName,TEXT("UserName"),TEXT(""),szLogServerUserName,CountArray(szLogServerUserName),szConfigFileName);
			GetPrivateProfileString(szServerName,TEXT("Password"),TEXT(""),szLogserverUserPwd,CountArray(szLogserverUserPwd),szConfigFileName);

			wLogServerPort = GetPrivateProfileInt(szServerName,TEXT("PORT"),22588,szConfigFileName);

			m_LogServerDBModule->SetConnectionInfo(szLogServerIPAddr,wLogServerPort,
				szDBName,szLogServerUserName,szLogserverUserPwd,
				"��Ϸ������","LogServerDBParameter");

			m_LogServerDBModule->OpenConnection();
			m_LogServerDBAide.SetDataBase(m_LogServerDBModule.GetInterface());

			// ��ʾ������־�����������ͺ͵�ַ
			TCHAR pszDescribe[MAX_PATH]=TEXT("");
			_sntprintf(pszDescribe,CountArray(pszDescribe),TEXT("ServerType : %s Link success, IPAddress : %s, PORT : %ld"),szServerName, szLogServerIPAddr, wLogServerPort);
			CTraceService::TraceString(pszDescribe,TraceLevel_Normal);
		}
		catch (IDataBaseException * pIException)
		{
			LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
			CTraceService::TraceString(TEXT("logserver error"),TraceLevel_Exception);
			CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
		}
	}

	//������Ϸ
	try
	{
		//������Ϣ
		tagDataBaseParameter * pTreasureDBParameter=&m_pInitParameter->m_TreasureDBParameter;
		tagDataBaseParameter * pPlatformDBParameter=&m_pInitParameter->m_PlatformDBParameter;

		//��������
		m_GameDBModule->SetConnectionInfo(m_pDataBaseParameter->szDataBaseAddr,m_pDataBaseParameter->wDataBasePort,
			m_pDataBaseParameter->szDataBaseName,m_pDataBaseParameter->szDataBaseUser,m_pDataBaseParameter->szDataBasePass,
			m_pGameServiceOption->szServerName,"GameDBModule");

		//��������
		m_TreasureDBModule->SetConnectionInfo(pTreasureDBParameter->szDataBaseAddr,pTreasureDBParameter->wDataBasePort,
			pTreasureDBParameter->szDataBaseName,pTreasureDBParameter->szDataBaseUser,pTreasureDBParameter->szDataBasePass,
			m_pGameServiceOption->szServerName,"TreasureDBModule");

		//��������
		m_PlatformDBModule->SetConnectionInfo(pPlatformDBParameter->szDataBaseAddr,pPlatformDBParameter->wDataBasePort,
			pPlatformDBParameter->szDataBaseName,pPlatformDBParameter->szDataBaseUser,pPlatformDBParameter->szDataBasePass,
			m_pGameServiceOption->szServerName,"PlatformDBModule");

		//��������
		m_GameDBModule->OpenConnection();
		m_GameDBAide.SetDataBase(m_GameDBModule.GetInterface());

		//��������
		m_TreasureDBModule->OpenConnection();
		m_TreasureDBAide.SetDataBase(m_TreasureDBModule.GetInterface());

		//��������
		m_PlatformDBModule->OpenConnection();
		m_PlatformDBAide.SetDataBase(m_PlatformDBModule.GetInterface());

		//���ݹ���
		ASSERT(m_pIGameServiceManager!=NULL);
		m_pIGameDataBaseEngineSink=(IGameDataBaseEngineSink *)m_pIGameServiceManager->CreateGameDataBaseEngineSink(IID_IGameDataBaseEngineSink,VER_IGameDataBaseEngineSink);

		//���ö���
		if ((m_pIGameDataBaseEngineSink!=NULL)&&(m_pIGameDataBaseEngineSink->InitializeSink(QUERY_ME_INTERFACE(IUnknownEx))==false))
		{
			//�������
			ASSERT(FALSE);

			//�����Ϣ
			CTraceService::TraceString(TEXT("��Ϸ���ݿ���չ���������������ʧ��"),TraceLevel_Exception);

			return false;
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//ֹͣ�¼�
bool CDataBaseEngineSink::OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx)
{
	//���ñ���
	m_pInitParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//�������
	m_pIGameServiceManager=NULL;
	m_pIDataBaseEngineEvent=NULL;

	//���ö���
	m_GameDBAide.SetDataBase(NULL);

	//�ͷŶ���
	SafeRelease(m_pIGameDataBaseEngineSink);

	//�ر�����
	if (m_GameDBModule.GetInterface()!=NULL)
	{
		m_GameDBModule->CloseConnection();
		m_GameDBModule.CloseInstance();
	}

	//�ر�����
	if (m_TreasureDBModule.GetInterface()!=NULL)
	{
		m_TreasureDBModule->CloseConnection();
		m_TreasureDBModule.CloseInstance();
	}

	//�ر�����
	if (m_PlatformDBModule.GetInterface()!=NULL)
	{
		m_PlatformDBModule->CloseConnection();
		m_PlatformDBModule.CloseInstance();
	}

	return true;
}

//ʱ���¼�
bool CDataBaseEngineSink::OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
	return false;
}

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize)
{
	return false;
}

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	bool bSucceed = false;
	DWORD dwUserID = 0L;

	//// test
	//TCHAR pszDescribe[MAX_PATH]=TEXT("");
	//_sntprintf(pszDescribe,CountArray(pszDescribe),TEXT("the msg request id is %d"),wRequestID);
	//CTraceService::TraceString(pszDescribe,TraceLevel_Normal);

	//������
	switch (wRequestID)
	{
	case DBR_GR_LOGON_USERID:			//I D ��¼
		{
			bSucceed = OnRequestLogonUserID(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_LOGON_MOBILE:			//�ֻ���¼
		{
			bSucceed = OnRequestLogonMobile(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_LOGON_ACCOUNTS:			//�ʺŵ�¼
		{
			bSucceed = OnRequestLogonAccounts(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_LOGON_MATCHTIMER:		//��ʱِ��������
		{
			bSucceed = OnRequestLogonUserID(dwContextID,pData,wDataSize,dwUserID,TRUE);
		}
		break;
	case DBR_GR_WRITE_GAME_SCORE:		//��Ϸд��
		{
			//// test
			//TCHAR pszDescribe[MAX_PATH]=TEXT("");
			//_sntprintf(pszDescribe,CountArray(pszDescribe),TEXT("file: %s, function : %s line : %d"),__FILE__, __FUNCTION__,  __LINE__);
			//CTraceService::TraceString(pszDescribe,TraceLevel_Normal);

			bSucceed = OnRequestWriteGameScore(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_LEAVE_GAME_SERVER:		//�뿪����
		{
			bSucceed = OnRequestLeaveGameServer(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_GAME_SCORE_RECORD:		//��Ϸ��¼
		{
			bSucceed = OnRequestGameScoreRecord(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_LOAD_PARAMETER:			//���ز���
		{
			bSucceed = OnRequestLoadParameter(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_LOAD_GAME_COLUMN:		//�����б�
		{
			bSucceed = OnRequestLoadGameColumn(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_LOAD_ANDROID_USER:		//���ػ���
		{
			bSucceed = OnRequestLoadAndroidUser(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_LOAD_GAME_PROPERTY:		//���ص���
		{
			bSucceed = OnRequestLoadGameProperty(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_USER_SAVE_SCORE:		//������Ϸ��
		{
			bSucceed = OnRequestUserSaveScore(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_USER_TAKE_SCORE:		//��ȡ��Ϸ��
		{
			bSucceed = OnRequestUserTakeScore(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_USER_TRANSFER_SCORE:	//ת����Ϸ��
		{
			bSucceed = OnRequestUserTransferScore(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_USER_TRANSFER_DRAGON:
		{
			bSucceed = OnRequestUserTransferDragon(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_USER_USE_DRAGON:
		{
			bSucceed = OnRequestUserUseDragon(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_USER_WIN_DRAGON_PIECE:
		{
			bSucceed = OnRequestUserWinDragonPiece(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_WIN_PEARL:
		{
			bSucceed = OnRequestUserWinPearl(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_SKILL_GIVE:
		{
			bSucceed = OnRequestSkillGive(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_QUERY_INSURE_INFO:		//��ѯ����
		{
			bSucceed = OnRequestQueryInsureInfo(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_QUERY_TRANSFER_USER_INFO:		//��ѯ�û�
		{
			bSucceed = OnRequestQueryTransferUserInfo(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_PROPERTY_REQUEST:		//��������
		{
			bSucceed = OnRequestPropertyRequest(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_CHECK_TRUMPET:
		{
			bSucceed = OnRequestCheckTrumpet(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_FIRST_ENTER_SUPER:
		{
			bSucceed = OnRequestFirstEnterSuper(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_MATCH_GET_PRIZE:
		{
			bSucceed = OnRequestGetMatchPirze(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_MANAGE_USER_RIGHT:		//�û�Ȩ��
		{
			bSucceed = OnRequestManageUserRight(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_LOAD_SYSTEM_MESSAGE:   //ϵͳ��Ϣ
		{
			bSucceed = OnRequestLoadSystemMessage(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_LOAD_MATCH_CONFIG:   //���ر���������Ϣ
		{
			bSucceed = OnRequestLoadMatchConfig(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_LOAD_MATCH_REWARD_CONFIG:   //���ر�������������Ϣ
		{
			bSucceed = OnRequestLoadMatchRewardConfig(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_LOAD_SENSITIVE_WORDS://�������д�
		{
			return OnRequestLoadSensitiveWords(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_MATCH_FEE:				//��������
		{
			bSucceed = OnRequestMatchFee(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_MATCH_START:			//������ʼ
		{
			bSucceed = OnRequestMatchStart(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_MATCH_OVER:				//��������
		{
			bSucceed = OnRequestMatchOver(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_MATCH_REWARD:			//��������
		{
			bSucceed = OnRequestMatchReward(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_MATCH_QUIT:				//�˳�����
		{
			bSucceed = OnRequestMatchQuit(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_CHECKSTOCK:
		{
			bSucceed = OnBankCheckStock(dwContextID,pData,wDataSize);
		}
		break;
	case DBR_GR_GET_USER_LUCKY:
		{
			bSucceed = OnGetUserLucky(dwContextID,pData,wDataSize);
		}
		break;
	case DBR_GR_SAVEUSERITEM:
		{
			bSucceed = OnSaveUserItem(dwContextID,pData,wDataSize);
		}
		break;
	case DBR_GR_GAME_JSON:
		{
			bSucceed = OnGameJson(dwContextID,pData,wDataSize);
		}
		break;
	case DBR_GR_SENTENCE:
		{
			bSucceed = OnSentence(dwContextID,pData,wDataSize);
		}
		break;
	case DBR_GR_QUERY_SKILL:
		{
			bSucceed = OnRequestQuerySkill(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_USE_SKILL:
		{
			bSucceed = OnRequestUseSkill(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_NEW_GUID_GIFT:
		{
			bSucceed = OnRequestNewGuidGift(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_USERLOG_CLOWN_OPERATION:
		{
			// test
			TCHAR pszDescribe[MAX_PATH]=TEXT("");
			_sntprintf(pszDescribe,CountArray(pszDescribe),TEXT("file: %s, function : %s line : %d"),__FILE__, __FUNCTION__,  __LINE__);
			CTraceService::TraceString(pszDescribe,TraceLevel_Normal);

			OnRequestLogClownOperation(dwContextID,pData,wDataSize,dwUserID);
			bSucceed = true;
		}
		break;
	case DBR_GET_USER_ITEM_COUNT:
		{
			bSucceed = OnRequestGetUserItemCount(dwContextID, pData, wDataSize, dwUserID);
		}
		break;
	case DBR_SAVE_USER_ITEM_COUNT:
		{
			bSucceed = OnRequestSaveUserItemCount(dwContextID, pData, wDataSize, dwUserID);
		}
		break;
	case DBR_GR_UPDATE_TREASUREMAP:
		{
			bSucceed = OnRequestUpdateTreasureMap(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_UPDATE_KVALUEANDINTEGRATION:
		{
			bSucceed = OnRequestKValueAndIntegration(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_UPDATE_MAXKVALUE:
		{
			bSucceed = OnRequestUserMaxKValue(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	case DBR_GR_MATCH_USERSIGNUP:
		{
			bSucceed = OnRequestUserMatchSignUP(dwContextID,pData,wDataSize,dwUserID);
		}
		break;
	}

	//Э��֪ͨ
	if(m_pIDBCorrespondManager) m_pIDBCorrespondManager->OnPostRequestComplete(dwUserID, bSucceed);

	return bSucceed;
}

//I D ��¼
bool CDataBaseEngineSink::OnRequestLogonUserID(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID,BOOL bMatch)
{
	//ִ�в�ѯ
	DBR_GR_LogonUserID * pLogonUserID=(DBR_GR_LogonUserID *)pData;
	dwUserID = pLogonUserID->dwUserID;

	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_LogonUserID));
	if (wDataSize!=sizeof(DBR_GR_LogonUserID)) return false;


	//�洢����ִ�в�������
	DWORD dwInoutIndex=0;
	BYTE cbVIPLevel=0;
	DWORD dwVIPExp=0;
	SCORE lScore=0;

	try
	{
		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonUserID->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pLogonUserID->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@strPassword"),pLogonUserID->szPassword);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_GameDBAide.AddParameter(TEXT("@strMachineID"),pLogonUserID->szMachineID);
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		if((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH))
			m_GameDBAide.AddParameter(TEXT("@cbTimeMatch"),bMatch);
		m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),m_LogonFailure.szDescribeString,sizeof(m_LogonFailure.szDescribeString),adParamOutput);
		TCHAR szBuffer[128]=TEXT("");
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("ִ�д洢���� UserID=%ld,ClientAddr = %s\n"),pLogonUserID->dwUserID,szClientAddr);

		CTraceService::TraceString(szBuffer,TraceLevel_Info);
		//ִ�в�ѯ
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_EfficacyUserID"),true);

		//�û���Ϣ
		lstrcpyn(m_LogonSuccess.szPassword,pLogonUserID->szPassword,CountArray(m_LogonSuccess.szPassword));
		lstrcpyn(m_LogonSuccess.szMachineID,pLogonUserID->szMachineID,CountArray(m_LogonSuccess.szMachineID));
	
		//�������
		CDBVarValue DBVarValue;
		m_GameDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		//�洢���̽������
		if (lResultCode==DB_SUCCESS)
		{
			dwInoutIndex=m_GameDBAide.GetValue_DWORD(TEXT("InoutIndex"));
			cbVIPLevel=m_GameDBAide.GetValue_BYTE(TEXT("vipLevel"));
			dwVIPExp=m_GameDBAide.GetValue_DWORD(TEXT("vipExp"));
			lScore=m_GameDBAide.GetValue_LONGLONG(TEXT("Score"));
		}

		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_EfficacyUserID"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false);

		return false;
	}

	//��־������
	try
	{
		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonUserID->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_LogServerDBAide.ResetParameter();
		m_LogServerDBAide.AddParameter(TEXT("@dwUserID"),pLogonUserID->dwUserID);
		m_LogServerDBAide.AddParameter(TEXT("@strPassword"),pLogonUserID->szPassword);
		m_LogServerDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_LogServerDBAide.AddParameter(TEXT("@strMachineID"),pLogonUserID->szMachineID);
		m_LogServerDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_LogServerDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		if((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH))
			m_LogServerDBAide.AddParameter(TEXT("@cbTimeMatch"),bMatch);
		m_LogServerDBAide.AddParameter(TEXT("@dwInoutIndex"),dwInoutIndex);
		m_LogServerDBAide.AddParameter(TEXT("@cbVIPLevel"),cbVIPLevel);
		m_LogServerDBAide.AddParameter(TEXT("@dwVIPExp"),dwVIPExp);
		m_LogServerDBAide.AddParameter(TEXT("@lScore"),lScore);

		m_LogServerDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),m_LogonFailure.szDescribeString,sizeof(m_LogonFailure.szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_LogServerDBAide.ExecuteProcess(TEXT("Log_GSP_GR_EfficacyUserID"),false);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("Log_GSP_GR_EfficacyUserID"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
	}

	return true;
}

//I D ��¼
bool CDataBaseEngineSink::OnRequestLogonMobile(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//ִ�в�ѯ
	DBR_GR_LogonMobile * pLogonMobile=(DBR_GR_LogonMobile *)pData;
	dwUserID = pLogonMobile->dwUserID;

	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GR_LogonMobile));
		if (wDataSize!=sizeof(DBR_GR_LogonMobile)) return false;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonMobile->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pLogonMobile->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@strPassword"),pLogonMobile->szPassword);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_GameDBAide.AddParameter(TEXT("@strMachineID"),pLogonMobile->szMachineID);
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),m_LogonFailure.szDescribeString,sizeof(m_LogonFailure.szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_EfficacyMobile"),true);

		//�û���Ϣ
		lstrcpyn(m_LogonSuccess.szPassword,pLogonMobile->szPassword,CountArray(m_LogonSuccess.szPassword));
		lstrcpyn(m_LogonSuccess.szMachineID,pLogonMobile->szMachineID,CountArray(m_LogonSuccess.szMachineID));
		m_LogonSuccess.cbDeviceType=pLogonMobile->cbDeviceType;
		m_LogonSuccess.wBehaviorFlags=pLogonMobile->wBehaviorFlags;
		m_LogonSuccess.wPageTableCount=pLogonMobile->wPageTableCount;
	
		//�������
		CDBVarValue DBVarValue;
		m_GameDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true,pLogonMobile->cbDeviceType,pLogonMobile->wBehaviorFlags,pLogonMobile->wPageTableCount);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_EfficacyMobile"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),true);

		return false;
	}

	return true;
}

//�ʺŵ�¼
bool CDataBaseEngineSink::OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GR_LogonAccounts));
		if (wDataSize!=sizeof(DBR_GR_LogonAccounts)) return false;

		//������
		DBR_GR_LogonAccounts * pLogonAccounts=(DBR_GR_LogonAccounts *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
		m_GameDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_GameDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),m_LogonFailure.szDescribeString,sizeof(m_LogonFailure.szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_EfficacyAccounts"),true);

		//�û���Ϣ
		lstrcpyn(m_LogonSuccess.szPassword,pLogonAccounts->szPassword,CountArray(m_LogonSuccess.szPassword));
		lstrcpyn(m_LogonSuccess.szMachineID,pLogonAccounts->szMachineID,CountArray(m_LogonSuccess.szMachineID));

		//�������
		CDBVarValue DBVarValue;
		m_GameDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_EfficacyAccounts"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false);

		return false;
	}

	return true;
}

//��Ϸд��
bool CDataBaseEngineSink::OnRequestWriteGameScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//������
	DBR_GR_WriteGameScore * pWriteGameScore=(DBR_GR_WriteGameScore *)pData;
	dwUserID=pWriteGameScore->dwUserID;

	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GR_WriteGameScore));
		if (wDataSize!=sizeof(DBR_GR_WriteGameScore)) return false;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pWriteGameScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_GameDBAide.ResetParameter();

		//�û���Ϣ
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pWriteGameScore->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@dwDBQuestID"),pWriteGameScore->dwDBQuestID);
		m_GameDBAide.AddParameter(TEXT("@dwInoutIndex"),pWriteGameScore->dwInoutIndex);

		//����ɼ�
		m_GameDBAide.AddParameter(TEXT("@lScore"),pWriteGameScore->VariationInfo.lScore);
		m_GameDBAide.AddParameter(TEXT("@lGrade"),pWriteGameScore->VariationInfo.lGrade);
		m_GameDBAide.AddParameter(TEXT("@lInsure"),pWriteGameScore->VariationInfo.lInsure);
		m_GameDBAide.AddParameter(TEXT("@lRevenue"),pWriteGameScore->VariationInfo.lRevenue);
		m_GameDBAide.AddParameter(TEXT("@lWinCount"),pWriteGameScore->VariationInfo.dwWinCount);
		m_GameDBAide.AddParameter(TEXT("@lLostCount"),pWriteGameScore->VariationInfo.dwLostCount);
		m_GameDBAide.AddParameter(TEXT("@lDrawCount"),pWriteGameScore->VariationInfo.dwDrawCount);
		m_GameDBAide.AddParameter(TEXT("@lFleeCount"),pWriteGameScore->VariationInfo.dwFleeCount);
		m_GameDBAide.AddParameter(TEXT("@lUserMedal"),pWriteGameScore->VariationInfo.dwUserMedal);
		m_GameDBAide.AddParameter(TEXT("@lExperience"),pWriteGameScore->VariationInfo.dwExperience);
		m_GameDBAide.AddParameter(TEXT("@lLoveLiness"),pWriteGameScore->VariationInfo.lLoveLiness);
		m_GameDBAide.AddParameter(TEXT("@dwPlayTimeCount"),pWriteGameScore->VariationInfo.dwPlayTimeCount);

		//������Ϣ
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//ִ�в�ѯ
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_WriteGameScore"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GR_WriteGameScore"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//�뿪����
bool CDataBaseEngineSink::OnRequestLeaveGameServer(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//������
	DBR_GR_LeaveGameServer * pLeaveGameServer=(DBR_GR_LeaveGameServer *)pData;
	dwUserID=pLeaveGameServer->dwUserID;

	try
	{
		// test
		TCHAR pszDescribe[MAX_PATH]=TEXT("");
		_sntprintf(pszDescribe,CountArray(pszDescribe),TEXT("leave server go file: %s, function : %s line : %d"),__FILE__, __FUNCTION__,  __LINE__);
		CTraceService::TraceString(pszDescribe,TraceLevel_Normal);

		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GR_LeaveGameServer));
		if (wDataSize!=sizeof(DBR_GR_LeaveGameServer)) return false;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLeaveGameServer->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_GameDBAide.ResetParameter();

		//�û���Ϣ
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pLeaveGameServer->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@dwOnLineTimeCount"),pLeaveGameServer->dwOnLineTimeCount);

		//ϵͳ��Ϣ
		m_GameDBAide.AddParameter(TEXT("@dwInoutIndex"),pLeaveGameServer->dwInoutIndex);
		m_GameDBAide.AddParameter(TEXT("@dwLeaveReason"),pLeaveGameServer->dwLeaveReason);

		//��¼�ɼ�
		m_GameDBAide.AddParameter(TEXT("@lRecordScore"),pLeaveGameServer->RecordInfo.lScore);
		m_GameDBAide.AddParameter(TEXT("@lRecordGrade"),pLeaveGameServer->RecordInfo.lGrade);
		m_GameDBAide.AddParameter(TEXT("@lRecordInsure"),pLeaveGameServer->RecordInfo.lInsure);
		m_GameDBAide.AddParameter(TEXT("@lRecordRevenue"),pLeaveGameServer->RecordInfo.lRevenue);
		m_GameDBAide.AddParameter(TEXT("@lRecordWinCount"),pLeaveGameServer->RecordInfo.dwWinCount);
		m_GameDBAide.AddParameter(TEXT("@lRecordLostCount"),pLeaveGameServer->RecordInfo.dwLostCount);
		m_GameDBAide.AddParameter(TEXT("@lRecordDrawCount"),pLeaveGameServer->RecordInfo.dwDrawCount);
		m_GameDBAide.AddParameter(TEXT("@lRecordFleeCount"),pLeaveGameServer->RecordInfo.dwFleeCount);
		m_GameDBAide.AddParameter(TEXT("@lRecordUserMedal"),pLeaveGameServer->RecordInfo.dwUserMedal);
		m_GameDBAide.AddParameter(TEXT("@lRecordExperience"),pLeaveGameServer->RecordInfo.dwExperience);
		m_GameDBAide.AddParameter(TEXT("@lRecordLoveLiness"),pLeaveGameServer->RecordInfo.lLoveLiness);
		m_GameDBAide.AddParameter(TEXT("@dwRecordPlayTimeCount"),pLeaveGameServer->RecordInfo.dwPlayTimeCount);

		//����ɼ�
		m_GameDBAide.AddParameter(TEXT("@lVariationScore"),pLeaveGameServer->VariationInfo.lScore);
		m_GameDBAide.AddParameter(TEXT("@lVariationGrade"),pLeaveGameServer->VariationInfo.lGrade);
		m_GameDBAide.AddParameter(TEXT("@lVariationInsure"),pLeaveGameServer->VariationInfo.lInsure);
		m_GameDBAide.AddParameter(TEXT("@lVariationRevenue"),pLeaveGameServer->VariationInfo.lRevenue);
		m_GameDBAide.AddParameter(TEXT("@lVariationWinCount"),pLeaveGameServer->VariationInfo.dwWinCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationLostCount"),pLeaveGameServer->VariationInfo.dwLostCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationDrawCount"),pLeaveGameServer->VariationInfo.dwDrawCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationFleeCount"),pLeaveGameServer->VariationInfo.dwFleeCount);
		m_GameDBAide.AddParameter(TEXT("@lVariationUserMedal"),pLeaveGameServer->VariationInfo.dwUserMedal);
		m_GameDBAide.AddParameter(TEXT("@lVariationExperience"),pLeaveGameServer->VariationInfo.dwExperience);
		m_GameDBAide.AddParameter(TEXT("@lVariationLoveLiness"),pLeaveGameServer->VariationInfo.lLoveLiness);
		m_GameDBAide.AddParameter(TEXT("@dwVariationPlayTimeCount"),pLeaveGameServer->VariationInfo.dwPlayTimeCount);

		//��������
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_GameDBAide.AddParameter(TEXT("@strMachineID"),pLeaveGameServer->szMachineID);

		//ִ�в�ѯ
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LeaveGameServer"),true);

		if (lResultCode==DB_SUCCESS)
		{
			DBO_GR_PresentScore PresentScore={0};
			PresentScore.nScore=m_GameDBAide.GetValue_LONG(TEXT("PresentScore"));
			if (PresentScore.nScore>0)
			{
				PresentScore.nTime=pLeaveGameServer->RecordInfo.dwPlayTimeCount;
				lstrcpyn(PresentScore.szNickName,pLeaveGameServer->szNickName,CountArray(PresentScore.szNickName));
				lstrcpyn(PresentScore.szServerName,pLeaveGameServer->szServerName,CountArray(PresentScore.szServerName));

				//������Ϣ
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_PRESENT_SCORE,dwContextID,&PresentScore,sizeof(PresentScore));
			}
		}
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GR_LeaveGameServer"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	// ��־������
	try
	{
		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLeaveGameServer->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_LogServerDBAide.ResetParameter();

		//�û���Ϣ
		m_LogServerDBAide.AddParameter(TEXT("@dwUserID"),pLeaveGameServer->dwUserID);
		m_LogServerDBAide.AddParameter(TEXT("@dwOnLineTimeCount"),pLeaveGameServer->dwOnLineTimeCount);

		//ϵͳ��Ϣ
		m_LogServerDBAide.AddParameter(TEXT("@dwInoutIndex"),pLeaveGameServer->dwInoutIndex);
		m_LogServerDBAide.AddParameter(TEXT("@dwLeaveReason"),pLeaveGameServer->dwLeaveReason);  

		//��¼�ɼ�
		m_LogServerDBAide.AddParameter(TEXT("@lRecordScore"),pLeaveGameServer->RecordInfo.lScore); 
		m_LogServerDBAide.AddParameter(TEXT("@lRecordGrade"),pLeaveGameServer->RecordInfo.lGrade);
		m_LogServerDBAide.AddParameter(TEXT("@lRecordInsure"),pLeaveGameServer->RecordInfo.lInsure);
		m_LogServerDBAide.AddParameter(TEXT("@lRecordRevenue"),pLeaveGameServer->RecordInfo.lRevenue);
		m_LogServerDBAide.AddParameter(TEXT("@lRecordWinCount"),pLeaveGameServer->RecordInfo.dwWinCount);
		m_LogServerDBAide.AddParameter(TEXT("@lRecordLostCount"),pLeaveGameServer->RecordInfo.dwLostCount);
		m_LogServerDBAide.AddParameter(TEXT("@lRecordDrawCount"),pLeaveGameServer->RecordInfo.dwDrawCount);
		m_LogServerDBAide.AddParameter(TEXT("@lRecordFleeCount"),pLeaveGameServer->RecordInfo.dwFleeCount);
		m_LogServerDBAide.AddParameter(TEXT("@lRecordUserMedal"),pLeaveGameServer->RecordInfo.dwUserMedal);
		m_LogServerDBAide.AddParameter(TEXT("@lRecordExperience"),pLeaveGameServer->RecordInfo.dwExperience);
		m_LogServerDBAide.AddParameter(TEXT("@lRecordLoveLiness"),pLeaveGameServer->RecordInfo.lLoveLiness);
		m_LogServerDBAide.AddParameter(TEXT("@dwRecordPlayTimeCount"),pLeaveGameServer->RecordInfo.dwPlayTimeCount);

		//����ɼ�
		m_LogServerDBAide.AddParameter(TEXT("@lVariationScore"),pLeaveGameServer->VariationInfo.lScore);
		m_LogServerDBAide.AddParameter(TEXT("@lVariationGrade"),pLeaveGameServer->VariationInfo.lGrade);
		m_LogServerDBAide.AddParameter(TEXT("@lVariationInsure"),pLeaveGameServer->VariationInfo.lInsure);
		m_LogServerDBAide.AddParameter(TEXT("@lVariationRevenue"),pLeaveGameServer->VariationInfo.lRevenue);
		m_LogServerDBAide.AddParameter(TEXT("@lVariationWinCount"),pLeaveGameServer->VariationInfo.dwWinCount);
		m_LogServerDBAide.AddParameter(TEXT("@lVariationLostCount"),pLeaveGameServer->VariationInfo.dwLostCount);
		m_LogServerDBAide.AddParameter(TEXT("@lVariationDrawCount"),pLeaveGameServer->VariationInfo.dwDrawCount);
		m_LogServerDBAide.AddParameter(TEXT("@lVariationFleeCount"),pLeaveGameServer->VariationInfo.dwFleeCount);
		m_LogServerDBAide.AddParameter(TEXT("@lVariationUserMedal"),pLeaveGameServer->VariationInfo.dwUserMedal);
		m_LogServerDBAide.AddParameter(TEXT("@lVariationExperience"),pLeaveGameServer->VariationInfo.dwExperience);
		m_LogServerDBAide.AddParameter(TEXT("@lVariationLoveLiness"),pLeaveGameServer->VariationInfo.lLoveLiness);
		m_LogServerDBAide.AddParameter(TEXT("@dwVariationPlayTimeCount"),pLeaveGameServer->VariationInfo.dwPlayTimeCount);

		//��������
		m_LogServerDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_LogServerDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_LogServerDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_LogServerDBAide.AddParameter(TEXT("@strMachineID"),pLeaveGameServer->szMachineID);

		//ִ�в�ѯ
		LONG lResultCode=m_LogServerDBAide.ExecuteProcess(TEXT("Log_GSP_GR_LeaveGameServer"),true);
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("Log_GSP_GR_LeaveGameServer"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
	}

	return true;
}

//��Ϸ��¼
bool CDataBaseEngineSink::OnRequestGameScoreRecord(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//��������
		DBR_GR_GameScoreRecord * pGameScoreRecord=(DBR_GR_GameScoreRecord *)pData;
		dwUserID=INVALID_DWORD;

		//Ч�����
		ASSERT(wDataSize<=sizeof(DBR_GR_GameScoreRecord));
		ASSERT(wDataSize>=(sizeof(DBR_GR_GameScoreRecord)-sizeof(pGameScoreRecord->GameScoreRecord)));
		ASSERT(wDataSize==(sizeof(DBR_GR_GameScoreRecord)-sizeof(pGameScoreRecord->GameScoreRecord)+pGameScoreRecord->wRecordCount*sizeof(pGameScoreRecord->GameScoreRecord[0])));

		//Ч�����
		if (wDataSize>sizeof(DBR_GR_GameScoreRecord)) return false;
		if (wDataSize<(sizeof(DBR_GR_GameScoreRecord)-sizeof(pGameScoreRecord->GameScoreRecord))) return false;
		if (wDataSize!=(sizeof(DBR_GR_GameScoreRecord)-sizeof(pGameScoreRecord->GameScoreRecord)+pGameScoreRecord->wRecordCount*sizeof(pGameScoreRecord->GameScoreRecord[0]))) return false;

		//������Ϣ
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

		//������Ϣ
		m_GameDBAide.AddParameter(TEXT("@wTableID"),pGameScoreRecord->wTableID);
		m_GameDBAide.AddParameter(TEXT("@wUserCount"),pGameScoreRecord->wUserCount);
		m_GameDBAide.AddParameter(TEXT("@wAndroidCount"),pGameScoreRecord->wAndroidCount);

		//˰�����
		m_GameDBAide.AddParameter(TEXT("@lWasteCount"),pGameScoreRecord->lWasteCount);
		m_GameDBAide.AddParameter(TEXT("@lRevenueCount"),pGameScoreRecord->lRevenueCount);

		//ͳ����Ϣ
		m_GameDBAide.AddParameter(TEXT("@dwUserMemal"),pGameScoreRecord->dwUserMemal);
		m_GameDBAide.AddParameter(TEXT("@dwPlayTimeCount"),pGameScoreRecord->dwPlayTimeCount);

		//ʱ����Ϣ
		m_GameDBAide.AddParameter(TEXT("@SystemTimeStart"),pGameScoreRecord->SystemTimeStart);
		m_GameDBAide.AddParameter(TEXT("@SystemTimeConclude"),pGameScoreRecord->SystemTimeConclude);

		//ִ�в�ѯ
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_RecordDrawInfo"),true);

		//д���¼
		if (lResultCode==DB_SUCCESS)
		{
			//��ȡ��ʶ
			DWORD dwDrawID=m_GameDBAide.GetValue_DWORD(TEXT("DrawID"));

			//д���¼
			for (WORD i=0;i<pGameScoreRecord->wRecordCount;i++)
			{
				//���ò���
				m_GameDBAide.ResetParameter();
				
				//������Ϣ
				m_GameDBAide.AddParameter(TEXT("@dwDrawID"),dwDrawID);
				m_GameDBAide.AddParameter(TEXT("@dwUserID"),pGameScoreRecord->GameScoreRecord[i].dwUserID);
				m_GameDBAide.AddParameter(TEXT("@wChairID"),pGameScoreRecord->GameScoreRecord[i].wChairID);

				//�û���Ϣ
				m_GameDBAide.AddParameter(TEXT("@dwDBQuestID"),pGameScoreRecord->GameScoreRecord[i].dwDBQuestID);
				m_GameDBAide.AddParameter(TEXT("@dwInoutIndex"),pGameScoreRecord->GameScoreRecord[i].dwInoutIndex);

				//�ɼ���Ϣ
				m_GameDBAide.AddParameter(TEXT("@lScore"),pGameScoreRecord->GameScoreRecord[i].lScore);
				m_GameDBAide.AddParameter(TEXT("@lGrade"),pGameScoreRecord->GameScoreRecord[i].lGrade);
				m_GameDBAide.AddParameter(TEXT("@lRevenue"),pGameScoreRecord->GameScoreRecord[i].lRevenue);
				m_GameDBAide.AddParameter(TEXT("@dwUserMedal"),pGameScoreRecord->GameScoreRecord[i].dwUserMemal);
				m_GameDBAide.AddParameter(TEXT("@dwPlayTimeCount"),pGameScoreRecord->GameScoreRecord[i].dwPlayTimeCount);

				//ִ�в�ѯ
				m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_RecordDrawScore"),false);
			}
		}
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GR_RecordDrawScore"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//���ز���
bool CDataBaseEngineSink::OnRequestLoadParameter(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//��������
		DBO_GR_GameParameter GameParameter;
		ZeroMemory(&GameParameter,sizeof(GameParameter));

		//ִ�в�ѯ
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

		//ִ�в�ѯ
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LoadParameter"),true);

		//��ȡ��Ϣ
		if (lResultCode==DB_SUCCESS)
		{
			//������Ϣ
			GameParameter.wMedalRate=m_GameDBAide.GetValue_WORD(TEXT("MedalRate"));
			GameParameter.wRevenueRate=m_GameDBAide.GetValue_WORD(TEXT("RevenueRate"));

			//�汾��Ϣ
			GameParameter.dwClientVersion=m_GameDBAide.GetValue_DWORD(TEXT("ClientVersion"));
			GameParameter.dwServerVersion=m_GameDBAide.GetValue_DWORD(TEXT("ServerVersion"));
		
			//������Ϣ
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GAME_PARAMETER,dwContextID,&GameParameter,sizeof(GameParameter));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GR_LoadParameter"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//�����б�
bool CDataBaseEngineSink::OnRequestLoadGameColumn(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//��������
		DBO_GR_GameColumnInfo GameColumnInfo;
		ZeroMemory(&GameColumnInfo,sizeof(GameColumnInfo));

		//ִ�в�ѯ
		m_GameDBAide.ResetParameter();
		GameColumnInfo.lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_GameColumnItem"),true);

		//��ȡ��Ϣ
		for (WORD i=0;i<CountArray(GameColumnInfo.ColumnItemInfo);i++)
		{
			//�����ж�
			if (m_GameDBModule->IsRecordsetEnd()==true) break;

			//���Ч��
			ASSERT(GameColumnInfo.cbColumnCount<CountArray(GameColumnInfo.ColumnItemInfo));
			if (GameColumnInfo.cbColumnCount>=CountArray(GameColumnInfo.ColumnItemInfo)) break;

			//��ȡ����
			GameColumnInfo.cbColumnCount++;
			GameColumnInfo.ColumnItemInfo[i].cbColumnWidth=m_GameDBAide.GetValue_BYTE(TEXT("ColumnWidth"));
			GameColumnInfo.ColumnItemInfo[i].cbDataDescribe=m_GameDBAide.GetValue_BYTE(TEXT("DataDescribe"));
			m_GameDBAide.GetValue_String(TEXT("ColumnName"),GameColumnInfo.ColumnItemInfo[i].szColumnName,CountArray(GameColumnInfo.ColumnItemInfo[i].szColumnName));

			//�ƶ���¼
			m_GameDBModule->MoveToNext();
		}

		//������Ϣ
		WORD wHeadSize=sizeof(GameColumnInfo)-sizeof(GameColumnInfo.ColumnItemInfo);
		WORD wPacketSize=wHeadSize+GameColumnInfo.cbColumnCount*sizeof(GameColumnInfo.ColumnItemInfo[0]);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GAME_COLUMN_INFO,dwContextID,&GameColumnInfo,wPacketSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GR_GameColumnItem"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//��������
		DBO_GR_GameColumnInfo GameColumnInfo;
		ZeroMemory(&GameColumnInfo,sizeof(GameColumnInfo));

		//�������
		GameColumnInfo.cbColumnCount=0L;
		GameColumnInfo.lResultCode=pIException->GetExceptionResult();

		//������Ϣ
		WORD wPacketSize=sizeof(GameColumnInfo)-sizeof(GameColumnInfo.ColumnItemInfo);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GAME_COLUMN_INFO,dwContextID,&GameColumnInfo,wPacketSize);

		return false;
	}

	return true;
}

//���ػ���
bool CDataBaseEngineSink::OnRequestLoadAndroidUser(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//��������
		DBO_GR_GameAndroidInfo GameAndroidInfo;
		ZeroMemory(&GameAndroidInfo,sizeof(GameAndroidInfo));

		//�û��ʻ�
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceAttrib->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

		//ִ�в�ѯ
		GameAndroidInfo.lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LoadAndroidUser"),true);

		//��ȡ��Ϣ
		for (WORD i=0;i<CountArray(GameAndroidInfo.AndroidParameter);i++)
		{
			//�����ж�
			if (m_GameDBModule->IsRecordsetEnd()==true) break;

			//���Ч��
			ASSERT(GameAndroidInfo.wAndroidCount<CountArray(GameAndroidInfo.AndroidParameter));
			if (GameAndroidInfo.wAndroidCount>=CountArray(GameAndroidInfo.AndroidParameter)) break;

			//��ȡ����
			GameAndroidInfo.wAndroidCount++;
			GameAndroidInfo.AndroidParameter[i].dwUserID=m_GameDBAide.GetValue_DWORD(TEXT("UserID"));
			GameAndroidInfo.AndroidParameter[i].dwServiceTime=m_GameDBAide.GetValue_DWORD(TEXT("ServiceTime"));
			GameAndroidInfo.AndroidParameter[i].dwMinPlayDraw=m_GameDBAide.GetValue_DWORD(TEXT("MinPlayDraw"));
			GameAndroidInfo.AndroidParameter[i].dwMaxPlayDraw=m_GameDBAide.GetValue_DWORD(TEXT("MaxPlayDraw"));
			GameAndroidInfo.AndroidParameter[i].dwMinReposeTime=m_GameDBAide.GetValue_DWORD(TEXT("MinReposeTime"));
			GameAndroidInfo.AndroidParameter[i].dwMaxReposeTime=m_GameDBAide.GetValue_DWORD(TEXT("MaxReposeTime"));
			GameAndroidInfo.AndroidParameter[i].dwServiceGender=m_GameDBAide.GetValue_DWORD(TEXT("ServiceGender"));
			GameAndroidInfo.AndroidParameter[i].lMinTakeScore=m_GameDBAide.GetValue_LONGLONG(TEXT("MinTakeScore"));
			GameAndroidInfo.AndroidParameter[i].lMaxTakeScore=m_GameDBAide.GetValue_LONGLONG(TEXT("MaxTakeScore"));

			m_GameDBModule->MoveToNext();
		}

		//������Ϣ
		WORD wHeadSize=sizeof(GameAndroidInfo)-sizeof(GameAndroidInfo.AndroidParameter);
		WORD wDataSize=GameAndroidInfo.wAndroidCount*sizeof(GameAndroidInfo.AndroidParameter[0]);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_GAME_ANDROID_INFO,dwContextID,&GameAndroidInfo,wHeadSize+wDataSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GR_LoadAndroidUser"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//��������
		DBO_GR_GameAndroidInfo GameAndroidInfo;
		ZeroMemory(&GameAndroidInfo,sizeof(GameAndroidInfo));

		//�������
		GameAndroidInfo.wAndroidCount=0L;
		GameAndroidInfo.lResultCode=DB_ERROR;

		//������Ϣ
		WORD wHeadSize=sizeof(GameAndroidInfo)-sizeof(GameAndroidInfo.AndroidParameter);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_GAME_ANDROID_INFO,dwContextID,&GameAndroidInfo,wHeadSize);
	}

	return false;
}

//���ص���
bool CDataBaseEngineSink::OnRequestLoadGameProperty(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//��������
		DBO_GR_GamePropertyInfo GamePropertyInfo;
		ZeroMemory(&GamePropertyInfo,sizeof(GamePropertyInfo));

		//�������
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

		//ִ�в�ѯ
		GamePropertyInfo.lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LoadGameProperty"),true);

		//��ȡ��Ϣ
		for (WORD i=0;i<CountArray(GamePropertyInfo.PropertyInfo);i++)
		{
			//�����ж�
			if (m_GameDBModule->IsRecordsetEnd()==true) break;

			//���Ч��
			ASSERT(GamePropertyInfo.cbPropertyCount<CountArray(GamePropertyInfo.PropertyInfo));
			if (GamePropertyInfo.cbPropertyCount>=CountArray(GamePropertyInfo.PropertyInfo)) break;

			//��ȡ����
			GamePropertyInfo.cbPropertyCount++;
			GamePropertyInfo.PropertyInfo[i].wIndex=m_GameDBAide.GetValue_WORD(TEXT("ID"));
			GamePropertyInfo.PropertyInfo[i].wDiscount=m_GameDBAide.GetValue_WORD(TEXT("Discount"));
			GamePropertyInfo.PropertyInfo[i].wIssueArea=m_GameDBAide.GetValue_WORD(TEXT("IssueArea"));
			GamePropertyInfo.PropertyInfo[i].dPropertyCash=m_GameDBAide.GetValue_DOUBLE(TEXT("Cash"));
			GamePropertyInfo.PropertyInfo[i].lPropertyGold=m_GameDBAide.GetValue_LONGLONG(TEXT("Gold"));
			GamePropertyInfo.PropertyInfo[i].lSendLoveLiness=m_GameDBAide.GetValue_LONGLONG(TEXT("SendLoveLiness"));
			GamePropertyInfo.PropertyInfo[i].lRecvLoveLiness=m_GameDBAide.GetValue_LONGLONG(TEXT("RecvLoveLiness"));

			m_GameDBModule->MoveToNext();
		}

		//������Ϣ
		WORD wHeadSize=sizeof(GamePropertyInfo)-sizeof(GamePropertyInfo.PropertyInfo);
		WORD wDataSize=GamePropertyInfo.cbPropertyCount*sizeof(GamePropertyInfo.PropertyInfo[0]);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GAME_PROPERTY_INFO,dwContextID,&GamePropertyInfo,wHeadSize+wDataSize);
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GR_LoadGameProperty"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//��������
		DBO_GR_GamePropertyInfo GamePropertyInfo;
		ZeroMemory(&GamePropertyInfo,sizeof(GamePropertyInfo));

		//�������
		GamePropertyInfo.cbPropertyCount=0L;
		GamePropertyInfo.lResultCode=DB_ERROR;

		//������Ϣ
		WORD wHeadSize=sizeof(GamePropertyInfo)-sizeof(GamePropertyInfo.PropertyInfo);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_GAME_PROPERTY_INFO,dwContextID,&GamePropertyInfo,wHeadSize);

		return false;
	}

	return true;
}

//������Ϸ��
bool CDataBaseEngineSink::OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_UserSaveScore));
	if (wDataSize!=sizeof(DBR_GR_UserSaveScore)) return false;

	//��������
	DBR_GR_UserSaveScore * pUserSaveScore=(DBR_GR_UserSaveScore *)pData;
	dwUserID=pUserSaveScore->dwUserID;

	//������
	try
	{
		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserSaveScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserSaveScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lSaveScore"),pUserSaveScore->lSaveScore);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserSaveScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserSaveScore"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,pUserSaveScore->lSaveScore,CW2CT(DBVarValue.bstrVal),false,pUserSaveScore->cbActivityGame);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_UserSaveScore"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,pUserSaveScore->lSaveScore,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false,pUserSaveScore->cbActivityGame);

		return false;
	}

	return true;
}

//��ȡ��Ϸ��
bool CDataBaseEngineSink::OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_UserTakeScore));
	if (wDataSize!=sizeof(DBR_GR_UserTakeScore)) return false;

	//��������
	DBR_GR_UserTakeScore * pUserTakeScore=(DBR_GR_UserTakeScore *)pData;
	dwUserID=pUserTakeScore->dwUserID;

	//������
	try
	{
		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTakeScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTakeScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lTakeScore"),pUserTakeScore->lTakeScore);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTakeScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTakeScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTakeScore"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,0L,CW2CT(DBVarValue.bstrVal),false,pUserTakeScore->cbActivityGame);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_UserTakeScore"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false,pUserTakeScore->cbActivityGame);

		return false;
	}

	return true;
}

//ת����Ϸ��
bool CDataBaseEngineSink::OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_UserTransferScore));
	if (wDataSize!=sizeof(DBR_GR_UserTransferScore)) return false;

	//��������
	DBR_GR_UserTransferScore * pUserTransferScore=(DBR_GR_UserTransferScore *)pData;
	dwUserID=pUserTransferScore->dwUserID;

	//������
	try
	{
		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTransferScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTransferScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@cbByNickName"),pUserTransferScore->cbByNickName);
		m_TreasureDBAide.AddParameter(TEXT("@lTransferScore"),pUserTransferScore->lTransferScore);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTransferScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@strNickName"),pUserTransferScore->szNickName);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTransferScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTransferScore"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnInsureDisposeResult(dwContextID,lResultCode,0L,CW2CT(DBVarValue.bstrVal),false,pUserTransferScore->cbActivityGame);
		//OnInsureDisposeResult(dwContextID,lResultCode,0L,TEXT(""),false,pUserTransferScore->cbActivityGame);
		OnInsureTransferInfo(dwContextID,lResultCode);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_UserTransferScore"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false,pUserTransferScore->cbActivityGame);

		return false;
	}

	return true;
}

//��ѯ����
bool CDataBaseEngineSink::OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_QueryInsureInfo));
	if (wDataSize!=sizeof(DBR_GR_QueryInsureInfo)) return false;

	//������
	DBR_GR_QueryInsureInfo * pQueryInsureInfo=(DBR_GR_QueryInsureInfo *)pData;
	dwUserID=pQueryInsureInfo->dwUserID;

	try
	{
		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pQueryInsureInfo->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pQueryInsureInfo->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryUserInsureInfo"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GR_UserInsureInfo UserInsureInfo;
			ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

			//������Ϣ
			UserInsureInfo.cbActivityGame=pQueryInsureInfo->cbActivityGame;
			UserInsureInfo.wRevenueTake=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTake"));
			UserInsureInfo.wRevenueTransfer=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTransfer"));
			UserInsureInfo.wServerID=m_TreasureDBAide.GetValue_WORD(TEXT("ServerID"));
			UserInsureInfo.lUserScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Score"));
			UserInsureInfo.lUserInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Insure"));
			UserInsureInfo.lTransferPrerequisite=m_TreasureDBAide.GetValue_LONGLONG(TEXT("TransferPrerequisite"));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_INSURE_INFO,dwContextID,&UserInsureInfo,sizeof(UserInsureInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//������
			OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),0L,CW2CT(DBVarValue.bstrVal),false,pQueryInsureInfo->cbActivityGame);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GR_QueryUserInsureInfo"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false,pQueryInsureInfo->cbActivityGame);

		return false;
	}

	return true;
}

//��ѯ�û�
bool CDataBaseEngineSink::OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_QueryTransferUserInfo));
	if (wDataSize!=sizeof(DBR_GR_QueryTransferUserInfo)) return false;

	//������
	DBR_GR_QueryTransferUserInfo * pQueryTransferUserInfo=(DBR_GR_QueryTransferUserInfo *)pData;
	dwUserID=pQueryTransferUserInfo->dwUserID;

	try
	{
		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@cbByNickName"),pQueryTransferUserInfo->cbByNickName);
		m_TreasureDBAide.AddParameter(TEXT("@strNickName"),pQueryTransferUserInfo->szNickName);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		if (m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_QueryTransferUserInfo"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GR_UserTransferUserInfo TransferUserInfo;
			ZeroMemory(&TransferUserInfo,sizeof(TransferUserInfo));

			//������Ϣ
			TransferUserInfo.cbActivityGame=pQueryTransferUserInfo->cbActivityGame;
			TransferUserInfo.dwGameID=m_TreasureDBAide.GetValue_DWORD(TEXT("GameID"));
			m_TreasureDBAide.GetValue_String(TEXT("NickName"), TransferUserInfo.szNickName, CountArray(TransferUserInfo.szNickName));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_INSURE_USER_INFO,dwContextID,&TransferUserInfo,sizeof(TransferUserInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//������
			OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),0L,CW2CT(DBVarValue.bstrVal),false,pQueryTransferUserInfo->cbActivityGame);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GR_QueryTransferUserInfo"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false,pQueryTransferUserInfo->cbActivityGame);

		return false;
	}

	return true;
}

//��������
bool CDataBaseEngineSink::OnRequestPropertyRequest(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GR_PropertyRequest));
		if (wDataSize!=sizeof(DBR_GR_PropertyRequest)) return false;

		//������
		DBR_GR_PropertyRequest * pPropertyRequest=(DBR_GR_PropertyRequest *)pData;
		dwUserID=pPropertyRequest->dwSourceUserID;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pPropertyRequest->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_GameDBAide.ResetParameter();

		//������Ϣ
		m_GameDBAide.AddParameter(TEXT("@dwSourceUserID"),pPropertyRequest->dwSourceUserID);
		m_GameDBAide.AddParameter(TEXT("@dwTargetUserID"),pPropertyRequest->dwTargetUserID);
		m_GameDBAide.AddParameter(TEXT("@wPropertyID"),pPropertyRequest->wPropertyIndex);
		m_GameDBAide.AddParameter(TEXT("@wPropertyCount"),pPropertyRequest->wItemCount);

		//��������
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameter(TEXT("@wTableID"),pPropertyRequest->wTableID);

		//����ʽ
		m_GameDBAide.AddParameter(TEXT("@cbConsumeScore"),pPropertyRequest->cbConsumeScore);
		m_GameDBAide.AddParameter(TEXT("@lFrozenedScore"),pPropertyRequest->lFrozenedScore);

		//ϵͳ��Ϣ
		m_GameDBAide.AddParameter(TEXT("@dwEnterID"),pPropertyRequest->dwInoutIndex);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_GameDBAide.AddParameter(TEXT("@strMachineID"),pPropertyRequest->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		if (m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_ConsumeProperty"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GR_S_PropertySuccess PresentSuccess;
			ZeroMemory(&PresentSuccess,sizeof(PresentSuccess));

			//������Ϣ
			PresentSuccess.cbRequestArea=pPropertyRequest->cbRequestArea;
			PresentSuccess.wItemCount=pPropertyRequest->wItemCount;
			PresentSuccess.wPropertyIndex=pPropertyRequest->wPropertyIndex;
			PresentSuccess.dwSourceUserID=pPropertyRequest->dwSourceUserID;
			PresentSuccess.dwTargetUserID=pPropertyRequest->dwTargetUserID;

			//����ģʽ
			PresentSuccess.cbConsumeScore=pPropertyRequest->cbConsumeScore;
			PresentSuccess.lFrozenedScore=pPropertyRequest->lFrozenedScore;

			//�û�Ȩ��
			PresentSuccess.dwUserRight=pPropertyRequest->dwUserRight;

			//�����Ϣ
			PresentSuccess.lConsumeGold=m_GameDBAide.GetValue_LONGLONG(TEXT("ConsumeGold"));
			PresentSuccess.lSendLoveLiness=m_GameDBAide.GetValue_LONG(TEXT("SendLoveLiness"));
			PresentSuccess.lRecvLoveLiness=m_GameDBAide.GetValue_LONG(TEXT("RecvLoveLiness"));
			PresentSuccess.cbMemberOrder=m_GameDBAide.GetValue_BYTE(TEXT("MemberOrder"));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_PROPERTY_SUCCESS,dwContextID,&PresentSuccess,sizeof(PresentSuccess));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_GameDBAide.GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//��������
			DBO_GR_PropertyFailure PropertyFailure;
			ZeroMemory(&PropertyFailure,sizeof(PropertyFailure));

			//���ñ���
			PropertyFailure.lResultCode=m_GameDBAide.GetReturnValue();
			PropertyFailure.lFrozenedScore=pPropertyRequest->lFrozenedScore;
			PropertyFailure.cbRequestArea=pPropertyRequest->cbRequestArea;
			lstrcpyn(PropertyFailure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(PropertyFailure.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(PropertyFailure.szDescribeString);
			WORD wHeadSize=sizeof(PropertyFailure)-sizeof(PropertyFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_PROPERTY_FAILURE,dwContextID,&PropertyFailure,wHeadSize+wDataSize);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GR_ConsumeProperty"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//��������
		DBO_GR_PropertyFailure PropertyFailure;
		ZeroMemory(&PropertyFailure,sizeof(PropertyFailure));

		//��������
		DBR_GR_PropertyRequest * pPropertyRequest=(DBR_GR_PropertyRequest *)pData;

		//���ñ���
		PropertyFailure.lResultCode=DB_ERROR;
		PropertyFailure.lFrozenedScore=pPropertyRequest->lFrozenedScore;
		PropertyFailure.cbRequestArea=pPropertyRequest->cbRequestArea;
		lstrcpyn(PropertyFailure.szDescribeString,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),CountArray(PropertyFailure.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(PropertyFailure.szDescribeString);
		WORD wHeadSize=sizeof(PropertyFailure)-sizeof(PropertyFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_PROPERTY_FAILURE,dwContextID,&PropertyFailure,wHeadSize+wDataSize);

		return false;
	}

	return true;
}

//������
bool CDataBaseEngineSink::OnRequestCheckTrumpet(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
printf("%s %d\n", __FUNCTION__, __LINE__);
	//������
	try
	{
		if (wDataSize!=sizeof(DBR_GR_CheckTrumpet)) return false;
		DBR_GR_CheckTrumpet * pCheckTrumpet = (DBR_GR_CheckTrumpet *)pData;
		pCheckTrumpet->lReturnValue = 3;
		//�������
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pCheckTrumpet->dwUserID);

		//�������
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_AgentHornInfo"),true);

		pCheckTrumpet->lReturnValue = lReturnValue;

		//���ͽ��
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_CHECK_TRUMPET,dwContextID,pCheckTrumpet,sizeof(DBR_GR_CheckTrumpet));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_AgentHornInfo"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestFirstEnterSuper(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	printf("%s %d\n", __FUNCTION__, __LINE__);
	//������
	try
	{
		if (wDataSize!=sizeof(DBR_GR_First_Enter_Super)) return false;
		DBR_GR_First_Enter_Super * pRequestInfo = (DBR_GR_First_Enter_Super *)pData;
		//�������
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@UserID"),pRequestInfo->dwUserID);
		//�������
		dwUserID = pRequestInfo->dwUserID;
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_First_Enter_Super"),true);
		if (lReturnValue==0)
		{
			int nRewardCoin = m_GameDBAide.GetValue_LONG(TEXT("SuperGold"));
			DBO_GR_First_Enter_Super sResult={0};
			sResult.nRewardCoin = nRewardCoin;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DB0_GR_FIRST_ENTER_SUPER,dwContextID,&sResult,sizeof(sResult));
		}
		//���ͽ��


		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_AgentHornInfo"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��������
bool CDataBaseEngineSink::OnRequestMatchFee(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_MatchFee));
	if (wDataSize!=sizeof(DBR_GR_MatchFee)) return false;

	//��������
	DBR_GR_MatchFee * pMatchFee=(DBR_GR_MatchFee *)pData;
	dwUserID=INVALID_DWORD;

	//������
	try
	{
		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pMatchFee->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pMatchFee->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@dwMatchFee"),pMatchFee->dwMatchFee);
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_GameDBAide.AddParameter(TEXT("@dwMatchID"),pMatchFee->dwMatchID);
		m_GameDBAide.AddParameter(TEXT("@dwMatchNO"),pMatchFee->dwMatchNO);
		m_GameDBAide.AddParameter(TEXT("@strMachineID"),pMatchFee->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_UserMatchFee"),true);

		//���ͽ��
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_MATCH_FEE_RESULT,dwContextID,&lReturnValue,sizeof(lReturnValue));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_UserMatchFee"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//������ʼ
bool CDataBaseEngineSink::OnRequestMatchStart(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_MatchStart));
	if (wDataSize!=sizeof(DBR_GR_MatchStart)) return false;

	//��������
	DBR_GR_MatchStart * pMatchStart=(DBR_GR_MatchStart *)pData;
	dwUserID=INVALID_DWORD;

	//������
	try
	{
		//�������
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wMatchID"),pMatchStart->dwMatchID);
		m_GameDBAide.AddParameter(TEXT("@wMatchNo"),pMatchStart->dwMatchNo);
		m_GameDBAide.AddParameter(TEXT("@wMatchCount"),pMatchStart->wMatchCount);	
		m_GameDBAide.AddParameter(TEXT("@lInitScore"),pMatchStart->lInitScore);	

// 		//�������
// 		TCHAR szDescribeString[128]=TEXT("");
// 		m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_MatchStart"),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_MatchStart"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��������
bool CDataBaseEngineSink::OnRequestMatchOver(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_MatchOver));
	if (wDataSize!=sizeof(DBR_GR_MatchOver)) return false;

	//��������
	DBR_GR_MatchOver * pMatchOver=(DBR_GR_MatchOver *)pData;
	dwUserID=INVALID_DWORD;

	//������
	try
	{
		//�������
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wMatchID"),pMatchOver->dwMatchID);
		m_GameDBAide.AddParameter(TEXT("@wMatchNo"),pMatchOver->dwMatchNo);
		m_GameDBAide.AddParameter(TEXT("@wMatchCount"),pMatchOver->wMatchCount);		

// 		//�������
// 		TCHAR szDescribeString[128]=TEXT("");
// 		m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_MatchOver"),true);

		if(lReturnValue==DB_SUCCESS)
		{
			LONG lCount=0;
			if(m_GameDBModule.GetInterface()!=NULL)
				lCount=m_GameDBModule->GetRecordCount();
			DBO_GR_MatchRank *pMatchRank=new DBO_GR_MatchRank[lCount];
			for(LONG i=0; i<lCount; i++)
			{
				pMatchRank[i].cbRank=(BYTE)i;
				m_GameDBAide.GetValue_String(TEXT("NickName"),pMatchRank[i].szNickName,CountArray(pMatchRank[i].szNickName));
				pMatchRank[i].lMatchScore=m_GameDBAide.GetValue_LONG(TEXT("HScore"));
				m_GameDBModule->MoveToNext();
				if(m_GameDBModule->IsRecordsetEnd()) break;
			}
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_MATCH_RANK,dwContextID,pMatchRank,(WORD)(sizeof(DBO_GR_MatchRank)*lCount));
			if(pMatchRank!=NULL)
				SafeDeleteArray(pMatchRank);
		}
	
		
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_MatchOver"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��������
bool CDataBaseEngineSink::OnRequestMatchReward(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_MatchReward));
	if (wDataSize!=sizeof(DBR_GR_MatchReward)) return false;

	//��������
	DBR_GR_MatchReward * pMatchReward=(DBR_GR_MatchReward *)pData;
	dwUserID=INVALID_DWORD;

	//������
	try
	{
		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pMatchReward->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pMatchReward->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@dwMatchID"),pMatchReward->dwMatchID);
		m_GameDBAide.AddParameter(TEXT("@dwMatchNO"),pMatchReward->dwMatchNO);
		m_GameDBAide.AddParameter(TEXT("@wRank"),pMatchReward->wRank);
		m_GameDBAide.AddParameter(TEXT("@lMatchScore"),pMatchReward->lMatchScore);
		m_GameDBAide.AddParameter(TEXT("@dwExperience"),pMatchReward->dwExperience);
		m_GameDBAide.AddParameter(TEXT("@dwGold"),pMatchReward->dwGold);
		m_GameDBAide.AddParameter(TEXT("@dwMedal"),pMatchReward->dwMedal);
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_MatchReward"),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_MatchReward"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�˳�����
bool CDataBaseEngineSink::OnRequestMatchQuit(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_MatchFee));
	if (wDataSize!=sizeof(DBR_GR_MatchFee)) return false;

	//��������
	DBR_GR_MatchFee * pMatchFee=(DBR_GR_MatchFee *)pData;
	dwUserID=INVALID_DWORD;

	//������
	try
	{
		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pMatchFee->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pMatchFee->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@dwMatchFee"),pMatchFee->dwMatchFee);
		m_GameDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_GameDBAide.AddParameter(TEXT("@dwMatchID"),pMatchFee->dwMatchID);
		m_GameDBAide.AddParameter(TEXT("@dwMatchNO"),pMatchFee->dwMatchNO);
		m_GameDBAide.AddParameter(TEXT("@strMachineID"),pMatchFee->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_GameDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_UserMatchQuit"),true);

		//���ͽ��
		if(INVALID_WORD!=dwContextID)
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_MATCH_QUIT_RESULT,dwContextID,&lReturnValue,sizeof(lReturnValue));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_UserMatchQuit"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}
}

//�û�Ȩ��
bool CDataBaseEngineSink::OnRequestManageUserRight(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GR_ManageUserRight));
		if (wDataSize!=sizeof(DBR_GR_ManageUserRight)) return false;

		//������
		DBR_GR_ManageUserRight * pManageUserRight=(DBR_GR_ManageUserRight *)pData;
		dwUserID=pManageUserRight->dwUserID;

		//�������
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@dwUserID"),pManageUserRight->dwUserID);
		m_GameDBAide.AddParameter(TEXT("@dwAddRight"),pManageUserRight->dwAddRight);
		m_GameDBAide.AddParameter(TEXT("@dwRemoveRight"),pManageUserRight->dwRemoveRight);

		//ִ�в�ѯ
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_ManageUserRight"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_ManageUserRight"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//ϵͳ��Ϣ
bool CDataBaseEngineSink::OnRequestLoadSystemMessage(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//�������
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

		//ִ�в�ѯ
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LoadSystemMessage"),true);

		//�������
		if(lReturnValue==0)
		{
			TCHAR szServerID[32]={0};
			_sntprintf(szServerID, CountArray(szServerID), TEXT("%d"), m_pGameServiceOption->wServerID);

			while(true)
			{
				//�����ж�
				if (m_GameDBModule->IsRecordsetEnd()==true) break;

				//�������
				TCHAR szServerRange[1024]={0};
				CString strServerRange;
				bool bSendMessage=false;
				bool bAllRoom=false;

				//��ȡ��Χ
				m_GameDBAide.GetValue_String(TEXT("ServerRange"), szServerRange, CountArray(szServerRange));
				szServerRange[1023]=0;
				strServerRange.Format(TEXT("%s"), szServerRange);

				//��Χ�ж�
				while(true)
				{
					int nfind=strServerRange.Find(TEXT(','));
					if(nfind!=-1 && nfind>0)
					{
						CString strID=strServerRange.Left(nfind);
						WORD wServerID=StrToInt(strID);
						bSendMessage=(wServerID==0 || wServerID==m_pGameServiceOption->wServerID);
						if(wServerID==0)bAllRoom=true;

						if(bSendMessage) break;

						strServerRange=strServerRange.Right(strServerRange.GetLength()-nfind-1);
					}
					else
					{
						WORD wServerID=StrToInt(szServerRange);
						bSendMessage=(wServerID==0 || wServerID==m_pGameServiceOption->wServerID);
						if(wServerID==0)bAllRoom=true;

						break;
					}
				}

				//������Ϣ
				if(bSendMessage)
				{
					//�������
					DBR_GR_SystemMessage SystemMessage;
					ZeroMemory(&SystemMessage, sizeof(SystemMessage));

					//��ȡ��Ϣ
					SystemMessage.cbMessageID=m_GameDBAide.GetValue_BYTE(TEXT("ID"));
					SystemMessage.cbMessageType=m_GameDBAide.GetValue_BYTE(TEXT("MessageType"));
					SystemMessage.dwTimeRate=m_GameDBAide.GetValue_DWORD(TEXT("TimeRate"));
					SystemMessage.cbAllRoom=bAllRoom?TRUE:FALSE;
					m_GameDBAide.GetValue_String(TEXT("MessageString"),SystemMessage.szSystemMessage,CountArray(SystemMessage.szSystemMessage));

					//���ͽ��
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_SYSTEM_MESSAGE_RESULT,dwContextID,&SystemMessage,sizeof(SystemMessage));
				}

				//��һ��
				m_GameDBModule->MoveToNext();
			}
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_LoadSystemMessage"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,0L,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�������д�
bool CDataBaseEngineSink::OnRequestLoadSensitiveWords(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//�������
		m_PlatformDBAide.ResetParameter();

		//ִ�в�ѯ
		LONG lReturnValue=m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GR_LoadSensitiveWords"),true);

		//��ȡ��Ϣ
		if (lReturnValue==DB_SUCCESS)
		{
			//��ʼ��Ϣ
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_SENSITIVE_WORDS,0xfffe,NULL,0);

			//��ȡ��Ϣ
			while (m_PlatformDBModule->IsRecordsetEnd()==false)
			{
				//��������
				TCHAR szSensitiveWords[32]=TEXT("");

				//��ȡ��Ϣ
				m_PlatformDBAide.GetValue_String(TEXT("SensitiveWords"),szSensitiveWords,CountArray(szSensitiveWords));				

				//������Ϣ
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_SENSITIVE_WORDS,0,szSensitiveWords,sizeof(szSensitiveWords));

				//�ƶ���¼
				m_PlatformDBModule->MoveToNext();
			};

			//������Ϣ
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_SENSITIVE_WORDS,0xffff,NULL,0);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CTraceService::TraceString(TEXT("GSP_GR_LoadSensitiveWords"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//��¼���
VOID CDataBaseEngineSink::OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,BYTE cbDeviceType,WORD wBehaviorFlags,WORD wPageTableCount)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//��������
		m_LogonSuccess.wFaceID=m_GameDBAide.GetValue_WORD(TEXT("FaceID"));
		m_LogonSuccess.dwUserID=m_GameDBAide.GetValue_DWORD(TEXT("UserID"));
		m_LogonSuccess.dwGameID=m_GameDBAide.GetValue_DWORD(TEXT("GameID"));
		m_LogonSuccess.dwGroupID=m_GameDBAide.GetValue_DWORD(TEXT("GroupID"));
		m_LogonSuccess.dwCustomID=m_GameDBAide.GetValue_DWORD(TEXT("CustomID"));
		m_GameDBAide.GetValue_String(TEXT("NickName"),m_LogonSuccess.szNickName,CountArray(m_LogonSuccess.szNickName));
		m_GameDBAide.GetValue_String(TEXT("GroupName"),m_LogonSuccess.szGroupName,CountArray(m_LogonSuccess.szGroupName));

		//�û�����
		m_LogonSuccess.cbGender=m_GameDBAide.GetValue_BYTE(TEXT("Gender"));
		m_LogonSuccess.cbMemberOrder=m_GameDBAide.GetValue_BYTE(TEXT("MemberOrder"));
		m_LogonSuccess.cbMasterOrder=m_GameDBAide.GetValue_BYTE(TEXT("MasterOrder"));
		m_GameDBAide.GetValue_String(TEXT("UnderWrite"),m_LogonSuccess.szUnderWrite,CountArray(m_LogonSuccess.szUnderWrite));

		//������Ϣ
		m_LogonSuccess.lScore=m_GameDBAide.GetValue_LONGLONG(TEXT("Score"));
		m_LogonSuccess.lGrade=m_GameDBAide.GetValue_LONGLONG(TEXT("Grade"));
		m_LogonSuccess.lInsure=m_GameDBAide.GetValue_LONGLONG(TEXT("Insure"));

		//������Ϣ
		m_LogonSuccess.dwWinCount=m_GameDBAide.GetValue_LONG(TEXT("WinCount"));
		m_LogonSuccess.dwLostCount=m_GameDBAide.GetValue_LONG(TEXT("LostCount"));
		m_LogonSuccess.dwDrawCount=m_GameDBAide.GetValue_LONG(TEXT("DrawCount"));
		m_LogonSuccess.dwFleeCount=m_GameDBAide.GetValue_LONG(TEXT("FleeCount"));
		m_LogonSuccess.dwUserMedal=m_GameDBAide.GetValue_LONG(TEXT("UserMedal"));
		m_LogonSuccess.dwExperience=m_GameDBAide.GetValue_LONG(TEXT("Experience"));
		m_LogonSuccess.lLoveLiness=m_GameDBAide.GetValue_LONG(TEXT("LoveLiness"));

		//������Ϣ
		m_LogonSuccess.dwUserRight=m_GameDBAide.GetValue_DWORD(TEXT("UserRight"));
		m_LogonSuccess.dwMasterRight=m_GameDBAide.GetValue_DWORD(TEXT("MasterRight"));
		m_LogonSuccess.nTreasureMapCount=m_GameDBAide.GetValue_LONG(TEXT("TreasureMap"));
		m_LogonSuccess.cbDeviceType=cbDeviceType;
		m_LogonSuccess.wBehaviorFlags=wBehaviorFlags;
		m_LogonSuccess.wPageTableCount=wPageTableCount;

		//��������
		m_LogonSuccess.dwInoutIndex=m_GameDBAide.GetValue_DWORD(TEXT("InoutIndex"));

		m_LogonSuccess.cbVIPLevel=m_GameDBAide.GetValue_BYTE(TEXT("vipLevel"));
		m_LogonSuccess.dwVIPExp=m_GameDBAide.GetValue_DWORD(TEXT("vipExp"));

		//��ȡ��Ϣ
		if(pszErrorString!=NULL)lstrcpyn(m_LogonSuccess.szDescribeString,pszErrorString,CountArray(m_LogonSuccess.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(m_LogonSuccess.szDescribeString);
		WORD wHeadSize=sizeof(m_LogonSuccess)-sizeof(m_LogonSuccess.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_LOGON_SUCCESS,dwContextID,&m_LogonSuccess,wHeadSize+wDataSize);

		//////////////////////////////////////////////////////////////////////////
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@dwUserID"), m_LogonSuccess.dwUserID);

		LONG lReturnValue = m_GameDBAide.ExecuteProcess(TEXT("GSP_GP_LoadUserItem"),true);

		if (0 == lReturnValue && 0 != m_GameDBModule->GetRecordCount())
		{
			m_GameDBModule->MoveToFirst();
			while ( m_GameDBModule->IsRecordsetEnd() == false )
			{
				tagUserItem useritem;
				memset(&useritem, 0, sizeof(useritem));
				bool bValue = false;

				useritem.nUserID = m_LogonSuccess.dwUserID;
				useritem.nDBID = m_GameDBAide.GetValue_LONG(TEXT("id"));
				useritem.nType = m_GameDBAide.GetValue_LONG(TEXT("type"));
				useritem.lDBCount = m_GameDBAide.GetValue_LONGLONG(TEXT("count"));
				useritem.lCount = useritem.lDBCount;

				useritem.nFlag = m_GameDBAide.GetValue_LONG(TEXT("flag"));

				bValue = m_GameDBAide.GetValue_LONG(TEXT("isgamepreload")) > 0;
				if (bValue)
				{
					useritem.nFlag |= eUserItemFlag_GamePreLoad;
				}

				useritem.guidTag[0] = m_GameDBAide.GetValue_DWORD(TEXT("guidtag1"));
				useritem.guidTag[1] = m_GameDBAide.GetValue_DWORD(TEXT("guidtag2"));
				useritem.guidTag[2] = m_GameDBAide.GetValue_DWORD(TEXT("guidtag3"));
				useritem.guidTag[3] = m_GameDBAide.GetValue_DWORD(TEXT("guidtag4"));

				useritem.nSourceID = m_GameDBAide.GetValue_LONG(TEXT("source"));
				useritem.nSubSourceID = m_GameDBAide.GetValue_LONG(TEXT("subsource"));

				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_LOADUSERITEM_SUCCESS,dwContextID,&useritem,sizeof(useritem));
				m_GameDBModule->MoveToNext();
			}
		}
		//////////////////////////////////////////////////////////////////////////
	}
	else
	{
		//��������
		DBO_GR_LogonFailure LogonFailure;
		ZeroMemory(&LogonFailure,sizeof(LogonFailure));

		//��������
		LogonFailure.lResultCode=dwErrorCode;
		lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
		WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
	}

	return;
}

//������Ϣ
VOID CDataBaseEngineSink::OnInsureTransferInfo(DWORD dwContextID, DWORD dwErrorCode)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		DBO_GR_UserInsureTransferInfo UserInsureTransferInfo={0};
		UserInsureTransferInfo.dwRecordID=m_TreasureDBAide.GetValue_DWORD(TEXT("RecordID"));
		UserInsureTransferInfo.dwSourceID=m_TreasureDBAide.GetValue_DWORD(TEXT("SourceGameID"));
		UserInsureTransferInfo.dwTargetID=m_TreasureDBAide.GetValue_DWORD(TEXT("TargetGameID"));
		UserInsureTransferInfo.lSwapScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("TransferScore"));
		m_TreasureDBAide.GetValue_String(TEXT("SourceNickName"),UserInsureTransferInfo.szSourceName,CountArray(UserInsureTransferInfo.szSourceName));
		m_TreasureDBAide.GetValue_String(TEXT("TargetNickName"),UserInsureTransferInfo.szTargetName,CountArray(UserInsureTransferInfo.szTargetName));
		m_TreasureDBAide.GetValue_SystemTime(TEXT("TransferTime"),UserInsureTransferInfo.timeTransfer);

		WORD wDataSize=sizeof(DBO_GR_UserInsureTransferInfo);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_INSURE_TRANSFER_INFO,dwContextID,&UserInsureTransferInfo,wDataSize);
	}
}

//���н��
VOID CDataBaseEngineSink::OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, SCORE lFrozenedScore, LPCTSTR pszErrorString, bool bMobileClient, BYTE cbActivityGame)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//��������
		DBO_GR_UserInsureSuccess UserInsureSuccess;
		ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

		//�������
		UserInsureSuccess.cbActivityGame=cbActivityGame;
		UserInsureSuccess.lFrozenedScore=lFrozenedScore;
		UserInsureSuccess.dwUserID=m_TreasureDBAide.GetValue_DWORD(TEXT("UserID"));
		UserInsureSuccess.lSourceScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("SourceScore"));
		UserInsureSuccess.lSourceInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("SourceInsure"));
		UserInsureSuccess.lInsureRevenue=m_TreasureDBAide.GetValue_LONGLONG(TEXT("InsureRevenue"));
		UserInsureSuccess.lVariationScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("VariationScore"));
		UserInsureSuccess.lVariationInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("VariationInsure"));
		lstrcpyn(UserInsureSuccess.szDescribeString,pszErrorString,CountArray(UserInsureSuccess.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(UserInsureSuccess.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_INSURE_SUCCESS,dwContextID,&UserInsureSuccess,wHeadSize+wDataSize);
	}
	else
	{
		//��������
		DBO_GR_UserInsureFailure UserInsureFailure;
		ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

		//�������
		UserInsureFailure.cbActivityGame=cbActivityGame;
		UserInsureFailure.lResultCode=dwErrorCode;
		UserInsureFailure.lFrozenedScore=lFrozenedScore;
		lstrcpyn(UserInsureFailure.szDescribeString,pszErrorString,CountArray(UserInsureFailure.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(UserInsureFailure.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_INSURE_FAILURE,dwContextID,&UserInsureFailure,wHeadSize+wDataSize);
	}

	return;
}

//���¿��
bool CDataBaseEngineSink::OnBankCheckStock(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//����Ч��
	ASSERT(wDataSize==sizeof(tagDBR_GR_CheckStock));
	if (wDataSize!=sizeof(tagDBR_GR_CheckStock)) return false;

	tagDBR_GR_CheckStock* pCheckStock = (tagDBR_GR_CheckStock*)pData;
	if (NULL == pCheckStock) return false;

	//��ȡ�����Ϣ
	try
	{
		//�û��ʻ�
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@lTypeID"),(long)pCheckStock->lTypeID);
		m_TreasureDBAide.AddParameter(TEXT("@lRoomID"),(long)pCheckStock->lRoomID);
		m_TreasureDBAide.AddParameter(TEXT("@lTableID"),(long)pCheckStock->lTableID);
		m_TreasureDBAide.AddParameter(TEXT("@lStock"),(long)pCheckStock->lStockScore);
		m_TreasureDBAide.AddParameter(TEXT("@lRevenue"),(long)pCheckStock->lRevenue);
		m_TreasureDBAide.AddParameter(TEXT("@lCaiJin"),(long)pCheckStock->lCaiJin);
		m_TreasureDBAide.AddParameter(TEXT("@lCaiJinSent"),(long)pCheckStock->lCaiJinSent);
		m_TreasureDBAide.AddParameter(TEXT("@lbLoadOnly"),(long)(pCheckStock->lbLoad ? 1 : 0));
		m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_FishCheckStock"),true);

		tagDBR_GR_CheckStockSuccess lRet;
		memset(&lRet, 0, sizeof(lRet));
		lRet.lRoomID = pCheckStock->lRoomID;
		lRet.lTableID = pCheckStock->lTableID;
		lRet.lType = pCheckStock->lTypeID;

		if (0 != m_TreasureDBModule->GetRecordCount())
		{
			m_TreasureDBModule->MoveToFirst();

			if (false == m_TreasureDBModule->IsRecordsetEnd())
			{
				//��ȡ����
				lRet.lStockScore =m_TreasureDBAide.GetValue_LONG(TEXT("dbstock"));
				lRet.lRevenue = m_TreasureDBAide.GetValue_LONG(TEXT("dbrevenue"));
				lRet.lStockChange = m_TreasureDBAide.GetValue_LONG(TEXT("dbchangestock"));
				lRet.lCaiJin = m_TreasureDBAide.GetValue_LONG(TEXT("dbcaijin"));
				lRet.lCaiJinSent = m_TreasureDBAide.GetValue_LONG(TEXT("dbcaijinsent"));
				lRet.lbSuccess = TRUE;
			}
		}

		//Ͷ�ݵ���֪ͨ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_CHECKSTOCK_SUCCESS,dwContextID, &lRet, (WORD)(sizeof(tagDBR_GR_CheckStockSuccess)));
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(TEXT("GSP_GR_FishCheckStock"),TraceLevel_Exception);
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�رռ�¼��
	m_TreasureDBModule->CloseRecordset();

	return true;
}

//��ȡ�û�����ֵ
bool CDataBaseEngineSink::OnGetUserLucky(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	DBR_GR_UserLucky UserLucky;
	ZeroMemory(&UserLucky,sizeof(UserLucky));

	try
	{
		m_TreasureDBAide.ResetParameter();
		LONG lReturnValue=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_GetUserLucky"),true);

		DWORD dwSerial=GetTickCount();
		UserLucky.dwSerial=dwSerial;

		if (lReturnValue==0 && 0 != m_TreasureDBModule->GetRecordCount())
		{
			m_TreasureDBModule->MoveToFirst();

			while (false == m_TreasureDBModule->IsRecordsetEnd())
			{
				UserLucky.dwUserID = m_TreasureDBAide.GetValue_DWORD(TEXT("UserID"));
				UserLucky.fLucky = (float)m_TreasureDBAide.GetValue_DOUBLE(TEXT("Lucky"));
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GET_USER_LUCKY_SUCCESS,dwContextID,&UserLucky,(WORD)(sizeof(DBR_GR_UserLucky)));

				m_TreasureDBModule->MoveToNext();
			}
		}

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GET_USER_LUCKY_SUCCESS,-1,&UserLucky,(WORD)(sizeof(DBR_GR_UserLucky)));
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(TEXT("GSP_GP_GetUserLucky"),TraceLevel_Exception);
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�رռ�¼��
	m_TreasureDBModule->CloseRecordset();

	return true;
}

//�����û���Ʒ
bool CDataBaseEngineSink::OnSaveUserItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(tagUserItem));
	if (wDataSize!=sizeof(tagUserItem)) return false;

	tagUserItem * pUserItem = (tagUserItem *)pData;
	if (NULL == pUserItem) return false;

	try
	{
		SCORE lChangeCount = 0;

		if (0 == (eUserItemFlag_OverWrite & pUserItem->nFlag))
		{
			lChangeCount = pUserItem->lCount - pUserItem->lDBCount;
		}
		else
		{
			lChangeCount = pUserItem->lCount;
		}

		long nPreLoad = (0 != (eUserItemFlag_GamePreLoad & pUserItem->nFlag)) ? 1 : 0;
		long nOverRw = (0 != (eUserItemFlag_OverWrite & pUserItem->nFlag)) ? 1 : 0;
		long nHeap = (0 != (eUserItemFlag_Heap & pUserItem->nFlag)) ? 1 : 0;

		//�û��ʻ�
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@UserID"),(long)pUserItem->nUserID);
		m_TreasureDBAide.AddParameter(TEXT("@TypeID"),(long)pUserItem->nType);
		m_TreasureDBAide.AddParameter(TEXT("@ChangeCount"),lChangeCount);
		m_TreasureDBAide.AddParameter(TEXT("@SourceID"),(long)pUserItem->nSourceID);
		m_TreasureDBAide.AddParameter(TEXT("@SubSourceID"),(long)pUserItem->nSubSourceID);
		m_TreasureDBAide.AddParameter(TEXT("@IsGamePreLoad"),(long)nPreLoad);
		m_TreasureDBAide.AddParameter(TEXT("@IsOverWrite"),(long)nOverRw);
		m_TreasureDBAide.AddParameter(TEXT("@IsHeap"),(long)nHeap);
		m_TreasureDBAide.AddParameter(TEXT("@DBRowID"),(long)pUserItem->nDBID);
		m_TreasureDBAide.AddParameter(TEXT("@GuidTag1"),(long)pUserItem->guidTag[0]);
		m_TreasureDBAide.AddParameter(TEXT("@GuidTag2"),(long)pUserItem->guidTag[1]);
		m_TreasureDBAide.AddParameter(TEXT("@GuidTag3"),(long)pUserItem->guidTag[2]);
		m_TreasureDBAide.AddParameter(TEXT("@GuidTag4"),(long)pUserItem->guidTag[3]);
		m_TreasureDBAide.AddParameter(TEXT("@flag"),(long)pUserItem->nFlag);
		m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_WriteUserItem"),false);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(TEXT("GSP_GP_WriteUserItem"),TraceLevel_Exception);
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	return true;
}

//��Ϸ��¼Json��ʽ
bool CDataBaseEngineSink::OnGameJson(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//����Ч��
	ASSERT(wDataSize==sizeof(DBR_GR_GameJson));
	if (wDataSize!=sizeof(DBR_GR_GameJson)) return false;

	DBR_GR_GameJson * pGameJson = (DBR_GR_GameJson *)pData;
	if (NULL == pGameJson) return false;

	try
	{
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@UserID"),pGameJson->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@KindID"),pGameJson->dwKindID);
		m_TreasureDBAide.AddParameter(TEXT("@ServerID"),pGameJson->dwServerID);
		m_TreasureDBAide.AddParameter(TEXT("@PresentGold"),pGameJson->lPresentGold);
		m_TreasureDBAide.AddParameter(TEXT("@Json"),pGameJson->szJson);
		m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_RecordGameJson"),false);

		CString str;
		str.Format(TEXT("exec GSP_GR_SaveInsideScore %d,%d;"),pGameJson->dwUserID,0);
		m_TreasureDBAide.ExecuteSentence(str,true);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(TEXT("GSP_GR_RecordGameJson"),TraceLevel_Exception);
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		return true;
	}

	try
	{
		m_LogServerDBAide.ResetParameter();
		m_LogServerDBAide.AddParameter(TEXT("@UserID"),pGameJson->dwUserID);
		m_LogServerDBAide.AddParameter(TEXT("@KindID"),pGameJson->dwKindID);
		m_LogServerDBAide.AddParameter(TEXT("@ServerID"),pGameJson->dwServerID);
		m_LogServerDBAide.AddParameter(TEXT("@PresentGold"),pGameJson->lPresentGold);
		m_LogServerDBAide.AddParameter(TEXT("@Json"),pGameJson->szJson);
		m_LogServerDBAide.ExecuteProcess(TEXT("Log_GSP_GR_RecordGameJson"),false);
	}
	catch (IDataBaseException * pIException)
	{
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(TEXT("Log_GSP_GR_RecordGameJson"),TraceLevel_Exception);
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	return true;
}

//ִ�����ݿ�ű�
bool CDataBaseEngineSink::OnSentence(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//����Ч��
	ASSERT(wDataSize==sizeof(DBR_GR_Sentence));
	if (wDataSize!=sizeof(DBR_GR_Sentence)) return false;

	DBR_GR_Sentence * pSentence = (DBR_GR_Sentence *)pData;
	if (NULL == pSentence) return false;

	try
	{
		m_TreasureDBAide.ExecuteSentence(pSentence->szSQL,true);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	return true;
}

//��������
bool CDataBaseEngineSink::OnRequestUserTransferDragon(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//Ч�����
		if (wDataSize!=sizeof(DBR_GR_UserTransferDragon)) return false;

		//������
		DBR_GR_UserTransferDragon * pUserTransferScore=(DBR_GR_UserTransferDragon *)pData;
		dwUserID=pUserTransferScore->dwUserID;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTransferScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTransferScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@cbByNickName"),pUserTransferScore->cbByNickName);
		m_TreasureDBAide.AddParameter(TEXT("@lTransferDragon"),pUserTransferScore->lTransferDragon);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTransferScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@strNickName"),pUserTransferScore->szNickName);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTransferScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserTransferDragon"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		OnDragonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),0);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_UserTransferDragon"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnDragonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),0);

		return false;
	}

	return true;
}

//������
VOID CDataBaseEngineSink::OnDragonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, BYTE cbParam1)
{
	if (cbParam1 == 2)
	{
		DBO_GR_UserWinDragonPiece UserWinDragon = {0};

		if (dwErrorCode==DB_SUCCESS)
		{
			UserWinDragon.lWinDragonPiece=m_TreasureDBAide.GetValue_LONG(TEXT("lWinDragonPiece"));
			UserWinDragon.lWinScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("lWinScore"));
		}

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_WIN_DRAGON_PIECE,dwContextID,&UserWinDragon,sizeof(UserWinDragon));
	}
	else
	{
		DBO_GR_UserTransferDragon UserTransferDragon = {0};
		UserTransferDragon.lResultCode = dwErrorCode;
		lstrcpyn(UserTransferDragon.szDescribeString,pszErrorString,CountArray(UserTransferDragon.szDescribeString));

		if (dwErrorCode==DB_SUCCESS)
		{
			UserTransferDragon.lDragon=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Dragon"));
			UserTransferDragon.lVariationDragon=m_TreasureDBAide.GetValue_LONGLONG(TEXT("VariationDragon"));
			UserTransferDragon.lSourceScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("SourceScore"));
			UserTransferDragon.lVariationScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("VariationScore"));
		}

		//���ͽ��
		WORD wDataSize=CountStringBuffer(UserTransferDragon.szDescribeString);
		WORD wHeadSize=sizeof(UserTransferDragon)-sizeof(UserTransferDragon.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USER_TRANSER_DRAGON,dwContextID,&UserTransferDragon,wHeadSize+wDataSize);
	}
}

//ʹ������
bool CDataBaseEngineSink::OnRequestUserUseDragon(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GR_UserUseDragon));
		if (wDataSize!=sizeof(DBR_GR_UserUseDragon)) return false;

		//������
		DBR_GR_UserUseDragon * pUserTakeScore=(DBR_GR_UserUseDragon *)pData;
		dwUserID=pUserTakeScore->dwUserID;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTakeScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTakeScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lUseDragon"),pUserTakeScore->lUseDragon);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTakeScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserTakeScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserUseDragon"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnDragonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),1);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_UserUseDragon"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnDragonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),1);

		return false;
	}

	return true;
}

//Ӯ��������Ƭ
bool CDataBaseEngineSink::OnRequestUserWinDragonPiece(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//Ч�����
	if (wDataSize!=sizeof(DBR_GR_UserWinDragonPiece)) return false;

	//��������
	DBR_GR_UserWinDragonPiece * pUserSaveScore=(DBR_GR_UserWinDragonPiece *)pData;
	dwUserID=pUserSaveScore->dwUserID;

	//������
	try
	{
		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserSaveScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserSaveScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lWinDragonPiece"),pUserSaveScore->lWinDragonPiece);
		m_TreasureDBAide.AddParameter(TEXT("@lWinScore"),pUserSaveScore->lWinScore);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_TreasureDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_TreasureDBAide.AddParameter(TEXT("@strMachineID"),pUserSaveScore->szMachineID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserWinDragonPiece"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnDragonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),2);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_UserWinDragonPiece"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnDragonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),2);

		return false;
	}

	return true;
}

//Ӯ������
bool CDataBaseEngineSink::OnRequestUserWinPearl(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//Ч�����
	if (wDataSize!=sizeof(DBR_GR_WinPearl)) return false;

	//��������
	DBR_GR_WinPearl * pWinPearl=(DBR_GR_WinPearl *)pData;
	dwUserID=pWinPearl->dwUserID;

	//������
	try
	{
		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pWinPearl->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@nPearlType"),pWinPearl->nPearlType);
		m_TreasureDBAide.AddParameter(TEXT("@nPearlCount"),pWinPearl->nPearlCount);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceOption->wKindID);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UserWinPearl"),true);

		DBR_GO_WinPearl WinPearl1 = {0};

		if (lResultCode==DB_SUCCESS)
		{
			WinPearl1.nPearlType=m_TreasureDBAide.GetValue_LONG(TEXT("nPearlType"));
			WinPearl1.nPearlCount=m_TreasureDBAide.GetValue_LONGLONG(TEXT("nPearlCount"));
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_WIN_PEARL,dwContextID,&WinPearl1,sizeof(WinPearl1));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_UserWinPearl"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//���ܴ���
bool CDataBaseEngineSink::OnRequestQuerySkill(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	if (wDataSize!=sizeof(DBR_GR_QuerySkill)) return false;
	DBR_GR_QuerySkill * pQuerySkill=(DBR_GR_QuerySkill *)pData;

	dwUserID=pQuerySkill->dwUserID;

	try
	{
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pQuerySkill->dwUserID);

		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_SkillQuery"),true);

		DBO_GR_QuerySkill QuerySkill = {0};

		if (lResultCode==DB_SUCCESS)
		{
			QuerySkill.nCannonStatus=m_TreasureDBAide.GetValue_LONG(TEXT("nCannonStatus"));
			QuerySkill.nJiaSu=m_TreasureDBAide.GetValue_LONG(TEXT("nJiaSu"));
			QuerySkill.nSanShe=m_TreasureDBAide.GetValue_LONG(TEXT("nSanShe"));
			QuerySkill.nBaoJi=m_TreasureDBAide.GetValue_LONG(TEXT("nBaoJi"));

			QuerySkill.cbCount[0]=QuerySkill.cbCount[1]=QuerySkill.cbCount[2]=9;//1ԪǮ��9������

			for (int i=0; i<7; i++)
			{
				QuerySkill.cbPrice[i]=18;//������̨����18Ԫ
			}
		}

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_QUERY_SKILL,dwContextID,&QuerySkill,sizeof(QuerySkill));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CString str = TEXT("[GSP_GR_SkillQuery] ");
		str += pIException->GetExceptionDescribe();
		CTraceService::TraceString(str,TraceLevel_Exception);

		return false;
	}

	return true;
}

//����ʹ��
bool CDataBaseEngineSink::OnRequestUseSkill(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	if (wDataSize!=sizeof(DBR_GR_UseSkill)) return false;
	DBR_GR_UseSkill * pUseSkill=(DBR_GR_UseSkill *)pData;

	dwUserID=pUseSkill->dwUserID;

	try
	{
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUseSkill->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@cbSkillID"),pUseSkill->cbSkillID);

		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_SkillUse"),true);

		DBO_GR_UseSkill UseSkill = {0};

		if (lResultCode==DB_SUCCESS)
		{
			UseSkill.cbSkillID=m_TreasureDBAide.GetValue_BYTE(TEXT("cbSkillID"));
			UseSkill.nCount=m_TreasureDBAide.GetValue_LONG(TEXT("nCount"));
			printf("UseSkill.nCount is %d\n", UseSkill.nCount);
		}

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_USE_SKILL,dwContextID,&UseSkill,sizeof(UseSkill));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CString str = TEXT("[GSP_GR_SkillUse] ");
		str += pIException->GetExceptionDescribe();
		CTraceService::TraceString(str,TraceLevel_Exception);

		return false;
	}

	return true;
}

//ϵͳ���ͼ���
bool CDataBaseEngineSink::OnRequestSkillGive(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	//Ч�����
	if (wDataSize!=sizeof(DBR_GR_GiveSkill)) return false;

	//��������
	DBR_GR_GiveSkill * pGiveSkill=(DBR_GR_GiveSkill *)pData;
	dwUserID=pGiveSkill->dwUserID;

	//������
	try
	{
		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pGiveSkill->dwUserID);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_SkillGive"),true);

		DBO_GR_GiveSkill GiveSkill = {0};

		if (lResultCode==DB_SUCCESS)
		{
			GiveSkill.cbSkillID=m_TreasureDBAide.GetValue_BYTE(TEXT("cbSkillID"));
			GiveSkill.nCount=m_TreasureDBAide.GetValue_LONG(TEXT("nCount"));
			printf("UseSkill.nCount is %d\n", GiveSkill.nCount);
		}

		if (GiveSkill.cbSkillID != 255 && GiveSkill.nCount > 0)
		{
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_SKILL_GIVE,dwContextID,&GiveSkill,sizeof(GiveSkill));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_SkillGive"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//������������
bool CDataBaseEngineSink::OnRequestNewGuidGift(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	if (wDataSize!=sizeof(DBR_GR_NewGuidGift)) return false;
	DBR_GR_NewGuidGift * pNewGuidGift=(DBR_GR_NewGuidGift *)pData;

	dwUserID=pNewGuidGift->dwUserID;

	try
	{
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@nUserID"),pNewGuidGift->dwUserID);

		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_NewGuidGift"),true);

		DBO_GR_NewGuidGift NewGuidGift = {0};

		if (lResultCode==DB_SUCCESS)
		{
			NewGuidGift.bSuccess=true;
			NewGuidGift.nCannonStatus=m_TreasureDBAide.GetValue_LONG(TEXT("nCannonStatus"));
			NewGuidGift.nLotteryCount=m_TreasureDBAide.GetValue_LONG(TEXT("nLotteryCount"));
		}

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_NEW_GUID_GIFT,dwContextID,&NewGuidGift,sizeof(NewGuidGift));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CString str = TEXT("[GSP_GR_NewGuidGift] ");
		str += pIException->GetExceptionDescribe();
		CTraceService::TraceString(str,TraceLevel_Exception);

		return false;
	}

	return true;
}

//��־��¼С�����
bool CDataBaseEngineSink::OnRequestLogClownOperation(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	// test check the size of the tag
	stClownOperationInfo * pClownOperationInfo = (stClownOperationInfo *)pData;
	WORD wDataSizeTmp = pClownOperationInfo->dwOperationCount * sizeof(tagClownOperation) + sizeof(stClownOperationInfo);
	TCHAR	pszDescribe[MAX_PATH];
	_sntprintf(pszDescribe,CountArray(pszDescribe),TEXT("the datasizetemp = %ld, the wdatasize is %ld"),wDataSizeTmp, wDataSize);
	CTraceService::TraceString(pszDescribe,TraceLevel_Normal);

	dwUserID = pClownOperationInfo->dwUserID;

	//���ݰ���С�ж�
	if (wDataSize!=wDataSizeTmp) return true;

	// ��һ�ֵ�ִ�д���, ��һ���ں͵ڶ��ְ���ѭ������, ����ı�־��nRoundNum, һ������ѭ������һ��nRoundNum = 2, ��ʾ���ڵ�i + 1 ���ǵ�һ�ֵ�����
	WORD wRoundCntFirst = 0;
	for (int i = 0; i < pClownOperationInfo->dwOperationCount; i++)
	{
		wRoundCntFirst = i;
		if ((*(pClownOperationInfo->pClownOperation))[i].nRoundNum == 2)
		{
			break;
		}
	}

	//��������
	CString cstrSQL=TEXT("");
	for (unsigned int i = 0; i < pClownOperationInfo->dwOperationCount; i++)
	{
		// 0 ~ wRoundCntFirst - 1, ��ʾ��һ��, wRoundCntFirst ~ dwOperationCount - 1 
		// ��ʾ�ڶ���, ��Ϊ1, βΪ3, ��ʾÿһ�ֵĿ�ʼ�ֺͽ����� 
		WORD wRoundMark = 0;
		if (i == 0 || i == wRoundCntFirst)		
		{
			wRoundMark = 1;
		}
		else if (i == (wRoundCntFirst - 1) || i == (pClownOperationInfo->dwOperationCount - 1))
		{
			wRoundMark = 3;
		}
		else
		{
			wRoundMark = 2;
		}

		CString strSQLTmp;
		tagClownOperation stClownOpTmp = (*(pClownOperationInfo->pClownOperation))[i];
		strSQLTmp.Format(TEXT("exec LOG_GSP_GR_INSERT_CLOWNOPERATION %ld,%d,%d,%lld,%lld,\"%s\",%d,%d, %d ;"),
			pClownOperationInfo->dwUserID,
			stClownOpTmp.nRoundNum,
			stClownOpTmp.bIsWin,
			stClownOpTmp.lUserScore,
			stClownOpTmp.lCurrentChip,
			stClownOpTmp.szTime,
			stClownOpTmp.stGuesOperation.nStarCount,
			stClownOpTmp.stGuesOperation.nChipTimes,
			wRoundMark
			);

		if (cstrSQL.GetLength()>=1023 - 180)
		{
			try
			{
				TCHAR * szSQL = (LPTSTR)(LPCTSTR)cstrSQL;
				m_LogServerDBAide.ExecuteSentence(szSQL,true);
			}
			catch (IDataBaseException * pIException)
			{
				//������Ϣ
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
			m_LogServerDBAide.ExecuteSentence(szSQLExtra,true);
		}
	}
	catch  (IDataBaseException * pIException)
	{
		//������Ϣ
		TCHAR	szSQL1[1024];
		_tcsncpy(szSQL1,cstrSQL,CountArray(szSQL1));
		CTraceService::TraceString(szSQL1,TraceLevel_Exception);

		LPCTSTR pszDescribeDBOError2=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribeDBOError2,TraceLevel_Exception);
	}


	return true;
}

//��ȡ�û�����
bool CDataBaseEngineSink::OnRequestGetUserItemCount(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
printf("%s %d\n", __FUNCTION__, __LINE__);
	if (wDataSize!=sizeof(DBR_GetUserItemCount))
	{
		return false;
	}
	DBR_GetUserItemCount * pGetUserItemCount=(DBR_GetUserItemCount *)pData;

	dwUserID=pGetUserItemCount->dwUserID;

	try
	{
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pGetUserItemCount->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@nType"),pGetUserItemCount->nType);

		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_GetUserItemCount"),true);

		DBO_GetUserItemCount GetUserItemCount1 = {};

		if (lResultCode==DB_SUCCESS)
		{
			GetUserItemCount1.nType=m_TreasureDBAide.GetValue_BYTE(TEXT("nType"));
			GetUserItemCount1.lCount=m_TreasureDBAide.GetValue_LONG(TEXT("lCount"));
printf("%d %lld\n", GetUserItemCount1.nType, GetUserItemCount1.lCount);
		}

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GET_USER_ITEM_COUNT,dwContextID,&GetUserItemCount1,sizeof(GetUserItemCount1));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CString str = TEXT("[GSP_GR_GetUserItemCount] ");
		str += pIException->GetExceptionDescribe();
		CTraceService::TraceString(str,TraceLevel_Exception);

		return false;
	}

	return true;
}

//�����û�����
bool CDataBaseEngineSink::OnRequestSaveUserItemCount(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	if (wDataSize!=sizeof(DBR_SaveUserItemCount))
	{
		return false;
	}
	DBR_SaveUserItemCount * pSaveUserItemCount=(DBR_SaveUserItemCount *)pData;

	dwUserID=pSaveUserItemCount->dwUserID;

	try
	{
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pSaveUserItemCount->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@nType"),pSaveUserItemCount->nType);
		m_TreasureDBAide.AddParameter(TEXT("@lCount"),pSaveUserItemCount->lCount);

		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_SaveUserItemCount"),true);

		//DBO_SaveUserItemCount SaveUserItemCount1 = {};

		//if (lResultCode==DB_SUCCESS)
		//{
		//	SaveUserItemCount1.nType=m_TreasureDBAide.GetValue_BYTE(TEXT("nType"));
		//	SaveUserItemCount1.lCount=m_TreasureDBAide.GetValue_LONG(TEXT("lCount"));
		//}

		//m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_SAVE_USER_ITEM_COUNT,dwContextID,&SaveUserItemCount1,sizeof(SaveUserItemCount1));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CString str = TEXT("[GSP_GR_SkillUse] ");
		str += pIException->GetExceptionDescribe();
		CTraceService::TraceString(str,TraceLevel_Exception);

		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestUpdateTreasureMap(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	if (wDataSize!=sizeof(DBR_GR_Update_Treasuremap)) return false;
	DBR_GR_Update_Treasuremap * pRequestInfo=(DBR_GR_Update_Treasuremap *)pData;
	dwUserID = pRequestInfo->dwUserID;
	try
	{
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pRequestInfo->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@nCount"),pRequestInfo->nCount);
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UpdateTreasureMap"),true);

		DBO_GR_Update_Treasuremap Result = {0};

		if (lResultCode==DB_SUCCESS)
		{
			Result.nCount=m_TreasureDBAide.GetValue_LONG(TEXT("nTreasureCount"));
		}
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_UPDATE_TREASUREMAP,dwContextID,&Result,sizeof(Result));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CString str = TEXT("GSP_GR_UpdateTreasureMap");
		str += pIException->GetExceptionDescribe();
		CTraceService::TraceString(str,TraceLevel_Exception);

		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestKValueAndIntegration(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	if (wDataSize!=sizeof(DBR_GR_Update_KValueAndIntegration)) return false;
	DBR_GR_Update_KValueAndIntegration * pRequestInfo=(DBR_GR_Update_KValueAndIntegration *)pData;
	dwUserID = pRequestInfo->dwUserID;
	try
	{
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pRequestInfo->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@nKValue"),pRequestInfo->nKValue);
		m_TreasureDBAide.AddParameter(TEXT("@nIntegration"),pRequestInfo->nIntegration);
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UpdateKValueAndIntegration"),true);

		DBO_GR_Update_KValueAndIntegration Result = {0};

		Result.nKValue=m_TreasureDBAide.GetValue_LONG(TEXT("nKValueRe"));
		Result.nIntegration=m_TreasureDBAide.GetValue_LONG(TEXT("nIntegrationRe"));
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_UPDATE_KVALUEANDINTEGRATION,dwContextID,&Result,sizeof(Result));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CString str = TEXT("GSP_GR_UpdateKValueAndIntegration");
		str += pIException->GetExceptionDescribe();
		CTraceService::TraceString(str,TraceLevel_Exception);

		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestUserMaxKValue(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	if (wDataSize!=sizeof(DBR_GR_Update_MaxKValue)) return false;
	DBR_GR_Update_MaxKValue * pRequestInfo=(DBR_GR_Update_MaxKValue *)pData;
	dwUserID = pRequestInfo->dwUserID;
	try
	{
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pRequestInfo->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@nMaxKValue"),pRequestInfo->nCount);
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_UpdateMaxKValue"),true);

		DBO_GR_Update_MaxKValue Result = {0};

		Result.nCount=m_TreasureDBAide.GetValue_LONG(TEXT("nMaxKValue"));
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_UPDATE_MAXKVALUE,dwContextID,&Result,sizeof(Result));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CString str = TEXT("GSP_GR_UpdateMaxKValue");
		str += pIException->GetExceptionDescribe();
		CTraceService::TraceString(str,TraceLevel_Exception);

		return false;
	}

	return true;
}

//��ȡ����������Ϣ
bool CDataBaseEngineSink::OnRequestLoadMatchConfig(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	return true;
	//try
	//{
	//	WORD wPacketSize=0;
	//	BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];
	//	//�������
	//	m_GameDBAide.ResetParameter();
	//	//ִ�в�ѯ
	//	LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LoadMatchConfig"),true);
	//	DBO_Load_Match_Config_Item * pMatchConfigItem=NULL;
	//	//�������
	//	if(lReturnValue==0)
	//	{
	//		//��ȡ��Ϣ
	//		while(true)
	//		{
	//			if (m_GameDBModule->IsRecordsetEnd()==true) break;
	//			pMatchConfigItem=(DBO_Load_Match_Config_Item *)(cbBuffer+wPacketSize);
	//			//��ȡ����
	//			pMatchConfigItem->nStartTime=m_GameDBAide.GetValue_LONG(TEXT("StartTime"));
	//			pMatchConfigItem->nEndTime=m_GameDBAide.GetValue_LONG(TEXT("EndTime"));
	//			pMatchConfigItem->nMatchNum=m_GameDBAide.GetValue_LONG(TEXT("MachNum"));
	//			pMatchConfigItem->nApplyCost=m_GameDBAide.GetValue_LONG(TEXT("ApplyCost"));
	//			pMatchConfigItem->nMatchUserCount=m_GameDBAide.GetValue_LONG(TEXT("MachUserCount"));
	//			pMatchConfigItem->nGameTime=m_GameDBAide.GetValue_LONG(TEXT("GameTimes"));
	//			//����λ��
	//			wPacketSize+=sizeof(DBO_Load_Match_Config_Item);
	//			//�ƶ���¼
	//			m_GameDBModule->MoveToNext();
	//		}
	//		if (wPacketSize>0)m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_LOAD_MATCH_CONFIG,dwContextID,cbBuffer,wPacketSize);
	//	}

		//return true;
	//}
	//catch (IDataBaseException * pIException)
	//{
	//	//������Ϣ
	//	CTraceService::TraceString(TEXT("GSP_GR_LoadMatchConfig"),TraceLevel_Exception);
	//	CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

	//	return false;
	//}

	return true;
}

//��ȡ��������������Ϣ
bool CDataBaseEngineSink::OnRequestLoadMatchRewardConfig(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	try
	{
		WORD wPacketSize=0;
		BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];
		//�������
		m_GameDBAide.ResetParameter();
		//ִ�в�ѯ
		LONG lReturnValue=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_LoadMatchRewardConfig"),true);
		DBO_Load_Match_Reward_Config_Item * pMatchConfigItem=NULL;
		//�������
		if(lReturnValue==0)
		{
			//��ȡ��Ϣ
			while(true)
			{
				if (m_GameDBModule->IsRecordsetEnd()==true) break;
				pMatchConfigItem=(DBO_Load_Match_Reward_Config_Item *)(cbBuffer+wPacketSize);
				//��ȡ����
				pMatchConfigItem->nRankStart=m_GameDBAide.GetValue_LONG(TEXT("RankStart"));
				pMatchConfigItem->nRankEnd=m_GameDBAide.GetValue_LONG(TEXT("RankEnd"));
				pMatchConfigItem->nReward[0] = m_GameDBAide.GetValue_LONG(TEXT("Award0"));
				pMatchConfigItem->nReward[1] = m_GameDBAide.GetValue_LONG(TEXT("Award1"));
				pMatchConfigItem->nReward[2] = m_GameDBAide.GetValue_LONG(TEXT("Award2"));
				pMatchConfigItem->nReward[3] = m_GameDBAide.GetValue_LONG(TEXT("Award3"));
				pMatchConfigItem->nReward[4] = m_GameDBAide.GetValue_LONG(TEXT("Award4"));
				pMatchConfigItem->nReward[5] = m_GameDBAide.GetValue_LONG(TEXT("Award5"));
				pMatchConfigItem->nReward[6] = m_GameDBAide.GetValue_LONG(TEXT("Award6"));
				pMatchConfigItem->nReward[7] = m_GameDBAide.GetValue_LONG(TEXT("Award7"));
				pMatchConfigItem->nReward[8] = m_GameDBAide.GetValue_LONG(TEXT("Award8"));
				pMatchConfigItem->nReward[9] = m_GameDBAide.GetValue_LONG(TEXT("Award9"));
				pMatchConfigItem->nMachType = m_GameDBAide.GetValue_LONG(TEXT("MachType"));
				pMatchConfigItem->nShareType = m_GameDBAide.GetValue_LONG(TEXT("ShareType"));
				//����λ��
				wPacketSize+=sizeof(DBO_Load_Match_Reward_Config_Item);
				//�ƶ���¼
				m_GameDBModule->MoveToNext();
			}
			if (wPacketSize>0)m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_LOAD_MATCH_REWARD_CONFIG,dwContextID,cbBuffer,wPacketSize);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_LoadMatchRewardConfig"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestUserMatchSignUP(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	if (wDataSize!=sizeof(DBR_GR_Match_UserSignup)) return false;

	//��������
	DBO_GR_GameParameter GameParameter;
	ZeroMemory(&GameParameter,sizeof(GameParameter));

	//������Ϣ
	DBR_GR_Match_UserSignup * pMatchUserSignUp = (DBR_GR_Match_UserSignup *)pData;
	dwUserID = pMatchUserSignUp->dwUserID;
	try
	{

		//ִ�в�ѯ
		m_GameDBAide.ResetParameter();
		m_GameDBAide.AddParameter(TEXT("@wUserID"), pMatchUserSignUp->dwUserID);

		//ִ�в�ѯ
		LONG lResultCode=m_GameDBAide.ExecuteProcess(TEXT("GSP_GR_AddMatchSignUpInfo"),true);

		CDBVarValue DBVarValue;
		m_GameDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		OnMatchSignUpDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false, pMatchUserSignUp->dwUserID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_AddMatchSignUpInfo"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		OnMatchSignUpDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false, pMatchUserSignUp->dwUserID);
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestGetMatchPirze(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID)
{
	if (wDataSize!=sizeof(DBR_GR_Match_Get_Prize)) return false;

	//��������
	DBO_GR_Match_Get_Prize stMatchGetPrize;
	ZeroMemory(&stMatchGetPrize,sizeof(stMatchGetPrize));
	//������Ϣ
	DBR_GR_Match_Get_Prize * pGetMatchPrize = (DBR_GR_Match_Get_Prize *)pData;
	dwUserID = pGetMatchPrize->dwUserID;
	try
	{

		//ִ�в�ѯ
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@UserID"), pGetMatchPrize->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@MatchNum"), pGetMatchPrize->nMatchID);
		m_TreasureDBAide.AddParameter(TEXT("@MatchDay"), pGetMatchPrize->szMatchData);
		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_Get_Match_Prize"),true);
		printf("ִ���콱�洢����\n");
		DBO_GR_Match_Get_Prize pMatchGetPrize={0};
		//�������
		if(lResultCode==0)
		{
			//��ȡ����
			pMatchGetPrize.bPriseStatus = true;
		}
		else
		{
			pMatchGetPrize.bPriseStatus = false;
		}
		pMatchGetPrize.nPriseCount=m_TreasureDBAide.GetValue_LONG(TEXT("RewardGold"));
		pMatchGetPrize.nPriseType=m_TreasureDBAide.GetValue_LONG(TEXT("MatchType"));
		pMatchGetPrize.lluserScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Score"));
		pMatchGetPrize.nMatchId = m_TreasureDBAide.GetValue_LONGLONG(TEXT("MatchID"));
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_MATCH_GET_PRIZE,dwContextID,&pMatchGetPrize,sizeof(DBO_GR_Match_Get_Prize));
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GP_Get_Match_Prize"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
	}

	return true;
}

// �����û���½���
VOID CDataBaseEngineSink::OnMatchSignUpDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient, DWORD dwUserID,BYTE cbDeviceType,WORD wBehaviorFlags,WORD wPageTableCount)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//����ʧ�ܽ��
		DBO_GR_Match_UserSignUp MatchSignUpRes;
		ZeroMemory(&MatchSignUpRes,sizeof(DBO_GR_Match_UserSignUp));

		//��������
		MatchSignUpRes.dwUserID=dwUserID;
		MatchSignUpRes.bIsMatchSignUPSuccess = true;
		lstrcpyn(MatchSignUpRes.szDescribeString,pszErrorString,CountArray(MatchSignUpRes.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(MatchSignUpRes.szDescribeString);
		WORD wHeadSize=sizeof(MatchSignUpRes)-sizeof(MatchSignUpRes.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_MATCH_USERSIGNUP,dwContextID,&MatchSignUpRes,wHeadSize+wDataSize);
	}
	else
	{
		//����ʧ�ܽ��
		DBO_GR_Match_UserSignUp MatchSignUpRes;
		ZeroMemory(&MatchSignUpRes,sizeof(DBO_GR_Match_UserSignUp));

		//��������
		MatchSignUpRes.dwUserID=dwUserID;
		MatchSignUpRes.bIsMatchSignUPSuccess = false;
		lstrcpyn(MatchSignUpRes.szDescribeString,pszErrorString,CountArray(MatchSignUpRes.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(MatchSignUpRes.szDescribeString);
		WORD wHeadSize=sizeof(MatchSignUpRes)-sizeof(MatchSignUpRes.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_MATCH_USERSIGNUP,dwContextID,&MatchSignUpRes,wHeadSize+wDataSize);
	}

	return;
}
//////////////////////////////////////////////////////////////////////////////////
