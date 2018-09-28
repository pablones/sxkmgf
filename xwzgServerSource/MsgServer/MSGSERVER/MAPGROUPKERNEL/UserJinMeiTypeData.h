// UserJinMeiTypeData.h: interface for the CUserJinMeiTypeData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERJINMEITYPEDATA_H__D68D6D68_5DF7_49E9_A247_C0EAB6D66294__INCLUDED_)
#define AFX_USERJINMEITYPEDATA_H__D68D6D68_5DF7_49E9_A247_C0EAB6D66294__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include <string>
#include "../share/define.h"
#include "../../GameBaseCodeMT/ConstGameData.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"
#include "T_SingleObjSet2.h"
using namespace std;

enum JINMEITYPEDATA
{
	JINMEITYPEDATA_ID_=0,
	JINMEITYPEDATA_TYPE,
    JINMEITYPEDATA_LEV,
    JINMEITYPEDATA_REQLEV,
	JINMEITYPEDATA_REQMONEY,
	JINMEITYPEDATA_REQEXP,
	JINMEITYPEDATA_REQMAGICLEV,
	JINMEITYPEDATA_NEEDTIME,
	JINMEITYPEDATA_DATA,
};

char	szJinmeiTypeTable[];
typedef	CGameData<JINMEITYPEDATA, szJinmeiTypeTable, szID>	CGameJinmeiTypeData;


enum
{
	USERJINMEITYPE_UNKOWN = 0,
	USERJINMEITYPE_YANGQIAO,
	USERJINMEITYPE_YINGQIAO,
	USERJINMEITYPE_YANGWEI,
	USERJINMEITYPE_YINGWEI,
	USERJINMEITYPE_DAIMEI,
	USERJINMEITYPE_CHONGMEI,
	USERJINMEITYPE_DUMEI,
	USERJINMEITYPE_RENMEI,
};

//经脉修炼状态
enum 
{
	LEARNSTATU_STOP = 0,
	LEARNSTATU_WAIT,
	LEARNSTATU_PROCESS,
	LEARNSTATU_COMPELETE,
};

//经脉升级类型
enum 
{
	UPJINMEILEV_PART = 0, // 经脉升级加速
	UPJINMEILEV_SUC,
	UPJINMEILEV_FAIL,
};

enum
{
	ADDWAITQUEUE_FAIL = 0, //加入等待队列失败
	ADDWAITQUEUE_SUC,      //加入等待队列成功
	STARTLEARN_FAIL,       //开始修炼失败
	STARTLEARN_SUC,        //开始修炼成功
	UPLEV_FAIL,            //升级失败
	UPLEV_SUC,             //升级成功
	UPLEV_PART,            //经脉加速
	STRENGTHJINMEI_FAIL,   //强化失败
	STRENGTHJINMEI_SUC,    //强化成功
	MOVEWAITQUEUE_FAIL,    //移除失败
	MOVEWAITQUEUE_SUC,     //移除成功
};
//提升经脉等级的消耗类型
enum
{
	UPJINMEILEV_TIME = 0,
	UPJINMEILEV_EMONEY,
};

//经脉和经脉强化达到等级
enum
{
	JINMEIACHIEVE_LEV = 1,
    JINMEISTENGTHACHIEVE = 2,
};


const int MAX_JINMEI_LEV = 90;
const int MAX_JINMEI_NUM = 8;
const int MAX_JINMEI_STRENGTHLEV = 10;
const int CONSUME_VALUE_PER_MIN = 2;

const char szJinmeiName[MAX_JINMEI_NUM][16] = {"阳跷", "阴跷", "阳维", "阴维", "带脉", "冲脉", "督脉", "任脉"};

class CJinmeiTypeData
{
public:
	CJinmeiTypeData()			{ m_pData = NULL; }
	virtual ~CJinmeiTypeData()	{ if (m_pData) SAFE_RELEASE (m_pData); }

public:
	static CJinmeiTypeData*	CreateNew	()	{ return new CJinmeiTypeData; }
	ULONG	Release			()				{ delete this; return 0; }
	ULONG	ReleaseByOwner	()				{ delete this; return 0; }
	bool	Create			(IRecordset* pRes);

public: // get set
	OBJID		GetID()					{ CHECKF(m_pData); return m_pData->GetKey(); }

	int		GetInt(JINMEITYPEDATA idx)	{ CHECKF(m_pData); return m_pData->GetInt(idx); }
	LPCTSTR	GetStr(JINMEITYPEDATA idx)	{ CHECKF(m_pData); return m_pData->GetStr(idx); } 

protected:
	CGameJinmeiTypeData*		m_pData;
};

//////////////////////////////////////////////////////////////////////

typedef	ISingleObjSet2<CJinmeiTypeData>		IJinmeiTypeSet;
typedef	CSingleObjSet2<CJinmeiTypeData>		CJinmeiTypeSet;
#endif // !defined(AFX_USERJINMEITYPEDATA_H__D68D6D68_5DF7_49E9_A247_C0EAB6D66294__INCLUDED_)
