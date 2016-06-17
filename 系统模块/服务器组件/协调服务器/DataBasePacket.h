#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//请求数据包

//比赛命令
#define	DBR_GC_INSERT_MATCH_SIGNUP_USER		1				//插入报名用户信息
struct DBR_GC_InsertMatchSignUpUser
{
	DWORD dwUserID;
	WORD  wServerID;
	int	  nMatchID;
	WORD  wGroupID;
	bool  bIsInGame;
	SCORE llUserScore;
};

#define DBO_GC_INSERT_MATCH_SIGNUP_USER		1				// 插入用户报名信息结果
struct DBO_GC_Insert_Match_SignUp_User
{
	DWORD dwUserID;
	WORD  wServerID;
	WORD  wGroupID;
	bool  bIsSignUpSuc;
	int   nMatchID;
	bool  bMatchStatus;//报名状态，0为未报名，1为已经报名
	int   nMatchType;
	bool  bIsInGame;
	WORD  wEnrollmentFee;
	TCHAR szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR szDescription[128];//描述信息
};

#define DBR_GR_LOAD_MATCH_CONFIG	2						//加载比赛时间配置信息

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
#define DBR_GR_SAVE_MATCH_RESULT	4						//保存比赛结果
struct stMatchResult
{
	DWORD dwUserID;
	SCORE lUserScore;
	int nMatchRank;
	TCHAR szBeginTime[20];
	TCHAR szEndTime[20];
	int nMatchNum;
	int nServerId;
	int nIsComplete;
	int nIsRobot;
	int nMatchType;
	int nGroupId;
	int nIsGet;
};

#define DBR_GR_GET_SQL_TIME	5						//获得数据库时间

#define DBO_GR_GET_SQL_TIME 6

struct DBO_Get_Sql_Time
{
	TCHAR szTime[128];
};
#pragma pack()

#endif