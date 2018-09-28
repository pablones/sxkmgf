// MsgGmMac.h: interface for the CMsgGmMac class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGGMMAC_H__8F5636BA_176D_4EBD_B9D5_F0AA3D95208F__INCLUDED_)
#define AFX_MSGGMMAC_H__8F5636BA_176D_4EBD_B9D5_F0AA3D95208F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NETWORK\NetMsg.h"
enum{
	ACTION_CHECKMAC				=0,
	ACTION_CHECKGMMACFAIL		=1,
	ACTION_CHECKGMMACSUC		=2,
	};
class CMsgGmMac : public CNetMsg  
{
public:
	CMsgGmMac();
	virtual ~CMsgGmMac();
public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void* pInfo);
	BOOL			Create		(char* usMac, USHORT usAction,USHORT usSuc);
protected:
	typedef struct {
		MSGHEAD_DEFINE
		char      usMac[32];			//MAC地址
		USHORT		usSuc;				//是否成功
		USHORT		usAction;			//暂时无用保留
	}MSG_Info;

	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGGMMAC_H__8F5636BA_176D_4EBD_B9D5_F0AA3D95208F__INCLUDED_)
