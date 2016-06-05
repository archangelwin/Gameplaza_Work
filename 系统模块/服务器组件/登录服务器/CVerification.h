#include "StdAfx.h"
#include "CCPRestClient.h"
#include "CCPRestDefine.h"


struct tagMBVerifInfoThread
{
	TCHAR szMobilePhone[LEN_MOBILE_PHONE];
	DWORD dSocketId;
	TCHAR szVerificationCode[LEN_VERIFICATION_CODE];
};

class CVerification
{
private:
	static std::vector<tagMBVerifInfoThread> G_vThreadRequestMobileNum;
	static int m_iThreadRequestNum;
	static int m_iThreadSendNum;
	HANDLE  hSendhandle; 
	CString strThreadLogDir;
	CVerification();
	~CVerification();
	static CVerification *m_pInstance;
public:
	static unsigned __stdcall SendThreadFunProc(LPVOID lpParameter);
	void SendThreadMsgRecord(char * cMobileNum,SMSInfo &p);
	void ThreadRecvMsgRecord(char * cMobileNum);
	void StartThread(LPVOID lpParameter);
	void EndThread();
	CString GetFileDialogPath();
	void AddNewVerificationCode(DWORD dwSocketID,char * szVerificationCode,char * szMobilePhone);
	static CVerification * GetInstance()  
	{  
		if(m_pInstance == NULL)  //判断是否第一次调用  
			m_pInstance = new CVerification();  
		return m_pInstance;  
	}
	void OnRecordSendLoginNum(DWORD dwSendNum);
	void OnRecordFinishLoginNum(DWORD dwSendNum);
};
