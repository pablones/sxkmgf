// SynManager.h: interface for the CSynManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNMANAGER_H__500BB831_F770_4231_8391_E7B38A5EF78A__INCLUDED_)
#define AFX_SYNMANAGER_H__500BB831_F770_4231_8391_E7B38A5EF78A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "protocol.h"
#include "GameObjSet.h"
#include "Syndicate.h"
#include "MyHeap.h"
#include "T_SingleMap.h"

//////////////////////////////////////////////////////////////////////
typedef	IGameObjSet<CSyndicate>		ISynSet;
typedef	CGameObjSet<CSyndicate>		CSynSet;

typedef CSingleMap<CSyndicate>		SynSet;

//////////////////////////////////////////////////////////////////////
// class ISynMngModify
// {
// public:
// 	virtual OBJID	CreateSyndicate(const CreateSyndicateInfo* pInfo)		PURE_VIRTUAL_FUNCTION_0
// 	virtual bool	DestroySyndicate(OBJID idSyn, OBJID idTarget = ID_NONE)	PURE_VIRTUAL_FUNCTION_0
// 	virtual bool	CombineSyndicate(OBJID idSyn, OBJID idTarget)			PURE_VIRTUAL_FUNCTION_0
// 	virtual void	SetMapSynID(OBJID idMap, OBJID idSyn)					PURE_VIRTUAL_FUNCTION
// };

//////////////////////////////////////////////////////////////////////
class CSynManager/* : ISynMngModify*/
{
protected:
	virtual ~CSynManager();

public:
	CSynManager(PROCESS_ID idProcess);
	ULONG	Release()							{ delete this; return 0; }
	bool	Create();
	bool	Create(OBJID idSyn);
	bool	UpdateSyn(CMsgSynInfo& msg);
	CSyndicate* QuerySyndicate(OBJID idSyn)		{ if(idSyn==ID_NONE) return NULL; return m_setSyn->GetObj(idSyn); }
	CSynSet*	QuerySynSet()					{ ASSERT(m_setSyn); return m_setSyn; }
//	ISynMngModify* QueryModify()				{ return &m_obj; }
//	ISynMngModify* QuerySynchro()				{ return (ISynMngModify*)this; }
	CSyndicate* QuerySynByName(LPCTSTR szSyn);
	bool	FindNextSyn(int& nIter);			// order by amount, return false: no more syn
//	OBJID	GetMapSynID(OBJID idMap)			{ if(idMap==WHITE_SYN_MAP_ID) return m_idWhiteSyn; if(idMap==BLACK_SYN_MAP_ID) return m_idBlackSyn; return ID_NONE; }
	//void    CreateNewSynNpc(int nMapid,int nNpcID,int nPosx,int nPosy);
	//自动降级或解散军团
	void	ProcAutoDestoryOrFallSyn();
	void    ProcChackSynWar();//军团约战检查
	void    SynchroAllLine(OBJID idSyn);///同步线上所有服务此帮派的信息

	OBJID	CreateSyndicate(CreateSyndicateInfo* pInfo);
	bool	DestroySyndicate(OBJID idSyn, OBJID idTarget = ID_NONE);
	void    SendCreateSynToUser(CUser* pUser,int Page = 1);
	void    SendSynList(CUser* pUser,int Page = 1);
 protected:
 	CSynSet*		m_setSyn;
	ISynSet*		m_ISynSet;
	SynSet*			m_Synset;

protected: // ctrl
	PROCESS_ID		m_idProcess;
public:
	BOOL CheckSynName(const char* szName);
	bool DestroySynSub(OBJID idSyn, OBJID idSub);
	void OnTimer(time_t tCurr);
	void MaintainSyn();
	MYHEAP_DECLARATION(s_heap)

//protected:
// 	class XSynMngModify : public ISynMngModify
// 	{
// 	public:
// 		OBJID	CreateSyndicate(const CreateSyndicateInfo* pInfo);
// 		bool	DestroySyndicate(OBJID idSyn, OBJID idTarget = ID_NONE);
// 		bool	CombineSyndicate(OBJID idSyn, OBJID idTarget);
// 		void	SetMapSynID(OBJID idMap, OBJID idSyn);
// 	protected:
// 		CSynManager*	This()	{ return m_pOwner; }
// 		CSynManager*	m_pOwner;
// 		friend class CSynManager;
// 	} m_obj;
// 	friend class CSynManager::XSynMngModify;
};

#endif // !defined(AFX_SYNMANAGER_H__500BB831_F770_4231_8391_E7B38A5EF78A__INCLUDED_)
