// Npc.h: interface for the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKNPC_H__D7A2DFC3_2C2C_4F67_BB52_8F55409E76BC__INCLUDED_)
#define AFX_TASKNPC_H__D7A2DFC3_2C2C_4F67_BB52_8F55409E76BC__INCLUDED_

#include "MyHeap.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "NpcShop.h"
#include "NpcStorage.h"
#include "NpcTable.h"
#include "Myheap.h"
#include "Role.h"
#include "AutoPtr.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUser;
class CGameMap;

/////////////////////////////////////////////////////////////////////////////////////
//define constant ..
const	int	ADD_NEW_NPC				= 0;
const	int LAY_NEW_NPC				= 10;
const	int PRE_CHANGE_POSITION		= 11;
const	int CHANGE_POSITION			= 12;
const	int DYNANPCDIEDELAY_SECS	= 3;		// DYNA_NPC�����������ɾ��
const	int	GOAL_EXP_PERCENT		= 10;		// ����ӵľ���ֵ�ٷֱ�(���εľ���ֵ��5%�Ļ���)
const	int	SYNFLAG_SIZEADD			= 5;		// �������ӵĸ��ӳߴ�
const	int	SYNWAR_GAINMONEY_PERCENT			= 10;		// ����ս�õ��Է�����ı���
const	int	DEFAULT_NPC_SIZEADD			= 4;		// ÿ�����������
const	int SYNWARLETTERNPC_CHECKINTERVAL = 60*20;//ÿ���20���Ӽ��ռ������Ƿ���7��
/////////////////////////////////////////////////////////////////////////////////////
const	int	_MAX_NPCTASK			=8;
const	int	MAX_NPCDATA				=4;		// ����С��_MAX_NPCTASK

/////////////////////////////////////////////////////////////////////////////////////
// ����ʹ�õ�DATA���
enum {
		SYNFLAGNPC_FIXFUND=0,SYNFLAGNPC_FIGHTBEGIN,SYNFLAGNPC_FIGHTEND,SYNFLAGNPC_MOREMAPID,					// ���ɱ�־(time:WWHHMMSS, sun day = 7)
		SYNWARLETTER_ISATTACK=0,SYNWARLETTER_ATTACK_SYNID=1,SYNWARLETTER_OCCUPYTIME=2,SYNWARLETTER_DEPOSIT=3,
		SYNDRESS_SORT=0,SYNDRESS_SETTING=1,
		GOALNPC_LEVEL=3,
		TIMERNPC_TIME=3,
		TRUNKNPC_SIZE=3,
		EUDEMON_BROODER_NPC_SIZE=2, EUDEMON_STORAGE_NPC_SIZE=3,
		STORAGENPC_SIZE=3,
		NEIGHBORNPC_X=1,NEIGHBORNPC_Y=2,NEIGHBORNPC_SERIAL=3,
};			//? Ŀǰһ�����֧��4������

/////////////////////////////////////////////////////////////////////////////////////
enum NPCTYPE{
		_NPC_NONE				= 0,			// ͨ��NPC
		_SHOPKEEPER_NPC			= 1,			// �̵�NPC
		_TASK_NPC				= 2,			// ����NPC(�����ϣ������ڼ��ݾ�����)
		_STORAGE_NPC			= 3,			// �Ĵ洦NPC
		_TRUNCK_NPC				= 4,			// ����NPC
		_FACE_NPC				= 5,			// ��ͷ��NPC
		_FORGE_NPC				= 6,			// ����NPC		(only use for client)
		_EMBED_NPC				= 7,			// ��ǶNPC
		_STATUARY_NPC			= 9,			// ����NPC
		_SYNFLAG_NPC			= 10,			// ���ɱ��NPC
		_ROLE_PLAYER			= 11,			// �������		(only use for client)
		_TASK_NPC_NEW			= 12,			// �Լ�			(only use for client)
		_ROLE_MONSTER			= 13,			// ����			(only use for client)
		_BOOTH_NPC				= 14,			// ��̯NPC		(CBooth class)
		_SYNTRANS_NPC			= 15,			// ���ɴ���NPC, �̶��Ǹ���Ҫ�ô�����! (����00:00�շ�)(LINKIDΪ�̶�NPC��ID��������ʹ��LINKID�Ļ���)
		_ROLE_BOOTH_FLAG_NPC	= 16,			// ̯λ��־NPC	(only use for client)
		_ROLE_MOUSE_NPC			= 17,			// ����ϵ�NPC	(only use for client)
		_ROLE_MAGICITEM			= 18,			// �����ǽ		(only use for client)
		_ROLE_DICE_NPC			= 19,			// ����NPC
		_ROLE_SHELF_NPC			= 20,			// ��Ʒ��
		_WEAPONGOAL_NPC			= 21,			// ��������NPC
		_MAGICGOAL_NPC			= 22,			// ħ������NPC
		_BOWGOAL_NPC			= 23,			// ��������NPC
		_ROLE_TARGET_NPC		= 24,			// ���򣬲���������	(only use for client)
		_ROLE_FURNITURE_NPC		= 25,			// �Ҿ�NPC	(only use for client)
		_ROLE_CITY_GATE_NPC		= 26,			// ����NPC	(only use for client)
		_ROLE_NEIGHBOR_DOOR		= 27,			// �ھӵ���
		_ROLE_CALL_PET			= 28,			// �ٻ���	(only use for client)
		_EUDEMON_TRAINPLACE_NPC	= 29,			// ����ѱ����
		 _AUCTION_NPC			= 30,			// ����NPC	��Ʒ��ȡNPC  LW
		 _ROLE_MINE_NPC			= 31,			// ��ʯNPC
		 
		 _ROLE_GRASS_MINE       = 32,           // �ݿ�
		 _ROLE_PET_MINE         = 33,           //����ʬ���
         _ROLE_BOX_MINE         = 34,           //����

		 //̽��ϵ����
		_ROLE_TASK_HAND         = 35,  //��   
		_ROLE_TASK_SWORD        = 36,  //����
	    _ROLE_TASK_CHU          = 37,  //��ͷ
		_ROLE_TASK_SKIFE        = 38,  //С��
		_ROLE_TASK_CHOPPER     = 39,  //��ͷ
		_ROLE_TASK_KEY          = 40,  //Կ��
		_ROLE_TASK_SAW          = 41,  //����
		_ROLE_TASK_HAMMAER      = 42,  //׶��
		_ROLE_TASK_DUKE         = 43,  //ȭͷ
		_ROLE_TASK_BARK         = 44,  //�㹳

		 _EUDEMON_EVO_NPC		= 32,			// ���޽���NPC
		 
		 _ROLE_3DFURNITURE_NPC	= 101,			// 3D�Ҿ�NPC 
		 
		 _SYN_NPC_WARLETTER     = 110,			//�����µģΣУ����͡�ר����������ս��ġ����ɣΣУ�
};

/////////////////////////////////////////////////////////////////////////////////////
enum NPCSORT{
		NPCSORT_NONE		= 0,
		NPCSORT_TASK		= 1,			// ������
		NPCSORT_RECYCLE		= 2,			// �ɻ�����
		NPCSORT_SCENE		= 4,			// ������(����ͼ���)
		NPCSORT_LINKMAP		= 8,			// �ҵ�ͼ��(LINKIDΪ��ͼID��������ʹ��LINKID�Ļ���)
		NPCSORT_DIEACTION	= 16,			// ����������(LINKIDΪACTION_ID��������ʹ��LINKID�Ļ���)
		NPCSORT_DELENABLE	= 32,			// �����ֶ�ɾ��(����ָͨ������)
		NPCSORT_EVENT		= 64,			// ����ʱ����, ʱ����data3�У���ʽΪMMWWHHMMSS��(LINKIDΪACTION_ID��������ʹ��LINKID�Ļ���)
		NPCSORT_TABLE		= 128,			// �����ݱ�����

//		NPCSORT_SHOP		= ,			// �̵���
//		NPCSORT_DICE		= ,			// ����NPC

		NPCSORT_USELINKID	= NPCSORT_LINKMAP | NPCSORT_DIEACTION | NPCSORT_EVENT,
		NPCSORT_DYNANPC_INSTANCE     = 256,			//����ר��NPC
		
};

/////////////////////////////////////////////////////////////////////////////////////
struct NPCINFO_ST{
	OBJID	idOwner;
	int		nOwnerType;
	OBJID	idMap;
	int		nPosX;
	int		nPosY;
	int		nType;
	int		nLookFace;
	DWORD	nAction;
	int		nSort;
	UINT    nNpcType;
	USHORT	nLev;
};

/////////////////////////////////////////////////////////////////////////////////////
enum NPCDATA{
	    NPCDATA_ID,
		NPCDATA_NAME = 1,
		NPCDATA_LOOKFACE, STATUARYDATA_LOOKFACE = NPCDATA_LOOKFACE,
		NPCDATA_MAPID,
		NPCDATA_POSX_,		// �ⲿ��Ҫֱ��ʹ�ã�ע����m_nPosX��ͬ��
		NPCDATA_POSY_,		// �ⲿ��Ҫֱ��ʹ�ã�ע����m_nPosY��ͬ��
		NPCDATA_SORT,
		NPCDATA_TYPE,
		NPCDATA_ACTION,
		NPCDATA_LEVEL,
		NPCDATA_SHOWTIME,
		NPCDATA_HIDETIME,
		NPCDATA_SHOWSPAN,
		NPCDATA_HIDEMSPAN,
};

typedef	IGameData<NPCDATA>	INpcData;
char	szNpcTable[];
typedef	CGameData<NPCDATA,szNpcTable,szID>	CNpcData;
char	szDynTable[];
typedef	CGameData<NPCDATA,szDynTable,szID>	CDynaNpcData;

/////////////////////////////////////////////////////////////////////////////////////
//class CNpcTrunk;
// class CNpcTask;
// class CNpcScene;
// class CNpcLink;
// class CNpcBet;
class CNpc : public CGameObj, public CRole
{
protected:
	CNpc();
	virtual ~CNpc();
public:
	static CNpc* CreateNew()								{ return new CNpc; }
	bool	CreateSynMapNpc(PROCESS_ID idProcess, NPCINFO_ST* pInfo, OBJID idItemType, LPCTSTR szName);
	bool	Create(PROCESS_ID idProcess, NPCINFO_ST* pInfo, OBJID idItemType, LPCTSTR szName);
	bool	Create(PROCESS_ID idProcess, NPCINFO_ST* pInfo, OBJID idItemType, LPCTSTR szName,CGameMap* pGameMap);
	bool	Create(PROCESS_ID idProcess, IRecordset* pRecordset, bool bDynaNpc);
	IRole*			QueryRole()									{ return (IRole*)this; }
	virtual		bool		FindAroundObj	(OBJID idObjType, OBJID idObj, void** pp)		{ return GetMap()->QueryObj(GetPosX(), GetPosY(), idObjType, idObj, pp); }
public: // IRole ================================================================================
	virtual void	ReleaseByOwner()							{ delete this; }
	virtual OBJID	GetID()										{ return m_id; }
	virtual int 	GetObjType()								{ return CGameObj::GetObjType(); }
	virtual int		GetPosX()									{ return m_nPosX; }//m_pData->GetInt(NPCDATA_POSX); }
	virtual int		GetPosY()									{ return m_nPosY; }//m_pData->GetInt(NPCDATA_POSY); }
	virtual void	SendShow(IRole* pRole);							// ����ҷ��������������

public: // get attrib
	virtual LPCTSTR	GetName				();
//	virtual LPCTSTR	GetMate				()						{ return NOMATE_NAME; }

	virtual DWORD	GetLookFace			()						{ return m_pData->GetInt(NPCDATA_LOOKFACE); }
	virtual DWORD	GetHair				()						{ return /*m_pData->GetInt(STATUARYDATA_RANKSHOW_HAIR)&0xFFFF*/0; }
	virtual int 	GetDir				()						{ return /*MaskDir(m_pData->GetInt(NPCDATA_LOOKFACE))%MAX_DIRSIZE*/2; }
	virtual I64		GetEffect			()						{ return m_i64Effect; }
	virtual DWORD	GetLev				()						{ return m_pData->GetInt(NPCDATA_LEVEL); }

	virtual DWORD	GetLife				()						{ return /*m_pData->GetInt(NPCDATA_LIFE)*/100; }
	virtual DWORD	GetMaxLife			()						{ return /*m_pData->GetInt(NPCDATA_MAXLIFE)*/100; }

	virtual DWORD	GetAtkHitRate		()						{ ASSERT(!"GetAtkHitRate"); return 0; }
	virtual DWORD	GetAddAttByType     (int nType)				{ return 0; }
	virtual DWORD	GetAtkType			()						{ return 0; }

	virtual int		GetArmorTypeID		();
	virtual int		GetWeaponRTypeID	();
	virtual int 	GetWeaponLTypeID	();
	virtual int 	GetMountTypeID		();
	virtual int		GetWeaponSpecialView()						{ return 0; }

//	virtual CGameMap*	GetMap			()						{ CHECKF(m_pMap); return m_pMap; }

	virtual DWORD	GetSynID			()						{ return /*m_pData->GetInt(NPCDATA_OWNERTYPE)==OWNER_SYN ? m_pData->GetInt(NPCDATA_OWNERID) : ID_NONE*/0; }
	virtual DWORD	GetSynRankShow		()						{ return /*m_pData->GetInt(STATUARYDATA_RANKSHOW_HAIR)>>16*/0; }

public: // const
	virtual bool	IsAlive				()						{ return /*!(IsDynaNpc() && m_pData->GetInt(NPCDATA_MAXLIFE)>0 && m_pData->GetInt(NPCDATA_LIFE)<=0 ||*/ !IsDeleted()/*)*/; }		// NPCDATA_LIFE is a flag
	virtual bool	IsDelThis			()						{ return IsDeleted() && m_tDie.IsTimeOut(); }		// ASSERT(!IsAlive()); 
	virtual BOOL	IsNpc				()						{ return true; }

	// return true: ppv���ض���ָ��
	virtual bool	QueryObj(OBJID idObjType, void** ppv)		{ if(idObjType==OBJ_NPC) return *ppv=this,true; return *ppv=NULL,false; }
	virtual bool	QueryRole(void** ppv)						{ return *ppv=QueryRole(),true; }
	virtual CUser*	QueryOwnerUser		()						{ return NULL; }		//@@@ ��
	virtual void    ClsBuff				(int nStatus)			{}
public:	// modify attrib ------------------------------
	virtual	void	SetStatus			(int nStatus, bool bSynchro=true, int nPower=0);
	virtual	void	ClsStatus			(int nStatus, bool bSynchro=true);
	virtual bool	AddAttrib			(int idxAttr, __int64 i64Data, int bSynchro);
	virtual void	ProcessOnMove		(int nMoveMode)						{}

	virtual	IStatus*	QueryStatus(OBJID idType)
	{
		return CRole::QueryStatus(idType);
	}
	virtual	IStatusSet*	QueryStatusSet()									
	{
		return CRole::QueryStatusSet();
	}

public: // money --------------------------------
	virtual bool	InitNpc(int nParam1,int nParam2){ return true;}
public: // system
	virtual void	BroadcastRoomMsg(CNetMsg* pMsg, 
		bool bSendSelf = false,
		EM_SENDNPCFLAG sendNpc = EM_SENDNPCFLAG_TRUE);
	//-------------------------------------
	virtual void	BroadcastRoomMsg(LPCTSTR szMsg, bool bSendSelf = false);
	virtual void	OnTimer(time_t tCurr);

public: // map
	virtual int		MoveToward(int nDir)									{ return 0; }
	virtual void	TransPos(int nPosX, int nPosY);
	virtual void	JumpPos(int nPosX, int nPosY)							{}
	virtual bool	SyncTrackTo(int nPosX, int nPosY, int nDir, DWORD dwAction)		{ return 0; }
	virtual void	GetFootPrint	(int& nPosX, int& nPosY)				{ nPosX = GetPosX(); nPosY = GetPosY(); }

public: // fight ------------------------------
	virtual void	ClrAttackTarget		()									{ SetAttackTarget(NULL); }
	virtual bool	SetAttackTarget(IRole* pTarget)							{ return true; }
	virtual bool	AutoSkillAttack(IRole* pTarget)							{ return false; }
	virtual int		Attack( IRole* pTarget,DWORD& dwBattleAddInfoRef )		{ ASSERT(!"Attack"); return 0; }
	virtual bool	BeAttack(bool bMagic, IRole* pTarget, int nPower, bool bReflectEnable=true);
	virtual bool	MagicAttack(int nType, OBJID idTarget, int x, int y,bool needLearn = false,int learnLev = 0)	{ ASSERT(!"MagicAttack"); return 0; }
	virtual void	Kill(IRole* pTarget, DWORD dwDieWay)					{ ASSERT(!"Kill"); }
	virtual void	BeKill(IRole* pTarget = NULL);
	virtual bool	IsAtkable(IRole* pTarget, bool bSendHint=false)			{ return false; }
	virtual bool	IsBeAtkable();
	virtual int		GetAttackRange(int nTargetSizeAdd)						{ ASSERT(!"GetAttackRange"); return 0; }
	virtual int		GetSizeAdd()											{ if(IsSynFlag()) return SYNFLAG_SIZEADD; return DEFAULT_NPC_SIZEADD; }
	virtual void	SendDamageMsg( OBJID idTarget,int nDamage,DWORD dwBattleAddInfo )		{ ASSERT(!"SendDamageMsg"); }
private: // IRole ================================================================================
//	int			m_nPosX, m_nPosY;

public: // const
	int 			GetSort()									{ return m_pData->GetInt(NPCDATA_SORT); }
	int 			GetType()									{ return m_pData->GetInt(NPCDATA_TYPE); }
	void			ChangeDir(int nLook)						{ m_pData->SetInt(NPCDATA_LOOKFACE, MakeLookFace(MaskLook(GetLookFace()), MaskDir(nLook))); }
	bool			IsRecycleEnable()							{ return (GetSort() & NPCSORT_RECYCLE) != 0; }
	enum { MATE_ENABLE=true };
	bool			IsOwnerOf(CUser* pUser, bool bMateEnable=false);
	bool			IsDieAction()								{ return (GetSort() & NPCSORT_DIEACTION) != 0; }
	bool			IsStatuary()								{ return GetType() == _STATUARY_NPC; }		// �Զ�����CGameMap::m_idSyn
	bool			IsGoal()									{ return GetType()==_WEAPONGOAL_NPC || GetType()==_MAGICGOAL_NPC || GetType()==_BOWGOAL_NPC; }
	bool			IsSynFlag()									{ return GetType() == _SYNFLAG_NPC; }
	bool			IsAwardScore()								{ return GetType() == _SYNFLAG_NPC; }
	bool			IsDelEnable();
	bool			IsSynNpc()									{ return /*GetInt(NPCDATA_OWNERTYPE)==OWNER_SYN && GetInt(NPCDATA_OWNERID)!=ID_NONE*/false; }
	bool			IsUserNpc()									{ return /*GetInt(NPCDATA_OWNERTYPE)==OWNER_USER && GetInt(NPCDATA_OWNERID)!=ID_NONE*/false; }
	int 			GetStorageSize()							{ return /*GetData(STORAGENPC_SIZE)*/0; }
	bool			IsSynMoneyEmpty();
	bool			IsShelfNpc()								{ return /*m_pData->GetInt(NPCDATA_TYPE) == _ROLE_SHELF_NPC*/0; }
	bool			IsSynWarLetter()							{ return GetType() == _SYN_NPC_WARLETTER; }//�Ƿ�����ս��ģΣУ�
	
public: // application
	bool			SendSelfToBlock();
	bool			SendLeaveFromBlock();
	bool			ActivateNpc(IRole* pRole, int nAction);			// ��������в���
	bool			ActivateShop(IRole* pRole);
	CItem*			Recycle(OBJID idUser);			//??? need call DelNpc() follow
	bool			DelNpc();						// call this mast !IsDeleted()
	bool			EnterMapGroup();
protected:
	bool			DeleteRecord()								{ CHECKF(IsDynaNpc()); return m_pData->DeleteRecord(); }
	void			CheckFightTime();
	void			RecruitLife();
	void			BroadcastScore();

public:
	//---------- NPC ���Դ�ȡ���� --------------------------
	int				GetInt		(NPCDATA idx)					{ CHECKF(m_pData); return m_pData->GetInt(idx); }
	LPCTSTR			GetStr		(NPCDATA idx)					{ CHECKF(m_pData); return m_pData->GetStr(idx); }
	OBJID			GetMapID	();

