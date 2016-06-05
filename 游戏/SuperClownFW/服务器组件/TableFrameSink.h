#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgCustomRule.h"

//////////////////////////////////////////////////////////////////////////////////
//struct tagTurnTableRecord
//{
//	bool  bIsWin;       //�Ƿ�ɹ�
//	bool  bIsEnd;       //�Ƿ����
//	SCORE lUserScore;
//	SCORE lCurrentChip;
//	TCHAR szTime[50];
//};

struct tagGuessOperation
{
	int   nStarCount;
	int   nChipTimes;	//����	
};
//С�������־
struct tagClownOperation
{
	int   nRoundNum;	//�ڼ��ֲ���, ת�̻��ǲ´�С
	bool  bIsWin;       //�Ƿ�ɹ�
	SCORE lUserScore;
	SCORE lCurrentChip;
	TCHAR szTime[20];
	tagGuessOperation stGuesOperation;
};


struct stClownOperationInfo
{
	DWORD dwUserID;
	DWORD dwOperationCount;
	DWORD dwDataSize;	
	std::vector<tagClownOperation> *	pClownOperation;					//��¼һ���������еĲ���
};

//��Ϸ����
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
//�������
protected:
																		                    //��Ϸ�߼�
	ITableFrame							* m_pITableFrame;									//��ܽӿ�
	const tagGameServiceOption			* m_pGameServiceOption;								//���ò���
	tagGameServiceAttrib *				m_pGameServiceAttrib;								//��Ϸ����
	tagCustomRule *						m_pGameCustomRule;									//�Զ�����
//���Ա���
protected:
	static const WORD					m_wPlayerCount;										//��Ϸ����
	static const BYTE					m_GameStartMode;									//��ʼģʽ
//��Ϸ����
	LONGLONG                            m_lBeginGuessPirateScore;                           //�û���ʼ�º���ʱ�ķ���
	LONGLONG							m_lDefultChip;									    //Ĭ����ע��С
	LONGLONG							m_lThreeRollChip;									//����ͼ��ת��ʱ�Ķ�ע
	LONGLONG							m_lCurrentChip;									    //ʵʱ��ע��С
	LONGLONG                            m_lChipPrize;                                       //��ע����
	LONGLONG                            m_lFinalPrize;                                      //���
	int                                 m_nPrizeTimes;                                      //��������
	int                                 m_nStarCount;                                       //��ע��¶Դ���
	int                                 m_nClownIndex;                                      //С������
	int                                 m_nTimes;                                           //��ע����
	int                                 m_nAvailableTimes;                                  //���ʣ��������
	int                                 m_nYear;                                            //�Աȵ�¼����ˢ��ÿ�տ������
	int                                 m_nMonth;
	int                                 m_nDay;
	WORD                                m_wChipUser;                                        //��ǰ���
	bool                                m_bIsSelected;                                      //�Ƿ�ѡ��
	bool                                m_bIsRolling;                                       //�Ƿ���ת��
	bool                                m_bLackChip;                                        //�Ƿ�����ע
	int									m_miniRevenue;
	bool								m_bTreasureMapValueZero;							//�ر�ͼ������Ϊ��
	LONGLONG							m_lUserMaxThresholdNum;								//�������������kֵ
	int									m_nCurrentBand;										//������ڵĵ�ǰ����
	//���ñ���
	static LONGLONG						m_nProbability[10];									//�������ͼ���ĸ���
	static LONGLONG						m_nMaxThresholdNum;							        //ƽ̨�㷨�����ֵ
	static int							m_nClownProbability[8];						        //�¶�С���С������Ӧ�ĵĸ���
	static int							m_nRewardTimes[10];
	static int							m_nExtralReward[4];									//���⽱��
	static int							m_nMinRandKNum;
	static int							m_nMaxRandKNum;
	static int							m_nDecProbability[3];								//�´�Сʱ��ע������С�ĸ�����
	static int							m_nBandScoreBoundary[5];							//Ĭ��Ѻע�ķ�����
	static int							m_nBandScore[5];									//Ĭ��Ѻע�ķ����ζ�Ӧ�ķ���ֵ

	static int							m_nBigRewardBand;									//��þ޽���������ֵ
	static int							m_nTreasureMapValue;								//�ر�ͼֵ���ٽ��
	static int							m_nMinScoreCanPlay;									//���ٿ�������Ϸ�ķ���
	//С����־��Ϣ
	//std::vector<tagClownOperationEachRound> *	m_pClownOperation;							
	stClownOperationInfo				m_stClownOperation;									//��¼һ���������еĲ���/
	static CString						m_strLogDir;										//��־Ŀ¼

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release();
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��������
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID RepositionSink();

	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem) { return 0; }
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem) { return 0; }
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID) { return false; }

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);
	//ȷ����ע��
	void EnsureChip(LONGLONG lUserScore,bool bItemUseUp=false);
	//���㵱ǰ�Ƕ��⽱��
	int CalCulExtralReward();
	//�����¸��Ƕ��⽱��
	int CalCulNextStarExtralReward();

	//��Ϣ����
public:
	//��ʼ��Ϸ
	bool OnSubStartGame(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//ֹͣת��
	bool OnSubStopScroll(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//�ı��ע
	bool OnSubChangeBet(IServerUserItem * pIServerUserItem, const void * pDataBuffer, WORD wDataSize);
	//�жϲʻ�С��
	bool OnSubClownBet(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	//�ı��С���ע
	bool OnSubChangePrize(IServerUserItem * pIServerUserItem, const void * pDataBuffer, WORD wDataSize);
	//������Ϸ
	bool OnSubGetPrize(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);

	//�¼��ӿ�
public:
	//ʱ���¼�
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize);
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

	//����ӿ�
public:
	//��Ϸ��Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore) {}

	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//��ȡ��Ϸ�ڲ�����
	virtual	LONG GetInsideScore(WORD wChairID) { return 0; }
	//������������
	virtual bool OnNewGuidGift(IServerUserItem * pIServerUserItem, bool bSuccess, int nCannonStatus, int nLotteryCount) { return true; }
	//Ӯ������
	virtual bool OnWinPearl(IServerUserItem * pIServerUserItem, int nPearlType, int nPearlCount) { return true; }
	//�����������
	virtual void OnMatchRank(IServerUserItem * pIServerUserItem, SCORE lMatchScore, int nCurrentRank, int nChangeRank) { return; }
	void OnRecord(CString strLogDir,CString strMsg);
	//���ͻ�þ޽�����
	virtual bool OnWinBigReward(IServerUserItem * pIServerUserItem,int Score);
	//���߲���
	virtual void SetTreasureMapCount(IServerUserItem * pIServerUserItem,int nCount);
	//�������kvalue
	virtual void UpdateUserKValueAndIntegration(IServerUserItem * pIServerUserItem,int nKvalue,int nIntegration);
	//����������kvalue
	virtual void UpdateUserMaxKValue(IServerUserItem * pIServerUserItem);
	virtual bool OnGetUserItemCount(IServerUserItem * pIServerUserItem, int nType, SCORE lCount){ return false; };
	void LoadConfig();
};

//////////////////////////////////////////////////////////////////////////////////

#endif