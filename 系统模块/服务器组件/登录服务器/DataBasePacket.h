#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//�������ݰ�

//��¼����
#define	DBR_GP_LOGON_GAMEID			1									//I D ��¼
#define	DBR_GP_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define DBR_GP_REGISTER_ACCOUNTS	3									//ע���ʺ�
#define DBR_GP_GUEST_REGISTER		4									//�ο͵�¼
#define DBR_GP_RECORD_LOGON			5

//�˺ŷ���
#define DBR_GP_MODIFY_MACHINE		10									//�޸Ļ���
#define DBR_GP_MODIFY_LOGON_PASS	11									//�޸�����
#define DBR_GP_MODIFY_INSURE_PASS	12									//�޸�����
#define DBR_GP_MODIFY_UNDER_WRITE	13									//�޸�ǩ��
#define DBR_GP_MODIFY_INDIVIDUAL	14									//�޸�����

//ͷ������
#define DBR_GP_MODIFY_SYSTEM_FACE	20									//�޸�ͷ��
#define DBR_GP_MODIFY_CUSTOM_FACE	21									//�޸�ͷ��

//��������
#define DBR_GP_USER_SAVE_SCORE		30									//������Ϸ��
#define DBR_GP_USER_TAKE_SCORE		31									//��ȡ��Ϸ��
#define DBR_GP_USER_TRANSFER_SCORE	32									//ת����Ϸ��
#define DBR_GP_USER_CHECK_PASSWORD	33

//��ѯ����
#define DBR_GP_QUERY_INDIVIDUAL		40									//��ѯ����
#define DBR_GP_QUERY_INSURE_INFO	41									//��ѯ����
#define DBR_GP_QUERY_USER_INFO	    42									//��ѯ�û�

//ϵͳ����
#define DBR_GP_LOAD_GAME_LIST		50									//�����б�
#define DBR_GP_ONLINE_COUNT_INFO	51									//������Ϣ

//ǩ������
#define DBR_GP_QUERY_SIGIN_INFO		61									//��ѯǩ��
#define DBR_GP_USER_SIGIN			62									//���ǩ��

//�齱����
#define DBR_GP_QUERY_LOTTERY_INFO	63									//�齱��ѯ
#define DBR_GP_LOTTERY_REQ			64									//�齱����


//////////////////////////////////////////////////////////////////////////////////

//ID ��¼
struct DBR_GP_LogonGameID
{
	//��¼��Ϣ
	DWORD							dwGameID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	BYTE							cbNeeValidateMBCard;				//�ܱ�У��

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺŵ�¼
struct DBR_GP_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	BYTE							cbNeeValidateMBCard;				//�ܱ�У��

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺ�ע��
struct DBR_GP_RegisterAccounts
{
	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwAgentID;							//�Ƽ���ID
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//��ȫ�ֻ�

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����(MD5����)
	TCHAR							szInsurePass[LEN_MD5];				//��������
	TCHAR							szGuestPass[LEN_PASSWORD];			//��¼����(δ����)

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szIDFA[40];

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
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

//�޸Ļ���
struct DBR_GP_ModifyMachine
{
	BYTE							cbBind;								//�󶨱�־
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�����
struct DBR_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�����
struct DBR_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�ǩ��
struct DBR_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
};

//�޸�ͷ��
struct DBR_GP_ModifySystemFace
{
	//�û���Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�ͷ��
struct DBR_GP_ModifyCustomFace
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//ͼƬ��Ϣ

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�����
struct DBR_GP_ModifyIndividual
{
	//��֤����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����

	//�ʺ�����
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//�û���Ϣ
	TCHAR							szUserNote[LEN_USER_NOTE];			//�û�˵��
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//�绰����
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//�̶��绰
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ƶ��绰

	//��ϵ����
	TCHAR							szQQ[LEN_QQ];						//Q Q ����
	TCHAR							szEMail[LEN_EMAIL];					//�����ʼ�
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//��ϵ��ַ
};

//��ѯ����
struct DBR_GP_QueryIndividual
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//������Ϸ��
struct DBR_GP_UserSaveScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������Ϸ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ȡ����Ϸ��
struct DBR_GP_UserTakeScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ��Ϸ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת����Ϸ��
struct DBR_GP_UserTransferScore
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	BYTE                            cbByNickName;                       //�ǳ�����
	SCORE							lTransferScore;						//ת����Ϸ��
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ѯ����
struct DBR_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//��ѯ�û�
struct DBR_GP_QueryInsureUserInfo
{
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
};

//�û�����
struct DBO_GP_UserTransferUserInfo
{
	DWORD							dwGameID;							//�û� I D
	TCHAR							szNickName[LEN_NICKNAME];			//�ʺ��ǳ�
};

//������Ϣ
struct DBR_GP_OnLineCountInfo
{
	WORD							wKindCount;							//������Ŀ
	DWORD							dwOnLineCountSum;					//��������
	tagOnLineInfoKind				OnLineCountKind[MAX_KIND];			//��������
};

//ǩ����Ϣ��ѯ
struct DBR_GP_QuerySignInInfo
{
	DWORD dwUserID;
};

//���ǩ��
struct DBR_GP_UserSignIn
{
	DWORD dwUserID;
	BYTE  cbVip;								//�Ƿ���Vip�û�, 0 ����ͨ�û�ǩ��, 1 ��Vipǩ��
};

//�齱��Ϣ��ѯ
struct DBR_GP_QryLtryInfo
{
	DWORD dwUserID;
};

//�û��齱����
struct DBR_GP_UserLtry
{
	DWORD dwUserID;
};


//////////////////////////////////////////////////////////////////////////////////
//�����Ϣ

//��¼���
#define DBO_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define DBO_GP_LOGON_FAILURE		101									//��¼ʧ��
#define DBO_GP_VALIDATE_MBCARD		102									//��¼ʧ��
#define DBO_GP_VALIDATE_PASSPOSTID	103									//��¼ʧ��
#define DBO_GP_GUEST_LOGON_SUCCESS	104									//�ο͵�¼

//�������
#define DBO_GP_USER_FACE_INFO		110									//�û�ͷ��
#define DBO_GP_USER_INDIVIDUAL		111									//�û�����

//��������
#define DBO_GP_USER_INSURE_INFO		120									//��������
#define DBO_GP_USER_INSURE_SUCCESS	121									//���гɹ�
#define DBO_GP_USER_INSURE_FAILURE	122									//����ʧ��
#define DBO_GP_USER_INSURE_USER_INFO   123								//�û�����
#define DBO_GP_USER_INSURE_TRANSFER_INFO	124
#define DBO_GP_USER_CHECK_PASSWORD	125

//�б���
#define DBO_GP_GAME_TYPE_ITEM			130									//������Ϣ
#define DBO_GP_GAME_KIND_ITEM			131									//������Ϣ
#define DBO_GP_GAME_NODE_ITEM			132									//�ڵ���Ϣ
#define DBO_GP_GAME_PAGE_ITEM			133									//������Ϣ
#define DBO_GP_GAME_LIST_RESULT			134									//���ؽ��

//ǩ�����
#define DBO_GP_SIGNIN_QUERY_SUCCESS		201									//��ѯ�ɹ�
#define DBO_GP_SIGNIN_QUERY_FAILURE		202									//��ѯʧ��
#define DBO_GP_USER_SIGNIN_SUCCESS		203									//��ѯ�ɹ�
#define DBO_GP_USER_SIGNIN_FAILURE		204									//��ѯʧ��

//�齱���
#define DBO_GP_LOTTERY_QUERY_RESULT		205									//�齱�ɹ�
#define DBO_GP_USER_LOTTERY_RESULT		206									//�齱�ɹ�


//������
#define DBO_GP_OPERATE_SUCCESS			500									//�����ɹ�
#define DBO_GP_OPERATE_FAILURE			501									//����ʧ��

//��¼�ɹ�
struct DBO_GP_LogonSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwGameID;							//��Ϸ��ʶ
	DWORD							dwGroupID;							//���ű�ʶ
	DWORD							dwCustomID;							//�Զ�����
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������

	//�û��ɼ�
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;						//�û�����

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//��Ա����
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��

	//������Ϣ
	TCHAR							szDescribeString[128];				//������Ϣ
	int								nTotalCharge;
	int								nTotalPlayTime;

	BYTE							cbVIPLevel;
	DWORD							dwVIPExp;//VIP����
	DWORD							dwExpPerCent;//һ��Ǯ��Ӧ�ľ���ֵ
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

//��¼ʧ��
struct DBO_GP_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
	bool							bIsLogin;
};

//��¼ʧ��
struct DBR_GP_ValidateMBCard
{
	UINT							uMBCardID;						//��������
};

//ͷ����Ϣ
struct DBO_GP_UserFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwCustomID;							//�Զ�����
};

//��������
struct DBO_GP_UserIndividual
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szUserNote[LEN_USER_NOTE];			//�û�˵��
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//�绰����
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//�̶��绰
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ƶ��绰

	//��ϵ����
	TCHAR							szQQ[LEN_QQ];						//Q Q ����
	TCHAR							szEMail[LEN_EMAIL];					//�����ʼ�
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//��ϵ��ַ
};

//��������
struct DBO_GP_UserInsureInfo
{
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;						//������Ϸ��
	SCORE							lTransferPrerequisite;				//ת������
};

//���гɹ�
struct DBO_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSourceScore;						//ԭ����Ϸ��
	SCORE							lSourceInsure;						//ԭ����Ϸ��
	SCORE							lInsureRevenue;						//����˰��
	SCORE							lVariationScore;					//��Ϸ�ұ仯
	SCORE							lVariationInsure;					//���б仯
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct  DBO_GP_UserInsureFailure
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct DBO_GP_OperateFailure
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�����ɹ�
struct DBO_GP_OperateSuccess
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
struct DBO_GP_GameType
{
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wTypeID;							//��������
	TCHAR							szTypeName[LEN_TYPE];				//��������
};

//��Ϸ����
struct DBO_GP_GameKind
{
	WORD							wTypeID;							//��������
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wKindID;							//��������
	WORD							wGameID;							//ģ������
	TCHAR							szKindName[LEN_KIND];				//��Ϸ����
	TCHAR							szProcessName[LEN_PROCESS];			//��������
};

//��Ϸ�ڵ�
struct DBO_GP_GameNode
{
	WORD							wKindID;							//��������
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wNodeID;							//�ڵ�����
	TCHAR							szNodeName[LEN_NODE];				//�ڵ�����
};

//��������
struct DBO_GP_GamePage
{
	WORD							wKindID;							//��������
	WORD							wNodeID;							//�ڵ�����
	WORD							wSortID;							//��������
	WORD							wPageID;							//��������
	WORD							wOperateType;						//��������
	TCHAR							szDisplayName[LEN_PAGE];			//��ʾ����
};

//���ؽ��
struct DBO_GP_GameListResult
{
	BYTE							cbSuccess;							//�ɹ���־
};
//////////////////////////////////////////////////////////////////////////
//ǩ����Ϣ
struct DBO_GP_UserSignInInfo
{
	INT nNum1;
	INT nNum2;
	tagSignInDayInfo normal[7];
	tagSignInDayInfo vip[7];
};

//���ǩ�����
struct DBO_GP_UserSignInResult
{
	BOOL bSuccess;					//�Ƿ�ɹ�
	BYTE cbUserStatus;				//VIP or Normal
	SCORE llScoreCount;				//�����������
	SCORE llLotteryCount;			//������ȯ����
	TCHAR szDescription[128];		//ǩ���������
};

//��ѯ�齱���
struct DBO_GP_Ltry_Qry_Res
{
	WORD							wFreeCount;
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�û��齱���
struct DBO_GP_User_Ltry_Res
{
	WORD							wIsLtrySuc;							//�齱�Ƿ�ɹ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û�����
	WORD							wPrizeID;							//����ID
	WORD							wFreeCount;							//��Ѵ���
	SCORE                           lUserScore;                        //��ҵ�ǰʣ����
	TCHAR							szDescribeString[128];				//������Ϣ
};
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//�ֻ����ݰ�

//��¼����
#define	DBR_MB_LOGON_GAMEID			500									//I D ��¼
#define	DBR_MB_LOGON_ACCOUNTS		501									//�ʺŵ�¼
#define DBR_MB_REGISTER_ACCOUNTS	502									//ע���ʺ�

//��¼���
#define DBO_MB_LOGON_SUCCESS		600									//��¼�ɹ�
#define DBO_MB_LOGON_FAILURE		601									//��¼ʧ��

//////////////////////////////////////////////////////////////////////////////////

//ID ��¼
struct DBR_MB_LogonGameID
{
	//��¼��Ϣ
	DWORD							dwGameID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺŵ�¼
struct DBR_MB_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺ�ע��
struct DBR_MB_RegisterAccounts
{
	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//��¼�ɹ�
struct DBO_MB_LogonSuccess
{
	//�û�����
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�

	//������Ϣ
	TCHAR							szDescribeString[128];				//������Ϣ
};


//��¼ʧ��
struct DBO_MB_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�ʼ�����
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
	BYTE cbCanGet;//0��������Ʒ��1����ȡ��Ʒ��2����ȡ��Ʒ��������ȡ����Ʒ�����ݽ�������
	char szMailTime[20];//����2015-08-06 17:33:25������19���ַ�����19���ֽ�
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

//��ֵ����
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

//�յ���վ�����ĳ�ֵ�����Ϣ
#define DBR_GP_WEB_CHARGE_VALUE 76

struct DBR_WebChargeValue
{
	DWORD dwUserID;
	SCORE lChargeValue;
};

//VIPϵͳ
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
	BYTE cbVIPLevel;//VIP�ȼ�
	BYTE cbReserved[3];//����
	DWORD dwVIPExp;//VIP����
	DWORD dwVIPNextExp;//��������
	DWORD dwEverydayGift;//ÿ����ȡ��ң����Ϊ0��˵��������ȡ�������Ѿ���ȡ
	DWORD dwChargeBack;//��ֵ���ֽ�ң����Ϊ0��˵����ǰû�з���
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

//��ҵ�ǰ����
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
	BYTE cbVIPLevel;//VIP�ȼ�
	DWORD dwVIPExp;//VIP����
	DWORD dwExpPerCent;//һ��Ǯ��Ӧ�ľ���ֵ
	DWORD dwChargeBack;//��ֵ���ֽ�ң����Ϊ0��˵����ǰû�з���
};

//��¼����
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

//�׳����
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

//��������
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

//ʹ������
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
	int nZhiZunCount;	//�ر�ͼ
	int nCannonStatus;//��̨״̬
	int nJiaSu;//����
	int nSanShe;//ɢ��
	int nBaoJi;//����
};

#define DBR_GP_TRANSFER_ITEM 1002
#define DBO_GP_TRANSFER_ITEM 2002


struct DBR_GP_TransferItem
{
	DWORD dwUserID;
	DWORD dwTargetGameID;
	int nItemID;//��������ID
	int nItemCount;//��������
	TCHAR szPassword[LEN_MD5];
	TCHAR szItemName[32];
};

struct DBO_GP_TransferItem
{
	int nItemID;//��������ID
	int nSourceCount;//ԭ��������
	int nVariationCount;//�ı������
	int nResultCode;//������
	TCHAR szDescribeString[128];
};

#define DBR_GP_COMBINE_DRAGON 1003
#define DBO_GP_COMBINE_DRAGON 2003

struct DBR_GP_CombineItem
{
	DWORD dwUserID;
	int nCombineType;//1=�ϳ�С���飬2=�ϳɴ�����
};

struct DBO_GP_CombineItem
{
	int nDragonPieceCount;//������Ƭ����
	int nSmallDragonCount;//С��������
	int nBigDragonCount;//����������
	int nResultCode;//������
	TCHAR szDescribeString[128];
};

#define DBR_GP_USE_DRAGON 1004
#define DBO_GP_USE_DRAGON 2004

struct DBR_GP_UseDragon
{
	DWORD dwUserID;
	int nDragonType;//0�������飬1�м����飬2�߼����飬3��������
};

struct DBO_GP_UseDragon
{
	SCORE lCurrentScore;//��ǰ����
	int nChuJiCount;
	int nZhongJiCount;
	int nGaoJiCount;
	int nZhiZunCount;
	int nResultCode;//������
	TCHAR szDescribeString[128];
};

#define DBR_GP_EQUIPMENT_DRAGON 1005
#define DBO_GP_EQUIPMENT_DRAGON 2005

struct DBR_GP_EquipmentDragon
{
	DWORD dwUserID;
	int nDragonType;//0�������飬1�м����飬2�߼����飬3��������
};

struct DBO_GP_EquipmentDragon
{
	int nEquipmentDragonCount;//װ���Ľ��
	int nChuJiCount;
	int nZhongJiCount;
	int nGaoJiCount;
	int nZhiZunCount;
	int nResultCode;//������
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
	int nCannonStatus;//��̨״̬
	int nJiaSu;//����
	int nSanShe;//ɢ��
	int nBaoJi;//����
	BYTE cbCount[3];//����
	BYTE cbPrice[7];//�۸�
};


#define DBR_GP_VERIFY_MBNUM			1007									//�ֻ������Ƿ����
#define DBO_GP_VERIFY_MBNUM			2007									//�ֻ������Ƿ����

struct DBR_GP_MBVerifCode
{
	DWORD dwUserId;
	TCHAR szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ���
	TCHAR szVerificationCode[LEN_VERIFICATION_CODE];	//��֤��
};

struct DBO_GP_MBVerifCode
{
	bool bIsMBNumExist;
	TCHAR szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ���
	TCHAR szVerificationCode[LEN_VERIFICATION_CODE];	//��֤��
};


// ���ݿ⴦���ֻ�ע�ᴦ����, ʧ�ܵĽ����������������
#define DBR_GP_MB_REGITER			1008
#define DBO_GP_MB_REGITER_SUCCESS	2008


struct DBO_GP_MBRegisterRes
{
	TCHAR szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ���
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

// ���ݿ⴦���ֻ��󶨵Ľ��
#define DBR_GP_MB_BIND_NUM			1010
#define DBO_GP_MB_BIND_NUM_RES		2010
struct DBR_GP_MBBindNum
{
	DWORD							dwUserID;
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ���
};

struct DBO_GP_MB_Check_Can_Bind
{
	BYTE byResult;							//��ѯ���ݿ���
	TCHAR szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ���
	TCHAR szVerificationCode[LEN_VERIFICATION_CODE];	//��֤��
};

//���ֻ��Ŵ�����
struct DBO_GP_MBBindRes
{
	BYTE							byResult;							//�󶨽�� 0 ʧ�� 1�ɹ�
	int								nCurrentGold;						//����֮��Ľ��ֵ
	TCHAR							szDescribeString[128];				//������Ϣ
};

#define DBR_GP_MB_CHECK_NUM_EXIT	1011
#define DBO_GP_MB_CHECK_NUM_EXIT	2011
//�ֻ����Ƿ��
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
	int nDragonType;//0�������飬1�м����飬2�߼����飬3��������
};

struct DBO_GP_ExchangeLottery
{
	int nLotteryCount;//��ǰ��ȯ�������һ���
	int nChuJiCount;
	int nZhongJiCount;
	int nGaoJiCount;
	int nZhiZunCount;
	int nResultCode;//������
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
//�ֻ����Ƿ��
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
	bool bPriseStatus;//��ȡ״̬��0Ϊ��ȡʧ�ܣ�1Ϊ��ȡ�ɹ�
	BYTE nPriseType;//������������
	int nPriseCount;//��ȡ����
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