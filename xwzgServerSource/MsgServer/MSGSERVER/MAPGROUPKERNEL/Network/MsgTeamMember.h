// MsgTeamMember.h: interface for the CMsgTeamMember class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGTEAMMEMBER_H__0C122D39_8E5B_48A9_94BA_0C20ED3C3E23__INCLUDED_)
#define AFX_MSGTEAMMEMBER_H__0C122D39_8E5B_48A9_94BA_0C20ED3C3E23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "../User.h"

enum
{
	_MSG_TEAMMEMBER_ADDMEMBER = 0,
	_MSG_TEAMMEMBER_DROP_ADDMEMBER = 1,
	_MSG_TEAMMEMBER_CHGMAP = 2 ,
	_MSG_TEAMMEMBER_LEV = 3,
	_MSG_TEAMMEMBER_ANSWERINFO = 4,
	_MSG_TEAMMEMBER_ANSWERINVITEINFO = 5,
};

#define _MAX_TEAMAPPLY 8

// By Minsky,2010-05-28�������˵���Ϣ
struct TeamApplyInfo
{
	OBJID			idPlayer;// ��ҵ�ID
	OBJID			dwLookFace;// ��ҵ�ͷ��
	char            szPlayerName[_MAX_NAMESIZE];// �������
	USHORT			usLev;// �����˵ļ���,������999
	USHORT			usPro;// �����˵�ְҵ
	USHORT			usProLev;// �����˵ļ��ܵȼ�,������999
//	USHORT          idMap;// ������ڵĵ�ͼ
};

class CMsgTeamMember : public CNetMsg  
{
public:
	CMsgTeamMember();
	virtual ~CMsgTeamMember();

public:
	BOOL	Create			(CUser* pMember);
	BOOL	Create			(int nAction, TeamMemberInfo setInfo);
    BOOL    Create          (int nAction, UINT nMapID, UINT id, int nData = 0);
	BOOL	Create			(char* pbufMsg, DWORD dwMsgSize);
	BOOL	Append			(int nAction,TeamMemberInfo info);

	void	Process			(void* pInfo);

private:
	typedef struct 
	{
		MSGHEAD_DEFINE
		UINT				ucAction;
		UINT        		ucAmount;
		UINT                id;
		int                 nData;
		USHORT				usSizeIn;// ���ڳ�Ա������
		USHORT				usSizeOut;// �ȴ����������
		TeamMemberInfo		memInfo[1];
	}MSG_Info;
	
	MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGTEAMMEMBER_H__0C122D39_8E5B_48A9_94BA_0C20ED3C3E23__INCLUDED_)
