#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"
#include "SensitiveWords.h"
#include <vector>
#include "CVerification.h"
//////////////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_LOAD_GAME_LIST			1									//加载列表
#define IDI_CONNECT_CORRESPOND		2									//重连标识
#define IDI_COLLECT_ONLINE_INFO		3									//统计在线
#define IDI_MAIL_CHECK				4
#define IDI_WEALTH_BAG_TIME			5
#define IDI_WEALTH_BAG_BEGIN		6
#define IDI_WEALTH_BAG_END			7
#define IDI_WEALTH_BAG_RESULT		8
#define IDI_WEALTH_BAG_WILL			9									//福袋小游戏即将开始
#define IDI_CHECK_TIMEOUT_MBNUM		10									//检查验证码超时手机号, 未超时手机号不会再次发送验证码 
#define IDI_GET_COMPRANK_INFO		11									//获取比赛排名
#define IDI_SEND_LOGIN_QUEUEING		12									//发送排队信息
#define IDI_GET_SUPERRANK_INFO		13									//获取超级海盗排名
#define TIME_WEALTH_BAG_BEFORE		60									//还有多长时间才开始
#define IDI_GET_NEW_MAIL_REMIND		61									//获取新邮件提醒
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

//构造函数
CAttemperEngineSink::CAttemperEngineSink()
{
	//状态变量
	m_bNeekCorrespond=true;
	m_bShowServerStatus=false;

	//状态变量
	m_pInitParameter=NULL;
	m_pBindParameter=NULL;

	//组件变量
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketService=NULL;

	//视频配置
	m_wAVServerPort=0;
	m_dwAVServerAddr=0;

	//短信验证码
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

//析构函数
CAttemperEngineSink::~CAttemperEngineSink()
{
	//逆初始化 sms sdk
	unInit();
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

	//设置时间
	ASSERT(m_pITimerEngine!=NULL);
	m_pITimerEngine->SetTimer(IDI_COLLECT_ONLINE_INFO,m_pInitParameter->m_wCollectTime*1000L,TIMES_INFINITY,0);

	//获取目录
	TCHAR szPath[MAX_PATH]=TEXT("");
	CString szFileName;
	GetModuleFileName(AfxGetInstanceHandle(),szPath,sizeof(szPath));
	szFileName=szPath;
	int nIndex = szFileName.ReverseFind(TEXT('\\'));
	szFileName = szFileName.Left(nIndex);
	szFileName += TEXT("\\PlazaOptionConfig.ini");

	//读取配置
	m_bShowServerStatus = (GetPrivateProfileInt(TEXT("ServerStatus"),TEXT("ShowServerStatus"),0,szFileName) != 0);

	//获取目录
	TCHAR szServerAddr[LEN_SERVER]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);

	//读取配置
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

	////@@@福袋小游戏
	//m_pITimerEngine->SetTimer(IDI_WEALTH_BAG_WILL, 20 * 1000, -1, 0);
	m_bWealthBagRunning = false;

	//手机验证码
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

	//读取签到奖励配置
	//ReadSignInRewardConfig();
	m_pITimerEngine->SetTimer(IDI_GET_NEW_MAIL_REMIND,5000L,TIMES_INFINITY,0);
	m_pITimerEngine->SetTimer(IDI_QUERY_FAKE_SERVER_INFO,m_wSendFakeInfoInterval,TIMES_INFINITY,0);
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
	//停止线程
	m_mMBVerifInfo.clear();
	m_mRegisteredMbNum.clear();
	CVerification * pVerification = CVerification::GetInstance();
	if (pVerification!= NULL)
	{
		pVerification->EndThread();
	}
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
	case IDI_COLLECT_ONLINE_INFO:	//统计在线
		{
			//变量定义
			DBR_GP_OnLineCountInfo OnLineCountInfo;
			ZeroMemory(&OnLineCountInfo,sizeof(OnLineCountInfo));

			//获取总数
			OnLineCountInfo.dwOnLineCountSum=m_ServerListManager.CollectOnlineInfo();

			//获取类型
			POSITION KindPosition=NULL;
			do
			{
				//获取类型
				CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

				//设置变量
				if (pGameKindItem!=NULL)
				{
					WORD wKindIndex=OnLineCountInfo.wKindCount++;
					OnLineCountInfo.OnLineCountKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;
					OnLineCountInfo.OnLineCountKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;
				}

				//溢出判断
				if (OnLineCountInfo.wKindCount>=CountArray(OnLineCountInfo.OnLineCountKind))
				{
					ASSERT(FALSE);
					break;
				}

			} while (KindPosition!=NULL);

			//发送请求
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

			// 遍历发送时间大于10 min 删除 验证码信息
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
	pBindParameter->bIsLtryInfoSend=false;

	return true;
}

//关闭事件
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//清除信息
	WORD wBindIndex=LOWORD(dwSocketID);
	ZeroMemory((m_pBindParameter+wBindIndex),sizeof(tagBindParameter));
	OnCancelLoginQueueing(dwSocketID);
	return false;
}

//读取事件
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//printf("wMainCmdID = %hd wSubCmdID = %hd\n", Command.wMainCmdID, Command.wSubCmdID);
	switch (Command.wMainCmdID)
	{
	case MDM_GP_LOGON:			//登录命令
		{
			return OnTCPNetworkMainPCLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_SERVER_LIST:	//列表命令
		{
			return OnTCPNetworkMainPCServerList(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_USER_SERVICE:	//服务命令
		{
			return OnTCPNetworkMainPCUserService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GP_REMOTE_SERVICE:	//远程服务
		{
			return OnTCPNetworkMainPCRemoteService(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_MB_LOGON:			//登录命令
		{
			return OnTCPNetworkMainMBLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_MB_SERVER_LIST:	//列表命令
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
				//低于一定等级的不公告
				if (pVIPUpgrade->cbVIPLevel < 1)
				{
					return true;
				}

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

				if (pVIPUpgrade->cbVIPLevel > CountArray(pUpgrade))
				{
					return true;
				}

				//构造结构
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
				//低于一定的奖券不公告
				if (pChangeItem->dwLotteryCount < 30000)
				{
					return true;
				}

				CMD_CS_C_VIPUpgrade VIPUpgrade = {0};
				_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), "#ffffff恭喜【#ff0000%s#ffffff】在羡慕的眼光中，兑换了【#ff0000%s#ffffff】。", pChangeItem->szNickName, pChangeItem->szItemName);
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

//数据库事件
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBO_GP_LOGON_SUCCESS:			//登录成功
		{
			return OnDBPCLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_LOGON_FAILURE:			//登录失败
		{
			return OnDBPCLogonFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GUEST_LOGON_SUCCESS:	//游客登录成功
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
	case DBO_GP_USER_FACE_INFO:			//用户头像
		{
			return OnDBPCUserFaceInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INDIVIDUAL:		//用户信息
		{
			return OnDBPCUserIndividual(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_INFO:		//银行资料
		{
			return OnDBPCUserInsureInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_SUCCESS:	//银行成功
		{
			return OnDBPCUserInsureSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_TRANSFER_INFO:
		{
			return OnDBPCUserInsureTransferInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_FAILURE:	//银行失败
		{
			return OnDBPCUserInsureFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_INSURE_USER_INFO:  //用户信息
		{
			return OnDBPCUserInsureUserInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GP_USER_CHECK_PASSWORD:
		{
			return OnDBPCUserCheckPassword(dwContextID,pData,wDataSize);
		}
	case DBO_GP_OPERATE_SUCCESS:		//操作成功
		{
			return OnDBPCOperateSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_OPERATE_FAILURE:		//操作失败
		{
			return OnDBPCOperateFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_TYPE_ITEM:			//类型子项
		{
			return OnDBPCGameTypeItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_KIND_ITEM:			//类型子项
		{
			return OnDBPCGameKindItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_NODE_ITEM:			//节点子项
		{
			return OnDBPCGameNodeItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_PAGE_ITEM:			//定制子项
		{
			return OnDBPCGamePageItem(dwContextID,pData,wDataSize);
		}
	case DBO_GP_GAME_LIST_RESULT:		//加载结果
		{
			return OnDBPCGameListResult(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SIGNIN_QUERY_SUCCESS:	//查询签到成功
		{
			return OnDBPCSignInQuerySuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GP_SIGNIN_QUERY_FAILURE:   //查询签到失败
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
	case DBO_MB_LOGON_SUCCESS:			//登录成功
		{
			return OnDBMBLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_MB_LOGON_FAILURE:			//登录失败
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
			//自己的分数和名次
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
	case DBO_GP_MB_BIND_NUM_RES:			//手机号绑定结果
		{
			m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_MOBILE_VERIFY_RES, pData, wDataSize);
			return true;
		}
	case DBO_GP_MB_CHECK_NUM_EXIT:			//手机号是否绑定过
		{
			m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPR_CHECK_MOBILE_EXIT_RES, pData, wDataSize);
			return true;
		}
	case DBO_GP_BROADCAST_SPEED:			//请求广播速度
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
		CTraceService::TraceString(TEXT("正在注册游戏登录服务器..."),TraceLevel_Normal);

		//变量定义
		CMD_CS_C_RegisterPlaza RegisterPlaza;
		ZeroMemory(&RegisterPlaza,sizeof(RegisterPlaza));

		//设置变量
		lstrcpyn(RegisterPlaza.szServerName,m_pInitParameter->m_szServerName,CountArray(RegisterPlaza.szServerName));
		lstrcpyn(RegisterPlaza.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(RegisterPlaza.szServerAddr));

		//发送数据
		m_pITCPSocketService->SendData(MDM_CS_REGISTER,SUB_CS_C_REGISTER_PLAZA,&RegisterPlaza,sizeof(RegisterPlaza));

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
				//printf("MDM_CS_MANAGER_SERVICE\n");
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
	// online fake cnt 
	// case fake cnt 
	// SUB_GP_GET_ONLINE copy the content of the 
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

//登录处理
bool CAttemperEngineSink::OnTCPNetworkMainPCLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
	switch (wSubCmdID)
	{
	case SUB_GP_LOGON_GAMEID:		//I D 登录
		{
			return OnTCPNetworkSubPCLogonGameID(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_LOGON_ACCOUNTS:		//帐号登录
		{
			return OnTCPNetworkSubPCLogonAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_REGISTER_ACCOUNTS:	//帐号注册
		{
			return OnTCPNetworkSubPCRegisterAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_GUEST_REG:		//游客注册处理函数
		{
			return OnTCPNetworkSubPCGuestReg(pData,wDataSize,dwSocketID);
		}
	case SUB_GPR_MBREIGSTER:  // 手机号注册
		{
			return OnTCPNetworkSubPCMBNumRegister(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_MOBILE_VERIFICAT:
		{
			return OnTCPNetworkSubPCCodeVerification(pData,wDataSize,dwSocketID);
		}
	case SUB_GPR_CANCEL_LOGINING:  //取消登录排队
		{
			return OnCancelLoginQueueing(dwSocketID);
		}
	}

	return false;
}

//列表处理
bool CAttemperEngineSink::OnTCPNetworkMainPCServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_GET_LIST:			//获取列表
		{
			//发送列表
			SendGameTypeInfo(dwSocketID);
			SendGameKindInfo(dwSocketID);

			//发送列表
			if (m_pInitParameter->m_cbDelayList==TRUE)
			{
				//发送列表
				SendGamePageInfo(dwSocketID,INVALID_WORD);
				SendGameNodeInfo(dwSocketID,INVALID_WORD);
				SendGameServerInfo(dwSocketID,INVALID_WORD);
			}
			else
			{
				//发送页面
				SendGamePageInfo(dwSocketID,0);
			}

			//发送完成
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);

			return true;
		}
	case SUB_GP_GET_SERVER:			//获取房间
		{
			//效验数据
			ASSERT(wDataSize%sizeof(WORD)==0);
			if (wDataSize%sizeof(WORD)!=0) return false;

			//发送列表
			UINT nKindCount=wDataSize/sizeof(WORD);
			for (UINT i=0;i<nKindCount;i++)
			{
				SendGameNodeInfo(dwSocketID,((WORD *)pData)[i]);
				SendGamePageInfo(dwSocketID,((WORD *)pData)[i]);
				SendGameServerInfo(dwSocketID,((WORD *)pData)[i]);
			}

			//发送完成
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_SERVER_FINISH,pData,wDataSize);

			return true;
		}
	case SUB_GP_GET_ONLINE:			//获取在线
		{
			//变量定义
			CMD_GP_GetOnline * pGetOnline=(CMD_GP_GetOnline *)pData;
			WORD wHeadSize=(sizeof(CMD_GP_GetOnline)-sizeof(pGetOnline->wOnLineServerID));

			//效验数据
			ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGetOnline->wServerCount*sizeof(WORD))));
			if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGetOnline->wServerCount*sizeof(WORD)))) return false;

			//变量定义
			CMD_GP_KindOnline KindOnline;
			CMD_GP_ServerOnline ServerOnline;
			ZeroMemory(&KindOnline,sizeof(KindOnline));
			ZeroMemory(&ServerOnline,sizeof(ServerOnline));

			//获取类型
			POSITION KindPosition=NULL;
			do
			{
				//获取类型
				CGameKindItem * pGameKindItem=m_ServerListManager.EmunGameKindItem(KindPosition);

				//设置变量
				if (pGameKindItem!=NULL)
				{
					WORD wKindIndex=KindOnline.wKindCount++;
					KindOnline.OnLineInfoKind[wKindIndex].wKindID=pGameKindItem->m_GameKind.wKindID;
					KindOnline.OnLineInfoKind[wKindIndex].dwOnLineCount=pGameKindItem->m_GameKind.dwOnLineCount;
				}

				//溢出判断
				if (KindOnline.wKindCount>=CountArray(KindOnline.OnLineInfoKind))
				{
					ASSERT(FALSE);
					break;
				}

			} while (KindPosition!=NULL);

			//获取房间
			for (WORD i=0;i<pGetOnline->wServerCount;i++)
			{
				//获取房间
				WORD wServerID=pGetOnline->wOnLineServerID[i];
				CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(wServerID);

				//设置变量
				if (pGameServerItem!=NULL)
				{
					WORD wServerIndex=ServerOnline.wServerCount++;
					ServerOnline.OnLineInfoServer[wServerIndex].wServerID=wServerID;
					ServerOnline.OnLineInfoServer[wServerIndex].dwOnLineCount=pGameServerItem->m_GameServer.dwOnLineCount;
				}

				//溢出判断
				if (ServerOnline.wServerCount>=CountArray(ServerOnline.OnLineInfoServer))
				{
					ASSERT(FALSE);
					break;
				}
			}

			//类型在线
			if (KindOnline.wKindCount>0)
			{
				WORD wHeadSize=sizeof(KindOnline)-sizeof(KindOnline.OnLineInfoKind);
				WORD wSendSize=wHeadSize+KindOnline.wKindCount*sizeof(KindOnline.OnLineInfoKind[0]);
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GR_KINE_ONLINE,&KindOnline,wSendSize);
			}

			//房间在线
			if (ServerOnline.wServerCount>0)
			{
				WORD wHeadSize=sizeof(ServerOnline)-sizeof(ServerOnline.OnLineInfoServer);
				WORD wSendSize=wHeadSize+ServerOnline.wServerCount*sizeof(ServerOnline.OnLineInfoServer[0]);
				m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GR_SERVER_ONLINE,&ServerOnline,wSendSize);
			}

			return true;
		}
	case SUB_GP_GET_COLLECTION:		//获取收藏
		{
			return true;
		}
	}

	return false;
}

//服务处理
bool CAttemperEngineSink::OnTCPNetworkMainPCUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_MODIFY_MACHINE:		//绑定机器
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyMachine));
			if (wDataSize!=sizeof(CMD_GP_ModifyMachine)) return false;

			//处理消息
			CMD_GP_ModifyMachine * pModifyMachine=(CMD_GP_ModifyMachine *)pData;
			pModifyMachine->szPassword[CountArray(pModifyMachine->szPassword)-1]=0;

			//变量定义
			DBR_GP_ModifyMachine ModifyMachine;
			ZeroMemory(&ModifyMachine,sizeof(ModifyMachine));

			//构造数据
			ModifyMachine.cbBind=pModifyMachine->cbBind;
			ModifyMachine.dwUserID=pModifyMachine->dwUserID;
			ModifyMachine.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyMachine.szPassword,pModifyMachine->szPassword,CountArray(ModifyMachine.szPassword));
			lstrcpyn(ModifyMachine.szMachineID,pModifyMachine->szMachineID,CountArray(ModifyMachine.szMachineID));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_MACHINE,dwSocketID,&ModifyMachine,sizeof(ModifyMachine));

			return true;
		}
	case SUB_GP_MODIFY_LOGON_PASS:	//修改密码
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyLogonPass));
			if (wDataSize!=sizeof(CMD_GP_ModifyLogonPass)) return false;

			//处理消息
			CMD_GP_ModifyLogonPass * pModifyLogonPass=(CMD_GP_ModifyLogonPass *)pData;
			pModifyLogonPass->szDesPassword[CountArray(pModifyLogonPass->szDesPassword)-1]=0;
			pModifyLogonPass->szScrPassword[CountArray(pModifyLogonPass->szScrPassword)-1]=0;

			//变量定义
			DBR_GP_ModifyLogonPass ModifyLogonPass;
			ZeroMemory(&ModifyLogonPass,sizeof(ModifyLogonPass));

			//构造数据
			ModifyLogonPass.dwUserID=pModifyLogonPass->dwUserID;
			ModifyLogonPass.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyLogonPass.szDesPassword,pModifyLogonPass->szDesPassword,CountArray(ModifyLogonPass.szDesPassword));
			lstrcpyn(ModifyLogonPass.szScrPassword,pModifyLogonPass->szScrPassword,CountArray(ModifyLogonPass.szScrPassword));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_LOGON_PASS,dwSocketID,&ModifyLogonPass,sizeof(ModifyLogonPass));

			return true;
		}
	case SUB_GP_MODIFY_INSURE_PASS:	//修改密码
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_ModifyInsurePass));
			if (wDataSize!=sizeof(CMD_GP_ModifyInsurePass)) return false;

			//处理消息
			CMD_GP_ModifyInsurePass * pModifyInsurePass=(CMD_GP_ModifyInsurePass *)pData;
			pModifyInsurePass->szDesPassword[CountArray(pModifyInsurePass->szDesPassword)-1]=0;
			pModifyInsurePass->szScrPassword[CountArray(pModifyInsurePass->szScrPassword)-1]=0;

			//变量定义
			DBR_GP_ModifyInsurePass ModifyInsurePass;
			ZeroMemory(&ModifyInsurePass,sizeof(ModifyInsurePass));

			//构造数据
			ModifyInsurePass.dwUserID=pModifyInsurePass->dwUserID;
			ModifyInsurePass.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyInsurePass.szDesPassword,pModifyInsurePass->szDesPassword,CountArray(ModifyInsurePass.szDesPassword));
			lstrcpyn(ModifyInsurePass.szScrPassword,pModifyInsurePass->szScrPassword,CountArray(ModifyInsurePass.szScrPassword));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INSURE_PASS,dwSocketID,&ModifyInsurePass,sizeof(ModifyInsurePass));

			return true;
		}
	case SUB_GP_MODIFY_UNDER_WRITE:	//修改签名
		{
			//变量定义
			CMD_GP_ModifyUnderWrite * pModifyUnderWrite=(CMD_GP_ModifyUnderWrite *)pData;

			//效验参数
			ASSERT(wDataSize<=sizeof(CMD_GP_ModifyUnderWrite));
			ASSERT(wDataSize>=(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite)));
			ASSERT(wDataSize==(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite)+CountStringBuffer(pModifyUnderWrite->szUnderWrite)));

			//效验参数
			if (wDataSize>sizeof(CMD_GP_ModifyUnderWrite)) return false;
			if (wDataSize<(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite))) return false;
			if (wDataSize!=(sizeof(CMD_GP_ModifyUnderWrite)-sizeof(pModifyUnderWrite->szUnderWrite)+CountStringBuffer(pModifyUnderWrite->szUnderWrite))) return false;

			//处理消息
			pModifyUnderWrite->szPassword[CountArray(pModifyUnderWrite->szPassword)-1]=0;
			pModifyUnderWrite->szUnderWrite[CountArray(pModifyUnderWrite->szUnderWrite)-1]=0;

			//变量定义
			DBR_GP_ModifyUnderWrite ModifyUnderWrite;
			ZeroMemory(&ModifyUnderWrite,sizeof(ModifyUnderWrite));

			//构造数据
			ModifyUnderWrite.dwUserID=pModifyUnderWrite->dwUserID;
			ModifyUnderWrite.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyUnderWrite.szPassword,pModifyUnderWrite->szPassword,CountArray(ModifyUnderWrite.szPassword));
			lstrcpyn(ModifyUnderWrite.szUnderWrite,pModifyUnderWrite->szUnderWrite,CountArray(ModifyUnderWrite.szUnderWrite));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_UNDER_WRITE,dwSocketID,&ModifyUnderWrite,sizeof(ModifyUnderWrite));

			return true;
		}
	case SUB_GP_SYSTEM_FACE_INFO:	//修改头像
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_SystemFaceInfo));
			if (wDataSize!=sizeof(CMD_GP_SystemFaceInfo)) return false;

			//处理消息
			CMD_GP_SystemFaceInfo * pSystemFaceInfo=(CMD_GP_SystemFaceInfo *)pData;

			//变量定义
			DBR_GP_ModifySystemFace ModifySystemFace;
			ZeroMemory(&ModifySystemFace,sizeof(ModifySystemFace));

			//构造数据
			ModifySystemFace.wFaceID=pSystemFaceInfo->wFaceID;
			ModifySystemFace.dwUserID=pSystemFaceInfo->dwUserID;
			ModifySystemFace.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifySystemFace.szPassword,pSystemFaceInfo->szPassword,CountArray(ModifySystemFace.szPassword));
			lstrcpyn(ModifySystemFace.szMachineID,pSystemFaceInfo->szMachineID,CountArray(ModifySystemFace.szMachineID));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_SYSTEM_FACE,dwSocketID,&ModifySystemFace,sizeof(ModifySystemFace));

			return true;
		}
	case SUB_GP_CUSTOM_FACE_INFO:	//修改头像
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_CustomFaceInfo));
			if (wDataSize!=sizeof(CMD_GP_CustomFaceInfo)) return false;

			//处理消息
			CMD_GP_CustomFaceInfo * pCustomFaceInfo=(CMD_GP_CustomFaceInfo *)pData;

			//变量定义
			DBR_GP_ModifyCustomFace ModifyCustomFace;
			ZeroMemory(&ModifyCustomFace,sizeof(ModifyCustomFace));

			//构造数据
			ModifyCustomFace.dwUserID=pCustomFaceInfo->dwUserID;
			ModifyCustomFace.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyCustomFace.szPassword,pCustomFaceInfo->szPassword,CountArray(ModifyCustomFace.szPassword));
			lstrcpyn(ModifyCustomFace.szMachineID,pCustomFaceInfo->szMachineID,CountArray(ModifyCustomFace.szMachineID));
			CopyMemory(ModifyCustomFace.dwCustomFace,pCustomFaceInfo->dwCustomFace,sizeof(ModifyCustomFace.dwCustomFace));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_CUSTOM_FACE,dwSocketID,&ModifyCustomFace,sizeof(ModifyCustomFace));

			return true;
		}
	case SUB_GP_QUERY_INDIVIDUAL:	//查询信息
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_QueryIndividual));
			if (wDataSize!=sizeof(CMD_GP_QueryIndividual)) return false;

			//处理消息
			CMD_GP_QueryIndividual * pQueryIndividual=(CMD_GP_QueryIndividual *)pData;

			//变量定义
			DBR_GP_QueryIndividual QueryIndividual;
			ZeroMemory(&QueryIndividual,sizeof(QueryIndividual));

			//构造数据
			QueryIndividual.dwUserID=pQueryIndividual->dwUserID;
			QueryIndividual.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_INDIVIDUAL,dwSocketID,&QueryIndividual,sizeof(QueryIndividual));

			return true;
		}
	case SUB_GP_MODIFY_INDIVIDUAL:	//修改资料
		{
			//效验参数
			ASSERT(wDataSize>=sizeof(CMD_GP_ModifyIndividual));
			if (wDataSize<sizeof(CMD_GP_ModifyIndividual)) return false;

			//处理消息
			CMD_GP_ModifyIndividual * pModifyIndividual=(CMD_GP_ModifyIndividual *)pData;
			pModifyIndividual->szPassword[CountArray(pModifyIndividual->szPassword)-1]=0;

			//变量定义
			DBR_GP_ModifyIndividual ModifyIndividual;
			ZeroMemory(&ModifyIndividual,sizeof(ModifyIndividual));

			//设置变量
			ModifyIndividual.dwUserID=pModifyIndividual->dwUserID;
			ModifyIndividual.cbGender=pModifyIndividual->cbGender;
			ModifyIndividual.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(ModifyIndividual.szPassword,pModifyIndividual->szPassword,CountArray(ModifyIndividual.szPassword));

			//变量定义
			VOID * pDataBuffer=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pModifyIndividual+1,wDataSize-sizeof(CMD_GP_ModifyIndividual));

			//扩展信息
			while (true)
			{
				pDataBuffer=RecvPacket.GetData(DataDescribe);
				if (DataDescribe.wDataDescribe==DTP_NULL) break;
				switch (DataDescribe.wDataDescribe)
				{
				case DTP_GP_UI_NICKNAME:		//用户昵称
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
				case DTP_GP_UI_UNDER_WRITE:		//个性签名
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
				case DTP_GP_UI_USER_NOTE:		//用户备注
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
				case DTP_GP_UI_COMPELLATION:	//真实名字
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
				case DTP_GP_UI_SEAT_PHONE:		//固定电话
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
				case DTP_GP_UI_MOBILE_PHONE:	//移动电话
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
				case DTP_GP_UI_QQ:				//Q Q 号码
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
				case DTP_GP_UI_EMAIL:			//电子邮件
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
				case DTP_GP_UI_DWELLING_PLACE:	//联系地址
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

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INDIVIDUAL,dwSocketID,&ModifyIndividual,sizeof(ModifyIndividual));

			return true;
		}
	case SUB_GP_USER_SAVE_SCORE:	//存入游戏币
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_UserSaveScore));
			if (wDataSize!=sizeof(CMD_GP_UserSaveScore)) return false;

			//处理消息
			CMD_GP_UserSaveScore * pUserSaveScore=(CMD_GP_UserSaveScore *)pData;
			pUserSaveScore->szMachineID[CountArray(pUserSaveScore->szMachineID)-1]=0;

			//变量定义
			DBR_GP_UserSaveScore UserSaveScore;
			ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

			//构造数据
			UserSaveScore.dwUserID=pUserSaveScore->dwUserID;
			UserSaveScore.lSaveScore=pUserSaveScore->lSaveScore;
			UserSaveScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserSaveScore.szMachineID,pUserSaveScore->szMachineID,CountArray(UserSaveScore.szMachineID));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_SAVE_SCORE,dwSocketID,&UserSaveScore,sizeof(UserSaveScore));

			return true;
		}
	case SUB_GP_USER_TAKE_SCORE:	//取出游戏币
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_UserTakeScore));
			if (wDataSize!=sizeof(CMD_GP_UserTakeScore)) return false;

			//处理消息
			CMD_GP_UserTakeScore * pUserTakeScore=(CMD_GP_UserTakeScore *)pData;
			pUserTakeScore->szPassword[CountArray(pUserTakeScore->szPassword)-1]=0;
			pUserTakeScore->szMachineID[CountArray(pUserTakeScore->szMachineID)-1]=0;

			//变量定义
			DBR_GP_UserTakeScore UserTakeScore;
			ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

			//构造数据
			UserTakeScore.dwUserID=pUserTakeScore->dwUserID;
			UserTakeScore.lTakeScore=pUserTakeScore->lTakeScore;
			UserTakeScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserTakeScore.szPassword,pUserTakeScore->szPassword,CountArray(UserTakeScore.szPassword));
			lstrcpyn(UserTakeScore.szMachineID,pUserTakeScore->szMachineID,CountArray(UserTakeScore.szMachineID));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_TAKE_SCORE,dwSocketID,&UserTakeScore,sizeof(UserTakeScore));

			return true;
		}
	case SUB_GP_USER_TRANSFER_SCORE://转账游戏币
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_UserTransferScore));
			if (wDataSize!=sizeof(CMD_GP_UserTransferScore)) return false;

			//处理消息
			CMD_GP_UserTransferScore * pUserTransferScore=(CMD_GP_UserTransferScore *)pData;
			pUserTransferScore->szNickName[CountArray(pUserTransferScore->szNickName)-1]=0;
			pUserTransferScore->szPassword[CountArray(pUserTransferScore->szPassword)-1]=0;
			pUserTransferScore->szMachineID[CountArray(pUserTransferScore->szMachineID)-1]=0;

			//变量定义
			DBR_GP_UserTransferScore UserTransferScore;
			ZeroMemory(&UserTransferScore,sizeof(UserTransferScore));

			//构造数据
			UserTransferScore.dwUserID=pUserTransferScore->dwUserID;
			UserTransferScore.cbByNickName=pUserTransferScore->cbByNickName;
			UserTransferScore.lTransferScore=pUserTransferScore->lTransferScore;
			UserTransferScore.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
			lstrcpyn(UserTransferScore.szNickName,pUserTransferScore->szNickName,CountArray(UserTransferScore.szNickName));
			lstrcpyn(UserTransferScore.szPassword,pUserTransferScore->szPassword,CountArray(UserTransferScore.szPassword));
			lstrcpyn(UserTransferScore.szMachineID,pUserTransferScore->szMachineID,CountArray(UserTransferScore.szMachineID));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_TRANSFER_SCORE,dwSocketID,&UserTransferScore,sizeof(UserTransferScore));

			return true;
		}
	case SUB_GP_QUERY_INSURE_INFO:	//查询银行
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_QueryInsureInfo));
			if (wDataSize!=sizeof(CMD_GP_QueryInsureInfo)) return false;

			//处理消息
			CMD_GP_QueryInsureInfo * pQueryInsureInfo=(CMD_GP_QueryInsureInfo *)pData;

			//变量定义
			DBR_GP_QueryInsureInfo QueryInsureInfo;
			ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

			//构造数据
			QueryInsureInfo.dwUserID=pQueryInsureInfo->dwUserID;
			QueryInsureInfo.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_INSURE_INFO,dwSocketID,&QueryInsureInfo,sizeof(QueryInsureInfo));

			return true;
		}
	case SUB_GP_USER_CHECK_PASSWORD:
		{
			if (wDataSize!=sizeof(CMD_GP_CheckPassword))
			{
				return false;
			}

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_CHECK_PASSWORD,dwSocketID,pData,wDataSize);

			return true;
		}
	case SUB_GP_QUERY_USER_INFO_REQUEST:  //查询用户
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_QueryUserInfoRequest));
			if (wDataSize!=sizeof(CMD_GP_QueryUserInfoRequest)) return false;

			//处理消息
			CMD_GP_QueryUserInfoRequest * pQueryUserInfoRequest=(CMD_GP_QueryUserInfoRequest *)pData;

			//变量定义
			DBR_GP_QueryInsureUserInfo QueryInsureUserInfo;
			ZeroMemory(&QueryInsureUserInfo,sizeof(QueryInsureUserInfo));

			//构造数据
			QueryInsureUserInfo.cbByNickName=pQueryUserInfoRequest->cbByNickName;
			lstrcpyn(QueryInsureUserInfo.szNickName,pQueryUserInfoRequest->szNickName,CountArray(QueryInsureUserInfo.szNickName));

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_USER_INFO,dwSocketID,&QueryInsureUserInfo,sizeof(QueryInsureUserInfo));

			return true;
		}
	case SUB_GP_QUERY_SIGNIN_INFO:
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_QuerySigninInfo));
			if (wDataSize!=sizeof(CMD_GP_QuerySigninInfo)) return false;

			//处理消息
			CMD_GP_QuerySigninInfo * pQuerySignInInfo=(CMD_GP_QuerySigninInfo *)pData;

			//变量定义
			DBR_GP_QuerySignInInfo QuerySignInInfo;
			ZeroMemory(&QuerySignInInfo,sizeof(QuerySignInInfo));

			QuerySignInInfo.dwUserID = pQuerySignInInfo->dwUserID;

			//投递请求
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_SIGIN_INFO,dwSocketID,&QuerySignInInfo,sizeof(QuerySignInInfo));

			return true;
		}
	case SUB_GP_USER_SIGNIN_REQ:
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_SignIn));
			if (wDataSize!=sizeof(CMD_GP_SignIn)) return false;

			//处理消息
			CMD_GP_SignIn * pUserSignIn=(CMD_GP_SignIn *)pData;

			//变量定义
			DBR_GP_UserSignIn UserSignIn;
			ZeroMemory(&UserSignIn,sizeof(UserSignIn));

			UserSignIn.dwUserID = pUserSignIn->dwUserID;
			UserSignIn.cbVip = pUserSignIn->cbVip;

			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_SIGIN,dwSocketID,&UserSignIn,sizeof(UserSignIn));

			return true;
		}
	case SUB_GP_QUERY_FREE_LABA_COUNT:
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GPR_FREE_LABA_QUERY));
			if (wDataSize!=sizeof(CMD_GPR_FREE_LABA_QUERY)) return false;

			//处理消息
			CMD_GPR_FREE_LABA_QUERY * pQueryInfo=(CMD_GPR_FREE_LABA_QUERY *)pData;

			//变量定义
			DBR_GP_Free_LaBa_Count sQueryInfo;
			ZeroMemory(&sQueryInfo,sizeof(sQueryInfo));
			sQueryInfo.dwUserID = pQueryInfo->dwUserID;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_FREE_LABA_COUNT,dwSocketID,&sQueryInfo,sizeof(sQueryInfo));

			return true;
		}
	case SUB_GP_BUY_SKILL:
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GPR_BuySkill));
			if (wDataSize!=sizeof(CMD_GPR_BuySkill)) return false;

			//处理消息
			CMD_GPR_BuySkill * pBuyInfo=(CMD_GPR_BuySkill *)pData;

			//变量定义
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
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_QryLtryInfo));
			if (wDataSize!=sizeof(CMD_GP_QryLtryInfo)) return false;

			//变量定义
			WORD wBindIndex=LOWORD(dwSocketID);
			tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);
			pBindParameter->bIsLtryInfoSend=true;

			//处理消息
			CMD_GP_QryLtryInfo * pUsrLtry=(CMD_GP_QryLtryInfo *)pData;

			//变量定义
			DBR_GP_UserLtry UserLtry;
			ZeroMemory(&UserLtry,sizeof(UserLtry));

			UserLtry.dwUserID = pUsrLtry->dwUserID;

			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_QUERY_LOTTERY_INFO,dwSocketID,&UserLtry,sizeof(UserLtry));

			return true;
		}
	case SUB_GP_USER_LOTTERY:
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_UserLtry));
			if (wDataSize!=sizeof(CMD_GP_UserLtry)) return false;

			//处理消息
			CMD_GP_UserLtry * pUserLtry=(CMD_GP_UserLtry *)pData;

			//变量定义
			DBR_GP_UserLtry UserLtry;
			ZeroMemory(&UserLtry,sizeof(UserLtry));

			UserLtry.dwUserID = pUserLtry->dwUserID;

			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOTTERY_REQ,dwSocketID,&UserLtry,sizeof(UserLtry));

			return true;
		}
	case SUB_GP_LOTTERY_FINISH:
		{
			//变量定义
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

			//判断是否已存在
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
			_snprintf(ShareInfo.szContent, sizeof(ShareInfo.szContent), "%s", "捕鱼大咖里的这条美人鱼价值1000元，炮炮中奖，万人在线");
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
	case SUB_GPR_BROADCAST_SPEED:  //请求广播速度
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
	case SUB_GPR_BROAD_LABA:  //喇叭广播
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

//远程处理
bool CAttemperEngineSink::OnTCPNetworkMainPCRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_C_SEARCH_CORRESPOND:	//协调查找
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GP_C_SearchCorrespond));
			if (wDataSize!=sizeof(CMD_GP_C_SearchCorrespond)) return false;

			//处理消息
			CMD_GP_C_SearchCorrespond * pSearchCorrespond=(CMD_GP_C_SearchCorrespond *)pData;
			pSearchCorrespond->szNickName[CountArray(pSearchCorrespond->szNickName)-1]=0;

			//变量定义
			CMD_CS_C_SearchCorrespond SearchCorrespond;
			ZeroMemory(&SearchCorrespond,sizeof(SearchCorrespond));

			//连接变量
			SearchCorrespond.dwSocketID=dwSocketID;
			SearchCorrespond.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;

			//查找变量
			SearchCorrespond.dwGameID=pSearchCorrespond->dwGameID;
			lstrcpyn(SearchCorrespond.szNickName,pSearchCorrespond->szNickName,CountArray(SearchCorrespond.szNickName));

			//发送数据
			m_pITCPSocketService->SendData(MDM_CS_REMOTE_SERVICE,SUB_CS_C_SEARCH_CORRESPOND,&SearchCorrespond,sizeof(SearchCorrespond));

			return true;
		}
	}

	return false;
}

//登录处理
bool CAttemperEngineSink::OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_LOGON_GAMEID:		//I D 登录
		{
			return OnTCPNetworkSubMBLogonGameID(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_LOGON_ACCOUNTS:		//帐号登录
		{
			return OnTCPNetworkSubMBLogonAccounts(pData,wDataSize,dwSocketID);
		}
	case SUB_GP_REGISTER_ACCOUNTS:	//帐号注册
		{
			return OnTCPNetworkSubMBRegisterAccounts(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//列表处理
bool CAttemperEngineSink::OnTCPNetworkMainMBServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	return false;
}

//网站消息
bool CAttemperEngineSink::OnTCPNetworkMainWebSite(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GP_WEB_CHARGE_VALUE:
		{
			////接受字符串
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

			//验证
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

//I D 登录
bool CAttemperEngineSink::OnTCPNetworkSubPCLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_GP_LogonGameID));
	if (wDataSize<sizeof(CMD_GP_LogonGameID))
	{
		if (wDataSize<sizeof(CMD_GP_LogonGameID)-sizeof(BYTE))
			return false;
	}

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_GP_LogonGameID * pLogonGameID=(CMD_GP_LogonGameID *)pData;
	pLogonGameID->szPassword[CountArray(pLogonGameID->szPassword)-1]=0;
	pLogonGameID->szPassPortID[CountArray(pLogonGameID->szPassPortID)-1]=0;
	pLogonGameID->szMachineID[CountArray(pLogonGameID->szMachineID)-1]=0;

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonGameID->dwPlazaVersion;

	//效验版本
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pLogonGameID->dwPlazaVersion,dwSocketID,((pLogonGameID->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//变量定义
	DBR_GP_LogonGameID LogonGameID;
	ZeroMemory(&LogonGameID,sizeof(LogonGameID));

	//附加信息
	LogonGameID.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	LogonGameID.dwGameID=pLogonGameID->dwGameID;
	LogonGameID.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonGameID.szPassword,pLogonGameID->szPassword,CountArray(LogonGameID.szPassword));
	lstrcpyn(LogonGameID.szPassPortID,pLogonGameID->szPassPortID,CountArray(LogonGameID.szPassPortID));
	lstrcpyn(LogonGameID.szMachineID,pLogonGameID->szMachineID,CountArray(LogonGameID.szMachineID));
	LogonGameID.cbNeeValidateMBCard=(pLogonGameID->cbValidateFlags&MB_VALIDATE_FLAGS);

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_GAMEID,dwSocketID,&LogonGameID,sizeof(LogonGameID));

	return true;
}

//帐号登录
bool CAttemperEngineSink::OnTCPNetworkSubPCLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_GP_LogonAccounts));
	if (wDataSize<sizeof(CMD_GP_LogonAccounts))
	{
		if (wDataSize<sizeof(CMD_GP_LogonAccounts)-sizeof(BYTE))
			return false;
	}

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_GP_LogonAccounts * pLogonAccounts=(CMD_GP_LogonAccounts *)pData;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szPassPortID[CountArray(pLogonAccounts->szPassPortID)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//版本判断
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pLogonAccounts->dwPlazaVersion,dwSocketID,((pLogonAccounts->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//变量定义
	DBR_GP_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//附加信息
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
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

	//扩展信息
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pLogonAccounts+1,wDataSize-sizeof(CMD_GP_LogonAccounts));
	while (true)
	{
		//提取数据
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;

		char szBuffer[2048] = {0};
		//提取数据
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

	//投递请求
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

//帐号注册
bool CAttemperEngineSink::OnTCPNetworkSubPCRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_GP_RegisterAccounts));
	if (wDataSize<sizeof(CMD_GP_RegisterAccounts))
	{
		if (wDataSize<sizeof(CMD_GP_RegisterAccounts)-sizeof(BYTE))
			return false;
	}

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
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
	//敏感词
	CString sNickName = pRegisterAccounts->szNickName;
	for (int i = 0; i < (sizeof(pszSensitiveWords)/sizeof(pszSensitiveWords[0])); i++)
	{
		if (sNickName.Find(pszSensitiveWords[i]) != -1)
		{
			printf("sensitive words %s %s %d\n", sNickName, pszSensitiveWords[i], i);
			CMD_GP_LogonFailure LogonFailure = {0};

			LogonFailure.lResultCode = 0;
			lstrcpyn(LogonFailure.szDescribeString,TEXT("您的昵称中包含敏感词汇，注册失败！"),CountArray(LogonFailure.szDescribeString));

			WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_LOGON_FAILURE,&LogonFailure,wSendSize);

			return true;
		}
	}
	//--------

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	//效验版本
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pRegisterAccounts->dwPlazaVersion,dwSocketID,((pRegisterAccounts->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//变量定义
	DBR_GP_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//附加信息
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
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

	//扩展信息
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pRegisterAccounts+1,wDataSize-sizeof(CMD_GP_RegisterAccounts));
	while (true)
	{
		//提取数据
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;

		char szBuffer[2048] = {0};
		//提取数据
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_REG_AGENTID:	//推荐人信息
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

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_REGISTER_ACCOUNTS,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));
	return true;
}

//产生随机密码
VOID CAttemperEngineSink::GenRandCode(TCHAR szRandCode[])
{
	//生成六位随机字符串
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

//游客登录
bool CAttemperEngineSink::OnTCPNetworkSubPCGuestReg( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_GP_Guest_Reg));
	if (wDataSize<sizeof(CMD_GP_Guest_Reg))
	{
		if (wDataSize<sizeof(CMD_GP_Guest_Reg)-sizeof(BYTE))
			return false;
	}

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_GP_Guest_Reg * pGuestRegInfo=(CMD_GP_Guest_Reg *)pData;

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pGuestRegInfo->dwPlazaVersion;

	//效验版本
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pGuestRegInfo->dwPlazaVersion,dwSocketID,((pGuestRegInfo->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//变量定义
	DBR_GP_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//附加信息
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据 测试数据
	TCHAR szLogonPass[LEN_PASSWORD];
	RegisterAccounts.wFaceID=1;
	RegisterAccounts.cbGender=1;
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szAccounts,pGuestRegInfo->szAccounts,CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName,pGuestRegInfo->szNickName,CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szMachineID,pGuestRegInfo->szMachineID,CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szPassPortID,pGuestRegInfo->szPassPortID,CountArray(RegisterAccounts.szPassPortID));
	GenRandCode(szLogonPass);

	//数据加密
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

	//扩展信息
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pGuestRegInfo+1,wDataSize-sizeof(CMD_GP_Guest_Reg));
	while (true)
	{
		//提取数据
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;

		char szBuffer[2048] = {0};
		//提取数据
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
	
	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GUEST_REGISTER,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	return true;
}

//I D 登录
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonGameID));
	if (wDataSize<sizeof(CMD_MB_LogonGameID)) return false;

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_MB_LogonGameID * pLogonGameID=(CMD_MB_LogonGameID *)pData;
	pLogonGameID->szPassword[CountArray(pLogonGameID->szPassword)-1]=0;
	pLogonGameID->szMachineID[CountArray(pLogonGameID->szMachineID)-1]=0;
	pLogonGameID->szMobilePhone[CountArray(pLogonGameID->szMobilePhone)-1]=0;

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonGameID->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonGameID->dwPlazaVersion;

	//效验版本
	if (CheckPlazaVersion(pLogonGameID->cbDeviceType,pLogonGameID->dwPlazaVersion,dwSocketID)==false) return true;

	//变量定义
	DBR_MB_LogonGameID LogonGameID;
	ZeroMemory(&LogonGameID,sizeof(LogonGameID));

	//附加信息
	LogonGameID.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	LogonGameID.dwGameID=pLogonGameID->dwGameID;
	LogonGameID.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonGameID.szPassword,pLogonGameID->szPassword,CountArray(LogonGameID.szPassword));
	lstrcpyn(LogonGameID.szMachineID,pLogonGameID->szMachineID,CountArray(LogonGameID.szMachineID));
	lstrcpyn(LogonGameID.szMobilePhone,pLogonGameID->szMobilePhone,CountArray(LogonGameID.szMobilePhone));

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_GAMEID,dwSocketID,&LogonGameID,sizeof(LogonGameID));

	return true;
}

//帐号登录
bool CAttemperEngineSink::OnTCPNetworkSubMBLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_MB_LogonAccounts));
	if (wDataSize<sizeof(CMD_MB_LogonAccounts)) return false;

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_MB_LogonAccounts * pLogonAccounts=(CMD_MB_LogonAccounts *)pData;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;
	pLogonAccounts->szMobilePhone[CountArray(pLogonAccounts->szMobilePhone)-1]=0;

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pLogonAccounts->wModuleID;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;

	//版本判断
	if (CheckPlazaVersion(pLogonAccounts->cbDeviceType,pLogonAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//变量定义
	DBR_MB_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//附加信息
	LogonAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	LogonAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));
	lstrcpyn(LogonAccounts.szMobilePhone,pLogonAccounts->szMobilePhone,CountArray(LogonAccounts.szMobilePhone));

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	return true;
}

//帐号注册
bool CAttemperEngineSink::OnTCPNetworkSubMBRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_MB_RegisterAccounts));
	if (wDataSize<sizeof(CMD_MB_RegisterAccounts)) return false;

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_MB_RegisterAccounts * pRegisterAccounts=(CMD_MB_RegisterAccounts *)pData;
	pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
	pRegisterAccounts->szNickName[CountArray(pRegisterAccounts->szNickName)-1]=0;
	pRegisterAccounts->szMachineID[CountArray(pRegisterAccounts->szMachineID)-1]=0;
	pRegisterAccounts->szLogonPass[CountArray(pRegisterAccounts->szLogonPass)-1]=0;
	pRegisterAccounts->szInsurePass[CountArray(pRegisterAccounts->szInsurePass)-1]=0;
	pRegisterAccounts->szMobilePhone[CountArray(pRegisterAccounts->szMobilePhone)-1]=0;

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->wModuleID=pRegisterAccounts->wModuleID;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	//效验版本
	if (CheckPlazaVersion(pRegisterAccounts->cbDeviceType,pRegisterAccounts->dwPlazaVersion,dwSocketID)==false) return true;

	//变量定义
	DBR_MB_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//附加信息
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
	RegisterAccounts.wFaceID=pRegisterAccounts->wFaceID;
	RegisterAccounts.cbGender=pRegisterAccounts->cbGender;
	RegisterAccounts.dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientAddr;
	lstrcpyn(RegisterAccounts.szAccounts,pRegisterAccounts->szAccounts,CountArray(RegisterAccounts.szAccounts));
	lstrcpyn(RegisterAccounts.szNickName,pRegisterAccounts->szNickName,CountArray(RegisterAccounts.szNickName));
	lstrcpyn(RegisterAccounts.szMachineID,pRegisterAccounts->szMachineID,CountArray(RegisterAccounts.szMachineID));
	lstrcpyn(RegisterAccounts.szLogonPass,pRegisterAccounts->szLogonPass,CountArray(RegisterAccounts.szLogonPass));
	lstrcpyn(RegisterAccounts.szInsurePass,pRegisterAccounts->szInsurePass,CountArray(RegisterAccounts.szInsurePass));
	lstrcpyn(RegisterAccounts.szMobilePhone,pRegisterAccounts->szMobilePhone,CountArray(RegisterAccounts.szMobilePhone));

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_MB_REGISTER_ACCOUNTS,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));

	return true;
}

//登录成功
bool CAttemperEngineSink::OnDBPCLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	DBO_GP_LogonSuccess * pDBOLogonSuccess=(DBO_GP_LogonSuccess *)pData;
	if (pDBOLogonSuccess->bIsLogin)
	{
		m_nSendMaxNum--;
		CVerification::GetInstance()->OnRecordFinishLoginNum(m_nSendMaxNum);
	}
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;
	tagBindParameter * pBindParameter = (m_pBindParameter+LOWORD(dwContextID));

	//变量定义
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	CMD_GP_LogonSuccess * pCMDLogonSuccess=(CMD_GP_LogonSuccess *)cbDataBuffer;

	//发送定义
	WORD wHeadSize=sizeof(CMD_GP_LogonSuccess);
	CSendPacketHelper SendPacket(cbDataBuffer+wHeadSize,sizeof(cbDataBuffer)-wHeadSize);

	//设置变量
	ZeroMemory(pCMDLogonSuccess,sizeof(CMD_GP_LogonSuccess));

	//构造数据
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

	//用户成绩
	pCMDLogonSuccess->lUserScore=pDBOLogonSuccess->lUserScore;
	pCMDLogonSuccess->lUserInsure=pDBOLogonSuccess->lUserInsure;

	//配置信息
	pCMDLogonSuccess->cbShowServerStatus=m_bShowServerStatus?1:0;

	//会员信息
	if (pDBOLogonSuccess->cbMemberOrder!=0)
	{
		DTP_GP_MemberInfo MemberInfo;
		ZeroMemory(&MemberInfo,sizeof(MemberInfo));
		MemberInfo.cbMemberOrder=pDBOLogonSuccess->cbMemberOrder;
		MemberInfo.MemberOverDate=pDBOLogonSuccess->MemberOverDate;
		SendPacket.AddPacket(&MemberInfo,sizeof(MemberInfo),DTP_GP_MEMBER_INFO);
	}

	//个性签名
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

	//玩家会员等级（会员可以看到银行赠送记录）
	DTP_GP_MemberOrder MemberOrder = {0};
	MemberOrder.cbMemberOrder = pDBOLogonSuccess->cbMemberOrder;
	SendPacket.AddPacket(&MemberOrder, sizeof(DTP_GP_MemberOrder), DTP_GP_MEMBER_ORDER);

	//奖券
	DTP_GP_GiftTicket GiftTicket = {0};
	GiftTicket.dwCount = pDBOLogonSuccess->dwLotteryCount;
	SendPacket.AddPacket(&GiftTicket, sizeof(DTP_GP_GiftTicket), DTP_GP_GIFT_TICKET);

	//牛牛底注
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

	////显示假桌子
	//DTP_GP_ShowFakeTable ShowFakeTable = {0};
	//ShowFakeTable.cbShowFakeTable = 1;
	//SendPacket.AddPacket(&ShowFakeTable, sizeof(DTP_GP_ShowFakeTable), DTP_GP_SHOWFAKETABLE);

	//玩家的VIP信息
	DTP_GP_VipInfo VipInfo = {0};
	VipInfo.cbVIPLevel = pDBOLogonSuccess->cbVIPLevel;
	VipInfo.dwVIPExp = pDBOLogonSuccess->dwVIPExp;
	VipInfo.dwExpPerCent = pDBOLogonSuccess->dwExpPerCent;
	SendPacket.AddPacket(&VipInfo, sizeof(DTP_GP_VipInfo), DTP_GP_VIPINFO);

	//客服QQ
	SendPacket.AddPacket(pDBOLogonSuccess->szCustomerServiceQQ, DTP_GP_CUSTOMERSERVICEQQ);

	//登录奖励
	DTP_GP_LogonGift LogonGift = {0};
	LogonGift.dwGiftScore = pDBOLogonSuccess->nLogonGift;
	LogonGift.cbCanGetBankruptcy = pDBOLogonSuccess->cbCanGetBankruptcy;
	LogonGift.nBankruptcyCount = pDBOLogonSuccess->nBankruptcyCount;
	LogonGift.nCanShare = pDBOLogonSuccess->nCanShare;
	LogonGift.nBankruptcyCondition = 1000;
	LogonGift.nBankruptcyCountMax = 3;
	SendPacket.AddPacket(&LogonGift, sizeof(DTP_GP_LogonGift), DTP_GP_LOGONGIFT);

	//首充礼包信息
	DTP_GP_Charge1stInfo Charge1stInfo = {0};
	Charge1stInfo.cbCount = pDBOLogonSuccess->cbCharge1stCount;
	Charge1stInfo.cbCanGet = pDBOLogonSuccess->cbCharge1stCanGet;
	SendPacket.AddPacket(&Charge1stInfo, sizeof(DTP_GP_Charge1stInfo), DTP_GP_CHARGE1ST_INFO);

	SendPacket.AddPacket(pDBOLogonSuccess->szPassPortID, DTP_GP_PASS_PORT_ID);

	//登录成功
	WORD wSendSize=SendPacket.GetDataSize()+sizeof(CMD_GP_LogonSuccess);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_SUCCESS,cbDataBuffer,wSendSize);

	//printf("wSendSize is %hu\n", wSendSize);

	//发送列表
	if (m_pInitParameter->m_cbDelayList==TRUE)
	{
		SendGameServerInfoEx(dwContextID,INVALID_WORD);
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);
	}
	else
	{
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_FINISH);
	}

	//登录完成
	CMD_GP_LogonFinish LogonFinish;
	ZeroMemory(&LogonFinish,sizeof(LogonFinish));
	LogonFinish.wIntermitTime=m_pInitParameter->m_wIntermitTime;
	LogonFinish.wOnLineCountTime=m_pInitParameter->m_wOnLineCountTime;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_FINISH,&LogonFinish,sizeof(LogonFinish));
	printf("登录成功\n");
	OnRequestDataBaseLogin();
	return true;
}

//登录失败
bool CAttemperEngineSink::OnDBPCLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//变量定义
	CMD_GP_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));
	DBO_GP_LogonFailure * pLogonFailure=(DBO_GP_LogonFailure *)pData;
	if (pLogonFailure->bIsLogin)
	{
		m_nSendMaxNum--;
		CVerification::GetInstance()->OnRecordFinishLoginNum(m_nSendMaxNum);
	}
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//构造数据
	LogonFailure.lResultCode=pLogonFailure->lResultCode;
	lstrcpyn(LogonFailure.szDescribeString,pLogonFailure->szDescribeString,CountArray(LogonFailure.szDescribeString));

	//发送数据
	WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_FAILURE,&LogonFailure,wSendSize);

	//关闭连接
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	printf("登录失败 %s\n", LogonFailure.szDescribeString);
	OnRequestDataBaseLogin();

	return true;
}

//登录失败
bool CAttemperEngineSink::OnDBPCLogonValidateMBCard(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBR_GP_ValidateMBCard));
	if (wDataSize!=sizeof(DBR_GP_ValidateMBCard)) return false;

	DBR_GP_ValidateMBCard *pValidateMBCard=(DBR_GP_ValidateMBCard *)pData;

	//发送消息
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_VALIDATE_MBCARD,pData,wDataSize);

	return true;
}

//登录失败
bool CAttemperEngineSink::OnDBPCLogonValidatePassPostID(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//发送消息
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_VALIDATE_PASSPOSTID);

	return true;
}

//游客登录
bool CAttemperEngineSink::OnDBPCGuestLogon(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;
	tagBindParameter * pBindParameter = (m_pBindParameter+LOWORD(dwContextID));

	//变量定义
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_LogonSuccess * pDBOLogonSuccess=(DBO_GP_LogonSuccess *)pData;
	CMD_GP_GuestRegSuccess * pCMDLogonSuccess=(CMD_GP_GuestRegSuccess *)cbDataBuffer;

	//发送定义
	WORD wHeadSize=sizeof(CMD_GP_GuestRegSuccess);
	CSendPacketHelper SendPacket(cbDataBuffer+wHeadSize,sizeof(cbDataBuffer)-wHeadSize);

	//设置变量
	ZeroMemory(pCMDLogonSuccess,sizeof(CMD_GP_GuestRegSuccess));

	//构造数据
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

	//用户成绩
	pCMDLogonSuccess->lUserScore=pDBOLogonSuccess->lUserScore;
	pCMDLogonSuccess->lUserInsure=pDBOLogonSuccess->lUserInsure;

	//配置信息
	pCMDLogonSuccess->cbShowServerStatus=m_bShowServerStatus?1:0;

	//会员信息
	if (pDBOLogonSuccess->cbMemberOrder!=0)
	{
		DTP_GP_MemberInfo MemberInfo;
		ZeroMemory(&MemberInfo,sizeof(MemberInfo));
		MemberInfo.cbMemberOrder=pDBOLogonSuccess->cbMemberOrder;
		MemberInfo.MemberOverDate=pDBOLogonSuccess->MemberOverDate;
		SendPacket.AddPacket(&MemberInfo,sizeof(MemberInfo),DTP_GP_MEMBER_INFO);
	}

	//个性签名
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

	//玩家的VIP信息
	DTP_GP_VipInfo VipInfo = {0};
	VipInfo.cbVIPLevel = pDBOLogonSuccess->cbVIPLevel;
	VipInfo.dwVIPExp = pDBOLogonSuccess->dwVIPExp;
	VipInfo.dwExpPerCent = pDBOLogonSuccess->dwExpPerCent;
	SendPacket.AddPacket(&VipInfo, sizeof(DTP_GP_VipInfo), DTP_GP_VIPINFO);

	//客服QQ
	SendPacket.AddPacket(pDBOLogonSuccess->szCustomerServiceQQ, DTP_GP_CUSTOMERSERVICEQQ);

	//登录奖励
	DTP_GP_LogonGift LogonGift = {0};
	LogonGift.dwGiftScore = pDBOLogonSuccess->nLogonGift;
	LogonGift.cbCanGetBankruptcy = pDBOLogonSuccess->cbCanGetBankruptcy;
	LogonGift.nBankruptcyCount = pDBOLogonSuccess->nBankruptcyCount;
	LogonGift.nCanShare = pDBOLogonSuccess->nCanShare;
	LogonGift.nBankruptcyCondition = 1000;
	SendPacket.AddPacket(&LogonGift, sizeof(DTP_GP_LogonGift), DTP_GP_LOGONGIFT);
	
	SendPacket.AddPacket(pDBOLogonSuccess->szPassPortID, DTP_GP_PASS_PORT_ID);

	//登录成功
	WORD wSendSize=SendPacket.GetDataSize()+sizeof(CMD_GP_GuestRegSuccess);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_GUEST_REG_SUCCESS,cbDataBuffer,wSendSize);

	//发送列表
	//if (m_pInitParameter->m_cbDelayList==TRUE)
	//{
	//	//发送列表
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


	//登录完成
	CMD_GP_LogonFinish LogonFinish;
	ZeroMemory(&LogonFinish,sizeof(LogonFinish));
	LogonFinish.wIntermitTime=m_pInitParameter->m_wIntermitTime;
	LogonFinish.wOnLineCountTime=m_pInitParameter->m_wOnLineCountTime;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_FINISH,&LogonFinish,sizeof(LogonFinish));

	return true;
}

//用户头像
bool CAttemperEngineSink::OnDBPCUserFaceInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	CMD_GP_UserFaceInfo UserFaceInfo;
	ZeroMemory(&UserFaceInfo,sizeof(UserFaceInfo));
	DBO_GP_UserFaceInfo * pUserFaceInfo=(DBO_GP_UserFaceInfo *)pData;

	//设置变量
	UserFaceInfo.wFaceID=pUserFaceInfo->wFaceID;
	UserFaceInfo.dwCustomID=pUserFaceInfo->dwCustomID;

	//发送消息
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_FACE_INFO,&UserFaceInfo,sizeof(UserFaceInfo));

	return true;
}

//用户信息
bool CAttemperEngineSink::OnDBPCUserIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_UserIndividual * pDBOUserIndividual=(DBO_GP_UserIndividual *)pData;
	CMD_GP_UserIndividual * pCMDUserIndividual=(CMD_GP_UserIndividual *)cbDataBuffer;
	CSendPacketHelper SendPacket(cbDataBuffer+sizeof(CMD_GP_UserIndividual),sizeof(cbDataBuffer)-sizeof(CMD_GP_UserIndividual));

	//设置变量
	ZeroMemory(pCMDUserIndividual,sizeof(CMD_GP_UserIndividual));

	//构造数据
	pCMDUserIndividual->dwUserID=pDBOUserIndividual->dwUserID;

	//用户信息
	if (pDBOUserIndividual->szUserNote[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szUserNote);
		SendPacket.AddPacket(pDBOUserIndividual->szUserNote,wBufferSize,DTP_GP_UI_USER_NOTE);
	}

	//真实姓名
	if (pDBOUserIndividual->szCompellation[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szCompellation);
		SendPacket.AddPacket(pDBOUserIndividual->szCompellation,wBufferSize,DTP_GP_UI_COMPELLATION);
	}

	//电话号码
	if (pDBOUserIndividual->szSeatPhone[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szSeatPhone);
		SendPacket.AddPacket(pDBOUserIndividual->szSeatPhone,wBufferSize,DTP_GP_UI_SEAT_PHONE);
	}

	//移动电话
	if (pDBOUserIndividual->szMobilePhone[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szMobilePhone);
		SendPacket.AddPacket(pDBOUserIndividual->szMobilePhone,wBufferSize,DTP_GP_UI_MOBILE_PHONE);
	}

	//联系资料
	if (pDBOUserIndividual->szQQ[0]!=0) 
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szQQ);
		SendPacket.AddPacket(pDBOUserIndividual->szQQ,wBufferSize,DTP_GP_UI_QQ);
	}

	//电子邮件
	if (pDBOUserIndividual->szEMail[0]!=0) 
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szEMail);
		SendPacket.AddPacket(pDBOUserIndividual->szEMail,wBufferSize,DTP_GP_UI_EMAIL);
	}

	//联系地址
	if (pDBOUserIndividual->szDwellingPlace[0]!=0)
	{
		WORD wBufferSize=CountStringBuffer(pDBOUserIndividual->szDwellingPlace);
		SendPacket.AddPacket(pDBOUserIndividual->szDwellingPlace,wBufferSize,DTP_GP_UI_DWELLING_PLACE);
	}

	//发送消息
	WORD wSendSize=sizeof(CMD_GP_UserIndividual)+SendPacket.GetDataSize();
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INDIVIDUAL,cbDataBuffer,wSendSize);

	return true;
}

//银行信息
bool CAttemperEngineSink::OnDBPCUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	DBO_GP_UserInsureInfo * pUserInsureInfo=(DBO_GP_UserInsureInfo *)pData;

	//变量定义
	CMD_GP_UserInsureInfo UserInsureInfo;
	ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

	//构造数据
	UserInsureInfo.wRevenueTake=pUserInsureInfo->wRevenueTake;
	UserInsureInfo.wRevenueTransfer=pUserInsureInfo->wRevenueTransfer;
	UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
	UserInsureInfo.lUserScore=pUserInsureInfo->lUserScore;
	UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;
	UserInsureInfo.lTransferPrerequisite=pUserInsureInfo->lTransferPrerequisite;

	//发送数据
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_INFO,&UserInsureInfo,sizeof(UserInsureInfo));

	////关闭连接
	//m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//赠送信息
bool CAttemperEngineSink::OnDBPCUserInsureTransferInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_TRANSFER_INFO,pData,wDataSize);

	return true;
}

//银行成功
bool CAttemperEngineSink::OnDBPCUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	DBO_GP_UserInsureSuccess * pUserInsureSuccess=(DBO_GP_UserInsureSuccess *)pData;

	//变量定义
	CMD_GP_UserInsureSuccess UserInsureSuccess;
	ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

	//设置变量
	UserInsureSuccess.dwUserID=pUserInsureSuccess->dwUserID;
	UserInsureSuccess.lUserScore=pUserInsureSuccess->lSourceScore+pUserInsureSuccess->lVariationScore;
	UserInsureSuccess.lUserInsure=pUserInsureSuccess->lSourceInsure+pUserInsureSuccess->lVariationInsure;
	lstrcpyn(UserInsureSuccess.szDescribeString,pUserInsureSuccess->szDescribeString,CountArray(UserInsureSuccess.szDescribeString));

	//发送消息
	WORD wDescribe=CountStringBuffer(UserInsureSuccess.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_SUCCESS,&UserInsureSuccess,wHeadSize+wDescribe);

	return true;
}

//操作失败
bool CAttemperEngineSink::OnDBPCUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	CMD_GP_UserInsureFailure UserInsureFailure;
	ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

	//变量定义
	DBO_GP_UserInsureFailure * pUserInsureFailure=(DBO_GP_UserInsureFailure *)pData;

	//构造数据
	UserInsureFailure.lResultCode=pUserInsureFailure->lResultCode;
	lstrcpyn(UserInsureFailure.szDescribeString,pUserInsureFailure->szDescribeString,CountArray(UserInsureFailure.szDescribeString));

	//发送数据
	WORD wDescribe=CountStringBuffer(UserInsureFailure.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_INSURE_FAILURE,&UserInsureFailure,wHeadSize+wDescribe);

	////关闭连接
	//m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//用户信息
bool CAttemperEngineSink::OnDBPCUserInsureUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	DBO_GP_UserTransferUserInfo * pTransferUserInfo=(DBO_GP_UserTransferUserInfo *)pData;

	//变量定义
	CMD_GP_UserTransferUserInfo UserTransferUserInfo;
	ZeroMemory(&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	//构造变量
	UserTransferUserInfo.dwTargetGameID=pTransferUserInfo->dwGameID;
	lstrcpyn(UserTransferUserInfo.szNickName,pTransferUserInfo->szNickName,CountArray(UserTransferUserInfo.szNickName));

	//发送数据
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_USER_INFO_RESULT,&UserTransferUserInfo,sizeof(UserTransferUserInfo));

	return true;
}

//密码验证
bool CAttemperEngineSink::OnDBPCUserCheckPassword(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//发送数据
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_CHECK_PASSWORD_RETURN,pData,wDataSize);

	return true;
}

//操作成功
bool CAttemperEngineSink::OnDBPCOperateSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	CMD_GP_OperateSuccess OperateSuccess;
	ZeroMemory(&OperateSuccess,sizeof(OperateSuccess));

	//变量定义
	DBO_GP_OperateSuccess * pOperateSuccess=(DBO_GP_OperateSuccess *)pData;

	//构造数据
	OperateSuccess.lResultCode=pOperateSuccess->lResultCode;
	lstrcpyn(OperateSuccess.szDescribeString,pOperateSuccess->szDescribeString,CountArray(OperateSuccess.szDescribeString));

	//发送数据
	WORD wDescribe=CountStringBuffer(OperateSuccess.szDescribeString);
	WORD wHeadSize=sizeof(OperateSuccess)-sizeof(OperateSuccess.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_SUCCESS,&OperateSuccess,wHeadSize+wDescribe);

	////关闭连接
	//m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//操作失败
bool CAttemperEngineSink::OnDBPCOperateFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	CMD_GP_OperateFailure OperateFailure;
	ZeroMemory(&OperateFailure,sizeof(OperateFailure));

	//变量定义
	DBO_GP_OperateFailure * pOperateFailure=(DBO_GP_OperateFailure *)pData;

	//构造数据
	OperateFailure.lResultCode=pOperateFailure->lResultCode;
	lstrcpyn(OperateFailure.szDescribeString,pOperateFailure->szDescribeString,CountArray(OperateFailure.szDescribeString));

	//发送数据
	WORD wDescribe=CountStringBuffer(OperateFailure.szDescribeString);
	WORD wHeadSize=sizeof(OperateFailure)-sizeof(OperateFailure.szDescribeString);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_OPERATE_FAILURE,&OperateFailure,wHeadSize+wDescribe);

	//关闭连接
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//登录成功
bool CAttemperEngineSink::OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	DBO_MB_LogonSuccess * pDBOLogonSuccess=(DBO_MB_LogonSuccess *)pData;

	//变量定义
	CMD_MB_LogonSuccess LogonSuccess;
	ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

	//构造数据
	LogonSuccess.wFaceID=pDBOLogonSuccess->wFaceID;
	LogonSuccess.cbGender=pDBOLogonSuccess->cbGender;
	LogonSuccess.dwGameID=pDBOLogonSuccess->dwGameID;
	LogonSuccess.dwUserID=pDBOLogonSuccess->dwUserID;
	LogonSuccess.dwExperience=pDBOLogonSuccess->dwExperience;
	LogonSuccess.dwLoveLiness=pDBOLogonSuccess->dwLoveLiness;
	lstrcpyn(LogonSuccess.szNickName,pDBOLogonSuccess->szNickName,CountArray(LogonSuccess.szNickName));

	//登录成功
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON,SUB_MB_LOGON_SUCCESS,&LogonSuccess,sizeof(LogonSuccess));

	//发送房间
	WORD wIndex=LOWORD(dwContextID);
	SendMobileKindInfo(dwContextID,(m_pBindParameter+wIndex)->wModuleID);
	SendMobileServerInfo(dwContextID,(m_pBindParameter+wIndex)->wModuleID);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_SERVER_LIST,SUB_MB_LIST_FINISH);

	return true;
}

