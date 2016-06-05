#include "StdAfx.h"
#include "Resource.h"
#include "TableFrameSink.h"
#include "GameServiceManager.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameServiceManager::CGameServiceManager()
{
	//�ں�����
	m_GameServiceAttrib.wKindID=KIND_ID;
	m_GameServiceAttrib.wChairCount=GAME_PLAYER;
	m_GameServiceAttrib.wSupporType=(GAME_GENRE_GOLD|GAME_GENRE_SCORE|GAME_GENRE_EDUCATE);

	//���ܱ�־
	m_GameServiceAttrib.cbDynamicJoin=TRUE;
	m_GameServiceAttrib.cbAndroidUser=TRUE;
	m_GameServiceAttrib.cbOffLineTrustee=FALSE;

	//��������
	m_GameServiceAttrib.dwServerVersion=VERSION_SERVER;
	m_GameServiceAttrib.dwClientVersion=VERSION_CLIENT;
	lstrcpyn(m_GameServiceAttrib.szGameName,GAME_NAME,CountArray(m_GameServiceAttrib.szGameName));
	lstrcpyn(m_GameServiceAttrib.szDataBaseName,TEXT("QPTreasureDB"),CountArray(m_GameServiceAttrib.szDataBaseName));
	lstrcpyn(m_GameServiceAttrib.szClientEXEName,TEXT("SuperClown.exe"),CountArray(m_GameServiceAttrib.szClientEXEName));
	lstrcpyn(m_GameServiceAttrib.szServerDLLName,TEXT("SuperClown.dll"),CountArray(m_GameServiceAttrib.szServerDLLName));

	return;
}

//��������
CGameServiceManager::~CGameServiceManager()
{
}

//�ӿڲ�ѯ
VOID * CGameServiceManager::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameServiceManager,Guid,dwQueryVer);
	QUERYINTERFACE(IGameServiceCustomRule,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameServiceManager,Guid,dwQueryVer);
	return NULL;
}

//��������
VOID * CGameServiceManager::CreateTableFrameSink(REFGUID Guid, DWORD dwQueryVer)
{
	//��������
	CTableFrameSink * pTableFrameSink=NULL;

	try
	{
		//��������
		pTableFrameSink=new CTableFrameSink();
		if (pTableFrameSink==NULL) throw TEXT("����ʧ��");

		//��ѯ�ӿ�
		VOID * pObject=pTableFrameSink->QueryInterface(Guid,dwQueryVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");

		return pObject;
	}
	catch (...) {}

	//ɾ������
	SafeDelete(pTableFrameSink);

	return NULL;
}

//��������
VOID * CGameServiceManager::CreateAndroidUserItemSink(REFGUID Guid, DWORD dwQueryVer)
{
	////��������
	//CAndroidUserItemSink * pAndroidUserItemSink=NULL;

	//try
	//{
	//	//��������
	//	pAndroidUserItemSink=new CAndroidUserItemSink();
	//	if (pAndroidUserItemSink==NULL) throw TEXT("����ʧ��");

	//	//��ѯ�ӿ�
	//	void * pObject=pAndroidUserItemSink->QueryInterface(Guid,dwQueryVer);
	//	if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");

	//	return pObject;
	//}
	//catch (...) {}

	////ɾ������
	//SafeDelete(pAndroidUserItemSink);
	return NULL;
}

//��������
VOID * CGameServiceManager::CreateGameDataBaseEngineSink( REFGUID Guid, DWORD dwQueryVer )
{
	QUERYINTERFACE(IGameServiceManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameServiceManager,Guid,dwQueryVer);
	return NULL;
}

//�������
bool CGameServiceManager::GetServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)
{
	//���ñ���
	GameServiceAttrib=m_GameServiceAttrib;

	return true;
}

//��������
bool CGameServiceManager::RectifyParameter(tagGameServiceOption & GameServiceOption)
{
	//Ч�����
	ASSERT(&GameServiceOption!=NULL);
	if (&GameServiceOption==NULL)
		printf("%s, %d, ERROR!!\n", __FUNCTION__, __LINE__);
	//��Ԫ����
	GameServiceOption.lCellScore=GameServiceOption.lCellScore;

	//�����Ϸ
	if (GameServiceOption.wServerType&(GAME_GENRE_GOLD|SCORE_GENRE_POSITIVE))
	{
		GameServiceOption.lMinTableScore=GameServiceOption.lMinTableScore;
	}

	//��������
	if (GameServiceOption.lRestrictScore!=0L)
	{
		GameServiceOption.lRestrictScore=__max(GameServiceOption.lRestrictScore,GameServiceOption.lMinTableScore);
	}

	return true;
}

//��ȡ����
bool CGameServiceManager::SaveCustomRule( LPBYTE pcbCustomRule, WORD wCustonSize )
{
	return true;
}

//Ĭ������
bool CGameServiceManager::DefaultCustomRule( LPBYTE pcbCustomRule, WORD wCustonSize )
{
	return true;
}

//��������
HWND CGameServiceManager::CreateCustomRule(CWnd * pParentWnd, CRect rcCreate, LPBYTE pcbCustomRule, WORD wCustonSize)
{
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) VOID * CreateGameServiceManager(const GUID & Guid, DWORD dwInterfaceVer)
{
	static CGameServiceManager GameServiceManager;
	return GameServiceManager.QueryInterface(Guid,dwInterfaceVer);
}

//////////////////////////////////////////////////////////////////////////////////