	//---------- �޸�NPC���� ------------------
	void			SetInt		(NPCDATA idx, int nData, bool bUpdate = false)	{ CHECK(m_pData && IsDynaNpc()); m_pData->SetInt(idx, nData); if(bUpdate) m_pData->Update(); }
	void			SetStr		(NPCDATA idx, LPCTSTR szData, int nSize, bool bUpdate = false)	{ CHECK(m_pData && IsDynaNpc()); m_pData->SetStr(idx, szData, nSize); if(bUpdate) m_pData->Update(); }
/*	void			SetData		(int idx, int nData, BOOL bUpdate)	{ 
										CHECK(m_pData && IsDynaNpc()); 
										CHECK(idx < MAX_NPCDATA); 
										m_pData->SetInt(NPCDATA(NPCDATA_DATA0 + idx), nData); 
										if(bUpdate) m_pData->Update(); }*/
	void			SaveInfo	();
	BOOL			ChangePos		(OBJID idMap, int nPosX, int nPosY);
	bool			SetSynOwnerID(OBJID idSyn, BOOL bWithLinkMap);		// true: with link map
	bool			SetUserOwnerID(OBJID idUser, BOOL bWithLinkMap);		// true: with link map

public: // ����ӿ� ------------------------------------------------------------------	
	virtual bool		IsShopNpc()							{ return m_pData->GetInt(NPCDATA_SORT) == _SHOPKEEPER_NPC; }		// || (m_pData->GetInt(NPCDATA_SORT)&NPCSORT_SHOP)!=0
	virtual bool		IsTaskShopNpc()						{ return ( m_pData->GetInt(NPCDATA_SORT) == _TASK_NPC || m_pData->GetInt(NPCDATA_SORT) == _TASK_NPC_NEW) && (m_pData->GetInt(NPCDATA_TYPE) == 1 );  }
	virtual bool		IsRandTaskShopNpc()					{ return ( m_pData->GetInt(NPCDATA_SORT) == _TASK_NPC || m_pData->GetInt(NPCDATA_SORT) == _TASK_NPC_NEW) && (m_pData->GetInt(NPCDATA_TYPE) == 2 );  }
	virtual CNpcShop*	QueryShop()							{ CHECKF(m_pShop); return m_pShop; }

	OBJID				GetRecordID()						{ CHECKF(m_pData); return m_pData->GetID(); } 
	bool				IsDeleted()							{ return m_tDie.IsActive(); }

	bool				IsDynaNpc()							{ return m_bDynaNpc; }
	bool                IsTimeDuring(unsigned long nBegin, unsigned long nEnd, time_t tCurr/* =0 */);
private:
	bool				CreateFuncObj(PROCESS_ID idProcess);
	bool				CreateFuncTrunk(PROCESS_ID idProcess);
	bool				CreateFuncEudemonBrooder(PROCESS_ID idProcess);
	bool				CreateFuncEudemonStorage(PROCESS_ID idProcess);
	bool				CreateFuncEudemonEvo(PROCESS_ID idProcess);
	bool				CheckSortMutex();
	void				UpdateHoldFloor(bool bEnter);
public:
    void                SetNpcOwner(OBJID idUser);
	OBJID               GetNPcOwner(){return m_nToken;}
	bool                IsActive(unsigned long nBegin);//�Ƿ񼤻�
	bool                MovePos(int posX,int poxY);   //Npc�ƶ�
	UINT                GetNpcType()  { return m_nNpcType; };
	bool				IsShow();
	void				SetDelShow();
protected:
	CNpcShop*			m_pShop;

protected: // data
	OBJID			m_id;
	INpcData*		m_pData;
	bool			m_bDynaNpc;
	I64				m_i64Effect;
public:
   clock_t         m_beginTime;
   int             m_startPosX;
   int             m_startPosY;
protected: // ctrl
	CTimeOut		m_tSynEvent;
	CTimeOut		m_tBroadcastScore;
	CTimeOut		m_tDie;
	CTimeOut		m_tSynWarLetter;//ÿ���20���Ӽ��ռ������Ƿ���7��
	CTimeOut		m_tUpdateShow;
	bool			m_bFeeOK;

	bool            m_bTimeDuring; //�Ƿ���ˢ��ʱ����֮��
	bool            m_bNPCShow;
	bool            m_bSetShow;
protected:
	OBJID           m_nToken;
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_TASKNPC_H__D7A2DFC3_2C2C_4F67_BB52_8F55409E76BC__INCLUDED_)