//登录失败
bool CAttemperEngineSink::OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
	CMD_MB_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));
	DBO_MB_LogonFailure * pLogonFailure=(DBO_MB_LogonFailure *)pData;

	//构造数据
	LogonFailure.lResultCode=pLogonFailure->lResultCode;
	lstrcpyn(LogonFailure.szDescribeString,pLogonFailure->szDescribeString,CountArray(LogonFailure.szDescribeString));

	//发送数据
	WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_MB_LOGON,SUB_MB_LOGON_FAILURE,&LogonFailure,wSendSize);

	//关闭连接
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	return true;
}

//游戏种类
bool CAttemperEngineSink::OnDBPCGameTypeItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize%sizeof(DBO_GP_GameType)==0);
	if (wDataSize%sizeof(DBO_GP_GameType)!=0) return false;

	//变量定义
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameType);
	DBO_GP_GameType * pGameType=(DBO_GP_GameType *)pData;

	//更新数据
	for (WORD i=0;i<wItemCount;i++)
	{
		//变量定义
		tagGameType GameType;
		ZeroMemory(&GameType,sizeof(GameType));

		//构造数据
		GameType.wTypeID=(pGameType+i)->wTypeID;
		GameType.wJoinID=(pGameType+i)->wJoinID;
		GameType.wSortID=(pGameType+i)->wSortID;
		lstrcpyn(GameType.szTypeName,(pGameType+i)->szTypeName,CountArray(GameType.szTypeName));

		//插入列表
		m_ServerListManager.InsertGameType(&GameType);
	}

	return true;
}

//游戏类型
bool CAttemperEngineSink::OnDBPCGameKindItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize%sizeof(DBO_GP_GameKind)==0);
	if (wDataSize%sizeof(DBO_GP_GameKind)!=0) return false;

	//变量定义
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameKind);
	DBO_GP_GameKind * pGameKind=(DBO_GP_GameKind *)pData;

	//更新数据
	for (WORD i=0;i<wItemCount;i++)
	{
		//变量定义
		tagGameKind GameKind;
		ZeroMemory(&GameKind,sizeof(GameKind));

		//构造数据
		GameKind.wTypeID=(pGameKind+i)->wTypeID;
		GameKind.wJoinID=(pGameKind+i)->wJoinID;
		GameKind.wSortID=(pGameKind+i)->wSortID;
		GameKind.wKindID=(pGameKind+i)->wKindID;
		GameKind.wGameID=(pGameKind+i)->wGameID;
		GameKind.dwOnLineCount=m_ServerListManager.CollectOnlineInfo((pGameKind+i)->wKindID);
		lstrcpyn(GameKind.szKindName,(pGameKind+i)->szKindName,CountArray(GameKind.szKindName));
		lstrcpyn(GameKind.szProcessName,(pGameKind+i)->szProcessName,CountArray(GameKind.szProcessName));

		//插入列表
		m_ServerListManager.InsertGameKind(&GameKind);
	}

	return true;
}

//游戏节点
bool CAttemperEngineSink::OnDBPCGameNodeItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize%sizeof(DBO_GP_GameNode)==0);
	if (wDataSize%sizeof(DBO_GP_GameNode)!=0) return false;

	//变量定义
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GameNode);
	DBO_GP_GameNode * pGameNode=(DBO_GP_GameNode *)pData;

	//更新数据
	for (WORD i=0;i<wItemCount;i++)
	{
		//变量定义
		tagGameNode GameNode;
		ZeroMemory(&GameNode,sizeof(GameNode));

		//构造数据
		GameNode.wKindID=(pGameNode+i)->wKindID;
		GameNode.wJoinID=(pGameNode+i)->wJoinID;
		GameNode.wSortID=(pGameNode+i)->wSortID;
		GameNode.wNodeID=(pGameNode+i)->wNodeID;
		lstrcpyn(GameNode.szNodeName,(pGameNode+i)->szNodeName,CountArray(GameNode.szNodeName));

		//插入列表
		m_ServerListManager.InsertGameNode(&GameNode);
	}

	return true;
}

//游戏定制
bool CAttemperEngineSink::OnDBPCGamePageItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize%sizeof(DBO_GP_GamePage)==0);
	if (wDataSize%sizeof(DBO_GP_GamePage)!=0) return false;

	//变量定义
	WORD wItemCount=wDataSize/sizeof(DBO_GP_GamePage);
	DBO_GP_GamePage * pGamePage=(DBO_GP_GamePage *)pData;

	//更新数据
	for (WORD i=0;i<wItemCount;i++)
	{
		//变量定义
		tagGamePage GamePage;
		ZeroMemory(&GamePage,sizeof(GamePage));

		//构造数据
		GamePage.wKindID=(pGamePage+i)->wKindID;
		GamePage.wNodeID=(pGamePage+i)->wNodeID;
		GamePage.wSortID=(pGamePage+i)->wSortID;
		GamePage.wPageID=(pGamePage+i)->wPageID;
		GamePage.wOperateType=(pGamePage+i)->wOperateType;
		lstrcpyn(GamePage.szDisplayName,(pGamePage+i)->szDisplayName,CountArray(GamePage.szDisplayName));

		//插入列表
		m_ServerListManager.InsertGamePage(&GamePage);
	}

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

//签到查询成功
bool CAttemperEngineSink::OnDBPCSignInQuerySuccess( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//效验参数
	ASSERT(wDataSize<=sizeof(DBO_GP_UserSignInInfo));
	if (wDataSize > sizeof(DBO_GP_UserSignInInfo)) return false;

	//变量定义
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_UserSignInInfo * pDBOUserSignInInfo=(DBO_GP_UserSignInInfo *)pData;
	CMD_GP_UserSigninInfo * pCMDUserSignInInfo=(CMD_GP_UserSigninInfo *)cbDataBuffer;

	//发送定义
	WORD wHeadSize=sizeof(CMD_GP_UserSigninInfo);

	//设置变量
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

	//签到查询成功
	WORD wSendSize=sizeof(CMD_GP_UserSigninInfo);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_SIGNIN_INFO_RESULT,cbDataBuffer,wSendSize);

	return true;
}

//签到查询失败
bool CAttemperEngineSink::OnDBPCSignInQueryFailure( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_SIGNIN_INFO_RESULT,cbDataBuffer,wSendSize);
	return true;
}

//用户签到成功
bool CAttemperEngineSink::OnDBPCUserSignInSuccess( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//效验参数
	ASSERT(wDataSize<=sizeof(DBO_GP_UserSignInResult));
	if (wDataSize > sizeof(DBO_GP_UserSignInResult)) return false;

	//变量定义
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_UserSignInResult * pDBOUserSignIn=(DBO_GP_UserSignInResult *)pData;
	CMD_GP_SignIn_Result * pCMDUserSignInRes=(CMD_GP_SignIn_Result *)cbDataBuffer;

	//设置变量
	ZeroMemory(pCMDUserSignInRes,sizeof(CMD_GP_SignIn_Result));

	pCMDUserSignInRes->bSuccess = pDBOUserSignIn->bSuccess;
	pCMDUserSignInRes->cbUserStatus = pDBOUserSignIn->cbUserStatus;
	pCMDUserSignInRes->llLotteryCount = pDBOUserSignIn->llLotteryCount;
	pCMDUserSignInRes->llScoreCount= pDBOUserSignIn->llScoreCount;
	lstrcpyn(pCMDUserSignInRes->szDescription,pDBOUserSignIn->szDescription,CountArray(pCMDUserSignInRes->szDescription));

	//发送结果
	WORD wDataSizeSend=sizeof(CMD_GP_SignIn_Result);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_SIGNIN_RESULT,cbDataBuffer,wDataSizeSend);

	return true;
}

//用户签到失败
bool CAttemperEngineSink::OnDBPCUserSignInFailure( DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize<=sizeof(DBO_GP_UserSignInResult));
	if (wDataSize > sizeof(DBO_GP_UserSignInResult)) return false;

	//变量定义
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_UserSignInResult * pDBOUserSignIn=(DBO_GP_UserSignInResult *)pData;
	CMD_GP_SignIn_Result * pCMDUserSignInRes=(CMD_GP_SignIn_Result *)cbDataBuffer;

	//设置变量
	ZeroMemory(pCMDUserSignInRes,sizeof(CMD_GP_SignIn_Result));

	pCMDUserSignInRes->bSuccess = pDBOUserSignIn->bSuccess;
	pCMDUserSignInRes->llLotteryCount = pDBOUserSignIn->llLotteryCount;
	pCMDUserSignInRes->llScoreCount = pDBOUserSignIn->llScoreCount;
	lstrcpyn(pCMDUserSignInRes->szDescription,pDBOUserSignIn->szDescription,CountArray(pCMDUserSignInRes->szDescription));

	WORD wDataSizeSend=sizeof(CMD_GP_SignIn_Result);
	printf("%s", pCMDUserSignInRes->szDescription);
	printf("the size of the data is %ld", wDataSizeSend);
	//用户签到失败
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_USER_SIGNIN_RESULT,cbDataBuffer,wDataSizeSend);

	return true;
}

//抽奖查询结果
bool CAttemperEngineSink::OnDBPCLotteryQueryResult( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//效验参数
	ASSERT(wDataSize<=sizeof(DBO_GP_Ltry_Qry_Res));
	if (wDataSize > sizeof(DBO_GP_Ltry_Qry_Res)) return false;

	//变量定义
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];
	DBO_GP_Ltry_Qry_Res * pDBOLtryQryRes=(DBO_GP_Ltry_Qry_Res *)pData;
	CMD_GP_QryLtryInfo_Res * pCMDLtryQryRes=(CMD_GP_QryLtryInfo_Res *)cbDataBuffer;

	//发送定义
	WORD wHeadSize=sizeof(CMD_GP_QryLtryInfo);
	CSendPacketHelper SendPacket(cbDataBuffer+wHeadSize,sizeof(cbDataBuffer)-wHeadSize);

	//设置变量
	ZeroMemory(pCMDLtryQryRes,sizeof(CMD_GP_QryLtryInfo_Res));
	pCMDLtryQryRes->wFreeCount=pDBOLtryQryRes->wFreeCount;

	//抽奖查询成功
	WORD wSendSize=SendPacket.GetDataSize()+sizeof(CMD_GP_QryLtryInfo_Res);
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GP_QUERY_LOTTERY_INFO_RESULT,cbDataBuffer,wSendSize);
	return true;
}

//用户抽奖结果
bool CAttemperEngineSink::OnDBPCUserLotteryResult( DWORD dwContextID, VOID * pData, WORD wDataSize )
{
	//效验参数
	ASSERT(wDataSize<=sizeof(DBO_GP_User_Ltry_Res));
	if (wDataSize > sizeof(DBO_GP_User_Ltry_Res)) return false;

	//变量定义
	BYTE cbDataBuffer_LtryRes[SOCKET_TCP_PACKET];
	BYTE cbDataBuffer_LtryMsg[SOCKET_TCP_PACKET];
	DBO_GP_User_Ltry_Res * pDBOUserLtry=(DBO_GP_User_Ltry_Res *)pData;
	CMD_GP_UserLtry_Res * pCMDUserLtryRes=(CMD_GP_UserLtry_Res *)cbDataBuffer_LtryRes;

	//发送定义
	WORD wHeadSize=sizeof(CMD_GP_UserLtry_Res);
	CSendPacketHelper SendPacket(cbDataBuffer_LtryRes+wHeadSize,sizeof(cbDataBuffer_LtryRes)-wHeadSize);

	//设置变量
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
		//变量定义
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

//邮件ID列表
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

//邮件信息
bool CAttemperEngineSink::OnDBMailInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//char * pBuffer = (char *)pData;
	//DWORD * pCount = (DWORD *)pBuffer;
	//DBO_MailInfo * pMailInfo = (DBO_MailInfo *)&pBuffer[4];

	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_QUERY_MAIL_INFO, pData, wDataSize);

	return true;
}

//邮件内容
bool CAttemperEngineSink::OnDBMailContent(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_QUERY_MAIL_CONTENT, pData, wDataSize);

	return true;
}

//邮件物品
bool CAttemperEngineSink::OnDBMailItem(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	DBO_MailItem * MailItem =(DBO_MailItem *)pData;
	BYTE cbResult= MailItem->byResult;
	printf("领取结果%d\n",cbResult);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_QUERY_MAIL_ITEM, MailItem, wDataSize);

	return true;
}

//充值到账
bool CAttemperEngineSink::OnDBChargeValue(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	DBO_ChargeValue * pChargeValue = (DBO_ChargeValue *)pData;

	CMD_GPO_ChargeValue ChargeValue = {0};
	ChargeValue.lChargeValue = pChargeValue->lChargeValue;

	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GPO_CHARGE_VALUE,&ChargeValue,sizeof(CMD_GPO_ChargeValue));

	return true;
}

//VIP信息
bool CAttemperEngineSink::OnDBVIPInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_VIP_INFO, pData, wDataSize);
	return true;
}

//VIP处理结果
bool CAttemperEngineSink::OnDBVIPResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_VIP_RESULT, pData, wDataSize);
	return true;
}

//当前分数
bool CAttemperEngineSink::OnDBCurrentScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_CURRENT_SCORE, pData, wDataSize);
	return true;
}

//登录奖励
bool CAttemperEngineSink::OnDBLogonGift(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_LOGON_GIFT, pData, wDataSize);
	return true;
}

//首充成功
bool CAttemperEngineSink::OnDBCharge1stRequest(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_CHARGE1ST_REQUEST, pData, wDataSize);
	return true;
}

//领取首充礼包奖励
bool CAttemperEngineSink::OnDBCharge1stGet(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GPO_CHARGE1ST_GET, pData, wDataSize);
	return true;
}

//版本检测
bool CAttemperEngineSink::CheckPlazaVersion(BYTE cbDeviceType, DWORD dwPlazaVersion, DWORD dwSocketID, bool bCheckLowVer)
{
	//变量定义
	bool bMustUpdate=false;
	bool bAdviceUpdate=false;
	DWORD dwVersion=VERSION_PLAZA;

	//手机版本
	if(cbDeviceType >= DEVICE_TYPE_IPAD) dwVersion=VERSION_MOBILE_IOS;
	else if(cbDeviceType >= DEVICE_TYPE_IPHONE) dwVersion=VERSION_MOBILE_IOS;
	else if(cbDeviceType >= DEVICE_TYPE_ITOUCH) dwVersion=VERSION_MOBILE_IOS;
	else if(cbDeviceType >= DEVICE_TYPE_ANDROID) dwVersion=VERSION_MOBILE_ANDROID;
	else if(cbDeviceType == DEVICE_TYPE_PC) dwVersion=VERSION_PLAZA;

	//版本判断
	if (bCheckLowVer && GetSubVer(dwPlazaVersion)<GetSubVer(dwVersion)) bAdviceUpdate=true;
	if (GetMainVer(dwPlazaVersion)!=GetMainVer(dwVersion)) bMustUpdate=true;
	if (GetProductVer(dwPlazaVersion)!=GetProductVer(dwVersion)) bMustUpdate=true;

	//升级判断
	if ((bMustUpdate==true)||(bAdviceUpdate==true))
	{
		//变量定义
		CMD_GP_UpdateNotify UpdateNotify;
		ZeroMemory(&UpdateNotify,sizeof(UpdateNotify));

		//变量定义
		UpdateNotify.cbMustUpdate=bMustUpdate;
		UpdateNotify.cbAdviceUpdate=bAdviceUpdate;
		UpdateNotify.dwCurrentVersion=dwVersion;

		//发送消息
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_UPDATE_NOTIFY,&UpdateNotify,sizeof(UpdateNotify));

		//中断判断
		if (bMustUpdate==true) 
		{
			m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
			return false;
		}
	}

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

//发送类型
VOID CAttemperEngineSink::SendGameTypeInfo(DWORD dwSocketID)
{
	//网络数据
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position=NULL;
	CGameTypeItem * pGameTypeItem=NULL;

	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGameTypeCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGameType))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_TYPE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGameTypeItem=m_ServerListManager.EmunGameTypeItem(Position);
		if (pGameTypeItem==NULL) break;

		//拷贝数据
		CopyMemory(cbDataBuffer+wSendSize,&pGameTypeItem->m_GameType,sizeof(tagGameType));
		wSendSize+=sizeof(tagGameType);
	}

	//发送剩余
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_TYPE,cbDataBuffer,wSendSize);

	return;
}

//发送种类
VOID CAttemperEngineSink::SendGameKindInfo(DWORD dwSocketID)
{
	//网络数据
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position=NULL;
	CGameKindItem * pGameKindItem=NULL;

	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGameKindCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGameKind))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_KIND,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGameKindItem=m_ServerListManager.EmunGameKindItem(Position);
		if (pGameKindItem==NULL) break;

		//拷贝数据
		CopyMemory(cbDataBuffer+wSendSize,&pGameKindItem->m_GameKind,sizeof(tagGameKind));
		wSendSize+=sizeof(tagGameKind);
	}

	//发送剩余
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_KIND,cbDataBuffer,wSendSize);

	return;
}

//发送节点
VOID CAttemperEngineSink::SendGameNodeInfo(DWORD dwSocketID, WORD wKindID)
{
	//网络数据
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position=NULL;
	CGameNodeItem * pGameNodeItem=NULL;

	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGameNodeCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGameNode))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_NODE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGameNodeItem=m_ServerListManager.EmunGameNodeItem(Position);
		if (pGameNodeItem==NULL) break;

		//拷贝数据
		if ((wKindID==INVALID_WORD)||(pGameNodeItem->m_GameNode.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameNodeItem->m_GameNode,sizeof(tagGameNode));
			wSendSize+=sizeof(tagGameNode);
		}
	}

	//发送剩余
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_NODE,cbDataBuffer,wSendSize);

	return;
}

//发送定制
VOID CAttemperEngineSink::SendGamePageInfo(DWORD dwSocketID, WORD wKindID)
{
	//网络数据
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position=NULL;
	CGamePageItem * pGamePageItem=NULL;

	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGamePageCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGamePage))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_PAGE,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGamePageItem=m_ServerListManager.EmunGamePageItem(Position);
		if (pGamePageItem==NULL) break;

		//拷贝数据
		if ((wKindID==INVALID_WORD)||(pGamePageItem->m_GamePage.wKindID==wKindID))
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGamePageItem->m_GamePage,sizeof(tagGamePage));
			wSendSize+=sizeof(tagGamePage);
		}
	}

	//发送剩余
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_PAGE,cbDataBuffer,wSendSize);

	return;
}

//发送房间
VOID CAttemperEngineSink::SendGameServerInfo(DWORD dwSocketID, WORD wKindID)
{
	//网络数据
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;

	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGameServer))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SERVER,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;

		//拷贝数据
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

	//发送剩余
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SERVER,cbDataBuffer,wSendSize);

	if(m_wAVServerPort!=0 && m_dwAVServerAddr!=0)
	{
		//变量定义
		tagAVServerOption AVServerOption;
		AVServerOption.wAVServerPort=m_wAVServerPort;
		AVServerOption.dwAVServerAddr=m_dwAVServerAddr;

		//发送配置
		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_VIDEO_OPTION,&AVServerOption,sizeof(AVServerOption));
	};
	
	return;
}

//发送房间
VOID CAttemperEngineSink::SendGameServerInfoEx(DWORD dwSocketID, WORD wKindID)
{
	//网络数据
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;

	//枚举数据
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
		//获取数据
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;
		//拷贝数据
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

	//发送剩余
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SERVER_LIST,SUB_GP_LIST_SMALL_SERVER,cbDataBuffer,wSendSize);

	return;
}

//发送类型
VOID CAttemperEngineSink::SendMobileKindInfo(DWORD dwSocketID, WORD wModuleID)
{
	return;
}

//发送房间
VOID CAttemperEngineSink::SendMobileServerInfo(DWORD dwSocketID, WORD wModuleID)
{
	//网络数据
	WORD wSendSize=0;
	BYTE cbDataBuffer[SOCKET_TCP_PACKET];

	//枚举数据
	POSITION Position=NULL;
	CGameServerItem * pGameServerItem=NULL;

	//枚举数据
	for (DWORD i=0;i<m_ServerListManager.GetGameServerCount();i++)
	{
		//发送数据
		if ((wSendSize+sizeof(tagGameServer))>sizeof(cbDataBuffer))
		{
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER,cbDataBuffer,wSendSize);
			wSendSize=0;
		}

		//获取数据
		pGameServerItem=m_ServerListManager.EmunGameServerItem(Position);
		if (pGameServerItem==NULL) break;

		//拷贝数据
		if (pGameServerItem->m_GameServer.wKindID==wModuleID)
		{
			CopyMemory(cbDataBuffer+wSendSize,&pGameServerItem->m_GameServer,sizeof(tagGameServer));
			wSendSize+=sizeof(tagGameServer);
		}
	}

	//发送剩余
	if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_MB_SERVER_LIST,SUB_MB_LIST_SERVER,cbDataBuffer,wSendSize);

	return;
}

//道具处理
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

			LPCTSTR pszItemName[] = { TEXT("初级珍珠"), TEXT("中级珍珠"), TEXT("高级珍珠"), TEXT("至尊珍珠") };
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

			//只能合成小龙珠或者大龙珠
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

//技能处理
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

// 手机验证码处理
bool CAttemperEngineSink::OnTCPNetworkSubPCMBCODEVERIFY( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//CTraceService::TraceString(TEXT("sms: receive success"),TraceLevel_Warning);

	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_GPR_MBVerificationCode));
	if (wDataSize<sizeof(CMD_GPR_MBVerificationCode))
	{
		if (wDataSize<sizeof(CMD_GPR_MBVerificationCode)-sizeof(BYTE))
			return false;
	}

	CMD_GPR_MBVerificationCode * pMBVerificationCode = (CMD_GPR_MBVerificationCode *)pData;

	//变量定义
	DBR_GP_MBVerifCode MBVerifiCode;
	ZeroMemory(&MBVerifiCode,sizeof(DBR_GP_MBVerifCode));

	//构造数据 
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
		//如果发送过，超出60秒后重新发送
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
			lstrcpyn(pCMDMBVerifyRes->szDescribeString,TEXT("请不要频繁请求验证码!"),CountArray(pCMDMBVerifyRes->szDescribeString));
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GPO_MBCODE_VERIFY,cbDataBuffer,wSendSize);
			return true;
		}
	}


	if (bIsVerifiCodeSent == false)
	{
		//判断手机号是否注册过
		std::map<std::string,int>::iterator iter = m_mRegisteredMbNum.find(strMobilePhone);
		if(iter==m_mRegisteredMbNum.end())
		{
			tagMapMBVerifInfo MBVerifInfoVecEle;
			ZeroMemory(&MBVerifInfoVecEle,sizeof(tagMapMBVerifInfo));
			MBVerifInfoVecEle.m_dwVerifCodeSendTime = time(0);
			lstrcpyn(MBVerifInfoVecEle.szVerificationCode,szVerificationCode,CountArray(MBVerifInfoVecEle.szVerificationCode));
			m_mMBVerifInfo.insert(std::pair<std::string,tagMapMBVerifInfo>(strMobilePhone,MBVerifInfoVecEle));
			//增加新处理过程

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
			lstrcpyn(pCMDMBVerifyRes->szDescribeString,TEXT("手机号已经注册"),CountArray(pCMDMBVerifyRes->szDescribeString));

			//手机号已经注册
			WORD wSendSize=sizeof(CMD_GPO_MBVerificationCode);
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GPO_MBCODE_VERIFY,cbDataBuffer,wSendSize);

		}

	}
	return true;
}

bool CAttemperEngineSink::OnDBPCVerifyMBNumRes(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//CTraceService::TraceString(TEXT("sms: OnDBPCVerifyMBNumRes"),TraceLevel_Warning);

	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
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
		//该用户已经绑定手机号
		BYTE cbDataBuffer[SOCKET_TCP_PACKET];
		CMD_GPO_MBVerificationCode * pCMDMBVerifyRes=(CMD_GPO_MBVerificationCode *)cbDataBuffer;
		WORD wSendSize=sizeof(CMD_GPO_MBVerificationCode);
		pCMDMBVerifyRes->bIsVerificationCodeSend = false;
		lstrcpyn(pCMDMBVerifyRes->szDescribeString,TEXT("该用户已经绑定手机号!"),CountArray(pCMDMBVerifyRes->szDescribeString));
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GPO_MBCODE_VERIFY,cbDataBuffer,wSendSize);
	}
	else if(pDBOMBVerifCode->byResult == 2)
	{
		//该手机号已经绑定
		BYTE cbDataBuffer[SOCKET_TCP_PACKET];
		CMD_GPO_MBVerificationCode * pCMDMBVerifyRes=(CMD_GPO_MBVerificationCode *)cbDataBuffer;
		WORD wSendSize=sizeof(CMD_GPO_MBVerificationCode);
		pCMDMBVerifyRes->bIsVerificationCodeSend = false;
		lstrcpyn(pCMDMBVerifyRes->szDescribeString,TEXT("该手机号已经绑定!"),CountArray(pCMDMBVerifyRes->szDescribeString));
		m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_USER_SERVICE,SUB_GPO_MBCODE_VERIFY,cbDataBuffer,wSendSize);
	}
	return true;
}

// 手机号注册成功处理
bool CAttemperEngineSink::OnDBMBNumRegisterSuccessRes(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	ASSERT(LOWORD(dwContextID)<m_pInitParameter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//变量定义
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

//手机号注册
bool CAttemperEngineSink::OnTCPNetworkSubPCMBNumRegister( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_GP_MBNumRegister));
	if (wDataSize<sizeof(CMD_GP_MBNumRegister))
	{
		if (wDataSize<sizeof(CMD_GP_MBNumRegister)-sizeof(BYTE))
			return false;
	}

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_GP_MBNumRegister * pMBNumRegisterAccounts=(CMD_GP_MBNumRegister *)pData;
	bool bIsVerificationCodeCorrect = false;
	pMBNumRegisterAccounts->RegisterAccounts.szMobilePhone[CountArray(pMBNumRegisterAccounts->RegisterAccounts.szMobilePhone)-1]=0;
	pMBNumRegisterAccounts->szVerificationCode[CountArray(pMBNumRegisterAccounts->szVerificationCode)-1]=0;

	// 验证码验证
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
		//变量定义
		BYTE cbDataBuffer[SOCKET_TCP_PACKET];
		CMD_GPO_MBVerificationCode * pCMDMBVerifyRes=(CMD_GPO_MBVerificationCode *)cbDataBuffer;
		WORD wSendSize=sizeof(CMD_GPO_MBVerificationCode);

		pCMDMBVerifyRes->bIsVerificationCodeSend = false;
		lstrcpyn(pCMDMBVerifyRes->szDescribeString,TEXT("验证码验证错误或者验证码已经过期!"),CountArray(pCMDMBVerifyRes->szDescribeString));
		printf("error in verification code verify");

		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GPO_MBCODE_VERIFY,cbDataBuffer,wSendSize);
		return true;
	}

	VOID * pNewData = (VOID*)(&pMBNumRegisterAccounts->RegisterAccounts);
	WORD wNewDataSize = wDataSize - sizeof(pMBNumRegisterAccounts->szVerificationCode);
	//OnTCPNetworkSubPCRegisterAccounts(pNewData,wNewDataSize,dwSocketID);

	//处理消息
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
	//敏感词
	CString sNickName = pRegisterAccounts->szNickName;
	for (int i = 0; i < (sizeof(pszSensitiveWords)/sizeof(pszSensitiveWords[0])); i++)
	{
		if (sNickName.Find(pszSensitiveWords[i]) != -1)
		{
			printf("sensitive words %s %s %d\n", sNickName, pszSensitiveWords[i], i);
			CMD_GP_LogonFailure LogonFailure = {0};

			LogonFailure.lResultCode = 0;
			lstrcpyn(LogonFailure.szDescribeString,TEXT("您的昵称中包含敏感词汇，注册失败！"),CountArray(LogonFailure.szDescribeString));

			WORD wStringSize=CountStringBuffer(LogonFailure.szDescribeString);
			WORD wSendSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString)+wStringSize;
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_LOGON,SUB_GP_LOGON_FAILURE,&LogonFailure,wSendSize);

			return true;
		}
	}
	//--------

	//设置连接
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pRegisterAccounts->dwPlazaVersion;

	//效验版本
	if (CheckPlazaVersion(DEVICE_TYPE_PC,pRegisterAccounts->dwPlazaVersion,dwSocketID,((pRegisterAccounts->cbValidateFlags&LOW_VER_VALIDATE_FLAGS)!=0))==false)
	{
		return true;
	}

	//变量定义
	DBR_GP_RegisterAccounts RegisterAccounts;
	ZeroMemory(&RegisterAccounts,sizeof(RegisterAccounts));

	//附加信息
	RegisterAccounts.pBindParameter=(m_pBindParameter+LOWORD(dwSocketID));

	//构造数据
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

	//扩展信息
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pRegisterAccounts+1,wDataSize-sizeof(CMD_GP_RegisterAccounts));
	while (true)
	{
		//提取数据
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;

		char szBuffer[2048] = {0};
		//提取数据
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_REG_AGENTID:	//推荐人信息
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
	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MB_REGITER,dwSocketID,&RegisterAccounts,sizeof(RegisterAccounts));
	return true;
}

