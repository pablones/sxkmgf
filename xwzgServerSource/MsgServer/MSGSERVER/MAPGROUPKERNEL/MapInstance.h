// MapInstance.h: interface for the CMapInstance class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPINSTANCE_H__1CB83F41_11BA_4625_B1B8_249123985025__INCLUDED_)
#define AFX_MAPINSTANCE_H__1CB83F41_11BA_4625_B1B8_249123985025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sigslot.h"
#include "../share/protocol.h"
using namespace sigslot;

#include <string>
#include <set>
#include <vector>
using namespace std;

const int INSTANCETYPE_USER    = 0; //��ͨ���������ˣ�
const int INSTANCETYPE_TEAM    = 1; //��ͨ��������ӣ�

const int SYNMEMPOSUPDATE_INTERVAL = 300;  //���¾��ų�Աλ�ü��
const int MUSTKICKOUT_INTERVAL = 10*1000;  //ǿ�����˼��10��


struct StateItemSortInfo
{
	OBJID idUser;
	UINT  nAmount;
};

struct StateItemSortInfoLess: public binary_function<StateItemSortInfo&, StateItemSortInfo&, bool>
{
	bool operator()(const StateItemSortInfo& info1, const StateItemSortInfo& info2) const
	{
		return info1.nAmount < info2.nAmount;
	}
};

class CInstanceUser///��¼�����е������Ϣ
{
public:
	CInstanceUser(){m_idUser=ID_NONE;m_nTeamGroup=0;m_dwSynMemUpdateTime=0;m_bKicked=false;}
	CInstanceUser(OBJID idUser){m_idUser=idUser;}
	CTimeOut& GetTeamLimitTimer(){return m_tTeamLimit;}
	CTimeOut& GetSynLimitTimer(){return m_tSynLimit;}
	OBJID	GetID(){return m_idUser;}
	void	SetID(OBJID idUser){m_idUser=idUser;}
	OBJID	GetTeamGroup(){return m_nTeamGroup;}
	void	SetTeamGroup(UINT nTeamGroup){m_nTeamGroup=nTeamGroup;}
	void    SetSynMemTime(DWORD dwTime) { m_dwSynMemUpdateTime = dwTime; }
	DWORD   GetSynMemTime()  { return m_dwSynMemUpdateTime; }
	bool	GetBeKickOut() { return m_bKicked;}
	void	SetBeKickout() { m_bKicked = true;}
	OBJID	m_idUser;
private:
	CTimeOut m_tTeamLimit;///��Ӷ�ʱ��]
	CTimeOut m_tSynLimit;///�˰ﶨʱ��
	UINT	 m_nTeamGroup;///�ֶӱ�ʶ,1,2,3,4,5
	DWORD    m_dwSynMemUpdateTime;
	bool	 m_bKicked;
};

typedef map<OBJID,CInstanceUser> InstanceUserList;	
typedef vector<OBJID> VECMEM;
typedef vector<OBJID>::iterator ITMEM;
typedef set<OBJID> setFreeUser;
typedef set<OBJID>::iterator FreeUserIt;

///softworms-������-2008-01-04

char	szDummyDataObjectTable[];
typedef	CGameData<LPCSTR, szDummyDataObjectTable, szDummyDataObjectTable>	CInstanceTypeData;///�����ģ�巽ʽ,��������,Ӧ�ú���Ĳ���Ӧ�ñ�ɺ�������.
typedef ISingleObjSet2<CInstanceTypeData> IInstanceTypeSet;
typedef	CSingleObjSet2<CInstanceTypeData>  CInstanceTypeSet;

class CDataObject;

