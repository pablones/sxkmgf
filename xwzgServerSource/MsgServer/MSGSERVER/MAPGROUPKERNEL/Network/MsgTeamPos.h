// MsgTeamPos.h: interface for the CMsgTeamPos class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGTEAMPOS_H__FA8398D7_EC5B_4A15_ACD6_F787E2FA8ABF__INCLUDED_)
#define AFX_MSGTEAMPOS_H__FA8398D7_EC5B_4A15_ACD6_F787E2FA8ABF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "../User.h"

const int _TEAMPOS_GETPOS	= 1;//��ȡ����λ��
const int _TEAMPOS_SENDSELFPOS	= 2;//����ѹ㲥�Լ�λ��
const int _TEAMPOS_GETPOSGROUP	= 3;//������ͼ������Ĳ�ѯλ����Ϣ
struct TeamPosInfo
{
	OBJID	idPlayer;//�����Ա��ID
	UINT	idMap;//��Ա���ڵ�ͼ
	USHORT	usPosX;//X����
	USHORT	usPosY;//Y����
	char    szPlayerName[_MAX_NAMESIZE];
};
class CMsgTeamPos : public CNetMsg  
{
public:
	CMsgTeamPos();
	virtual ~CMsgTeamPos();

public:
	BOOL	Create(char* pbufMsg, DWORD dwMsgSize);
	BOOL	Append(CUser* pMember);
	BOOL	Append(OBJID idMember, UINT idMap, USHORT ucPosX, USHORT ucPosY, const char* szPlayerName);
    BOOL	Create(int nAction, OBJID idUser);
	BOOL	Append(TeamPosInfo info);
	
	void	Process(void* pInfo);
	
private:
	typedef struct 
	{
		MSGHEAD_DEFINE
		OBJID               idUser;
		UINT				ucAction;
		UINT        		ucAmount;
		TeamPosInfo			memInfo[1];
	}MSG_Info;
	
	MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGTEAMPOS_H__FA8398D7_EC5B_4A15_ACD6_F787E2FA8ABF__INCLUDED_)
