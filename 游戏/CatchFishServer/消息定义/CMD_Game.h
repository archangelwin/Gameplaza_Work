#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE
#pragma pack(1)
/////////////////////////////////////////////////////////////////////////////////
//������

#define KIND_1

//��Ϸ����
#if defined(KIND_0)
#define KIND_ID						2000
#define GAME_NAME					TEXT("�������㵺")
#define CLIENT_EXE_NAME				TEXT("CatchFish01.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish01.dll")
#elif defined(KIND_1)
#define KIND_ID						2001
#define GAME_NAME					TEXT("��󸲶��")
#define CLIENT_EXE_NAME				TEXT("CatchFish02.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish02.dll")
#elif defined(KIND_2)
#define KIND_ID						2002
#define GAME_NAME					TEXT("��������")
#define CLIENT_EXE_NAME				TEXT("CatchFish03.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish03.dll")
#elif defined(KIND_4)
#define KIND_ID						2004
#define GAME_NAME					TEXT("��������")
#define CLIENT_EXE_NAME				TEXT("CatchFish05.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish05.dll")
#elif defined(KIND_5)
#define KIND_ID						2005
#define GAME_NAME					TEXT("��������")
#define CLIENT_EXE_NAME				TEXT("CatchFish06.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish06.dll")
//#elif defined(KIND_1)
//#define KIND_ID						2001
//#define GAME_NAME					TEXT("��󸲶��")
//#define CLIENT_EXE_NAME				TEXT("CatchFish02.exe")
//#define SERVER_DLL_NAME				TEXT("CatchFish02.dll")
//#elif defined(KIND_3)
//#define KIND_ID						2003
//#define GAME_NAME					TEXT("��߸�ֺ�")
//#define CLIENT_EXE_NAME				TEXT("CatchFish04.exe")
//#define SERVER_DLL_NAME				TEXT("CatchFish04.dll")
#elif defined(KIND_6)
#define KIND_ID						2006
#define GAME_NAME					TEXT("��������")
#define CLIENT_EXE_NAME				TEXT("CatchFish07.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish07.dll")
#elif defined(KIND_7)
#define KIND_ID						2007
#define GAME_NAME					TEXT("��󸸱��")
#define CLIENT_EXE_NAME				TEXT("CatchFish08.exe")
#define SERVER_DLL_NAME				TEXT("CatchFish08.dll")
#endif

//�������
#define GAME_PLAYER					2									//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY+1					//��Ϸ����

#define PATH_POINT_COUNT			5									//·��ʹ�õ����������

#define MAX_HITFISH 100

//�����
enum enServerSubCmd
{
	enServerSubCmd_Begin=59,
	SUB_S_GAME_SCENE=60,								//������Ϣ
	SUB_S_TRACE_POINT,									//�켣����
	SUB_S_USER_SHOOT,									//�����ڵ�
	SUB_S_CAPTURE_FISH,									//������Ⱥ
	SUB_S_BULLET_COUNT,									//�ӵ���Ŀ
	SUB_S_COIN_COUNT=65,								//�����Ŀ
	SUB_S_FISH_COUNT,									//��Ⱥ��Ŀ
	SUB_S_CHANGE_SCENE,									//�л�����
	SUB_S_SCORE_INFO,									//������Ϣ
	SUB_S_SET_CELL_SCORE,								//��Ԫ����
	SUB_S_BONUS_INFO=70,								//�ʽ���Ϣ
	SUB_S_GAME_MESSAGE,									//��Ϸ��Ϣ
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

//��ĸ�������
enum enFishAttrib
{
	enFreeze=0x01,//����
	enBomb=0x02,//ը��
	enBoss=0x04,//��̬������
	enSameDie=0x08,//ͬ��ը��
	enLine=0x10,//������
	enRangeBomb=0x20,//��Χը��
	enLighting=0x40,//������
	enJumpBomb=0x80,//���ֵ�
};

//
struct tagFishPack
{
	int nFishID;//��ID
	int nSpriteID;//������
	int nPathIndex1;//��켣
	int nCreateTime;
	int nCreateDelayTime;
	int nMultiple;
	int nProperty;

	int nXPos;//��ʼ���꣬��nPathIndex1Ϊ-1ʱ��Ч
	int nYPos;
	float fRotation[5];//��ʼ�Ƕȣ���nPathIndex1Ϊ-1ʱ��Ч
	int nMoveTime[5];
	int nSpeed[5];
};

//������Ϣ
struct CMD_S_CannonMultiple
{
	WORD							wChairID;						//�������
	BYTE							cbCannonType;//0С�ݣ�1˫���ڣ�2��˪�ڣ�3������
	int								nCannonMultiple;
};

//�л�����
struct CMD_S_ChangeScene
{
	WORD							SceneIndex;						//��ǰ����
	WORD							RmoveID;						//Ԥ������
};

//�ӵ���Ŀ
struct CMD_S_BulletCount
{
	WORD							wChairID;						//�������
	bool							bAdd;
	int								lScore;
};

//�����Ŀ
struct CMD_S_CoinCount
{
	WORD							wChairID;						//�������
	LONG							lCoinCount;						//�����Ŀ
};

//������Ϣ
struct CMD_S_GameScene
{
	int								nSceneIndex;
	int								nMaxBullet;
	LONG							lCellScore;
	LONG							lMaxMultiple;
	LONG							lUserAllScore[GAME_PLAYER];
	LONG							lUserCellScore[GAME_PLAYER];
	BYTE							cbCannonType[GAME_PLAYER];//0С�ݣ�1˫���ڣ�2��˪�ڣ�3������
	int								nExercise;
	bool							bNoFish;
};

//�����ڵ�
struct CMD_S_UserShoot
{
	WORD							wChairID;						//�������
	bool							bAndroid;
	BYTE							cbAttrib;						//�ӵ����ԣ�0��ͨ�ӵ���1�����ӵ�
	float							fAngle;							//����Ƕ�
	int								nUserScore;
	DWORD							dwBulletID;
	DWORD							dwCount;						//�ӵ�����������1ʱΪɢ��
};

//��Ԫ����
struct CMD_S_LockFishId
{
	LONG							lFishID;						//��Ԫ����
	int								iChairID;
};

//������Ⱥ
struct CMD_S_CaptureFish
{
	int								nLotteryCount;
	WORD							wChairID;						//����������
	bool							bSuperCannon;					//�Ƿ�任������
	bool							bBigLottery;
	int								nUserScore;						//��ҵ�ǰ����
	int								nTryScore;						//���÷���
	int								nProperty;						//���е��������
	int								nCaptureCount;
	int								nTotalScore;
	int								nTotalMultiple;
	int								nFishID[MAX_HITFISH];			//���е����ID
	int								nFishMultiple[MAX_HITFISH];
	int								nFishScore[MAX_HITFISH];
};

//����BOSS����
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

//@����
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
	SCORE lMatchScore;//��������
	int nCurrentRank;//��ǰ����
	int nChangeRank;//�����仯
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
//�����

enum enClientSubCmd
{
	enClientSubCmd_Begin=60,
	SUB_C_BUY_BULLETSPEED,								//�����ӵ�
	SUB_C_BUY_BULLET,									//�����ӵ�
	SUB_C_USER_SHOOT,									//�����ڵ�
	SUB_C_CAPTURE_FISH,									//������Ⱥ
	SUB_C_GET_SCORE_INFO=65,								//������Ϣ
	SUB_C_CALC_SCORE,									//�������
	SUB_C_SET_CELL_SCORE,								//��Ԫ����
	SUB_C_HIT_FISH,										//������Ⱥ
	SUB_C_CANNON_MULTIPLE,								//���ø���
	SUB_C_SET_COMEINUSER=70,							//���ø���
	SUB_C_LOCKFISH,										//������
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

//��Ԫ����
struct CMD_C_LockFishId
{
	LONG							lFishID;						//��Ԫ����
	WORD							wChairID;
	char							szCommand[MAX_PATH];
};

//�����ӵ�
struct CMD_C_BuyBullet
{
	int								lScore;							//���·���
	bool							bAdd;
};

//�����ڵ�
struct CMD_C_UserShoot
{
	float							fAngle;							//����Ƕ�
	DWORD							dwBulletID;						//�ӵ���ʶ
	DWORD							dwCount;						//�ӵ�����������1ʱΪɢ��
	BYTE							cbAttrib;						//�ӵ����ԣ�0��ͨ�ӵ���1�����ӵ�
};

//������Ⱥ
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
	LONG							lCoinCount;						//�����Ŀ
};

//���ø���
struct CMD_C_CannonMultiple
{
	BYTE							cbAddMultiple;//0���ı䣬1���ӣ�2����
	BYTE							cbCannonType;//0С�ݣ�1˫���ڣ�2��˪�ڣ�3������
};

enum eShareReason
{
	eShareMobileMoney=100,//����
};

enum eShareAction
{
	eShareClick=100,//�������
	eShareSuccess,//����ɹ�
	eShareFail,//����ʧ��
	eShareCancel,//ȡ������
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