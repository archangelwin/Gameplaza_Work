#include "StdAfx.h"
#include "DBCorrespondManager.h"

//////////////////////////////////////////////////////////////////////////
//���캯��
CDBCorrespondManager::CDBCorrespondManager(void)
{
	m_bService=false;
	m_pIKernelDataBaseEngine = NULL;
}

//��������
CDBCorrespondManager::~CDBCorrespondManager(void)
{
	m_pIKernelDataBaseEngine=NULL;

	ClearAmortizeData();

	m_DBRequestUserArray.RemoveAll();
}

//�ӿڲ�ѯ
VOID * CDBCorrespondManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDBCorrespondManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDBCorrespondManager,Guid,dwQueryVer);
	return NULL;
}

//��������
bool CDBCorrespondManager::StartService()
{
	ASSERT(!m_bService);
	m_bService=true;
	return true;
}

//ֹͣ����
bool CDBCorrespondManager::ConcludeService()
{
	CWHDataLocker DataLocker(m_AsyncCritical);

	m_bService=false;

	ClearAmortizeData();

	m_DBRequestUserArray.RemoveAll();

	return true;
}

//����ģ��
bool __cdecl CDBCorrespondManager::InitDBCorrespondManager(IDataBaseEngine * pIDataBaseEngine)
{
	m_pIKernelDataBaseEngine = pIDataBaseEngine;
	return true;
}

//�����¼�
bool __cdecl CDBCorrespondManager::PostDataBaseRequest(DWORD dwUserID, WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize, BYTE  cbCache)
{
	//����ӳ��
	CWHDataLocker DataLocker(m_AsyncCritical);

	//���洦��
	AmortizeSyncData(dwUserID, wRequestID,dwContextID,pData,wDataSize,cbCache);

	//ִ�л���
	PerformAmortisation();

	return true;
}

//�������
bool __cdecl CDBCorrespondManager::OnPostRequestComplete(DWORD dwUserID, bool bSucceed)
{
	//����ӳ��
	CWHDataLocker DataLocker(m_AsyncCritical);

	// test
	FILE *FSPOINTER;
	FSPOINTER = fopen("msgrecord.TXT", "a+");
	

	//���Ͷ�ݼ�¼
	INT_PTR nIndex = GetUserArrayIndex(dwUserID);
	if(nIndex != -1) 
	{
		fprintf(FSPOINTER, TEXT("OnPostRequestComplete UserID = %ld,RequestUserArray removed \n"), dwUserID);
		m_DBRequestUserArray.RemoveAt(nIndex);
	}
		

	//�������
	POSITION pos = m_DBRequestList.GetHeadPosition();
	while(pos != NULL)
	{
		POSITION tempPos = pos;
		tagDBRequestHead *pDBRequestHead = m_DBRequestList.GetNext(pos);
		if(pDBRequestHead->dwUserID == dwUserID)
		{
			SYSTEMTIME sys; 
			GetLocalTime(&sys);
			fprintf(FSPOINTER, TEXT("RequestList UserID In = %ld,UserID found, request id = %d,����%d:%d:%d:%d \n"), dwUserID, pDBRequestHead->wRequestID,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
			//���
			if(pDBRequestHead->cbCache == FALSE || bSucceed)
			{
				fprintf(FSPOINTER, TEXT("RequestList Remove UserID = %ld, request id = %d  \n"), dwUserID, pDBRequestHead->wRequestID);
				m_DBRequestList.RemoveAt(tempPos);
				delete [] ((BYTE*)pDBRequestHead);
			}

			break;
		}
	}

	// test
	fclose(FSPOINTER);

	//ִ�л���
	PerformAmortisation();

	return true;
}

//��ʱ�¼�
bool __cdecl CDBCorrespondManager::OnTimerNotify()
{
	//����ӳ��
	CWHDataLocker DataLocker(m_AsyncCritical);

	//ִ�л���
	PerformAmortisation();

	return true;
}

//�Ѿ��ύ����
bool CDBCorrespondManager::IsPostDBRequest(DWORD dwUserID)
{
	for(INT_PTR i=0; i<m_DBRequestUserArray.GetCount(); i++)
	{
		if(dwUserID == m_DBRequestUserArray[i]) return true;
	}

	return false;
}

//�������
INT_PTR CDBCorrespondManager::GetUserArrayIndex(DWORD dwUserID)
{
	for(INT_PTR i=0; i<m_DBRequestUserArray.GetCount(); i++)
	{
		if(dwUserID == m_DBRequestUserArray[i]) return i;
	}

	return -1;
}

//��������
bool CDBCorrespondManager::AmortizeSyncData(DWORD dwUserID, WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize, BYTE  cbCache)
{
	//�������
	WORD wBuffSize = sizeof(tagDBRequestHead)+wDataSize;
	BYTE *pBuff = new BYTE[wBuffSize];
	ZeroMemory(pBuff, wBuffSize);
	tagDBRequestHead *pDBRequestHead = (tagDBRequestHead*)pBuff;

	//�������
	pDBRequestHead->cbCache = cbCache;
	pDBRequestHead->dwUserID = dwUserID;
	pDBRequestHead->dwContextID = dwContextID;
	pDBRequestHead->wRequestID  = wRequestID;
	pDBRequestHead->wDataSize   = wDataSize;
	CopyMemory(((VOID*)(pDBRequestHead+1)), pData, wDataSize);

	//��������
	m_DBRequestList.AddTail(pDBRequestHead);

	return true;
}

//ִ�л���
VOID CDBCorrespondManager::PerformAmortisation()
{
	// test
	FILE *FSPOINTER;
	FSPOINTER = fopen("msgrecord.TXT", "a+");

	//��������
	POSITION pos = m_DBRequestList.GetHeadPosition();
	while(pos != NULL)
	{
		tagDBRequestHead *pDBRequestHead = m_DBRequestList.GetNext(pos);

		//Ͷ������
		if(!IsPostDBRequest(pDBRequestHead->dwUserID))
		{
			if(m_pIKernelDataBaseEngine)
			{
				fprintf(FSPOINTER, TEXT("request user array add wRequest ID  = %d, Userid = %ld\n"), pDBRequestHead->wRequestID, pDBRequestHead->dwUserID);
				m_DBRequestUserArray.Add(pDBRequestHead->dwUserID);
				m_pIKernelDataBaseEngine->PostDataBaseRequest(pDBRequestHead->wRequestID, pDBRequestHead->dwContextID, (VOID*)(pDBRequestHead+1), pDBRequestHead->wDataSize);
			}
		}
		//else
		//{
		//	printf("û�д������Ϣ=%d\n",pDBRequestHead->wRequestID);
		//}
	}

	fclose(FSPOINTER);
}

//�������
VOID CDBCorrespondManager::ClearAmortizeData()
{
	while(m_DBRequestList.GetCount() > 0)
	{
		tagDBRequestHead * pRqHead = m_DBRequestList.RemoveHead();
		if(pRqHead)
		{
			delete [] ((BYTE*)pRqHead);
		}
	}
}
