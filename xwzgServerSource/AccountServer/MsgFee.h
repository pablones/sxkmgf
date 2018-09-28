// �Ʒ���Ϣ��
// �ɽ��ޣ�2001.11.22
//////////////////////////////////////////////////////////////////////

#ifndef	MSGFEE_H
#define MSGFEE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgFee : public CNetMsg  
{
public:
	CMsgFee();
	virtual ~CMsgFee();

	BOOL	Create	(OBJID idAccount, unsigned char ucType);

public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
//	void			Process		(CGameSocket* pSocket);

	enum {FEE_BEGIN=0, FEE_END, FEE_POINT, HEARTBEAT, FEE_KICKOUT, 
				FEE_OFFLINE, SERVER_BUSY, SERVER_FULL, SERVER_CLOSE, FEE_TICK,FEE_KICKASS,FEE_HOLDASS};
//private:
	typedef struct{
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
		OBJID			idAccount;			// �ʺ�id
		unsigned char	ucType;
	}MSG_Info;

	MSG_Info*	m_pInfo;

};

#endif // MSGFEE_H
