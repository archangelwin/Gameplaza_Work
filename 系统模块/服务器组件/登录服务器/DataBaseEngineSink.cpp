#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"
//////////////////////////////////////////////////////////////////////////////////


CString GetFileDialogPath2();
int GloginProcCost=0;
//���캯��
CDataBaseEngineSink::CDataBaseEngineSink()
{
	//��Ա����
	m_uiRandCodeCnt=0;

	//�������
	m_pInitParameter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return;
}

//��������
CDataBaseEngineSink::~CDataBaseEngineSink()
{
}

//�ӿڲ�ѯ
VOID * CDataBaseEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngineSink,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)
{
	//��������
	if ((m_AccountsDBModule.GetInterface()==NULL)&&(m_AccountsDBModule.CreateInstance()==false))
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

	//��������
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
				"��½������","LogServerDBParameter");

			//��������
			m_LogServerDBModule->OpenConnection();
			m_LogServerDBAide.SetDataBase(m_LogServerDBModule.GetInterface());

			// ��ʾ������־�����������ͺ͵�ַ
			TCHAR pszDescribe[MAX_PATH]=TEXT("");
			_sntprintf(pszDescribe,CountArray(pszDescribe),TEXT("ServerType : %s Link success, IPAddress : %s, PORT : %ld"),szServerName, szLogServerIPAddr, wLogServerPort);
			CTraceService::TraceString(pszDescribe,TraceLevel_Normal);
		}
		catch (IDataBaseException * pIException)
		{
			//������Ϣ
			LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
			CTraceService::TraceString(TEXT("logserver error"),TraceLevel_Exception);
			CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
		}
	}

	try
	{
		//������Ϣ
		tagDataBaseParameter * pAccountsDBParameter=&m_pInitParameter->m_AccountsDBParameter;
		tagDataBaseParameter * pTreasureDBParameter=&m_pInitParameter->m_TreasureDBParameter;
		tagDataBaseParameter * pPlatformDBParameter=&m_pInitParameter->m_PlatformDBParameter;

		//��������
		m_AccountsDBModule->SetConnectionInfo(pAccountsDBParameter->szDataBaseAddr,pAccountsDBParameter->wDataBasePort,
			pAccountsDBParameter->szDataBaseName,pAccountsDBParameter->szDataBaseUser,pAccountsDBParameter->szDataBasePass,
			"��¼������","AccountsDBModule");
		m_TreasureDBModule->SetConnectionInfo(pTreasureDBParameter->szDataBaseAddr,pTreasureDBParameter->wDataBasePort,
			pTreasureDBParameter->szDataBaseName,pTreasureDBParameter->szDataBaseUser,pTreasureDBParameter->szDataBasePass,
			"��¼������","TreasureDBModule");
		m_PlatformDBModule->SetConnectionInfo(pPlatformDBParameter->szDataBaseAddr,pPlatformDBParameter->wDataBasePort,
			pPlatformDBParameter->szDataBaseName,pPlatformDBParameter->szDataBaseUser,pPlatformDBParameter->szDataBasePass,
			"��¼������","PlatformDBModule");

		//��������
		m_AccountsDBModule->OpenConnection();
		m_AccountsDBAide.SetDataBase(m_AccountsDBModule.GetInterface());

		//��������
		m_TreasureDBModule->OpenConnection();
		m_TreasureDBAide.SetDataBase(m_TreasureDBModule.GetInterface());

		//��������
		m_PlatformDBModule->OpenConnection();
		m_PlatformDBAide.SetDataBase(m_PlatformDBModule.GetInterface());

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		return false;
	}



	return true;
}

