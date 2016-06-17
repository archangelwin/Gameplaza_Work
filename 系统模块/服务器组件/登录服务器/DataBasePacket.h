#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//请求数据包

//登录命令
#define	DBR_GP_LOGON_GAMEID			1									//I D 登录
#define	DBR_GP_LOGON_ACCOUNTS		2									//帐号登录
#define DBR_GP_REGISTER_ACCOUNTS	3									//注册帐号
#define DBR_GP_GUEST_REGISTER		4									//游客登录
#define DBR_GP_RECORD_LOGON			5

//账号服务
#define DBR_GP_MODIFY_MACHINE		10									//修改机器
#define DBR_GP_MODIFY_LOGON_PASS	11									//修改密码
#define DBR_GP_MODIFY_INSURE_PASS	12									//修改密码
#define DBR_GP_MODIFY_UNDER_WRITE	13									//修改签名
#define DBR_GP_MODIFY_INDIVIDUAL	14									//修改资料

//头像命令
#define DBR_GP_MODIFY_SYSTEM_FACE	20									//修改头像
#define DBR_GP_MODIFY_CUSTOM_FACE	21									//修改头像

//银行命令
#define DBR_GP_USER_SAVE_SCORE		30									//存入游戏币
#define DBR_GP_USER_TAKE_SCORE		31									//提取游戏币
#define DBR_GP_USER_TRANSFER_SCORE	32									//转账游戏币
#define DBR_GP_USER_CHECK_PASSWORD	33

//查询命令
#define DBR_GP_QUERY_INDIVIDUAL		40									//查询资料
#define DBR_GP_QUERY_INSURE_INFO	41									//查询银行
#define DBR_GP_QUERY_USER_INFO	    42									//查询用户

//系统命令
#define DBR_GP_LOAD_GAME_LIST		50									//加载列表
#define DBR_GP_ONLINE_COUNT_INFO	51									//在线信息

//签到命令
#define DBR_GP_QUERY_SIGIN_INFO		61									//查询签到
#define DBR_GP_USER_SIGIN			62									//点击签到

//抽奖命令
#define DBR_GP_QUERY_LOTTERY_INFO	63									//抽奖查询
#define DBR_GP_LOTTERY_REQ			64									//抽奖操作


//////////////////////////////////////////////////////////////////////////////////

//ID 登录
struct DBR_GP_LogonGameID
{
	//登录信息
	DWORD							dwGameID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	BYTE							cbNeeValidateMBCard;				//密保校验

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//帐号登录
struct DBR_GP_LogonAccounts
{
	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	BYTE							cbNeeValidateMBCard;				//密保校验

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//帐号注册
struct DBR_GP_RegisterAccounts
{
	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	DWORD							dwAgentID;							//推荐人ID
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
	TCHAR							szSpreader[LEN_ACCOUNTS];			//推荐帐号
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//安全手机

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码(MD5加密)
	TCHAR							szInsurePass[LEN_MD5];				//银行密码
	TCHAR							szGuestPass[LEN_PASSWORD];			//登录密码(未加密)

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szIDFA[40];

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

struct DBR_GP_RecordLogon
{
	BYTE							cbDeviceType;
	BYTE							cbChannel;
	TCHAR							szClientVersion[8];
	TCHAR							szLAN[16];
	TCHAR							szWAN[16];
	TCHAR							szAccounts[32];
	TCHAR							szLogonPass[32];
	TCHAR							szMachineID[32];
	TCHAR							szComputerName[32];
	TCHAR							szUserData1[32];
	TCHAR							szUserData2[32];
	TCHAR							szUserData3[32];
	TCHAR							szUserData4[32];
	TCHAR							szUserData5[32];
	TCHAR							szUserData6[32];
	TCHAR							szUserData7[32];
	TCHAR							szUserData8[32];
};

//修改机器
struct DBR_GP_ModifyMachine
{
	BYTE							cbBind;								//绑定标志
	DWORD							dwUserID;							//用户标识
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改密码
struct DBR_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改密码
struct DBR_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改签名
struct DBR_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
};

//修改头像
struct DBR_GP_ModifySystemFace
{
	//用户信息
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//机器信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改头像
struct DBR_GP_ModifyCustomFace
{
	//用户信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//图片信息

