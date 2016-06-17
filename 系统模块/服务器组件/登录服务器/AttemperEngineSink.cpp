#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"
#include "SensitiveWords.h"
#include <vector>
#include "CVerification.h"
//////////////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_LOAD_GAME_LIST			1									//�����б�
#define IDI_CONNECT_CORRESPOND		2									//������ʶ
#define IDI_COLLECT_ONLINE_INFO		3									//ͳ������
#define IDI_MAIL_CHECK				4
#define IDI_WEALTH_BAG_TIME			5
#define IDI_WEALTH_BAG_BEGIN		6
#define IDI_WEALTH_BAG_END			7
#define IDI_WEALTH_BAG_RESULT		8
#define IDI_WEALTH_BAG_WILL			9									//����С��Ϸ������ʼ
#define IDI_CHECK_TIMEOUT_MBNUM		10									//�����֤�볬ʱ�ֻ���, δ��ʱ�ֻ��Ų����ٴη�����֤�� 
#define IDI_GET_COMPRANK_INFO		11									//��ȡ��������
#define IDI_SEND_LOGIN_QUEUEING		12									//�����Ŷ���Ϣ
#define IDI_GET_SUPERRANK_INFO		13									//��ȡ������������
#define TIME_WEALTH_BAG_BEFORE		60									//���ж೤ʱ��ſ�ʼ
#define IDI_GET_NEW_MAIL_REMIND		61									//��ȡ���ʼ�����
#define IDI_QUERY_FAKE_SERVER_INFO  62									//Get Fake Server Info

//////////////////////////////////////////////////////////////////////////////////

bool compare1(tagWealthBag & obj1, tagWealthBag & obj2)
{
	if (obj1.nScore > obj2.nScore)
	{
		return true;
	}
	return false;
}

//���캯��
CAttemperEngineSink::CAttemperEngineSink()
{
	//״̬����
	m_bNeekCorrespond=true;
	m_bShowServerStatus=false;

	//״̬����
	m_pInitParameter=NULL;
	m_pBindParameter=NULL;

	//�������
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;

	//��Ƶ����
	m_wAVServerPort=0;
	m_dwAVServerAddr=0;

	//������֤��
	m_SMSVerifyAccount="aaf98f894de79698014df4e7b41805ab";
	m_SMSVerifyAccountToken="741ea43bb75a40e6b45efdc351aea3bb";
	m_SMSVerifyAppID="8a48b551516c09cd01518574865b2fc1";
	m_SMSVerifyRestIP="app.cloopen.com"; 
	m_SMSVerifyTemplateID="68700";
	m_SMSVerifyPort=8883;
	setLogLevel(LOG_PACKAGE);
	init(m_SMSVerifyRestIP, m_SMSVerifyPort);
	setAccount(m_SMSVerifyAccount,m_SMSVerifyAccountToken);
	setAppId(m_SMSVerifyAppID);
	m_mMBVerifInfo.clear();
	m_mRegisteredMbNum.clear();
	m_CompRankInfo.clear();
	m_LoginQueueInfo.clear();
	m_nSendMaxNum = 0;
	m_SuperRankInfo.clear();

	// fake server info
	m_wSendFakeInfoInterval = 30000;

	ZeroMemory(m_NormalUserRewardConfig, sizeof(tagSignInDayInfo) * SIGNIN_REWARD_ARRAY_LENGTH);
	ZeroMemory(m_VIPUserRewardConfig, sizeof(tagSignInDayInfo) * SIGNIN_REWARD_ARRAY_LENGTH);

	return;
}

//��������
CAttemperEngineSink::~CAttemperEngineSink()
{
	//���ʼ�� sms sdk
	unInit();
}

//�ӿڲ�ѯ
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//�󶨲���
	m_pBindParameter=new tagBindParameter[m_pInitParameter->m_wMaxConnect];
	ZeroMemory(m_pBindParameter,sizeof(tagBindParameter)*m_pInitParameter->m_wMaxConnect);

	//����ʱ��
	ASSERT(m_pITimerEngine!=NULL);
	m_pITimerEngine->SetTimer(IDI_COLLECT_ONLINE_INFO,m_pInitParameter->m_wCollectTime*1000L,TIMES_INFINITY,0);

	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	CString szFileName;
	GetModuleFileName(AfxGetInstanceHandle(),szPath,sizeof(szPath));
	szFileName=szPath;
	int nIndex = szFileName.ReverseFind(TEXT('\\'));
	szFileName = szFileName.Left(nIndex);
	szFileName += TEXT("\\PlazaOptionConfig.ini");

	//��ȡ����
	m_bShowServerStatus = (GetPrivateProfileInt(TEXT("ServerStatus"),TEXT("ShowServerStatus"),0,szFileName) != 0);

	//��ȡĿ¼
	TCHAR szServerAddr[LEN_SERVER]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);

	//��ȡ����
	TCHAR szVideoFileName[MAX_PATH]=TEXT("");
	_sntprintf(szVideoFileName,CountArray(szVideoFileName),TEXT("%s\\VideoOption.ini"),szPath);
	m_wAVServerPort=GetPrivateProfileInt(TEXT("VideoOption"),TEXT("ServerPort"), 0,szVideoFileName);
	DWORD dwAddrLen=GetPrivateProfileString(TEXT("VideoOption"),TEXT("ServerAddr"), TEXT(""), szServerAddr,LEN_SERVER,szVideoFileName);
	if(dwAddrLen>0)
	{
		CT2CA strServerDomain(szServerAddr);
		m_dwAVServerAddr=inet_addr(strServerDomain);
	}
	else
	{
		m_dwAVServerAddr=0;
	}

	////@@@����С��Ϸ
	//m_pITimerEngine->SetTimer(IDI_WEALTH_BAG_WILL, 20 * 1000, -1, 0);
	m_bWealthBagRunning = false;

	//�ֻ���֤��
	m_pITimerEngine->SetTimer(IDI_CHECK_TIMEOUT_MBNUM, 60000L, -1, 0);
	m_bWealthBagRunning = false;
	
	m_mMBVerifInfo.clear();
	m_mRegisteredMbNum.clear();
	CVerification * pVerification = CVerification::GetInstance();
	if (pVerification != NULL)
	{
		pVerification->StartThread(this);
	}

	//m_pITimerEngine->SetTimer(IDI_GET_COMPRANK_INFO,180000L,TIMES_INFINITY,0);
	m_pITimerEngine->SetTimer(IDI_SEND_LOGIN_QUEUEING,5000L,TIMES_INFINITY,0);
	//m_pITimerEngine->SetTimer(IDI_GET_SUPERRANK_INFO,60000L,TIMES_INFINITY,0);

	//��ȡǩ����������
	//ReadSignInRewardConfig();
	m_pITimerEngine->SetTimer(IDI_GET_NEW_MAIL_REMIND,5000L,TIMES_INFINITY,0);
	m_pITimerEngine->SetTimer(IDI_QUERY_FAKE_SERVER_INFO,m_wSendFakeInfoInterval,TIMES_INFINITY,0);
	return true;
}

//ֹͣ�¼�
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)
{
	//״̬����
	m_bNeekCorrespond=true;

	//�������
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;

	//ɾ������
	SafeDeleteArray(m_pBindParameter);

	//�б����
	m_ServerListManager.ResetServerList();
	//ֹͣ�߳�
	m_mMBVerifInfo.clear();
	m_mRegisteredMbNum.clear();
	CVerification * pVerification = CVerification::GetInstance();
	if (pVerification!= NULL)
	{
		pVerification->EndThread();
	}
	return true;
}

//�����¼�
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	switch (wIdentifier)
	{
	case CT_LOAD_DB_GAME_LIST:		//�����б�
		{
			//�����б�
			m_ServerListManager.DisuseKernelItem();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_GAME_LIST,0,NULL,0);

			return true;
		}
	case CT_CONNECT_CORRESPOND:		//����Э��
		{
			//��������
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//������ʾ
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("��������Э�������� [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//��ʾ��Ϣ
			CTraceService::TraceString(szString,TraceLevel_Normal);

			return true;
		}
	}

	return false;
}

//�����¼�
bool CAttemperEngineSink::OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//ʱ���¼�
bool CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)
{
	switch (dwTimerID)
	{
	case IDI_LOAD_GAME_LIST:		//�����б�
		{
			//�����б�
			m_ServerListManager.DisuseKernelItem();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_GAME_LIST,0,NULL,0);

			return true;
		}
	case IDI_CONNECT_CORRESPOND:	//����Э��
		{
			//��������
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//������ʾ
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("��������Э�������� [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//��ʾ��Ϣ
			CTraceService::TraceString(szString,TraceLevel_Normal);

			return true;
		}
	case IDI_COLLECT_ONLINE_INFO:	//ͳ������
		{
			//��������
			DBR_GP_OnLineCountInfo OnLineCountInfo;
			ZeroMemory(&OnLineCountInfo,sizeof(OnLineCountInfo));

			//��ȡ����
			OnLineCountInfo.dwOnLineCountSum=m_ServerListManager.CollectOnlineInfo();

			//��ȡ����
			POSITION KindPosition=NULL;
			do
			{
				//��ȡ����
				CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

				//���ñ���
				if (pGameKindItem!=NULL)
				{
					WORD wKindIndex=OnLineCountInfo.wKindCount++;
					OnLineCountInfo.OnLineCountKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;
					OnLineCountInfo.OnLineCountKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;
				}

				//����ж�
				if (OnLineCountInfo.wKindCount>=CountArray(OnLineCountInfo.OnLineCountKind))
				{
					ASSERT(FALSE);
					break;
				}

			} while (KindPosition!=NULL);

			//��������
			WORD wHeadSize=sizeof(OnLineCountInfo)-sizeof(OnLineCountInfo.OnLineCountKind);
			WORD wSendSize=wHeadSize+OnLineCountInfo.wKindCount*sizeof(OnLineCountInfo.OnLineCountKind[0]);
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_ONLINE_COUNT_INFO,0,&OnLineCountInfo,wSendSize);

			return true;
		}
	case IDI_MAIL_CHECK:
		{
			return true;
		}
	case IDI_WEALTH_BAG_TIME:
		{
			m_vWealthBag.clear();
			m_dwWealthBagStartTime = time(NULL);
			m_pITCPNetworkEngine->SendDataBatch(MDM_GP_USER_SERVICE, SUB_GP_WEALTH_BAG_TIME, NULL, 0, 0);
			m_pITimerEngine->SetTimer(IDI_WEALTH_BAG_BEGIN, 10 * 1000, 1, 0);
			m_pITimerEngine->SetTimer(IDI_WEALTH_BAG_END, 70 * 1000, 1, 0);
			m_pITimerEngine->SetTimer(IDI_WEALTH_BAG_RESULT, 75 * 1000, 1, 0);
			return true;
		}
	case IDI_WEALTH_BAG_BEGIN:
		{
			m_bWealthBagRunning = true;
			for (unsigned int i = 0; i < m_vWealthBag.size(); i++)
			{
				m_pITCPNetworkEngine->SendData(m_vWealthBag[i].dwSocketID, MDM_GP_USER_SERVICE, SUB_GP_WEALTH_BAG_BEGIN);
			}
			return true;
		}
	case IDI_WEALTH_BAG_END:
		{
			m_bWealthBagRunning = false;
			for (unsigned int i = 0; i < m_vWealthBag.size(); i++)
			{
				CMD_GP_WealthBagEnd WealthBagEnd = {0};
				WealthBagEnd.nScore = m_vWealthBag[i].nScore;
				m_pITCPNetworkEngine->SendData(m_vWealthBag[i].dwSocketID, MDM_GP_USER_SERVICE, SUB_GP_WEALTH_BAG_END, &WealthBagEnd, sizeof(WealthBagEnd));
			}
			return true;
		}
	case IDI_WEALTH_BAG_RESULT:
		{
			std::sort(m_vWealthBag.begin(), m_vWealthBag.end(), compare1);

			memset(&m_WealthBagResult, 0, sizeof(m_WealthBagResult));
			m_WealthBagResult.nPlayerCount = m_vWealthBag.size();

			for (unsigned int i = 0; i < m_vWealthBag.size(); i++)
			{
				if (i < 10)
				{
					strcpy(m_WealthBagResult.szNickName[i], m_vWealthBag[i].szNickName);
					m_WealthBagResult.nScore[i] = m_vWealthBag[i].nScore;
				}
				else
				{
					break;
				}
			}

			for (unsigned int i = 0; i < m_vWealthBag.size(); i++)
			{
				DBR_WealthBagAddScore WealthBagAddScore = {0};
				WealthBagAddScore.dwUserID = m_vWealthBag[i].dwUserID;
				WealthBagAddScore.nScore = m_vWealthBag[i].nScore;
				m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_WEALTH_BAG_ADD_SCORE,m_vWealthBag[i].dwSocketID,&WealthBagAddScore,sizeof(WealthBagAddScore));
			}

			return true;
		}
	case IDI_WEALTH_BAG_WILL:
		{
			SYSTEMTIME st;
			GetLocalTime(&st);
			int nTimeNumber = st.wHour * 100 + st.wMinute + TIME_WEALTH_BAG_BEFORE / 60;
			printf("nTimeNumber=%d\n", nTimeNumber);

			static int nLastTimeNumber = 0;

			if ((nLastTimeNumber != nTimeNumber) && (nTimeNumber == 1000 || nTimeNumber == 1200 || nTimeNumber == 1800 || nTimeNumber == 2000))
			{
				nLastTimeNumber = nTimeNumber;
				m_pITimerEngine->SetTimer(IDI_WEALTH_BAG_TIME, TIME_WEALTH_BAG_BEFORE * 1000, 1, 0);
				CMD_GP_WealthBagBefore WealthBagBefore = {0};
				WealthBagBefore.dwTime = TIME_WEALTH_BAG_BEFORE;
				m_pITCPNetworkEngine->SendDataBatch(MDM_GP_USER_SERVICE, SUB_GP_WEALTH_BAG_BEFORE, &WealthBagBefore, sizeof(WealthBagBefore), 0);
			}
			return true;
		}
	case IDI_CHECK_TIMEOUT_MBNUM:
		{
			time_t now = time(0);
			// debug test
			double seconds;
			//printf("size of the %d \n", m_mMBVerifInfo.size());

			// ��������ʱ�����10 min ɾ�� ��֤����Ϣ
			std::map<std::string,tagMapMBVerifInfo>::iterator i,iend;
			iend = m_mMBVerifInfo.end(); 
			for (i = m_mMBVerifInfo.begin(); i != iend;)
			{
				seconds = difftime(now, (*i).second.m_dwVerifCodeSendTime);
				printf("time pass is %.fd\n", seconds);
				if (seconds >= 600 )
				{
					// debug test
					CTraceService::TraceString("erase success",TraceLevel_Normal);
					m_mMBVerifInfo.erase(i++);
				}
				else
				{
					++i;
				}
			}

			return true;			
		}
	case IDI_GET_COMPRANK_INFO:
		{
			SYSTEMTIME SystemTime;
			GetLocalTime(&SystemTime);
			int nType = 300000099;
			nType += SystemTime.wDay *100 + SystemTime.wMonth *10000 + (SystemTime.wYear%100) * 1000000;
			DBR_GP_REQUEST_COMPRANK_INFO sRequestCompRankInfo = {0};
			sRequestCompRankInfo.nType = nType;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_COMPRANK_INFO,0,&sRequestCompRankInfo,sizeof(DBR_GP_REQUEST_COMPRANK_INFO));
			return true;			
		}
	case IDI_SEND_LOGIN_QUEUEING:
		{
			CMD_GP_LoginingInfo LoginingInfo;
			for (int i = 0; i < m_LoginQueueInfo.size(); i++)
			{
				DWORD dwSocketID = m_LoginQueueInfo[i].dwSocketId;
				if (dwSocketID<=0)
				{
					continue;
				}
				LoginingInfo.wLoginingNum = i+m_nSendMaxNum;
				m_pITCPNetworkEngine->SendData(dwSocketID, MDM_GP_LOGON, SUB_GP_LOGINING_INFO, &LoginingInfo, sizeof(LoginingInfo));
			}
			return true;			
		}
	case IDI_GET_SUPERRANK_INFO:
		{
			DBR_GP_REQUEST_SUPERRANK_INFO sRequestSuperRankInfo={0};
			sRequestSuperRankInfo.nType = 117;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_SUPERRANK_INFO,0,&sRequestSuperRankInfo,sizeof(DBR_GP_REQUEST_COMPRANK_INFO));
			return true;			
		}
	case IDI_GET_NEW_MAIL_REMIND:
		{
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_NEWMAIL_REMIND,0,NULL,0);
			return true;			
		}
	// fake game server info
	case IDI_QUERY_FAKE_SERVER_INFO:
		{
			m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_QUERY_FAKE_SERVERINFO,NULL,0);
			return true;
		}
	}


	return false;
}