//生成六位随机数
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



// 手机绑定验证码请求处理
bool CAttemperEngineSink::OnTCPNetworkSubPCMoBileCode( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//CTraceService::TraceString(TEXT("sms: receive success"),TraceLevel_Warning);

	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_GPR_MB_BIND_REQUEST));
	if (wDataSize<sizeof(CMD_GPR_MB_BIND_REQUEST))
	{
		if (wDataSize<sizeof(CMD_GPR_MB_BIND_REQUEST)-sizeof(BYTE))
			return false;
	}

	CMD_GPR_MB_BIND_REQUEST * pMBVerificationCode = (CMD_GPR_MB_BIND_REQUEST *)pData;

	//变量定义
	DBR_GP_MBVerifCode MBVerifiCode;
	ZeroMemory(&MBVerifiCode,sizeof(DBR_GP_MBVerifCode));

	//构造数据 
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
		//如果发送过，超出60秒后重新发送
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
			lstrcpyn(pCMDMBVerifyRes->szDescribeString,TEXT("请不要频繁请求验证码!"),CountArray(pCMDMBVerifyRes->szDescribeString));
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GPO_MBCODE_VERIFY,cbDataBuffer,wSendSize);
			return true;
		}
	}


	if (bIsVerifiCodeSent == false)
	{
		//判断手机号是否绑定过
		tagMapMBVerifInfo MBVerifInfoVecEle;
		ZeroMemory(&MBVerifInfoVecEle,sizeof(tagMapMBVerifInfo));
		MBVerifInfoVecEle.m_dwVerifCodeSendTime = time(0);
		lstrcpyn(MBVerifInfoVecEle.szVerificationCode,szVerificationCode,CountArray(MBVerifInfoVecEle.szVerificationCode));
		m_mMBVerifInfo.insert(std::pair<std::string,tagMapMBVerifInfo>(strMobilePhone,MBVerifInfoVecEle));

		//投递请求
		m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_VERIFY_MBNUM,dwSocketID,&MBVerifiCode,sizeof(MBVerifiCode));

	}
	return true;
}

//手机号绑定验证码验证
bool CAttemperEngineSink::OnTCPNetworkSubPCCodeVerification( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_GP_MobileCaptchaVerific));
	if (wDataSize<sizeof(CMD_GP_MobileCaptchaVerific))
	{
		if (wDataSize<sizeof(CMD_GP_MobileCaptchaVerific)-sizeof(BYTE))
			return false;
	}

	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//处理消息
	CMD_GP_MobileCaptchaVerific * pMBCaptchaVerific=(CMD_GP_MobileCaptchaVerific *)pData;
	bool bIsVerificationCodeCorrect = false;
	pMBCaptchaVerific->szMobilePhone[CountArray(pMBCaptchaVerific->szMobilePhone)-1]=0;
	pMBCaptchaVerific->szVerificationCode[CountArray(pMBCaptchaVerific->szVerificationCode)-1]=0;
	pMBCaptchaVerific->szLogonPass[CountArray(pMBCaptchaVerific->szLogonPass)-1]=0;

	// 验证码验证
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
		//变量定义
		BYTE cbDataBuffer[SOCKET_TCP_PACKET];
		CMD_GPO_MBVerificationCode * pCMDMBVerifyRes=(CMD_GPO_MBVerificationCode *)cbDataBuffer;
		WORD wSendSize=sizeof(CMD_GPO_MBVerificationCode);

		pCMDMBVerifyRes->bIsVerificationCodeSend = false;
		lstrcpyn(pCMDMBVerifyRes->szDescribeString,TEXT("验证码验证错误或者验证码已经过期!"),CountArray(pCMDMBVerifyRes->szDescribeString));

		m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_USER_SERVICE,SUB_GPO_MBCODE_VERIFY,cbDataBuffer,wSendSize);
		return true;
	}
	DBR_GP_MBBindNum sMbBindNum;
	VOID * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pMBCaptchaVerific+1,wDataSize-sizeof(CMD_GP_MobileCaptchaVerific));
	lstrcpyn(sMbBindNum.szNickName,"",LEN_NICKNAME);
	//扩展信息
	while (true)
	{
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL) break;
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_GP_UI_NICKNAME:		//用户昵称
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
	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MB_BIND_NUM,dwSocketID,&sMbBindNum,sizeof(sMbBindNum));
	if(iter!=m_mMBVerifInfo.end())
	{
		m_mMBVerifInfo.erase(iter);
	}

	return true;
}

// 查询手机号是否绑定过
bool CAttemperEngineSink::OnTCPNetworkSubPCMobileCheckExit( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//CTraceService::TraceString(TEXT("sms: receive success"),TraceLevel_Warning);

	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_GPR_NumCheckExit));
	if (wDataSize<sizeof(CMD_GPR_NumCheckExit))
	{
		if (wDataSize<sizeof(CMD_GPR_NumCheckExit)-sizeof(BYTE))
			return false;
	}

	CMD_GPR_NumCheckExit * pMBCheckExit = (CMD_GPR_NumCheckExit*)pData;

	//变量定义
	DBR_GP_CHECK_BINDNUM_EXIT BindNum_Exit;
	ZeroMemory(&BindNum_Exit,sizeof(BindNum_Exit));

	BindNum_Exit.dwUserID = pMBCheckExit->dwUserID;

	//投递请求
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MB_CHECK_NUM_EXIT,dwSocketID,&BindNum_Exit,sizeof(BindNum_Exit));

	return true;
}

//广播速度
bool CAttemperEngineSink::OnTCPNetworkSubPCBroadCastSpeed( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//投递请求
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
	//投递请求
	//效验参数
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
	//效验参数
	ASSERT(wDataSize%sizeof(DB0_GP_COMPRANK_INFO_RES)==0);
	if (wDataSize%sizeof(DB0_GP_COMPRANK_INFO_RES)!=0) return false;

	//变量定义
	WORD wItemCount=wDataSize/sizeof(DB0_GP_COMPRANK_INFO_RES);
	DB0_GP_COMPRANK_INFO_RES * pInfo=(DB0_GP_COMPRANK_INFO_RES *)pData;
	m_CompRankInfo.clear();
	//更新数据
	for (WORD i=0;i<wItemCount;i++)
	{
		//变量定义
		tagCompRankInfo sCompRankInfo;
		ZeroMemory(&sCompRankInfo,sizeof(sCompRankInfo));

		//构造数据
		sCompRankInfo.nRank=(pInfo+i)->nRank;
		sCompRankInfo.nUserId=(pInfo+i)->nUserId;
		sCompRankInfo.nCount=(pInfo+i)->nCount;
		m_CompRankInfo.push_back(sCompRankInfo);
	}

	return true;
}

bool CAttemperEngineSink::OnDBBroadCastResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
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
	//判断在线
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
	//判断在线
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
		_snprintf(VIPUpgrade.szTrumpetContent, sizeof(VIPUpgrade.szTrumpetContent), "#ffffff恭喜【#ff0000%s#ffffff】成功的兑换了#ff0000%d元话费#ffffff！", pDBChangeResult->szNickName, pDBChangeResult->nChangeNum);
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
	//效验参数
	ASSERT(wDataSize%sizeof(DB0_GP_SUPERRANK_INFO_RES)==0);
	if (wDataSize%sizeof(DB0_GP_SUPERRANK_INFO_RES)!=0) return false;

	//变量定义
	WORD wItemCount=wDataSize/sizeof(DB0_GP_SUPERRANK_INFO_RES);
	DB0_GP_SUPERRANK_INFO_RES * pInfo=(DB0_GP_SUPERRANK_INFO_RES *)pData;
	m_SuperRankInfo.clear();
	//更新数据
	for (WORD i=0;i<wItemCount;i++)
	{
		//变量定义
		tagSuperRankInfo sRankInfo;
		ZeroMemory(&sRankInfo,sizeof(sRankInfo));

		//构造数据
		sRankInfo.nRank=(pInfo+i)->nRank;
		lstrcpyn(sRankInfo.szNickName,(pInfo+i)->szNickName,CountArray(sRankInfo.szNickName));
		sRankInfo.lCount=(pInfo+i)->lCount;
		m_SuperRankInfo.push_back(sRankInfo);
	}

	return true;
}

bool CAttemperEngineSink::OnTCPNetworkGetBindRewardNewGuid( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//投递请求
	ASSERT(wDataSize>=sizeof(CMD_GPR_BindRewardNewQuid));
	if (wDataSize<sizeof(CMD_GPR_BindRewardNewQuid))
	{
			return false;
	}

	CMD_GPR_BindRewardNewQuid * pRequest = (CMD_GPR_BindRewardNewQuid*)pData;

	//变量定义
	DBR_GP_GET_BINDREWARD sGetBindReward;
	ZeroMemory(&sGetBindReward,sizeof(sGetBindReward));

	sGetBindReward.dwUserID = pRequest->dwUserID;
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GETBINDREWARDNEWGUID,dwSocketID,&sGetBindReward,sizeof(sGetBindReward));

	return true;
}

bool CAttemperEngineSink::OnTCPNetworkSetSuperNewGuid( VOID * pData, WORD wDataSize, DWORD dwSocketID )
{
	//投递请求
	ASSERT(wDataSize>=sizeof(CMD_GPR_SetSuperNewQuid));
	if (wDataSize!=sizeof(CMD_GPR_SetSuperNewQuid))
	{
		return false;
	}

	CMD_GPR_SetSuperNewQuid * pRequest = (CMD_GPR_SetSuperNewQuid*)pData;

	//变量定义
	DBR_GP_Set_SuperNewGuid sSetSuperNewGuid;
	ZeroMemory(&sSetSuperNewGuid,sizeof(sSetSuperNewGuid));

	sSetSuperNewGuid.dwUserID = pRequest->dwUserID;
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_SETSUPERNEWGUID,dwSocketID,&sSetSuperNewGuid,sizeof(sSetSuperNewGuid));

	return true;
}

bool CAttemperEngineSink::ReadSignInRewardConfig()
{
	//获取路径
	TCHAR szWorkDir[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDir,CountArray(szWorkDir));

	//构造路径
	TCHAR szSignRewardIniFile[MAX_PATH]=TEXT("");
	_sntprintf(szSignRewardIniFile,CountArray(szSignRewardIniFile),TEXT("%s\\SignReward.ini"),szWorkDir);

	//读取配置
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
	//效验参数
	ASSERT(wDataSize%sizeof(DBO_GP_NewMail_Remind_Res)==0);
	if (wDataSize%sizeof(DBO_GP_NewMail_Remind_Res)!=0) return false;

	//变量定义
	WORD wItemCount=wDataSize/sizeof(DBO_GP_NewMail_Remind_Res);
	DBO_GP_NewMail_Remind_Res * pInfo=(DBO_GP_NewMail_Remind_Res *)pData;
	//更新数据
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
	//效验参数
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
	//效验参数
	ASSERT(wDataSize%sizeof(DBO_GR_Match_Get_Prize)==0);
	if (wDataSize%sizeof(DBO_GR_Match_Get_Prize)!=0) return false;

	//变量定义
	DBO_GR_Match_Get_Prize * pDBMatchResult=(DBO_GR_Match_Get_Prize *)pData;
	CMD_GPO_MatchGetMatchPriseRes stMatchGetMatchPrizeRes = {0};
	stMatchGetMatchPrizeRes.bPriseStatus = pDBMatchResult->bPriseStatus;
	stMatchGetMatchPrizeRes.nPriseType = pDBMatchResult->nPriseType;
	stMatchGetMatchPrizeRes.nPriseCount = pDBMatchResult->nPriseCount;
	if (pDBMatchResult->bPriseStatus==true)
	{
		char szText[128];
		sprintf(szText,"领取成功，获得%d金币。",pDBMatchResult->nPriseCount);
		lstrcpyn(stMatchGetMatchPrizeRes.szDescription,szText,sizeof(stMatchGetMatchPrizeRes.szDescription));
	}
	else
	{
		char szText[128];
		sprintf(szText,"奖励已经领取",pDBMatchResult->nPriseCount);
		lstrcpyn(stMatchGetMatchPrizeRes.szDescription,szText,sizeof(stMatchGetMatchPrizeRes.szDescription));
	}
	//m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER_SERVICE, SUB_GR_MATCH_GET_MATCH_PRIZE_RES, &stMatchGetMatchPrizeRes, sizeof(CMD_GPO_MatchGetMatchPriseRes));
	return true;
}

