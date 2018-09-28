// TaskSetRandChain.h: interface for the CTaskSetRandChain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKSETRANDCHAIN_H__B93423CC_B65A_4BD9_823F_4B9C38A66193__INCLUDED_)
#define AFX_TASKSETRANDCHAIN_H__B93423CC_B65A_4BD9_823F_4B9C38A66193__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "../share/define.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include "TaskSetRandData.h"
#include <vector>
using namespace std;

class IDatabase;
class CTaskSetRandChain  
{
public:
	CTaskSetRandChain();
	virtual ~CTaskSetRandChain();
public:
	static CTaskSetRandChain * CreateNew(){ return new CTaskSetRandChain;}
	ULONG  Release()	{ delete this; return 0; }

	
	bool   Create(OBJID idChain,IDatabase * pdb);
	void   Destroy();

	bool   LoadInfo(IRecordset*	pRes);
	CTaskSetRandData*  GetRandTask(OBJID idTask,OBJID step);
	CTaskSetRandData*  GetRandTask(OBJID idNpc);
	DWORD         GetActionData(DWORD taskId);
	DWORD  GetActionDataGenerate(DWORD taskId,DWORD npcId);
	DWORD  GetTargetAmount(DWORD taskId);
	int    IsTaskOnTime(DWORD taskId);  //是否为及时任务
	DWORD	GetSetTime(DWORD idTask,DWORD idStep);
	//bool   IsValid(){ for (int i=0; i<m_setRule.size(); i++) if (!m_setRule[i] || !m_setRule[i]->IsValid()) return false; return true; }
protected:
	typedef std::vector<CTaskSetRandData*> RANDDATA_SET;

	OBJID		   m_idChain;
	RANDDATA_SET   m_setRule;

	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_TASKSETRANDCHAIN_H__B93423CC_B65A_4BD9_823F_4B9C38A66193__INCLUDED_)

//////////////////////////////////////////////////////////////////////