	//机器信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改资料
struct DBR_GP_ModifyIndividual
{
	//验证资料
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码

	//帐号资料
	BYTE							cbGender;							//用户性别
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	//用户信息
	TCHAR							szUserNote[LEN_USER_NOTE];			//用户说明
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字

	//电话号码
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//固定电话
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//移动电话

	//联系资料
	TCHAR							szQQ[LEN_QQ];						//Q Q 号码
	TCHAR							szEMail[LEN_EMAIL];					//电子邮件
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//联系地址
};

//查询资料
struct DBR_GP_QueryIndividual
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
};

//存入游戏币
struct DBR_GP_UserSaveScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lSaveScore;							//存入游戏币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//取出游戏币
struct DBR_GP_UserTakeScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lTakeScore;							//提取游戏币
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//转账游戏币
struct DBR_GP_UserTransferScore
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
	BYTE                            cbByNickName;                       //昵称赠送
	SCORE							lTransferScore;						//转账游戏币
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szPassword[LEN_PASSWORD];			//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//查询银行
struct DBR_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//连接地址
};

//查询用户
struct DBR_GP_QueryInsureUserInfo
{
	BYTE                            cbByNickName;                       //昵称赠送
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
};

//用户资料
struct DBO_GP_UserTransferUserInfo
{
	DWORD							dwGameID;							//用户 I D
	TCHAR							szNickName[LEN_NICKNAME];			//帐号昵称
};

//在线信息
struct DBR_GP_OnLineCountInfo
{
	WORD							wKindCount;							//类型数目
	DWORD							dwOnLineCountSum;					//总在线数
	tagOnLineInfoKind				OnLineCountKind[MAX_KIND];			//类型在线
};

//签到信息查询
struct DBR_GP_QuerySignInInfo
{
	DWORD dwUserID;
};

//点击签到
struct DBR_GP_UserSignIn
{
	DWORD dwUserID;
	BYTE  cbVip;								//是否是Vip用户, 0 是普通用户签到, 1 是Vip签到
};

//抽奖信息查询
struct DBR_GP_QryLtryInfo
{
	DWORD dwUserID;
};

//用户抽奖操作
struct DBR_GP_UserLtry
{
	DWORD dwUserID;
};


//////////////////////////////////////////////////////////////////////////////////
//输出信息

//登录结果
#define DBO_GP_LOGON_SUCCESS		100									//登录成功
#define DBO_GP_LOGON_FAILURE		101									//登录失败
#define DBO_GP_VALIDATE_MBCARD		102									//登录失败
#define DBO_GP_VALIDATE_PASSPOSTID	103									//登录失败
#define DBO_GP_GUEST_LOGON_SUCCESS	104									//游客登录

//操作结果
#define DBO_GP_USER_FACE_INFO		110									//用户头像
#define DBO_GP_USER_INDIVIDUAL		111									//用户资料

//银行命令
#define DBO_GP_USER_INSURE_INFO		120									//银行资料
#define DBO_GP_USER_INSURE_SUCCESS	121									//银行成功
#define DBO_GP_USER_INSURE_FAILURE	122									//银行失败
#define DBO_GP_USER_INSURE_USER_INFO   123								//用户资料
#define DBO_GP_USER_INSURE_TRANSFER_INFO	124
#define DBO_GP_USER_CHECK_PASSWORD	125

//列表结果
#define DBO_GP_GAME_TYPE_ITEM			130									//种类信息
#define DBO_GP_GAME_KIND_ITEM			131									//类型信息
#define DBO_GP_GAME_NODE_ITEM			132									//节点信息
#define DBO_GP_GAME_PAGE_ITEM			133									//定制信息
#define DBO_GP_GAME_LIST_RESULT			134									//加载结果

//签到结果
#define DBO_GP_SIGNIN_QUERY_SUCCESS		201									//查询成功
#define DBO_GP_SIGNIN_QUERY_FAILURE		202									//查询失败
#define DBO_GP_USER_SIGNIN_SUCCESS		203									//查询成功
#define DBO_GP_USER_SIGNIN_FAILURE		204									//查询失败

//抽奖结果
#define DBO_GP_LOTTERY_QUERY_RESULT		205									//抽奖成功
#define DBO_GP_USER_LOTTERY_RESULT		206									//抽奖成功


//服务结果
#define DBO_GP_OPERATE_SUCCESS			500									//操作成功
#define DBO_GP_OPERATE_FAILURE			501									//操作失败

//登录成功
struct DBO_GP_LogonSuccess
{
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	DWORD							dwGroupID;							//社团标识
	DWORD							dwCustomID;							//自定索引
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字

	//用户成绩
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserInsure;						//用户银行

	//用户资料
	BYTE							cbGender;							//用户性别
	BYTE							cbMoorMachine;						//锁定机器
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	//会员资料
	BYTE							cbMemberOrder;						//会员等级
	SYSTEMTIME						MemberOverDate;						//到期时间

	//描述信息
	TCHAR							szDescribeString[128];				//描述消息
	int								nTotalCharge;
	int								nTotalPlayTime;

	BYTE							cbVIPLevel;
	DWORD							dwVIPExp;//VIP经验
	DWORD							dwExpPerCent;//一分钱对应的经验值
	TCHAR							szCustomerServiceQQ[17];
	DWORD							dwLotteryCount;
	int								nLogonGift;
	BYTE							cbCharge1stCount;
	BYTE							cbCharge1stCanGet;
	int								nCanShare;
	BYTE							cbCanGetBankruptcy;
	int								nBankruptcyCount;
	bool							bIsLogin;
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];
};

//登录失败
struct DBO_GP_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
	bool							bIsLogin;
};

//登录失败
struct DBR_GP_ValidateMBCard
{
	UINT							uMBCardID;						//机器序列
};

//头像信息
struct DBO_GP_UserFaceInfo
{
	WORD							wFaceID;							//头像标识
	DWORD							dwCustomID;							//自定索引
};

//个人资料
struct DBO_GP_UserIndividual
{
	//用户信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szUserNote[LEN_USER_NOTE];			//用户说明
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字

	//电话号码
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//固定电话
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//移动电话

	//联系资料
	TCHAR							szQQ[LEN_QQ];						//Q Q 号码
	TCHAR							szEMail[LEN_EMAIL];					//电子邮件
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//联系地址
};

//银行资料
struct DBO_GP_UserInsureInfo
{
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户游戏币
	SCORE							lUserInsure;						//银行游戏币
	SCORE							lTransferPrerequisite;				//转账条件
};

//银行成功
struct DBO_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lSourceScore;						//原来游戏币
	SCORE							lSourceInsure;						//原来游戏币
	SCORE							lInsureRevenue;						//银行税收
	SCORE							lVariationScore;					//游戏币变化
	SCORE							lVariationInsure;					//银行变化
	TCHAR							szDescribeString[128];				//描述消息
};

//银行失败
struct  DBO_GP_UserInsureFailure
{
	LONG							lResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//描述消息
};

//操作失败
struct DBO_GP_OperateFailure
{
	LONG							lResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//描述消息
};

//操作成功
struct DBO_GP_OperateSuccess
{
	LONG							lResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//成功消息
};

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
//////////////////////////////////////////////////////////////////////////
//签到信息
struct DBO_GP_UserSignInInfo
{
	INT nNum1;
	INT nNum2;
	tagSignInDayInfo normal[7];
	tagSignInDayInfo vip[7];
};

//点击签到结果
struct DBO_GP_UserSignInResult
{
	BOOL bSuccess;					//是否成功
	BYTE cbUserStatus;				//VIP or Normal
	SCORE llScoreCount;				//奖励金币数量
	SCORE llLotteryCount;			//奖励奖券数量
	TCHAR szDescription[128];		//签到结果描述
};

//查询抽奖结果
struct DBO_GP_Ltry_Qry_Res
{
	WORD							wFreeCount;
	TCHAR							szDescribeString[128];				//描述消息
};

//用户抽奖结果
struct DBO_GP_User_Ltry_Res
{
	WORD							wIsLtrySuc;							//抽奖是否成功
	TCHAR							szNickName[LEN_NICKNAME];			//用户名称
	WORD							wPrizeID;							//奖项ID
	WORD							wFreeCount;							//免费次数
	SCORE                           lUserScore;                        //玩家当前剩余金币
	TCHAR							szDescribeString[128];				//描述消息
};
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//手机数据包

//登录命令
#define	DBR_MB_LOGON_GAMEID			500									//I D 登录
#define	DBR_MB_LOGON_ACCOUNTS		501									//帐号登录
#define DBR_MB_REGISTER_ACCOUNTS	502									//注册帐号

//登录结果
#define DBO_MB_LOGON_SUCCESS		600									//登录成功
#define DBO_MB_LOGON_FAILURE		601									//登录失败

//////////////////////////////////////////////////////////////////////////////////

//ID 登录
struct DBR_MB_LogonGameID
{
	//登录信息
	DWORD							dwGameID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//帐号登录
struct DBR_MB_LogonAccounts
{
	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//帐号注册
struct DBR_MB_RegisterAccounts
{
	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//附加信息
	DWORD							dwClientAddr;						//连接地址
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码

	//连接信息
	LPVOID							pBindParameter;						//绑定参数
};

//登录成功
struct DBO_MB_LogonSuccess
{
	//用户属性
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称

