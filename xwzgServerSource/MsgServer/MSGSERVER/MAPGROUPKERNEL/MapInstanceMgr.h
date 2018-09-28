// MapInstanceMgr.h: interface for the CMapInstanceMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPINSTANCEMGR_H__59DC992B_F5B8_4A33_8DBF_5F5C0742B0E2__INCLUDED_)
#define AFX_MAPINSTANCEMGR_H__59DC992B_F5B8_4A33_8DBF_5F5C0742B0E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///softworms-����������-2008-01-04
#include "sigslot.h"
using namespace sigslot;
//#include "DataObject.h"
//
//#define		_TBL_INSTANCETYPE			_TBL_PREHEAD "instancetype"
class CMapInstance;
class CDataObject;

char	szDummyDataObjectTable[];

typedef pair<OBJID,UINT> InstanceID;///����Ψһ��ʶ
typedef map<InstanceID,CMapInstance*> CMapInstanceList;///��InstanceType�������ĸ����б�
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
	BOOL	RequestEnterInstance(PROCESS_ID pid,OBJID idUser,OBJID idOwn,UINT nInstanceType,UINT nDomainMapID,UINT nInstanceScope,UINT Cellx,UINT Celly,const char* TeamName);///�û��������ָ�����͵ĸ���
	void	RequestCloseInstance(CMsgMapInstance& msg);///
	void	RequestCloseInstance(OBJID idOwn, UINT nInstanceType);
public:
	void	EnterScript(OBJID idMap,OBJID idUser);
	///�¼�������
	void	OnPlayerEnterMap (OBJID idMap,OBJID idUser);///�û������ͼ
	void	OnPlayerExitMap (OBJID idMap,OBJID idUser);///�û��뿪��ͼ
	void	OnPlayerTeamApply (const ParamList& paramlist);///�û�����¼�

	void	OnTimer(time_t tCurr);///��ʱ��������
	CMapInstance* FindInstance(OBJID idMap);///Ѱ���û����ڸ���
protected:
	BOOL	ProcessAllInstanceEvent(time_t tCurr);///�������и����¼�
	CMapInstanceList::iterator RecycleInstance(const CMapInstanceList::iterator& it);
	BOOL	PureInstanceMgr();///��������ʧЧ��
	
	CMapInstance* GetInstance(const InstanceID& id);///Ѱ���û����ڸ���
	BOOL	SendReplyEnterMsg(OBJID idUser,const InstanceID& id,UINT nInstanceScope,int Cellx,int Celly,UINT nIdProcess=0);
	OBJID	GetInstanceMap(const InstanceID& id);
	UINT	GetInstanceMapType(const InstanceID& id);///�õ������ĵ�ͼ����
	OBJID	CreateInstanceMap(const InstanceID& id);///Ϊ������������ӵ�еĵ�ͼ,���ͼ��������������
	CMapInstance* CreateInstance();
	BOOL	CreateInstance(const InstanceID& id,PROCESS_ID idOwnProcess,UINT nInstanceScope,UINT nDoMainID = 0);///�����û�����ָ�����͵ĸ���
	BOOL	IsExistUserInstance(const InstanceID& id);
	BOOL	IsExistUserInstance(OBJID idUser,UINT nInstanceType);///�жϸ��û��Ƿ���ӵ�и����͵ĸ���
	BOOL	AddPlayer(OBJID idMap,OBJID idUser);///��������ͼ�����һ���û�
	BOOL	RemovePlayer(OBJID idMap,OBJID idUser);///ɾ��������ͼ��һ���û�
	BOOL	AddPlayer(CMapInstance* ptrInstance,OBJID idUser);///�����������һ���û�ID
	BOOL    CheckCurrentTime(UINT nTime);          //��鵱ǰʱ��
private:
	CMapInstanceList m_instancelist;///�����б�Instance
	CMapInstanceList m_instancerecyclelist;///�����б�Instance

	IDatabase* m_pDB;///�ڲ�����һ�����ݿ��ʵ��ָ��,���ڷ������ݿ��е��ʼ���
	PROCESS_ID m_idProcess;///��ͼ���߳�ID
	UINT       m_nActiveTime;
};

#endif // !defined(AFX_MAPINSTANCEMGR_H__59DC992B_F5B8_4A33_8DBF_5F5C0742B0E2__INCLUDED_)
