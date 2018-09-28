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

	TaskDetailInfoForMsg taskDetailArrayTemp[_MAX_TASK_DETAIL_STRUCT_NUM];//�ṹ������.
	int taskDetailNum=0;//�ṹ������ʵ�ʳ�Ա��

	switch(m_pInfo->usTaskDetailMsgType)
	{
	case _TASTHINT_REQUEST_RECEIVED_ALL:
		{
			CTaskDetail * pTask = pUser->GetTaskDetail();
			IF_NOT(pTask)
				return;
			for(int i=0; i<pTask->m_setData.size(); i++)//����
			{
				taskDetailArrayTemp[i].taskID = pTask->m_setData[i]->GetInt(TASKDETAILDATA_TASKID);//taskID
				taskDetailArrayTemp[i].taskPhase = pTask->m_setData[i]->GetInt(TASKDETAILDATA_TASKPHASE);//taskPhase
// 				taskDetailArrayTemp[i].taskCompleteNum = pTask->m_setData[i]->GetInt(TASKDETAILDATA_TASKCOMPLETENUM);//taskCompleteNum
// 				taskDetailArrayTemp[i].lifeTime = pTask->m_setData[i]->GetInt(TASKDETAILDATA_LIFETIME);
				taskDetailNum++;
			}
			//taskMaskҲ����ȥ
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
				//���ж�Ա������ӳ�����2��(18��,�����ӳ�������)��Χ,���򷵻�false
				CUser* pMember = pUser->FindAroundUser(pTeam->GetMemberByIndex(i));
				if(!pMember)
					continue;//������һ����Ա�Ĳ���

				if(pMember && pMember->GetID() != pUser->GetID())
				{
					CTaskDetail * pTask = pMember->GetTaskDetail();
					IF_NOT(pTask)
						continue;
					
					DWORD taskID = m_pInfo->taskDetailStruct[0].taskID;

					// ����Ѿ����ڣ���������
					if (pTask->QueryData(pMember->GetID(), taskID))
					{
						char szParam[1024];
						sprintf(szParam,"%s�Ѿ�����˸�����!",pMember->GetName());
						pUser->SendSysMsg(szParam);
						continue;//������һ����Ա
					}
// 					CMsgTaskDetail msg;
// 					if(msg.Create(_TASKHINT_SET_WAIT,taskID))
// 					   pMember->SendMsg(&msg);
// 					OBJID idAction = TaskSetRand()->GetActionGenerate(taskID);
// 
// 					if(idAction)
// 					{
// 						char szParam[1024];
// 						sprintf(szParam,"%s����˸�����!",pMember->GetName());
// 						pUser->SendSysMsg(szParam);
// 						GameAction()->ProcessAction(idAction, pMember);
// 					}
// 					
					//���ڵ���
					if(pUser&&pUser->IsGM())
					{
						char szMsg[1024];
						if(taskID)
						{
							sprintf(szMsg,"��������ϵͳ:�㹲����taskId:%u",taskID);
							szMsg[255] = '\0';
						}
						else
						{
							sprintf(szMsg,"�������������:taskIDΪ��,�����û�и�����");
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
			TaskDetailInfoForMsg *taskDetailStruct,//�ṹ�������ͷָ��
			unsigned short usTaskDetailStructNum)//�ṹ���������
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

