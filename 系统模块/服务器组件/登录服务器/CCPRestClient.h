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
//日志级别
#define LOG_PACKAGE 5
#define LOG_DEBUG   4
#define LOG_INFO    3
#define LOG_ERROR   2
#define LOG_OPEN    1
#define LOG_CLOSE   0

enum {//本地错误返回值
	ERR_LOCAL_NET =172001,//网络错误
	ERR_LOCAL_NO_RETURN,//无返回
	ERR_LOCAL_NO_RETURN_BODY,//无返回包体
	ERR_LOCAL_IP_NULL,//IP空
	ERR_LOCAL_IP_PORT,//端口错误（小于等于0）
	ERR_LOCAL_ACCOUNT_NULL,//主账号空
	ERR_LOCAL_ACCOUNT_TOKEN_NULL,//主账号令牌空
    ERR_LOCAL_SUBACCOUNT_NULL,//子账号空
	ERR_LOCAL_SUBACCOUNT_TOKEN_NULL,//子账号令牌空
	ERR_LOCAL_VOIP_NULL,//VoIP号空
	ERR_LOCAL_VOIP_TOKEN_NULL,//VoIP号令牌空
    ERR_LOCAL_APPID_NULL,//应用ID空
};
enum {//rest 请求 本地错误返回值

	ERR_REST_NULL =171300,
	ERR_REST_MD5_BASE64,
	ERR_REST_SYN_HTTP_REQUEST,//建立socket连接失败
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
	ERR_REST_BODY_NULL,//内容空
	ERR_REST_HEAD_LARGE,//头太长
	ERR_REST_RESPONSE_CONTENT_LENGTH,//下载二进制包体长度
	ERR_REST_CANCEL_UP_OR_DOWM_NOW,//外部取消附件上传或者下载
	ERR_REST_SEND_WSAECONNRESET,//虚电路被远端复位
	ERR_REST_RECV_BLOCK,//网络有时候阻塞，导致msglen=0情况出现，此时不应退出，后续优化加 循环退出机制
	ERR_REST_UNICODE_TO_UTF8_MORE_LEN,//传入的中文字符串太长
};
enum {//请求body类型
     HTTP_BODY_TYPE_XML= 0x00,
     HTTP_BODY_TYPE_JSON,

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//初始化和配置部分
	/*! @function
	********************************************************************************
	函数名   : init
	功能     : 初始化。初始化全局对象
	参数     : [IN]  rest_addr : 云通讯平台服务器地址
			   [IN]  rest_port : 服务器端口

	返回值   : 是否成功 0：成功； 非0失败
	*******************************************************************************/
	int init(const char *rest_addr, int rest_port);

	/*! @function
	********************************************************************************
	函数名   : setAccount
	功能     : 设置主账号。更新全局主账号
	参数     : [IN]  AccountSid      : 用户主账号
			   [IN]  AccountToken    : 主账号密码
	返回值   : 是否成功 0：成功； 非0失败
	*******************************************************************************/
	int setAccount(const char *AccountSid,  const char *AccountToken);

	/*! @function
	********************************************************************************
	函数名   : setSubAccount
	功能     : 设置子账号。更新全局子账号
	参数     : [IN]  SubAccountSid	 : 用户子账号
			   [IN]  SubAccountToken : 子账号密码
			   [IN]  VoIPAccount	 : VoIP账号
			   [IN]  VoIPPassword	 : VoIP密码
	返回值   : 是否成功 0：成功； 非0失败
	*******************************************************************************/
	int setSubAccount(const char *SubAccountSid,  const char *SubAccountToken, const char *VoIPAccount,const char *VoIPPassword);
	
	/*! @function
	********************************************************************************
	函数名   : setAppId
	功能     : 设置应用Id。更新全局应用Id
	参数     : [IN]  appId       : 应用Id

	返回值   : 是否成功 0：成功； 非0失败
	*******************************************************************************/	
	int setAppId(const char* appId);

	/*! @function
	********************************************************************************
	函数名   : unInit
	功能     : 注销
	返回值   : 是否成功 0：成功； 非0失败
	*******************************************************************************/
	int unInit();

	/*! @function
	********************************************************************************
	函数名   : setLogLevel
	功能     : 设置日志级别。在初始化前调用
	参数     : [IN]  Level : 级别参数，值越大，打印的越详细
	                        #define LOG_PACKAGE 5
							#define LOG_DEBUG   4
							#define LOG_INFO    3
							#define LOG_ERROR   2
							#define LOG_OPEN    1
							#define LOG_CLOSE   0
	返回值   : 是否成功 0：成功； 非0失败
	*******************************************************************************/
	int setLogLevel(int Level);

	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//REST 接口部分	

	//主账户信息查询
	/*! @function
	********************************************************************************
	函数名   : queryAccountInfo
	功能     : 账户信息查询。主账号进行验证
	参数     : [out]  outData : 服务器应答结果,外部分配内存
	返回值   : 是否成功 0：成功； 非0失败
	*******************************************************************************/
	int queryAccountInfo(AccountInfo* outData);

	//创建子账户
	/*! @function
	********************************************************************************
	函数名   : createSubAccount
	功能     : 创建子账户。主账号进行验证
	参数     : [out] outData     : 服务器应答结果,外部分配内存		   
	           [IN]  friendlyName: 子账户名称。可由英文字母和阿拉伯数字组成子账户唯一名称，推荐使用电子邮箱地址
	返回值   : 是否成功 0：成功； 非0失败
	*******************************************************************************/	
    int createSubAccount(SubAccountOne* outData,const char* friendlyName);

	//获取子账户
	/*! @function
	********************************************************************************
	函数名   : getSubAccounts
	功能     : 批量查询子账户。主账号进行验证
	参数     : [out] outData : 服务器应答结果,外部分配内存，分配offset个子账户数组空间
	           [IN]  startNo : 开始的序号，默认从0开始
			   [IN]  offset  : 一次查询的最大条数，最小是1条，最大是100条
	返回值   : 是否成功 0：成功； 非0失败
	*******************************************************************************/	
	int getSubAccounts(SubAccounts* outData,int startNo, int offset);

	//子账户信息查询
	/*! @function
	********************************************************************************
	函数名   : querySubAccount
	功能     : 子账户信息查询。主账号进行验证
	参数     : [out] outData     : 服务器应答结果,外部分配内存
	           [IN]  friendlyName: 子账户名称。可由英文字母和阿拉伯数字组成子账户唯一名称，推荐使用电子邮箱地址
	返回值   : 是否成功 0：成功； 非0失败
	*******************************************************************************/
	int querySubAccount(SubAccountOne* outData,const char* friendlyName);
	
	//模板短信
	/*! @function
	********************************************************************************
	函数名   : TemplateSMS
	功能     : 发送下行模板短信。主账号进行验证
	参数     : [out] outData    : 服务器应答结果,外部分配内存
			   [IN]  to         : 短信接收端手机号码集合，用英文逗号分开，每批发送的手机号数量不得超过100个
	           [IN]  templateId : 模板Id 
			   [IN]  dataArr    : 内容数据数组。用于替换模板中{序号}
	           [IN]  dataArrNum : 数组内成员个数 
	返回值   : 是否成功 0：成功； 非0失败
	*******************************************************************************/
	int sendTemplateSMS(SMSInfo* outData,const char* to,const char* templateId,const char** dataArr,int dataArrNum);

	//双向回拨
	/*! @function
	********************************************************************************
	函数名   : Callback
	功能     : 双向回拨。子账号进行验证
	参数     : [out] outData        : 服务器应答结果,外部分配内存
			   [IN]  from           : 主叫。国际标准,00+国家码+电话号码；中国国内可以省略为电话号码，固话号码须带区号。
	           [IN]  to             : 被叫。国际标准，00+国家码+电话号码；中国国内可以省略为电话号码，固话号码须带区号。
			   [IN]  fromSerNum     : 主叫侧显示的号码，根据平台侧显号规则控制。可以为NULL
	           [IN]  customerSerNum : 被叫侧显示的号码，根据平台侧显号规则控制。可以为NULL
			   [IN]  promptTone : 第三方自定义回拨提示音 *.wav。可以为NULL
	返回值   : 是否成功 0：成功； 非0失败
	*******************************************************************************/
	int callBack (CallInfo* outData,const char* from, const char* to,const char* fromSerNum, const char* customerSerNum, const char* promptTone);