//ֹͣ�¼�
bool CDataBaseEngineSink::OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx)
{
	//���ö���
	m_AccountsDBAide.SetDataBase(NULL);
	m_TreasureDBAide.SetDataBase(NULL);
	m_PlatformDBAide.SetDataBase(NULL);

	//�ر�����
	if (m_AccountsDBModule.GetInterface()!=NULL)
	{
		m_AccountsDBModule->CloseConnection();
		m_AccountsDBModule.CloseInstance();
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

	//�������
	m_pInitParameter=NULL;
	m_pIDataBaseEngineEvent=NULL;

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
	switch (wRequestID)
	{
	case DBR_GP_LOGON_GAMEID:			//��ʶ��¼
		{
			return OnRequestLogonGameID(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOGON_ACCOUNTS:			//�ʺŵ�¼
		{
			return OnRequestLogonAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_GP_RECORD_LOGON:
		{
			return OnRequestRecordLogon(dwContextID,pData,wDataSize);
		}
	case DBR_GP_REGISTER_ACCOUNTS:		//ע���ʺ�
		{
			return OnRequestRegisterAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GUEST_REGISTER:			//�ο�ע��
		{
			return OnRequestGuestRegister(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_MACHINE:			//�޸Ļ���
		{
			return OnRequestModifyMachine(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_LOGON_PASS:		//�޸�����
		{
			return OnRequestModifyLogonPass(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_INSURE_PASS:		//�޸�����
		{
			return OnRequestModifyInsurePass(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_UNDER_WRITE:		//�޸�ǩ��
		{
			return OnRequestModifyUnderWrite(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_SYSTEM_FACE:		//�޸�ͷ��
		{
			return OnRequestModifySystemFace(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_CUSTOM_FACE:		//�޸�ͷ��
		{
			return OnRequestModifyCustomFace(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_INDIVIDUAL:		//�޸�����
		{
			return OnRequestModifyIndividual(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_SAVE_SCORE:		//������Ϸ��
		{
			return OnRequestUserSaveScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_TAKE_SCORE:		//��ȡ��Ϸ��
		{
			return OnRequestUserTakeScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_TRANSFER_SCORE:	//ת����Ϸ��
		{
			return OnRequestUserTransferScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_INDIVIDUAL:		//��ѯ����
		{
			return OnRequestQueryIndividual(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_INSURE_INFO:		//��ѯ����
		{
			return OnRequestQueryInsureInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_CHECK_PASSWORD:
		{
			return OnRequestUserCheckPassword(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_USER_INFO:		//��ѯ�û�
		{
			return OnRequestQueryTransferUserInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_GAME_LIST:			//�����б�
		{
			return OnRequestLoadGameList(dwContextID,pData,wDataSize);
		}
	case DBR_GP_ONLINE_COUNT_INFO:		//������Ϣ
		{
			return OnRequestOnLineCountInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_SIGIN_INFO: //ǩ����ѯ 
		{
			return OnRequestQuerySignInInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_SIGIN: //ǩ������
		{
			return OnRequestUserSignIn(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_LOTTERY_INFO:
		{
			return OnRequestQueryLotteryInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOTTERY_REQ:
		{
			return OnRequestUserLtry(dwContextID,pData,wDataSize);
		}
	case DBR_MB_LOGON_GAMEID:			//��ʶ��¼
		{
			return OnMobileLogonGameID(dwContextID,pData,wDataSize);
		}
	case DBR_MB_LOGON_ACCOUNTS:			//�ʺŵ�¼
		{
			return OnMobileLogonAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_MB_REGISTER_ACCOUNTS:		//ע���ʺ�
		{
			return OnMobileRegisterAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_MAIL_ID_LIST:
		{
			return OnRequestQueryMailIDList(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_MAIL_INFO:
		{
			return OnRequestQueryMailInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_MAIL_CONTENT:
		{
			return OnRequestQueryMailContent(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_MAIL_ITEM:
		{
			return OnRequestQueryMailItem(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CHARGE_VALUE:
		{
			return OnRequestChargeValue(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CHARGE_CANCEL:
		{
			return OnRequestChargeCancel(dwContextID,pData,wDataSize);
		}
	case DBR_GP_WEB_CHARGE_VALUE:
		{
			return OnRequestWebChargeValue(dwContextID,pData,wDataSize);
		}
	case DBR_GP_VIP_INFO:
		{
			return OnRequestVIPInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_VIP_EVERYDAY_GIFT:
		{
			return OnRequestVIPEverydayGift(dwContextID,pData,wDataSize);
		}
	case DBR_GP_VIP_CHARGE_BACK:
		{
			return OnRequestVIPChargeBack(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CURRENT_SCORE:
		{
			return OnRequestCurrentScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOGON_GIFT:
		{
			return OnRequestLogonGift(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CHARGE1ST_REQUEST:
		{
			return OnRequestCharge1stRequest(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CHARGE1ST_GET:
		{
			return OnRequestCharge1stGet(dwContextID,pData,wDataSize);
		}
	case DBR_GP_WEALTH_BAG_ADD_SCORE:
		{
			return OnRequestWealthBagAddScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_TRANSFER_DRAGON:
		{
			return OnRequestUserTransferDragon(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_USE_DRAGON:
		{
			return OnRequestUserUseDragon(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_ITEM_COUNT:
		{
			return OnRequestQueryItemCount(dwContextID,pData,wDataSize);
		}
	case DBR_GP_TRANSFER_ITEM:
		{
			return OnRequestTransferItem(dwContextID,pData,wDataSize);
		}
	case DBR_GP_COMBINE_DRAGON:
		{
			return OnRequestCombineItem(dwContextID,pData,wDataSize);
		}
	case DBR_GP_USE_DRAGON:
		{
			return OnRequestUseDragon(dwContextID,pData,wDataSize);
		}
	case DBR_GP_EQUIPMENT_DRAGON:
		{
			return OnRequestEquipmentDragon(dwContextID,pData,wDataSize);
		}
	case DBR_GP_EXCHANGE_LOTTERY:
		{
			return OnRequestExchangeLottery(dwContextID,pData,wDataSize);
		}
	case DBR_GP_QUERY_SKILL:
		{
			return OnRequestQuerySkill(dwContextID,pData,wDataSize);
		}
	case DBR_GP_VERIFY_MBNUM:
		{
			return OnRequestMBVericationCode(dwContextID, pData, wDataSize); //�����Ƿ�󶨹����ֻ����� 
		}
	case DBR_GP_MB_REGITER:
		{
			return OnRequestMBRegister(dwContextID, pData, wDataSize); //��֤�ֻ������Ƿ���� 
		}
	//case DBR_GP_MBNUMREGISTER_ACCOUNTS:
	//	{
	//		return OnRequestMBNumRegisterAccounts(dwContextID, pData, wDataSize); 
	//	}
	case DBR_GP_QUERY_NEW_GUID:
		{
			return OnRequestQueryNewGuid(dwContextID,pData,wDataSize);
		}
	case DBR_GP_SUPER_GOLD:
		{
			return OnRequestQuerySuperGold(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MB_BIND_NUM:
		{
			return OnRequestMBBind(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MB_CHECK_NUM_EXIT:
		{
			return OnRequestMBBindNumExit(dwContextID,pData,wDataSize);
		}
	case DBR_GP_BROADCAST_SPEED:
		{
			return OnRequestBroadCastSpeed(dwContextID,pData,wDataSize);
		}
	case DBR_GP_TYPE_ITEM_COUNT:
		{
			return OnRequestTypeItemCount(dwContextID,pData,wDataSize);
		}
	case DBR_GP_COMPRANK_INFO:		//������Ϣ
		{
			return OnRequestCompRankInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_BROADLABA:		//���ȹ㲥
		{
			return OnRequestBroadCastLaBa(dwContextID,pData,wDataSize);
		}
	case DBR_GP_CHANGE_PHONE_FARE:
		{
			return OnRequestChangePhoneFare(dwContextID,pData,wDataSize);
		}
	case DBR_GP_SUPERRANK_INFO:		//������������
		{
			return OnRequestSuperRankInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GETBINDREWARDNEWGUID:
		{
			return OnRequestBindRewardNewGuid(dwContextID,pData,wDataSize);
		}
	case DBR_GP_SETSUPERNEWGUID:
		{
			return OnRequestSetSuperNewGuid(dwContextID,pData,wDataSize);
		}
	case DBR_GP_GET_NEWMAIL_REMIND:
		{
			return OnRequestGetNewMailRemind(dwContextID,pData,wDataSize);
	case DBR_GP_QUERY_FREE_LABA_COUNT:
		{
			return OnRequestQueryFreeLabaCount(dwContextID,pData,wDataSize);
		}
	case DBR_GR_MATCH_GET_PRIZE:
		{
			return OnRequestGetMatchPirze(dwContextID,pData,wDataSize);
		}
	}
	}

	return false;
}

//I D ��¼
bool CDataBaseEngineSink::OnRequestLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_LogonGameID));
		if (wDataSize!=sizeof(DBR_GP_LogonGameID)) return false;

		//ִ�в�ѯ
		DBR_GP_LogonGameID * pLogonGameID=(DBR_GP_LogonGameID *)pData;

		//��������
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonGameID->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonGameID->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),pLogonGameID->dwGameID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonGameID->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pLogonGameID->szPassPortID);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonGameID->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonGameID->cbNeeValidateMBCard);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyGameID"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GP_EfficacyGameID"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false);

		return false;
	}

	return true;
}

//�ʺŵ�¼
bool CDataBaseEngineSink::OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_LogonAccounts));
	if (wDataSize!=sizeof(DBR_GP_LogonAccounts)) return false;

	//������
	DBR_GP_LogonAccounts * pLogonAccounts=(DBR_GP_LogonAccounts *)pData;

	//ִ���ж�
	tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
	if (pBindParameter->dwSocketID!=dwContextID) return true;

	//ת����ַ
	TCHAR szClientAddr[16]=TEXT("");
	BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
	_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);
	//�ж����ֻ��Ż����˺ŵ�¼
	bool bIsAllNumber = true;
	int nSize = CountStringBuffer(pLogonAccounts->szAccounts);
	for (int i = 0;i < nSize-1; ++i)
	{
		if (pLogonAccounts->szAccounts[i]<'0'||pLogonAccounts->szAccounts[i]>'9')
		{
			bIsAllNumber = false;
			break;
		}
	}
	LONG lResultCode=0;
	try
	{
		if (bIsAllNumber)
		{
			//�������
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@strRegisterMobile"),pLogonAccounts->szAccounts);
			m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
			m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pLogonAccounts->szPassPortID);
			m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
			m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
			m_AccountsDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonAccounts->cbNeeValidateMBCard);

			//�������
			TCHAR szDescribeString[128]=TEXT("");
			m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

			//ִ�в�ѯ
			lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyAccountsMobile"),true);
		} 
		else
		{
			//�������
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
			m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
			m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pLogonAccounts->szPassPortID);
			m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
			m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
			m_AccountsDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonAccounts->cbNeeValidateMBCard);

			//�������
			TCHAR szDescribeString[128]=TEXT("");
			m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

			//ִ�в�ѯ
			lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyAccounts"),true);
		}


		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false,true);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GP_EfficacyAccounts"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false,true);

		return false;
	}

	// ��־���ݿ�
	if (lResultCode == DB_SUCCESS)
	{
		try
		{
			if (bIsAllNumber)
			{
				//�������
				m_LogServerDBAide.ResetParameter();
				m_LogServerDBAide.AddParameter(TEXT("@strRegisterMobile"),pLogonAccounts->szAccounts);
				m_LogServerDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
				m_LogServerDBAide.AddParameter(TEXT("@strPassPortID"),pLogonAccounts->szPassPortID);
				m_LogServerDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
				m_LogServerDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
				m_LogServerDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonAccounts->cbNeeValidateMBCard);

				//�������
				TCHAR szDescribeString[128]=TEXT("");
				m_LogServerDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

				//ִ�в�ѯ
				lResultCode=m_LogServerDBAide.ExecuteProcess(TEXT("Log_GSP_GP_EfficacyAccountsMobile"),true);
			} 
			else
			{
				//�������
				m_LogServerDBAide.ResetParameter();
				m_LogServerDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
				m_LogServerDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
				m_LogServerDBAide.AddParameter(TEXT("@strPassPortID"),pLogonAccounts->szPassPortID);
				m_LogServerDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
				m_LogServerDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
				m_LogServerDBAide.AddParameter(TEXT("@nNeeValidateMBCard"),pLogonAccounts->cbNeeValidateMBCard);

				//�������
				TCHAR szDescribeString[128]=TEXT("");
				m_LogServerDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

				//ִ�в�ѯ
				lResultCode=m_LogServerDBAide.ExecuteProcess(TEXT("Log_GSP_GP_EfficacyAccounts"),true);
			}

		}
		catch (IDataBaseException * pIException)
		{
			//������Ϣ
			CTraceService::TraceString(TEXT("Log_GSP_GP_EfficacyAccounts"),TraceLevel_Exception);
			CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		}
	}

	return true;
}

//��¼��¼
bool CDataBaseEngineSink::OnRequestRecordLogon(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_RecordLogon));
		if (wDataSize!=sizeof(DBR_GP_RecordLogon)) return false;

		//������
		DBR_GP_RecordLogon * pRecordLogon=(DBR_GP_RecordLogon *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@Accounts"),pRecordLogon->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@Password"),pRecordLogon->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@DeviceType"),pRecordLogon->cbDeviceType);
		m_AccountsDBAide.AddParameter(TEXT("@Channel"),pRecordLogon->cbChannel);
		m_AccountsDBAide.AddParameter(TEXT("@MachineID"),pRecordLogon->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@LAN"),pRecordLogon->szLAN);
		m_AccountsDBAide.AddParameter(TEXT("@WAN"),pRecordLogon->szWAN);
		m_AccountsDBAide.AddParameter(TEXT("@ClientVersion"),pRecordLogon->szClientVersion);
		m_AccountsDBAide.AddParameter(TEXT("@Node"),pRecordLogon->szComputerName);
		m_AccountsDBAide.AddParameter(TEXT("@UserData1"),pRecordLogon->szUserData1);
		m_AccountsDBAide.AddParameter(TEXT("@UserData2"),pRecordLogon->szUserData2);
		m_AccountsDBAide.AddParameter(TEXT("@UserData3"),pRecordLogon->szUserData3);
		m_AccountsDBAide.AddParameter(TEXT("@UserData4"),pRecordLogon->szUserData4);
		m_AccountsDBAide.AddParameter(TEXT("@UserData5"),pRecordLogon->szUserData5);
		m_AccountsDBAide.AddParameter(TEXT("@UserData6"),pRecordLogon->szUserData6);
		m_AccountsDBAide.AddParameter(TEXT("@UserData7"),pRecordLogon->szUserData7);
		m_AccountsDBAide.AddParameter(TEXT("@UserData8"),pRecordLogon->szUserData8);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_RecordLogon"),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GP_RecordLogon"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

//ע�ᴦ��
bool CDataBaseEngineSink::OnRequestRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{ 
	LONG lResultCode;
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_RegisterAccounts));
		if (wDataSize!=sizeof(DBR_GP_RegisterAccounts)) return false;

		//������
		DBR_GP_RegisterAccounts * pRegisterAccounts=(DBR_GP_RegisterAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),pRegisterAccounts->szSpreader);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@strInsurePass"),pRegisterAccounts->szInsurePass);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pRegisterAccounts->szPassPortID);
		m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pRegisterAccounts->szCompellation);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pRegisterAccounts->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@bIsGuestReg"),0);
		m_AccountsDBAide.AddParameter(TEXT("@AgentID"),pRegisterAccounts->dwAgentID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_RegisterAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GP_RegisterAccounts"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false);

		return false;
	}
	
	// ��־������
	if (lResultCode == DB_SUCCESS)
	{
		DWORD dwGameID = m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
		DWORD dwUserID = m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
		try
		{
			//������
			DBR_GP_RegisterAccounts * pRegisterAccounts=(DBR_GP_RegisterAccounts *)pData;

			//ת����ַ
			TCHAR szClientAddr[16]=TEXT("");
			BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
			_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

			//�������
			m_LogServerDBAide.ResetParameter();
			m_LogServerDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
			m_LogServerDBAide.AddParameter(TEXT("@dwGameID"),dwGameID);
			m_LogServerDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
			m_LogServerDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
			m_LogServerDBAide.AddParameter(TEXT("@strSpreader"),pRegisterAccounts->szSpreader);
			m_LogServerDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
			m_LogServerDBAide.AddParameter(TEXT("@strInsurePass"),pRegisterAccounts->szInsurePass);
			m_LogServerDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
			m_LogServerDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
			m_LogServerDBAide.AddParameter(TEXT("@strPassPortID"),pRegisterAccounts->szPassPortID);
			m_LogServerDBAide.AddParameter(TEXT("@strCompellation"),pRegisterAccounts->szCompellation);
			m_LogServerDBAide.AddParameter(TEXT("@strMobilePhone"),pRegisterAccounts->szMobilePhone);
			m_LogServerDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
			m_LogServerDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);
			m_LogServerDBAide.AddParameter(TEXT("@bIsGuestReg"),0);
			m_LogServerDBAide.AddParameter(TEXT("@AgentID"),pRegisterAccounts->dwAgentID);

			//�������
			TCHAR szDescribeString[128]=TEXT("");
			m_LogServerDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

			//ִ�в�ѯ
			LONG lResultCode=m_LogServerDBAide.ExecuteProcess(TEXT("Log_GSP_GP_RegisterAccounts"),true);
		}
		catch (IDataBaseException * pIException)
		{
			//������Ϣ
			CTraceService::TraceString(TEXT("Log_GSP_GP_RegisterAccounts"),TraceLevel_Exception);
			CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		}
	}

	return true;
}

//I D ��¼
bool CDataBaseEngineSink::OnMobileLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_MB_LogonGameID));
		if (wDataSize!=sizeof(DBR_MB_LogonGameID)) return false;

		//ִ�в�ѯ
		DBR_MB_LogonGameID * pLogonGameID=(DBR_MB_LogonGameID *)pData;

		//��������
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonGameID->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonGameID->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),pLogonGameID->dwGameID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonGameID->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonGameID->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonGameID->szMobilePhone);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyGameID"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_MB_EfficacyGameID"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),true);

		return false;
	}

	return true;
}

//�ʺŵ�¼
bool CDataBaseEngineSink::OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_MB_LogonAccounts));
		if (wDataSize!=sizeof(DBR_MB_LogonAccounts)) return false;

		//������
		DBR_MB_LogonAccounts * pLogonAccounts=(DBR_MB_LogonAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pLogonAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pLogonAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pLogonAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pLogonAccounts->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pLogonAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pLogonAccounts->szMobilePhone);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_EfficacyAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_MB_EfficacyAccounts"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),true);

		return false;
	}

	return true;
}

//�����ο���Ϣ
LONG CDataBaseEngineSink::InsGuestRegInfo(DBR_GP_RegisterAccounts* pGuestRegInfo, CDBVarValue * pDBVarValue)
{
	// �洢���̽��
	LONG lResultCode;

	try
	{
		//ת����ַ
		TCHAR szEncryCode[LEN_MD5];
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pGuestRegInfo->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);
		CWHEncrypt::MD5Encrypt(pGuestRegInfo->szGuestPass,szEncryCode);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pGuestRegInfo->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pGuestRegInfo->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),pGuestRegInfo->szSpreader);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),szEncryCode);
		m_AccountsDBAide.AddParameter(TEXT("@strInsurePass"),szEncryCode);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pGuestRegInfo->wFaceID);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pGuestRegInfo->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pGuestRegInfo->szPassPortID);
		m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pGuestRegInfo->szCompellation);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pGuestRegInfo->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pGuestRegInfo->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@bIsGuestReg"),1);
		m_AccountsDBAide.AddParameter(TEXT("@AgentID"),0);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_RegisterAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		memcpy(pDBVarValue, &DBVarValue, sizeof(CDBVarValue));

		_bstr_t bsErrString = DBVarValue.bstrVal;
		LPCTSTR lpErrString = bsErrString;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("InsGuestRegInfo"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return 0;
	}

	// ��־������
	if (lResultCode == DB_GUEST_REG_SUCCESS)
	{
		try
		{
			DWORD dwGameID = m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			DWORD dwUserID = m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));

			//ת����ַ
			TCHAR szEncryCode[LEN_MD5];
			TCHAR szClientAddr[16]=TEXT("");
			BYTE * pClientAddr=(BYTE *)&pGuestRegInfo->dwClientAddr;
			_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);
			CWHEncrypt::MD5Encrypt(pGuestRegInfo->szGuestPass,szEncryCode);

			//�������
			m_LogServerDBAide.ResetParameter();
			m_LogServerDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
			m_LogServerDBAide.AddParameter(TEXT("@dwGameID"),dwGameID);
			m_LogServerDBAide.AddParameter(TEXT("@strAccounts"),pGuestRegInfo->szAccounts);
			m_LogServerDBAide.AddParameter(TEXT("@strNickName"),pGuestRegInfo->szNickName);
			m_LogServerDBAide.AddParameter(TEXT("@strSpreader"),pGuestRegInfo->szSpreader);
			m_LogServerDBAide.AddParameter(TEXT("@strLogonPass"),szEncryCode);
			m_LogServerDBAide.AddParameter(TEXT("@strInsurePass"),szEncryCode);
			m_LogServerDBAide.AddParameter(TEXT("@wFaceID"),pGuestRegInfo->wFaceID);
			m_LogServerDBAide.AddParameter(TEXT("@cbGender"),pGuestRegInfo->cbGender);
			m_LogServerDBAide.AddParameter(TEXT("@strPassPortID"),pGuestRegInfo->szPassPortID);
			m_LogServerDBAide.AddParameter(TEXT("@strCompellation"),pGuestRegInfo->szCompellation);
			m_LogServerDBAide.AddParameter(TEXT("@strMobilePhone"),pGuestRegInfo->szMobilePhone);
			m_LogServerDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
			m_LogServerDBAide.AddParameter(TEXT("@strMachineID"),pGuestRegInfo->szMachineID);
			m_LogServerDBAide.AddParameter(TEXT("@bIsGuestReg"),1);
			m_LogServerDBAide.AddParameter(TEXT("@AgentID"),0);

			//�������
			TCHAR szDescribeString[128]=TEXT("");
			m_LogServerDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

			//ִ�в�ѯ
			m_LogServerDBAide.ExecuteProcess(TEXT("Log_GSP_GP_RegisterAccounts"),true);
		}
		catch (IDataBaseException * pIException)
		{
			//������Ϣ
			CTraceService::TraceString(TEXT("InsGuestRegInfo:Log_GSP_GP_RegisterAccounts"),TraceLevel_Exception);
			CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		}
	}

	return lResultCode;
}

//��ȡ�ο���Ϣ
LONG CDataBaseEngineSink::GetGuestRegInfo(DBR_GP_RegisterAccounts* pGuestRegInfo, CDBVarValue * pDBVarValue)
{
	try
	{
		//ת����ַ
		LONG lResultCode;
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pGuestRegInfo->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pGuestRegInfo->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pGuestRegInfo->szPassPortID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GuestRegister"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"), DBVarValue);
		memcpy(pDBVarValue, &DBVarValue, sizeof(CDBVarValue));

		_bstr_t bsErrString = DBVarValue.bstrVal;
		LPCTSTR lpErrString = bsErrString;

		return lResultCode;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GetGuestRegInfo"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return 0;
	}
}

//��������˺�
VOID CDataBaseEngineSink::GenRandAccount(TCHAR szRandAccount[])
{
	TCHAR szCharBase[100];
	TCHAR szRandString[10];
	INT iCharNum=0;

	//�������
	m_uiRandCodeCnt++;
	if (m_uiRandCodeCnt>=1000)
	{
		m_uiRandCodeCnt=0;
	}

	lstrcpyn(szCharBase,TEXT("abcdefghijklmnopqrstuvwxyz0123456789"),CountArray(szCharBase));

	for(;iCharNum<9;iCharNum++)
	{
		szRandString[iCharNum]=szCharBase[rand()%36];
	}
	szRandString[iCharNum]='\0';

	_sntprintf(szRandAccount,LEN_ACCOUNTS,TEXT("TY_%s%03u"),szRandString,m_uiRandCodeCnt);

	return;
}

//��������ַ���
VOID CDataBaseEngineSink::GenRandString(TCHAR szRandStr[], UINT uLength)
{
	if (uLength >= 100)
	{
		return;
	}

	TCHAR szCharBase[100];
	TCHAR szRandString[100];
	INT iCharNum=0;

	lstrcpyn(szCharBase,TEXT("abcdefghijklmnopqrstuvwxyz0123456789"),CountArray(szCharBase));

	for(;iCharNum<uLength;iCharNum++)
	{
		szRandString[iCharNum]=szCharBase[rand()%36];
	}
	szRandString[iCharNum]='\0';

	lstrcpyn(szRandStr,szRandString,CountArray(szRandString));

	return;
}

//�ο͵�¼
bool CDataBaseEngineSink::OnRequestGuestRegister( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_RegisterAccounts));
		if (wDataSize!=sizeof(DBR_GP_RegisterAccounts)) return false;

		//������
		DBR_GP_RegisterAccounts * pGuestRegInfo=(DBR_GP_RegisterAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pGuestRegInfo->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//�������
		CDBVarValue DBVarValue;
		LONG lResultCode=DB_ERROR;
		lResultCode=GetGuestRegInfo(pGuestRegInfo, &DBVarValue);

		if (lResultCode == DB_NO_AVAILABLE_ACCOUNT)
		{
			do
			{
				GenRandAccount(pGuestRegInfo->szAccounts);
				lstrcpyn(pGuestRegInfo->szNickName,pGuestRegInfo->szAccounts,CountArray(pGuestRegInfo->szAccounts));
				GenRandString(pGuestRegInfo->szGuestPass, 6);
				lResultCode=InsGuestRegInfo(pGuestRegInfo, &DBVarValue);

			}	while(lResultCode==DB_ACCINFO_ALREADYEXIST);
		}

		if (lResultCode == DB_GUEST_REG_SUCCESS)
		{
			OnLogonDisposeResult(dwContextID,lResultCode,pGuestRegInfo->szGuestPass,false);
		}
		else
		{
			OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("OnRequestGuestRegister"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false);

		return false;
	}

	return true;
}

//�ʺ�ע��
bool CDataBaseEngineSink::OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_MB_RegisterAccounts));
		if (wDataSize!=sizeof(DBR_MB_RegisterAccounts)) return false;

		//������
		DBR_MB_RegisterAccounts * pRegisterAccounts=(DBR_MB_RegisterAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@strInsurePass"),pRegisterAccounts->szInsurePass);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pRegisterAccounts->szMobilePhone);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_MB_RegisterAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnLogonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_MB_RegisterAccounts"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnLogonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),true);

		return false;
	}

	return true;
}


//�޸Ļ���
bool CDataBaseEngineSink::OnRequestModifyMachine(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyMachine));
		if (wDataSize!=sizeof(DBR_GP_ModifyMachine)) return false;

		//������
		DBR_GP_ModifyMachine * pModifyMachine=(DBR_GP_ModifyMachine *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyMachine->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyMachine->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyMachine->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifyMachine->szMachineID);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�󶨲���
		if (pModifyMachine->cbBind==TRUE)
		{
			m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_MoorMachine"),false);
		}
		else
		{
			m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UnMoorMachine"),false);
		}

		//�������
		if (m_AccountsDBAide.GetReturnValue()==DB_SUCCESS)
		{
			//��������
			DBO_GP_OperateSuccess OperateSuccess;
			ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

			//��ȡ��Ϣ
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OperateSuccess.lResultCode=m_AccountsDBModule->GetReturnValue();
			lstrcpyn(OperateSuccess.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateSuccess.szDescribeString));

			//���ͽ��
			WORD wStringSize=CountStringBuffer(OperateSuccess.szDescribeString);
			WORD wSendSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString)+wStringSize;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_SUCCESS,dwContextID,&OperateSuccess,wSendSize);
		}
		else
		{
			//��������
			DBO_GP_OperateFailure OperateFailure;
			ZeroMemory(&OperateFailure,sizeof(OperateFailure));

			//��ȡ��Ϣ
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OperateFailure.lResultCode=m_AccountsDBModule->GetReturnValue();
			lstrcpyn(OperateFailure.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(OperateFailure.szDescribeString));

			//���ͽ��
			WORD wStringSize=CountStringBuffer(OperateFailure.szDescribeString);
			WORD wSendSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString)+wStringSize;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wSendSize);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("OnRequestModifyMachine"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�ͷ��
bool CDataBaseEngineSink::OnRequestModifySystemFace(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifySystemFace));
		if (wDataSize!=sizeof(DBR_GP_ModifySystemFace)) return false;

		//������
		DBR_GP_ModifySystemFace * pModifySystemFace=(DBR_GP_ModifySystemFace *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifySystemFace->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifySystemFace->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifySystemFace->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pModifySystemFace->wFaceID);

		//������Ϣ
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifySystemFace->szMachineID);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�������
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_SystemFaceInsert"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserFaceInfo UserFaceInfo;
			ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));

			//��ȡ��Ϣ
			UserFaceInfo.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_FACE_INFO,dwContextID,&UserFaceInfo,sizeof(UserFaceInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GP_SystemFaceInsert"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�ͷ��
bool CDataBaseEngineSink::OnRequestModifyCustomFace(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyCustomFace));
		if (wDataSize!=sizeof(DBR_GP_ModifyCustomFace)) return false;

		//������
		DBR_GP_ModifyCustomFace * pModifyCustomFace=(DBR_GP_ModifyCustomFace *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyCustomFace->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//��������
		SAFEARRAYBOUND SafeArrayBound;
		ZeroMemory(&SafeArrayBound,sizeof(SafeArrayBound));

		//���ñ���
		SafeArrayBound.lLbound=0L;
		SafeArrayBound.cElements=sizeof(pModifyCustomFace->dwCustomFace);

		//��������
		BYTE * pcbCustomFace=(BYTE *)pModifyCustomFace->dwCustomFace;
		SAFEARRAY * pSafeArray=SafeArrayCreate(VT_UI1,1,&SafeArrayBound);

		//��������
		for (LONG nIndex=0;nIndex<sizeof(pModifyCustomFace->dwCustomFace);nIndex++)
		{
			SafeArrayPutElement(pSafeArray,&nIndex,pcbCustomFace+nIndex);
		}

		//���ñ���
		VARIANT VarChunk;
		VariantClear(&VarChunk);
		VarChunk.parray=pSafeArray;
		VarChunk.vt=VT_ARRAY|VT_UI1;

		//��ȡ����
		ASSERT(m_AccountsDBModule.GetInterface()!=NULL);
		IDataBase * pIDataBase=m_AccountsDBModule.GetInterface();

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyCustomFace->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyCustomFace->szPassword);

		//ͷ����Ϣ
		pIDataBase->AddParameter(TEXT("@dwCustomFace"),adLongVarBinary,adParamInput,sizeof(pModifyCustomFace->dwCustomFace),CDBVarValue(VarChunk));

		//������Ϣ
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pModifyCustomFace->szMachineID);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�ͷ���Դ
		SafeArrayDestroy(pSafeArray);

		//�������
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_CustomFaceInsert"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserFaceInfo UserFaceInfo;
			ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));

			//��ȡ��Ϣ
			UserFaceInfo.wFaceID=INVALID_WORD;
			UserFaceInfo.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_FACE_INFO,dwContextID,&UserFaceInfo,sizeof(UserFaceInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//�������
			OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GP_CustomFaceInsert"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�����
bool CDataBaseEngineSink::OnRequestModifyLogonPass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyLogonPass));
		if (wDataSize!=sizeof(DBR_GP_ModifyLogonPass)) return false;

		//������
		DBR_GP_ModifyLogonPass * pModifyLogonPass=(DBR_GP_ModifyLogonPass *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyLogonPass->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyLogonPass->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyLogonPass->szScrPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strDesPassword"),pModifyLogonPass->szDesPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyLogonPassword"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GP_ModifyLogonPassword"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�����
bool CDataBaseEngineSink::OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyInsurePass));
		if (wDataSize!=sizeof(DBR_GP_ModifyInsurePass)) return false;

		//������
		DBR_GP_ModifyInsurePass * pModifyInsurePass=(DBR_GP_ModifyInsurePass *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyInsurePass->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyInsurePass->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strScrPassword"),pModifyInsurePass->szScrPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strDesPassword"),pModifyInsurePass->szDesPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//�������
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyInsurePassword"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GP_ModifyInsurePassword"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�ǩ��
bool CDataBaseEngineSink::OnRequestModifyUnderWrite(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyUnderWrite));
		if (wDataSize!=sizeof(DBR_GP_ModifyUnderWrite)) return false;

		//������
		DBR_GP_ModifyUnderWrite * pModifyUnderWrite=(DBR_GP_ModifyUnderWrite *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyUnderWrite->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyUnderWrite->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyUnderWrite->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@strUnderWrite"),pModifyUnderWrite->szUnderWrite);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyUnderWrite"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GP_ModifyUnderWrite"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�޸�����
bool CDataBaseEngineSink::OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_ModifyIndividual));
		if (wDataSize!=sizeof(DBR_GP_ModifyIndividual)) return false;

		//������
		DBR_GP_ModifyIndividual * pModifyIndividual=(DBR_GP_ModifyIndividual *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pModifyIndividual->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pModifyIndividual->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pModifyIndividual->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pModifyIndividual->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pModifyIndividual->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strUnderWrite"),pModifyIndividual->szUnderWrite);
		m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pModifyIndividual->szCompellation);
		m_AccountsDBAide.AddParameter(TEXT("@strQQ"),pModifyIndividual->szQQ);
		m_AccountsDBAide.AddParameter(TEXT("@strEMail"),pModifyIndividual->szEMail);
		m_AccountsDBAide.AddParameter(TEXT("@strSeatPhone"),pModifyIndividual->szSeatPhone);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pModifyIndividual->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@strDwellingPlace"),pModifyIndividual->szDwellingPlace);
		m_AccountsDBAide.AddParameter(TEXT("@strUserNote"),pModifyIndividual->szUserNote);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		WCHAR szDescribe[128]=L"";
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribe,sizeof(szDescribe),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyUserIndividual"),false);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnOperateDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GP_ModifyUserIndividual"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//������Ϸ��
bool CDataBaseEngineSink::OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_UserSaveScore));
		if (wDataSize!=sizeof(DBR_GP_UserSaveScore)) return false;

		//������
		DBR_GP_UserSaveScore * pUserSaveScore=(DBR_GP_UserSaveScore *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserSaveScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserSaveScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lSaveScore"),pUserSaveScore->lSaveScore);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
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
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_UserSaveScore"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ȡ��Ϸ��
bool CDataBaseEngineSink::OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_UserTakeScore));
		if (wDataSize!=sizeof(DBR_GP_UserTakeScore)) return false;

		//������
		DBR_GP_UserTakeScore * pUserTakeScore=(DBR_GP_UserTakeScore *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pUserTakeScore->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserTakeScore->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@lTakeScore"),pUserTakeScore->lTakeScore);
		m_TreasureDBAide.AddParameter(TEXT("@strPassword"),pUserTakeScore->szPassword);
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
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
		OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_UserTakeScore"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//ת����Ϸ��
bool CDataBaseEngineSink::OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_UserTransferScore));
		if (wDataSize!=sizeof(DBR_GP_UserTransferScore)) return false;

		//������
		DBR_GP_UserTransferScore * pUserTransferScore=(DBR_GP_UserTransferScore *)pData;

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
		m_TreasureDBAide.AddParameter(TEXT("@wKindID"),0L);
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"),0L);
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
		//OnInsureDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
		OnInsureTransferInfo(dwContextID,lResultCode);
		OnInsureDisposeResult(dwContextID,lResultCode,TEXT(""),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_UserTransferScore"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ѯ����
bool CDataBaseEngineSink::OnRequestQueryIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_QueryIndividual));
		if (wDataSize!=sizeof(DBR_GP_QueryIndividual)) return false;

		//������
		DBR_GP_QueryIndividual * pQueryIndividual=(DBR_GP_QueryIndividual *)pData;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pQueryIndividual->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pQueryIndividual->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);

		//�������
		if (m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QueryUserIndividual"),true)==DB_SUCCESS)
		{
			//��������
			DBO_GP_UserIndividual UserIndividual;
			ZeroMemory(&UserIndividual,sizeof(UserIndividual));

			//�û���Ϣ
			UserIndividual.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			m_AccountsDBAide.GetValue_String(TEXT("UserNote"),UserIndividual.szUserNote,CountArray(UserIndividual.szUserNote));
			m_AccountsDBAide.GetValue_String(TEXT("Compellation"),UserIndividual.szCompellation,CountArray(UserIndividual.szCompellation));

			//�绰����
			m_AccountsDBAide.GetValue_String(TEXT("SeatPhone"),UserIndividual.szSeatPhone,CountArray(UserIndividual.szSeatPhone));
			m_AccountsDBAide.GetValue_String(TEXT("MobilePhone"),UserIndividual.szMobilePhone,CountArray(UserIndividual.szMobilePhone));

			//��ϵ����
			m_AccountsDBAide.GetValue_String(TEXT("QQ"),UserIndividual.szQQ,CountArray(UserIndividual.szQQ));
			m_AccountsDBAide.GetValue_String(TEXT("EMail"),UserIndividual.szEMail,CountArray(UserIndividual.szEMail));
			m_AccountsDBAide.GetValue_String(TEXT("DwellingPlace"),UserIndividual.szDwellingPlace,CountArray(UserIndividual.szDwellingPlace));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INDIVIDUAL,dwContextID,&UserIndividual,sizeof(UserIndividual));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//������
			OnOperateDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GP_QueryUserIndividual"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ѯ����
bool CDataBaseEngineSink::OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_QueryInsureInfo));
		if (wDataSize!=sizeof(DBR_GP_QueryInsureInfo)) return false;

		//������
		DBR_GP_QueryInsureInfo * pQueryInsureInfo=(DBR_GP_QueryInsureInfo *)pData;

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
			DBO_GP_UserInsureInfo UserInsureInfo;
			ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

			//������Ϣ
			UserInsureInfo.wRevenueTake=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTake"));
			UserInsureInfo.wRevenueTransfer=m_TreasureDBAide.GetValue_WORD(TEXT("RevenueTransfer"));
			UserInsureInfo.wServerID=m_TreasureDBAide.GetValue_WORD(TEXT("ServerID"));
			UserInsureInfo.lUserScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Score"));
			UserInsureInfo.lUserInsure=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Insure"));
			UserInsureInfo.lTransferPrerequisite=m_TreasureDBAide.GetValue_LONGLONG(TEXT("TransferPrerequisite"));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_INFO,dwContextID,&UserInsureInfo,sizeof(UserInsureInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//������
			OnOperateDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GR_QueryUserInsureInfo"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//������֤
bool CDataBaseEngineSink::OnRequestUserCheckPassword(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(CMD_GP_CheckPassword));
		if (wDataSize!=sizeof(CMD_GP_CheckPassword)) return false;

		//������
		CMD_GP_CheckPassword * pCheckPassword=(CMD_GP_CheckPassword *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pCheckPassword->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strInsurePassword"),pCheckPassword->szPassword);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//�������
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserCheckPassword"),true);

		//��������
		CMD_GP_CheckPasswordReturn CheckPasswordReturn={0};
		if (lResultCode==0)
		{
			CheckPasswordReturn.bPasswordRight=true;
		}
		else
		{
			CheckPasswordReturn.bPasswordRight=false;
		}

		//���ͽ��
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_CHECK_PASSWORD,dwContextID,&CheckPasswordReturn,sizeof(CheckPasswordReturn));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GP_UserCheckPassword"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnOperateDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ѯ�û�
bool CDataBaseEngineSink::OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_QueryInsureUserInfo));
	if (wDataSize!=sizeof(DBR_GP_QueryInsureUserInfo)) return false;

	//������
	DBR_GP_QueryInsureUserInfo * pQueryTransferUserInfo=(DBR_GP_QueryInsureUserInfo *)pData;

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
			DBO_GP_UserTransferUserInfo TransferUserInfo;
			ZeroMemory(&TransferUserInfo,sizeof(TransferUserInfo));

			//������Ϣ
			TransferUserInfo.dwGameID=m_TreasureDBAide.GetValue_DWORD(TEXT("GameID"));
			m_TreasureDBAide.GetValue_String(TEXT("NickName"), TransferUserInfo.szNickName, CountArray(TransferUserInfo.szNickName));

			//���ͽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_USER_INFO,dwContextID,&TransferUserInfo,sizeof(TransferUserInfo));
		}
		else
		{
			//��ȡ����
			CDBVarValue DBVarValue;
			m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

			//������
			OnInsureDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GR_QueryTransferUserInfo"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnInsureDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}
}

