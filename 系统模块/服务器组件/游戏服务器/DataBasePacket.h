#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�������ݰ�

//�û��¼�
#define	DBR_GR_LOGON_USERID			100									//I D ��¼
#define	DBR_GR_LOGON_MOBILE			101									//�ֻ���¼
#define	DBR_GR_LOGON_ACCOUNTS		102									//�ʺŵ�¼
#define DBR_GR_LOGON_MATCHTIMER		103									//��ʱِ��¼

//ϵͳ�¼�
#define DBR_GR_WRITE_GAME_SCORE		200									//��Ϸ����
#define DBR_GR_LEAVE_GAME_SERVER	201									//�뿪����
#define DBR_GR_GAME_SCORE_RECORD	202									//���ּ�¼
#define DBR_GR_MANAGE_USER_RIGHT	203									//Ȩ�޹���
#define DBR_GR_LOAD_SYSTEM_MESSAGE	204									//ϵͳ��Ϣ
#define DBR_GR_LOAD_SENSITIVE_WORDS	205									//���д�
#define DBR_GR_CHECKSTOCK			206									//�����
#define DBR_GR_GET_USER_LUCKY		207
#define DBR_GR_SAVEUSERITEM			208									//�����û���Ʒ
#define DBR_GR_GAME_JSON			210
#define DBR_GR_SENTENCE			    211
#define DBR_GR_LOAD_MATCH_CONFIG	212									//���ر���ʱ��������Ϣ
#define DBR_GR_LOAD_MATCH_REWARD_CONFIG	213								//���ر�������������Ϣ

//�����¼�
#define DBR_GR_LOAD_PARAMETER		300									//���ز���
#define DBR_GR_LOAD_GAME_COLUMN		301									//�����б�
#define DBR_GR_LOAD_ANDROID_USER	302									//���ػ���
#define DBR_GR_LOAD_GAME_PROPERTY	303									//���ص���

//�����¼�
#define DBR_GR_USER_SAVE_SCORE		400									//������Ϸ��
#define DBR_GR_USER_TAKE_SCORE		401									//��ȡ��Ϸ��
#define DBR_GR_USER_TRANSFER_SCORE	402									//ת����Ϸ��
#define DBR_GR_QUERY_INSURE_INFO	403									//��ѯ����
#define DBR_GR_QUERY_TRANSFER_USER_INFO	    404							//��ѯ�û�

//��Ϸ�¼�
#define DBR_GR_PROPERTY_REQUEST		500									//��������
#define DBR_GR_GAME_FRAME_REQUEST	502									//��Ϸ����
#define DBR_GR_CHECK_TRUMPET		503									//������
#define DBR_GR_FIRST_ENTER_SUPER	504									//��һ�ν��볬������
//�����¼�
#define DBR_GR_MATCH_FEE			600									//��������
#define DBR_GR_MATCH_START			601									//������ʼ
#define DBR_GR_MATCH_OVER			602									//��������
#define DBR_GR_MATCH_REWARD			603									//��������
#define DBR_GR_MATCH_QUIT			604									//�˳�����
#define DBR_GR_MATCH_ENTER_FOR		605									//��������

//////////////////////////////////////////////////////////////////////////////////
//�����Ϣ

//�߼��¼�
#define DBO_GR_LOGON_SUCCESS		100									//��¼�ɹ�
#define DBO_GR_LOGON_FAILURE		101									//��¼ʧ��

//�����¼�
#define DBO_GR_GAME_PARAMETER		200									//������Ϣ
#define DBO_GR_GAME_COLUMN_INFO		201									//�б���Ϣ
#define DBR_GR_GAME_ANDROID_INFO	202									//������Ϣ
#define DBO_GR_GAME_PROPERTY_INFO	203									//������Ϣ
#define DBR_GR_CHECKSTOCK_SUCCESS	204									//�����ɹ�
#define DBR_GET_USER_LUCKY_SUCCESS	205
#define DBR_LOADUSERITEM_SUCCESS	206									//�����û���Ʒ�ɹ�
#define DBO_GR_PRESENT_SCORE		207

//��������
#define DBO_GR_USER_INSURE_INFO		300									//��������
#define DBO_GR_USER_INSURE_SUCCESS	301									//���гɹ�
#define DBO_GR_USER_INSURE_FAILURE	302									//����ʧ��
#define DBO_GR_USER_INSURE_USER_INFO   303								//�û�����
#define DBO_GR_USER_INSURE_TRANSFER_INFO	304

