// MagicType.h: interface for the CMagicType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGICTYPE_H__344C2000_C8EA_4C2E_BDE7_A2772EB20976__INCLUDED_)
#define AFX_MAGICTYPE_H__344C2000_C8EA_4C2E_BDE7_A2772EB20976__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "../share/define.h"
#include "../../GameBaseCodeMT/ConstGameData.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"
#include "Myheap.h"
#include "T_SingleObjSet2.h"
#include <vector>
using namespace std;

//////////////////////////////////////////////////////////////////////
//enum MAGICTYPEDATA{
// 		MAGICTYPEDATA_ID_=0,
// 		MAGICTYPEDATA_TYPE,
// 		MAGICTYPEDATA_SORT,
// 		MAGICTYPEDATA_NAME,
// 		MAGICTYPEDATA_CRIME,
// 		MAGICTYPEDATA_GROUND,
// 		MAGICTYPEDATA_MULTI,
// 		MAGICTYPEDATA_TARGET,
// 		MAGICTYPEDATA_LEVEL,
// 		MAGICTYPEDATA_USE_MP,
// 		MAGICTYPEDATA_USE_POTENTIAL,
// 		MAGICTYPEDATA_POWER,
// 		MAGICTYPEDATA_INTONE,
// 		MAGICTYPEDATA_PERCENT,
// 		MAGICTYPEDATA_STEP,
// 		MAGICTYPEDATA_RANGE,
// 		MAGICTYPEDATA_DISTANCE,
// 		MAGICTYPEDATA_STATUS_CHANCE,		// ATKSTATUS����ħ����������״̬�ĸ��ʰٷֱ�
// 		MAGICTYPEDATA_STATUS,
// 		MAGICTYPEDATA_NEED_PROFMASK,
// 		MAGICTYPEDATA_NEED_EXP,
// 		MAGICTYPEDATA_NEED_LEVEL,
// 		MAGICTYPEDATA_NEED_GEMTYPE,		//---jinggy---��Ҫ�ı�ʯ����
// 		MAGICTYPEDATA_USE_XP,				// == TYPE_XPSKILL: use xp
// 		MAGICTYPEDATA_WEAPON_SUBTYPE,
// 		MAGICTYPEDATA_TIMES,				// =0 : once status
// 		MAGICTYPEDATA_AUTO_ACTIVE,			// 0x01: active when kill target
// 		MAGICTYPEDATA_FLOOR_ATTR,			// 0: need not match.
// 		MAGICTYPEDATA_AUTO_LEARN,			// bool
// 		MAGICTYPEDATA_LEARN_LEVEL,			// auto learn need level, nn: equ level, nnmm: between mm - nn level.
// 		MAGICTYPEDATA_DROPWEAPON,			// 1: throw weapon_r to target
// 		MAGICTYPEDATA_USE_EP,				// use energy
// 		MAGICTYPEDATA_WEAPON_HIT,			// 1: hit by weapon
// 		MAGICTYPEDATA_USE_ITEM,				// use equip item
// 		MAGICTYPEDATA_NEXT_MAGIC,			// launch this magic when filished current magic, use same target and pos
// 		MAGICTYPEDATA_DELAY,				// milli-secs
// 		MAGICTYPEDATA_ITEM_NUM,				// use equip item number
// 		MAGICTYPEDATA_WIDTH,
// 		MAGICTYPEDATA_DURABILITY,
// 		MAGICTYPEDATA_APPLY_MS,				// ħ��ÿ������ʩչʱ����
// 		MAGICTYPEDATA_TRACK_ID,
// 		MAGICTYPEDATA_TRACK_ID2,
// 		MAGICTYPEDATA_AUTO_LEARN_PROB,		// �Զ�ѧϰ�ĳɹ���(10000=100%)
// 	//	MAGICTYPEDATA_MAGIC1,               // �������ּ����ܷ���������ʹ�� [�½���9/11/2007]
// 	
// 	    MAGICTYPEDATA_ENMITY_ADDI = 52,
	//[2010-08-02 goto]���ܱ��޸�����
enum MAGICTYPEDATA{
	MAGICTYPEDATA_ID_=0,
	MAGICTYPEDATA_TYPE,
	MAGICTYPEDATA_SORT,					//��������(��Ч��)
	MAGICTYPEDATA_NAME,					//��������
	MAGICTYPEDATA_CRIME,				//�ƶ��ʶ(�ѷ��з�)
	MAGICTYPEDATA_MULTI,				//�Ƿ���Ⱥ�弼��
	MAGICTYPEDATA_GROUND,				//�Ƿ��ǵ�Ч����
	MAGICTYPEDATA_TARGET,				//���Ӱ��Ŀ����
	MAGICTYPEDATA_TARGET_ADD,			//Ӱ��Ŀ������
	MAGICTYPEDATA_NEED_PROFMASK,		//����ְҵ
	MAGICTYPEDATA_NEED_LEVEL,			//��ʼѧϰ�ȼ�
	MAGICTYPEDATA_NEED_COST_LEV,		//���ĵȼ�
	MAGICTYPEDATA_USE_MP,				//����ħ��ֵ
	MAGICTYPEDATA_USE_MP_add,			//����ħ��ֵ����
	MAGICTYPEDATA_USE_ANG,				//����ŭ��
	MAGICTYPEDATA_USE_ANG_ADD,			//����ŭ������
	MAGICTYPEDATA_POWER,				//����
	MAGICTYPEDATA_POWER_ADD,			//��������
	MAGICTYPEDATA_POWER2,				//����2����
	MAGICTYPEDATA_POWER2_ADD,			//����2��������
	MAGICTYPEDATA_DISTANCE,				//����
	MAGICTYPEDATA_RANGE,				//��Χ
	MAGICTYPEDATA_STATUS,				//����״̬/BUFF����ID
	MAGICTYPEDATA_STATUS_POWER,			//����״̬/BUFF����
	MAGICTYPEDATA_STATUS_ADD,			//����״̬����
	MAGICTYPEDATA_STATUS_CHANCE,		//����״̬�ɹ���
	MAGICTYPEDATA_STEP,					//����ʱ��
	MAGICTYPEDATA_STEP_ADD,				//����ʱ������
	MAGICTYPEDATA_TIMES,				//���ô���
	MAGICTYPEDATA_TIMES_ADD,			//���ô�������
	MAGICTYPEDATA_DELAY,				//˽��CD
	MAGICTYPEDATA_DELAY_ADD,			//˽��CD����
	MAGICTYPEDATA_DELAY_PUBLIC,			//����CD
	MAGICTYPEDATA_DELAY_PUBLIC_ADD,		//����CD����
	MAGICTYPEDATA_ATTACK_TYPE,			//��������
	MAGICTYPEDATA_BOOK,					//������ID
};
char	szMagicTypeTable[];
typedef	CGameData<MAGICTYPEDATA, szMagicTypeTable, szID>	CGameMagicTypeData;


typedef struct {
	UCHAR	ucDir;
	UCHAR	ucStep;
	UCHAR	ucAlt;
	OBJID	idAction;
	int		nPower;
	int		nApplyMS;
} ST_TRACK;

class CMagicTypeData
{
public:
	CMagicTypeData()			{ m_pData = NULL; }
	virtual ~CMagicTypeData()	{ if (m_pData) SAFE_RELEASE (m_pData); }

public:
	static CMagicTypeData*	CreateNew	()	{ return new CMagicTypeData; }
	ULONG	Release			()				{ delete this; return 0; }
	ULONG	ReleaseByOwner	()				{ delete this; return 0; }
	bool	Create			(IRecordset* pRes/*, IDatabase* pDb = NULL*/);

public: // get set
	OBJID		GetID()					{ CHECKF(m_pData); return m_pData->GetKey(); }

	int		GetInt(MAGICTYPEDATA idx)	{ CHECKF(m_pData); return m_pData->GetInt(idx); }
	LPCTSTR	GetStr(MAGICTYPEDATA idx)	{ CHECKF(m_pData); return m_pData->GetStr(idx); } 

protected:
	CGameMagicTypeData*		m_pData;
public:
	MYHEAP_DECLARATION(s_heap)
};

//////////////////////////////////////////////////////////////////////

typedef	ISingleObjSet2<CMagicTypeData>		IMagicTypeSet;
typedef	CSingleObjSet2<CMagicTypeData>		CMagicTypeSet;


//class CMagicType  
//{
//public:
//	CMagicType();
//	virtual ~CMagicType();
//	
//public:
//	static CMagicType*	CreateNew	()		{ return new CMagicType; }
//	ULONG	Release			()				{ delete this; return 0; }
//	bool	Create			(IDatabase* pDb);
//	CMagicTypeData*	QueryMagicType(OBJID idType);
//
//protected:
//	IMagicTypeSet*	m_setType;
//public:
//	MYHEAP_DECLARATION(s_heap)
//};

#endif // !defined(AFX_MAGICTYPE_H__344C2000_C8EA_4C2E_BDE7_A2772EB20976__INCLUDED_)
