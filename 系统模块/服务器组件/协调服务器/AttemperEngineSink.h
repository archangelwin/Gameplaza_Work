#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "GlobalInfoManager.h"
#include <list>
//////////////////////////////////////////////////////////////////////////////////
//数据定义

//服务类型
enum enServiceKind
{
	ServiceKind_None,				//无效服务
	ServiceKind_Game,				//游戏服务
	ServiceKind_Plaza,				//广场服务
};

//绑定参数
struct tagBindParameter
{
	//网络数据
	DWORD							dwSocketID;							//网络标识
	DWORD							dwClientAddr;						//连接地址
	DWORD							dwActiveTime;						//激活时间

	//附加数据
	WORD							wServiceID;							//服务标识
	enServiceKind					ServiceKind;						//服务类型
};

//数组定义
typedef CWHArray<WORD> CWordArrayTemplate;

struct tagLotteryInfo
{
	int nPrice;
	int nMaxGet;
	int nLessTime;
	int nPercent;
	int nMinusStorage;
	bool bHave;

	int nSendSecond;
	__int64 nSerial;
	WORD wServerID;
};

struct tagServerPlayerCount
{
	WORD wServerID;
	DWORD dwPlayerCount;
	DWORD dwGrowCount;
	TCHAR szServerName[LEN_SERVER];
};

struct tagMatchRankInfo
{
	DWORD dwUserID;
	bool bActive;
	WORD wServerID;
	SCORE lMatchScore;
	int nMatchRank;
	int nLastMatchRank;
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
	TCHAR szMatchName[128];
};

struct Sort_Match_Score
{
	inline bool operator() (const tagMatchItem& struct1, const tagMatchItem& struct2)
	{
		return (struct1.lMatchScore > struct2.lMatchScore);
	}
};

//////////////////////////////////////////////////////////////////////////////////

//调度钩子
class CAttemperEngineSink : public IAttemperEngineSink
{
	//友元定义
	friend class CServiceUnits;

	//状态变量
protected:
	WORD							m_wCollectItem;						//汇总连接
	CWordArrayTemplate				m_WaitCollectItemArray;				//汇总等待
	int								m_nLastSendElapse;
	bool							m_bInitLotteryInfo;
	std::vector<tagLotteryInfo>		m_vLotteryInfo;
	std::vector<tagMatchRankInfo>	m_vMatchRankInfo;
	std::vector<std::vector<WORD> > m_vMatchGameServerID;
	std::vector<std::vector<std::vector<tagMatchItem> > > m_vMatchData;	//协调服务器用户信息汇总
	int								m_nMessageRemindTime1;				//距离比赛开始信息展示提醒时间1
	int								m_nMessageRemindTime2;				//距离比赛开始信息展示提醒时间2
	int								m_nMessageRemindTime3;				//距离比赛开始信息展示提醒时间3
	int								m_nMatchBeginRemindTime;			//距离比赛开始提醒时间
	int								m_nNewPlayerServerId;				//新手房房间id
	int								m_nHundredServerId;					//百炮房房间id
	int								m_nThousandServerId;				//千炮房房间id
	int								m_nTenThousandServerId;				//万炮房房间id
	std::list<tagMatchConfigItem>	m_MatchConfigItemList;				//比赛配置数据
	TCHAR							m_szMatchBeginTime[20];
	WORD							m_wGroupCnt;						//比赛组数
	int								m_nCurrentMatchID;							//当前进行的比赛的ID
	WORD							m_wUserNumPerGroup;
	WORD							m_wAndroidFakeNameCnt;
	WORD							m_CurrentFakeNameNum;
	int								m_IsRemind[3];
	int								m_nDifferentTime;					
	bool							m_bIsGameBegin;
	// Time interval
	//WORD							m_wSendFakeInfoInterval;

	//变量定义
protected:
	CInitParameter *				m_pInitParameter;					//配置参数
	tagBindParameter *				m_pBindParameter;					//辅助数组

	//组件变量
protected:
	CGlobalInfoManager				m_GlobalInfoManager;				//全局管理

	//组件接口
protected:
	ITimerEngine *					m_pITimerEngine;					//时间引擎
	ITCPNetworkEngine *				m_pITCPNetworkEngine;				//网络引擎
	IDataBaseEngine *				m_pIDataBaseEngine;					//数据引擎

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

	//网络事件
protected:
	//注册服务
	bool OnTCPNetworkMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//服务状态
	bool OnTCPNetworkMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//用户汇总
	bool OnTCPNetworkMainUserCollect(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//远程服务
	bool OnTCPNetworkMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//管理服务
	bool OnTCPNetworkMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//辅助函数
protected:
	//发送列表
	bool SendServerListItem(DWORD dwSocketID);

	//数据发送
protected:
	//房间发送
	bool SendDataToGame(WORD wServerID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//用户发送
	bool SendDataToUser(WORD wServerID, DWORD dwUserID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//生成话费数据
	void GenLotteryInfo(int nMaxSend, int nSendElapse, tagLotteryInfo & LotteryInfo);
	//初始化话费数据
	void InitLotteryInfo();
	//获取下一比赛配置信息
	void GetNextMatchInfo(tagMatchConfigItem & MatchConfigItem);
	//Get Current Matchi Info
	void GetCurrentMatchInfo(int nMatchNum, tagMatchConfigItem & MatchConfigItem );

	//发送比赛通知
	bool SendMatchNotifyInfoBatch(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	bool SendMatchUserInfoBatch(WORD wMainCmdID, WORD wSubCmdID);
	bool SendMatchEndInfoToServer(int nMatchType);
	time_t StringToDatetime(const char *strTime);
	void OnRecord(CString strMsg);
};

//////////////////////////////////////////////////////////////////////////////////

#endif