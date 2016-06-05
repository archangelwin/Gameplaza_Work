#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE
#pragma pack(1)
/////////////////////////////////////////////////////////////////////////////////
//服务定义

#define KIND_1

//游戏属性
#if defined(KIND_0)
#define KIND_ID						2000
#define GAME_NAME					TEXT("保卫钓鱼岛")
#define CLIENT_EXE_NAME				TEXT("CatchFish01.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish01.dll")
#elif defined(KIND_1)
#define KIND_ID						2001
#define GAME_NAME					TEXT("金蟾捕鱼")
#define CLIENT_EXE_NAME				TEXT("CatchFish02.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish02.dll")
#elif defined(KIND_2)
#define KIND_ID						2002
#define GAME_NAME					TEXT("倚天屠龙")
#define CLIENT_EXE_NAME				TEXT("CatchFish03.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish03.dll")
#elif defined(KIND_4)
#define KIND_ID						2004
#define GAME_NAME					TEXT("西游争霸")
#define CLIENT_EXE_NAME				TEXT("CatchFish05.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish05.dll")
#elif defined(KIND_5)
#define KIND_ID						2005
#define GAME_NAME					TEXT("渔王争霸")
#define CLIENT_EXE_NAME				TEXT("CatchFish06.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish06.dll")
//#elif defined(KIND_1)
//#define KIND_ID						2001
//#define GAME_NAME					TEXT("金蟾捕鱼")
//#define CLIENT_EXE_NAME				TEXT("CatchFish02.exe")
//#define SERVER_DLL_NAME				TEXT("CatchFish02.dll")
//#elif defined(KIND_3)
//#define KIND_ID						2003
//#define GAME_NAME					TEXT("哪吒闹海")
//#define CLIENT_EXE_NAME				TEXT("CatchFish04.exe")
//#define SERVER_DLL_NAME				TEXT("CatchFish04.dll")
#elif defined(KIND_6)
#define KIND_ID						2006
#define GAME_NAME					TEXT("李逵劈鱼")
#define CLIENT_EXE_NAME				TEXT("CatchFish07.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish07.dll")
#elif defined(KIND_7)
#define KIND_ID						2007
#define GAME_NAME					TEXT("金蟾副本")
#define CLIENT_EXE_NAME				TEXT("CatchFish08.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish08.dll")
#endif

//组件属性
#define GAME_PLAYER					2									//游戏人数
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//程序版本

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//等待开始
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY+1					//游戏进行

#define PATH_POINT_COUNT			5									//路径使用的坐标点数量

#define MAX_HITFISH 100

//命令定义
enum enServerSubCmd
{
	enServerSubCmd_Begin=59,
	SUB_S_GAME_SCENE=60,								//场景消息
	SUB_S_TRACE_POINT,									//轨迹坐标
	SUB_S_USER_SHOOT,									//发射炮弹
	SUB_S_CAPTURE_FISH,									//捕获鱼群
	SUB_S_BULLET_COUNT,									//子弹数目
	SUB_S_COIN_COUNT=65,								//金币数目
	SUB_S_FISH_COUNT,									//鱼群数目
	SUB_S_CHANGE_SCENE,									//切换场景
	SUB_S_SCORE_INFO,									//分数信息
	SUB_S_SET_CELL_SCORE,								//单元积分
	SUB_S_BONUS_INFO=70,								//彩金信息
	SUB_S_GAME_MESSAGE,									//游戏消息
	SUB_S_LOCKFISH,
	SUB_S_UPDATE_BOSS_SCORE,
	SUB_S_JUMP_BOMB,
	SUB_S_TASK_INFO=75,
	SUB_S_TIME_PRIZE_INFO,
	SUB_S_TIME_PRIZE_RECEIVED,
	SUB_S_TASK_RECEIVED,
	SUB_S_MOBILE_MONEY,
	SUB_S_MOBILE_MONEY_SUCCESS=80,
	SUB_S_RETURN_MOBILE_MONEY,
	SUB_S_MOBILE_CHARGE,
	SUB_S_EQUIPMENT_DRAGON_COUNT,
	SUB_S_TRY_CANNON,
	SUB_S_TRY_CANNON_END=85,
	SUB_S_NEW_GUID_GIFT,
	SUB_S_TRY_CANNON_CHECK,
	SUB_S_WIN_PEARL,
	SUB_S_MATCH_RANK,
	SUB_S_SEND_PEARL_INFO=90,
	SUB_S_EXCHANGE_PEARL,
	SUB_S_FIRST_MOBILE_MONEY,
	SUB_S_SCORE_FULL,
	SUB_S_BOSS_COMING,
	SUB_S_VIP_LEVEL=95,
	enServerSubCmd_End
};

//鱼的附加属性
enum enFishAttrib
{
	enFreeze=0x01,//定屏
	enBomb=0x02,//炸弹
	enBoss=0x04,//动态倍率鱼
	enSameDie=0x08,//同类炸弹
	enLine=0x10,//连线鱼
	enRangeBomb=0x20,//范围炸弹
	enLighting=0x40,//闪电鱼
	enJumpBomb=0x80,//博乐弹
};

//
struct tagFishPack
{
	int nFishID;//鱼ID
	int nSpriteID;//鱼类型
	int nPathIndex1;//鱼轨迹
	int nCreateTime;
	int nCreateDelayTime;
	int nMultiple;
	int nProperty;

	int nXPos;//初始坐标，当nPathIndex1为-1时有效
	int nYPos;
	float fRotation[5];//初始角度，当nPathIndex1为-1时有效
	int nMoveTime[5];
	int nSpeed[5];
};

//倍率信息
struct CMD_S_CannonMultiple
{
	WORD							wChairID;						//设置玩家
	BYTE							cbCannonType;//0小泡，1双重炮，2冰霜炮，3月牙炮
	int								nCannonMultiple;
};

//切换场景
struct CMD_S_ChangeScene
{
	WORD							SceneIndex;						//当前场景
	WORD							RmoveID;						//预留这里
};

//子弹数目
struct CMD_S_BulletCount
{
	WORD							wChairID;						//玩家椅子
	bool							bAdd;
	int								lScore;
};

//金币数目
struct CMD_S_CoinCount
{
	WORD							wChairID;						//玩家椅子
	LONG							lCoinCount;						//金币数目
};

//场景消息
struct CMD_S_GameScene
{
	int								nSceneIndex;
	int								nMaxBullet;
	LONG							lCellScore;
	LONG							lMaxMultiple;
	LONG							lUserAllScore[GAME_PLAYER];
	LONG							lUserCellScore[GAME_PLAYER];
	BYTE							cbCannonType[GAME_PLAYER];//0小泡，1双重炮，2冰霜炮，3月牙炮
	int								nExercise;
	bool							bNoFish;
};

//发射炮弹
struct CMD_S_UserShoot
{
	WORD							wChairID;						//玩家椅子
	bool							bAndroid;
	BYTE							cbAttrib;						//子弹属性，0普通子弹、1暴击子弹
	float							fAngle;							//发射角度
	int								nUserScore;
	DWORD							dwBulletID;
	DWORD							dwCount;						//子弹数量，大于1时为散射
};

//单元积分
struct CMD_S_LockFishId
{
	LONG							lFishID;						//单元积分
	int								iChairID;
};

//捕获鱼群
struct CMD_S_CaptureFish
{
	int								nLotteryCount;
	WORD							wChairID;						//打中鱼的玩家
	bool							bSuperCannon;					//是否变换能量炮
	bool							bBigLottery;
	int								nUserScore;						//玩家当前分数
	int								nTryScore;						//试用分数
	int								nProperty;						//打中的鱼的属性
	int								nCaptureCount;
	int								nTotalScore;
	int								nTotalMultiple;
	int								nFishID[MAX_HITFISH];			//打中的鱼的ID
	int								nFishMultiple[MAX_HITFISH];
	int								nFishScore[MAX_HITFISH];
};

//更新BOSS分数
struct CMD_S_UpdateBossScore
{
	int								nBossScore;
};

//JumpBomb
struct CMD_S_JumpBomb
{
	WORD							wChairID;
	int								nSpriteID;
	CPoint							ptStart;
	CPoint							ptJump[3];

	CPoint calcPoint(CPoint & pt)
	{
		CPoint ptRet;
		if (pt.x<640&&pt.y<355)
		{
			ptRet.x=640+rand()%630;
			ptRet.y=355+rand()%310;
		}
		else if (pt.x>640&&pt.y<355)
		{
			ptRet.x=640-rand()%630;
			ptRet.y=355+rand()%310;
		}
		else if (pt.x>640&&pt.y>355)
		{
			ptRet.x=640-rand()%630;
			ptRet.y=355-rand()%310;
		}
		else if (pt.x<640&&pt.y>355)
		{
			ptRet.x=640+rand()%630;
			ptRet.y=355-rand()%310;
		}
		return ptRet;
	}
};

//@话费
struct CMD_S_MobileMoney
{
	WORD							wMoneyCount;
};

struct CMD_S_ReturnMobileMoney
{
	WORD							wMoneyCount;
};

struct CMD_S_MobileCharge
{
	bool							bSuccess;
	WORD							wMoneyCurrent;
};

struct CMD_S_EquipmentDragonCount
{
	int nEquipmentDragonCount[GAME_PLAYER];
};

struct CMD_S_TryCannonEnd
{
	int nTryScore;
};

struct CMD_S_NewGuidGift
{
	bool bSuccess;
	int nCannonStatus;
	int nLotteryCount;
	int nScoreGift;
	int nCurrentScore;
};

struct CMD_S_TryCannonCheck
{
	bool bCan;
};

struct CMD_S_WinPearl
{
	int nPearlType;
	int nPearlCount;
};

struct CMD_S_MatchRank
{
	SCORE lMatchScore;//比赛积分
	int nCurrentRank;//当前排名
	int nChangeRank;//排名变化
};

struct CMD_S_SendPearlInfo
{
	int nPearl1Price;
	int nPearl2Price;
	int nPearl3Price;
};

struct CMD_S_ExchangePearl
{
	BYTE cbResult;
	char szResult[64];
};

struct CMD_S_VipLevel
{
	BYTE cbVipLevel[GAME_PLAYER];
};

//////////////////////////////////////////////////////////////////////////
//命令定义

enum enClientSubCmd
{
	enClientSubCmd_Begin=60,
	SUB_C_BUY_BULLETSPEED,								//购买子弹
	SUB_C_BUY_BULLET,									//购买子弹
	SUB_C_USER_SHOOT,									//发射炮弹
	SUB_C_CAPTURE_FISH,									//捕获鱼群
	SUB_C_GET_SCORE_INFO=65,								//分数信息
	SUB_C_CALC_SCORE,									//计算分数
	SUB_C_SET_CELL_SCORE,								//单元积分
	SUB_C_HIT_FISH,										//捕获鱼群
	SUB_C_CANNON_MULTIPLE,								//设置概率
	SUB_C_SET_COMEINUSER=70,							//设置概率
	SUB_C_LOCKFISH,										//锁定鱼
	SUB_C_TIME_PRIZE_RECEIVE,
	SUB_C_GET_TASK,
	SUB_C_TASK_RECEIVE,
	SUB_C_SHARE_INFO=75,
	SUB_C_GET_MOBILE_MONEY,
	SUB_C_MOBILE_CHARGE,
	SUB_C_TRY_CANNON,
	SUB_C_NEW_GUID_GIFT,
	SUB_C_TRY_CANNON_CHECK=80,
	SUB_C_EXCHANGE_PEARL,
	enClientSubCmd_End
};

//单元积分
struct CMD_C_LockFishId
{
	LONG							lFishID;						//单元积分
	WORD							wChairID;
	char							szCommand[MAX_PATH];
};

//购买子弹
struct CMD_C_BuyBullet
{
	int								lScore;							//上下分数
	bool							bAdd;
};

//发射炮弹
struct CMD_C_UserShoot
{
	float							fAngle;							//发射角度
	DWORD							dwBulletID;						//子弹标识
	DWORD							dwCount;						//子弹数量，大于1时为散射
	BYTE							cbAttrib;						//子弹属性，0普通子弹、1暴击子弹
};

//击中鱼群
struct CMD_C_HitFish
{
	int								nFishID;
	unsigned int					dwBulletID;
	int								nHitUser;
	bool							bAndroid;
	int								nOtherCount;
	int								nXPos;
	int								nYPos;
	int								nOtherFishID[MAX_HITFISH];
};

struct CMD_C_CoinCount
{
	LONG							lCoinCount;						//金币数目
};

//设置概率
struct CMD_C_CannonMultiple
{
	BYTE							cbAddMultiple;//0不改变，1增加，2减少
	BYTE							cbCannonType;//0小泡，1双重炮，2冰霜炮，3月牙炮
};

enum eShareReason
{
	eShareMobileMoney=100,//话费
};

enum eShareAction
{
	eShareClick=100,//点击分享
	eShareSuccess,//分享成功
	eShareFail,//分享失败
	eShareCancel,//取消分享
};

struct CMD_C_ShareInfo
{
	int nShareReason;
	int nShareAction;
};

struct CMD_C_MobileCharge
{
	WORD wMoneyCount;
};

struct CMD_C_ExchangePearl
{
	int nPearlType;
	int nPearlCount;
};

//////////////////////////////////////////////////////////////////////////

#endif