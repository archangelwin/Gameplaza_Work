#ifndef PLATFORM_HEAD_FILE
#define PLATFORM_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

//�����ļ�
#include "Macro.h"
#include "Define.h"

//�ṹ�ļ�
#include "Struct.h"
#include "Packet.h"
#include "Property.h"

//ģ���ļ�
#include "Array.h"
#include "Module.h"
#include "PacketAide.h"
#include "ServerRule.h"
#include "RightDefine.h"

//////////////////////////////////////////////////////////////////////////////////

//����汾
#define VERSION_FRAME				PROCESS_VERSION(6,0,3)				//��ܰ汾
#define VERSION_PLAZA				PROCESS_VERSION(11,0,3)				//�����汾
#define VERSION_MOBILE_ANDROID		PROCESS_VERSION(6,0,3)				//�ֻ��汾
#define VERSION_MOBILE_IOS			PROCESS_VERSION(6,0,3)				//�ֻ��汾

//�汾����
#define VERSION_EFFICACY			0									//Ч��汾
#define VERSION_FRAME_SDK			INTERFACE_VERSION(6,3)				//��ܰ汾

//////////////////////////////////////////////////////////////////////////////////
//�����汾

#ifndef _DEBUG

//ƽ̨����
const TCHAR szProduct[]=TEXT("������Ƶ������Ϸƽ̨");							//��Ʒ����
const TCHAR szPlazaClass[]=TEXT("WH4699GAMEPlaza");						//�㳡����
const TCHAR szProductKey[]=TEXT("WH4699GAME");					        //��Ʒ����

//��ַ����
const TCHAR szCookieUrl[]=TEXT("http://www.4699.com");						//��¼��ַ
const TCHAR szLogonServer[]=TEXT("login.4699.com");					//��¼��ַ
const TCHAR szPlatformLink[]=TEXT("http://www.4699.com");			//ƽ̨��վ

#else

//////////////////////////////////////////////////////////////////////////////////
//�ڲ�汾

//ƽ̨����
const TCHAR szProduct[]=TEXT("������Ƶ������Ϸƽ̨");							//��Ʒ����
const TCHAR szPlazaClass[]=TEXT("WH4699GAMEPlaza");						//�㳡����
const TCHAR szProductKey[]=TEXT("WH4699GAME");					//��Ʒ����

//��ַ����
const TCHAR szCookieUrl[]=TEXT("http://127.0.0.1:6688");						//��¼��ַ
const TCHAR szLogonServer[]=TEXT("127.0.0.1");					//��¼��ַ
const TCHAR szPlatformLink[]=TEXT("http://127.0.0.1:6688");			//ƽ̨��վ

#endif

//////////////////////////////////////////////////////////////////////////////////

//���ݿ���
const TCHAR szPlatformDB[]=TEXT("QPPlatformDB");						//ƽ̨���ݿ�
const TCHAR szAccountsDB[]=TEXT("QPAccountsDB");						//�û����ݿ�
const TCHAR szTreasureDB[]=TEXT("QPTreasureDB");						//�Ƹ����ݿ�
const TCHAR szExerciseDB[]=TEXT("QPExerciseDB");						//��ϰ���ݿ�

//////////////////////////////////////////////////////////////////////////////////

//��Ȩ��Ϣ
const TCHAR szCompilation[]=TEXT("75794A4B-3891-4DF9-8EF4-B626C9EF6D8C");

//////////////////////////////////////////////////////////////////////////////////

#endif