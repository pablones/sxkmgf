// AINpc.h: interface for the CMonster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AINPC_H__52682938_5D82_4101_B888_6A4A3F30EB51__INCLUDED_)
#define AFX_AINPC_H__52682938_5D82_4101_B888_6A4A3F30EB51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "gameobj.h"
#include "GameBlock.h"
#include "Timeout.h"
#include "Item.h"
#include "battlesystem.h"
#include "Myheap.h"
#include "GameMap.h"
#include "StatusMore.h"
#include "protocol.h"
#include "UserTable.h"
#include "autolink.h"
#include "maptrap.h"
#include "Role.h"
#include "Magic.h"
#include "NpcMagicManager.h"
#include "User.h"
#include "sigslot.h"
using namespace sigslot;

//////////////////////////////////////////////////////////////////////
enum {	ATKUSER_LEAVEONLY				= 0,				// ֻ������
		ATKUSER_PASSIVE					= 0x01,				// ��������
		ATKUSER_ACTIVE					= 0x02,				// ��������
		ATKUSER_RIGHTEOUS				= 0x04,				// �����(����������ٻ��Ϳ��ƵĹ���)
		ATKUSER_GUARD					= 0x08,				// ����(���»�ԭλ��)
		ATKUSER_PPKER					= 0x10,				// ׷ɱ����
		ATKUSER_JUMP					= 0x20,				// ����
		ATKUSER_FIXED					= 0x40,				// ���ᶯ��
		ATKUSER_FASTBACK				= 0x0080,				// �ٹ�
		ATKUSER_LOCKUSER				= 0x0100,				// ��������ָ����ң�����뿪�Զ���ʧ
		ATKUSER_LOCKONE					= 0x0200,				// �����������ȹ����Լ������
		ATKUSER_ADDLIFE					= 0x0400,				// �Զ���Ѫ
		ATKUSER_EVIL_KILLER				= 0x0800,				// ����ɱ��
		ATKUSER_WING					= 0x1000,				// ����״̬
		ATKUSER_NEUTRAL					= 0x2000,				// ����
		ATKUSER_ROAR					= 0x4000,				// ����ʱȫ��ͼŭ��
		ATKUSER_NOESCAPE				= 0x8000,				// ��������
		ATKUSER_EQUALITY				= 0x00010000,			// ������
// 		ATKUSER_CAT                     = 0x00020000,           // cat
        ATKUSER_CITYWAR_TARGET          = 0x00020000,           // ��սս��Ŀ��
		ATKUSER_CITYWAR_ATTACk          = 0x00040000,           // citywar attack
		ATKUSER_CITYWAR_DEFENCE         = 0x00080000,           // citywar defence
	    ATKUSER_NON_ATTACk              = 0x00100000,           // �޹���������
		ATKUSER_NON_BEATTACK            = 0x00200000,           // ���ܱ���������

		ATKUSER_REALGUARD				= 0x00400000,			// ����������Ѿ����ã��������������������
		ATKUSER_NOWANDER				= 0x00800000,
		ATKUSER_TUTORBOSS				= 0x01000000,			// ʦͽ����� 0x00800000 �Ѿ���ʹ����
};

//////////////////////////////////////////////////////////////////////
const int	MONSTERDROPITEM_RANGE		= 2;		// 2*2+1��4�η�=625������
const int	NPCDIEDELAY_SECS			= 3;		// MONSTER�����������ɾ��
const int	SIMPLEMAGIC_RECRUIT			= 1005;
const OBJID	TYPE_TASK_MONSTER			= 3000;
const bool	DEL_NOW						= true;


const int	DEFAULT_EUDEMON_POTENTIAL	= 50;		// ���޵�ȱʡǱ��ֵ
const int	MAX_EUDEMON_POTENTIAL		= 100;		// �������Ǳ��ֵ
const int	ADD_POTENTIAL_KILLNUM		= 100;		// ÿɱ��100��Ŀ������Ǳ��ֵ 
const int	ADD_POTENTIAL_PER_KILLNUM	= 1;
const int	ADD_POTENTIAL_LEVUP			= 5;

const int	ADD_RELATIONSHIP_KILLNUM	= 100;		// ÿɱ��100��Ŀ�����ӹ�ϵֵ
const int	ADD_RELATIONSHIP_PER_TIME	= 1;		// ÿ�������Ĺ�ϵֵ

//////////////////////////////////////////////////////////////////////
// ���޹�������
enum EATK_MODE {
	EATK_MODE_NONE	= 0,	// ������
	EATK_MODE_ATK,			// ����
	EATK_MODE_ATKDEF,		// ����
	EATK_MODE_DEF,			// ����

	EATK_MODE_LIMIT,
};

enum EUDEMONGIFT {
	EUDEMONGIFT_DAMAGE,
	EUDEMONGIFT_REDAMAGE,

	EUDEMONGIFT_MAXLIFE,
	EUDEMONGIFT_MAXMANA,

	EUDEMONGIFT_ATT,
	EUDEMONGIFT_MAGATT,

	EUDEMONGIFT_DEF,
	EUDEMONGIFT_MAGDEF,
	
	EUDEMONGIFT_CRIHIT,
	EUDEMONGIFT_HIT,
	EUDEMONGIFT_DODGE,
	EUDEMONGIFT_ADDITEM,
};
//////////////////////////////////////////////////////////////////////
enum PETDATA{
		PETDATA_OWNERID=1,
		PETDATA_OWNERTYPE,
		PETDATA_GENID,
		PETDATA_TYPE,
		PETDATA_NAME,
		PETDATA_LIFE,
		PETDATA_MANA,
		PETDATA_MAPID,
		PETDATA_RECORD_X,
		PETDATA_RECORD_Y,
		PETDATA_DATA,						// ���������޼�¼NPCID
};

//////////////////////////////////////////////////////////////////////
enum MONSTER_NAME_TYPE{
	NAME_GREEN,			// ������
	NAME_WHITE,			// ������
	NAME_RED,			// ������
	NAME_BLACK,			// ������
};

enum EM_AISTATE //AI״̬
{ 
	EM_AISTATE_NONE,
	EM_AISTATE_Initialize,     //��ʼ��
	EM_AISTATE_Wander,         //�й� t1
	EM_AISTATE_Chase,          //׷�� t3
	EM_AISTATE_Atk,				//����
	EM_AISTATE_Die,            //���� 
	EM_AISTATE_SoonBack,       //�ٹ� 
	EM_AISTATE_Choose,         //ѡ�� t2
	EM_AISTATE_WalkMode,       //��Ŀ���߶�
};

const	OBJID	MONSTER_TYPE_DRAGON_FLAG = 790;		//	�������
const	OBJID	MONSTER_TYPE_HUMAN_FLAG = 791;		//	���������
const	OBJID	MONSTER_TYPE_GENE_FLAG = 792;		//	���������

const int CITYSYNMONSTER_BEKILL_ZONGTAN = 10;   //��̳
const int CITYSYNMONSTER_BEKILL_BUY     = 11;   //����
const int CITYSYNMONSTER_BEKILL_CHGMAP  = 12;   //����
const int CITYSYNMONSTER_BEKILL_TASK    = 13;   //����

const int SYNWAR_BEKILLMARK_NON      = 0;   //����� 
const int SYNWAR_BEKILLMARK_ZONGTANA = 1;   //������̳��ɱ���
const int SYNWAR_BEKILLMARK_ZONGTAND = 2;   //�ط���̳��ɱ���
const int SYNWAR_BEKILLMARK_BUY      = 4;   //������ɱ���
const int SYNWAR_BEKILLMARK_CHGMAP   = 8;   //���ͱ�ɱ���
const int SYNWAR_BEKILLMARK_TASK     = 16;  //���ͱ�ɱ���

//////////////////////////////////////////////////////////////////////
enum MONSTER_MASKDATA {
	MASK_NONE				= 0,
	MASK_DISABLE_SORB_SOUL	= 1,	// ��ֹ����
	MASK_DISABLE_STEAL_MONEY= 2,	// ��ֹ͵�Խ�Ǯ
};
//////////////////////////////////////////////////////////////////////
//06.12.7�������
typedef struct
{
	CTeam*	pTeam;
	DWORD	dwExp;	 
}TYPE_TEAMEXP,*PTYPE_TEAMEXP;

//////////////////////////////////////////////////////////////////////
char	szPetTable[];
typedef		CGameData<PETDATA, szPetTable, szID>	CPetData;
typedef		map<UINT,UINT> IntimacyList;///softworms ����������ܶ�����

//////////////////////////////////////////////////////////////////////
class CUser;

struct PETGIFTDATA
{
	DWORD	m_dwGiftAddphyatt;
	DWORD	m_dwGiftAddmagatt;
	DWORD	m_dwGiftAddphydef;
	DWORD	m_dwGiftAddmagdef;
	DWORD	m_dwGiftAddhit;
	DWORD	m_dwGiftAdddodge;
	DWORD	m_dwGiftAddcrihit;
	DWORD	m_dwGiftAddTough;
	DWORD	m_dwGiftAddhp;
	DWORD	m_dwGiftAddmp;
	DWORD	m_dwGiftAddStr;
	DWORD	m_dwGiftAddInt;
	DWORD	m_dwGiftAddSta;
	DWORD	m_dwGiftAddSpi;
	DWORD	m_dwGiftAddAgi;
	
	DWORD	m_GiftAtt1;
	DWORD	m_GiftAtt2;
	DWORD	m_GiftAtt3;
	DWORD	m_GiftAtt4;
	DWORD	m_GiftDef1;
	DWORD	m_GiftDef2;
	DWORD	m_GiftDef3;
	DWORD	m_GiftDef4;
};

class CMonster : public CGameObj, public CRole, public has_slots<>
{
protected:
	CMonster();
	virtual ~CMonster();
public:
	static CMonster*	CreateNew() 	{ return new CMonster; }
	signal1<OBJID>	slotOnMonsterBeKill;///��������ʱ�¼�

	bool    Create(PROCESS_ID idProcess,  const ST_CREATENEWNPC* pInfo,CUserPetData* pData,CEudemonTypeData* pTypeData,EudemonData eudemonData,CUser* pUser);
	bool	Create(PROCESS_ID idProcess, CNpcType* pType, const struct ST_CREATENEWNPC* pInfo, LPCTSTR pszName=NULL);
	bool	Create(PROCESS_ID idProcess, CNpcType* pType, const struct ST_CREATENEWNPC* pInfo, CUser* pUser, OBJID idItem=ID_NONE);
	void	ReleaseByOwner()					{ delete this; }

	virtual		bool		FindAroundObj	(OBJID idObjType, OBJID idObj, void** pp)		{ return GetMap()->QueryObj(GetPosX(), GetPosY(), idObjType, idObj, pp); }
 	virtual		IRole*		FindAroundRole	(OBJID id)		{ return GetMap()->QueryRole(GetPosX(), GetPosY(), id); }
	virtual		CUser*		QueryOwnerUser	()				{ if(IsEudemon()) return m_pOwner; return NULL; }	//@@@ ���� if(m_pData && m_pData->GetInt(PETDATA_OWNERTYPE) == OWNER_USER) return m_pData->GetInt(PETDATA_OWNERID); 
public:
	virtual bool		QueryRole(void** ppv)	{ return *ppv=(IRole*)this,true; }
	virtual bool		QueryObj(OBJID idObjType, void** ppv)			{ if(idObjType == OBJ_MONSTER) return *ppv=this,true; return *ppv=NULL,false; }
public:
	virtual IMapThing*	QueryMapThing()			{ return (IMapThing*)this; }
	IRole*				QueryRole()				{ return (IRole*)this; }
public:
	virtual LPCTSTR	GetName()							{ if(IsEudemon()) return m_pPetData->GetStr(EUDEMONDATA_name); else return m_pType->GetStr(NPCTYPEDATA_NAME); }
	virtual LPCTSTR	GetTitle()							{ return TITILE_NONE; }
	virtual DWORD	GetLookFace			()				
	{ 
		if(IsEudemon()) 
		{
			if(m_pPetTypeData)
				return m_pPetTypeData->GetInt(EUDEMONTYPEDATA_lookface); 
			else
				return 0;
		}
		else 
			return m_pType->GetInt(NPCTYPEDATA_LOOKFACE); 
	}
	virtual DWORD 	GetHair				()				{ return 0; }
	virtual int 	GetPosX				()				{ return m_nPosX; }
	virtual int 	GetPosY				()				{ return m_nPosY; }
	virtual int 	GetDir				()				{ return m_nDir; }
	virtual I64		GetEffect			()				{ return m_i64Effect; }
	virtual DWORD	GetLev				()				
	{ 
		if(IsEudemon() ) 
		{
			if(m_pPetData)
				return m_pPetData->GetInt(EUDEMONDATA_level);
			else
				return 0;
		}
		else 
			return m_pType->GetInt(NPCTYPEDATA_LEVEL); 
	}
	virtual	DWORD	GetLife				();
	virtual DWORD	GetMaxLife			();
	virtual DWORD	GetMana				();
	virtual DWORD	GetMaxMana			();
	virtual DWORD	GetAtk				();
	virtual DWORD	GetMgcAtk			();
	virtual DWORD	GetAttack			()				{ return GetAtk(); }
	virtual DWORD	GetAtkHitRate		();
	virtual DWORD	GetAddAttByType     (int nType);
	virtual DWORD	GetAtkType			()				{ return 0; }
	virtual DWORD	GetDef				();
	virtual DWORD	GetDefence			()				{ return GetDef(); }
	virtual DWORD	GetDefence2			()				{ return GetDef(); }
	virtual DWORD	GetDex				()				{ return 0; }
	virtual DWORD	GetDexterity		()				{ return 0; }
	virtual DWORD	GetDdg				()              { return GetDodge(); }
	virtual DWORD	GetDodge			();
	virtual DWORD	GetCrit();
	virtual DWORD	GetTough();
	virtual DWORD	GetMagicDef			();
	virtual	DWORD	GetInterAtkRate		();
	virtual	DWORD	GetIntervalAtkRate	()				{ return m_pType->GetInt(NPCTYPEDATA_ATKSPEED); }

	virtual DWORD	GetPotential		()				{ return m_nPotential; }
	virtual int		AdjustExp(IRole* pTarget, int nRawExp, bool bNewbieBonusMsg=false);
	virtual DWORD	AwardBattleExp		(int nExp, bool bGemEffect=true, bool bIncludeOwner=true);
	virtual void	SetFightPause		(int nInterval)			{ m_nFightPause = nInterval; }
	virtual int		GetHelmetTypeID		()				{ return ID_NONE; }
	virtual int		GetArmorTypeID		()				{ return ID_NONE; }
	virtual int 	GetWeaponRTypeID	()				{ return ID_NONE; }
	virtual int 	GetWeaponLTypeID	()				{ return ID_NONE; }
	virtual int		GetNecklaceTypeID	()				{ return ID_NONE; }
	virtual int		GetRingLTypeID		()				{ return ID_NONE; }
	virtual int		GetShoesTypeID		()				{ return ID_NONE; }
	virtual int		GetWeaponSpecialView()				{ return 0;	}
	DWORD			GetExp()							{ if(IsEudemon()) return m_pPetData->GetInt(EUDEMONDATA_exp); else  return m_pType->GetInt(NPCTYPEDATA_EXP); }//ɱ�������þ���
	DWORD GetMoveType();
	DWORD GetAttackType();
	void SetMoveType( DWORD data , bool isRecord );
	void SetAttackType(DWORD data , bool isRecord);

	virtual void	BroadcastRoomMsg(CNetMsg* pMsg, 
		bool bSendSelf = false,
		EM_SENDNPCFLAG sendNpc = EM_SENDNPCFLAG_TRUE);


	virtual void	BroadcastRoomMsg(LPCTSTR szMsg, bool bSendSelf = false);
	virtual void	BroadcastMapMsg(CNetMsg* pMsg, bool bSendSelf = false);
	virtual void	BroadcastMapMsg(LPCTSTR szMsg, bool bSendSelf = false);
	virtual bool	SendMsg(CNetMsg* pMsg);
	virtual bool	SendSysMsg(const char* fmt, ...);
	virtual bool	SendSysMsg(int nChannel, const char* fmt, ...);
	virtual void	OnTimer(time_t tCurr);

	virtual DWORD	GetSynID			()				{ return 0; }
	virtual DWORD	GetSynRank			()				{ return RANK_NONE; }
	virtual DWORD	GetSynRankShow		()				{ return RANK_NONE; }

	virtual bool	IsSimpleMagicAtk	()				{ return false; }
	virtual int 	GetSimpleMagicHitRate()				{ return 0; }
	int 			GetRecruitMagicPercent()			{ return 0; }

	virtual CMagic*	QueryMagic()						{ return m_pMagic; }
	virtual DWORD	GetProfession()						{ if(IsEudemon()) return 0; else  return m_pType->GetInt(NPCTYPEDATA_PROFESSION); }

public: // map
	bool	AwardExp(__int64 i64Exp);
	bool	EnterMapGroup();
	bool	LeaveMapGroup();
	virtual bool	SendSelfToBlock();
	bool	SendLeaveFromBlock();

	bool	KickBack();
	DWORD   GetKilledAction();

	OBJID GetOwnerID() { return m_idOwner; }
	void SetOwnerID(OBJID OwnerID) { m_idOwner = OwnerID; }

	char* GetOwnerName() { return m_OwnerName; }
	void SetOwnerName(const char* OwnerName) { strcpy(m_OwnerName,OwnerName); }

public: // foot print
	void	GetFootPrint	(int& nPosX, int& nPosY);

	bool	InitGiftData();
	PETGIFTDATA m_EudemonGifeData;
protected:
	char	m_OwnerName[64];
	OBJID	m_idOwner;
	DWORD	m_tFootPrint;
	POINT	m_posFootPrint;
	UINT    m_nCityWarType;
	UINT    m_nNotice;
	USHORT	m_count1;
	USHORT	m_count2;
public: // get
	virtual OBJID	GetID()			{ return m_idNpc; }
	virtual int		GetObjType()	{ return CGameObj::GetObjType(); }
	//virtual OBJID	GetMapID()		{ return m_idMap; }
	virtual void	SendShow(IRole* pRole);

public: // get attrib  ------------------------------------
	virtual int		GetType			()		{ if(IsEudemon()) return m_pPetData->GetInt(EUDEMONDATA_typeid); else   return m_pType->GetID(); }
//	virtual int		GetSex			()		{ return 0; }
	virtual bool	IsAlive			();
	virtual bool	IsTalkEnable	()		{ return m_dwStatus == STATUS_NORMAL; }
	virtual bool	IsMonster		()		{ return /*IsMonsterID(GetID()) && */!IsGuard() && !IsPkKiller()&& !IsEudemon(); }
	virtual bool	IsDelThis		()		{ return IsDeleted() && m_tDie.IsTimeOut(); }		// ASSERT(!IsAlive()); 
	virtual bool	IsVirtuous()			{ return IsGuard() || IsPkKiller(); }
	virtual bool	IsEvil()				{ if(IsEudemon()) return false; else return ((m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_RIGHTEOUS) == 0 || QueryStatus(STATUS_CRIME)); }
	virtual bool	IsWing()				{ if(IsEudemon()) return false; else return (m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_WING) != 0; }
	static	bool	IsMonsterID(OBJID id)	{ return id>=MONSTERID_FIRST && id<=MONSTERID_LAST; }
	int         GetAttackUser()             { if(IsEudemon()) return false; else return m_pType->GetInt(NPCTYPEDATA_ATKUSER); }
	bool        IsNonMove()                 { if(IsEudemon()) return false; else return (m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_FIXED) != 0; }
	bool		IsGuard()		        	{ if(IsEudemon()) return false; else return (m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_GUARD) != 0; }
	bool		IsRealGuard()		        { if(IsEudemon()) return false; else return (m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_REALGUARD) != 0; }
	bool        IsCityAttack()              { if(IsEudemon()) return false; else return (m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_CITYWAR_ATTACk) != 0; }
	bool        IsCityDefence()             { if(IsEudemon()) return false; else return (m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_CITYWAR_DEFENCE) != 0; }
	bool		IsPkKiller()	        	{ if(IsEudemon()) return false; else return (m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_PPKER) != 0; }
	bool        IsNonAttack()               { if(IsEudemon()) return false; else return (m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_NON_ATTACk) != 0; }
	bool        IsNonBeAttack()             { if(IsEudemon()) return false; else return (m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_NON_BEATTACK) != 0; }
	bool        IsCityWarStrategicTarget()  { if(IsEudemon()) return false; else return (m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_CITYWAR_TARGET) != 0; }

