// MsgPlayDetil.h: interface for the CMsgPlayDetil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGPLAYDETIL_H__84E5E889_4152_425D_80B1_DB0478C5E012__INCLUDED_)
#define AFX_MSGPLAYDETIL_H__84E5E889_4152_425D_80B1_DB0478C5E012__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "NetStringPacker.h"

class CUser;
class CMsgPlayDetil : public CNetMsg  
{
public:
	CMsgPlayDetil();
	virtual ~CMsgPlayDetil();
public:
	BOOL			Create		(CUser* pUser);
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void *pInfo);
protected:
	typedef struct {
		MSGHEAD_DEFINE
		OBJID	id;
		OBJID   nEquip[30];
		USHORT  usWarSroce;
		USHORT	usUserlev;
		USHORT	usFirPro;
		USHORT	usFirProLev;
		USHORT	usSecPro;
		USHORT	usSecProLev;
		USHORT	usSex;
		char szBuf[1];	
	}MSG_Info;
	MSG_Info*	m_pInfo;
	
private:
	CNetStringPacker	m_StrPacker;

};

#endif // !defined(AFX_MSGPLAYDETIL_H__84E5E889_4152_425D_80B1_DB0478C5E012__INCLUDED_)
