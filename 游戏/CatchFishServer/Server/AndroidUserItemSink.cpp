#include "Stdafx.h"

//定时器
enum enAndroidTimer
{
	TimerShoot,//发射子弹
	TimerAddMoney,//上分
	TimerChangeCannon,//炮台变换
};

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_nAllScore=0;
	m_nMultiple=0;
	m_bShoot=false;
	m_bHaveBuy=false;
	//接口变量
	m_pIAndroidUserItem=NULL;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
VOID * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//初始接口
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;
	return true;
}

//重置接口
bool CAndroidUserItemSink::RepositionSink()
{
	return true;
}

//时间消息
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	return false;
}

//游戏消息
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//场景消息
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	m_pIAndroidUserItem->SetGameStatus(GAME_SCENE_PLAY);
	CMD_S_GameScene * pGameScene=(CMD_S_GameScene *)pData;
	m_nMultiple=pGameScene->lCellScore;

	return true;
}

WORD CAndroidUserItemSink::SwitchChairID(WORD wChairID)
{
	WORD wResult=wChairID;
	switch(m_pIAndroidUserItem->GetChairID())
	{
	case 0:
	case 1:
	case 2:
		{
			switch(wChairID)
			{
			case 0:return 0;
			case 1:return 1;
			case 2:return 2;
			case 3:return 3;
			case 4:return 4;
			case 5:return 5;
			}
		}
	case 3:
	case 4:
	case 5:
		{
			switch(wChairID)
			{
			case 0:return 3;
			case 1:return 4;
			case 2:return 5;
			case 3:return 0;
			case 4:return 1;
			case 5:return 2;
			}
		}
	}
	return wResult;
}

//游戏消息
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//用户进入
VOID CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户离开
VOID CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户积分
VOID CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
VOID CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户段位
VOID CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(VOID * pData, WORD wDataSize)
{
	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(VOID * pData, WORD wDataSize)
{
	return true;
}
