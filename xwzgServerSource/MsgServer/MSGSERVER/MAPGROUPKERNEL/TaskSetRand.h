// TaskSetRand.h: interface for the CTaskSetRand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKSETRAND_H__4E4E1662_9600_4F48_8397_F88FB8CDB253__INCLUDED_)
#define AFX_TASKSETRAND_H__4E4E1662_9600_4F48_8397_F88FB8CDB253__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "../share/define.h"

#include "TaskSetRandChain.h"
#include "Myheap.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include <map>
using namespace std;

class IDatabase;

class CTaskSetRand  
{
public:
	CTaskSetRand();
	virtual ~CTaskSetRand();
public:
	static CTaskSetRand * CreateNew(){ return new CTaskSetRand;}
	bool	Release()	{delete this; return true;}
	bool	Create(IDatabase*	pDb);
	void    Destroy();
public:
	CTaskSetRandChain*	GetObjByIndex(OBJID	idChain)	{ if (m_map[idChain]) return m_map[idChain]; return NULL; }
	CTaskSetRandData*   GetDataStruct(OBJID	idChain,OBJID step); 
    DWORD               GetActionId(DWORD idTask);
	DWORD               GetActionGenerate(DWORD idTask,DWORD idNpc);
	DWORD               GetTargetNum(DWORD idTask);
	BOOL                IsTaskOnTime(DWORD idTask);//是否为及时任务
	DWORD				GetSetTime(DWORD idTask,DWORD idStep);
	bool				IsValid(OBJID idChain) {return ((idChain<m_map.size())&&idChain>0);}
protected:
	typedef map<OBJID, CTaskSetRandChain*>				MAP_TASKCHAIN;
	typedef map<OBJID, CTaskSetRandChain*>::iterator	Iter;

	MAP_TASKCHAIN	m_map;
	PROCESS_ID		m_idProcess;
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_TASKSETRAND_H__4E4E1662_9600_4F48_8397_F88FB8CDB253__INCLUDED_)
