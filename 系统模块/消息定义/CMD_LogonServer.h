#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//登录命令

#define MDM_GP_LOGON				1									//广场登录

//登录模式
#define SUB_GP_LOGON_GAMEID			1									//I D 登录
#define SUB_GP_LOGON_ACCOUNTS		2									//帐号登录
#define SUB_GP_REGISTER_ACCOUNTS	3									//注册帐号
#define SUB_GP_GUEST_REG			4									//游客注册
#define SUB_GPR_MBREIGSTER 	    	5									//手机号注册
#define SUB_GP_MOBILE_VERIFICAT		6									//手机验证码验证
#define SUB_GPR_CANCEL_LOGINING		7									//取消登录

//登录结果
#define SUB_GP_LOGON_SUCCESS		100									//登录成功
#define SUB_GP_LOGON_FAILURE		101									//登录失败
#define SUB_GP_LOGON_FINISH			102									//登录完成
#define SUB_GP_VALIDATE_MBCARD      103                                 //登录失败
#define SUB_GP_VALIDATE_PASSPOSTID  104                                 //登录失败
#define SUB_GP_GUEST_REG_SUCCESS	105									//游客注册成功

//升级提示
#define SUB_GP_UPDATE_NOTIFY		200									//升级提示

#define SUB_GP_LOGINING_INFO		201									//登录排队信息

//////////////////////////////////////////////////////////////////////////////////
//

#define MB_VALIDATE_FLAGS           0x01                                //效验密保
#define LOW_VER_VALIDATE_FLAGS      0x02                                //效验低版本

//I D 登录
struct CMD_GP_LogonGameID
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//登录信息
	DWORD							dwGameID;							//游戏 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	BYTE							cbValidateFlags;			        //校验标识
};

//帐号登录
struct CMD_GP_LogonAccounts
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	BYTE							cbValidateFlags;			        //校验标识
};

//////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_RegisterAccounts

#define DTP_GP_REG_AGENTID			1									//推荐人ID

//推荐人信息
struct DTP_GP_AgentInfo
{
	DWORD							dwAgentID;							//推荐人ID
};

//注册帐号
struct CMD_GP_RegisterAccounts
{
	//系统信息
	DWORD							dwPlazaVersion;						//广场版本
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szSpreader[LEN_ACCOUNTS];			//推荐帐号
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
	TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//安全手机
	BYTE							cbValidateFlags;			        //校验标识
};

//游客注册
struct CMD_GP_Guest_Reg
{
    //系统信息
    DWORD							dwPlazaVersion;						//广场版本
    TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
    
    //密码变量
    TCHAR							szLogonPass[LEN_MD5];				//登录密码
    TCHAR							szInsurePass[LEN_MD5];				//银行密码
    
    //注册信息
    WORD							wFaceID;							//头像标识
    BYTE							cbGender;							//用户性别
    TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
    TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
    TCHAR							szSpreader[LEN_ACCOUNTS];			//推荐帐号
    TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//证件号码
    TCHAR							szCompellation[LEN_COMPELLATION];	//真实名字
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//安全手机
    BYTE							cbValidateFlags;			        //校验标识
};

//手机注册帐号
struct CMD_GP_MBNumRegister
{
	TCHAR							szVerificationCode[LEN_MOBILE_PHONE];//验证码
	CMD_GP_RegisterAccounts			RegisterAccounts;
};

//手机号绑定验证码验证结构体
struct CMD_GP_MobileCaptchaVerific
{
	DWORD							dwUserId;
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	 //绑定手机;
	TCHAR							szVerificationCode[LEN_MOBILE_PHONE];//验证码
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
};

//CMD_GP_LogonAccounts、CMD_GP_RegisterAccounts、CMD_GP_Guest_Reg共同使用
#define DTP_GP_USERDATA1 10
#define DTP_GP_USERDATA2 11
#define DTP_GP_USERDATA3 12
#define DTP_GP_USERDATA4 13
#define DTP_GP_USERDATA5 14
#define DTP_GP_USERDATA6 15
#define DTP_GP_USERDATA7 16
#define DTP_GP_USERDATA8 17

#define DTP_GP_PASSWORD 100//玩家密码
#define DTP_GP_LOCALIP 101//内网IP
#define DTP_GP_CLIENTVERSION 102//客户端版本号
//#define DTP_GP_POSITION 23
//szMachineID里保存的是手机的机器码
//szPassPortID里保存的是渠道字符串

#define DTP_GP_CLIENTIDFA 103

struct DTP_GP_Password
{
	WORD wCount;
	char cbData[LEN_PASSWORD];
};

struct DTP_GP_Position
{
	float longitude;//精度
	float latitude;//纬度
};

//游客注册成功
struct CMD_GP_GuestRegSuccess
{
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwGroupID;							//社团标识
	DWORD							dwCustomID;							//自定标识
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力

	//用户成绩
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//用户银行

	//用户信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMoorMachine;						//锁定机器
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字

	//配置信息
	BYTE                            cbShowServerStatus;                 //显示服务器状态
};

//登录成功
struct CMD_GP_LogonSuccess
{
	//属性资料
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwGroupID;							//社团标识
	DWORD							dwCustomID;							//自定标识
	DWORD							dwUserMedal;						//用户奖牌
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力

	//用户成绩
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//用户银行

	//用户信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMoorMachine;						//锁定机器
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_ACCOUNTS];			//用户昵称
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字

	//配置信息
	BYTE                            cbShowServerStatus;                 //显示服务器状态
};

//登录失败
struct CMD_GP_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//登陆完成
struct CMD_GP_LogonFinish
{
	WORD							wIntermitTime;						//中断时间
	WORD							wOnLineCountTime;					//更新时间
};

//登录失败
struct CMD_GP_ValidateMBCard
{
	UINT								uMBCardID;						//机器序列
};

//升级提示
struct CMD_GP_UpdateNotify
{
	BYTE							cbMustUpdate;						//强行升级
	BYTE							cbAdviceUpdate;						//建议升级
	DWORD							dwCurrentVersion;					//当前版本
};

//登录排队信息
struct CMD_GP_LoginingInfo
{
	WORD							wLoginingNum;						//正在登录的人数
};

//////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_LogonSuccess

#define DTP_GP_GROUP_INFO			1									//社团信息
#define DTP_GP_MEMBER_INFO			2									//会员信息
#define	DTP_GP_UNDER_WRITE			3									//个性签名
#define DTP_GP_STATION_URL			4									//主页信息

#define DTP_GP_SPECIALNODEIP		20//特殊节点，类型为字符串
#define DTP_GP_SPECIALNODEURL		21//特殊节点，类型为字符串
#define DTP_GP_IOSCHARGEHIDE		22//苹果版，上苹果商店，隐藏充值
#define DTP_GP_IOSPASSED			23
#define DTP_GP_MEMBER_ORDER			24
#define DTP_GP_GIFT_TICKET			25//奖券
#define DTP_GP_SHOWFAKETABLE        26
#define DTP_GP_VIPINFO				27//玩家的VIP信息
#define DTP_GP_CUSTOMERSERVICEQQ	28//客服QQ
#define DTP_GP_LOGONGIFT			29//登陆奖励，每天可以领取一次

#define DTP_GP_SERVER_INFO			30//游戏房间信息
#define DTP_GP_CHARGE1ST_INFO		31

#define DTP_GP_PASS_PORT_ID			32

struct DTP_GP_Charge1stInfo
{
	BYTE cbCount;
	BYTE cbCanGet;
};

struct DTP_GP_LogonGift
{
	BYTE cbCanGetBankruptcy;
	int nBankruptcyCount;
	int nBankruptcyCountMax;
	int nBankruptcyCondition;
	DWORD dwGiftScore;
	int nCanShare;
};

struct DTP_GP_VipInfo
{
	BYTE cbVIPLevel;//VIP等级
	DWORD dwVIPExp;//VIP经验
	DWORD dwExpPerCent;//一分钱对应的经验值
};

struct DTP_GP_ShowFakeTable
{
	BYTE cbShowFakeTable;
};

struct DTP_GP_ServerInfo
{
	WORD							wServerID;
	int								nCellScore;
};

//社团信息
struct DTP_GP_GroupInfo
{
	DWORD							dwGroupID;							//社团索引
	TCHAR							szGroupName[LEN_GROUP_NAME];		//社团名字
};

//会员信息
struct DTP_GP_MemberInfo
{
	BYTE							cbMemberOrder;						//会员等级
	SYSTEMTIME						MemberOverDate;						//到期时间
};

//充值网址
struct DTP_GP_iOSChargeHide
{
	int id;
	char szURL[64];
};

struct DTP_GP_iOSPassed
{
	BYTE cbPassed;
};

struct DTP_GP_MemberOrder
{
	BYTE cbMemberOrder;
};

struct DTP_GP_GiftTicket
{
	DWORD dwCount;
};

//////////////////////////////////////////////////////////////////////////
//签到信息查询
struct CMD_GP_QuerySigninInfo
{
	DWORD dwUserID;
};

//签到查询每天信息
struct tagSignInDayInfo
{
	BYTE cbType;//1金币、2奖券、3话费、4初级珍珠、5中级珍珠、6高级珍珠、7加速、8散射、9暴击
	BYTE cbAttrib;//0bit底图两种、1bit标签两种, 2bit 今天是否签到
	int nCount;
};

//签到查询结果
struct CMD_GP_UserSigninInfo
{
	INT nNum1;
	INT nNum2;
	tagSignInDayInfo normal[7];
	tagSignInDayInfo vip[7];
};

//点击签到操作
struct CMD_GP_SignIn
{
	DWORD							dwUserID;							//用户ID
	BYTE							cbVip;								//是否是Vip用户, 0 是普通用户签到, 1 是Vip签到
};

//点击签到结果
struct CMD_GP_SignIn_Result
{
	bool bSuccess;
	BYTE cbUserStatus;
	SCORE llScoreCount;
	SCORE llLotteryCount;
	TCHAR szDescription[128];
};

//抽奖查询
struct CMD_GP_QryLtryInfo
{
	DWORD							dwUserID;							//用户ID
};