//��ѯǩ��
bool CDataBaseEngineSink::OnRequestQuerySignInInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_QuerySignInInfo));
	if (wDataSize!=sizeof(DBR_GP_QuerySignInInfo)) return false;

	//������
	DBR_GP_QuerySignInInfo * pQuerySignInInfo=(DBR_GP_QuerySignInInfo *)pData;

	try
	{
		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@UserID"),pQuerySignInInfo->dwUserID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_QuerySignInInfo"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnSignInDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GP_QuerySignInInfo"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnSignInDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}
}

//ǩ������
bool CDataBaseEngineSink::OnRequestUserSignIn( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_UserSignIn));
	if (wDataSize!=sizeof(DBR_GP_UserSignIn)) return false;

	//������
	DBR_GP_UserSignIn * pUserSignIn=(DBR_GP_UserSignIn *)pData;

	try
	{
		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@dwUserID"),pUserSignIn->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@cbIsVIP"),pUserSignIn->cbVip);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_UserSignIn"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnSignInDisposeResult(dwContextID,m_TreasureDBAide.GetReturnValue(),CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GP_UserSignIn"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnSignInDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}
}

//�齱��ѯ
bool CDataBaseEngineSink::OnRequestQueryLotteryInfo( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_QryLtryInfo));
	if (wDataSize!=sizeof(DBR_GP_QryLtryInfo)) return false;

	//������
	DBR_GP_QryLtryInfo * pQryLtryInfo=(DBR_GP_QryLtryInfo *)pData;

	try
	{
		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pQryLtryInfo->dwUserID);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("PROC_GET_PRIZE_UserInfo"),true);

		//�������
		OnLotteryDisposeResult(dwContextID,m_AccountsDBAide.GetReturnValue(),TEXT("�齱��ѯ�ɹ�"),false);
	}
	catch (IDataBaseException* pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("PROC_GET_PRIZE_UserInfo"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnLotteryDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);
	}

	return true;
}

//�齱����
bool CDataBaseEngineSink::OnRequestUserLtry( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_UserLtry));
	if (wDataSize!=sizeof(DBR_GP_UserLtry)) return false;

	//������
	DBR_GP_UserLtry * pUserLtry=(DBR_GP_UserLtry *)pData;

	//�����н�����
	INT iPrizeItem;

	try
	{
		//���������
		UINT uRandNum=rand()%10000;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pUserLtry->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),uRandNum);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//��ȡ�н�����
		printf(TEXT("GSP_GP_GetPrizeItemNum\n"));
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GetPrizeItemNum"),true);

		if (lResultCode == DB_GET_PRIZEITEM_SUCCESS)
		{
			iPrizeItem=m_AccountsDBAide.GetValue_DWORD(TEXT("PrizeItemID"));

			printf(TEXT("%d"), iPrizeItem);
			//��ȡ��ǰʱ��
			SYSTEMTIME systime; 
			GetLocalTime(&systime);
			TCHAR szTimeNow[100];
			_sntprintf(szTimeNow, 100, TEXT("%4d-%02d-%02d %02d:%02d:%02d.%03d"),systime.wYear,systime.wMonth,systime.wDay,systime.wHour,systime.wMinute, systime.wSecond,systime.wMilliseconds);

			//�н���Ϣ����
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@UserID"),pUserLtry->dwUserID);
			m_AccountsDBAide.AddParameter(TEXT("@PrizeItemID"),iPrizeItem);
			m_AccountsDBAide.AddParameter(TEXT("@PrizeTime"),szTimeNow);

			printf(TEXT("PROC_SAVE_PrizeLog\n"));
			lResultCode = m_AccountsDBAide.ExecuteProcess(TEXT("PROC_SAVE_PrizeLog"),true);

			//�������
			m_AccountsDBAide.GetValue_String(TEXT("ErrorText"),szDescribeString,CountArray(szDescribeString));
		}
		else 
		{
			//�������
			CDBVarValue DBVarValue;
			m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
			lstrcpyn(szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(szDescribeString));
			lResultCode = DB_GET_PRIZEITEM_FAILURE;
		}

		OnLotteryDisposeResult(dwContextID,lResultCode,szDescribeString,false);
	}
	catch (IDataBaseException* pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("PROC_SAVE_PrizeLog"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//�������
		OnLotteryDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);
	}

	return true;
}

//�����б�
bool CDataBaseEngineSink::OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//��������
		WORD wPacketSize=0;
		BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];

		//��������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameTypeItem"),true);

		//��������
		wPacketSize=0;
		DBO_GP_GameType * pGameType=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GameType))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGameType=(DBO_GP_GameType *)(cbBuffer+wPacketSize);
			pGameType->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameType->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameType->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
			m_PlatformDBAide.GetValue_String(TEXT("TypeName"),pGameType->szTypeName,CountArray(pGameType->szTypeName));

			//����λ��
			wPacketSize+=sizeof(DBO_GP_GameType);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��ȡ����
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameKindItem"),true);

		//��������
		wPacketSize=0;
		DBO_GP_GameKind * pGameKind=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GameKind))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGameKind=(DBO_GP_GameKind *)(cbBuffer+wPacketSize);
			pGameKind->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameKind->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
			pGameKind->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameKind->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGameKind->wGameID=m_PlatformDBAide.GetValue_WORD(TEXT("GameID"));
			m_PlatformDBAide.GetValue_String(TEXT("KindName"),pGameKind->szKindName,CountArray(pGameKind->szKindName));
			m_PlatformDBAide.GetValue_String(TEXT("ProcessName"),pGameKind->szProcessName,CountArray(pGameKind->szProcessName));

			//����λ��
			wPacketSize+=sizeof(DBO_GP_GameKind);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��ȡ�ڵ�
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameNodeItem"),true);

		//���ͽڵ�
		wPacketSize=0;
		DBO_GP_GameNode * pGameNode=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GameNode))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGameNode=(DBO_GP_GameNode *)(cbBuffer+wPacketSize);
			pGameNode->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameNode->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGameNode->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameNode->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
			m_PlatformDBAide.GetValue_String(TEXT("NodeName"),pGameNode->szNodeName,CountArray(pGameNode->szNodeName));
			
			//����λ��
			wPacketSize+=sizeof(DBO_GP_GameNode);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��ȡ����
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGamePageItem"),true);

		//���Ͷ���
		wPacketSize=0;
		DBO_GP_GamePage * pGamePage=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GamePage))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGamePage=(DBO_GP_GamePage *)(cbBuffer+wPacketSize);
			pGamePage->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGamePage->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
			pGamePage->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGamePage->wPageID=m_PlatformDBAide.GetValue_WORD(TEXT("PageID"));
			pGamePage->wOperateType=m_PlatformDBAide.GetValue_WORD(TEXT("OperateType"));
			m_PlatformDBAide.GetValue_String(TEXT("DisplayName"),pGamePage->szDisplayName,CountArray(pGamePage->szDisplayName));
			
			//����λ��
			wPacketSize+=sizeof(DBO_GP_GamePage);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��������
		DBO_GP_GameListResult GameListResult;
		ZeroMemory(&GameListResult,sizeof(GameListResult));

		//���ñ���
		GameListResult.cbSuccess=TRUE;

		//������Ϣ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("OnRequestLoadGameList"),TraceLevel_Exception);
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		//��������
		DBO_GP_GameListResult GameListResult;
		ZeroMemory(&GameListResult,sizeof(GameListResult));

		//���ñ���
		GameListResult.cbSuccess=FALSE;

		//������Ϣ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

		return false;
	}

	return true;
}

//������Ϣ
bool CDataBaseEngineSink::OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//��������
		DBR_GP_OnLineCountInfo * pOnLineCountInfo=(DBR_GP_OnLineCountInfo *)pData;
		WORD wHeadSize=(sizeof(DBR_GP_OnLineCountInfo)-sizeof(pOnLineCountInfo->OnLineCountKind));

		//Ч������
		ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKind))));
		if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pOnLineCountInfo->wKindCount*sizeof(tagOnLineInfoKind)))) return false;

		//������ʶ
		TCHAR szMachineID[LEN_MACHINE_ID];
		CWHService::GetMachineID(szMachineID);

		//������Ϣ
		TCHAR szOnLineCountKind[2048]=TEXT("");
		for (WORD i=0;i<pOnLineCountInfo->wKindCount;i++)
		{
			INT nLength=lstrlen(szOnLineCountKind);
			_sntprintf(&szOnLineCountKind[nLength],CountArray(szOnLineCountKind)-nLength,TEXT("%d:%ld;"),pOnLineCountInfo->OnLineCountKind[i].wKindID,
				pOnLineCountInfo->OnLineCountKind[i].dwOnLineCount);
		}

		//�������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.AddParameter(TEXT("@strMachineID"),szMachineID);
		m_PlatformDBAide.AddParameter(TEXT("@strMachineServer"),m_pInitParameter->m_szServerName);
		m_PlatformDBAide.AddParameter(TEXT("@dwOnLineCountSum"),pOnLineCountInfo->dwOnLineCountSum);
		m_PlatformDBAide.AddParameter(TEXT("@strOnLineCountKind"),szOnLineCountKind);

		//ִ������
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_OnLineCountInfo"),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("GSP_GP_OnLineCountInfo"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//��¼�ɹ�
VOID CDataBaseEngineSink::OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,bool bIsLogin)
{
	if (bMobileClient==false)
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//��������
			DBO_GP_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//��������
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
			LogonSuccess.dwUserMedal=m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));

			//�û��ɼ�
			LogonSuccess.lUserScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			LogonSuccess.lUserInsure=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));

			//�û�����
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.cbMoorMachine=m_AccountsDBAide.GetValue_BYTE(TEXT("MoorMachine"));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));

			//��Ա����
			LogonSuccess.cbMemberOrder=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			m_AccountsDBAide.GetValue_SystemTime(TEXT("MemberOverDate"),LogonSuccess.MemberOverDate);

			LogonSuccess.nTotalCharge = m_AccountsDBAide.GetValue_DWORD("totalCharge");
			LogonSuccess.nTotalPlayTime = m_AccountsDBAide.GetValue_DWORD("totalPlayTime");

			LogonSuccess.cbVIPLevel = m_AccountsDBAide.GetValue_BYTE("vipLevel");
			LogonSuccess.dwVIPExp = m_AccountsDBAide.GetValue_DWORD("vipExp");
			LogonSuccess.dwExpPerCent = m_AccountsDBAide.GetValue_DWORD("vipExpPerCent");
			m_AccountsDBAide.GetValue_String(TEXT("customerServiceQQ"), LogonSuccess.szCustomerServiceQQ, CountArray(LogonSuccess.szCustomerServiceQQ));
			LogonSuccess.dwLotteryCount = m_AccountsDBAide.GetValue_DWORD("lotteryCount");
			LogonSuccess.nLogonGift = m_AccountsDBAide.GetValue_LONG("logonGift");
			LogonSuccess.cbCharge1stCount = m_AccountsDBAide.GetValue_BYTE("Charge1stCount");
			LogonSuccess.cbCharge1stCanGet = m_AccountsDBAide.GetValue_BYTE("Charge1stCanGet");
			LogonSuccess.nCanShare = m_AccountsDBAide.GetValue_LONG("CanShare");
			LogonSuccess.cbCanGetBankruptcy = m_AccountsDBAide.GetValue_BYTE("CanGetBankruptcy");
			LogonSuccess.nBankruptcyCount = m_AccountsDBAide.GetValue_LONG("BankruptcyCount");

			//��ȡ��Ϣ
			lstrcpyn(LogonSuccess.szDescribeString,pszErrorString,CountArray(LogonSuccess.szDescribeString));
			LogonSuccess.bIsLogin = bIsLogin;
			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
		}
		else if(dwErrorCode == DB_GUEST_REG_SUCCESS)
		{
			//��������
			DBO_GP_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//��������
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
			LogonSuccess.dwUserMedal=m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));
			lstrcpyn(LogonSuccess.szPassword, pszErrorString, CountArray(LogonSuccess.szPassword));

			//�û��ɼ�
			LogonSuccess.lUserScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			LogonSuccess.lUserInsure=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));

			//�û�����
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.cbMoorMachine=m_AccountsDBAide.GetValue_BYTE(TEXT("MoorMachine"));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));

			//��Ա����
			LogonSuccess.cbMemberOrder=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			m_AccountsDBAide.GetValue_SystemTime(TEXT("MemberOverDate"),LogonSuccess.MemberOverDate);

			LogonSuccess.dwExpPerCent = m_AccountsDBAide.GetValue_DWORD("vipExpPerCent");
			m_AccountsDBAide.GetValue_String(TEXT("customerServiceQQ"), LogonSuccess.szCustomerServiceQQ, CountArray(LogonSuccess.szCustomerServiceQQ));
			LogonSuccess.nLogonGift = m_AccountsDBAide.GetValue_LONG("logonGift");

			LogonSuccess.nCanShare = m_AccountsDBAide.GetValue_LONG("CanShare");

			//��ȡ��Ϣ
			lstrcpyn(LogonSuccess.szDescribeString,"",CountArray(LogonSuccess.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GUEST_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
		}
		else if(dwErrorCode == DB_NEEDMB)
		{
			//��¼�ɹ�
			DBR_GP_ValidateMBCard ValidateMBCard;
			ZeroMemory(&ValidateMBCard,sizeof(ValidateMBCard));

			//��ȡ����
			ValidateMBCard.uMBCardID=m_AccountsDBAide.GetValue_UINT(TEXT("PasswordID"));

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VALIDATE_MBCARD,dwContextID,&ValidateMBCard,sizeof(ValidateMBCard));
		}
		else if(dwErrorCode == DB_VALIDATE_PASSPOSTID)
		{
			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VALIDATE_PASSPOSTID,dwContextID,NULL,0);
		}
		else
		{
			//��������
			DBO_GP_LogonFailure LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//��������
			LogonFailure.lResultCode=dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));
			LogonFailure.bIsLogin = bIsLogin;
			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}
	else
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//��������
			DBO_MB_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//��������
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));

			//��ȡ��Ϣ
			lstrcpyn(LogonSuccess.szDescribeString,pszErrorString,CountArray(LogonSuccess.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
		}
		else
		{
			//��������
			DBO_MB_LogonFailure LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//��������
			LogonFailure.lResultCode=dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}

	return;
}

//������Ϣ
VOID CDataBaseEngineSink::OnInsureTransferInfo(DWORD dwContextID, DWORD dwErrorCode)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		DBO_GP_UserInsureTransferInfo UserInsureTransferInfo={0};
		UserInsureTransferInfo.dwRecordID=m_TreasureDBAide.GetValue_DWORD(TEXT("RecordID"));
		UserInsureTransferInfo.dwSourceID=m_TreasureDBAide.GetValue_DWORD(TEXT("SourceGameID"));
		UserInsureTransferInfo.dwTargetID=m_TreasureDBAide.GetValue_DWORD(TEXT("TargetGameID"));
		UserInsureTransferInfo.lSwapScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("TransferScore"));
		m_TreasureDBAide.GetValue_String(TEXT("SourceNickName"),UserInsureTransferInfo.szSourceName,CountArray(UserInsureTransferInfo.szSourceName));
		m_TreasureDBAide.GetValue_String(TEXT("TargetNickName"),UserInsureTransferInfo.szTargetName,CountArray(UserInsureTransferInfo.szTargetName));
		m_TreasureDBAide.GetValue_SystemTime(TEXT("TransferTime"),UserInsureTransferInfo.timeTransfer);

		WORD wDataSize=sizeof(DBO_GP_UserInsureTransferInfo);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_TRANSFER_INFO,dwContextID,&UserInsureTransferInfo,wDataSize);
	}
}

//���н��
VOID CDataBaseEngineSink::OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//��������
		DBO_GP_UserInsureSuccess UserInsureSuccess;
		ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

		//�������
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
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_SUCCESS,dwContextID,&UserInsureSuccess,wHeadSize+wDataSize);
	}
	else
	{
		//��������
		DBO_GP_UserInsureFailure UserInsureFailure;
		ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

		//�������
		UserInsureFailure.lResultCode=dwErrorCode;
		lstrcpyn(UserInsureFailure.szDescribeString,pszErrorString,CountArray(UserInsureFailure.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(UserInsureFailure.szDescribeString);
		WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_INSURE_FAILURE,dwContextID,&UserInsureFailure,wHeadSize+wDataSize);
	}

	return;
}

//�������
VOID CDataBaseEngineSink::OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
	if (dwErrorCode==DB_SUCCESS)
	{
		//��������
		DBO_GP_OperateSuccess OperateSuccess;
		ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

		//�������
		OperateSuccess.lResultCode=dwErrorCode;
		lstrcpyn(OperateSuccess.szDescribeString,pszErrorString,CountArray(OperateSuccess.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(OperateSuccess.szDescribeString);
		WORD wHeadSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_SUCCESS,dwContextID,&OperateSuccess,wHeadSize+wDataSize);
	}
	else
	{
		//��������
		DBO_GP_OperateFailure OperateFailure;
		ZeroMemory(&OperateFailure,sizeof(OperateFailure));

		//�������
		OperateFailure.lResultCode=dwErrorCode;
		lstrcpyn(OperateFailure.szDescribeString,pszErrorString,CountArray(OperateFailure.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(OperateFailure.szDescribeString);
		WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_OPERATE_FAILURE,dwContextID,&OperateFailure,wHeadSize+wDataSize);
	}

	return;
}

//ǩ�����
VOID CDataBaseEngineSink::OnSignInDisposeResult( DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient )
{
	if (dwErrorCode==DB_QUERY_SIGNIN_SUCCESS)
	{
		//��������
		DBO_GP_UserSignInInfo SignInQrySuccess;
		ZeroMemory(&SignInQrySuccess,sizeof(SignInQrySuccess));

		////�������
		SignInQrySuccess.nNum1 = m_TreasureDBAide.GetValue_INT(TEXT("SignDayNormal"));
		SignInQrySuccess.nNum2 = m_TreasureDBAide.GetValue_INT(TEXT("SignDayVIP"));

		TCHAR szNormalRewardType[100]=TEXT("");
		TCHAR szVIPRewardType[100]=TEXT("");
		TCHAR szNormalRewardCount[100]=TEXT("");
		TCHAR szVIPRewardCount[100]=TEXT("");
		for (int i = 0; i < sizeof(SignInQrySuccess.normal)/sizeof(SignInQrySuccess.normal[0]) ; i++)
		{
			_sntprintf(szNormalRewardType,CountArray(szNormalRewardType),TEXT("Day%dRewardType"),(i + 1));
			_sntprintf(szNormalRewardCount,CountArray(szNormalRewardCount),TEXT("Day%dRewardCount"),(i + 1));
			_sntprintf(szVIPRewardType,CountArray(szVIPRewardType),TEXT("Day%dRewardTypeVIP"),(i + 1));
			_sntprintf(szVIPRewardCount,CountArray(szVIPRewardCount),TEXT("Day%dRewardCountVIP"),(i + 1));

			SignInQrySuccess.normal[i].cbType = m_TreasureDBAide.GetValue_INT(szNormalRewardType);
			SignInQrySuccess.normal[i].nCount = m_TreasureDBAide.GetValue_INT(szNormalRewardCount);

			SignInQrySuccess.vip[i].cbType = m_TreasureDBAide.GetValue_INT(szVIPRewardType);
			SignInQrySuccess.vip[i].nCount = m_TreasureDBAide.GetValue_INT(szVIPRewardCount);
		}

		// Modify the Day Mark
		for (int i = 0; i < sizeof(SignInQrySuccess.normal)/sizeof(SignInQrySuccess.normal[0]) ; i++)
		{
			if (SignInQrySuccess.nNum1 > 0 && (i + 1) < (SignInQrySuccess.nNum1))
			{
				SignInQrySuccess.normal[i].cbAttrib = 1;
				continue;
			}

			if (SignInQrySuccess.nNum1 < 0 && (i + 1) <= (-SignInQrySuccess.nNum1))
			{
				SignInQrySuccess.normal[i].cbAttrib = 1;
				continue;
			}

			SignInQrySuccess.normal[i].cbAttrib = 0;
		}

		for (int i = 0; i < sizeof(SignInQrySuccess.normal)/sizeof(SignInQrySuccess.normal[0]) ; i++)
		{
			if (SignInQrySuccess.nNum2 > 0 && (i + 1) < (SignInQrySuccess.nNum2))
			{
				SignInQrySuccess.vip[i].cbAttrib = 1;
				continue;
			}

			if (SignInQrySuccess.nNum2 < 0 && (i + 1) <= (-SignInQrySuccess.nNum2))
			{
				SignInQrySuccess.vip[i].cbAttrib = 1;
				continue;
			}

			SignInQrySuccess.vip[i].cbAttrib = 0;

		}

		for (int i = 0; i < 7; i++)
		{
			// test
			printf("day %d,normal is %hhu, %hhu\n", i, SignInQrySuccess.normal[i].cbAttrib,SignInQrySuccess.vip[i].cbAttrib);
		}

		// Get The Abs Values
		if (SignInQrySuccess.nNum1 < 0)
		{
			SignInQrySuccess.nNum1 = -SignInQrySuccess.nNum1;
		}
		if (SignInQrySuccess.nNum2 < 0)
		{
			SignInQrySuccess.nNum2 = -SignInQrySuccess.nNum2;
		}

		//���ͽ��
		WORD wDataSize=sizeof(SignInQrySuccess);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SIGNIN_QUERY_SUCCESS,dwContextID,&SignInQrySuccess,wDataSize);
	}

	else if (dwErrorCode==DB_QUERY_SIGNIN_FAILURE)
	{
		//m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SIGNIN_QUERY_FAILURE,dwContextID,&SignInFailure,wHeadSize+wDataSize);
	}

	//���ǩ���ɹ�
	else if (dwErrorCode==DB_USER_SIGNIN_SUCCESS)
	{
		//��������
		DBO_GP_UserSignInResult SignInSuccess;
		ZeroMemory(&SignInSuccess,sizeof(SignInSuccess));

		//�������
		SignInSuccess.bSuccess = true;
		SignInSuccess.llScoreCount = m_TreasureDBAide.GetValue_LONGLONG(TEXT("ScoreCount"));
		SignInSuccess.llLotteryCount = m_TreasureDBAide.GetValue_LONGLONG(TEXT("LotteryCount"));
		SignInSuccess.cbUserStatus = m_TreasureDBAide.GetValue_INT(TEXT("IsVIP"));

		//��ȡ��Ϣ
		lstrcpyn(SignInSuccess.szDescription,pszErrorString,CountArray(SignInSuccess.szDescription));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(SignInSuccess.szDescription);
		WORD wHeadSize=sizeof(SignInSuccess)-sizeof(SignInSuccess.szDescription);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_SIGNIN_SUCCESS,dwContextID,&SignInSuccess,wHeadSize+wDataSize);
	}

	//���ǩ��ʧ��
	else if (dwErrorCode==DB_USER_SIGNIN_FAILURE)
	{
		//��������
		DBO_GP_UserSignInResult SignInFailure;
		ZeroMemory(&SignInFailure,sizeof(SignInFailure));

		//�������
		SignInFailure.bSuccess = false;
		SignInFailure.llLotteryCount = 0;
		SignInFailure.llScoreCount = 0;

		//��ȡ��Ϣ
		lstrcpyn(SignInFailure.szDescription,pszErrorString,CountArray(SignInFailure.szDescription));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(SignInFailure.szDescription);
		WORD wHeadSize=sizeof(SignInFailure)-sizeof(SignInFailure.szDescription);

		printf("DBO_GP_USER_SIGNIN_FAILURE\n");
		printf("%s", SignInFailure.szDescription);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_SIGNIN_FAILURE,dwContextID,&SignInFailure,wHeadSize+wDataSize);
	}

	else
	{
		ASSERT(false);
		return;
	}
}

