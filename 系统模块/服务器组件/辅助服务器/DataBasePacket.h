#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//请求数据包

//系统命令
#define DBR_GP_LOAD_GAME_LIST		50									//加载列表
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

//在线信息
struct DBR_GP_OnLineCountInfo
{
	WORD							wKindCount;							//类型数目
	DWORD							dwOnLineCountSum;					//总在线数
	tagOnLineInfoKind				OnLineCountKind[MAX_KIND];			//类型在线
};

//////////////////////////////////////////////////////////////////////////////////
//输出信息

//列表结果
#define DBO_GP_GAME_TYPE_ITEM			130									//种类信息
#define DBO_GP_GAME_KIND_ITEM			131									//类型信息
#define DBO_GP_GAME_NODE_ITEM			132									//节点信息
#define DBO_GP_GAME_PAGE_ITEM			133									//定制信息
#define DBO_GP_GAME_LIST_RESULT			134									//加载结果
#define DBO_GR_LOAD_SYSTEM_MESSAGE		151
#define DBO_GP_LOAD_YESTERDAY_SCORE		152

//////////////////////////////////////////////////////////////////////////////////

//游戏类型
struct DBO_GP_GameType
{
	WORD							wJoinID;							//挂接索引
	WORD							wSortID;							//排序索引
	WORD							wTypeID;							//类型索引
	TCHAR							szTypeName[LEN_TYPE];				//种类名字
};

//游戏种类
struct DBO_GP_GameKind
{
	WORD							wTypeID;							//类型索引
	WORD							wJoinID;							//挂接索引
	WORD							wSortID;							//排序索引
	WORD							wKindID;							//类型索引
	WORD							wGameID;							//模块索引
	TCHAR							szKindName[LEN_KIND];				//游戏名字
	TCHAR							szProcessName[LEN_PROCESS];			//进程名字
};

//游戏节点
struct DBO_GP_GameNode
{
	WORD							wKindID;							//名称索引
	WORD							wJoinID;							//挂接索引
	WORD							wSortID;							//排序索引
	WORD							wNodeID;							//节点索引
	TCHAR							szNodeName[LEN_NODE];				//节点名称
};

//定制类型
struct DBO_GP_GamePage
{
	WORD							wKindID;							//名称索引
	WORD							wNodeID;							//节点索引
	WORD							wSortID;							//排序索引
	WORD							wPageID;							//定制索引
	WORD							wOperateType;						//控制类型
	TCHAR							szDisplayName[LEN_PAGE];			//显示名称
};

//加载结果
struct DBO_GP_GameListResult
{
	BYTE							cbSuccess;							//成功标志
};

struct DBO_GP_YesterdayScore
{
	SCORE							lYesterdayScore;
};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif