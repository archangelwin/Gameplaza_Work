#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////////////

//时间调度
#define IDI_GET_PARAMETER_VALUE     1                                   //隔一段时间读取参数

LONGLONG	CTableFrameSink::m_nProbability[10];									//随机三个图案的概率
LONGLONG	CTableFrameSink::m_nMaxThresholdNum;							        //平台算法最大阈值
int			CTableFrameSink::m_nClownProbability[8];						        //猜对小丑大小次数对应的的概率
int			CTableFrameSink::m_nRewardTimes[10];
int			CTableFrameSink::m_nExtralReward[4];									//额外奖励
int			CTableFrameSink::m_nMinRandKNum;
int			CTableFrameSink::m_nMaxRandKNum;
int			CTableFrameSink::m_nDecProbability[3];								//猜大小时下注倍数减小的概率数
int			CTableFrameSink::m_nBandScoreBoundary[5];							//默认押注的分数段
int			CTableFrameSink::m_nBandScore[5];									//默认押注的分数段对应的分数值
int			CTableFrameSink::m_nBigRewardBand;									//获得巨奖公告的最大值
int			CTableFrameSink::m_nTreasureMapValue;								//藏宝图值多少金币
CString		CTableFrameSink::m_strLogDir;										//日志目录
int			CTableFrameSink::m_nMinScoreCanPlay;								//最少可以玩游戏的分数
//构造函数
CTableFrameSink::CTableFrameSink()
{
AllocConsole();
freopen("CONOUT$","w+t",stdout);

	srand((unsigned)time(NULL));
	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameCustomRule=NULL;
	
	m_nAvailableTimes = 999;
	m_nTimes = 1;
	m_nClownIndex = 0;
	m_nPrizeTimes = 0;
	m_nStarCount = 0;
	m_lBeginGuessPirateScore = 0;
	m_lDefultChip = 0;
	m_lThreeRollChip = 0;
	m_lCurrentChip = 0;
	m_lChipPrize = 0;
	m_lFinalPrize = 0;
	m_nYear = 2014;
	m_nMonth = 11;
	m_nDay = 1;
	m_bIsRolling = false;
	m_bIsSelected = false;
	m_bLackChip = false;
	m_wChipUser = INVALID_CHAIR;
	m_miniRevenue = 0;
	ZeroMemory(&m_stClownOperation, sizeof(stClownOperationInfo));
	m_stClownOperation.pClownOperation = new std::vector<tagClownOperation>;
	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink()
{
	m_stClownOperation.pClownOperation->clear();
	SafeDelete(m_stClownOperation.pClownOperation);
}

//释放对象
VOID  CTableFrameSink::Release()
{
}

//接口查询
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
#ifdef __SPECIAL___
	QUERYINTERFACE(ITableUserActionEX,Guid,dwQueryVer);	
#endif
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;

	return NULL;
}

//配置桌子
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL)
		printf("%s, %d, ERROR!!\n", __FUNCTION__, __LINE__);

	//获取参数
	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);


	//开始模式
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	LoadConfig();

	return true;
}

//复位桌子
VOID CTableFrameSink::RepositionSink()
{
	m_wChipUser = INVALID_CHAIR;

	return;
}

//游戏开始
bool CTableFrameSink::OnEventGameStart()
{
printf("%s, %d\n", __FUNCTION__, __LINE__);
	return true;
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NETWORK_ERROR:		//网络中断
	case GER_USER_LEAVE:		//用户离开
		{
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE);

			return true;
		}
	case GER_DISMISS:
		{
			printf("踢人\n");
			IServerUserItem * pItemDismiss=NULL;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				pItemDismiss=m_pITableFrame->GetTableUserItem(i);
				if (NULL!=pItemDismiss)
				{
					m_pITableFrame->PerformStandUpAction(pItemDismiss);
					pItemDismiss->SetUserStatus(US_NULL, pItemDismiss->GetTableID(), wChairID);
				}
			}
			return true;
		}

	}
	ASSERT(FALSE);

	return false;
}

//发送场景
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	if (pIServerUserItem==NULL)
	{
		return true;
	}
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:
		{
			//构造数据
			m_pITableFrame->StartGame();
			//强制设置成游戏状态
			pIServerUserItem->SetUserStatus(US_PLAYING, pIServerUserItem->GetTableID(), wChairID);
			//获取玩家金币数量
			m_wChipUser = 0;
			m_lBeginGuessPirateScore = pIServerUserItem->GetUserScore();
			//判断道具数量
			if (pIServerUserItem->GetTreasureMapCount()>0)
			{
				m_lDefultChip = m_nTreasureMapValue;
				m_lThreeRollChip = m_lDefultChip;
				m_bTreasureMapValueZero = false;
			}
			else
			{
				EnsureChip(pIServerUserItem->GetUserScore());//确定赌注
			}

			//发送消息
			CMD_S_GAME_READY gameReady = {0};
			gameReady.lFinalPrize = m_lFinalPrize;
			gameReady.lGameNeed = m_lThreeRollChip;
			gameReady.lUserScore = pIServerUserItem->GetUserScore();
			gameReady.lDefultChip = m_lDefultChip;
			gameReady.wUserID = m_wChipUser;
			gameReady.nAvailableTimes = m_nAvailableTimes;
			gameReady.nTimes = m_nTimes;
			gameReady.nStarCount = m_nStarCount;
			gameReady.bIsRolling = m_bIsRolling;
			gameReady.bIsSelected = m_bIsSelected;
			gameReady.bLackChip = m_bLackChip;
			gameReady.nItemNum = pIServerUserItem->GetTreasureMapCount();
			gameReady.nFiveStarExtralReward = m_nExtralReward[0];
			gameReady.nSixStarExtralReward = m_nExtralReward[1];
			gameReady.nSevenStarExtralReward = m_nExtralReward[2];
			gameReady.nEightStarExtralReward = m_nExtralReward[3];
			m_lUserMaxThresholdNum = pIServerUserItem->GetUserMaxKValue();
			char szMsg[100];
			sprintf( szMsg, "用户Id=%ld,用户分数=%lld,当前注数=%lld，默认注数=%lld",pIServerUserItem->GetUserID(),pIServerUserItem->GetUserScore(),m_lThreeRollChip,m_lDefultChip);
			CString strTime(szMsg);
			OnRecord(m_strLogDir,strTime);
			return m_pITableFrame->SendGameScene(pIServerUserItem, &gameReady,sizeof(gameReady));
		}
		break;
	case GAME_SCENE_GO:
		{
			CMD_S_GAME_START chipStart = {0};
			chipStart.lGameNeed = m_lCurrentChip;
			chipStart.lUserScore = pIServerUserItem->GetUserScore();
			chipStart.lDefultChip = m_lDefultChip;
			return m_pITableFrame->SendGameScene(pIServerUserItem, &chipStart, sizeof(chipStart));
		}
		break;
	case GAME_SCENE_AUTORUN:
		{
			CMD_S_AUTORUN_GAME autoRun = {0};
			autoRun.bIsRolling = m_bIsRolling;
			autoRun.bIsSelected = m_bIsSelected;
			return m_pITableFrame->SendGameScene(pIServerUserItem, &autoRun, sizeof(autoRun));
		}
		break;
	case GAME_SCENE_PICTURES_ROLL:
		{
			CMD_S_PICTURES_ROLL picsRoll = {0};
			picsRoll.lGameNeed = m_lCurrentChip;
			picsRoll.lUserScore = pIServerUserItem->GetUserScore();
			picsRoll.bIsRolling = m_bIsRolling;
			picsRoll.nAvailableTimes = m_nAvailableTimes;
			return m_pITableFrame->SendGameScene(pIServerUserItem, &picsRoll, sizeof(picsRoll));
		}
		break;
	case GAME_SCENE_CHANGE_BET:
		{
			CMD_S_CHANGE_BET changeBet = {0};
			changeBet.lGameNeed = m_lCurrentChip;
			changeBet.lUserScore = pIServerUserItem->GetUserScore();
			changeBet.lDefultChip = m_lDefultChip;
			return m_pITableFrame->SendGameScene(pIServerUserItem,&changeBet, sizeof(changeBet));
		}
		break;
	case GAME_SCENE_STOP:
		{
			CMD_S_STOP_SCROLL stopScroll;
			ZeroMemory(&stopScroll, sizeof(stopScroll));
			stopScroll.bIsRolling = m_bIsRolling;
			return m_pITableFrame->SendGameScene(pIServerUserItem, &stopScroll, sizeof(stopScroll));
		}
		break;
	case GAME_SCENE_CHANGE_PRIZE:
		{
			CMD_S_CHANGE_PRIZE changePrize = {0};
			changePrize.lChipPrize = m_lChipPrize;
			changePrize.lUserScore = pIServerUserItem->GetUserScore();
			changePrize.nTimes = m_nTimes;

			return m_pITableFrame->SendGameScene(pIServerUserItem, &changePrize, sizeof(changePrize));
		}
		break;
	case GAME_SCENE_END:
		{
			CMD_S_GET_PRIZE gameEnd;
			ZeroMemory(&gameEnd, sizeof(gameEnd));
			gameEnd.lChipPrize = m_lChipPrize;
			gameEnd.lUserScore = pIServerUserItem->GetUserScore() + m_lChipPrize;
			EnsureChip(pIServerUserItem->GetUserScore());
			gameEnd.lDefultChip = m_lDefultChip;
			gameEnd.lCurrentChip = m_lCurrentChip;
			gameEnd.nPrizeTimes = m_nPrizeTimes;
			gameEnd.lFinalPrize = m_lFinalPrize;
			gameEnd.nStarCount = m_nStarCount;
			m_nTimes = 1;
			gameEnd.nTimes = m_nTimes;
			m_bLackChip = false;
			gameEnd.bLackChip = m_bLackChip;
			return m_pITableFrame->SendGameScene(pIServerUserItem, &gameEnd, sizeof(gameEnd));
		}
		break;
	}

	return false;
}

//时间调度
bool CTableFrameSink:: OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_GET_PARAMETER_VALUE:
		{
			LoadConfig();

		}
		break;
	}

	return true;
}

//数据事件
bool CTableFrameSink:: OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return true;
}

//积分事件
bool CTableFrameSink:: OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return true;
}

//确定下注量
void CTableFrameSink::EnsureChip(LONGLONG lUserScore,bool bItemUseUp)
{
	if (m_lBeginGuessPirateScore<lUserScore&&bItemUseUp==false)
	{
		return;
	}
	LONGLONG lDefultChip = m_lDefultChip;

	if (lUserScore >= m_nBandScoreBoundary[0])
	{
		m_nCurrentBand = 0;
		m_lDefultChip = m_nBandScore[0];
	}
	else if (lUserScore >= m_nBandScoreBoundary[1])
	{
		m_nCurrentBand = 1;
		m_lDefultChip = m_nBandScore[1];
	}
	else if (lUserScore >= m_nBandScoreBoundary[2])
	{
		m_nCurrentBand = 2;
		m_lDefultChip = m_nBandScore[2];
	}
	else if (lUserScore >= m_nBandScoreBoundary[3])
	{
		m_nCurrentBand = 3;
		m_lDefultChip = m_nBandScore[3];
	}
	else if (lUserScore >= m_nBandScoreBoundary[4])
	{
		m_nCurrentBand = 4;
		m_lDefultChip = m_nBandScore[4];
	}
	else if(lUserScore > 0)
	{
		m_lDefultChip = 1;
	}
	else if (lUserScore <= 0)
	{
		m_lThreeRollChip = 0;
		m_lDefultChip = 0;
	}
	//刚开始的赌注
	if (m_lThreeRollChip == 0)
	{
		m_lThreeRollChip = m_lDefultChip;
	}
	//printf("前默认赌注=%lld，这次赌注=%lld,前注数=%lld\n",lDefultChip,m_lDefultChip,m_lThreeRollChip);
	if (lUserScore < m_lThreeRollChip)
	{
		m_lThreeRollChip = m_lDefultChip;
	}
}
//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	////判断道具数量
	if (pIServerUserItem==NULL)
	{
		return true;
	}
	m_nTimes = 1;
	m_nStarCount = 0;
	m_nClownIndex = 0;
	m_nPrizeTimes = 0;
	m_lCurrentChip = 0;
	m_lChipPrize = 0;
	m_bIsRolling = false;
	m_bIsSelected = false;
	m_bLackChip = false;
	m_wChipUser = INVALID_CHAIR;
	m_lDefultChip = 0;
	m_lThreeRollChip = 0;
	m_pITableFrame->SetGameStatus(GAME_SCENE_FREE);
	m_nCurrentBand = -1;
	m_bTreasureMapValueZero=true;
	m_lUserMaxThresholdNum = 1000000;
	SetTreasureMapCount(pIServerUserItem,0);
	UpdateUserKValueAndIntegration(pIServerUserItem,0,0);
	UpdateUserMaxKValue(pIServerUserItem);
	return true;
}
//客户端消息事件
bool CTableFrameSink:: OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	if (pIServerUserItem==NULL)
	{
		return true;
	}
	switch (wSubCmdID)
	{
	case SUB_C_GO_GAME:
		{
			//printf("开始转动小丑\n");
			m_pITableFrame->SetGameStatus(GAME_STATUS_PLAY);
			const WORD wChairID = pIServerUserItem->GetUserInfo()->wChairID;
			//数据验证
			ASSERT(INVALID_CHAIR!=wChairID);
			if (INVALID_CHAIR == wChairID)
				printf("%s, %d, ERROR!!\n", __FUNCTION__, __LINE__);
			if (pIServerUserItem->GetUserScore() <= 0)
				return true;
			//接收客户端消息
			EnsureChip(pIServerUserItem->GetUserScore());
			//printf("当前分数=%lld，押注的分数=%lld\n",pIServerUserItem->GetUserScore(),m_lThreeRollChip);
			if (pIServerUserItem->GetUserScore() < m_lThreeRollChip)
				return true;
			pIServerUserItem->WriteUserScore(-m_lThreeRollChip, 0, 0, 0, SCORE_TYPE_LOSE, 0);
			//发送转动消息
			CMD_S_PICTURES_ROLL picsRoll = {0};
			picsRoll.lUserScore = pIServerUserItem->GetUserScore();
			//printf("转动之后玩家分数 = %lld\n",picsRoll.lUserScore);
			picsRoll.lGameNeed = m_lThreeRollChip;
			picsRoll.lDefultChip = m_lDefultChip;
			m_bIsRolling = true;
			picsRoll.bIsRolling = m_bIsRolling;
			//printf("发送图片转动消息\n");
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PICTURES_ROLL,&picsRoll,sizeof(picsRoll));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PICTURES_ROLL,&picsRoll,sizeof(picsRoll));
			
			//发送结果消息
			CMD_S_SEND_RESULT sendResult = {0};
			int nWiningProbability = rand()%10000;
			//printf("随机值= %d\n",nWiningProbability);
			int nSpriteID = 1;
			if (nWiningProbability >= m_nProbability[8] && nWiningProbability < m_nProbability[9])
				nSpriteID = 10;
			else if (nWiningProbability >= m_nProbability[7] && nWiningProbability < m_nProbability[8])
				nSpriteID = 9;
			else if (nWiningProbability >= m_nProbability[6] && nWiningProbability < m_nProbability[7])
				nSpriteID = 8;
			else if (nWiningProbability >= m_nProbability[5] && nWiningProbability < m_nProbability[6])
				nSpriteID = 7;
			else if (nWiningProbability >= m_nProbability[4] && nWiningProbability < m_nProbability[5])
				nSpriteID = 6;
			else if (nWiningProbability >= m_nProbability[3] && nWiningProbability < m_nProbability[4])
				nSpriteID = 5;
			else if (nWiningProbability >= m_nProbability[2] && nWiningProbability < m_nProbability[3])
				nSpriteID = 4;
			else if (nWiningProbability >= m_nProbability[1] && nWiningProbability < m_nProbability[2])
				nSpriteID = 3;
			else if (nWiningProbability >= m_nProbability[0] && nWiningProbability < m_nProbability[1])
				nSpriteID = 2;
			else if (nWiningProbability < m_nProbability[0])
				nSpriteID = 1;
			else
				nSpriteID = 0;

			if (nSpriteID>=1)
			{
				m_nPrizeTimes = m_nRewardTimes[nSpriteID-1];
			}
			else
			{
				m_nPrizeTimes = nSpriteID;
			}
			//获取玩家当前k值
			int nCurThreshold = pIServerUserItem->GetUserKValue();
			//printf("图片id = %d\n",nSpriteID);
			//printf("当前玩家k值 = %d\n",nCurThreshold);
			int nMaxThresholdNum = m_lUserMaxThresholdNum;
			//printf("玩家最大的k值 = %d\n",nMaxThresholdNum);
			if (nSpriteID > 0 && nCurThreshold + (m_nPrizeTimes-1)*m_lThreeRollChip < nMaxThresholdNum )//当前k值加上奖励倍数是否大于最大上限
			{
				sendResult.bPictureIsAllSame = true;
				for (int i = 0; i < 3; i++)
				{
					sendResult.bResultData[i] = nSpriteID;
				}
				printf("当前赌注 = %lld\n",m_lThreeRollChip);
				printf("当前倍数 = %d\n",m_nPrizeTimes);
				m_lCurrentChip = m_lThreeRollChip*m_nPrizeTimes;
				//改变k值改变积分
				UpdateUserKValueAndIntegration(pIServerUserItem,(m_nPrizeTimes-1)*m_lThreeRollChip,m_nPrizeTimes*m_lThreeRollChip);
				sendResult.lCurrentChip = m_lCurrentChip;
				m_lBeginGuessPirateScore = pIServerUserItem->GetUserScore();
			}
			else
			{
				//改变k值
				printf("当前赌注 = %lld\n",m_lThreeRollChip);
				UpdateUserKValueAndIntegration(pIServerUserItem,-m_lThreeRollChip,0);
				//printf("改变基本赌注\n");
				EnsureChip(pIServerUserItem->GetUserScore());
				sendResult.bPictureIsAllSame = false;
				int nSprite0 = rand()%10+1;
				int nSprite1 = rand()%10+1;
				int nSprite2 = rand()%10+1;
				sendResult.bResultData[0] = nSprite0;
				sendResult.bResultData[1] = nSprite1;
				sendResult.bResultData[2] = nSprite2;
				if (nSprite1 == nSprite2)
				{
					sendResult.bResultData[2] = nSprite1%10+1;
				}
				sendResult.lCurrentChip = m_lThreeRollChip;
			}
			//printf("当前玩家这局之后的k值 = %d\n",GetUserKeyValue(pIServerUserItem));
			m_nStarCount = 0;
			m_bLackChip = false;
			sendResult.nStarCount = m_nStarCount;
			sendResult.bLackChip = m_bLackChip;
			sendResult.lDefultChip = m_lDefultChip;
			sendResult.lUserScore = pIServerUserItem->GetUserScore();
			//printf("发送图片转动结果\n");
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_RESLUT,&sendResult,sizeof(sendResult));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_RESLUT,&sendResult,sizeof(sendResult));
			if (sendResult.lUserScore+m_lCurrentChip<m_nMinScoreCanPlay)
			{
				CMD_S_CANT_PLAY_REMIND sCantPlayRemind;
				//char szMsg[100];
				//sprintf( szMsg, "日期::%d:%d:%d:%d\r\n",sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
				lstrcpyn(sCantPlayRemind.szDescribeString,TEXT("您当前的金币不足，海盗不愿意再接受你的挑战了，去新手房多捕几条鱼再来挑战吧。"),CountArray(sCantPlayRemind.szDescribeString));
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_CANT_PLAY,&sCantPlayRemind,sizeof(sCantPlayRemind));
			}
			//printf("开始转动3个图案的倍数=%d\n",m_nTimes);
			//保存玩家这次转盘记录
			m_stClownOperation.dwUserID = pIServerUserItem->GetUserID();
			m_stClownOperation.dwOperationCount = m_stClownOperation.pClownOperation->size();
			tagClownOperation stClownOperation;
			ZeroMemory(&stClownOperation,sizeof(tagClownOperation));
			stClownOperation.nRoundNum = 1;			//表示第1轮操作
			stClownOperation.bIsWin = sendResult.bPictureIsAllSame;
			stClownOperation.lUserScore = pIServerUserItem->GetUserScore()+m_nPrizeTimes*m_lThreeRollChip;
			stClownOperation.lCurrentChip = sendResult.lCurrentChip;
			if (stClownOperation.bIsWin == true)
			{
				stClownOperation.lCurrentChip = (m_nPrizeTimes-1)*m_lThreeRollChip;
			}

			stClownOperation.stGuesOperation.nChipTimes = 1;

			//获取当前时间
			time_t now = time(NULL);
			strftime(stClownOperation.szTime, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
			m_stClownOperation.pClownOperation->push_back(stClownOperation);
			return true;
		}
	case SUB_C_CHANGE_BET:
		{
			return OnSubChangeBet( pIServerUserItem, pData, wDataSize);
		}
	case SUB_C_STOP_GAME:
		{
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			return OnSubStopScroll(pUserData->wChairID , pData, wDataSize);
		}
	case SUB_C_CLOWN_BET:
		{
			m_pITableFrame->SetGameStatus(GAME_SCENE_CLOWN_BET);
			//printf("猜小丑大小\n");
			if (m_lCurrentChip<=0)
			{
				return true;
			}
			const WORD wChairID = pIServerUserItem->GetUserInfo()->wChairID;
			//数据验证
			ASSERT(INVALID_CHAIR!=wChairID);
			if (INVALID_CHAIR == wChairID)
				printf("%s, %d, ERROR!!\n", __FUNCTION__, __LINE__);
			//发送消息
			CMD_S_CLOWN_BET clown = {0};
			m_bLackChip = false;
			//获取玩家当前k值
			int nCurThreshold = pIServerUserItem->GetUserKValue();
			int nMaxThresholdNum = m_lUserMaxThresholdNum;
			int nExtralReward = CalCulNextStarExtralReward();
			printf("当前k值=%d,额外奖励=%d,猜中后的值=%lld,最大k值=%d",nCurThreshold,nExtralReward,(nCurThreshold+m_lCurrentChip+nExtralReward),nMaxThresholdNum);
			if ((nCurThreshold+m_lCurrentChip+nExtralReward)>= nMaxThresholdNum)//不用看结果了，只要超过最大值就得输
			{
				clown.bChooseRight = false;
			}
			else
			{
				int n = (int)(rand()%100);
				int nDecProbability = 0;
				if (m_nTimes == 2)
				{
					nDecProbability = m_nDecProbability[0];
				}
				else if (m_nTimes == 3)
				{
					nDecProbability = m_nDecProbability[1];
				}
				else if (m_nTimes == 5)
				{
					nDecProbability = m_nDecProbability[2];
				}
				printf("对应星= %d,概率=%d,当前随机值=%d\n",m_nStarCount,(m_nClownProbability[m_nStarCount]-nDecProbability),n);
				if (n <= m_nClownProbability[m_nStarCount]-nDecProbability)
					clown.bChooseRight = true;
				else
					clown.bChooseRight = false;
			}
			if (clown.bChooseRight == true)
			{
				//将赢得押注钱写入k值//增加玩家积分
				UpdateUserKValueAndIntegration(pIServerUserItem,m_lCurrentChip,m_lCurrentChip);
				
				m_lCurrentChip = m_lCurrentChip* 2;
				//printf("猜对当前注数 = %d\n",m_lCurrentChip);
				clown.lChipPrize = m_lCurrentChip;
				clown.lUserScore = pIServerUserItem->GetUserScore();
				clown.nStarCount = ++m_nStarCount;
				clown.nClownIndex = m_nClownIndex;
				m_nTimes = 1;
			}
			else
			{
				
				if (pIServerUserItem->GetUserScore() >= m_lCurrentChip)
				{
					
					printf("猜错当前注数 = %d\n",m_lCurrentChip);
					pIServerUserItem->WriteUserScore(-m_lCurrentChip, 0, 0, 0, SCORE_TYPE_LOSE, 0);
					clown.lChipPrize = m_lCurrentChip;
					clown.nStarCount = m_nStarCount;
					clown.nClownIndex = -m_nClownIndex;
					UpdateUserKValueAndIntegration(pIServerUserItem,-m_lCurrentChip,0);
				}
				else
				{
					UpdateUserKValueAndIntegration(pIServerUserItem,-m_lCurrentChip,0);
					m_lCurrentChip = 0;
					printf("用户当前分数=%d\n",pIServerUserItem->GetUserScore());
					clown.lChipPrize = m_lCurrentChip;
					clown.nStarCount = m_nStarCount;
					clown.nClownIndex = -m_nClownIndex;
					m_bLackChip = true;			
				}
				clown.lUserScore = pIServerUserItem->GetUserScore();

			}
			printf("当前注数=%lld\n",m_lCurrentChip);
			clown.nTimes = m_nTimes;
			clown.bLackChip = m_bLackChip;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CLOWN_BET,&clown,sizeof(clown));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CLOWN_BET,&clown,sizeof(clown));
			if (clown.lUserScore+m_lCurrentChip+nExtralReward<m_nMinScoreCanPlay)
			{
				CMD_S_CANT_PLAY_REMIND sCantPlayRemind;
				//char szMsg[100];
				//sprintf( szMsg, "日期::%d:%d:%d:%d\r\n",sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
				lstrcpyn(sCantPlayRemind.szDescribeString,TEXT("您当前的金币不足，海盗不愿意再接受你的挑战了，去新手房多捕几条鱼再来挑战吧。"),CountArray(sCantPlayRemind.szDescribeString));
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_CANT_PLAY,&sCantPlayRemind,sizeof(sCantPlayRemind));
				printf("发送不能玩游戏的信息\n");
			}

			//保存玩家这次的猜大小记录 Operation Log
			m_stClownOperation.dwUserID = pIServerUserItem->GetUserID();
			m_stClownOperation.dwOperationCount = m_stClownOperation.pClownOperation->size();
			tagClownOperation stClownOperation;
			ZeroMemory(&stClownOperation,sizeof(tagClownOperation));
			stClownOperation.nRoundNum = 2;			//表示第二轮操作
			stClownOperation.bIsWin = clown.bChooseRight;
			stClownOperation.lUserScore = pIServerUserItem->GetUserScore()+m_lCurrentChip;
			stClownOperation.lCurrentChip = clown.lChipPrize;
			if (stClownOperation.bIsWin == true)
			{
				stClownOperation.lCurrentChip = stClownOperation.lCurrentChip / 2;
			}

			//获取当前时间
			time_t now = time(NULL);
			strftime(stClownOperation.szTime, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));

			stClownOperation.stGuesOperation.nStarCount = clown.nStarCount;
			stClownOperation.stGuesOperation.nChipTimes = clown.nTimes;

			m_stClownOperation.pClownOperation->push_back(stClownOperation);

			return true;
		}
	case SUB_C_CHANGE_PRIZE:
		{
			//printf("改变猜小丑倍数\n");
			return OnSubChangePrize(pIServerUserItem , pData, wDataSize);
		}
	case SUB_C_GET_PRIZE:
		{
			//printf("结束猜小丑\n");
			const WORD wChairID = pIServerUserItem->GetUserInfo()->wChairID;
			//数据验证
			ASSERT(INVALID_CHAIR!=wChairID);
			if (INVALID_CHAIR == wChairID)
				printf("%s, %d, ERROR!!\n", __FUNCTION__, __LINE__);
			//接收客户端消息
			CMD_C_GET_PRIZE* cGameEnd = (CMD_C_GET_PRIZE*)pData;
			LONGLONG lExtralReward = 0;
			if (m_lCurrentChip>0)
			{	
				lExtralReward = CalCulExtralReward();
				printf("额外奖励 = %lld\n",lExtralReward);
				pIServerUserItem->WriteUserScore(lExtralReward, 0, 0, 0, SCORE_TYPE_WIN, 0);
				UpdateUserKValueAndIntegration(pIServerUserItem,lExtralReward,lExtralReward);

			}
			pIServerUserItem->WriteUserScore(m_lCurrentChip, 0, 0, 0, SCORE_TYPE_WIN, 0);
			//额外奖励记录
			tagClownOperation stClownOperation;
			ZeroMemory(&stClownOperation,sizeof(tagClownOperation));
			stClownOperation.nRoundNum = 3;			//表示额外奖励
			stClownOperation.bIsWin = 1;
			stClownOperation.lUserScore = pIServerUserItem->GetUserScore();
			stClownOperation.lCurrentChip = lExtralReward;
			stClownOperation.stGuesOperation.nChipTimes = 0;
			time_t now = time(NULL);
			strftime(stClownOperation.szTime, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
			m_stClownOperation.pClownOperation->push_back(stClownOperation);

			if (lExtralReward + m_lCurrentChip/m_nTimes >= m_nBigRewardBand)
			{
				OnWinBigReward(pIServerUserItem,(lExtralReward + m_lCurrentChip/m_nTimes)/10000);
			}
			if (pIServerUserItem->GetTreasureMapCount()<=0&&m_bTreasureMapValueZero==false)
			{
				m_lThreeRollChip = 0;
				m_bTreasureMapValueZero = true;
				EnsureChip(pIServerUserItem->GetUserScore(),true);//刷新下注量
			}
			else
			{
				EnsureChip(pIServerUserItem->GetUserScore());//刷新下注量
			}
			
			//发送收金消息
			
			CMD_S_GET_PRIZE sGameEnd = {0};
			sGameEnd.wUserID = m_wChipUser;
			sGameEnd.lUserScore = pIServerUserItem->GetUserScore();
			sGameEnd.lFinalPrize = lExtralReward;
			m_lCurrentChip	= 0;
			sGameEnd.lDefultChip = m_lDefultChip;
			sGameEnd.lCurrentChip = m_lThreeRollChip;
			sGameEnd.lChipPrize = m_lThreeRollChip;
			sGameEnd.nPrizeTimes = m_nPrizeTimes;
			sGameEnd.nStarCount = m_nStarCount;
			m_nTimes = 1;
			sGameEnd.nTimes = m_nTimes;
			sGameEnd.bLackChip = m_bLackChip;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GET_PRIZE,&sGameEnd,sizeof(sGameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GET_PRIZE,&sGameEnd,sizeof(sGameEnd));
			//m_pITableFrame->SetGameStatus(GAME_SCENE_END);
			//保存玩家这局的猜大小记录[cdx]
			m_stClownOperation.dwUserID = pIServerUserItem->GetUserID();
			m_stClownOperation.dwOperationCount = m_stClownOperation.pClownOperation->size();
			//DWORD dwDataSize = m_stClownOperation.pClownOperation->size() * sizeof(tagClownOperation) + sizeof(DWORD) + sizeof(DWORD);
			DWORD dwDataSize = sizeof(stClownOperationInfo) + m_stClownOperation.pClownOperation->size() * sizeof(tagClownOperation);

			pIServerUserItem->WriteUserLog(0, (VOID *)&m_stClownOperation,dwDataSize);
			//FILE *FSPOINTER;
			//FSPOINTER = fopen("ClownLog.TXT", "a+");
			//fprintf(FSPOINTER, "Userid = %ld\n", m_stClownOperation.dwUserID);
			//for (int i = 0; i < m_stClownOperation.dwOperationCount; i ++)
			//{
			//	fprintf(FSPOINTER ,TEXT("RoundNum = %d, IsWin = %d, UserScore = %lld, CurrentChip = %lld, Time = %s\n"), 
			//		(*(m_stClownOperation.pClownOperation))[i].nRoundNum,
			//		(*(m_stClownOperation.pClownOperation))[i].bIsWin,
			//		(*(m_stClownOperation.pClownOperation))[i].lUserScore,
			//		(*(m_stClownOperation.pClownOperation))[i].lCurrentChip,
			//		(*(m_stClownOperation.pClownOperation))[i].szTime);	
			//}
			//fclose(FSPOINTER);
			m_stClownOperation.pClownOperation->clear();
			return true;
			/*tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			return OnSubGetPrize(pUserData->wChairID , pData, wDataSize);*/
		}
	case SUB_C_GO_GAME_ITEM:
		{
			printf("开始转动小丑\n");
			m_pITableFrame->SetGameStatus(GAME_STATUS_PLAY);
			const WORD wChairID = pIServerUserItem->GetUserInfo()->wChairID;
			//数据验证
			ASSERT(INVALID_CHAIR!=wChairID);
			if (INVALID_CHAIR == wChairID)
				printf("%s, %d, ERROR!!\n", __FUNCTION__, __LINE__);
			//判断道具数量是否足够
			if (pIServerUserItem->GetTreasureMapCount() <= 0)
				return true;
			//printf("当前分数=%lld，押注的分数=%lld\n",pIServerUserItem->GetUserScore(),m_lThreeRollChip);
			//发送结果消息
			CMD_S_USEITEM_SEND_RESULT sendResult = {0};
			int nWiningProbability = rand()%10000;
			//printf("随机值= %d\n",nWiningProbability);
			int nSpriteID = 1;
			if (nWiningProbability >= m_nProbability[8] && nWiningProbability < m_nProbability[9])
				nSpriteID = 10;
			else if (nWiningProbability >= m_nProbability[7] && nWiningProbability < m_nProbability[8])
				nSpriteID = 9;
			else if (nWiningProbability >= m_nProbability[6] && nWiningProbability < m_nProbability[7])
				nSpriteID = 8;
			else if (nWiningProbability >= m_nProbability[5] && nWiningProbability < m_nProbability[6])
				nSpriteID = 7;
			else if (nWiningProbability >= m_nProbability[4] && nWiningProbability < m_nProbability[5])
				nSpriteID = 6;
			else if (nWiningProbability >= m_nProbability[3] && nWiningProbability < m_nProbability[4])
				nSpriteID = 5;
			else if (nWiningProbability >= m_nProbability[2] && nWiningProbability < m_nProbability[3])
				nSpriteID = 4;
			else if (nWiningProbability >= m_nProbability[1] && nWiningProbability < m_nProbability[2])
				nSpriteID = 3;
			else if (nWiningProbability >= m_nProbability[0] && nWiningProbability < m_nProbability[1])
				nSpriteID = 2;
			else if (nWiningProbability < m_nProbability[0])
				nSpriteID = 1;
			else
				nSpriteID = 0;

			if (nSpriteID>=1)
			{
				m_nPrizeTimes = m_nRewardTimes[nSpriteID-1];
			}
			else
			{
				m_nPrizeTimes = nSpriteID;
			}
			SetTreasureMapCount(pIServerUserItem,-1);
			//获取玩家当前k值
			int nCurThreshold = pIServerUserItem->GetUserKValue();
			//printf("图片id = %d\n",nSpriteID);
			//printf("当前玩家k值 = %d\n",nCurThreshold);
			//int nMaxThresholdNum = m_nMaxThresholdNum;
			int nMaxThresholdNum = m_lUserMaxThresholdNum;
			//printf("玩家最大的k值 = %d\n",nMaxThresholdNum);
			if (nSpriteID > 0 && nCurThreshold + (m_nPrizeTimes-1)*m_lThreeRollChip < nMaxThresholdNum )//当前k值加上奖励倍数是否大于最大上限
			{
				sendResult.bPictureIsAllSame = true;
				for (int i = 0; i < 3; i++)
				{
					sendResult.bResultData[i] = nSpriteID;
				}
				//printf("当前赌注 = %lld\n",m_lThreeRollChip);
				//printf("当前倍数 = %d\n",m_nPrizeTimes);
				m_lCurrentChip = m_lThreeRollChip*m_nPrizeTimes;
				//改变k值改变积分
				UpdateUserKValueAndIntegration(pIServerUserItem,(m_nPrizeTimes-1)*m_lThreeRollChip,m_nPrizeTimes*m_lThreeRollChip);
				sendResult.lCurrentChip = m_lCurrentChip;
				m_lBeginGuessPirateScore = pIServerUserItem->GetUserScore();
			}
			else
			{
				//改变k值
				UpdateUserKValueAndIntegration(pIServerUserItem,-m_lThreeRollChip,0);
				//若果道具数量小于0
				if (pIServerUserItem->GetTreasureMapCount()<=0)
				{
					m_lThreeRollChip = 0;
					EnsureChip(pIServerUserItem->GetUserScore(),true);
				}	
				sendResult.bPictureIsAllSame = false;
				int nSprite0 = rand()%10+1;
				int nSprite1 = rand()%10+1;
				int nSprite2 = rand()%10+1;
				sendResult.bResultData[0] = nSprite0;
				sendResult.bResultData[1] = nSprite1;
				sendResult.bResultData[2] = nSprite2;
				if (nSprite1 == nSprite2)
				{
					sendResult.bResultData[2] = nSprite1%10+1;
				}
				sendResult.lCurrentChip = m_lThreeRollChip;
			}
			//printf("当前玩家这局之后的k值 = %d\n",GetUserKeyValue(pIServerUserItem));
			m_nStarCount = 0;
			m_bLackChip = false;
			sendResult.nStarCount = m_nStarCount;
			sendResult.bLackChip = m_bLackChip;
			sendResult.lDefultChip = m_lDefultChip;
			sendResult.lUserScore = pIServerUserItem->GetUserScore();
			sendResult.nItemNum=pIServerUserItem->GetTreasureMapCount();
			//printf("发送图片转动结果\n");
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_USEITEM_RESLUT,&sendResult,sizeof(sendResult));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_USEITEM_RESLUT,&sendResult,sizeof(sendResult));
			//printf("开始转动3个图案的倍数=%d\n",m_nTimes);
			//保存玩家这次转盘记录
			m_stClownOperation.dwUserID = pIServerUserItem->GetUserID();
			m_stClownOperation.dwOperationCount = m_stClownOperation.pClownOperation->size();
			tagClownOperation stClownOperation;
			ZeroMemory(&stClownOperation,sizeof(tagClownOperation));
			stClownOperation.nRoundNum = 1;			//表示第1轮操作
			stClownOperation.bIsWin = sendResult.bPictureIsAllSame;
			stClownOperation.lUserScore = pIServerUserItem->GetUserScore()+m_nPrizeTimes*m_lThreeRollChip;
			stClownOperation.lCurrentChip = sendResult.lCurrentChip;
			// 如果失败不扣分
			if (stClownOperation.bIsWin == false)
			{
				stClownOperation.lCurrentChip = 0;
			}

			//获取当前时间
			time_t now = time(NULL);
			strftime(stClownOperation.szTime, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
			m_stClownOperation.pClownOperation->push_back(stClownOperation);
			return true;
		}
	}
	return false;
}
//开始游戏
bool CTableFrameSink::OnSubStartGame(const WORD wChairID, const void * pDataBuffer, WORD wDataSize)
{
	return true;
}

