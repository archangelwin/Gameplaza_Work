#ifndef DATABASE_ENGINE_SINK_HEAD_FILE
#define DATABASE_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "DataBasePacket.h"

//////////////////////////////////////////////////////////////////////////////////

//�����
struct tagDBR_GR_CheckStock
{
	LONG								lCaiJin;
	LONG								lCaiJinSent;
	LONG								lTypeID;
	LONG								lRoomID;
	LONG								lTableID;
	LONG								lStockScore;
	LONG								lRevenue;
	BOOL								lbLoad;	
	LONG								lUserID;
};

struct tagDBR_GR_CheckStockSuccess
{
	LONG								lType;
	LONG								lRoomID;
	LONG								lTableID;
	LONG								lStockScore;
	LONG								lRevenue;
	LONG								lStockChange;
	LONG								lCaiJin;
	LONG								lCaiJinSent;
	BOOL								lbSuccess;
};

struct DBR_GR_UserLucky
{
	DWORD								dwUserID;
	DWORD								dwSerial;
	float								fLucky;
};

//���ݿ���
class CDataBaseEngineSink : public IDataBaseEngineSink, public IGameDataBaseEngine
{
	//��Ԫ����
	friend class CServiceUnits;

	//��������
protected:
	DBO_GR_LogonFailure				m_LogonFailure;						//��¼ʧ��
	DBO_GR_LogonSuccess				m_LogonSuccess;						//��¼�ɹ�

	//��Ϸ���ݿ�
protected:
	CDataBaseAide					m_GameDBAide;						//��Ϸ����
	CDataBaseHelper					m_GameDBModule;						//��Ϸ����

	//��Ϸ�����ݿ�
protected:
	CDataBaseAide					m_TreasureDBAide;					//��Ϸ�����ݿ�
	CDataBaseHelper					m_TreasureDBModule;					//��Ϸ�����ݿ�

	//ƽ̨���ݿ�
protected:
	CDataBaseAide					m_PlatformDBAide;					//ƽ̨���ݿ�
	CDataBaseHelper					m_PlatformDBModule;					//ƽ̨���ݿ�

	//���ñ���
protected:
	CDataBaseAide					m_LogServerDBAide;					//��־���ݿ�
	CDataBaseHelper					m_LogServerDBModule;				//��־����
protected:
	CInitParameter *				m_pInitParameter;					//���ò���
	tagGameParameter *				m_pGameParameter;					//���ò���
	tagDataBaseParameter *			m_pDataBaseParameter;				//������Ϣ
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//��������
	tagGameServiceOption *			m_pGameServiceOption;				//��������

	//�������
protected:
	IDataBaseEngine *				m_pIDataBaseEngine;					//����ӿ�
	IGameServiceManager *			m_pIGameServiceManager;				//�������
	IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//�����¼�

	//��ѯ�ӿ�
protected:
	IGameDataBaseEngineSink *		m_pIGameDataBaseEngineSink;			//���ݽӿ�

	//�������
public:
	IDBCorrespondManager *          m_pIDBCorrespondManager;            //����Э��

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

	//���ò���
public:
	//�Զ�����
	virtual VOID * GetCustomRule() { return m_pGameServiceOption->cbCustomRule; };
	//��������
	virtual tagGameServiceAttrib * GetGameServiceAttrib() { return m_pGameServiceAttrib; }
	//��������
	virtual tagGameServiceOption * GetGameServiceOption() { return m_pGameServiceOption; }

	//��ȡ����
public:
	//��ȡ����
	virtual VOID * GetDataBase(REFGUID Guid, DWORD dwQueryVer);
	//��ȡ����
	virtual VOID * GetDataBaseEngine(REFGUID Guid, DWORD dwQueryVer);

	//���ܽӿ�
public:
	//Ͷ�ݽ��
	virtual bool PostGameDataBaseResult(WORD wRequestID, VOID * pData, WORD wDataSize);

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

	//��¼����
protected:
	//I D ��¼
	bool OnRequestLogonUserID(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID,BOOL bMatch=false);
	//I D ��¼
	bool OnRequestLogonMobile(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//�ʺŵ�¼
	bool OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);

	//ϵͳ����
protected:
	//��Ϸд��
	bool OnRequestWriteGameScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//�뿪����
	bool OnRequestLeaveGameServer(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��Ϸ��¼
	bool OnRequestGameScoreRecord(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//���ز���
	bool OnRequestLoadParameter(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//�����б�
	bool OnRequestLoadGameColumn(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//���ػ���
	bool OnRequestLoadAndroidUser(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//���ص���
	bool OnRequestLoadGameProperty(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��������
	bool OnRequestPropertyRequest(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//�û�Ȩ��
	bool OnRequestManageUserRight(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//ϵͳ��Ϣ
	bool OnRequestLoadSystemMessage(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//����������Ϣ
	bool OnRequestLoadMatchConfig(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//����������Ϣ
	bool OnRequestLoadMatchRewardConfig(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//�������д�
	bool OnRequestLoadSensitiveWords(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��������
	bool OnRequestMatchFee(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//������ʼ
	bool OnRequestMatchStart(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��������
	bool OnRequestMatchOver(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��������
	bool OnRequestMatchReward(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//�˳�����
	bool OnRequestMatchQuit(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//������
	bool OnRequestCheckTrumpet(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//���ܴ���
	bool OnRequestQuerySkill(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//����ʹ��
	bool OnRequestUseSkill(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//������������
	bool OnRequestNewGuidGift(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��־��¼С�����
	bool OnRequestLogClownOperation(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��һ�ν��볬����������
	bool OnRequestFirstEnterSuper(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//�������kֵ�ͻ���
	bool OnRequestKValueAndIntegration(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	bool OnRequestUserMaxKValue(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	bool OnRequestUserMatchSignUP(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	bool OnRequestGetMatchPirze(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//���з���
protected:
	//������Ϸ��
	bool OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��ȡ��Ϸ��
	bool OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//ת����Ϸ��
	bool OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��������
	bool OnRequestUserTransferDragon(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//ʹ������
	bool OnRequestUserUseDragon(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//Ӯ��������Ƭ
	bool OnRequestUserWinDragonPiece(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//Ӯ������
	bool OnRequestUserWinPearl(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��ѯ����
	bool OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��ѯ�û�
	bool OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//ϵͳ���ͼ���
	bool OnRequestSkillGive(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//��ȡ�û�����
	bool OnRequestGetUserItemCount(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//�����û�����
	bool OnRequestSaveUserItemCount(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);

	//��������
private:
	//��¼���
	VOID OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,BYTE cbDeviceType=DEVICE_TYPE_PC,WORD wBehaviorFlags=0,WORD wPageTableCount=0);
	//���н��
	VOID OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, SCORE lFrozenedScore, LPCTSTR pszErrorString, bool bMobileClient,BYTE cbActivityGame=FALSE);
	//������Ϣ
	VOID OnInsureTransferInfo(DWORD dwContextID, DWORD dwErrorCode);
	//������
	VOID OnDragonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, BYTE cbParam1);
	//�����������
	VOID OnMatchSignUpDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient, DWORD dwUserID,BYTE cbDeviceType=DEVICE_TYPE_PC,WORD wBehaviorFlags=0,WORD wPageTableCount=0);
protected:
	//���¿��
	bool OnBankCheckStock(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ�û�����ֵ
	bool OnGetUserLucky(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����û���Ʒ
	bool OnSaveUserItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ��¼Json��ʽ
	bool OnGameJson(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ִ�����ݿ�ű�
	bool OnSentence(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ�ر�ͼ
	bool OnRequestUpdateTreasureMap(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
};

//////////////////////////////////////////////////////////////////////////////////

#endif