//��Ϸ�¼�
#define DBO_GR_PROPERTY_SUCCESS		400									//���߳ɹ�
#define DBO_GR_PROPERTY_FAILURE		401									//����ʧ��
#define DBO_GR_GAME_FRAME_RESULT	402									//��Ϸ���
#define DBO_GR_CHECK_TRUMPET		403

//�����¼�
#define DBO_GR_MATCH_EVENT_START	500									//�¼���Χ
#define DBO_GR_MATCH_FEE_RESULT		500									//��������
#define DBO_GR_MATCH_RANK			501									//��������
#define DBO_GR_MATCH_QUIT_RESULT	502									//�������
#define DBO_GR_MATCH_EVENT_END		599									//�¼���Χ

//ϵͳ�¼�
#define DBO_GR_SYSTEM_MESSAGE_RESULT	600									//��Ϣ���
#define DBO_GR_SENSITIVE_WORDS		    601									//���д�

//////////////////////////////////////////////////////////////////////////////////

//ID ��¼
struct DBR_GR_LogonUserID
{		
	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//ID ��¼
struct DBR_GR_LogonMobile
{		
	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	BYTE                            cbDeviceType;                       //�豸����
	WORD                            wBehaviorFlags;                     //��Ϊ��ʶ
	WORD                            wPageTableCount;                    //��ҳ����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//�ʺŵ�¼
struct DBR_GR_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//��Ϸ����
struct DBR_GR_WriteGameScore
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ

	//�û���Ϣ
	DWORD							dwDBQuestID;						//�����ʶ
	DWORD							dwInoutIndex;						//��������

	//�ɼ�����
	tagVariationInfo				VariationInfo;						//��ȡ��Ϣ
};

//�뿪����
struct DBR_GR_LeaveGameServer
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwInoutIndex;						//��¼����
	DWORD							dwLeaveReason;						//�뿪ԭ��
	DWORD							dwOnLineTimeCount;					//����ʱ��

	//�ɼ�����
	tagVariationInfo				RecordInfo;							//��¼��Ϣ
	tagVariationInfo				VariationInfo;						//��ȡ��Ϣ

	//ϵͳ��Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szNickName[LEN_NICKNAME];
	TCHAR							szServerName[LEN_SERVER];			//��������
};

//��Ϸ��¼
struct DBR_GR_GameScoreRecord
{
	//������Ϣ
	WORD							wTableID;							//���Ӻ���
	WORD							wUserCount;							//�û���Ŀ
	WORD							wAndroidCount;						//������Ŀ

	//���˰��
	SCORE							lWasteCount;						//�����Ŀ
	SCORE							lRevenueCount;						//˰����Ŀ

	//ͳ����Ϣ
	DWORD							dwUserMemal;						//������Ŀ
	DWORD							dwPlayTimeCount;					//��Ϸʱ��

	//ʱ����Ϣ
	SYSTEMTIME						SystemTimeStart;					//��ʼʱ��
	SYSTEMTIME						SystemTimeConclude;					//����ʱ��

	//���ּ�¼
	WORD							wRecordCount;						//��¼��Ŀ
	tagGameScoreRecord				GameScoreRecord[MAX_CHAIR];			//��Ϸ��¼
};

//������Ϸ��
struct DBR_GR_UserSaveScore
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������Ϸ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ȡ����Ϸ��
struct DBR_GR_UserTakeScore
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ��Ϸ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת����Ϸ��
struct DBR_GR_UserTransferScore
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	BYTE                            cbByNickName;                       //�ǳ�����
	SCORE							lTransferScore;						//ת����Ϸ��
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ѯ����
struct DBR_GR_QueryInsureInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//��ѯ�û�
struct DBR_GR_QueryTransferUserInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
	DWORD							dwUserID;							//�û� I D
};

//��������
struct DBR_GR_PropertyRequest
{
	//������Ϣ
	WORD							wItemCount;							//������Ŀ
	WORD							wPropertyIndex;						//��������
	DWORD							dwSourceUserID;						//�������
	DWORD							dwTargetUserID;						//ʹ�ö���

	//����ģʽ
	BYTE                            cbRequestArea;			            //ʹ�÷�Χ 
	BYTE							cbConsumeScore;						//��������
	SCORE							lFrozenedScore;						//�������

	//�û�Ȩ��
	DWORD                           dwUserRight;						//��ԱȨ��

	//ϵͳ��Ϣ
	WORD							wTableID;							//���Ӻ���
	DWORD							dwInoutIndex;						//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//������
struct DBR_GR_CheckTrumpet
{
	DWORD                          dwUserID;
	BYTE                           cbRequestArea;						//����Χ 
	WORD                           wPropertyIndex;						//�������� 
	DWORD                          TrumpetColor;						//������ɫ
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];	//��������
	LONG                           lReturnValue;
};

