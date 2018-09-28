#ifndef _BASEFUNC_H
#define _BASEFUNC_H

#pragma	warning(disable:4786)
#include <time.h>
#include "common.h"
#include "BaseType.h"
#include <vector>
using namespace std;
#include "AutoPtr.h"

//20070627����:��־���ر�־λ
extern BOOL g_bLogSave;
extern char g_szLogMemFile[];
extern char g_szLogMemDb[];

extern BOOL g_bEnableFatigue;
//  [2007-10-8] ���	��������д�������ݿ��־λ
extern BOOL g_bItemLifeTimeSaveDB;

//  [1/15/2008 ���]	��������
extern BOOL g_bEnableEquipCreate;	//	װ������
extern BOOL g_bEnableTopListRefresh;	//	�������а�
extern BOOL g_bEnableSynModificate;		//	���ξ�����Ƹ�뿪������
extern BOOL g_bEnableDatabaseReconnect;	//	���ݿ�ÿ���Զ�����

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
	TIME_EPOCH_DAY,//20070402����:ʱ���.��ǰʱ�����ο�ʱ��������
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
						 LPCSTR strServerName="���",
						 LPCSTR strEventSource="�߼�����",
						 LPCSTR strEventName="�������",
						 LPCSTR strCurrency="�ƽ�",
						 LPCSTR strActiveObj="�����",
						 LPCSTR strPassiveObj="С�մ�",
						 LPCSTR strValue1="N/A",
						 LPCSTR strValue2="N/A",
						 LPCSTR strValue3="N/A",
						 LPCSTR strValue4="N/A",
						 LPCSTR strValue5="N/A",
						 LPCSTR strValue6="N/A"
					);

//////////////////////////////////////////////////////////////////////////
//	20070709 ��� ��¼������Ʒ����
void	MyLogSaveCostlyItem(const char* pszServerName,
							int	nAccountID, int nCharID, const char* pszCharName,
							int nOtherAccountID, int nOtherCharID, const char* pszOtherCharName,
							int nType, int nItemTypeID, int	nItemID, const char* pszItemName, int nItemCount);

//[��;���� 2009.06.10]�ʲ�������־
//////////////////////////////////////////////////////////////////////////
enum
{
	NONE = 0,
	BOOTH_SALE,//��̯��
	BOOTH_BUY,//��̯����
	BANGPAI_HONGBAO,//�����
	TRADE_ONE,//����1
	TRADE_TWO,//����2
	SHOP_ADDBUY,//��Ӽ�����Ϣ
	SHOP_SALE,//���̹�����Ʒ
	SHOP_ADDSALE,//��ӳ�����Ʒ
	SHOP_BUY,//�����չ�
	SHOP_DEL_SALE,//�˻س�����Ʒ
	SHOP_DEL_BUY,//�˻��չ���Ʒ
	SHOP_PAIMAI_TUIHUI,//�˻�����Ǯ
	SHOP_PAIMAI,//����
	SHOP_XUZU,//��������
	SHOP_TIQU,//������ȡ
	MAIL_YIBAN,//�ʼ���־
	MAIL_JIAOYI,//�ʼ���־
	MAIL_TUIHUI,//�ʼ���־
};

struct STPROPERTYSTREAMLOG
{
	DWORD	idSource;//Դ��ʶ
	DWORD	idSAccount;
	DWORD	dwSVas;//Դ��ǰVAS
	DWORD	dwSMoney;//Դ��ǰ��Ϸ��
	DWORD	idTarget;//Ŀ���ʶ
	DWORD	idTAccount;
	DWORD	dwTVas;//Դ��ǰVAS
	DWORD	dwTMoney;//Դ��ǰ��Ϸ��
	DWORD	dwVas;//����VAS
	DWORD	dwMoney;//��������
	DWORD	idItemType;
	DWORD	idSItem;//������Ʒ��ʶ
	DWORD	dwSNumber;//������Ʒ��ʶ
	DWORD	idTItem;//������Ʒ��ʶ
	DWORD	dwTNumber;//������Ʒ��ʶ
	DWORD	dwSysVas;
	DWORD	dwSysMoney;
	DWORD	dwType;//����
	
	STPROPERTYSTREAMLOG()
	{
		idSource = NULL;//Դ��ʶ
		idSAccount = NULL;
		dwSVas = NULL;//Դ��ǰVAS
		dwSMoney = NULL;//Դ��ǰ��Ϸ��
		idTarget = NULL;//Ŀ���ʶ
		idTAccount = NULL;
		dwTVas = NULL;//Դ��ǰVAS
		dwTMoney = NULL;//Դ��ǰ��Ϸ��
		dwVas = NULL;//����VAS
		dwMoney = NULL;//��������
		idItemType = NULL;
		idSItem = NULL;//������Ʒ��ʶ
		dwSNumber = NULL;//������Ʒ��ʶ
		idTItem = NULL;//������Ʒ��ʶ
		dwTNumber = NULL;//������Ʒ��ʶ
		dwSysVas = NULL;
		dwSysMoney = NULL;
		dwType = NULL;//����
	}
};

void PropertyStreamLog(STPROPERTYSTREAMLOG &stProStream);
//////////////////////////////////////////////////////////////////////////

//20071212����:����ת��.nConvNum:2~9.
//��6����ת��.
//nConvNum = 6
//nTrainPoint = 7
//����ֵΪ11.
int NumConversion(int nTrainPoint,int nConvNum);


const	int	MY_LOG_SAVE_COSTLY_ITEM_TO_DISK = 1;	//	=1Ϊд�ļ�,0Ϊд���ݿ�(��ʵ��)

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

//#define _TIMECOSTCHECK///����ʱ��ɱ���⿪��

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
	const char* m_pszReason;///ԭ��
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

inline int AdditionPower(int nPower1,int nPower2)///��Powerֵ���мӳ�.softworms-2007-09-05
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
		 LOGERROR("Power�ӳɴ���,ǰһ���ǰٷֶ�,��һ������ֵ:%d/%d",nPower1,nPower2);
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

