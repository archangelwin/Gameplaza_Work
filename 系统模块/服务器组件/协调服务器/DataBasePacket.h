#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//�������ݰ�

//��������
#define	DBR_GC_INSERT_MATCH_SIGNUP_USER		1				//���뱨���û���Ϣ
struct DBR_GC_InsertMatchSignUpUser
{
	DWORD dwUserID;
	WORD  wServerID;
	int	  nMatchID;
	WORD  wGroupID;
};

#define DBO_GC_INSERT_MATCH_SIGNUP_USER		1				// �����û�������Ϣ���
struct DBO_GC_Insert_Match_SignUp_User
{
	DWORD dwUserID;
	WORD  wServerID;
	bool  bIsSignUpSuc;
	int   nMatchID;
	bool  bMatchStatus;//����״̬��0Ϊδ������1Ϊ�Ѿ�����
	TCHAR szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR szDescription[128];//������Ϣ
};

#define DBR_GR_LOAD_MATCH_CONFIG	2						//���ر���ʱ��������Ϣ

#define DBO_LOAD_MATCH_CONFIG 3

struct DBO_Load_Match_Config_Item
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
#define DBR_GR_SAVE_MATCH_RESULT	4						//����������
struct stMatchResult
{
	DWORD dwUserID;
	SCORE lUserScore;
	int nMatchRank;
	TCHAR szBeginTime[20];
	TCHAR szEndTime[20];
	int nMatchNum;
	int nIsShare;
	int nServerId;
	int nIsComplete;
	int nIsRobot;
	int nMatchType;
	int nGroupId;
};

#pragma pack()

#endif