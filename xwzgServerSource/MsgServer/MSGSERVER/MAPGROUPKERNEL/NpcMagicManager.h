// NpcMagicManager.h: interface for the CNpcMagicManager class.
//	20080114:����
//	���＼�ܹ�����
//	������monstertype�����map��
//	sk_monstermagic
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCMAGICMANAGER_H__DF54D1FE_B48F_4CB9_A1CE_6D7A1957B806__INCLUDED_)
#define AFX_NPCMAGICMANAGER_H__DF54D1FE_B48F_4CB9_A1CE_6D7A1957B806__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NpcMagicData.h"
#include <map>

typedef vector<CNpcMagicData*> VCT_NPCMAGICDATA;//�ض�type��magic����
typedef map<OBJID,VCT_NPCMAGICDATA*> MAP_NPCMAGICDATA;//����type��magic����

typedef vector<CEudemonMagicData*> VCT_EUDEMONMAGICDATA;//�ض�type��magic����
typedef vector<CSoulMagicData*> VCT_SOULMAGICDATA;

const int _DEFAULT_MONSTERMAGICLEV = 0;//���＼��Ĭ�ϼ���

class CNpcMagicManager  
{
public:
	CNpcMagicManager();
	virtual ~CNpcMagicManager();

	static CNpcMagicManager*	CreateNew	()		{ return new CNpcMagicManager; }

	BOOL CreateData( IDatabase* pDb );//�����ݿ��ж����� 

	const VCT_NPCMAGICDATA* QueryDataByOwnerType(OBJID monsterType);//ͨ��monsterType�õ�magic�ļ���
	const CNpcMagicData* QueryData(OBJID monsterType,
		OBJID magicType);//ͨ��monstertype��magicType����þ��弼����Ϣ

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

//ȫ��
extern CNpcMagicManager*	g_pNpcMagicManager;


#endif // !defined(AFX_NPCMAGICMANAGER_H__DF54D1FE_B48F_4CB9_A1CE_6D7A1957B806__INCLUDED_)
