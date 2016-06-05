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

//比赛配置
struct tagMatchConfig
{
	int nCfgID;//比赛编号
	tagMatchTimeDate CfgStartDate;//配置开始日期
	tagMatchTimeDate CfgEndDate;//配置结束日期
	int nCfgResetType;//比赛重置类型
	int nCfgFlag;//比赛标识(1-9)
	std::string m_sSendMatchInfo;//比赛发送信息
	int m_nSendMatchInfoElapse;//比赛发送信息间隔

	tagMatchTimeDate nGenBaseDate;//生成日期
	int nGenTag;//生成标签
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

//比赛信息
struct tagMatchInfo
{
	int nFlag;//比赛标识
	SCORE lWinScore;//比赛得分
};

class MatchManager
{
public:
	int m_nMatchCheckElapse;//比赛检查间隔(秒，0为禁用比赛)
	__int64 m_nLastCheckMatchTime;
	std::vector<tagMatchConfig> m_vMatchConfig;
public:
	MatchManager();
	~MatchManager();

	int CheckMatchConfig();
};