class CMapInstance  : public has_slots<>
{
public:
	static const int InstanceTeam;
	static const int InstanceSyn;
	static const int InstanceCity;
	enum InstanceType
	{
		itTaskInstance=1,
		itRoomInstance=2,
	};
	enum InstaceStatus
	{
		isInit=1,
	};
	static CMapInstance* CreateMapInstance();///�ù���ģʽ��������ʵ��
public:
	virtual ~CMapInstance();
	bool	IsLeaveSave();
	BOOL	AddPlayer(OBJID idUser);
	void	EnterScript(OBJID idUser);
	BOOL	RemovePlayer(OBJID idUser);
	BOOL    CityWarBeKill(CUser* pUser);
	BOOL	ClearPlayerList();
	BOOL	IsActive();///�жϸ����Ƿ����,Ŀǰ�򵥹���,�����Ͷ�ʱ��;
	BOOL	IsTimeOut();///�жϸ����Ƿ�ʱ
	BOOL	IsRecycle();///�жϸ����Ƿ����,Ŀǰ�򵥹���,�����Ͷ�ʱ��;
	BOOL	InitMapInstance(int idProcess,PROCESS_ID pid,OBJID idUser,UINT nInstanceType,UINT nInstanceScope, UINT nDoMain=0);///��ʼ������,����������ˢ�ֻ���һЩ׼������
	BOOL	ResetMapInstance();///���ø���
	BOOL	CloseInstance();///�رո���,���������ڽ����򸱱�Ŀ����ʱ����
	BOOL    CloseCityWarInstance(bool bAttack); //׼���رճ�ս����
	BOOL    CloseSynWarInstance(bool bAttack); //׼���رհ�սս����
	void    ProcessSWTBeKill(bool bAttack, UINT type);  //��ս���ﱻɱ
	void	ShutdownInstance(UINT nTimeOut);///�������������ڳ�ʱ�¼�
	friend  class CMapInstanceMgr;///����CMapInstanceMgr��ȵ���
	OBJID	GetType(){return m_pData->GetInt("type");}
	bool    IsAttackFull(int nAmount);
	bool    IsDefenceFull(int nAmount);
	int		GetLifeTime()
	{
		//����߻�û���ڽű�����д��ֵ��ֱ�Ӵ����ݿ��
		if(INSTANCE_EXPIRED_LIFETIME==0)
		{
			INSTANCE_EXPIRED_LIFETIME = m_pData->GetInt("lifetime");
		}

		return INSTANCE_EXPIRED_LIFETIME;
	}
//	void	SetType(OBJID idOwd){m_idOwn=idOwd;}
	OBJID	GetOwnID(){return m_idOwn;}
	void	SetOwnID(OBJID idOwd){m_idOwn=idOwd;}
	OBJID	GetInstanceType()
	{
		return m_pData->GetInt("id");
	}
//	void	SetInstanceType(UINT nInstanceType){m_nInstanceType=nInstanceType;}
	UINT	GetMapType(){return m_nMapType;}
//	void	SetMapType(UINT nMapType){m_idMapType=nMapType;}
	OBJID	GetMapID(){return m_idMap;}
	void	SetMapID(OBJID idMap){m_idMap=idMap;}
//	OBJID	GetNpcID(){return m_idNpc;}
//	void	SetNpcID(OBJID idNpc){m_idNpc=idNpc;}
	
	OBJID	GetOwnProcessID(){return m_idOwnProcess;}
	BOOL	IsEmpty();
	signal1<OBJID>	slotUserLeaveTeam;///�û��ڸ���������¼�
	signal1<OBJID>	slotUserRejoinTeam;///�û��ڸ����ڻض��¼�
	signal0<>		slotLifeTimeout;///�����������ڳ�ʱ�¼�
	signal0<>	slotInit;///������ʼ���¼�

	void  SetLeaveSynExpire(UINT nExpire);
	UINT  GetLeaveSynExpire();
	void  ResetInstanceLife(UINT nLiftTime);

	UINT	GetBornX() { return m_posRebornX;}
	UINT	GetBornY() { return m_posRebornY;}

	//��߻����ų�ʱʱ��ӿ�
	void SetNoUserExpire(UINT nExpire)
	{
		INSTANCE_EXPIRED_TIME= nExpire;
	}
	void SetRecycleExpire(UINT nExpire)
	{
		INSTANCE_EXPIRED_RECYCLETIME = nExpire;
	}
	void SetLeaveTeamExpire(UINT nExpire)
	{
		INSTANCE_EXPIRED_TEAMTIME = nExpire;
	}
	void SetInstanceLife(UINT nLiftTime)
	{
		ResetInstanceLife(nLiftTime);
	}

	void SetInstanceActiveOut(UINT nLiftTime)
	{
		m_tTimeActiveOut.Startup(nLiftTime);
		m_bAllowIn = false;
	}

	void SetUserMaxNumbers(UINT MaxNumbers)
	{
		INSTANCE_USER_MAX_NUMBERS = MaxNumbers;
	}

	void SetNewGenerageInterval(UINT timeValue)
	{
		INSTANCE_NEWGENERATE_TIME = timeValue;
	}

	void SetUserMinNumbers(UINT MinNumbers)
	{
		INSTANCE_USER_MIN_NUMBERS = MinNumbers;
	}

	void SetCountDownInstance(UINT nCountDownInstance)
	{
// 		INSTANCE_COUNTDOWN_TIME = nCountDownInstance;
// 		m_tCountDownInstance.Startup(GetLifeTime()-INSTANCE_COUNTDOWN_TIME);
	}
	
	//��ս�����������
	void SetDomainMapID(UINT nDomainMapID)
	{
		m_nDomainMapID = nDomainMapID;
	}

	void SetUserLev(UINT nUserLev)
	{
		m_nUserLev = nUserLev;
	}

	void SetAttackSynID(UINT nAttackSynID)
	{
		m_nAttackSynID = nAttackSynID;
	}

	void SetDefenceSynID(UINT nDefenceSynID)
	{
		m_nDefenceSynID = nDefenceSynID;
	}

	void SetInstanceScope(UINT nInstanceScope)
	{
		m_nInstanceScope = nInstanceScope;
	}
	UINT GetInstanceScope()
	{
		return m_nInstanceScope;
	}
	//��Ӧ��GET����������߻�����
	UINT GetNoUserExpire()
	{
		return INSTANCE_EXPIRED_TIME;
	}
	UINT GetRecycleExpire()
	{
		return INSTANCE_EXPIRED_RECYCLETIME;
	}
	UINT GetLeaveTeamExpire()
	{
		return INSTANCE_EXPIRED_TEAMTIME;
	}
	UINT GetInstanceLife()
	{
		return INSTANCE_EXPIRED_LIFETIME;
	}

	UINT GetInstanceNewGenerage()
	{
		return INSTANCE_NEWGENERATE_TIME;
	}

	UINT GetUserMaxNumbers()
	{
		return INSTANCE_USER_MAX_NUMBERS;
	}

	UINT GetUserMinNumbers()
	{
		return INSTANCE_USER_MIN_NUMBERS;
	}

	UINT CreateInstanceNpc(UINT nNpcType,int cellx,int celly);	//��������NPC
	UINT CreateChessMonsters(UINT nMonsterTypeA,int nMonsterACount,UINT nMonsterTypeB,int nMonsterBCount,int StartX,int StartY,int nMonsterWidth);
	UINT SetRebronPos(UINT posX,UINT posY);

	void CreateMonsterInInstance(UINT nMonsterType,int cellx,int celly);	//�ڸ����ﴴ��ָ�����͵Ĺ֣�����GMˢ������
    BOOL DeleteMonster(int nType);
	bool DeleteNpc(UINT nNpcType);

	void RegisterLuaInterface();
	void RegisterLuaInterfaceOntime(int nAction);

	BOOL IsInstanceUserFull();

	void SendInstanceMsg(const char* pMsg);				//�����������������ҷ�����Ϣ

	void MoveMonsterDest(UINT ntype,int x,int y);

	void SetOutInstancePos(UINT idMap,int cellx,int celly);

	void SetInstanceRebornPos(int attackPosX, int attackPosY, int defencePosX, int defencePosY, int nType);
	BOOL CanEnterInstance();
// 	void SynWarTargetBekill(bool bBoss, bool bAttack); //��սĿ�걻ɱ

	//zeng���������������,������ո���
	void ClearUser();

	bool BuyBeDestroy();
	bool ChgMapBeDestroy();
	bool TaskBeDestroy();
	bool HasMonster(int nType,int nCheckAmount = 1);
	bool HasMonster2(int nType,int nCheckAmount);
	BOOL ActiveKickAllOut();

	void addFreeUser(OBJID idUser);
	bool IsFreeUser(OBJID idUser);
	//��������غ���
	void RequestEnterArena(const string& TeamName,UINT UserID);
	void EnterArena(const string& TeamName,UINT UserID);
	bool IsEndArena() {return m_bArenaEnd;}
	bool OperateData(int nIndex,const char* szParam);
	bool SetInstanceTimer(int nAction,UINT nPassSec);
	int	 GetOperateData(int nIndex);
	void SetMonsterKillEvent(OBJID nMonsterType,UINT nMonsterCount,UINT nContext);
protected:
	void   OnMonsterBeKillEvent(UINT nMonsterType);
	void   OnMonsterBeKill(OBJID idMonster);
	BOOL	RunScript(InstaceStatus isStatus);
	BOOL	RunScriptOntime(int nAction);
	BOOL    RunScriptMonsterKill(int nAction);
	void	GeneratMonster(UINT nMonsterType,UINT nPosX,UINT nPosY);
	void    NewGenerat(OBJID idGen);
	BOOL	RemoveInstance();///�������,��InitMapInstance���
	void	NotifyOwnDestory();///֪ͨ����ӵ���߸����ѹر�
	BOOL	CreateInstanceMap();///Ϊ������������ӵ�еĵ�ͼ,���ͼ��������������
	void	OnLifeTimeout();///�������������ڳ�ʱ�¼�
	void	OnInit();///�������������ڳ�ʱ�¼�
	void	OnUserQuitTeam(OBJID idUser);///�����������¼�
	void	OnUserRejoinTeam(OBJID idUser);///������һض��¼�
	BOOL	IsUserLevelTeam(CUserPtr UserPtr);
	BOOL	RemoveInstanceMap();///ɾ��������������ӵ�еĵ�ͼ,���ͼ��������������
	BOOL	KickOutAllPlayer();///�߳��������
	BOOL	KickOutPlayer(OBJID idPlayer,OBJID idMap=3001, int nPosX=200, int nPosY=300);///�߳�ָ�����
	BOOL	ProcessAllPlayerEvent();///������������ڸ����е��¼�
	BOOL	ProcessInstanceEvent();///�����������¼�
	void	ProcessMonsterEvent();///��������¼�
	BOOL	ProcessPlayerEvent(CInstanceUser& userptr);//////����������¼�
	BOOL	IsUserInstanceTeam(CUserPtr UserPtr);///�жϸ��û��Ƿ����ڸ���������
	void	CloseMonsterGenerat(OBJID idGen);///�رո����е�GEN
	CMapInstance();
	void	OnTimer(time_t tCurr);///��ʱ��������
	void    StartUpNewGen(UINT timeVal);//����ˢ��ʱ���


    BOOL    PvpSample(const char *pText);
	bool    CitySynWarJoin(OBJID idUser, int nType);   //��ս����ս��Ա���븱��
	bool    SynWarAward(bool bAttack);                 //��ս��������
	void    AddAttackPlayer(OBJID idPlayer);
	void    AddDefencePlayer(OBJID idPlayer);
	void    ClearFreeUser();

	int     instanceData[2];
	int     m_nActionForTimer;
private:
	CInstanceTypeData * m_pData;
//	OBJID	m_idInstance;
//	UINT	m_nInstanceType;///������չ
//	UINT	m_nInstanceStatus;///������չ
	OBJID	m_idMap;///�������ڵ�ͼid
//	OBJID	m_idNpc;///�������ڵ�ͼ����id
	OBJID	m_idOwn;///�����߱��,�����Ƕ���ID����playerid
//	UINT	m_nExpireTime;///����������Чʱ��,���ڽ�ɾ���ø���
//	UINT	m_nLifeTime;///��������������,���ڽ�������Ҵ�������,ɾ���ø���
//    typedef vector<OBJID> PlayerList;	
//    typedef map<OBJID,CTimeOut> TeamLimitTimerList;	
//	PlayerList m_PlayerList;
	InstanceUserList m_InstanceUserList;
	VECMEM   m_vecAttackMember;
	VECMEM   m_vecDefenceMember;
	CTimeOut m_tForDelay;
	CTimeOut m_tExpiredInstance;
	CTimeOut m_tLifeTimeInstance;
	CTimeOut m_tRecycleInstance;
	CTimeOut m_tCountDownInstance;
	CTimeOut m_tLifeTimeNewGenerage;
	CTimeOut m_tLifeCityWarVictory;
	CTimeOut m_tTimeActiveOut;
	
	int		m_idProcess;//��ͼ��ID
	int		m_idOwnProcess;//ӵ���ߵ�ͼ��ID
	UINT	m_InstanceType;

	
	UINT	m_posRebornX;
	UINT	m_posRebornY;
//private: // ctrl
//	MYHEAP_DECLARATION(s_heap)
	typedef struct NPCINFO {
		UINT		NpcType;
		CNpc*		pNpc;
		NPCINFO(){}
		NPCINFO(UINT npctype,CNpc* pnpc){NpcType = npctype;pNpc = pnpc;}
	}NPCINFO;
	map<UINT,NPCINFO>	m_InstanceNpcs;
//	vector<OBJID>       m_Monster;
	map<OBJID,UINT>		m_MonsterCount;///��������������
	setFreeUser			m_SetFreeUser;
//	BOOL m_bSetExpiredTime;
	

	//��Ҵ���������λ��
	UINT	m_OutMapID;
	int		m_OutCellX;
	int		m_OutCellY;

	UINT    m_nMapType;  //��ͼ����
	UINT    m_nUserLev;

	//��������������
	int     m_nRebornAttackX;
	int     m_nRebornAttackY;
	int     m_nRebornDefenceX;
	int     m_nRebornDefenceY;

	//zeng�޸ģ�д��̫���ˣ��߻�����������Щʱ�䣬���ǲ��еģ������ṩLUA�ӿڹ��߻�ʹ��
	UINT INSTANCE_EXPIRED_TIME;///��ʱʱ����
	UINT INSTANCE_EXPIRED_TEAMTIME;///��ʱ�������ʱ����
	UINT INSTANCE_EXPIRED_LIFETIME;///��ʱ�������ʱ����
	UINT INSTANCE_EXPIRED_RECYCLETIME;///��ʱ�������ʱ����
// 	UINT INSTANCE_COUNTDOWN_TIME;		//����ʱ������Ĭ���趨Ϊ120��
	UINT INSTANCE_USER_MAX_NUMBERS;//��������Ҹ�����������
	UINT INSTANCE_USER_MIN_NUMBERS;//��������Ҹ�����������

    UINT INSTANCE_NEWGENERATE_TIME;//������ˢ��ʱ���

	UINT m_nLeaveSynTime;           //�˰�����ʱ��
	UINT m_nCityWarVictoryTime;           //��սʤ������ʱ
	
	//Ϊ֧�־��Ÿ����ͳ�ս������ӵ�
	int  m_nInstanceScope;			//������ģ
	
	UINT m_SynID;					//���Ÿ���ID

	UINT m_nDomainMapID;			//����ռ�������
	UINT m_nAttackSynID;			//��������ID
	UINT m_nDefenceSynID;			//���ط�����ID

	OBJID m_nAttackBossID;          //��ս����boss
	OBJID m_nDefenceBossID;         //��ս�ط�boss

	UINT  m_nLimitCityWar;          //��ս��������

	BOOL m_CityWarClose;
	BOOL m_CityMsg;

//	vector<CityWarNPCInfo> m_vecCityWarNpc;
    DWORD m_dwKickPlayerInterval;   //ǿ�����˼��
	int  m_nSWABCount;
	int  m_nSWDBCount;
	UINT m_nSWBeKillMark;           //��ս���ﱻɱ���

	//������

	map<string,set<UINT> >			m_RequstUserMap;
	map<string,vector<UINT> >		m_EnterArenaUserMap;
	CTimeOut						m_ArenaBegin;
	bool							m_bArenaEnd;
	UINT							m_TeamType;
	OBJID m_nMonsterTypeEvent;///��������
	UINT  m_nMonsterCountEvent;///��������
	UINT  m_nMonsterEventContext;///�����¼�������
	bool  m_bAllowIn;
	bool  m_bNeedKickout;
};

//typedef CDataObjectSet CInstanceTypeData;
#endif // !defined(AFX_MAPINSTANCE_H__1CB83F41_11BA_4625_B1B8_249123985025__INCLUDED_)
