#include "Stdafx.h"

MatchManager::MatchManager()
{

}

MatchManager::~MatchManager()
{

}

int MatchManager::CheckMatchConfig()
{
	int nInMatchCount = 0;

	__time64_t lNow;
	_time64(&lNow);

	if (0 == m_nMatchCheckElapse)
	{
		return -1;
	}

	if (-1 != m_nLastCheckMatchTime && m_nLastCheckMatchTime + m_nMatchCheckElapse > lNow)
	{
		return -1;
	}

	m_nLastCheckMatchTime = lNow;

	struct tm * lTimeCache;
	lTimeCache=_localtime64(&lNow);
	struct tm * lTime = lTimeCache;

	for (int i = 0; i < (int)m_vMatchConfig.size(); i++)
	{
		tagMatchConfig* pMatchCfg = &m_vMatchConfig[i];
		tagMatchTimeDate* pStartDate = &(pMatchCfg->CfgStartDate);
		tagMatchTimeDate* pEndDate = &(pMatchCfg->CfgEndDate);
		tagMatchTimeDate* pGenDate = &(pMatchCfg->nGenBaseDate);
		tagMatchTimeDate tempGenDate;
		memset(&tempGenDate, 0, sizeof(tempGenDate));

		pMatchCfg->bInMatch = false;
//printf("nTag=%d\n",pMatchCfg->nGenTag);
		//年
		tempGenDate.nYear = lTime->tm_year + 1900;
		//月
		tempGenDate.nMonth = lTime->tm_mon + 1;
		//日
		tempGenDate.nDay = lTime->tm_mday;
		//周(周六-周日必须分开)
		tempGenDate.nWeek = lTime->tm_wday;
		tempGenDate.nHour = lTime->tm_hour;
		tempGenDate.nMin = lTime->tm_min;

		bool bError = false;
		//判断最小值
		do
		{
			if (pStartDate->nYear >= 0)
			{
				if (tempGenDate.nYear > pStartDate->nYear)
				{
					break;
				}
				else if (tempGenDate.nYear < pStartDate->nYear)
				{
					bError = true;
					break;
				}
			}

			if (pStartDate->nMonth >= 0)
			{
				if (tempGenDate.nMonth > pStartDate->nMonth)
				{
					break;
				}
				else if (tempGenDate.nMonth < pStartDate->nMonth)
				{
					bError = true;
					break;
				}
			}

			if (pStartDate->nDay >= 0)
			{
				if (tempGenDate.nDay > pStartDate->nDay)
				{
					break;
				}
				else if (tempGenDate.nDay < pStartDate->nDay)
				{
					bError = true;
					break;
				}
			}

			if (pStartDate->nWeek >= 0)
			{
				if (tempGenDate.nWeek > pStartDate->nWeek)
				{
					break;
				}
				else if (tempGenDate.nWeek < pStartDate->nWeek)
				{
					bError = true;
					break;
				}
			}

			if (pStartDate->nHour >= 0)
			{
				if (tempGenDate.nHour > pStartDate->nHour)
				{
					break;
				}
				else if (tempGenDate.nHour < pStartDate->nHour)
				{
					bError = true;
					break;
				}
			}

			if (pStartDate->nMin >= 0)
			{
				if (tempGenDate.nMin > pStartDate->nMin)
				{
					break;
				}
				else if (tempGenDate.nMin < pStartDate->nMin)
				{
					bError = true;
					break;
				}
			}

		} while (false);

		if (bError)
		{
			continue;
		}

		//判断最大值
		do
		{
			if (pEndDate->nYear >= 0)
			{
				if (tempGenDate.nYear < pEndDate->nYear)
				{
					break;
				}
				else if (tempGenDate.nYear > pEndDate->nYear)
				{
					bError = true;
					break;
				}
			}

			if (pEndDate->nMonth >= 0)
			{
				if (tempGenDate.nMonth < pEndDate->nMonth)
				{
					break;
				}
				else if (tempGenDate.nMonth > pEndDate->nMonth)
				{
					bError = true;
					break;
				}
			}

			if (pEndDate->nDay >= 0)
			{
				if (tempGenDate.nDay < pEndDate->nDay)
				{
					break;
				}
				else if (tempGenDate.nDay > pEndDate->nDay)
				{
					bError = true;
					break;
				}
			}

			if (pEndDate->nWeek >= 0)
			{
				if (tempGenDate.nWeek < pEndDate->nWeek)
				{
					break;
				}
				else if (tempGenDate.nWeek > pEndDate->nWeek)
				{
					bError = true;
					break;
				}
			}

			if (pEndDate->nHour >= 0)
			{
				if (tempGenDate.nHour < pEndDate->nHour)
				{
					break;
				}
				else if (tempGenDate.nHour > pEndDate->nHour)
				{
					bError = true;
					break;
				}
			}

			if (pEndDate->nMin >= 0)
			{
				if (tempGenDate.nMin < pEndDate->nMin)
				{
					break;
				}
				else if (tempGenDate.nMin > pEndDate->nMin)
				{
					bError = true;
					break;
				}
			}

		} while (false);

		if (bError)
		{
			continue;
		}

		pMatchCfg->bInMatch = true;
		nInMatchCount++;

		//是否重新生成tag
		if (tempGenDate.nHour == pMatchCfg->nGenBaseDate.nHour &&
			tempGenDate.nDay == pMatchCfg->nGenBaseDate.nDay &&
			tempGenDate.nMonth == pMatchCfg->nGenBaseDate.nMonth &&
			tempGenDate.nYear == pMatchCfg->nGenBaseDate.nYear)
		{
			continue;
		}

		memcpy(&(pMatchCfg->nGenBaseDate), &tempGenDate, sizeof(pMatchCfg->nGenBaseDate));
		int nResetFlag = pMatchCfg->nCfgResetType;
		int nFlag = pMatchCfg->nCfgFlag;
		int nTag = 0;

		if (nFlag < 1)
		{
			nFlag = 1;
		}

		if (nResetFlag == eMatchResetFlag_Null)
		{
			nFlag = 0;
			tempGenDate.nYear = pStartDate->nYear;
			tempGenDate.nMonth = pStartDate->nMonth;
			tempGenDate.nDay = pStartDate->nDay;
			tempGenDate.nHour = pStartDate->nHour;
		}
		else if (nResetFlag == eMatchResetFlag_Hour)
		{

		}
		else if (nResetFlag == eMatchResetFlag_Day)
		{
			tempGenDate.nHour = 99;
		}
		else if (nResetFlag == eMatchResetFlag_Week)
		{
			tempGenDate.nDay = tempGenDate.nWeek;
			tempGenDate.nHour = 98;
		}
		else if (nResetFlag == eMatchResetFlag_Month)
		{
			tempGenDate.nDay = 99;
			tempGenDate.nHour = 99;
		}
		else
		{
			printf("unkown match info reset flag\r\n");
			ERROR_LOG;
			continue;
		}

		//标识
		nTag += (nFlag % 20) * 100000000;

		//年
		if (tempGenDate.nYear < 0)
		{
			tempGenDate.nYear = 99;
		}
		nTag += (tempGenDate.nYear % 100) * 1000000;
		//月
		if (tempGenDate.nMonth < 0)
		{
			tempGenDate.nMonth = 99;
		}
		nTag += tempGenDate.nMonth * 10000;
		//日
		if (tempGenDate.nDay < 0)
		{
			tempGenDate.nDay = 99;
		}
		nTag += tempGenDate.nDay * 100;
		//小时
		if (tempGenDate.nHour < 0)
		{
			tempGenDate.nHour = 99;
		}
		nTag += tempGenDate.nHour;

		pMatchCfg->nGenTag = nTag;

		if (nTag < 0)
		{
			ERROR_LOG;
			pMatchCfg->bInMatch = false;
		}
	}
//printf("nInMatchCount=%d\n",nInMatchCount);
	return nInMatchCount;
}