//�齱���
VOID CDataBaseEngineSink::OnLotteryDisposeResult( DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient )
{
	if (dwErrorCode==DB_QUERY_LOTTERY_SUCCESS)
	{
		//��������
		DBO_GP_Ltry_Qry_Res LtryQryRes;
		ZeroMemory(&LtryQryRes,sizeof(DBO_GP_Ltry_Qry_Res));

		//�������
		LtryQryRes.wFreeCount = m_AccountsDBAide.GetValue_WORD(TEXT("ALLCOUNT"));

		//��ȡ��Ϣ
		lstrcpyn(LtryQryRes.szDescribeString,pszErrorString,CountArray(LtryQryRes.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(LtryQryRes.szDescribeString);
		WORD wHeadSize=sizeof(LtryQryRes)-sizeof(LtryQryRes.szDescribeString);

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOTTERY_QUERY_RESULT,dwContextID,&LtryQryRes,wHeadSize+wDataSize);
	}
	else if (dwErrorCode==DB_QUERY_LOTTERY_FAILURE)
	{
		return;
	}
	else if (dwErrorCode==DB_USER_LOTTERY_SUCCESS)
	{
		//��������
		DBO_GP_User_Ltry_Res UserLtryRes;
		ZeroMemory(&UserLtryRes,sizeof(DBO_GP_User_Ltry_Res));

		//��ȡ��Ϣ
		UserLtryRes.wIsLtrySuc=true;
		UserLtryRes.wPrizeID=m_AccountsDBAide.GetValue_WORD(TEXT("PrizeItemID"));
		UserLtryRes.wFreeCount=m_AccountsDBAide.GetValue_WORD(TEXT("ALLCOUNT"));
		UserLtryRes.lUserScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
		m_AccountsDBAide.GetValue_String(TEXT("NickName"),UserLtryRes.szNickName,CountArray(UserLtryRes.szNickName,));
		lstrcpyn(UserLtryRes.szDescribeString,pszErrorString,CountArray(UserLtryRes.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(UserLtryRes.szDescribeString);
		WORD wHeadSize=sizeof(UserLtryRes)-sizeof(UserLtryRes.szDescribeString);

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_LOTTERY_RESULT,dwContextID,&UserLtryRes,wHeadSize+wDataSize);
	}
	else if ((dwErrorCode==DB_USER_LOTTERY_FAILURE) || (dwErrorCode==DB_GET_PRIZEITEM_FAILURE))
	{
		//��������
		DBO_GP_User_Ltry_Res UserLtryRes;
		ZeroMemory(&UserLtryRes,sizeof(DBO_GP_User_Ltry_Res));

		//��ȡ��Ϣ
		UserLtryRes.wIsLtrySuc=false;
		UserLtryRes.wPrizeID=-1;
		UserLtryRes.wFreeCount=-1;
		lstrcpyn(UserLtryRes.szDescribeString, pszErrorString,CountArray(UserLtryRes.szDescribeString));

		//���ͽ��
		WORD wDataSize=CountStringBuffer(UserLtryRes.szDescribeString);
		WORD wHeadSize=sizeof(UserLtryRes)-sizeof(UserLtryRes.szDescribeString);

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_LOTTERY_RESULT,dwContextID,&UserLtryRes,wHeadSize+wDataSize);
	}

	else
	{
		ASSERT(false);
	}
}

//�ʼ�ID�б�
bool CDataBaseEngineSink::OnRequestQueryMailIDList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
//printf("@@@@@ OnRequestQueryMailIDList\n");
	try
	{
		DBR_MailIDList * pMailIDList=(DBR_MailIDList *)pData;

		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pMailIDList->dwUserID);
		LONG lReturnValue = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QueryMailIDList"),true);

		if (lReturnValue==0 && 0 != m_AccountsDBModule->GetRecordCount())
		{
			m_AccountsDBModule->MoveToFirst();

			DBO_MailIDList MailIDList = {0};
			while (false == m_AccountsDBModule->IsRecordsetEnd())
			{
				MailIDList.dwMailID[MailIDList.cbCount++] = m_AccountsDBAide.GetValue_DWORD(TEXT("MailID"));
				if (MailIDList.cbCount >= 30) break;
				m_AccountsDBModule->MoveToNext();
			}
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_MAIL_ID_LIST,dwContextID,&MailIDList,sizeof(MailIDList));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CTraceService::TraceString(TEXT("GSP_GP_QueryMailIDList"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

//�ʼ���Ϣ
bool CDataBaseEngineSink::OnRequestQueryMailInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
//printf("@@@@@ OnRequestQueryMailInfo\n");
	try
	{
		DBR_MailInfo * pMailInfo=(DBR_MailInfo *)pData;

		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@MailID1"),pMailInfo->dwMailID[0]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID2"),pMailInfo->dwMailID[1]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID3"),pMailInfo->dwMailID[2]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID4"),pMailInfo->dwMailID[3]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID5"),pMailInfo->dwMailID[4]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID6"),pMailInfo->dwMailID[5]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID7"),pMailInfo->dwMailID[6]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID8"),pMailInfo->dwMailID[7]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID9"),pMailInfo->dwMailID[8]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID10"),pMailInfo->dwMailID[9]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID11"),pMailInfo->dwMailID[10]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID12"),pMailInfo->dwMailID[11]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID13"),pMailInfo->dwMailID[12]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID14"),pMailInfo->dwMailID[13]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID15"),pMailInfo->dwMailID[14]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID16"),pMailInfo->dwMailID[15]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID17"),pMailInfo->dwMailID[16]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID18"),pMailInfo->dwMailID[17]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID19"),pMailInfo->dwMailID[18]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID20"),pMailInfo->dwMailID[19]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID21"),pMailInfo->dwMailID[20]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID22"),pMailInfo->dwMailID[21]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID23"),pMailInfo->dwMailID[22]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID24"),pMailInfo->dwMailID[23]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID25"),pMailInfo->dwMailID[24]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID26"),pMailInfo->dwMailID[25]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID27"),pMailInfo->dwMailID[26]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID28"),pMailInfo->dwMailID[27]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID29"),pMailInfo->dwMailID[28]);
		m_AccountsDBAide.AddParameter(TEXT("@MailID30"),pMailInfo->dwMailID[29]);
		LONG lReturnValue = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QueryMailInfo"),true);

		if (lReturnValue==0 && 0 != m_AccountsDBModule->GetRecordCount())
		{
			m_AccountsDBModule->MoveToFirst();

			char cbBuffer[4096] = {0};
			SYSTEMTIME st = {0};
			DWORD * pCount = (DWORD *)cbBuffer;
			DBO_MailInfo * pMailInfo = (DBO_MailInfo *)&cbBuffer[4];
			while (false == m_AccountsDBModule->IsRecordsetEnd())
			{
				pMailInfo->dwMailID = m_AccountsDBAide.GetValue_DWORD(TEXT("MailID"));
				pMailInfo->cbMailStatus = m_AccountsDBAide.GetValue_BYTE(TEXT("MailStatus"));
				pMailInfo->cbCanGet = m_AccountsDBAide.GetValue_BYTE(TEXT("CanGet"));
				m_AccountsDBAide.GetValue_String(TEXT("MailTitle"),pMailInfo->szMailTitle,CountArray(pMailInfo->szMailTitle));
				m_AccountsDBAide.GetValue_SystemTime(TEXT("MailTime"),st);
				sprintf(pMailInfo->szMailTime, "%04hu-%02hu-%02hu %02hu:%02hu:%02hu", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

				if (++(*pCount) >= 30) break;
				pMailInfo++;

				m_AccountsDBModule->MoveToNext();
			}
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_MAIL_INFO,dwContextID,cbBuffer,sizeof(DBO_MailInfo)*(*pCount)+sizeof(DWORD));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CTraceService::TraceString(TEXT("GSP_GP_QueryMailInfo"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

//�ʼ�����
bool CDataBaseEngineSink::OnRequestQueryMailContent(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
//printf("@@@@@ OnRequestQueryMailContent\n");
	try
	{
		DBR_MailContent * pMailContent=(DBR_MailContent *)pData;

		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@MailID"),pMailContent->dwMailID);
		LONG lReturnValue = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QueryMailContent"),true);

		if (lReturnValue == DB_SUCCESS)
		{
			BYTE cbDataBuffer[SOCKET_TCP_PACKET];
			MAIL_CONTENT_STRING *sMailItemContent = (MAIL_CONTENT_STRING *)cbDataBuffer;;
			sMailItemContent->bySize= 1;
			WORD wHeadSize=sizeof(MAIL_CONTENT_STRING);
			CSendPacketHelper SendPacket(cbDataBuffer+ wHeadSize,sizeof(cbDataBuffer)-wHeadSize);
			int nItem[10];
			nItem[0] = m_AccountsDBAide.GetValue_INT(TEXT("Item0"));
			nItem[1] = m_AccountsDBAide.GetValue_INT(TEXT("Item1"));
			nItem[2] = m_AccountsDBAide.GetValue_INT(TEXT("Item2"));
			nItem[3] = m_AccountsDBAide.GetValue_INT(TEXT("TreasureMap"));
			nItem[4] = m_AccountsDBAide.GetValue_INT(TEXT("Item4"));
			nItem[5] = m_AccountsDBAide.GetValue_INT(TEXT("Item5"));
			nItem[6] = m_AccountsDBAide.GetValue_INT(TEXT("Item6"));
			nItem[7] = m_AccountsDBAide.GetValue_INT(TEXT("Item7"));
			nItem[8] = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Lottery"));
			nItem[9] = m_AccountsDBAide.GetValue_LONGLONG(TEXT("GiftGold"));
			char cbBuffer[4096] = {0};
			DWORD *pSize = (DWORD *)cbBuffer;
			char * pBuffer = (char *)&cbBuffer[4];
			m_AccountsDBAide.GetValue_String(TEXT("MailContent"),pBuffer,4096-sizeof(DWORD));
			*pSize = strlen(pBuffer)+1;
			if (*pSize> 1)
			{
				SendPacket.AddPacket(cbBuffer,*pSize+sizeof(DWORD),DTP_GP_MAIL_CONTENT);
			}
			MAIL_ITEM_INFO sMailItemInfo;
			for (int i = 0;i < 10; ++i)
			{
				if (nItem[i]<=0)
				{
					continue;
				}
				ZeroMemory(&sMailItemInfo,sizeof(sMailItemInfo));
				sMailItemInfo.nItemCount = nItem[i];
				SendPacket.AddPacket(&sMailItemInfo,sizeof(sMailItemInfo),DTP_GP_MAIL_CHUJI_DRAGON + i);
			}

			WORD wSendSize=SendPacket.GetDataSize()+sizeof(MAIL_CONTENT_STRING);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_MAIL_CONTENT,dwContextID,cbDataBuffer,wSendSize);
		}
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CTraceService::TraceString(TEXT("GSP_GP_QueryMailContent"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

//�ʼ���Ʒ
bool CDataBaseEngineSink::OnRequestQueryMailItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
//printf("@@@@@ OnRequestQueryMailItem\n");
	try
	{
		DBR_MailItem * pMailItem=(DBR_MailItem *)pData;

		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@MailID"),pMailItem->dwMailID);
		LONG lReturnValue = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_QueryMailItem"),true);

		BYTE cbDataBuffer[SOCKET_TCP_PACKET];
		DBO_MailItem *MailItem =(DBO_MailItem *)cbDataBuffer;
		MailItem->byResult = 0;
		//���Ͷ���
		if (lReturnValue == DB_SUCCESS)
		{
			sprintf(MailItem->szResult, "��ȡ�ɹ���");
			MailItem->byResult = 0;
			printf("��ȡ�ɹ�%d\n",MailItem->byResult);
		}
		else if (lReturnValue == 1)
		{
			MailItem->byResult = 1;
			sprintf(MailItem->szResult, "��ȡʧ�ܣ�����ȡ���ˡ�");
			printf("��ȡʧ�ܣ�����ȡ����%d\n",MailItem->byResult);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_MAIL_ITEM,dwContextID,MailItem,sizeof(DBO_MailItem));
			return true;
		}
		else if (lReturnValue == 2)
		{
			MailItem->byResult = 2;
			sprintf(MailItem->szResult, "�����˳���Ϸ���䡣");
			printf("�����˳���Ϸ����%d\n",MailItem->byResult);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_MAIL_ITEM,dwContextID,MailItem,sizeof(DBO_MailItem));
			return true;
		}

		WORD wHeadSize=sizeof(DBO_MailItem);
		CSendPacketHelper SendPacket(cbDataBuffer+ wHeadSize,sizeof(cbDataBuffer)-wHeadSize);
		LONGLONG lItem[2];
		lItem[0] = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Lottery"));
		lItem[1] = m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
		MAIL_ITEM_INFO sMailItemInfo;
		for (int i = 0;i < 2; ++i)
		{
			if (lItem[i]<=0)
			{
				continue;
			}
			ZeroMemory(&sMailItemInfo,sizeof(sMailItemInfo));
			sMailItemInfo.nItemCount = lItem[i];
			SendPacket.AddPacket(&sMailItemInfo,sizeof(sMailItemInfo),DTP_GP_MAIL_LOTTERY + i);
		}

		WORD wSendSize=SendPacket.GetDataSize()+sizeof(DBO_MailItem);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_MAIL_ITEM,dwContextID,cbDataBuffer,wSendSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CTraceService::TraceString(TEXT("GSP_GP_QueryMailItem"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

//��ֵ����-���
bool CDataBaseEngineSink::OnRequestChargeValue(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
//printf("@@@@@ OnRequestChargeValue\n");
	try
	{
		DBR_ChargeValue * pChargeValue=(DBR_ChargeValue *)pData;

		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pChargeValue->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@Clear"),0);
		LONG lReturnValue = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ChargeValue"),true);

		if (lReturnValue == DB_SUCCESS)
		{
			DBO_ChargeValue ChargeValue = {0};
			ChargeValue.lChargeValue = m_AccountsDBAide.GetValue_LONGLONG(TEXT("ChargeValue"));

			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHARGE_VALUE,dwContextID,&ChargeValue,sizeof(DBO_ChargeValue));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CTraceService::TraceString(TEXT("GSP_GP_ChargeValue"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

//��ֵ����-ȡ��
bool CDataBaseEngineSink::OnRequestChargeCancel(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
//printf("@@@@@ OnRequestChargeCancel\n");
	try
	{
		DBR_ChargeValue * pChargeValue=(DBR_ChargeValue *)pData;

		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pChargeValue->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@Clear"),1);
		LONG lReturnValue = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ChargeValue"),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CTraceService::TraceString(TEXT("GSP_GP_ChargeValue"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

//��ֵ����-��վ��ֵ���
bool CDataBaseEngineSink::OnRequestWebChargeValue(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
//printf("@@@@@ OnRequestWebChargeValue\n");
	try
	{
		DBR_WebChargeValue * pWebChargeValue=(DBR_WebChargeValue *)pData;

		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pWebChargeValue->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@ChargeValue"),pWebChargeValue->lChargeValue);
		LONG lReturnValue = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_WebChargeValue"),true);

		//�����޷�֪ͨ�ͻ��ˣ���Ϊ��¼�������޷������ĸ��ͻ��˷���Ϣ

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CTraceService::TraceString(TEXT("GSP_GP_WebChargeValue"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

//VIP��Ϣ
bool CDataBaseEngineSink::OnRequestVIPInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
//printf("@@@@@ OnRequestVIPInfo\n");
	try
	{
		DBR_VIPUser * pVIPUser=(DBR_VIPUser *)pData;

		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pVIPUser->dwUserID);
		LONG lReturnValue = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_VIPInfo"),true);

		if (lReturnValue == DB_SUCCESS)
		{
			DBO_VIPInfo VIPInfo = {0};
			VIPInfo.cbVIPLevel = m_AccountsDBAide.GetValue_BYTE(TEXT("VipLevel"));
			VIPInfo.dwVIPExp = m_AccountsDBAide.GetValue_DWORD(TEXT("VipExp"));
			VIPInfo.dwVIPNextExp = m_AccountsDBAide.GetValue_DWORD(TEXT("VipNextExp"));
			VIPInfo.dwEverydayGift = m_AccountsDBAide.GetValue_DWORD(TEXT("EverydayGift"));
			VIPInfo.dwChargeBack = m_AccountsDBAide.GetValue_DWORD(TEXT("ChargeBack"));

			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VIP_INFO,dwContextID,&VIPInfo,sizeof(DBO_VIPInfo));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CTraceService::TraceString(TEXT("GSP_GP_VIPInfo"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

//VIPÿ�����ͽ��
bool CDataBaseEngineSink::OnRequestVIPEverydayGift(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
//printf("@@@@@ OnRequestVIPEverydayGift\n");
	try
	{
		DBR_VIPUser * pVIPUser=(DBR_VIPUser *)pData;

		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pVIPUser->dwUserID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		LONG lReturnValue = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_VIPEverydayGift"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		DBO_VIPResult VIPResult = {0};

		if (lReturnValue == DB_SUCCESS)
		{
			VIPResult.lScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("CurrentScore"));
		}
		else
		{
			VIPResult.lScore = -1;
		}
		strncpy(VIPResult.szDescription,CW2CT(DBVarValue.bstrVal),CountArray(VIPResult.szDescription));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VIP_RESULT,dwContextID,&VIPResult,sizeof(DBO_VIPResult));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CTraceService::TraceString(TEXT("GSP_GP_VIPEverydayGift"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

//VIP��ֵ����
bool CDataBaseEngineSink::OnRequestVIPChargeBack(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
//printf("@@@@@ OnRequestVIPChargeBack\n");
	try
	{
		DBR_VIPUser * pVIPUser=(DBR_VIPUser *)pData;

		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pVIPUser->dwUserID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		LONG lReturnValue = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_VIPChargeBack"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		DBO_VIPResult VIPResult = {0};

		if (lReturnValue == DB_SUCCESS)
		{
			VIPResult.lScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("CurrentScore"));
		}
		else
		{
			VIPResult.lScore = -1;
		}
		strncpy(VIPResult.szDescription,CW2CT(DBVarValue.bstrVal),CountArray(VIPResult.szDescription));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VIP_RESULT,dwContextID,&VIPResult,sizeof(DBO_VIPResult));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CTraceService::TraceString(TEXT("GSP_GP_VIPChargeBack"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

//��ǰ����
bool CDataBaseEngineSink::OnRequestCurrentScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
//printf("@@@@@ OnRequestCurrentScore\n");
	DBR_CurrentScore * pCurrentScore=(DBR_CurrentScore *)pData;
	LONG lReturnValue = 0;
	DBO_CurrentScore CurrentScore = {0};

	try
	{
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pCurrentScore->dwUserID);
		lReturnValue = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_CurrentScore"),true);

		if (lReturnValue == DB_SUCCESS)
		{
			CurrentScore.lScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("CurrentScore"));
			CurrentScore.nLotteryCount = m_AccountsDBAide.GetValue_DWORD(TEXT("LotteryCount"));
			CurrentScore.cbCount = m_AccountsDBAide.GetValue_BYTE(TEXT("Charge1stCount"));
			CurrentScore.cbCanGet = m_AccountsDBAide.GetValue_BYTE(TEXT("Charge1stCanGet"));
			CurrentScore.cbVIPLevel = m_AccountsDBAide.GetValue_BYTE(TEXT("VIPLevel"));
			CurrentScore.dwVIPExp = m_AccountsDBAide.GetValue_DWORD(TEXT("VIPExp"));
			CurrentScore.dwExpPerCent = m_AccountsDBAide.GetValue_DWORD(TEXT("VIPExpPerCent"));
			CurrentScore.dwChargeBack = m_AccountsDBAide.GetValue_DWORD(TEXT("ChargeBack"));

			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CURRENT_SCORE,dwContextID,&CurrentScore,sizeof(CurrentScore));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		char szTemp[128] = {0};
		sprintf(szTemp, "%s @UserID=%u\n", "GSP_GP_CurrentScore", pCurrentScore->dwUserID);
		CTraceService::TraceString(szTemp,TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

//��¼����
bool CDataBaseEngineSink::OnRequestLogonGift(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		DBR_LogonGift * pLogonGift=(DBR_LogonGift *)pData;

		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@UserID"),pLogonGift->dwUserID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		LONG lReturnValue = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_LogonGift"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		DBO_LogonGift LogonGift = {0};

		if (lReturnValue == DB_SUCCESS)
		{
			LogonGift.lScore = m_TreasureDBAide.GetValue_LONGLONG(TEXT("CurrentScore"));
		}
		else
		{
			LogonGift.lScore = -1;
		}
		strncpy(LogonGift.szDescription,CW2CT(DBVarValue.bstrVal),CountArray(LogonGift.szDescription));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_GIFT,dwContextID,&LogonGift,sizeof(DBO_LogonGift));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CTraceService::TraceString(TEXT("GSP_GP_LogonGift"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

//�׳����
bool CDataBaseEngineSink::OnRequestCharge1stRequest(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		DBR_Charge1stRequest * pCharge1stRequest=(DBR_Charge1stRequest *)pData;

		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@UserID"),pCharge1stRequest->dwUserID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		LONG lReturnValue = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_Charge1stRequest"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		DBO_Charge1stRequest Charge1stRequest = {0};

		if (lReturnValue == DB_SUCCESS)
		{
			Charge1stRequest.cbCount = m_TreasureDBAide.GetValue_BYTE(TEXT("Charge1stCount"));
		}
		strncpy(Charge1stRequest.szDescription,CW2CT(DBVarValue.bstrVal),CountArray(Charge1stRequest.szDescription));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHARGE1ST_REQUEST,dwContextID,&Charge1stRequest,sizeof(DBO_Charge1stRequest));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CTraceService::TraceString(TEXT("GSP_GP_Charge1stRequest"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

//��ȡ�׳��������
bool CDataBaseEngineSink::OnRequestCharge1stGet(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		DBR_Charge1stGet * pCharge1stGet=(DBR_Charge1stGet *)pData;

		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@UserID"),pCharge1stGet->dwUserID);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		LONG lReturnValue = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_Charge1stGet"),true);

		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		DBO_Charge1stGet Charge1stGet = {0};

		if (lReturnValue == DB_SUCCESS)
		{
			Charge1stGet.cbSuccess = 1;
			Charge1stGet.cbCount = m_TreasureDBAide.GetValue_BYTE(TEXT("Charge1stCount"));
			Charge1stGet.nGiftScore = m_TreasureDBAide.GetValue_LONG(TEXT("GiftScore"));
			Charge1stGet.lCurrentScore = m_TreasureDBAide.GetValue_LONGLONG(TEXT("CurrentScore"));
		}
		strncpy(Charge1stGet.szDescription,CW2CT(DBVarValue.bstrVal),CountArray(Charge1stGet.szDescription));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHARGE1ST_GET,dwContextID,&Charge1stGet,sizeof(DBO_Charge1stGet));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CTraceService::TraceString(TEXT("GSP_GP_Charge1stGet"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestWealthBagAddScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	DBR_WealthBagAddScore * pWealthBagAddScore=(DBR_WealthBagAddScore *)pData;
	LONG lReturnValue = 0;
	DBO_WealthBagAddScore WealthBagAddScore = {0};

	try
	{
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pWealthBagAddScore->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@AddScore"),pWealthBagAddScore->nScore);
		lReturnValue = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_WealthBagAddScore"),true);

		if (lReturnValue == DB_SUCCESS)
		{
			WealthBagAddScore.lCurrentScore = m_AccountsDBAide.GetValue_LONGLONG(TEXT("CurrentScore"));
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_WEALTH_BAG_ADD_SCORE,dwContextID,&WealthBagAddScore,sizeof(WealthBagAddScore));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		char szTemp[128] = {0};
		sprintf(szTemp, "%s lReturnValue=%d @UserID=%u @AddScore=%d\n", "GSP_GP_WealthBagAddScore", lReturnValue, pWealthBagAddScore->dwUserID, pWealthBagAddScore->nScore);
		CTraceService::TraceString(szTemp,TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

//��������
bool CDataBaseEngineSink::OnRequestUserTransferDragon(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		if (wDataSize!=sizeof(DBR_GP_UserTransferDragon)) return false;

		//������
		DBR_GP_UserTransferDragon * pUserTransferScore=(DBR_GP_UserTransferDragon *)pData;

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

		OnDragonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_UserTransferDragon"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnDragonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//������
VOID CDataBaseEngineSink::OnDragonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
	DBO_GP_UserTransferDragon UserTransferDragon = {0};
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
	m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USER_TRANSER_DRAGON,dwContextID,&UserTransferDragon,wHeadSize+wDataSize);

	return;
}

//ʹ������
bool CDataBaseEngineSink::OnRequestUserUseDragon(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_UserUseDragon));
		if (wDataSize!=sizeof(DBR_GP_UserUseDragon)) return false;

		//������
		DBR_GP_UserUseDragon * pUserTakeScore=(DBR_GP_UserUseDragon *)pData;

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
		OnDragonDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_UserUseDragon"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnDragonDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//��ѯ��������
bool CDataBaseEngineSink::OnRequestQueryItemCount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	if (wDataSize!=sizeof(DBR_GP_QueryItemCount)) return false;
	DBR_GP_QueryItemCount * pQueryItemCount=(DBR_GP_QueryItemCount *)pData;

	try
	{
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pQueryItemCount->dwUserID);

		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_ItemQueryCount"),true);

		DBO_GP_QueryItemCount QueryItemCount = {0};

		if (lResultCode==DB_SUCCESS)
		{
			QueryItemCount.nChuJiCount=m_AccountsDBAide.GetValue_LONG(TEXT("nChuJiCount"));
			QueryItemCount.nZhongJiCount=m_AccountsDBAide.GetValue_LONG(TEXT("nZhongJiCount"));
			QueryItemCount.nGaoJiCount=m_AccountsDBAide.GetValue_LONG(TEXT("nGaoJiCount"));
			QueryItemCount.nZhiZunCount=m_AccountsDBAide.GetValue_LONG(TEXT("nTreasureMap"));
			QueryItemCount.nCannonStatus=m_AccountsDBAide.GetValue_LONG(TEXT("nCannonStatus"));
			QueryItemCount.nJiaSu=m_AccountsDBAide.GetValue_LONG(TEXT("nJiaSu"));
			QueryItemCount.nSanShe=m_AccountsDBAide.GetValue_LONG(TEXT("nSanShe"));
			QueryItemCount.nBaoJi=m_AccountsDBAide.GetValue_LONG(TEXT("nBaoJi"));
		}

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_ITEM_COUNT,dwContextID,&QueryItemCount,sizeof(QueryItemCount));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CString str = TEXT("[GSP_ItemQueryCount] ");
		str += pIException->GetExceptionDescribe();
		CTraceService::TraceString(str,TraceLevel_Exception);

		return false;
	}

	return true;
}

//���͵���
bool CDataBaseEngineSink::OnRequestTransferItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	if (wDataSize!=sizeof(DBR_GP_TransferItem)) return false;
	DBR_GP_TransferItem * pTransferItem=(DBR_GP_TransferItem *)pData;

	try
	{
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pTransferItem->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwTargetGameID"),pTransferItem->dwTargetGameID);
		m_AccountsDBAide.AddParameter(TEXT("@nItemID"),pTransferItem->nItemID);
		m_AccountsDBAide.AddParameter(TEXT("@nItemCount"),pTransferItem->nItemCount);
		m_AccountsDBAide.AddParameter(TEXT("@szPassword"),pTransferItem->szPassword);
		m_AccountsDBAide.AddParameter(TEXT("@szItemName"),pTransferItem->szItemName);

		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_ItemTransfer"),true);

		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		DBO_GP_TransferItem TransferItem = {0};
		TransferItem.nResultCode = lResultCode;
		lstrcpyn(TransferItem.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(TransferItem.szDescribeString));

		if (lResultCode==DB_SUCCESS)
		{
			TransferItem.nItemID=pTransferItem->nItemID;
			TransferItem.nSourceCount=m_AccountsDBAide.GetValue_LONG(TEXT("SourceCount"));
			TransferItem.nVariationCount=m_AccountsDBAide.GetValue_LONG(TEXT("VariationCount"));
		}

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TRANSFER_ITEM,dwContextID,&TransferItem,sizeof(TransferItem));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CString str = TEXT("[GSP_ItemTransfer] ");
		str += pIException->GetExceptionDescribe();
		CTraceService::TraceString(str,TraceLevel_Exception);

		return false;
	}

	return true;
}

//�ϳɵ���
bool CDataBaseEngineSink::OnRequestCombineItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	if (wDataSize!=sizeof(DBR_GP_CombineItem)) return false;
	DBR_GP_CombineItem * pCombineItem=(DBR_GP_CombineItem *)pData;

	try
	{
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pCombineItem->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@nCombineType"),pCombineItem->nCombineType);

		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_ItemCombine"),true);

		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		DBO_GP_CombineItem CombineItem = {0};
		CombineItem.nResultCode = lResultCode;
		lstrcpyn(CombineItem.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(CombineItem.szDescribeString));

		if (lResultCode==DB_SUCCESS)
		{
			CombineItem.nDragonPieceCount=m_AccountsDBAide.GetValue_LONG(TEXT("nDragonPieceCount"));
			CombineItem.nSmallDragonCount=m_AccountsDBAide.GetValue_LONG(TEXT("nSmallDragonCount"));
			CombineItem.nBigDragonCount=m_AccountsDBAide.GetValue_LONG(TEXT("nBigDragonCount"));
		}

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_COMBINE_DRAGON,dwContextID,&CombineItem,sizeof(CombineItem));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CString str = TEXT("[GSP_ItemCombine] ");
		str += pIException->GetExceptionDescribe();
		CTraceService::TraceString(str,TraceLevel_Exception);

		return false;
	}

	return true;
}

//ʹ������
bool CDataBaseEngineSink::OnRequestUseDragon(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	if (wDataSize!=sizeof(DBR_GP_UseDragon)) return false;
	DBR_GP_UseDragon * pUseDragon=(DBR_GP_UseDragon *)pData;

	try
	{
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pUseDragon->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@nDragonType"),pUseDragon->nDragonType);

		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_ItemUseDragon"),true);

		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		DBO_GP_UseDragon UseDragon = {0};
		UseDragon.nResultCode = lResultCode;
		lstrcpyn(UseDragon.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(UseDragon.szDescribeString));

		if (lResultCode==DB_SUCCESS)
		{
			UseDragon.lCurrentScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("lCurrentScore"));
			UseDragon.nChuJiCount=m_AccountsDBAide.GetValue_LONG(TEXT("nChuJiCount"));
			UseDragon.nZhongJiCount=m_AccountsDBAide.GetValue_LONG(TEXT("nZhongJiCount"));
			UseDragon.nGaoJiCount=m_AccountsDBAide.GetValue_LONG(TEXT("nGaoJiCount"));
			UseDragon.nZhiZunCount=m_AccountsDBAide.GetValue_LONG(TEXT("nZhiZunCount"));
		}

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_USE_DRAGON,dwContextID,&UseDragon,sizeof(UseDragon));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CString str = TEXT("[GSP_ItemUseDragon] ");
		str += pIException->GetExceptionDescribe();
		CTraceService::TraceString(str,TraceLevel_Exception);

		return false;
	}

	return true;
}

//װ������
bool CDataBaseEngineSink::OnRequestEquipmentDragon(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	if (wDataSize!=sizeof(DBR_GP_EquipmentDragon)) return false;
	DBR_GP_EquipmentDragon * pEquipmentDragon=(DBR_GP_EquipmentDragon *)pData;

	try
	{
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pEquipmentDragon->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@nDragonType"),pEquipmentDragon->nDragonType);

		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_ItemEquipmentDragon"),true);

		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		DBO_GP_EquipmentDragon EquipmentDragon = {0};
		EquipmentDragon.nResultCode = lResultCode;
		lstrcpyn(EquipmentDragon.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(EquipmentDragon.szDescribeString));

		if (lResultCode==DB_SUCCESS)
		{
			EquipmentDragon.nEquipmentDragonCount=m_AccountsDBAide.GetValue_LONG(TEXT("nEquipmentDragonCount"));
			EquipmentDragon.nChuJiCount=m_AccountsDBAide.GetValue_LONG(TEXT("nChuJiCount"));
			EquipmentDragon.nZhongJiCount=m_AccountsDBAide.GetValue_LONG(TEXT("nZhongJiCount"));
			EquipmentDragon.nGaoJiCount=m_AccountsDBAide.GetValue_LONG(TEXT("nGaoJiCount"));
			EquipmentDragon.nZhiZunCount=m_AccountsDBAide.GetValue_LONG(TEXT("nZhiZunCount"));
		}

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_EQUIPMENT_DRAGON,dwContextID,&EquipmentDragon,sizeof(EquipmentDragon));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CString str = TEXT("[GSP_ItemEquipmentDragon] ");
		str += pIException->GetExceptionDescribe();
		CTraceService::TraceString(str,TraceLevel_Exception);

		return false;
	}

	return true;
}

//װ������
bool CDataBaseEngineSink::OnRequestExchangeLottery(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	if (wDataSize!=sizeof(DBR_GP_ExchangeLottery)) return false;
	DBR_GP_ExchangeLottery * pEquipmentDragon=(DBR_GP_ExchangeLottery *)pData;

	try
	{
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pEquipmentDragon->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@nDragonType"),pEquipmentDragon->nDragonType);

		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ExchangeLottery"),true);

		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);

		DBO_GP_ExchangeLottery ExchangeLottery = {0};
		ExchangeLottery.nResultCode = lResultCode;
		lstrcpyn(ExchangeLottery.szDescribeString,CW2CT(DBVarValue.bstrVal),CountArray(ExchangeLottery.szDescribeString));

		if (lResultCode==DB_SUCCESS)
		{
			ExchangeLottery.nLotteryCount=m_AccountsDBAide.GetValue_LONG(TEXT("nLottery"));
			ExchangeLottery.nChuJiCount=m_AccountsDBAide.GetValue_LONG(TEXT("nChuJiCount"));
			ExchangeLottery.nZhongJiCount=m_AccountsDBAide.GetValue_LONG(TEXT("nZhongJiCount"));
			ExchangeLottery.nGaoJiCount=m_AccountsDBAide.GetValue_LONG(TEXT("nGaoJiCount"));
			ExchangeLottery.nZhiZunCount=m_AccountsDBAide.GetValue_LONG(TEXT("nZhiZunCount"));
		}

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_EXCHANGE_LOTTERY,dwContextID,&ExchangeLottery,sizeof(ExchangeLottery));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CString str = TEXT("[GSP_GP_ExchangeLottery]");
		str += pIException->GetExceptionDescribe();
		CTraceService::TraceString(str,TraceLevel_Exception);

		return false;
	}

	return true;
}

//��ѯ����
bool CDataBaseEngineSink::OnRequestQuerySkill(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	if (wDataSize!=sizeof(DBR_GP_QuerySkill)) return false;
	DBR_GP_QuerySkill * pQuerySkill=(DBR_GP_QuerySkill *)pData;

	try
	{
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),pQuerySkill->dwUserID);

		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_SkillQuery"),true);

		DBO_GP_QuerySkill QuerySkill = {0};

		if (lResultCode==DB_SUCCESS)
		{
			QuerySkill.nCannonStatus=m_AccountsDBAide.GetValue_LONG(TEXT("nCannonStatus"));
			QuerySkill.nJiaSu=m_AccountsDBAide.GetValue_LONG(TEXT("nJiaSu"));
			QuerySkill.nSanShe=m_AccountsDBAide.GetValue_LONG(TEXT("nSanShe"));
			QuerySkill.nBaoJi=m_AccountsDBAide.GetValue_LONG(TEXT("nBaoJi"));

			QuerySkill.cbCount[0]=QuerySkill.cbCount[1]=QuerySkill.cbCount[2]=9;//1ԪǮ��9������

			for (int i=0; i<7; i++)
			{
				QuerySkill.cbPrice[i]=18;//������̨����18Ԫ
			}
		}

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_SKILL,dwContextID,&QuerySkill,sizeof(QuerySkill));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CString str = TEXT("[GSP_SkillQuery] ");
		str += pIException->GetExceptionDescribe();
		CTraceService::TraceString(str,TraceLevel_Exception);

		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestMBVericationCode(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	/*CTraceService::TraceString(TEXT("sms: OnRequestMBVericationCode"),TraceLevel_Warning);*/

	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_MBVerifCode));
		if (wDataSize!=sizeof(DBR_GP_MBVerifCode)) return false;

		//������
		DBR_GP_MBVerifCode * pMBVerifiCode=(DBR_GP_MBVerifCode *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pMBVerifiCode->dwUserId);
		m_AccountsDBAide.AddParameter(TEXT("@strMBNum"),pMBVerifiCode->szMobilePhone);
		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_CheckVerifyMB"),true);
		printf("the result of the db %ld\n", lResultCode);

		DBO_GP_MB_Check_Can_Bind MBVerifCode;
		ZeroMemory(&MBVerifCode,sizeof(MBVerifCode));
		MBVerifCode.byResult = lResultCode;
		lstrcpyn(MBVerifCode.szMobilePhone,pMBVerifiCode->szMobilePhone,CountArray(MBVerifCode.szMobilePhone));
		lstrcpyn(MBVerifCode.szVerificationCode,pMBVerifiCode->szVerificationCode,CountArray(MBVerifCode.szVerificationCode));
		printf ("mobile phone is %s", pMBVerifiCode->szMobilePhone);

		//���ͽ��
		WORD wDataSize=sizeof(MBVerifCode);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VERIFY_MBNUM,dwContextID,&MBVerifCode,wDataSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GP_VerifyMBNum"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		DBO_GP_MB_Check_Can_Bind MBVerifCode;
		ZeroMemory(&MBVerifCode,sizeof(MBVerifCode));

		//�������
		MBVerifCode.byResult = 1;

		//���ͽ��
		WORD wDataSize=sizeof(MBVerifCode);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VERIFY_MBNUM,dwContextID,&MBVerifCode,wDataSize);;
		return false;
	}

	return true;
}

//��֤�ֻ��Ž��
VOID CDataBaseEngineSink::OnVerifyMBNumDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
	CTraceService::TraceString(TEXT("sms: OnVerifyMBNumDisposeResult"),TraceLevel_Warning);

	DBO_GP_MBVerifCode MBVerifCode;
	ZeroMemory(&MBVerifCode,sizeof(MBVerifCode));

	//�������
	if (dwErrorCode == DB_SUCCESS)
	{
		MBVerifCode.bIsMBNumExist = false;
	}
	else
	{
		MBVerifCode.bIsMBNumExist = true;
	}

	//lstrcpyn(MBVerifCode.szDescribeString,pszErrorString,CountArray(MBVerifCode.szDescribeString));

	////���ͽ��
	//WORD wDataSize=CountStringBuffer(MBVerifCode.szDescribeString);
	//WORD wHeadSize=sizeof(MBVerifCode)-sizeof(MBVerifCode.szDescribeString);
	//m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_VERIFY_MBNUM,dwContextID,&MBVerifCode,wHeadSize+wDataSize);

	return;
}

//�ֻ���ע������
bool CDataBaseEngineSink::OnRequestMBRegister(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	LONG lResultCode;
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_RegisterAccounts));
		if (wDataSize!=sizeof(DBR_GP_RegisterAccounts)) return false;

		//������
		DBR_GP_RegisterAccounts * pRegisterAccounts=(DBR_GP_RegisterAccounts *)pData;

		//ִ���ж�
		tagBindParameter * pBindParameter=(tagBindParameter *)pRegisterAccounts->pBindParameter;
		if (pBindParameter->dwSocketID!=dwContextID) return true;

		//ת����ַ
		TCHAR szClientAddr[16]=TEXT("");
		BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
		_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),pRegisterAccounts->szSpreader);
		m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
		m_AccountsDBAide.AddParameter(TEXT("@strInsurePass"),pRegisterAccounts->szInsurePass);
		m_AccountsDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
		m_AccountsDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
		m_AccountsDBAide.AddParameter(TEXT("@strPassPortID"),pRegisterAccounts->szPassPortID);
		m_AccountsDBAide.AddParameter(TEXT("@strCompellation"),pRegisterAccounts->szCompellation);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pRegisterAccounts->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
		m_AccountsDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);
		m_AccountsDBAide.AddParameter(TEXT("@bIsGuestReg"),0);
		m_AccountsDBAide.AddParameter(TEXT("@AgentID"),pRegisterAccounts->dwAgentID);
		m_AccountsDBAide.AddParameter(TEXT("@bIsMobileReg"),1);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_MBRegisterAccounts"),true);

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnMBRegisterDisposeResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GP_MBRegisterAccounts"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnMBRegisterDisposeResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),false);

		return false;
	}

	//��־������
	if (lResultCode == DB_SUCCESS)
	{
		DWORD dwGameID = m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
		DWORD dwUserID = m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
		try
		{
			//������
			DBR_GP_RegisterAccounts * pRegisterAccounts=(DBR_GP_RegisterAccounts *)pData;

			//ת����ַ
			TCHAR szClientAddr[16]=TEXT("");
			BYTE * pClientAddr=(BYTE *)&pRegisterAccounts->dwClientAddr;
			_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

			//�������
			m_LogServerDBAide.ResetParameter();
			m_LogServerDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
			m_LogServerDBAide.AddParameter(TEXT("@dwGameID"),dwGameID);
			m_LogServerDBAide.AddParameter(TEXT("@strAccounts"),pRegisterAccounts->szAccounts);
			m_LogServerDBAide.AddParameter(TEXT("@strNickName"),pRegisterAccounts->szNickName);
			m_LogServerDBAide.AddParameter(TEXT("@strSpreader"),pRegisterAccounts->szSpreader);
			m_LogServerDBAide.AddParameter(TEXT("@strLogonPass"),pRegisterAccounts->szLogonPass);
			m_LogServerDBAide.AddParameter(TEXT("@strInsurePass"),pRegisterAccounts->szInsurePass);
			m_LogServerDBAide.AddParameter(TEXT("@wFaceID"),pRegisterAccounts->wFaceID);
			m_LogServerDBAide.AddParameter(TEXT("@cbGender"),pRegisterAccounts->cbGender);
			m_LogServerDBAide.AddParameter(TEXT("@strPassPortID"),pRegisterAccounts->szPassPortID);
			m_LogServerDBAide.AddParameter(TEXT("@strCompellation"),pRegisterAccounts->szCompellation);
			m_LogServerDBAide.AddParameter(TEXT("@strMobilePhone"),pRegisterAccounts->szMobilePhone);
			m_LogServerDBAide.AddParameter(TEXT("@strClientIP"),szClientAddr);
			m_LogServerDBAide.AddParameter(TEXT("@strMachineID"),pRegisterAccounts->szMachineID);
			m_LogServerDBAide.AddParameter(TEXT("@bIsGuestReg"),0);
			m_LogServerDBAide.AddParameter(TEXT("@AgentID"),pRegisterAccounts->dwAgentID);
			m_LogServerDBAide.AddParameter(TEXT("@bIsMobileReg"),1);

			//�������
			TCHAR szDescribeString[128]=TEXT("");
			m_LogServerDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

			//ִ�в�ѯ
			LONG lResultCode=m_LogServerDBAide.ExecuteProcess(TEXT("Log_GSP_GP_MBRegisterAccounts"),true);

		}
		catch (IDataBaseException * pIException)
		{
			//������Ϣ
			CTraceService::TraceString(TEXT("Log_GSP_GP_MBRegisterAccounts"),TraceLevel_Exception);
			CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		}
	}

	return true;
}

//��¼�ɹ�
VOID CDataBaseEngineSink::OnMBRegisterDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{
	if (bMobileClient==false)
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//��������
			DBO_GP_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//��������
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwCustomID=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomID"));
			LogonSuccess.dwUserMedal=m_AccountsDBAide.GetValue_DWORD(TEXT("UserMedal"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));

			//�û��ɼ�
			LogonSuccess.lUserScore=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Score"));
			LogonSuccess.lUserInsure=m_AccountsDBAide.GetValue_LONGLONG(TEXT("Insure"));

			//�û�����
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.cbMoorMachine=m_AccountsDBAide.GetValue_BYTE(TEXT("MoorMachine"));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));

			//��Ա����
			LogonSuccess.cbMemberOrder=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			m_AccountsDBAide.GetValue_SystemTime(TEXT("MemberOverDate"),LogonSuccess.MemberOverDate);

			LogonSuccess.nTotalCharge = m_AccountsDBAide.GetValue_DWORD("totalCharge");
			LogonSuccess.nTotalPlayTime = m_AccountsDBAide.GetValue_DWORD("totalPlayTime");

			LogonSuccess.cbVIPLevel = m_AccountsDBAide.GetValue_BYTE("vipLevel");
			LogonSuccess.dwVIPExp = m_AccountsDBAide.GetValue_DWORD("vipExp");
			LogonSuccess.dwExpPerCent = m_AccountsDBAide.GetValue_DWORD("vipExpPerCent");
			m_AccountsDBAide.GetValue_String(TEXT("customerServiceQQ"), LogonSuccess.szCustomerServiceQQ, CountArray(LogonSuccess.szCustomerServiceQQ));
			LogonSuccess.dwLotteryCount = m_AccountsDBAide.GetValue_DWORD("lotteryCount");
			LogonSuccess.nLogonGift = m_AccountsDBAide.GetValue_LONG("logonGift");
			LogonSuccess.cbCharge1stCount = m_AccountsDBAide.GetValue_BYTE("Charge1stCount");
			LogonSuccess.cbCharge1stCanGet = m_AccountsDBAide.GetValue_BYTE("Charge1stCanGet");
			LogonSuccess.nCanShare = m_AccountsDBAide.GetValue_LONG("CanShare");
			LogonSuccess.cbCanGetBankruptcy = m_AccountsDBAide.GetValue_BYTE("CanGetBankruptcy");
			LogonSuccess.nBankruptcyCount = m_AccountsDBAide.GetValue_LONG("BankruptcyCount");

			//��ȡ��Ϣ
			lstrcpyn(LogonSuccess.szDescribeString,pszErrorString,CountArray(LogonSuccess.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);

			//ɾ��m_mMBVerifInfoע��ɹ����û�
			DBO_GP_MBRegisterRes MobileRegisterRes;
			ZeroMemory(&MobileRegisterRes,sizeof(MobileRegisterRes));

			// ��ȡ�ֻ���
			m_AccountsDBAide.GetValue_String(TEXT("MobilePhone"),MobileRegisterRes.szMobilePhone,CountArray(MobileRegisterRes.szMobilePhone));
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_MB_REGITER_SUCCESS,dwContextID,&MobileRegisterRes,sizeof(MobileRegisterRes));
		}
		else
		{
			//��������
			DBO_GP_LogonFailure LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//��������
			LogonFailure.lResultCode=dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}
	else
	{
		if (dwErrorCode==DB_SUCCESS)
		{
			//��������
			DBO_MB_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//��������
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwLoveLiness=m_AccountsDBAide.GetValue_DWORD(TEXT("LoveLiness"));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));

			//��ȡ��Ϣ
			lstrcpyn(LogonSuccess.szDescribeString,pszErrorString,CountArray(LogonSuccess.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonSuccess.szDescribeString);
			WORD wHeadSize=sizeof(LogonSuccess)-sizeof(LogonSuccess.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_SUCCESS,dwContextID,&LogonSuccess,wHeadSize+wDataSize);
		}
		else
		{
			//��������
			DBO_MB_LogonFailure LogonFailure;
			ZeroMemory(&LogonFailure,sizeof(LogonFailure));

			//��������
			LogonFailure.lResultCode=dwErrorCode;
			lstrcpyn(LogonFailure.szDescribeString,pszErrorString,CountArray(LogonFailure.szDescribeString));

			//���ͽ��
			WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_MB_LOGON_FAILURE,dwContextID,&LogonFailure,wHeadSize+wDataSize);
		}
	}

	return;
}

