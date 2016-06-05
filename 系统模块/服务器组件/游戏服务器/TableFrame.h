#ifndef TABLE_FRAME_HEAD_FILE
#define TABLE_FRAME_HEAD_FILE

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//���鶨��
typedef IServerUserItem * CTableUserItemArray[MAX_CHAIR];				//��Ϸ����
typedef CWHArray<IServerUserItem *> CLookonUserItemArray;				//�Թ�����
typedef CWHArray<tagGameScoreRecord *> CGameScoreRecordArray;			//��¼����
interface IMatchTableFrameNotify;
//////////////////////////////////////////////////////////////////////////////////

//���ӿ��
class CTableFrame : public ITableFrame
{
	//��Ϸ����
protected:
	WORD							m_wTableID;							//���Ӻ���
	WORD							m_wChairCount;						//������Ŀ
	BYTE							m_cbStartMode;						//��ʼģʽ
	WORD							m_wUserCount;						//�û���Ŀ

	//״̬����
protected:
	bool							m_bGameStarted;						//��Ϸ��־
	bool							m_bDrawStarted;						//��Ϸ��־
	bool							m_bTableStarted;					//��Ϸ��־

	//״̬����
protected:
	bool							m_bAllowLookon[MAX_CHAIR];			//�Թ۱�־
	SCORE							m_lFrozenedScore[MAX_CHAIR];		//�������

	//��Ϸ����
protected:
	bool							m_bDynamicJoinAndriod;				//��̬���������(�����������޶���������������)
	LONG							m_lCellScore;						//��Ԫ����
	BYTE							m_cbGameStatus;						//��Ϸ״̬

	//ʱ�����
protected:
	DWORD							m_dwDrawStartTime;					//��ʼʱ��
	SYSTEMTIME						m_SystemTimeStart;					//��ʼʱ��
	WORD                            m_wDrawCount;                       //��Ϸ����

	//��̬����
protected:
	DWORD							m_dwTableOwnerID;					//�����û�
	TCHAR							m_szEnterPassword[LEN_PASSWORD];	//��������

	//���߱���
protected:
	WORD							m_wOffLineCount[MAX_CHAIR];			//���ߴ���
	DWORD							m_dwOffLineTime[MAX_CHAIR];			//����ʱ��

	//�û�����
protected:
	CTableUserItemArray				m_TableUserItemArray;				//��Ϸ�û�
	CLookonUserItemArray			m_LookonUserItemArray;				//�Թ��û�

	//����ӿ�
protected:
	ITimerEngine *					m_pITimerEngine;					//ʱ������
	ITableFrameSink	*				m_pITableFrameSink;					//���ӽӿ�
	IMainServiceFrame *				m_pIMainServiceFrame;				//����ӿ�
	IAndroidUserManager *			m_pIAndroidUserManager;				//�����ӿ�

	//��չ�ӿ�
protected:
	ITableUserAction *				m_pITableUserAction;				//�����ӿ�
	ITableUserRequest *				m_pITableUserRequest;				//����ӿ�
	ITableUserAction *				m_pIMatchTableAction;				//�����ӿ�(������)

	//���ݽӿ�
protected:
	IDataBaseEngine *				m_pIKernelDataBaseEngine;			//�ں�����
	IDataBaseEngine *				m_pIRecordDataBaseEngine;			//��¼����

	//������Ϣ
protected:
	tagGameParameter *				m_pGameParameter;					//���ò���
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//��������
	tagGameServiceOption *			m_pGameServiceOption;				//��������

	//��Ϸ��¼
protected:
	CGameScoreRecordArray			m_GameScoreRecordActive;			//��Ϸ��¼
	static CGameScoreRecordArray	m_GameScoreRecordBuffer;			//��Ϸ��¼

	//�����ӿ�
protected:
	IGameMatchSink					* m_pIGameMatchSink;				//�����ӿ�

	//��������
public:
	//���캯��
	CTableFrame();
	//��������
	virtual ~CTableFrame();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���Խӿ�
public:
	//���Ӻ���
	virtual WORD GetTableID() { return m_wTableID; }
	//��Ϸ����
	virtual WORD GetChairCount() { return m_wChairCount; }
	//��λ����Ŀ
	virtual WORD GetNullChairCount(){return m_wChairCount - m_wUserCount;}

	//���ò���
public:
	//�Զ�����
	virtual VOID * GetCustomRule() { return m_pGameServiceOption->cbCustomRule; };
	//��������
	virtual tagGameServiceAttrib * GetGameServiceAttrib() { return m_pGameServiceAttrib; }
	//��������
	virtual tagGameServiceOption * GetGameServiceOption() { return m_pGameServiceOption; }

	//���ýӿ�
public:
	//��ʼģʽ
	virtual BYTE GetStartMode() { return m_cbStartMode; }
	//��ʼģʽ
	virtual VOID SetStartMode(BYTE cbStartMode) { m_cbStartMode=cbStartMode; }

	//��Ԫ����
public:
	//��Ԫ����
	virtual LONG GetCellScore() { return m_lCellScore; }
	//��Ԫ����
	virtual VOID SetCellScore(LONG lCellScore) { m_lCellScore=lCellScore; m_pITableFrameSink->SetGameBaseScore(lCellScore);}

	//��Ԫ����
	virtual bool GetDynamicJoinAndriod() { return m_bDynamicJoinAndriod; }
	//��Ԫ����
	virtual VOID SetDynamicJoinAndriod(bool bDynamicJoinAndriod) { m_bDynamicJoinAndriod=bDynamicJoinAndriod;}

	//״̬�ӿ�
public:
	//��ȡ״̬
	virtual BYTE GetGameStatus() { return m_cbGameStatus; }
	//����״̬
	virtual VOID SetGameStatus(BYTE cbGameStatus) { m_cbGameStatus=cbGameStatus; }

	//��Ϣ�ӿ�
public:
	//��Ϸ״̬
	virtual bool IsGameStarted() { return m_bGameStarted; }
	//��Ϸ״̬
	virtual bool IsDrawStarted() { return m_bDrawStarted; }
	//��Ϸ״̬
	virtual bool IsTableStarted() { return m_bTableStarted; }
	//����״̬
	virtual bool IsTableLocked() { return (m_szEnterPassword[0]!=0); }

	//���ƽӿ�
public:
	//��ʼ��Ϸ
	virtual bool StartGame();
	//��ɢ��Ϸ
	virtual bool DismissGame();
	//������Ϸ
	virtual bool ConcludeGame(BYTE cbGameStatus);
	//��������
	virtual bool ConcludeTable();

	//�û��ӿ�
public:
	//Ѱ���û�
	virtual IServerUserItem * SearchUserItem(DWORD dwUserID);
	//��Ϸ�û�
	virtual IServerUserItem * GetTableUserItem(WORD wChairID);
	//�Թ��û�
	virtual IServerUserItem * EnumLookonUserItem(WORD wEnumIndex);

	//д�ֽӿ�
public:
	//д�����
	virtual bool WriteUserScore(WORD wChairID, tagScoreInfo & ScoreInfo, DWORD dwGameMemal=INVALID_DWORD, DWORD dwPlayGameTime=INVALID_DWORD);
	//д�����
	virtual bool WriteTableScore(tagScoreInfo ScoreInfoArray[], WORD wScoreCount);

	//����ӿ�
public:
	//����˰��
	virtual SCORE CalculateRevenue(WORD wChairID, SCORE lScore);
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);

	//ʱ��ӿ�
public:
	//����ʱ��
	virtual bool SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter);
	//ɾ��ʱ��
	virtual bool KillGameTimer(DWORD dwTimerID);

	//��Ϸ�û�
public:
	//��������
	virtual bool SendTableData(WORD wChairID, WORD wSubCmdID);
	//��������
	virtual bool SendTableData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize,WORD wMainCmdID=MDM_GF_GAME);

	//�Թ��û�
public:
	//��������
	virtual bool SendLookonData(WORD wChairID, WORD wSubCmdID);
	//��������
	virtual bool SendLookonData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//�����û�
public:
	//��������
	virtual bool SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID);
	//��������
	virtual bool SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//ϵͳ��Ϣ
public:
	//������Ϣ
	virtual bool SendGameMessage(LPCTSTR lpszMessage, WORD wType);
	//��Ϸ��Ϣ
	virtual bool SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType);
	//������Ϣ
	virtual bool SendRoomMessage(LPCTSTR lpszMessage, WORD wType);
	//������Ϣ
	virtual bool SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType);

	//��������
public:
	//��������
	virtual bool PerformStandUpAction(IServerUserItem * pIServerUserItem);
	//�Թ۶���
	virtual bool PerformLookonAction(WORD wChairID, IServerUserItem * pIServerUserItem);
	//���¶���
	virtual bool PerformSitDownAction(WORD wChairID, IServerUserItem * pIServerUserItem, LPCTSTR lpszPassword=NULL);

	//���ܽӿ�
public:
	//���ͳ���
	virtual bool SendGameScene(IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize);

	//�����ӿ�
public:
	virtual bool SetMatchInterface(IUnknownEx * pIUnknownEx);
	//��ȡ�������ӽӿ�
	virtual IGameMatchSink* GetGameMatchSink(){return m_pIGameMatchSink;}

	//���ܺ���
public:
	//��ȡ��λ
	WORD GetNullChairID();
	//�����λ
	WORD GetRandNullChairID();
	//�û���Ŀ
	WORD GetSitUserCount();
	//�Թ���Ŀ
	WORD GetLookonUserCount();
	//������Ŀ
	WORD GetOffLineUserCount();

	//���ܺ���
public:
	//�û�״��
	WORD GetTableUserInfo(tagTableUserInfo & TableUserInfo);
	//��������
	bool InitializationFrame(WORD wTableID, tagTableFrameParameter & TableFrameParameter);

	//�û��¼�
public:
	//�����¼�
	bool OnEventUserOffLine(IServerUserItem * pIServerUserItem);
	//�����¼�
	bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//������������
	bool OnNewGuidGift(IServerUserItem * pIServerUserItem, bool bSuccess, int nCannonStatus, int nLotteryCount);
	//Ӯ������
	bool OnWinPearl(IServerUserItem * pIServerUserItem, int nPearlType, int nPearlCount);
	//��ȡ�û�����
	bool OnGetUserItemCount(IServerUserItem * pIServerUserItem, int nType, SCORE lCount);

	//ϵͳ�¼�
public:
	//ʱ���¼�
	bool OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//��Ϸ�¼�
	bool OnEventSocketGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//����¼�
	bool OnEventSocketFrame(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//��������
public:
	//����״̬
	bool SendTableStatus();
	//����ʧ��
	bool SendRequestFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, LONG lErrorCode);

	//Ч�麯��
public:
	//��ʼЧ��
	bool EfficacyStartGame(WORD wReadyChairID);
	//��ַЧ��
	bool EfficacyIPAddress(IServerUserItem * pIServerUserItem);
	//����Ч��
	bool EfficacyScoreRule(IServerUserItem * pIServerUserItem);
	//����Ч��
	bool EfficacyEnterTableScoreRule(WORD wChairID, IServerUserItem * pIServerUserItem);

public:
	//������
	bool CheckDistribute();

public:
	virtual const tagStockInfo * GetStockInfo(int nRoom, int nTable, int nType);
	virtual void ChangeStock(int nType, WORD wChairID, int nStock, int nRevenue, int nCaiJin);
	virtual float GetUserFactor(unsigned int nUserID);
	virtual int SaveUserItem(IServerUserItem * pIServerUserItem, int nItemIndex);
	//��Ϸ��¼Json��ʽ
	virtual void SaveGameJson(IServerUserItem * pIServerUserItem, const TCHAR * str, SCORE lScore);
	//��ȡ��Ϸ�ڲ�����
	virtual	LONG GetInsideScore(WORD wChairID);
	virtual tagLotteryInfo * GetLotteryInfo();
	virtual tagMobileMoney * GetMobileMoney();
	//Ӯ��������Ƭ
	bool WinDragonPiece(IServerUserItem * pIServerUserItem, SCORE lWinDragonPiece, SCORE lWinScore);
	//Ӯ������
	bool WinPearl(IServerUserItem * pIServerUserItem, int nPearlType, int nPearlCount);
	//ϵͳ���ͼ���
	bool SkillGive(IServerUserItem * pIServerUserItem);
	//������������
	bool NewGuidGift(IServerUserItem * pIServerUserItem);
	//���ͱ�������
	bool SendMatchScore(IServerUserItem * pIServerUserItem, SCORE lMatchScore);
	//�����������
	void OnMatchRank(IServerUserItem * pIServerUserItem, SCORE lMatchScore, int nCurrentRank, int nChangeRank);
	//����ȫ���㲥
	void SendPlatformMessage(LPCTSTR lpszMessage, BYTE cbLevel);
	void getRecordCount();
	//��ȡ�û�����
	void GetUserItemCount(IServerUserItem * pIServerUserItem, int nType);
	//�����û�����
	void SaveUserItemCount(IServerUserItem * pIServerUserItem, int nType, SCORE lCount);
//���ͻ�þ޽�����
	bool OnWinBigReward(IServerUserItem * pIServerUserItem,int nScore);
	void SetTreasureMapCount(IServerUserItem * pIServerUserItem,int nScore);
	void UpdateUserKValueAndIntegration(IServerUserItem * pIServerUserItem,int nKvalue,int nIntegration);
	void UpdateUserMaxKValue(IServerUserItem * pIServerUserItem,int nScore);
	void UpdateUserIntegrate(IServerUserItem * pIServerUserItem,int nAddScore);
};

//////////////////////////////////////////////////////////////////////////////////

#endif