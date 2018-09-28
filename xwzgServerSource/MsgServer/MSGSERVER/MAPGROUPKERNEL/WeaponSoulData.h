// WeaponSoulData.h: interface for the CWeaponSoulData class.
//	20071112:修罗
//	武器魂
//	sk_weaponSoul
//	模仿CItemData
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPONSOULDATA_H__F46F718C_ACE3_4AEF_8421_F31485A0C9BA__INCLUDED_)
#define AFX_WEAPONSOULDATA_H__F46F718C_ACE3_4AEF_8421_F31485A0C9BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "../share/define.h"
#include "../../GameBaseCodeMT/ConstGameData.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"
#include "Myheap.h"
#include "T_SingleMap2.h"

const int EFFECTARRAY[10] = {12,21,39,69,110,165,234,319,420,538};

//与数据库同步的内存结构
enum WEAPONSOULDATA
{
	WEAPONSOULDATA_ID = 0,//ID
	WEAPONSOULDATA_ITEMID,
	WEAPONSOULDATA_TYPE,
	WEAPONSOULDATA_LEVEL,//等级
	WEAPONSOULDATA_EXP,
	WEAPONSOULDATA_LIFESPAN,
	WEAPONSOULDATA_GROW,
	WEAPONSOULDATA_MIXLEV,
	WEAPONSOULDATA_PHYATTLEV,
	WEAPONSOULDATA_MAGATTLEV,
	WEAPONSOULDATA_BASESTR,
	WEAPONSOULDATA_BASEINT,
	WEAPONSOULDATA_BASESTA,
	WEAPONSOULDATA_BASESPI,
	WEAPONSOULDATA_BASEAGI,
	WEAPONSOULDATA_ADDTION0,
	WEAPONSOULDATA_ADDTION1,
	WEAPONSOULDATA_ADDTION2,
	WEAPONSOULDATA_ADDTION3,
	WEAPONSOULDATA_ADDTION4,
	WEAPONSOULDATA_ADDTION5,
	WEAPONSOULDATA_ADDTION6,
	WEAPONSOULDATA_ADDTION7,
	WEAPONSOULDATA_ADDTION8,
	WEAPONSOULDATA_ADDTION9,
	WEAPONSOULDATA_SKILL1,
	WEAPONSOULDATA_SKILL2,
	WEAPONSOULDATA_SKILL3,
};
char	szWeaponSoulTable[];
typedef	CGameData<WEAPONSOULDATA,szWeaponSoulTable,szID> CGameDataWeaponSoul;

struct ST_WEAPONSOULDATA
{
	OBJID ID;//ID
	OBJID idItem;
	UINT  nType;
	UINT  level;//等级
	UINT  exp;
	UINT  lifespan;
	UINT  grow;
	UINT  mixlev;
	UINT  phyattlev;
	UINT  magattlev;
	UINT  basestr;
	UINT  baseint;
	UINT  basesta;
	UINT  basespi;
	UINT  baseagi;
	UINT  addition[10];
	UINT  skill[3];
};

enum WEAPONSOULUPDATE
{
//	WEAPONSOULUPDATE_MIXLEV = 1,
	WEAPONSOULUPDATE_PROPOPEN = 1,
	WEAPONSOULUPDATE_PROPRAND,
	WEAPONSOULUPDATE_PROPLEVUP,
	WEAPONSOULUPDATE_EXP,
	WEAPONSOULUPDATE_SKILLRAND,
	WEAPONSOULUPDATE_SKILLLEVUP,
	WEAPONSOULUPDATE_TYPERAND,
	WEAPONSOULUPDATE_GROW,
};
struct stRandData
{
	int nChange;
	int nData;
};


struct stRandBuffData
{
	int nBuff;
	int nPow;
	int nSec;
	int nTime;
};

class CWeaponSoulData  
{
public:
	CWeaponSoulData();
	virtual ~CWeaponSoulData();
	
public:
 	bool		Create			(IDatabase* pDb, IRecord* pDefault,const ST_WEAPONSOULDATA* pInfo,bool bInsert=true,OBJID idNew=ID_NONE);	
	bool		Create			(OBJID idWeaponSoul, IDatabase* pDb);
	bool		DeleteRecord	(IDatabase* pDb);
	IRecord*	GetRecord()		{ ASSERT(m_pData); return m_pData->GetRecord(); }
	ULONG		Release			(){ delete this; return 0; }

public: // info
	bool		GetInfo	(ST_WEAPONSOULDATA& pInfo,bool bCount = true);
	static bool	GetInfoByDefault(ST_WEAPONSOULDATA& pInfo,int nType = 0);
	bool		SaveInfo		(void);
	bool		UpdateAll(ST_WEAPONSOULDATA wpslInfo,WEAPONSOULUPDATE nType =WEAPONSOULUPDATE_PROPOPEN,int nData = 0);
public: // get set
	OBJID		GetID()					{ return m_pData->GetKey(); }

	int			GetInt(WEAPONSOULDATA idx);
	LPCTSTR		GetStr(WEAPONSOULDATA idx);
	void		SetInt(WEAPONSOULDATA idx, int nData, bool bUpdate = false);
	void		SetStr(WEAPONSOULDATA idx, LPCTSTR szData, int nSize, bool bUpdate = false);

	int			ProcGift1001(int nType, int oldValue);
	int			ProcGift1002(int nType, int oldValue);
	int			ProcGift1003(stRandBuffData& stData);
	int			ProcGift1004(int nType, int oldValue,stRandData& stData);
protected:
	CGameDataWeaponSoul* m_pData;
};

#endif // !defined(AFX_WEAPONSOULDATA_H__F46F718C_ACE3_4AEF_8421_F31485A0C9BA__INCLUDED_)