CString GetFileDialogPath2()
{
	CString strFileDlgPath;
	TCHAR szModuleDirectory[MAX_PATH];	//ģ��Ŀ¼
	GetModuleFileName(AfxGetInstanceHandle(),szModuleDirectory,sizeof(szModuleDirectory));
	int nModuleLen=lstrlen(szModuleDirectory);
	int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".EXE")) + 1;
	if (nModuleLen<=nProcessLen) 
		return TEXT("");
	szModuleDirectory[nModuleLen-nProcessLen]=0;
	strFileDlgPath = szModuleDirectory;
	return strFileDlgPath;
}

//��ѯ��������
bool CDataBaseEngineSink::OnRequestQueryNewGuid(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	if (wDataSize!=sizeof(DBR_GP_QueryNewGuid)) return false;
	DBR_GP_QueryNewGuid * pQueryNewGuid=(DBR_GP_QueryNewGuid *)pData;

	try
	{
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@nUserID"),pQueryNewGuid->dwUserID);

		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_NewGuidQuery"),true);

		DBO_GP_QueryNewGuid QueryNewGuid = {0};

		if (lResultCode==DB_SUCCESS)
		{
			QueryNewGuid.bCan = true;
		}
		else
		{
			QueryNewGuid.bCan = false;
		}

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_NEW_GUID,dwContextID,&QueryNewGuid,sizeof(QueryNewGuid));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CString str = TEXT("[GSP_GR_NewGuidQuery] ");
		str += pIException->GetExceptionDescribe();
		CTraceService::TraceString(str,TraceLevel_Exception);

		return false;
	}

	return true;
}

