// UserManager.h: interface for the CMapGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPGROUP_H__70A5EB1D_66D5_4DF0_BE98_F7E3498FFC72__INCLUDED_)
#define AFX_MAPGROUP_H__70A5EB1D_66D5_4DF0_BE98_F7E3498FFC72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma	warning(disable:4786)
#include "../share/define.h"
#include "RoleManager.h"
#include "UserManager.h"
#include "MapManager.h"
#include "NpcManager.h"
#include "SynManager.h"
#include "LeaveWord.h"
#include "autoptr.h"
#include "GameAction.h"
#include "EventPack.h"
//modify code by 林德渊 begin 2011.5.19
#include "statistical.h"
//modify code by 林德渊 end
#include "DropRuleMap.h"	//add by zlong 2003-11-15
#include "MineRuleMap.h"
#include "Announce.h"
#include "MarryGod.h"///softworms
#include "ChannelMsgManager.h"
#include "Tutor.h"
#include "LuaScriptMachine.h"
#include "MapInstance.h"
#include "VariantManager.h"
#include "PKWorldCupManager.h"
#include "TaskSetRand.h"
//#include "HouseManager.h"
#include "WeekActive.h"
#include "UserMaterialItem.h"
#include "PvpActive.h"
#include "InstanceUseTimeMgr.h"
#include "MapClear.h"
#include "PVPManager.h"
#include "LoverHomeManager.h"
#include "Suit.h"
#include "MapArena.h"
#include "ShopVAS.h"
#include "TitleTypeData.h"
#include "TreasureType.h"
#include "UserJinMeiTypeData.h"
#include "JinmeiStrengthData.h"
#include "UserJinMeiSysMgr.h"
#include "PortableShopManager.h"
#define SPECIALITEMBASE 1000000
#include <vector>
using namespace std;
//class CLuaScriptMachine;
class CWargameManager;
class CMapInstanceMgr;
typedef vector<CTutor*>		TUTOR_SET;
//////////////////////////////////////////////////////////////////////
class IIntraMsg
{
public:
//	virtual ~IIntraMsg() {}
	virtual bool	SendClientMsg		(SOCKET_ID idSocket, CNetMsg* pMsg)					PURE_VIRTUAL_FUNCTION_0
//	virtual bool	SendNpcMsg			(OBJID idNpc, CNetMsg* pMsg)						PURE_VIRTUAL_FUNCTION_0
	virtual bool	SendBroadcastMsg	(CNetMsg* pNetMsg)									PURE_VIRTUAL_FUNCTION_0
	virtual bool	TransmitMsg			(CNetMsg* pNetMsg, SOCKET_ID idSocket, OBJID idNpc)	PURE_VIRTUAL_FUNCTION_0
	virtual bool	TransmitWorldMsg	(CNetMsg* pNetMsg)									PURE_VIRTUAL_FUNCTION_0
	virtual bool	TransmitWorldMsg	(CNetMsg* pNetMsg, LPCTSTR szName)					PURE_VIRTUAL_FUNCTION_0
	virtual bool	TransmitWorldMsg	(CNetMsg* pNetMsg, OBJID idUser)					PURE_VIRTUAL_FUNCTION_0
	virtual bool	TransmitWorldMsg	(SOCKET_ID idSocket,CNetMsg* pMsg)					PURE_VIRTUAL_FUNCTION_0
	virtual bool	TransmitWorldMsg2	(CNetMsg* pMsg, OBJID idUser)						PURE_VIRTUAL_FUNCTION_0
//	virtual bool	SetNpcProcessID		(OBJID idNpc, bool bAddNew)							PURE_VIRTUAL_FUNCTION_0
	virtual bool	SendSocketUserInfo	(PROCESS_ID idProcess, SOCKET_ID idSocket, UserInfoStruct* pInfo, ULONG nIP)		PURE_VIRTUAL_FUNCTION_0
	virtual bool    SendNewPlayerState(PROCESS_ID idProcess, OBJID userID, char* data, int size)		PURE_VIRTUAL_FUNCTION_0
	virtual bool	SendPresentState(PROCESS_ID idProcess, OBJID userID, int* data, int size)		PURE_VIRTUAL_FUNCTION_0
//	virtual bool    SendTutorState(PROCESS_ID idProcess, OBJID userID, TUTOR_SET	m_setStudent, CTutor*		m_pTutor,DWORD		m_dwExpToTutor)		PURE_VIRTUAL_FUNCTION_0
	virtual bool    SendTutorToWorld(const ChgTutorInfo * pInfo)    PURE_VIRTUAL_FUNCTION_0   //发送师徒信息到世界线程
	virtual bool	TransmitMapGroupMsg(PROCESS_ID idProcess,CNetMsg* pMsg) PURE_VIRTUAL_FUNCTION_0				// 广播给指定地图组处理
	virtual bool	SendObjInfo			(PROCESS_ID idProcess, OBJID idUser, INFO_ID nInfoMsgID, void* pInfo, int nSize)		PURE_VIRTUAL_FUNCTION_0
	//20070329修罗:加一个参数.区分[请求]-[响应处理]
	//virtual bool	ChangeMapGroup		(PROCESS_ID idProcess, OBJID idUser, OBJID idMap, int nPosX, int nPosY)		PURE_VIRTUAL_FUNCTION_0
	virtual bool	ChangeMapGroup		(PROCESS_ID idProcess, 
		OBJID idUser, 
		OBJID idMap, 
		int nPosX, 
		int nPosY,
		Enum_ChangeMapGroupMode mode = ENUM_CNGMAPGROUP_MODE_PROC,
		void *lpBooth = NULL)		PURE_VIRTUAL_FUNCTION_0
	virtual bool	PrintText			(LPCTSTR szText)									PURE_VIRTUAL_FUNCTION_0
	virtual bool	CloseSocket			(SOCKET_ID idSocket)								PURE_VIRTUAL_FUNCTION_0
	virtual bool	CloseMapGroup		(SOCKET_ID idGmSocket)								PURE_VIRTUAL_FUNCTION_0
	//20070202修罗:加一个参数.小队名称
	//virtual bool	ChangeTeam			(int nAction, OBJID idTeam, OBJID idUser, int nData=0)			PURE_VIRTUAL_FUNCTION_0
	virtual bool ChangeTeam (int nAction, 
		OBJID idTeam, 
		OBJID idUser, 
		int nData1=0,
		int nData2=0,
		int nData3=0,
		int nData4=0,
		char *teamName = NULL)		PURE_VIRTUAL_FUNCTION_0
	virtual void	RemoteCall			(REMOTE_CALL0* pInfo,bool bSendToLocal=false)								PURE_VIRTUAL_FUNCTION
	virtual void	LevelUp				(OBJID idUser, int nLevel)							PURE_VIRTUAL_FUNCTION
	virtual void	QueryFee			(OBJID idAccount)									PURE_VIRTUAL_FUNCTION
	virtual void	ModifyNpc			(OBJID idNpc, LPCTSTR szField, LPCTSTR szData)		PURE_VIRTUAL_FUNCTION
	virtual void	DelTransNpc			(OBJID idMaster, bool bBroadcast=true)				PURE_VIRTUAL_FUNCTION
	virtual void	ChangeCode			(SOCKET_ID idSocket, DWORD dwCode)					PURE_VIRTUAL_FUNCTION

	//20070122修罗:开始更新角色卡
	virtual BOOL	NowUpdateCalculatedProperty(PROCESS_ID idProcess, OBJID userID)											PURE_VIRTUAL_FUNCTION_0

	//  [12/6/2007 朱斌] 更新天赋
	virtual BOOL	NowUpdateGift(PROCESS_ID idProcess, OBJID userID)											PURE_VIRTUAL_FUNCTION_0
	virtual BOOL    UpdateSaveItemViewInfo(PROCESS_ID idProcess,OBJID userID) PURE_VIRTUAL_FUNCTION_0
};

//////////////////////////////////////////////////////////////////////
class IDataDefault
{
public:
	virtual IRecordset*		GetMagicData()												PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetUserWeaponSkill()										PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetGameItemData()											PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetUserData()												PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetGameMapData()											PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetSynData()												PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetSynAttrData()											PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetPetData()												PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetDynaNpc()												PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetDynaMap()												PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetSynMap()												PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetEudemonData()											PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetPeifangData()											PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetUserGiftData()											PURE_VIRTUAL_FUNCTION_0//20071123修罗:
	virtual IRecordset*		GetTaskDetailData()											PURE_VIRTUAL_FUNCTION_0//06.11.21王玉波添加
	virtual IRecordset*		GetWeaponSoulData()											PURE_VIRTUAL_FUNCTION_0//20071113修罗:
	virtual IRecordset*		GetHouseData()										    	PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetCityWarData()											PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetInstanceUseData()										PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetTitleData()												PURE_VIRTUAL_FUNCTION_0//goto
	virtual IRecordset*		GetTreasureData()											PURE_VIRTUAL_FUNCTION_0//goto
	virtual IRecordset*     GetJinmeiData()                                             PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetOnlineGiftData()											PURE_VIRTUAL_FUNCTION_0//goto
	virtual IRecordset*		GetTutorData()												PURE_VIRTUAL_FUNCTION_0//goto
	virtual IRecordset*		GetPeteggData()												PURE_VIRTUAL_FUNCTION_0//goto
	virtual IRecordset*		GetKeyData()												PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetAutoFightData()											PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetWarGameData()											PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetSpecilLineData()											PURE_VIRTUAL_FUNCTION_0
	virtual IRecordset*		GetCPSMData()												PURE_VIRTUAL_FUNCTION_0
//modify code by 林德渊 begin 2011.5.30-
	virtual IRecordset*		GetVaslvData()												PURE_VIRTUAL_FUNCTION_0
//modify code by 林德渊 end
};

class ISynMessage
{
public:
	virtual bool	CreateSyndicate		(const CreateSyndicateInfo* pInfo)				PURE_VIRTUAL_FUNCTION_0
	virtual bool	DestroySyndicate	(OBJID idSyn, OBJID idTarget = ID_NONE)			PURE_VIRTUAL_FUNCTION_0
	virtual bool	ChangeSyndicate		(const SynFuncInfo0* pFuncInfo)					PURE_VIRTUAL_FUNCTION_0
	virtual bool	StopSayByGM			(char* szname, int sec)							PURE_VIRTUAL_FUNCTION_0
};

//////////////////////////////////////////////////////////////////////
class ISocket;
class IDatabase;
class IMessagePort;
//modify code by 林德渊 begin
#define LIN_CIRCLE_NUM  30
//modify code by 林德渊 end
class CSKPostOffice;///softworms
class CMapGroup : IIntraMsg, IDataDefault, ISynMessage
{
public:
	CMapGroup();
	virtual ~CMapGroup() {}
	PROCESS_ID		GetID() { return m_idProcess; }
	int		GetMapGroup()	{ return m_idProcess-MSGPORT_MAPGROUP_FIRST; }

public: // create
	bool	Create(PROCESS_ID idProcess, ISocket* pSocket, IDatabase* pDb, IMessagePort* pMsgPort);
	void	Destroy();
	void	OnTimer(time_t tCurr);
	IUserManager*	CreateNewUserManager();
	IMapManager*	CreateNewMapManager();
	INpcManager*	CreateNewNpcManager();

private:// action
	CTimeOut	m_tmEvent;

	void	ProcessEvent	(void);

public: // interface
	IRoleManager*	GetRoleManager()	{ ASSERT(m_pRoleManager); return m_pRoleManager; }
	IUserManager*	GetUserManager()	{ ASSERT(m_pUserManager); return m_pUserManager; }
	IMapManager*	GetMapManager()		{ ASSERT(m_pMapManager); return m_pMapManager; }
	INpcManager*	GetNpcManager()		{ ASSERT(m_pNpcManager); return m_pNpcManager; }	
	IDatabase*		GetDatabase()		{ ASSERT(m_pDb); return m_pDb; }
	CSynManager*	GetSynManager()		{ ASSERT(m_pSynManager); return m_pSynManager; }
	CPVPManager*	GetPVPManger()		{ ASSERT(m_PVPManager); return m_PVPManager; 	}
	IIntraMsg*		QueryIntraMsg()		{ return (IIntraMsg*)this; }
	IDataDefault*	QueryDataDefault()	{ return (IDataDefault*)this; }
	ISynMessage*	QuerySynMessage()	{ return (ISynMessage*)this; }
	CLeaveWord*		GetLeaveWord()		{ ASSERT(m_pLeaveWord); return m_pLeaveWord; }
	CGameAction*	GetGameAction()		{ ASSERT(m_pGameAction); return m_pGameAction; }
	IMessagePort*	QueryMsgPort()		{ CHECKF(m_pMsgPort); return m_pMsgPort; }
	CMapInstanceMgr* GetMapInstanceMgr(){CHECKF(m_ptrMapInstanceMgr);return m_ptrMapInstanceMgr;}
	CLuaScriptMachine* GetLuaScriptMachine(){CHECKF(m_ptrLuaScriptMachine);return m_ptrLuaScriptMachine;}
	CVariantManager* GetVariantManager(){CHECKF(m_ptrVariantManager);return m_ptrVariantManager;}
	CInstanceUseTimeMgr*	GetInstanceUseMgr()		{ ASSERT(m_pInstanceUseMgr); return m_pInstanceUseMgr; }
	CUserJinMeiSysMgr*    GetUserJinmeiSysMgr()      { ASSERT(m_pUserJinmeiSysMgr); return m_pUserJinmeiSysMgr; }
	bool	TransmitMsg					( CNetMsg* pMsg,int nPhyle );	//种族聊天信息,王玉波添加..
	bool	TransmitMsgToWorld					( CNetMsg* pMsg);//向世界发
	CWargameManager* GetWargameManager(){CHECKF(m_ptrWargameManager);return m_ptrWargameManager;}
public:
	vector<OBJID>&  GetNoItemIDVec() { return m_NoitemIdVec;}
public: // const
	int				GetMapGroupAmount();

protected: // IIntraMsg

	virtual bool	SendClientMsg		(SOCKET_ID idSocket, CNetMsg* pMsg);			// to network
//	virtual bool	SendNpcMsg			(OBJID idNpc, CNetMsg* pMsg);					// to network
	virtual bool	SendBroadcastMsg	(CNetMsg* pNetMsg);
	virtual bool	TransmitMsg			(CNetMsg* pNetMsg, SOCKET_ID idSocket, OBJID idNpc);		// 广播给其它地图组核心处理
	virtual bool	TransmitWorldMsg	(CNetMsg* pNetMsg);									// 广播给世界核心处理
	virtual bool	TransmitWorldMsg	(CNetMsg* pNetMsg, LPCTSTR szName);					// 通过世界核心转发给玩家
	virtual bool	TransmitWorldMsg	(CNetMsg* pNetMsg, OBJID idUser);					// 通过世界核心转发给玩家
	virtual bool	TransmitWorldMsg	(SOCKET_ID idSocket,CNetMsg* pMsg);
	virtual bool	TransmitMapGroupMsg(PROCESS_ID idProcess,CNetMsg* pMsg);				// 广播给指定地图组处理
	virtual bool	TransmitWorldMsg2(CNetMsg* pMsg, OBJID idUser);///转发到关系服务器
	virtual bool	SendSocketUserInfo	(PROCESS_ID idProcess, SOCKET_ID idSocket, UserInfoStruct* pInfo, ULONG nIP);
	virtual bool	SendObjInfo			(PROCESS_ID idProcess, OBJID idUser, INFO_ID nInfoMsgID, void* pInfo, int nSize);
//	virtual bool	SetNpcProcessID		(OBJID idNpc, bool bAddNew);
	virtual bool    SendTutorToWorld(const ChgTutorInfo * pInfo);    //发送师徒信息到世界线程
 
	//20070329修罗:加一个参数.区分[请求]-[响应处理]
	//virtual bool	ChangeMapGroup		(PROCESS_ID idProcess, OBJID idUser, OBJID idMap, int nPosX, int nPosY);
	virtual bool	ChangeMapGroup		(PROCESS_ID idProcess, 
		OBJID idUser, 
		OBJID idMap, 
		int nPosX, 
		int nPosY,
		Enum_ChangeMapGroupMode mode = ENUM_CNGMAPGROUP_MODE_PROC,//默认为[响应处理]
		void *lpBooth = NULL);//[游途道标 2009.05.12]摊位

	virtual bool	PrintText			(LPCTSTR szText);
	virtual bool	CloseSocket			(SOCKET_ID idSocket)			{ return m_pSocket->CloseSocket(idSocket); }
	virtual bool	CloseMapGroup		(SOCKET_ID idGmSocket);		// 关闭所有socket(除GM)，禁止登录
	//20070202修罗:增加小队名称参数
	//virtual bool	ChangeTeam			(int nAction, OBJID idTeam, OBJID idUser, int nData);			// 组队相关
	virtual bool ChangeTeam (int nAction, 
		OBJID idTeam, 
		OBJID idUser, 
		int nData1=0,
		int nData2=0,
		int nData3=0,
		int nData4=0,
		char *teamName = NULL);
	virtual void	RemoteCall			(REMOTE_CALL0* pInfo,bool bSendToLocal=false);
	virtual void	LevelUp				(OBJID idUser, int nLevel);
	virtual void	QueryFee			(OBJID idAccount);
	virtual void	ModifyNpc			(OBJID idNpc, LPCTSTR szField, LPCTSTR szData);
	virtual void	DelTransNpc			(OBJID idMaster, bool bBroadcast=true);
	virtual void	ChangeCode			(SOCKET_ID idSocket, DWORD dwCode);

	//20070122修罗:开始更新角色卡
	virtual BOOL	NowUpdateCalculatedProperty(PROCESS_ID idProcess, OBJID userID);
protected:
	PROCESS_ID		m_idProcess;
	ISocket*		m_pSocket;
	IDatabase*		m_pDb;
	IMessagePort*	m_pMsgPort;

	IRoleManager*	m_pRoleManager;
	IUserManager*	m_pUserManager;
	IMapManager*	m_pMapManager;
	INpcManager*	m_pNpcManager;
	CPVPManager*   	m_PVPManager; //决斗管理
	CSynManager*	m_pSynManager;
	CLeaveWord*		m_pLeaveWord;
	CGameAction*	m_pGameAction;
	CMapInstanceMgr* m_ptrMapInstanceMgr;///副本管理器,只有在线程5才有效
	CLuaScriptMachine* m_ptrLuaScriptMachine;///脚本机
	CVariantManager	*m_ptrVariantManager;///临时变量管理器
	CInstanceUseTimeMgr*   m_pInstanceUseMgr;   //副本使用时间管理
    CUserJinMeiSysMgr*   m_pUserJinmeiSysMgr;
	CWargameManager* m_ptrWargameManager;

	char			 m_circle[256];
	int				 m_circleTime;
	CTimeOut		 m_circletime;
	bool			 m_circleFlag;
//public:
//	CChannelMsgManager* m_ptrChannelMsgManager;
protected: // GameDataDefault
	IRecordset*		GetMagicData()			{ ASSERT(m_pMagicDataDefault); return m_pMagicDataDefault; }
	IRecordset*		GetUserWeaponSkill()	{ ASSERT(m_pUserWeaponSkillDefault); return m_pUserWeaponSkillDefault; }
	IRecordset*		GetGameItemData()		{ ASSERT(m_pGameItemDataDefault); return m_pGameItemDataDefault; }
	IRecordset*		GetUserData()			{ ASSERT(m_pUserDataDefault); return m_pUserDataDefault; }
	IRecordset*		GetGameMapData()		{ ASSERT(m_pGameMapDataDefault); return m_pGameMapDataDefault; }
	IRecordset*		GetSynData()			{ ASSERT(m_pSynDataDefault); return m_pSynDataDefault; }
	IRecordset*		GetSynAttrData()		{ ASSERT(m_pSynAttrDataDefault); return m_pSynAttrDataDefault; }
	IRecordset*		GetPetData()			{ ASSERT(m_pPetDataDefault); return m_pPetDataDefault; }
	IRecordset*		GetDynaNpc()			{ ASSERT(m_pDynaNpcDataDefault); return m_pDynaNpcDataDefault; }
	IRecordset*		GetDynaMap()			{ ASSERT(m_pDynaMapDataDefault); return m_pDynaMapDataDefault; }
	IRecordset*		GetSynMap()				{ ASSERT(m_pSynMapDataDefault); return m_pSynMapDataDefault; }
	IRecordset*		GetEudemonData()		{ ASSERT(m_pEudemonDataDefault); return m_pEudemonDataDefault; }
	IRecordset*		GetCPSMData()			{ ASSERT(m_pCPSationDefault); return m_pCPSationDefault; }	
	IRecordset*		GetPeifangData()		{ ASSERT(m_pPeifangDataDefault); return m_pPeifangDataDefault; }
	IRecordset*		GetUserGiftData()		{ ASSERT( m_pUserGiftDefault );return m_pUserGiftDefault;}//20071123修罗:天赋
	IRecordset*		GetTaskDetailData()		{ ASSERT( m_pTaskDetailDataDefault );return m_pTaskDetailDataDefault;}//06.11.21王玉波添加
	IRecordset*		GetWeaponSoulData()		{ ASSERT( m_pWeaponSoulDataDefault );return m_pWeaponSoulDataDefault;}//20071113修罗:
	IRecordset*		GetHouseData()		    { ASSERT( m_pHouseDataDefault );return m_pHouseDataDefault;}
	IRecordset*		GetCityWarData()		{ ASSERT( m_pCityWarDataDefault );return m_pCityWarDataDefault;}
	IRecordset*		GetInstanceUseData()	{ ASSERT( m_pInstanceUseDataDefault );return m_pInstanceUseDataDefault;}
	IRecordset*		GetTitleData()			{ ASSERT(m_pTitleDataDefault); return m_pTitleDataDefault; }
	IRecordset*		GetTreasureData()		{ ASSERT(m_pTreasureDataDefault); return m_pTreasureDataDefault; }
	IRecordset*     GetJinmeiData()         { ASSERT(m_pJinmeiDataDefault); return m_pJinmeiDataDefault; }
	IRecordset*		GetOnlineGiftData()		{ ASSERT(m_pOnlineGiftDefault); return m_pOnlineGiftDefault; }
	IRecordset*		GetTutorData()			{ ASSERT(m_pTutorDataDefault); return m_pTutorDataDefault; }
	IRecordset*		GetPeteggData()			{ ASSERT(m_pPeteggDataDefault); return m_pPeteggDataDefault; }
	IRecordset*		GetKeyData()			{ ASSERT(m_pUserKeyDataDefault); return m_pUserKeyDataDefault; }
	IRecordset*		GetAutoFightData()		{ ASSERT(m_pAutoFightDataDefault); return m_pAutoFightDataDefault; }
	IRecordset*		GetWarGameData()		{ ASSERT(m_pWarGameDataDefault); return m_pWarGameDataDefault; }
	IRecordset*		GetSpecilLineData()		{ ASSERT(m_pSpecilLineDataDefault); return m_pSpecilLineDataDefault; }
//modify code by 林德渊 begin 2011.5.30-
	IRecordset*		GetVaslvData()			{ ASSERT(m_pVaslvDataDefault); return m_pVaslvDataDefault; }
//modify code by 林德渊 end

protected:
	IRecordset*			m_pMagicDataDefault;
	IRecordset*			m_pUserWeaponSkillDefault;
	IRecordset*			m_pGameItemDataDefault;
	IRecordset*			m_pUserDataDefault;
	IRecordset*			m_pGameMapDataDefault;
	IRecordset*			m_pSynDataDefault;
	IRecordset*			m_pSynAttrDataDefault;
	IRecordset*			m_pPetDataDefault;
	IRecordset*			m_pDynaNpcDataDefault;
	IRecordset*			m_pDynaMapDataDefault;
	IRecordset*			m_pSynMapDataDefault;
	IRecordset*			m_pEudemonDataDefault;
	IRecordset*			m_pCPSationDefault;
	IRecordset*			m_pPeifangDataDefault;
	IRecordset*			m_pUserGiftDefault;//20071123修罗:
	IRecordset*			m_pTaskDetailDataDefault;				//06.11.21王玉波添加
	IRecordset*			m_pWeaponSoulDataDefault;//20071113修罗:
	IRecordset*			m_pHouseDataDefault;
	IRecordset*			m_pCityWarDataDefault;
	IRecordset*			m_pInstanceUseDataDefault;
	IRecordset*			m_pTitleDataDefault;
	IRecordset*			m_pTreasureDataDefault;
	IRecordset*         m_pJinmeiDataDefault;
	IRecordset*			m_pOnlineGiftDefault;
	IRecordset*			m_pTutorDataDefault;
	IRecordset*			m_pPeteggDataDefault;
	IRecordset*			m_pUserKeyDataDefault;
	IRecordset*			m_pAutoFightDataDefault;
	IRecordset*			m_pWarGameDataDefault;
	IRecordset*			m_pSpecilLineDataDefault;
//modify code by 林德渊 begin 2011.5.30-
	IRecordset*			m_pVaslvDataDefault;
//modify code by 林德渊 end
protected: // Syndicate
	virtual bool	CreateSyndicate		(const CreateSyndicateInfo* pInfo);
	virtual bool	DestroySyndicate	(OBJID idSyn, OBJID idTarget = ID_NONE);
	virtual bool	ChangeSyndicate		(const SynFuncInfo0* pFuncInfo);
	virtual bool	StopSayByGM			(char* szname, int sec);

public: // static
	static void	AddMapGroup(PROCESS_ID idProcess) 
			{ if(idProcess>=m_setMapGroup.size())m_setMapGroup.resize(idProcess+1); }
	static int	IsValidMapGroupProcessID(PROCESS_ID idProcess)
			{ return idProcess >= MSGPORT_MAPGROUP_FIRST && idProcess <= MSGPORT_MAPGROUP_INSTANCE; }

	static BOOL	CheckTime(const DWORD dwType, const char* pszParam);

	LPCSTR GetServerName()
	{
		return m_szServerName;
	}

private: // static
	typedef	vector<CMapGroup>	MAPGROUP_SET;
	static MAPGROUP_SET		m_setMapGroup;
	friend CMapGroup*	MapGroup(PROCESS_ID idProcess);		// 可存取以上静态对象

	char m_szServerName[16];
public:

//////////////////////////////////////////////////////////////////////////
//处理发送给npc服务器的玩家消息 zeng
public:
// 	void AddToNpcBigSet(OBJID nUserID)
// 	{
// 		m_NpcBigUserSet[nUserID]++;
// 	}
// 	bool RemoveFromNpcBigSet(OBJID nUserID,bool bClrBigBc = false)
// 	{
// 		map<OBJID,UINT>::iterator it = m_NpcBigUserSet.find(nUserID);
// 		if(it!=m_NpcBigUserSet.end())
// 		{
// 			int nData = 0;
// 			(it->second)--;
// 			nData = it->second;
// 
// 			if((it->second)==0 || bClrBigBc)
// 			{
// 				//引用计数为0，删除该玩家记录
// 				m_NpcBigUserSet.erase(it);
// 				nData = 0;
// 				return true;
// 			}
// 		}
// 		return false;
// 	}
// 	bool FindNpcBigSet(OBJID nUserID)
// 	{
// 		map<OBJID,UINT>::iterator it = m_NpcBigUserSet.find(nUserID);
// 		return it!=m_NpcBigUserSet.end();
// 	}
// 	void ClearNpcBigSet()
// 	{
// 		m_NpcBigUserSet.clear();
// 	}
	void DoBroadcastMsg();
protected:
	int weekState;
	int nHourseState;
	int MartialAct;
	int AltarAct;
	int Allday;
	int Hunt;
	int SynMaintain;
	int m_nDayState;//特价商品更新时间
	//优化发送给NPC服务器的CMsgPlayer数量
//	map<OBJID,UINT>		m_NpcBigUserSet;
public:
	char* GetCircle();
	void AddCircle(char* szWords, int CircleTime);
	vector<OBJID>       m_NoitemIdVec; //已经删除物品的itemId
};

//////////////////////////////////////////////////////////////////////
#define		PID	m_idProcess
extern CMapGroup*	MapGroup(PROCESS_ID idProcess);
inline CMapGroup*	MapGroup(PROCESS_ID idProcess){ CHECKF(CMapGroup::IsValidMapGroupProcessID(idProcess)); return &CMapGroup::m_setMapGroup[idProcess]; }
// 用法：MapGroup(PID)->UserManager()->GetUser(idUser);

//////////////////////////////////////////////////////////////////////
#define	Database()			(::MapGroup(PID)->GetDatabase())
#define	RoleManager()		(::MapGroup(PID)->GetRoleManager())
#define	UserManager()		(::MapGroup(PID)->GetUserManager())
#define	MapManager()		(::MapGroup(PID)->GetMapManager())
#define	NpcManager()		(::MapGroup(PID)->GetNpcManager())
#define	GameDataDefault()	(::MapGroup(PID)->QueryDataDefault())
#define	SynManager()		(::MapGroup(PID)->GetSynManager())
#define	LeaveWord()			(::MapGroup(PID)->GetLeaveWord())
#define	GameAction()		(::MapGroup(PID)->GetGameAction())
#define WargameManager()    (::MapGroup(PID)->GetWargameManager())
#define MapInstanceMgr()    (::MapGroup(PID)->GetMapInstanceMgr())
#define LuaScriptMachine()  (::MapGroup(PID)->GetLuaScriptMachine())
#define VariantManager()    (::MapGroup(PID)->GetLuaScriptMachine());
#define InstanceUseMgr()    (::MapGroup(PID)->GetInstanceUseMgr())
#define MapClear()          (::MapGroup(PID)->GetMapClear())
#define NoItemId()          (::MapGroup(PID)->GetNoItemIDVec())
#define UserJinmeiSysMgr()  (::MapGroup(PID)->GetUserJinmeiSysMgr())
#define WargameManager()    (::MapGroup(PID)->GetWargameManager())
#define CurrentMapGroup()(::MapGroup(PID))

//////////////////////////////////////////////////////////////////////
inline IDatabase*		_Database(PROCESS_ID PID)			{ return ::MapGroup(PID)->GetDatabase(); }
inline IRoleManager*	_RoleManager(PROCESS_ID PID)		{ return ::MapGroup(PID)->GetRoleManager(); }
inline IUserManager*	_UserManager(PROCESS_ID PID)		{ return ::MapGroup(PID)->GetUserManager(); }
inline IMapManager*		_MapManager(PROCESS_ID PID)			{ return ::MapGroup(PID)->GetMapManager(); }
inline INpcManager*		_NpcManager(PROCESS_ID PID)			{ return ::MapGroup(PID)->GetNpcManager(); }
inline IDataDefault*	_GameDataDefault(PROCESS_ID PID)	{ return ::MapGroup(PID)->QueryDataDefault(); }
//inline CSynManager*		_SynManager(PROCESS_ID PID)			{ return ::MapGroup(PID)->GetSynManager(); }
inline CLeaveWord*		_LeaveWord(PROCESS_ID PID)			{ return ::MapGroup(PID)->GetLeaveWord(); }
inline CGameAction*		_GameAction(PROCESS_ID PID)			{ return ::MapGroup(PID)->GetGameAction(); }

//////////////////////////////////////////////////////////////////////
inline OBJID			GetServerID()			{ extern DWORD	g_dwServerID; return g_dwServerID; }
inline CItemType*		ItemType()				{ extern CItemType* g_pItemType; CHECKF(g_pItemType); return g_pItemType; }
inline CPortableShopManager* GetPPortableShopManager(){extern CPortableShopManager* g_pPortableShopManager; CHECKF(g_pPortableShopManager); return g_pPortableShopManager;}
inline CShopVASItem*	VASItem()				{ extern CShopVASItem* g_pShopVas; CHECKF(g_pShopVas); return g_pShopVas; }

// add by zlong 2003-11-15
inline CDropRuleMap*	DropRuleMap()			{ extern CDropRuleMap* g_pDropRuleMap; CHECKF(g_pDropRuleMap); return g_pDropRuleMap; }
inline CTaskSetRand*    TaskSetRand()           { extern CTaskSetRand* g_pTaskSetRand; CHECKF(g_pTaskSetRand); return g_pTaskSetRand; }

inline IActionSet*		ActionSet()				{ extern IActionSet* g_setAction; CHECKF(g_setAction); return g_setAction; }
inline ILevupexpSet*	LevupexpSet()			{ extern ILevupexpSet* g_setLevupexp; CHECKF(g_setLevupexp); return g_setLevupexp; }
inline IMagicTypeSet*	MagicTypeSet()			{ extern IMagicTypeSet* g_setMagicType; CHECKF(g_setMagicType); return g_setMagicType; }
inline INpcTypeSet*		StaticNpcTypeSet()		{ extern INpcTypeSet* g_setStaticNpcType; CHECKF(g_setStaticNpcType); return g_setStaticNpcType; }
inline IMagicTypeSet*	AutoMagicTypeSet()		{ extern IMagicTypeSet* g_setAutoMagicType; CHECKF(g_setAutoMagicType); return g_setAutoMagicType; }
inline IMonsterTypeSet*	MonsterType()			{ extern IMonsterTypeSet* g_setMonsterType; CHECKF(g_setMonsterType); return g_setMonsterType; }
inline IEudemonTypeSet*	EudemonType()			{ extern IEudemonTypeSet* g_setEudemonType; CHECKF(g_setEudemonType); return g_setEudemonType; }
inline IPeifangTypeSet*	PeifangType()			{ extern IPeifangTypeSet* g_setPeifangType; CHECKF(g_setPeifangType); return g_setPeifangType; }
inline ITitleTypeSet*	TitleTypeSet()			{ extern ITitleTypeSet* g_setTitleType; CHECKF(g_setTitleType); return g_setTitleType; }
inline ITreasureTypeSet*	TreasureTypeSet()	{ extern ITreasureTypeSet* g_setTreasureType; CHECKF(g_setTreasureType); return g_setTreasureType; }
inline IJinmeiTypeSet*  JinmeiTypeSet()         { extern IJinmeiTypeSet* g_setJinmeiType; CHECKF(g_setJinmeiType); return g_setJinmeiType;}
inline IJinmeiEffectTypeSet* JinmeiEffectTypeSet() { extern IJinmeiEffectTypeSet* g_setJinmeiEffectType; CHECKF(g_setJinmeiEffectType); return g_setJinmeiEffectType; }
inline IJinmeiStrengthTypeSet* JinmeiStrengthTypeSet() { extern IJinmeiStrengthTypeSet* g_setJinmeiStrengthType; CHECKF(g_setJinmeiStrengthType); return g_setJinmeiStrengthType; }
// inline CSynManager* GetSynManager()		{ extern CSynManager* g_pSynManager; CHECKF(g_pSynManager); return g_pSynManager; }
inline CSuit* Suit() {extern CSuit* g_pSuit; CHECKF(g_pSuit); return g_pSuit;}
//inline IInstanceTypeSet*	InstanceTypeSet()			{ extern IInstanceTypeSet* g_pItemAddition; CHECKF(g_pItemAddition); return g_pItemAddition; }

inline IInstanceTypeSet*	InstanceTypeSet()	{ extern IInstanceTypeSet* g_setInstanceType; CHECKF(g_setInstanceType); return g_setInstanceType; }
inline int				  MainServerFlag()	   { extern int g_MainGameServer; return g_MainGameServer; }
inline int				  MainServerData()	   { extern int g_MainServerData; return g_MainServerData; }
//////////////////////////////////////////////////////////////////////
#define	g_db			(*Database())			// 兼容于旧代码
#define	g_UserManager	(*UserManager())		// 兼容于旧代码

#endif // !defined(AFX_MAPGROUP_H__70A5EB1D_66D5_4DF0_BE98_F7E3498FFC72__INCLUDED_)
