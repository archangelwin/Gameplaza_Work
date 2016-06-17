#ifndef DATABASE_ENGINE_SINK_HEAD_FILE
#define DATABASE_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "DataBasePacket.h"

//////////////////////////////////////////////////////////////////////////////////

#define DB_NO_AVAILABLE_ACCOUNT	1									//û�п����˻�
#define DB_ACCINFO_ALREADYEXIST		7									//�˻��Ѿ�����
#define DB_GUEST_REG_SUCCESS		21									//�ο�ע��ɹ�

#define DB_QUERY_SIGNIN_SUCCESS     22									//ǩ����ѯ�ɹ�
#define DB_QUERY_SIGNIN_FAILURE		23									//ǩ����ѯʧ��
#define DB_USER_SIGNIN_SUCCESS		24									//���ǩ���ɹ�
#define DB_USER_SIGNIN_FAILURE		25									//���ǩ��ʧ��
#define DB_USER_SIGNIN_ALREADY		26									//�û���ǩ��
#define DB_USER_BQ_SUCCESS			27									//��ǩ�ɹ�

#define DB_QUERY_LOTTERY_SUCCESS    30									//�齱��ѯ�ɹ�
#define DB_QUERY_LOTTERY_FAILURE    31									//�齱��ѯʧ��
#define DB_USER_LOTTERY_SUCCESS     32									//�齱�����ɹ�
#define DB_USER_LOTTERY_FAILURE     33									//�齱����ʧ��
#define DB_GET_PRIZEITEM_SUCCESS    34									//�н��ȼ���ȡ�ɹ�
#define DB_GET_PRIZEITEM_FAILURE    35									//�н��ȼ���ȡʧ��

//���ݿ���
class CDataBaseEngineSink : public IDataBaseEngineSink
{
	//��Ԫ����
	friend class CServiceUnits;

	//��Ա����
private:
	UINT m_uiRandCodeCnt;													//�������

	//�û����ݿ�
protected:
	CDataBaseAide					m_AccountsDBAide;					//�û����ݿ�
	CDataBaseHelper					m_AccountsDBModule;					//�û����ݿ�

	//��Ϸ�����ݿ�
protected:
	CDataBaseAide					m_TreasureDBAide;					//��Ϸ�����ݿ�
	CDataBaseHelper					m_TreasureDBModule;					//��Ϸ�����ݿ�

	//ƽ̨���ݿ�
protected:
	CDataBaseAide					m_PlatformDBAide;					//ƽ̨���ݿ�
	CDataBaseHelper					m_PlatformDBModule;					//ƽ̨���ݿ�

	//��־���ݿ�
protected:
	CDataBaseAide					m_LogServerDBAide;					//��־���ݿ�
	CDataBaseHelper					m_LogServerDBModule;				//��־���ݿ�

	//�������
protected:
	CInitParameter *				m_pInitParameter;					//���ò���
	IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//�����¼�

	//��������
public:
	//���캯��
	CDataBaseEngineSink();
	//��������
	virtual ~CDataBaseEngineSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//ϵͳ�¼�
public:
	//�����¼�
	virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx);
	//ֹͣ�¼�
	virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx);

	//�ں��¼�
public:
	//ʱ���¼�
	virtual bool OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//�����¼�
	virtual bool OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize);
	//�����¼�
	virtual bool OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//������������û�
	bool OnRequestInsertMatchSignUpUser(DWORD dwContextID, VOID * pData, WORD wDataSize);

	bool OnRequestLoadMatchConfig(DWORD dwContextID, VOID * pData, WORD wDataSize);

	bool OnRequestSaveMatchResult(DWORD dwContextID, VOID * pData, WORD wDataSize);

	bool OnRequestGetSqlTime(DWORD dwContextID, VOID * pData, WORD wDataSize);

	void OnRecord(CString strMsg);
};

//////////////////////////////////////////////////////////////////////////////////

#endif