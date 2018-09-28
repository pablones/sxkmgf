// RoleManager.cpp: implementation of the RoleManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RoleManager.h"
#include "MsgManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RoleManager::RoleManager()
{

}

RoleManager::~RoleManager()
{

}

UINT RoleManager::GetRoleCountAtServer(OBJID idServer)///��ѯָ�����е������
{
	Role* roleptr=NULL;
	UINT nRoleCount=0;
	for(RoleIter it=m_roleContainer.begin();it!=m_roleContainer.end();++it)
	{
		Role& role=it->second;
		if(role.idServer==idServer)
		{
			nRoleCount++;
		}
	}
	return nRoleCount;

}

Role* RoleManager::GetRole(OBJID idRole)///ͨ����ɫ����ȡ��ɫ��Ϣ
{
	Role* roleptr=NULL;
	RoleIter it=m_roleContainer.find(idRole);
	if(it!=m_roleContainer.end())
		roleptr=&it->second;
	return roleptr;
}

Role* RoleManager::GetRole(LPCSTR strRoleName)///ͨ����ɫ����ȡ��ɫ��Ϣ
{
	Role* roleptr=NULL;
	for(RoleIter it=m_roleContainer.begin();it!=m_roleContainer.end();++it)
	{
		Role& role=it->second;
		if(strcmp(role.strRoleName,strRoleName)==0)
		{
			roleptr=&role;
			break;
		}
	}
	return roleptr;
}

void RoleManager::OnlineRole(OBJID idRole,LPCSTR strRoleName,OBJID idServer)///�������
{
	Role role;
	role.idRole=idRole;
	role.idServer=idServer;
	strncpy(role.strRoleName,strRoleName,_MAX_NAMESIZE);
	m_roleContainer[idRole]=role;
}

void RoleManager::OfflineRole(OBJID idRole)///�������
{
	m_roleContainer.erase(idRole);
}

void RoleManager::QueryRole(int nServerIndex,MsgRelayAction msgAction,OBJID idDstRole,OBJID idSrcRole)
{
	char szServerName[_MAX_NAMESIZE]="";
	RoleIter it=m_roleContainer.find(idDstRole);
	if(it!=m_roleContainer.end())
		strncpy(szServerName,g_aServerAccount[it->second.idServer].m_szServerName,_MAX_NAMESIZE);
	MsgManager::Instance().SendRelationReplyMsg(nServerIndex,msgAction,idSrcRole,idDstRole,szServerName);
}

///�����������������������ӵ�е����
void RoleManager::PureServerRoleList(OBJID idServer)
{
	for(RoleIter it=m_roleContainer.begin();it!=m_roleContainer.end();)
	{
		Role& role=it->second;
		if(role.idServer==idServer)
			it=m_roleContainer.erase(it);
		else
			++it;
	}
}

void RoleManager::PrintAllUser()
{
	printf("-----PrintAllUser------\n");
	for(RoleIter it=m_roleContainer.begin();it!=m_roleContainer.end();++it)
	{
		Role& role=it->second;
		printf("id:%d---name:%s---serverid:%d---servername:%s\n",role.idRole,role.strRoleName,role.idServer,g_aServerAccount[role.idServer].m_szServerName);
	}
	printf("-----PrintAllUser------\n");
}

int RoleManager::GetRoleLine(LPCTSTR szServer)
{
	int nLine = 0;
	if(szServer && strlen(szServer)>0)
	{
		if (strcmp(szServer, "һ��") == 0)
			nLine = 1;
		else if (strcmp(szServer, "����") == 0)
			nLine = 2;
		else if (strcmp(szServer, "����") == 0)
			nLine = 3;
		else if (strcmp(szServer, "����") == 0)
			nLine = 4;
		else if (strcmp(szServer, "����") == 0)
			nLine = 5;
		else if (strcmp(szServer, "����") == 0)
			nLine = 6;
	}
	
	return nLine;
}