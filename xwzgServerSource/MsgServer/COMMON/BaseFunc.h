#ifndef _BASEFUNC_H
#define _BASEFUNC_H

#pragma	warning(disable:4786)
#include <time.h>
#include "common.h"
#include "BaseType.h"
#include <vector>
using namespace std;
#include "AutoPtr.h"

//20070627修罗:日志开关标志位
extern BOOL g_bLogSave;
extern char g_szLogMemFile[];
extern char g_szLogMemDb[];

extern BOOL g_bEnableFatigue;
//  [2007-10-8] 朱斌	生命周期写更新数据库标志位
extern BOOL g_bItemLifeTimeSaveDB;

//  [1/15/2008 朱斌]	新增参数
extern BOOL g_bEnableEquipCreate;	//	装备打造
extern BOOL g_bEnableTopListRefresh;	//	屏蔽排行榜
extern BOOL g_bEnableSynModificate;		//	屏蔽军团招聘与开除功能
extern BOOL g_bEnableDatabaseReconnect;	//	数据库每天自动重连

struct stLogData 
{
	unsigned short unFirst;
	unsigned short unLast;
	char arrayFile[100][128];
	char arrayLog[100][1024];
};

struct stLogData_db
{
	unsigned short unFirst;
	unsigned short unLast;
	unsigned short usOp[100];
	unsigned short usOpType[100];
	unsigned short usGetType[100];
	unsigned short usNum[100];
	char szTarget[100][32];
	DWORD idTarget[100];
	char szOwner[100][32];
	DWORD idOwner[100];
	char szTime[100][64];
	char szMsg[100][512];
};

struct stDataSave_db
{
	unsigned short usOp;
	unsigned short usOpType;
	unsigned short usGetType;
	unsigned short usNum;
	char szTarget[32];
	DWORD idTarget;
	char szOwner[32];
	DWORD idOwner;
	char szTime[64];
	char szMsg[512];
};

enum{
	OPDATA_ITEM = 1,
	OPDATA_MONEY,
	OPDATA_EUDEMON,
	OPDATA_TREASURE,
};

enum{
	OPTYPEDATA_TRADEOUT = 1,
	OPTYPEDATA_TREASURETYPE = 1, 
	OPTYPEDATA_DROPOUT,
	OPTYPEDATA_DONATEOUT,
	OPTYPEDATA_USED,
	OPTYPEDATA_KILLDROP,
	OPTYPEDATA_BOOTHOUT,
	OPTYPEDATA_GET,
	OPTYPEDATA_UPDATE,
};

enum{
	OPGETTYPE_PICK = 1,
	OPGETTYPE_SHOP,
	OPGETTYPE_TRADE,
	OPGETTYPE_MAKE,
	OPGETTYPE_TREASURE,
	OPGETTYPE_VASSHOP,
	OPGETTYPE_PETEGG,
};

enum{
	OPUPDATETYPE_PACKAGEIN = 1,
	OPUPDATETYPE_PACKAGEOUT,
	OPUPDATETYPE_ADDSTAR,
	OPUPDATETYPE_ADDLEV,
	OPUPDATETYPE_MAKEHOLE,
	OPUPDATETYPE_EM,
	OPUPDATETYPE_EMOUT,
	OPUPDATETYPE_TRANS,
	OPUPDATETYPE_INTUP,
	OPUPDATETYPE_FUSUP,
	OPUPDATETYPE_UNDUP,
	OPUPDATETYPE_CHANGESEX,
	OPUPDATETYPE_SKILL,
	OPUPDATETYPE_BABY,
};


bool log_SaveByMemfile(const char * pFilename, const char * pBuf);
bool log_SaveByMemDb(stDataSave_db stData);
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
//DWORD	TimeGet(void);
DWORD	SysTimeGet(void);
DWORD	SysTimeGetEx(void);

inline long	GetClock()		{ return clock(); }
int		RandGet		(int nMax, BOOL bReset=false);
void	DDALineEx(int x0, int y0, int x1, int y1, vector<POINT>& vctPoint);

void	LogSave		(const char* fmt, ...);
void	MyLogSave	(const char* pszName, const char* fmt, ...);
void	MyLogSave2	(const char* pszName, const char* fmt, ...);

void    FinanceLog	(
						 LPCSTR strServerName="皓月",
						 LPCSTR strEventSource="高级店铺",
						 LPCSTR strEventName="购买道具",
						 LPCSTR strCurrency="黄金",
						 LPCSTR strActiveObj="送你爹",
						 LPCSTR strPassiveObj="小苏打",
						 LPCSTR strValue1="N/A",
						 LPCSTR strValue2="N/A",
						 LPCSTR strValue3="N/A",
						 LPCSTR strValue4="N/A",
						 LPCSTR strValue5="N/A",
						 LPCSTR strValue6="N/A"
					);

//////////////////////////////////////////////////////////////////////////
//	20070709 朱斌 记录贵重物品流向
void	MyLogSaveCostlyItem(const char* pszServerName,
							int	nAccountID, int nCharID, const char* pszCharName,
							int nOtherAccountID, int nOtherCharID, const char* pszOtherCharName,
							int nType, int nItemTypeID, int	nItemID, const char* pszItemName, int nItemCount);

//[游途道标 2009.06.10]资产流向日志
//////////////////////////////////////////////////////////////////////////
enum
{
	NONE = 0,
	BOOTH_SALE,//摆摊卖
	BOOTH_BUY,//摆摊购买
	BANGPAI_HONGBAO,//发红包
	TRADE_ONE,//交易1
	TRADE_TWO,//交易2
	SHOP_ADDBUY,//添加寄售信息
	SHOP_SALE,//店铺购买物品
	SHOP_ADDSALE,//添加出售物品
	SHOP_BUY,//店铺收购
	SHOP_DEL_SALE,//退回出售物品
	SHOP_DEL_BUY,//退回收购物品
	SHOP_PAIMAI_TUIHUI,//退回拍卖钱
	SHOP_PAIMAI,//拍卖
	SHOP_XUZU,//店铺续租
	SHOP_TIQU,//店铺提取
	MAIL_YIBAN,//邮件日志
	MAIL_JIAOYI,//邮件日志
	MAIL_TUIHUI,//邮件日志
};

struct STPROPERTYSTREAMLOG
{
	DWORD	idSource;//源标识
	DWORD	idSAccount;
	DWORD	dwSVas;//源当前VAS
	DWORD	dwSMoney;//源当前游戏币
	DWORD	idTarget;//目标标识
	DWORD	idTAccount;
	DWORD	dwTVas;//源当前VAS
	DWORD	dwTMoney;//源当前游戏币
	DWORD	dwVas;//流动VAS
	DWORD	dwMoney;//流动货币
	DWORD	idItemType;
	DWORD	idSItem;//流动物品标识
	DWORD	dwSNumber;//流动物品标识
	DWORD	idTItem;//流动物品标识
	DWORD	dwTNumber;//流动物品标识
	DWORD	dwSysVas;
	DWORD	dwSysMoney;
	DWORD	dwType;//类型
	
	STPROPERTYSTREAMLOG()
	{
		idSource = NULL;//源标识
		idSAccount = NULL;
		dwSVas = NULL;//源当前VAS
		dwSMoney = NULL;//源当前游戏币
		idTarget = NULL;//目标标识
		idTAccount = NULL;
		dwTVas = NULL;//源当前VAS
		dwTMoney = NULL;//源当前游戏币
		dwVas = NULL;//流动VAS
		dwMoney = NULL;//流动货币
		idItemType = NULL;
		idSItem = NULL;//流动物品标识
		dwSNumber = NULL;//流动物品标识
		idTItem = NULL;//流动物品标识
		dwTNumber = NULL;//流动物品标识
		dwSysVas = NULL;
		dwSysMoney = NULL;
		dwType = NULL;//类型
	}
};

void PropertyStreamLog(STPROPERTYSTREAMLOG &stProStream);
//////////////////////////////////////////////////////////////////////////

//20071212修罗:进制转换.nConvNum:2~9.
//如6进制转换.
//nConvNum = 6
//nTrainPoint = 7
//返回值为11.
int NumConversion(int nTrainPoint,int nConvNum);


const	int	MY_LOG_SAVE_COSTLY_ITEM_TO_DISK = 1;	//	=1为写文件,0为写数据库(不实现)

void	DebugSave	(const char* fmt, ...);
void	GmLogSave	(const char* fmt, ...);
void	MsgLogSave	(const char* pszFileName, const char* fmt, ...);

BOOL	StringCheck	(char* pszString, BOOL bNewLineEnable=false);
BOOL	TalkStrCheck(char* pszString, BOOL bNewLineEnable=false);
BOOL	DisableWordCheck(char* pszString, BOOL bNewLineEnable=false);
BOOL	NameStrCheck(char* pszString);

double	RandomRateGet	(double dRange);
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
// 			if (m_pszStr)
				::LogSave("CTimeCostChk Overtime (%u) when szReason:%s chk:%s", tmCost,m_pszReason,m_pszStr); 
// 			else
// 				::LogSave("CTimeCostChk Overtime (%u)", tmCost); 
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

/////////////////////////////////////////////////////////////////////////////
inline unsigned int	ExchangeShortBits(unsigned long nData, int nBits)
{
	ASSERT(nBits >= 0 && nBits < 16);
	nData	&= 0xFFFF;
	return ((nData>>nBits) | (nData<<(16-nBits))) & 0xFFFF;
}

inline int AdditionPower(int nPower1,int nPower2)///对Power值进行加成.softworms-2007-09-05
{
	if (nPower1>=30000 && nPower2>=30000)
	{
		nPower1=30000+(float)(nPower1-30000)*(float)(nPower2-30000)/(float)100;
	}
	else if(nPower1<30000 && nPower2>=30000)
	{
		nPower1*=(float)(nPower2-30000)/(float)100;
	}
	else if(nPower1>=30000 && nPower2<30000)
	{
		 LOGERROR("Power加成错误,前一个是百分多,后一个是数值:%d/%d",nPower1,nPower2);
	}
	else
		nPower1+=(float)nPower2;
	return nPower1;
}

inline unsigned int	ExchangeLongBits(unsigned long nData, int nBits)
{
	ASSERT(nBits >= 0 && nBits < 32);
	return (nData>>nBits) | (nData<<(32-nBits));
}

//////////////////////////////////////////////////////////////////////
inline int pos2index(int x, int y, int cx, int cy) { return (x + y*cx); }
inline int index2x(int idx, int cx, int cy) { return (idx % cy); }
inline int index2y(int idx, int cx, int cy) { return (idx / cy); }
inline void pos2lt(POINT* pTarget, const POINT& pos, int nRange) { pTarget->x = pos.x - nRange; pTarget->y = pos.y - nRange; }
inline void posadd(POINT* pTarget, int nOffsetX, int nOffsetY) { pTarget->x += nOffsetX; pTarget->y += nOffsetY; }
inline void possub(POINT* pTarget, int nOffsetX, int nOffsetY) { pTarget->x -= nOffsetX; pTarget->y -= nOffsetY; }


#endif

