// TaskSetRandData.cpp: implementation of the CTaskSetRandData class.
//
//////////////////////////////////////////////////////////////////////
#include "TaskSetRandData.h"
#include "basefunc.h"


MYHEAP_IMPLEMENTATION(CTaskSetRandData,s_heap)



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaskSetRandData::CTaskSetRandData()
{

}

CTaskSetRandData::~CTaskSetRandData()
{

}

bool CTaskSetRandData::Create(IRecordset* pRes)
{
	if(!pRes)
		return false;

	IRecord* pRecord = pRes->CreateNewRecord();

	bool ret = LoadInfo(pRecord);
	SAFE_RELEASE (pRecord);
	return ret;
}

bool CTaskSetRandData::LoadInfo(IRecord* pRes)
{
	if(!pRes)
       return false;

	TaskData.taskid = pRes->LoadDWord("taskid");
	TaskData.ischain = pRes->LoadDWord("ischain");
	TaskData.taskchain = pRes->LoadDWord("taskchain");
	TaskData.monster1 = pRes->LoadDWord("mst1");
	TaskData.monsterNum1 = pRes->LoadDWord("mstnum1");
	TaskData.monster2 = pRes->LoadDWord("mst2");
	TaskData.monsterNum2 = pRes->LoadDWord("mstnum2");
	TaskData.monster3 = pRes->LoadDWord("mst3");
	TaskData.monsterNum3 = pRes->LoadDWord("mstnum3");
	TaskData.item1 = pRes->LoadDWord("item1");
	TaskData.itemNum1 = pRes->LoadDWord("itemnum1");
	TaskData.item2 = pRes->LoadDWord("item2");
	TaskData.itemNum2 = pRes->LoadDWord("itemnum2");
	TaskData.item3 = pRes->LoadDWord("item3");
	TaskData.itemNum3 = pRes->LoadDWord("itemnum3");
	TaskData.Receive_NPC = pRes->LoadDWord("Receive_NPC");
	TaskData.Complete_NPC = pRes->LoadDWord("Complete_NPC");
	TaskData.ReceiveAction = pRes->LoadDWord("receiveaction");
	TaskData.CompleteAction = pRes->LoadDWord("endaction");
	TaskData.setTime = pRes->LoadDWord("settime");
	TaskData.GiveupAction = pRes->LoadDWord("giveupaction");
	TaskData.maskIndex = pRes->LoadDWord("maskIndex");
	TaskData.utype = pRes->LoadDWord("type");
	TaskData.uRandExp = pRes->LoadDWord("randBasExp");
	TaskData.uRandMoney = pRes->LoadDWord("randBasMoney");
	TaskData.uChoseType = pRes->LoadDWord("choseType");;//goto 任务的加成类型,扣精力或装备等
	return true;
}