	//描述信息
	TCHAR							szDescribeString[128];				//描述消息
};


//登录失败
struct DBO_MB_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//邮件命令
#define DBR_GP_QUERY_MAIL_ID_LIST	70
#define DBO_GP_QUERY_MAIL_ID_LIST	170
#define DBR_GP_QUERY_MAIL_INFO		71
#define DBO_GP_QUERY_MAIL_INFO		171
#define DBR_GP_QUERY_MAIL_CONTENT	72
#define DBO_GP_QUERY_MAIL_CONTENT	172
#define DBR_GP_QUERY_MAIL_ITEM		73
#define DBO_GP_QUERY_MAIL_ITEM		173

struct DBR_MailIDList
{
	DWORD dwUserID;
};

struct DBO_MailIDList
{
	BYTE cbCount;
	DWORD dwMailID[30];
};

struct DBR_MailInfo
{
	DWORD dwUserID;
	BYTE cbCount;
	DWORD dwMailID[30];
};

struct DBO_MailInfo
{
	unsigned int dwMailID;
	BYTE cbMailStatus;
	BYTE cbCanGet;//0无领悟物品，1有领取物品，2已领取物品，具体领取的物品由内容进行描述
	char szMailTime[20];//例：2015-08-06 17:33:25，共计19个字符，即19个字节
	char szMailTitle[64];
};

struct DBR_MailContent
{
	DWORD dwMailID;
};

struct DBR_MailItem
{
	DWORD dwMailID;
};

struct DBO_MailItem
{
	BYTE byResult;
	char szResult[30];
};

//充值到账
#define DBR_GP_CHARGE_VALUE 74
#define DBO_GP_CHARGE_VALUE 174
#define DBR_GP_CHARGE_CANCEL 75

struct DBR_ChargeValue
{
	DWORD dwUserID;
};

struct DBO_ChargeValue
{
	SCORE lChargeValue;
};

struct DBR_ChargeCancel
{
	DWORD dwUserID;
};

//收到网站发来的充值金额信息
#define DBR_GP_WEB_CHARGE_VALUE 76

struct DBR_WebChargeValue
{
	DWORD dwUserID;
	SCORE lChargeValue;
};

//VIP系统
#define DBR_GP_VIP_INFO 77
#define DBO_GP_VIP_INFO 177
#define DBR_GP_VIP_EVERYDAY_GIFT 78
#define DBR_GP_VIP_CHARGE_BACK 79
#define DBO_GP_VIP_RESULT 178

struct DBR_VIPUser
{
	DWORD dwUserID;
};

struct DBO_VIPInfo
{
	BYTE cbVIPLevel;//VIP等级
	BYTE cbReserved[3];//保留
	DWORD dwVIPExp;//VIP经验
	DWORD dwVIPNextExp;//升级经验
	DWORD dwEverydayGift;//每日领取金币，如果为0，说明不能领取，或者已经领取
	DWORD dwChargeBack;//充值返现金币，如果为0，说明当前没有返现
};

//struct DBO_VIPResult
//{
//	WORD wSize;
//	char szDescription[1024];
//};

struct DBO_VIPResult
{
	SCORE lScore;
	char szDescription[128];
};

//玩家当前分数
#define DBR_GP_CURRENT_SCORE 80
#define DBO_GP_CURRENT_SCORE 180

struct DBR_CurrentScore
{
	DWORD dwUserID;
};

struct DBO_CurrentScore
{
	SCORE lScore;
	int nLotteryCount;
	BYTE cbCount;
	BYTE cbCanGet;
	BYTE cbVIPLevel;//VIP等级
	DWORD dwVIPExp;//VIP经验
	DWORD dwExpPerCent;//一分钱对应的经验值
	DWORD dwChargeBack;//充值返现金币，如果为0，说明当前没有返现
};

//登录奖励
#define DBR_GP_LOGON_GIFT 81
#define DBO_GP_LOGON_GIFT 181

struct DBR_LogonGift
{
	DWORD dwUserID;
};

struct DBO_LogonGift
{
	SCORE lScore;
	char szDescription[128];
};

//首充礼包
#define DBR_GP_CHARGE1ST_REQUEST 82
#define DBO_GP_CHARGE1ST_REQUEST 182
#define DBR_GP_CHARGE1ST_GET 83
#define DBO_GP_CHARGE1ST_GET 183

struct DBR_Charge1stRequest
{
	DWORD dwUserID;
};

struct DBO_Charge1stRequest
{
	BYTE cbCount;
	char szDescription[128];
};

struct DBR_Charge1stGet
{
	DWORD dwUserID;
};

struct DBO_Charge1stGet
{
	BYTE cbSuccess;
	BYTE cbCount;
	int nGiftScore;
	SCORE lCurrentScore;
	char szDescription[128];
};

#define DBR_GP_WEALTH_BAG_ADD_SCORE 84
#define DBO_GP_WEALTH_BAG_ADD_SCORE 184

struct DBR_WealthBagAddScore
{
	DWORD dwUserID;
	int nScore;
};

struct DBO_WealthBagAddScore
{
	SCORE lCurrentScore;
};

#define DBR_GP_USER_TRANSFER_DRAGON 87
#define DBO_GP_USER_TRANSER_DRAGON 187

//赠送龙珠
struct DBR_GP_UserTransferDragon
{
	DWORD dwUserID;
	DWORD dwClientAddr;
	BYTE cbByNickName;
	SCORE lTransferDragon;
	TCHAR szNickName[LEN_NICKNAME];
	TCHAR szPassword[LEN_PASSWORD];
	TCHAR szMachineID[LEN_MACHINE_ID];
};

struct DBO_GP_UserTransferDragon
{
	SCORE lDragon;
	SCORE lVariationDragon;
	SCORE lSourceScore;
	SCORE lVariationScore;
	LONG lResultCode;
	TCHAR szDescribeString[128];
};

#define DBR_GP_USER_USE_DRAGON 85

//使用龙珠
struct DBR_GP_UserUseDragon
{
	DWORD dwUserID;
	SCORE lUseDragon;
	DWORD dwClientAddr;
	TCHAR szPassword[LEN_PASSWORD];
	TCHAR szMachineID[LEN_MACHINE_ID];
};

#define DBR_GP_WEB_COMMAND 86
#define DBO_GP_WEB_COMMAND 186

struct DBO_GP_WEBCommand
{
	int nType;
	int nParam1;
	int nParam2;
};

#define DBR_GP_QUERY_ITEM_COUNT 1001
#define DBO_GP_QUERY_ITEM_COUNT 2001

struct DBR_GP_QueryItemCount
{
	DWORD dwUserID;
};

struct DBO_GP_QueryItemCount
{
	int nChuJiCount;
	int nZhongJiCount;
	int nGaoJiCount;
	int nZhiZunCount;	//藏宝图
	int nCannonStatus;//炮台状态
	int nJiaSu;//加速
	int nSanShe;//散射
	int nBaoJi;//暴击
};

#define DBR_GP_TRANSFER_ITEM 1002
#define DBO_GP_TRANSFER_ITEM 2002


struct DBR_GP_TransferItem
{
	DWORD dwUserID;
	DWORD dwTargetGameID;
	int nItemID;//道具类型ID
	int nItemCount;//道具数量
	TCHAR szPassword[LEN_MD5];
	TCHAR szItemName[32];
};

struct DBO_GP_TransferItem
{
	int nItemID;//道具类型ID
	int nSourceCount;//原来的数量
	int nVariationCount;//改变的数量
	int nResultCode;//处理结果
	TCHAR szDescribeString[128];
};

#define DBR_GP_COMBINE_DRAGON 1003
#define DBO_GP_COMBINE_DRAGON 2003

struct DBR_GP_CombineItem
{
	DWORD dwUserID;
	int nCombineType;//1=合成小龙珠，2=合成大龙珠
};

struct DBO_GP_CombineItem
{
	int nDragonPieceCount;//龙珠碎片数量
	int nSmallDragonCount;//小龙珠数量
	int nBigDragonCount;//大龙珠数量
	int nResultCode;//处理结果
	TCHAR szDescribeString[128];
};

#define DBR_GP_USE_DRAGON 1004
#define DBO_GP_USE_DRAGON 2004

struct DBR_GP_UseDragon
{
	DWORD dwUserID;
	int nDragonType;//0初级珍珠，1中级珍珠，2高级珍珠，3至尊珍珠
};

struct DBO_GP_UseDragon
{
	SCORE lCurrentScore;//当前分数
	int nChuJiCount;
	int nZhongJiCount;
	int nGaoJiCount;
	int nZhiZunCount;
	int nResultCode;//处理结果
	TCHAR szDescribeString[128];
};

#define DBR_GP_EQUIPMENT_DRAGON 1005
#define DBO_GP_EQUIPMENT_DRAGON 2005

struct DBR_GP_EquipmentDragon
{
	DWORD dwUserID;
	int nDragonType;//0初级珍珠，1中级珍珠，2高级珍珠，3至尊珍珠
};

struct DBO_GP_EquipmentDragon
{
	int nEquipmentDragonCount;//装备的结果
	int nChuJiCount;
	int nZhongJiCount;
	int nGaoJiCount;
	int nZhiZunCount;
	int nResultCode;//处理结果
	TCHAR szDescribeString[128];
};

#define DBR_GP_QUERY_SKILL 1006
#define DBO_GP_QUERY_SKILL 2006

struct DBR_GP_QuerySkill
{
	DWORD dwUserID;
};

struct DBO_GP_QuerySkill
{
	int nCannonStatus;//炮台状态
	int nJiaSu;//加速
	int nSanShe;//散射
	int nBaoJi;//暴击
	BYTE cbCount[3];//数量
	BYTE cbPrice[7];//价格
};


#define DBR_GP_VERIFY_MBNUM			1007									//手机号码是否存在
#define DBO_GP_VERIFY_MBNUM			2007									//手机号码是否存在

struct DBR_GP_MBVerifCode
{
	DWORD dwUserId;
	TCHAR szMobilePhone[LEN_MOBILE_PHONE];	//手机号
	TCHAR szVerificationCode[LEN_VERIFICATION_CODE];	//验证码
};

struct DBO_GP_MBVerifCode
{
	bool bIsMBNumExist;
	TCHAR szMobilePhone[LEN_MOBILE_PHONE];	//手机号
	TCHAR szVerificationCode[LEN_VERIFICATION_CODE];	//验证码
};


// 数据库处理手机注册处理结果, 失败的结果交给正常的流程
#define DBR_GP_MB_REGITER			1008
#define DBO_GP_MB_REGITER_SUCCESS	2008


struct DBO_GP_MBRegisterRes
{
	TCHAR szMobilePhone[LEN_MOBILE_PHONE];	//手机号
};

#define DBR_GP_QUERY_NEW_GUID 1009
#define DBO_GP_QUERY_NEW_GUID 2009

struct DBR_GP_QueryNewGuid
{
	DWORD dwUserID;
};

struct DBO_GP_QueryNewGuid
{
	bool bCan;
};

// 数据库处理手机绑定的结果
#define DBR_GP_MB_BIND_NUM			1010
#define DBO_GP_MB_BIND_NUM_RES		2010
struct DBR_GP_MBBindNum
{
	DWORD							dwUserID;
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//手机号
};

struct DBO_GP_MB_Check_Can_Bind
{
	BYTE byResult;							//查询数据库结果
	TCHAR szMobilePhone[LEN_MOBILE_PHONE];	//手机号
	TCHAR szVerificationCode[LEN_VERIFICATION_CODE];	//验证码
};

//绑定手机号处理结果
struct DBO_GP_MBBindRes
{
	BYTE							byResult;							//绑定结果 0 失败 1成功
	int								nCurrentGold;						//奖励之后的金币值
	TCHAR							szDescribeString[128];				//描述消息
};

#define DBR_GP_MB_CHECK_NUM_EXIT	1011
#define DBO_GP_MB_CHECK_NUM_EXIT	2011
//手机号是否绑定
struct DBR_GP_CHECK_BINDNUM_EXIT
{
	DWORD dwUserID;
};

struct DBO_GP_CHECK_BINDNUM_EXIT_RES
{
	BYTE byIsExit;
};

#define DBR_GP_EXCHANGE_LOTTERY 1012
#define DBO_GP_EXCHANGE_LOTTERY 2012

struct DBR_GP_ExchangeLottery
{
	DWORD dwUserID;
	int nDragonType;//0初级珍珠，1中级珍珠，2高级珍珠，3至尊珍珠
};

struct DBO_GP_ExchangeLottery
{
	int nLotteryCount;//当前奖券数量（兑换后）
	int nChuJiCount;
	int nZhongJiCount;
	int nGaoJiCount;
	int nZhiZunCount;
	int nResultCode;//处理结果
	TCHAR szDescribeString[128];
};

#define DBR_GP_BROADCAST_SPEED		1013
#define DBO_GP_BROADCAST_SPEED		2013
struct DBO_GP_BROADCAST_SPEED_RES
{
	float fBroadCastSpeed;
};

#define DBR_GP_COMPRANK_INFO		1014
#define DBO_GP_COMPRANK_INFO		2014
struct DBR_GP_REQUEST_COMPRANK_INFO
{
	int nType;
};

struct DB0_GP_COMPRANK_INFO_RES
{
	int nRank;
	int nUserId;
	int nCount;
};

#define DBR_GP_BROADLABA		1015
#define DBO_GP_BROADLABA		2015
struct DBR_GP_REQUEST_BROADLABA
{
	DWORD dwUserId;
	WORD wSize;
	char szNickName[64];
	char szContent[1024];
};

struct DB0_GP_BROADLABA_RES
{
	WORD lResult;
	SCORE score;
	WORD wSize;
	char szNickName[64];
	TCHAR szErrorDes[128];
	TCHAR szContent[1024];
};

#define DBR_GP_TYPE_ITEM_COUNT		1016
#define DBO_GP_TYPE_ITEM_COUNT		2016
struct DBR_GP_REQUEST_TYPE_ITEM_COUNT
{
	DWORD dwUserId;
};

struct DB0_GP_TYPE_ITEM_COUNT_RES
{
	int nCount;
};

#define DBR_GP_CHANGE_PHONE_FARE		1017
#define DBO_GP_CHANGE_PHONE_FARE		2017
struct DBR_GP_REQUESTCHANGE_PHONE_FARE
{
	DWORD dwUserID;
	int nMobileCharge;
};

struct DB0_GP_CHANGE_PHONE_FARE_RES
{
	bool bSuccess;
	int nCurrentMobileCharge;
	TCHAR szNickName[32];
	int nChangeNum;
};

#define DBR_GP_SUPERRANK_INFO		1018
#define DBO_GP_SUPERRANK_INFO		2018
struct DBR_GP_REQUEST_SUPERRANK_INFO
{
	int nType;
};

struct DB0_GP_SUPERRANK_INFO_RES
{
	int nRank;
	TCHAR szNickName[32];
	LONGLONG lCount;
};

#define DBR_GP_GETBINDREWARDNEWGUID	1019
#define DBO_GP_GETBINDREWARDNEWGUID	2019
//手机号是否绑定
struct DBR_GP_GET_BINDREWARD
{
	DWORD dwUserID;
};

struct DBO_GP_GET_BINDREWARD_RES
{
	int nBindReward;
	int nShareReward;
	BYTE byNewGuid;
};

#define DBR_GP_SETSUPERNEWGUID	1020
#define DBO_GP_SETSUPERNEWGUID	2020


struct DBR_GP_Set_SuperNewGuid
{
	DWORD dwUserID;
};

#define DBR_GP_SUPER_GOLD 1021
#define DBO_GP_SUPER_GOLD 2021

struct DBR_GP_SuperGold
{
	DWORD dwUserID;
};

struct DBO_GP_SuperGold
{
	BYTE bCan;
	int nSuperGold;
};

#define DBR_GP_GET_NEWMAIL_REMIND 1022
#define DBO_GP_GET_NEWMAIL_REMIND 2022


struct DBO_GP_NewMail_Remind_Res
{
	DWORD dwUserID;
	DWORD dwMailID;
};

#define DBR_GP_QUERY_FREE_LABA_COUNT 1023
#define DBO_GP_QUERY_FREE_LABA_COUNT 2023

struct DBR_GP_Free_LaBa_Count
{
	DWORD dwUserID;
};

struct DBO_GP_Free_LaBa_Count_Res
{
	int nCount;
};

#define  DBR_GR_MATCH_GET_PRIZE		1024
#define  DBO_GR_MATCH_GET_PRIZE		2024
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
};

#define DBR_GP_BUY_SKILL 1025
#define DBO_GP_BUY_SKILL 2025

struct DBR_GP_Buy_Skill
{
	DWORD dwUserID;
	int nSkillID;
	int nCount;
};

struct DBO_GP_Buy_Skill_Res
{
	bool bSuccess;
	BYTE cbSkillID;
	int nCount;
	SCORE llScore;
	TCHAR szDescribeString[128];
};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif