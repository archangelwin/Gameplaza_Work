#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "ServerListManager.h"
#include <string>
#include <map>

//////////////////////////////////////////////////////////////////////////////////
//���ݶ���

//��������
#define CLIENT_KIND_FALSH			1									//��ҳ����
#define CLIENT_KIND_MOBILE			2									//�ֻ�����
#define CLIENT_KIND_COMPUTER		3									//��������

//ǩ����Ϣ����
#define SIGNIN_REWARD_ARRAY_LENGTH  7									//ǩ��������Ϣ����


//�󶨲���
struct tagBindParameter
{
	//�������
	DWORD							dwSocketID;							//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	DWORD							dwActiveTime;						//����ʱ��

	//������Ϣ
	WORD							wModuleID;							//ģ���ʶ
	BYTE							cbClientKind;						//��������
	DWORD							dwPlazaVersion;						//�����汾

	//�齱��Ϣ
	bool							bIsLtryInfoSend;					//�Ƿ���Ҫ���ͳ齱��Ϣ

	DWORD							dwUserID;
	TCHAR							szNickName[LEN_NICKNAME];
};

struct tagWealthBag
{
	DWORD dwUserID;
	DWORD dwSocketID;
	int nScore;
	TCHAR szNickName[LEN_NICKNAME];
};

struct tagMBVerifInfo
{
	TCHAR szMobilePhone[LEN_MOBILE_PHONE];
	TCHAR szVerificationCode[LEN_VERIFICATION_CODE];
	time_t m_dwVerifCodeSendTime;
};

struct tagMapMBVerifInfo
{
	TCHAR szVerificationCode[LEN_VERIFICATION_CODE];
	time_t m_dwVerifCodeSendTime;
};

struct tagCompRankInfo
{
	int nRank;
	int nUserId;
	int nCount;
};

struct tagSuperRankInfo
{
	int nRank;
	TCHAR szNickName[32];
	LONGLONG lCount;
};

struct tagLoginQueueInfo
{
	DWORD							dwSocketId;
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	BYTE							cbNeeValidateMBCard;				//�ܱ�У��

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//////////////////////////////////////////////////////////////////////////////////
//���ȹ���
class CAttemperEngineSink : public IAttemperEngineSink
{
	//��Ԫ����
	friend class CServiceUnits;

	//״̬����
protected:
	bool							m_bNeekCorrespond;					//Э����־
	bool                            m_bShowServerStatus;                //��ʾ������״̬

	//��������
protected:
	CInitParameter *				m_pInitParameter;					//���ò���
	tagBindParameter *				m_pBindParameter;					//��������

	//�������
protected:
	CServerListManager				m_ServerListManager;				//�б����
	std::vector<tagWealthBag>		m_vWealthBag;
	time_t							m_dwWealthBagStartTime;
	CMD_GPO_WealthBagResult			m_WealthBagResult;
	bool							m_bWealthBagRunning;
public:
	std::map<std::string,tagMapMBVerifInfo> m_mMBVerifInfo;
	std::map<std::string,int> m_mRegisteredMbNum;

	//����ӿ�
protected:
	ITimerEngine *					m_pITimerEngine;					//ʱ������
	IDataBaseEngine *				m_pIDataBaseEngine;					//��������
	ITCPSocketService *				m_pITCPSocketService;				//Э������
public:
	ITCPNetworkEngine *				m_pITCPNetworkEngine;				//��������

	//��Ƶ����
protected:
	WORD							m_wAVServerPort;					//��Ƶ�˿�
	DWORD							m_dwAVServerAddr;					//��Ƶ��ַ

	//������֤��
public:
	char *							m_SMSVerifyAccount;
	char *							m_SMSVerifyAccountToken;
	char *							m_SMSVerifyAppID;
	char *							m_SMSVerifyRestIP;//app.cloopen.com,����Ҫдhttps��//
	char *							m_SMSVerifyTemplateID;
	int								m_SMSVerifyPort;
	//��������
public:
	std::vector<tagCompRankInfo>	m_CompRankInfo;
	WORD	m_nSendMaxNum;
	//������������
public:
	std::vector<tagSuperRankInfo>	m_SuperRankInfo;
	//��¼�Ŷ�
public:
	std::vector<tagLoginQueueInfo>	m_LoginQueueInfo;

	//ǩ����Ϣ
tagSignInDayInfo m_NormalUserRewardConfig[7];
tagSignInDayInfo m_VIPUserRewardConfig[7];

	// fake server info
	WORD							m_wSendFakeInfoInterval;
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

	//���Ӵ���
protected:
	//ע���¼�
	bool OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//Զ�̷���
	bool OnTCPSocketMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//�������
	bool OnTCPSocketMainManagerService(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//�����¼�
protected:
	//��¼����
	bool OnTCPNetworkMainPCLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�б���
	bool OnTCPNetworkMainPCServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//������
	bool OnTCPNetworkMainPCUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//Զ�̴���
	bool OnTCPNetworkMainPCRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//���ߴ���
	bool OnTCPNetworkMainItemService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//���ܴ���
	bool OnTCPNetworkMainSkillService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//�ֻ��¼�
protected:
	//��¼����
	bool OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�б���
	bool OnTCPNetworkMainMBServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//��վ��Ϣ
	bool OnTCPNetworkMainWebSite(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//�����¼�
protected:
	//I D ��¼
	bool OnTCPNetworkSubPCLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺŵ�¼
	bool OnTCPNetworkSubPCLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺ�ע��
	bool OnTCPNetworkSubPCRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ο͵�¼
	bool OnTCPNetworkSubPCGuestReg(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ֻ���ע��
	bool OnTCPNetworkSubPCMBNumRegister(VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//�ֻ��¼�
protected:
	//I D ��¼
	bool OnTCPNetworkSubMBLogonGameID(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺŵ�¼
	bool OnTCPNetworkSubMBLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺ�ע��
	bool OnTCPNetworkSubMBRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//��Ϸ�¼�
protected:
	//��¼�ɹ�
	bool OnDBPCLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnDBPCLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnDBPCLogonValidateMBCard(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnDBPCLogonValidatePassPostID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û�ͷ��
	bool OnDBPCUserFaceInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û���Ϣ
	bool OnDBPCUserIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnDBPCUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���гɹ�
	bool OnDBPCUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnDBPCUserInsureTransferInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ʧ��
	bool OnDBPCUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û���Ϣ
	bool OnDBPCUserInsureUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������֤
	bool OnDBPCUserCheckPassword(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����ɹ�
	bool OnDBPCOperateSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ʧ��
	bool OnDBPCOperateFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�ֻ��¼�
protected:
	//��¼�ɹ�
	bool OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ο͵�¼�ɹ�
	bool OnDBPCGuestLogon(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�б��¼�
protected:
	//��Ϸ����
	bool OnDBPCGameTypeItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnDBPCGameKindItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ�ڵ�
	bool OnDBPCGameNodeItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnDBPCGamePageItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ�б�
	bool OnDBPCGameListResult(DWORD dwContextID, VOID * pData, WORD wDataSize);

protected:
	//ǩ����ѯ�ɹ�
	bool OnDBPCSignInQuerySuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ǩ����ѯʧ��
	bool OnDBPCSignInQueryFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û�ǩ���ɹ�
	bool OnDBPCUserSignInSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û�ǩ��ʧ��
	bool OnDBPCUserSignInFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�齱��ѯ���
	bool OnDBPCLotteryQueryResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û��齱���
	bool OnDBPCUserLotteryResult(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�ʼ�ID�б�
	bool OnDBMailIDList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ʼ���Ϣ
	bool OnDBMailInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ʼ�����
	bool OnDBMailContent(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ʼ���Ʒ
	bool OnDBMailItem(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��ֵ����
	bool OnDBChargeValue(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//VIP��Ϣ
	bool OnDBVIPInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//VIPÿ�����ͽ��
	bool OnDBVIPResult(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��ǰ����
	bool OnDBCurrentScore(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��¼����
	bool OnDBLogonGift(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�׳�ɹ�
	bool OnDBCharge1stRequest(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ�׳��������
	bool OnDBCharge1stGet(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��������
protected:
	//�汾���
	bool CheckPlazaVersion(BYTE cbDeviceType, DWORD dwPlazaVersion, DWORD dwSocketID, bool bCheckLowVer=true);
	//��������
	bool SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize);
	//�����������
	VOID GenRandCode(TCHAR szRandCode[]);

	//�����б�
protected:
	//��������
	VOID SendGameTypeInfo(DWORD dwSocketID);
	//��������
	VOID SendGameKindInfo(DWORD dwSocketID);
	//���ͽڵ�
	VOID SendGameNodeInfo(DWORD dwSocketID, WORD wKindID);
	//���Ͷ���
	VOID SendGamePageInfo(DWORD dwSocketID, WORD wKindID);
	//���ͷ���
	VOID SendGameServerInfo(DWORD dwSocketID, WORD wKindID);
	//���ͷ���
	VOID SendGameServerInfoEx(DWORD dwSocketID, WORD wKindID);

	//�ֻ��б�
protected:
	//��������
	VOID SendMobileKindInfo(DWORD dwSocketID, WORD wModuleID);
	//���ͷ���
	VOID SendMobileServerInfo(DWORD dwSocketID, WORD wModuleID);

protected:
	//������֤������
	bool OnTCPNetworkSubPCMBCODEVERIFY(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�����ֻ�����֤���
	bool OnDBPCVerifyMBNumRes(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ֻ�ע����
	bool OnDBMBNumRegisterSuccessRes(DWORD dwContextID, VOID * pData, WORD wDataSize);

protected:
	char * ConvertLPWSTRToLPSTR(LPWSTR lpwszStrIn); 
// ������λ�����
	VOID GenRandNumCode(TCHAR szRandCode[]);

	//�ֻ��Ű�
protected:
	//������֤������
	bool OnTCPNetworkSubPCMoBileCode(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�����ֻ��Ű���֤����֤
	bool OnTCPNetworkSubPCCodeVerification(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��ѯ�ֻ����Ƿ�󶨹�
	bool OnTCPNetworkSubPCMobileCheckExit(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//����㲥�ٶ�
protected:
	bool OnTCPNetworkSubPCBroadCastSpeed(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//����ĳһ��Ʒ����
protected:
	bool OnTCPNetworkSubPCItemCount(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��ȡ����
	bool OnTCPNetworkSubMatchReward(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//���ȹ㲥
protected:
	bool OnTCPNetworkSubPCBroadLaBa(VOID * pData, WORD wDataSize, DWORD dwSocketID);
protected:
	bool OnDBGetCompRankInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
protected:
	bool OnDBBroadCastResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
protected:
	bool OnCancelLoginQueueing(DWORD dwSocketID);
protected:
	bool OnRequestDataBaseLogin();
protected:
	bool OnTCPNetworkSubPCChangePhoneFare(VOID * pData, WORD wDataSize, DWORD dwSocketID);
protected:
	bool OnDBChangePhoneFareResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
protected:
	bool OnDBGetSuperRankInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ���ֻ�����������������Ϣ
	bool OnTCPNetworkGetBindRewardNewGuid(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//���ó���������������״̬
	bool OnTCPNetworkSetSuperNewGuid(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��ʼ��ǩ��������Ϣ
	bool ReadSignInRewardConfig();
	bool OnDBGetNewMailRemindInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	bool OnDBGetMatchResultPrize(DWORD dwContextID, VOID * pData, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////////////

#endif