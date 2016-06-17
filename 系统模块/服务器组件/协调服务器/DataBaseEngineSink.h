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

	//插入比赛报名用户
	bool OnRequestInsertMatchSignUpUser(DWORD dwContextID, VOID * pData, WORD wDataSize);

	bool OnRequestLoadMatchConfig(DWORD dwContextID, VOID * pData, WORD wDataSize);

	bool OnRequestSaveMatchResult(DWORD dwContextID, VOID * pData, WORD wDataSize);

	bool OnRequestGetSqlTime(DWORD dwContextID, VOID * pData, WORD wDataSize);

	void OnRecord(CString strMsg);
};

//////////////////////////////////////////////////////////////////////////////////

#endif