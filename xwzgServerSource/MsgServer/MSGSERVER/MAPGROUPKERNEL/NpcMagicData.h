// NpcMagicData.h: interface for the CNpcMagicData class.
//	20080112:修罗
//	npc魔法
//	sk_monstermagic
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCMAGICDATA_H__45367F16_52DE_49DF_91F0_D2EB93EF3F1C__INCLUDED_)
#define AFX_NPCMAGICDATA_H__45367F16_52DE_49DF_91F0_D2EB93EF3F1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameObj.h"
#include "GameData.h"


//////////////////////////////////////////////////////////////////////
enum EM_NPCMAGICDATA
{
	EM_NPCMAGICDATA_id	= 0,
	EM_NPCMAGICDATA_ownertype,
	EM_NPCMAGICDATA_magictype, 
	EM_NPCMAGICDATA_magiclev, 
	EM_NPCMAGICDATA_magictypekind,
};

char	szNpcMagic[];
typedef CGameData<EM_NPCMAGICDATA,szNpcMagic,szID>	CGameDataNpcMagic;

class CNpcMagicData  
{
	//常规函数及成员---------------------------------------
public:
	CNpcMagicData();
	virtual ~CNpcMagicData();
	bool		Create			(OBJID id, IDatabase* pDb);
	bool		Create			(IRecordset* pRes);
	bool		DeleteRecord	(IDatabase* pDb);
	IRecord*	GetRecord()		{ ASSERT(m_pData); return m_pData->GetRecord(); }
	ULONG		Release			(){ delete this; return 0; }
	//into
	bool		SaveInfo		(void);
public: // get set
	OBJID		GetID()					{ return m_pData->GetKey(); }
	int			GetInt(EM_NPCMAGICDATA idx);
	LPCTSTR		GetStr(EM_NPCMAGICDATA idx);
	void		SetInt(EM_NPCMAGICDATA idx, int nData, bool bUpdate = false);
	void		SetStr(EM_NPCMAGICDATA idx, LPCTSTR szData, int nSize, bool bUpdate = false);
protected: // database
	CGameDataNpcMagic*		m_pData;
	//常规函数及成员end-----------------------------------

};

enum EM_EUDEMONMAGIC
{
	EM_EUDEMONMAGIC_id	= 0,
	EM_EUDEMONMAGIC_type,
	EM_EUDEMONMAGIC_dataID, 
	EM_EUDEMONMAGIC_nextLev,
	EM_EUDEMONMAGIC_basicLev,
	EM_EUDEMONMAGIC_character,
	EM_EUDEMONMAGIC_affect,
	EM_EUDEMONMAGIC_uplevitem,
};

char	szEudemonMagic[];
typedef CGameData<EM_EUDEMONMAGIC,szEudemonMagic,szID>	CGameDataEudemonMagic;

class CEudemonMagicData  
{
	//常规函数及成员---------------------------------------
public:
	CEudemonMagicData();
	virtual ~CEudemonMagicData();
	bool		Create			(OBJID id, IDatabase* pDb);
	bool		Create			(IRecordset* pRes);
	bool		DeleteRecord	(IDatabase* pDb);
	IRecord*	GetRecord()		{ ASSERT(m_pData); return m_pData->GetRecord(); }
	ULONG		Release			(){ delete this; return 0; }
	//into
	bool		SaveInfo		(void);
public: // get set
	OBJID		GetID()					{ return m_pData->GetKey(); }
	int			GetInt(EM_EUDEMONMAGIC idx);
	LPCTSTR		GetStr(EM_EUDEMONMAGIC idx);
	void		SetInt(EM_EUDEMONMAGIC idx, int nData, bool bUpdate = false);
	void		SetStr(EM_EUDEMONMAGIC idx, LPCTSTR szData, int nSize, bool bUpdate = false);
protected: // database
	CGameDataEudemonMagic*		m_pData;
	//常规函数及成员end-----------------------------------
	
};


enum EM_SOULMAGIC
{
	EM_SOULMAGIC_id	= 0,
	EM_SOULMAGIC_type,
	EM_SOULMAGIC_dataID, 
	EM_SOULMAGIC_countLev,
	EM_SOULMAGIC_Levparam,
	EM_SOULMAGIC_mixparam,
};

char	szSoulMagic[];
typedef CGameData<EM_SOULMAGIC,szSoulMagic,szID>	CGameDataSoulMagic;

class CSoulMagicData  
{
	//常规函数及成员---------------------------------------
public:
	CSoulMagicData();
	virtual ~CSoulMagicData();
	bool		Create			(OBJID id, IDatabase* pDb);
	bool		Create			(IRecordset* pRes);
	bool		DeleteRecord	(IDatabase* pDb);
	IRecord*	GetRecord()		{ ASSERT(m_pData); return m_pData->GetRecord(); }
	ULONG		Release			(){ delete this; return 0; }
	//into
	bool		SaveInfo		(void);
public: // get set
	OBJID		GetID()					{ return m_pData->GetKey(); }
	int			GetInt(EM_SOULMAGIC idx);
	LPCTSTR		GetStr(EM_SOULMAGIC idx);
	void		SetInt(EM_SOULMAGIC idx, int nData, bool bUpdate = false);
	void		SetStr(EM_SOULMAGIC idx, LPCTSTR szData, int nSize, bool bUpdate = false);
protected: // database
	CGameDataSoulMagic*		m_pData;
	//常规函数及成员end-----------------------------------
	
};
#endif // !defined(AFX_NPCMAGICDATA_H__45367F16_52DE_49DF_91F0_D2EB93EF3F1C__INCLUDED_)
