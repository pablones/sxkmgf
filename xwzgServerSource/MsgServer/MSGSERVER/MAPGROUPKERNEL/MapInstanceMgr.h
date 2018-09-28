// MapInstanceMgr.h: interface for the CMapInstanceMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPINSTANCEMGR_H__59DC992B_F5B8_4A33_8DBF_5F5C0742B0E2__INCLUDED_)
#define AFX_MAPINSTANCEMGR_H__59DC992B_F5B8_4A33_8DBF_5F5C0742B0E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///softworms-副本管理类-2008-01-04
#include "sigslot.h"
using namespace sigslot;
//#include "DataObject.h"
//
//#define		_TBL_INSTANCETYPE			_TBL_PREHEAD "instancetype"
class CMapInstance;
class CDataObject;

char	szDummyDataObjectTable[];

typedef pair<OBJID,UINT> InstanceID;///副本唯一标识
typedef map<InstanceID,CMapInstance*> CMapInstanceList;///按InstanceType做索引的副本列表
class CMsgMapInstance;
class CMapInstanceMgr  : public has_slots<>
{
//private:
public:
	CMapInstanceMgr();
	virtual ~CMapInstanceMgr();
	BOOL	AddInstance(CMapInstance* ptrInstance);
//	BOOL	RemoveInstance(const InstanceID& id);
	BOOL	Clear();
	bool	CreateMonster(ST_CREATENEWNPC* pInfo);
	OBJID	GetInstanceMapid();
	BOOL	Init(IDatabase* pDB,PROCESS_ID idProcess);
	BOOL	RequestEnterInstance(PROCESS_ID pid,OBJID idUser,OBJID idOwn,UINT nInstanceType,UINT nDomainMapID,UINT nInstanceScope,UINT Cellx,UINT Celly,const char* TeamName);///用户请求进入指定类型的副本
	void	RequestCloseInstance(CMsgMapInstance& msg);///
	void	RequestCloseInstance(OBJID idOwn, UINT nInstanceType);
public:
	void	EnterScript(OBJID idMap,OBJID idUser);
	///事件处理函数
	void	OnPlayerEnterMap (OBJID idMap,OBJID idUser);///用户进入地图
	void	OnPlayerExitMap (OBJID idMap,OBJID idUser);///用户离开地图
	void	OnPlayerTeamApply (const ParamList& paramlist);///用户组队事件

	void	OnTimer(time_t tCurr);///定时器处理函数
	CMapInstance* FindInstance(OBJID idMap);///寻找用户所在副本
protected:
	BOOL	ProcessAllInstanceEvent(time_t tCurr);///处理所有副本事件
	CMapInstanceList::iterator RecycleInstance(const CMapInstanceList::iterator& it);
	BOOL	PureInstanceMgr();///清理所有失效副
	
	CMapInstance* GetInstance(const InstanceID& id);///寻找用户所在副本
	BOOL	SendReplyEnterMsg(OBJID idUser,const InstanceID& id,UINT nInstanceScope,int Cellx,int Celly,UINT nIdProcess=0);
	OBJID	GetInstanceMap(const InstanceID& id);
	UINT	GetInstanceMapType(const InstanceID& id);///得到副本的地图类型
	OBJID	CreateInstanceMap(const InstanceID& id);///为副本创建它所拥有的地图,向地图管理器发出请求
	CMapInstance* CreateInstance();
	BOOL	CreateInstance(const InstanceID& id,PROCESS_ID idOwnProcess,UINT nInstanceScope,UINT nDoMainID = 0);///处理用户创建指定类型的副本
	BOOL	IsExistUserInstance(const InstanceID& id);
	BOOL	IsExistUserInstance(OBJID idUser,UINT nInstanceType);///判断该用户是否已拥有该类型的副本
	BOOL	AddPlayer(OBJID idMap,OBJID idUser);///往副本地图里添加一个用户
	BOOL	RemovePlayer(OBJID idMap,OBJID idUser);///删除副本地图里一个用户
	BOOL	AddPlayer(CMapInstance* ptrInstance,OBJID idUser);///往副本里添加一个用户ID
	BOOL    CheckCurrentTime(UINT nTime);          //检查当前时间
private:
	CMapInstanceList m_instancelist;///副本列表Instance
	CMapInstanceList m_instancerecyclelist;///副本列表Instance

	IDatabase* m_pDB;///内部保存一个数据库的实例指针,用于访问数据库中的邮件表
	PROCESS_ID m_idProcess;///地图组线程ID
	UINT       m_nActiveTime;
};

#endif // !defined(AFX_MAPINSTANCEMGR_H__59DC992B_F5B8_4A33_8DBF_5F5C0742B0E2__INCLUDED_)
