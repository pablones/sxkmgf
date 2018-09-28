// GameMap.h: interface for the CGameMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEMAP_H__C01C6BBF_DE8E_486F_B369_B6A1E735FF80__INCLUDED_)
#define AFX_GAMEMAP_H__C01C6BBF_DE8E_486F_B369_B6A1E735FF80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "GameBlock.h"
#include "GameObj.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "I_MapData.h"
#include "Myheap.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "EventPack.h"
#include "T_SingleObjSet2.h"
#include <vector>
using namespace std;

///////////////////////////////////////////////////////
// global
inline int	Block(int nPos)				{ return nPos / CELLS_PER_BLOCK; }

//////////////////////////////////////////////////////////////////////
inline int POS2INDEX(int x, int y, int cx, int cy) { return (x*cy + y); }
inline int INDEX2X(int idx, int cx, int cy) { return (idx % cy); }
inline int INDEX2Y(int idx, int cx, int cy) { return (idx / cy); }


///////////////////////////////////////////////////////
enum GAMEMAPDATA{
	GAMEMAPDATA_NAME=1,
	GAMEMAPDATA_MAPDOC,
	GAMEMAPDATA_TYPE,
	GAMEMAPDATA_MAPGROUP,
	GAMEMAPDATA_OWNERTYPE,
	GAMEMAPDATA_OWNERID,
	GAMEMAPDATA_DISABLE,
};
char	szMapTable[];
typedef	CGameData<GAMEMAPDATA,szMapTable,szID>	CGameMapData;


enum ENUM_MAPTYPE {
	MAPTYPE_NORMAL						= 0x00000000,	// 00
	MAPTYPE_PKFIELD						= 0x00000001,	// 01		
	MAPTYPE_CHGMAP_DISABLE				= 0x00000002,	// 02					// magic call team member
	MAPTYPE_RECORD_DISABLE				= 0x00000004,	// 04
	MAPTYPE_PK_DISABLE					= 0x00000008,	// 08
	MAPTYPE_BOOTH_ENABLE				= 0x00000010,	// 16 ���԰�̯
	MAPTYPE_TEAM_DISABLE				= 0x00000020,	// 32
	MAPTYPE_TELEPORT_DISABLE			= 0x00000040,	// 64					// chgmap by action
	MAPTYPE_SYN_MAP						= 0x00000080,	// 128
	MAPTYPE_PRISON_MAP					= 0x00000100,	// 256
	MAPTYPE_WING_DISABLE				= 0x00000200,	// 512					// bowman fly disable
	MAPTYPE_FAMILY						= 0x00000400,	// 1024
	MAPTYPE_MINEFIELD					= 0x00000800,	// 2048
	MAPTYPE_PKGAME						= 0x00001000,	// 4096					// PK��
	MAPTYPE_NEVERWOUND					= 0x00002000,	// 8192					// ������ // Add by Arhun
	MAPTYPE_DEADISLAND					= 0x00004000,	// 16384				// ������ 

	MAPTYPE_PKFIELD_NO_DEATH_PUNHISH	= 0x00008000,		// 32768	20070705 ��� ���� "�����޳ͷ�����������Ʒ�������پ��顣"
	// Add by Arhun // ɱ�˲���PKֵ�����ӷ���״̬��������������������װ�����������ɹ��׶ȣ������ӶԷ��İ�������
	MAPTYPE_PKFIELD_NO_PK_VALUE			= 0x00010000,		// 65536	20070705 ��� ���� Ԥ��	ʹ��ΪPK��,����"ɱ�˲�����PKֵ"
	MAPTYPE_NPCCHGMAP_ONLY				= 0x00020000,		//20070719����:��npc����,����.һ�з�npc���봫����ֹ
	MAPTYPE_NO_RELIVEATWHEREYOUDIE		= 0x00040000,		//20070719����:��ֹԭ�ظ���.
															//ԭ�ظ���;����a������"�����"��itemtypeΪ950401��
															//				b������"��������"��magictypeΪ6426��
	MAPTYPE_NO_RECOVERIMM				= 0x00080000,		//20070719����:��ֹ˲��
															//����ʹ�����е���ֵҩˮ��itemtypeΪ920001��920002��920101��920102��
	MAPTYPE_CONTESTED					= 0x00100000,		//20070719����:�����е�����

	MAPTYPE_NO_MEMORY_CRYSTAL_UNRECORD  = 0X00200000,		//�˵�ͼ�����ü���ˮ��������

	MAPTYPE_NO_IMMEDIATERESUMPTIVE		= 0X00400000,		//�˵�ͼ����˲��ָ�(����ʹ��˲��ָ�����)

	MAPTYPE_RANDMON_REBORN_INMAP		= 0x00800000,		//	�����ڱ���ͼ�������
	
	MAPTYPE_NO_MOUNT                    = 0x01000000,       //�˵�ͼ����������

	MAPTYPE_REBORN_INSTANCE             = 0x02000000,       //�����ڱ���ͼָ��λ�ø���
	MAPTYPE_PVP_ENABLE		            = 0x04000000,       //�������
};
enum ENUM_MAPDISABLE 
{
	MAPDISABLE_LEAVE					= 0x00000001,		//��ͼ���Ƿ��ܹ�ʹ�ûس������
	MAPDISABLE_ReducePk					= 0x00000002,		//��ͼ�Ƿ��ܼ���PKֵ
	MAPDISABLE_Pkill					= 0x00000004,		//��ͼ�Ƿ��ܹ�PK
	MAPDISABLE_FlyShoe					= 0x00000008,		//��ͼ���ܷ�ʹ��С��Ь
	MAPDISABLE_CUT						= 0x00000010,       //��ľ
	MAPDISABLE_MINE						= 0x00000020,       //�ɿ�
	MAPDISABLE_HUNT						= 0x00000040,       //����
	MAPDISABLE_STRUGGLE					= 0x00000080,		//��ͼ���ܾ���
	MAPDISABLE_ISPKGAMEMAP				= 0x00000100,		//���䳡
};	
///////////////////////////////////////////////////////
CONST OBJID MAP_PRISON			= 1027;							//������ͼ//20061231����:4013,ԭ��4003
const int	DYNAMIC_MAP_ID		= 1000000;
const int	SYN_MAP_DOWN		= 9000;
const int	SYN_MAP_UP			= 10000;
///////////////////////////////////////////////////////
enum { RPC_MAP_DELNPC };

///////////////////////////////////////////////////////
struct	NewMapInfo{
		NAMESTR		szName;
		int		nMapDoc;
		int		nType;
		int		nOwnerType;
		OBJID	idOwner;
		int		nMapGroup;
		OBJID	nDisable;
};
struct MapUserAmount{
	OBJID idMap;
	DWORD dwAmount;
};
#include "WeatherRegion.h"
#include "NpcGenerator.h"
///////////////////////////////////////////////////////
const bool	WITH_BLOCK		= true;

typedef	vector<IMapThing*>	MAPTHING_SET_Z;//Z��
typedef	vector<MAPTHING_SET_Z>	MAPTHING_SET;//XY��
//-------------------------------
class CGameMap : public CGameObj
{
protected:
	CGameMap();
	virtual ~CGameMap();
public:
	static CGameMap*	CreateNew()	{ return new CGameMap(); }
	ULONG	ReleaseByOwner()		{ delete this; return 0; }
	IMapData*		QueryMapData()	 { ASSERT(m_pMapData); return m_pMapData; }

public:
	OBJID	GetID() { return m_pData->GetKey(); }
	bool	Create(PROCESS_ID idProcess, IRecordset* pRes);
	void	CreateGenData();
	void	GeneratorMonster();
	void	GeneratorMonsterDel(OBJID idGen);
	OBJID	CreateMonster(OBJID idMap,int x,int y,int type,OBJID idGen);
	OBJID	CreateDynaMap(PROCESS_ID idProcess, const NewMapInfo* pInfo);
	OBJID	CreateSpecilMap(PROCESS_ID idProcess,const NewMapInfo* pInfo);//�����ͼ
	OBJID	CreateSpecilMap(PROCESS_ID idProcess,IRecordset* pRes);//�����ͼ
	void	OnTimer(DWORD tCurr);
	bool	QueryObjInPos(int nPosX, int nPosY, OBJID idObjType, void** ppObj);
	bool	QueryObj(int nCenterX, int nCenterY, OBJID idObjType, OBJID idObj, void** ppObj);
	IRole*	QueryRole(int nCenterX, int nCenterY, OBJID idObj);
	IRole*	QueryRoleByPos(int nPosX, int nPosY);

public: // block
	//20070320����:����ֵ��Ϊָ��,ԭΪ����,���ɷ��ؿ�.����ȫ.
	CGameBlock*	QueryBlock(int nPosX, int nPosY);
	IMapThing*	QueryThingByIndex(int x, int y, int z)	{ return BlockByIndex(x,y).QuerySet()->GetObjByIndex(z); }
	CGameBlock&	BlockByIndex(int x, int y)				{ return m_setBlock[x][y]; }		// call by FOR_9_xxx
// protected:
// 	IRegionSet*	QueryRegionSet()						{ CHECKF(m_setRegion); return m_setRegion; }

public: // block info
	virtual void	SendBlockInfo(IRole* pRole);		// �������Լ�
	virtual void	BroadcastBlockMsg(IMapThing* pThing, CNetMsg* pMsg, bool bSendSelf = false);
	virtual void	BroadcastBlockMsg(int nPosX, int nPosY, CNetMsg* pMsg);

public: // role
	void	EnterRoom(IMapThing* pThing, BOOL bWithBlock=false);
	void	LeaveRoom(IMapThing* pThing, BOOL bWithBlock=false);
	void	MoveTo(IRole* pRole, int nNewPosX, int nNewPosY, BOOL bLeaveBlock=false, BOOL bEnterBlock=false);		// �п����ƶ������ڵ�BLOCK
	void	IncRole(int x, int y)		{ m_pMapData->IncRole(x, y); }
	void	DecRole(int x, int y)		{ m_pMapData->DecRole(x, y); }		// normal use LeaveRoom or MoveTo but dead
	
	bool	ChangeUserMapData(CUser* pUser, int nNewPosX, int nNewPosY);

public: // get
	int		GetOwnerID()					{ return m_pData->GetInt(GAMEMAPDATA_OWNERID); }
	int		GetOwnerType()					{ return m_pData->GetInt(GAMEMAPDATA_OWNERTYPE); }
	int		GetWidth()						{ return m_pMapData->GetMapWidth(); }
	int		GetHeight()						{ return m_pMapData->GetMapHeight(); }
	int		GetWidthOfBlock()				{ return (m_pMapData->GetMapWidth()-1) / CELLS_PER_BLOCK + 1; }
	int		GetHeightOfBlock()				{ return (m_pMapData->GetMapHeight()-1) / CELLS_PER_BLOCK + 1; }
	OBJID	GetSynID()						{ if(GetOwnerType() == OWNER_SYN) return GetOwnerID(); return ID_NONE; }
	DWORD	GetType()						{ return m_pData->GetInt(GAMEMAPDATA_TYPE); }
	OBJID	GetDocID()						{ return m_pData->GetInt(GAMEMAPDATA_MAPDOC); }
	const char* GetName()					{ return m_pData->GetStr(GAMEMAPDATA_NAME); }
	DWORD	GetUserAmount()					{ return m_nUserAmount; }
public: // const
	bool	IsNotAllowNewRemedy(){return (GetType()&MAPTYPE_NO_RECOVERIMM)!=0;}///�Ƿ��ֹ����.
	bool	IsNotAllowReborn(){return (GetType()&MAPTYPE_NO_RELIVEATWHEREYOUDIE)!=0;}///�Ƿ��ֹ����.
	bool	IsStandEnable(int nPosX, int nPosY);
	bool	IsMoveEnable(int x, int y);
	bool	IsMoveEnable(int xSour, int ySour, int x, int y, int nClimbCap);
	int		Distance(int x1, int y1, int x2, int y2)	{ return __max(abs(x1-x2), abs(y1-y2)); }
	bool	IsValidPoint(POINT pos)			{ return IsValidPoint(pos.x, pos.y); }
	bool	IsValidPoint(int x, int y)		{ return (x >=0 && x < GetWidth() && y >= 0 && y < GetHeight()); }
	bool	IsPkField(void)					{ return ((m_pData->GetInt(GAMEMAPDATA_TYPE)&MAPTYPE_PKFIELD) != 0); }
	bool	IsPVPEnable(void)				{ return ((m_pData->GetInt(GAMEMAPDATA_TYPE)&MAPTYPE_PVP_ENABLE) != 0); }
	bool	IsPkFieldNoPkValue(void)		{ return ((m_pData->GetInt(GAMEMAPDATA_TYPE)&MAPTYPE_PKFIELD_NO_PK_VALUE) != 0); }
	bool	IsPkFieldNoDeathPunish(void)		{ return ((m_pData->GetInt(GAMEMAPDATA_TYPE)&MAPTYPE_PKFIELD_NO_DEATH_PUNHISH) != 0); }
	bool	IsRandomRebornInMap(void)		{return ((m_pData->GetInt(GAMEMAPDATA_TYPE) & MAPTYPE_RANDMON_REBORN_INMAP) != 0); }
	bool	IsRebornInstance(void)	    	{return ((m_pData->GetInt(GAMEMAPDATA_TYPE) & MAPTYPE_REBORN_INSTANCE) != 0); }
    bool    IsNoMount(void)                 {return ((m_pData->GetInt(GAMEMAPDATA_TYPE) & MAPTYPE_NO_MOUNT) != 0); }
	bool	IsFlyToDisable(void)			{ return ((m_pData->GetInt(GAMEMAPDATA_TYPE)&MAPTYPE_CHGMAP_DISABLE) != 0); }
	bool	IsRecordDisable(void)			{ return (m_pData->GetInt(GAMEMAPDATA_TYPE) == 1); }
	bool	IsPkDisable(void)				{ return ((m_pData->GetInt(GAMEMAPDATA_TYPE)&MAPTYPE_PK_DISABLE) != 0); }
	bool	IsTeamDisable()					{ return (m_pData->GetInt(GAMEMAPDATA_TYPE)&MAPTYPE_TEAM_DISABLE) != 0; }
	bool	IsTeleportDisable()				{ return (m_pData->GetInt(GAMEMAPDATA_TYPE)&MAPTYPE_TELEPORT_DISABLE) != 0; }
	//bool	IsSynMap()						{ return (m_pData->GetInt(GAMEMAPDATA_TYPE)&MAPTYPE_SYN_MAP) != 0; }
	bool	IsPrisonMap()					{ return (m_pData->GetInt(GAMEMAPDATA_TYPE)&MAPTYPE_PRISON_MAP) != 0; }
	bool	IsDynaMap()						{ return GetID() > DYNAMIC_MAP_ID; }
	bool	IsSynMap()						{ return(GetID() >= SYN_MAP_DOWN && GetID() < SYN_MAP_UP);}
	bool	IsWingEnable()					{ return (m_pData->GetInt(GAMEMAPDATA_TYPE)&MAPTYPE_WING_DISABLE) == 0; }
	bool	IsMineField()					{ return (m_pData->GetInt(GAMEMAPDATA_TYPE)&MAPTYPE_MINEFIELD) != 0; }
	bool	IsFamilyMap()					{ return (m_pData->GetInt(GAMEMAPDATA_TYPE)&MAPTYPE_FAMILY) != 0; }
	bool	IsNeverWoundMap()				{ return (m_pData->GetInt(GAMEMAPDATA_TYPE)&MAPTYPE_NEVERWOUND) != 0;}
	bool	IsDeadIsland()					{ return (m_pData->GetInt(GAMEMAPDATA_TYPE)&MAPTYPE_DEADISLAND) != 0;}
	bool	IsBoothEnable()					{ return (m_pData->GetInt(GAMEMAPDATA_TYPE)&MAPTYPE_BOOTH_ENABLE) != 0;}
	bool	isDisableLeave()				{ return (m_pData->GetInt(GAMEMAPDATA_DISABLE)&MAPDISABLE_LEAVE) !=0;}
	bool	isDisableReducePk()				{ return (m_pData->GetInt(GAMEMAPDATA_DISABLE)&MAPDISABLE_ReducePk) !=0;}
	bool	isDisablePkill()				{ return (m_pData->GetInt(GAMEMAPDATA_DISABLE)&MAPDISABLE_Pkill) !=0;}
	bool	isDisableFlyShoe()				{ return (m_pData->GetInt(GAMEMAPDATA_DISABLE)&MAPDISABLE_FlyShoe) !=0;}
	bool	isCutMap()						{ return (m_pData->GetInt(GAMEMAPDATA_DISABLE)&MAPDISABLE_CUT) !=0;}
	bool	isMineMap()						{ return (m_pData->GetInt(GAMEMAPDATA_DISABLE)&MAPDISABLE_MINE) !=0;}
	bool	isHuntMap()						{ return (m_pData->GetInt(GAMEMAPDATA_DISABLE)&MAPDISABLE_HUNT) !=0;}
	bool	isDisableStruggle()				{ return (m_pData->GetInt(GAMEMAPDATA_DISABLE)&MAPDISABLE_STRUGGLE) !=0;}
	bool	IsPkGameMap()					{ return (m_pData->GetInt(GAMEMAPDATA_DISABLE)&MAPDISABLE_ISPKGAMEMAP) !=0;}
public: // application
	void	CollectMapThing(MAPTHING_SET& psetMapThing, const POINT pos, int nRange, OBJID idObjTypeUnion);	// idObjTypeUnion ֧�ֶ����Ͷ���
	bool	FindDropItemCell(int nRange, POINT* pos,MAP_OJBTYPE motType=OBJ_MAPITEM);			// pos: in/out
	DWORD	GetPassageAction(const POINT& pos);
	bool	GetRebornMap(OBJID* pidMap, POINT* pposTarget);
	bool	ChangeMapDoc(OBJID idDoc);						// false: user in map, can't change
	bool	EraseMap();
	bool	EraseInstanceMap();
	bool	CheckBooth( int nPosX,int nPosY,OBJID idObjType,void** ppObj );//  [5/29/2007]
	void	AddUserAmount(int nAmount)		{ m_nUserAmount += nAmount; }
public: // debug
	bool	GetDebugData(int* pAttr, int* pMask, int* pAlt, int* pAlt2, int* pCount, int x, int y)
			{ if(!IsValidPoint(x,y)) return false;
				*pMask=m_pMapData->GetFloorMask(x,y); 
				return true; }

protected: // block
	typedef	vector<CGameBlock>	BLOCK_SET;
	typedef	vector<BLOCK_SET>	BLOCKS_SET;
	BLOCKS_SET		m_setBlock;

protected: // data
	CGameMapData*	m_pData;
	IMapData*		m_pMapData;

protected: // ctrl
	PROCESS_ID		m_idProcess;
	MYHEAP_DECLARATION(s_heap)

public:
	bool GetRandomPos(int &nX,int &nY);
	BOOL IsMarketRegion(int x,int y);//20070123����:�г�������ʵ��
protected: // application
	int				m_nUserAmount;

protected:
	INpcGenSet* m_pGenData;
};

#define	FOR_9_BLOCKS(P,X,Y)							\
	for(x = __max(Block(X)-1,0); x <= Block(X)+1 && x < P->GetWidthOfBlock(); x++)		\
		for(y = __max(Block(Y)-1,0); y <= Block(Y)+1 && y < P->GetHeightOfBlock(); y++)
#define FOR_9_BLOCKTHINGS(P,X,Y)	\
	FOR_9_BLOCKS((P),(X),(Y))		\
		for(z = 0; z < P->BlockByIndex(x,y).QuerySet()->GetAmount(); z++)
// ��ע��ʹ��FOR_9_BLOCKS��FOR_9_BLOCKTHINGSʱҪע��elseƥ�������break�Ĳ������
#endif // !defined(AFX_GAMEMAP_H__C01C6BBF_DE8E_486F_B369_B6A1E735FF80__INCLUDED_)