//Ӧ���¼�
bool CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)
{
	//��ȡ����
	ASSERT(LOWORD(dwSocketID)<m_pInitParameter->m_wMaxConnect);
	if (LOWORD(dwSocketID)>=m_pInitParameter->m_wMaxConnect) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//���ñ���
	pBindParameter->dwSocketID=dwSocketID;
	pBindParameter->dwClientAddr=dwClientAddr;
	pBindParameter->dwActiveTime=(DWORD)time(NULL);
	pBindParameter->bIsLtryInfoSend=false;

	return true;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//�����Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	ZeroMemory((m_pBindParameter+wBindIndex),sizeof(tagBindParameter));
	OnCancelLoginQueueing(dwSocketID);
	return false;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//printf("wMainCmdID = %hd wSubCmdID = %hd\n", Command.wMainCmdID, Command.wSubCmdID);
	switch (Command.wMainCmdID)
	{
	case MDM_GP_LOGON:			//��¼����
		{
			return OnTCPNetworkMainPCLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_SERVER_LIST:	//�б�����
		{
			return OnTCPNetworkMainPCServerList(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_USER_SERVICE:	//��������
		{
			return OnTCPNetworkMainPCUserService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_REMOTE_SERVICE:	//Զ�̷���
		{
			return OnTCPNetworkMainPCRemoteService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_MB_LOGON:			//��¼����
		{
			return OnTCPNetworkMainMBLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_MB_SERVER_LIST:	//�б�����
		{
			return OnTCPNetworkMainMBServerList(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_WEB:
		{
			return OnTCPNetworkMainWebSite(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_CORRESPOND:
		{
			if (Command.wSubCmdID == SUB_GPR_VIPUpgrade)
			{
				CMD_GP_VIPUpgrade * pVIPUpgrade = (CMD_GP_VIPUpgrade *)pData;
				//����һ���ȼ��Ĳ�����
				if (pVIPUpgrade->cbVIPLevel < 1)
				{
					return true;
				}

				LPCTSTR pUpgrade[] =
				{
					"#ffffff��#ff0000%s#ffffff�����������۵�Ŀ���У���Ϊ�ˡ�#ff0000VIP1#ffffff����#L01�鿴VIP1�����Ȩ",
					"#ffffff��ϲ��#ff0000%s#ffffff���ɹ�������#ff0000VIP2#ffffff�������Ǵ�����⡣#L01�鿴VIP2�����Ȩ",
					"#ffffff������#ff0000%s#ffffff��˵����Ϊ��#ff0000VIP3#ffffff����Ҫ�����ʡ�#L01�鿴VIP3�����Ȩ",
					"#ffffff��#ff0000%s#ffffff��˵����Ϊ��#ff0000VIP4#ffffff������;���ǳ��󺣡�#L01�鿴VIP4�����Ȩ",
					"#ffffff��#ff0000%s#ffffff�������ǡ�#ff0000VIP5��VIP5��VIP5��#ffffff����Ҫ������˵���顣#L01�鿴VIP5�����Ȩ",
					"#ffffff��#ff0000%s#ffffff�����ɵ�����Ϊ��#ff0000VIP6#ffffff������Ǯ�����ԡ�#L01�鿴VIP6�����Ȩ",
					"#ffffff�ᣡ��#ff0000%s#ffffff����Ϊ�˶���֮�£�����֮�ϵġ�#ff0000VIP7#ffffff����#L01�鿴VIP7�����Ȩ",
					"#ffffff��Ϊ��#ff0000VIP8#ffffff���ġ�#ff0000%s#ffffff��˵����������Ҹ��Լ������֡�#L01�鿴VIP8�����Ȩ",
					"#ffffff��ü�����#ff0000%s#ffffff���Ƕ�����#ff0000VIP9#ffffff�������쳤̾������˭��#L01�鿴VIP9�����Ȩ",
				};

				if (pVIPUpgrade->cbVIPLevel > CountArray(pUpgrade))
				{
					return true;
				}

				//����ṹ
				CMD_CS_C_VIPUpgrade VIPUpgrade = {0};
				_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), pUpgrade[pVIPUpgrade->cbVIPLevel-1], pVIPUpgrade->szNickName);
				VIPUpgrade.cbType = 40;

				if(m_pITCPSocketService)
				{
					m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_VIPUpgrade,&VIPUpgrade,sizeof(CMD_CS_C_VIPUpgrade));
				}
				return true;
			}
			else if (Command.wSubCmdID == SUB_GPR_ChangeItem)
			{
				CMD_GP_ChangeItem * pChangeItem = (CMD_GP_ChangeItem *)pData;
				//����һ���Ľ�ȯ������
				if (pChangeItem->dwLotteryCount < 30000)
				{
					return true;
				}

				CMD_CS_C_VIPUpgrade VIPUpgrade = {0};
				_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), "#ffffff��ϲ��#ff0000%s#ffffff������Ľ���۹��У��һ��ˡ�#ff0000%s#ffffff����", pChangeItem->szNickName, pChangeItem->szItemName);
				VIPUpgrade.cbType = 20;
				if(m_pITCPSocketService)
				{
					m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_VIPUpgrade,&VIPUpgrade,sizeof(CMD_CS_C_VIPUpgrade));
				}
				return true;
			}

			return true;
		}
	case MDM_KN_COMMAND:
		{
			//printf("SUB_KN_DETECT_MANUAL\n");
			if (Command.wSubCmdID == SUB_KN_DETECT_MANUAL)
			{
				m_pITCPNetworkEngine->SendData(dwSocketID, MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL, NULL, 0);
			}
			return true;
		}
	case MDM_GP_ITEM:
		{
			return OnTCPNetworkMainItemService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_SKILL:
		{
			return OnTCPNetworkMainSkillService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//���ݿ��¼�
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBO_GP_LOGON_SUCCESS:			//��¼�ɹ�
		{
			return OnDBPCLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_LOGON_FAILURE:			//��¼ʧ��
		{
			return OnDBPCLogonFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GUEST_LOGON_SUCCESS:	//�ο͵�¼�ɹ�
		{
			return OnDBPCGuestLogon(dwContextID,pData,wDataSize);
		}
	case DBO_GP_VALIDATE_MBCARD:
		{
			return OnDBPCLogonValidateMBCard(dwContextID,pData,wDataSize);
		}
	case DBO_GP_VALIDATE_PASSPOSTID:
		{
			return OnDBPCLogonValidatePassPostID(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_FACE_INFO:			//�û�ͷ��
		{
			return OnDBPCUserFaceInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INDIVIDUAL:		//�û���Ϣ
		{
			return OnDBPCUserIndividual(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_INFO:		//��������
		{
			return OnDBPCUserInsureInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_SUCCESS:	//���гɹ�
		{
			return OnDBPCUserInsureSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_TRANSFER_INFO:
		{
			return OnDBPCUserInsureTransferInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_FAILURE:	//����ʧ��
		{
			return OnDBPCUserInsureFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_USER_INFO:  //�û���Ϣ
		{
			return OnDBPCUserInsureUserInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_CHECK_PASSWORD:
		{
			return OnDBPCUserCheckPassword(dwContextID,pData,wDataSize);
		}
	case DBO_GP_OPERATE_SUCCESS:		//�����ɹ�
		{
			return OnDBPCOperateSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_OPERATE_FAILURE:		//����ʧ��
		{
			return OnDBPCOperateFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_TYPE_ITEM:			//��������
		{
			return OnDBPCGameTypeItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_KIND_ITEM:			//��������
		{
			return OnDBPCGameKindItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_NODE_ITEM:			//�ڵ�����
		{
			return OnDBPCGameNodeItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_PAGE_ITEM:			//��������
		{
			return OnDBPCGamePageItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_LIST_RESULT:		//���ؽ��
		{
			return OnDBPCGameListResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SIGNIN_QUERY_SUCCESS:	//��ѯǩ���ɹ�
		{
			return OnDBPCSignInQuerySuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SIGNIN_QUERY_FAILURE:   //��ѯǩ��ʧ��
		{
			return OnDBPCSignInQueryFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_SIGNIN_SUCCESS:
		{
			return OnDBPCUserSignInSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_SIGNIN_FAILURE:
		{
			return OnDBPCUserSignInFailure(dwContextID,pData,wDataSize);
		}	
	case DBO_GP_LOTTERY_QUERY_RESULT:
		{
			return OnDBPCLotteryQueryResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_LOTTERY_RESULT:
		{
			return OnDBPCUserLotteryResult(dwContextID,pData,wDataSize);
		}
	case DBO_MB_LOGON_SUCCESS:			//��¼�ɹ�
		{
			return OnDBMBLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_MB_LOGON_FAILURE:			//��¼ʧ��
		{
			return OnDBMBLogonFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_QUERY_MAIL_ID_LIST:
		{
			return OnDBMailIDList(dwContextID,pData,wDataSize);
		}
	case DBO_GP_QUERY_MAIL_INFO:
		{
			return OnDBMailInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_QUERY_MAIL_CONTENT:
		{
			return OnDBMailContent(dwContextID,pData,wDataSize);
		}
	case DBO_GP_QUERY_MAIL_ITEM:
		{
			return OnDBMailItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_CHARGE_VALUE:
		{
			return OnDBChargeValue(dwContextID,pData,wDataSize);
		}
	case DBO_GP_VIP_INFO:
		{
			return OnDBVIPInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_VIP_RESULT:
		{
			return OnDBVIPResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_CURRENT_SCORE:
		{
			return OnDBCurrentScore(dwContextID,pData,wDataSize);
		}
	case DBO_GP_LOGON_GIFT:
		{
			return OnDBLogonGift(dwContextID,pData,wDataSize);
		}
	case DBO_GP_CHARGE1ST_REQUEST:
		{
			return OnDBCharge1stRequest(dwContextID,pData,wDataSize);
		}
	case DBO_GP_CHARGE1ST_GET:
		{
			return OnDBCharge1stGet(dwContextID,pData,wDataSize);
		}
	case DBO_GP_WEALTH_BAG_ADD_SCORE:
		{
			//�Լ��ķ���������
			for (unsigned int i = 0; i < m_vWealthBag.size(); i++)
			{
				if (dwContextID == m_vWealthBag[i].dwSocketID)
				{
					DBO_WealthBagAddScore * pWealthBagAddScore = (DBO_WealthBagAddScore *)pData;
					m_WealthBagResult.nMyRank = i + 1;
					m_WealthBagResult.nMyScore = m_vWealthBag[i].nScore;
					m_WealthBagResult.lCurrentScore = pWealthBagAddScore->lCurrentScore;
					m_pITCPNetworkEngine->SendData(m_vWealthBag[i].dwSocketID, MDM_GP_USER_SERVICE, SUB_GP_WEALTH_BAG_RESULT, &m_WealthBagResult, sizeof(CMD_GPO_WealthBagResult));
					break;
				}
			}

			return true;
		}
	case DBO_GP_USER_TRANSER_DRAGON:
		{
			m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_USER_TRANSER_DRAGON, pData, wDataSize);
			return true;
		}
	case DBO_GP_QUERY_ITEM_COUNT:
		{
			m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_ITEM, SUB_GPO_QUERY_ITEM_COUNT, pData, wDataSize);
			return true;
		}
	case DBO_GP_TRANSFER_ITEM:
		{
			m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_ITEM, SUB_GPO_TRANSFER_ITEM, pData, wDataSize);
			return true;
		}
	case DBO_GP_COMBINE_DRAGON:
		{
			m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_ITEM, SUB_GPO_COMBINE_ITEM, pData, wDataSize);
			return true;
		}
	case DBO_GP_USE_DRAGON:
		{
			m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_ITEM, SUB_GPO_USE_DRAGON, pData, wDataSize);
			return true;
		}
	case DBO_GP_EQUIPMENT_DRAGON:
		{
			m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_ITEM, SUB_GPO_EQUIPMENT_DRAGON, pData, wDataSize);
			return true;
		}
	case DBO_GP_EXCHANGE_LOTTERY:
		{
			m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_ITEM, SUB_GPO_EXCHANGE_LOTTERY, pData, wDataSize);
			return true;
		}
	case DBO_GP_QUERY_SKILL:
		{
			m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_SKILL, SUB_GPO_QUERY_SKILL, pData, wDataSize);
			return true;
		}
	case DBO_GP_VERIFY_MBNUM:
		{
			return OnDBPCVerifyMBNumRes(dwContextID,pData,wDataSize);
		}
	case DBO_GP_MB_REGITER_SUCCESS:
		{
			return OnDBMBNumRegisterSuccessRes(dwContextID,pData,wDataSize);
		}
	case DBO_GP_QUERY_NEW_GUID:
		{
			m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_SKILL, SUB_GPO_NEW_GUID_QUERY, pData, wDataSize);
			return true;
		}
	case DBO_GP_SUPER_GOLD:
		{
			m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_SKILL, SUB_GPO_SuperGold, pData, wDataSize);
			return true;
		}
	case DBO_GP_MB_BIND_NUM_RES:			//�ֻ��Ű󶨽��
		{
			m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_MOBILE_VERIFY_RES, pData, wDataSize);
			return true;
		}
	case DBO_GP_MB_CHECK_NUM_EXIT:			//�ֻ����Ƿ�󶨹�
		{
			m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPR_CHECK_MOBILE_EXIT_RES, pData, wDataSize);
			return true;
		}
	case DBO_GP_BROADCAST_SPEED:			//����㲥�ٶ�
		{
			m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_BROADCAST_SPEED_RES, pData, wDataSize);
			return true;
		}
	case DBO_GP_COMPRANK_INFO:
		{
			return OnDBGetCompRankInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_BROADLABA:
		{
			return OnDBBroadCastResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_TYPE_ITEM_COUNT:
		{
			return m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_TYPE_ITEM_COUNT_RES, pData, wDataSize);
		}
	case DBO_GP_CHANGE_PHONE_FARE:
		{
			return OnDBChangePhoneFareResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SUPERRANK_INFO:
		{
			return OnDBGetSuperRankInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GETBINDREWARDNEWGUID:
		{
			return m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_GET_BINDREWARD_NEWGUID, pData, wDataSize);
		}
	case DBO_GP_GET_NEWMAIL_REMIND:
		{
			return OnDBGetNewMailRemindInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_QUERY_FREE_LABA_COUNT:
		{
			return m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_QUERY_FREE_LABA_RESULT, pData, wDataSize);
		}
	case DBO_GR_MATCH_GET_PRIZE:
		{
			return OnDBGetMatchResultPrize(dwContextID,pData,wDataSize);
		}
	case DBO_GP_BUY_SKILL:
		{
			return m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GP_BUY_SKILL_RESULT, pData, wDataSize);
		}
	}

	return false;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//�����ж�
		if (m_bNeekCorrespond==true)
		{
			//������ʾ
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("��Э�������������ӹر��ˣ�%ld �����������"),m_pInitParameter->m_wConnectTime);

			//��ʾ��Ϣ
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//����ʱ��
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return true;
		}
	}

	return false;
}

//�����¼�
bool CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//�����ж�
		if (nErrorCode!=0)
		{
			//������ʾ
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("Э������������ʧ�� [ %ld ]��%ld �����������"),
				nErrorCode,m_pInitParameter->m_wConnectTime);

			//��ʾ��Ϣ
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//����ʱ��
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return false;
		}

		//��ʾ��Ϣ
		CTraceService::TraceString(TEXT("����ע����Ϸ��¼������..."),TraceLevel_Normal);

		//��������
		CMD_CS_C_RegisterPlaza RegisterPlaza;
		ZeroMemory(&RegisterPlaza,sizeof(RegisterPlaza));

		//���ñ���
		lstrcpyn(RegisterPlaza.szServerName,m_pInitParameter->m_szServerName,CountArray(RegisterPlaza.szServerName));
		lstrcpyn(RegisterPlaza.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(RegisterPlaza.szServerAddr));

		//��������
		m_pITCPSocketService->SendData(MDM_CS_REGISTER,SUB_CS_C_REGISTER_PLAZA,&RegisterPlaza,sizeof(RegisterPlaza));

		return true;
	}

	return true;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//Э������
	if (wServiceID==NETWORK_CORRESPOND)
	{
		switch (Command.wMainCmdID)
		{
		case MDM_CS_REGISTER:		//ע�����
			{
				return OnTCPSocketMainRegister(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_SERVICE_INFO:	//������Ϣ
			{
				return OnTCPSocketMainServiceInfo(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_REMOTE_SERVICE:	//Զ�̷���
			{
				return OnTCPSocketMainRemoteService(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_MANAGER_SERVICE: //�������
			{
				//printf("MDM_CS_MANAGER_SERVICE\n");
				return OnTCPSocketMainManagerService(Command.wSubCmdID,pData,wDataSize);
			}
		}
	}

	//�������
	ASSERT(FALSE);

	return true;
}

//ע���¼�
bool CAttemperEngineSink::OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_REGISTER_FAILURE:		//ע��ʧ��
		{
			//��������
			CMD_CS_S_RegisterFailure * pRegisterFailure=(CMD_CS_S_RegisterFailure *)pData;

			//Ч�����
			ASSERT(wDataSize>=(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString)));
			if (wDataSize<(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString))) return false;

			//�رմ���
			m_bNeekCorrespond=false;
			m_pITCPSocketService->CloseSocket();

			//��ʾ��Ϣ
			LPCTSTR pszDescribeString=pRegisterFailure->szDescribeString;
			if (lstrlen(pszDescribeString)>0) CTraceService::TraceString(pszDescribeString,TraceLevel_Exception);

			//�¼�֪ͨ
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_FAILURE;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//�б��¼�
bool CAttemperEngineSink::OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SERVER_INFO:		//������Ϣ
		{
			//�����б�
			m_ServerListManager.DisuseServerItem();

			return true;
		}
	case SUB_CS_S_SERVER_ONLINE:	//��������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerOnLine));
			if (wDataSize!=sizeof(CMD_CS_S_ServerOnLine)) return false;

			//��������
			CMD_CS_S_ServerOnLine * pServerOnLine=(CMD_CS_S_ServerOnLine *)pData;

			//���ҷ���
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerOnLine->wServerID);
			if(pGameServerItem == NULL) return true;

			//��������
			DWORD dwOldOnlineCount=0;
			dwOldOnlineCount = pGameServerItem->m_GameServer.dwOnLineCount;
			pGameServerItem->m_GameServer.dwOnLineCount=pServerOnLine->dwOnLineCount;

			//Ŀ¼����
			CGameKindItem * pGameKindItem=m_ServerListManager.SearchGameKind(pGameServerItem->m_GameServer.wKindID);
			if (pGameKindItem!=NULL)
			{
				tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
				pGameKindItem->m_GameKind.dwOnLineCount -= dwOldOnlineCount;
				pGameKindItem->m_GameKind.dwOnLineCount += pGameServer->dwOnLineCount;
			}

			return true;
		}
	case SUB_CS_S_SERVER_INSERT:	//�������
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(tagGameServer)==0);
			if (wDataSize%sizeof(tagGameServer)!=0) return false;

			//��������
			WORD wItemCount=wDataSize/sizeof(tagGameServer);
			tagGameServer * pGameServer=(tagGameServer *)pData;

			//��������
			for (WORD i=0;i<wItemCount;i++)
			{
				m_ServerListManager.InsertGameServer(pGameServer++);
			}

			return true;
		}
	case SUB_CS_S_SERVER_MODIFY:	//�����޸�
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerModify));
			if (wDataSize!=sizeof(CMD_CS_S_ServerModify)) return false;

			//��������
			CMD_CS_S_ServerModify * pServerModify=(CMD_CS_S_ServerModify *)pData;

			//���ҷ���
			ASSERT(m_ServerListManager.SearchGameServer(pServerModify->wServerID));
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerModify->wServerID);

			//���÷���
			if (pGameServerItem!=NULL)
			{
				//��������
				DWORD dwOldOnlineCount=0, dwOldFullCount=0;
				dwOldOnlineCount = pGameServerItem->m_GameServer.dwOnLineCount;
				dwOldFullCount   = pGameServerItem->m_GameServer.dwFullCount;

				//�޸ķ�����Ϣ
				pGameServerItem->m_GameServer.wKindID=pServerModify->wKindID;
				pGameServerItem->m_GameServer.wNodeID=pServerModify->wNodeID;
				pGameServerItem->m_GameServer.wSortID=pServerModify->wSortID;
				pGameServerItem->m_GameServer.wServerPort=pServerModify->wServerPort;
				pGameServerItem->m_GameServer.dwOnLineCount=pServerModify->dwOnLineCount;
				pGameServerItem->m_GameServer.dwFullCount=pServerModify->dwFullCount;
				lstrcpyn(pGameServerItem->m_GameServer.szServerName,pServerModify->szServerName,CountArray(pGameServerItem->m_GameServer.szServerName));
				lstrcpyn(pGameServerItem->m_GameServer.szServerAddr,pServerModify->szServerAddr,CountArray(pGameServerItem->m_GameServer.szServerAddr));

				//Ŀ¼����
				CGameKindItem * pGameKindItem=m_ServerListManager.SearchGameKind(pGameServerItem->m_GameServer.wKindID);
				if (pGameKindItem!=NULL)
				{
					tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
					pGameKindItem->m_GameKind.dwOnLineCount -= dwOldOnlineCount;
					pGameKindItem->m_GameKind.dwOnLineCount += pGameServer->dwOnLineCount;

					pGameKindItem->m_GameKind.dwFullCount -= dwOldFullCount;
					pGameKindItem->m_GameKind.dwFullCount += pGameServer->dwFullCount;
				}
			}

			return true;
		}
	case SUB_CS_S_SERVER_REMOVE:	//����ɾ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerRemove));
			if (wDataSize!=sizeof(CMD_CS_S_ServerRemove)) return false;

			//��������
			CMD_CS_S_ServerRemove * pServerRemove=(CMD_CS_S_ServerRemove *)pData;

			//��������
			m_ServerListManager.DeleteGameServer(pServerRemove->wServerID);

			return true;
		}
	case SUB_CS_S_SERVER_FINISH:	//�������
		{
			//�����б�
			m_ServerListManager.CleanServerItem();

			//�¼�����
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_SUCCESS;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	// online fake cnt 
	// case fake cnt 
	// SUB_GP_GET_ONLINE copy the content of the 
	}

	return true;
}

//Զ�̷���
bool CAttemperEngineSink::OnTCPSocketMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SEARCH_CORRESPOND:	//Э������
		{
			//��������
			CMD_CS_S_SearchCorrespond * pSearchCorrespond=(CMD_CS_S_SearchCorrespond *)pData;

			//Ч�����
			ASSERT(wDataSize<=sizeof(CMD_CS_S_SearchCorrespond));
			ASSERT(wDataSize>=(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)));
			ASSERT(wDataSize==(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0])));

			//Ч�����
			if (wDataSize>sizeof(CMD_CS_S_SearchCorrespond)) return false;
			if (wDataSize<(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo))) return false;
			if (wDataSize!=(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0]))) return false;

			//�ж�����
			ASSERT(LOWORD(pSearchCorrespond->dwSocketID)<m_pInitParameter->m_wMaxConnect);
			if ((m_pBindParameter+LOWORD(pSearchCorrespond->dwSocketID))->dwSocketID!=pSearchCorrespond->dwSocketID) return true;

			//��������
			CMD_GP_S_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//���ñ���
			for (WORD i=0;i<pSearchCorrespond->wUserCount;i++)
			{
				//����Ч��
				ASSERT(SearchCorrespond.wUserCount<CountArray(SearchCorrespond.UserRemoteInfo));
				if (SearchCorrespond.wUserCount>=CountArray(SearchCorrespond.UserRemoteInfo)) break;

				//���ñ���
				WORD wIndex=SearchCorrespond.wUserCount++;
				CopyMemory(&SearchCorrespond.UserRemoteInfo[wIndex],&pSearchCorrespond->UserRemoteInfo[i],sizeof(SearchCorrespond.UserRemoteInfo[wIndex]));
			}

			//��������
			WORD wHeadSize=sizeof(SearchCorrespond)-sizeof(SearchCorrespond.UserRemoteInfo);
			WORD wItemSize=sizeof(SearchCorrespond.UserRemoteInfo[0])*SearchCorrespond.wUserCount;
			m_pITCPNetworkEngine->SendData(pSearchCorrespond->dwSocketID,MDM_GP_REMOTE_SERVICE,SUB_GP_S_SEARCH_CORRESPOND,&SearchCorrespond,wHeadSize+wItemSize);

			return true;
		}
	}

	return true;
}

//�������
bool CAttemperEngineSink::OnTCPSocketMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_VIPUpgrade:
		{
			//printf("SUB_CS_C_VIPUpgrade\n");
			//for (WORD wConnectCount = 0; wConnectCount < m_pInitParameter->m_wMaxConnect; wConnectCount++)
			//{
			//	tagBindParameter * pBindParameter = m_pBindParameter + wConnectCount;
			//	if (pBindParameter != NULL && pBindParameter->dwSocketID != 0)
			//	{
			//		printf("[%u]%s\n", pBindParameter->dwUserID, pBindParameter->szNickName);
			//		m_pITCPNetworkEngine->SendData(pBindParameter->dwSocketID, MDM_GP_USER_SERVICE, SUB_GPO_VIPUpgrade, pData, wDataSize);
			//	}
			//}
			m_pITCPNetworkEngine->SendDataBatch(MDM_GP_CORRESPOND, SUB_GPO_VIPUpgrade, pData, wDataSize, 0);
			return true;
		}
	case SUB_CS_S_SystemBroadcast:
		{
			//printf("SUB_CS_S_SystemBroadcast\n");
			m_pITCPNetworkEngine->SendDataBatch(MDM_GP_CORRESPOND, SUB_GP_SystemBroadcast, pData, wDataSize, 0);
			return true;
		}
	case SUB_CS_S_BROAD_LABA:
		{
			//printf("SUB_CS_S_SystemBroadcast\n");
			m_pITCPNetworkEngine->SendDataBatch(MDM_GP_CORRESPOND, SUB_GP_SystemBroadLaBa, pData, wDataSize, 0);
			return true;
		}
	case SUB_CS_S_QUERY_FAKE_SERVERINFO:
		{
			CMD_CS_S_FakeServerInfo * pFakeServerInfo = (CMD_CS_S_FakeServerInfo *)pData;
			WORD wTmpSize = sizeof(CMD_CS_S_FakeServerInfo) - sizeof(tagFakeServerInfo) * (100 - pFakeServerInfo->wServerCnt);
			if (wTmpSize != pFakeServerInfo->wDataSize)
			{
				return false;
			}

			// save a data local at the logon server, every time clear the vector and send 
			// fill the vector no I should ask the correspond every time for the info then we use the data in the package to 

			// copy data to the 
			CopyMemory();
			m_pITCPNetworkEngine->SendDataBatch(MDM_GP_USER_SERVICE, SUB_GP_WEALTH_BAG_BEFORE, &WealthBagBefore, sizeof(WealthBagBefore), 0);
		}
	}
	return true;
}

//��¼����
bool CAttemperEngineSink::OnTCPNetworkMainPCLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
	switch (wSubCmdID)
	{
	case SUB_GP_LOGON_GAMEID:		//I D ��¼
		{
			return OnTCPNetworkSubPCLogonGameID(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_LOGON_ACCOUNTS:		//�ʺŵ�¼
		{
			return OnTCPNetworkSubPCLogonAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_REGISTER_ACCOUNTS:	//�ʺ�ע��
		{
			return OnTCPNetworkSubPCRegisterAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_GUEST_REG:		//�ο�ע�ᴦ����
		{
			return OnTCPNetworkSubPCGuestReg(pData,wDataSize,dwSocketID);
		}
	case SUB_GPR_MBREIGSTER:  // �ֻ���ע��
		{
			return OnTCPNetworkSubPCMBNumRegister(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_MOBILE_VERIFICAT:
		{
			return OnTCPNetworkSubPCCodeVerification(pData,wDataSize,dwSocketID);
		}
	case SUB_GPR_CANCEL_LOGINING:  //ȡ����¼�Ŷ�
		{
			return OnCancelLoginQueueing(dwSocketID);
		}
	}

	return false;
}

//�б���
bool CAttemperEngineSink::OnTCPNetworkMainPCServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_GET_LIST:			//��ȡ�б�
		{
			//�����б�
			SendGameTypeInfo(dwSocketID);
			SendGameKindInfo(dwSocketID);

			//�����б�
			if (m_pInitParameter->m_cbDelayList==TRUE)
			{
				//�����б�
				SendGamePageInfo(dwSocketID,INVALID_WORD);
				SendGameNodeInfo(dwSocketID,INVALID_WORD);
				SendGameServerInfo(dwSocketID,INVALID_WORD);
			}
			else
			{
				//����ҳ��
				SendGamePageInfo(dwSocketID,0);
			}

			//�������
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);

			return true;
		}
	case SUB_GP_GET_SERVER:			//��ȡ����
		{
			//Ч������
			ASSERT(wDataSize%sizeof(WORD)==0);
			if (wDataSize%sizeof(WORD)!=0) return false;

			//�����б�
			UINT nKindCount=wDataSize/sizeof(WORD);
			for (UINT i=0;i<nKindCount;i++)
			{
				SendGameNodeInfo(dwSocketID,((WORD *)pData)[i]);
				SendGamePageInfo(dwSocketID,((WORD *)pData)[i]);
				SendGameServerInfo(dwSocketID,((WORD *)pData)[i]);
			}

			//�������
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_SERVER_FINISH,pData,wDataSize);

			return true;
		}
	case SUB_GP_GET_ONLINE:			//��ȡ����
		{
			//��������
			CMD_GP_GetOnline * pGetOnline=(CMD_GP_GetOnline *)pData;
			WORD wHeadSize=(sizeof(CMD_GP_GetOnline)-sizeof(pGetOnline->wOnLineServerID));

			//Ч������
			ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGetOnline->wServerCount*sizeof(WORD))));
			if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGetOnline->wServerCount*sizeof(WORD)))) return false;

			//��������
			CMD_GP_KindOnline KindOnline;
			CMD_GP_ServerOnline ServerOnline;
			ZeroMemory(&KindOnline,sizeof(KindOnline));
			ZeroMemory(&ServerOnline,sizeof(ServerOnline));

			//��ȡ����
			POSITION KindPosition=NULL;
			do
			{
				//��ȡ����
				CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

				//���ñ���
				if (pGameKindItem!=NULL)
				{
					WORD wKindIndex=KindOnline.wKindCount++;
					KindOnline.OnLineInfoKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;
					KindOnline.OnLineInfoKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;
				}

				//����ж�
				if (KindOnline.wKindCount>=CountArray(KindOnline.OnLineInfoKind))
				{
					ASSERT(FALSE);
					break;
				}

			} while (KindPosition!=NULL);

			//��ȡ����
			for (WORD i=0;i<pGetOnline->wServerCount;i++)
			{
				//��ȡ����
				WORD wServerID=pGetOnline->wOnLineServerID[i];
				CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(wServerID);

				//���ñ���
				if (pGameServerItem!=NULL)
				{
					WORD wServerIndex=ServerOnline.wServerCount++;
					ServerOnline.OnLineInfoServer[wServerIndex].wServerID=wServerID;
					ServerOnline.OnLineInfoServer[wServerIndex].dwOnLineCount=pGameServerItem->m_GameServer.dwOnLineCount;
				}

				//����ж�
				if (ServerOnline.wServerCount>=CountArray(ServerOnline.OnLineInfoServer))
				{
					ASSERT(FALSE);
					break;
				}
			}

			//��������
			if (KindOnline.wKindCount>0)
			{
				WORD wHeadSize=sizeof(KindOnline)-sizeof(KindOnline.OnLineInfoKind);
				WORD wSendSize=wHeadSize+KindOnline.wKindCount*sizeof(KindOnline.OnLineInfoKind[0]);
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GR_KINE_ONLINE,&KindOnline,wSendSize);
			}

			//��������
			if (ServerOnline.wServerCount>0)
			{
				WORD wHeadSize=sizeof(ServerOnline)-sizeof(ServerOnline.OnLineInfoServer);
				WORD wSendSize=wHeadSize+ServerOnline.wServerCount*sizeof(ServerOnline.OnLineInfoServer[0]);
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GR_SERVER_ONLINE,&ServerOnline,wSendSize);
			}

			return true;
		}
	case SUB_GP_GET_COLLECTION:		//��ȡ�ղ�
		{
			return true;
		}
	}

	return false;
}