//查询结果
struct CMD_GP_QryLtryInfo_Res
{
	WORD							wFreeCount;							//免费次数
	TCHAR							szErrorDescribe[128];				//结果描述
};

//抽奖操作
struct CMD_GP_UserLtry
{
	DWORD							dwUserID;							//用户ID
};

//抽奖结果
struct CMD_GP_UserLtry_Res
{
	WORD							wIsLtrySuc;							//抽奖成功
	WORD							wPrizeID;							//中奖等级
	WORD							wFreeCount;							//免费次数
	SCORE                           llUserScore;                        //玩家当前剩余金币
	TCHAR							szErrorDescribe[128];				//结果描述
};

//抽奖消息
struct CMD_GP_LTRY_MSG
{
	TCHAR							szNickName[128];					//用户名称
	WORD							wPrizeID;							//中奖ID
};

//充值查询
struct CMD_GP_RechargeInfo
{
	DWORD							dwUserID;							//用户ID
};

//充值信息结果
struct CMD_GP_RechargeInfo_Res
{
	BYTE							cbIsRecharge;
	TCHAR							szDescribeString[128];				//描述消息
};


//////////////////////////////////////////////////////////////////////////////////
//列表命令

#define MDM_GP_SERVER_LIST			2									//列表信息

//获取命令
#define SUB_GP_GET_LIST				1									//获取列表
#define SUB_GP_GET_SERVER			2									//获取房间
#define SUB_GP_GET_ONLINE			3									//获取在线
#define SUB_GP_GET_COLLECTION		4									//获取收藏

//列表信息
#define SUB_GP_LIST_TYPE			100									//类型列表
#define SUB_GP_LIST_KIND			101									//种类列表
#define SUB_GP_LIST_NODE			102									//节点列表
#define SUB_GP_LIST_PAGE			103									//定制列表
#define SUB_GP_LIST_SERVER			104									//房间列表
#define SUB_GP_VIDEO_OPTION			105									//视频配置
#define SUB_GP_LIST_SMALL_SERVER	106									//房间列表改进

//完成信息
#define SUB_GP_LIST_FINISH			200									//发送完成
#define SUB_GP_SERVER_FINISH		201									//房间完成

//在线信息
#define SUB_GR_KINE_ONLINE			300									//类型在线
#define SUB_GR_SERVER_ONLINE		301									//房间在线

//////////////////////////////////////////////////////////////////////////////////

//获取在线
struct CMD_GP_GetOnline
{
	WORD							wServerCount;						//房间数目
	WORD							wOnLineServerID[MAX_SERVER];		//房间标识
};

//类型在线
struct CMD_GP_KindOnline
{
	WORD							wKindCount;							//类型数目
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//类型在线
};

//房间在线
struct CMD_GP_ServerOnline
{
	WORD							wServerCount;						//房间数目
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//房间在线
};

//////////////////////////////////////////////////////////////////////////////////
//服务命令

#define MDM_GP_USER_SERVICE			3									//用户服务

//账号服务
#define SUB_GP_MODIFY_MACHINE		100									//修改机器
#define SUB_GP_MODIFY_LOGON_PASS	101									//修改密码
#define SUB_GP_MODIFY_INSURE_PASS	102									//修改密码
#define SUB_GP_MODIFY_UNDER_WRITE	103									//修改签名

//修改头像
#define SUB_GP_USER_FACE_INFO		200									//头像信息
#define SUB_GP_SYSTEM_FACE_INFO		201									//系统头像
#define SUB_GP_CUSTOM_FACE_INFO		202									//自定头像

//个人资料
#define SUB_GP_USER_INDIVIDUAL		301									//个人资料
#define	SUB_GP_QUERY_INDIVIDUAL		302									//查询信息
#define SUB_GP_MODIFY_INDIVIDUAL	303									//修改资料

//银行服务
#define SUB_GP_USER_SAVE_SCORE		400									//存款操作
#define SUB_GP_USER_TAKE_SCORE		401									//取款操作
#define SUB_GP_USER_TRANSFER_SCORE	402									//转账操作
#define SUB_GP_USER_INSURE_INFO		403									//银行资料
#define SUB_GP_QUERY_INSURE_INFO	404									//查询银行
#define SUB_GP_USER_INSURE_SUCCESS	405									//银行成功
#define SUB_GP_USER_INSURE_FAILURE	406									//银行失败
#define SUB_GP_QUERY_USER_INFO_REQUEST		407							//查询用户
#define SUB_GP_QUERY_USER_INFO_RESULT		408							//用户信息
#define SUB_GP_USER_TRANSFER_INFO			409
#define SUB_GP_USER_CHECK_PASSWORD			410
#define SUB_GP_USER_CHECK_PASSWORD_RETURN	411


//用户抽奖
#define SUB_GP_QUERY_LOTTERY_INFO			416							//查询抽奖
#define SUB_GP_QUERY_LOTTERY_INFO_RESULT	417							//查询结果
#define SUB_GP_USER_LOTTERY					418							//用户抽奖
#define SUB_GP_USER_LOTTERY_RESULT			419							//用户抽奖
#define SUB_GP_LOTTERY_FINISH				420							//抽奖结束
#define SUB_GP_LOTTERY_MSG					421							//抽奖消息
#define SUB_GP_QUERY_RECHARGE_INFO			422							//充值信息
#define SUB_GP_QUERY_RECHARGE_INFO_RES		423							//充值信息

