#ifndef DATABASE_ENGINE_SINK_HEAD_FILE
#define DATABASE_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "DataBasePacket.h"

//////////////////////////////////////////////////////////////////////////////////

//检查库存
struct tagDBR_GR_CheckStock
{
	LONG								lCaiJin;
	LONG								lCaiJinSent;
	LONG								lTypeID;
	LONG								lRoomID;
	LONG								lTableID;
	LONG								lStockScore;
	LONG								lRevenue;
	BOOL								lbLoad;	
	LONG								lUserID;
};

struct tagDBR_GR_CheckStockSuccess
{
	LONG								lType;
	LONG								lRoomID;
	LONG								lTableID;
	LONG								lStockScore;
	LONG								lRevenue;
	LONG								lStockChange;
	LONG								lCaiJin;
	LONG								lCaiJinSent;
	BOOL								lbSuccess;
};

struct DBR_GR_UserLucky
{
	DWORD								dwUserID;
	DWORD								dwSerial;
	float								fLucky;
};

//数据库类
class CDataBaseEngineSink : public IDataBaseEngineSink, public IGameDataBaseEngine
{
	//友元定义
	friend class CServiceUnits;

	//辅助变量
protected:
	DBO_GR_LogonFailure				m_LogonFailure;						//登录失败
	DBO_GR_LogonSuccess				m_LogonSuccess;						//登录成功

	//游戏数据库
protected:
	CDataBaseAide					m_GameDBAide;						//游戏数据
	CDataBaseHelper					m_GameDBModule;						//游戏数据

	//游戏币数据库
protected:
	CDataBaseAide					m_TreasureDBAide;					//游戏币数据库
	CDataBaseHelper					m_TreasureDBModule;					//游戏币数据库

	//平台数据库
protected:
	CDataBaseAide					m_PlatformDBAide;					//平台数据库
	CDataBaseHelper					m_PlatformDBModule;					//平台数据库

	//配置变量
protected:
	CDataBaseAide					m_LogServerDBAide;					//日志数据库
	CDataBaseHelper					m_LogServerDBModule;				//日志数据
protected:
	CInitParameter *				m_pInitParameter;					//配置参数
	tagGameParameter *				m_pGameParameter;					//配置参数
	tagDataBaseParameter *			m_pDataBaseParameter;				//连接信息
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//服务属性
	tagGameServiceOption *			m_pGameServiceOption;				//服务配置

	//组件变量
protected:
	IDataBaseEngine *				m_pIDataBaseEngine;					//引擎接口
	IGameServiceManager *			m_pIGameServiceManager;				//服务管理
	IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//数据事件

	//查询接口
protected:
	IGameDataBaseEngineSink *		m_pIGameDataBaseEngineSink;			//数据接口

	//组件变量
public:
	IDBCorrespondManager *          m_pIDBCorrespondManager;            //数据协调

	//函数定义
public:
	//构造函数
	CDataBaseEngineSink();
	//析构函数
	virtual ~CDataBaseEngineSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//配置参数
public:
	//自定配置
	virtual VOID * GetCustomRule() { return m_pGameServiceOption->cbCustomRule; };
	//服务属性
	virtual tagGameServiceAttrib * GetGameServiceAttrib() { return m_pGameServiceAttrib; }
	//服务配置
	virtual tagGameServiceOption * GetGameServiceOption() { return m_pGameServiceOption; }

	//获取对象
public:
	//获取对象
	virtual VOID * GetDataBase(REFGUID Guid, DWORD dwQueryVer);
	//获取对象
	virtual VOID * GetDataBaseEngine(REFGUID Guid, DWORD dwQueryVer);

	//功能接口
public:
	//投递结果
	virtual bool PostGameDataBaseResult(WORD wRequestID, VOID * pData, WORD wDataSize);

	//系统事件
public:
	//启动事件
	virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx);
	//停止事件
	virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx);

	//内核事件
