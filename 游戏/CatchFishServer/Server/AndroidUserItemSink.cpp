#include "Stdafx.h"

//��ʱ��
enum enAndroidTimer
{
	TimerShoot,//�����ӵ�
	TimerAddMoney,//�Ϸ�
	TimerChangeCannon,//��̨�任
};

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_nAllScore=0;
	m_nMultiple=0;
	m_bShoot=false;
	m_bHaveBuy=false;
	//�ӿڱ���
	m_pIAndroidUserItem=NULL;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
VOID * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ�ӿ�
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;
	return true;
}

//���ýӿ�
bool CAndroidUserItemSink::RepositionSink()
{
	return true;
}

//ʱ����Ϣ
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	return false;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
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

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//�û�����
VOID CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
VOID CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
VOID CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
VOID CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û���λ
VOID CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(VOID * pData, WORD wDataSize)
{
	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(VOID * pData, WORD wDataSize)
{
	return true;
}
