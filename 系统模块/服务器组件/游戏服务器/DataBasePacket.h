#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//请求数据包

//用户事件
#define	DBR_GR_LOGON_USERID			100									//I D 登录
#define	DBR_GR_LOGON_MOBILE			101									//手机登录
#define	DBR_GR_LOGON_ACCOUNTS		102									//帐号登录
#define DBR_GR_LOGON_MATCHTIMER		103									//定时賽登录

//系统事件
#define DBR_GR_WRITE_GAME_SCORE		200									//游戏积分
#define DBR_GR_LEAVE_GAME_SERVER	201									//离开房间
#define DBR_GR_GAME_SCORE_RECORD	202									//积分记录
#define DBR_GR_MANAGE_USER_RIGHT	203									//权限管理
#define DBR_GR_LOAD_SYSTEM_MESSAGE	204									//系统消息
#define DBR_GR_LOAD_SENSITIVE_WORDS	205									//敏感词
#define DBR_GR_CHECKSTOCK			206									//检查库存
#define DBR_GR_GET_USER_LUCKY		207
#define DBR_GR_SAVEUSERITEM			208									//保存用户物品
#define DBR_GR_GAME_JSON			210
#define DBR_GR_SENTENCE			    211
#define DBR_GR_LOAD_MATCH_CONFIG	212									//加载比赛时间配置信息
#define DBR_GR_LOAD_MATCH_REWARD_CONFIG	213								//加载比赛奖励配置信息

//配置事件
#define DBR_GR_LOAD_PARAMETER		300									//加载参数
#define DBR_GR_LOAD_GAME_COLUMN		301									//加载列表
#define DBR_GR_LOAD_ANDROID_USER	302									//加载机器
#define DBR_GR_LOAD_GAME_PROPERTY	303									//加载道具

//银行事件
#define DBR_GR_USER_SAVE_SCORE		400									//存入游戏币
#define DBR_GR_USER_TAKE_SCORE		401									//提取游戏币
#define DBR_GR_USER_TRANSFER_SCORE	402									//转账游戏币
#define DBR_GR_QUERY_INSURE_INFO	403									//查询银行
#define DBR_GR_QUERY_TRANSFER_USER_INFO	    404							//查询用户

//游戏事件
#define DBR_GR_PROPERTY_REQUEST		500									//购买礼物
#define DBR_GR_GAME_FRAME_REQUEST	502									//游戏请求
#define DBR_GR_CHECK_TRUMPET		503									//大喇叭
#define DBR_GR_FIRST_ENTER_SUPER	504									//第一次进入超级海盗
//比赛事件
#define DBR_GR_MATCH_FEE			600									//比赛费用
#define DBR_GR_MATCH_START			601									//比赛开始
#define DBR_GR_MATCH_OVER			602									//比赛结束
#define DBR_GR_MATCH_REWARD			603									//比赛奖励
#define DBR_GR_MATCH_QUIT			604									//退出比赛
#define DBR_GR_MATCH_ENTER_FOR		605									//报名进入

//////////////////////////////////////////////////////////////////////////////////
//输出信息

//逻辑事件
#define DBO_GR_LOGON_SUCCESS		100									//登录成功
#define DBO_GR_LOGON_FAILURE		101									//登录失败

//配置事件
#define DBO_GR_GAME_PARAMETER		200									//配置信息
#define DBO_GR_GAME_COLUMN_INFO		201									//列表信息
#define DBR_GR_GAME_ANDROID_INFO	202									//机器信息
#define DBO_GR_GAME_PROPERTY_INFO	203									//道具信息
#define DBR_GR_CHECKSTOCK_SUCCESS	204									//检查库存成功
#define DBR_GET_USER_LUCKY_SUCCESS	205
#define DBR_LOADUSERITEM_SUCCESS	206									//加载用户物品成功
#define DBO_GR_PRESENT_SCORE		207

//银行命令
#define DBO_GR_USER_INSURE_INFO		300									//银行资料
#define DBO_GR_USER_INSURE_SUCCESS	301									//银行成功
#define DBO_GR_USER_INSURE_FAILURE	302									//银行失败
#define DBO_GR_USER_INSURE_USER_INFO   303								//用户资料
#define DBO_GR_USER_INSURE_TRANSFER_INFO	304

//游戏事件
#define DBO_GR_PROPERTY_SUCCESS		400									//道具成功
#define DBO_GR_PROPERTY_FAILURE		401									//道具失败
#define DBO_GR_GAME_FRAME_RESULT	402									//游戏结果
#define DBO_GR_CHECK_TRUMPET		403

//比赛事件
#define DBO_GR_MATCH_EVENT_START	500									//事件范围
#define DBO_GR_MATCH_FEE_RESULT		500									//比赛费用
#define DBO_GR_MATCH_RANK			501									//比赛名次
#define DBO_GR_MATCH_QUIT_RESULT	502									//退赛结果
#define DBO_GR_MATCH_EVENT_END		599									//事件范围

//系统事件
#define DBO_GR_SYSTEM_MESSAGE_RESULT	600									//消息结果
#define DBO_GR_SENSITIVE_WORDS		    601									//敏感词

//////////////////////////////////////////////////////////////////////////////////

//ID 登录
struct DBR_GR_LogonUserID
{		
	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//ID 登录
struct DBR_GR_LogonMobile
{		
	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	BYTE                            cbDeviceType;                       //设备类型
	WORD                            wBehaviorFlags;                     //行为标识
	WORD                            wPageTableCount;                    //分页桌数

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//帐号登录
struct DBR_GR_LogonAccounts
{
	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
};

//游戏积分
struct DBR_GR_WriteGameScore
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwClientAddr;						//连接地址

	//用户信息
	DWORD							dwDBQuestID;						//请求标识
	DWORD							dwInoutIndex;						//进出索引

	//成绩变量
	tagVariationInfo				VariationInfo;						//提取信息
};

//离开房间
struct DBR_GR_LeaveGameServer
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwInoutIndex;						//记录索引
	DWORD							dwLeaveReason;						//离开原因
	DWORD							dwOnLineTimeCount;					//在线时长

	//成绩变量
	tagVariationInfo				RecordInfo;							//记录信息
	tagVariationInfo				VariationInfo;						//提取信息

	//系统信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szNickName[LEN_NICKNAME];
	TCHAR							szServerName[LEN_SERVER];			//房间名称
};

//游戏记录
struct DBR_GR_GameScoreRecord
{
	//桌子信息
	WORD							wTableID;							//桌子号码
	WORD							wUserCount;							//用户数目
	WORD							wAndroidCount;						//机器数目

	//损耗税收
	SCORE							lWasteCount;						//损耗数目
	SCORE							lRevenueCount;						//税收数目

	//统计信息
	DWORD							dwUserMemal;						//奖牌数目
	DWORD							dwPlayTimeCount;					//游戏时间

	//时间信息
	SYSTEMTIME						SystemTimeStart;					//开始时间
	SYSTEMTIME						SystemTimeConclude;					//结束时间

	//积分记录
	WORD							wRecordCount;						//记录数目
	tagGameScoreRecord				GameScoreRecord[MAX_CHAIR];			//游戏记录
};

//存入游戏币
struct DBR_GR_UserSaveScore
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	SCORE							lSaveScore;							//存入游戏币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//取出游戏币
struct DBR_GR_UserTakeScore
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	SCORE							lTakeScore;							//提取游戏币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//转账游戏币
struct DBR_GR_UserTransferScore
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	BYTE                            cbByNickName;                       //昵称赠送
	SCORE							lTransferScore;						//转账游戏币
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//查询银行
struct DBR_GR_QueryInsureInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
};

//查询用户
struct DBR_GR_QueryTransferUserInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	BYTE                            cbByNickName;                       //昵称赠送
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
	DWORD							dwUserID;							//用户 I D
};

//道具请求
struct DBR_GR_PropertyRequest
{
	//购买信息
	WORD							wItemCount;							//购买数目
	WORD							wPropertyIndex;						//道具索引
	DWORD							dwSourceUserID;						//购买对象
	DWORD							dwTargetUserID;						//使用对象

