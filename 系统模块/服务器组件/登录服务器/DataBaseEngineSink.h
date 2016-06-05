#ifndef DATABASE_ENGINE_SINK_HEAD_FILE
#define DATABASE_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "DataBasePacket.h"

//////////////////////////////////////////////////////////////////////////////////

#define DB_NO_AVAILABLE_ACCOUNT	1									//没有可用账户
#define DB_ACCINFO_ALREADYEXIST		7									//账户已经存在
#define DB_GUEST_REG_SUCCESS		21									//游客注册成功

#define DB_QUERY_SIGNIN_SUCCESS     22									//签到查询成功
#define DB_QUERY_SIGNIN_FAILURE		23									//签到查询失败
#define DB_USER_SIGNIN_SUCCESS		24									//点击签到成功
#define DB_USER_SIGNIN_FAILURE		25									//点击签到失败
#define DB_USER_SIGNIN_ALREADY		26									//用户已签到
#define DB_USER_BQ_SUCCESS			27									//补签成功

#define DB_QUERY_LOTTERY_SUCCESS    30									//抽奖查询成功
#define DB_QUERY_LOTTERY_FAILURE    31									//抽奖查询失败
#define DB_USER_LOTTERY_SUCCESS     32									//抽奖操作成功
#define DB_USER_LOTTERY_FAILURE     33									//抽奖操作失败
#define DB_GET_PRIZEITEM_SUCCESS    34									//中奖等级获取成功
#define DB_GET_PRIZEITEM_FAILURE    35									//中奖等级获取失败

//数据库类
class CDataBaseEngineSink : public IDataBaseEngineSink
{
	//友元定义
	friend class CServiceUnits;

	//成员变量
private:
	UINT m_uiRandCodeCnt;													//密码计数

	//用户数据库
protected:
	CDataBaseAide					m_AccountsDBAide;					//用户数据库
	CDataBaseHelper					m_AccountsDBModule;					//用户数据库

	//游戏币数据库
protected:
	CDataBaseAide					m_TreasureDBAide;					//游戏币数据库
	CDataBaseHelper					m_TreasureDBModule;					//游戏币数据库

	//平台数据库
protected:
	CDataBaseAide					m_PlatformDBAide;					//平台数据库
	CDataBaseHelper					m_PlatformDBModule;					//平台数据库

	//日志数据库
protected:
	CDataBaseAide					m_LogServerDBAide;					//日志数据库
	CDataBaseHelper					m_LogServerDBModule;				//日志数据库

	//组件变量
protected:
	CInitParameter *				m_pInitParameter;					//配置参数
	IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//数据事件

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

