#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "TableFrame.h"
#include "InitParameter.h"
#include "ServerListManager.h"
#include "DataBasePacket.h"
#include "SensitiveWordsFilter.h"
#include <list>
#include <string>

//////////////////////////////////////////////////////////////////////////////////

//连接类型
#define CLIENT_KIND_FALSH			1									//网页类型
#define CLIENT_KIND_MOBILE			2									//手机类型
#define CLIENT_KIND_COMPUTER		3									//电脑类型

//比赛信息
#define MATCH_STATUS_BEGIN			1
#define MATCH_STATUS_END			2
//绑定参数
struct tagBindParameter
{
	//连接属性
	DWORD							dwSocketID;							//网络标识
	DWORD							dwClientAddr;						//连接地址
	DWORD							dwActiveTime;						//激活时间

	//版本信息
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwProcessVersion;					//进程版本

	//用户属性
	BYTE							cbClientKind;						//连接类型
	IServerUserItem *				pIServerUserItem;					//用户接口
};

//系统消息
struct tagSystemMessage
{
	DWORD							dwLastTime;						   //发送时间
	DBR_GR_SystemMessage            SystemMessage;                     //系统消息
};

struct tagUserLucky
{
	DWORD							dwUserID;
	DWORD							dwSerial;
	float							fLucky;
};

struct tagMatchUserInfoGameServer
{
	DWORD dwUserID;
	WORD  wGroupID;
	SCORE lUserScore;
};
struct tagMatchConfigItem
{
	int nStartTime;
	int nEndTime;
	int nMatchNum;
	int nApplyCost;
	int nMatchUserCount;
	int nGameTime;
	int nMatchType;
};

struct tagMatchRewardConfigItem
{
	int nRankStart;
	int nRankEnd;
	int nReward[10];
	int nMachType;
	int nShareType;
};

//数组说明
typedef CWHArray<CTableFrame *>		CTableFrameArray;					//桌子数组
typedef CMap<DWORD,DWORD,DWORD,DWORD &>  CKickUserItemMap;              //踢人映射 
typedef CList<tagSystemMessage *>   CSystemMessageList;                 //系统消息
typedef CList<IServerUserItem *>    CWaitDistributeList;                //等待分配

//////////////////////////////////////////////////////////////////////////////////

//调度钩子
class CAttemperEngineSink : public IAttemperEngineSink, public IMainServiceFrame,
	public IServerUserItemSink
{
	//友元定义
	friend class CServiceUnits;

	//分配份数
protected:
	UINT m_RandPartNum;
	UINT m_PrecedenceTableArray[3];							//优先填充的桌子

	//状态变量
protected:
	bool							m_bCollectUser;						//汇总标志
	bool							m_bNeekCorrespond;					//协调标志

	//绑定信息
protected:
	tagBindParameter *				m_pNormalParameter;					//绑定信息
	tagBindParameter *				m_pAndroidParameter;				//绑定信息

	//配置信息
protected:
	CInitParameter *				m_pInitParameter;					//配置参数
	tagGameParameter *				m_pGameParameter;					//配置参数
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//服务属性
	tagGameServiceOption *			m_pGameServiceOption;				//服务配置

	//配置数据
protected:
	CMD_GR_ConfigColumn				m_DataConfigColumn;					//列表配置
	CMD_GR_ConfigProperty			m_DataConfigProperty;				//道具配置

	//组件变量
protected:
	CTableFrameArray				m_TableFrameArray;					//桌子数组
	CServerListManager				m_ServerListManager;				//列表管理
	CServerUserManager				m_ServerUserManager;				//用户管理
	CAndroidUserManager				m_AndroidUserManager;				//机器管理
	CGamePropertyManager			m_GamePropertyManager;				//道具管理
	CKickUserItemMap                m_KickUserItemMap;                  //踢人管理
	CSystemMessageList              m_SystemMessageList;                //系统消息
	CSensitiveWordsFilter			m_WordsFilter;						//脏字过滤
	CWaitDistributeList             m_WaitDistributeList;               //等待分配

	//组件接口
protected:
	ITimerEngine *					m_pITimerEngine;					//时间引擎
	IAttemperEngine *				m_pIAttemperEngine;					//调度引擎
	ITCPSocketService *				m_pITCPSocketService;				//网络服务
	ITCPNetworkEngine *				m_pITCPNetworkEngine;				//网络引擎
	IGameServiceManager *			m_pIGameServiceManager;				//服务管理

	//比赛服务
public:
	IGameMatchServiceManager		* m_pIGameMatchServiceManager;		//比赛管理接口

	//数据引擎
public:
	IDataBaseEngine *				m_pIRecordDataBaseEngine;			//数据引擎
	IDataBaseEngine *				m_pIKernelDataBaseEngine;			//数据引擎
	IDBCorrespondManager *          m_pIDBCorrespondManager;            //数据协调

	tagStockInfo					m_TotalStock;						//总库存
	tagStockInfo					m_RobotStock;						//机器人库存
	tagLotteryInfo					m_LotteryInfo;
	tagMobileMoney					m_MobileMoney;
	std::list<tagUserLucky *>		m_UserLucky;
	std::list<tagMatchConfigItem>	m_MatchConfigItemList;				//比赛配置数据
	std::list<tagMatchRewardConfigItem>	m_MatchRewardConfigItemList;	//比赛奖励配置数据
//可变定时器时间间隔
	WORD							m_wFakeAnnounceTimeInterval;          //发送假公告的间隔

	LONGLONG						m_lBigReward;							//巨奖

	LONGLONG						m_lBigRewardBaseNum ;					//巨奖最小值
	LONGLONG						m_lBigRewardMaxNum ;					//巨奖最大值
	int								m_lBigRewardAddBaseNum;					//巨奖增加最小值
	int								m_lBigRewardAddRandNum;					//巨奖随机范围
	int								m_nSignUpCost;							//报名比赛的费用
	int								m_nTeamMemberNum;						//每个队伍的成员
	int								m_nMessageRemindTime1;					//距离比赛开始信息展示提醒时间1
	int								m_nMessageRemindTime2;					//距离比赛开始信息展示提醒时间2
	int								m_nMessageRemindTime3;					//距离比赛开始信息展示提醒时间3
	int								m_nMatchBeginRemindTime;				//距离比赛开始提醒时间
// 比赛用变量
	std::vector<std::vector<tagMatchUserInfoGameServer> > m_vMatchUserInfo;	//本服务器保存的比赛用户的记录
	bool							m_bMatchSignUpMark;						//比赛报名是否开始的标记
	bool							m_bIsMatchTime;							//比赛持续时间
	byte							m_cbMatchStatus;
	int								m_nCurrentMatchID;
	WORD							m_wUserCntInGroup;

	//函数定义
public:
	//构造函数
	CAttemperEngineSink();
	//析构函数
	virtual ~CAttemperEngineSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//异步接口
public:
	//启动事件
	virtual bool OnAttemperEngineStart(IUnknownEx * pIUnknownEx);
	//停止事件
	virtual bool OnAttemperEngineConclude(IUnknownEx * pIUnknownEx);

	//事件接口
public:
	//控制事件
	virtual bool OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize);
	//自定事件
	virtual bool OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize);

	//内核事件
public:
	//时间事件
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM wBindParam);
	//数据库事件
	virtual bool OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//连接事件
public:
	//连接事件
	virtual bool OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode);
	//关闭事件
	virtual bool OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason);
	//读取事件
	virtual bool OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize);

	//网络事件
public:
	//应答事件
	virtual bool OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID);
	//关闭事件
	virtual bool OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID);
	//读取事件
	virtual bool OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//消息接口
public:
	//房间消息
	virtual bool SendRoomMessage(LPCTSTR lpszMessage, WORD wType);
	//游戏消息
	virtual bool SendGameMessage(LPCTSTR lpszMessage, WORD wType);
	//房间消息
	virtual bool SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType);
	//游戏消息
	virtual bool SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType);
	//房间消息
	virtual bool SendRoomMessage(DWORD dwSocketID, LPCTSTR lpszMessage, WORD wType, bool bAndroid);

	//网络接口
public:
	//发送数据
	virtual bool SendData(BYTE cbSendMask, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//发送数据
	virtual bool SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//发送数据
	virtual bool SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//功能接口
public:
	//插入用户
	virtual bool InsertDistribute(IServerUserItem * pIServerUserItem);
	//插入用户
	virtual bool InsertWaitDistribute(IServerUserItem * pIServerUserItem);
	//删除用户
	virtual bool DeleteWaitDistribute(IServerUserItem * pIServerUserItem);
	//分配用户
	virtual bool DistributeUserGame();
	//敏感词过滤
	virtual void SensitiveWordFilter(LPCTSTR pMsg, LPTSTR pszFiltered, int nMaxLen);

	//用户接口
public:
	//用户积分
	virtual bool OnEventUserItemScore(IServerUserItem * pIServerUserItem, BYTE cbReason);
	//用户状态
	virtual bool OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID=INVALID_TABLE, WORD wOldChairID=INVALID_CHAIR);
	//用户权限
	virtual bool OnEventUserItemRight(IServerUserItem *pIServerUserItem, DWORD dwAddRight, DWORD dwRemoveRight,bool bGameRight=true);
	//用户日志
	virtual bool OnEventUserItemLog(IServerUserItem *pIServerUserItem, WORD wRequestID, VOID * pData, DWORD dwDataSize);

	//数据事件
protected:
	//登录成功
	bool OnDBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//登录失败
	bool OnDBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//配置信息
	bool OnDBGameParameter(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//列表信息
	bool OnDBGameColumnInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//机器信息
	bool OnDBGameAndroidInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//道具信息
	bool OnDBGamePropertyInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//银行信息
	bool OnDBUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//银行成功
	bool OnDBUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//赠送信息
	bool OnDBUserInsureTransferInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//银行失败
	bool OnDBUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//用户信息
	bool OnDBUserInsureUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//道具成功
	bool OnDBPropertySuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//道具失败
	bool OnDBPropertyFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//系统消息
	bool OnDBSystemMessage(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//加载敏感词
	bool OnDBSensitiveWords(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//幸运值
	bool OnDBGetUserLucky(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//比赛报名结果
	bool OnUserMatchSignUPRes(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//连接处理
protected:
	//注册事件
	bool OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//列表事件
	bool OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//汇总事件
	bool OnTCPSocketMainUserCollect(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//管理服务
	bool OnTCPSocketMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//网络事件
protected:
	//用户处理
	bool OnTCPNetworkMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//登录处理
	bool OnTCPNetworkMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//游戏处理
	bool OnTCPNetworkMainGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//框架处理
	bool OnTCPNetworkMainFrame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//银行处理
	bool OnTCPNetworkMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//管理处理
	bool OnTCPNetworkMainManage(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//比赛命令
	bool OnTCPNetworkMainMatch(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//技能处理
	bool OnTCPNetworkMainSkill(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//请求巨奖
	bool OnTCPNetworkGetBigReward(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//网络事件
protected:
	//I D 登录
	bool OnTCPNetworkSubLogonUserID(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//手机登录
	bool OnTCPNetworkSubLogonMobile(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//帐号登录
	bool OnTCPNetworkSubLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//用户命令
protected:
	//用户规则
	bool OnTCPNetworkSubUserRule(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//用户旁观
	bool OnTCPNetworkSubUserLookon(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//用户坐下
	bool OnTCPNetworkSubUserSitDown(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//用户起立
	bool OnTCPNetworkSubUserStandUp(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//用户聊天
	bool OnTCPNetworkSubUserChat(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//用户私聊
	bool OnTCPNetworkSubWisperChat(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//用户表情
	bool OnTCPNetworkSubUserExpression(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//用户表情
	bool OnTCPNetworkSubWisperExpression(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//购买道具
	bool OnTCPNetworkSubPropertyBuy(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//使用道具
	bool OnTCPNetwordSubSendTrumpet(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//使用道具
	bool OnTCPNetworkSubVIPTrumpet(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//邀请用户
	bool OnTCPNetworkSubUserInviteReq(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//拒绝厌友
	bool OnTCPNetworkSubUserRepulseSit(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//踢出命令
	bool OnTCPNetworkSubMemberKickUser(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//请求用户信息
	bool OnTCPNetworkSubUserInfoReq(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//请求更换位置
	bool OnTCPNetworkSubUserChairReq(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//请求椅子用户信息
	bool OnTCPNetworkSubChairUserInfoReq(VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//银行命令
protected:
	//查询银行
	bool OnTCPNetworkSubQueryInsureInfo(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//存款请求
	bool OnTCPNetworkSubSaveScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//取款请求
	bool OnTCPNetworkSubTakeScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//转账请求
	bool OnTCPNetworkSubTransferScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//查询用户请求
	bool OnTCPNetworkSubQueryUserInfoRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//管理命令
protected:
	//查询设置
	bool OnTCPNetworkSubQueryOption(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//房间设置
	bool OnTCPNetworkSubOptionServer(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//踢出用户
	bool OnTCPNetworkSubManagerKickUser(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//限制聊天
	bool OnTCPNetworkSubLimitUserChat(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//踢出所有用户
	bool OnTCPNetworkSubKickAllUser(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//发布消息
	bool OnTCPNetworkSubSendMessage(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//解散游戏
	bool OnTCPNetworkSubDismissGame(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//第一次进入超级海盗奖励
	bool OnTCPNetworkSubFirstEnterSuper(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//用户报名
	bool OnTCPNetworkSubMatchSignUP(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//获得比赛奖励
	bool OnTCPNetworkSubMatchGetPrize(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//内部事件
protected:
	//用户登录
	VOID OnEventUserLogon(IServerUserItem * pIServerUserItem, bool bAlreadyOnLine);
	//用户登出
	VOID OnEventUserLogout(IServerUserItem * pIServerUserItem, DWORD dwLeaveReason);

	//执行功能
protected:
	//解锁游戏币
	bool PerformUnlockScore(DWORD dwUserID, DWORD dwInoutIndex, DWORD dwLeaveReason);
	//版本检查
	bool PerformCheckVersion(DWORD dwPlazaVersion, DWORD dwFrameVersion, DWORD dwClientVersion, DWORD dwSocketID);
	//切换连接
	bool SwitchUserItemConnect(IServerUserItem * pIServerUserItem, TCHAR szMachineID[LEN_MACHINE_ID], WORD wTargetIndex,BYTE cbDeviceType=DEVICE_TYPE_PC,WORD wBehaviorFlags=0,WORD wPageTableCount=0);

	//发送函数
protected:
	//用户信息
	bool SendUserInfoPacket(IServerUserItem * pIServerUserItem, DWORD dwSocketID);

	//辅助函数
protected:
	//购前事件
	bool OnEventPropertyBuyPrep(WORD cbRequestArea,WORD wPropertyIndex,IServerUserItem *pISourceUserItem,IServerUserItem *pTargetUserItem);
	//道具消息
	bool SendPropertyMessage(DWORD dwSourceID,DWORD dwTargerID,WORD wPropertyIndex,WORD wPropertyCount);
	//道具效应
	bool SendPropertyEffect(IServerUserItem * pIServerUserItem);

	//辅助函数
protected:
	//登录失败
	bool SendLogonFailure(LPCTSTR pszString, LONG lErrorCode, DWORD dwSocketID);
	//银行失败
	bool SendInsureFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode,BYTE cbActivityGame);
	//请求失败
	bool SendRequestFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode);
	//道具失败
	bool SendPropertyFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode,WORD wRequestArea);

	//辅助函数
public:
	//绑定用户
	IServerUserItem * GetBindUserItem(WORD wBindIndex);
	//绑定参数
	tagBindParameter * GetBindParameter(WORD wBindIndex);
	//道具类型
	WORD GetPropertyType(WORD wPropertyIndex);

	//辅助函数
protected:
	//配置机器
	bool InitAndroidUser();
	//配置桌子
	bool InitTableFrameArray();
	//发送请求
	bool SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize);
	//设置参数
	void SetMobileUserParameter(IServerUserItem * pIServerUserItem,BYTE cbDeviceType,WORD wBehaviorFlags,WORD wPageTableCount);
	//群发数据
	virtual bool SendDataBatchToMobileUser(WORD wCmdTable, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//群发用户信息
	bool SendUserInfoPacketBatchToMobileUser(IServerUserItem * pIServerUserItem);
	//发可视用户信息
	bool SendViewTableUserInfoPacketToMobileUser(IServerUserItem * pIServerUserItem,DWORD dwUserIDReq);
	//手机立即登录
	bool MobileUserImmediately(IServerUserItem * pIServerUserItem);
	//购买道具
	int OnPropertyBuy(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//发送系统消息
	bool SendSystemMessage(CMD_GR_SendMessage * pSendMessage, WORD wDataSize);
	//清除消息数据
	void ClearSystemMessageData();
public:
	const tagStockInfo* GetStockInfo(WORD wTableID, int nType);
	void ChangeStock(int nType, WORD wTableID, DWORD nUserID, int nStock, int nRevenue, int nCaiJin);
	float GetUserFactor(unsigned int nUserID);
	virtual int SaveUserItem(IServerUserItem * pIServerUserItem, int nItemIndex);
	//游戏记录Json格式
	virtual void SaveGameJson(IServerUserItem * pIServerUserItem, const TCHAR * str, SCORE lScore);
	//在线人数
	void ShowRealOnlineCount();
	//写分并剔除
	void SaveAndKick();
	tagLotteryInfo * GetLotteryInfo();
	tagMobileMoney * GetMobileMoney();
	//赢得龙珠碎片
	bool WinDragonPiece(IServerUserItem * pIServerUserItem, SCORE lWinDragonPiece, SCORE lWinScore);
	//赢得珍珠
	bool WinPearl(IServerUserItem * pIServerUserItem, int nPearlType, int nPearlCount);
	//系统赠送技能
	bool SkillGive(IServerUserItem * pIServerUserItem);
	//新手引导奖励
	bool OnNewGuidGift(IServerUserItem * pIServerUserItem);
	//发送比赛积分
	bool SendMatchScore(IServerUserItem * pIServerUserItem, SCORE lMatchScore);
	//发送全服广播
	void SendPlatformMessage(LPCTSTR lpszMessage, BYTE cbLevel);
	//获取用户道具
	void GetUserItemCount(IServerUserItem * pIServerUserItem, int nType);
	//保存用户道具
	void SaveUserItemCount(IServerUserItem * pIServerUserItem, int nType, SCORE lCount);
	//获得巨奖
	virtual bool OnWinBigReward(IServerUserItem * pIServerUserItem,int nScore);
	//设置藏宝图数量
	virtual void SetTreasureMapCount(IServerUserItem * pIServerUserItem,int nCount);
	//设置玩家kValue值
	virtual void UpdateUserKValueAndIntegration(IServerUserItem * pIServerUserItem,int nKvalue,int nIntegration);
	//设置玩家最大k值
	virtual void UpdateUserMaxKValue(IServerUserItem * pIServerUserItem,int nCount);
	//获取下一比赛配置信息
	void GetNextMatchInfo(tagMatchConfigItem & MatchConfigItem);
	virtual void UpdateUserIntegrate(IServerUserItem * pIServerUserItem,int nAddScore);
	//获取用户比赛奖励信息
	void GetUserMatchRewardInfo(tagMatchRewardConfigItem & MatchConfigItem,int Rank,int nMatchType);
	//设置优先填充的桌子
protected:
	VOID AndriodSitTableAssign();

};

//////////////////////////////////////////////////////////////////////////////////

#endif