	//外呼通知
	/*! @function
	********************************************************************************
	函数名   : landingCall
	功能     : 外呼通知。主账号进行验证
	参数     : [out] outData       : 服务器应答结果,外部分配内存
			   [IN]  to            : 被叫号码
	           [IN]  mediaName     : 语音文件名称，格式 wav。与mediaTxt不能同时为NULL，不为NULL时mediaTxt属性失效。
			   [IN]  mediaNameType : 默认值为0，表示用户语音文件；　值为1表示平台通用文件
			   [IN]  mediaTxt      : 文本内容，默认值为空。可以为NULL
	           [IN]  playTimes     : 循环播放次数，1－3次，默认播放1次
			   [IN]  displayNum    : 显示的主叫号码，显示权限由服务侧控制。可以为NULL
	           [IN]  respUrl       : 状态通知回调地址，云通讯平台将向该Url地址发送呼叫结果通知。可以为NULL
	返回值   : 是否成功 0：成功； 非0失败
	*******************************************************************************/	
	int landingCall(CallInfo* outData,const char* to,const char* mediaName,int mediaNameType,const char* mediaTxt,int playTimes, const char* diaplayNum,const char* respUrl);
 
	//语音验证码
	/*! @function
	********************************************************************************
	函数名   : voiceVerify
	功能     : 语音验证码。主账号进行验证
	参数     : [out] outData    : 服务器应答结果,外部分配内存
			   [IN]  to         : 接收号码
	           [IN]  verifyCode : 验证码内容，为数字和英文字母，不区分大小写，长度4-8位
	           [IN]  playTimes  : 循环播放次数，1－3次，默认播放1次
			   [IN]  displayNum : 显示的主叫号码，显示权限由服务侧控制。可以为NULL
	           [IN]  respUrl    : 状态通知回调地址，云通讯平台将向该Url地址发送呼叫结果通知。可以为NULL
	返回值   : 是否成功 0：成功； 非0失败
	*******************************************************************************/
	int voiceVerify(CallInfo* outData,const char* to, const char* verifyCode, int playTimes,const char* diaplayNum,const char* respUrl);

	//IVR外呼
	/*! @function
	********************************************************************************
	函数名   : ivrDial 
	功能     : IVR外呼。主账号进行验证
	参数     : [out] outData  : 服务器应答结果,外部分配内存
			   [IN]  number   : 待呼叫号码，为Dial节点的属性
	           [IN]  userdata : 用户数据，在<startservice>通知中返回，只允许填写数字字符，为Dial节点的属性。可以为NULL
	           [IN]  record   : 是否录音，可填项为true和false，默认值为false不录音，为Dial节点的属性。可以为NULL
	返回值   : 是否成功 0：成功； 非0失败
	*******************************************************************************/
	int ivrDial (IVRCall* outData,const char* number, const char* userdata, bool record);

	//话单下载
	/*! @function
	********************************************************************************
	函数名   : billRecords
	功能     : 话单下载。主账号进行验证
	参数     : [out] outData  : 服务器应答结果,外部分配内存
			   [IN]  date     : day 代表前一天的数据（从00:00 – 23:59）;week代表前一周的数据(周一 到周日)；month表示上一个月的数据（上个月表示当前月减1，如果今天是4月10号，则查询结果是3月份的数据）
	           [IN]  keywords : 客户的查询条件，由客户自行定义并提供给云通讯平台。默认不填忽略此参数。可以为NULL
	返回值   : 是否成功 0：成功； 非0失败
	*******************************************************************************/
	int billRecords(BillRecord* outData,const char* date, const char* keywords);


#ifdef __cplusplus
}
#endif

#endif 