//������
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_MODIFY_MACHINE:		//�󶨻���
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyMachine));
			if (wDataSize!=sizeof(CMD_GP_ModifyMachine)) return false;

			//������Ϣ
			CMD_GP_ModifyMachine * pModifyMachine=(CMD_GP_ModifyMachine *)pData;
			pModifyMachine->szPassword[CountArray(pModifyMachine->szPassword)-1]=0;

			//��������
			DBR_GP_ModifyMachine ModifyMachine;
			ZeroMemory(&ModifyMachine,sizeof(ModifyMachine));

			//��������
			ModifyMachine.cbBind=pModifyMachine->cbBind;
			ModifyMachine.dwUserID=pModifyMachine->dwUserID;
			ModifyMachine.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyMachine.szPassword,pModifyMachine->szPassword,CountArray(ModifyMachine.szPassword));
			lstrcpyn(ModifyMachine.szMachineID,pModifyMachine->szMachineID,CountArray(ModifyMachine.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_MACHINE,dwSocketID,&ModifyMachine,sizeof(ModifyMachine));

			return true;
		}
	case SUB_GP_MODIFY_LOGON_PASS:	//�޸�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyLogonPass));
			if (wDataSize!=sizeof(CMD_GP_ModifyLogonPass)) return false;

			//������Ϣ
			CMD_GP_ModifyLogonPass * pModifyLogonPass=(CMD_GP_ModifyLogonPass *)pData;
			pModifyLogonPass->szDesPassword[CountArray(pModifyLogonPass->szDesPassword)-1]=0;
			pModifyLogonPass->szScrPassword[CountArray(pModifyLogonPass->szScrPassword)-1]=0;

			//��������
			DBR_GP_ModifyLogonPass ModifyLogonPass;
			ZeroMemory(&ModifyLogonPass,sizeof(ModifyLogonPass));

			//��������
			ModifyLogonPass.dwUserID=pModifyLogonPass->dwUserID;
			ModifyLogonPass.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyLogonPass.szDesPassword,pModifyLogonPass->szDesPassword,CountArray(ModifyLogonPass.szDesPassword));
			lstrcpyn(ModifyLogonPass.szScrPassword,pModifyLogonPass->szScrPassword,CountArray(ModifyLogonPass.szScrPassword));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_LOGON_PASS,dwSocketID,&ModifyLogonPass,sizeof(ModifyLogonPass));

			return true;
		}
	case SUB_GP_MODIFY_INSURE_PASS:	//�޸�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyInsurePass));
			if (wDataSize!=sizeof(CMD_GP_ModifyInsurePass)) return false;

			//������Ϣ
			CMD_GP_ModifyInsurePass * pModifyInsurePass=(CMD_GP_ModifyInsurePass *)pData;
			pModifyInsurePass->szDesPassword[CountArray(pModifyInsurePass->szDesPassword)-1]=0;
			pModifyInsurePass->szScrPassword[CountArray(pModifyInsurePass->szScrPassword)-1]=0;

			//��������
			DBR_GP_ModifyInsurePass ModifyInsurePass;
			ZeroMemory(&ModifyInsurePass,sizeof(ModifyInsurePass));

			//��������
			ModifyInsurePass.dwUserID=pModifyInsurePass->dwUserID;
			ModifyInsurePass.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyInsurePass.szDesPassword,pModifyInsurePass->szDesPassword,CountArray(ModifyInsurePass.szDesPassword));
			lstrcpyn(ModifyInsurePass.szScrPassword,pModifyInsurePass->szScrPassword,CountArray(ModifyInsurePass.szScrPassword));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INSURE_PASS,dwSocketID,&ModifyInsurePass,sizeof(ModifyInsurePass));

			return true;
		}
	case SUB_GP_MODIFY_UNDER_WRITE:	//�޸�ǩ��
		{
			//��������
			CMD_GP_ModifyUnderWrite * pModifyUnderWrite=(CMD_GP_ModifyUnderWrite *)pData;

			//Ч�����
			ASSERT(wDataSize<=sizeof(CMD_GP_ModifyUnderWrite));
			ASSERT(wDataSize>=(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite)));
			ASSERT(wDataSize==(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite)+CountStringBuffer(pModifyUnderWrite->szUnderWrite)));

			//Ч�����
			if (wDataSize>sizeof(CMD_GP_ModifyUnderWrite)) return false;
			if (wDataSize<(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite))) return false;
			if (wDataSize!=(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite)+CountStringBuffer(pModifyUnderWrite->szUnderWrite))) return false;

			//������Ϣ
			pModifyUnderWrite->szPassword[CountArray(pModifyUnderWrite->szPassword)-1]=0;
			pModifyUnderWrite->szUnderWrite[CountArray(pModifyUnderWrite->szUnderWrite)-1]=0;

			//��������
			DBR_GP_ModifyUnderWrite ModifyUnderWrite;
			ZeroMemory(&ModifyUnderWrite,sizeof(ModifyUnderWrite));

			//��������
			ModifyUnderWrite.dwUserID=pModifyUnderWrite->dwUserID;
			ModifyUnderWrite.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyUnderWrite.szPassword,pModifyUnderWrite->szPassword,CountArray(ModifyUnderWrite.szPassword));
			lstrcpyn(ModifyUnderWrite.szUnderWrite,pModifyUnderWrite->szUnderWrite,CountArray(ModifyUnderWrite.szUnderWrite));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_UNDER_WRITE,dwSocketID,&ModifyUnderWrite,sizeof(ModifyUnderWrite));

			return true;
		}
	case SUB_GP_SYSTEM_FACE_INFO:	//�޸�ͷ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_SystemFaceInfo));
			if (wDataSize!=sizeof(CMD_GP_SystemFaceInfo)) return false;

			//������Ϣ
			CMD_GP_SystemFaceInfo * pSystemFaceInfo=(CMD_GP_SystemFaceInfo *)pData;

			//��������
			DBR_GP_ModifySystemFace ModifySystemFace;
			ZeroMemory(&ModifySystemFace,sizeof(ModifySystemFace));

			//��������
			ModifySystemFace.wFaceID=pSystemFaceInfo->wFaceID;
			ModifySystemFace.dwUserID=pSystemFaceInfo->dwUserID;
			ModifySystemFace.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifySystemFace.szPassword,pSystemFaceInfo->szPassword,CountArray(ModifySystemFace.szPassword));
			lstrcpyn(ModifySystemFace.szMachineID,pSystemFaceInfo->szMachineID,CountArray(ModifySystemFace.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_SYSTEM_FACE,dwSocketID,&ModifySystemFace,sizeof(ModifySystemFace));

			return true;
		}
	case SUB_GP_CUSTOM_FACE_INFO:	//�޸�ͷ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_CustomFaceInfo));
			if (wDataSize!=sizeof(CMD_GP_CustomFaceInfo)) return false;

			//������Ϣ
			CMD_GP_CustomFaceInfo * pCustomFaceInfo=(CMD_GP_CustomFaceInfo *)pData;

			//��������
			DBR_GP_ModifyCustomFace ModifyCustomFace;
			ZeroMemory(&ModifyCustomFace,sizeof(ModifyCustomFace));

			//��������
			ModifyCustomFace.dwUserID=pCustomFaceInfo->dwUserID;
			ModifyCustomFace.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyCustomFace.szPassword,pCustomFaceInfo->szPassword,CountArray(ModifyCustomFace.szPassword));
			lstrcpyn(ModifyCustomFace.szMachineID,pCustomFaceInfo->szMachineID,CountArray(ModifyCustomFace.szMachineID));
			CopyMemory(ModifyCustomFace.dwCustomFace,pCustomFaceInfo->dwCustomFace,sizeof(ModifyCustomFace.dwCustomFace));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_CUSTOM_FACE,dwSocketID,&ModifyCustomFace,sizeof(ModifyCustomFace));

			return true;
		}
	case SUB_GP_QUERY_INDIVIDUAL:	//��ѯ��Ϣ
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QueryIndividual));
			if (wDataSize!=sizeof(CMD_GP_QueryIndividual)) return false;

			//������Ϣ
			CMD_GP_QueryIndividual * pQueryIndividual=(CMD_GP_QueryIndividual *)pData;

			//��������
			DBR_GP_QueryIndividual QueryIndividual;
			ZeroMemory(&QueryIndividual,sizeof(QueryIndividual));

			//��������
			QueryIndividual.dwUserID=pQueryIndividual->dwUserID;
			QueryIndividual.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_INDIVIDUAL,dwSocketID,&QueryIndividual,sizeof(QueryIndividual));

			return true;
		}
	case SUB_GP_MODIFY_INDIVIDUAL:	//�޸�����
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(CMD_GP_ModifyIndividual));
			if (wDataSize<sizeof(CMD_GP_ModifyIndividual)) return false;

			//������Ϣ
			CMD_GP_ModifyIndividual * pModifyIndividual=(CMD_GP_ModifyIndividual *)pData;
			pModifyIndividual->szPassword[CountArray(pModifyIndividual->szPassword)-1]=0;

			//��������
			DBR_GP_ModifyIndividual ModifyIndividual;
			ZeroMemory(&ModifyIndividual,sizeof(ModifyIndividual));

			//���ñ���
			ModifyIndividual.dwUserID=pModifyIndividual->dwUserID;
			ModifyIndividual.cbGender=pModifyIndividual->cbGender;
			ModifyIndividual.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyIndividual.szPassword,pModifyIndividual->szPassword,CountArray(ModifyIndividual.szPassword));

			//��������
			VOID * pDataBuffer=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pModifyIndividual+1,wDataSize-sizeof(CMD_GP_ModifyIndividual));

			//��չ��Ϣ
			while (true)
			{
				pDataBuffer=RecvPacket.GetData(DataDescribe);
				if (DataDescribe.wDataDescribe==DTP_NULL) break;
				switch (DataDescribe.wDataDescribe)
				{
				case DTP_GP_UI_NICKNAME:		//�û��ǳ�
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szNickName));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szNickName))
						{
							CopyMemory(&ModifyIndividual.szNickName,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szNickName[CountArray(ModifyIndividual.szNickName)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_UNDER_WRITE:		//����ǩ��
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUnderWrite));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUnderWrite))
						{
							CopyMemory(&ModifyIndividual.szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szUnderWrite[CountArray(ModifyIndividual.szUnderWrite)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_USER_NOTE:		//�û���ע
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUserNote));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szUserNote))
						{
							CopyMemory(&ModifyIndividual.szUserNote,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szUserNote[CountArray(ModifyIndividual.szUserNote)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_COMPELLATION:	//��ʵ����
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szCompellation));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szCompellation))
						{
							CopyMemory(&ModifyIndividual.szCompellation,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szCompellation[CountArray(ModifyIndividual.szCompellation)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_SEAT_PHONE:		//�̶��绰
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szSeatPhone));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szSeatPhone))
						{
							CopyMemory(ModifyIndividual.szSeatPhone,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szSeatPhone[CountArray(ModifyIndividual.szSeatPhone)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_MOBILE_PHONE:	//�ƶ��绰
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szMobilePhone));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szMobilePhone))
						{
							CopyMemory(ModifyIndividual.szMobilePhone,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szMobilePhone[CountArray(ModifyIndividual.szMobilePhone)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_QQ:				//Q Q ����
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szQQ));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szQQ))
						{
							CopyMemory(ModifyIndividual.szQQ,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szQQ[CountArray(ModifyIndividual.szQQ)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_EMAIL:			//�����ʼ�
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szEMail));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szEMail))
						{
							CopyMemory(ModifyIndividual.szEMail,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szEMail[CountArray(ModifyIndividual.szEMail)-1]=0;
						}
						break;
					}
				case DTP_GP_UI_DWELLING_PLACE:	//��ϵ��ַ
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize<=sizeof(ModifyIndividual.szDwellingPlace));
						if (DataDescribe.wDataSize<=sizeof(ModifyIndividual.szDwellingPlace))
						{
							CopyMemory(ModifyIndividual.szDwellingPlace,pDataBuffer,DataDescribe.wDataSize);
							ModifyIndividual.szDwellingPlace[CountArray(ModifyIndividual.szDwellingPlace)-1]=0;
						}
						break;
					}
				}
			}

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INDIVIDUAL,dwSocketID,&ModifyIndividual,sizeof(ModifyIndividual));

			return true;
		}
	case SUB_GP_USER_SAVE_SCORE:	//������Ϸ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserSaveScore));
			if (wDataSize!=sizeof(CMD_GP_UserSaveScore)) return false;

			//������Ϣ
			CMD_GP_UserSaveScore * pUserSaveScore=(CMD_GP_UserSaveScore *)pData;
			pUserSaveScore->szMachineID[CountArray(pUserSaveScore->szMachineID)-1]=0;

			//��������
			DBR_GP_UserSaveScore UserSaveScore;
			ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

			//��������
			UserSaveScore.dwUserID=pUserSaveScore->dwUserID;
			UserSaveScore.lSaveScore=pUserSaveScore->lSaveScore;
			UserSaveScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserSaveScore.szMachineID,pUserSaveScore->szMachineID,CountArray(UserSaveScore.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_SAVE_SCORE,dwSocketID,&UserSaveScore,sizeof(UserSaveScore));

			return true;
		}
	case SUB_GP_USER_TAKE_SCORE:	//ȡ����Ϸ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserTakeScore));
			if (wDataSize!=sizeof(CMD_GP_UserTakeScore)) return false;

			//������Ϣ
			CMD_GP_UserTakeScore * pUserTakeScore=(CMD_GP_UserTakeScore *)pData;
			pUserTakeScore->szPassword[CountArray(pUserTakeScore->szPassword)-1]=0;
			pUserTakeScore->szMachineID[CountArray(pUserTakeScore->szMachineID)-1]=0;

			//��������
			DBR_GP_UserTakeScore UserTakeScore;
			ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

			//��������
			UserTakeScore.dwUserID=pUserTakeScore->dwUserID;
			UserTakeScore.lTakeScore=pUserTakeScore->lTakeScore;
			UserTakeScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserTakeScore.szPassword,pUserTakeScore->szPassword,CountArray(UserTakeScore.szPassword));
			lstrcpyn(UserTakeScore.szMachineID,pUserTakeScore->szMachineID,CountArray(UserTakeScore.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_TAKE_SCORE,dwSocketID,&UserTakeScore,sizeof(UserTakeScore));

			return true;
		}
	case SUB_GP_USER_TRANSFER_SCORE://ת����Ϸ��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserTransferScore));
			if (wDataSize!=sizeof(CMD_GP_UserTransferScore)) return false;

			//������Ϣ
			CMD_GP_UserTransferScore * pUserTransferScore=(CMD_GP_UserTransferScore *)pData;
			pUserTransferScore->szNickName[CountArray(pUserTransferScore->szNickName)-1]=0;
			pUserTransferScore->szPassword[CountArray(pUserTransferScore->szPassword)-1]=0;
			pUserTransferScore->szMachineID[CountArray(pUserTransferScore->szMachineID)-1]=0;

			//��������
			DBR_GP_UserTransferScore UserTransferScore;
			ZeroMemory(&UserTransferScore,sizeof(UserTransferScore));

			//��������
			UserTransferScore.dwUserID=pUserTransferScore->dwUserID;
			UserTransferScore.cbByNickName=pUserTransferScore->cbByNickName;
			UserTransferScore.lTransferScore=pUserTransferScore->lTransferScore;
			UserTransferScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserTransferScore.szNickName,pUserTransferScore->szNickName,CountArray(UserTransferScore.szNickName));
			lstrcpyn(UserTransferScore.szPassword,pUserTransferScore->szPassword,CountArray(UserTransferScore.szPassword));
			lstrcpyn(UserTransferScore.szMachineID,pUserTransferScore->szMachineID,CountArray(UserTransferScore.szMachineID));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_TRANSFER_SCORE,dwSocketID,&UserTransferScore,sizeof(UserTransferScore));

			return true;
		}
	case SUB_GP_QUERY_INSURE_INFO:	//��ѯ����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QueryInsureInfo));
			if (wDataSize!=sizeof(CMD_GP_QueryInsureInfo)) return false;

			//������Ϣ
			CMD_GP_QueryInsureInfo * pQueryInsureInfo=(CMD_GP_QueryInsureInfo *)pData;

			//��������
			DBR_GP_QueryInsureInfo QueryInsureInfo;
			ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

			//��������
			QueryInsureInfo.dwUserID=pQueryInsureInfo->dwUserID;
			QueryInsureInfo.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_INSURE_INFO,dwSocketID,&QueryInsureInfo,sizeof(QueryInsureInfo));

			return true;
		}
	case SUB_GP_USER_CHECK_PASSWORD:
		{
			if (wDataSize!=sizeof(CMD_GP_CheckPassword))
			{
				return false;
			}

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_CHECK_PASSWORD,dwSocketID,pData,wDataSize);

			return true;
		}
	case SUB_GP_QUERY_USER_INFO_REQUEST:  //��ѯ�û�
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QueryUserInfoRequest));
			if (wDataSize!=sizeof(CMD_GP_QueryUserInfoRequest)) return false;

			//������Ϣ
			CMD_GP_QueryUserInfoRequest * pQueryUserInfoRequest=(CMD_GP_QueryUserInfoRequest *)pData;

			//��������
			DBR_GP_QueryInsureUserInfo QueryInsureUserInfo;
			ZeroMemory(&QueryInsureUserInfo,sizeof(QueryInsureUserInfo));

			//��������
			QueryInsureUserInfo.cbByNickName=pQueryUserInfoRequest->cbByNickName;
			lstrcpyn(QueryInsureUserInfo.szNickName,pQueryUserInfoRequest->szNickName,CountArray(QueryInsureUserInfo.szNickName));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_USER_INFO,dwSocketID,&QueryInsureUserInfo,sizeof(QueryInsureUserInfo));

			return true;
		}
	case SUB_GP_QUERY_SIGNIN_INFO:
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QuerySigninInfo));
			if (wDataSize!=sizeof(CMD_GP_QuerySigninInfo)) return false;

			//������Ϣ
			CMD_GP_QuerySigninInfo * pQuerySignInInfo=(CMD_GP_QuerySigninInfo *)pData;

			//��������
			DBR_GP_QuerySignInInfo QuerySignInInfo;
			ZeroMemory(&QuerySignInInfo,sizeof(QuerySignInInfo));

			QuerySignInInfo.dwUserID = pQuerySignInInfo->dwUserID;

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_SIGIN_INFO,dwSocketID,&QuerySignInInfo,sizeof(QuerySignInInfo));

			return true;
		}
	case SUB_GP_USER_SIGNIN_REQ:
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_SignIn));
			if (wDataSize!=sizeof(CMD_GP_SignIn)) return false;

			//������Ϣ
			CMD_GP_SignIn * pUserSignIn=(CMD_GP_SignIn *)pData;

			//��������
			DBR_GP_UserSignIn UserSignIn;
			ZeroMemory(&UserSignIn,sizeof(UserSignIn));

			UserSignIn.dwUserID = pUserSignIn->dwUserID;
			UserSignIn.cbVip = pUserSignIn->cbVip;

			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_SIGIN,dwSocketID,&UserSignIn,sizeof(UserSignIn));

			return true;
		}
	case SUB_GP_QUERY_FREE_LABA_COUNT:
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GPR_FREE_LABA_QUERY));
			if (wDataSize!=sizeof(CMD_GPR_FREE_LABA_QUERY)) return false;

			//������Ϣ
			CMD_GPR_FREE_LABA_QUERY * pQueryInfo=(CMD_GPR_FREE_LABA_QUERY *)pData;

			//��������
			DBR_GP_Free_LaBa_Count sQueryInfo;
			ZeroMemory(&sQueryInfo,sizeof(sQueryInfo));
			sQueryInfo.dwUserID = pQueryInfo->dwUserID;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_FREE_LABA_COUNT,dwSocketID,&sQueryInfo,sizeof(sQueryInfo));

			return true;
		}
	case SUB_GP_BUY_SKILL:
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GPR_BuySkill));
			if (wDataSize!=sizeof(CMD_GPR_BuySkill)) return false;

			//������Ϣ
			CMD_GPR_BuySkill * pBuyInfo=(CMD_GPR_BuySkill *)pData;

			//��������
			DBR_GP_Buy_Skill sBuySkill;
			ZeroMemory(&sBuySkill,sizeof(sBuySkill));
			sBuySkill.dwUserID = pBuyInfo->dwUserID;
			sBuySkill.nSkillID = pBuyInfo->cbSkillID;
			sBuySkill.nCount = pBuyInfo->nCount;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_BUY_SKILL,dwSocketID,&sBuySkill,sizeof(DBR_GP_Buy_Skill));

			return true;
		}
	case SUB_GP_QUERY_LOTTERY_INFO:
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_QryLtryInfo));
			if (wDataSize!=sizeof(CMD_GP_QryLtryInfo)) return false;

			//��������
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);
			pBindParameter->bIsLtryInfoSend=true;

			//������Ϣ
			CMD_GP_QryLtryInfo * pUsrLtry=(CMD_GP_QryLtryInfo *)pData;

			//��������
			DBR_GP_UserLtry UserLtry;
			ZeroMemory(&UserLtry,sizeof(UserLtry));

			UserLtry.dwUserID = pUsrLtry->dwUserID;

			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_LOTTERY_INFO,dwSocketID,&UserLtry,sizeof(UserLtry));

			return true;
		}
	case SUB_GP_USER_LOTTERY:
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_UserLtry));
			if (wDataSize!=sizeof(CMD_GP_UserLtry)) return false;

			//������Ϣ
			CMD_GP_UserLtry * pUserLtry=(CMD_GP_UserLtry *)pData;

			//��������
			DBR_GP_UserLtry UserLtry;
			ZeroMemory(&UserLtry,sizeof(UserLtry));

			UserLtry.dwUserID = pUserLtry->dwUserID;

			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOTTERY_REQ,dwSocketID,&UserLtry,sizeof(UserLtry));

			return true;
		}
	case SUB_GP_LOTTERY_FINISH:
		{
			//��������
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);
			pBindParameter->bIsLtryInfoSend=false;

			return true;
		}
	case SUB_GPR_QUERY_MAIL_ID_LIST:
		{
//printf("@@@@@ SUB_GP_QUERY_MAIL_ID_LIST\n");
			if (wDataSize != sizeof(CMD_GPR_MailIDList))
			{
				printf("error at %s %d\n", __FUNCTION__, __LINE__);
				return true;
			}
			CMD_GPR_MailIDList * pMailIDList = (CMD_GPR_MailIDList *)pData;

			DBR_MailIDList MailIDList = {0};
			MailIDList.dwUserID = pMailIDList->dwUserID;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_MAIL_ID_LIST,dwSocketID,&MailIDList,sizeof(MailIDList));

			return true;
		}
	case SUB_GPR_QUERY_MAIL_INFO:
		{
			if (wDataSize < sizeof(BYTE))
			{
				printf("error at %s %d\n", __FUNCTION__, __LINE__);
				return true;
			}
			CMD_GPR_MailInfo * pMailInfo = (CMD_GPR_MailInfo *)pData;
			if (wDataSize != sizeof(BYTE) + sizeof(DWORD) * pMailInfo->cbCount)
			{
				printf("error at %s %d\n", __FUNCTION__, __LINE__);
				return true;
			}

			DBR_MailInfo MailInfo = {0};
			MailInfo.cbCount = pMailInfo->cbCount;
			//printf("packet size is %hu\n", wDataSize);
			//printf("count is %hhu\n", MailInfo.cbCount);
			for (BYTE i = 0; i < MailInfo.cbCount; i++)
			{
				MailInfo.dwMailID[i] = pMailInfo->dwMailID[i];
				//printf("mail id is %u\n", MailInfo.dwMailID[i]);
			}
			//MailInfo.dwUserID = pMailInfo->dwUserID;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_MAIL_INFO,dwSocketID,&MailInfo,sizeof(MailInfo));

			return true;
		}
	case SUB_GPR_QUERY_MAIL_CONTENT:
		{
			if (wDataSize != sizeof(CMD_GPR_MailContent))
			{
				printf("error at %s %d\n", __FUNCTION__, __LINE__);
				return true;
			}
			CMD_GPR_MailContent * pMailContent = (CMD_GPR_MailContent *)pData;

			DBR_MailContent MailContent = {0};
			MailContent.dwMailID = pMailContent->dwMailID;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_MAIL_CONTENT,dwSocketID,&MailContent,sizeof(MailContent));

			return true;
		}
	case SUB_GPR_QUERY_MAIL_ITEM:
		{
			if (wDataSize != sizeof(CMD_GPR_MailItem))
			{
				printf("error at %s %d\n", __FUNCTION__, __LINE__);
				return true;
			}
			CMD_GPR_MailItem * pMailItem = (CMD_GPR_MailItem *)pData;

			DBR_MailItem MailItem = {0};
			MailItem.dwMailID = pMailItem->dwMailID;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_MAIL_ITEM,dwSocketID,&MailItem,sizeof(MailItem));

			return true;
		}
	case SUB_GPR_IAP_PRODUCT_LIST:
		{
			CMD_GPR_IAPProductList IAPProductList = {0};

			WORD wPrice[12] =
			{
				6, 12, 30, 50, 98, 188, 388, 648, 18, 18, 18, 18,
			};

			LPCTSTR pszProductID[12] =
			{
				"com.youkuss.dakacoin1", "com.youkuss.dakacoin2", "com.youkuss.dakacoin3", "com.youkuss.dakacoin4",
				"com.youkuss.dakacoin5", "com.youkuss.dakacoin6", "com.youkuss.dakacoin7", "com.youkuss.dakacoin8",
				"com.youkuss.dakaa2", "com.youkuss.dakaa3",
				"com.youkuss.dakaa4", "com.youkuss.dakaa6",
			};

			for (int i = 0; i < 12; i++)
			{
				IAPProductList.product[i].wRMB = wPrice[i];
				IAPProductList.product[i].wType = i<8?0:1;
				sprintf(IAPProductList.product[i].szProductID, pszProductID[i]);
			}

			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GPO_IAP_PRODUCT_LIST,&IAPProductList,sizeof(CMD_GPR_IAPProductList));

			return true;
		}
	case SUB_GPR_CHARGE_VALUE:
		{
			CMD_GPR_ChargeValue * pChargeValue = (CMD_GPR_ChargeValue *)pData;

			//printf("userid is %u\n", pChargeValue->dwUserID);

			DBR_ChargeValue ChargeValue = {0};
			ChargeValue.dwUserID = pChargeValue->dwUserID;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CHARGE_VALUE,dwSocketID,&ChargeValue,sizeof(ChargeValue));

			return true;
		}
	case SUB_GPR_CHARGE_CANCEL:
		{
			CMD_GPR_ChargeCancel * pChargeCancel = (CMD_GPR_ChargeCancel *)pData;

			//printf("userid is %u\n", pChargeCancel->dwUserID);

			DBR_ChargeCancel ChargeCancel = {0};
			ChargeCancel.dwUserID = pChargeCancel->dwUserID;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CHARGE_CANCEL,dwSocketID,&ChargeCancel,sizeof(ChargeCancel));

			return true;
		}
	case SUB_GPR_VIP_INFO:
		{
			CMD_GPR_VIPUser * pVIPUser = (CMD_GPR_VIPUser *)pData;
			if (wDataSize != sizeof(CMD_GPR_VIPUser))
			{
				printf("error %s %d\n", __FUNCTION__, __LINE__);
				return true;
			}

			DBR_VIPUser VipUser = {0};
			VipUser.dwUserID = pVIPUser->dwUserID;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_VIP_INFO,dwSocketID,&VipUser,sizeof(VipUser));

			//CMD_GPO_VIPInfo VIPInfo = {0};
			//VIPInfo.cbVIPLevel = 3;
			//VIPInfo.dwVIPExp = 333;
			//VIPInfo.dwVIPNextExp = 5000;
			//VIPInfo.dwEverydayGift = 100000;
			//VIPInfo.dwChargeBack = 200000;

			//m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GPO_VIP_INFO,&VIPInfo,sizeof(CMD_GPO_VIPInfo));

			return true;
		}
	case SUB_GPR_VIP_EVERYDAY_GIFT:
		{
			CMD_GPR_VIPUser * pVIPUser = (CMD_GPR_VIPUser *)pData;
			if (wDataSize != sizeof(CMD_GPR_VIPUser))
			{
				printf("error %s %d\n", __FUNCTION__, __LINE__);
				return true;
			}

			DBR_VIPUser VipUser = {0};
			VipUser.dwUserID = pVIPUser->dwUserID;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_VIP_EVERYDAY_GIFT,dwSocketID,&VipUser,sizeof(VipUser));

			return true;
		}
	case SUB_GPR_VIP_CHARGE_BACK:
		{
			CMD_GPR_VIPUser * pVIPUser = (CMD_GPR_VIPUser *)pData;
			if (wDataSize != sizeof(CMD_GPR_VIPUser))
			{
				printf("error %s %d\n", __FUNCTION__, __LINE__);
				return true;
			}

			DBR_VIPUser VipUser = {0};
			VipUser.dwUserID = pVIPUser->dwUserID;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_VIP_CHARGE_BACK,dwSocketID,&VipUser,sizeof(VipUser));

			return true;
		}
	case SUB_GPR_CURRENT_SCORE:
		{
			CMD_GPR_CurrentScore * pCurrentScore = (CMD_GPR_CurrentScore *)pData;
			if (wDataSize != sizeof(CMD_GPR_CurrentScore))
			{
				printf("error %s %d\n", __FUNCTION__, __LINE__);
				return true;
			}

			DBR_CurrentScore CurrentScore = {0};
			CurrentScore.dwUserID = pCurrentScore->dwUserID;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CURRENT_SCORE,dwSocketID,&CurrentScore,sizeof(CurrentScore));

			return true;
		}
	case SUB_GPR_LOGON_GIFT:
		{
			CMD_GPR_LogonGift * pLogonGift = (CMD_GPR_LogonGift *)pData;
			if (wDataSize != sizeof(CMD_GPR_LogonGift))
			{
				printf("error %s %d\n", __FUNCTION__, __LINE__);
				return true;
			}

			DBR_LogonGift LogonGift = {0};
			LogonGift.dwUserID = pLogonGift->dwUserID;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_GIFT,dwSocketID,&LogonGift,sizeof(LogonGift));

			return true;
		}
	case SUB_GPR_CHARGE1ST_REQUEST:
		{
			CMD_GPR_Charge1stRequest * pCharge1stRequest = (CMD_GPR_Charge1stRequest *)pData;
			if (wDataSize != sizeof(CMD_GPR_Charge1stRequest))
			{
				printf("error %s %d\n", __FUNCTION__, __LINE__);
				return true;
			}

			DBR_Charge1stRequest Charge1stRequest = {0};
			Charge1stRequest.dwUserID = pCharge1stRequest->dwUserID;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CHARGE1ST_REQUEST,dwSocketID,&Charge1stRequest,sizeof(Charge1stRequest));

			return true;
		}
	case SUB_GPR_CHARGE1ST_GET:
		{
printf("@@@@@SUB_GPR_CHARGE1ST_GET\n");
			CMD_GPR_Charge1stGet * pCharge1stGet = (CMD_GPR_Charge1stGet *)pData;
			if (wDataSize != sizeof(CMD_GPR_Charge1stGet))
			{
				printf("error %s %d\n", __FUNCTION__, __LINE__);
				return true;
			}

			DBR_Charge1stGet Charge1stGet = {0};
			Charge1stGet.dwUserID = pCharge1stGet->dwUserID;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CHARGE1ST_GET,dwSocketID,&Charge1stGet,sizeof(Charge1stGet));

			return true;
		}
	case SUB_GPR_WEALTH_BAG_ENROLL:
		{
			CMD_GPR_WealthBagEnroll * pWealthBagEnroll = (CMD_GPR_WealthBagEnroll *)pData;

			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			CMD_GPO_WealthBagEnroll WealthBagEnroll = {0};

			if (time(NULL) - m_dwWealthBagStartTime < 10)
			{
				WealthBagEnroll.bCan = true;

				bool bHave = false;
				for (unsigned int i = 0; i < m_vWealthBag.size(); i++)
				{
					if (pBindParameter->dwSocketID == m_vWealthBag[i].dwSocketID)
					{
						bHave = true;
						break;
					}
				}

				if (!bHave)
				{
					tagWealthBag WealthBag = {0};
					WealthBag.dwSocketID = pBindParameter->dwSocketID;
					if (pBindParameter->dwUserID == 0)
					{
						WealthBag.dwUserID = pWealthBagEnroll->dwUserID;
						strcpy(WealthBag.szNickName, pWealthBagEnroll->szNickName);
					}
					else
					{
						WealthBag.dwUserID = pBindParameter->dwUserID;
						strcpy(WealthBag.szNickName, pBindParameter->szNickName);
					}
					m_vWealthBag.push_back(WealthBag);
				}
			}
			else
			{
				WealthBagEnroll.bCan = false;
			}
			m_pITCPNetworkEngine->SendData(dwSocketID, MDM_GP_USER_SERVICE, SUB_GPO_WEALTH_BAG_ENROLL, &WealthBagEnroll, sizeof(CMD_GPO_WealthBagEnroll));

			return true;
		}
	case SUB_GP_WEALTH_BAG_SCORE:
		{
			if (sizeof(CMD_GPR_WealthBagScore) != wDataSize)
			{
				return true;
			}

			if (!m_bWealthBagRunning)
			{
				return true;
			}

			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

			CMD_GPR_WealthBagScore * pWealthBagResult = (CMD_GPR_WealthBagScore *)pData;

			DWORD dwUserID = pBindParameter->dwUserID;
			if (dwUserID == 0)
			{
				dwUserID = pWealthBagResult->dwUserID;
			}

			//�ж��Ƿ��Ѵ���
			bool bHave = false;
			for (unsigned int i = 0; i < m_vWealthBag.size(); i++)
			{
				if (dwUserID == m_vWealthBag[i].dwUserID)
				{
					bHave = true;
					m_vWealthBag[i].nScore += pWealthBagResult->nScore;
					break;
				}
			}

			return true;
		}
	case SUB_GPR_SHARE_INFO:
		{
			CMD_GPO_ShareInfo ShareInfo = {0};
			_snprintf(ShareInfo.szURL, sizeof(ShareInfo.szURL), "%s", "http://lb.66y.com");
			_snprintf(ShareInfo.szContent, sizeof(ShareInfo.szContent), "%s", "�������������������ֵ1000Ԫ�������н�����������");
			m_pITCPNetworkEngine->SendData(dwSocketID, MDM_GP_USER_SERVICE, SUB_GPO_SHARE_INFO, &ShareInfo, sizeof(ShareInfo));
			return true;
		}
	case SUB_GPR_MBCODE_VERIFY:
		{
			return OnTCPNetworkSubPCMBCODEVERIFY(pData,wDataSize,dwSocketID);
		}
	case SUB_GPR_MOBILE_BIND:
		{
			return OnTCPNetworkSubPCMoBileCode(pData,wDataSize,dwSocketID);
		}
	case SUB_GPR_MOBILE_BIND_VERIFY:
		{
			return OnTCPNetworkSubPCCodeVerification(pData,wDataSize,dwSocketID);
		}
	case SUB_GPR_CHECK_MOBILE_EXIT:
		{
			return OnTCPNetworkSubPCMobileCheckExit(pData,wDataSize,dwSocketID);
		}
	case SUB_GPR_BROADCAST_SPEED:  //����㲥�ٶ�
		{
			return OnTCPNetworkSubPCBroadCastSpeed(pData,wDataSize,dwSocketID);
		}
	case SUB_GPR_TYPE_ITEM_COUNT: 
		{
			return OnTCPNetworkSubPCItemCount(pData,wDataSize,dwSocketID);
		}
	//case SUB_GPR_IAP_TRANSACTIONRECEIPT:
	//	{
	//		unsigned char * pBuffer = (unsigned char *)pData;
	//		__int64 * pSize = (__int64 *)pData;
	//		printf("size is %lld\n", *pSize);
	//		if (*pSize < 4096)
	//		{
	//			printf("receipt is %s\n", &pBuffer[8]);
	//		}

	//		return true;
	//	}
	case SUB_GPR_BROAD_LABA:  //���ȹ㲥
		{
			return OnTCPNetworkSubPCBroadLaBa(pData,wDataSize,dwSocketID);
		}
	case SUB_GPR_MOBILE_CHARGE: 
		{
			return OnTCPNetworkSubPCChangePhoneFare(pData,wDataSize,dwSocketID);
		}
	case SUB_GPR_GET_BINDREWARD_NEWGUID:
		{
			return OnTCPNetworkGetBindRewardNewGuid(pData,wDataSize,dwSocketID);
		}
	case SUB_GPR_SET_SUPER_NEWGUID:
		{
			return OnTCPNetworkSetSuperNewGuid(pData,wDataSize,dwSocketID);
		}
	//case SUB_GR_MATCH_GET_MATCH_PRIZE:
	//	{
	//		return OnTCPNetworkSubMatchReward(pData,wDataSize,dwSocketID);
	//	}
	}

	return false;
}

//Զ�̴���
bool CAttemperEngineSink::OnTCPNetworkMainPCRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_C_SEARCH_CORRESPOND:	//Э������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GP_C_SearchCorrespond));
			if (wDataSize!=sizeof(CMD_GP_C_SearchCorrespond)) return false;

			//������Ϣ
			CMD_GP_C_SearchCorrespond * pSearchCorrespond=(CMD_GP_C_SearchCorrespond *)pData;
			pSearchCorrespond->szNickName[CountArray(pSearchCorrespond->szNickName)-1]=0;

			//��������
			CMD_CS_C_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//���ӱ���
			SearchCorrespond.dwSocketID=dwSocketID;
			SearchCorrespond.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//���ұ���
			SearchCorrespond.dwGameID=pSearchCorrespond->dwGameID;
			lstrcpyn(SearchCorrespond.szNickName,pSearchCorrespond->szNickName,CountArray(SearchCorrespond.szNickName));

			//��������
			m_pITCPSocketService->SendData(MDM_CS_REMOTE_SERVICE,SUB_CS_C_SEARCH_CORRESPOND,&SearchCorrespond,sizeof(SearchCorrespond));

			return true;
		}
	}

	return false;
}

//��¼����
bool CAttemperEngineSink::OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_LOGON_GAMEID:		//I D ��¼
		{
			return OnTCPNetworkSubMBLogonGameID(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_LOGON_ACCOUNTS:		//�ʺŵ�¼
		{
			return OnTCPNetworkSubMBLogonAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_REGISTER_ACCOUNTS:	//�ʺ�ע��
		{
			return OnTCPNetworkSubMBRegisterAccounts(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//�б���
bool CAttemperEngineSink::OnTCPNetworkMainMBServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	return false;
}

//��վ��Ϣ
bool CAttemperEngineSink::OnTCPNetworkMainWebSite(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_WEB_CHARGE_VALUE:
		{
			////�����ַ���
			//printf("web\n");
			//char szTemp[1024] = {0};
			//memcpy(szTemp, pData, wDataSize);
			//szTemp[wDataSize] = 0;
			//printf("%s\n", szTemp);
			//for (WORD i = 0; i < wDataSize; i++)
			//{
			//	unsigned char * pByte = (unsigned char *)pData;
			//	printf("%02hhd ", pByte[i]);
			//}
			//printf("\n");

			printf("wDataSize is %hu\n", wDataSize);

			if (wDataSize != sizeof(CMD_GP_WebChargeValue))
			{
				printf("error %s %d\n", __FUNCTION__, __LINE__);
				return true;
			}

			CMD_GP_WebChargeValue * pWebChargeValue = (CMD_GP_WebChargeValue *)pData;

			//��֤
			if (pWebChargeValue->dwUserID == 0 || pWebChargeValue->lChargeValue > 100000000)
			{
				printf("error %s %d\n", __FUNCTION__, __LINE__);
				return true;
			}

			DBR_WebChargeValue WebChargeValue = {0};
			WebChargeValue.dwUserID = pWebChargeValue->dwUserID;
			WebChargeValue.lChargeValue = pWebChargeValue->lChargeValue;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_WEB_CHARGE_VALUE,dwSocketID,&WebChargeValue,sizeof(WebChargeValue));

			return true;
		}
	}

	return true;
}

//I D ��¼
bool CAttemperEngineSink::OnTCPNetworkSubPCLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_LogonGameID));
	if (wDataSize<sizeof(CMD_GP_LogonGameID))
	{
		if (wDataSize<sizeof(CMD_GP_LogonGameID)-sizeof(BYTE))
			return false;
	}

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_GP_LogonGameID * pLogonGameID=(CMD_GP_LogonGameID *)pData;
	pLogonGameID->szPassword[CountArray(pLogonGameID->szPassword)-1]=0;
	pLogonGameID->szPassPortID[CountArray(pLogonGameID->szPassPortID)-1]=0;
	pLogonGameID->szMachineID[CountArray(pLogonGameID->szMachineID)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonGameID->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pLogonGameID->dwPlazaVersion,dwSocketID,((pLogonGameID->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//��������
	DBR_GP_LogonGameID LogonGameID;
	ZeroMemory(&LogonGameID,sizeof(LogonGameID));

	//������Ϣ
	LogonGameID.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonGameID.dwGameID=pLogonGameID->dwGameID;
	LogonGameID.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonGameID.szPassword,pLogonGameID->szPassword,CountArray(LogonGameID.szPassword));
	lstrcpyn(LogonGameID.szPassPortID,pLogonGameID->szPassPortID,CountArray(LogonGameID.szPassPortID));
	lstrcpyn(LogonGameID.szMachineID,pLogonGameID->szMachineID,CountArray(LogonGameID.szMachineID));
	LogonGameID.cbNeeValidateMBCard=(pLogonGameID->cbValidateFlags&MB_VALIDATE_FLAGS);

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_GAMEID,dwSocketID,&LogonGameID,sizeof(LogonGameID));

	return true;
}

//�ʺŵ�¼
bool CAttemperEngineSink::OnTCPNetworkSubPCLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_LogonAccounts));
	if (wDataSize<sizeof(CMD_GP_LogonAccounts))
	{
		if (wDataSize<sizeof(CMD_GP_LogonAccounts)-sizeof(BYTE))
			return false;
	}

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_GP_LogonAccounts * pLogonAccounts=(CMD_GP_LogonAccounts *)pData;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szPassPortID[CountArray(pLogonAccounts->szPassPortID)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//�汾�ж�
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pLogonAccounts->dwPlazaVersion,dwSocketID,((pLogonAccounts->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//��������
	DBR_GP_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//������Ϣ
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szPassPortID,pLogonAccounts->szPassPortID,CountArray(LogonAccounts.szPassPortID));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));
	LogonAccounts.cbNeeValidateMBCard=(pLogonAccounts->cbValidateFlags&MB_VALIDATE_FLAGS);

	DBR_GP_RecordLogon RecordLogon = {0};
	lstrcpyn(RecordLogon.szAccounts, pLogonAccounts->szAccounts, CountArray(RecordLogon.szAccounts));
	lstrcpyn(RecordLogon.szMachineID, pLogonAccounts->szMachineID, CountArray(RecordLogon.szMachineID));

	BYTE * pByte = (BYTE *)(&LogonAccounts.dwClientAddr);
	sprintf(RecordLogon.szWAN, "%hhd.%hhd.%hhd.%hhd", pByte[0], pByte[1], pByte[2], pByte[3]);

	if (lstrcmp(pLogonAccounts->szPassPortID, "iphone") == 0)
	{
		RecordLogon.cbDeviceType = 20;
		RecordLogon.cbChannel = 10;
	}
	else if (lstrcmp(pLogonAccounts->szPassPortID, "wapiPhone") == 0)
	{
		RecordLogon.cbDeviceType = 20;
		RecordLogon.cbChannel = 20;
	}
	else if (lstrcmp(pLogonAccounts->szPassPortID, "mwapiPhone") == 0)
	{
		RecordLogon.cbDeviceType = 20;
		RecordLogon.cbChannel = 30;
	}
	else if (lstrcmp(pLogonAccounts->szPassPortID, "android") == 0)
	{
		RecordLogon.cbDeviceType = 30;
		RecordLogon.cbChannel = 10;
	}
	else if (lstrcmp(pLogonAccounts->szPassPortID, "wapAndroid") == 0)
	{
		RecordLogon.cbDeviceType = 30;
		RecordLogon.cbChannel = 20;
	}
	else if (lstrcmp(pLogonAccounts->szPassPortID, "mwapAndroid") == 0)
	{
		RecordLogon.cbDeviceType = 30;
		RecordLogon.cbChannel = 30;
	}

	bool bHaveRecord = false;

	//��չ��Ϣ
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pLogonAccounts+1,wDataSize-sizeof(CMD_GP_LogonAccounts));
	while (true)
	{
		//��ȡ����
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;

		char szBuffer[2048] = {0};
		//��ȡ����
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_USERDATA1:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData1, szBuffer, CountArray(RecordLogon.szUserData1));
				}
				break;
			}
		case DTP_GP_USERDATA2:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData2, szBuffer, CountArray(RecordLogon.szUserData2));
				}
				break;
			}
		case DTP_GP_USERDATA3:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData3, szBuffer, CountArray(RecordLogon.szUserData3));
				}
				break;
			}
		case DTP_GP_USERDATA4:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData4, szBuffer, CountArray(RecordLogon.szUserData4));
				}
				break;
			}
		case DTP_GP_USERDATA5:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData5, szBuffer, CountArray(RecordLogon.szUserData5));
				}
				break;
			}
		case DTP_GP_USERDATA6:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData6, szBuffer, CountArray(RecordLogon.szUserData6));
				}
				break;
			}
		case DTP_GP_USERDATA7:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData7, szBuffer, CountArray(RecordLogon.szUserData7));
				}
				break;
			}
		case DTP_GP_USERDATA8:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData8, szBuffer, CountArray(RecordLogon.szUserData8));
				}
				break;
			}
		case DTP_GP_LOCALIP:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szLAN, szBuffer, CountArray(RecordLogon.szLAN));
				}
				break;
			}
		case DTP_GP_CLIENTVERSION:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szClientVersion, szBuffer, CountArray(RecordLogon.szClientVersion));
				}
				break;
			}
		case DTP_GP_PASSWORD:
			{
				if (DataDescribe.wDataSize > 2)
				{
					bHaveRecord = true;
					DTP_GP_Password * pPassword = (DTP_GP_Password *)pDataBuffer;
					//BYTE cbKey = 0xAA;
					//for (WORD i = 0; i < pPassword->wCount; i++)
					//{
					//	printf("%hhx", (unsigned char)pPassword->cbData[i]);
					//	pPassword->cbData[i] ^= cbKey;
					//	cbKey++;
					//}
					//printf("\n");
					for (WORD i = 0; i < pPassword->wCount; i++)
					{
						char szTemp[8];
						sprintf(szTemp, "%02x", (unsigned char)pPassword->cbData[i]);
						strcat(szBuffer, szTemp);
					}
					szBuffer[pPassword->wCount*2] = 0;
					lstrcpyn(RecordLogon.szLogonPass, szBuffer, CountArray(RecordLogon.szLogonPass));
				}
				break;
			}
		//case DTP_GP_POSITION:
		//	{
		//		if (DataDescribe.wDataSize > 0)
		//		{
		//			DTP_GP_Position * pPosition = (DTP_GP_Position *)pDataBuffer;
		//			printf("longitude is %f latitude is %f\n", pPosition->longitude, pPosition->latitude);
		//		}
		//		break;
		//	}
		}
	}

	//Ͷ������
	if (m_nSendMaxNum<LOGIN_MAX_SEND_TO_DATABASE_NUM)
	{
		m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));
		m_nSendMaxNum++;
		CVerification::GetInstance()->OnRecordSendLoginNum(m_nSendMaxNum);
	}
	else
	{
		std::vector<tagLoginQueueInfo>::iterator iter;
		for (iter=m_LoginQueueInfo.begin(); iter!=m_LoginQueueInfo.end(); iter++)
		{
			if (iter->dwSocketId == dwSocketID)
			{
				return true;
			}
		}
		tagLoginQueueInfo tagLoginInfo;
		tagLoginInfo.dwSocketId = dwSocketID;
		tagLoginInfo.cbNeeValidateMBCard = LogonAccounts.cbNeeValidateMBCard;
		tagLoginInfo.dwClientAddr = LogonAccounts.dwClientAddr;
		tagLoginInfo.pBindParameter = LogonAccounts.pBindParameter;
		lstrcpyn(tagLoginInfo.szAccounts,LogonAccounts.szAccounts,CountArray(tagLoginInfo.szAccounts));
		lstrcpyn(tagLoginInfo.szPassword,LogonAccounts.szPassword,CountArray(tagLoginInfo.szPassword));
		lstrcpyn(tagLoginInfo.szPassPortID,LogonAccounts.szPassPortID,CountArray(tagLoginInfo.szPassPortID));
		lstrcpyn(tagLoginInfo.szMachineID,LogonAccounts.szMachineID,CountArray(tagLoginInfo.szMachineID));
		m_LoginQueueInfo.push_back(tagLoginInfo);
		CMD_GP_LoginingInfo loginInfo;
		loginInfo.wLoginingNum = m_LoginQueueInfo.size()+m_nSendMaxNum;
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_LOGINING_INFO,&loginInfo,sizeof(CMD_GP_LoginingInfo));
	}
	

	return true;
}

//�ʺ�ע��
bool CAttemperEngineSink::OnTCPNetworkSubPCRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_RegisterAccounts));
	if (wDataSize<sizeof(CMD_GP_RegisterAccounts))
	{
		if (wDataSize<sizeof(CMD_GP_RegisterAccounts)-sizeof(BYTE))
			return false;
	}

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_GP_RegisterAccounts * pRegisterAccounts=(CMD_GP_RegisterAccounts *)pData;
	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
	pRegisterAccounts->szSpreader[CountArray(pRegisterAccounts->szSpreader)-1]=0;
	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
	pRegisterAccounts->szInsurePass[CountArray(pRegisterAccounts->szInsurePass)-1]=0;
	pRegisterAccounts->szPassPortID[CountArray(pRegisterAccounts->szPassPortID)-1]=0;
	pRegisterAccounts->szCompellation[CountArray(pRegisterAccounts->szCompellation)-1]=0;
	pRegisterAccounts->szMobilePhone[CountArray(pRegisterAccounts->szMobilePhone)-1]=0;

	//--------
	//���д�
	CString sNickName = pRegisterAccounts->szNickName;
	for (int i = 0; i < (sizeof(pszSensitiveWords)/sizeof(pszSensitiveWords[0])); i++)
	{
		if (sNickName.Find(pszSensitiveWords[i]) != -1)
		{
			printf("sensitive words %s %s %d\n", sNickName, pszSensitiveWords[i], i);
			CMD_GP_LogonFailure LogonFailure = {0};

			LogonFailure.lResultCode = 0;
			lstrcpyn(LogonFailure.szDescribeString,TEXT("�����ǳ��а������дʻ㣬ע��ʧ�ܣ�"),CountArray(LogonFailure.szDescribeString));

			WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_LOGON_FAILURE,&LogonFailure,wSendSize);

			return true;
		}
	}
	//--------

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pRegisterAccounts->dwPlazaVersion,dwSocketID,((pRegisterAccounts->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//��������
	DBR_GP_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//������Ϣ
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	RegisterAccounts.wFaceID=pRegisterAccounts->wFaceID;
	RegisterAccounts.cbGender=pRegisterAccounts->cbGender;
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szAccounts,pRegisterAccounts->szAccounts,CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName,pRegisterAccounts->szNickName,CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szSpreader,pRegisterAccounts->szSpreader,CountArray(RegisterAccounts.szSpreader));
	lstrcpyn(RegisterAccounts.szMachineID,pRegisterAccounts->szMachineID,CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szLogonPass,pRegisterAccounts->szLogonPass,CountArray(RegisterAccounts.szLogonPass));
	lstrcpyn(RegisterAccounts.szInsurePass,pRegisterAccounts->szInsurePass,CountArray(RegisterAccounts.szInsurePass));
	lstrcpyn(RegisterAccounts.szPassPortID,pRegisterAccounts->szPassPortID,CountArray(RegisterAccounts.szPassPortID));
	lstrcpyn(RegisterAccounts.szCompellation,pRegisterAccounts->szCompellation,CountArray(RegisterAccounts.szCompellation));
	lstrcpyn(RegisterAccounts.szMobilePhone,pRegisterAccounts->szMobilePhone,CountArray(RegisterAccounts.szMobilePhone));

	DBR_GP_RecordLogon RecordLogon = {0};
	lstrcpyn(RecordLogon.szAccounts, pRegisterAccounts->szAccounts, CountArray(RecordLogon.szAccounts));
	lstrcpyn(RecordLogon.szMachineID, pRegisterAccounts->szMachineID, CountArray(RecordLogon.szMachineID));

	BYTE * pByte = (BYTE *)(&RegisterAccounts.dwClientAddr);
	sprintf(RecordLogon.szWAN, "%hhd.%hhd.%hhd.%hhd", pByte[0], pByte[1], pByte[2], pByte[3]);

	if (lstrcmp(pRegisterAccounts->szPassPortID, "iphone") == 0)
	{
		RecordLogon.cbDeviceType = 20;
		RecordLogon.cbChannel = 10;
	}
	else if (lstrcmp(pRegisterAccounts->szPassPortID, "wapiPhone") == 0)
	{
		RecordLogon.cbDeviceType = 20;
		RecordLogon.cbChannel = 20;
	}
	else if (lstrcmp(pRegisterAccounts->szPassPortID, "mwapiPhone") == 0)
	{
		RecordLogon.cbDeviceType = 20;
		RecordLogon.cbChannel = 30;
	}
	else if (lstrcmp(pRegisterAccounts->szPassPortID, "android") == 0)
	{
		RecordLogon.cbDeviceType = 30;
		RecordLogon.cbChannel = 10;
	}
	else if (lstrcmp(pRegisterAccounts->szPassPortID, "wapAndroid") == 0)
	{
		RecordLogon.cbDeviceType = 30;
		RecordLogon.cbChannel = 20;
	}
	else if (lstrcmp(pRegisterAccounts->szPassPortID, "mwapAndroid") == 0)
	{
		RecordLogon.cbDeviceType = 30;
		RecordLogon.cbChannel = 30;
	}

	bool bHaveRecord = false;

	//��չ��Ϣ
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pRegisterAccounts+1,wDataSize-sizeof(CMD_GP_RegisterAccounts));
	while (true)
	{
		//��ȡ����
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;

		char szBuffer[2048] = {0};
		//��ȡ����
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_REG_AGENTID:	//�Ƽ�����Ϣ
			{
				ASSERT(DataDescribe.wDataSize==sizeof(DTP_GP_AgentInfo));
				if (DataDescribe.wDataSize==sizeof(DTP_GP_AgentInfo))
				{
					DTP_GP_AgentInfo * pAgentInfo=(DTP_GP_AgentInfo *)pDataBuffer;
					RegisterAccounts.dwAgentID=pAgentInfo->dwAgentID;
				}
				break;
			}
		case DTP_GP_USERDATA1:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData1, szBuffer, CountArray(RecordLogon.szUserData1));
				}
				break;
			}
		case DTP_GP_USERDATA2:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData2, szBuffer, CountArray(RecordLogon.szUserData2));
				}
				break;
			}
		case DTP_GP_USERDATA3:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData3, szBuffer, CountArray(RecordLogon.szUserData3));
				}
				break;
			}
		case DTP_GP_USERDATA4:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData4, szBuffer, CountArray(RecordLogon.szUserData4));
				}
				break;
			}
		case DTP_GP_USERDATA5:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData5, szBuffer, CountArray(RecordLogon.szUserData5));
				}
				break;
			}
		case DTP_GP_USERDATA6:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData6, szBuffer, CountArray(RecordLogon.szUserData6));
				}
				break;
			}
		case DTP_GP_USERDATA7:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData7, szBuffer, CountArray(RecordLogon.szUserData7));
				}
				break;
			}
		case DTP_GP_USERDATA8:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData8, szBuffer, CountArray(RecordLogon.szUserData8));
				}
				break;
			}
		case DTP_GP_LOCALIP:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szLAN, szBuffer, CountArray(RecordLogon.szLAN));
				}
				break;
			}
		case DTP_GP_CLIENTVERSION:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szClientVersion, szBuffer, CountArray(RecordLogon.szClientVersion));
				}
				break;
			}
		case DTP_GP_CLIENTIDFA:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RegisterAccounts.szIDFA, szBuffer, CountArray(RegisterAccounts.szIDFA));
				}
				break;
			}
		case DTP_GP_PASSWORD:
			{
				if (DataDescribe.wDataSize > 2)
				{
					bHaveRecord = true;
					DTP_GP_Password * pPassword = (DTP_GP_Password *)pDataBuffer;
					//BYTE cbKey = 0xAA;
					//for (WORD i = 0; i < pPassword->wCount; i++)
					//{
					//	printf("%hhx", (unsigned char)pPassword->cbData[i]);
					//	pPassword->cbData[i] ^= cbKey;
					//	cbKey++;
					//}
					//printf("\n");
					for (WORD i = 0; i < pPassword->wCount; i++)
					{
						char szTemp[8];
						sprintf(szTemp, "%02x", (unsigned char)pPassword->cbData[i]);
						strcat(szBuffer, szTemp);
					}
					szBuffer[pPassword->wCount*2] = 0;
					lstrcpyn(RecordLogon.szLogonPass, szBuffer, CountArray(RecordLogon.szLogonPass));
				}
				break;
			}
			//case DTP_GP_POSITION:
			//	{
			//		if (DataDescribe.wDataSize > 0)
			//		{
			//			DTP_GP_Position * pPosition = (DTP_GP_Position *)pDataBuffer;
			//			printf("longitude is %f latitude is %f\n", pPosition->longitude, pPosition->latitude);
			//		}
			//		break;
			//	}
		}
	}

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_REGISTER_ACCOUNTS,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));
	return true;
}

//�����������
VOID CAttemperEngineSink::GenRandCode(TCHAR szRandCode[])
{
	//������λ����ַ���
	TCHAR szCharBase[100];
	INT iCharNum=0;

	lstrcpyn(szCharBase,TEXT("abcdefghijklmnopqrstuvwxyz0123456789"),CountArray(szCharBase));

	for(iCharNum=0;iCharNum<6;iCharNum++)
	{
		szRandCode[iCharNum]=szCharBase[rand()%10];
	}

	szRandCode[iCharNum]='\0';

	return;
}

//�ο͵�¼
bool CAttemperEngineSink::OnTCPNetworkSubPCGuestReg( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_Guest_Reg));
	if (wDataSize<sizeof(CMD_GP_Guest_Reg))
	{
		if (wDataSize<sizeof(CMD_GP_Guest_Reg)-sizeof(BYTE))
			return false;
	}

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_GP_Guest_Reg * pGuestRegInfo=(CMD_GP_Guest_Reg *)pData;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pGuestRegInfo->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pGuestRegInfo->dwPlazaVersion,dwSocketID,((pGuestRegInfo->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//��������
	DBR_GP_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//������Ϣ
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//�������� ��������
	TCHAR szLogonPass[LEN_PASSWORD];
	RegisterAccounts.wFaceID=1;
	RegisterAccounts.cbGender=1;
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szAccounts,pGuestRegInfo->szAccounts,CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName,pGuestRegInfo->szNickName,CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szMachineID,pGuestRegInfo->szMachineID,CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szPassPortID,pGuestRegInfo->szPassPortID,CountArray(RegisterAccounts.szPassPortID));
	GenRandCode(szLogonPass);

	//���ݼ���
	CWHEncrypt::MD5Encrypt(szLogonPass,RegisterAccounts.szLogonPass);
	lstrcpyn(RegisterAccounts.szInsurePass,RegisterAccounts.szLogonPass,CountArray(RegisterAccounts.szInsurePass));

	DBR_GP_RecordLogon RecordLogon = {0};
	lstrcpyn(RecordLogon.szAccounts, pGuestRegInfo->szAccounts, CountArray(RecordLogon.szAccounts));
	lstrcpyn(RecordLogon.szMachineID, pGuestRegInfo->szMachineID, CountArray(RecordLogon.szMachineID));

	BYTE * pByte = (BYTE *)(&RegisterAccounts.dwClientAddr);
	sprintf(RecordLogon.szWAN, "%hhd.%hhd.%hhd.%hhd", pByte[0], pByte[1], pByte[2], pByte[3]);

	if (lstrcmp(pGuestRegInfo->szPassPortID, "iphone") == 0)
	{
		RecordLogon.cbDeviceType = 20;
		RecordLogon.cbChannel = 10;
	}
	else if (lstrcmp(pGuestRegInfo->szPassPortID, "wapiPhone") == 0)
	{
		RecordLogon.cbDeviceType = 20;
		RecordLogon.cbChannel = 20;
	}
	else if (lstrcmp(pGuestRegInfo->szPassPortID, "mwapiPhone") == 0)
	{
		RecordLogon.cbDeviceType = 20;
		RecordLogon.cbChannel = 30;
	}
	else if (lstrcmp(pGuestRegInfo->szPassPortID, "android") == 0)
	{
		RecordLogon.cbDeviceType = 30;
		RecordLogon.cbChannel = 10;
	}
	else if (lstrcmp(pGuestRegInfo->szPassPortID, "wapAndroid") == 0)
	{
		RecordLogon.cbDeviceType = 30;
		RecordLogon.cbChannel = 20;
	}
	else if (lstrcmp(pGuestRegInfo->szPassPortID, "mwapAndroid") == 0)
	{
		RecordLogon.cbDeviceType = 30;
		RecordLogon.cbChannel = 30;
	}

	bool bHaveRecord = false;

	//��չ��Ϣ
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pGuestRegInfo+1,wDataSize-sizeof(CMD_GP_Guest_Reg));
	while (true)
	{
		//��ȡ����
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;

		char szBuffer[2048] = {0};
		//��ȡ����
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_USERDATA1:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData1, szBuffer, CountArray(RecordLogon.szUserData1));
				}
				break;
			}
		case DTP_GP_USERDATA2:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData2, szBuffer, CountArray(RecordLogon.szUserData2));
				}
				break;
			}
		case DTP_GP_USERDATA3:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData3, szBuffer, CountArray(RecordLogon.szUserData3));
				}
				break;
			}
		case DTP_GP_USERDATA4:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData4, szBuffer, CountArray(RecordLogon.szUserData4));
				}
				break;
			}
		case DTP_GP_USERDATA5:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData5, szBuffer, CountArray(RecordLogon.szUserData5));
				}
				break;
			}
		case DTP_GP_USERDATA6:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData6, szBuffer, CountArray(RecordLogon.szUserData6));
				}
				break;
			}
		case DTP_GP_USERDATA7:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData7, szBuffer, CountArray(RecordLogon.szUserData7));
				}
				break;
			}
		case DTP_GP_USERDATA8:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData8, szBuffer, CountArray(RecordLogon.szUserData8));
				}
				break;
			}
		case DTP_GP_LOCALIP:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szLAN, szBuffer, CountArray(RecordLogon.szLAN));
				}
				break;
			}
		case DTP_GP_CLIENTVERSION:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szClientVersion, szBuffer, CountArray(RecordLogon.szClientVersion));
				}
				break;
			}
		case DTP_GP_CLIENTIDFA:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RegisterAccounts.szIDFA, szBuffer, CountArray(RegisterAccounts.szIDFA));
				}
				break;
			}
		case DTP_GP_PASSWORD:
			{
				if (DataDescribe.wDataSize > 2)
				{
					bHaveRecord = true;
					DTP_GP_Password * pPassword = (DTP_GP_Password *)pDataBuffer;
					//BYTE cbKey = 0xAA;
					//for (WORD i = 0; i < pPassword->wCount; i++)
					//{
					//	printf("%hhx", (unsigned char)pPassword->cbData[i]);
					//	pPassword->cbData[i] ^= cbKey;
					//	cbKey++;
					//}
					//printf("\n");
					for (WORD i = 0; i < pPassword->wCount; i++)
					{
						char szTemp[8];
						sprintf(szTemp, "%02x", (unsigned char)pPassword->cbData[i]);
						strcat(szBuffer, szTemp);
					}
					szBuffer[pPassword->wCount*2] = 0;
					lstrcpyn(RecordLogon.szLogonPass, szBuffer, CountArray(RecordLogon.szLogonPass));
				}
				break;
			}
			//case DTP_GP_POSITION:
			//	{
			//		if (DataDescribe.wDataSize > 0)
			//		{
			//			DTP_GP_Position * pPosition = (DTP_GP_Position *)pDataBuffer;
			//			printf("longitude is %f latitude is %f\n", pPosition->longitude, pPosition->latitude);
			//		}
			//		break;
			//	}
		}
	}
	
	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GUEST_REGISTER,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	return true;
}

//I D ��¼
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonGameID));
	if (wDataSize<sizeof(CMD_MB_LogonGameID)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_LogonGameID * pLogonGameID=(CMD_MB_LogonGameID *)pData;
	pLogonGameID->szPassword[CountArray(pLogonGameID->szPassword)-1]=0;
	pLogonGameID->szMachineID[CountArray(pLogonGameID->szMachineID)-1]=0;
	pLogonGameID->szMobilePhone[CountArray(pLogonGameID->szMobilePhone)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonGameID->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonGameID->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(pLogonGameID->cbDeviceType,pLogonGameID->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_MB_LogonGameID LogonGameID;
	ZeroMemory(&LogonGameID,sizeof(LogonGameID));

	//������Ϣ
	LogonGameID.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonGameID.dwGameID=pLogonGameID->dwGameID;
	LogonGameID.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonGameID.szPassword,pLogonGameID->szPassword,CountArray(LogonGameID.szPassword));
	lstrcpyn(LogonGameID.szMachineID,pLogonGameID->szMachineID,CountArray(LogonGameID.szMachineID));
	lstrcpyn(LogonGameID.szMobilePhone,pLogonGameID->szMobilePhone,CountArray(LogonGameID.szMobilePhone));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_GAMEID,dwSocketID,&LogonGameID,sizeof(LogonGameID));

	return true;
}

//�ʺŵ�¼
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonAccounts));
	if (wDataSize<sizeof(CMD_MB_LogonAccounts)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_LogonAccounts * pLogonAccounts=(CMD_MB_LogonAccounts *)pData;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;
	pLogonAccounts->szMobilePhone[CountArray(pLogonAccounts->szMobilePhone)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonAccounts->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//�汾�ж�
	if (CheckPlazaVersion(pLogonAccounts->cbDeviceType,pLogonAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_MB_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//������Ϣ
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	LogonAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));
	lstrcpyn(LogonAccounts.szMobilePhone,pLogonAccounts->szMobilePhone,CountArray(LogonAccounts.szMobilePhone));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	return true;
}

//�ʺ�ע��
bool CAttemperEngineSink::OnTCPNetworkSubMBRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_MB_RegisterAccounts));
	if (wDataSize<sizeof(CMD_MB_RegisterAccounts)) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_MB_RegisterAccounts * pRegisterAccounts=(CMD_MB_RegisterAccounts *)pData;
	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
	pRegisterAccounts->szInsurePass[CountArray(pRegisterAccounts->szInsurePass)-1]=0;
	pRegisterAccounts->szMobilePhone[CountArray(pRegisterAccounts->szMobilePhone)-1]=0;

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pRegisterAccounts->wModuleID;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(pRegisterAccounts->cbDeviceType,pRegisterAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//��������
	DBR_MB_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//������Ϣ
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	RegisterAccounts.wFaceID=pRegisterAccounts->wFaceID;
	RegisterAccounts.cbGender=pRegisterAccounts->cbGender;
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szAccounts,pRegisterAccounts->szAccounts,CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName,pRegisterAccounts->szNickName,CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szMachineID,pRegisterAccounts->szMachineID,CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szLogonPass,pRegisterAccounts->szLogonPass,CountArray(RegisterAccounts.szLogonPass));
	lstrcpyn(RegisterAccounts.szInsurePass,pRegisterAccounts->szInsurePass,CountArray(RegisterAccounts.szInsurePass));
	lstrcpyn(RegisterAccounts.szMobilePhone,pRegisterAccounts->szMobilePhone,CountArray(RegisterAccounts.szMobilePhone));

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_REGISTER_ACCOUNTS,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	return true;
}

