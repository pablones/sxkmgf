// GameAction.h: interface for the CGameAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEACTION_H__7F7A15BB_A4E8_4B07_A932_4B9ABA6BC9EC__INCLUDED_)
#define AFX_GAMEACTION_H__7F7A15BB_A4E8_4B07_A932_4B9ABA6BC9EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "StrRes.h"

class CScriptActionData : public CActionData
{

public:
	int		GetInt(ACTIONDATA nDataName)
	{
		return atoi(m_data[nDataName].c_str());
	}
	void	SetInt(ACTIONDATA nDataName,int data)
	{
		char tmp[256]="";
		sprintf(tmp,"%d",data);
		m_data[nDataName]=tmp;
	}
	LPCTSTR GetStr (ACTIONDATA nDataName)
	{
		return m_data[nDataName].c_str();	
	}
	void	SetStr(ACTIONDATA nDataName,LPCTSTR strData)
	{
		m_data[nDataName]=strData;
	}
private:
	map<int,string> m_data;
};

class CGameAction  
{
protected:
	CGameAction(PROCESS_ID idProcess)				{ m_idProcess = idProcess; }
	virtual ~CGameAction()							{}
public:
	static CGameAction* CreateNew(PROCESS_ID idProcess)		{ return new CGameAction(idProcess); }
	void	Release()								{ delete this; }
//	friend CUser;
public:
	bool	ProcessAction	(OBJID idAction, CUser* pUser=NULL, IRole* pRole=NULL, CItem* pItem=NULL, LPCTSTR pszAccept=NULL,bool bNeedEnd = false);
	///softworms为了扩展脚本使用遗留接口而扩展的新action调用函数,背锅可能啊..-2007-12-12
	bool    ProcessScriptAction(OBJID idActionType,LPCTSTR szParam,int data, CUser* pUser=NULL, IRole* pRole=NULL, CItem* pItem=NULL, LPCTSTR pszAccept=NULL);
protected:
	void	ReplaceAttrStr		(char* pszTarget, const char* pszSource, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept=NULL);
	bool	ProcessActionSys	(CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept);
	bool	ProcessActionNpc	(CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept);
	bool	ProcessActionMap	(CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept);
	bool	ProcessActionItemOnly	(CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept);
	bool	ProcessActionItem	(CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept);
	bool	ProcessActionSyn	(CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept);
	bool	ProcessActionUser	(CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept);
	bool	ProcessActionMonster(CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept);
	bool	ProcessActionMagic	(CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pazAccept);
	bool	ProcessActionScript (CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pazAccept);
private:
	CUser*	m_pUser;
	IRole*	m_pRole;
	CItem*	m_pItem;

protected:
	PROCESS_ID	m_idProcess;

protected: // ctrl
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_GAMEACTION_H__7F7A15BB_A4E8_4B07_A932_4B9ABA6BC9EC__INCLUDED_)
