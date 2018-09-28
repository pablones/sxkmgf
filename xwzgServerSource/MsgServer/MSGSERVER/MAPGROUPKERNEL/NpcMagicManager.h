// NpcMagicManager.h: interface for the CNpcMagicManager class.
//	20080114:修罗
//	怪物技能管理器
//	技能依monstertype存放在map里
//	sk_monstermagic
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCMAGICMANAGER_H__DF54D1FE_B48F_4CB9_A1CE_6D7A1957B806__INCLUDED_)
#define AFX_NPCMAGICMANAGER_H__DF54D1FE_B48F_4CB9_A1CE_6D7A1957B806__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NpcMagicData.h"
#include <map>

typedef vector<CNpcMagicData*> VCT_NPCMAGICDATA;//特定type的magic集合
typedef map<OBJID,VCT_NPCMAGICDATA*> MAP_NPCMAGICDATA;//所有type的magic集合

typedef vector<CEudemonMagicData*> VCT_EUDEMONMAGICDATA;//特定type的magic集合
typedef vector<CSoulMagicData*> VCT_SOULMAGICDATA;

const int _DEFAULT_MONSTERMAGICLEV = 0;//怪物技能默认级别

class CNpcMagicManager  
{
public:
	CNpcMagicManager();
	virtual ~CNpcMagicManager();

	static CNpcMagicManager*	CreateNew	()		{ return new CNpcMagicManager; }

	BOOL CreateData( IDatabase* pDb );//从数据库中读数据 

	const VCT_NPCMAGICDATA* QueryDataByOwnerType(OBJID monsterType);//通过monsterType得到magic的集合
	const CNpcMagicData* QueryData(OBJID monsterType,
		OBJID magicType);//通过monstertype和magicType来获得具体技能信息

	CSoulMagicData* QuerySoulData(int id);
	CSoulMagicData* QuerySoulDataByType(int nType);
	
	CEudemonMagicData* QueryEudemonData(int id);
	int QueryEudemonSkill(int skillType);
	CEudemonMagicData* QueryEudemonDataByType(int nType);
	int GetEudSkillIDByType(int skillType);
private:

	MAP_NPCMAGICDATA m_mNpcMagicData;
	VCT_EUDEMONMAGICDATA m_mEudemonMagicData;
	VCT_SOULMAGICDATA m_mSoulMagicData;
};

//全局
extern CNpcMagicManager*	g_pNpcMagicManager;


#endif // !defined(AFX_NPCMAGICMANAGER_H__DF54D1FE_B48F_4CB9_A1CE_6D7A1957B806__INCLUDED_)
