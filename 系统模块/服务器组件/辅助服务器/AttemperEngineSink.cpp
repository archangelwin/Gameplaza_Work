#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"
#include "StringData.h"

//////////////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_LOAD_GAME_LIST			1									//加载列表
#define IDI_CONNECT_CORRESPOND		2									//重连标识
#define IDI_SEND_SYSTEM_BROADCAST	3
#define IDI_FALSE_BROADCAST_VIP			4									//虚假VIP升级
#define IDI_LOAD_SYSTEM_BROADCAST	5									//加载系统公告
#define IDI_PEARL					6
#define IDI_FALSE_BROADCAST_ITEM		7									//虚假奖券兑换
#define IDI_FALSE_BROADCAST_PEARL		8									//虚假珍珠公告
#define IDI_GET_RANK_INFO		11									//获取比赛排名
#define IDI_SYSTEM				12
#define IDI_FALSE_BIG_FISH		13
#define IDI_FALSE_BROADCAST_ITEM2 14
#define IDI_FALSE_BROADCAST_BIGREWARD			15					//虚假大奖消息

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CAttemperEngineSink::CAttemperEngineSink()
{
	//状态变量
	m_bNeekCorrespond=true;

	//状态变量
	m_pInitParameter=NULL;
	m_pBindParameter=NULL;

	//组件变量
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;

	return;
}

//析构函数
CAttemperEngineSink::~CAttemperEngineSink()
{
}

//接口查询
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//启动事件
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//绑定参数
	m_pBindParameter=new tagBindParameter[m_pInitParameter->m_wMaxConnect];
	ZeroMemory(m_pBindParameter,sizeof(tagBindParameter)*m_pInitParameter->m_wMaxConnect);
	m_bSendPearlInfo = false;
	m_pITimerEngine->SetTimer(IDI_PEARL,1000,TIMES_INFINITY,NULL);

	return true;
}

//停止事件
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)
{
	//状态变量
	m_bNeekCorrespond=true;

	//组件变量
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;

	//删除数据
	SafeDeleteArray(m_pBindParameter);

	//列表组件
	m_ServerListManager.ResetServerList();

	return true;
}

//控制事件
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	switch (wIdentifier)
	{
	case CT_LOAD_DB_GAME_LIST:		//加载列表
		{
			//加载列表
			m_ServerListManager.DisuseKernelItem();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_GAME_LIST,0,NULL,0);

			return true;
		}
	case CT_CONNECT_CORRESPOND:		//连接协调
		{
			//发起连接
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//构造提示
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("正在连接协调服务器 [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//提示消息
			CTraceService::TraceString(szString,TraceLevel_Normal);

			return true;
		}
	}

	return false;
}

//调度事件
bool CAttemperEngineSink::OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//时间事件
bool CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)
{
	switch (dwTimerID)
	{
	case IDI_LOAD_GAME_LIST:		//加载列表
		{
			//加载列表
			m_ServerListManager.DisuseKernelItem();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOAD_GAME_LIST,0,NULL,0);

			return true;
		}
	case IDI_CONNECT_CORRESPOND:	//连接协调
		{
			//发起连接
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//构造提示
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("正在连接协调服务器 [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//提示消息
			CTraceService::TraceString(szString,TraceLevel_Normal);

			return true;
		}
	case IDI_LOAD_SYSTEM_BROADCAST:
		{
			//清除消息数据
			ClearSystemMessageData();

			//加载消息
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LOAD_SYSTEM_MESSAGE,0L,NULL,0L);
			return true;
		}
	case IDI_SEND_SYSTEM_BROADCAST:
		{
			//数量判断
			if (m_SystemMessageList.GetCount() == 0)
			{
				return true;
			}

			//时效判断
			DWORD dwCurrTime = (DWORD)time(NULL);
			POSITION pos = m_SystemMessageList.GetHeadPosition();
			while (pos != NULL)
			{
				tagSystemMessage * pTagSystemMessage = m_SystemMessageList.GetNext(pos);
				if (pTagSystemMessage->dwLastTime + pTagSystemMessage->SystemMessage.dwTimeRate < dwCurrTime)
				{
					//更新数据
					pTagSystemMessage->dwLastTime = dwCurrTime;

					//构造结构
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
				"#ffffff【#ff0000%s#ffffff】在众人艳羡的目光中，成为了【#ff0000VIP1#ffffff】。#L01查看VIP1尊贵特权",
				"#ffffff恭喜【#ff0000%s#ffffff】成功升级【#ff0000VIP2#ffffff】，真是春风得意。#L01查看VIP2尊贵特权",
				"#ffffff土豪【#ff0000%s#ffffff】说：成为【#ff0000VIP3#ffffff】主要靠气质。#L01查看VIP3尊贵特权",
				"#ffffff【#ff0000%s#ffffff】说：成为【#ff0000VIP4#ffffff】的征途是星辰大海。#L01查看VIP4尊贵特权",
				"#ffffff【#ff0000%s#ffffff】：我是【#ff0000VIP5！VIP5！VIP5！#ffffff】重要的事情说三遍。#L01查看VIP5尊贵特权",
				"#ffffff【#ff0000%s#ffffff】轻松的升级为【#ff0000VIP6#ffffff】，有钱、任性。#L01查看VIP6尊贵特权",
				"#ffffff酷！【#ff0000%s#ffffff】成为了二人之下，六人之上的【#ff0000VIP7#ffffff】。#L01查看VIP7尊贵特权",
				"#ffffff成为【#ff0000VIP8#ffffff】的【#ff0000%s#ffffff】说，这次升级我给自己打满分。#L01查看VIP8尊贵特权",
				"#ffffff活久见！【#ff0000%s#ffffff】登顶至尊【#ff0000VIP9#ffffff】，仰天长叹：还有谁？#L01查看VIP9尊贵特权",
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
				_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), "#ffffff恭喜【#ff0000%s#ffffff】在羡慕的眼光中，兑换了【#ff0000%s#ffffff】。", g_pszNickName[rand() % CountArray(g_pszNickName)], g_pszItemName[rand() % CountArray(g_pszItemName)]);
			}
			else
			{
				_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), "#ffffff恭喜【#ff0000%s#ffffff】在羡慕的眼光中，兑换了【#ff0000%s#ffffff】。", g_pszNickName[rand() % CountArray(g_pszNickName)], g_pszMobileMoney[rand() % CountArray(g_pszMobileMoney)]);
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

			//_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), "#ffffff恭喜【#ff0000%s#ffffff】成功的兑换了#ff0000%d元话费#ffffff！", g_pszNickName[rand() % CountArray(g_pszNickName)], 10);

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
				TEXT("初级珍珠"), TEXT("中级珍珠"), TEXT("高级珍珠"),
			};

			LPCTSTR pRoomName[] =
			{
				TEXT("新手房"), TEXT("百炮房"), TEXT("千炮房"), TEXT("万炮房"),
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
			_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), "#ffffff恭喜【#ff0000%s#ffffff】在#ff0000%s#ffffff幸运的捕获#ff0000%s#ffffff一颗。", g_pszNickName[rand() % CountArray(g_pszNickName)], pRoomName[nTemp + 1], pPearlType[nTemp]);

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
				TEXT("24小时渔王争霸，火爆开赛。千万金币、丰厚奖券加iphone6 plus海量实物奖品寻找捕鱼大咖，就等你来。"),
				TEXT("每日渔王争霸，火热进行（24小时），千万金币、珍珠奖券众多豪礼于次日中午12点前通过邮件发放，请随时留意比赛排名。"),
				TEXT("关注捕鱼大咖微信公众号 buyudaka 可领取精美礼品一份。"),
				TEXT("客服24小时在线:反馈bug、提交意见被采纳获得金币、现金、iphone6 plus等丰厚奖励。"),
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
				TEXT("新手房"), TEXT("百炮房"), TEXT("千炮房"), TEXT("万炮房"),
			};

			VIPUpgrade.cbType = 40;
			if (rand() % 2 == 0)
			{
				LPCTSTR pFishName[] =
				{
					TEXT("美人鱼"), TEXT("绿蟾"), TEXT("金蟾"),
				};

				int nMultiple[] =
				{
					330, 250, 300,
				};

				int nTemp = rand() % CountArray(pFishName);
				_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), "#ffffff恭喜【#ff0000%s#ffffff】在#ff0000%s#ffffff击中了#ff0000%s#ffffff获得了#ff0000%d万#ffffff金币。",
					g_pszNickName[rand() % CountArray(g_pszNickName)],pRoomName[2],pFishName[nTemp],nMultiple[nTemp]);
			}
			else
			{
				LPCTSTR pFishName[] =
				{
					TEXT("石斑鱼"), TEXT("金鲨"), TEXT("电鳗"), TEXT("金龙鱼"), TEXT("美人鱼"), TEXT("绿蟾"), TEXT("金蟾"),
				};

				int nMultiple[] =
				{
					20, 40, 80, 100, 330, 250, 300,
				};

				int nTemp = rand() % CountArray(pFishName);
				_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), "#ffffff恭喜【#ff0000%s#ffffff】在#ff0000%s#ffffff击中了#ff0000%s#ffffff获得了#ff0000%d万#ffffff金币。",
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
			_sntprintf(VIPUpgrade.szTrumpetContent,CountArray(VIPUpgrade.szTrumpetContent),TEXT("#ffffff恭喜【#ff0000%s#ffffff】在超级海盗中赢得#ff0000%d万#ffffff大奖，#L01点击此处赢大奖！"),g_pszNickName[wFNNum],wRewardNum);

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

