// RoleManager.h: interface for the RoleManager class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_ROLEMANAGER_H__86E7410A_E8FB_40C9_AAF7_21CCCA873389__INCLUDED_)
#define AFX_ROLEMANAGER_H__86E7410A_E8FB_40C9_AAF7_21CCCA873389__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AllHeads.h"
#include "MsgRelay.h"

///������ҽ�ɫ����״̬
class Role
{
public:
	OBJID		idRole;///��ɫ���
	NAMESTR		strRoleName;///��ɫ����
	OBJID		idServer;///������·������ID����ʵ����������
};

typedef map<OBJID,Role> RoleContainer;
typedef RoleContainer::iterator RoleIter;
///��������ע��Ľ�ɫ
class RoleManager  
{
public:
	static RoleManager& Instance()
	{
		static RoleManager object;
		return object;
	}
	~RoleManager();

public:
	Role* GetRole(LPCSTR strRoleName);///ͨ����ɫ����ȡ��ɫ��Ϣ
	Role* GetRole(OBJID idRole);///ͨ����ɫ����ȡ��ɫ��Ϣ
	void OnlineRole(OBJID idRole,LPCSTR	strRoleName,OBJID idServer);///�������
	void QueryRole(int nServerIndex,MsgRelayAction msgAction,OBJID idDstRole,OBJID idSrcRole);///��ѯ����������
	void OfflineRole(OBJID idRole);///�������;
	void PureServerRoleList(OBJID idServer);///�����������������������ӵ�е����
	void PrintAllUser();
	UINT GetRoleCountAtServer(OBJID idServer);///��ѯָ�����е������
	//void UpdateRole(OBJID idRole,LPCSTR	strRoleName,OBJID idServer);///��Ҹ��£��Ƕ�ʱ���໥����ʱ�����Լ�
	int GetRoleLine(LPCTSTR szServer);
private:
	RoleManager();
	RoleContainer m_roleContainer;
};

#endif // !defined(AFX_ROLEMANAGER_H__86E7410A_E8FB_40C9_AAF7_21CCCA873389__INCLUDED_)