//��ѯС����
bool CDataBaseEngineSink::OnRequestQuerySuperGold(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	if (wDataSize!=sizeof(DBR_GP_SuperGold)) return false;
	DBR_GP_SuperGold * pSuperGold=(DBR_GP_SuperGold *)pData;

	try
	{
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@nUserID"),pSuperGold->dwUserID);

		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_NewGuidQuery2"),true);

		DBO_GP_SuperGold SuperGold = {0};

		if (lResultCode==DB_SUCCESS)
		{
			SuperGold.bCan = (BYTE)m_AccountsDBAide.GetValue_LONG(TEXT("nFlag"));
			SuperGold.nSuperGold = m_AccountsDBAide.GetValue_LONG(TEXT("SuperGold"));
		}

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SUPER_GOLD,dwContextID,&SuperGold,sizeof(SuperGold));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CString str = TEXT("[GSP_GR_NewGuidQuery] ");
		str += pIException->GetExceptionDescribe();
		CTraceService::TraceString(str,TraceLevel_Exception);

		return false;
	}

	return true;
}

//�ֻ��Ű�
bool CDataBaseEngineSink::OnRequestMBBind(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_MBBindNum));
		if (wDataSize!=sizeof(DBR_GP_MBBindNum)) return false;

		//������
		DBR_GP_MBBindNum * pUserBindNum=(DBR_GP_MBBindNum *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pUserBindNum->dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strNickName"),pUserBindNum->szNickName);
		m_AccountsDBAide.AddParameter(TEXT("@strMobilePhone"),pUserBindNum->szMobilePhone);
		m_AccountsDBAide.AddParameter(TEXT("@PassWord"),pUserBindNum->szLogonPass);

		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_AccountsDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UpdateMobilePhone"),true);

		//��Ҵ���

		//�������
		CDBVarValue DBVarValue;
		m_AccountsDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		OnRequestMBBindResult(dwContextID,lResultCode,CW2CT(DBVarValue.bstrVal),false);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GP_UpdateMobilePhone"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		//������
		OnRequestMBBindResult(dwContextID,DB_ERROR,TEXT("�������ݿ�����쳣�������Ժ����ԣ�"),false);

		return false;
	}

	return true;
}