//改变转动3个图案的赌注
bool CTableFrameSink::OnSubChangeBet(IServerUserItem * pIServerUserItem, const void * pDataBuffer, WORD wDataSize)
{
	//数据验证
	printf("改变基本赌注\n");
	if (pIServerUserItem == NULL)
	{
		return true;
	}
	if (pIServerUserItem->GetTreasureMapCount()>0)
	{
		return true;
	}
	ASSERT(INVALID_CHAIR!=pIServerUserItem->GetUserInfo()->wChairID);
	if (INVALID_CHAIR == pIServerUserItem->GetUserInfo()->wChairID)
		printf("%s, %d, ERROR!!\n", __FUNCTION__, __LINE__);
	CMD_C_CHANGE_BET* cChangeBet = (CMD_C_CHANGE_BET*)pDataBuffer;
	CMD_S_CHANGE_BET sChangeBet = {0};
	//增加赌注
	if (cChangeBet->bPlusChip == true)
	{
		if (pIServerUserItem->GetUserScore()>=m_lThreeRollChip*2&&m_lThreeRollChip*2<=999999)
		{
			sChangeBet.lDefultChip = m_lDefultChip;
			sChangeBet.cbCanChange = 1;
			m_lThreeRollChip = m_lThreeRollChip*2;
			sChangeBet.lGameNeed = m_lThreeRollChip;
		}
		else
		{
			if (pIServerUserItem->GetUserScore()>=m_lThreeRollChip*2&&m_lThreeRollChip*2>999999)
			{
				m_lThreeRollChip = 999999;
			}
			sChangeBet.lGameNeed = m_lThreeRollChip;
			sChangeBet.lDefultChip = m_lDefultChip;
			sChangeBet.cbCanChange = 2;
		}

	}
	//减少赌注
	else
	{
		//printf("当前档次=%d，下一档次边界值=%d\n",m_nCurrentBand,m_nBandScore[m_nCurrentBand+1]);
		if ((m_nCurrentBand+1 < 5 && m_lThreeRollChip>m_nBandScore[m_nCurrentBand+1])|| ((m_nCurrentBand + 1 == 5) && (m_lThreeRollChip>m_nBandScore[m_nCurrentBand])))
		{
			if (m_lThreeRollChip==999999)
			{
				m_lThreeRollChip = 500000;
			}
			else
			{
				if (m_nCurrentBand + 1 == 5)
				{
					m_lThreeRollChip = m_lThreeRollChip/2;
				}
				else
				{
					//最小减到当前档的下一档
					m_lThreeRollChip = m_lThreeRollChip/2;
					if (m_lThreeRollChip<m_nBandScore[m_nCurrentBand+1])
					{
						m_lThreeRollChip = m_nBandScore[m_nCurrentBand+1];
					}
				}

			}
			sChangeBet.lDefultChip = m_lDefultChip;	
			sChangeBet.lGameNeed = m_lThreeRollChip;
			sChangeBet.cbCanChange = 1;
		}
		else
		{
			sChangeBet.lGameNeed = m_lThreeRollChip;
			sChangeBet.lDefultChip = m_lDefultChip;
			sChangeBet.cbCanChange = 3;
		}

	}
	//printf("当前注数=%d\n",m_lThreeRollChip);
	sChangeBet.lUserScore = pIServerUserItem->GetUserScore();
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BET,&sChangeBet,sizeof(sChangeBet));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BET,&sChangeBet,sizeof(sChangeBet));
	return true;
}
//停止转动
bool CTableFrameSink::OnSubStopScroll(const WORD wChairID, const void * pDataBuffer, WORD wDataSize)
{
	//数据验证
	ASSERT(INVALID_CHAIR!=wChairID);
	if (INVALID_CHAIR == wChairID)
		printf("%s, %d, ERROR!!\n", __FUNCTION__, __LINE__);
	//接收客户端消息
	CMD_C_STOP_SCROLL* cStopRoll = (CMD_C_STOP_SCROLL*)pDataBuffer;
	m_bIsRolling = cStopRoll->bIsRolling;
	//发送消息
	CMD_S_STOP_SCROLL sStopRoll = {0};
	m_bIsRolling = false;
	sStopRoll.bIsRolling = m_bIsRolling;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_STOP_SCROLL,&sStopRoll,sizeof(sStopRoll));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_STOP_SCROLL,&sStopRoll,sizeof(sStopRoll));

	return true;
}
//判断彩灰小丑
bool CTableFrameSink::OnSubClownBet(const WORD wChairID, const void * pDataBuffer, WORD wDataSize)
{
	return true;
}
//改变猜小丑赌注倍数
bool CTableFrameSink::OnSubChangePrize(IServerUserItem * pIServerUserItem, const void * pDataBuffer, WORD wDataSize)
{
	//数据验证
	if (pIServerUserItem == NULL)
	{
		return true;
	}
	ASSERT(INVALID_CHAIR!=pIServerUserItem->GetUserInfo()->wChairID);
	if (INVALID_CHAIR == pIServerUserItem->GetUserInfo()->wChairID)
		printf("%s, %d, ERROR!!\n", __FUNCTION__, __LINE__);

	CMD_C_CHANGE_PRIZE* cChangePrize = (CMD_C_CHANGE_PRIZE*)pDataBuffer;
	
	CMD_S_CHANGE_PRIZE sChangePrize = {0};
	if (m_nTimes <= 0)
	{
		return true;
	}
	int nChangeTimes = m_nTimes - cChangePrize->nTimes;
	LONGLONG lChangeChip = (m_lCurrentChip/m_nTimes)*(cChangePrize->nTimes);
	if (nChangeTimes>0)
	{
		LONGLONG lAddScore = (m_lCurrentChip/m_nTimes)*nChangeTimes;
		pIServerUserItem->WriteUserScore(lAddScore, 0, 0, 0, SCORE_TYPE_WIN, 0);
	}
	else if (nChangeTimes<0  )
	{
		LONGLONG lMinusScore = (m_lCurrentChip/m_nTimes)*nChangeTimes;	//增加倍数需要从玩家身上扣除多余的钱
		if (pIServerUserItem->GetUserScore() >= (-lMinusScore))			//判断玩家身上的钱够不够扣
		{
			pIServerUserItem->WriteUserScore(lMinusScore, 0, 0, 0, SCORE_TYPE_LOSE, 0);
		}
		else
		{
			sChangePrize.bLackChip = false;
			sChangePrize.lChipPrize = m_lCurrentChip;
			sChangePrize.lUserScore = pIServerUserItem->GetUserScore();
			sChangePrize.nTimes = m_nTimes;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_PRIZE,&sChangePrize,sizeof(sChangePrize));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_PRIZE,&sChangePrize,sizeof(sChangePrize));
			return true;
		}
		
	}
	m_lCurrentChip = lChangeChip;
	sChangePrize.bLackChip = true;
	sChangePrize.lChipPrize = m_lCurrentChip;
	sChangePrize.lUserScore = pIServerUserItem->GetUserScore();
	sChangePrize.nTimes = 1;
	m_nTimes = cChangePrize->nTimes;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_PRIZE,&sChangePrize,sizeof(sChangePrize));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_PRIZE,&sChangePrize,sizeof(sChangePrize));

	return true;
}
//收金
bool CTableFrameSink::OnSubGetPrize(const WORD wChairID, const void * pDataBuffer, WORD wDataSize)
{
	return true;
}
//用户起立
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if ( bLookonUser )
		return true;
	if (pIServerUserItem==NULL)
	{
		return true;
	}
	//意外断线用户没有结算，将注的钱加到玩家身上
	LONGLONG lExtralReward = 0;
	if (m_lCurrentChip > 0)
	{
		lExtralReward = CalCulExtralReward();
		printf("当前注数=%lld，当前倍数=%lld，额外奖励=%lld\n",m_lCurrentChip,m_nTimes,lExtralReward);
		pIServerUserItem->WriteUserScore(m_lCurrentChip+lExtralReward, 0, 0, 0, SCORE_TYPE_WIN, 0);
		UpdateUserKValueAndIntegration(pIServerUserItem,lExtralReward,lExtralReward);		

		// 额外奖励分数记录
		tagClownOperation stClownOperation;
		ZeroMemory(&stClownOperation,sizeof(tagClownOperation));
		stClownOperation.nRoundNum = 3;			//表示额外奖励
		stClownOperation.bIsWin = 1;
		stClownOperation.lUserScore = pIServerUserItem->GetUserScore();
		stClownOperation.lCurrentChip = lExtralReward;
		stClownOperation.stGuesOperation.nChipTimes = 0;
		time_t now = time(NULL);
		strftime(stClownOperation.szTime, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
		m_stClownOperation.pClownOperation->push_back(stClownOperation);
	}
	//设置变量
	m_nTimes = 1;
	m_nStarCount = 0;
	m_nClownIndex = 0;
	m_nPrizeTimes = 0;
	m_lCurrentChip = 0;
	m_lChipPrize = 0;
	m_bIsRolling = false;
	m_bIsSelected = false;
	m_bLackChip = false;
	m_wChipUser = INVALID_CHAIR;
	m_lDefultChip = 0;
	m_lThreeRollChip = 0;
	m_pITableFrame->SetGameStatus(GAME_SCENE_FREE);
	m_nCurrentBand = -1;
	m_bTreasureMapValueZero=true;
	m_lUserMaxThresholdNum = 1000000;

	//保存玩家这局的猜大小记录[cdx]
	m_stClownOperation.dwUserID = pIServerUserItem->GetUserID();
	m_stClownOperation.dwOperationCount = m_stClownOperation.pClownOperation->size();
	DWORD dwDataSize = sizeof(stClownOperationInfo) + m_stClownOperation.pClownOperation->size() * sizeof(tagClownOperation);
	pIServerUserItem->WriteUserLog(0, (VOID *)&m_stClownOperation,dwDataSize);
	m_stClownOperation.pClownOperation->clear();
	return true;
}
//框架消息
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}



