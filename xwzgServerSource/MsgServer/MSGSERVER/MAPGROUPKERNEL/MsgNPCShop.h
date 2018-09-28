// MsgNPCShop.h: interface for the CMsgNPCShop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGNPCSHOP_H__B534C0E4_8480_4149_8B74_C5E76EE8E2BD__INCLUDED_)
#define AFX_MSGNPCSHOP_H__B534C0E4_8480_4149_8B74_C5E76EE8E2BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "NetStringPacker.h"

class CMsgNPCShop  : public CNetMsg
{
public:
	CMsgNPCShop();
	virtual ~CMsgNPCShop();

	BOOL	Create(OBJID id, int nType,const char* pszName=NULL);
	BOOL	AddItem(OBJID itemType);
public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(CGameSocket* pSocket);
	
protected:
	typedef struct {
		MSGHEAD_DEFINE
		OBJID		id;
		UINT        nType;
		UINT		nItemAmount;
		DWORD		nItemType[60];
		char		szBuf[1];
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
	
private:
	CNetStringPacker	m_StrPacker;

};

#endif // !defined(AFX_MSGNPCSHOP_H__B534C0E4_8480_4149_8B74_C5E76EE8E2BD__INCLUDED_)
