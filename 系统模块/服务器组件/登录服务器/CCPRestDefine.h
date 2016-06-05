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

//rest 原因及描述
typedef struct _RestReason
{
	int statusCode;     //原因
	char statusMsg[256];//原因描述
}RestReason;

//账户信息查询
typedef struct _AccountInfo
{
	RestReason reason;

	char  dateCreated[20]; //主账户的创建时间
	char  dateUpdated[20]; //主账户的创建时间
	char  friendlyName[65];//主账户名称，唯一，由64个英文字母和阿拉伯数字组成，默认为您的电子邮箱地址
	int   type;            //主账户类型，取值有0（试用）和1（已注册），默认值为0
	int   status;          //主账户状态，取值有0（未激活）、1（激活）、2（暂停）、3（关闭），默认值为0
	double balance;        //主账户余额

}AccountInfo;

//子账号信息
typedef struct _SubAccountInfo
{
	char subAccountSid[33];//子账户Id。由32个英文字母和阿拉伯数字组成的子账户唯一标识符
	char subToken[33];     //子账户的授权令牌。由32个英文字母和阿拉伯数字组成
	char voipAccount[33];  //VoIP号码。由14位数字组成
	char voipPwd[33];      //VoIP密码。由8位数字和字母组成
	char dateCreated[20];  //子账户的创建时间
	char friendlyName[65]; //子账户名称，唯一，由64个英文字母和阿拉伯数字组成，默认为您的电子邮箱地址
}SubAccountInfo;
//创建子账号 子账号信息查询
typedef struct _SubAccountOne
{
	RestReason     reason;
	SubAccountInfo SubAccount;
}SubAccountOne;
//获取子账号 
typedef struct _SubAccounts
{
	RestReason      reason;
	int             count;//实际子账户个数
	SubAccountInfo* SubAccounts;//子账户数组首指针
}SubAccounts;


//双向回呼 外呼通知 语音验证码
typedef struct _CallInfo
{
	RestReason reason;

	char dateCreated[20]; //电话的创建时间
	char callSid[33];//一个由32个字符组成的唯一电话标识符
}CallInfo;

//发送短信 模版短信
typedef struct _SMSInfo
{
	RestReason reason;

	char dateCreated[20]; //短信的创建时间
	char smsMessageSid[33];//短信标识符。一个由32个字符组成的短信唯一标识符 
}SMSInfo;

//话单下载
typedef struct _BillRecord
{
	RestReason reason;

	char downurl[512];//话单下载地址
	char token[128];  //文件下载检验码 
}BillRecord;

//IVR外呼
typedef struct _IVRCall
{
	RestReason reason;

	char callSid[33];//一个由32个字符组成的唯一电话标识符
}IVRCall;

#endif 