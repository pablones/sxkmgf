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
const int _TEAMPOS_GETPOS	= 1;//获取队友位置
const int _TEAMPOS_SENDSELFPOS	= 2;//向队友广播自己位置
const int _TEAMPOS_GETPOSGROUP	= 3;//到其他地图组去查找队友位置
struct TeamPosInfo
{
	OBJID	idPlayer;//队伍成员的ID
	UINT	idMap;//成员所在地图
	USHORT	usPosX;//X坐标
	USHORT	usPosY;//Y坐标
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
