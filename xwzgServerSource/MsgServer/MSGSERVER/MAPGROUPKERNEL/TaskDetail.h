// TaskDetail.h: interface for the CTaskDetail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKDETAIL_H__0223CFB9_198C_474D_B3EF_39531B04F1CD__INCLUDED_)
#define AFX_TASKDETAIL_H__0223CFB9_198C_474D_B3EF_39531B04F1CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable:4786)
#include "../share/define.h"
#include <vector>
#include "TaskDetailData.h"
#include "TaskSetRandData.h"
class IDatabase;
class IRole;

class CTaskDetail  
{
public:
	CTaskDetail();
	virtual ~CTaskDetail();

	static CTaskDetail* CreateNew(){return new CTaskDetail;}
	bool	CreateNewTaskDetailData(ST_TASKDETAILDATA * pInfo,bool bInsert = true);
public:
	bool	Create(PROCESS_ID idProcess,CUser *pUser,bool bLogin = true);
	bool	Release()	{delete this; return true;}
public:
	CTaskDetailData* QueryData(OBJID idData);
	CTaskDetailData* QueryData(OBJID idUser,OBJID idTask);
	CTaskDetailData* QueryDataByChain(OBJID idUser,OBJID idChain);
	int              QueryDataNumByChain(OBJID idUser,OBJID idChain);
	bool	FindNextData(int &nIter);
	bool	FindData(OBJID  idTask,int nPhase,int nCompleteNum, int &nIter);
	bool	FindNextData(OBJID idUser,int& nIter);
	bool	DeleteData(OBJID idUser,OBJID idTask);
	ULONG    DeleteDataByChain(OBJID idUser,OBJID idChain);
    CTaskSetRandData*   GetRandTask(int nChain,OBJID idNpc);//那个任务连,那中类型，产生的怪或物品的最大值(用于获得随机数)
	void	SetTaskSpendEneType(OBJID idUser, OBJID idTask, USHORT nType); //设置玩家选择的消耗精力类型
	UINT	GetTaskSpendEneType(OBJID idUser, OBJID idTask);			   //获取玩家选择的消耗精力类型

	void  SendAllObjInfo(CUser* pUser,PROCESS_ID idProcess);
	bool CreateBySend(ST_TASKDETAILDATA pInfo);
protected:
	typedef std::vector<CTaskDetailData*>	TASKDETAILDATA_SET;
public:
	TASKDETAILDATA_SET	m_setData;//20061109修罗.改为public.方便访问.
protected:
	PROCESS_ID		m_idProcess;

	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_TASKDETAIL_H__0223CFB9_198C_474D_B3EF_39531B04F1CD__INCLUDED_)