//��¼�ɹ�
bool CAttemperEngineSink::OnDBPCLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	DBO_GP_LogonSuccess * pDBOLogonSuccess=(DBO_GP_LogonSuccess *)pData;
	if (pDBOLogonSuccess->bIsLogin)
	{
		m_nSendMaxNum--;
		CVerification::GetInstance()->OnRecordFinishLoginNum(m_nSendMaxNum);
	}
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;
	tagBindParameter * pBindParameter = (m_pBindParameter+LOWORD(dwContextID));

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	CMD_GP_LogonSuccess * pCMDLogonSuccess=(CMD_GP_LogonSuccess *)cbDataBuffer;

	//���Ͷ���
	WORD wHeadSize=sizeof(CMD_GP_LogonSuccess);
	CSendPacketHelper SendPacket(cbDataBuffer+wHeadSize,sizeof(cbDataBuffer)-wHeadSize);

	//���ñ���
	ZeroMemory(pCMDLogonSuccess,sizeof(CMD_GP_LogonSuccess));

	//��������
	pCMDLogonSuccess->wFaceID=pDBOLogonSuccess->wFaceID;
	pCMDLogonSuccess->cbGender=pDBOLogonSuccess->cbGender;
	pCMDLogonSuccess->dwGameID=pDBOLogonSuccess->dwGameID;
	pCMDLogonSuccess->dwUserID=pDBOLogonSuccess->dwUserID;
	pCMDLogonSuccess->dwCustomID=pDBOLogonSuccess->dwCustomID;
	pCMDLogonSuccess->dwUserMedal=pDBOLogonSuccess->dwUserMedal;
	pCMDLogonSuccess->dwExperience=pDBOLogonSuccess->dwExperience;
	pCMDLogonSuccess->dwLoveLiness=pDBOLogonSuccess->dwLoveLiness;
	pCMDLogonSuccess->cbMoorMachine=pDBOLogonSuccess->cbMoorMachine;
	lstrcpyn(pCMDLogonSuccess->szAccounts,pDBOLogonSuccess->szAccounts,CountArray(pCMDLogonSuccess->szAccounts));
	lstrcpyn(pCMDLogonSuccess->szNickName,pDBOLogonSuccess->szNickName,CountArray(pCMDLogonSuccess->szNickName));

	pBindParameter->dwUserID = pDBOLogonSuccess->dwUserID;
	lstrcpyn(pBindParameter->szNickName, pDBOLogonSuccess->szNickName, CountArray(pBindParameter->szNickName));

	//�û��ɼ�
	pCMDLogonSuccess->lUserScore=pDBOLogonSuccess->lUserScore;
	pCMDLogonSuccess->lUserInsure=pDBOLogonSuccess->lUserInsure;

	//������Ϣ
	pCMDLogonSuccess->cbShowServerStatus=m_bShowServerStatus?1:0;

	//��Ա��Ϣ
	if (pDBOLogonSuccess->cbMemberOrder!=0)
	{
		DTP_GP_MemberInfo MemberInfo;
		ZeroMemory(&MemberInfo,sizeof(MemberInfo));
		MemberInfo.cbMemberOrder=pDBOLogonSuccess->cbMemberOrder;
		MemberInfo.MemberOverDate=pDBOLogonSuccess->MemberOverDate;
		SendPacket.AddPacket(&MemberInfo,sizeof(MemberInfo),DTP_GP_MEMBER_INFO);
	}

	//����ǩ��
	if (pDBOLogonSuccess->szUnderWrite[0]!=0)
	{
		SendPacket.AddPacket(pDBOLogonSuccess->szUnderWrite,CountStringBuffer(pDBOLogonSuccess->szUnderWrite),DTP_GP_UNDER_WRITE);
	}

	if (1)
	{
		DTP_GP_iOSChargeHide iOSChargeHide = {0};

		iOSChargeHide.id = 7;
		lstrcpyn(iOSChargeHide.szURL, "http://zf.66y.com", CountArray(iOSChargeHide.szURL));
		SendPacket.AddPacket(&iOSChargeHide, sizeof(DTP_GP_iOSChargeHide), DTP_GP_IOSCHARGEHIDE);
	}

	if (1)
	{
		DTP_GP_iOSPassed iOSPassed = {0};
		iOSPassed.cbPassed = 2;
		SendPacket.AddPacket(&iOSPassed, sizeof(DTP_GP_iOSPassed), DTP_GP_IOSPASSED);
	}

	//��һ�Ա�ȼ�����Ա���Կ����������ͼ�¼��
	DTP_GP_MemberOrder MemberOrder = {0};
	MemberOrder.cbMemberOrder = pDBOLogonSuccess->cbMemberOrder;
	SendPacket.AddPacket(&MemberOrder, sizeof(DTP_GP_MemberOrder), DTP_GP_MEMBER_ORDER);

	//��ȯ
	DTP_GP_GiftTicket GiftTicket = {0};
	GiftTicket.dwCount = pDBOLogonSuccess->dwLotteryCount;
	SendPacket.AddPacket(&GiftTicket, sizeof(DTP_GP_GiftTicket), DTP_GP_GIFT_TICKET);

	//ţţ��ע
	if (0)
	{
		DTP_GP_ServerInfo ServerInfo = {0};

		ServerInfo.wServerID = 301;
		ServerInfo.nCellScore = 100;
		SendPacket.AddPacket(&ServerInfo, sizeof(DTP_GP_ServerInfo), DTP_GP_SERVER_INFO);

		ServerInfo.wServerID = 302;
		ServerInfo.nCellScore = 1000;
		SendPacket.AddPacket(&ServerInfo, sizeof(DTP_GP_ServerInfo), DTP_GP_SERVER_INFO);

		ServerInfo.wServerID = 303;
		ServerInfo.nCellScore = 1000;
		SendPacket.AddPacket(&ServerInfo, sizeof(DTP_GP_ServerInfo), DTP_GP_SERVER_INFO);

		ServerInfo.wServerID = 304;
		ServerInfo.nCellScore = 1000;
		SendPacket.AddPacket(&ServerInfo, sizeof(DTP_GP_ServerInfo), DTP_GP_SERVER_INFO);

		ServerInfo.wServerID = 305;
		ServerInfo.nCellScore = 100;
		SendPacket.AddPacket(&ServerInfo, sizeof(DTP_GP_ServerInfo), DTP_GP_SERVER_INFO);
	}

	////��ʾ������
	//DTP_GP_ShowFakeTable ShowFakeTable = {0};
	//ShowFakeTable.cbShowFakeTable = 1;
	//SendPacket.AddPacket(&ShowFakeTable, sizeof(DTP_GP_ShowFakeTable), DTP_GP_SHOWFAKETABLE);

	//��ҵ�VIP��Ϣ
	DTP_GP_VipInfo VipInfo = {0};
	VipInfo.cbVIPLevel = pDBOLogonSuccess->cbVIPLevel;
	VipInfo.dwVIPExp = pDBOLogonSuccess->dwVIPExp;
	VipInfo.dwExpPerCent = pDBOLogonSuccess->dwExpPerCent;
	SendPacket.AddPacket(&VipInfo, sizeof(DTP_GP_VipInfo), DTP_GP_VIPINFO);

	//�ͷ�QQ
	SendPacket.AddPacket(pDBOLogonSuccess->szCustomerServiceQQ, DTP_GP_CUSTOMERSERVICEQQ);

	//��¼����
	DTP_GP_LogonGift LogonGift = {0};
	LogonGift.dwGiftScore = pDBOLogonSuccess->nLogonGift;
	LogonGift.cbCanGetBankruptcy = pDBOLogonSuccess->cbCanGetBankruptcy;
	LogonGift.nBankruptcyCount = pDBOLogonSuccess->nBankruptcyCount;
	LogonGift.nCanShare = pDBOLogonSuccess->nCanShare;
	LogonGift.nBankruptcyCondition = 1000;
	LogonGift.nBankruptcyCountMax = 3;
	SendPacket.AddPacket(&LogonGift, sizeof(DTP_GP_LogonGift), DTP_GP_LOGONGIFT);

	//�׳������Ϣ
	DTP_GP_Charge1stInfo Charge1stInfo = {0};
	Charge1stInfo.cbCount = pDBOLogonSuccess->cbCharge1stCount;
	Charge1stInfo.cbCanGet = pDBOLogonSuccess->cbCharge1stCanGet;
	SendPacket.AddPacket(&Charge1stInfo, sizeof(DTP_GP_Charge1stInfo), DTP_GP_CHARGE1ST_INFO);

	SendPacket.AddPacket(pDBOLogonSuccess->szPassPortID, DTP_GP_PASS_PORT_ID);

	//��¼�ɹ�
	WORD wSendSize=SendPacket.GetDataSize()+sizeof(CMD_GP_LogonSuccess);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_SUCCESS,cbDataBuffer,wSendSize);

	//printf("wSendSize is %hu\n", wSendSize);

	//�����б�
	if (m_pInitParameter->m_cbDelayList==TRUE)
	{
		SendGameServerInfoEx(dwContextID,INVALID_WORD);
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);
	}
	else
	{
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);
	}

	//��¼���
	CMD_GP_LogonFinish LogonFinish;
	ZeroMemory(&LogonFinish,sizeof(LogonFinish));
	LogonFinish.wIntermitTime=m_pInitParameter->m_wIntermitTime;
	LogonFinish.wOnLineCountTime=m_pInitParameter->m_wOnLineCountTime;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_FINISH,&LogonFinish,sizeof(LogonFinish));
	printf("��¼�ɹ�\n");
	OnRequestDataBaseLogin();
	return true;
}

//��¼ʧ��
bool CAttemperEngineSink::OnDBPCLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	CMD_GP_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));
	DBO_GP_LogonFailure * pLogonFailure=(DBO_GP_LogonFailure *)pData;
	if (pLogonFailure->bIsLogin)
	{
		m_nSendMaxNum--;
		CVerification::GetInstance()->OnRecordFinishLoginNum(m_nSendMaxNum);
	}
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	LogonFailure.lResultCode=pLogonFailure->lResultCode;
	lstrcpyn(LogonFailure.szDescribeString,pLogonFailure->szDescribeString,CountArray(LogonFailure.szDescribeString));

	//��������
	WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_FAILURE,&LogonFailure,wSendSize);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	printf("��¼ʧ�� %s\n", LogonFailure.szDescribeString);
	OnRequestDataBaseLogin();

	return true;
}

//��¼ʧ��
bool CAttemperEngineSink::OnDBPCLogonValidateMBCard(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_ValidateMBCard));
	if (wDataSize!=sizeof(DBR_GP_ValidateMBCard)) return false;

	DBR_GP_ValidateMBCard *pValidateMBCard=(DBR_GP_ValidateMBCard *)pData;

	//������Ϣ
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_VALIDATE_MBCARD,pData,wDataSize);

	return true;
}

//��¼ʧ��
bool CAttemperEngineSink::OnDBPCLogonValidatePassPostID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������Ϣ
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_VALIDATE_PASSPOSTID);

	return true;
}

//�ο͵�¼
bool CAttemperEngineSink::OnDBPCGuestLogon(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;
	tagBindParameter * pBindParameter = (m_pBindParameter+LOWORD(dwContextID));

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_LogonSuccess * pDBOLogonSuccess=(DBO_GP_LogonSuccess *)pData;
	CMD_GP_GuestRegSuccess * pCMDLogonSuccess=(CMD_GP_GuestRegSuccess *)cbDataBuffer;

	//���Ͷ���
	WORD wHeadSize=sizeof(CMD_GP_GuestRegSuccess);
	CSendPacketHelper SendPacket(cbDataBuffer+wHeadSize,sizeof(cbDataBuffer)-wHeadSize);

	//���ñ���
	ZeroMemory(pCMDLogonSuccess,sizeof(CMD_GP_GuestRegSuccess));

	//��������
	pCMDLogonSuccess->wFaceID=pDBOLogonSuccess->wFaceID;
	pCMDLogonSuccess->cbGender=pDBOLogonSuccess->cbGender;
	pCMDLogonSuccess->dwGameID=pDBOLogonSuccess->dwGameID;
	pCMDLogonSuccess->dwUserID=pDBOLogonSuccess->dwUserID;
	pCMDLogonSuccess->dwCustomID=pDBOLogonSuccess->dwCustomID;
	pCMDLogonSuccess->dwUserMedal=pDBOLogonSuccess->dwUserMedal;
	pCMDLogonSuccess->dwExperience=pDBOLogonSuccess->dwExperience;
	pCMDLogonSuccess->dwLoveLiness=pDBOLogonSuccess->dwLoveLiness;
	pCMDLogonSuccess->cbMoorMachine=pDBOLogonSuccess->cbMoorMachine;
	lstrcpyn(pCMDLogonSuccess->szAccounts,pDBOLogonSuccess->szAccounts,CountArray(pCMDLogonSuccess->szAccounts));
	lstrcpyn(pCMDLogonSuccess->szNickName,pDBOLogonSuccess->szNickName,CountArray(pCMDLogonSuccess->szNickName));
	lstrcpyn(pCMDLogonSuccess->szPassword,pDBOLogonSuccess->szPassword,CountArray(pCMDLogonSuccess->szPassword));

	pBindParameter->dwUserID = pDBOLogonSuccess->dwUserID;
	lstrcpyn(pBindParameter->szNickName, pDBOLogonSuccess->szNickName, CountArray(pBindParameter->szNickName));

	//�û��ɼ�
	pCMDLogonSuccess->lUserScore=pDBOLogonSuccess->lUserScore;
	pCMDLogonSuccess->lUserInsure=pDBOLogonSuccess->lUserInsure;

	//������Ϣ
	pCMDLogonSuccess->cbShowServerStatus=m_bShowServerStatus?1:0;

	//��Ա��Ϣ
	if (pDBOLogonSuccess->cbMemberOrder!=0)
	{
		DTP_GP_MemberInfo MemberInfo;
		ZeroMemory(&MemberInfo,sizeof(MemberInfo));
		MemberInfo.cbMemberOrder=pDBOLogonSuccess->cbMemberOrder;
		MemberInfo.MemberOverDate=pDBOLogonSuccess->MemberOverDate;
		SendPacket.AddPacket(&MemberInfo,sizeof(MemberInfo),DTP_GP_MEMBER_INFO);
	}

	//����ǩ��
	if (pDBOLogonSuccess->szUnderWrite[0]!=0)
	{
		SendPacket.AddPacket(pDBOLogonSuccess->szUnderWrite,CountStringBuffer(pDBOLogonSuccess->szUnderWrite),DTP_GP_UNDER_WRITE);
	}

	if (1)
	{
		DTP_GP_iOSChargeHide iOSChargeHide = {0};

		iOSChargeHide.id = 1;
		lstrcpyn(iOSChargeHide.szURL, "http://pay.youkuss.net/Mp_Pay/alipay/alipay_wsite_url.aspx", CountArray(iOSChargeHide.szURL));
		SendPacket.AddPacket(&iOSChargeHide, sizeof(DTP_GP_iOSChargeHide), DTP_GP_IOSCHARGEHIDE);

		iOSChargeHide.id = 2;
		lstrcpyn(iOSChargeHide.szURL, "http://wap.youkuss.net/Pay_bank.aspx", CountArray(iOSChargeHide.szURL));
		SendPacket.AddPacket(&iOSChargeHide, sizeof(DTP_GP_iOSChargeHide), DTP_GP_IOSCHARGEHIDE);

		iOSChargeHide.id = 3;
		lstrcpyn(iOSChargeHide.szURL, "http://wap.youkuss.net/Pay_gcard.aspx", CountArray(iOSChargeHide.szURL));
		SendPacket.AddPacket(&iOSChargeHide, sizeof(DTP_GP_iOSChargeHide), DTP_GP_IOSCHARGEHIDE);

		iOSChargeHide.id = 4;
		lstrcpyn(iOSChargeHide.szURL, "http://wap.youkuss.net/Pay_mob.aspx", CountArray(iOSChargeHide.szURL));
		SendPacket.AddPacket(&iOSChargeHide, sizeof(DTP_GP_iOSChargeHide), DTP_GP_IOSCHARGEHIDE);

		iOSChargeHide.id = 5;
		lstrcpyn(iOSChargeHide.szURL, "http://pay.youkuss.net/pay/sms.aspx", CountArray(iOSChargeHide.szURL));
		SendPacket.AddPacket(&iOSChargeHide, sizeof(DTP_GP_iOSChargeHide), DTP_GP_IOSCHARGEHIDE);

		iOSChargeHide.id = 6;
		lstrcpyn(iOSChargeHide.szURL, "http://zf.66y.com", CountArray(iOSChargeHide.szURL));
		SendPacket.AddPacket(&iOSChargeHide, sizeof(DTP_GP_iOSChargeHide), DTP_GP_IOSCHARGEHIDE);

		iOSChargeHide.id = 7;
		lstrcpyn(iOSChargeHide.szURL, "http://zf.66y.com", CountArray(iOSChargeHide.szURL));
		SendPacket.AddPacket(&iOSChargeHide, sizeof(DTP_GP_iOSChargeHide), DTP_GP_IOSCHARGEHIDE);
	}

	if (1)
	{
		DTP_GP_iOSPassed iOSPassed = {0};
		iOSPassed.cbPassed = 2;
		SendPacket.AddPacket(&iOSPassed, sizeof(DTP_GP_iOSPassed), DTP_GP_IOSPASSED);
	}

	//��ҵ�VIP��Ϣ
	DTP_GP_VipInfo VipInfo = {0};
	VipInfo.cbVIPLevel = pDBOLogonSuccess->cbVIPLevel;
	VipInfo.dwVIPExp = pDBOLogonSuccess->dwVIPExp;
	VipInfo.dwExpPerCent = pDBOLogonSuccess->dwExpPerCent;
	SendPacket.AddPacket(&VipInfo, sizeof(DTP_GP_VipInfo), DTP_GP_VIPINFO);

	//�ͷ�QQ
	SendPacket.AddPacket(pDBOLogonSuccess->szCustomerServiceQQ, DTP_GP_CUSTOMERSERVICEQQ);

	//��¼����
	DTP_GP_LogonGift LogonGift = {0};
	LogonGift.dwGiftScore = pDBOLogonSuccess->nLogonGift;
	LogonGift.cbCanGetBankruptcy = pDBOLogonSuccess->cbCanGetBankruptcy;
	LogonGift.nBankruptcyCount = pDBOLogonSuccess->nBankruptcyCount;
	LogonGift.nCanShare = pDBOLogonSuccess->nCanShare;
	LogonGift.nBankruptcyCondition = 1000;
	SendPacket.AddPacket(&LogonGift, sizeof(DTP_GP_LogonGift), DTP_GP_LOGONGIFT);
	
	SendPacket.AddPacket(pDBOLogonSuccess->szPassPortID, DTP_GP_PASS_PORT_ID);

	//��¼�ɹ�
	WORD wSendSize=SendPacket.GetDataSize()+sizeof(CMD_GP_GuestRegSuccess);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_GUEST_REG_SUCCESS,cbDataBuffer,wSendSize);

	//�����б�
	//if (m_pInitParameter->m_cbDelayList==TRUE)
	//{
	//	//�����б�
	//	SendGameTypeInfo(dwContextID);
	//	SendGameKindInfo(dwContextID);
	//	SendGamePageInfo(dwContextID,INVALID_WORD);
	//	SendGameNodeInfo(dwContextID,INVALID_WORD);
	//	SendGameServerInfo(dwContextID,INVALID_WORD);
	//	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);
	//}
	//else
	//{
	//	SendGameTypeInfo(dwContextID);
	//	SendGameKindInfo(dwContextID);
	//	SendGamePageInfo(dwContextID,0);
	//	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);
	//}

	if (m_pInitParameter->m_cbDelayList==TRUE)
	{
		SendGameServerInfoEx(dwContextID,INVALID_WORD);
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);
	}
	else
	{
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);
	}


	//��¼���
	CMD_GP_LogonFinish LogonFinish;
	ZeroMemory(&LogonFinish,sizeof(LogonFinish));
	LogonFinish.wIntermitTime=m_pInitParameter->m_wIntermitTime;
	LogonFinish.wOnLineCountTime=m_pInitParameter->m_wOnLineCountTime;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_FINISH,&LogonFinish,sizeof(LogonFinish));

	return true;
}

//�û�ͷ��
bool CAttemperEngineSink::OnDBPCUserFaceInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_UserFaceInfo UserFaceInfo;
	ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));
	DBO_GP_UserFaceInfo * pUserFaceInfo=(DBO_GP_UserFaceInfo *)pData;

	//���ñ���
	UserFaceInfo.wFaceID=pUserFaceInfo->wFaceID;
	UserFaceInfo.dwCustomID=pUserFaceInfo->dwCustomID;

	//������Ϣ
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_FACE_INFO,&UserFaceInfo,sizeof(UserFaceInfo));

	return true;
}

//�û���Ϣ
bool CAttemperEngineSink::OnDBPCUserIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_UserIndividual * pDBOUserIndividual=(DBO_GP_UserIndividual *)pData;
	CMD_GP_UserIndividual * pCMDUserIndividual=(CMD_GP_UserIndividual *)cbDataBuffer;
	CSendPacketHelper SendPacket(cbDataBuffer+sizeof(CMD_GP_UserIndividual),sizeof(cbDataBuffer)-sizeof(CMD_GP_UserIndividual));

	//���ñ���
	ZeroMemory(pCMDUserIndividual,sizeof(CMD_GP_UserIndividual));

	//��������
	pCMDUserIndividual->dwUserID=pDBOUserIndividual->dwUserID;

	//�û���Ϣ
	if (pDBOUserIndividual->szUserNote[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szUserNote);
		SendPacket.AddPacket(pDBOUserIndividual->szUserNote,wBufferSize,DTP_GP_UI_USER_NOTE);
	}

	//��ʵ����
	if (pDBOUserIndividual->szCompellation[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szCompellation);
		SendPacket.AddPacket(pDBOUserIndividual->szCompellation,wBufferSize,DTP_GP_UI_COMPELLATION);
	}

	//�绰����
	if (pDBOUserIndividual->szSeatPhone[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szSeatPhone);
		SendPacket.AddPacket(pDBOUserIndividual->szSeatPhone,wBufferSize,DTP_GP_UI_SEAT_PHONE);
	}

	//�ƶ��绰
	if (pDBOUserIndividual->szMobilePhone[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szMobilePhone);
		SendPacket.AddPacket(pDBOUserIndividual->szMobilePhone,wBufferSize,DTP_GP_UI_MOBILE_PHONE);
	}

	//��ϵ����
	if (pDBOUserIndividual->szQQ[0]!=0) 
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szQQ);
		SendPacket.AddPacket(pDBOUserIndividual->szQQ,wBufferSize,DTP_GP_UI_QQ);
	}

	//�����ʼ�
	if (pDBOUserIndividual->szEMail[0]!=0) 
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szEMail);
		SendPacket.AddPacket(pDBOUserIndividual->szEMail,wBufferSize,DTP_GP_UI_EMAIL);
	}

	//��ϵ��ַ
	if (pDBOUserIndividual->szDwellingPlace[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szDwellingPlace);
		SendPacket.AddPacket(pDBOUserIndividual->szDwellingPlace,wBufferSize,DTP_GP_UI_DWELLING_PLACE);
	}

	//������Ϣ
	WORD wSendSize=sizeof(CMD_GP_UserIndividual)+SendPacket.GetDataSize();
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INDIVIDUAL,cbDataBuffer,wSendSize);

	return true;
}

//������Ϣ
bool CAttemperEngineSink::OnDBPCUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserInsureInfo * pUserInsureInfo=(DBO_GP_UserInsureInfo *)pData;

	//��������
	CMD_GP_UserInsureInfo UserInsureInfo;
	ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

	//��������
	UserInsureInfo.wRevenueTake=pUserInsureInfo->wRevenueTake;
	UserInsureInfo.wRevenueTransfer=pUserInsureInfo->wRevenueTransfer;
	UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
	UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
	UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;
	UserInsureInfo.lTransferPrerequisite=pUserInsureInfo->lTransferPrerequisite;

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_INFO,&UserInsureInfo,sizeof(UserInsureInfo));

	////�ر�����
	//m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//������Ϣ
bool CAttemperEngineSink::OnDBPCUserInsureTransferInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_TRANSFER_INFO,pData,wDataSize);

	return true;
}

//���гɹ�
bool CAttemperEngineSink::OnDBPCUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserInsureSuccess * pUserInsureSuccess=(DBO_GP_UserInsureSuccess *)pData;

	//��������
	CMD_GP_UserInsureSuccess UserInsureSuccess;
	ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

	//���ñ���
	UserInsureSuccess.dwUserID=pUserInsureSuccess->dwUserID;
	UserInsureSuccess.lUserScore=pUserInsureSuccess->lSourceScore+pUserInsureSuccess->lVariationScore;
	UserInsureSuccess.lUserInsure=pUserInsureSuccess->lSourceInsure+pUserInsureSuccess->lVariationInsure;
	lstrcpyn(UserInsureSuccess.szDescribeString,pUserInsureSuccess->szDescribeString,CountArray(UserInsureSuccess.szDescribeString));

	//������Ϣ
	WORD wDescribe=CountStringBuffer(UserInsureSuccess.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_SUCCESS,&UserInsureSuccess,wHeadSize+wDescribe);

	return true;
}

//����ʧ��
bool CAttemperEngineSink::OnDBPCUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_UserInsureFailure UserInsureFailure;
	ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

	//��������
	DBO_GP_UserInsureFailure * pUserInsureFailure=(DBO_GP_UserInsureFailure *)pData;

	//��������
	UserInsureFailure.lResultCode=pUserInsureFailure->lResultCode;
	lstrcpyn(UserInsureFailure.szDescribeString,pUserInsureFailure->szDescribeString,CountArray(UserInsureFailure.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(UserInsureFailure.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_FAILURE,&UserInsureFailure,wHeadSize+wDescribe);

	////�ر�����
	//m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//�û���Ϣ
bool CAttemperEngineSink::OnDBPCUserInsureUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_GP_UserTransferUserInfo * pTransferUserInfo=(DBO_GP_UserTransferUserInfo *)pData;

	//��������
	CMD_GP_UserTransferUserInfo UserTransferUserInfo;
	ZeroMemory(&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	//�������
	UserTransferUserInfo.dwTargetGameID=pTransferUserInfo->dwGameID;
	lstrcpyn(UserTransferUserInfo.szNickName,pTransferUserInfo->szNickName,CountArray(UserTransferUserInfo.szNickName));

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_USER_INFO_RESULT,&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	return true;
}

//������֤
bool CAttemperEngineSink::OnDBPCUserCheckPassword(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_CHECK_PASSWORD_RETURN,pData,wDataSize);

	return true;
}

//�����ɹ�
bool CAttemperEngineSink::OnDBPCOperateSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_OperateSuccess OperateSuccess;
	ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

	//��������
	DBO_GP_OperateSuccess * pOperateSuccess=(DBO_GP_OperateSuccess *)pData;

	//��������
	OperateSuccess.lResultCode=pOperateSuccess->lResultCode;
	lstrcpyn(OperateSuccess.szDescribeString,pOperateSuccess->szDescribeString,CountArray(OperateSuccess.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(OperateSuccess.szDescribeString);
	WORD wHeadSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_SUCCESS,&OperateSuccess,wHeadSize+wDescribe);

	////�ر�����
	//m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//����ʧ��
bool CAttemperEngineSink::OnDBPCOperateFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_OperateFailure OperateFailure;
	ZeroMemory(&OperateFailure,sizeof(OperateFailure));

	//��������
	DBO_GP_OperateFailure * pOperateFailure=(DBO_GP_OperateFailure *)pData;

	//��������
	OperateFailure.lResultCode=pOperateFailure->lResultCode;
	lstrcpyn(OperateFailure.szDescribeString,pOperateFailure->szDescribeString,CountArray(OperateFailure.szDescribeString));

	//��������
	WORD wDescribe=CountStringBuffer(OperateFailure.szDescribeString);
	WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_FAILURE,&OperateFailure,wHeadSize+wDescribe);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//��¼�ɹ�
bool CAttemperEngineSink::OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	DBO_MB_LogonSuccess * pDBOLogonSuccess=(DBO_MB_LogonSuccess *)pData;

	//��������
	CMD_MB_LogonSuccess LogonSuccess;
	ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

	//��������
	LogonSuccess.wFaceID=pDBOLogonSuccess->wFaceID;
	LogonSuccess.cbGender=pDBOLogonSuccess->cbGender;
	LogonSuccess.dwGameID=pDBOLogonSuccess->dwGameID;
	LogonSuccess.dwUserID=pDBOLogonSuccess->dwUserID;
	LogonSuccess.dwExperience=pDBOLogonSuccess->dwExperience;
	LogonSuccess.dwLoveLiness=pDBOLogonSuccess->dwLoveLiness;
	lstrcpyn(LogonSuccess.szNickName,pDBOLogonSuccess->szNickName,CountArray(LogonSuccess.szNickName));

	//��¼�ɹ�
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON,SUB_MB_LOGON_SUCCESS,&LogonSuccess,sizeof(LogonSuccess));

	//���ͷ���
	WORD wIndex=LOWORD(dwContextID);
	SendMobileKindInfo(dwContextID,(m_pBindParameter+wIndex)->wModuleID);
	SendMobileServerInfo(dwContextID,(m_pBindParameter+wIndex)->wModuleID);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_SERVER_LIST,SUB_MB_LIST_FINISH);

	return true;
}

//��¼ʧ��
bool CAttemperEngineSink::OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_MB_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));
	DBO_MB_LogonFailure * pLogonFailure=(DBO_MB_LogonFailure *)pData;

	//��������
	LogonFailure.lResultCode=pLogonFailure->lResultCode;
	lstrcpyn(LogonFailure.szDescribeString,pLogonFailure->szDescribeString,CountArray(LogonFailure.szDescribeString));

	//��������
	WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON,SUB_MB_LOGON_FAILURE,&LogonFailure,wSendSize);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//��Ϸ����
bool CAttemperEngineSink::OnDBPCGameTypeItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GameType)==0);
	if (wDataSize%sizeof(DBO_GP_GameType)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameType);
	DBO_GP_GameType * pGameType=(DBO_GP_GameType *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGameType GameType;
		ZeroMemory(&GameType,sizeof(GameType));

		//��������
		GameType.wTypeID=(pGameType+i)->wTypeID;
		GameType.wJoinID=(pGameType+i)->wJoinID;
		GameType.wSortID=(pGameType+i)->wSortID;
		lstrcpyn(GameType.szTypeName,(pGameType+i)->szTypeName,CountArray(GameType.szTypeName));

		//�����б�
		m_ServerListManager.InsertGameType(&GameType);
	}

	return true;
}

