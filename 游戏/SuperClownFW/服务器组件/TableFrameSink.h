#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgCustomRule.h"

//////////////////////////////////////////////////////////////////////////////////
//struct tagTurnTableRecord
//{
//	bool  bIsWin;       //是否成功
//	bool  bIsEnd;       //是否结束
//	SCORE lUserScore;
//	SCORE lCurrentChip;
//	TCHAR szTime[50];
//};

struct tagGuessOperation
{
	int   nStarCount;
	int   nChipTimes;	//倍数	
};
//小丑操作日志
struct tagClownOperation
{
	int   nRoundNum;	//第几轮操作, 转盘还是猜大小
	bool  bIsWin;       //是否成功
	SCORE lUserScore;
	SCORE lCurrentChip;
	TCHAR szTime[20];
	tagGuessOperation stGuesOperation;
};


struct stClownOperationInfo
{
	DWORD dwUserID;
	DWORD dwOperationCount;
	DWORD dwDataSize;	
	std::vector<tagClownOperation> *	pClownOperation;					//记录一个环节所有的操作
};

//游戏桌子
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
//组件变量
protected:
																		                    //游戏逻辑
	ITableFrame							* m_pITableFrame;									//框架接口
	const tagGameServiceOption			* m_pGameServiceOption;								//配置参数
	tagGameServiceAttrib *				m_pGameServiceAttrib;								//游戏属性
	tagCustomRule *						m_pGameCustomRule;									//自定规则
//属性变量
protected:
	static const WORD					m_wPlayerCount;										//游戏人数
	static const BYTE					m_GameStartMode;									//开始模式
//游戏变量
	LONGLONG                            m_lBeginGuessPirateScore;                           //用户开始猜海盗时的分数
	LONGLONG							m_lDefultChip;									    //默认下注大小
	LONGLONG							m_lThreeRollChip;									//三个图案转动时的赌注
	LONGLONG							m_lCurrentChip;									    //实时下注大小
	LONGLONG                            m_lChipPrize;                                       //赌注奖励
	LONGLONG                            m_lFinalPrize;                                      //最大奖
	int                                 m_nPrizeTimes;                                      //奖励倍数
	int                                 m_nStarCount;                                       //下注后猜对次数
	int                                 m_nClownIndex;                                      //小丑索引
	int                                 m_nTimes;                                           //下注倍数
	int                                 m_nAvailableTimes;                                  //玩家剩余可玩次数
	int                                 m_nYear;                                            //对比登录日期刷新每日可玩次数
	int                                 m_nMonth;
	int                                 m_nDay;
	WORD                                m_wChipUser;                                        //当前玩家
	bool                                m_bIsSelected;                                      //是否选中
	bool                                m_bIsRolling;                                       //是否在转动
	bool                                m_bLackChip;                                        //是否不足下注
	int									m_miniRevenue;
	bool								m_bTreasureMapValueZero;							//藏宝图数量变为零
	LONGLONG							m_lUserMaxThresholdNum;								//玩家随机到的最大k值
	int									m_nCurrentBand;										//玩家所在的当前档次
	//配置变量
	static LONGLONG						m_nProbability[10];									//随机三个图案的概率
	static LONGLONG						m_nMaxThresholdNum;							        //平台算法最大阈值
	static int							m_nClownProbability[8];						        //猜对小丑大小次数对应的的概率
	static int							m_nRewardTimes[10];
	static int							m_nExtralReward[4];									//额外奖励
	static int							m_nMinRandKNum;
	static int							m_nMaxRandKNum;
	static int							m_nDecProbability[3];								//猜大小时下注倍数减小的概率数
	static int							m_nBandScoreBoundary[5];							//默认押注的分数段
	static int							m_nBandScore[5];									//默认押注的分数段对应的分数值

	static int							m_nBigRewardBand;									//获得巨奖公告的最大值
	static int							m_nTreasureMapValue;								//藏宝图值多少金币
	static int							m_nMinScoreCanPlay;									//最少可以玩游戏的分数
	//小丑日志信息
	//std::vector<tagClownOperationEachRound> *	m_pClownOperation;							
	stClownOperationInfo				m_stClownOperation;									//记录一个环节所有的操作/
	static CString						m_strLogDir;										//日志目录

	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release();
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//管理接口
public:
	//配置桌子
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual VOID RepositionSink();

	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem) { return 0; }
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem) { return 0; }
	//查询是否扣服务费
	virtual bool QueryBuckleServiceCharge(WORD wChairID) { return false; }

	//游戏事件
public:
	//游戏开始
	virtual bool OnEventGameStart();
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);
	//确定下注量
	void EnsureChip(LONGLONG lUserScore,bool bItemUseUp=false);
	//计算当前星额外奖励
	int CalCulExtralReward();
	//计算下个星额外奖励
	int CalCulNextStarExtralReward();

	//消息处理
public:
	//开始游戏
	bool OnSubStartGame(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//停止转动
	bool OnSubStopScroll(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//改变赌注
	bool OnSubChangeBet(IServerUserItem * pIServerUserItem, const void * pDataBuffer, WORD wDataSize);
	//判断彩灰小丑
	bool OnSubClownBet(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//改变猜小丑赌注
	bool OnSubChangePrize(IServerUserItem * pIServerUserItem, const void * pDataBuffer, WORD wDataSize);
	//结束游戏
	bool OnSubGetPrize(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);

	//事件接口
public:
	//时间事件
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize);
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

	//网络接口
public:
	//游戏消息
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//比赛接口
public:
	//设置基数
	virtual void SetGameBaseScore(LONG lBaseScore) {}

	//用户事件
public:
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户重入
	virtual bool OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起立
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//获取游戏内部分数
	virtual	LONG GetInsideScore(WORD wChairID) { return 0; }
	//新手引导奖励
	virtual bool OnNewGuidGift(IServerUserItem * pIServerUserItem, bool bSuccess, int nCannonStatus, int nLotteryCount) { return true; }
	//赢得珍珠
	virtual bool OnWinPearl(IServerUserItem * pIServerUserItem, int nPearlType, int nPearlCount) { return true; }
	//处理比赛排名
	virtual void OnMatchRank(IServerUserItem * pIServerUserItem, SCORE lMatchScore, int nCurrentRank, int nChangeRank) { return; }
	void OnRecord(CString strLogDir,CString strMsg);
	//发送获得巨奖公告
	virtual bool OnWinBigReward(IServerUserItem * pIServerUserItem,int Score);
	//道具操作
	virtual void SetTreasureMapCount(IServerUserItem * pIServerUserItem,int nCount);
	//设置玩家kvalue
	virtual void UpdateUserKValueAndIntegration(IServerUserItem * pIServerUserItem,int nKvalue,int nIntegration);
	//设置玩家最大kvalue
	virtual void UpdateUserMaxKValue(IServerUserItem * pIServerUserItem);
	virtual bool OnGetUserItemCount(IServerUserItem * pIServerUserItem, int nType, SCORE lCount){ return false; };
	void LoadConfig();
};

//////////////////////////////////////////////////////////////////////////////////

#endif