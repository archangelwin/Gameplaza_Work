#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//��¼����

#define MDM_GP_LOGON				1									//�㳡��¼

//��¼ģʽ
#define SUB_GP_LOGON_GAMEID			1									//I D ��¼
#define SUB_GP_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define SUB_GP_REGISTER_ACCOUNTS	3									//ע���ʺ�
#define SUB_GP_GUEST_REG			4									//�ο�ע��
#define SUB_GPR_MBREIGSTER 	    	5									//�ֻ���ע��
#define SUB_GP_MOBILE_VERIFICAT		6									//�ֻ���֤����֤
#define SUB_GPR_CANCEL_LOGINING		7									//ȡ����¼

//��¼���
#define SUB_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_GP_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_GP_LOGON_FINISH			102									//��¼���
#define SUB_GP_VALIDATE_MBCARD      103                                 //��¼ʧ��
#define SUB_GP_VALIDATE_PASSPOSTID  104                                 //��¼ʧ��
#define SUB_GP_GUEST_REG_SUCCESS	105									//�ο�ע��ɹ�

//������ʾ
#define SUB_GP_UPDATE_NOTIFY		200									//������ʾ

#define SUB_GP_LOGINING_INFO		201									//��¼�Ŷ���Ϣ

//////////////////////////////////////////////////////////////////////////////////
//

#define MB_VALIDATE_FLAGS           0x01                                //Ч���ܱ�
#define LOW_VER_VALIDATE_FLAGS      0x02                                //Ч��Ͱ汾

//I D ��¼
struct CMD_GP_LogonGameID
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//��¼��Ϣ
	DWORD							dwGameID;							//��Ϸ I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	BYTE							cbValidateFlags;			        //У���ʶ
};

//�ʺŵ�¼
struct CMD_GP_LogonAccounts
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	BYTE							cbValidateFlags;			        //У���ʶ
};

//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ CMD_GP_RegisterAccounts

#define DTP_GP_REG_AGENTID			1									//�Ƽ���ID

//�Ƽ�����Ϣ
struct DTP_GP_AgentInfo
{
	DWORD							dwAgentID;							//�Ƽ���ID
};

//ע���ʺ�
struct CMD_GP_RegisterAccounts
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//��ȫ�ֻ�
	BYTE							cbValidateFlags;			        //У���ʶ
};

//�ο�ע��
struct CMD_GP_Guest_Reg
{
    //ϵͳ��Ϣ
    DWORD							dwPlazaVersion;						//�㳡�汾
    TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
    
    //�������
    TCHAR							szLogonPass[LEN_MD5];				//��¼����
    TCHAR							szInsurePass[LEN_MD5];				//��������
    
    //ע����Ϣ
    WORD							wFaceID;							//ͷ���ʶ
    BYTE							cbGender;							//�û��Ա�
    TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
    TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
    TCHAR							szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
    TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
    TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
    TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//��ȫ�ֻ�
    BYTE							cbValidateFlags;			        //У���ʶ
};

//�ֻ�ע���ʺ�
struct CMD_GP_MBNumRegister
{
	TCHAR							szVerificationCode[LEN_MOBILE_PHONE];//��֤��
	CMD_GP_RegisterAccounts			RegisterAccounts;
};

//�ֻ��Ű���֤����֤�ṹ��
struct CMD_GP_MobileCaptchaVerific
{
	DWORD							dwUserId;
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	 //���ֻ�;
	TCHAR							szVerificationCode[LEN_MOBILE_PHONE];//��֤��
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
};

//CMD_GP_LogonAccounts��CMD_GP_RegisterAccounts��CMD_GP_Guest_Reg��ͬʹ��
#define DTP_GP_USERDATA1 10
#define DTP_GP_USERDATA2 11
#define DTP_GP_USERDATA3 12
#define DTP_GP_USERDATA4 13
#define DTP_GP_USERDATA5 14
#define DTP_GP_USERDATA6 15
#define DTP_GP_USERDATA7 16
#define DTP_GP_USERDATA8 17

#define DTP_GP_PASSWORD 100//�������
#define DTP_GP_LOCALIP 101//����IP
#define DTP_GP_CLIENTVERSION 102//�ͻ��˰汾��
//#define DTP_GP_POSITION 23
//szMachineID�ﱣ������ֻ��Ļ�����
//szPassPortID�ﱣ����������ַ���

#define DTP_GP_CLIENTIDFA 103

struct DTP_GP_Password
{
	WORD wCount;
	char cbData[LEN_PASSWORD];
};

struct DTP_GP_Position
{
	float longitude;//����
	float latitude;//γ��
};

//�ο�ע��ɹ�
struct CMD_GP_GuestRegSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwGroupID;							//���ű�ʶ
	DWORD							dwCustomID;							//�Զ���ʶ
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����

	//�û��ɼ�
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//�û�����

	//�û���Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������

	//������Ϣ
	BYTE                            cbShowServerStatus;                 //��ʾ������״̬
};

//��¼�ɹ�
struct CMD_GP_LogonSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwGroupID;							//���ű�ʶ
	DWORD							dwCustomID;							//�Զ���ʶ
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����

	//�û��ɼ�
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//�û�����

	//�û���Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������

	//������Ϣ
	BYTE                            cbShowServerStatus;                 //��ʾ������״̬
};

//��¼ʧ��
struct CMD_GP_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��½���
struct CMD_GP_LogonFinish
{
	WORD							wIntermitTime;						//�ж�ʱ��
	WORD							wOnLineCountTime;					//����ʱ��
};

//��¼ʧ��
struct CMD_GP_ValidateMBCard
{
	UINT								uMBCardID;						//��������
};

//������ʾ
struct CMD_GP_UpdateNotify
{
	BYTE							cbMustUpdate;						//ǿ������
	BYTE							cbAdviceUpdate;						//��������
	DWORD							dwCurrentVersion;					//��ǰ�汾
};

//��¼�Ŷ���Ϣ
struct CMD_GP_LoginingInfo
{
	WORD							wLoginingNum;						//���ڵ�¼������
};

//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ CMD_GP_LogonSuccess

#define DTP_GP_GROUP_INFO			1									//������Ϣ
#define DTP_GP_MEMBER_INFO			2									//��Ա��Ϣ
#define	DTP_GP_UNDER_WRITE			3									//����ǩ��
#define DTP_GP_STATION_URL			4									//��ҳ��Ϣ

#define DTP_GP_SPECIALNODEIP		20//����ڵ㣬����Ϊ�ַ���
#define DTP_GP_SPECIALNODEURL		21//����ڵ㣬����Ϊ�ַ���
#define DTP_GP_IOSCHARGEHIDE		22//ƻ���棬��ƻ���̵꣬���س�ֵ
#define DTP_GP_IOSPASSED			23
#define DTP_GP_MEMBER_ORDER			24
#define DTP_GP_GIFT_TICKET			25//��ȯ
#define DTP_GP_SHOWFAKETABLE        26
#define DTP_GP_VIPINFO				27//��ҵ�VIP��Ϣ
#define DTP_GP_CUSTOMERSERVICEQQ	28//�ͷ�QQ
#define DTP_GP_LOGONGIFT			29//��½������ÿ�������ȡһ��

#define DTP_GP_SERVER_INFO			30//��Ϸ������Ϣ
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
	BYTE cbVIPLevel;//VIP�ȼ�
	DWORD dwVIPExp;//VIP����
	DWORD dwExpPerCent;//һ��Ǯ��Ӧ�ľ���ֵ
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

//������Ϣ
struct DTP_GP_GroupInfo
{
	DWORD							dwGroupID;							//��������
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
};

//��Ա��Ϣ
struct DTP_GP_MemberInfo
{
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��
};

//��ֵ��ַ
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
//ǩ����Ϣ��ѯ
struct CMD_GP_QuerySigninInfo
{
	DWORD dwUserID;
};

//ǩ����ѯÿ����Ϣ
struct tagSignInDayInfo
{
	BYTE cbType;//1��ҡ�2��ȯ��3���ѡ�4�������顢5�м����顢6�߼����顢7���١�8ɢ�䡢9����
	BYTE cbAttrib;//0bit��ͼ���֡�1bit��ǩ����, 2bit �����Ƿ�ǩ��
	int nCount;
};

//ǩ����ѯ���
struct CMD_GP_UserSigninInfo
{
	INT nNum1;
	INT nNum2;
	tagSignInDayInfo normal[7];
	tagSignInDayInfo vip[7];
};

//���ǩ������
struct CMD_GP_SignIn
{
	DWORD							dwUserID;							//�û�ID
	BYTE							cbVip;								//�Ƿ���Vip�û�, 0 ����ͨ�û�ǩ��, 1 ��Vipǩ��
};

//���ǩ�����
struct CMD_GP_SignIn_Result
{
	bool bSuccess;
	BYTE cbUserStatus;
	SCORE llScoreCount;
	SCORE llLotteryCount;
	TCHAR szDescription[128];
};

//�齱��ѯ
struct CMD_GP_QryLtryInfo
{
	DWORD							dwUserID;							//�û�ID
};

//��ѯ���
struct CMD_GP_QryLtryInfo_Res
{
	WORD							wFreeCount;							//��Ѵ���
	TCHAR							szErrorDescribe[128];				//�������
};

//�齱����
struct CMD_GP_UserLtry
{
	DWORD							dwUserID;							//�û�ID
};

//�齱���
struct CMD_GP_UserLtry_Res
{
	WORD							wIsLtrySuc;							//�齱�ɹ�
	WORD							wPrizeID;							//�н��ȼ�
	WORD							wFreeCount;							//��Ѵ���
	SCORE                           llUserScore;                        //��ҵ�ǰʣ����
	TCHAR							szErrorDescribe[128];				//�������
};

//�齱��Ϣ
struct CMD_GP_LTRY_MSG
{
	TCHAR							szNickName[128];					//�û�����
	WORD							wPrizeID;							//�н�ID
};

//��ֵ��ѯ
struct CMD_GP_RechargeInfo
{
	DWORD							dwUserID;							//�û�ID
};

//��ֵ��Ϣ���
struct CMD_GP_RechargeInfo_Res
{
	BYTE							cbIsRecharge;
	TCHAR							szDescribeString[128];				//������Ϣ
};


//////////////////////////////////////////////////////////////////////////////////
//�б�����

#define MDM_GP_SERVER_LIST			2									//�б���Ϣ

//��ȡ����
#define SUB_GP_GET_LIST				1									//��ȡ�б�
#define SUB_GP_GET_SERVER			2									//��ȡ����
#define SUB_GP_GET_ONLINE			3									//��ȡ����
#define SUB_GP_GET_COLLECTION		4									//��ȡ�ղ�

//�б���Ϣ
#define SUB_GP_LIST_TYPE			100									//�����б�
#define SUB_GP_LIST_KIND			101									//�����б�
#define SUB_GP_LIST_NODE			102									//�ڵ��б�
#define SUB_GP_LIST_PAGE			103									//�����б�
#define SUB_GP_LIST_SERVER			104									//�����б�
#define SUB_GP_VIDEO_OPTION			105									//��Ƶ����
#define SUB_GP_LIST_SMALL_SERVER	106									//�����б�Ľ�

//�����Ϣ
#define SUB_GP_LIST_FINISH			200									//�������
#define SUB_GP_SERVER_FINISH		201									//�������

//������Ϣ
#define SUB_GR_KINE_ONLINE			300									//��������
#define SUB_GR_SERVER_ONLINE		301									//��������

//////////////////////////////////////////////////////////////////////////////////

//��ȡ����
struct CMD_GP_GetOnline
{
	WORD							wServerCount;						//������Ŀ
	WORD							wOnLineServerID[MAX_SERVER];		//�����ʶ
};

//��������
struct CMD_GP_KindOnline
{
	WORD							wKindCount;							//������Ŀ
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//��������
};

//��������
struct CMD_GP_ServerOnline
{
	WORD							wServerCount;						//������Ŀ
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//��������
};

//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GP_USER_SERVICE			3									//�û�����

//�˺ŷ���
#define SUB_GP_MODIFY_MACHINE		100									//�޸Ļ���
#define SUB_GP_MODIFY_LOGON_PASS	101									//�޸�����
#define SUB_GP_MODIFY_INSURE_PASS	102									//�޸�����
#define SUB_GP_MODIFY_UNDER_WRITE	103									//�޸�ǩ��

//�޸�ͷ��
#define SUB_GP_USER_FACE_INFO		200									//ͷ����Ϣ
#define SUB_GP_SYSTEM_FACE_INFO		201									//ϵͳͷ��
#define SUB_GP_CUSTOM_FACE_INFO		202									//�Զ�ͷ��

//��������
#define SUB_GP_USER_INDIVIDUAL		301									//��������
#define	SUB_GP_QUERY_INDIVIDUAL		302									//��ѯ��Ϣ
#define SUB_GP_MODIFY_INDIVIDUAL	303									//�޸�����

//���з���
#define SUB_GP_USER_SAVE_SCORE		400									//������
#define SUB_GP_USER_TAKE_SCORE		401									//ȡ�����
#define SUB_GP_USER_TRANSFER_SCORE	402									//ת�˲���
#define SUB_GP_USER_INSURE_INFO		403									//��������
#define SUB_GP_QUERY_INSURE_INFO	404									//��ѯ����
#define SUB_GP_USER_INSURE_SUCCESS	405									//���гɹ�
#define SUB_GP_USER_INSURE_FAILURE	406									//����ʧ��
#define SUB_GP_QUERY_USER_INFO_REQUEST		407							//��ѯ�û�
#define SUB_GP_QUERY_USER_INFO_RESULT		408							//�û���Ϣ
#define SUB_GP_USER_TRANSFER_INFO			409
#define SUB_GP_USER_CHECK_PASSWORD			410
#define SUB_GP_USER_CHECK_PASSWORD_RETURN	411


//�û��齱
#define SUB_GP_QUERY_LOTTERY_INFO			416							//��ѯ�齱
#define SUB_GP_QUERY_LOTTERY_INFO_RESULT	417							//��ѯ���
#define SUB_GP_USER_LOTTERY					418							//�û��齱
#define SUB_GP_USER_LOTTERY_RESULT			419							//�û��齱
#define SUB_GP_LOTTERY_FINISH				420							//�齱����
#define SUB_GP_LOTTERY_MSG					421							//�齱��Ϣ
#define SUB_GP_QUERY_RECHARGE_INFO			422							//��ֵ��Ϣ
#define SUB_GP_QUERY_RECHARGE_INFO_RES		423							//��ֵ��Ϣ

#define SUB_GPR_USER_TRANSER_DRAGON			425
#define SUB_GPO_USER_TRANSER_DRAGON			426

//��֤������
#define SUB_GPR_MBCODE_VERIFY				427							//�ֻ�����֤
#define SUB_GPO_MBCODE_VERIFY				428							//�ֻ�����֤

//�ֻ��Ű���֤������
#define SUB_GPR_MOBILE_BIND					429							//�ֻ��Ű���֤������
#define SUB_GPR_MOBILE_BIND_VERIFY			430							//�ֻ��Ű���֤����֤
#define SUB_GPO_MOBILE_VERIFY_RES			431							//�ֻ��Ű���֤����֤���

//��ѯ�Ƿ�󶨹��ֻ���
#define SUB_GPR_CHECK_MOBILE_EXIT			432							//��ѯ�Ƿ�󶨹��ֻ���
#define SUB_GPR_CHECK_MOBILE_EXIT_RES		433							//��ѯ�Ƿ�󶨹��ֻ��Ž��
//��������
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

//ʹ������
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
#define SUB_GPR_QUERY_MAIL_INFO				502							//��ȡ�ʼ���Ϣ
#define SUB_GPO_QUERY_MAIL_INFO				503
#define SUB_GPR_QUERY_MAIL_CONTENT			504
#define SUB_GPO_QUERY_MAIL_CONTENT			505
#define SUB_GPR_QUERY_MAIL_ITEM				506							//������ȡ��Ʒ
#define SUB_GPO_QUERY_MAIL_ITEM				507							//�����ַ���

#define SUB_GPR_IAP_PRODUCT_LIST			600							//in app purchase
#define SUB_GPO_IAP_PRODUCT_LIST			601
#define SUB_GPR_IAP_TRANSACTIONRECEIPT		602

#define SUB_GPR_CHARGE_VALUE				603							//�����Ƿ��г�ֵ
#define SUB_GPO_CHARGE_VALUE				604							//�����Ƿ��г�ֵ
#define SUB_GPR_CHARGE_CANCEL				605							//ȡ��ȡ����ҵ���ʾ

#define SUB_GPR_VIP_INFO					610
#define SUB_GPR_VIP_EVERYDAY_GIFT			612
#define SUB_GPR_VIP_CHARGE_BACK				614

#define SUB_GPO_VIP_INFO					611
#define SUB_GPO_VIP_RESULT					613

#define SUB_GPR_CURRENT_SCORE				616
#define SUB_GPO_CURRENT_SCORE				617

#define SUB_GPR_LOGON_GIFT					618
#define SUB_GPO_LOGON_GIFT					619

#define SUB_GPR_CHARGE1ST_REQUEST			620//�׳�ɹ�
#define SUB_GPO_CHARGE1ST_REQUEST			621

#define SUB_GPR_CHARGE1ST_GET				622//��ȡ�׳�����Ľ���
#define SUB_GPO_CHARGE1ST_GET				623

#define SUB_GP_WEALTH_BAG_TIME				625//����ʱ��
#define SUB_GPR_WEALTH_BAG_ENROLL			626//����
#define SUB_GPO_WEALTH_BAG_ENROLL			627//�������
#define SUB_GP_WEALTH_BAG_BEGIN				628//��Ϸ��ʼ
#define SUB_GP_WEALTH_BAG_END				629//��Ϸ����
#define SUB_GP_WEALTH_BAG_SCORE				630//�ͻ��˷��ؽ��
#define SUB_GP_WEALTH_BAG_RESULT			631//����˷��ؽ��
#define SUB_GP_WEALTH_BAG_BEFORE			632//���ж೤ʱ��ſ�ʼ

#define SUB_GPR_SHARE_INFO					633
#define SUB_GPO_SHARE_INFO					634

//�㲥�ٶ�
#define SUB_GPR_BROADCAST_SPEED				635							//����㲥�ٶ�
#define SUB_GPO_BROADCAST_SPEED_RES			636							//����㲥�ٶȽ��

//����type = 113����Ʒ����
#define SUB_GPR_TYPE_ITEM_COUNT				637							//����type���͵���Ʒ����
#define SUB_GPO_TYPE_ITEM_COUNT_RES			638							//����type���͵���Ʒ�������

#define SUB_GPR_MOBILE_CHARGE               639                         //�һ�����
#define SUB_GPO_MOBILE_CHARGE               640

#define SUB_GPR_GET_BINDREWARD_NEWGUID      643                         //��ð��ֻ�����������������Ϣ
#define SUB_GPO_GET_BINDREWARD_NEWGUID      644

#define SUB_GPR_SET_SUPER_NEWGUID		    645							//���ó���������������

//�û�ǩ��
#define SUB_GP_QUERY_SIGNIN_INFO			646							//��ѯǩ��
#define SUB_GP_QUERY_SIGNIN_INFO_RESULT		647							//ǩ����Ϣ
#define SUB_GP_USER_SIGNIN_REQ				648							//���ǩ��
#define SUB_GP_USER_SIGNIN_RESULT			649							//ǩ�����

#define SUB_GP_QUERY_FREE_LABA_COUNT		650							//��ѯ������ȴ���
#define SUB_GP_QUERY_FREE_LABA_RESULT		651							//��ѯ������ȴ������

//#define SUB_GR_MATCH_GET_MATCH_PRIZE		652							//�û���ȡ����
//#define SUB_GR_MATCH_GET_MATCH_PRIZE_RES	653							//�û���ȡ�������

#define SUB_GP_BUY_SKILL		652										//��Ҷһ�����
#define SUB_GP_BUY_SKILL_RESULT		653									//��Ҷһ����ܽ��

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

struct CMD_GPR_VIPUser//610��612��614����
{
	DWORD dwUserID;
};

struct CMD_GPO_VIPInfo
{
	BYTE cbVIPLevel;//VIP�ȼ�
	BYTE cbReserved[3];//����
	DWORD dwVIPExp;//VIP����
	DWORD dwVIPNextExp;//��������
	DWORD dwEverydayGift;//ÿ����ȡ��ң����Ϊ0��˵��������ȡ�������Ѿ���ȡ
	DWORD dwChargeBack;//��ֵ���ֽ�ң����Ϊ0��˵����ǰû�з���
};

struct CMD_GPO_VIPResult
{
	WORD wSize;
	char szDescription[1024];
};

struct CMD_GPR_IAPProduct
{
	WORD wRMB;//�۸�
	WORD wType;//0��ң�1��̨
	char szProductID[25];//��ƷID
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
	int nItemCount;							//��������
};

struct MAIL_CONTENT_STRING
{
	BYTE bySize;							//���ݴ�С
};

//�������
#define SUB_GP_OPERATE_SUCCESS		900									//�����ɹ�
#define SUB_GP_OPERATE_FAILURE		901									//����ʧ��

//////////////////////////////////////////////////////////////////////////////////

//�޸�����
struct CMD_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�����
struct CMD_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�ǩ��
struct CMD_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
};

//////////////////////////////////////////////////////////////////////////////////

//�û�ͷ��
struct CMD_GP_UserFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwCustomID;							//�Զ���ʶ
};

//�޸�ͷ��
struct CMD_GP_SystemFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�ͷ��
struct CMD_GP_CustomFaceInfo
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//ͼƬ��Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

//�󶨻���
struct CMD_GP_ModifyMachine
{
	BYTE							cbBind;								//�󶨱�־
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//////////////////////////////////////////////////////////////////////////////////

//��������
struct CMD_GP_UserIndividual
{
	DWORD							dwUserID;							//�û� I D
};

//��ѯ��Ϣ
struct CMD_GP_QueryIndividual
{
	DWORD							dwUserID;							//�û� I D
};

//�޸�����
struct CMD_GP_ModifyIndividual
{
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};

//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ CMD_GP_UserIndividual

#define DTP_GP_UI_NICKNAME			1									//�û��ǳ�
#define DTP_GP_UI_USER_NOTE			2									//�û�˵��
#define DTP_GP_UI_UNDER_WRITE		3									//����ǩ��
#define DTP_GP_UI_QQ				4									//Q Q ����
#define DTP_GP_UI_EMAIL				5									//�����ʼ�
#define DTP_GP_UI_SEAT_PHONE		6									//�̶��绰
#define DTP_GP_UI_MOBILE_PHONE		7									//�ƶ��绰
#define DTP_GP_UI_COMPELLATION		8									//��ʵ����
#define DTP_GP_UI_DWELLING_PLACE	9									//��ϵ��ַ

//////////////////////////////////////////////////////////////////////////////////

//��������
struct CMD_GP_UserInsureInfo
{
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	SCORE							lTransferPrerequisite;				//ת������
};

//������
struct CMD_GP_UserSaveScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ȡ���
struct CMD_GP_UserTakeScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ���
	TCHAR							szPassword[LEN_MD5];				//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת�˽��
struct CMD_GP_UserTransferScore
{
	DWORD							dwUserID;							//�û� I D
	BYTE                            cbByNickName;                       //�ǳ�����
	SCORE							lTransferScore;						//ת�˽��
	TCHAR							szPassword[LEN_MD5];				//��������
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//���гɹ�
struct CMD_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//������Ϣ
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

//����ʧ��
struct CMD_GP_UserInsureFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��ȡ���
struct CMD_GP_UserTakeResult
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
};

//��ѯ����
struct CMD_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//�û� I D
};

//��ѯ�û�
struct CMD_GP_QueryUserInfoRequest
{
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
};

//�û���Ϣ
struct CMD_GP_UserTransferUserInfo
{
	DWORD							dwTargetGameID;						//Ŀ���û�
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
};

//������֤
struct CMD_GP_CheckPassword
{
	DWORD							dwUserID;
	TCHAR							szPassword[LEN_PASSWORD];
};

//������֤���
struct CMD_GP_CheckPasswordReturn
{
	bool							bPasswordRight;
};
//////////////////////////////////////////////////////////////////////////////////

//����ʧ��
struct CMD_GP_OperateFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�����ɹ�
struct CMD_GP_OperateSuccess
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
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
//Զ�̷���

#define MDM_GP_REMOTE_SERVICE		4									//Զ�̷���

//���ҷ���
#define SUB_GP_C_SEARCH_DATABASE	100									//���ݲ���
#define SUB_GP_C_SEARCH_CORRESPOND	101									//Э������

//���ҷ���
#define SUB_GP_S_SEARCH_DATABASE	200									//���ݲ���
#define SUB_GP_S_SEARCH_CORRESPOND	201									//Э������

//////////////////////////////////////////////////////////////////////////////////

//Э������
struct CMD_GP_C_SearchCorrespond
{
	DWORD							dwGameID;							//��Ϸ��ʶ
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//Э������
struct CMD_GP_S_SearchCorrespond
{
	WORD							wUserCount;							//�û���Ŀ
	tagUserRemoteInfo				UserRemoteInfo[16];					//�û���Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//��¼����
#define MDM_MB_LOGON				100									//�㳡��¼

//��¼ģʽ
#define SUB_MB_LOGON_GAMEID			1									//I D ��¼
#define SUB_MB_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define SUB_MB_REGISTER_ACCOUNTS	3									//ע���ʺ�

//��¼���
#define SUB_MB_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_MB_LOGON_FAILURE		101									//��¼ʧ��

//������ʾ
#define SUB_MB_UPDATE_NOTIFY		200									//������ʾ

//////////////////////////////////////////////////////////////////////////////////

//I D ��¼
struct CMD_MB_LogonGameID
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//��¼��Ϣ
	DWORD							dwGameID;							//��Ϸ I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//�ʺŵ�¼
struct CMD_MB_LogonAccounts
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//ע���ʺ�
struct CMD_MB_RegisterAccounts
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//��¼�ɹ�
struct CMD_MB_LogonSuccess
{
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//��¼ʧ��
struct CMD_MB_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//������ʾ
struct CMD_MB_UpdateNotify
{
	BYTE							cbMustUpdate;						//ǿ������
	BYTE							cbAdviceUpdate;						//��������
	DWORD							dwCurrentVersion;					//��ǰ�汾
};

//////////////////////////////////////////////////////////////////////////////////
//�б�����

#define MDM_MB_SERVER_LIST			101									//�б���Ϣ

//�б���Ϣ
#define SUB_MB_LIST_KIND			100									//�����б�
#define SUB_MB_LIST_SERVER			101									//�����б�
#define SUB_MB_LIST_FINISH			200									//�б����

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

//VIP����
struct CMD_GP_VIPUpgrade
{
	BYTE							cbVIPLevel;
	TCHAR							szNickName[LEN_NICKNAME];
};

#define SUB_GPR_ChangeItem 702

//�һ�����
struct CMD_GP_ChangeItem
{
	DWORD							dwLotteryCount;
	TCHAR							szNickName[LEN_NICKNAME];
	TCHAR							szItemName[LEN_NICKNAME];
};

#define SUB_GP_SystemBroadcast 703

#define SUB_GP_SystemBroadLaBa 704

#define MDM_GP_ITEM 1000

//��ѯ��������
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
	int nCannonStatus;//��̨״̬
	int nJiaSu;//����
	int nSanShe;//ɢ��
	int nBaoJi;//����
};

//���͵���
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
	int nItemID;//��������ID
	int nSourceCount;//ԭ��������
	int nVariationCount;//�ı������
	int nResultCode;//������
	TCHAR szDescribeString[128];
};

//�ϳɵ���
#define SUB_GPR_COMBINE_ITEM 3
#define SUB_GPO_COMBINE_ITEM 1003
//GSP_ItemCombine

struct CMD_GPR_CombineItem
{
	DWORD dwUserID;
	int nCombineType;//1=�ϳ�С���飬2=�ϳɴ�����
};

struct CMD_GPO_CombineItem
{
	int nDragonPieceCount;//������Ƭ����
	int nSmallDragonCount;//С��������
	int nBigDragonCount;//����������
	int nResultCode;//������
	TCHAR szDescribeString[128];
};

//ʹ���������
#define SUB_GPR_USE_DRAGON 4
#define SUB_GPO_USE_DRAGON 1004
//GSP_ItemUseDragon

struct CMD_GPR_UseDragon
{
	DWORD dwUserID;
	int nDragonType;//0�������飬1�м����飬2�߼����飬3��������
};

struct CMD_GPO_UseDragon
{
	SCORE lCurrentScore;//��ǰ����
	int nChuJiCount;
	int nZhongJiCount;
	int nGaoJiCount;
	int nZhiZunCount;
	int nResultCode;//������
	TCHAR szDescribeString[128];
};

//װ������
#define SUB_GPR_EQUIPMENT_DRAGON 5
#define SUB_GPO_EQUIPMENT_DRAGON 1005
//GSP_ItemEquipmentDragon

struct CMD_GPR_EquipmentDragon
{
	DWORD dwUserID;
	int nDragonType;//0�������飬1�м����飬2�߼����飬3��������
};

struct CMD_GPO_EquipmentDragon
{
	int nEquipmentDragonCount;//װ���Ľ��
	int nChuJiCount;
	int nZhongJiCount;
	int nGaoJiCount;
	int nZhiZunCount;
	int nResultCode;//������
	TCHAR szDescribeString[128];
};

//���߶һ�
#define SUB_GPR_EXCHANGE_LOTTERY 6
#define SUB_GPO_EXCHANGE_LOTTERY 1006
struct CMD_GPR_ExchangeLottery
{
	DWORD dwUserID;
	int nDragonType;//0�������飬1�м����飬2�߼����飬3��������
};

struct CMD_GPO_ExchangeLottery
{
	int nLotteryCount;//��ǰ��ȯ�������һ���
	int nChuJiCount;
	int nZhongJiCount;
	int nGaoJiCount;
	int nZhiZunCount;
	int nResultCode;//������
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
	int nCannonStatus;//��̨״̬
	int nJiaSu;//����
	int nSanShe;//ɢ��
	int nBaoJi;//����
	BYTE cbCount[3];//����
	BYTE cbPrice[7];//�۸�
};

//ע���ʺ���������֤��
struct CMD_GPR_MBVerificationCode
{
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ���
};

//���ֻ�����֤������
struct CMD_GPR_MB_BIND_REQUEST
{
	DWORD							dwUserId;							//�û�id
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ���
};

struct CMD_GPO_MBVerificationCode
{
	bool bIsVerificationCodeSend;
	TCHAR szDescribeString[128];				//������Ϣ
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

//�ֻ��Ű󶨳ɹ�
struct CMD_MB_NumBindSuccess
{
	int								nCurrentGold;
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�ֻ��Ű�ʧ��
struct CMD_MB_NumBindFailure
{
	TCHAR							szDescribeString[128];				//������Ϣ
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

struct CMD_GPR_MatchGetMatchPrise                       //�û���ȡ����
{
	int nMatchID;
	DWORD dwUserID;
	char szMatchData[16];
};

struct CMD_GPO_MatchGetMatchPriseRes                    //�û���ȡ�������
{
	bool bPriseStatus;//��ȡ״̬��0Ϊ��ȡʧ�ܣ�1Ϊ��ȡ�ɹ�
	BYTE nPriseType;//������������
	int nPriseCount;//��ȡ����
	SCORE llUserScore;//��ҵ�ǰ�������
	char szDescription[128];
	int nMatchId;
};


#pragma pack()

#endif