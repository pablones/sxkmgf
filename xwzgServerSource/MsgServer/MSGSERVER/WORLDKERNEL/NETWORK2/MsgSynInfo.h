// MsgSynAttrInfo.h: interface for the CMsgSynInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSYNINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
#define AFX_MSGSYNINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
//#include "../Syndicate.h"
#include "NetStringPacker.h"


class CMsgSynInfo : public CNetMsg  
{
public:
	CMsgSynInfo();
	virtual ~CMsgSynInfo();

	BOOL	Create		(CSyndicate* pSyn);
	BOOL	Create		(char* pMsgBuf, DWORD dwSize);
	void	Process		(void* pInfo);

protected:
	typedef struct {
		MSGHEAD_DEFINE
		USHORT		level;
		USHORT		affluent;
		USHORT      amount;
		USHORT      shopAmount;
		USHORT      nBuld;
		USHORT      nMove;
		OBJID		idSyn;
		DWORD		dwMoney;
		DWORD		dwCreateTime;
		USHORT		state;
		USHORT		onlineUser;
		char		szBuf[1];
	}MSG_Info;
	MSG_Info*	m_pInfo;

	CNetStringPacker m_StrPacker;
};

#endif // !defined(AFX_MSGSYNINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
