// MagicData.h: interface for the CTempMagicData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGICDATA_H__59045F42_8A10_44CB_A75B_D6C519454845__INCLUDED_)
#define AFX_MAGICDATA_H__59045F42_8A10_44CB_A75B_D6C519454845__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../share/define.h"
#include "MagicType.h"
#include "MyHeap.h"
#include "TimeOut.h"

//////////////////////////////////////////////////////////////////////
enum MAGICDATA{
	MAGICDATA_ID_=0,				//? �����������ϴ�ħ���ķ����ݿ������ٴ�ѧϰʱ��UPDATE

	MAGICDATA_OWNERID=1,
	MAGICDATA_TYPE,
	MAGICDATA_LEVEL,
 	MAGICDATA_EXP,
	MAGICDATA_LASTUSE,
// 	MAGICDATA_UNLEARN,
// 	MAGICDATA_OLD_LEVEL,
// 	MAGICDATA_GIFTFLAG,	//  [12/12/2007 ���]	�츳���ܱ�־λ
// 	MAGICDATA_ADDLEVEL,
// 	MAGICDATA_LEARN,

	MAGICTYPEDATA_OFFSET=100,		// ������������ʹ��
	MAGICDATA_TYPE_X=101,		// ��ʹ��
// 	MAGICDATA_SORT,
// 	MAGICDATA_NAME,
// 	MAGICDATA_CRIME,
// 	MAGICDATA_GROUND,
// 	MAGICDATA_MULTI,
// 	MAGICDATA_TARGET,
// 	MAGICDATA_LEVEL_X,			// ��ʹ��
// 	MAGICDATA_USE_MP,
// 	MAGICDATA_USE_POTENTIAL,	// ����Ǳ��
// 	MAGICDATA_POWER,
// 	MAGICDATA_INTONE,			// ħ������ʱ��(ms)
// 	MAGICDATA_PERCENT,
// 	MAGICDATA_STEP,
// 	MAGICDATA_RANGE,
// 	MAGICDATA_DISTANCE,
// 	MAGICDATA_STATUS_CHANCE,		// ATKSTATUS����ħ������״̬�ĸ��ʰٷֱ�
// 	MAGICDATA_STATUS,
// 	MAGICDATA_NEED_PROFMASK,
// 	MAGICDATA_NEED_EXP,
// 	MAGICDATA_NEED_LEVEL,
// 	MAGICDATA_NEED_GEMTYPE,		//---jinggy---��Ҫ�ı�ʯ����
// 	MAGICDATA_USE_XP,				// == TYPE_XPSKILL: use xp
// 	MAGICDATA_WEAPON_SUBTYPE,
// 	MAGICDATA_TIMES,				// =0 : once status
// 	MAGICDATA_AUTO_ACTIVE,			// 0x01: active when kill target
// 	MAGICDATA_FLOOR_ATTR,			// 0: need not match.
// 	MAGICDATA_AUTO_LEARN,			// bool
// 	MAGICDATA_LEARN_LEVEL,			// auto learn need level, nn: equ level, nnmm: between mm - nn level.
// 	MAGICDATA_DROPWEAPON,			// 1: throw weapon_r to target
// 	MAGICDATA_USE_EP,				// use energy
// 	MAGICDATA_WEAPON_HIT,			// 1: hit by weapon
// 	MAGICDATA_USE_ITEM,				// use equip item
// 	MAGICDATA_NEXT_MAGIC,			// launch this magic when filished current magic, use same target and pos
// 	MAGICDATA_DELAY,				// milli-secs
// 	MAGICDATA_ITEM_NUM,				// use equip item number
// 	MAGICDATA_WIDTH,				// width of magic. 0-180 for degree of fan (sort 4), 0 for default.
// 	MAGICDATA_DURABILITY,
// 
// 	MAGICDATA_APPLY_MS,				// ħ��ÿ������ʩ��ʱ��
// 	MAGICDATA_TRACK_ID,
// 	MAGICDATA_TRACK_ID2,
// 
// 	MAGICDATA_AUTO_LEARN_PROB,		// �Զ�ѧϰ�ĳɹ���(10000=100%)
// 	MAGICDATA_GROUP_TYPE,			// �ϻ�����
// 	MAGICDATA_GROUP_MEMBER1_POS,	// �ϻ�����1���λ��
// 	MAGICDATA_GROUP_MEMBER2_POS,	// �ϻ�����2���λ��
// 	MAGICDATA_GROUP_MEMBER3_POS,	// �ϻ�����3���λ��
// 	////////////////////////////////////////////////////
// 	//tcp2005.9.21 add  for union attack begin
// 	MAGICDATA_MAGIC1,               // �˵�ħ�� 
// 	MAGICDATA_MAGIC2,               // ����1��ħ��
// 	MAGICDATA_MAGIC3,               // ����2��ħ��
// 	MAGICDATA_MAGIC4,	            // ����3��ħ��
// 	MAGICDATA_MASKBIT,				// �ɻ����ͼ����PK����
// 	MAGICDATA_ENMITY,
// 	MAGICDATA_PUBLICDELAY,
// 	
// 	//tcp2005.9.21 add  for union attack end

	//[2010-08-02 goto]���ܱ��޸�����
	MAGICDATA_SORT,					//��������(��Ч��)
	MAGICDATA_NAME,					//��������
	MAGICDATA_CRIME,				//�ƶ��ʶ(�ѷ��з�)
	MAGICDATA_MULTI,				//�Ƿ���Ⱥ�弼��
	MAGICDATA_GROUND,				//�Ƿ��ǵ�Ч����
	MAGICDATA_TARGET,				//���Ӱ��Ŀ����
	MAGICDATA_TARGET_ADD,			//Ӱ��Ŀ������
	MAGICDATA_NEED_PROFMASK,		//����ְҵ
	MAGICDATA_NEED_LEVEL,			//��ʼѧϰ�ȼ�
	MAGICDATA_NEED_COST_LEV,		//���ĵȼ�
	MAGICDATA_USE_MP,				//����ħ��ֵ
	MAGICDATA_USE_MP_ADD,			//����ħ��ֵ����
	MAGICDATA_USE_ANG,				//����ŭ��
	MAGICDATA_USE_ANG_ADD,			//����ŭ������
	MAGICDATA_POWER,				//����
	MAGICDATA_POWER_ADD,			//��������
	MAGICDATA_POWER2,				//����2����
	MAGICDATA_POWER2_ADD,			//����2��������
	MAGICDATA_DISTANCE,				//����
	MAGICDATA_RANGE,				//��Χ
	MAGICDATA_STATUS,				//����״̬/BUFF����ID
	MAGICDATA_STATUS_POWER,			//����״̬/BUFF����
	MAGICDATA_STATUS_ADD,			//����״̬����
	MAGICDATA_STATUS_CHANCE,		//����״̬�ɹ���
	MAGICDATA_STEP,					//����ʱ��
	MAGICDATA_STEP_ADD,				//����ʱ������
	MAGICDATA_TIMES,				//���ô���
	MAGICDATA_TIMES_ADD,			//���ô�������
	MAGICDATA_DELAY,				//˽��CD
	MAGICDATA_DELAY_ADD,			//˽��CD����
	MAGICDATA_PUBLICDELAY,			//����CD
	MAGICDATA_PUBLICDELAY_ADD,		//����CD����
	MAGICDATA_ATTACK_TYPE,			//��������
	MAGICDATA_BOOK,					//������ID
};


struct ST_MAGIC_DATA {
	OBJID	idMagic;
	OBJID	idOwner;
	USHORT	usType;
	USHORT	usLevel;
	DWORD	dwExp;
	UCHAR	ucUnlearn;
	USHORT	usOldLevel;
	DWORD	dwGiftFlag;
	DWORD	dwAddLevel;
	DWORD	dwLearn;
	USHORT	usCDIndex;
};

//////////////////////////////////////////////////////////////////////
class IMagicData
{
protected:
	IMagicData()			{ m_pType = NULL; }
	virtual ~IMagicData()	{}
public:
	// ����CSingleObjSet2��ҪCreateNew�ӿڣ����ò�����
	static IMagicData*	CreateNew();
	virtual int		GetMagicLevel()										PURE_VIRTUAL_FUNCTION_0
	virtual void	ReleaseByOwner()											PURE_VIRTUAL_FUNCTION
	virtual bool	Create(IRecordset* pRecordset/*,IDatabase* pDb = NULL*/)		PURE_VIRTUAL_FUNCTION_0
	virtual bool	Create(IRecordset* pDefaultRecord, OBJID idNewKey)			PURE_VIRTUAL_FUNCTION_0
	virtual bool	Create(ST_MAGIC_DATA* pMagicData)							PURE_VIRTUAL_FUNCTION_0
	virtual int		GetInt(MAGICDATA nDataName)									PURE_VIRTUAL_FUNCTION_0
	virtual void	SetInt(MAGICDATA idx, int nData)							PURE_VIRTUAL_FUNCTION
	virtual void	SetStr(MAGICDATA idx, char* szData,int nSize)				PURE_VIRTUAL_FUNCTION
	virtual bool	FindType()													PURE_VIRTUAL_FUNCTION_0

	virtual OBJID	GetID()														PURE_VIRTUAL_FUNCTION_0
	virtual bool	Update()													PURE_VIRTUAL_FUNCTION_0
	virtual void	ClearUpdateFlags()											PURE_VIRTUAL_FUNCTION
	virtual OBJID	InsertRecord()												PURE_VIRTUAL_FUNCTION_0
	virtual bool	DeleteRecord()												PURE_VIRTUAL_FUNCTION_0
	virtual int	NeedSend()											            PURE_VIRTUAL_FUNCTION_0
	virtual int		GetIndex()													PURE_VIRTUAL_FUNCTION_0
	virtual void	SetIndex(int nIndex)										PURE_VIRTUAL_FUNCTION

public:
	virtual const char*		GetName()		{ CHECKF(m_pType); return m_pType->GetStr(MAGICTYPEDATA_NAME); }

	virtual bool	IsReady()							{ return ( !m_tDelay.IsActive() || ( m_tDelay.IsActive() && m_tDelay.IsTimeOut() ) ); }
	virtual void	StartDelay()						{ m_tDelay.Update(); }
	virtual void	SetDelay();
	virtual int		LeaveTime()							{ return m_tDelay.LeaveTime(); }	
	virtual clock_t			GetDelayUpdateTime()		{ if( m_tDelay.IsActive() ) return m_tDelay.GetUpdateTime();return 0;}// ������� [5/30/2007]
	virtual CTimeOutMS&		QueryDelayObj()				{ return m_tDelay;}													//	������� [5/30/2007]
protected:
	CMagicTypeData*	m_pType;
	CTimeOutMS		m_tDelay;			// ʩչ������ʱ		
};


//////////////////////////////////////////////////////////////////////
class CMagicTempData  : public IMagicData
{
protected:
	CMagicTempData();
	virtual ~CMagicTempData()	{}

public:
	static IMagicData*	CreateNew()			{ return new CMagicTempData; }
	virtual void	ReleaseByOwner()		{ delete this; }
	virtual int		GetMagicLevel();

	virtual bool	Create(IRecordset* pRecordset);
	virtual bool	Create(IRecordset* pDefaultRecord, OBJID idNewKey);
	virtual bool	Create(ST_MAGIC_DATA* pMagicData);
	virtual int		GetInt(MAGICDATA nDataName);
	virtual void	SetInt(MAGICDATA idx, int nData);
	virtual bool	FindType();

	virtual OBJID	GetID()					{ return m_data.idMagic; }
	virtual bool	Update()				{ return false; }
	virtual void	ClearUpdateFlags()		{}
	virtual OBJID	InsertRecord()			{ return m_data.idMagic; }
	virtual bool	DeleteRecord()			{ return false; }

protected:
	ST_MAGIC_DATA	m_data;
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_MAGICDATA_H__59045F42_8A10_44CB_A75B_D6C519454845__INCLUDED_)
