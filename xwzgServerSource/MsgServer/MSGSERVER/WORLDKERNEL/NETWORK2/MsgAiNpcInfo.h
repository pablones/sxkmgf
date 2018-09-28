// MsgAiNpcInfo.h: interface for the CMsgAiNpcInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(MSGAINPC_HEAD)
#define MSGAINPC_HEAD

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "NetMsg.h"
#include "protocol.h"

WORLDKERNEL_BEGIN
//define constant ..
enum {
		MSGAINPCINFO_CREATENEW		= 1,		// NPCSERVER上传：要求创建并返回。NPCSERVER下传：添加
		MSGAINPCINFO_CREATEGENNEW,///在副本中刷怪
		MSGAINPCINFO_REMOVEGEN,///删除副本动态生成的怪物召唤石
		MSGAINPCINFO_MOVEDEST,				//移动到指定的目标
};

class CMsgAiNpcInfo  : public CNetMsg
{
public:
	CMsgAiNpcInfo();
	virtual ~CMsgAiNpcInfo();

	BOOL	Create	(ST_CREATENEWNPC* pInfo);
	BOOL	CreateMoveDest(UINT idMap,UINT nType,int x,int y);
public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void* pSocket);

protected:
	typedef struct {
		MSGHEAD_DEFINE

		OBJID			id;
		OBJID			idMap;
		OBJID			idGen;
		OBJID			idOwner;
		int				nData;
		unsigned short	usPosX;
		unsigned short	usPosY;
		unsigned short	usAction;
		unsigned short	usType;
		unsigned short	ucOwnerType;
	}MSG_Info;

	MSG_Info*	m_pInfo;
};
WORLDKERNEL_END

#endif // !defined(MSGAINPC_HEAD)
