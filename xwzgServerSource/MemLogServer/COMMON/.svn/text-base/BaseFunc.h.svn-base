#ifndef _BASEFUNC_H
#define _BASEFUNC_H

#pragma	warning(disable:4786)
#include <time.h>
#include "common.h"
#include "BaseType.h"
#include <vector>
using namespace std;
#include "AutoPtr.h"

//日志开关标志位
extern BOOL g_bLogSave;

String	DumpBuffer(const char* buf, int nSize);
void	DateTime(char * buf20, time_t tInput = 0);		// 0 : curr time
bool	FormatDateTime(char* buf, const char* szFormat, time_t tInput = 0);		// szFormat: "%04d-%02d-%02d %02d:%02d:%02d"
void	DateTimeStamp(char * buf15, time_t tInput = 0);		// 0 : curr time
int		DateStamp(time_t tTime=0);
int		DateStampPass(int nDate, int nDays);	// return -1: error
bool	IsActiveTime(time_t tCurr, unsigned long nFormat);
bool	IsTimeRange(unsigned long nBegin, unsigned long nEnd, time_t tCurr=0);
inline long	GetCurrTime()				{ return time(NULL); }
char*	StrStrCh(const char* string, const char* strCharSet);

enum TIME_TYPE{
	TIME_MILLISECOND=0, 
	TIME_SECOND, 
	TIME_MINUTE, 
	TIME_DAY, 
	TIME_DAYTIME, 
	TIME_STAMP,
	TIME_EPOCH_DAY,//20070402修罗:时间点.当前时间距离参考时间点的天数
};

DWORD	TimeGet(TIME_TYPE type=TIME_MILLISECOND);
DWORD	SysTimeGet(void);
DWORD	SysTimeGetEx(void);

inline long	GetClock()		{ return clock(); }
int		RandGet		(int nMax, BOOL bReset=false);

void	LogSave		(const char* fmt, ...);
void	MyLogSave	(const char* pszName, const char* fmt, ...);


BOOL	StringCheck	(char* pszString, BOOL bNewLineEnable=false);
BOOL	TalkStrCheck(char* pszString, BOOL bNewLineEnable=false);
BOOL	DisableWordCheck(char* pszString, BOOL bNewLineEnable=false);
BOOL	NameStrCheck(char* pszString);

int		Double2Int		(double dValue);

inline int	AddToTop(int nData, int nAdd, int nTop) { int res=nData+nAdd; if(nAdd>0 && res<nData || res>nTop) return nTop; else return res; }

//#define _TIMECOSTCHECK///定义时间成本检测开启

/////////////////////////////////////////////////////////////////////////////
class CTimeCostChk
{
public:
	CTimeCostChk(const char* szReason,const char* pszStr, DWORD tmChk) : m_tmChk(tmChk) 
	{
		m_tmBegin	= GetTickCount();
		m_pszStr	= pszStr;
		m_pszReason =szReason;
	}
	
	~CTimeCostChk(void)
	{
		DWORD tmCost = GetTickCount()-m_tmBegin;
		if (tmCost > m_tmChk)
		{
			::LogSave("CTimeCostChk Overtime (%u) when szReason:%s chk:%s", tmCost,m_pszReason,m_pszStr); 
		}
	}

private:
	DWORD m_tmChk;
	const char* m_pszStr;
	const char* m_pszReason;///原因
	DWORD m_tmBegin;
};
#ifdef _TIMECOSTCHECK
	#define TimeCostChk(x,y) CTimeCostChk chk(x,y,100)
#else
	#define TimeCostChk(x,y)
#endif

/////////////////////////////////////////////////////////////////////////////
template<TYPENAME T>
inline T	CutOverflow(T x, T y) { return (x <= y) ? x : y; }
template<TYPENAME T>
inline T	CutTrail(T x, T y) { return (x >= y) ? x : y; }
template<TYPENAME T>
inline T	CutRange(T n, T min, T max) { return (n<min) ? min : ((n>max) ? max : n); }

//////////////////////////////////////////////////////////////////////
inline int pos2index(int x, int y, int cx, int cy) { return (x + y*cx); }
inline int index2x(int idx, int cx, int cy) { return (idx % cy); }
inline int index2y(int idx, int cx, int cy) { return (idx / cy); }
#endif

