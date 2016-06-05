/*
 *  Copyright (c) 2014 The CCP project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a Beijing Speedtong Information Technology Co.,Ltd license
 *  that can be found in the LICENSE file in the root of the web site.
 *
 *   http://www.yuntongxun.com
 *
 *  An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#ifndef _CCP_REST_DEFINE_H
#define _CCP_REST_DEFINE_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//rest ԭ������
typedef struct _RestReason
{
	int statusCode;     //ԭ��
	char statusMsg[256];//ԭ������
}RestReason;

//�˻���Ϣ��ѯ
typedef struct _AccountInfo
{
	RestReason reason;

	char  dateCreated[20]; //���˻��Ĵ���ʱ��
	char  dateUpdated[20]; //���˻��Ĵ���ʱ��
	char  friendlyName[65];//���˻����ƣ�Ψһ����64��Ӣ����ĸ�Ͱ�����������ɣ�Ĭ��Ϊ���ĵ��������ַ
	int   type;            //���˻����ͣ�ȡֵ��0�����ã���1����ע�ᣩ��Ĭ��ֵΪ0
	int   status;          //���˻�״̬��ȡֵ��0��δ�����1�������2����ͣ����3���رգ���Ĭ��ֵΪ0
	double balance;        //���˻����

}AccountInfo;

//���˺���Ϣ
typedef struct _SubAccountInfo
{
	char subAccountSid[33];//���˻�Id����32��Ӣ����ĸ�Ͱ�����������ɵ����˻�Ψһ��ʶ��
	char subToken[33];     //���˻�����Ȩ���ơ���32��Ӣ����ĸ�Ͱ������������
	char voipAccount[33];  //VoIP���롣��14λ�������
	char voipPwd[33];      //VoIP���롣��8λ���ֺ���ĸ���
	char dateCreated[20];  //���˻��Ĵ���ʱ��
	char friendlyName[65]; //���˻����ƣ�Ψһ����64��Ӣ����ĸ�Ͱ�����������ɣ�Ĭ��Ϊ���ĵ��������ַ
}SubAccountInfo;
//�������˺� ���˺���Ϣ��ѯ
typedef struct _SubAccountOne
{
	RestReason     reason;
	SubAccountInfo SubAccount;
}SubAccountOne;
//��ȡ���˺� 
typedef struct _SubAccounts
{
	RestReason      reason;
	int             count;//ʵ�����˻�����
	SubAccountInfo* SubAccounts;//���˻�������ָ��
}SubAccounts;


//˫��غ� ���֪ͨ ������֤��
typedef struct _CallInfo
{
	RestReason reason;

	char dateCreated[20]; //�绰�Ĵ���ʱ��
	char callSid[33];//һ����32���ַ���ɵ�Ψһ�绰��ʶ��
}CallInfo;

//���Ͷ��� ģ�����
typedef struct _SMSInfo
{
	RestReason reason;

	char dateCreated[20]; //���ŵĴ���ʱ��
	char smsMessageSid[33];//���ű�ʶ����һ����32���ַ���ɵĶ���Ψһ��ʶ�� 
}SMSInfo;

//��������
typedef struct _BillRecord
{
	RestReason reason;

	char downurl[512];//�������ص�ַ
	char token[128];  //�ļ����ؼ����� 
}BillRecord;

//IVR���
typedef struct _IVRCall
{
	RestReason reason;

	char callSid[33];//һ����32���ַ���ɵ�Ψһ�绰��ʶ��
}IVRCall;

#endif 