//////////////////////////////////////////////////////////////////////////////////

//��¼�ɹ�
struct DBO_GR_LogonSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwGroupID;							//���� I D
	DWORD							dwCustomID;							//�Զ�ͷ��
	TCHAR							szNickName[LEN_NICKNAME];			//�ʺ��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	BYTE							cbMasterOrder;						//����ȼ�
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//������Ϣ
	SCORE							lScore;								//�û�����
	SCORE							lGrade;								//�û��ɼ�
	SCORE							lInsure;							//�û�����

	//��Ϸ��Ϣ
	DWORD							dwWinCount;							//ʤ������
	DWORD							dwLostCount;						//ʧ������
	DWORD							dwDrawCount;						//�;�����
	DWORD							dwFleeCount;						//��������
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//�û�����
	LONG							lLoveLiness;						//�û�����

	//�û�Ȩ��
	DWORD							dwUserRight;						//�û�Ȩ��
	DWORD							dwMasterRight;						//����Ȩ��

	//��������
	DWORD							dwInoutIndex;						//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ

	//�ֻ�����
	BYTE                            cbDeviceType;                       //�豸����
	WORD                            wBehaviorFlags;                     //��Ϊ��ʶ
	WORD                            wPageTableCount;                    //��ҳ����

	BYTE							cbVIPLevel;
	DWORD							dwVIPExp;

	//������Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szDescribeString[128];				//������Ϣ
	int								nTreasureMapCount;					//�ر�ͼ����
};

//��¼ʧ��
struct DBO_GR_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//������Ϣ
struct DBO_GR_GameParameter
{
	//������Ϣ
	WORD							wMedalRate;							//���ƻ���
	WORD							wRevenueRate;						//����˰��

	//�汾��Ϣ
	DWORD							dwClientVersion;					//�ͻ��汾
	DWORD							dwServerVersion;					//����汾
};

//�б���Ϣ
struct DBO_GR_GameColumnInfo
{
	LONG							lResultCode;						//�������
	BYTE							cbColumnCount;						//�б���Ŀ
	tagColumnItem					ColumnItemInfo[MAX_COLUMN];			//�б���Ϣ
};

//������Ϣ
struct DBO_GR_GameAndroidInfo
{
	LONG							lResultCode;						//�������
	WORD							wAndroidCount;						//�û���Ŀ
	tagAndroidParameter				AndroidParameter[MAX_ANDROID];		//������Ϣ
};

//������Ϣ
struct DBO_GR_GamePropertyInfo
{
	LONG							lResultCode;						//�������
	BYTE							cbPropertyCount;					//������Ŀ
	tagPropertyInfo					PropertyInfo[MAX_PROPERTY];			//������Ϣ
};

//��������
struct DBO_GR_UserInsureInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;						//������Ϸ��
	SCORE							lTransferPrerequisite;				//ת������
};

//���гɹ�
struct DBO_GR_UserInsureSuccess
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	SCORE							lSourceScore;						//ԭ����Ϸ��
	SCORE							lSourceInsure;						//ԭ����Ϸ��
	SCORE							lInsureRevenue;						//����˰��
	SCORE							lFrozenedScore;						//�������
	SCORE							lVariationScore;					//��Ϸ�ұ仯
	SCORE							lVariationInsure;					//���б仯
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct DBO_GR_UserInsureFailure
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	LONG							lResultCode;						//��������
	SCORE							lFrozenedScore;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�û�����
struct DBO_GR_UserTransferUserInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwGameID;							//�û� I D
	TCHAR							szNickName[LEN_NICKNAME];			//�ʺ��ǳ�
};

//���߳ɹ�
struct DBO_GR_S_PropertySuccess
{
	//������Ϣ
	WORD							wItemCount;							//������Ŀ
	WORD							wPropertyIndex;						//��������
	DWORD							dwSourceUserID;						//�������
	DWORD							dwTargetUserID;						//ʹ�ö���

	//����ģʽ
	BYTE                            cbRequestArea;						//����Χ
	BYTE							cbConsumeScore;						//��������
	SCORE							lFrozenedScore;						//�������

	//��ԱȨ��
	DWORD                           dwUserRight;						//��ԱȨ��

	//�����Ϣ
	SCORE							lConsumeGold;						//������Ϸ��
	LONG							lSendLoveLiness;					//��������
	LONG							lRecvLoveLiness;					//��������

	//��Ա��Ϣ
	BYTE							cbMemberOrder;						//��Ա�ȼ�
};

//����ʧ��
struct DBO_GR_PropertyFailure
{
	BYTE                            cbRequestArea;						//����Χ
	LONG							lResultCode;						//��������
	SCORE							lFrozenedScore;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��������
struct DBO_GR_MatchRank
{
	BYTE							cbRank;								//��������
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	LONG							lMatchScore;						//�û��÷�
};

//�ݷ�
struct DBO_GR_PresentScore
{
	int								nScore;
	int								nTime;
	TCHAR							szNickName[LEN_NICKNAME];
	TCHAR							szServerName[LEN_SERVER];
};

//Ȩ�޹���
struct DBR_GR_ManageUserRight
{
	DWORD							dwUserID;							//Ŀ���û�
	DWORD							dwAddRight;							//���Ȩ��
	DWORD							dwRemoveRight;						//ɾ��Ȩ��
	bool							bGameRight;							//��ϷȨ��
};

//��������
struct DBR_GR_MatchFee
{
	//��������
	DWORD							dwUserID;							//�û�ID
	DWORD							dwMatchFee;							//��������

	//ϵͳ��Ϣ
	DWORD							dwInoutIndex;						//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	DWORD							dwMatchID;							//����ID
	DWORD							dwMatchNO;							//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//������ʼ
struct DBR_GR_MatchStart
{
	DWORD							dwMatchID;						//������ʶ
	DWORD							dwMatchNo;						//��������
	WORD							wMatchCount;					//�ܳ���
	LONG							lInitScore;						//��ʼ����
};

//��������
struct DBR_GR_MatchOver
{
	DWORD							dwMatchID;						//������ʶ
	DWORD							dwMatchNo;						//��������
	WORD							wMatchCount;					//�ܳ���
};

//��������
struct DBR_GR_MatchReward
{
	DWORD								dwUserID;							//�û� I D
	DWORD								dwMatchID;							//���� I D 
	DWORD								dwMatchNO;							//��������
	WORD								wRank;								//��������
	SCORE								lMatchScore;						//�����÷�
	DWORD								dwExperience;						//�û�����
	DWORD								dwGold;								//��Ϸ��
	DWORD								dwMedal;							//�û�����
	DWORD								dwClientAddr;						//���ӵ�ַ
};

//��ʱģʽ������¼
struct DBR_GR_MatchEnterFor
{
	DBR_GR_LogonUserID			LogonUserID;
};


//ϵͳ��Ϣ
struct DBR_GR_SystemMessage
{
	BYTE                            cbMessageID;                        //��ϢID
	BYTE							cbMessageType;						//��Ϣ����
	BYTE                            cbAllRoom;                          //ȫ�巿��
	DWORD							dwTimeRate;						    //ʱ��Ƶ��
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//ϵͳ��Ϣ
};

//��Ϸ��¼Json��ʽ
struct DBR_GR_GameJson
{
	DWORD							dwUserID;
	DWORD							dwKindID;
	DWORD							dwServerID;
	SCORE							lPresentGold;
	TCHAR							szJson[1024];
};

//ִ�����ݿ�ű�
struct DBR_GR_Sentence
{
	TCHAR							szSQL[1024];
};

#define DBR_GR_USER_TRANSFER_DRAGON 700
#define DBO_GR_USER_TRANSER_DRAGON 701

//��������
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

//ʹ������
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

//Ӯ��������Ƭ
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
	int nCannonStatus;//��̨״̬
	int nJiaSu;//����
	int nSanShe;//ɢ��
	int nBaoJi;//����
	BYTE cbCount[3];//����
	BYTE cbPrice[7];//�۸�
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
	int nCannonStatus;//��̨״̬
	int nLotteryCount;//��ȯ����
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

//�û���־
#define DBR_USERLOG_CLOWN_OPERATION		715
struct tagGuessOperation
{
	int   nStarCount;
	int   nChipTimes;	//����	
};
//С�������־
struct tagClownOperation
{
	int   nRoundNum;	//�ڼ��ֲ���, ת�̻��ǲ´�С
	bool  bIsWin;       //�Ƿ�ɹ�
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
	std::vector<tagClownOperation> *	pClownOperation;					//��¼һ���������еĲ���
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


// ��������
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
	TCHAR szDescribeString[128];				//������Ϣ
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

// ��������
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
	bool bPriseStatus;//��ȡ״̬��0Ϊ��ȡʧ�ܣ�1Ϊ��ȡ�ɹ�
	BYTE nPriseType;//������������
	int nPriseCount;//��ȡ����
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