// TaskSetRandData.h: interface for the CTaskSetRandData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKSETRANDDATA_H__61A1C23A_DE72_4FC4_A40C_16AF3ED3FEC7__INCLUDED_)
#define AFX_TASKSETRANDDATA_H__61A1C23A_DE72_4FC4_A40C_16AF3ED3FEC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "../share/define.h"
#include "Myheap.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include <vector>
using namespace std;

enum RANDTASKSET{
	RANDTASKSET_ID,
	RANDTASKSET_TASKID,
	RANDTASKSET_MONSTER,
	RANDTASKSET_ITEM,
	RANDTASKSET_TYPE,
	RANDTASKSET_TASKCHAIN,
	RANDTASKSET_AWARDACTION,
	RANDTASKSET_GENERATEACTION,
	RANDTASKSET_TARGETNUM,
};
//goto 任务接取类型
enum TASKCHOSETYPE{
	TASKCHOSETYPE_MUSTENE  = 0x0001, //只能消耗精力接取的任务
	TASKCHOSETYPE_CHOSEENE = 0x0002, //可选择消耗精力值接取的任务
	TASKCHOSETYPE_EQUIP	   = 0x0004, //可选择扣除装备的任务
};

class CTaskSetRandData  
{
public:
	CTaskSetRandData();
	virtual ~CTaskSetRandData();

public:
	static CTaskSetRandData * CreateNew() {return new CTaskSetRandData;}
	ULONG Release(){delete this;return 0;}

public:	
	bool	Create(IRecordset*	pRes);
	bool	LoadInfo(IRecord*	pRes);
	
	struct RanTaskSet
	{
		unsigned int taskid;
		unsigned int ischain;
		unsigned int taskchain;
		unsigned int monster1;
		unsigned int monsterNum1;
		unsigned int monster2;
		unsigned int monsterNum2;
		unsigned int monster3;
		unsigned int monsterNum3;
		unsigned int item1;
		unsigned int itemNum1;
		unsigned int item2;
		unsigned int itemNum2;
		unsigned int item3;
		unsigned int itemNum3;
		unsigned int Receive_NPC;
		unsigned int Complete_NPC;
		unsigned int ReceiveAction;
		unsigned int CompleteAction;
		unsigned int GiveupAction;
		unsigned int setTime;
		unsigned int maskIndex;
		unsigned int utype;
		unsigned int uRandExp;
		unsigned int uRandMoney;
		unsigned int uChoseType;//goto 任务的加成类型,扣精力或装备等
	};

	RanTaskSet TaskData;
	
	bool	IsValid()	{ return true/*(m_setItemType.size() > 0)*/; }
	bool	IsNormalTask()	{ return TaskData.uChoseType == 0; }//普通任务，无精力或装备附加选项
	bool	CanSpendEne()	{ return (TaskData.uChoseType & TASKCHOSETYPE_CHOSEENE) != 0; }//是否是可选精力任务
	bool	MustSpendEne()	{ return (TaskData.uChoseType & TASKCHOSETYPE_MUSTENE) != 0; } //是否为必须消耗精力任务
	bool	CanSpendEquip()	{ return (TaskData.uChoseType & TASKCHOSETYPE_EQUIP) != 0; }   //是否为可回收装备任务
protected:
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_TASKSETRANDDATA_H__61A1C23A_DE72_4FC4_A40C_16AF3ED3FEC7__INCLUDED_)