//应答事件
bool CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)
{
	//获取索引
	ASSERT(LOWORD(dwSocketID)<m_pInitParameter->m_wMaxConnect);
	if (LOWORD(dwSocketID)>=m_pInitParameter->m_wMaxConnect) return false;

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//设置变量
	pBindParameter->dwSocketID=dwSocketID;
	pBindParameter->dwClientAddr=dwClientAddr;
	pBindParameter->dwActiveTime=(DWORD)time(NULL);

	return true;
}

//关闭事件
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//清除信息
	WORD wBindIndex=LOWORD(dwSocketID);
	ZeroMemory((m_pBindParameter+wBindIndex),sizeof(tagBindParameter));

	return false;
}

//读取事件
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

//数据库事件
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBO_GP_GAME_LIST_RESULT:		//加载结果
		{
			return OnDBPCGameListResult(dwContextID,pData,wDataSize);
		}
	case DBO_GR_LOAD_SYSTEM_MESSAGE:  //系统消息
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

//关闭事件
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	//协调连接
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//重连判断
		if (m_bNeekCorrespond==true)
		{
			//构造提示
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("与协调服务器的连接关闭了，%ld 秒后将重新连接"),m_pInitParameter->m_wConnectTime);

			//提示消息
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//设置时间
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return true;
		}
	}

	return false;
}

//连接事件
bool CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	//协调连接
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//错误判断
		if (nErrorCode!=0)
		{
			//构造提示
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("协调服务器连接失败 [ %ld ]，%ld 秒后将重新连接"),
				nErrorCode,m_pInitParameter->m_wConnectTime);

			//提示消息
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//设置时间
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return false;
		}

		//提示消息
		CTraceService::TraceString(TEXT("正在注册游戏辅助服务器..."),TraceLevel_Normal);

		//变量定义
		CMD_CS_C_RegisterPlaza RegisterPlaza;
		ZeroMemory(&RegisterPlaza,sizeof(RegisterPlaza));

		//设置变量
		lstrcpyn(RegisterPlaza.szServerName,m_pInitParameter->m_szServerName,CountArray(RegisterPlaza.szServerName));
		lstrcpyn(RegisterPlaza.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(RegisterPlaza.szServerAddr));

		//发送数据
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

//读取事件
bool CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//协调连接
	if (wServiceID==NETWORK_CORRESPOND)
	{
		switch (Command.wMainCmdID)
		{
		case MDM_CS_REGISTER:		//注册服务
			{
				return OnTCPSocketMainRegister(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_SERVICE_INFO:	//服务信息
			{
				return OnTCPSocketMainServiceInfo(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_REMOTE_SERVICE:	//远程服务
			{
				return OnTCPSocketMainRemoteService(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_MANAGER_SERVICE: //管理服务
			{
				printf("MDM_CS_MANAGER_SERVICE\n");
				return OnTCPSocketMainManagerService(Command.wSubCmdID,pData,wDataSize);
			}
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//注册事件
bool CAttemperEngineSink::OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_REGISTER_FAILURE:		//注册失败
		{
			//变量定义
			CMD_CS_S_RegisterFailure * pRegisterFailure=(CMD_CS_S_RegisterFailure *)pData;

			//效验参数
			ASSERT(wDataSize>=(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString)));
			if (wDataSize<(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString))) return false;

			//关闭处理
			m_bNeekCorrespond=false;
			m_pITCPSocketService->CloseSocket();

			//显示消息
			LPCTSTR pszDescribeString=pRegisterFailure->szDescribeString;
			if (lstrlen(pszDescribeString)>0) CTraceService::TraceString(pszDescribeString,TraceLevel_Exception);

			//事件通知
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_FAILURE;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//列表事件
bool CAttemperEngineSink::OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SERVER_INFO:		//房间信息
		{
			//废弃列表
			m_ServerListManager.DisuseServerItem();

			return true;
		}
	case SUB_CS_S_SERVER_ONLINE:	//房间人数
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerOnLine));
			if (wDataSize!=sizeof(CMD_CS_S_ServerOnLine)) return false;

			//变量定义
			CMD_CS_S_ServerOnLine * pServerOnLine=(CMD_CS_S_ServerOnLine *)pData;

			//查找房间
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerOnLine->wServerID);
			if(pGameServerItem == NULL) return true;

			//设置人数
			DWORD dwOldOnlineCount=0;
			dwOldOnlineCount = pGameServerItem->m_GameServer.dwOnLineCount;
			pGameServerItem->m_GameServer.dwOnLineCount=pServerOnLine->dwOnLineCount;

			//目录人数
			CGameKindItem * pGameKindItem=m_ServerListManager.SearchGameKind(pGameServerItem->m_GameServer.wKindID);
			if (pGameKindItem!=NULL)
			{
				tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
				pGameKindItem->m_GameKind.dwOnLineCount -= dwOldOnlineCount;
				pGameKindItem->m_GameKind.dwOnLineCount += pGameServer->dwOnLineCount;
			}

			return true;
		}
	case SUB_CS_S_SERVER_INSERT:	//房间插入
		{
			//效验参数
			ASSERT(wDataSize%sizeof(tagGameServer)==0);
			if (wDataSize%sizeof(tagGameServer)!=0) return false;

			//变量定义
			WORD wItemCount=wDataSize/sizeof(tagGameServer);
			tagGameServer * pGameServer=(tagGameServer *)pData;

			//更新数据
			for (WORD i=0;i<wItemCount;i++)
			{
				m_ServerListManager.InsertGameServer(pGameServer++);
			}

			return true;
		}
	case SUB_CS_S_SERVER_MODIFY:	//房间修改
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerModify));
			if (wDataSize!=sizeof(CMD_CS_S_ServerModify)) return false;

			//变量定义
			CMD_CS_S_ServerModify * pServerModify=(CMD_CS_S_ServerModify *)pData;

			//查找房间
			ASSERT(m_ServerListManager.SearchGameServer(pServerModify->wServerID));
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerModify->wServerID);

			//设置房间
			if (pGameServerItem!=NULL)
			{
				//设置人数
				DWORD dwOldOnlineCount=0, dwOldFullCount=0;
				dwOldOnlineCount = pGameServerItem->m_GameServer.dwOnLineCount;
				dwOldFullCount   = pGameServerItem->m_GameServer.dwFullCount;

				//修改房间信息
				pGameServerItem->m_GameServer.wKindID=pServerModify->wKindID;
				pGameServerItem->m_GameServer.wNodeID=pServerModify->wNodeID;
				pGameServerItem->m_GameServer.wSortID=pServerModify->wSortID;
				pGameServerItem->m_GameServer.wServerPort=pServerModify->wServerPort;
				pGameServerItem->m_GameServer.dwOnLineCount=pServerModify->dwOnLineCount;
				pGameServerItem->m_GameServer.dwFullCount=pServerModify->dwFullCount;
				lstrcpyn(pGameServerItem->m_GameServer.szServerName,pServerModify->szServerName,CountArray(pGameServerItem->m_GameServer.szServerName));
				lstrcpyn(pGameServerItem->m_GameServer.szServerAddr,pServerModify->szServerAddr,CountArray(pGameServerItem->m_GameServer.szServerAddr));

				//目录人数
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
	case SUB_CS_S_SERVER_REMOVE:	//房间删除
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerRemove));
			if (wDataSize!=sizeof(CMD_CS_S_ServerRemove)) return false;

			//变量定义
			CMD_CS_S_ServerRemove * pServerRemove=(CMD_CS_S_ServerRemove *)pData;

			//变量定义
			m_ServerListManager.DeleteGameServer(pServerRemove->wServerID);

			return true;
		}
	case SUB_CS_S_SERVER_FINISH:	//房间完成
		{
			//清理列表
			m_ServerListManager.CleanServerItem();

			//事件处理
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_SUCCESS;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//远程服务
bool CAttemperEngineSink::OnTCPSocketMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SEARCH_CORRESPOND:	//协调查找
		{
			//变量定义
			CMD_CS_S_SearchCorrespond * pSearchCorrespond=(CMD_CS_S_SearchCorrespond *)pData;

			//效验参数
			ASSERT(wDataSize<=sizeof(CMD_CS_S_SearchCorrespond));
			ASSERT(wDataSize>=(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)));
			ASSERT(wDataSize==(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0])));

			//效验参数
			if (wDataSize>sizeof(CMD_CS_S_SearchCorrespond)) return false;
			if (wDataSize<(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo))) return false;
			if (wDataSize!=(sizeof(CMD_CS_S_SearchCorrespond)-sizeof(pSearchCorrespond->UserRemoteInfo)+pSearchCorrespond->wUserCount*sizeof(pSearchCorrespond->UserRemoteInfo[0]))) return false;

			//判断在线
			ASSERT(LOWORD(pSearchCorrespond->dwSocketID)<m_pInitParameter->m_wMaxConnect);
			if ((m_pBindParameter+LOWORD(pSearchCorrespond->dwSocketID))->dwSocketID!=pSearchCorrespond->dwSocketID) return true;

			//变量定义
			CMD_GP_S_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//设置变量
			for (WORD i=0;i<pSearchCorrespond->wUserCount;i++)
			{
				//数据效验
				ASSERT(SearchCorrespond.wUserCount<CountArray(SearchCorrespond.UserRemoteInfo));
				if (SearchCorrespond.wUserCount>=CountArray(SearchCorrespond.UserRemoteInfo)) break;

				//设置变量
				WORD wIndex=SearchCorrespond.wUserCount++;
				CopyMemory(&SearchCorrespond.UserRemoteInfo[wIndex],&pSearchCorrespond->UserRemoteInfo[i],sizeof(SearchCorrespond.UserRemoteInfo[wIndex]));
			}

			//发送数据
			WORD wHeadSize=sizeof(SearchCorrespond)-sizeof(SearchCorrespond.UserRemoteInfo);
			WORD wItemSize=sizeof(SearchCorrespond.UserRemoteInfo[0])*SearchCorrespond.wUserCount;
			m_pITCPNetworkEngine->SendData(pSearchCorrespond->dwSocketID,MDM_GP_REMOTE_SERVICE,SUB_GP_S_SEARCH_CORRESPOND,&SearchCorrespond,wHeadSize+wItemSize);

			return true;
		}
	}

	return true;
}