//�ֻ��Ű���֤���
VOID CDataBaseEngineSink::OnRequestMBBindResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient)
{

		//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	DBO_GP_MBBindRes * pMBBindRes=(DBO_GP_MBBindRes *)cbDataBuffer;
	if (dwErrorCode==0)
	{
		pMBBindRes->byResult = 1;
	}
	else
	{
		pMBBindRes->byResult = 0;
	}
	pMBBindRes->nCurrentGold=m_AccountsDBAide.GetValue_LONGLONG(TEXT("CurrentScore"));
	lstrcpyn(pMBBindRes->szDescribeString,pszErrorString,CountArray(pMBBindRes->szDescribeString));
	TCHAR szNickName[LEN_NICKNAME];
	m_AccountsDBAide.GetValue_String(TEXT("NickName"), szNickName, CountArray(szNickName));
	//���Ͷ���
	WORD wHeadSize=sizeof(DBO_GP_MBBindRes);
	CSendPacketHelper SendPacket(cbDataBuffer+wHeadSize,sizeof(cbDataBuffer)-wHeadSize);
	SendPacket.AddPacket(szNickName,CountStringBuffer(szNickName),DTP_GP_UI_NICKNAME);
	WORD wSendSize=SendPacket.GetDataSize()+sizeof(DBO_GP_MBBindRes);
	//���ͽ��
	m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_MB_BIND_NUM_RES,dwContextID,cbDataBuffer,wSendSize);

	return;
}

//�ֻ����Ƿ�󶨹�
bool CDataBaseEngineSink::OnRequestMBBindNumExit(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_CHECK_BINDNUM_EXIT));
		if (wDataSize!=sizeof(DBR_GP_CHECK_BINDNUM_EXIT)) return false;

		//������
		DBR_GP_CHECK_BINDNUM_EXIT * pUserBindNum=(DBR_GP_CHECK_BINDNUM_EXIT *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pUserBindNum->dwUserID);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GETMobilePhone"),true);
		DBO_GP_CHECK_BINDNUM_EXIT_RES sIsExit_Res;
		if (lResultCode>0)
		{
			sIsExit_Res.byIsExit = 0;
		}
		else
		{
			sIsExit_Res.byIsExit = 1;
		}
		//sIsExit_Res.nBindReward=m_AccountsDBAide.GetValue_INT(TEXT("BindReward"));
		//sIsExit_Res.nShareReward=m_AccountsDBAide.GetValue_INT(TEXT("ShareReward"));
		//�������
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_MB_CHECK_NUM_EXIT,dwContextID,&sIsExit_Res,sizeof(sIsExit_Res));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GP_GETMobilePhone"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestBroadCastSpeed(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		
		//ִ�в�ѯ
		//�������
		m_AccountsDBAide.ResetParameter();
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GetBroadCast_Speed"),true);
		DBO_GP_BROADCAST_SPEED_RES s_Res;
		float fSpeed=m_AccountsDBAide.GetValue_FLOAT(TEXT("Speed"));
		s_Res.fBroadCastSpeed = fSpeed;
		//�������
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BROADCAST_SPEED,dwContextID,&s_Res,sizeof(DBO_GP_BROADCAST_SPEED_RES));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GP_GetBroadCast_Speed"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}


bool CDataBaseEngineSink::OnRequestCompRankInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{

		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_REQUEST_COMPRANK_INFO));
		if (wDataSize!=sizeof(DBR_GP_REQUEST_COMPRANK_INFO)) return false;
		WORD wPacketSize = 0;
		BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];
		//������
		DBR_GP_REQUEST_COMPRANK_INFO * pRequstInfo=(DBR_GP_REQUEST_COMPRANK_INFO *)pData;
		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@timeinfo"),pRequstInfo->nType);
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_GetCompRankInfo"),true);
		DB0_GP_COMPRANK_INFO_RES * pCompRankInfo=NULL;
		int nRow = 0;
		while (m_TreasureDBModule->IsRecordsetEnd()==false)
		{
			//��ȡ��Ϣ
			nRow++;
			pCompRankInfo=(DB0_GP_COMPRANK_INFO_RES *)(cbBuffer+wPacketSize);
			pCompRankInfo->nRank=nRow;
			pCompRankInfo->nUserId=m_TreasureDBAide.GetValue_UINT(TEXT("UserId"));
			pCompRankInfo->nCount=m_TreasureDBAide.GetValue_UINT(TEXT("Count"));
			wPacketSize = wPacketSize + sizeof(DB0_GP_COMPRANK_INFO_RES);
			//�ƶ���¼
			m_TreasureDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_COMPRANK_INFO,dwContextID,cbBuffer,wPacketSize);
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GP_GetCompRankInfo"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestBroadCastLaBa(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{

		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_REQUEST_BROADLABA));
		if (wDataSize!=sizeof(DBR_GP_REQUEST_BROADLABA)) return false;
		BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];
		//������
		DBR_GP_REQUEST_BROADLABA * pRequstInfo=(DBR_GP_REQUEST_BROADLABA *)pData;
		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@UserID"),pRequstInfo->dwUserId);
		//�������
		TCHAR szDescribeString[128]=TEXT("");
		m_TreasureDBAide.AddParameterOutput(TEXT("@strErrorDescribe"),szDescribeString,sizeof(szDescribeString),adParamOutput);

		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_BroadCastLaBa"),true);
		//�������
		CDBVarValue DBVarValue;
		m_TreasureDBModule->GetParameter(TEXT("@strErrorDescribe"),DBVarValue);
		DB0_GP_BROADLABA_RES s_Res;
		if (lResultCode==1)
		{
			lstrcpyn(s_Res.szErrorDes,CW2CT(DBVarValue.bstrVal),sizeof(s_Res.szErrorDes));
		}
		else
		{
			s_Res.score = m_TreasureDBAide.GetValue_LONGLONG(TEXT("Score"));
			lstrcpyn(s_Res.szErrorDes,CW2CT(DBVarValue.bstrVal),sizeof(s_Res.szErrorDes));
			lstrcpyn(s_Res.szContent,pRequstInfo->szContent,sizeof(pRequstInfo->szContent));
			lstrcpyn(s_Res.szNickName,pRequstInfo->szNickName,sizeof(s_Res.szNickName));
			s_Res.wSize = pRequstInfo->wSize;
		}
		s_Res.lResult = lResultCode;
		
		//�������
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_BROADLABA,dwContextID,&s_Res,sizeof(DB0_GP_BROADLABA_RES));
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GP_BroadCastLaBa"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestTypeItemCount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{

		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_REQUEST_TYPE_ITEM_COUNT));
		if (wDataSize!=sizeof(DBR_GP_REQUEST_TYPE_ITEM_COUNT)) return false;
		WORD wPacketSize = 0;
		//������
		DBR_GP_REQUEST_TYPE_ITEM_COUNT * pRequstInfo=(DBR_GP_REQUEST_TYPE_ITEM_COUNT *)pData;
		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@UserID"),pRequstInfo->dwUserId);
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_GetTypeItemCount"),true);
		DB0_GP_TYPE_ITEM_COUNT_RES  ResInfo={0};
		ResInfo.nCount=m_TreasureDBAide.GetValue_LONG(TEXT("nCount"));
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_TYPE_ITEM_COUNT,dwContextID,&ResInfo,sizeof(DB0_GP_TYPE_ITEM_COUNT_RES));
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GP_GetTypeItemCount"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestChangePhoneFare(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{

		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_REQUESTCHANGE_PHONE_FARE));
		if (wDataSize!=sizeof(DBR_GP_REQUESTCHANGE_PHONE_FARE)) return false;
		WORD wPacketSize = 0;
		//������
		DBR_GP_REQUESTCHANGE_PHONE_FARE * pRequstInfo=(DBR_GP_REQUESTCHANGE_PHONE_FARE *)pData;
		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@UserID"),pRequstInfo->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@nChangeNum"),pRequstInfo->nMobileCharge);
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_ChangePhoneFare"),true);
		DB0_GP_CHANGE_PHONE_FARE_RES  ResInfo={0};
		if (lResultCode>0)
		{
			ResInfo.bSuccess = false;
			ResInfo.nCurrentMobileCharge = 0;
		}
		else
		{
			ResInfo.bSuccess = true;
			ResInfo.nCurrentMobileCharge = m_TreasureDBAide.GetValue_LONG(TEXT("nCount"));
			m_TreasureDBAide.GetValue_String(TEXT("NickName"), ResInfo.szNickName, CountArray(ResInfo.szNickName));
			ResInfo.nChangeNum = pRequstInfo->nMobileCharge;
		}
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_CHANGE_PHONE_FARE,dwContextID,&ResInfo,sizeof(DB0_GP_CHANGE_PHONE_FARE_RES));
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GP_ChangePhoneFare"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestSuperRankInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{

		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_REQUEST_SUPERRANK_INFO));
		if (wDataSize!=sizeof(DBR_GP_REQUEST_SUPERRANK_INFO)) return false;
		WORD wPacketSize = 0;
		BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];
		//������
		DBR_GP_REQUEST_SUPERRANK_INFO * pRequstInfo=(DBR_GP_REQUEST_SUPERRANK_INFO *)pData;
		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@type"),pRequstInfo->nType);
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_GetSuperRankInfo"),true);
		DB0_GP_SUPERRANK_INFO_RES * pRankInfo=NULL;
		int nRow = 0;
		while (m_TreasureDBModule->IsRecordsetEnd()==false)
		{
			//��ȡ��Ϣ
			nRow++;
			pRankInfo=(DB0_GP_SUPERRANK_INFO_RES *)(cbBuffer+wPacketSize);
			pRankInfo->nRank=nRow;
			m_TreasureDBAide.GetValue_String(TEXT("NickName"), pRankInfo->szNickName, CountArray(pRankInfo->szNickName));
			pRankInfo->lCount=m_TreasureDBAide.GetValue_UINT(TEXT("count"));
			wPacketSize = wPacketSize + sizeof(DB0_GP_SUPERRANK_INFO_RES);
			//�ƶ���¼
			m_TreasureDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_SUPERRANK_INFO,dwContextID,cbBuffer,wPacketSize);
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GP_GetSuperRankInfo"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestBindRewardNewGuid(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_GET_BINDREWARD));
		if (wDataSize!=sizeof(DBR_GP_GET_BINDREWARD)) return false;

		//������
		DBR_GP_GET_BINDREWARD * pRequest=(DBR_GP_GET_BINDREWARD *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pRequest->dwUserID);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GETMobilePhone"),true);
		DBO_GP_GET_BINDREWARD_RES s_Res;
		s_Res.nBindReward=m_AccountsDBAide.GetValue_LONG(TEXT("BindReward"));
		s_Res.nShareReward=m_AccountsDBAide.GetValue_LONG(TEXT("ShareReward"));
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@nUserID"),pRequest->dwUserID);

		//ִ�в�ѯ
		LONG lResult=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_SuperNewGuidQuery"),true);
		if (lResult == 0)
		{
			s_Res.byNewGuid = 0;
		}
		else
		{
			s_Res.byNewGuid = 1;
		}
		printf("�Ƿ���й���������=%d\n",s_Res.byNewGuid);
		//�������
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GETBINDREWARDNEWGUID,dwContextID,&s_Res,sizeof(s_Res));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GP_GETMobilePhone"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		CTraceService::TraceString(TEXT("GSP_GR_SuperNewGuidQuery"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestSetSuperNewGuid(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_Set_SuperNewGuid));
		if (wDataSize!=sizeof(DBR_GP_Set_SuperNewGuid)) return false;

		//������
		DBR_GP_Set_SuperNewGuid * pRequest=(DBR_GP_Set_SuperNewGuid *)pData;

		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pRequest->dwUserID);

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_SetSuperNewGuid"),true);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_SetSuperNewGuid"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestGetNewMailRemind(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//�������
		m_AccountsDBAide.ResetParameter();

		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_GetNewMailRemind"),true);
		WORD wPacketSize = 0;
		BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];
		DBO_GP_NewMail_Remind_Res * pRemindInfo=NULL;
		int nRow = 0;
		while (m_AccountsDBModule->IsRecordsetEnd()==false)
		{
			//��ȡ��Ϣ
			nRow++;
			pRemindInfo=(DBO_GP_NewMail_Remind_Res *)(cbBuffer+wPacketSize);
			pRemindInfo->dwUserID=m_AccountsDBAide.GetValue_LONG(TEXT("UserId"));
			pRemindInfo->dwMailID=m_AccountsDBAide.GetValue_LONG(TEXT("MailID"));
			wPacketSize = wPacketSize + sizeof(DBO_GP_NewMail_Remind_Res);
			//�ƶ���¼
			m_AccountsDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GET_NEWMAIL_REMIND,dwContextID,cbBuffer,wPacketSize);
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_GetNewMailRemind"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestQueryFreeLabaCount(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		ASSERT(wDataSize==sizeof(DBR_GP_Free_LaBa_Count));
		if (wDataSize!=sizeof(DBR_GP_Free_LaBa_Count)) return false;

		//������
		DBR_GP_Free_LaBa_Count * pRequest=(DBR_GP_Free_LaBa_Count *)pData;
		//�������
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),pRequest->dwUserID);
		//ִ�в�ѯ
		LONG lResultCode=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_QueryFreeLabaCount"),true);
		DBO_GP_Free_LaBa_Count_Res s_Res;
		int nCount=m_AccountsDBAide.GetValue_LONG(TEXT("nCount"));
		s_Res.nCount = nCount;
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_QUERY_FREE_LABA_COUNT,dwContextID,&s_Res,sizeof(DBO_GP_Free_LaBa_Count_Res));
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GR_QueryFreeLabaCount"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestGetMatchPirze(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	if (wDataSize!=sizeof(DBR_GR_Match_Get_Prize)) return false;

	//��������
	DBO_GR_Match_Get_Prize stMatchGetPrize;
	ZeroMemory(&stMatchGetPrize,sizeof(stMatchGetPrize));

	//������Ϣ
	DBR_GR_Match_Get_Prize * pGetMatchPrize = (DBR_GR_Match_Get_Prize *)pData;
	try
	{

		//ִ�в�ѯ
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@UserID"), pGetMatchPrize->dwUserID);
		m_TreasureDBAide.AddParameter(TEXT("@MatchNum"), pGetMatchPrize->nMatchID);
		m_TreasureDBAide.AddParameter(TEXT("@MatchDay"), pGetMatchPrize->szMatchData);
		//ִ�в�ѯ
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_Get_Match_Prize"),true);

		DBO_GR_Match_Get_Prize * pMatchGetPrize=NULL;
		//�������
		if(lResultCode==0)
		{
			//��ȡ����
			pMatchGetPrize->bPriseStatus = true;
		}
		else
		{
			pMatchGetPrize->bPriseStatus = false;
		}
		pMatchGetPrize->nPriseCount=m_TreasureDBAide.GetValue_LONG(TEXT("RewardGold"));
		pMatchGetPrize->nPriseType=m_TreasureDBAide.GetValue_LONG(TEXT("MatchType"));
		pMatchGetPrize->lluserScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("Score"));
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_MATCH_GET_PRIZE,dwContextID,pMatchGetPrize,sizeof(DBO_GR_Match_Get_Prize));
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GP_Get_Match_Prize"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////////////
