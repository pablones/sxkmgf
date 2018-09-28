// MsgItem.h: interface for the CMsgMapItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(MSGMAPITEM_HEADER)
#define MSGMAPITEM_HEADER

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "NetStringPacker.h"

enum {	
		MSGMAPITEM_CREATE		=1,			// to client
		MSGMAPITEM_DELETE		=2,			// to client
		MSGMAPITEM_PICK			=3,			// to server, client: perform action of pick

		MSGMAPITEM_PICK_IN_RANGE=4,			//20060919ÐÞÂÞ.·¶Î§ÄÚÊ°È¡.to Server

		MSGMAPITEM_CAST_TRAP	=10,		// to client
		MSGMAPITEM_SYNCHRO_TRAP	=11,		// to client
		MSGMAPITEM_DROP_TRAP	=12,		// to client, id=trap_id
};

class CMapItem;
class CMsgMapItem : public CNetMsg  
{
public:
	CMsgMapItem();
	virtual ~CMsgMapItem();

	BOOL	Create	(int nAction, CMapItem* pMapItem);
	BOOL	Create	(OBJID id, int nPosX, int nPosY, int nAction=MSGMAPITEM_PICK);

public:	
	BOOL	Create		(char* pMsgBuf, DWORD dwSize);
	void	Process		(void* pInfo);

protected:
	typedef struct {
		MSGHEAD_DEFINE
		OBJID			nAction;
		OBJID			id;
		USHORT			nPosX;
		USHORT			nPosY;
		OBJID			ReTime;
		DWORD			nItemAmount;
		OBJID			idType[8];
	}MSG_Info;

	MSG_Info*	m_pInfo;
};

#endif // !defined(MSGMAPITEM_HEADER)
