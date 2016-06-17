#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "GlobalInfoManager.h"
#include <list>
//////////////////////////////////////////////////////////////////////////////////
//���ݶ���

//��������
enum enServiceKind
{
	ServiceKind_None,				//��Ч����
	ServiceKind_Game,				//��Ϸ����
	ServiceKind_Plaza,				//�㳡����
};

//�󶨲���
struct tagBindParameter
{
	//��������
	DWORD							dwSocketID;							//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	DWORD							dwActiveTime;						//����ʱ��

	//��������
	WORD							wServiceID;							//�����ʶ
	enServiceKind					ServiceKind;						//��������
};

//���鶨��
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

//���ȹ���
class CAttemperEngineSink : public IAttemperEngineSink
{
	//��Ԫ����
	friend class CServiceUnits;

	//״̬����
protected:
	WORD							m_wCollectItem;						//��������
	CWordArrayTemplate				m_WaitCollectItemArray;				//���ܵȴ�
	int								m_nLastSendElapse;
	bool							m_bInitLotteryInfo;
	std::vector<tagLotteryInfo>		m_vLotteryInfo;
	std::vector<tagMatchRankInfo>	m_vMatchRankInfo;
	std::vector<std::vector<WORD> > m_vMatchGameServerID;
	std::vector<std::vector<std::vector<tagMatchItem> > > m_vMatchData;	//Э���������û���Ϣ����
	int								m_nMessageRemindTime1;				//���������ʼ��Ϣչʾ����ʱ��1
	int								m_nMessageRemindTime2;				//���������ʼ��Ϣչʾ����ʱ��2
	int								m_nMessageRemindTime3;				//���������ʼ��Ϣչʾ����ʱ��3
	int								m_nMatchBeginRemindTime;			//���������ʼ����ʱ��
	int								m_nNewPlayerServerId;				//���ַ�����id
	int								m_nHundredServerId;					//���ڷ�����id
	int								m_nThousandServerId;				//ǧ�ڷ�����id
	int								m_nTenThousandServerId;				//���ڷ�����id
	std::list<tagMatchConfigItem>	m_MatchConfigItemList;				//������������
	TCHAR							m_szMatchBeginTime[20];
	WORD							m_wGroupCnt;						//��������
	int								m_nCurrentMatchID;							//��ǰ���еı�����ID
	WORD							m_wUserNumPerGroup;
	WORD							m_wAndroidFakeNameCnt;
	WORD							m_CurrentFakeNameNum;
	int								m_IsRemind[3];
	int								m_nDifferentTime;					
	bool							m_bIsGameBegin;
	// Time interval
	//WORD							m_wSendFakeInfoInterval;

	//��������
protected:
	CInitParameter *				m_pInitParameter;					//���ò���
	tagBindParameter *				m_pBindParameter;					//��������

	//�������
protected:
	CGlobalInfoManager				m_GlobalInfoManager;				//ȫ�ֹ���

	//����ӿ�
protected:
	ITimerEngine *					m_pITimerEngine;					//ʱ������
	ITCPNetworkEngine *				m_pITCPNetworkEngine;				//��������
	IDataBaseEngine *				m_pIDataBaseEngine;					//��������

	//��������
public:
	//���캯��
	CAttemperEngineSink();
	//��������
	virtual ~CAttemperEngineSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//�첽�ӿ�
public:
	//�����¼�
	virtual bool OnAttemperEngineStart(IUnknownEx * pIUnknownEx);
	//ֹͣ�¼�
	virtual bool OnAttemperEngineConclude(IUnknownEx * pIUnknownEx);

	//�¼��ӿ�
public:
	//�����¼�
	virtual bool OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize);
	//�Զ��¼�
	virtual bool OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize);

	//�ں��¼�
public:
	//ʱ���¼�
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM wBindParam);
	//���ݿ��¼�
	virtual bool OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//�����¼�
	virtual bool OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//Ӧ���¼�
	virtual bool OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID);
	//�ر��¼�
	virtual bool OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID);
	//��ȡ�¼�
	virtual bool OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//�����¼�
protected:
	//ע�����
	bool OnTCPNetworkMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//����״̬
	bool OnTCPNetworkMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�û�����
	bool OnTCPNetworkMainUserCollect(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//Զ�̷���
	bool OnTCPNetworkMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�������
	bool OnTCPNetworkMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//��������
protected:
	//�����б�
	bool SendServerListItem(DWORD dwSocketID);

	//���ݷ���
protected:
	//���䷢��
	bool SendDataToGame(WORD wServerID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//�û�����
	bool SendDataToUser(WORD wServerID, DWORD dwUserID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//���ɻ�������
	void GenLotteryInfo(int nMaxSend, int nSendElapse, tagLotteryInfo & LotteryInfo);
	//��ʼ����������
	void InitLotteryInfo();
	//��ȡ��һ����������Ϣ
	void GetNextMatchInfo(tagMatchConfigItem & MatchConfigItem);
	//Get Current Matchi Info
	void GetCurrentMatchInfo(int nMatchNum, tagMatchConfigItem & MatchConfigItem );

	//���ͱ���֪ͨ
	bool SendMatchNotifyInfoBatch(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	bool SendMatchUserInfoBatch(WORD wMainCmdID, WORD wSubCmdID);
	bool SendMatchEndInfoToServer(int nMatchType);
	time_t StringToDatetime(const char *strTime);
	void OnRecord(CString strMsg);
};

//////////////////////////////////////////////////////////////////////////////////

#endif