void CTableFrameSink::OnRecord(CString strLogDir,CString strMsg)
{
	SYSTEMTIME sys; 
	GetLocalTime(&sys);
	char szTime[100];
	sprintf( szTime, "日期::%d:%d:%d:%d\r\n",sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
	CString strTime(szTime);
	strMsg = strMsg + strTime;
	CFile fLog;
	if(fLog.Open( strLogDir, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite ))
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

bool CTableFrameSink::OnWinBigReward(IServerUserItem * pIServerUserItem,int nScore)
{
	if (pIServerUserItem == NULL)
	{
		return false;
	}
	m_pITableFrame->OnWinBigReward(pIServerUserItem,nScore);
	return true;
}

void CTableFrameSink::SetTreasureMapCount(IServerUserItem * pIServerUserItem,int nCount)
{
	if (pIServerUserItem == NULL)
	{
		return ;
	}
	//printf("CTableFrameSink=%d\n",nCount);
	m_pITableFrame->SetTreasureMapCount(pIServerUserItem,nCount);
	return;
}

void CTableFrameSink::UpdateUserKValueAndIntegration(IServerUserItem * pIServerUserItem,int nKvalue,int nIntegration)
{
	if (pIServerUserItem == NULL)
	{
		return ;
	}
	//printf("CTableFrameSink=%d\n",nKvalue,nIntegration);
	m_pITableFrame->UpdateUserKValueAndIntegration(pIServerUserItem,nKvalue,nIntegration);
	return;
}

void CTableFrameSink::UpdateUserMaxKValue(IServerUserItem * pIServerUserItem)
{
	if (pIServerUserItem == NULL)
	{
		return ;
	}
	int nMaxKvalue = rand()%(m_nMaxRandKNum - m_nMinRandKNum +1) + m_nMinRandKNum;
	m_pITableFrame->UpdateUserMaxKValue(pIServerUserItem,nMaxKvalue);
	return;
}

//计算额外奖励
int CTableFrameSink::CalCulExtralReward()
{
	int nExtralReward=0;
	if (m_nStarCount==5)
	{
		nExtralReward = (m_lCurrentChip/m_nTimes*m_nExtralReward[0])/100;
	}
	else if (m_nStarCount==6)
	{
		nExtralReward = (m_lCurrentChip/m_nTimes*m_nExtralReward[1])/100;
	}
	else if (m_nStarCount==7)
	{
		nExtralReward = (m_lCurrentChip/m_nTimes*m_nExtralReward[2])/100;
	}
	else if (m_nStarCount==8)
	{
		nExtralReward = (m_lCurrentChip/m_nTimes*m_nExtralReward[3])/100;
	}
	return nExtralReward;
}

int CTableFrameSink::CalCulNextStarExtralReward()
{
	int nExtralReward=0;
	if (m_nStarCount==4)
	{
		nExtralReward = (m_lCurrentChip*2/m_nTimes*m_nExtralReward[0])/100;
	}
	else if (m_nStarCount==5)
	{
		nExtralReward = (m_lCurrentChip*2/m_nTimes*m_nExtralReward[1])/100;
	}
	else if (m_nStarCount==6)
	{
		nExtralReward = (m_lCurrentChip*2/m_nTimes*m_nExtralReward[2])/100;
	}
	else if (m_nStarCount==7)
	{
		nExtralReward = (m_lCurrentChip*2/m_nTimes*m_nExtralReward[3])/100;
	}
	return nExtralReward;
}

void CTableFrameSink::LoadConfig()
{
	//读取配置
	if (m_pITableFrame->GetTableID()==0)
	{
		TCHAR szPath[MAX_PATH]=TEXT("");
		TCHAR szConfigFileName[MAX_PATH] = TEXT("");
		GetCurrentDirectory(sizeof(szPath),szPath);
		_sntprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\SuperClown.ini"),szPath);
		TCHAR szServerName[MAX_PATH]=TEXT("");
		_sntprintf(szServerName,CountArray(szServerName),TEXT("%s"),m_pGameServiceOption->szServerName);
		TCHAR sValueName[100];
		for (int i = 0;i < 10;++i)
		{
			_sntprintf(sValueName,100, TEXT("Probability%d"), i+1);
			m_nProbability[i]=GetPrivateProfileInt(szServerName,sValueName,0,szConfigFileName);
			if (i > 0)
			{
				m_nProbability[i] += m_nProbability[i-1];
			}	
			printf("概率值=%d\n",m_nProbability[i]);
		}
		m_nMaxThresholdNum=GetPrivateProfileInt(szServerName,TEXT("MaxThresholdNum"),100001,szConfigFileName);
		printf("k最大值=%d\n",m_nMaxThresholdNum);
		for (int i = 0;i < 8;++i)
		{
			_sntprintf(sValueName,100, TEXT("ClownProbability%d"), i+1);
			m_nClownProbability[i]=GetPrivateProfileInt(szServerName,sValueName,0,szConfigFileName);
			printf("概率值=%d\n",m_nClownProbability[i]);
		}

		for (int i = 0;i < 10;++i)
		{
			_sntprintf(sValueName,100, TEXT("RewardTimes%d"), i+1);
			m_nRewardTimes[i]=GetPrivateProfileInt(szServerName,sValueName,0,szConfigFileName);
			printf("奖励倍数=%d\n",m_nRewardTimes[i]);
		}
		for (int i = 0;i < 4;++i)
		{
			_sntprintf(sValueName,100, TEXT("ExtralReward%d"), i+1);
			m_nExtralReward[i]=GetPrivateProfileInt(szServerName,sValueName,0,szConfigFileName);
			printf("额外奖励=%d\n",m_nExtralReward[i]);
		}
		m_nMinRandKNum=GetPrivateProfileInt(szServerName,TEXT("MinRandKNum"),100000,szConfigFileName);
		m_nMaxRandKNum=GetPrivateProfileInt(szServerName,TEXT("MaxRandKNum"),10000000,szConfigFileName);
		m_nDecProbability[0]=GetPrivateProfileInt(szServerName,TEXT("DecProbability2"),2,szConfigFileName);
		m_nDecProbability[1]=GetPrivateProfileInt(szServerName,TEXT("DecProbability3"),3,szConfigFileName);
		m_nDecProbability[2]=GetPrivateProfileInt(szServerName,TEXT("DecProbability5"),5,szConfigFileName);
		printf("减少的数目1=%d，减少的数目2=%d，减少的数目3=%d\n",m_nDecProbability[0],m_nDecProbability[1],m_nDecProbability[2]);
		for (int i = 0;i < 5;++i)
		{
			_sntprintf(sValueName,100, TEXT("BandScoreBoundary%d"), i+1);
			m_nBandScoreBoundary[i]=GetPrivateProfileInt(szServerName,sValueName,1,szConfigFileName);
			printf("额外奖励=%d\n",m_nBandScoreBoundary[i]);
		}
		for (int i = 0;i < 5;++i)
		{
			_sntprintf(sValueName,100, TEXT("BandScore%d"), i+1);
			m_nBandScore[i]=GetPrivateProfileInt(szServerName,sValueName,1,szConfigFileName);
			printf("额外奖励=%d\n",m_nBandScore[i]);
		}

		m_nBigRewardBand=GetPrivateProfileInt(szServerName,TEXT("BigRewardBand"),300000,szConfigFileName);
		m_nTreasureMapValue=GetPrivateProfileInt(szServerName,TEXT("TreasureMapValue"),1000,szConfigFileName);
		m_nMinScoreCanPlay=GetPrivateProfileInt(szServerName,TEXT("MinScoreCanPlay"),1000,szConfigFileName);
		printf("最低可以玩的分数限制=%d",m_nMinScoreCanPlay);
		//自定规则
		ASSERT(m_pITableFrame->GetCustomRule()!=NULL);
		m_pGameCustomRule=(tagCustomRule *)m_pITableFrame->GetCustomRule();
		m_pITableFrame->SetGameTimer(IDI_GET_PARAMETER_VALUE,60000,-1,0);
		CString strFileDlgPath;
		TCHAR szModuleDirectory[MAX_PATH];	//模块目录
		GetModuleFileName(AfxGetInstanceHandle(),szModuleDirectory,sizeof(szModuleDirectory));
		int nModuleLen=lstrlen(szModuleDirectory);
		int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".EXE")) + 1;
		if (nModuleLen<=nProcessLen) 
			return ;
		szModuleDirectory[nModuleLen-nProcessLen]=0;
		strFileDlgPath = szModuleDirectory;
		CString strThreadLogDir  = strFileDlgPath+"\\log";
		m_strLogDir = strThreadLogDir+"\\Record.log";
	}
}
