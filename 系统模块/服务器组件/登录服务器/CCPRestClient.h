/*
 *  Copyright (c) 2014 The CCP project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a Beijing Speedtong Information Technology Co.,Ltd license
 *  that can be found in the LICENSE file in the root of the web site.
 *
 *    http://www.yuntongxun.com
 *
 *  An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#ifndef _CCPRESTCLIENT_H_
#define _CCPRESTCLIENT_H_

#include "CCPRestDefine.h"

#ifndef __cplusplus
typedef enum {false, true} bool;
#endif

#ifdef __cplusplus
extern "C" {
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////
//��־����
#define LOG_PACKAGE 5
#define LOG_DEBUG   4
#define LOG_INFO    3
#define LOG_ERROR   2
#define LOG_OPEN    1
#define LOG_CLOSE   0

enum {//���ش��󷵻�ֵ
	ERR_LOCAL_NET =172001,//�������
	ERR_LOCAL_NO_RETURN,//�޷���
	ERR_LOCAL_NO_RETURN_BODY,//�޷��ذ���
	ERR_LOCAL_IP_NULL,//IP��
	ERR_LOCAL_IP_PORT,//�˿ڴ���С�ڵ���0��
	ERR_LOCAL_ACCOUNT_NULL,//���˺ſ�
	ERR_LOCAL_ACCOUNT_TOKEN_NULL,//���˺����ƿ�
    ERR_LOCAL_SUBACCOUNT_NULL,//���˺ſ�
	ERR_LOCAL_SUBACCOUNT_TOKEN_NULL,//���˺����ƿ�
	ERR_LOCAL_VOIP_NULL,//VoIP�ſ�
	ERR_LOCAL_VOIP_TOKEN_NULL,//VoIP�����ƿ�
    ERR_LOCAL_APPID_NULL,//Ӧ��ID��
};
enum {//rest ���� ���ش��󷵻�ֵ

	ERR_REST_NULL =171300,
	ERR_REST_MD5_BASE64,
	ERR_REST_SYN_HTTP_REQUEST,//����socket����ʧ��
	ERR_REST_SYN_HTTP_STATAUS_CODE,
	ERR_REST_CREATE_FILE,
	ERR_REST_XML_NODE,
	ERR_REST_XML_ELEMENT,
	ERR_REST_XML_STATAUS_CODE,
	ERR_REST_XML_FILESTATAUS_CODE,
	ERR_REST_QUEUE_EMPTY,
	ERR_REST_XML_SWITCHIP_NULL,
	ERR_REST_ENCODE_HTTP,
	ERR_REST_RECV_NULL,
	ERR_REST_BODY_NULL,//���ݿ�
	ERR_REST_HEAD_LARGE,//ͷ̫��
	ERR_REST_RESPONSE_CONTENT_LENGTH,//���ض����ư��峤��
	ERR_REST_CANCEL_UP_OR_DOWM_NOW,//�ⲿȡ�������ϴ���������
	ERR_REST_SEND_WSAECONNRESET,//���·��Զ�˸�λ
	ERR_REST_RECV_BLOCK,//������ʱ������������msglen=0������֣���ʱ��Ӧ�˳��������Ż��� ѭ���˳�����
	ERR_REST_UNICODE_TO_UTF8_MORE_LEN,//����������ַ���̫��
};
enum {//����body����
     HTTP_BODY_TYPE_XML= 0x00,
     HTTP_BODY_TYPE_JSON,

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʼ�������ò���
	/*! @function
	********************************************************************************
	������   : init
	����     : ��ʼ������ʼ��ȫ�ֶ���
	����     : [IN]  rest_addr : ��ͨѶƽ̨��������ַ
			   [IN]  rest_port : �������˿�

	����ֵ   : �Ƿ�ɹ� 0���ɹ��� ��0ʧ��
	*******************************************************************************/
	int init(const char *rest_addr, int rest_port);

	/*! @function
	********************************************************************************
	������   : setAccount
	����     : �������˺š�����ȫ�����˺�
	����     : [IN]  AccountSid      : �û����˺�
			   [IN]  AccountToken    : ���˺�����
	����ֵ   : �Ƿ�ɹ� 0���ɹ��� ��0ʧ��
	*******************************************************************************/
	int setAccount(const char *AccountSid,  const char *AccountToken);

	/*! @function
	********************************************************************************
	������   : setSubAccount
	����     : �������˺š�����ȫ�����˺�
	����     : [IN]  SubAccountSid	 : �û����˺�
			   [IN]  SubAccountToken : ���˺�����
			   [IN]  VoIPAccount	 : VoIP�˺�
			   [IN]  VoIPPassword	 : VoIP����
	����ֵ   : �Ƿ�ɹ� 0���ɹ��� ��0ʧ��
	*******************************************************************************/
	int setSubAccount(const char *SubAccountSid,  const char *SubAccountToken, const char *VoIPAccount,const char *VoIPPassword);
	
	/*! @function
	********************************************************************************
	������   : setAppId
	����     : ����Ӧ��Id������ȫ��Ӧ��Id
	����     : [IN]  appId       : Ӧ��Id

	����ֵ   : �Ƿ�ɹ� 0���ɹ��� ��0ʧ��
	*******************************************************************************/	
	int setAppId(const char* appId);

	/*! @function
	********************************************************************************
	������   : unInit
	����     : ע��
	����ֵ   : �Ƿ�ɹ� 0���ɹ��� ��0ʧ��
	*******************************************************************************/
	int unInit();

	/*! @function
	********************************************************************************
	������   : setLogLevel
	����     : ������־�����ڳ�ʼ��ǰ����
	����     : [IN]  Level : ���������ֵԽ�󣬴�ӡ��Խ��ϸ
	                        #define LOG_PACKAGE 5
							#define LOG_DEBUG   4
							#define LOG_INFO    3
							#define LOG_ERROR   2
							#define LOG_OPEN    1
							#define LOG_CLOSE   0
	����ֵ   : �Ƿ�ɹ� 0���ɹ��� ��0ʧ��
	*******************************************************************************/
	int setLogLevel(int Level);

	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//REST �ӿڲ���	

	//���˻���Ϣ��ѯ
	/*! @function
	********************************************************************************
	������   : queryAccountInfo
	����     : �˻���Ϣ��ѯ�����˺Ž�����֤
	����     : [out]  outData : ������Ӧ����,�ⲿ�����ڴ�
	����ֵ   : �Ƿ�ɹ� 0���ɹ��� ��0ʧ��
	*******************************************************************************/
	int queryAccountInfo(AccountInfo* outData);

	//�������˻�
	/*! @function
	********************************************************************************
	������   : createSubAccount
	����     : �������˻������˺Ž�����֤
	����     : [out] outData     : ������Ӧ����,�ⲿ�����ڴ�		   
	           [IN]  friendlyName: ���˻����ơ�����Ӣ����ĸ�Ͱ���������������˻�Ψһ���ƣ��Ƽ�ʹ�õ��������ַ
	����ֵ   : �Ƿ�ɹ� 0���ɹ��� ��0ʧ��
	*******************************************************************************/	
    int createSubAccount(SubAccountOne* outData,const char* friendlyName);

	//��ȡ���˻�
	/*! @function
	********************************************************************************
	������   : getSubAccounts
	����     : ������ѯ���˻������˺Ž�����֤
	����     : [out] outData : ������Ӧ����,�ⲿ�����ڴ棬����offset�����˻�����ռ�
	           [IN]  startNo : ��ʼ����ţ�Ĭ�ϴ�0��ʼ
			   [IN]  offset  : һ�β�ѯ�������������С��1���������100��
	����ֵ   : �Ƿ�ɹ� 0���ɹ��� ��0ʧ��
	*******************************************************************************/	
	int getSubAccounts(SubAccounts* outData,int startNo, int offset);

	//���˻���Ϣ��ѯ
	/*! @function
	********************************************************************************
	������   : querySubAccount
	����     : ���˻���Ϣ��ѯ�����˺Ž�����֤
	����     : [out] outData     : ������Ӧ����,�ⲿ�����ڴ�
	           [IN]  friendlyName: ���˻����ơ�����Ӣ����ĸ�Ͱ���������������˻�Ψһ���ƣ��Ƽ�ʹ�õ��������ַ
	����ֵ   : �Ƿ�ɹ� 0���ɹ��� ��0ʧ��
	*******************************************************************************/
	int querySubAccount(SubAccountOne* outData,const char* friendlyName);
	
	//ģ�����
	/*! @function
	********************************************************************************
	������   : TemplateSMS
	����     : ��������ģ����š����˺Ž�����֤
	����     : [out] outData    : ������Ӧ����,�ⲿ�����ڴ�
			   [IN]  to         : ���Ž��ն��ֻ����뼯�ϣ���Ӣ�Ķ��ŷֿ���ÿ�����͵��ֻ����������ó���100��
	           [IN]  templateId : ģ��Id 
			   [IN]  dataArr    : �����������顣�����滻ģ����{���}
	           [IN]  dataArrNum : �����ڳ�Ա���� 
	����ֵ   : �Ƿ�ɹ� 0���ɹ��� ��0ʧ��
	*******************************************************************************/
	int sendTemplateSMS(SMSInfo* outData,const char* to,const char* templateId,const char** dataArr,int dataArrNum);

	//˫��ز�
	/*! @function
	********************************************************************************
	������   : Callback
	����     : ˫��ز������˺Ž�����֤
	����     : [out] outData        : ������Ӧ����,�ⲿ�����ڴ�
			   [IN]  from           : ���С����ʱ�׼,00+������+�绰���룻�й����ڿ���ʡ��Ϊ�绰���룬�̻�����������š�
	           [IN]  to             : ���С����ʱ�׼��00+������+�绰���룻�й����ڿ���ʡ��Ϊ�绰���룬�̻�����������š�
			   [IN]  fromSerNum     : ���в���ʾ�ĺ��룬����ƽ̨���ԺŹ�����ơ�����ΪNULL
	           [IN]  customerSerNum : ���в���ʾ�ĺ��룬����ƽ̨���ԺŹ�����ơ�����ΪNULL
			   [IN]  promptTone : �������Զ���ز���ʾ�� *.wav������ΪNULL
	����ֵ   : �Ƿ�ɹ� 0���ɹ��� ��0ʧ��
	*******************************************************************************/
	int callBack (CallInfo* outData,const char* from, const char* to,const char* fromSerNum, const char* customerSerNum, const char* promptTone);

	//���֪ͨ
	/*! @function
	********************************************************************************
	������   : landingCall
	����     : ���֪ͨ�����˺Ž�����֤
	����     : [out] outData       : ������Ӧ����,�ⲿ�����ڴ�
			   [IN]  to            : ���к���
	           [IN]  mediaName     : �����ļ����ƣ���ʽ wav����mediaTxt����ͬʱΪNULL����ΪNULLʱmediaTxt����ʧЧ��
			   [IN]  mediaNameType : Ĭ��ֵΪ0����ʾ�û������ļ�����ֵΪ1��ʾƽ̨ͨ���ļ�
			   [IN]  mediaTxt      : �ı����ݣ�Ĭ��ֵΪ�ա�����ΪNULL
	           [IN]  playTimes     : ѭ�����Ŵ�����1��3�Σ�Ĭ�ϲ���1��
			   [IN]  displayNum    : ��ʾ�����к��룬��ʾȨ���ɷ������ơ�����ΪNULL
	           [IN]  respUrl       : ״̬֪ͨ�ص���ַ����ͨѶƽ̨�����Url��ַ���ͺ��н��֪ͨ������ΪNULL
	����ֵ   : �Ƿ�ɹ� 0���ɹ��� ��0ʧ��
	*******************************************************************************/	
	int landingCall(CallInfo* outData,const char* to,const char* mediaName,int mediaNameType,const char* mediaTxt,int playTimes, const char* diaplayNum,const char* respUrl);
 
	//������֤��
	/*! @function
	********************************************************************************
	������   : voiceVerify
	����     : ������֤�롣���˺Ž�����֤
	����     : [out] outData    : ������Ӧ����,�ⲿ�����ڴ�
			   [IN]  to         : ���պ���
	           [IN]  verifyCode : ��֤�����ݣ�Ϊ���ֺ�Ӣ����ĸ�������ִ�Сд������4-8λ
	           [IN]  playTimes  : ѭ�����Ŵ�����1��3�Σ�Ĭ�ϲ���1��
			   [IN]  displayNum : ��ʾ�����к��룬��ʾȨ���ɷ������ơ�����ΪNULL
	           [IN]  respUrl    : ״̬֪ͨ�ص���ַ����ͨѶƽ̨�����Url��ַ���ͺ��н��֪ͨ������ΪNULL
	����ֵ   : �Ƿ�ɹ� 0���ɹ��� ��0ʧ��
	*******************************************************************************/
	int voiceVerify(CallInfo* outData,const char* to, const char* verifyCode, int playTimes,const char* diaplayNum,const char* respUrl);

	//IVR���
	/*! @function
	********************************************************************************
	������   : ivrDial 
	����     : IVR��������˺Ž�����֤
	����     : [out] outData  : ������Ӧ����,�ⲿ�����ڴ�
			   [IN]  number   : �����к��룬ΪDial�ڵ������
	           [IN]  userdata : �û����ݣ���<startservice>֪ͨ�з��أ�ֻ������д�����ַ���ΪDial�ڵ�����ԡ�����ΪNULL
	           [IN]  record   : �Ƿ�¼����������Ϊtrue��false��Ĭ��ֵΪfalse��¼����ΪDial�ڵ�����ԡ�����ΪNULL
	����ֵ   : �Ƿ�ɹ� 0���ɹ��� ��0ʧ��
	*******************************************************************************/
	int ivrDial (IVRCall* outData,const char* number, const char* userdata, bool record);

	//��������
	/*! @function
	********************************************************************************
	������   : billRecords
	����     : �������ء����˺Ž�����֤
	����     : [out] outData  : ������Ӧ����,�ⲿ�����ڴ�
			   [IN]  date     : day ����ǰһ������ݣ���00:00 �C 23:59��;week����ǰһ�ܵ�����(��һ ������)��month��ʾ��һ���µ����ݣ��ϸ��±�ʾ��ǰ�¼�1�����������4��10�ţ����ѯ�����3�·ݵ����ݣ�
	           [IN]  keywords : �ͻ��Ĳ�ѯ�������ɿͻ����ж��岢�ṩ����ͨѶƽ̨��Ĭ�ϲ�����Դ˲���������ΪNULL
	����ֵ   : �Ƿ�ɹ� 0���ɹ��� ��0ʧ��
	*******************************************************************************/
	int billRecords(BillRecord* outData,const char* date, const char* keywords);


#ifdef __cplusplus
}
#endif

#endif 
