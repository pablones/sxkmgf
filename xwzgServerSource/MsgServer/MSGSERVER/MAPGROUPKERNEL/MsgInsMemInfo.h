// MsgInsMemInfo.h: interface for the CMsgInsMemInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGINSMEMINFO_H__CC3C32C6_F9CE_45B5_958A_1AA9B08ECAAF__INCLUDED_)
#define AFX_MSGINSMEMINFO_H__CC3C32C6_F9CE_45B5_958A_1AA9B08ECAAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgInsMemInfo : public CNetMsg  
{
public:
	CMsgInsMemInfo();
	virtual ~CMsgInsMemInfo();

	BOOL	Create(const char* szName,bool condition1, bool condition2, bool condition3, bool condition4, bool condition5);
	BOOL	AddMem(const char* szName,bool condition1, bool condition2, bool condition3, bool condition4, bool condition5);
public:	
	BOOL		Create		(char* pMsgBuf, DWORD dwSize);
	void		Process		(void* pInfo);
	
private:
	typedef struct{
		char szName[_MAX_NAMESIZE];
		DWORD dwCondition;
	}MemInfo;

	typedef struct{
		MSGHEAD_DEFINE
		DWORD	dwData;
		MemInfo stInfo[1];
	}MSG_Info;
	
	MSG_Info*	m_pInfo;

};

#endif // !defined(AFX_MSGINSMEMINFO_H__CC3C32C6_F9CE_45B5_958A_1AA9B08ECAAF__INCLUDED_)