	//登录服务
protected:
	//I D 登录
	bool OnRequestLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号登录
	bool OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//登录记录
	bool OnRequestRecordLogon(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号注册
	bool OnRequestRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获取游客信息
	LONG InsGuestRegInfo(DBR_GP_RegisterAccounts* pGuestRegInfo, CDBVarValue * pDBVarValue);
	//插入游客信息
	LONG GetGuestRegInfo(DBR_GP_RegisterAccounts* pGuestRegInfo, CDBVarValue * pDBVarValue);
	//游客登录
	bool OnRequestGuestRegister(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//生成随机账户
	VOID GenRandAccount(TCHAR szRandAccount[]);
	//生成随机字符串
	VOID GenRandString(TCHAR szRandStr[], UINT uLength);

	//手机登录
protected:
	//I D 登录
	bool OnMobileLogonGameID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号登录
	bool OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//帐号注册
	bool OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//// 手机号注册
	//bool OnRequestMBNumRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//账号服务
protected:
	//修改机器
	bool OnRequestModifyMachine(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改密码
	bool OnRequestModifyLogonPass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改密码
	bool OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改签名
	bool OnRequestModifyUnderWrite(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改头像
	bool OnRequestModifySystemFace(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改头像
	bool OnRequestModifyCustomFace(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//修改资料
	bool OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//签到服务
protected:
	bool OnRequestUserSignIn(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//银行服务
protected:
	//存入游戏币
	bool OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//提取游戏币
	bool OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//转账游戏币
	bool OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//赠送龙珠
	bool OnRequestUserTransferDragon(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//使用龙珠
	bool OnRequestUserUseDragon(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//信息查询
protected:
	//查询资料
	bool OnRequestQueryIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询银行
	bool OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//密码验证
	bool OnRequestUserCheckPassword(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询用户
	bool OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询签到
	bool OnRequestQuerySignInInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//抽奖查询
	bool OnRequestQueryLotteryInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//抽奖操作
	bool OnRequestUserLtry(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//系统功能
protected:
	//加载列表
	bool OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//在线信息
	bool OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//邮件ID列表
	bool OnRequestQueryMailIDList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//邮件信息
	bool OnRequestQueryMailInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//邮件内容
	bool OnRequestQueryMailContent(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//邮件物品
	bool OnRequestQueryMailItem(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//充值到账-金额
	bool OnRequestChargeValue(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//充值到账-取消
	bool OnRequestChargeCancel(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//充值到账-网站充值金额
	bool OnRequestWebChargeValue(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//VIP信息
	bool OnRequestVIPInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//VIP每日赠送金币
	bool OnRequestVIPEverydayGift(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//VIP充值返现
	bool OnRequestVIPChargeBack(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//当前分数
	bool OnRequestCurrentScore(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//登录奖励
	bool OnRequestLogonGift(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//首充礼包
	bool OnRequestCharge1stRequest(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//领取首充礼包奖励
	bool OnRequestCharge1stGet(DWORD dwContextID, VOID * pData, WORD wDataSize);

	bool OnRequestWealthBagAddScore(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//查询道具数量
	bool OnRequestQueryItemCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//赠送道具
	bool OnRequestTransferItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//合成道具
	bool OnRequestCombineItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//使用龙珠
	bool OnRequestUseDragon(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//装备龙珠
	bool OnRequestEquipmentDragon(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询技能
	bool OnRequestQuerySkill(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询手机号
	bool OnRequestMBVericationCode(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询新手引导
	bool OnRequestQueryNewGuid(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询小丑金币
	bool OnRequestQuerySuperGold(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//兑换奖券
	bool OnRequestExchangeLottery(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//结果处理
protected:
	//登录结果
	VOID OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient, bool bIsLogin = false);
	//银行结果
	VOID OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//操作结果
	VOID OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//赠送信息
	VOID OnInsureTransferInfo(DWORD dwContextID, DWORD dwErrorCode);
	//签到结果
	VOID OnSignInDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//抽奖结果
	VOID OnLotteryDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//龙珠结果
	VOID OnDragonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//查询手机号结果
	VOID OnVerifyMBNumDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//手机号注册
	bool OnRequestMBRegister(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//手机号
	VOID OnMBRegisterDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//手机号绑定
	bool OnRequestMBBind(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//手机号绑定结果
	VOID OnRequestMBBindResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//手机号是否绑定过
	bool OnRequestMBBindNumExit(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//请求广播速度
	bool OnRequestBroadCastSpeed(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//比赛信息
	bool OnRequestCompRankInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//喇叭广播
	bool OnRequestBroadCastLaBa(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//请求type类型的物品数量
	bool OnRequestTypeItemCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//兑换话费
	bool OnRequestChangePhoneFare(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//超级海盗排名
	bool OnRequestSuperRankInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//请求绑定奖励和新手引导
	bool OnRequestBindRewardNewGuid(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//设置超级海盗新手引导
	bool OnRequestSetSuperNewGuid(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获得新邮件
	bool OnRequestGetNewMailRemind(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//查询免费喇叭次数
	bool OnRequestQueryFreeLabaCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//获得比赛奖励
	bool OnRequestGetMatchPirze(DWORD dwContextID, VOID * pData, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////////////

#endif