#define SUB_GPR_USER_TRANSER_DRAGON			425
#define SUB_GPO_USER_TRANSER_DRAGON			426

//验证码请求
#define SUB_GPR_MBCODE_VERIFY				427							//手机号验证
#define SUB_GPO_MBCODE_VERIFY				428							//手机号验证

//手机号绑定验证码请求
#define SUB_GPR_MOBILE_BIND					429							//手机号绑定验证码请求
#define SUB_GPR_MOBILE_BIND_VERIFY			430							//手机号绑定验证码验证
#define SUB_GPO_MOBILE_VERIFY_RES			431							//手机号绑定验证码验证结果

//查询是否绑定过手机号
#define SUB_GPR_CHECK_MOBILE_EXIT			432							//查询是否绑定过手机号
#define SUB_GPR_CHECK_MOBILE_EXIT_RES		433							//查询是否绑定过手机号结果
//赠送龙珠
struct CMD_GP_UserTransferDragon
{
	DWORD dwUserID;
	BYTE cbByNickName;
	SCORE lTransferDragon;
	TCHAR szPassword[LEN_MD5];
	TCHAR szNickName[LEN_NICKNAME];
	TCHAR szMachineID[LEN_MACHINE_ID];
};

#define SUB_GPR_USER_USE_DRAGON				434

//使用龙珠
struct CMD_GP_UserUseDragon
{
	DWORD dwUserID;
	SCORE lUseDragon;
	TCHAR szPassword[LEN_MD5];
	TCHAR szMachineID[LEN_MACHINE_ID];
};

#define SUB_GPR_BROAD_LABA					435
#define SUB_GPO_BROAD_LABA					436

struct CMD_GPR_BroadLaba
{
	DWORD dwUserID;
	WORD wSize;
	char szNickName[64];
	char szContent[1024];
};

struct CMD_GPO_BroadLaba
{
	BYTE cbSuccess;
	SCORE score;
	char szContent[128];
};

#define SUB_GPR_QUERY_MAIL_ID_LIST			500
#define SUB_GPO_QUERY_MAIL_ID_LIST			501
#define SUB_GPR_QUERY_MAIL_INFO				502							//获取邮件信息
#define SUB_GPO_QUERY_MAIL_INFO				503
#define SUB_GPR_QUERY_MAIL_CONTENT			504
#define SUB_GPO_QUERY_MAIL_CONTENT			505
#define SUB_GPR_QUERY_MAIL_ITEM				506							//请求领取物品
#define SUB_GPO_QUERY_MAIL_ITEM				507							//反馈字符串

#define SUB_GPR_IAP_PRODUCT_LIST			600							//in app purchase
#define SUB_GPO_IAP_PRODUCT_LIST			601
#define SUB_GPR_IAP_TRANSACTIONRECEIPT		602

#define SUB_GPR_CHARGE_VALUE				603							//请求是否有充值
#define SUB_GPO_CHARGE_VALUE				604							//返回是否有充值
#define SUB_GPR_CHARGE_CANCEL				605							//取消取出金币的提示

#define SUB_GPR_VIP_INFO					610
#define SUB_GPR_VIP_EVERYDAY_GIFT			612
#define SUB_GPR_VIP_CHARGE_BACK				614

#define SUB_GPO_VIP_INFO					611
#define SUB_GPO_VIP_RESULT					613

#define SUB_GPR_CURRENT_SCORE				616
#define SUB_GPO_CURRENT_SCORE				617

#define SUB_GPR_LOGON_GIFT					618
#define SUB_GPO_LOGON_GIFT					619

#define SUB_GPR_CHARGE1ST_REQUEST			620//首充成功
#define SUB_GPO_CHARGE1ST_REQUEST			621

#define SUB_GPR_CHARGE1ST_GET				622//领取首充礼包的奖励
#define SUB_GPO_CHARGE1ST_GET				623

#define SUB_GP_WEALTH_BAG_TIME				625//福袋时间
#define SUB_GPR_WEALTH_BAG_ENROLL			626//报名
#define SUB_GPO_WEALTH_BAG_ENROLL			627//报名结果
#define SUB_GP_WEALTH_BAG_BEGIN				628//游戏开始
#define SUB_GP_WEALTH_BAG_END				629//游戏结束
#define SUB_GP_WEALTH_BAG_SCORE				630//客户端返回结果
#define SUB_GP_WEALTH_BAG_RESULT			631//服务端返回结果
#define SUB_GP_WEALTH_BAG_BEFORE			632//还有多长时间才开始

#define SUB_GPR_SHARE_INFO					633
#define SUB_GPO_SHARE_INFO					634

//广播速度
#define SUB_GPR_BROADCAST_SPEED				635							//请求广播速度
#define SUB_GPO_BROADCAST_SPEED_RES			636							//请求广播速度结果

//请求type = 113的物品数量
#define SUB_GPR_TYPE_ITEM_COUNT				637							//请求type类型的物品数量
#define SUB_GPO_TYPE_ITEM_COUNT_RES			638							//请求type类型的物品数量结果

#define SUB_GPR_MOBILE_CHARGE               639                         //兑换话费
#define SUB_GPO_MOBILE_CHARGE               640

#define SUB_GPR_GET_BINDREWARD_NEWGUID      643                         //获得绑定手机奖励和新手引导信息
#define SUB_GPO_GET_BINDREWARD_NEWGUID      644

#define SUB_GPR_SET_SUPER_NEWGUID		    645							//设置超级海盗新手引导

//用户签到
#define SUB_GP_QUERY_SIGNIN_INFO			646							//查询签到
#define SUB_GP_QUERY_SIGNIN_INFO_RESULT		647							//签到信息
#define SUB_GP_USER_SIGNIN_REQ				648							//点击签到
#define SUB_GP_USER_SIGNIN_RESULT			649							//签到结果

#define SUB_GP_QUERY_FREE_LABA_COUNT		650							//查询免费喇叭次数
#define SUB_GP_QUERY_FREE_LABA_RESULT		651							//查询免费喇叭次数结果

//#define SUB_GR_MATCH_GET_MATCH_PRIZE		652							//用户领取奖励
//#define SUB_GR_MATCH_GET_MATCH_PRIZE_RES	653							//用户领取奖励结果

#define SUB_GP_BUY_SKILL		652										//金币兑换技能
#define SUB_GP_BUY_SKILL_RESULT		653									//金币兑换技能结果

struct CMD_GPO_ShareInfo
{
	char szURL[64];
	char szContent[256];
};

struct CMD_GPO_WealthBagEnroll
{
	bool bCan;
};

struct CMD_GPR_WealthBagEnroll
{
	DWORD dwUserID;
	TCHAR szNickName[LEN_NICKNAME];
};

struct CMD_GP_WealthBagEnd
{
	int nScore;
};

struct CMD_GPR_WealthBagScore
{
	DWORD dwUserID;
	int nScore;
};

struct CMD_GPO_WealthBagResult
{
	TCHAR szNickName[10][LEN_NICKNAME];
	int nScore[10];
	int nMyScore;
	int nMyRank;
	unsigned int nPlayerCount;
	SCORE lCurrentScore;
};

struct CMD_GP_WealthBagBefore
{
	DWORD dwTime;
};

struct CMD_GPR_Charge1stGet
{
	DWORD dwUserID;
};

struct CMD_GPR_Charge1stRequest
{
	DWORD dwUserID;
};

struct CMD_GPR_LogonGift
{
	DWORD dwUserID;
};

struct CMD_GPR_CurrentScore
{
	DWORD dwUserID;
};

struct CMD_GPR_VIPUser//610、612、614共用
{
	DWORD dwUserID;
};

struct CMD_GPO_VIPInfo
{
	BYTE cbVIPLevel;//VIP等级
	BYTE cbReserved[3];//保留
	DWORD dwVIPExp;//VIP经验
	DWORD dwVIPNextExp;//升级经验
	DWORD dwEverydayGift;//每日领取金币，如果为0，说明不能领取，或者已经领取
	DWORD dwChargeBack;//充值返现金币，如果为0，说明当前没有返现
};

struct CMD_GPO_VIPResult
{
	WORD wSize;
	char szDescription[1024];
};

struct CMD_GPR_IAPProduct
{
	WORD wRMB;//价格
	WORD wType;//0金币，1炮台
	char szProductID[25];//商品ID
};

struct CMD_GPR_IAPProductList
{
	CMD_GPR_IAPProduct product[12];
};

struct CMD_GPR_MailIDList
{
	DWORD dwUserID;
};

struct CMD_GPO_MailIDList
{
	BYTE cbCount;
	DWORD dwMailID[30];
};

struct CMD_GPR_MailInfo
{
	BYTE cbCount;
	DWORD dwMailID[30];
};

struct CMD_GPR_MailContent
{
	unsigned int dwMailID;
};

struct CMD_GPR_MailItem
{
	unsigned int dwMailID;
};

struct CMD_GPR_ChargeValue
{
	DWORD dwUserID;
};

struct CMD_GPO_ChargeValue
{
	SCORE lChargeValue;
};

struct CMD_GPR_ChargeCancel
{
	DWORD dwUserID;
};

struct CMD_GP0_ChangePhoneFare
{
	bool bSuccess;
	int nCurrentMobileCharge;
};

struct CMD_GPR_FREE_LABA_QUERY
{
	DWORD dwUserID;
};

#define DTP_GP_MAIL_CHUJI_DRAGON			1
#define DTP_GP_MAIL_ZHONGJI_DRAGON			2
#define DTP_GP_MAIL_GAOJI_DRAGON			3
#define DTP_GP_MAIL_ZHIZUN_DRAGON			4
#define DTP_GP_MAIL_CANNON					5
#define DTP_GP_MAIL_SKILL_JS				6
#define DTP_GP_MAIL_SKILL_SS				7
#define DTP_GP_MAIL_SKILL_BJ				8
#define DTP_GP_MAIL_LOTTERY					9
#define DTP_GP_MAIL_GIFTGOLD				10
#define DTP_GP_MAIL_CONTENT					100

struct MAIL_ITEM_INFO
{
	int nItemCount;							//道具数量
};

struct MAIL_CONTENT_STRING
{
	BYTE bySize;							//内容大小
};

//操作结果
#define SUB_GP_OPERATE_SUCCESS		900									//操作成功
#define SUB_GP_OPERATE_FAILURE		901									//操作失败

//////////////////////////////////////////////////////////////////////////////////

//修改密码
struct CMD_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改密码
struct CMD_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改签名
struct CMD_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
};

//////////////////////////////////////////////////////////////////////////////////

//用户头像
struct CMD_GP_UserFaceInfo
{
	WORD							wFaceID;							//头像标识
	DWORD							dwCustomID;							//自定标识
};

//修改头像
struct CMD_GP_SystemFaceInfo
{
	WORD							wFaceID;							//头像标识
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//修改头像
struct CMD_GP_CustomFaceInfo
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//图片信息
};

//////////////////////////////////////////////////////////////////////////////////

//绑定机器
struct CMD_GP_ModifyMachine
{
	BYTE							cbBind;								//绑定标志
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//////////////////////////////////////////////////////////////////////////////////

//个人资料
struct CMD_GP_UserIndividual
{
	DWORD							dwUserID;							//用户 I D
};

//查询信息
struct CMD_GP_QueryIndividual
{
	DWORD							dwUserID;							//用户 I D
};

//修改资料
struct CMD_GP_ModifyIndividual
{
	BYTE							cbGender;							//用户性别
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
};

//////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_UserIndividual

#define DTP_GP_UI_NICKNAME			1									//用户昵称
#define DTP_GP_UI_USER_NOTE			2									//用户说明
#define DTP_GP_UI_UNDER_WRITE		3									//个性签名
#define DTP_GP_UI_QQ				4									//Q Q 号码
#define DTP_GP_UI_EMAIL				5									//电子邮件
#define DTP_GP_UI_SEAT_PHONE		6									//固定电话
#define DTP_GP_UI_MOBILE_PHONE		7									//移动电话
#define DTP_GP_UI_COMPELLATION		8									//真实名字
#define DTP_GP_UI_DWELLING_PLACE	9									//联系地址

//////////////////////////////////////////////////////////////////////////////////

//银行资料
struct CMD_GP_UserInsureInfo
{
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	SCORE							lTransferPrerequisite;				//转账条件
};

//存入金币
struct CMD_GP_UserSaveScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lSaveScore;							//存入金币
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//提取金币
struct CMD_GP_UserTakeScore
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lTakeScore;							//提取金币
	TCHAR							szPassword[LEN_MD5];				//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//转账金币
struct CMD_GP_UserTransferScore
{
	DWORD							dwUserID;							//用户 I D
	BYTE                            cbByNickName;                       //昵称赠送
	SCORE							lTransferScore;						//转账金币
	TCHAR							szPassword[LEN_MD5];				//银行密码
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//银行成功
struct CMD_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	TCHAR							szDescribeString[128];				//描述消息
};

//赠送信息
struct DBO_GP_UserInsureTransferInfo
{
	DWORD							dwRecordID;
	DWORD							dwSourceID;
	DWORD							dwTargetID;
	SCORE							lSwapScore;
	SYSTEMTIME						timeTransfer;
	TCHAR							szSourceName[LEN_NICKNAME];
	TCHAR							szTargetName[LEN_NICKNAME];
};

//银行失败
struct CMD_GP_UserInsureFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//提取结果
struct CMD_GP_UserTakeResult
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
};

//查询银行
struct CMD_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//用户 I D
};

//查询用户
struct CMD_GP_QueryUserInfoRequest
{
	BYTE                            cbByNickName;                       //昵称赠送
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
};

//用户信息
struct CMD_GP_UserTransferUserInfo
{
	DWORD							dwTargetGameID;						//目标用户
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
};

//密码验证
struct CMD_GP_CheckPassword
{
	DWORD							dwUserID;
	TCHAR							szPassword[LEN_PASSWORD];
};

//密码验证结果
struct CMD_GP_CheckPasswordReturn
{
	bool							bPasswordRight;
};
//////////////////////////////////////////////////////////////////////////////////

//操作失败
struct CMD_GP_OperateFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//操作成功
struct CMD_GP_OperateSuccess
{
	LONG							lResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//成功消息
};

struct CMD_GPR_BuySkill
{
	DWORD dwUserID;
	BYTE cbSkillID;
	int nCount;
};

//struct CMD_GPO_BuySkill_Result
//{
//	bool bSuccess;
//	BYTE cbSkillID;
//	int nCount;
//	SCORE llScore;
//	TCHAR szDescribeString[128];				
//};

//////////////////////////////////////////////////////////////////////////////////
//远程服务

#define MDM_GP_REMOTE_SERVICE		4									//远程服务