	//消费模式
	BYTE                            cbRequestArea;			            //使用范围 
	BYTE							cbConsumeScore;						//积分消费
	SCORE							lFrozenedScore;						//冻结积分

	//用户权限
	DWORD                           dwUserRight;						//会员权限

	//系统信息
	WORD							wTableID;							//桌子号码
	DWORD							dwInoutIndex;						//进入标识
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//大喇叭
struct DBR_GR_CheckTrumpet
{
	DWORD                          dwUserID;
	BYTE                           cbRequestArea;						//请求范围 
	WORD                           wPropertyIndex;						//道具索引 
	DWORD                          TrumpetColor;						//喇叭颜色
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];	//喇叭内容
	LONG                           lReturnValue;
};

//////////////////////////////////////////////////////////////////////////////////

//登录成功
struct DBO_GR_LogonSuccess
{
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwGroupID;							//社团 I D
	DWORD							dwCustomID;							//自定头像
	TCHAR							szNickName[LEN_NICKNAME];			//帐号昵称
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字

	//用户资料
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lGrade;								//用户成绩
	SCORE							lInsure;							//用户银行

	//游戏信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//用户经验
	LONG							lLoveLiness;						//用户魅力

	//用户权限
	DWORD							dwUserRight;						//用户权限
	DWORD							dwMasterRight;						//管理权限

	//索引变量
	DWORD							dwInoutIndex;						//记录索引
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识

	//手机定义
	BYTE                            cbDeviceType;                       //设备类型
	WORD                            wBehaviorFlags;                     //行为标识
	WORD                            wPageTableCount;                    //分页桌数

	BYTE							cbVIPLevel;
	DWORD							dwVIPExp;

	//辅助信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szDescribeString[128];				//错误消息
	int								nTreasureMapCount;					//藏宝图数量
};

//登录失败
struct DBO_GR_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//错误消息
};

//配置信息
struct DBO_GR_GameParameter
{
	//汇率信息
	WORD							wMedalRate;							//奖牌汇率
	WORD							wRevenueRate;						//银行税收

	//版本信息
	DWORD							dwClientVersion;					//客户版本
	DWORD							dwServerVersion;					//服务版本
};

//列表信息
struct DBO_GR_GameColumnInfo
{
	LONG							lResultCode;						//结果代码
	BYTE							cbColumnCount;						//列表数目
	tagColumnItem					ColumnItemInfo[MAX_COLUMN];			//列表信息
};

//机器信息
struct DBO_GR_GameAndroidInfo
{
	LONG							lResultCode;						//结果代码
	WORD							wAndroidCount;						//用户数目
	tagAndroidParameter				AndroidParameter[MAX_ANDROID];		//机器信息
};

//道具信息
struct DBO_GR_GamePropertyInfo
{
	LONG							lResultCode;						//结果代码
	BYTE							cbPropertyCount;					//道具数目
	tagPropertyInfo					PropertyInfo[MAX_PROPERTY];			//道具信息
};

//银行资料
struct DBO_GR_UserInsureInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserInsure;						//银行游戏币
	SCORE							lTransferPrerequisite;				//转账条件
};

//银行成功
struct DBO_GR_UserInsureSuccess
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwUserID;							//用户 I D
	SCORE							lSourceScore;						//原来游戏币
	SCORE							lSourceInsure;						//原来游戏币
	SCORE							lInsureRevenue;						//银行税收
	SCORE							lFrozenedScore;						//冻结积分
	SCORE							lVariationScore;					//游戏币变化
	SCORE							lVariationInsure;					//银行变化
	TCHAR							szDescribeString[128];				//描述消息
};

//银行失败
struct DBO_GR_UserInsureFailure
{
	BYTE                            cbActivityGame;                     //游戏动作
	LONG							lResultCode;						//操作代码
	SCORE							lFrozenedScore;						//冻结积分
	TCHAR							szDescribeString[128];				//描述消息
};

//用户资料
struct DBO_GR_UserTransferUserInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	DWORD							dwGameID;							//用户 I D
	TCHAR							szNickName[LEN_NICKNAME];			//帐号昵称
};

//道具成功
struct DBO_GR_S_PropertySuccess
{
	//道具信息
	WORD							wItemCount;							//购买数目
	WORD							wPropertyIndex;						//道具索引
	DWORD							dwSourceUserID;						//购买对象
	DWORD							dwTargetUserID;						//使用对象

	//消费模式
	BYTE                            cbRequestArea;						//请求范围
	BYTE							cbConsumeScore;						//积分消费
	SCORE							lFrozenedScore;						//冻结积分

	//会员权限
	DWORD                           dwUserRight;						//会员权限

	//结果信息
	SCORE							lConsumeGold;						//消费游戏币
	LONG							lSendLoveLiness;					//赠送魅力
	LONG							lRecvLoveLiness;					//接受魅力

	//会员信息
	BYTE							cbMemberOrder;						//会员等级
};

//道具失败
struct DBO_GR_PropertyFailure
{
	BYTE                            cbRequestArea;						//请求范围
	LONG							lResultCode;						//操作代码
	SCORE							lFrozenedScore;						//冻结积分
	TCHAR							szDescribeString[128];				//描述消息
};

//比赛名次
struct DBO_GR_MatchRank
{
	BYTE							cbRank;								//比赛名次
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	LONG							lMatchScore;						//用户得分
};

//泡分
struct DBO_GR_PresentScore
{
	int								nScore;
	int								nTime;
	TCHAR							szNickName[LEN_NICKNAME];
	TCHAR							szServerName[LEN_SERVER];
};

//权限管理
struct DBR_GR_ManageUserRight
{
	DWORD							dwUserID;							//目标用户
	DWORD							dwAddRight;							//添加权限
	DWORD							dwRemoveRight;						//删除权限
	bool							bGameRight;							//游戏权限
};

//比赛报名
struct DBR_GR_MatchFee
{
	//报名费用
	DWORD							dwUserID;							//用户ID
	DWORD							dwMatchFee;							//报名费用

