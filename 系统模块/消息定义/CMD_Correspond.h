#ifndef CMD_CORRESPOND_HEAD_FILE
#define CMD_CORRESPOND_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//注册命令

#define MDM_CS_REGISTER				1									//服务注册

//服务注册
#define SUB_CS_C_REGISTER_PLAZA		100									//注册广场
#define SUB_CS_C_REGISTER_SERVER	101									//注册房间

//注册结果
#define SUB_CS_S_REGISTER_FAILURE	200									//注册失败

//////////////////////////////////////////////////////////////////////////////////

//注册广场
struct CMD_CS_C_RegisterPlaza
{
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[LEN_SERVER];			//服务器名
};

//注册游戏
struct CMD_CS_C_RegisterServer
{
	WORD							wKindID;							//名称索引
	WORD							wNodeID;							//节点索引
	WORD							wSortID;							//排序索引
	WORD							wServerID;							//房间索引
	WORD							wServerPort;						//房间端口
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwFullCount;						//满员人数
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[LEN_SERVER];			//房间名称

	//房间配置
	SCORE							lMinEnterScore;						//最低积分
};

//注册失败
struct CMD_CS_S_RegisterFailure
{
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[128];				//错误消息
};

//////////////////////////////////////////////////////////////////////////////////
//服务信息

#define MDM_CS_SERVICE_INFO			2									//服务信息

//房间信息
#define SUB_CS_C_SERVER_ONLINE		1									//房间人数
#define SUB_CS_C_SERVER_MODIFY		2									//房间修改

//广场命令
#define SUB_CS_S_PLAZA_INFO			100									//广场信息
#define SUB_CS_S_PLAZA_INSERT		101									//广场列表
#define SUB_CS_S_PLAZA_REMOVE		103									//广场删除
#define SUB_CS_S_PLAZA_FINISH		104									//广场完成

//房间命令
#define SUB_CS_S_SERVER_INFO		110									//房间信息
#define SUB_CS_S_SERVER_ONLINE		111									//房间人数
#define SUB_CS_S_SERVER_INSERT		112									//房间列表
#define SUB_CS_S_SERVER_MODIFY		113									//房间修改
#define SUB_CS_S_SERVER_REMOVE		114									//房间删除
#define SUB_CS_S_SERVER_FINISH		115									//房间完成

//////////////////////////////////////////////////////////////////////////////////

//房间数据
struct CMD_CS_C_ServerModify
{
	WORD							wSortID;							//排序索引
	WORD							wKindID;							//类型索引
	WORD							wNodeID;							//节点索引
	WORD							wServerPort;						//房间端口
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwFullCount;						//满员人数
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[LEN_SERVER];			//房间名称
};

//房间人数
struct CMD_CS_C_ServerOnLine
{
	DWORD							dwOnLineCount;						//在线人数
};

//////////////////////////////////////////////////////////////////////////////////

//广场删除
struct CMD_CS_S_PlazaRemove
{
	DWORD							dwPlazaID;							//广场标识
};

//房间人数
struct CMD_CS_S_ServerOnLine
{
	WORD							wServerID;							//房间标识
	DWORD							dwOnLineCount;						//在线人数
};

//房间修改
struct CMD_CS_S_ServerModify
{
	WORD							wSortID;							//排序索引
	WORD							wKindID;							//类型索引
	WORD							wNodeID;							//节点索引
	WORD							wServerID;							//房间标识
	WORD							wServerPort;						//房间端口
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwFullCount;						//满员人数
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[LEN_SERVER];			//房间名称
};

//房间删除
struct CMD_CS_S_ServerRemove
{
	WORD							wServerID;							//房间标识
};

//////////////////////////////////////////////////////////////////////////////////
//用户汇总

#define MDM_CS_USER_COLLECT			3									//用户汇总

//用户状态
#define SUB_CS_C_USER_ENTER			1									//用户进入
#define SUB_CS_C_USER_LEAVE			2									//用户离开
#define SUB_CS_C_USER_FINISH		3									//用户完成

//用户状态
#define SUB_CS_S_COLLECT_REQUEST	100									//汇总请求

//////////////////////////////////////////////////////////////////////////////////

//用户进入
struct CMD_CS_C_UserEnter
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//辅助信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级
	BYTE							cbAndroid;
};

//用户离开
struct CMD_CS_C_UserLeave
{
	DWORD							dwUserID;							//用户标识
};

//////////////////////////////////////////////////////////////////////////////////
//远程服务

#define MDM_CS_REMOTE_SERVICE		4									//远程服务

//查找服务
#define SUB_CS_C_SEARCH_DATABASE	1									//数据查找
#define SUB_CS_C_SEARCH_CORRESPOND	2									//协调查找

//查找结果
#define SUB_CS_S_SEARCH_DATABASE	100									//数据查找
#define SUB_CS_S_SEARCH_CORRESPOND	101									//协调查找

//////////////////////////////////////////////////////////////////////////////////

//协调查找
struct CMD_CS_C_SearchCorrespond
{
	//连接参数
	DWORD							dwSocketID;							//网络标识
	DWORD							dwClientAddr;						//连接地址

	//查找数据
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//协调查找
struct CMD_CS_S_SearchCorrespond
{
	//连接信息
	DWORD							dwSocketID;							//网络标识
	DWORD							dwClientAddr;						//连接地址

	//数据信息
	WORD							wUserCount;							//用户数目
	tagUserRemoteInfo				UserRemoteInfo[16];					//用户信息
};

//////////////////////////////////////////////////////////////////////////////////
//管理服务

#define MDM_CS_MANAGER_SERVICE		5									//管理服务

#define SUB_CS_C_SYSTEM_MESSAGE 	1									//系统消息
#define SUB_CS_C_PROPERTY_TRUMPET 	2									//喇叭消息
#define SUB_CS_C_VIP_TRUMPET		4
#define SUB_CS_C_VIPUpgrade			5
#define SUB_CS_C_SystemBroadcast	6
#define SUB_CS_C_MatchScore			7
#define SUB_CS_C_PEARL_INFO			8
#define SUB_CS_C_BROAD_LABA			9
#define SUB_CS_C_MatchScoreBegin	10
#define SUB_CS_C_MatchScoreFinish	11
#define SUB_CS_C_MATCH_SIGNUP		12
#define SUB_CS_C_MATCH_USERSCORE_UPDATE		13

#define SUB_CS_S_SYSTEM_MESSAGE 	100									//系统消息
#define SUB_CS_S_PROPERTY_TRUMPET  	200									//喇叭消息
#define SUB_CS_S_LOTTERY_INFO		300
#define SUB_CS_S_MOBILE_MONEY		301
#define SUB_CS_S_VIP_TRUMPET		400
#define SUB_CS_S_VIPUpgrade			500
#define SUB_CS_S_SystemBroadcast	600
#define SUB_CS_S_MatchRank			700
#define SUB_CS_S_PEARL_INFO			800
#define SUB_CS_S_BROAD_LABA			900

// Match MSG
#define SUB_CS_S_MATCH_SIGNUP_RES	1000								//比赛报名结果
#define SUB_CS_S_MATCH_NOTIFICATION 1100								//比赛时间通知
#define SUB_CS_S_MATCH_NOTIFICATION_COUNTDOWN 1200						//比赛时间倒计时
#define SUB_CS_S_MATCH_BEGIN		1300						//比赛开始
#define SUB_CS_S_MATCH_END			1400						//比赛结束
#define SUB_CS_S_MATCH_SCORE_UPDATE	1500						//比赛分数刷新


//发送喇叭
struct CMD_CS_S_SendTrumpet
{
	WORD                           wPropertyIndex;                      //道具索引 
	DWORD                          dwSendUserID;                         //用户 I D
	DWORD                          TrumpetColor;                        //喇叭颜色
	TCHAR                          szSendNickName[32];				    //玩家昵称 
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //喇叭内容
};

//VIP升级
struct CMD_CS_C_VIPUpgrade
{
	BYTE							cbType;
	TCHAR							szTrumpetContent[LEN_USER_CHAT];
};

struct CMD_CS_S_LotteryInfo
{
	int nPrice;
	int nMaxGet;
	int nLessTime;//至少玩多少秒才开始获得
	int nPercent;//获得概率
	int nMinusStorage;//从库存中减掉的分数
};

struct CMD_CS_C_MatchScore
{
	DWORD dwUserID;
	SCORE lMatchScore;
};

struct CMD_CS_S_MatchRank
{
	DWORD dwUserID;
	SCORE lMatchScore;//比赛积分
	int nCurrentRank;//当前排名
	int nChangeRank;//排名变化
};

struct CMD_CS_C_PearlInfo
{
	SCORE lYesterdayScore;
};

struct CMD_CS_S_PearlInfo
{
	DWORD dwUserID;
	int nType;
};

struct CMD_CS_C_BroadLaBa
{
	DWORD dwUserID;
	WORD wSize;
	char szNickName[64];
	char szContent[1024];
};

struct CMD_CS_S_MobileMoney
{
	WORD wPrice;
	WORD wMaxGet;
	int nLessTime;
	int nPercent;
	int nMinusStorage;
};

// 比赛数据
// 比赛报名
//---------------------------------------------------
struct CMD_CS_C_MatchSignUp
{
	DWORD dwUserID;
	WORD wServerID;
	int nMatchID;
};

// 比赛报名结果
struct CMD_CS_S_MatchSignUpRes
{
	DWORD dwUserID;
	WORD  wGroupID;
	int   nMatchID;
	bool  bIsSignUpSuc;
	bool  bMatchStatus;//报名状态，0为未报名，1为已经报名
	SCORE llUserScore;//玩家当前金币
	char szDescription[128];//描述信息
};
//---------------------------------------------------
// 比赛分数更新
//tag 
struct tagMatchUserInfo
{
	DWORD dwUserID;
	WORD  wGroupID;
	SCORE lUserScore;
};

// 比赛实时分数
struct CMD_CS_C_MatchUserScore
{
	WORD  wServerID;
	WORD  wUserCnt;
	WORD  wDataSize;
	tagMatchUserInfo aMatchUserInfo[100];
};

//比赛通知信息 给GameServer中统一发送的消息, 消息的内容全部是一致的
struct CMD_CS_S_MatchInfoNotify
{
	std::vector<DWORD>	vUserIDInGameRoom;
	VOID * pData;
	WORD wVectorSize;
	WORD wDataSize;
};

//比赛开始信息
//struct CMD_CS_S_MatchBegin
//{
//	TCHAR szDescription[128];
//	TCHAR szDescription2[128];
//	int nPeoCount;
//	int nEnterScore;
//	bool bAlreadyIn;
//};
// Match Begin Notify
struct CMD_CS_S_MatchBegin
{
	int nMatchID;//比赛场次
	char szMatchDate[16];//比赛日期
	char szMatchName[128];//比赛标题
};

//struct CMD_CS_S_MatchBegin
//{
//	WORD wVectorSize;
//	std::vector<tagMatchBeginNotfify> vMatchBeginNotify;
//};

//比赛结束信息
struct CMD_CS_S_MatchEnd
{
	TCHAR szDescription[128];
	bool bAlreadyIn;
};

struct tagMatchItem
{
	DWORD dwUserID;
	WORD wServerID;
	SCORE lMatchScore;
	WORD wGroupID;//组号
	BYTE cbRank;
	BYTE cbMatchType;
	BYTE cbMatchNum;
	bool bIsRobot;
	bool bMatchStatus;
	TCHAR szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR szDescription[128];			//用户昵称
};

struct CMD_CS_S_UserInfo
{
	WORD wVectorSize;
	std::vector<tagMatchItem> vUserInfo;
};

struct tagUpdateInfoItem
{
	DWORD dwUserID;
	SCORE lUserScore;
	TCHAR szNickName[LEN_NICKNAME];			//用户昵称
};

struct CMD_CS_S_MatchScoreUpdate
{
	int  nSecond;
	WORD wGroupCnt;							// Count of the Group
	WORD wUserNumPerGroup;					// equals 8 tmp
	WORD wDataSize;
	WORD wUserCnt;
	tagUpdateInfoItem aMatchUserUpdInfo[200];		//协调服务器用户信息汇总
};

//比赛结束信息
struct tagMatchEndInfo
{
	DWORD dwUserID;
	SCORE lUserScore;//积分
	int nRanking;//排名
};

struct CMD_CS_S_MatchEndInfo
{
	WORD wVectorSize;
	WORD wMatchType;
	tagMatchEndInfo	vUserMatchEndInfo[2000];
};

// 比赛通知内容
struct MatchNotification
{
	int nMatchPeopleNum;//参赛人数
	int nMatchScore;//报名费
	int nMatchID;//比赛场次
	TCHAR szNotification[128];
	char szMatchPrise[128];//比赛奖励说明
};

struct CMD_GPO_MatchTimeNotify                          //比赛倒计时倒计时
{
	BYTE cbMatchStatus;//0代表比赛之前倒计时，1代表比赛结束倒计时
	int nSecond;//倒计时提示时间
	TCHAR szTitle[128];
};
//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif