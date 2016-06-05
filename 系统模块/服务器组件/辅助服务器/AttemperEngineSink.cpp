#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"
#include "StringData.h"

//////////////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_LOAD_GAME_LIST			1									//�����б�
#define IDI_CONNECT_CORRESPOND		2									//������ʶ
#define IDI_SEND_SYSTEM_BROADCAST	3
#define IDI_FALSE_BROADCAST_VIP			4									//���VIP����
#define IDI_LOAD_SYSTEM_BROADCAST	5									//����ϵͳ����
#define IDI_PEARL					6
#define IDI_FALSE_BROADCAST_ITEM		7									//��ٽ�ȯ�һ�
#define IDI_FALSE_BROADCAST_PEARL		8									//������鹫��
#define IDI_GET_RANK_INFO		11									//��ȡ��������
#define IDI_SYSTEM				12
#define IDI_FALSE_BIG_FISH		13
#define IDI_FALSE_BROADCAST_ITEM2 14
#define IDI_FALSE_BROADCAST_BIGREWARD			15					//��ٴ���Ϣ

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CAttemperEngineSink::CAttemperEngineSink()
{
	//״̬����
	m_bNeekCorrespond=true;

	//״̬����
	m_pInitParameter=NULL;
	m_pBindParameter=NULL;

	//�������
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;

	return;
}

//��������
CAttemperEngineSink::~CAttemperEngineSink()
{
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
	m_bSendPearlInfo = false;
	m_pITimerEngine->SetTimer(IDI_PEARL,1000,TIMES_INFINITY,NULL);

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
	case IDI_LOAD_SYSTEM_BROADCAST:
		{
			//�����Ϣ����
			ClearSystemMessageData();

			//������Ϣ
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LOAD_SYSTEM_MESSAGE,0L,NULL,0L);
			return true;
		}
	case IDI_SEND_SYSTEM_BROADCAST:
		{
			//�����ж�
			if (m_SystemMessageList.GetCount() == 0)
			{
				return true;
			}

			//ʱЧ�ж�
			DWORD dwCurrTime = (DWORD)time(NULL);
			POSITION pos = m_SystemMessageList.GetHeadPosition();
			while (pos != NULL)
			{
				tagSystemMessage * pTagSystemMessage = m_SystemMessageList.GetNext(pos);
				if (pTagSystemMessage->dwLastTime + pTagSystemMessage->SystemMessage.dwTimeRate < dwCurrTime)
				{
					//��������
					pTagSystemMessage->dwLastTime = dwCurrTime;

					//����ṹ
					CMD_CS_C_VIPUpgrade VIPUpgrade = {0};
					VIPUpgrade.cbType = 10;
					strncpy(VIPUpgrade.szTrumpetContent, pTagSystemMessage->SystemMessage.szSystemMessage, sizeof(VIPUpgrade.szTrumpetContent));

					if (m_pITCPSocketService)
					{
						m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE, SUB_CS_C_SystemBroadcast, &VIPUpgrade, sizeof(CMD_CS_C_VIPUpgrade));
					}
				}
			}

			return true;
		}
	case IDI_FALSE_BROADCAST_VIP:
		{
			CMD_CS_C_VIPUpgrade VIPUpgrade = {0};

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

			VIPUpgrade.cbType = 30;
			_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), pUpgrade[rand() % 2], g_pszNickName[rand() % CountArray(g_pszNickName)]);

			if (m_pITCPSocketService)
			{
				m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_VIPUpgrade,&VIPUpgrade,sizeof(CMD_CS_C_VIPUpgrade));
			}

			return true;
		}
	case IDI_FALSE_BROADCAST_ITEM:
		{
			CMD_CS_C_VIPUpgrade VIPUpgrade = {0};

			static unsigned int nItemCount = 1;

			VIPUpgrade.cbType = 20;
			if (nItemCount % 4 == 0)
			{
				_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), "#ffffff��ϲ��#ff0000%s#ffffff������Ľ���۹��У��һ��ˡ�#ff0000%s#ffffff����", g_pszNickName[rand() % CountArray(g_pszNickName)], g_pszItemName[rand() % CountArray(g_pszItemName)]);
			}
			else
			{
				_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), "#ffffff��ϲ��#ff0000%s#ffffff������Ľ���۹��У��һ��ˡ�#ff0000%s#ffffff����", g_pszNickName[rand() % CountArray(g_pszNickName)], g_pszMobileMoney[rand() % CountArray(g_pszMobileMoney)]);
			}
			nItemCount++;

			if (m_pITCPSocketService)
			{
				m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_VIPUpgrade,&VIPUpgrade,sizeof(CMD_CS_C_VIPUpgrade));
			}

			return true;
		}
	case IDI_FALSE_BROADCAST_ITEM2:
		{
			//CMD_CS_C_VIPUpgrade VIPUpgrade = {0};

			//VIPUpgrade.cbType = 20;

			//_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), "#ffffff��ϲ��#ff0000%s#ffffff���ɹ��Ķһ���#ff0000%dԪ����#ffffff��", g_pszNickName[rand() % CountArray(g_pszNickName)], 10);

			//if (m_pITCPSocketService)
			//{
			//	m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_VIPUpgrade,&VIPUpgrade,sizeof(CMD_CS_C_VIPUpgrade));
			//}

			return true;
		}
	case IDI_FALSE_BROADCAST_PEARL:
		{
			CMD_CS_C_VIPUpgrade VIPUpgrade = {0};

			LPCTSTR pPearlType[] =
			{
				TEXT("��������"), TEXT("�м�����"), TEXT("�߼�����"),
			};

			LPCTSTR pRoomName[] =
			{
				TEXT("���ַ�"), TEXT("���ڷ�"), TEXT("ǧ�ڷ�"), TEXT("���ڷ�"),
			};

			VIPUpgrade.cbType = 40;
			int nRand = rand() % 100;
			int nTemp = 3;
			if (nRand < 10)
			{
				nTemp = 1;
			}
			else if (nRand >= 10 && nRand < 30)
			{
				nTemp = 2;
			}
			else
			{
				nTemp = 3;
			}
			_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), "#ffffff��ϲ��#ff0000%s#ffffff����#ff0000%s#ffffff���˵Ĳ���#ff0000%s#ffffffһ�š�", g_pszNickName[rand() % CountArray(g_pszNickName)], pRoomName[nTemp + 1], pPearlType[nTemp]);

			if (m_pITCPSocketService)
			{
				m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_VIPUpgrade,&VIPUpgrade,sizeof(CMD_CS_C_VIPUpgrade));
			}

			return true;
		}
	case IDI_PEARL:
		{
			SYSTEMTIME st;
			GetLocalTime(&st);
			int nCurrentSecond = st.wHour;
			nCurrentSecond *= 3600;
			nCurrentSecond += st.wMinute * 60 + st.wSecond;

			if (st.wHour == 0 && st.wMinute == 0)
			{
				if (!m_bSendPearlInfo)
				{
					m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_YESTERDAY_SCORE, 0, NULL, 0);
				}
			}
			else
			{
				m_bSendPearlInfo = false;
			}

			return true;
		}
	case IDI_GET_RANK_INFO:
		{
			SYSTEMTIME st;
			GetLocalTime(&st);
			int nType = 300000099;
			nType += st.wDay * 100 + st.wMonth * 10000 + (st.wYear%100) * 1000000;
			DBR_GP_RankInfo RankInfo = {0};
			RankInfo.nType = nType;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_RANK_INFO, 0, &RankInfo, sizeof(RankInfo));
			return true;			
		}
	case IDI_SYSTEM:
		{
			CMD_CS_C_VIPUpgrade VIPUpgrade = {0};

			static int nTextIndex = 0;

			LPCTSTR pText[] =
			{
				TEXT("24Сʱ�������ԣ��𱬿�����ǧ���ҡ����ȯ��iphone6 plus����ʵ�ｱƷѰ�Ҳ���󿧣��͵�������"),
				TEXT("ÿ���������ԣ����Ƚ��У�24Сʱ����ǧ���ҡ����齱ȯ�ڶ�����ڴ�������12��ǰͨ���ʼ����ţ�����ʱ�������������"),
				TEXT("��ע�����΢�Ź��ں� buyudaka ����ȡ������Ʒһ�ݡ�"),
				TEXT("�ͷ�24Сʱ����:����bug���ύ��������ɻ�ý�ҡ��ֽ�iphone6 plus�ȷ������"),
			};

			VIPUpgrade.cbType = 55;
			_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), "%s", pText[nTextIndex % CountArray(pText)]);
			nTextIndex++;

			if (m_pITCPSocketService)
			{
				m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_VIPUpgrade,&VIPUpgrade,sizeof(CMD_CS_C_VIPUpgrade));
			}

			return true;
		}
	case IDI_FALSE_BIG_FISH:
		{
			CMD_CS_C_VIPUpgrade VIPUpgrade = {0};

			LPCTSTR pRoomName[] =
			{
				TEXT("���ַ�"), TEXT("���ڷ�"), TEXT("ǧ�ڷ�"), TEXT("���ڷ�"),
			};

			VIPUpgrade.cbType = 40;
			if (rand() % 2 == 0)
			{
				LPCTSTR pFishName[] =
				{
					TEXT("������"), TEXT("���"), TEXT("���"),
				};

				int nMultiple[] =
				{
					330, 250, 300,
				};

				int nTemp = rand() % CountArray(pFishName);
				_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), "#ffffff��ϲ��#ff0000%s#ffffff����#ff0000%s#ffffff������#ff0000%s#ffffff�����#ff0000%d��#ffffff��ҡ�",
					g_pszNickName[rand() % CountArray(g_pszNickName)],pRoomName[2],pFishName[nTemp],nMultiple[nTemp]);
			}
			else
			{
				LPCTSTR pFishName[] =
				{
					TEXT("ʯ����"), TEXT("����"), TEXT("����"), TEXT("������"), TEXT("������"), TEXT("���"), TEXT("���"),
				};

				int nMultiple[] =
				{
					20, 40, 80, 100, 330, 250, 300,
				};

				int nTemp = rand() % CountArray(pFishName);
				_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), "#ffffff��ϲ��#ff0000%s#ffffff����#ff0000%s#ffffff������#ff0000%s#ffffff�����#ff0000%d��#ffffff��ҡ�",
					g_pszNickName[rand() % CountArray(g_pszNickName)],pRoomName[3],pFishName[nTemp],nMultiple[nTemp] * 10);
			}

			if (m_pITCPSocketService)
			{
				m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_VIPUpgrade,&VIPUpgrade,sizeof(CMD_CS_C_VIPUpgrade));
			}

			return true;
		}
	case IDI_FALSE_BROADCAST_BIGREWARD:
		{
			CMD_CS_C_VIPUpgrade VIPUpgrade = {0};

			WORD wFakeNameSum = sizeof(g_pszNickName)/sizeof(*g_pszNickName);
			WORD wFNNum = rand()%wFakeNameSum;
			WORD wRewardNum = rand()%131 + 30;  // 30 ~ 200 
			_sntprintf(VIPUpgrade.szTrumpetContent,CountArray(VIPUpgrade.szTrumpetContent),TEXT("#ffffff��ϲ��#ff0000%s#ffffff���ڳ���������Ӯ��#ff0000%d��#ffffff�󽱣�#L01����˴�Ӯ�󽱣�"),g_pszNickName[wFNNum],wRewardNum);

			VIPUpgrade.cbType = 35;

			if (m_pITCPSocketService)
			{
				m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_VIPUpgrade,&VIPUpgrade,sizeof(CMD_CS_C_VIPUpgrade));
			}

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

	return true;
}

