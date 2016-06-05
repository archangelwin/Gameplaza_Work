#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS						1									//����ʱ��

//��Ϸʱ��


#define TIME_CHECK_BANKER				30									//̯��ʱ��

//��Ϸʱ��
#define IDI_START_GAME					(100)									//��ʼ��ʱ��
#define IDI_CALL_BANKER					(101)									//��ׯ��ʱ��
#define IDI_USER_ADD_SCORE				(102)									//��ע��ʱ��
#define IDI_OPEN_CARD					(103)									//���ƶ�ʱ��

#define IDI_CHECK_BANKER_OPERATE        (104)									//��鶨ʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_nRobotBankStorageMul=0;
	m_lRobotBankGetScore=0;
	m_lRobotBankGetScoreBanker=0;
	ZeroMemory(m_lRobotScoreRange,sizeof(m_lRobotScoreRange));

	//�ӿڱ���
	m_pIAndroidUserItem=NULL;
	srand((unsigned)time(NULL));   

	return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	printf("�ӿڲ�ѯ\n");
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

	ReadConfigInformation();

	//�������
	UINT nElapse=TIME_CHECK_BANKER+rand()%100;
	m_pIAndroidUserItem->SetGameTimer(IDI_CHECK_BANKER_OPERATE,nElapse);

	return true;
}

//���ýӿ�
bool CAndroidUserItemSink::RepositionSink()
{
	printf("���ýӿ� %d\n",__LINE__);
	printf("------------------------------------------\n");

	//�������
	UINT nElapse=TIME_CHECK_BANKER+rand()%100;
	m_pIAndroidUserItem->SetGameTimer(IDI_CHECK_BANKER_OPERATE,nElapse);

	return true;
}

//ʱ����Ϣ
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_CHECK_BANKER_OPERATE://�������
		{

			IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
			if(pUserItem->GetUserStatus()<US_SIT)
			{
				ReadConfigInformation();
				BankOperate(1);
			}
			//�������
			UINT nElapse=TIME_CHECK_BANKER+rand()%100;
			m_pIAndroidUserItem->SetGameTimer(IDI_CHECK_BANKER_OPERATE,nElapse);

			return true;
		}
	}

	return false;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//switch (wSubCmdID)
	//{
	//case SUB_S_CALL_BANKER:	//�û���ׯ
	//	{
	//		//��Ϣ����
	//		return OnSubCallBanker(pData,wDataSize);
	//	}
	//case SUB_S_GAME_START:	//��Ϸ��ʼ
	//	{
	//		//��Ϣ����
	//		return OnSubGameStart(pData,wDataSize);
	//	}
	//case SUB_S_ADD_SCORE:	//�û���ע
	//	{
	//		//��Ϣ����
	//		return OnSubAddScore(pData,wDataSize);
	//	}
	//case SUB_S_SEND_CARD:	//������Ϣ
	//	{
	//		//��Ϣ����
	//		return OnSubSendCard(pData,wDataSize);
	//	}
	//case SUB_S_OPEN_CARD:	//�û�̯��
	//	{
	//		//��Ϣ����
	//		return OnSubOpenCard(pData,wDataSize);
	//	}
	//case SUB_S_PLAYER_EXIT:	//�û�ǿ��
	//	{
	//		//��Ϣ����
	//		return OnSubPlayerExit(pData,wDataSize);
	//	}
	//case SUB_S_GAME_END:	//��Ϸ����
	//	{
	//		//��Ϣ����
	//		return OnSubGameEnd(pData,wDataSize);
	//	}
	//case SUB_S_ALL_CARD:
	//	{
	//		return true;
	//	}
	//case SUB_S_BANKER_OPERATE:
	//	{
	//		//BankOperate();
	//		return true;
	//	}
	//}

	//�������
	ASSERT(FALSE);

	return true;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
printf("Android ��Ϸ��Ϣ %d\n",__LINE__);
	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	printf("Android ������Ϣ %d\n",__LINE__);
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_READY_STATUS)) return false;

			//��Ϣ����
			CMD_S_READY_STATUS * pStatusFree=(CMD_S_READY_STATUS *)pData;

			//memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));

			ReadConfigInformation();

			BankOperate(2);

			//��ʼʱ��
			UINT nElapse=rand()%(15)+3;
			m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//�û�����
void CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	printf("�û�����\n");
	return;
}

//�û��뿪
void CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	printf("�û��뿪\n");
	return;
}

//�û�����
void CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
void CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û���λ
void CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û���ׯ
bool CAndroidUserItemSink::OnSubCallBanker(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//�û���ע
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{


	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//�û�̯��
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{

	return true;
}

//�û�ǿ��
bool CAndroidUserItemSink::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	return true;
}

//���в���
void CAndroidUserItemSink::BankOperate(BYTE cbType)
{
	IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
	if(pUserItem->GetUserStatus()>=US_SIT)
	{
		if(cbType==1)
		{
			CString strInfo;
			strInfo.Format(TEXT("������״̬���ԣ���ִ�д�ȡ��"));
		}
		return;
	}

	//��������
	LONGLONG lRobotScore = pUserItem->GetUserScore();

	CString strInfo;
	strInfo.Format(TEXT("������Ϣ��ȡ������(%I64d)���������(%I64d),�����˷���(%I64d)"),m_lRobotScoreRange[0],m_lRobotScoreRange[1],lRobotScore);
}

//��ȡ����
void CAndroidUserItemSink::ReadConfigInformation()
{
	printf("��ȡ���� %d\n",__LINE__);
	//�����ļ���
	TCHAR szPath[MAX_PATH] = TEXT("");
	TCHAR szConfigFileName[MAX_PATH] = TEXT("");
	TCHAR OutBuf[255] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	myprintf(szConfigFileName, sizeof(szConfigFileName), _T("%s\\OxConfig.ini"), szPath);

	//��������
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("RobotScoreMin"), _T("100000"), OutBuf, 255, szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotScoreRange[0]);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("RobotScoreMax"), _T("10000000"), OutBuf, 255, szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotScoreRange[1]);

	if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	m_lRobotScoreRange[1] = m_lRobotScoreRange[0];

	//�������

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("RobotBankGet"), _T("1000000"), OutBuf, 255, szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotBankGetScore);

	//������� (ׯ��)
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("RobotBankGetBanker"), _T("500000"), OutBuf, 255, szConfigFileName);
	myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &m_lRobotBankGetScoreBanker);

	//����
	m_nRobotBankStorageMul = GetPrivateProfileInt(m_szRoomName, _T("RobotBankStoMul"), 20, szConfigFileName);

	if (m_nRobotBankStorageMul<0||m_nRobotBankStorageMul>100) m_nRobotBankStorageMul =20;
}


//�����������
DECLARE_CREATE_MODULE(AndroidUserItemSink);

