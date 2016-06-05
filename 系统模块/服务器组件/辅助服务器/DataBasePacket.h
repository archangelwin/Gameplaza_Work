#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//�������ݰ�

//ϵͳ����
#define DBR_GP_LOAD_GAME_LIST		50									//�����б�
#define DBR_GR_LOAD_SYSTEM_MESSAGE	51
#define DBR_GP_LOAD_YESTERDAY_SCORE	52

#define DBR_GP_RANK_INFO		53
#define DBO_GP_RANK_INFO		1053

struct DBR_GP_RankInfo
{
	int nType;
};

struct DB0_GP_RankInfo
{
	int nUserID;
	SCORE lMatchScore;
};

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct DBR_GP_OnLineCountInfo
{
	WORD							wKindCount;							//������Ŀ
	DWORD							dwOnLineCountSum;					//��������
	tagOnLineInfoKind				OnLineCountKind[MAX_KIND];			//��������
};

//////////////////////////////////////////////////////////////////////////////////
//�����Ϣ

//�б���
#define DBO_GP_GAME_TYPE_ITEM			130									//������Ϣ
#define DBO_GP_GAME_KIND_ITEM			131									//������Ϣ
#define DBO_GP_GAME_NODE_ITEM			132									//�ڵ���Ϣ
#define DBO_GP_GAME_PAGE_ITEM			133									//������Ϣ
#define DBO_GP_GAME_LIST_RESULT			134									//���ؽ��
#define DBO_GR_LOAD_SYSTEM_MESSAGE		151
#define DBO_GP_LOAD_YESTERDAY_SCORE		152

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
struct DBO_GP_GameType
{
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wTypeID;							//��������
	TCHAR							szTypeName[LEN_TYPE];				//��������
};

//��Ϸ����
struct DBO_GP_GameKind
{
	WORD							wTypeID;							//��������
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wKindID;							//��������
	WORD							wGameID;							//ģ������
	TCHAR							szKindName[LEN_KIND];				//��Ϸ����
	TCHAR							szProcessName[LEN_PROCESS];			//��������
};

//��Ϸ�ڵ�
struct DBO_GP_GameNode
{
	WORD							wKindID;							//��������
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wNodeID;							//�ڵ�����
	TCHAR							szNodeName[LEN_NODE];				//�ڵ�����
};

//��������
struct DBO_GP_GamePage
{
	WORD							wKindID;							//��������
	WORD							wNodeID;							//�ڵ�����
	WORD							wSortID;							//��������
	WORD							wPageID;							//��������
	WORD							wOperateType;						//��������
	TCHAR							szDisplayName[LEN_PAGE];			//��ʾ����
};

//���ؽ��
struct DBO_GP_GameListResult
{
	BYTE							cbSuccess;							//�ɹ���־
};

struct DBO_GP_YesterdayScore
{
	SCORE							lYesterdayScore;
};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif