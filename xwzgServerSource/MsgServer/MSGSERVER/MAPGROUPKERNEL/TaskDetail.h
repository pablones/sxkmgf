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
    CTaskSetRandData*   GetRandTask(int nChain,OBJID idNpc);//�Ǹ�������,�������ͣ������Ĺֻ���Ʒ�����ֵ(���ڻ�������)
	void	SetTaskSpendEneType(OBJID idUser, OBJID idTask, USHORT nType); //�������ѡ������ľ�������
	UINT	GetTaskSpendEneType(OBJID idUser, OBJID idTask);			   //��ȡ���ѡ������ľ�������

	void  SendAllObjInfo(CUser* pUser,PROCESS_ID idProcess);
	bool CreateBySend(ST_TASKDETAILDATA pInfo);
protected:
	typedef std::vector<CTaskDetailData*>	TASKDETAILDATA_SET;
public:
	TASKDETAILDATA_SET	m_setData;//20061109����.��Ϊpublic.�������.
protected:
	PROCESS_ID		m_idProcess;

	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_TASKDETAIL_H__0223CFB9_198C_474D_B3EF_39531B04F1CD__INCLUDED_)
