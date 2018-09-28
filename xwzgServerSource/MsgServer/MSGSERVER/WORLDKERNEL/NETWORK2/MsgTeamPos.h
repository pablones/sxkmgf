// MsgTeamPos.h: interface for the CMsgTeamPos class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGTEAMPOS_H__D6675EA9_948E_4F02_981F_B9CED0E1EFF9__INCLUDED_)
#define AFX_MSGTEAMPOS_H__D6675EA9_948E_4F02_981F_B9CED0E1EFF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
WORLDKERNEL_BEGIN
const int _TEAMPOS_GETPOS	= 1;//��ȡ����λ��
const int _TEAMPOS_SENDSELFPOS	= 2;//����ѹ㲥�Լ�λ��
const int _TEAMPOS_GETPOSGROUP	= 3;//��������ͼ��ȥ���Ҷ���λ��
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
WORLDKERNEL_END
#endif // !defined(AFX_MSGTEAMPOS_H__D6675EA9_948E_4F02_981F_B9CED0E1EFF9__INCLUDED_)
