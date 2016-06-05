#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
#define KIND_ID						2008								//��Ϸ I D
#define GAME_PLAYER					1							        //��Ϸ����
#define GAME_NAME					TEXT("����С��")					//��Ϸ����
#define GAME_GENRE				   (GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����
#define SCROLL_PICTURE_COUNT		3									//��ʾ����ͼƬ����

//�������
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

//////////////////////////////////////////////////////////////////////////////////
//״̬����
#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_GO				GAME_STATUS_PLAY					//���GO
#define GAME_SCENE_AUTORUN          (GAME_STATUS_PLAY+7)                //�й���Ϸ
#define GAME_SCENE_PICTURES_ROLL    (GAME_STATUS_PLAY+1)                //ͼƬת��
#define GAME_SCENE_CHANGE_BET       (GAME_STATUS_PLAY+2)                //�ı��ע
#define GAME_SCENE_STOP				(GAME_STATUS_PLAY+3)				//���STOP
#define GAME_SCENE_CLOWN_BET        (GAME_STATUS_PLAY+4)                //��С��
#define GAME_SCENE_CHANGE_PRIZE     (GAME_STATUS_PLAY+5)                //�ı��С��ע��
#define GAME_SCENE_END				(GAME_STATUS_PLAY+6)  				//��Ϸ����

//ͼ������
#define SPRITE_YELLOW_DRAGON			    0							//����
#define SPRITE_BLUA_DRAGON					1							//����
#define SPRITE_COLOR_CLOWN					2							//��С��
#define SPRITE_GRAY_CLOWN					3							//��С��
#define SPRITE_BEAUTI_GIRL					4							//��Ů
#define SPRITE_BOX				        	5							//����
#define SPRITE_DIAMOND					    6							//��ʯ
#define SPRITE_SHELL						7							//����
#define SPRITE_COIN					        8							//���
#define SPRITE_SILVER					    9							//����
#define SPRITE_MAX                          10                          //��������

////////////////////////////////////////////////////////////////////////////////////
//����������ṹ
#define SUB_S_GAME_READY                  115                           //׼����ʼ��Ϸ
#define SUB_S_START_GAME                  116                           //��ʼ��Ϸ
#define SUB_S_SEND_RESLUT                 117                           //���ͽ��
#define SUB_S_PICTURES_ROLL               118                           //ͼƬת��
#define SUB_S_CHANGE_BET                  119                           //�ı��ע
#define SUB_S_STOP_SCROLL                 120                           //ֹͣת��
#define SUB_S_CLOWN_BET                   122                           //��עС��
#define SUB_S_CHANGE_PRIZE                123                           //�ı��С��ע��
#define SUB_S_GET_PRIZE                   124                           //����������Ϸ
#define SUB_S_SEND_USEITEM_RESLUT         125                           //ת�����(�õ���)
#define SUB_S_USER_CANT_PLAY			  126                           //��ҷ�������Ҫ���������ʾ

//////////////////////////////////////////////////////////////////////////////////
//׼����Ϸ
struct CMD_S_GAME_READY
{
	WORD			wUserID;							//����κ�
	LONGLONG		lUserScore;							//��ҽ������
	LONGLONG        lGameNeed;                          //��Ϸ������
	LONGLONG        lDefultChip;                        //��ϷĬ��������
	LONGLONG        lFinalPrize;                        //�����
	int             nAvailableTimes;                    //���տ������
	int             nTimes;                             //��С����
	int             nStarCount;                         //�¶�С�����
	bool            bIsSelected;                        //�Ƿ�ѡ��
	bool            bIsRolling;                         //�Ƿ���ת��
	bool            bLackChip;                          //�Ƿ�����ע
	int				nItemNum;							//��ǰ��������
	int				nFiveStarExtralReward;				//���Ƕ��⽱��
	int				nSixStarExtralReward;				//���Ƕ��⽱��
	int				nSevenStarExtralReward;				//���Ƕ��⽱��
	int				nEightStarExtralReward;				//���Ƕ��⽱��
};
//��ʼ��Ϸ
struct CMD_S_GAME_START
{
	LONGLONG        lGameNeed;                          //��Ϸ������
	LONGLONG		lUserScore;							//��ҽ������
	LONGLONG        lDefultChip;                        //��ϷĬ��������
};
//�й���Ϸ
struct CMD_S_AUTORUN_GAME
{
	bool            bIsSelected;                       //�Ƿ�ѡ��
	bool            bIsRolling;                        //�Ƿ���ת��
};
//���ͽ��
struct CMD_S_SEND_RESULT
{
	int			    bResultData[SCROLL_PICTURE_COUNT];	//�������
	int             nStarCount;                         //�¶Դ���
	bool            bPictureIsAllSame;                  //��ʾͼƬ�Ƿ�ȫ��ͬ
	bool            bLackChip;                          //�Ƿ�����ע
	LONGLONG        lUserScore;                         //�û���ǰ����
	LONGLONG        lCurrentChip;                       //��ǰ��ע��
	LONGLONG        lDefultChip;                        //��ϷĬ��������
};
struct CMD_S_USEITEM_SEND_RESULT
{
	int			    bResultData[SCROLL_PICTURE_COUNT];	//�������
	int             nStarCount;                         //�¶Դ���
	bool            bPictureIsAllSame;                  //��ʾͼƬ�Ƿ�ȫ��ͬ
	bool            bLackChip;                          //�Ƿ�����ע
	LONGLONG        lUserScore;                         //�û���ǰ����
	LONGLONG        lCurrentChip;                       //��ǰ��ע��
	LONGLONG        lDefultChip;                        //��ϷĬ��������
	int				nItemNum;							//��ǰ��������
};
//ͼ��ת��
struct CMD_S_PICTURES_ROLL
{
	LONGLONG        lGameNeed;                          //��Ϸ������
	LONGLONG		lUserScore;							//��ҽ������
	LONGLONG	    lDefultChip;					    //Ĭ�����Ľ����
	int             nAvailableTimes;                    //ʣ�������Ϸ����
	bool            bIsRolling;                         //�Ƿ���ת��
};

