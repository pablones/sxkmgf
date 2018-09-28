// MsgRandNPCShop.h: interface for the CMsgRandNPCShop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGRANDNPCSHOP_H__2FB2DE0E_C04B_4E53_B211_F0D366804F0D__INCLUDED_)
#define AFX_MSGRANDNPCSHOP_H__2FB2DE0E_C04B_4E53_B211_F0D366804F0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "NetStringPacker.h"

class CMsgRandNPCShop  : public CNetMsg
{
public:
	CMsgRandNPCShop();
	virtual ~CMsgRandNPCShop();
	
	BOOL	Create(OBJID id, int nType,const char* pszName=NULL);
	BOOL	AddItem(OBJID itemType,DWORD sellType, DWORD leaveAmount, DWORD price);
public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(CGameSocket* pSocket);
	
protected:
	struct randNpcItem
	{
		DWORD nItemType;
		USHORT nSellType;
		USHORT nLeaveAmount;
		DWORD nPrice;
	};

	typedef struct {
		MSGHEAD_DEFINE
		OBJID		id;
		UINT        nType;
		UINT		nItemAmount;
		randNpcItem	stItemInfo[30];
		char		szBuf[1];
	}MSG_Info;
	
	MSG_Info*	m_pInfo;

private:
	CNetStringPacker	m_StrPacker;
};


#endif // !defined(AFX_MSGRANDNPCSHOP_H__2FB2DE0E_C04B_4E53_B211_F0D366804F0D__INCLUDED_)
