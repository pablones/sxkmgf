
#ifndef __TOPLISTSYSTEM__H
#define __TOPlISTSYSTEM__H
#pragma warning(disable:4786)
#include "../../GameBaseCodeMT/I_mydb.h"
#include <vector>
using namespace std;

const int PAGENUM = 20;
const int TOPNUM = 200;

//玩家等级排行
typedef struct  
{
	char	szName[16];	
	char	szSynName[16];	
	int		nLev;
}USERLEVTOPLIST;

//富豪排行
typedef struct  
{
	char	szName[16];	
	char	szSynName[16];	
	DWORD   dwMoney;
}MONEYTOPLIST;

//属性排行
typedef struct  
{
	char	szName[16];	
	DWORD	dwProf;	
	DWORD	nData;
}ATTTOPLIST;


typedef struct
{
	USERLEVTOPLIST		LevelList[ TOPNUM ];
	ATTTOPLIST			FirJobList[ TOPNUM ];
	ATTTOPLIST			SecJobList[ TOPNUM ];
	MONEYTOPLIST		MoneyList[ TOPNUM ];
	ATTTOPLIST			PhyAttList[ TOPNUM ];
	ATTTOPLIST			MagAttList[TOPNUM];
	ATTTOPLIST			LifeList[TOPNUM];
	ATTTOPLIST			CritList[TOPNUM];
	ATTTOPLIST			HitRateList[TOPNUM];
	ATTTOPLIST			ScoreList[TOPNUM];
	ATTTOPLIST			SynMemList[TOPNUM];
	ATTTOPLIST			SynMoneyList[TOPNUM];
	USERLEVTOPLIST		EudemonScoreList[TOPNUM];
}TOPLIST;

// struct toplistMemData
// {
// 	char szName[16];
// 	char szSyn[16];
// 	unsigned int data1;
// 	unsigned int data2;
// };

enum	EM_TOPLIST_OPER
{
	GREATER  = 1,
	LESSER,
	EQUAL_OR_GREATER,
	EQUAL_OR_LESSER,
	EQUAL,
};

struct toplistMemData;
class IDatabase;
class CTopListSystem
{
public:
	CTopListSystem(); 
	virtual ~CTopListSystem();
public:
	bool	CreateData( IDatabase* pDb );
	void	Init();

	void	UpdateWorldData();
	void	OnTimer( time_t tCurr );

	const TOPLIST& QueryWorldTopList()	{return m_WorldTopList;}
	bool	GetInfo( USHORT usType,toplistMemData* ppData,int* pDataSize, USHORT nPage ); //查询某一类型信息
	
	int		GetTopListPlace(int nTopListType, int nID,const char* szName);
	BOOL	CheckTopListPlace(int nTopListType, int nID, int nOper, int nData);
	void CheckSynName(char *szSynName, int SynID, IDatabase* pDb);
private:
	TOPLIST		m_WorldTopList;
	IDatabase*	m_pDb;
	SYSTEMTIME	m_LastUpdateTime;			//上次更新时间
	HANDLE		m_UpdateCompleteEvent;		//事件对象,更新完成
	int			m_TopListUpMin;
};

extern CTopListSystem	g_TopListSystem;			//排行榜系统,全局对象

#endif //__TOPlISTSYSTEM__H