	//系统信息
	DWORD							dwInoutIndex;						//进入标识
	DWORD							dwClientAddr;						//连接地址
	DWORD							dwMatchID;							//比赛ID
	DWORD							dwMatchNO;							//比赛场次
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//比赛开始
struct DBR_GR_MatchStart
{
	DWORD							dwMatchID;						//比赛标识
	DWORD							dwMatchNo;						//比赛场次
	WORD							wMatchCount;					//总场次
	LONG							lInitScore;						//初始积分
};

//比赛结束
struct DBR_GR_MatchOver
{
	DWORD							dwMatchID;						//比赛标识
	DWORD							dwMatchNo;						//比赛场次
	WORD							wMatchCount;					//总场次
};

//比赛奖励
struct DBR_GR_MatchReward
{
	DWORD								dwUserID;							//用户 I D
	DWORD								dwMatchID;							//比赛 I D 
	DWORD								dwMatchNO;							//比赛场次
	WORD								wRank;								//比赛名次
	SCORE								lMatchScore;						//比赛得分
	DWORD								dwExperience;						//用户经验
	DWORD								dwGold;								//游戏币
	DWORD								dwMedal;							//用户奖牌
	DWORD								dwClientAddr;						//连接地址
};

//定时模式比赛登录
struct DBR_GR_MatchEnterFor
{
	DBR_GR_LogonUserID			LogonUserID;
};


//系统消息
struct DBR_GR_SystemMessage
{
	BYTE                            cbMessageID;                        //消息ID
	BYTE							cbMessageType;						//消息类型
	BYTE                            cbAllRoom;                          //全体房间
	DWORD							dwTimeRate;						    //时间频率
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//系统消息
};

//游戏记录Json格式
struct DBR_GR_GameJson
{
	DWORD							dwUserID;
	DWORD							dwKindID;
	DWORD							dwServerID;
	SCORE							lPresentGold;
	TCHAR							szJson[1024];
};

//执行数据库脚本
struct DBR_GR_Sentence
{
	TCHAR							szSQL[1024];
};

#define DBR_GR_USER_TRANSFER_DRAGON 700
#define DBO_GR_USER_TRANSER_DRAGON 701

//赠送龙珠
struct DBR_GR_UserTransferDragon
{
	DWORD dwUserID;
	DWORD dwClientAddr;
	BYTE cbByNickName;
	SCORE lTransferDragon;
	TCHAR szNickName[LEN_NICKNAME];
	TCHAR szPassword[LEN_PASSWORD];
	TCHAR szMachineID[LEN_MACHINE_ID];
};

struct DBO_GR_UserTransferDragon
{
	SCORE lDragon;
	SCORE lVariationDragon;
	SCORE lSourceScore;
	SCORE lVariationScore;
	LONG lResultCode;
	TCHAR szDescribeString[128];
};

#define DBR_GR_USER_USE_DRAGON 702

//使用龙珠
struct DBR_GR_UserUseDragon
{
	DWORD dwUserID;
	SCORE lUseDragon;
	DWORD dwClientAddr;
	TCHAR szPassword[LEN_PASSWORD];
	TCHAR szMachineID[LEN_MACHINE_ID];
};

#define DBR_GR_USER_WIN_DRAGON_PIECE 703
#define DBO_GR_USER_WIN_DRAGON_PIECE 704

//赢得龙珠碎片
struct DBR_GR_UserWinDragonPiece
{
	DWORD dwUserID;
	SCORE lWinDragonPiece;
	SCORE lWinScore;
	DWORD dwClientAddr;
	TCHAR szMachineID[LEN_MACHINE_ID];
};

struct DBO_GR_UserWinDragonPiece
{
	SCORE lWinDragonPiece;
	SCORE lWinScore;
};

#define DBR_GR_QUERY_SKILL 705
#define DBO_GR_QUERY_SKILL 706

struct DBR_GR_QuerySkill
{
	DWORD dwUserID;
};

struct DBO_GR_QuerySkill
{
	int nCannonStatus;//炮台状态
	int nJiaSu;//加速
	int nSanShe;//散射
	int nBaoJi;//暴击
	BYTE cbCount[3];//数量
	BYTE cbPrice[7];//价格
};

#define DBR_GR_USE_SKILL 707
#define DBO_GR_USE_SKILL 708

struct DBR_GR_UseSkill
{
	DWORD dwUserID;
	BYTE cbSkillID;
};

struct DBO_GR_UseSkill
{
	BYTE cbSkillID;
	int nCount;
};

#define DBR_GR_SKILL_GIVE 709
#define DBO_GR_SKILL_GIVE 710

struct DBR_GR_GiveSkill
{
	DWORD dwUserID;
};

struct DBO_GR_GiveSkill
{
	BYTE cbSkillID;
	int nCount;
};

#define DBR_GR_NEW_GUID_GIFT 711
#define DBO_GR_NEW_GUID_GIFT 712

struct DBR_GR_NewGuidGift
{
	DWORD dwUserID;
};

struct DBO_GR_NewGuidGift
{
	bool bSuccess;
	int nCannonStatus;//炮台状态
	int nLotteryCount;//奖券数量
};

#define DBR_GR_WIN_PEARL 713
#define DBO_GR_WIN_PEARL 714

struct DBR_GR_WinPearl
{
	DWORD dwUserID;
	int nPearlType;
	int nPearlCount;
};

struct DBR_GO_WinPearl
{
	int nPearlType;
	int nPearlCount;
};

//用户日志
#define DBR_USERLOG_CLOWN_OPERATION		715
struct tagGuessOperation
{
	int   nStarCount;
	int   nChipTimes;	//倍数	
};
//小丑操作日志
struct tagClownOperation
{
	int   nRoundNum;	//第几轮操作, 转盘还是猜大小
	bool  bIsWin;       //是否成功
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
	std::vector<tagClownOperation> *	pClownOperation;					//记录一个环节所有的操作
};

#define DBR_GR_UPDATE_TREASUREMAP 716
#define DBO_GR_UPDATE_TREASUREMAP 717

struct DBR_GR_Update_Treasuremap
{
	DWORD dwUserID;
	int		nCount;
};

struct DBO_GR_Update_Treasuremap
{
	int		nCount;
};

#define DBR_GR_FIRST_ENTER_SUPER 718
#define DB0_GR_FIRST_ENTER_SUPER 719
struct DBR_GR_First_Enter_Super
{
	DWORD dwUserID;
};
struct DBO_GR_First_Enter_Super
{
	int nRewardCoin;
};

#define DBR_GR_UPDATE_KVALUEANDINTEGRATION 720
#define DBO_GR_UPDATE_KVALUEANDINTEGRATION 721

struct DBR_GR_Update_KValueAndIntegration
{
	DWORD dwUserID;
	int		nKValue;
	int		nIntegration;
};

struct DBO_GR_Update_KValueAndIntegration
{
	int		nKValue;
	int		nIntegration;
};

#define DBR_GR_UPDATE_MAXKVALUE 722
#define DBO_GR_UPDATE_MAXKVALUE 723

struct DBR_GR_Update_MaxKValue
{
	DWORD dwUserID;
	int		nCount;
};

struct DBO_GR_Update_MaxKValue
{
	int		nCount;
};

#define DBR_GET_USER_ITEM_COUNT 724
#define DBO_GET_USER_ITEM_COUNT 725

struct DBR_GetUserItemCount
{
	DWORD dwUserID;
	int nType;
};

struct DBO_GetUserItemCount
{
	int nType;
	SCORE lCount;
};

#define DBR_SAVE_USER_ITEM_COUNT 726
#define DBO_SAVE_USER_ITEM_COUNT 727

struct DBR_SaveUserItemCount
{
	DWORD dwUserID;
	int nType;
	SCORE lCount;
};

struct DBO_SaveUserItemCount
{
	int nType;
	SCORE lCount;
};

#define DBO_LOAD_MATCH_CONFIG 728

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


// 比赛报名
#define  DBR_GR_MATCH_USERSIGNUP		729
#define  DBO_GR_MATCH_USERSIGNUP		730
struct DBR_GR_Match_UserSignup
{
	DWORD dwUserID;
};

struct DBO_GR_Match_UserSignUp
{
	DWORD dwUserID;
	bool  bIsMatchSignUPSuccess;
	TCHAR szDescribeString[128];				//错误消息
};

#define DBO_LOAD_MATCH_REWARD_CONFIG 731

struct DBO_Load_Match_Reward_Config_Item
{
	int nRankStart;
	int nRankEnd;
	int nReward[10];
	int nMachType;
	int nShareType;
	TCHAR szReward[128];
	TCHAR szShare[128];
};

// 比赛报名
#define  DBR_GR_MATCH_GET_PRIZE		732
#define  DBO_GR_MATCH_GET_PRIZE		733
struct DBR_GR_Match_Get_Prize
{
	int nMatchID;
	DWORD dwUserID;
	char szMatchData[16];
};

struct DBO_GR_Match_Get_Prize
{
	bool bPriseStatus;//领取状态，0为领取失败，1为领取成功
	BYTE nPriseType;//比赛奖励类型
	int nPriseCount;//领取数量
	SCORE lluserScore;
	int nMatchId;
};

#define DBR_MODIFY_BACKPACK 734

struct DBR_ModifyBackpack
{
	DWORD dwUserID;
	BYTE cbType;
	int nChange;
};

#define  DBR_GR_BUY_SKILL		735
#define  DBO_GR_BUY_SKILL		736

struct DBR_GR_Buy_Skill
{
	DWORD dwUserID;
	int nSkillID;
	int nCount;
	SCORE llScore;
};

struct DBO_GR_Buy_Skill_Res
{
	bool bSuccess;
	BYTE cbSkillID;
	int nCount;
	SCORE llCostScore;
	TCHAR szDescribeString[128];
};

#define  DBR_GR_BROAD_LABA		737
#define  DBO_GR_BROAD_LABA		738

struct DBR_GR_Broad_Laba
{
	DWORD dwUserId;
	WORD wSize;
	SCORE llUserScore;
	char szNickName[64];
	char szContent[1024];
};

struct DBO_GR_Broad_Laba_Res
{
	WORD lResult;
	SCORE llCostScore;
	WORD wSize;
	char szNickName[64];
	TCHAR szErrorDes[128];
	TCHAR szContent[1024];
};

//////////////////////////////////////////////////////////////////////////////////

#endif