//��Ϸ����
bool CAttemperEngineSink::OnDBPCGameKindItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GameKind)==0);
	if (wDataSize%sizeof(DBO_GP_GameKind)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameKind);
	DBO_GP_GameKind * pGameKind=(DBO_GP_GameKind *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGameKind GameKind;
		ZeroMemory(&GameKind,sizeof(GameKind));

		//��������
		GameKind.wTypeID=(pGameKind+i)->wTypeID;
		GameKind.wJoinID=(pGameKind+i)->wJoinID;
		GameKind.wSortID=(pGameKind+i)->wSortID;
		GameKind.wKindID=(pGameKind+i)->wKindID;
		GameKind.wGameID=(pGameKind+i)->wGameID;
		GameKind.dwOnLineCount=m_ServerListManager.CollectOnlineInfo((pGameKind+i)->wKindID);
		lstrcpyn(GameKind.szKindName,(pGameKind+i)->szKindName,CountArray(GameKind.szKindName));
		lstrcpyn(GameKind.szProcessName,(pGameKind+i)->szProcessName,CountArray(GameKind.szProcessName));

		//�����б�
		m_ServerListManager.InsertGameKind(&GameKind);
	}

	return true;
}

//��Ϸ�ڵ�
bool CAttemperEngineSink::OnDBPCGameNodeItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GameNode)==0);
	if (wDataSize%sizeof(DBO_GP_GameNode)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameNode);
	DBO_GP_GameNode * pGameNode=(DBO_GP_GameNode *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGameNode GameNode;
		ZeroMemory(&GameNode,sizeof(GameNode));

		//��������
		GameNode.wKindID=(pGameNode+i)->wKindID;
		GameNode.wJoinID=(pGameNode+i)->wJoinID;
		GameNode.wSortID=(pGameNode+i)->wSortID;
		GameNode.wNodeID=(pGameNode+i)->wNodeID;
		lstrcpyn(GameNode.szNodeName,(pGameNode+i)->szNodeName,CountArray(GameNode.szNodeName));

		//�����б�
		m_ServerListManager.InsertGameNode(&GameNode);
	}

	return true;
}

//��Ϸ����
bool CAttemperEngineSink::OnDBPCGamePageItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_GamePage)==0);
	if (wDataSize%sizeof(DBO_GP_GamePage)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GamePage);
	DBO_GP_GamePage * pGamePage=(DBO_GP_GamePage *)pData;

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagGamePage GamePage;
		ZeroMemory(&GamePage,sizeof(GamePage));

		//��������
		GamePage.wKindID=(pGamePage+i)->wKindID;
		GamePage.wNodeID=(pGamePage+i)->wNodeID;
		GamePage.wSortID=(pGamePage+i)->wSortID;
		GamePage.wPageID=(pGamePage+i)->wPageID;
		GamePage.wOperateType=(pGamePage+i)->wOperateType;
		lstrcpyn(GamePage.szDisplayName,(pGamePage+i)->szDisplayName,CountArray(GamePage.szDisplayName));

		//�����б�
		m_ServerListManager.InsertGamePage(&GamePage);
	}

	return true;
}

//��Ϸ�б�
bool CAttemperEngineSink::OnDBPCGameListResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBO_GP_GameListResult));
	if (wDataSize!=sizeof(DBO_GP_GameListResult)) return false;

	//��������
	DBO_GP_GameListResult * pGameListResult=(DBO_GP_GameListResult *)pData;

	//��Ϣ����
	if (pGameListResult->cbSuccess==TRUE)
	{
		//�����б�
		m_ServerListManager.CleanKernelItem();

		//�¼�֪ͨ
		CP_ControlResult ControlResult;
		ControlResult.cbSuccess=ER_SUCCESS;
		SendUIControlPacket(UI_LOAD_DB_LIST_RESULT,&ControlResult,sizeof(ControlResult));

		//����ʱ��
		ASSERT(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_LOAD_GAME_LIST,m_pInitParameter->m_wLoadListTime*1000L,1,0);
	}
	else
	{
		//������ʾ
		TCHAR szDescribe[128]=TEXT("");
		_sntprintf(szDescribe,CountArray(szDescribe),TEXT("�������б����ʧ�ܣ�%ld ������¼���"),m_pInitParameter->m_wReLoadListTime);

		//��ʾ��Ϣ
		CTraceService::TraceString(szDescribe,TraceLevel_Warning);

		//����ʱ��
		ASSERT(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_LOAD_GAME_LIST,m_pInitParameter->m_wReLoadListTime*1000L,1,0);
	}

	return true;
}

//ǩ����ѯ�ɹ�
bool CAttemperEngineSink::OnDBPCSignInQuerySuccess( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//Ч�����
	ASSERT(wDataSize<=sizeof(DBO_GP_UserSignInInfo));
	if (wDataSize > sizeof(DBO_GP_UserSignInInfo)) return false;

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_UserSignInInfo * pDBOUserSignInInfo=(DBO_GP_UserSignInInfo *)pData;
	CMD_GP_UserSigninInfo * pCMDUserSignInInfo=(CMD_GP_UserSigninInfo *)cbDataBuffer;

	//���Ͷ���
	WORD wHeadSize=sizeof(CMD_GP_UserSigninInfo);

	//���ñ���
	ZeroMemory(pCMDUserSignInInfo,sizeof(CMD_GP_UserSigninInfo));

	pCMDUserSignInInfo->nNum1 = pDBOUserSignInInfo->nNum1;
	pCMDUserSignInInfo->nNum2 = pDBOUserSignInInfo->nNum2;

	for (UINT i = 0; i < sizeof(pCMDUserSignInInfo->normal)/sizeof(pCMDUserSignInInfo->normal[0]); i++)
	{
		pCMDUserSignInInfo->normal[i].cbType = pDBOUserSignInInfo->normal[i].cbType;
		pCMDUserSignInInfo->normal[i].cbAttrib = pDBOUserSignInInfo->normal[i].cbAttrib;
		pCMDUserSignInInfo->normal[i].nCount = pDBOUserSignInInfo->normal[i].nCount;

		pCMDUserSignInInfo->vip[i].cbType = pDBOUserSignInInfo->vip[i].cbType;
		pCMDUserSignInInfo->vip[i].cbAttrib = pDBOUserSignInInfo->vip[i].cbAttrib;
		pCMDUserSignInInfo->vip[i].nCount = pDBOUserSignInInfo->vip[i].nCount;
	}

	//ǩ����ѯ�ɹ�
	WORD wSendSize=sizeof(CMD_GP_UserSigninInfo);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_SIGNIN_INFO_RESULT,cbDataBuffer,wSendSize);

	return true;
}

//ǩ����ѯʧ��
bool CAttemperEngineSink::OnDBPCSignInQueryFailure( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_SIGNIN_INFO_RESULT,cbDataBuffer,wSendSize);
	return true;
}

//�û�ǩ���ɹ�
bool CAttemperEngineSink::OnDBPCUserSignInSuccess( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//Ч�����
	ASSERT(wDataSize<=sizeof(DBO_GP_UserSignInResult));
	if (wDataSize > sizeof(DBO_GP_UserSignInResult)) return false;

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_UserSignInResult * pDBOUserSignIn=(DBO_GP_UserSignInResult *)pData;
	CMD_GP_SignIn_Result * pCMDUserSignInRes=(CMD_GP_SignIn_Result *)cbDataBuffer;

	//���ñ���
	ZeroMemory(pCMDUserSignInRes,sizeof(CMD_GP_SignIn_Result));

	pCMDUserSignInRes->bSuccess = pDBOUserSignIn->bSuccess;
	pCMDUserSignInRes->cbUserStatus = pDBOUserSignIn->cbUserStatus;
	pCMDUserSignInRes->llLotteryCount = pDBOUserSignIn->llLotteryCount;
	pCMDUserSignInRes->llScoreCount= pDBOUserSignIn->llScoreCount;
	lstrcpyn(pCMDUserSignInRes->szDescription,pDBOUserSignIn->szDescription,CountArray(pCMDUserSignInRes->szDescription));

	//���ͽ��
	WORD wDataSizeSend=sizeof(CMD_GP_SignIn_Result);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_SIGNIN_RESULT,cbDataBuffer,wDataSizeSend);

	return true;
}

//�û�ǩ��ʧ��
bool CAttemperEngineSink::OnDBPCUserSignInFailure( DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize<=sizeof(DBO_GP_UserSignInResult));
	if (wDataSize > sizeof(DBO_GP_UserSignInResult)) return false;

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_UserSignInResult * pDBOUserSignIn=(DBO_GP_UserSignInResult *)pData;
	CMD_GP_SignIn_Result * pCMDUserSignInRes=(CMD_GP_SignIn_Result *)cbDataBuffer;

	//���ñ���
	ZeroMemory(pCMDUserSignInRes,sizeof(CMD_GP_SignIn_Result));

	pCMDUserSignInRes->bSuccess = pDBOUserSignIn->bSuccess;
	pCMDUserSignInRes->llLotteryCount = pDBOUserSignIn->llLotteryCount;
	pCMDUserSignInRes->llScoreCount = pDBOUserSignIn->llScoreCount;
	lstrcpyn(pCMDUserSignInRes->szDescription,pDBOUserSignIn->szDescription,CountArray(pCMDUserSignInRes->szDescription));

	WORD wDataSizeSend=sizeof(CMD_GP_SignIn_Result);
	printf("%s", pCMDUserSignInRes->szDescription);
	printf("the size of the data is %ld", wDataSizeSend);
	//�û�ǩ��ʧ��
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_SIGNIN_RESULT,cbDataBuffer,wDataSizeSend);

	return true;
}

//�齱��ѯ���
bool CAttemperEngineSink::OnDBPCLotteryQueryResult( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//Ч�����
	ASSERT(wDataSize<=sizeof(DBO_GP_Ltry_Qry_Res));
	if (wDataSize > sizeof(DBO_GP_Ltry_Qry_Res)) return false;

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_Ltry_Qry_Res * pDBOLtryQryRes=(DBO_GP_Ltry_Qry_Res *)pData;
	CMD_GP_QryLtryInfo_Res * pCMDLtryQryRes=(CMD_GP_QryLtryInfo_Res *)cbDataBuffer;

	//���Ͷ���
	WORD wHeadSize=sizeof(CMD_GP_QryLtryInfo);
	CSendPacketHelper SendPacket(cbDataBuffer+wHeadSize,sizeof(cbDataBuffer)-wHeadSize);

	//���ñ���
	ZeroMemory(pCMDLtryQryRes,sizeof(CMD_GP_QryLtryInfo_Res));
	pCMDLtryQryRes->wFreeCount=pDBOLtryQryRes->wFreeCount;

	//�齱��ѯ�ɹ�
	WORD wSendSize=SendPacket.GetDataSize()+sizeof(CMD_GP_QryLtryInfo_Res);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_LOTTERY_INFO_RESULT,cbDataBuffer,wSendSize);
	return true;
}

//�û��齱���
bool CAttemperEngineSink::OnDBPCUserLotteryResult( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//Ч�����
	ASSERT(wDataSize<=sizeof(DBO_GP_User_Ltry_Res));
	if (wDataSize > sizeof(DBO_GP_User_Ltry_Res)) return false;

	//��������
	BYTE cbDataBuffer_LtryRes[SOCKET_TCP_PACKET];
	BYTE cbDataBuffer_LtryMsg[SOCKET_TCP_PACKET];
	DBO_GP_User_Ltry_Res * pDBOUserLtry=(DBO_GP_User_Ltry_Res *)pData;
	CMD_GP_UserLtry_Res * pCMDUserLtryRes=(CMD_GP_UserLtry_Res *)cbDataBuffer_LtryRes;

	//���Ͷ���
	WORD wHeadSize=sizeof(CMD_GP_UserLtry_Res);
	CSendPacketHelper SendPacket(cbDataBuffer_LtryRes+wHeadSize,sizeof(cbDataBuffer_LtryRes)-wHeadSize);

	//���ñ���
	ZeroMemory(pCMDUserLtryRes,sizeof(CMD_GP_UserLtry_Res));

	pCMDUserLtryRes->wIsLtrySuc=pDBOUserLtry->wIsLtrySuc;
	pCMDUserLtryRes->wPrizeID=pDBOUserLtry->wPrizeID;
	pCMDUserLtryRes->wFreeCount=pDBOUserLtry->wFreeCount;
	pCMDUserLtryRes->llUserScore=pDBOUserLtry->lUserScore;
	lstrcpyn(pCMDUserLtryRes->szErrorDescribe,pDBOUserLtry->szDescribeString,CountArray(pCMDUserLtryRes->szErrorDescribe));	

	WORD wSendSize=SendPacket.GetDataSize()+sizeof(CMD_GP_UserLtry_Res);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_LOTTERY_RESULT,cbDataBuffer_LtryRes,wSendSize);

	if (pDBOUserLtry->wIsLtrySuc)
	{
		//��������
		WORD wConnectCount=0;
		CMD_GP_LTRY_MSG * pCMDUserLtryMsg=(CMD_GP_LTRY_MSG *)cbDataBuffer_LtryMsg;

		pCMDUserLtryMsg->wPrizeID=pDBOUserLtry->wPrizeID;
		lstrcpyn(pCMDUserLtryMsg->szNickName,pDBOUserLtry->szNickName,CountArray(pCMDUserLtryMsg->szNickName));	
		for (; wConnectCount<m_pInitParameter->m_wMaxConnect; wConnectCount++)
		{
			if (((m_pBindParameter + wConnectCount) != NULL) && ((m_pBindParameter + wConnectCount)->bIsLtryInfoSend))
			{
				m_pITCPNetworkEngine->SendData((m_pBindParameter + wConnectCount)->dwSocketID,MDM_GP_USER_SERVICE,SUB_GP_LOTTERY_MSG,cbDataBuffer_LtryMsg,wSendSize);
			}
		}
	}

	return true;
}

//�ʼ�ID�б�
bool CAttemperEngineSink::OnDBMailIDList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
//printf("@@@@@ OnDBMailIDList\n");
	DBO_MailIDList * pMailIDList = (DBO_MailIDList *)pData;

	CMD_GPO_MailIDList MailIDListResult = {0};
	MailIDListResult.cbCount = pMailIDList->cbCount;
	for (BYTE i = 0; i < MailIDListResult.cbCount; i++)
	{
		MailIDListResult.dwMailID[i] = pMailIDList->dwMailID[i];

		//printf("MailIDListResult.dwMailID[%hhd] is %u\n", i, MailIDListResult.dwMailID[i]);
	}

	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_QUERY_MAIL_ID_LIST, &MailIDListResult, sizeof(MailIDListResult));

	return true;
}

//�ʼ���Ϣ
bool CAttemperEngineSink::OnDBMailInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//char * pBuffer = (char *)pData;
	//DWORD * pCount = (DWORD *)pBuffer;
	//DBO_MailInfo * pMailInfo = (DBO_MailInfo *)&pBuffer[4];

	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_QUERY_MAIL_INFO, pData, wDataSize);

	return true;
}

//�ʼ�����
bool CAttemperEngineSink::OnDBMailContent(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_QUERY_MAIL_CONTENT, pData, wDataSize);

	return true;
}

//�ʼ���Ʒ
bool CAttemperEngineSink::OnDBMailItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	DBO_MailItem * MailItem =(DBO_MailItem *)pData;
	BYTE cbResult= MailItem->byResult;
	printf("��ȡ���%d\n",cbResult);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_QUERY_MAIL_ITEM, MailItem, wDataSize);

	return true;
}

//��ֵ����
bool CAttemperEngineSink::OnDBChargeValue(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	DBO_ChargeValue * pChargeValue = (DBO_ChargeValue *)pData;

	CMD_GPO_ChargeValue ChargeValue = {0};
	ChargeValue.lChargeValue = pChargeValue->lChargeValue;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GPO_CHARGE_VALUE,&ChargeValue,sizeof(CMD_GPO_ChargeValue));

	return true;
}

//VIP��Ϣ
bool CAttemperEngineSink::OnDBVIPInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_VIP_INFO, pData, wDataSize);
	return true;
}

//VIP������
bool CAttemperEngineSink::OnDBVIPResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_VIP_RESULT, pData, wDataSize);
	return true;
}

//��ǰ����
bool CAttemperEngineSink::OnDBCurrentScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_CURRENT_SCORE, pData, wDataSize);
	return true;
}

//��¼����
bool CAttemperEngineSink::OnDBLogonGift(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_LOGON_GIFT, pData, wDataSize);
	return true;
}

//�׳�ɹ�
bool CAttemperEngineSink::OnDBCharge1stRequest(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_CHARGE1ST_REQUEST, pData, wDataSize);
	return true;
}

//��ȡ�׳��������
bool CAttemperEngineSink::OnDBCharge1stGet(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_CHARGE1ST_GET, pData, wDataSize);
	return true;
}

//�汾���
bool CAttemperEngineSink::CheckPlazaVersion(BYTE cbDeviceType, DWORD dwPlazaVersion, DWORD dwSocketID, bool bCheckLowVer)
{
	//��������
	bool bMustUpdate=false;
	bool bAdviceUpdate=false;
	DWORD dwVersion=VERSION_PLAZA;

	//�ֻ��汾
	if(cbDeviceType >= DEVICE_TYPE_IPAD) dwVersion=VERSION_MOBILE_IOS;
	else if(cbDeviceType >= DEVICE_TYPE_IPHONE) dwVersion=VERSION_MOBILE_IOS;
	else if(cbDeviceType >= DEVICE_TYPE_ITOUCH) dwVersion=VERSION_MOBILE_IOS;
	else if(cbDeviceType >= DEVICE_TYPE_ANDROID) dwVersion=VERSION_MOBILE_ANDROID;
	else if(cbDeviceType == DEVICE_TYPE_PC) dwVersion=VERSION_PLAZA;

	//�汾�ж�
	if (bCheckLowVer && GetSubVer(dwPlazaVersion)<GetSubVer(dwVersion)) bAdviceUpdate=true;
	if (GetMainVer(dwPlazaVersion)!=GetMainVer(dwVersion)) bMustUpdate=true;
	if (GetProductVer(dwPlazaVersion)!=GetProductVer(dwVersion)) bMustUpdate=true;

	//�����ж�
	if ((bMustUpdate==true)||(bAdviceUpdate==true))
	{
		//��������
		CMD_GP_UpdateNotify UpdateNotify;
		ZeroMemory(&UpdateNotify,sizeof(UpdateNotify));

		//��������
		UpdateNotify.cbMustUpdate=bMustUpdate;
		UpdateNotify.cbAdviceUpdate=bAdviceUpdate;
		UpdateNotify.dwCurrentVersion=dwVersion;

		//������Ϣ
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_UPDATE_NOTIFY,&UpdateNotify,sizeof(UpdateNotify));

		//�ж��ж�
		if (bMustUpdate==true) 
		{
			m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
			return false;
		}
	}

	return true;
}

//��������
bool CAttemperEngineSink::SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	//��������
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	pServiceUnits->PostControlRequest(wRequestID,pData,wDataSize);

	return true;
}

//��������
VOID CAttemperEngineSink::SendGameTypeInfo(DWORD dwSocketID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameTypeItem * pGameTypeItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameTypeCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameType))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_TYPE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameTypeItem=m_ServerListManager.EmunGameTypeItem(Position);
		if (pGameTypeItem==NULL) break;

		//��������
		CopyMemory(cbDataBuffer+wSendSize,&pGameTypeItem->m_GameType,sizeof(tagGameType));
		wSendSize+=sizeof(tagGameType);
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_TYPE,cbDataBuffer,wSendSize);

	return;
}

//��������
VOID CAttemperEngineSink::SendGameKindInfo(DWORD dwSocketID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameKindItem * pGameKindItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameKindCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameKind))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_KIND,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameKindItem=m_ServerListManager.EmunGameKindItem(Position);
		if (pGameKindItem==NULL) break;

		//��������
		CopyMemory(cbDataBuffer+wSendSize,&pGameKindItem->m_GameKind,sizeof(tagGameKind));
		wSendSize+=sizeof(tagGameKind);
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_KIND,cbDataBuffer,wSendSize);

	return;
}

//���ͽڵ�
VOID CAttemperEngineSink::SendGameNodeInfo(DWORD dwSocketID, WORD wKindID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameNodeItem * pGameNodeItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameNodeCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameNode))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_NODE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameNodeItem=m_ServerListManager.EmunGameNodeItem(Position);
		if (pGameNodeItem==NULL) break;

		//��������
		if ((wKindID==INVALID_WORD)||(pGameNodeItem->m_GameNode.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameNodeItem->m_GameNode,sizeof(tagGameNode));
			wSendSize+=sizeof(tagGameNode);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_NODE,cbDataBuffer,wSendSize);

	return;
}

//���Ͷ���
VOID CAttemperEngineSink::SendGamePageInfo(DWORD dwSocketID, WORD wKindID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGamePageItem * pGamePageItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGamePageCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGamePage))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_PAGE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGamePageItem=m_ServerListManager.EmunGamePageItem(Position);
		if (pGamePageItem==NULL) break;

		//��������
		if ((wKindID==INVALID_WORD)||(pGamePageItem->m_GamePage.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGamePageItem->m_GamePage,sizeof(tagGamePage));
			wSendSize+=sizeof(tagGamePage);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_PAGE,cbDataBuffer,wSendSize);

	return;
}

//���ͷ���
VOID CAttemperEngineSink::SendGameServerInfo(DWORD dwSocketID, WORD wKindID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameServer))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SERVER,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;

		//��������
		if ((wKindID==INVALID_WORD)||(pGameServerItem->m_GameServer.wKindID==wKindID))
		{
			//if (pGameServerItem->m_GameServer.wServerID == 309)
			//{
			//	pGameServerItem->m_GameServer.dwServerFlag &= ~eFlagHaveLottery;
			//}
			//else
			{
				pGameServerItem->m_GameServer.dwServerFlag |= eFlagHaveLottery;
			}
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameServer,sizeof(tagGameServer));
			wSendSize+=sizeof(tagGameServer);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SERVER,cbDataBuffer,wSendSize);

	if(m_wAVServerPort!=0 && m_dwAVServerAddr!=0)
	{
		//��������
		tagAVServerOption AVServerOption;
		AVServerOption.wAVServerPort=m_wAVServerPort;
		AVServerOption.dwAVServerAddr=m_dwAVServerAddr;

		//��������
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_VIDEO_OPTION,&AVServerOption,sizeof(AVServerOption));
	};
	
	return;
}

//���ͷ���
VOID CAttemperEngineSink::SendGameServerInfoEx(DWORD dwSocketID, WORD wKindID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;

	//ö������
	tagSmallGameServer sgsXinShouRoom = {0};
	tagSmallGameServer sgsBaiPaoRoom = {0};
	tagSmallGameServer sgsQianPaoRoom = {0};
	tagSmallGameServer sgsWanPaoRoom = {0};
	tagSmallGameServer sgsXiaoChouRoom = {0};
	tagSmallGameServer sgsSixRoom = {0};
	DWORD dwOnLineCount1 = 0;
	DWORD dwOnLineCount2 = 0;
	DWORD dwOnLineCount3 = 0;
	DWORD dwOnLineCount4 = 0;
	DWORD dwOnLineCount5 = 0;
	DWORD dwOnLineCount6 = 0;
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//��ȡ����
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;
		//��������
		if ((wKindID==INVALID_WORD)||(pGameServerItem->m_GameServer.wKindID==wKindID))
		{
			if (pGameServerItem->m_GameServer.wServerPort > 10000 && pGameServerItem->m_GameServer.wServerPort < 20000)
			{
//printf("pGameServerItem->m_GameServer.wServerPort = %hu\n", pGameServerItem->m_GameServer.wServerPort);
				if (sgsXinShouRoom.wServerPort == 0)
				{
//printf("%s\n", pGameServerItem->m_GameServer.szServerName);
					dwOnLineCount1 = pGameServerItem->m_GameServer.dwOnLineCount;
					sgsXinShouRoom.wKindID = pGameServerItem->m_GameServer.wKindID;
					sgsXinShouRoom.wSortID = pGameServerItem->m_GameServer.wSortID;
					sgsXinShouRoom.wServerID = pGameServerItem->m_GameServer.wServerID;
					sgsXinShouRoom.wServerPort = pGameServerItem->m_GameServer.wServerPort;
					sgsXinShouRoom.dwServerFlag = pGameServerItem->m_GameServer.dwServerFlag | eFlagHaveLottery;
					sgsXinShouRoom.dwServerAddr = inet_addr(pGameServerItem->m_GameServer.szServerAddr);
					sgsXinShouRoom.lMinEnterScore = (int)pGameServerItem->m_GameServer.lMinEnterScore;
				}
				else
				{
					if (pGameServerItem->m_GameServer.dwOnLineCount < dwOnLineCount1)
					{
//printf("%s\n", pGameServerItem->m_GameServer.szServerName);
						dwOnLineCount1 = pGameServerItem->m_GameServer.dwOnLineCount;
						sgsXinShouRoom.wKindID = pGameServerItem->m_GameServer.wKindID;
						sgsXinShouRoom.wSortID = pGameServerItem->m_GameServer.wSortID;
						sgsXinShouRoom.wServerID = pGameServerItem->m_GameServer.wServerID;
						sgsXinShouRoom.wServerPort = pGameServerItem->m_GameServer.wServerPort;
						sgsXinShouRoom.dwServerFlag = pGameServerItem->m_GameServer.dwServerFlag | eFlagHaveLottery;
						sgsXinShouRoom.dwServerAddr = inet_addr(pGameServerItem->m_GameServer.szServerAddr);
						sgsXinShouRoom.lMinEnterScore = (int)pGameServerItem->m_GameServer.lMinEnterScore;
					}
				}
			}
			if (pGameServerItem->m_GameServer.wServerPort > 20000 && pGameServerItem->m_GameServer.wServerPort < 30000)
			{
				if (sgsBaiPaoRoom.wServerPort == 0)
				{
					dwOnLineCount2 = pGameServerItem->m_GameServer.dwOnLineCount;
					sgsBaiPaoRoom.wKindID = pGameServerItem->m_GameServer.wKindID;
					sgsBaiPaoRoom.wSortID = pGameServerItem->m_GameServer.wSortID;
					sgsBaiPaoRoom.wServerID = pGameServerItem->m_GameServer.wServerID;
					sgsBaiPaoRoom.wServerPort = pGameServerItem->m_GameServer.wServerPort;
					sgsBaiPaoRoom.dwServerFlag = pGameServerItem->m_GameServer.dwServerFlag | eFlagHaveLottery;
					sgsBaiPaoRoom.dwServerAddr = inet_addr(pGameServerItem->m_GameServer.szServerAddr);
					sgsBaiPaoRoom.lMinEnterScore = (int)pGameServerItem->m_GameServer.lMinEnterScore;
				}
				else
				{
					if (pGameServerItem->m_GameServer.dwOnLineCount < dwOnLineCount2)
					{
						dwOnLineCount2 = pGameServerItem->m_GameServer.dwOnLineCount;
						sgsBaiPaoRoom.wKindID = pGameServerItem->m_GameServer.wKindID;
						sgsBaiPaoRoom.wSortID = pGameServerItem->m_GameServer.wSortID;
						sgsBaiPaoRoom.wServerID = pGameServerItem->m_GameServer.wServerID;
						sgsBaiPaoRoom.wServerPort = pGameServerItem->m_GameServer.wServerPort;
						sgsBaiPaoRoom.dwServerFlag = pGameServerItem->m_GameServer.dwServerFlag | eFlagHaveLottery;
						sgsBaiPaoRoom.dwServerAddr = inet_addr(pGameServerItem->m_GameServer.szServerAddr);
						sgsBaiPaoRoom.lMinEnterScore = (int)pGameServerItem->m_GameServer.lMinEnterScore;
					}
				}
			}
			if (pGameServerItem->m_GameServer.wServerPort > 30000 && pGameServerItem->m_GameServer.wServerPort < 40000)
			{
				if (sgsQianPaoRoom.wServerPort == 0)
				{
					dwOnLineCount3 = pGameServerItem->m_GameServer.dwOnLineCount;
					sgsQianPaoRoom.wKindID = pGameServerItem->m_GameServer.wKindID;
					sgsQianPaoRoom.wSortID = pGameServerItem->m_GameServer.wSortID;
					sgsQianPaoRoom.wServerID = pGameServerItem->m_GameServer.wServerID;
					sgsQianPaoRoom.wServerPort = pGameServerItem->m_GameServer.wServerPort;
					sgsQianPaoRoom.dwServerFlag = pGameServerItem->m_GameServer.dwServerFlag | eFlagHaveLottery;
					sgsQianPaoRoom.dwServerAddr = inet_addr(pGameServerItem->m_GameServer.szServerAddr);
					sgsQianPaoRoom.lMinEnterScore = (int)pGameServerItem->m_GameServer.lMinEnterScore;
				}
				else
				{
					if (pGameServerItem->m_GameServer.dwOnLineCount < dwOnLineCount3)
					{
						dwOnLineCount3 = pGameServerItem->m_GameServer.dwOnLineCount;
						sgsQianPaoRoom.wKindID = pGameServerItem->m_GameServer.wKindID;
						sgsQianPaoRoom.wSortID = pGameServerItem->m_GameServer.wSortID;
						sgsQianPaoRoom.wServerID = pGameServerItem->m_GameServer.wServerID;
						sgsQianPaoRoom.wServerPort = pGameServerItem->m_GameServer.wServerPort;
						sgsQianPaoRoom.dwServerFlag = pGameServerItem->m_GameServer.dwServerFlag | eFlagHaveLottery;
						sgsQianPaoRoom.dwServerAddr = inet_addr(pGameServerItem->m_GameServer.szServerAddr);
						sgsQianPaoRoom.lMinEnterScore = (int)pGameServerItem->m_GameServer.lMinEnterScore;
					}
				}
			}
			if (pGameServerItem->m_GameServer.wServerPort > 40000 && pGameServerItem->m_GameServer.wServerPort < 50000)
			{
				if (sgsWanPaoRoom.wServerPort == 0)
				{
					dwOnLineCount4 = pGameServerItem->m_GameServer.dwOnLineCount;
					sgsWanPaoRoom.wKindID = pGameServerItem->m_GameServer.wKindID;
					sgsWanPaoRoom.wSortID = pGameServerItem->m_GameServer.wSortID;
					sgsWanPaoRoom.wServerID = pGameServerItem->m_GameServer.wServerID;
					sgsWanPaoRoom.wServerPort = pGameServerItem->m_GameServer.wServerPort;
					sgsWanPaoRoom.dwServerFlag = pGameServerItem->m_GameServer.dwServerFlag | eFlagHaveLottery;
					sgsWanPaoRoom.dwServerAddr = inet_addr(pGameServerItem->m_GameServer.szServerAddr);
					sgsWanPaoRoom.lMinEnterScore = (int)pGameServerItem->m_GameServer.lMinEnterScore;
				}
				else
				{
					if (pGameServerItem->m_GameServer.dwOnLineCount < dwOnLineCount4)
					{
						dwOnLineCount4 = pGameServerItem->m_GameServer.dwOnLineCount;
						sgsWanPaoRoom.wKindID = pGameServerItem->m_GameServer.wKindID;
						sgsWanPaoRoom.wSortID = pGameServerItem->m_GameServer.wSortID;
						sgsWanPaoRoom.wServerID = pGameServerItem->m_GameServer.wServerID;
						sgsWanPaoRoom.wServerPort = pGameServerItem->m_GameServer.wServerPort;
						sgsWanPaoRoom.dwServerFlag = pGameServerItem->m_GameServer.dwServerFlag | eFlagHaveLottery;
						sgsWanPaoRoom.dwServerAddr = inet_addr(pGameServerItem->m_GameServer.szServerAddr);
						sgsWanPaoRoom.lMinEnterScore = (int)pGameServerItem->m_GameServer.lMinEnterScore;
					}
				}
			}
			if (pGameServerItem->m_GameServer.wServerPort > 50000 && pGameServerItem->m_GameServer.wServerPort < 60000)
			{
				if (sgsXiaoChouRoom.wServerPort == 0)
				{
					dwOnLineCount5 = pGameServerItem->m_GameServer.dwOnLineCount;
					sgsXiaoChouRoom.wKindID = pGameServerItem->m_GameServer.wKindID;
					sgsXiaoChouRoom.wSortID = pGameServerItem->m_GameServer.wSortID;
					sgsXiaoChouRoom.wServerID = pGameServerItem->m_GameServer.wServerID;
					sgsXiaoChouRoom.wServerPort = pGameServerItem->m_GameServer.wServerPort;
					sgsXiaoChouRoom.dwServerFlag = pGameServerItem->m_GameServer.dwServerFlag | eFlagHaveLottery;
					sgsXiaoChouRoom.dwServerAddr = inet_addr(pGameServerItem->m_GameServer.szServerAddr);
					sgsXiaoChouRoom.lMinEnterScore = (int)pGameServerItem->m_GameServer.lMinEnterScore;
				}
				else
				{
					if (pGameServerItem->m_GameServer.dwOnLineCount < dwOnLineCount5)
					{
						dwOnLineCount5 = pGameServerItem->m_GameServer.dwOnLineCount;
						sgsXiaoChouRoom.wKindID = pGameServerItem->m_GameServer.wKindID;
						sgsXiaoChouRoom.wSortID = pGameServerItem->m_GameServer.wSortID;
						sgsXiaoChouRoom.wServerID = pGameServerItem->m_GameServer.wServerID;
						sgsXiaoChouRoom.wServerPort = pGameServerItem->m_GameServer.wServerPort;
						sgsXiaoChouRoom.dwServerFlag = pGameServerItem->m_GameServer.dwServerFlag | eFlagHaveLottery;
						sgsXiaoChouRoom.dwServerAddr = inet_addr(pGameServerItem->m_GameServer.szServerAddr);
						sgsXiaoChouRoom.lMinEnterScore = (int)pGameServerItem->m_GameServer.lMinEnterScore;
					}
				}
			}
			if (pGameServerItem->m_GameServer.wServerPort > 60000 && pGameServerItem->m_GameServer.wServerPort < 70000)
			{
				if (sgsXiaoChouRoom.wServerPort == 0)
				{
					dwOnLineCount6 = pGameServerItem->m_GameServer.dwOnLineCount;
					sgsSixRoom.wKindID = pGameServerItem->m_GameServer.wKindID;
					sgsSixRoom.wSortID = pGameServerItem->m_GameServer.wSortID;
					sgsSixRoom.wServerID = pGameServerItem->m_GameServer.wServerID;
					sgsSixRoom.wServerPort = pGameServerItem->m_GameServer.wServerPort;
					sgsSixRoom.dwServerFlag = pGameServerItem->m_GameServer.dwServerFlag | eFlagHaveLottery;
					sgsSixRoom.dwServerAddr = inet_addr(pGameServerItem->m_GameServer.szServerAddr);
					sgsSixRoom.lMinEnterScore = (int)pGameServerItem->m_GameServer.lMinEnterScore;
				}
				else
				{
					if (pGameServerItem->m_GameServer.dwOnLineCount < dwOnLineCount6)
					{
						dwOnLineCount6 = pGameServerItem->m_GameServer.dwOnLineCount;
						sgsSixRoom.wKindID = pGameServerItem->m_GameServer.wKindID;
						sgsSixRoom.wSortID = pGameServerItem->m_GameServer.wSortID;
						sgsSixRoom.wServerID = pGameServerItem->m_GameServer.wServerID;
						sgsSixRoom.wServerPort = pGameServerItem->m_GameServer.wServerPort;
						sgsSixRoom.dwServerFlag = pGameServerItem->m_GameServer.dwServerFlag | eFlagHaveLottery;
						sgsSixRoom.dwServerAddr = inet_addr(pGameServerItem->m_GameServer.szServerAddr);
						sgsSixRoom.lMinEnterScore = (int)pGameServerItem->m_GameServer.lMinEnterScore;
					}
				}
			}
		}
	}

	CopyMemory(cbDataBuffer+wSendSize,&sgsXinShouRoom,sizeof(tagSmallGameServer));
	wSendSize+=sizeof(tagSmallGameServer);
	CopyMemory(cbDataBuffer+wSendSize,&sgsBaiPaoRoom,sizeof(tagSmallGameServer));
	wSendSize+=sizeof(tagSmallGameServer);
	CopyMemory(cbDataBuffer+wSendSize,&sgsQianPaoRoom,sizeof(tagSmallGameServer));
	wSendSize+=sizeof(tagSmallGameServer);
	CopyMemory(cbDataBuffer+wSendSize,&sgsWanPaoRoom,sizeof(tagSmallGameServer));
	wSendSize+=sizeof(tagSmallGameServer);
	CopyMemory(cbDataBuffer+wSendSize,&sgsXiaoChouRoom,sizeof(tagSmallGameServer));
	wSendSize+=sizeof(tagSmallGameServer);
	CopyMemory(cbDataBuffer+wSendSize,&sgsSixRoom,sizeof(tagSmallGameServer));
	wSendSize+=sizeof(tagSmallGameServer);

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SMALL_SERVER,cbDataBuffer,wSendSize);

	return;
}

//��������
VOID CAttemperEngineSink::SendMobileKindInfo(DWORD dwSocketID, WORD wModuleID)
{
	return;
}

//���ͷ���
VOID CAttemperEngineSink::SendMobileServerInfo(DWORD dwSocketID, WORD wModuleID)
{
	//��������
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//ö������
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;

	//ö������
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//��������
		if ((wSendSize+sizeof(tagGameServer))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//��ȡ����
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;

		//��������
		if (pGameServerItem->m_GameServer.wKindID==wModuleID)
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameServer,sizeof(tagGameServer));
			wSendSize+=sizeof(tagGameServer);
		}
	}

	//����ʣ��
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER,cbDataBuffer,wSendSize);

	return;
}

//���ߴ���
bool CAttemperEngineSink::OnTCPNetworkMainItemService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GPR_QUERY_ITEM_COUNT:
		{
			if (wDataSize != sizeof(CMD_GPR_QueryItemCount))
			{
				return false;
			}

			CMD_GPR_QueryItemCount * pQueryItemCount = (CMD_GPR_QueryItemCount *)pData;
			DBR_GP_QueryItemCount QueryItemCount = {0};
			QueryItemCount.dwUserID = pQueryItemCount->dwUserID;

			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_ITEM_COUNT,dwSocketID,&QueryItemCount,sizeof(QueryItemCount));

			return true;
		}
	case SUB_GPR_TRANSFER_ITEM:
		{
			if (wDataSize!=sizeof(CMD_GPR_TransferItem)) return false;

			CMD_GPR_TransferItem * pTransferItem=(CMD_GPR_TransferItem *)pData;
			pTransferItem->szPassword[CountArray(pTransferItem->szPassword)-1]=0;

			if (pTransferItem->nItemCount <= 0)
			{
				return true;
			}

			LPCTSTR pszItemName[] = { TEXT("��������"), TEXT("�м�����"), TEXT("�߼�����"), TEXT("��������") };
			if (pTransferItem->nItemID >= CountArray(pszItemName))
			{
				return true;
			}

			DBR_GP_TransferItem TransferItem = {0};

			TransferItem.dwUserID=pTransferItem->dwUserID;
			TransferItem.dwTargetGameID=pTransferItem->dwTargetGameID;
			TransferItem.nItemID=pTransferItem->nItemID;
			TransferItem.nItemCount=pTransferItem->nItemCount;
			lstrcpyn(TransferItem.szPassword,pTransferItem->szPassword,CountArray(TransferItem.szPassword));
			lstrcpyn(TransferItem.szItemName,pszItemName[pTransferItem->nItemID],CountArray(TransferItem.szItemName));

			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_TRANSFER_ITEM,dwSocketID,&TransferItem,sizeof(TransferItem));

			return true;
		}
	case SUB_GPR_COMBINE_ITEM:
		{
			if (wDataSize!=sizeof(CMD_GPR_CombineItem)) return false;

			CMD_GPR_CombineItem * pCombineItem=(CMD_GPR_CombineItem *)pData;

			//ֻ�ܺϳ�С������ߴ�����
			if (pCombineItem->nCombineType != 1 && pCombineItem->nCombineType != 2)
			{
				return true;
			}

			DBR_GP_CombineItem CombineItem = {0};
			CombineItem.dwUserID=pCombineItem->dwUserID;
			CombineItem.nCombineType=pCombineItem->nCombineType;

			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_COMBINE_DRAGON,dwSocketID,&CombineItem,sizeof(CombineItem));

			return true;
		}
	case SUB_GPR_USE_DRAGON:
		{
			if (wDataSize!=sizeof(CMD_GPR_UseDragon)) return false;

			CMD_GPR_UseDragon * pUseDragon=(CMD_GPR_UseDragon *)pData;

			if (pUseDragon->nDragonType < 0 && pUseDragon->nDragonType > 3)
			{
				return true;
			}

			DBR_GP_UseDragon UseDragon = {0};
			UseDragon.dwUserID=pUseDragon->dwUserID;
			UseDragon.nDragonType=pUseDragon->nDragonType;

			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USE_DRAGON,dwSocketID,&UseDragon,sizeof(UseDragon));

			return true;
		}
	case SUB_GPR_EQUIPMENT_DRAGON:
		{
			if (wDataSize!=sizeof(CMD_GPR_EquipmentDragon)) return false;

			CMD_GPR_EquipmentDragon * pEquipmentDragon=(CMD_GPR_EquipmentDragon *)pData;

			if (pEquipmentDragon->nDragonType < 0 && pEquipmentDragon->nDragonType > 3)
			{
				return true;
			}

			DBR_GP_EquipmentDragon EquipmentDragon = {0};
			EquipmentDragon.dwUserID=pEquipmentDragon->dwUserID;
			EquipmentDragon.nDragonType=pEquipmentDragon->nDragonType;

			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_EQUIPMENT_DRAGON,dwSocketID,&EquipmentDragon,sizeof(EquipmentDragon));

			return true;
		}
	case SUB_GPR_EXCHANGE_LOTTERY:
		{
			if (wDataSize!=sizeof(CMD_GPR_ExchangeLottery)) return false;

			CMD_GPR_ExchangeLottery * pExchangeLottery=(CMD_GPR_ExchangeLottery *)pData;

			if (pExchangeLottery->nDragonType < 0 && pExchangeLottery->nDragonType > 3)
			{
				return true;
			}

			DBR_GP_ExchangeLottery ExchangeLottery  = {0};
			ExchangeLottery .dwUserID=pExchangeLottery->dwUserID;
			ExchangeLottery .nDragonType=pExchangeLottery->nDragonType;

			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_EXCHANGE_LOTTERY,dwSocketID,&ExchangeLottery ,sizeof(ExchangeLottery));

			return true;
		}
	}

	return true;
}

//���ܴ���
bool CAttemperEngineSink::OnTCPNetworkMainSkillService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GPR_QUERY_SKILL:
		{
			if (wDataSize != sizeof(CMD_GPR_QuerySkill))
			{
				return false;
			}

			CMD_GPR_QuerySkill * pQuerySkill = (CMD_GPR_QuerySkill *)pData;
			DBR_GP_QuerySkill QuerySkill = {0};
			QuerySkill.dwUserID = pQuerySkill->dwUserID;

			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_SKILL,dwSocketID,&QuerySkill,sizeof(QuerySkill));

			return true;
		}
	case SUB_GPR_NEW_GUID_QUERY:
		{
			if (wDataSize != sizeof(CMD_GPR_NewGuidQuery))
			{
				return true;
			}
			CMD_GPR_NewGuidQuery * pNewGuidQuery = (CMD_GPR_NewGuidQuery *)pData;

			DBR_GP_QueryNewGuid QueryNewGuid = {0};
			QueryNewGuid.dwUserID = pNewGuidQuery->dwUserID;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_NEW_GUID,dwSocketID,&QueryNewGuid,sizeof(QueryNewGuid));

			return true;
		}
	case SUB_GPR_SuperGold:
		{
			if (wDataSize != sizeof(CMD_GPR_SuperGold))
			{
				return true;
			}
			CMD_GPR_SuperGold * pSuperGold = (CMD_GPR_SuperGold *)pData;

			DBR_GP_SuperGold SuperGold = {0};
			SuperGold.dwUserID = pSuperGold->dwUserID;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_SUPER_GOLD,dwSocketID,&SuperGold,sizeof(SuperGold));

			return true;
		}
	}

	return true;
}

// �ֻ���֤�봦��
bool CAttemperEngineSink::OnTCPNetworkSubPCMBCODEVERIFY( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//CTraceService::TraceString(TEXT("sms: receive success"),TraceLevel_Warning);

	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GPR_MBVerificationCode));
	if (wDataSize<sizeof(CMD_GPR_MBVerificationCode))
	{
		if (wDataSize<sizeof(CMD_GPR_MBVerificationCode)-sizeof(BYTE))
			return false;
	}

	CMD_GPR_MBVerificationCode * pMBVerificationCode = (CMD_GPR_MBVerificationCode *)pData;

	//��������
	DBR_GP_MBVerifCode MBVerifiCode;
	ZeroMemory(&MBVerifiCode,sizeof(DBR_GP_MBVerifCode));

	//�������� 
	bool bIsVerifiCodeSent = false;
	TCHAR szVerificationCode[LEN_PASSWORD];
	GenRandNumCode(szVerificationCode);
	lstrcpyn(MBVerifiCode.szMobilePhone,pMBVerificationCode->szMobilePhone,CountArray(MBVerifiCode.szMobilePhone));
	lstrcpyn(MBVerifiCode.szVerificationCode,szVerificationCode,CountArray(MBVerifiCode.szVerificationCode));

	std::string strMobilePhone = pMBVerificationCode->szMobilePhone;
	std::map<std::string,tagMapMBVerifInfo>::iterator iter = m_mMBVerifInfo.find(strMobilePhone); 
	if(iter!=m_mMBVerifInfo.end())
	{
		time_t now = time(0);
		//������͹�������60������·���
		if (difftime(now, (*iter).second.m_dwVerifCodeSendTime) >= 60) 
		{
			m_mMBVerifInfo.erase(iter);
			bIsVerifiCodeSent = false;
		}
		else
		{
			BYTE cbDataBuffer[SOCKET_TCP_PACKET];
			CMD_GPO_MBVerificationCode * pCMDMBVerifyRes=(CMD_GPO_MBVerificationCode *)cbDataBuffer;
			WORD wSendSize=sizeof(CMD_GPO_MBVerificationCode);
			pCMDMBVerifyRes->bIsVerificationCodeSend = false;
			lstrcpyn(pCMDMBVerifyRes->szDescribeString,TEXT("�벻ҪƵ��������֤��!"),CountArray(pCMDMBVerifyRes->szDescribeString));
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GPO_MBCODE_VERIFY,cbDataBuffer,wSendSize);
			return true;
		}
	}


	if (bIsVerifiCodeSent == false)
	{
		//�ж��ֻ����Ƿ�ע���
		std::map<std::string,int>::iterator iter = m_mRegisteredMbNum.find(strMobilePhone);
		if(iter==m_mRegisteredMbNum.end())
		{
			tagMapMBVerifInfo MBVerifInfoVecEle;
			ZeroMemory(&MBVerifInfoVecEle,sizeof(tagMapMBVerifInfo));
			MBVerifInfoVecEle.m_dwVerifCodeSendTime = time(0);
			lstrcpyn(MBVerifInfoVecEle.szVerificationCode,szVerificationCode,CountArray(MBVerifInfoVecEle.szVerificationCode));
			m_mMBVerifInfo.insert(std::pair<std::string,tagMapMBVerifInfo>(strMobilePhone,MBVerifInfoVecEle));
			//�����´������

			CVerification *pVerification = CVerification::GetInstance();
			if (pVerification!=NULL)
			{
				pVerification->AddNewVerificationCode(dwSocketID,szVerificationCode,pMBVerificationCode->szMobilePhone);
			}
		}
		else
		{
			BYTE cbDataBuffer[SOCKET_TCP_PACKET];
			CMD_GPO_MBVerificationCode * pCMDMBVerifyRes=(CMD_GPO_MBVerificationCode *)cbDataBuffer;
			ZeroMemory(pCMDMBVerifyRes,sizeof(CMD_GPO_MBVerificationCode));

			pCMDMBVerifyRes->bIsVerificationCodeSend = false;
			lstrcpyn(pCMDMBVerifyRes->szDescribeString,TEXT("�ֻ����Ѿ�ע��"),CountArray(pCMDMBVerifyRes->szDescribeString));

			//�ֻ����Ѿ�ע��
			WORD wSendSize=sizeof(CMD_GPO_MBVerificationCode);
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GPO_MBCODE_VERIFY,cbDataBuffer,wSendSize);

		}

	}
	return true;
}

bool CAttemperEngineSink::OnDBPCVerifyMBNumRes(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//CTraceService::TraceString(TEXT("sms: OnDBPCVerifyMBNumRes"),TraceLevel_Warning);

	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_MB_Check_Can_Bind * pDBOMBVerifCode=(DBO_GP_MB_Check_Can_Bind *)pData;

	std::string strMobilePhone = pDBOMBVerifCode->szMobilePhone;


	if (pDBOMBVerifCode->byResult == 0)
	{

		CVerification *pVerification = CVerification::GetInstance();
		if (pVerification!=NULL)
		{
			pVerification->AddNewVerificationCode(dwContextID,pDBOMBVerifCode->szVerificationCode,pDBOMBVerifCode->szMobilePhone);
		}
		return true;
	}
	else if(pDBOMBVerifCode->byResult == 1)
	{
		//���û��Ѿ����ֻ���
		BYTE cbDataBuffer[SOCKET_TCP_PACKET];
		CMD_GPO_MBVerificationCode * pCMDMBVerifyRes=(CMD_GPO_MBVerificationCode *)cbDataBuffer;
		WORD wSendSize=sizeof(CMD_GPO_MBVerificationCode);
		pCMDMBVerifyRes->bIsVerificationCodeSend = false;
		lstrcpyn(pCMDMBVerifyRes->szDescribeString,TEXT("���û��Ѿ����ֻ���!"),CountArray(pCMDMBVerifyRes->szDescribeString));
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GPO_MBCODE_VERIFY,cbDataBuffer,wSendSize);
	}
	else if(pDBOMBVerifCode->byResult == 2)
	{
		//���ֻ����Ѿ���
		BYTE cbDataBuffer[SOCKET_TCP_PACKET];
		CMD_GPO_MBVerificationCode * pCMDMBVerifyRes=(CMD_GPO_MBVerificationCode *)cbDataBuffer;
		WORD wSendSize=sizeof(CMD_GPO_MBVerificationCode);
		pCMDMBVerifyRes->bIsVerificationCodeSend = false;
		lstrcpyn(pCMDMBVerifyRes->szDescribeString,TEXT("���ֻ����Ѿ���!"),CountArray(pCMDMBVerifyRes->szDescribeString));
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GPO_MBCODE_VERIFY,cbDataBuffer,wSendSize);
	}
	return true;
}

// �ֻ���ע��ɹ�����
bool CAttemperEngineSink::OnDBMBNumRegisterSuccessRes(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_MBRegisterRes * pDBOMBRegisterRes=(DBO_GP_MBRegisterRes *)pData;

	std::string strMobilePhone = pDBOMBRegisterRes->szMobilePhone;
	std::map<std::string,tagMapMBVerifInfo>::iterator iter;
	iter = m_mMBVerifInfo.find(strMobilePhone);
	CTraceService::TraceString("in the db mb num proces",TraceLevel_Normal);
	if(iter!=m_mMBVerifInfo.end())
	{
		// debug test
		CTraceService::TraceString("hehe erase success",TraceLevel_Normal);
		printf("hhehe  size of the %d \n", m_mMBVerifInfo.size());

		m_mMBVerifInfo.erase(iter);
	}

	m_mRegisteredMbNum.insert(std::pair<std::string,int>(strMobilePhone,1));

	return true;
}

char* CAttemperEngineSink::ConvertLPWSTRToLPSTR (LPWSTR lpwszStrIn)  
{  
	LPSTR pszOut = NULL;  
	if (lpwszStrIn != NULL)  
	{  
		int nInputStrLen = wcslen (lpwszStrIn);  

		// Double NULL Termination  
		int nOutputStrLen = WideCharToMultiByte (CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;  
		pszOut = new char [nOutputStrLen];  

		if (pszOut)  
		{  
			memset (pszOut, 0x00, nOutputStrLen);  
			WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);  
		}  
	}  
	return pszOut;  
}  //////////////////////////////////////////////////////////////////////////////////

//�ֻ���ע��
bool CAttemperEngineSink::OnTCPNetworkSubPCMBNumRegister( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_MBNumRegister));
	if (wDataSize<sizeof(CMD_GP_MBNumRegister))
	{
		if (wDataSize<sizeof(CMD_GP_MBNumRegister)-sizeof(BYTE))
			return false;
	}

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_GP_MBNumRegister * pMBNumRegisterAccounts=(CMD_GP_MBNumRegister *)pData;
	bool bIsVerificationCodeCorrect = false;
	pMBNumRegisterAccounts->RegisterAccounts.szMobilePhone[CountArray(pMBNumRegisterAccounts->RegisterAccounts.szMobilePhone)-1]=0;
	pMBNumRegisterAccounts->szVerificationCode[CountArray(pMBNumRegisterAccounts->szVerificationCode)-1]=0;

	// ��֤����֤
	std::string strMobilePhone = pMBNumRegisterAccounts->RegisterAccounts.szMobilePhone;
	std::map<std::string,tagMapMBVerifInfo>::iterator iter;
	iter = m_mMBVerifInfo.find(strMobilePhone);
	if(iter!=m_mMBVerifInfo.end())
	{
		if (lstrcmp(pMBNumRegisterAccounts->szVerificationCode, iter->second.szVerificationCode) == 0)
		{
			printf("verification code suc");
			bIsVerificationCodeCorrect = true;
		}
	}

	if (bIsVerificationCodeCorrect == false)
	{
		//��������
		BYTE cbDataBuffer[SOCKET_TCP_PACKET];
		CMD_GPO_MBVerificationCode * pCMDMBVerifyRes=(CMD_GPO_MBVerificationCode *)cbDataBuffer;
		WORD wSendSize=sizeof(CMD_GPO_MBVerificationCode);

		pCMDMBVerifyRes->bIsVerificationCodeSend = false;
		lstrcpyn(pCMDMBVerifyRes->szDescribeString,TEXT("��֤����֤���������֤���Ѿ�����!"),CountArray(pCMDMBVerifyRes->szDescribeString));
		printf("error in verification code verify");

		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GPO_MBCODE_VERIFY,cbDataBuffer,wSendSize);
		return true;
	}

	VOID * pNewData = (VOID*)(&pMBNumRegisterAccounts->RegisterAccounts);
	WORD wNewDataSize = wDataSize - sizeof(pMBNumRegisterAccounts->szVerificationCode);
	//OnTCPNetworkSubPCRegisterAccounts(pNewData,wNewDataSize,dwSocketID);

	//������Ϣ
	CMD_GP_RegisterAccounts * pRegisterAccounts=&pMBNumRegisterAccounts->RegisterAccounts;
	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
	pRegisterAccounts->szSpreader[CountArray(pRegisterAccounts->szSpreader)-1]=0;
	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
	pRegisterAccounts->szInsurePass[CountArray(pRegisterAccounts->szInsurePass)-1]=0;
	pRegisterAccounts->szPassPortID[CountArray(pRegisterAccounts->szPassPortID)-1]=0;
	pRegisterAccounts->szCompellation[CountArray(pRegisterAccounts->szCompellation)-1]=0;
	pRegisterAccounts->szMobilePhone[CountArray(pRegisterAccounts->szMobilePhone)-1]=0;


	//--------
	//���д�
	CString sNickName = pRegisterAccounts->szNickName;
	for (int i = 0; i < (sizeof(pszSensitiveWords)/sizeof(pszSensitiveWords[0])); i++)
	{
		if (sNickName.Find(pszSensitiveWords[i]) != -1)
		{
			printf("sensitive words %s %s %d\n", sNickName, pszSensitiveWords[i], i);
			CMD_GP_LogonFailure LogonFailure = {0};

			LogonFailure.lResultCode = 0;
			lstrcpyn(LogonFailure.szDescribeString,TEXT("�����ǳ��а������дʻ㣬ע��ʧ�ܣ�"),CountArray(LogonFailure.szDescribeString));

			WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_LOGON_FAILURE,&LogonFailure,wSendSize);

			return true;
		}
	}
	//--------

	//��������
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	//Ч��汾
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pRegisterAccounts->dwPlazaVersion,dwSocketID,((pRegisterAccounts->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//��������
	DBR_GP_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//������Ϣ
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//��������
	RegisterAccounts.wFaceID=pRegisterAccounts->wFaceID;
	RegisterAccounts.cbGender=pRegisterAccounts->cbGender;
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szAccounts,pRegisterAccounts->szAccounts,CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName,pRegisterAccounts->szNickName,CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szSpreader,pRegisterAccounts->szSpreader,CountArray(RegisterAccounts.szSpreader));
	lstrcpyn(RegisterAccounts.szMachineID,pRegisterAccounts->szMachineID,CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szLogonPass,pRegisterAccounts->szLogonPass,CountArray(RegisterAccounts.szLogonPass));
	lstrcpyn(RegisterAccounts.szInsurePass,pRegisterAccounts->szInsurePass,CountArray(RegisterAccounts.szInsurePass));
	lstrcpyn(RegisterAccounts.szPassPortID,pRegisterAccounts->szPassPortID,CountArray(RegisterAccounts.szPassPortID));
	lstrcpyn(RegisterAccounts.szCompellation,pRegisterAccounts->szCompellation,CountArray(RegisterAccounts.szCompellation));
	lstrcpyn(RegisterAccounts.szMobilePhone,pRegisterAccounts->szMobilePhone,CountArray(RegisterAccounts.szMobilePhone));

	DBR_GP_RecordLogon RecordLogon = {0};
	lstrcpyn(RecordLogon.szAccounts, pRegisterAccounts->szAccounts, CountArray(RecordLogon.szAccounts));
	lstrcpyn(RecordLogon.szMachineID, pRegisterAccounts->szMachineID, CountArray(RecordLogon.szMachineID));

	BYTE * pByte = (BYTE *)(&RegisterAccounts.dwClientAddr);
	sprintf(RecordLogon.szWAN, "%hhd.%hhd.%hhd.%hhd", pByte[0], pByte[1], pByte[2], pByte[3]);

	if (lstrcmp(pRegisterAccounts->szPassPortID, "iphone") == 0)
	{
		RecordLogon.cbDeviceType = 20;
		RecordLogon.cbChannel = 10;
	}
	else if (lstrcmp(pRegisterAccounts->szPassPortID, "wapiPhone") == 0)
	{
		RecordLogon.cbDeviceType = 20;
		RecordLogon.cbChannel = 20;
	}
	else if (lstrcmp(pRegisterAccounts->szPassPortID, "mwapiPhone") == 0)
	{
		RecordLogon.cbDeviceType = 20;
		RecordLogon.cbChannel = 30;
	}
	else if (lstrcmp(pRegisterAccounts->szPassPortID, "android") == 0)
	{
		RecordLogon.cbDeviceType = 30;
		RecordLogon.cbChannel = 10;
	}
	else if (lstrcmp(pRegisterAccounts->szPassPortID, "wapAndroid") == 0)
	{
		RecordLogon.cbDeviceType = 30;
		RecordLogon.cbChannel = 20;
	}
	else if (lstrcmp(pRegisterAccounts->szPassPortID, "mwapAndroid") == 0)
	{
		RecordLogon.cbDeviceType = 30;
		RecordLogon.cbChannel = 30;
	}

	bool bHaveRecord = false;

	//��չ��Ϣ
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pRegisterAccounts+1,wDataSize-sizeof(CMD_GP_RegisterAccounts));
	while (true)
	{
		//��ȡ����
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;

		char szBuffer[2048] = {0};
		//��ȡ����
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_REG_AGENTID:	//�Ƽ�����Ϣ
			{
				ASSERT(DataDescribe.wDataSize==sizeof(DTP_GP_AgentInfo));
				if (DataDescribe.wDataSize==sizeof(DTP_GP_AgentInfo))
				{
					DTP_GP_AgentInfo * pAgentInfo=(DTP_GP_AgentInfo *)pDataBuffer;
					RegisterAccounts.dwAgentID=pAgentInfo->dwAgentID;
				}
				break;
			}
		case DTP_GP_USERDATA1:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData1, szBuffer, CountArray(RecordLogon.szUserData1));
				}
				break;
			}
		case DTP_GP_USERDATA2:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData2, szBuffer, CountArray(RecordLogon.szUserData2));
				}
				break;
			}
		case DTP_GP_USERDATA3:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData3, szBuffer, CountArray(RecordLogon.szUserData3));
				}
				break;
			}
		case DTP_GP_USERDATA4:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData4, szBuffer, CountArray(RecordLogon.szUserData4));
				}
				break;
			}
		case DTP_GP_USERDATA5:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData5, szBuffer, CountArray(RecordLogon.szUserData5));
				}
				break;
			}
		case DTP_GP_USERDATA6:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData6, szBuffer, CountArray(RecordLogon.szUserData6));
				}
				break;
			}
		case DTP_GP_USERDATA7:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData7, szBuffer, CountArray(RecordLogon.szUserData7));
				}
				break;
			}
		case DTP_GP_USERDATA8:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szUserData8, szBuffer, CountArray(RecordLogon.szUserData8));
				}
				break;
			}
		case DTP_GP_LOCALIP:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szLAN, szBuffer, CountArray(RecordLogon.szLAN));
				}
				break;
			}
		case DTP_GP_CLIENTVERSION:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RecordLogon.szClientVersion, szBuffer, CountArray(RecordLogon.szClientVersion));
				}
				break;
			}
		case DTP_GP_CLIENTIDFA:
			{
				if (DataDescribe.wDataSize > 0)
				{
					memcpy(szBuffer, pDataBuffer, DataDescribe.wDataSize);
					szBuffer[DataDescribe.wDataSize] = 0;
					lstrcpyn(RegisterAccounts.szIDFA, szBuffer, CountArray(RegisterAccounts.szIDFA));
				}
				break;
			}
		case DTP_GP_PASSWORD:
			{
				if (DataDescribe.wDataSize > 2)
				{
					bHaveRecord = true;
					DTP_GP_Password * pPassword = (DTP_GP_Password *)pDataBuffer;
					//BYTE cbKey = 0xAA;
					//for (WORD i = 0; i < pPassword->wCount; i++)
					//{
					//	printf("%hhx", (unsigned char)pPassword->cbData[i]);
					//	pPassword->cbData[i] ^= cbKey;
					//	cbKey++;
					//}
					//printf("\n");
					for (WORD i = 0; i < pPassword->wCount; i++)
					{
						char szTemp[8];
						sprintf(szTemp, "%02x", (unsigned char)pPassword->cbData[i]);
						strcat(szBuffer, szTemp);
					}
					szBuffer[pPassword->wCount*2] = 0;
					lstrcpyn(RecordLogon.szLogonPass, szBuffer, CountArray(RecordLogon.szLogonPass));
				}
				break;
			}
			//case DTP_GP_POSITION:
			//	{
			//		if (DataDescribe.wDataSize > 0)
			//		{
			//			DTP_GP_Position * pPosition = (DTP_GP_Position *)pDataBuffer;
			//			printf("longitude is %f latitude is %f\n", pPosition->longitude, pPosition->latitude);
			//		}
			//		break;
			//	}
		}
	}


	CTraceService::TraceString(TEXT("mb reg go in"),TraceLevel_Exception);
	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MB_REGITER,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));
	return true;
}

//������λ�����
VOID CAttemperEngineSink::GenRandNumCode(TCHAR szRandCode[])
{
	TCHAR szCharBase[100];
	INT iCharNum=0;

	lstrcpyn(szCharBase,TEXT("0123456789"),CountArray(szCharBase));

	for(iCharNum=0;iCharNum<6;iCharNum++)
	{
		szRandCode[iCharNum]=szCharBase[rand()%10];
	}

	szRandCode[iCharNum]='\0';

	return;
}



// �ֻ�����֤��������
bool CAttemperEngineSink::OnTCPNetworkSubPCMoBileCode( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//CTraceService::TraceString(TEXT("sms: receive success"),TraceLevel_Warning);

	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GPR_MB_BIND_REQUEST));
	if (wDataSize<sizeof(CMD_GPR_MB_BIND_REQUEST))
	{
		if (wDataSize<sizeof(CMD_GPR_MB_BIND_REQUEST)-sizeof(BYTE))
			return false;
	}

	CMD_GPR_MB_BIND_REQUEST * pMBVerificationCode = (CMD_GPR_MB_BIND_REQUEST *)pData;

	//��������
	DBR_GP_MBVerifCode MBVerifiCode;
	ZeroMemory(&MBVerifiCode,sizeof(DBR_GP_MBVerifCode));

	//�������� 
	bool bIsVerifiCodeSent = false;
	TCHAR szVerificationCode[LEN_PASSWORD];
	GenRandNumCode(szVerificationCode);
	MBVerifiCode.dwUserId = pMBVerificationCode->dwUserId;
	lstrcpyn(MBVerifiCode.szMobilePhone,pMBVerificationCode->szMobilePhone,CountArray(MBVerifiCode.szMobilePhone));
	lstrcpyn(MBVerifiCode.szVerificationCode,szVerificationCode,CountArray(MBVerifiCode.szVerificationCode));

	std::string strMobilePhone = pMBVerificationCode->szMobilePhone;
	std::map<std::string,tagMapMBVerifInfo>::iterator iter = m_mMBVerifInfo.find(strMobilePhone); 
	if(iter!=m_mMBVerifInfo.end())
	{
		time_t now = time(0);
		//������͹�������60������·���
		if (difftime(now, (*iter).second.m_dwVerifCodeSendTime) >= 60) 
		{
			m_mMBVerifInfo.erase(iter);
			bIsVerifiCodeSent = false;
		}
		else
		{
			BYTE cbDataBuffer[SOCKET_TCP_PACKET];
			CMD_GPO_MBVerificationCode * pCMDMBVerifyRes=(CMD_GPO_MBVerificationCode *)cbDataBuffer;
			WORD wSendSize=sizeof(CMD_GPO_MBVerificationCode);
			pCMDMBVerifyRes->bIsVerificationCodeSend = false;
			lstrcpyn(pCMDMBVerifyRes->szDescribeString,TEXT("�벻ҪƵ��������֤��!"),CountArray(pCMDMBVerifyRes->szDescribeString));
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GPO_MBCODE_VERIFY,cbDataBuffer,wSendSize);
			return true;
		}
	}


	if (bIsVerifiCodeSent == false)
	{
		//�ж��ֻ����Ƿ�󶨹�
		tagMapMBVerifInfo MBVerifInfoVecEle;
		ZeroMemory(&MBVerifInfoVecEle,sizeof(tagMapMBVerifInfo));
		MBVerifInfoVecEle.m_dwVerifCodeSendTime = time(0);
		lstrcpyn(MBVerifInfoVecEle.szVerificationCode,szVerificationCode,CountArray(MBVerifInfoVecEle.szVerificationCode));
		m_mMBVerifInfo.insert(std::pair<std::string,tagMapMBVerifInfo>(strMobilePhone,MBVerifInfoVecEle));

		//Ͷ������
		m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_VERIFY_MBNUM,dwSocketID,&MBVerifiCode,sizeof(MBVerifiCode));

	}
	return true;
}

//�ֻ��Ű���֤����֤
bool CAttemperEngineSink::OnTCPNetworkSubPCCodeVerification( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GP_MobileCaptchaVerific));
	if (wDataSize<sizeof(CMD_GP_MobileCaptchaVerific))
	{
		if (wDataSize<sizeof(CMD_GP_MobileCaptchaVerific)-sizeof(BYTE))
			return false;
	}

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//������Ϣ
	CMD_GP_MobileCaptchaVerific * pMBCaptchaVerific=(CMD_GP_MobileCaptchaVerific *)pData;
	bool bIsVerificationCodeCorrect = false;
	pMBCaptchaVerific->szMobilePhone[CountArray(pMBCaptchaVerific->szMobilePhone)-1]=0;
	pMBCaptchaVerific->szVerificationCode[CountArray(pMBCaptchaVerific->szVerificationCode)-1]=0;
	pMBCaptchaVerific->szLogonPass[CountArray(pMBCaptchaVerific->szLogonPass)-1]=0;

	// ��֤����֤
	std::string strMobilePhone = pMBCaptchaVerific->szMobilePhone;
	std::map<std::string,tagMapMBVerifInfo>::iterator iter;
	iter = m_mMBVerifInfo.find(strMobilePhone);
	if(iter!=m_mMBVerifInfo.end())
	{
		if (lstrcmp(pMBCaptchaVerific->szVerificationCode, iter->second.szVerificationCode) == 0)
		{
			printf("verification code suc");
			bIsVerificationCodeCorrect = true;
		}
	}

	if (bIsVerificationCodeCorrect == false)
	{
		//��������
		BYTE cbDataBuffer[SOCKET_TCP_PACKET];
		CMD_GPO_MBVerificationCode * pCMDMBVerifyRes=(CMD_GPO_MBVerificationCode *)cbDataBuffer;
		WORD wSendSize=sizeof(CMD_GPO_MBVerificationCode);

		pCMDMBVerifyRes->bIsVerificationCodeSend = false;
		lstrcpyn(pCMDMBVerifyRes->szDescribeString,TEXT("��֤����֤���������֤���Ѿ�����!"),CountArray(pCMDMBVerifyRes->szDescribeString));

		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GPO_MBCODE_VERIFY,cbDataBuffer,wSendSize);
		return true;
	}
	DBR_GP_MBBindNum sMbBindNum;
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pMBCaptchaVerific+1,wDataSize-sizeof(CMD_GP_MobileCaptchaVerific));
	lstrcpyn(sMbBindNum.szNickName,"",LEN_NICKNAME);
	//��չ��Ϣ
	while (true)
	{
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_UI_NICKNAME:		//�û��ǳ�
			{
				ASSERT(pDataBuffer!=NULL);
				ASSERT(DataDescribe.wDataSize<=LEN_NICKNAME);
				if (DataDescribe.wDataSize<=LEN_NICKNAME)
				{
					CopyMemory(sMbBindNum.szNickName,pDataBuffer,DataDescribe.wDataSize);
					sMbBindNum.szNickName[DataDescribe.wDataSize]=0;
				}
				break;
			}
		}
	}

	//CTraceService::TraceString(TEXT("will go in register"),TraceLevel_Exception);
	
	sMbBindNum.dwUserID = pMBCaptchaVerific->dwUserId;
	lstrcpyn(sMbBindNum.szLogonPass,pMBCaptchaVerific->szLogonPass,CountArray(sMbBindNum.szLogonPass));
	lstrcpyn(sMbBindNum.szMobilePhone,pMBCaptchaVerific->szMobilePhone,CountArray(sMbBindNum.szMobilePhone));
	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MB_BIND_NUM,dwSocketID,&sMbBindNum,sizeof(sMbBindNum));
	if(iter!=m_mMBVerifInfo.end())
	{
		m_mMBVerifInfo.erase(iter);
	}

	return true;
}

// ��ѯ�ֻ����Ƿ�󶨹�
bool CAttemperEngineSink::OnTCPNetworkSubPCMobileCheckExit( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//CTraceService::TraceString(TEXT("sms: receive success"),TraceLevel_Warning);

	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GPR_NumCheckExit));
	if (wDataSize<sizeof(CMD_GPR_NumCheckExit))
	{
		if (wDataSize<sizeof(CMD_GPR_NumCheckExit)-sizeof(BYTE))
			return false;
	}

	CMD_GPR_NumCheckExit * pMBCheckExit = (CMD_GPR_NumCheckExit*)pData;

	//��������
	DBR_GP_CHECK_BINDNUM_EXIT BindNum_Exit;
	ZeroMemory(&BindNum_Exit,sizeof(BindNum_Exit));

	BindNum_Exit.dwUserID = pMBCheckExit->dwUserID;

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MB_CHECK_NUM_EXIT,dwSocketID,&BindNum_Exit,sizeof(BindNum_Exit));

	return true;
}

//�㲥�ٶ�
bool CAttemperEngineSink::OnTCPNetworkSubPCBroadCastSpeed( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_BROADCAST_SPEED,dwSocketID,pData,wDataSize);

	return true;
}


bool CAttemperEngineSink::OnTCPNetworkSubPCItemCount( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_TYPE_ITEM_COUNT,dwSocketID,pData,wDataSize);

	return true;
}

bool CAttemperEngineSink::OnTCPNetworkSubPCChangePhoneFare( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CHANGE_PHONE_FARE,dwSocketID,pData,wDataSize);

	return true;
}

bool CAttemperEngineSink::OnTCPNetworkSubPCBroadLaBa( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//Ͷ������
	//Ч�����
	CMD_GPR_BroadLaba *pBroadLaba = (CMD_GPR_BroadLaba*)pData;
	DBR_GP_REQUEST_BROADLABA requestBroadLaba;
	requestBroadLaba.dwUserId = pBroadLaba->dwUserID;
	requestBroadLaba.wSize = pBroadLaba->wSize;
	lstrcpyn(requestBroadLaba.szContent,pBroadLaba->szContent,CountArray(requestBroadLaba.szContent));
	lstrcpyn(requestBroadLaba.szNickName,pBroadLaba->szNickName,CountArray(requestBroadLaba.szNickName));
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_BROADLABA,dwSocketID,&requestBroadLaba,sizeof(requestBroadLaba));

	return true;
}

bool CAttemperEngineSink::OnDBGetCompRankInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DB0_GP_COMPRANK_INFO_RES)==0);
	if (wDataSize%sizeof(DB0_GP_COMPRANK_INFO_RES)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DB0_GP_COMPRANK_INFO_RES);
	DB0_GP_COMPRANK_INFO_RES * pInfo=(DB0_GP_COMPRANK_INFO_RES *)pData;
	m_CompRankInfo.clear();
	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagCompRankInfo sCompRankInfo;
		ZeroMemory(&sCompRankInfo,sizeof(sCompRankInfo));

		//��������
		sCompRankInfo.nRank=(pInfo+i)->nRank;
		sCompRankInfo.nUserId=(pInfo+i)->nUserId;
		sCompRankInfo.nCount=(pInfo+i)->nCount;
		m_CompRankInfo.push_back(sCompRankInfo);
	}

	return true;
}

bool CAttemperEngineSink::OnDBBroadCastResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	DB0_GP_BROADLABA_RES * pDBBroadLaBa=(DB0_GP_BROADLABA_RES *)pData;
	CMD_GPO_BroadLaba BroadLabaRes = {0};
	if (pDBBroadLaBa->lResult == 0)
	{
		BroadLabaRes.cbSuccess = 1;
		lstrcpyn(BroadLabaRes.szContent,pDBBroadLaBa->szErrorDes,CountArray(BroadLabaRes.szContent));
		BroadLabaRes.score = pDBBroadLaBa->score;
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GPO_BROAD_LABA,&BroadLabaRes,sizeof(CMD_GPO_BroadLaba));
		CMD_CS_C_BroadLaBa sBroadLaBa = {0};
		CString sContent = pDBBroadLaBa->szContent;
		for (int i = 0; i < (sizeof(pszSensitiveWords)/sizeof(pszSensitiveWords[0])); i++)
		{
			if (sContent.Find(pszSensitiveWords[i]) != -1)
			{
				CString cbadstr = pszSensitiveWords[i];
				int nSize = cbadstr.GetLength()/2;
				CString sReplace = "";
				for (int j =0; j<nSize;j++)
				{
					sReplace = sReplace + '*';
				}
				sContent.Replace(pszSensitiveWords[i],sReplace);
			}
		}
		lstrcpyn(sBroadLaBa.szContent,sContent,CountArray(sBroadLaBa.szContent));
		lstrcpyn(sBroadLaBa.szNickName,pDBBroadLaBa->szNickName,CountArray(sBroadLaBa.szNickName));
		sBroadLaBa.wSize = pDBBroadLaBa->wSize;
		m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE, SUB_CS_C_BROAD_LABA, &sBroadLaBa, sizeof(sBroadLaBa));
		return true;
	}
	else if(pDBBroadLaBa->lResult == 1)
	{
		BroadLabaRes.cbSuccess = 0;
		lstrcpyn(BroadLabaRes.szContent,pDBBroadLaBa->szErrorDes,CountArray(BroadLabaRes.szContent));
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GPO_BROAD_LABA,&BroadLabaRes,sizeof(CMD_GPO_BroadLaba));
	}
	return true;
}

bool CAttemperEngineSink::OnCancelLoginQueueing(DWORD dwSocketID)
{
	//�ж�����
	std::vector<tagLoginQueueInfo>::iterator iter;
	for (iter=m_LoginQueueInfo.begin(); iter!=m_LoginQueueInfo.end(); iter++)
	{
		if (iter->dwSocketId == dwSocketID)
		{
			m_LoginQueueInfo.erase(iter);
		}
		break;
	}
	return true;
}

bool CAttemperEngineSink::OnRequestDataBaseLogin()
{
	if (m_LoginQueueInfo.size()>0)
	{
		std::vector<tagLoginQueueInfo>::iterator iter;
		iter = m_LoginQueueInfo.begin();
		if (iter==m_LoginQueueInfo.end())
		{
			return true;
		}
		DBR_GP_LogonAccounts LogonAccounts;
		LogonAccounts.cbNeeValidateMBCard = (*iter).cbNeeValidateMBCard;
		LogonAccounts.dwClientAddr = (*iter).dwClientAddr;
		LogonAccounts.pBindParameter = (*iter).pBindParameter;
		lstrcpyn(LogonAccounts.szAccounts,(*iter).szAccounts,CountArray(LogonAccounts.szAccounts));
		lstrcpyn(LogonAccounts.szPassword,(*iter).szPassword,CountArray(LogonAccounts.szPassword));
		lstrcpyn(LogonAccounts.szPassPortID,(*iter).szPassPortID,CountArray(LogonAccounts.szPassPortID));
		lstrcpyn(LogonAccounts.szMachineID,(*iter).szMachineID,CountArray(LogonAccounts.szMachineID));
		DWORD dwSocketID = (*iter).dwSocketId;
		m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));
		m_LoginQueueInfo.erase(iter);
		m_nSendMaxNum++;
	}
	return true;
}

bool CAttemperEngineSink::OnDBChangePhoneFareResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	DB0_GP_CHANGE_PHONE_FARE_RES * pDBChangeResult=(DB0_GP_CHANGE_PHONE_FARE_RES *)pData;
	CMD_GP0_ChangePhoneFare ChangePhoneRes = {0};
	if (pDBChangeResult->bSuccess == true)
	{
		ChangePhoneRes.bSuccess = true;
		ChangePhoneRes.nCurrentMobileCharge = pDBChangeResult->nCurrentMobileCharge;
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GPO_MOBILE_CHARGE,&ChangePhoneRes,sizeof(CMD_GP0_ChangePhoneFare));
		CMD_CS_C_VIPUpgrade VIPUpgrade = {0};
		_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), "#ffffff��ϲ��#ff0000%s#ffffff���ɹ��Ķһ���#ff0000%dԪ����#ffffff��", pDBChangeResult->szNickName, pDBChangeResult->nChangeNum);
		VIPUpgrade.cbType = 20;
		if(m_pITCPSocketService)
		{
			m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_VIPUpgrade,&VIPUpgrade,sizeof(CMD_CS_C_VIPUpgrade));
		}
		return true;
	}
	else if(pDBChangeResult->bSuccess == false)
	{
		ChangePhoneRes.bSuccess = false;
		ChangePhoneRes.nCurrentMobileCharge = pDBChangeResult->nCurrentMobileCharge;
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GPO_MOBILE_CHARGE,&ChangePhoneRes,sizeof(CMD_GP0_ChangePhoneFare));
	}
	return true;
}

bool CAttemperEngineSink::OnDBGetSuperRankInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DB0_GP_SUPERRANK_INFO_RES)==0);
	if (wDataSize%sizeof(DB0_GP_SUPERRANK_INFO_RES)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DB0_GP_SUPERRANK_INFO_RES);
	DB0_GP_SUPERRANK_INFO_RES * pInfo=(DB0_GP_SUPERRANK_INFO_RES *)pData;
	m_SuperRankInfo.clear();
	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		//��������
		tagSuperRankInfo sRankInfo;
		ZeroMemory(&sRankInfo,sizeof(sRankInfo));

		//��������
		sRankInfo.nRank=(pInfo+i)->nRank;
		lstrcpyn(sRankInfo.szNickName,(pInfo+i)->szNickName,CountArray(sRankInfo.szNickName));
		sRankInfo.lCount=(pInfo+i)->lCount;
		m_SuperRankInfo.push_back(sRankInfo);
	}

	return true;
}

bool CAttemperEngineSink::OnTCPNetworkGetBindRewardNewGuid( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//Ͷ������
	ASSERT(wDataSize>=sizeof(CMD_GPR_BindRewardNewQuid));
	if (wDataSize<sizeof(CMD_GPR_BindRewardNewQuid))
	{
			return false;
	}

	CMD_GPR_BindRewardNewQuid * pRequest = (CMD_GPR_BindRewardNewQuid*)pData;

	//��������
	DBR_GP_GET_BINDREWARD sGetBindReward;
	ZeroMemory(&sGetBindReward,sizeof(sGetBindReward));

	sGetBindReward.dwUserID = pRequest->dwUserID;
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GETBINDREWARDNEWGUID,dwSocketID,&sGetBindReward,sizeof(sGetBindReward));

	return true;
}

bool CAttemperEngineSink::OnTCPNetworkSetSuperNewGuid( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//Ͷ������
	ASSERT(wDataSize>=sizeof(CMD_GPR_SetSuperNewQuid));
	if (wDataSize!=sizeof(CMD_GPR_SetSuperNewQuid))
	{
		return false;
	}

	CMD_GPR_SetSuperNewQuid * pRequest = (CMD_GPR_SetSuperNewQuid*)pData;

	//��������
	DBR_GP_Set_SuperNewGuid sSetSuperNewGuid;
	ZeroMemory(&sSetSuperNewGuid,sizeof(sSetSuperNewGuid));

	sSetSuperNewGuid.dwUserID = pRequest->dwUserID;
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_SETSUPERNEWGUID,dwSocketID,&sSetSuperNewGuid,sizeof(sSetSuperNewGuid));

	return true;
}

bool CAttemperEngineSink::ReadSignInRewardConfig()
{
	//��ȡ·��
	TCHAR szWorkDir[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDir,CountArray(szWorkDir));

	//����·��
	TCHAR szSignRewardIniFile[MAX_PATH]=TEXT("");
	_sntprintf(szSignRewardIniFile,CountArray(szSignRewardIniFile),TEXT("%s\\SignReward.ini"),szWorkDir);

	//��ȡ����
	CWHIniData IniDataSignReward;
	IniDataSignReward.SetIniFilePath(szSignRewardIniFile);
	TCHAR szSectionName[100];
	TCHAR szSectionNameVIP[100];
	TCHAR szValueName[100];

	for (int i = 0; i < 7; i++)
	{
		_sntprintf(szSectionName,100, TEXT("DayReward%d"), i+1);
		_sntprintf(szSectionNameVIP,100, TEXT("DayVIPReward%d"), i+1);
		for (int j = 0; j < 3; j++)
		{
			m_NormalUserRewardConfig[i].cbType=GetPrivateProfileInt(szSectionName,TEXT("TYPE"),0,szSignRewardIniFile);
			m_NormalUserRewardConfig[i].cbAttrib=GetPrivateProfileInt(szSectionName,TEXT("ATTRIBUTE"),0,szSignRewardIniFile);
			m_NormalUserRewardConfig[i].nCount=GetPrivateProfileInt(szSectionName,TEXT("COUNT"),0,szSignRewardIniFile);

			m_VIPUserRewardConfig[i].cbType=GetPrivateProfileInt(szSectionNameVIP,TEXT("TYPE"),0,szSignRewardIniFile);
			m_VIPUserRewardConfig[i].cbAttrib=GetPrivateProfileInt(szSectionNameVIP,TEXT("ATTRIBUTE"),0,szSignRewardIniFile);
			m_VIPUserRewardConfig[i].nCount=GetPrivateProfileInt(szSectionNameVIP,TEXT("COUNT"),0,szSignRewardIniFile);
		}
			printf (TEXT("normal value : %hhd, %hhd, %d\n"), m_NormalUserRewardConfig[i].cbType,
				m_NormalUserRewardConfig[i].cbAttrib,
				m_NormalUserRewardConfig[i].nCount);
			printf (TEXT("VIP value : %hhd, %hhd, %d\n"), m_VIPUserRewardConfig[i].cbType,
				m_VIPUserRewardConfig[i].cbAttrib,
				m_VIPUserRewardConfig[i].nCount);
	}

	return TRUE;
}

bool CAttemperEngineSink::OnDBGetNewMailRemindInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GP_NewMail_Remind_Res)==0);
	if (wDataSize%sizeof(DBO_GP_NewMail_Remind_Res)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DBO_GP_NewMail_Remind_Res);
	DBO_GP_NewMail_Remind_Res * pInfo=(DBO_GP_NewMail_Remind_Res *)pData;
	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		CMD_GPO_MailIDList MailIDListResult = {0};

		MailIDListResult.cbCount = 1;
		MailIDListResult.dwMailID[0] = (pInfo+i)->dwMailID;
		tagBindParameter *pBindParameter = m_pBindParameter;
		for (int j =0;j<m_pInitParameter->m_wMaxConnect;++j)
		{
			if((pBindParameter+j)==NULL)continue;
			if ((pBindParameter+j)->dwUserID == (pInfo+i)->dwUserID)
			{
				m_pITCPNetworkEngine->SendData((pBindParameter+j)->dwSocketID, MDM_GP_USER_SERVICE, SUB_GPO_QUERY_MAIL_ID_LIST, &MailIDListResult, sizeof(MailIDListResult));
			}
		}
		
	}

	return true;
}

bool CAttemperEngineSink::OnTCPNetworkSubMatchReward(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GPR_MatchGetMatchPrise));
	if (wDataSize!=sizeof(CMD_GPR_MatchGetMatchPrise)) return false;

	CMD_GPR_MatchGetMatchPrise * pGetMatchPrise = (CMD_GPR_MatchGetMatchPrise *)pData;
	DBR_GR_Match_Get_Prize stMatchGetPrize={0};
	stMatchGetPrize.dwUserID = pGetMatchPrise->dwUserID;
	stMatchGetPrize.nMatchID = pGetMatchPrise->nMatchID;
	lstrcpyn(stMatchGetPrize.szMatchData,pGetMatchPrise->szMatchData,sizeof(stMatchGetPrize.szMatchData));
	m_pIDataBaseEngine->PostDataBaseRequest(pGetMatchPrise->dwUserID,DBR_GR_MATCH_GET_PRIZE,&stMatchGetPrize,sizeof(DBR_GR_Match_Get_Prize));

	return true;
}

bool CAttemperEngineSink::OnDBGetMatchResultPrize(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DBO_GR_Match_Get_Prize)==0);
	if (wDataSize%sizeof(DBO_GR_Match_Get_Prize)!=0) return false;

	//��������
	DBO_GR_Match_Get_Prize * pDBMatchResult=(DBO_GR_Match_Get_Prize *)pData;
	CMD_GPO_MatchGetMatchPriseRes stMatchGetMatchPrizeRes = {0};
	stMatchGetMatchPrizeRes.bPriseStatus = pDBMatchResult->bPriseStatus;
	stMatchGetMatchPrizeRes.nPriseType = pDBMatchResult->nPriseType;
	stMatchGetMatchPrizeRes.nPriseCount = pDBMatchResult->nPriseCount;
	if (pDBMatchResult->bPriseStatus==true)
	{
		char szText[128];
		sprintf(szText,"��ȡ�ɹ������%d��ҡ�",pDBMatchResult->nPriseCount);
		lstrcpyn(stMatchGetMatchPrizeRes.szDescription,szText,sizeof(stMatchGetMatchPrizeRes.szDescription));
	}
	else
	{
		char szText[128];
		sprintf(szText,"�����Ѿ���ȡ",pDBMatchResult->nPriseCount);
		lstrcpyn(stMatchGetMatchPrizeRes.szDescription,szText,sizeof(stMatchGetMatchPrizeRes.szDescription));
	}
	//m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GR_MATCH_GET_MATCH_PRIZE_RES, &stMatchGetMatchPrizeRes, sizeof(CMD_GPO_MatchGetMatchPriseRes));
	return true;
}

