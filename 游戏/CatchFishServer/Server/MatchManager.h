#pragma once

enum eMatchResetFlag
{
	eMatchResetFlag_Null,
	eMatchResetFlag_Hour,
	eMatchResetFlag_Day,
	eMatchResetFlag_Week,
	eMatchResetFlag_Month,
};

struct tagMatchTimeDate
{
	short nYear;
	char nMonth;
	char nDay;
	char nWeek;
	char nHour;
	char nMin;

	tagMatchTimeDate()
	{
		nYear=-1;
		nMonth=-1;
		nDay=-1;
		nWeek=-1;
		nHour=-1;
		nMin=-1;
	}
};

//��������
struct tagMatchConfig
{
	int nCfgID;//�������
	tagMatchTimeDate CfgStartDate;//���ÿ�ʼ����
	tagMatchTimeDate CfgEndDate;//���ý�������
	int nCfgResetType;//������������
	int nCfgFlag;//������ʶ(1-9)
	std::string m_sSendMatchInfo;//����������Ϣ
	int m_nSendMatchInfoElapse;//����������Ϣ���

	tagMatchTimeDate nGenBaseDate;//��������
	int nGenTag;//���ɱ�ǩ
	bool bInMatch;
	__int64 m_nLastSendMatchInfoTime;

	tagMatchConfig()
	{
		nCfgID=0;
		nCfgResetType=eMatchResetFlag_Null;
		nCfgFlag=0;

		nGenTag=0;
		bInMatch=false;

		m_nSendMatchInfoElapse=0;
		m_nLastSendMatchInfoTime=-1;
	}
};

//������Ϣ
struct tagMatchInfo
{
	int nFlag;//������ʶ
	SCORE lWinScore;//�����÷�
};

class MatchManager
{
public:
	int m_nMatchCheckElapse;//���������(�룬0Ϊ���ñ���)
	__int64 m_nLastCheckMatchTime;
	std::vector<tagMatchConfig> m_vMatchConfig;
public:
	MatchManager();
	~MatchManager();

	int CheckMatchConfig();
};