//�ı��ע
struct CMD_S_CHANGE_BET
{
	LONGLONG         lGameNeed;                          //��ǰ��ע
	LONGLONG		 lUserScore;					   	 //��ҽ������
	LONGLONG		 lDefultChip;					     //Ĭ�����Ľ����
	BYTE			 cbCanChange;						 //�ı���1.�ɹ���2.��ҷ���������3.����С��Ĭ��ֵ��
};
//ֹͣת��
struct CMD_S_STOP_SCROLL
{
	bool            bIsRolling;                         //�Ƿ���ת��
};
//Ӯ��֮��ʼ��ע
struct CMD_S_CLOWN_BET
{
	int              nStarCount;                         //�¶�С������Ǽ�
	int              nClownIndex;                        //С������
	bool             bChooseRight;                       //�Ƿ�¶�С��
	LONGLONG         lChipPrize;                         //��С���ע
	LONGLONG		 lUserScore;					   	 //��ҽ������
	int              nTimes;                             //��С���ע����
	bool             bLackChip;                          //�Ƿ�����ע
};
//�ı��С����ע����
struct CMD_S_CHANGE_PRIZE
{
	LONGLONG         lChipPrize;                        //��С���ע
	LONGLONG		 lUserScore;						//��ҽ������
	int              nTimes;                            //��ע����
	bool             bLackChip;                          //�Ƿ�����ע
};
//����������Ϸ
struct CMD_S_GET_PRIZE
{
	WORD			 wUserID;							//����κ�
	LONGLONG         lChipPrize;                        //��ע����
	LONGLONG         lFinalPrize;                       //���⽱��
	LONGLONG		 lUserScore;						//��ҽ������
	LONGLONG         lDefultChip;                        //��ϷĬ��������
	LONGLONG         lCurrentChip;                      //��ǰ��ע
	BYTE			 bEndMode;						    //������ʽ
	int              nPrizeTimes;                       //��������
	int              nStarCount;                        //�¶�С�����
	int              nTimes;                            //��ע����
	bool             bLackChip;                         //�Ƿ�����ע
};
//׼��״̬
struct CMD_S_READY_STATUS
{
	WORD			wUserID;							//����κ�
	LONGLONG		lUserScore;							//��ҽ������
	LONGLONG        lGameNeed;                          //��Ϸ������
	LONGLONG        lDefultChip;                        //��ϷĬ��������
	LONGLONG        lFinalPrize;                        //�����
	int             nAvailableTimes;                    //���տ������
	int             nTimes;                             //��С����
	int             nStarCount;                         //�¶�С�����
	bool            bIsSelected;                        //�Ƿ�ѡ��
	bool            bIsRolling;                         //�Ƿ���ת��
	bool            bLackChip;                          //�Ƿ�����ע
};
//��ʼ״̬
struct CMD_S_START_STATUS
{
	LONGLONG        lGameNeed;                          //��Ϸ������
	LONGLONG		lUserScore;							//��ҽ������
	LONGLONG        lDefultChip;                        //��ϷĬ��������
};
//�й�״̬
struct CMD_S_AOTURUN_STATUS
{
	bool            bIsSelected;                       //�Ƿ�ѡ��
	bool            bIsRolling;                        //�Ƿ���ת��
};
//ת��״̬
struct CMD_S_PICROLL_STATUS
{
	LONGLONG        lGameNeed;                          //��Ϸ������
	LONGLONG		lUserScore;							//��ҽ������
	LONGLONG		lDefultChip;					    //Ĭ�����Ľ����
	int             nAvailableTimes;                    //ʣ�������Ϸ����
	bool            bIsRolling;                         //�Ƿ���ת��
};
//�ı���ע
struct CMD_S_CHANGEBET_STATUS
{
	LONGLONG        lGameNeed;                          //��ǰ��ע
	LONGLONG		lUserScore;						    //��ҽ������
	LONGLONG		lDefultChip;						//Ĭ�����Ľ����
};
//ֹͣ״̬
struct CMD_S_STOP_STATUS
{
	bool            bIsRolling;                         //�Ƿ���ת��
};
//��С��״̬
struct CMD_S_CLOWNBET_STATUS
{
	int              nStarCount;                         //�¶�С������Ǽ�
	int              nClownIndex;                        //С������
	bool             bChooseRight;                       //�Ƿ�¶�С��
	LONGLONG         lChipPrize;                         //��ǰ��ע
	LONGLONG		 lUserScore;						 //��ҽ������
	int              nTimes;                             //��С���ע����
	bool             bLackChip;                          //�Ƿ�����ע
};
//�ı䱶��
struct CMD_S_CHANGEPRIZE_STATUS
{
	LONGLONG         lGameNeed;                         //��ǰ��ע
	LONGLONG		 lUserScore;						//��ҽ������
	int              nTimes;                            //��ע����
};
//������Ϸ
struct CMD_S_GETPRIZE_STATUS
{
	WORD			 wUserID;							 //����κ�
	LONGLONG         lChipPrize;                         //��ǰ��ע
	LONGLONG         lFinalPrize;                        //�����
	LONGLONG		 lUserScore;					   	 //��ҽ������
	LONGLONG         lDefultChip;                        //��ϷĬ��������
	LONGLONG         lCurrentChip;                       //��ǰ��ע
	BYTE			 bEndMode;						     //������ʽ
	int              nPrizeTimes;                        //��������
	int              nStarCount;                         //�¶�С�����
	int              nTimes;                             //��ע����
	bool             bLackChip;                          //�Ƿ�����ע
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////

//�ͻ�������ṹ
#define SUB_C_GO_GAME                   300               //��ʼ����
#define SUB_C_CHANGE_BET                301               //�ı��ע
#define SUB_C_STOP_GAME                 302               //ֹͣ����
#define SUB_C_CLOWN_BET                 303               //�ж�С��
#define SUB_C_CHANGE_PRIZE              304               //�ı��С����
#define SUB_C_GET_PRIZE                 305               //������Ϸ
#define SUB_C_GO_GAME_ITEM              306               //��ʼ����(ʹ�õ���)

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʼ��Ϸ
struct CMD_C_START_GAME
{
	LONGLONG        lGameNeed;                          //��Ϸ������
	LONGLONG		lUserScore;							//��ҽ������
	LONGLONG        lDefultChip;                        //��ϷĬ��������
	int             nAvailableTimes;                   //ʣ�������Ϸ����
};
//�й���Ϸ
struct CMD_C_AUTORUN_GAME
{
	bool            bIsSelected;                       //�Ƿ�ѡ��
	bool            bIsRolling;                        //�Ƿ���ת��
};
//�ı���ע
struct CMD_C_CHANGE_BET
{
	bool            bPlusChip;                          //������ע
	bool            bMinusChip;                         //������ע
	LONGLONG        lGameNeed;                          //��ǰ��ע
	LONGLONG		lUserScore;						    //��ҽ������
	LONGLONG		lDefultChip;						//Ĭ�����Ľ����
};
//ֹͣ��Ϸ
struct CMD_C_STOP_SCROLL
{
	bool            bIsRolling;                        //�Ƿ���ת��
};
//�ж�С��
struct CMD_C_CLOWN_BET
{
	int              nStarCount;                         //�¶�С������Ǽ�
	int              nClownIndex;                        //С������
	bool             bChooseRight;                       //�Ƿ�¶�С��
	LONGLONG         lChipPrize;                         //��С���ע
	LONGLONG		 lUserScore;					     //��ҽ������
	int              nTimes;                             //��С���ע����
	bool             bLackChip;                          //�Ƿ�����ע
};
//�ı��С���ע
struct CMD_C_CHANGE_PRIZE
{
	//bool            bTwiceChip;                         //˫����ע
	//bool            bThreeTimesChip;                    //������ע
	//bool            bFiveTimesChip;                     //�屶��ע
	LONGLONG        lChipPrize;                         //��С���ע
	LONGLONG		lUserScore;						    //��ҽ������
	int             nTimes;                             //��ע����
};
//����������Ϸ
struct CMD_C_GET_PRIZE
{
	WORD			 wUserID;							//����κ�
	LONGLONG         lChipPrize;                        //��С���ע
	LONGLONG         lFinalPrize;                       //�����
	LONGLONG		 lUserScore;						//��ҽ������
	LONGLONG         lDefultChip;                       //��ϷĬ��������
	LONGLONG         lCurrentChip;                      //��ǰ��ע
	BYTE			 bEndMode;						    //������ʽ
	int              nPrizeTimes;                       //��������
	int              nStarCount;                        //�¶�С�����
	int              nTimes;                            //��ע����
	bool             bLackChip;                         //�Ƿ�����ע
};

struct CMD_S_CANT_PLAY_REMIND
{
	TCHAR							szDescribeString[128];				//������Ϣ
};
////////////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif