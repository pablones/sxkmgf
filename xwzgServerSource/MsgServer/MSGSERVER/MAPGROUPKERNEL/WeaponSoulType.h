// WeaponSoulType.h: interface for the CWeaponSoulType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPONSOULTYPE_H__8C094750_53E7_4E67_9579_C1FE493DDD28__INCLUDED_)
#define AFX_WEAPONSOULTYPE_H__8C094750_53E7_4E67_9579_C1FE493DDD28__INCLUDED_

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


enum WEAPONSOULTYPEDATA
{
	WEAPONSOULTYPEDATA_ID = 0,//TYPEID
	WEAPONSOULTYPEDATA_SZNAME,//����
	WEAPONSOULTYPEDATA_ABILITY_MARK,//�����Ա�ʶ
//	WEAPONSOULTYPEDATA_MIN_ATTACK_FIRST,//��ʼ_��С��������
// 	WEAPONSOULTYPEDATA_MAX_ATTACK_FIRST,//��ʼ_�����������
	WEAPONSOULTYPEDATA_MIN_ATTACK_FIRST_MAX,//����_��ʼ��С��������
	WEAPONSOULTYPEDATA_MAX_ATTACK_FIRST_MAX,//����_��ʼ�����������
//	WEAPONSOULTYPEDATA_MIN_MAGIC_ATTACK_FIRST,//��ʼ_��Сħ��������
// 	WEAPONSOULTYPEDATA_MAX_MAGIC_ATTACK_FIRST,//��ʼ_���ħ��������
	WEAPONSOULTYPEDATA_MIN_MAGIC_ATTACK_FIRST_MAX,//����_��ʼ��Сħ��������
	WEAPONSOULTYPEDATA_MAX_MAGIC_ATTACK_FIRST_MAX,//����_��ʼ���ħ��������
//	WEAPONSOULTYPEDATA_DEFENSE_FIRST,//��ʼ_�������
// 	WEAPONSOULTYPEDATA_MAGIC_DEF_FIRST,//��ʼ_ħ������
	WEAPONSOULTYPEDATA_DEFENSE_MAX,//����_�������
	WEAPONSOULTYPEDATA_MAGIC_DEF_MAX,//����_ħ������
	WEAPONSOULTYPEDATA_MIN_ATTACK_GROW_FIRST,//��ʼ_��С���������ɳ�ֵ
	WEAPONSOULTYPEDATA_MAX_ATTACK_GROW_FIRST,//��ʼ_������������ɳ�ֵ
	WEAPONSOULTYPEDATA_MIN_ATTACK_GROW_MAX,//����_��С���������ɳ�ֵ
	WEAPONSOULTYPEDATA_MAX_ATTACK_GROW_MAX,//����_������������ɳ�ֵ
	WEAPONSOULTYPEDATA_MIN_MAGIC_ATTACK_GROW_FIRST,//��ʼ_��Сħ���������ɳ�ֵ
	WEAPONSOULTYPEDATA_MAX_MAGIC_ATTACK_GROW_FIRST,//��ʼ_���ħ���������ɳ�ֵ
	WEAPONSOULTYPEDATA_MIN_MAGIC_ATTACK_GROW_MAX,//����_��Сħ���������ɳ�ֵ
	WEAPONSOULTYPEDATA_MAX_MAGIC_ATTACK_GROW_MAX,//����_���ħ���������ɳ�ֵ
	WEAPONSOULTYPEDATA_DEFENSE_GROW_FIRST,//��ʼ_��������ɳ�ֵ
	WEAPONSOULTYPEDATA_DEFENSE_GROW_MAX,//����_��������ɳ�ֵ
	WEAPONSOULTYPEDATA_MAGIC_DEF_GROW_FIRST,//��ʼ_ħ�������ɳ�ֵ
	WEAPONSOULTYPEDATA_MAGIC_DEF_GROW_MAX,//����_ħ�������ɳ�ֵ
	WEAPONSOULTYPEDATA_SKILL_POINT_GROW,//��Ϊֵ�ɳ�ֵ
	WEAPONSOULTYPEDATA_SKILL_POINT_FIRST,//��ʼ��Ϊ��
	WEAPONSOULTYPEDATA_TEAM_WORK,//���幱��
};

struct ST_WeaponSoulInfo
{
	OBJID	id;//ID
	char	szName[_MAX_NAMESIZE];//����
	UINT	ability_mark;//�����Ա�ʶ
//	UINT	min_attack_first;//��ʼ_��С��������
// 	UINT	max_attack_first;//��ʼ_�����������
	UINT	min_attack_first_max;//����_��ʼ��С��������
	UINT	max_attack_first_max;//����_��ʼ�����������
//	UINT	min_magic_attack_first;//��ʼ_��Сħ��������
// 	UINT	max_magic_attack_first;//��ʼ_���ħ��������
	UINT	min_magic_attack_first_max;//����_��ʼ��Сħ��������
	UINT	max_magic_attack_first_max;//����_��ʼ���ħ��������
//	UINT	defense_first;//��ʼ_�������
// 	UINT	magic_def_first;//��ʼ_ħ������
	UINT	defense_max;//����_�������
	UINT	magic_def_max;//����_ħ������
	UINT	min_attack_grow_first;//��ʼ_��С���������ɳ�ֵ
	UINT	max_attack_grow_first;//��ʼ_������������ɳ�ֵ
	UINT	min_attack_grow_max;//����_��С���������ɳ�ֵ
	UINT	max_attack_grow_max;//����_������������ɳ�ֵ
	UINT	min_magic_attack_grow_first;//��ʼ_��Сħ���������ɳ�ֵ
	UINT	max_magic_attack_grow_first;//��ʼ_���ħ���������ɳ�ֵ
	UINT	min_magic_attack_grow_max;//����_��Сħ���������ɳ�ֵ
	UINT	max_magic_attack_grow_max;//����_���ħ���������ɳ�ֵ
	UINT	defense_grow_first;//��ʼ_��������ɳ�ֵ
	UINT	defense_grow_max;//����_��������ɳ�ֵ
	UINT	magic_def_grow_first;//��ʼ_ħ�������ɳ�ֵ
	UINT	magic_def_grow_max;//����_ħ�������ɳ�ֵ
	UINT	skill_point_grow;//��Ϊֵ�ɳ�ֵ
	UINT	skill_point_first;//��ʼ��Ϊ��
	UINT	team_work;//���幱��
};


char	szWeaponSoulTypeTable[];
typedef	CGameData<WEAPONSOULTYPEDATA,szWeaponSoulTypeTable,szID>	CGameDataWeaponSoulType;
typedef	ISingleMap2<CGameDataWeaponSoulType>	IWeaponSoulTypeSet;
typedef	CSingleMap2<CGameDataWeaponSoulType>	CWeaponSoulTypeSet;

enum EM_WPSL_ABILITY_MARK
{
	EM_WPSLABILITYMARK_max_attack_grow_first	=1,
	EM_WPSLABILITYMARK_min_attack_grow_first	=2,
	EM_WPSLABILITYMARK_max_magic_attack_grow_first	=4,
	EM_WPSLABILITYMARK_min_magic_attack_grow_first	=8,
	EM_WPSLABILITYMARK_defense_grow_first	=16,
	EM_WPSLABILITYMARK_magic_def_grow_first	=32,
};

class CWeaponSoulType  
{
public:
	CWeaponSoulType();
	virtual ~CWeaponSoulType();
public:
	static CWeaponSoulType*	CreateNew	()		{ return new CWeaponSoulType; }
	ULONG	Release			()				{ delete this; return 0; }
	bool	Create			(IDatabase* pDb);
	CGameDataWeaponSoulType*  QueryWeaponSoulType(OBJID idType);
	bool GetInfo(OBJID idType, ST_WeaponSoulInfo* pInfo);
	

protected:
	IWeaponSoulTypeSet*	m_setWeaponSoulType;
	MYHEAP_DECLARATION(s_heap)

};

//global
extern CWeaponSoulType* g_pWeaponSoulType;

#endif // !defined(AFX_WEAPONSOULTYPE_H__8C094750_53E7_4E67_9579_C1FE493DDD28__INCLUDED_)