//查找服务
#define SUB_GP_C_SEARCH_DATABASE	100									//数据查找
#define SUB_GP_C_SEARCH_CORRESPOND	101									//协调查找

//查找服务
#define SUB_GP_S_SEARCH_DATABASE	200									//数据查找
#define SUB_GP_S_SEARCH_CORRESPOND	201									//协调查找

//////////////////////////////////////////////////////////////////////////////////

//协调查找
struct CMD_GP_C_SearchCorrespond
{
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//协调查找
struct CMD_GP_S_SearchCorrespond
{
	WORD							wUserCount;							//用户数目
	tagUserRemoteInfo				UserRemoteInfo[16];					//用户信息
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//登录命令
#define MDM_MB_LOGON				100									//广场登录

//登录模式
#define SUB_MB_LOGON_GAMEID			1									//I D 登录
#define SUB_MB_LOGON_ACCOUNTS		2									//帐号登录
#define SUB_MB_REGISTER_ACCOUNTS	3									//注册帐号

//登录结果
#define SUB_MB_LOGON_SUCCESS		100									//登录成功
#define SUB_MB_LOGON_FAILURE		101									//登录失败

//升级提示
#define SUB_MB_UPDATE_NOTIFY		200									//升级提示

//////////////////////////////////////////////////////////////////////////////////

//I D 登录
struct CMD_MB_LogonGameID
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//登录信息
	DWORD							dwGameID;							//游戏 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//帐号登录
struct CMD_MB_LogonAccounts
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//登录信息
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//注册帐号
struct CMD_MB_RegisterAccounts
{
	//系统信息
	WORD							wModuleID;							//模块标识
	DWORD							dwPlazaVersion;						//广场版本
	BYTE                            cbDeviceType;                       //设备类型

	//密码变量
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szInsurePass[LEN_MD5];				//银行密码

	//注册信息
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	TCHAR							szAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//连接信息
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器标识
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//电话号码
};

//登录成功
struct CMD_MB_LogonSuccess
{
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwExperience;						//经验数值
	DWORD							dwLoveLiness;						//用户魅力
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//登录失败
struct CMD_MB_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//升级提示
struct CMD_MB_UpdateNotify
{
	BYTE							cbMustUpdate;						//强行升级
	BYTE							cbAdviceUpdate;						//建议升级
	DWORD							dwCurrentVersion;					//当前版本
};

//////////////////////////////////////////////////////////////////////////////////
//列表命令

#define MDM_MB_SERVER_LIST			101									//列表信息

//列表信息
#define SUB_MB_LIST_KIND			100									//种类列表
#define SUB_MB_LIST_SERVER			101									//房间列表
#define SUB_MB_LIST_FINISH			200									//列表完成

//////////////////////////////////////////////////////////////////////////////////

#define MDM_GP_WEB 5
#define SUB_GP_WEB_CHARGE_VALUE 1

struct CMD_GP_WebChargeValue
{
	DWORD dwUserID;
	SCORE lChargeValue;
};

#define MDM_GP_CORRESPOND 6

#define SUB_GPR_VIPUpgrade 700
#define SUB_GPO_VIPUpgrade 701

//VIP升级
struct CMD_GP_VIPUpgrade
{
	BYTE							cbVIPLevel;
	TCHAR							szNickName[LEN_NICKNAME];
};

#define SUB_GPR_ChangeItem 702

//兑换公告
struct CMD_GP_ChangeItem
{
	DWORD							dwLotteryCount;
	TCHAR							szNickName[LEN_NICKNAME];
	TCHAR							szItemName[LEN_NICKNAME];
};

#define SUB_GP_SystemBroadcast 703

#define SUB_GP_SystemBroadLaBa 704

#define MDM_GP_ITEM 1000

//查询道具数量
#define SUB_GPR_QUERY_ITEM_COUNT 1
#define SUB_GPO_QUERY_ITEM_COUNT 1001
//GSP_ItemQueryCount

struct CMD_GPR_QueryItemCount
{
	DWORD dwUserID;
};

struct CMD_GPO_QueryItemCount
{
	int nChuJiCount;
	int nZhongJiCount;
	int nGaoJiCount;
	int nZhiZunCount;
	int nCannonStatus;//炮台状态
	int nJiaSu;//加速
	int nSanShe;//散射
	int nBaoJi;//暴击
};

//赠送道具
#define SUB_GPR_TRANSFER_ITEM 2
#define SUB_GPO_TRANSFER_ITEM 1002
//GSP_ItemTransfer

struct CMD_GPR_TransferItem
{
	DWORD dwUserID;
	DWORD dwTargetGameID;
	int nItemID;
	int nItemCount;
	TCHAR szPassword[LEN_MD5];
};

struct CMD_GPO_TransferItem
{
	int nItemID;//道具类型ID
	int nSourceCount;//原来的数量
	int nVariationCount;//改变的数量
	int nResultCode;//处理结果
	TCHAR szDescribeString[128];
};

//合成道具
#define SUB_GPR_COMBINE_ITEM 3
#define SUB_GPO_COMBINE_ITEM 1003
//GSP_ItemCombine

struct CMD_GPR_CombineItem
{
	DWORD dwUserID;
	int nCombineType;//1=合成小龙珠，2=合成大龙珠
};

struct CMD_GPO_CombineItem
{
	int nDragonPieceCount;//龙珠碎片数量
	int nSmallDragonCount;//小龙珠数量
	int nBigDragonCount;//大龙珠数量
	int nResultCode;//处理结果
	TCHAR szDescribeString[128];
};

//使用龙珠道具
#define SUB_GPR_USE_DRAGON 4
#define SUB_GPO_USE_DRAGON 1004
//GSP_ItemUseDragon

struct CMD_GPR_UseDragon
{
	DWORD dwUserID;
	int nDragonType;//0初级珍珠，1中级珍珠，2高级珍珠，3至尊珍珠
};

struct CMD_GPO_UseDragon
{
	SCORE lCurrentScore;//当前分数
	int nChuJiCount;
	int nZhongJiCount;
	int nGaoJiCount;
	int nZhiZunCount;
	int nResultCode;//处理结果
	TCHAR szDescribeString[128];
};

//装备龙珠
#define SUB_GPR_EQUIPMENT_DRAGON 5
#define SUB_GPO_EQUIPMENT_DRAGON 1005
//GSP_ItemEquipmentDragon

struct CMD_GPR_EquipmentDragon
{
	DWORD dwUserID;
	int nDragonType;//0初级珍珠，1中级珍珠，2高级珍珠，3至尊珍珠
};

struct CMD_GPO_EquipmentDragon
{
	int nEquipmentDragonCount;//装备的结果
	int nChuJiCount;
	int nZhongJiCount;
	int nGaoJiCount;
	int nZhiZunCount;
	int nResultCode;//处理结果
	TCHAR szDescribeString[128];
};

//道具兑换
#define SUB_GPR_EXCHANGE_LOTTERY 6
#define SUB_GPO_EXCHANGE_LOTTERY 1006
struct CMD_GPR_ExchangeLottery
{
	DWORD dwUserID;
	int nDragonType;//0初级珍珠，1中级珍珠，2高级珍珠，3至尊珍珠
};

struct CMD_GPO_ExchangeLottery
{
	int nLotteryCount;//当前奖券数量（兑换后）
	int nChuJiCount;
	int nZhongJiCount;
	int nGaoJiCount;
	int nZhiZunCount;
	int nResultCode;//处理结果
	TCHAR szDescribeString[128];
};


#define MDM_GP_SKILL 1001

#define SUB_GPR_QUERY_SKILL 1
#define SUB_GPO_QUERY_SKILL 1001

struct CMD_GPR_QuerySkill
{
	DWORD dwUserID;
};

struct CMD_GPO_QuerySkill
{
	int nCannonStatus;//炮台状态
	int nJiaSu;//加速
	int nSanShe;//散射
	int nBaoJi;//暴击
	BYTE cbCount[3];//数量
	BYTE cbPrice[7];//价格
};

//注册帐号请求发送验证码
struct CMD_GPR_MBVerificationCode
{
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//手机号
};

//绑定手机号验证码请求
struct CMD_GPR_MB_BIND_REQUEST
{
	DWORD							dwUserId;							//用户id
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//手机号
};

struct CMD_GPO_MBVerificationCode
{
	bool bIsVerificationCodeSend;
	TCHAR szDescribeString[128];				//描述消息
};

#define SUB_GPR_NEW_GUID_QUERY 2
#define SUB_GPO_NEW_GUID_QUERY 1002

struct CMD_GPR_NewGuidQuery
{
	DWORD dwUserID;
};

struct CMD_GPO_NewGuidQuery
{
	bool bCan;
};

#define SUB_GPR_SuperGold 3
#define SUB_GPO_SuperGold 1003

struct CMD_GPR_SuperGold
{
	DWORD dwUserID;
};

struct CMD_GPO_SuperGold
{
	BYTE cbStatus;
	int nSuperGold;
};

//手机号绑定成功
struct CMD_MB_NumBindSuccess
{
	int								nCurrentGold;
	TCHAR							szDescribeString[128];				//描述消息
};

//手机号绑定失败
struct CMD_MB_NumBindFailure
{
	TCHAR							szDescribeString[128];				//描述消息
};

struct CMD_GPR_NumCheckExit
{
	DWORD dwUserID;
};

struct CMD_GPR_BindRewardNewQuid
{
	DWORD dwUserID;
};

struct CMD_GPR_SetSuperNewQuid
{
	DWORD dwUserID;
};

struct CMD_GPR_MatchGetMatchPrise                       //用户领取奖励
{
	int nMatchID;
	DWORD dwUserID;
	char szMatchData[16];
};

struct CMD_GPO_MatchGetMatchPriseRes                    //用户领取奖励结果
{
	bool bPriseStatus;//领取状态，0为领取失败，1为领取成功
	BYTE nPriseType;//比赛奖励类型
	int nPriseCount;//领取数量
	SCORE llUserScore;//玩家当前金币数量
	char szDescription[128];
	int nMatchId;
};


#pragma pack()

#endif