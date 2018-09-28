// MsgUserPos.h: interface for the CMsgUserPos class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGUSERPOS_H__9BA82E3D_A8A4_479A_A33E_D989EB73348B__INCLUDED_)
#define AFX_MSGUSERPOS_H__9BA82E3D_A8A4_479A_A33E_D989EB73348B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NETWORK\NetMsg.h"

class CMsgUserPos : public CNetMsg  
{
public:
	CMsgUserPos();
	virtual ~CMsgUserPos();

	public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void *pInfo);

//public:
//	BOOL			Create		(OBJID idPlayer);

protected:
	typedef struct {
		MSGHEAD_DEFINE

		OBJID		idUser;
		USHORT		usPosX;
		USHORT		usPosY;
	}MSG_Info;

	MSG_Info*	m_pInfo;

};

#endif // !defined(AFX_MSGUSERPOS_H__9BA82E3D_A8A4_479A_A33E_D989EB73348B__INCLUDED_)