//�ر��¼�
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//�����Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	ZeroMemory((m_pBindParameter+wBindIndex),sizeof(tagBindParameter));

	return false;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (Command.wMainCmdID)
	{
	case MDM_KN_COMMAND:
		{
			printf("SUB_KN_DETECT_MANUAL\n");
			if (Command.wSubCmdID == SUB_KN_DETECT_MANUAL)
			{
				m_pITCPNetworkEngine->SendData(dwSocketID, MDM_KN_COMMAND, SUB_KN_DETECT_MANUAL, NULL, 0);
			}
			return true;
		}
	}

	return false;
}

//���ݿ��¼�
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBO_GP_GAME_LIST_RESULT:		//���ؽ��
		{
			return OnDBPCGameListResult(dwContextID,pData,wDataSize);
		}
	case DBO_GR_LOAD_SYSTEM_MESSAGE:  //ϵͳ��Ϣ
		{
			return OnDBLoadSystemMessage(dwContextID,pData,wDataSize);
		}
	case DBO_GP_LOAD_YESTERDAY_SCORE:
		{
			if (wDataSize != sizeof(DBO_GP_YesterdayScore))
			{
				return true;
			}
			DBO_GP_YesterdayScore * pYesterdayScore = (DBO_GP_YesterdayScore *)pData;

			CMD_CS_C_PearlInfo PearlInfo = {0};

			PearlInfo.lYesterdayScore = pYesterdayScore->lYesterdayScore;

			m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE, SUB_CS_C_PEARL_INFO, &PearlInfo, sizeof(PearlInfo));
			m_bSendPearlInfo = true;

			return true;
		}
	case DBO_GP_RANK_INFO:
		{
			return OnDBGetRankInfo(dwContextID,pData,wDataSize);
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
		CTraceService::TraceString(TEXT("����ע����Ϸ����������..."),TraceLevel_Normal);

		//��������
		CMD_CS_C_RegisterPlaza RegisterPlaza;
		ZeroMemory(&RegisterPlaza,sizeof(RegisterPlaza));

		//���ñ���
		lstrcpyn(RegisterPlaza.szServerName,m_pInitParameter->m_szServerName,CountArray(RegisterPlaza.szServerName));
		lstrcpyn(RegisterPlaza.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(RegisterPlaza.szServerAddr));

		//��������
		m_pITCPSocketService->SendData(MDM_CS_REGISTER,SUB_CS_C_REGISTER_PLAZA,&RegisterPlaza,sizeof(RegisterPlaza));

		m_pITimerEngine->SetTimer(IDI_LOAD_SYSTEM_BROADCAST, 300 * 1000, -1, 0);
		m_pITimerEngine->SetTimer(IDI_SEND_SYSTEM_BROADCAST, 1 * 1000, -1, 0);

		m_pITimerEngine->SetTimer(IDI_FALSE_BROADCAST_VIP, 300 * 1000, -1, 0);
		m_pITimerEngine->SetTimer(IDI_FALSE_BROADCAST_ITEM, 300 * 1000, -1, 0);
		//m_pITimerEngine->SetTimer(IDI_FALSE_BROADCAST_PEARL, 150 * 1000, -1, 0);
		m_pITimerEngine->SetTimer(IDI_GET_RANK_INFO, 30 * 1000 , -1, 0);
		m_pITimerEngine->SetTimer(IDI_SYSTEM, 60 * 1000, -1, 0);
		m_pITimerEngine->SetTimer(IDI_FALSE_BIG_FISH, 60 * 1000, -1, 0);
		//m_pITimerEngine->SetTimer(IDI_FALSE_BROADCAST_ITEM2, 180 * 1000, -1, 0);
		m_pITimerEngine->SetTimer(IDI_FALSE_BROADCAST_BIGREWARD, 50 * 1000, -1, 0);
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
				printf("MDM_CS_MANAGER_SERVICE\n");
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

//ϵͳ��Ϣ
bool CAttemperEngineSink::OnDBLoadSystemMessage(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	if (wDataSize != sizeof(DBR_GR_SystemMessage))
	{
		return false;
	}

	//��ȡ����
	DBR_GR_SystemMessage * pSystemMessage = (DBR_GR_SystemMessage *)pData;
	if (pSystemMessage == NULL)
	{
		return false;
	}

	printf("OnDBSystemMessage %s\n", pSystemMessage->szSystemMessage);

	//�ظ��ж�
	POSITION pos = m_SystemMessageList.GetHeadPosition();
	while (pos != NULL)
	{
		tagSystemMessage * pTagSystemMessage = m_SystemMessageList.GetNext(pos);
		if (pTagSystemMessage->SystemMessage.cbMessageID == pSystemMessage->cbMessageID)
		{
			//��������
			CopyMemory(&pTagSystemMessage->SystemMessage, pSystemMessage, sizeof(DBR_GR_SystemMessage));
			return true;
		}
	}

	tagSystemMessage * pSendMessage = new tagSystemMessage;
	ZeroMemory(pSendMessage, sizeof(tagSystemMessage));

	//���ñ���
	CopyMemory(&pSendMessage->SystemMessage, pSystemMessage, sizeof(DBR_GR_SystemMessage));

	//��¼��Ϣ
	m_SystemMessageList.AddTail(pSendMessage);

	return true;
}

//�����Ϣ����
void CAttemperEngineSink::ClearSystemMessageData()
{
	while(m_SystemMessageList.GetCount() > 0)
	{
		tagSystemMessage * pRqHead = m_SystemMessageList.RemoveHead();
		if(pRqHead)
		{
			delete [] ((BYTE*)pRqHead);
		}
	}
}

bool CAttemperEngineSink::OnDBGetRankInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize%sizeof(DB0_GP_RankInfo)==0);
	if (wDataSize%sizeof(DB0_GP_RankInfo)!=0) return false;

	//��������
	WORD wItemCount=wDataSize/sizeof(DB0_GP_RankInfo);
	DB0_GP_RankInfo * pRankInfo=(DB0_GP_RankInfo *)pData;

	m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE, SUB_CS_C_MatchScoreBegin, NULL, 0);

	//��������
	for (WORD i=0;i<wItemCount;i++)
	{
		CMD_CS_C_MatchScore MatchScore = {0};
		MatchScore.dwUserID = (pRankInfo+i)->nUserID;
		MatchScore.lMatchScore = (pRankInfo+i)->lMatchScore;
		m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE, SUB_CS_C_MatchScore, &MatchScore, sizeof(MatchScore));
	}

	m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE, SUB_CS_C_MatchScoreFinish, NULL, 0);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