//管理服务
bool CAttemperEngineSink::OnTCPSocketMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//发送请求
bool CAttemperEngineSink::SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	//发送数据
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	pServiceUnits->PostControlRequest(wRequestID,pData,wDataSize);

	return true;
}

//游戏列表
bool CAttemperEngineSink::OnDBPCGameListResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBO_GP_GameListResult));
	if (wDataSize!=sizeof(DBO_GP_GameListResult)) return false;

	//变量定义
	DBO_GP_GameListResult * pGameListResult=(DBO_GP_GameListResult *)pData;

	//消息处理
	if (pGameListResult->cbSuccess==TRUE)
	{
		//清理列表
		m_ServerListManager.CleanKernelItem();

		//事件通知
		CP_ControlResult ControlResult;
		ControlResult.cbSuccess=ER_SUCCESS;
		SendUIControlPacket(UI_LOAD_DB_LIST_RESULT,&ControlResult,sizeof(ControlResult));

		//设置时间
		ASSERT(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_LOAD_GAME_LIST,m_pInitParameter->m_wLoadListTime*1000L,1,0);
	}
	else
	{
		//构造提示
		TCHAR szDescribe[128]=TEXT("");
		_sntprintf(szDescribe,CountArray(szDescribe),TEXT("服务器列表加载失败，%ld 秒后将重新加载"),m_pInitParameter->m_wReLoadListTime);

		//提示消息
		CTraceService::TraceString(szDescribe,TraceLevel_Warning);

		//设置时间
		ASSERT(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_LOAD_GAME_LIST,m_pInitParameter->m_wReLoadListTime*1000L,1,0);
	}

	return true;
}

//系统消息
bool CAttemperEngineSink::OnDBLoadSystemMessage(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	if (wDataSize != sizeof(DBR_GR_SystemMessage))
	{
		return false;
	}

	//提取数据
	DBR_GR_SystemMessage * pSystemMessage = (DBR_GR_SystemMessage *)pData;
	if (pSystemMessage == NULL)
	{
		return false;
	}

	printf("OnDBSystemMessage %s\n", pSystemMessage->szSystemMessage);

	//重复判断
	POSITION pos = m_SystemMessageList.GetHeadPosition();
	while (pos != NULL)
	{
		tagSystemMessage * pTagSystemMessage = m_SystemMessageList.GetNext(pos);
		if (pTagSystemMessage->SystemMessage.cbMessageID == pSystemMessage->cbMessageID)
		{
			//更新数据
			CopyMemory(&pTagSystemMessage->SystemMessage, pSystemMessage, sizeof(DBR_GR_SystemMessage));
			return true;
		}
	}

	tagSystemMessage * pSendMessage = new tagSystemMessage;
	ZeroMemory(pSendMessage, sizeof(tagSystemMessage));

	//设置变量
	CopyMemory(&pSendMessage->SystemMessage, pSystemMessage, sizeof(DBR_GR_SystemMessage));

	//记录消息
	m_SystemMessageList.AddTail(pSendMessage);

	return true;
}

//清除消息数据
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
	//效验参数
	ASSERT(wDataSize%sizeof(DB0_GP_RankInfo)==0);
	if (wDataSize%sizeof(DB0_GP_RankInfo)!=0) return false;

	//变量定义
	WORD wItemCount=wDataSize/sizeof(DB0_GP_RankInfo);
	DB0_GP_RankInfo * pRankInfo=(DB0_GP_RankInfo *)pData;

	m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE, SUB_CS_C_MatchScoreBegin, NULL, 0);

	//更新数据
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
