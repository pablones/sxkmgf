// Rename.h: interface for the CRename class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RENAME_H__6B3553C4_DF91_4E39_8CA7_F5E45972F876__INCLUDED_)
#define AFX_RENAME_H__6B3553C4_DF91_4E39_8CA7_F5E45972F876__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../share/define.h"
#include "GameObj.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "T_SingleObjSet2.h"
#include "Myheap.h"
#include "OnlineGiftData.h"
#include <list>
class CRename  
{
public:
	CRename(PROCESS_ID idProcess);
	virtual ~CRename();
public:
	ULONG	ReleaseByOwner()	{ delete this; return 0; }
	bool	Create();
	bool    Rename(OBJID nUserId,const char *szName,int nType);//0是个人，1是帮派
	bool    ISCanRename(OBJID nUserId);
	void    DeleteRecord(OBJID nUserId,bool exSql=true);
	void	ReSetMapName(const char*szName,int nId);
	void	ReSetTutorListName(CUser* pUser,const char*szName,int nId);
	void	ReSetSynName(const char* szName,OBJID nSynid);
	void	ReSetSynMemberName(const char* szName,OBJID nUserid);
	void	ReSetFriendName(const char* szName,OBJID nUserid);
	void	ReSetVipListName(const char* szName,OBJID nUserid);
protected:
	PROCESS_ID		m_idProcess;
public:
	list<OBJID> m_Canrename;
};

#endif // !defined(AFX_RENAME_H__6B3553C4_DF91_4E39_8CA7_F5E45972F876__INCLUDED_)
