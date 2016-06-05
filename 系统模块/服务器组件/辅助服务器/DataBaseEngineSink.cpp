#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDataBaseEngineSink::CDataBaseEngineSink()
{
	//�������
	m_pInitParameter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return;
}

//��������
CDataBaseEngineSink::~CDataBaseEngineSink()
{
}

//�ӿڲ�ѯ
VOID * CDataBaseEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngineSink,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)
{
	//��������
	if ((m_AccountsDBModule.GetInterface()==NULL)&&(m_AccountsDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	if ((m_TreasureDBModule.GetInterface()==NULL)&&(m_TreasureDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	if ((m_PlatformDBModule.GetInterface()==NULL)&&(m_PlatformDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	try
	{
		//������Ϣ
		tagDataBaseParameter * pAccountsDBParameter=&m_pInitParameter->m_AccountsDBParameter;
		tagDataBaseParameter * pTreasureDBParameter=&m_pInitParameter->m_TreasureDBParameter;
		tagDataBaseParameter * pPlatformDBParameter=&m_pInitParameter->m_PlatformDBParameter;

		//��������
		m_AccountsDBModule->SetConnectionInfo(pAccountsDBParameter->szDataBaseAddr,pAccountsDBParameter->wDataBasePort,
			pAccountsDBParameter->szDataBaseName,pAccountsDBParameter->szDataBaseUser,pAccountsDBParameter->szDataBasePass,
			"����������","AccountsDBModule");
		m_TreasureDBModule->SetConnectionInfo(pTreasureDBParameter->szDataBaseAddr,pTreasureDBParameter->wDataBasePort,
			pTreasureDBParameter->szDataBaseName,pTreasureDBParameter->szDataBaseUser,pTreasureDBParameter->szDataBasePass,
			"����������","TreasureDBModule");
		m_PlatformDBModule->SetConnectionInfo(pPlatformDBParameter->szDataBaseAddr,pPlatformDBParameter->wDataBasePort,
			pPlatformDBParameter->szDataBaseName,pPlatformDBParameter->szDataBaseUser,pPlatformDBParameter->szDataBasePass,
			"����������","PlatformDBModule");

		//��������
		m_AccountsDBModule->OpenConnection();
		m_AccountsDBAide.SetDataBase(m_AccountsDBModule.GetInterface());

		//��������
		m_TreasureDBModule->OpenConnection();
		m_TreasureDBAide.SetDataBase(m_TreasureDBModule.GetInterface());

		//��������
		m_PlatformDBModule->OpenConnection();
		m_PlatformDBAide.SetDataBase(m_PlatformDBModule.GetInterface());

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		return false;
	}

	return true;
}

//ֹͣ�¼�
bool CDataBaseEngineSink::OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx)
{
	//���ö���
	m_AccountsDBAide.SetDataBase(NULL);
	m_TreasureDBAide.SetDataBase(NULL);
	m_PlatformDBAide.SetDataBase(NULL);

	//�ر�����
	if (m_AccountsDBModule.GetInterface()!=NULL)
	{
		m_AccountsDBModule->CloseConnection();
		m_AccountsDBModule.CloseInstance();
	}

	//�ر�����
	if (m_TreasureDBModule.GetInterface()!=NULL)
	{
		m_TreasureDBModule->CloseConnection();
		m_TreasureDBModule.CloseInstance();
	}

	//�ر�����
	if (m_PlatformDBModule.GetInterface()!=NULL)
	{
		m_PlatformDBModule->CloseConnection();
		m_PlatformDBModule.CloseInstance();
	}

	//�������
	m_pInitParameter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return true;
}

//ʱ���¼�
bool CDataBaseEngineSink::OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
	return false;
}

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize)
{
	return false;
}

//�����¼�
bool CDataBaseEngineSink::OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBR_GP_LOAD_GAME_LIST:			//�����б�
		{
			return OnRequestLoadGameList(dwContextID,pData,wDataSize);
		}
	case DBR_GR_LOAD_SYSTEM_MESSAGE:   //ϵͳ��Ϣ
		{
			return OnRequestLoadSystemMessage(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOAD_YESTERDAY_SCORE:
		{
			return OnRequestLoadYesterdayScore(dwContextID,pData,wDataSize);
		}
	case DBR_GP_RANK_INFO:
		{
			return OnRequestRankInfo(dwContextID,pData,wDataSize);
		}
	}

	return false;
}

//�����б�
bool CDataBaseEngineSink::OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//��������
		WORD wPacketSize=0;
		BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];

		//��������
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameTypeItem"),true);

		//��������
		wPacketSize=0;
		DBO_GP_GameType * pGameType=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GameType))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGameType=(DBO_GP_GameType *)(cbBuffer+wPacketSize);
			pGameType->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameType->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameType->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
			m_PlatformDBAide.GetValue_String(TEXT("TypeName"),pGameType->szTypeName,CountArray(pGameType->szTypeName));

			//����λ��
			wPacketSize+=sizeof(DBO_GP_GameType);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_TYPE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��ȡ����
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameKindItem"),true);

		//��������
		wPacketSize=0;
		DBO_GP_GameKind * pGameKind=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GameKind))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGameKind=(DBO_GP_GameKind *)(cbBuffer+wPacketSize);
			pGameKind->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameKind->wTypeID=m_PlatformDBAide.GetValue_WORD(TEXT("TypeID"));
			pGameKind->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameKind->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGameKind->wGameID=m_PlatformDBAide.GetValue_WORD(TEXT("GameID"));
			m_PlatformDBAide.GetValue_String(TEXT("KindName"),pGameKind->szKindName,CountArray(pGameKind->szKindName));
			m_PlatformDBAide.GetValue_String(TEXT("ProcessName"),pGameKind->szProcessName,CountArray(pGameKind->szProcessName));

			//����λ��
			wPacketSize+=sizeof(DBO_GP_GameKind);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_KIND_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��ȡ�ڵ�
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGameNodeItem"),true);

		//���ͽڵ�
		wPacketSize=0;
		DBO_GP_GameNode * pGameNode=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GameNode))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGameNode=(DBO_GP_GameNode *)(cbBuffer+wPacketSize);
			pGameNode->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGameNode->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGameNode->wJoinID=m_PlatformDBAide.GetValue_WORD(TEXT("JoinID"));
			pGameNode->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
			m_PlatformDBAide.GetValue_String(TEXT("NodeName"),pGameNode->szNodeName,CountArray(pGameNode->szNodeName));
			
			//����λ��
			wPacketSize+=sizeof(DBO_GP_GameNode);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_NODE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��ȡ����
		m_PlatformDBAide.ResetParameter();
		m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadGamePageItem"),true);

		//���Ͷ���
		wPacketSize=0;
		DBO_GP_GamePage * pGamePage=NULL;
		while (m_PlatformDBModule->IsRecordsetEnd()==false)
		{
			//������Ϣ
			if ((wPacketSize+sizeof(DBO_GP_GamePage))>sizeof(cbBuffer))
			{
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);
				wPacketSize=0;
			}

			//��ȡ��Ϣ
			pGamePage=(DBO_GP_GamePage *)(cbBuffer+wPacketSize);
			pGamePage->wKindID=m_PlatformDBAide.GetValue_WORD(TEXT("KindID"));
			pGamePage->wNodeID=m_PlatformDBAide.GetValue_WORD(TEXT("NodeID"));
			pGamePage->wSortID=m_PlatformDBAide.GetValue_WORD(TEXT("SortID"));
			pGamePage->wPageID=m_PlatformDBAide.GetValue_WORD(TEXT("PageID"));
			pGamePage->wOperateType=m_PlatformDBAide.GetValue_WORD(TEXT("OperateType"));
			m_PlatformDBAide.GetValue_String(TEXT("DisplayName"),pGamePage->szDisplayName,CountArray(pGamePage->szDisplayName));
			
			//����λ��
			wPacketSize+=sizeof(DBO_GP_GamePage);

			//�ƶ���¼
			m_PlatformDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_PAGE_ITEM,dwContextID,cbBuffer,wPacketSize);

		//��������
		DBO_GP_GameListResult GameListResult;
		ZeroMemory(&GameListResult,sizeof(GameListResult));

		//���ñ���
		GameListResult.cbSuccess=TRUE;

		//������Ϣ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString(TEXT("OnRequestLoadGameList"),TraceLevel_Exception);
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		//��������
		DBO_GP_GameListResult GameListResult;
		ZeroMemory(&GameListResult,sizeof(GameListResult));

		//���ñ���
		GameListResult.cbSuccess=FALSE;

		//������Ϣ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_GAME_LIST_RESULT,dwContextID,&GameListResult,sizeof(GameListResult));

		return false;
	}

	return true;
}

//ϵͳ��Ϣ
bool CDataBaseEngineSink::OnRequestLoadSystemMessage(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		WORD wServerID = 0;

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@wServerID"), wServerID);

		//ִ�в�ѯ
		LONG lReturnValue = m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GR_LoadSystemMessage"), true);

		//�������
		if (lReturnValue == 0)
		{
			while (true)
			{
				//�����ж�
				if (m_TreasureDBModule->IsRecordsetEnd() == true)
				{
					break;
				}

				//�������
				TCHAR szServerRange[1024] = {0};
				CString strServerRange;
				bool bSendMessage = false;
				bool bAllRoom = false;

				//��ȡ��Χ
				m_TreasureDBAide.GetValue_String(TEXT("ServerRange"), szServerRange, CountArray(szServerRange));
				szServerRange[1023] = 0;
				strServerRange.Format(TEXT("%s"), szServerRange);

				//��Χ�ж�
				while (true)
				{
					int nfind = strServerRange.Find(TEXT(','));
					if (nfind != -1 && nfind > 0)
					{
						CString strID = strServerRange.Left(nfind);
						WORD wServerID = StrToInt(strID);
						bSendMessage = (wServerID == 0 || wServerID == wServerID);
						if (wServerID == 0)
						{
							bAllRoom = true;
						}

						if (bSendMessage)
						{
							break;
						}

						strServerRange = strServerRange.Right(strServerRange.GetLength()-nfind-1);
					}
					else
					{
						WORD wServerID = StrToInt(szServerRange);
						bSendMessage = (wServerID == 0 || wServerID == wServerID);
						if (wServerID == 0)
						{
							bAllRoom = true;
						}

						break;
					}
				}

				//������Ϣ
				if (bSendMessage)
				{
					DBR_GR_SystemMessage SystemMessage = {0};

					//��ȡ��Ϣ
					SystemMessage.cbMessageID = m_TreasureDBAide.GetValue_BYTE(TEXT("ID"));
					SystemMessage.cbMessageType = m_TreasureDBAide.GetValue_BYTE(TEXT("MessageType"));
					SystemMessage.dwTimeRate = m_TreasureDBAide.GetValue_DWORD(TEXT("TimeRate"));
					SystemMessage.cbAllRoom = bAllRoom ? TRUE : FALSE;
					m_TreasureDBAide.GetValue_String(TEXT("MessageString"), SystemMessage.szSystemMessage, CountArray(SystemMessage.szSystemMessage));

					//���ͽ��
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GR_LOAD_SYSTEM_MESSAGE, dwContextID, &SystemMessage, sizeof(SystemMessage));
				}

				//��һ��
				m_TreasureDBModule->MoveToNext();
			}
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CTraceService::TraceString(TEXT("GSP_GR_LoadSystemMessage"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

//�������ս������
bool CDataBaseEngineSink::OnRequestLoadYesterdayScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		m_PlatformDBAide.ResetParameter();
		LONG lReturnValue = m_PlatformDBAide.ExecuteProcess(TEXT("GSP_GP_LoadYesterdayScore"), true);

		if (lReturnValue == 0)
		{
			DBO_GP_YesterdayScore YesterdayScore = {0};
			YesterdayScore.lYesterdayScore = m_PlatformDBAide.GetValue_LONGLONG("lYesterdayScore");
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_LOAD_YESTERDAY_SCORE, dwContextID, &YesterdayScore, sizeof(YesterdayScore));
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		CTraceService::TraceString(TEXT("GSP_GP_LoadYesterdayScore"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);
		return false;
	}

	return true;
}

bool CDataBaseEngineSink::OnRequestRankInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GP_RankInfo));
		if (wDataSize!=sizeof(DBR_GP_RankInfo)) return false;

		WORD wPacketSize = 0;
		BYTE cbBuffer[MAX_ASYNCHRONISM_DATA];

		//������
		DBR_GP_RankInfo * pRankInfo=(DBR_GP_RankInfo *)pData;

		//�������
		m_TreasureDBAide.ResetParameter();
		m_TreasureDBAide.AddParameter(TEXT("@type"),pRankInfo->nType);
		LONG lResultCode=m_TreasureDBAide.ExecuteProcess(TEXT("GSP_GP_GetRankInfo"),true);

		DB0_GP_RankInfo * pCompRankInfo=NULL;
		while (m_TreasureDBModule->IsRecordsetEnd()==false)
		{
			//��ȡ��Ϣ
			pCompRankInfo=(DB0_GP_RankInfo *)(cbBuffer+wPacketSize);
			pCompRankInfo->nUserID=m_TreasureDBAide.GetValue_LONG(TEXT("userid"));
			pCompRankInfo->lMatchScore=m_TreasureDBAide.GetValue_LONGLONG(TEXT("count"));
			wPacketSize = wPacketSize + sizeof(DB0_GP_RankInfo);
			if (wPacketSize > MAX_ASYNCHRONISM_DATA)
			{
				break;
			}
			//�ƶ���¼
			m_TreasureDBModule->MoveToNext();
		}
		if (wPacketSize>0) m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBO_GP_RANK_INFO,dwContextID,cbBuffer,wPacketSize);
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		CTraceService::TraceString(TEXT("GSP_GP_GetRankInfo"),TraceLevel_Exception);
		CTraceService::TraceString(pIException->GetExceptionDescribe(),TraceLevel_Exception);

		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
