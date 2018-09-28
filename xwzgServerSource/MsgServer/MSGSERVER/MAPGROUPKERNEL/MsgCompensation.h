// MsgCompensation.h: interface for the CMsgCompensation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGCOMPENSATION_H__D8432F47_D950_41CB_A171_DB1693155463__INCLUDED_)
#define AFX_MSGCOMPENSATION_H__D8432F47_D950_41CB_A171_DB1693155463__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NETWORK\NetMsg.h"
enum{
	COMPOSITONACTION_NULL = 0,
	COMPOSITONACTION_ADD,
	COMPOSITONACTION_QUERY,
	COMPOSITONACTION_QUERYMSG,
	COMPOSITONACTION_PICK,
	COMPOSITONACTION_PICK_FAIL,
	COMPOSITONACTION_PICK_SUC,
};
class CMsgCompensation : public CNetMsg  
{
public:
	CMsgCompensation();
	virtual ~CMsgCompensation();

	BOOL	Create		(USHORT nAction,OBJID id,OBJID idType,DWORD dwAmount);
	BOOL	Add			(OBJID id,OBJID idType,DWORD	dwAmount);
public:	
	BOOL	Create		(char* pMsgBuf, DWORD dwSize);
	void	Process		(void* pInfo);
	
	//protected:
	struct CompensationMiniInfo
	{
		OBJID	id;
		OBJID	typeId;
		DWORD	nAmount;
	};

	typedef struct {
		MSGHEAD_DEFINE		
		USHORT		usAction;
		USHORT		usAmount;
		CompensationMiniInfo m_CompensationInfo[1];
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGCOMPENSATION_H__D8432F47_D950_41CB_A171_DB1693155463__INCLUDED_)
