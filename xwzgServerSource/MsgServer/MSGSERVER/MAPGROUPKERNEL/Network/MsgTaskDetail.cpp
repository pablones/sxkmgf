// MsgTaskDetail.cpp: implementation of the CMsgTaskDetail class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgTaskDetail.h"
#include "TaskDetail.h"
#include "mapgroup.h"
#include "MsgTalk.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMsgTaskDetail::CMsgTaskDetail()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgTaskDetail::~CMsgTaskDetail()
{

}
//---------------------------
BOOL CMsgTaskDetail::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;
	
	if(_MSG_TASKDETAIL != this->GetType())
		return false;
	
	return true;
}
//-----------------------------
void CMsgTaskDetail::Process (void *pInfo)
{
	CUser* pUser = UserManager()->GetUser(this);
	if(!pUser)
		return;

	TaskDetailInfoForMsg taskDetailArrayTemp[_MAX_TASK_DETAIL_STRUCT_NUM];//结构体数组.
	int taskDetailNum=0;//结构体数组实际成员数

	switch(m_pInfo->usTaskDetailMsgType)
	{
	case _TASTHINT_REQUEST_RECEIVED_ALL:
		{
			CTaskDetail * pTask = pUser->GetTaskDetail();
			IF_NOT(pTask)
				return;
			for(int i=0; i<pTask->m_setData.size(); i++)//遍历
			{
				taskDetailArrayTemp[i].taskID = pTask->m_setData[i]->GetInt(TASKDETAILDATA_TASKID);//taskID
				taskDetailArrayTemp[i].taskPhase = pTask->m_setData[i]->GetInt(TASKDETAILDATA_TASKPHASE);//taskPhase
// 				taskDetailArrayTemp[i].taskCompleteNum = pTask->m_setData[i]->GetInt(TASKDETAILDATA_TASKCOMPLETENUM);//taskCompleteNum
// 				taskDetailArrayTemp[i].lifeTime = pTask->m_setData[i]->GetInt(TASKDETAILDATA_LIFETIME);
				taskDetailNum++;
			}
			//taskMask也塞进去
			taskDetailArrayTemp[i].taskID=0;
//			taskDetailArrayTemp[i].taskMask=pUser->GetTaskMask();
//			taskDetailArrayTemp[i].taskMask=pUser->GetDayTaskMask();
			
			taskDetailNum++;

// 			CMsgTaskDetail msg;
// 			IF_OK(msg.Create(_TASTHINT_APPLY_RECEIVED,
// 				taskDetailArrayTemp,
// 				taskDetailNum))//maybe 0
// 				pUser->SendMsg(&msg);
		}
		break;
	case _TASKHINT_GIVEUP:
		{
			DWORD taskID = m_pInfo->taskDetailStruct[0].taskID;
			CHECK(taskID);
			OBJID idTaskIndex = pUser->GetTaskIndex(taskID);
			pUser->TaskGiveUp(taskID);
			if(idTaskIndex)
				pUser->TaskGiveUp(idTaskIndex);
		}
		break;
	case _TASKHINT_SET_TASKSHARE:
		{
			CTeam* pTeam = pUser->GetTeam();
			if(!pTeam || pTeam->GetMemberAmount() <= 1)
				return ;
			
			for(int i = 0; i < pTeam->GetMemberAmount(); i++)
			{
				//所有队员不可离队长超出2屏(18格,包含队长所在屏)范围,否则返回false
				CUser* pMember = pUser->FindAroundUser(pTeam->GetMemberByIndex(i));
				if(!pMember)
					continue;//继续下一个队员的操作

				if(pMember && pMember->GetID() != pUser->GetID())
				{
					CTaskDetail * pTask = pMember->GetTaskDetail();
					IF_NOT(pTask)
						continue;
					
					DWORD taskID = m_pInfo->taskDetailStruct[0].taskID;

					// 如果已经存在，则不允许创建
					if (pTask->QueryData(pMember->GetID(), taskID))
					{
						char szParam[1024];
						sprintf(szParam,"%s已经完成了该任务!",pMember->GetName());
						pUser->SendSysMsg(szParam);
						continue;//继续下一个队员
					}
// 					CMsgTaskDetail msg;
// 					if(msg.Create(_TASKHINT_SET_WAIT,taskID))
// 					   pMember->SendMsg(&msg);
// 					OBJID idAction = TaskSetRand()->GetActionGenerate(taskID);
// 
// 					if(idAction)
// 					{
// 						char szParam[1024];
// 						sprintf(szParam,"%s获得了该任务!",pMember->GetName());
// 						pUser->SendSysMsg(szParam);
// 						GameAction()->ProcessAction(idAction, pMember);
// 					}
// 					
					//用于调试
					if(pUser&&pUser->IsGM())
					{
						char szMsg[1024];
						if(taskID)
						{
							sprintf(szMsg,"共享任务系统:你共享了taskId:%u",taskID);
							szMsg[255] = '\0';
						}
						else
						{
							sprintf(szMsg,"任务共享参数错误:taskID为空,任务库没有该任务");
						}
						CMsgTalk msg;
						if(msg.Create(SYSTEM_NAME,pUser->GetName(),szMsg,NULL,0x00ffff00,_TXTATR_NORMAL))
							pUser->SendMsg(&msg);
					}
				}
			}	
		}
		break;
	case _TASKHINT_SET_WAIT:
		{

		}
		break;
	case _TASKHINT_SET_TASKTIME:
		{

		}
		break;
	default:
		{
			//warning
		}
		break;
	}
}

BOOL CMsgTaskDetail::Create(unsigned short usTaskDetailMsgType,
			TaskDetailInfoForMsg *taskDetailStruct,//结构体数组的头指针
			unsigned short usTaskDetailStructNum)//结构体数组个数
{
	if(usTaskDetailMsgType == 0	|| usTaskDetailStructNum > _MAX_TASK_DETAIL_STRUCT_NUM)
	{
		return FALSE;
	}
	this->Init();

	m_unMsgSize	= sizeof(MSG_Info) + sizeof(TaskDetailInfoForMsg) * ( (usTaskDetailStructNum == 0?1:usTaskDetailStructNum)-1 );
	if(m_unMsgSize > MAX_PACKETSIZE)
	{
		return FALSE;
	}
	m_unMsgType	=_MSG_TASKDETAIL;

	m_pInfo->usTaskDetailMsgType = usTaskDetailMsgType;
	for(int i=0; i<usTaskDetailStructNum; i++)
	{
		//fill struct
		m_pInfo->taskDetailStruct[i].taskID = taskDetailStruct[i].taskID;
		m_pInfo->taskDetailStruct[i].taskPhase = taskDetailStruct[i].taskPhase;
		m_pInfo->taskDetailStruct[i].data1 = taskDetailStruct[i].data1;
		m_pInfo->taskDetailStruct[i].data2 = taskDetailStruct[i].data2;
		m_pInfo->taskDetailStruct[i].data3 = taskDetailStruct[i].data3;
		m_pInfo->taskDetailStruct[i].data4 = taskDetailStruct[i].data4;
		m_pInfo->taskDetailStruct[i].data5 = taskDetailStruct[i].data5;
		m_pInfo->taskDetailStruct[i].data6 = taskDetailStruct[i].data6;
		m_pInfo->taskDetailStruct[i].isRec = taskDetailStruct[i].isRec;
		m_pInfo->taskDetailStruct[i].isLink = taskDetailStruct[i].isLink;
		m_pInfo->taskDetailStruct[i].randStep = taskDetailStruct[i].randStep;
	}
	m_pInfo->usTaskDetailStructNum = usTaskDetailStructNum;
	return TRUE;
}