public:
	//时间事件
	virtual bool OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//控制事件
	virtual bool OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize);
	//请求事件
	virtual bool OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//登录函数
protected:
	//I D 登录
	bool OnRequestLogonUserID(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID,BOOL bMatch=false);
	//I D 登录
	bool OnRequestLogonMobile(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//帐号登录
	bool OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);

	//系统处理
protected:
	//游戏写分
	bool OnRequestWriteGameScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//离开房间
	bool OnRequestLeaveGameServer(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//游戏记录
	bool OnRequestGameScoreRecord(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//加载参数
	bool OnRequestLoadParameter(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//加载列表
	bool OnRequestLoadGameColumn(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//加载机器
	bool OnRequestLoadAndroidUser(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//加载道具
	bool OnRequestLoadGameProperty(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//礼物请求
	bool OnRequestPropertyRequest(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//用户权限
	bool OnRequestManageUserRight(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//系统消息
	bool OnRequestLoadSystemMessage(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//比赛配置信息
	bool OnRequestLoadMatchConfig(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//比赛配置信息
	bool OnRequestLoadMatchRewardConfig(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//加载敏感词
	bool OnRequestLoadSensitiveWords(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//比赛报名
	bool OnRequestMatchFee(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//比赛开始
	bool OnRequestMatchStart(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//比赛结束
	bool OnRequestMatchOver(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//比赛奖励
	bool OnRequestMatchReward(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//退出比赛
	bool OnRequestMatchQuit(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//大喇叭
	bool OnRequestCheckTrumpet(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//技能处理
	bool OnRequestQuerySkill(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//技能使用
	bool OnRequestUseSkill(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//新手引导奖励
	bool OnRequestNewGuidGift(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//日志记录小丑操作
	bool OnRequestLogClownOperation(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//第一次进入超级海盗房间
	bool OnRequestFirstEnterSuper(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//更新玩家k值和积分
	bool OnRequestKValueAndIntegration(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	bool OnRequestUserMaxKValue(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	bool OnRequestUserMatchSignUP(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	bool OnRequestGetMatchPirze(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//银行服务
protected:
	//存入游戏币
	bool OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//提取游戏币
	bool OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//转账游戏币
	bool OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//赠送龙珠
	bool OnRequestUserTransferDragon(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//使用龙珠
	bool OnRequestUserUseDragon(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//赢得龙珠碎片
	bool OnRequestUserWinDragonPiece(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//赢得珍珠
	bool OnRequestUserWinPearl(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//查询银行
	bool OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//查询用户
	bool OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//系统赠送技能
	bool OnRequestSkillGive(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//获取用户道具
	bool OnRequestGetUserItemCount(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//保存用户道具
	bool OnRequestSaveUserItemCount(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);

	//辅助函数
private:
	//登录结果
	VOID OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,BYTE cbDeviceType=DEVICE_TYPE_PC,WORD wBehaviorFlags=0,WORD wPageTableCount=0);
	//银行结果
	VOID OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, SCORE lFrozenedScore, LPCTSTR pszErrorString, bool bMobileClient,BYTE cbActivityGame=FALSE);
	//赠送信息
	VOID OnInsureTransferInfo(DWORD dwContextID, DWORD dwErrorCode);
	//龙珠结果
	VOID OnDragonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, BYTE cbParam1);
	//比赛报名结果
	VOID OnMatchSignUpDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient, DWORD dwUserID,BYTE cbDeviceType=DEVICE_TYPE_PC,WORD wBehaviorFlags=0,WORD wPageTableCount=0);
protected:
	//更新库存
	bool OnBankCheckStock(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取用户幸运值
	bool OnGetUserLucky(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//保存用户物品
	bool OnSaveUserItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//游戏记录Json格式
	bool OnGameJson(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//执行数据库脚本
	bool OnSentence(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询藏宝图
	bool OnRequestUpdateTreasureMap(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
};

//////////////////////////////////////////////////////////////////////////////////

#endif