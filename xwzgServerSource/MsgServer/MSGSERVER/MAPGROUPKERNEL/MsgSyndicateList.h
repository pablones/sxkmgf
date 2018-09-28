// MsgSyndicateList.h: interface for the CMsgSyndicateList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSYNDICATELIST_H__B0FF14EC_5937_42EA_9C86_5404ED3FE99D__INCLUDED_)
#define AFX_MSGSYNDICATELIST_H__B0FF14EC_5937_42EA_9C86_5404ED3FE99D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum{
	SYNLISTACTION_NONE		= 0,
	SYNLISTACTION_CREATE	= 1,
	SYNLISTACTION_HAS		= 2,
	SYNLISTACTION_UPDATA	= 3,

	SYNLISTACTION_GETSYNLIST= 4,//  ÐÂ
};

class CMsgSyndicateList : public CNetMsg  
{
public:
	CMsgSyndicateList();
	virtual ~CMsgSyndicateList();

	BOOL	Create(unsigned short usAction,OBJID idSyn,const char* szName,const char* szLeaderName,int nLev , int nAmount);
	BOOL	Append(OBJID idSyn,const char* szName,const  char* szLeaderName,int nLev , int nAmount);
	void	SetMaxData(int data);
public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(CGameSocket* pSocket);
protected:
	struct synMsgData
	{
		OBJID idSyn;
		char szName[16];
		char szLeaderName[16];
		USHORT lev;
		USHORT amount;
	};

	typedef struct {
		MSGHEAD_DEFINE
		unsigned short	usAction;
		unsigned short	usData;
		synMsgData m_data[1];
	}MSG_Info;
	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGSYNDICATELIST_H__B0FF14EC_5937_42EA_9C86_5404ED3FE99D__INCLUDED_)