	bool		IsDeleted()			{ return m_tDie.IsActive(); }
	void		DelMonster(bool bNow=false);			//? call this mast !IsDeleted()
	bool		IsSynPet()				{ return m_idNpc >= PETID_FIRST && m_idNpc <= PETID_LAST; }
	bool		IsSynMonster()			{ return m_idNpc >= PETID_FIRST && m_idNpc <= PETID_LAST; }
	bool		IsCallPet()				{ return m_idNpc >= CALLPETID_FIRST && m_idNpc <= CALLPETID_LAST; }
	OBJID		GetMasterID();
//	int			GetData()			{ CHECKF(m_pData); return m_pData->GetInt(PETDATA_DATA); }
	OBJID		GetGenID()			{ return m_idGen; }
	bool		IsTaskMonster()		{ return this->GetType() >= TYPE_TASK_MONSTER; }
	bool		IsMapRoar()				{ return (m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_ROAR) != 0; }
	bool		IsEquality()			{ return (m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_EQUALITY) != 0; }
	bool		IsTutorBoss()			{ if(!m_pType) return false; return (m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_TUTORBOSS) != 0; }
	bool		IsEudemon()				{ return (m_idNpc >= EUDEMON_ID_FIRST && m_idNpc <= EUDEMON_ID_LAST); }
	static int	GetNameType(int nAtkerLev, int nMonsterLev);
	static void MoveMonsterDest(UINT idMap,UINT nType,int x,int y);
protected:
	virtual void	AddLife(int nLifeAdd, int bSynchro=false);
public:	// modify attrib ------------------------------
	virtual void	SetDir(int nDir)			{ m_nDir = nDir%8; }
	virtual DWORD 	AdjustDamage		(DWORD nDamage,int nAttribType);
	virtual int 	AdjustUserAttrib	(DWORD nAttrib,int nAttribType);
	virtual	void	SetStatus(int nStatus, bool bSynchro=true,int nPower=0);
	virtual	void	ClsStatus(int nStatus, bool bSynchro=true);

	virtual bool	AddAttrib(int idxAttr, __int64 i64Data, int nSynchro);
	virtual	void	ProcessOnMove(int nMoveMode);
	virtual void	ProcessAfterMove();
public: // money --------------------------------
//	virtual bool	SpendMoney(DWORD nMoney, bool bSynchro)		{ return true; }
//	virtual bool	GainMoney(DWORD nMoney, bool bSynchro)		{ return true; }

public: // fight ------------------------------
	virtual void	ClrAttackTarget		()									{ SetAttackTarget(NULL); }
	virtual bool	SetAttackTarget(IRole* pTarget);
	virtual bool	AutoSkillAttack(IRole* pTarget);
	virtual int		Attack( IRole* pTarget,DWORD& dwBattleAddInfoRef );		// return : lose life
	virtual bool	BeAttack(bool bMagic, IRole* pTarget, int nPower,int addi = 0, bool bReflectEnable=true);
	virtual	bool    BeEnmityAttack(bool bMagic, IRole* pTarget,int addi = 0, bool bReflectEnable = true);
	virtual bool	MyEnmityClr(void);
	virtual bool	ReleaseSleep(void);

	virtual bool	MagicAttack(int nType, OBJID idTarget, int x, int y,bool needLearn = false,int learnLev = 0)	{ if (m_pMagic) return m_pMagic->MagicAttack(nType, idTarget, x, y); return false; }
	virtual void	Kill(IRole* pTarget, DWORD dwDieWay);
	virtual void	BeKill(IRole* pRole = NULL);	//pTarget = NULL);						// ����ɾ��NPC����
	virtual bool	IsAtkable(IRole* pTarget, bool bSendHint=false);
	virtual bool	IsBeAtkable();
	virtual int		GetAttackRange(int nTargetSizeAdd)		{ if(IsEudemon()) return 16; else return m_pType->GetInt(NPCTYPEDATA_ATKRANGE); }
	virtual int		GetSizeAdd()							{ return 0; }
	virtual void	SendDamageMsg( OBJID idTarget, int nDamage,DWORD dwBattleAddInfo );

	void	SetEudemonData(CUserPetData* pData);
//	bool	CreateData2();
	void	MonsterMagic_PointAttack( IRole* pTarget );	//06.9.27
	void	MonsterMagic_CircleAttack();
	void	AllotExp2User(CUserPtr pGetUser);
	void	AllotItem2User(CUserPtr pGetUser);
	int 	GetDieMode();
	bool	IsInsteadMagic();
	bool	IsImmunity			(IRole* pRole);	
	UINT    GetCityWarType()  { return m_nCityWarType; }
	int		AdjustGift(IRole* pTarget,int nType,int nOldData,int valueMask = valueMask_ALL);
	int		ProcGift1001(int nType, int oldValue,int valueMask = valueMask_ALL);
	//void	SetPosData(int x,int y) {m_nPosX = x; m_nPosY = y; }
	void	BeKillByDot(OBJID idAtker);//���ﱻdot�����Ĵ���
	void	AllotTutorPreach(CUserPtr pGetUser);//ʦͽ ��������
	void	AllotKill2User(CUserPtr pGetUser);
	BOOL    RunScript(const char *szFileName);
	void	RegisterLuaInterface();
	OBJID	GetMonsterInSynID(){return m_nSynId;}

public: // interactive
//	virtual bool	AddItem		(CItem* pItem)				{ CHECKF(pItem); pItem->ReleaseByOwner(); return true; }
//	virtual bool	IsItemFull	()							{ return false; }
	virtual bool	Freeze(int nSecs);							// return false: ��Ѩ���ɹ�

//	virtual CGameMap*		GetMap			()			{ ASSERT(m_pMap); return m_pMap; }

//	virtual void	ClsBuff(int nStatus)					{}
public: // action
//	bool	ProcessAction	(CUser* pUser, OBJID idAction, LPCTSTR pszAccept = NULL);

public: // status
public: // user status --------------------------------------------------------------------------------
//	IStatus*	QueryStatus(OBJID idType)		{ CHECKF(m_setStatus); return m_setStatus->GetObj(idType); }
//	IStatusSet*	QueryStatusSet()				{ CHECKF(m_setStatus); return m_setStatus; }
	void		ClearAllStatus	();
protected: // data
//	IStatusSet*	m_setStatus;

public: // common -------------------------------------
//	static int	AdjustData			(int nData, int nAdjust, int nMaxData=0);

public: // pet ----------------------------------------------------
	bool	Create(PROCESS_ID idProcess, IRecordset* pRes);
	void	SaveInfo();			//??? save all when close server!!!

public: // call pet ---------------------------------
	CAutoLink<CMonster>&	QueryLink()			{ return m_link; }
protected:
	CAutoLink<CUser>	m_pOwner;
	CAutoLink<CMonster>	m_link;
//	CMagicTypeData*		m_pMagicType;			// may be null
	CTimeOut			m_tMagic;
	CMagic*				m_pMagic;				// may be null 

public:
	DWORD	GetDropMoneyAvg();
	//20070206����:����teamID
	//bool	DropMoney	(DWORD dwMoney, OBJID idOwner);
	bool	DropMoney	(DWORD dwMoney, 
		OBJID idOwner,
		OBJID teamID=ID_NONE);

	// modifyed by zlong 2003-11-19
	// ��������magic2,magic3��Ĭ�ϲ���
	//20070206����:����teamID
	//bool	DropItem	(OBJID idItemType, OBJID idOwner, int nMagic2 = 0, int nMagic3 = 0, int nUserLuck = 0,bool bIsAllowDropUnident=false);
	bool	DropItem	(OBJID idItemType, 
		OBJID idOwner, 
		int nMagic2 = 0, 
		int nMagic3 = 0, 
		int nUserLuck = 0,
		bool bIsAllowDropUnident=false,
		OBJID teamID=ID_NONE);

protected:
	CUserPetData* m_pPetData;
	CEudemonTypeData* m_pPetTypeData;
	EudemonData m_eudemonData;

protected: // attrib
	OBJID		m_idNpc;
//	int			m_nPosX;
//	int			m_nPosY;
//	int			m_nDir;
	DWORD		m_dwStatus;
	I64			m_i64Effect;
	I64			m_i64Effect2;		//07.4.17�������

	int			m_nCurrLife;
	int			m_nCurrMana;

	int			m_nPotential;		// Ǳ��ֵ
	DWORD		m_dwMaskData;

	DWORD       m_addAtkType[4];
	DWORD       m_addDefType[4];
	OBJID		m_nSynId;
public:
	DWORD	GetMaskData()		{ return m_dwMaskData; }
	void	SetMaskData(DWORD dwMask)		{ m_dwMaskData = dwMask; }
	void	AddMaskData(DWORD dwMask)		{ m_dwMaskData |= dwMask; }
	void	ClsMaskData(DWORD dwMask)		{ m_dwMaskData &= ~dwMask; }

public:	// eudemon -------------------------------
	int GetPhyle();	
	// �˺���ֻ��Ҫ�ڴ������޵�ʱ�����һ�μ��ɣ����ڹ����Ѿ��ı䣬�Ժ���û���������
	void	SetEudemonAddi(CItem*	pEudemon) {};
	virtual DWORD	GetFidelity() { return 0; }	//	{ IF_NOT (IsEudemon() && m_pEudemonItem) return 0; return m_pEudemonItem->GetInt(ITEMDATA_FIDELITY); }

	OBJID	GetDivineID()	{ return ID_NONE; } //	{ IF_NOT (IsEudemon() && m_pEudemonItem) return 0; return m_pEudemonItem->GetDivineID(); }
	UCHAR	GetRelationShip(OBJID idDivine)	{ return 0; } // { IF_NOT (IsEudemon() && m_pEudemonItem) return 0; return m_pEudemonItem->GetRelationShip(idDivine); }
	bool	AddRelationShip(OBJID idDivine, int nValue)	{ return false; } // { IF_NOT (IsEudemon() && m_pEudemonItem) return 0; return m_pEudemonItem->AddRelationShip(idDivine, nValue); }

	// equipment
	bool	LoadEquipment(IDatabase* pDb);
	bool	EquipItem(CItem* pItem, int nPosition, bool bSynchro);
	bool	UnEquipItem(int nPosition, bool bSynchro);

	CItem*	GetEquipItem(OBJID idItem);
	CItem*	GetEquipItemByPos(int nPosition);
	bool	DropItem(OBJID idItem, int x, int y);
	void	UpdateEudemonAll();

protected:
//	DWORD	GetEudemonLev()		{ IF_NOT (IsEudemon() && m_pEudemonItem) return 0; return m_pEudemonItem->GetEudemonLevel(); }

	bool	TryItem(CItem* pItem, int nPosition);
	CItem*	UnEquipOnly(int nPosition);
	CItemPtr*	GetEquipItemPtr(int nPosition);
	CItemPtr&	GetEquipItemRef(int nPosition);

protected:
	// ���޸����Ը��ݵȼ����ɳ���������Ĳ�ֵ����Ҫ�ӳɵ������е�
/*
	struct {
		int			nMaxAtkAdd;			// ������
		int			nMinAtkAdd;
		int			nMaxMAtkAdd;		// ħ������
		int			nMinMAtkAdd;
		int			nDefAdd;			// �������
		int			nMDefAdd;			// ħ������
		int			nMaxLifeAdd;		// �������

		int			nAtkSpeedAdd;		// �����ٶ�
		int			nHitRateAdd;		// ������
		int			nDodgeAdd;			// �����

//		int			nEudemonLev;		// �ȼ�
	} m_AddInfo;


	CItem*		m_pEudemonItem;		// refrence
*/
	// Equipment
	CItem*		m_pHelmet;
	CItem*		m_pNecklace;
	CItem*		m_pArmor;
	CItem*		m_pWeaponR;
	CItem*		m_pRingL;
	CItem*		m_pShoes;
	
protected: // fight
	CBattleSystem*		m_pBattleSystem;
	CTimeOutMS			m_tFight;
	int					m_nFightPause;						// ��Ӳ״̬ʱ��һ�������ӵļ��������
	
	
	int					m_nKillNum;							// ɱ����Ŀ����
	int					m_nKillNum4Potential;				// ɱ��Ŀ����������Ϊ������Ǳ��ֵ������ͳ��
	int					m_nKillNum4RelationShip;			// ɱ��Ŀ����������Ϊ�����ӹ�ϵֵ������ͳ��

	int					m_nAtkMode;		// ��������

protected: // npctype
	CNpcType*	m_pType;		// refrence

protected: // application
	CTimeOut	m_tDie;
	auto_long	m_bLeaveMap;
	bool		m_bDecRole;

protected: // ctrl
	CTimeOut	m_tAddLife;
	CTimeOut	m_tIncLife;
	OBJID		m_idGen;
//	PROCESS_ID	m_idProcess;
	//20070209����:�ŵ�role����ȥ----------------
//	typedef struct
//	{
//		OBJID userID;
//		int	  enmity;
//		int	  defoult;
//	}ENMITY,*LPENMITY;
//	vector<ENMITY>	m_vEnmity;
	//-------------------------------------------
public:		
	bool MyAddEnmitySynNPC(OBJID id,int nEnmAddi /* = NULL*/,BOOL bSyn/* = true*/);

	//20080108����:�����µĳ�޲�ͬ����npc
	void AddEnmitySynNPC(OBJID id,
		int nEnmity,
		USHORT nMax = 0,
		int nEnmAddi = NULL,
		BOOL bSyn = true);

	//20080110����:��ճ�ޱ�.��ͬ����npc
	void ClsEnmityListSynNpc(BOOL bSyn = true);



// 	int GetMaxLifeGrow();
// 	int GetMaxManaGrow();
// 	int GetAttackGrow();
// 	int GetAttackMagicGrow();
// 	int GetDefanceGrow();
// 	int GetDefanceMagicGrow();
	int GetSkill1(); 
	int GetSkill2(); 
	int GetSkill3(); 
	int GetSkill4(); 
	int GetSkill5(); 
	int GetSkill6(); 
	int GetSkill7();
	int GetSkill8();
	int GetSkill9();
	int GetSkill10();
	int GetIdentity1();
	int GetIdentity2();
	int GetIdentity3();
	int GetIdentity4();
	int GetIdentity5();
	int GetIdentity6();
	int GetHabit();
	int GetSex();
	int GetPosition();
	OBJID GetPlayTimes();
	int GetEudemonType();
	OBJID GetHpTimes();
	OBJID GetMagTimes();
	OBJID GetPhyTimes();
	OBJID GetDefTimes();
	CTeam*	GetTeam();
	int		GetSkillCD(int nCDIndex);
	void	CreateAllCD(bool bSynchro=true);
	void	SetMagicLastuse(EUDEMONDATA idx, LPCTSTR szData, int nSize, bool bUpdate=false);
protected:
	//20080105����:�����µĳ�޲����¹���Ŀ��
	//����б���������
	//����µĳ��״Ԫ
	//�����ͬ�ڵ�ǰĿ��.����Ϣ��npc������
	//���ص�Ϊ�µĳ��״ԪID
	//���ǳ�޼�Ȩ
	/*����.�ⲿ���жϽ���npc�Լ����
	OBJID AddEnmityAndUpdateTarget(OBJID id,
		int nEnmity,
		int nEnmAddi = NULL);
		*/
	OBJID GetAttackTargetID(){if(m_pBattleSystem) return m_pBattleSystem->GetTargetID(); return ID_NONE;}

	//20080114����:���＼��---------------
	BOOL InitMagic();//���ܳ�ʼ��
	BOOL InitEudemonMagic();
	BOOL UpLevEudemonMagic();
	//���＼��end-------------------------

//�������ֵ
private:
	DWORD m_lastEudemonHappyCheck;
public:
	int GetUnderstand();
	int GetGetType();
	void CheckEudeHappy();

	void ClsBuff(int nStatus);
	void AddBuff( int nStatus,int nTime,int nTimes,int nPower,int nParam1,int nParam2,int nParam3 );
	void UpdateBuff( int nStatus,int nTime,int nTimes,int nPower,int nParam1,int nParam2,int nParam3 );
	bool UpdateMonsterProperty(bool bIncludeBuff=true);

	static float  GetQualityParam(int nQuality);
	static float  GetFusionParam(int nFusion);

//ai
protected:
	int			m_nBornX;
	int			m_nBornY;
	int         m_nAIStatus;
	int         m_nSeeUserNum;
	OBJID       m_idTarget;
//modify code by �ֵ�Ԩ begin 2011.6.22
	OBJID       m_idTargetBoss;
//modify code by �ֵ�Ԩ end
	int			CheckAIStatus(int nStatus);
	void		ProcessAIStatus(int nStatus);
	void		EnterAIStatus(int nStatus);
	void		LeaveAIStatus(int nStatus);
	void		UpdateAIStatus();

	OBJID		FindAttackUser(int nRange);
	void		ProcessWander();
	void		ProcessChase();
	void		ProcessSoonBack();
	void		ProcessAtk();
//ai֧��
public:
	bool		TestMagicCastEnable(OBJID idMagicType,OBJID idTarget,int x,int y);
	void		AddSeeUserNum() { m_nSeeUserNum ++ ;}
	void		DecSeeUserNum() { if(m_nSeeUserNum > 0) m_nSeeUserNum -- ;}
	void		Run(int x,int y,bool CheckFloor);
	void		Walk(int x,int y,bool CheckFloor);
	void		Standby();
	void		ClearPath()				{ m_nPathLen = 0; m_posTarget.x = 0;m_posTarget.y = 0;}
protected: // path ////////////////////////////////////////////////////////////////////////////
	bool		FindPath(int x, int y,bool CheckFloor = true);
	void		MoveNext(int nMoveMode);
	POINT		m_posTarget;
	int			m_nPathLen;
	int			m_nMoveMode;
	char		m_bufPath[_MAX_PATHSTEP];
	DWORD		m_dwMoveInterTime;
	CTimeOutMS	m_tMove;
	CTimeOutMS	m_tWander;
protected:
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_AINPC_H__52682938_5D82_4101_B888_6A4A3F30EB51__INCLUDED_)
