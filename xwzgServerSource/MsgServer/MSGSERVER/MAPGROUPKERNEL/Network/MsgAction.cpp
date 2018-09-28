// MsgAction.cpp: implementation of the CMsgAction class.
//
//////////////////////////////////////////////////////////////////////
#include "afx.h"

#include "AllMsg.h"
#include "../mapgroup.h"
#include "../transformation.h"
#include "../Agent.h"
#include "../User.h"
#include "../SKPostOffice.h"
#include "MsgVAS.h"
#include "AINpc.h"
#include "MsgTeamPos.h"
extern int g_SpecilLine;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgAction::CMsgAction()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgAction::~CMsgAction()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgAction::Create(OBJID idPlayer, int nPosX, int nPosY, USHORT usDir, USHORT usAction, DWORD dwData)
{
	DEBUG_CREATEMSG("ACTION",idPlayer,usAction,"",usDir,0);

	// param check
	if (idPlayer == ID_NONE)
		return false;

	// init
	this->Init();

	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_ACTION;
	m_pInfo->dwTimeStamp=::TimeGet();	

	m_pInfo->idUser		=idPlayer;
	m_pInfo->unPosX		=(USHORT)nPosX;
	m_pInfo->unPosY		=(USHORT)nPosY;
	m_pInfo->unDir		=usDir;
	m_pInfo->dwData		=dwData;
	m_pInfo->usAction	=usAction;

	return true;
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgAction::Create(MSG_GIFT cGift)
{

	this->Init();
	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_ACTION;

	m_pInfo->dwTimeStamp=cGift.Gift1;	
	m_pInfo->idUser		=cGift.Gift2;
	m_pInfo->dwData		=cGift.Gift3;
	m_pInfo->usAction   =cGift.usAction;
	m_pInfo->idNpc		=cGift.Gift4;
	m_pInfo->unPosX		=cGift.DayAmount;
	m_pInfo->unPosY		=cGift.Num;
	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgAction::Create(OBJID idPlayer, int nPosX, int nPosY, USHORT usDir, USHORT usAction, USHORT usTargetPosX, USHORT usTargetPosY)
{
	DEBUG_CREATEMSG("ACTION",idPlayer,usAction,"",usDir,0);

	// param check
	if (idPlayer == ID_NONE)
		return false;

	// init
	this->Init();

	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_ACTION;
	m_pInfo->dwTimeStamp=::TimeGet();	

	m_pInfo->idUser		=idPlayer;
	m_pInfo->dwData	    = usTargetPosX;
 	m_pInfo->unPosX		=(USHORT)nPosX;
 	m_pInfo->unPosY		=(USHORT)nPosY;
	m_pInfo->unDir		=usDir;
	m_pInfo->usAction	=usAction;

	m_pInfo->usTargetPosX	=usTargetPosX;
	m_pInfo->usTargetPosY	=usTargetPosY;

	return true;
}
//////////////////////////////////////////////////////////////////////
//goto
BOOL CMsgAction::Create(OBJID idPlayer, int nPosX, int nPosY, USHORT usDir, USHORT usAction, USHORT usTargetPosX, USHORT usTargetPosY, const char* szTargetName)
{
	DEBUG_CREATEMSG("ACTION",idPlayer,usAction,"",usDir,0);
	
	// param check
	if (idPlayer == ID_NONE)
		return false;
	
	// init
	this->Init();
	
	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_ACTION;
	m_pInfo->dwTimeStamp=::TimeGet();	
	
	m_pInfo->idUser		=idPlayer;
	m_pInfo->dwData	    = usTargetPosX;
	m_pInfo->unPosX		=(USHORT)nPosX;
	m_pInfo->unPosY		=(USHORT)nPosY;
	m_pInfo->unDir		=usDir;
	m_pInfo->usAction	=usAction;
	if (szTargetName && strlen(szTargetName)>0)
		strcpy(m_pInfo->szTargetName, szTargetName);
	
	m_pInfo->usTargetPosX	=usTargetPosX;
	m_pInfo->usTargetPosY	=usTargetPosY;
	
	return true;
}
//////////////////////////////////////////////////////////////////////
//  [�½���9/26/2007]
BOOL CMsgAction::Create(USHORT usDir, USHORT usAction, DWORD dwData/* =0 */)
{
	//DEBUG_CREATEMSG("ACTION",usAction,"",usDir,0);
		
	// init
	this->Init();
	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_ACTION;
	m_pInfo->dwTimeStamp=::TimeGet();	

	m_pInfo->unDir		=usDir;
	m_pInfo->dwData		=dwData;
	m_pInfo->usAction	=usAction;

	return true;
}

BOOL CMsgAction::Create(OBJID idPlayer, int nPosX, int nPosY, USHORT usDir, USHORT usAction, DWORD dwData,OBJID idNpc,BOOL bShow)
{
	DEBUG_CREATEMSG("ACTION",idPlayer,usAction,"",usDir,0);

	// param check
	if (idPlayer == ID_NONE)
		return false;

	// init
	this->Init();

	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_ACTION;
	m_pInfo->dwTimeStamp=::TimeGet();

	m_pInfo->idUser		=idPlayer;
	m_pInfo->unPosX		=(USHORT)nPosX;
	m_pInfo->unPosY		=(USHORT)nPosY;
	m_pInfo->unDir		=usDir;
	m_pInfo->dwData		=dwData;
	m_pInfo->usAction	=usAction;

	m_pInfo->idNpc	=idNpc;
	return true;
}

void CMsgAction::DumpMsg(LPCTSTR szReasion,...)
{
	DEBUG_TRY 
	char buffer[2048]="";
    vsprintf( buffer, szReasion, (char*) ((&szReasion)+1) );
	
	::LogSave("CMsgAction::DumpMsg szReasion:%s,msgsize:%d,usAction:%d,idTarget:%d,idUser:%d,dwData:%d,szTargetName:%s,idNpc:%d,unDir:%d,unPosX:%d,unPosY:%d",
		buffer,m_unMsgSize,m_pInfo->usAction,m_pInfo->idTarget,m_pInfo->idUser,m_pInfo->dwData,m_pInfo->szTargetName,m_pInfo->idNpc,m_pInfo->unDir,m_pInfo->unPosX,m_pInfo->unPosY);

	DEBUG_CATCH("CMsgAction::DumpMsg crash")
}

//////////////////////////////////////////////////////////////////////////
BOOL CMsgAction::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_ACTION != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgAction::Process(void *pInfo)
{
#ifdef _MSGDEBUG
	::LogMsg("Process CMsgAction, idUser:%u, data:%u",
					m_pInfo->idUser,
					m_pInfo->dwData);
#endif
//	DEBUG_PROCESSMSG("ACTION",m_pInfo->idUser,m_pInfo->usAction,"",m_pInfo->unDir,0);
 
	int nStep = 0;
	DEBUG_TRY
	CUserPtr pUser	= UserManager()->GetUser(this);
	nStep = 1;
	
	if(IsNpcMsg())
	{
		IRole* pRole	= RoleManager()->QueryRole(this, m_pInfo->idUser);
		if (pRole)
		{
			CMonster* pMonster = NULL;			
			pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster));
			if(!pMonster)
				return;

			switch(m_pInfo->usAction)
			{
			//20080110����:AI���--------------------------
			case MSGACTION_CHGTARGET_ACK:
				{
					IRole* pTarget	= pMonster->FindAroundRole(m_pInfo->dwData);
					pMonster->SetAttackTarget(pTarget);
				}
				break;
			case MSGACTION_RESET_REQ:
				{
					//˲���Ѫ
					int nLifeAdd = pMonster->GetMaxLife() - pMonster->GetLife();
					pMonster->AddAttrib(_USERATTRIB_LIFE,
						nLifeAdd,
						SYNCHRO_BROADCAST);//�㲥
						
					//��ճ�ޱ�
					pMonster->ClsEnmityListSynNpc(true);
						
					//��Ӧ
					CMsgAction msg;
					IF_OK(msg.Create(pMonster->GetID(),//idPlayer
						0,//nPosX
						0,//nPosY
						0,//usDir
						MSGACTION_RESET_ACK))
					{
						pMonster->SendMsg(&msg);
					}
				}
				break;
			case actionJump:
				{
					pMonster->BroadcastRoomMsg(this, INCLUDE_SELF);
						
					if(!(pMonster->GetMap()))
						return;

					if(!pMonster->GetMap()->IsValidPoint(m_pInfo->unPosX, m_pInfo->unPosY))
						return;
						
					if(pMonster->GetDistance(m_pInfo->unPosX, m_pInfo->unPosY) <= CELLS_PER_BLOCK)
					{
						pMonster->ProcessOnMove(MOVEMODE_TRANS);			//@@@ ��ʱ���룬��Ӧ������ôԶ��
						pMonster->TransPos(m_pInfo->unPosX, m_pInfo->unPosY);
					}
					else
					{
						CHECK(!"pMonster->GetDistance(m_pInfo->unPosX, m_pInfo->unPosY) <= CELLS_PER_BLOCK");
					}
				}
				break;
			case actionMoveComplete:
// 				if (!pUser)
// 				{
// 					CMonster* pMonster = NULL;
// 					if(pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
// 					{
// 						if(!pMonster)
// 							break;
// 						
// 						if(pMonster->GetOwnerID() != 0)
// 						{					
// 							CUser* pTarget = UserManager()->GetUser(pMonster->GetOwnerID());
// 							if(pTarget && pTarget->GetYabiaoState() == 1)
// 							{
// 								OBJID nAction = NpcManager()->GetYabiaoAction(pTarget->GetYabiaoRule(),1);
// 								GameAction()->ProcessAction(nAction, pTarget);
// 							}
// 							else
// 							{
// 								NpcManager()->DelMonster(pMonster);
// 							}
// 						}
// 					}
// 				}
				break;
			case actionSynchro:
				break;
			//AI���end------------------------------------
			}
		}
	}
	

	nStep = 2;
	if (!pUser)
		return;

	CHECK(pUser->GetID());
	switch(m_pInfo->usAction)
	{
	case actionEudemonBorn:
		m_pInfo->idUser	= pUser->GetID();
	}

	switch(m_pInfo->usAction)
	{
	case actionFeePoint:
	case actionEnterMap:
	case actionClientInitOK:
	case actionGetSecInfo:
	case actionGetItemSet:
	case actionQueryEudemonList:
	case actionQuerySkillList:
	case actionQueryUserkeyList:
	case actionQueryCDList:
	case actionGetEquipItemSet:
	case actionQueryMissList:
	case actionQueryMoneyAdd:
	case actionQuerySynInfo:
	case actionQueryOtherInfo:
	case actionQueryAuto:
	case actionQueryTitleList:
	case actionQueryTreasure:
	case actionQueryJinmeiInfo:
//	case actionClientInitComplete://�ͻ��˳�ʼ��ȫ�����
		break;
	default:
		if(!pUser->CanBeSee())
			return;
	}
	
	nStep = 3;
	// stop fight
	switch(m_pInfo->usAction)
	{
	case actionGift:
	case actionAcceptGift:
	case actionSetDress:
	case actionChangeFace:
	case actionQueryFriendInfo:
	case actionQueryEnemyInfo:
	case actionQueryLeaveWord:
	case actionQueryPlayer:
	case actionXpCLear:
	case actionQueryCryOut:
	case actionQueryTeamMember:
	case actionQueryEudemonList:
	case actionQuerySkillList:
	case actionQueryUserkeyList:
	case actionQueryCDList:
	case actionQueryMissList:
	case actionQueryMoneyAdd:
	case actionQueryBuffAdd:
	case actionQuerySynInfo:
	case actionChgDir:					//06.10.24�������,ʹ�ü��ܸı䷽�򲻵���DEFAULT,�����Ŀ��
	case actionFeePoint:
	case actionSychroVas:
	case actionGetPlayDetil:
	case actionEudemonChoose:
	case actionQueryEudemon:
	case actionGetSecInfo:
	case actionUpUserlev:
	case actionAddPoint:
	case actionActiveNPC:
	case actionAcceptTask:
	case actionGiveupTask:
	case actionEndTask:
	case actionEudemonChgName:
	case actionEudemonEat:
	case actionEudemonHappy:
	case actionEudemonGiveup:
	case actionAddEudemonPoint:
	case actionEudemonError:
	case actionEudemonBornAdd:
	case actionEudemonBornLock:
	case actionEudemonBorn:
	case actionEudemonBornCancel:
	case actionEudemonBaby:
	case actionEudemonUnderUp:
	case actionEudemonMix:
	case actionEudemonLean:
	case actionEudemonKillUp:
	case actionEudemonCall:
	case actionEudemonReset:
	case actionEudemonIntelUp:
	case actionEudemonFusionUp:
	case actionEudemonTranse:
	case actionEudemonSoul:
	case actionRemoveEudemonSoul:
	case actionEudemonChangeSex:
	case actionEudSkillUnlean:
	case actionTaskSelectItem:
	case actionTaskAnswerOk:
	case actionFlyMapByItem:
	case actionPKProcSyn:
	case actionGotojial_Public:
	case actionLifeSpanadd:
	case actionQueryLeaveExp:
	case actionGetLeaveExp:
	case actionVasToCheck:
	case actionEXPToCheck:
//	case actionQuerySchoolMember:
	case actionMine:
	case actionStopMine:
	case actionQueryNewGive:
	case actionGetNewGive:
	case actionQueryOtherInfo:
	case actionGetAward:
	case actionGetEudemonMagicCD:
	case actionGetTeamMemPos:
	case actionSendPosToTeam:
	case actionQueryTitleList:
	case actionSetUseTitle:
	case actionQueryTreasure:
	case actionLottery:
	case actionGetTreasure:
	case actionDropTreasure:
	case actionQueryJinmeiInfo:
	case actionUpJinmeiLev:
	case actionUpjinmeiLevQuick:
	case actionStartLearnJinmei:
	case actionStopLearnJinmei:
	case actionStrengthJinmei:
	case actionAddJinmeiWaitQueue:
    case actionMoveJinmeiWaitQueue:
	case actionAddJinmeiValue:
	case actionUseAllZhenyuanDan:
	case actionChoseTimeBox:
	case actionVipGetVas:
		break;
	default:
		{
			pUser->ClrAttackTarget();
		}
		break;
	}

	nStep = 4;
	switch(m_pInfo->usAction)
	{
	case actionClientInitComplete://�ͻ��˳�ʼ��ȫ�����
		{
			CHECK(pUser);
			///�ж��������ϴ����ڸ�������ߵ�,�Ϳ��Դ�������
			DEBUG_TRY
			TeamMemberInfo menberInfo;
			if(pUser->GetTeam())
			{	
				pUser->GetTeam()->GetMemberInfo(menberInfo,pUser->GetID());
				if(menberInfo.idMap)
				{
					if(!pUser->GetMap()->IsDynaMap())
					{
						if(menberInfo.idMap < 100000)
							pUser->RequestEnterInstance(menberInfo.idMap,1000001,1,menberInfo.usPosX,menberInfo.usPosY);
						else
							pUser->RequestEnterInstance(menberInfo.idMap - 100000,2000001,1,menberInfo.usPosX,menberInfo.usPosY);
					}
					else
					{
						LOGWARNING("������ڸ�����ʱ����ڲ�����Ϣ");
						pUser->GetTeam()->SetLocation(pUser->GetID(),0,0,0,true);
					}
				}
			}
			DEBUG_CATCH("actionClientInitComplete ���߽��������̱�����!")

		}
		break;
	case actionFeePoint:
		{
			SYSTEMTIME sysTime={0};
			::GetLocalTime( &sysTime );	//�õ����ػ�ϵͳʱ��
			m_pInfo->dwTimeStamp=sysTime.wYear;
			m_pInfo->unPosX=sysTime.wMonth;
			m_pInfo->unPosY=sysTime.wDay;
			m_pInfo->unDir=sysTime.wHour;
			m_pInfo->dwData=sysTime.wMinute;
			m_pInfo->idNpc=sysTime.wSecond;
			pUser->SendMsg(this);
			////��������Դ���
			///�����������ʱ������������б�
			CMsgTalk msg;
			if (g_SpecilLine==1)
				msg.Create(pUser->GetName(),"s","","",GetSocketID(),_TXTATR_MSG_SERVERLIST);
			else
				msg.Create(pUser->GetName(),SYSTEM_NAME,"","",GetSocketID(),_TXTATR_MSG_SERVERLIST);
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,"");
		}
		break;
	case actionGetPlayDetil:
		{
			CUser* pTarget =  NULL;
			if(m_pInfo->idUser != ID_NONE)
				pTarget = UserManager()->GetUser(m_pInfo->idUser);
			if (!pTarget)
				pTarget = UserManager()->GetUser(m_pInfo->szTargetName);

			if (pTarget && pTarget->CanBeSee()/* && pTarget->GetMapID() == pUser->GetMapID()*/)
			{
				CMsgPlayDetil msg;
				IF_OK (msg.Create(pTarget))
					pUser->SendMsg(&msg);
			}
			else
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է�����̫Զ������");
			}
		}
		break;
	case actionQueryJinmeiInfo:
		{
			CUser* pTargetUser = NULL;
			if (m_pInfo->idUser != ID_NONE && m_pInfo->idUser != pUser->GetID())
			{
				pTargetUser = UserManager()->GetUser(m_pInfo->idUser);
				if (NULL == pTargetUser)
				{
					return;
				}
			}

			UserJinmeiSysMgr()->SendJinmeiSysInfo(pUser->GetID(), pTargetUser);
		}
		break;
	case actionUpJinmeiLev:
		{
			if (m_pInfo->unPosX <= USERJINMEITYPE_UNKOWN || m_pInfo->unPosX > USERJINMEITYPE_RENMEI)
			{
				return;
			}

			UserJinmeiSysMgr()->UpJinmeiLevByType(m_pInfo->unPosX, UPJINMEILEV_TIME, pUser->GetID());
		}
		break;
	case actionUpjinmeiLevQuick:
		{
			if (m_pInfo->unPosX <= USERJINMEITYPE_UNKOWN || m_pInfo->unPosX > USERJINMEITYPE_RENMEI)
			{
				return;
			}

			UserJinmeiSysMgr()->UpJinmeiLevByType(m_pInfo->unPosX, UPJINMEILEV_EMONEY, pUser->GetID());	
		}
		break;
	case actionStartLearnJinmei:
		{
			if (m_pInfo->unPosX <= USERJINMEITYPE_UNKOWN || m_pInfo->unPosX > USERJINMEITYPE_RENMEI)
			{
				return;
			}

			UserJinmeiSysMgr()->StartLearnJinmei(m_pInfo->unPosX, pUser->GetID());			
		}
		break;
	case actionStopLearnJinmei:
		{
			if (m_pInfo->unPosX <= USERJINMEITYPE_UNKOWN || m_pInfo->unPosX > USERJINMEITYPE_RENMEI)
			{
				return;
			}

			UserJinmeiSysMgr()->StopLearnJinmei(m_pInfo->unPosX, pUser->GetID());			
		}
		break;
	case actionStrengthJinmei:
		{
			if (m_pInfo->unPosX <= USERJINMEITYPE_UNKOWN || m_pInfo->unPosX > USERJINMEITYPE_RENMEI)
			{
				return;
			}

			bool bProtect = false;
			if (1 == m_pInfo->unPosY)
			{
				bProtect = true;
			}

			UserJinmeiSysMgr()->StrengthJinmeiSys(m_pInfo->unPosX, bProtect, pUser->GetID());
		}
		break;
	case actionAddJinmeiWaitQueue:
		{
			if (m_pInfo->unPosX <= USERJINMEITYPE_UNKOWN || m_pInfo->unPosX > USERJINMEITYPE_RENMEI)
			{
				return;
			}

			UserJinmeiSysMgr()->AddWaitJinmeiType(m_pInfo->unPosX, pUser->GetID());			
		}
		break;
	case actionMoveJinmeiWaitQueue:
		{
			if (m_pInfo->unPosX <= USERJINMEITYPE_UNKOWN || m_pInfo->unPosX > USERJINMEITYPE_RENMEI)
			{
				return;
			}

			UserJinmeiSysMgr()->MoveWaitJinmeiType(m_pInfo->unPosX, pUser->GetID());			
		}
		break;
	case actionAddJinmeiValue:
		{
			const int ADD_JINMEI_VALUE = 1005;
			bool bRet = pUser->ProcessAction(ADD_JINMEI_VALUE);
			m_pInfo->idUser = pUser->GetID();
			if (bRet)
			{
				m_pInfo->unPosX = 1;
			}
			else
			{
				m_pInfo->unPosX = 0;
			}

			pUser->SendMsg(this);
		}
		break;
	case actionUseAllZhenyuanDan:
		{
			const int TYPE_ZHENYUANDAN = 381001;
			const int MAX_SIZE = 120;
			int nItemAmount = 0;
			int nItemSize = MAX_SIZE;
			OBJID sourceItemID[MAX_SIZE] = {0};
	     	nItemAmount = pUser->GetItemByTypeMulti(TYPE_ZHENYUANDAN, nItemSize, sourceItemID);
			if (nItemAmount <= 0)
			{
				return;
			}
			
			int nAddValue = ItemType()->QueryItemType(TYPE_ZHENYUANDAN)->GetInt(ITEMTYPEDATA_BASE1_INIT);
			if (pUser->DelItemByTypeMulti(sourceItemID, nItemSize, nItemAmount))
            {
				pUser->IncJinmeiValue(nAddValue * nItemAmount); 
			}
		}   
		break;
	case actionEudemonBornAdd:
		{
			CTeam* pTeam = pUser->GetTeam();
			if(!pTeam)
				return;
			pTeam->EudemonBronChange(pUser->GetID(),m_pInfo->idTarget);
		}
		break;
	case actionEudemonBornLock:
		{
			CTeam* pTeam = pUser->GetTeam();
			if(!pTeam)
				return;
			pTeam->EudemonBronChange(pUser->GetID(),true);
		}
		break;
	case actionGetEquipItemSet:
		break;
	case actionEudemonBornCancel:
		{
			CTeam* pTeam = pUser->GetTeam();
			if(!pTeam)
				return;
			pTeam->EudemonCancel();
		}
		break;
	case actionEudemonBorn:
		{
			OBJID idUser1 = m_pInfo->idUser;
			OBJID idUser2 = m_pInfo->dwTimeStamp;
			OBJID idEudemon1 = m_pInfo->idTarget;
			OBJID idEudemon2 = m_pInfo->idNpc;
			if(idUser1 == idUser2)//����
			{
				CUser* pThisUser = UserManager()->GetUser(idUser1);
				if(!pThisUser)
					return;
				
				m_pInfo->idTarget = 0;
				if(pThisUser->EudemonBron(1,idEudemon1,idEudemon2,NULL))
				{
					m_pInfo->idTarget = 1;
					pThisUser->SendSysMsg("�ɹ���ʼ��ֳ");
				}
				pThisUser->SendMsg(this);
			}
			else
			{
				CTeam* pTeam = pUser->GetTeam();
				if(!pTeam)
					return;
				pTeam->EudemonBron();
			}
		}
		break;
	case actionEudemonBaby:
		{		
			OBJID idEudemon = m_pInfo->idTarget;
			OBJID idItem = ID_NONE;
			if(pUser->EudemonBaby(idEudemon))
			{
				pUser->SendSysMsg("��ͯ�ɹ�");					
			}
			pUser->SendMsg(this);
		}
		break;
	case actionEudemonUnderUp:
		{		
			OBJID idEudemon = m_pInfo->idTarget;
			OBJID idItem = ID_NONE;
			pUser->EudemonUnderUp(idEudemon);
			pUser->SendMsg(this);
			
		}
		break;
	case actionEudemonMix:
		{		
			OBJID idEudemon = m_pInfo->idTarget;
			OBJID idItem = ID_NONE;
			if(pUser->EudemonMix(idEudemon,m_pInfo->idNpc))
			{
				EudemonData data;
				CMsgEudemon msg;
				data.id = m_pInfo->idNpc;
				if(msg.Create(PETACTION_DelPet,&data,pUser->GetID()))
					SendMsg(&msg);
			}
		}
		break;
	case actionEudemonLean:
		{		
			OBJID idEudemon = m_pInfo->idTarget;
			OBJID idItem = ID_NONE;
			int ignoreSkill = 0;
			if(m_pInfo->idNpc != 0)//ѡ�񶥵�һ������
			{
				ignoreSkill = g_pNpcMagicManager->GetEudSkillIDByType(m_pInfo->idNpc);
				CHECK(ignoreSkill);
			}
			if(sscanf(m_pInfo->szTargetName,"%u",&idItem) == 1)
			{
				pUser->EudemonSkill(idEudemon,idItem,ignoreSkill,true);
			}
		}
		break;
	case actionEudemonKillUp:
		{		
			OBJID idEudemon = m_pInfo->idTarget;
			OBJID idItem = ID_NONE;
			
			pUser->EudemonSkill(idEudemon,0,g_pNpcMagicManager->GetEudSkillIDByType(m_pInfo->idNpc),false);
		}
		break;
	case actionEudemonCall:
		{
			pUser->CallEudemon(m_pInfo->idTarget);
		}
		break;
	case actionEudemonReset:
		{
			pUser->CallBackEudemon(m_pInfo->idTarget);
		}
		break;
	case actionEudemonIntelUp:
		{
			if(m_pInfo->idTarget == ID_NONE)
				return;
			
			OBJID idEudemon = m_pInfo->idTarget;

			CUserPet* pPetSet = pUser->QueryUserPetSet();
			if(pPetSet)
				pPetSet->EudemonIntelUp(idEudemon);		
		}
		break;
	case actionEudemonTranse:
		{
			if(m_pInfo->idTarget == ID_NONE)
				return;

			OBJID idEudemon = m_pInfo->idTarget;

			CUserPet* pPetSet = pUser->QueryUserPetSet();
			if(pPetSet)
				pPetSet->EudemonTransform(idEudemon);
		}
		break;

	case actionEudemonSoul:
		{
			OBJID idEudemon = m_pInfo->idTarget;
			pUser->EudemonSoul(idEudemon);
		}
		break;
	case actionRemoveEudemonSoul:
		{
			pUser->RemoveEudemonSoul();
		}
		break;
	case actionEudemonFusionUp:
		{
			if(m_pInfo->idTarget == ID_NONE)
				return;
			
			OBJID idEudemon = m_pInfo->idTarget;
			
			CUserPet* pPetSet = pUser->QueryUserPetSet();
			if(pPetSet)
				pPetSet->EudemonFusionUp(idEudemon);	
		}
		break;
	case actionEudemonChangeSex:
		{
			if(m_pInfo->idTarget == ID_NONE)
				return;
			
			OBJID idEudemon = m_pInfo->idTarget;
			
			CUserPet* pPetSet = pUser->QueryUserPetSet();
			if(pPetSet)
				pPetSet->EudemonChangeSex(idEudemon);
		}
		break;
	case actionTaskSelectItem:
		{
			pUser->SetTaskSelect(m_pInfo->dwData);
		}
		break;
	case actionTaskAnswerOk:
		{
			if(m_pInfo->dwData >= 1 && m_pInfo->dwData <= 8)
			{
				if(!pUser->ChkNewGiveMask(40 + m_pInfo->dwData))
				{
					pUser->ProcessAction(3000 + m_pInfo->dwData);
					pUser->AddNewGiveMask(40 + m_pInfo->dwData);
				}
			}
		}
		break;
	case actionFlyMapByItem:
		{
			if (!pUser->GetMap())
			{
				return;
			}
			if(pUser->GetMap()->isDisableLeave())//С��Ь�ڲ���ʹ�õĵ�ͼ������������
				return;
			if (pUser->GetMap()->isDisableFlyShoe())
			{
				pUser->SendSysMsg("�õ�ͼ����ʹ��С��Ь");
				return;
			}
			if (pUser->GetMap()->IsPkGameMap())
			{ 
				pUser->SendSysMsg("�õ�ͼ����ʹ��С��Ь");
				return;
			}
			if(!pUser->IsVipActive())
			{
				CItem* pItem = pUser->GetItemByType(630006); //С��Ь
				if(!pItem)
					return;

				if(!pUser->SpendItem(pItem))
					return;
			}
			
			
			pUser->FlyMap(m_pInfo->unDir,m_pInfo->unPosX,m_pInfo->unPosY);
		}
		break;
	case actionChangeFace:
		{
			int nFace = m_pInfo->unDir;
			CItem* pItem = pUser->GetItemByType(630021);//���ݾ�
			if (!pItem)
			{
				return;
			}
			if (pUser->GetSex() == 0 && (nFace < 60  && nFace > 19 ))
			{
				pUser->SendSysMsg("���ܱ���");
				return;
			}
			else	if (pUser->GetSex() == 1 && (nFace <  100 && nFace > 59))
			{
				pUser->SendSysMsg("���ܱ���");
				return;
			}
			if (!pUser->SpendItem(pItem))
			{
				return;
			}

			pUser->SetVipface(nFace);
		}
	case actionQueryEudemon:
		{
			EudemonData data;
			CMsgEudemonInfo msgEudemon;	
			pUser->LoadEudemon(m_pInfo->dwData,data);
			if(msgEudemon.Create(data))
				pUser->SendMsg(&msgEudemon);
		}
		break;
// 	case actionQueryPlayer:
// 		{
// 			if (pUser)
// 			{
// 				IRole* pRole = pUser->FindAroundRole(m_pInfo->idTarget);
// 				if (pRole)
// 				{
// 					if (pRole->GetDistance(pUser->GetPosX(), pUser->GetPosY()) <= CELLS_PER_VIEW)
// 					{
// 						pRole->QueryMapThing()->SendShow(pUser->QueryRole());
// 					}
// 				}
// 				if(pUser->IsGM())
// 				{
// 					pUser->SendSysMsg("actionQueryPlayer: [%d]", m_pInfo->idTarget);
// 					if(pRole)
// 						pUser->SendSysMsg("target: [%s][%d], pos: [%d][%d]", pRole->GetName(), pRole->GetID(), pRole->GetPosX(), pRole->GetPosY());
// #ifdef	PALED_DEBUG
// 					LOGERROR("actionQueryPlayer: [%d]", m_pInfo->idTarget);
// 					if(pRole)
// 					{
// 						LOGERROR("target: [%s][%d], pos: [%d][%d]", pRole->GetName(), pRole->GetID(), pRole->GetPosX(), pRole->GetPosY());
// 					}
// 					//ASSERT(!"actionQueryPlayer");
// #endif
// 				}
// 			}
// 		}
// 		break;
	case actionEnterMap:			// 1
		{
			if(!pUser->IsLoaded())//�Ѿ�֪ͨ�����˻�Ҫ�����ͼ!!
			{
				::LogSave("User enter map without data");
				return;
			}
			if (!pUser->IsChangeLine())
			{
				OBJID idMapDoc = ID_NONE;
				CGameMap* pMap=MapManager()->QueryMap(pUser->GetMapID());

				if(pMap)
				{
					idMapDoc = pMap->GetDocID();

					pUser->UpdateCalculatedProperty(); //����

					CMsgAction	msg;
					if(msg.Create(pUser->GetMapID(), pUser->GetPosX(), pUser->GetPosY(), pUser->GetDir(), actionEnterMap, idMapDoc))
						SendMsg(&msg);		// can't send to other user, because have not user id.

				}
				else
				{
					LOGERROR("Error: invalid map id[%u] of user:%u,name:%s", pUser->GetMapID(), pUser->GetID(),pUser->GetName());
				}
			}
			else
				LOGWARNING("Warning: ����ͻ�������,���ͻ���û����,UserName=[%s],UserId=%u",pUser->GetName(),pUser->GetID());
		}	
		break;
	case actionClientInitOK:			// 1
		{
			DEBUG_TRY
			OBJID idMapDoc = ID_NONE;
			CGameMap* pMap=MapManager()->QueryMap(pUser->GetMapID());
			IF_OK (pMap)
			{
				pUser->EnterMap();
				pUser->SendMsg(this);
				pUser->SendJinmeiSysLogInOKInfo();
				if(pUser->GetTutorManager())
					pUser->GetTutorManager()->SendTutorOnLineInfo();
				pUser->CheckVerificationState();//�޸�����״̬

			}
			DEBUG_CATCH("actionClientInitOK");
		}
		break;
	case actionGetSecInfo:
		{
			DEBUG_TRY
			if(m_pInfo->iData >=0 && m_pInfo->iData <=2)
				pUser->SendSecAtt(m_pInfo->iData);
			DEBUG_CATCH("actionGetSecInfo");
		}
		break;
	case actionUpUserlev:
		{
			if(m_pInfo->unDir >=0 && m_pInfo->unDir <=2)
				pUser->IncLev(1,m_pInfo->unDir);
		}
		break;
	case actionAddPoint:
		{
			if(m_pInfo->unDir > 0 && m_pInfo->unDir < 13)
			{
				if (m_pInfo->iData >= 0 && m_pInfo->iData < 1000)//goto ֻ���ܵ�����0��1000֮��,�ͻ�����������������ʱ����0
					pUser->AllotPoint(m_pInfo->iData,m_pInfo->unDir);
			}
		}
		break;
	case actionAddEudemonPoint:
		{
			if(m_pInfo->unDir > 0 && m_pInfo->unDir < 7)
				pUser->AllotEudemonPoint(m_pInfo->idTarget,m_pInfo->unDir,m_pInfo->idNpc);
		}
		break;
	case  actionActiveNPC:
		{
			CNpc* pNpc;
			int idnpc=m_pInfo->idUser;
// 			if (m_pInfo->idUser >= 9046 && m_pInfo->idUser <=9050)
// 			{
// 				idnpc=NpcManager()->QueryNpcIDByTypeMapID(m_pInfo->idUser,pUser->GetMapID());
// 				IRole* pRole = RoleManager()->QuerySet()->GetObj(idnpc);
// 				if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)))
// 				{
// 					m_pInfo->idUser=idnpc;
// 					pUser->SendMsg(this);
// 					pNpc->ActivateNpc(pUser->QueryRole(), 0);
// 					pUser->SetClickNpcID(idnpc);
// 				}
// 			}
// 			else
// 			{
				
				IRole* pRole = RoleManager()->QuerySet()->GetObj(idnpc);
				if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pUser->GetMapID() == pNpc->GetMapID())
				{
					pUser->SendMsg(this);
					pNpc->ActivateNpc(pUser->QueryRole(), 0);
					pUser->SetClickNpcID(idnpc);
				}
//			}
		}
		break;
	case  actionAcceptTask:
		{
			CTaskSetRandData* pSetData = TaskSetRand()->GetDataStruct(m_pInfo->iData, m_pInfo->unDir);
			if(!pSetData)
				return;
// 			OBJID idIndex = pUser->GetTaskIndex(m_pInfo->iData);
// 			if(idIndex != 1)
// 				return;
			if (pSetData->MustSpendEne())//����ľ����ӵ�����
			{
				CHECK(m_pInfo->idNpc);
			}
			if (pSetData->IsNormalTask())//��ͨ����,����ʹ�þ�����ȡ
			{
				CHECK(m_pInfo->idNpc == 0);
			}
			LuaScriptMachine()->InitVar(m_pInfo->idNpc);//�������������������ҵ�ѡ����ű�
			pUser->ProcessAction(pSetData->TaskData.ReceiveAction);
		}
		break;
	case  actionGiveupTask:
		{
			CTaskSetRandData* pSetData = TaskSetRand()->GetDataStruct(m_pInfo->iData, m_pInfo->unDir);
			if(!pSetData)
				return;

			OBJID idIndex = pUser->GetTaskIndex(m_pInfo->iData);
			if(idIndex != 1)
				return;
			if (pSetData->TaskData.taskid == 29)//����
				pUser->VerificationGivepupTask();
			pUser->ProcessAction(pSetData->TaskData.GiveupAction);
		}
		break;
	case  actionEndTask:
		{
			CTaskSetRandData* pSetData = TaskSetRand()->GetDataStruct(m_pInfo->iData, m_pInfo->unDir);
			if(!pSetData)
				return;
			OBJID idIndex = pUser->GetTaskIndex(m_pInfo->iData);
			if(idIndex != 1)
				return;

			if(m_pInfo->idNpc != ID_NONE)
			{
				if(pUser->IsLockedEudemon(m_pInfo->idNpc))
					return;
				if(pUser->m_idSoulEudemon == m_pInfo->idNpc || pUser->GetCallPetID() == m_pInfo->idNpc)//��Ȼ�ж��������ټ�һ��ȷ����ȫ
					return;
				pUser->SetLastTaskEudemon(m_pInfo->idNpc);
			}
			LuaScriptMachine()->InitVar(0);//�������������������ҵ�ѡ����ű�
			if (pSetData->CanSpendEquip())//�ж������Ƿ������ύװ��
			{
				if (m_pInfo->dwTimeStamp == ID_NONE)//�潻���ύװ��
					pUser->ProcessAction(pSetData->TaskData.CompleteAction);
				else
				{
					CItem* pItem = pUser->GetItem(m_pInfo->dwTimeStamp, false);
					if (pItem && pItem->IsEquipment())//�ж�һ���ǲ���װ��,����ʲô��Ʒ�����Խ���
					{
						LuaScriptMachine()->InitVar(m_pInfo->dwTimeStamp);//�������������������ҵ�ѡ����ű�
						pUser->ProcessActionIncludeItem(pSetData->TaskData.CompleteAction, pItem);
					}
				}
			}
			else
				pUser->ProcessAction(pSetData->TaskData.CompleteAction);
		}
		break;
	case  actionEudemonChgName:
		{
			if(!pUser->EudemonChgName(m_pInfo->idTarget,m_pInfo->szTargetName))
			{
				this->Create(m_pInfo->idTarget,0,0,0,actionEudemonError);
				pUser->SendMsg(this);
			}
		}
		break;
	case  actionEudemonEat:
		{
			pUser->EudemonEat(m_pInfo->idTarget);
		}
		break;
	case  actionQueryLeaveExp:
		{
			DWORD dwLeaveExp = 0;
			dwLeaveExp = pUser->QueryLeaveExp();

			CMsgAction	msg;
			if(msg.Create(pUser->GetID(), 0, 0, 0, actionQueryLeaveExp, dwLeaveExp, 0))
				SendMsg(&msg);
		}
		break;
	case  actionGetLeaveExp:
		{
			DWORD dwLeaveExp = 0;
			CMsgAction	msg;

			if(pUser->GetLeaveExp(m_pInfo->dwData))
			{
				if(msg.Create(pUser->GetID(), 0, 0, 0, actionGetLeaveExpSuc, dwLeaveExp, 0))
					SendMsg(&msg);
			}
//				pUser->SendSysMsg("��ȡ�ɹ�");
			else
			{
				if(msg.Create(pUser->GetID(), 0, 0, 0, actionGetLeaveExpFai, dwLeaveExp, 0))
					SendMsg(&msg);
//				pUser->SendSysMsg("��ȡʧ��");
			}
		}
		break;

	case actionGift:
		{
			pUser->ProcessOnlineGift();		
		}
		break;
	case actionQueryGift:
		{
			if(pUser->IsGive(0))
			{
				CMsgAction	msg;
				msg.Create(0,actionQueryGift,0);
				pUser->SendMsg(&msg);
			}
			if(pUser->IsGive(1))
			{
				CMsgAction	msg;
				msg.Create(1,actionQueryGift,1);
				pUser->SendMsg(&msg);
			}
			if(pUser->IsGive(2))
			{
				CMsgAction	msg;
				msg.Create(2,actionQueryGift,2);
				pUser->SendMsg(&msg);
			}
		}
		break;
	case actionAcceptGift:
		{
			pUser->GiveOnlineGift();
		}
		break;
	case  actionLifeSpanadd:
		{
			pUser->EudemonLifeSpanadd(m_pInfo->idTarget);
		}
		break;
	case  actionEudemonHappy:
		{
			pUser->EudemonTrian(m_pInfo->idTarget);
		}
		break;
	case  actionEudemonGiveup:
		{
			pUser->EudemonGiveup(m_pInfo->idTarget);
		}
		break;
	case actionGetItemSet:	
		{
			int amount = pUser->SendItemSet();
	
			CMsgAction	msg;
			if(msg.Create(pUser->GetID(), 0, 0, 0, actionGetItemSet, amount, 0))
				SendMsg(&msg);
		}
		break;
	case actionGetGoodFriend:		// 3
		{
			if(!pUser)
				break;
			
			pUser->SendGoodFriend();

			LeaveWord()->ShowWords(pUser);
			//goto ���Ͷ���λ��
			if (pUser->GetTeam())
			{
				pUser->SendPosToTeamMem(pUser->GetMapID(), pUser->GetPosX(), pUser->GetPosY());
				pUser->GetPosFromTeamMem();
			}
		}
		break;
	case actionJump:
		{
			if(!pUser)
				break;

			CMonster* pPet = pUser->QueryCallPet();
			//IRole* pRole	= RoleManager()->QueryRole(this, m_pInfo->idUser);

			if(pPet==NULL)
			{
				CMsgAction msgAction;
				if (msgAction.Create(pUser->GetID(), 0, 0, 0, actionNoEudemon))
				{
					pUser->SendMsg(&msgAction);
				}
				
				LogSave("actionJump pPet==NULL! userid:%d",pUser->GetID());
				return;
			}
// 			CHECK(pPet);
			CHECK(pPet->GetMap());

			m_pInfo->unPosX = pUser->GetPosX();
			m_pInfo->unPosY = pUser->GetPosY();
			if(pPet->GetDistance(m_pInfo->unPosX, m_pInfo->unPosY) <= 4)
			{
				return;
			}
		
			pPet->BroadcastRoomMsg(this, INCLUDE_SELF);
			

			if(!pPet->GetMap()->IsValidPoint(m_pInfo->unPosX, m_pInfo->unPosY))
				return;

		
			pPet->ProcessOnMove(MOVEMODE_TRANS);
			pPet->TransPos(m_pInfo->unPosX, m_pInfo->unPosY);
 		
		}
		break;
	case actionSynchro:	
		break;

// 	case actionReborn:
// 		{
// 			if(pUser && !pUser->IsAlive())
// 			{
// 				pUser->Reborn();
// 			}
// 		}
// 		break;
	case actionRebornUseScroll://ʹ�ø������
		{
			bool bUse = false;
			if(m_pInfo->dwData > 0)
				bUse = true;
			pUser->RebornUseScroll(bUse);
		}
		break;
	case actionRebornWithOutScroll:// 30���������3���ԭ�ظ���,���ø����
		{
			bool bUse = false;
			if(m_pInfo->dwData > 0)
				bUse = true;
			pUser->RebornUseScroll(bUse);
		}
		break;

	case atcionPkProcRed:   //��������
	case actionPkProcWhile: //��������
	case atcionPkProcSyn:
		{
			pUser->SetPkProc(m_pInfo->unPosX);
			CMsgAction	msg;
			IF_OK(msg.Create(pUser->GetID(), m_pInfo->unPosX, 0, 0, atcionPkProcRed, 0, 0))			
			pUser->SendMsg(&msg);
		}
		break;
	case actionQueryFriendInfo:
		{
			CFriend* pFriend = pUser->GetFriend(m_pInfo->idTarget);
			if(pFriend)
			{
				CUser* pTarget = UserManager()->GetUser(m_pInfo->idTarget);
				if(pTarget)
				{
					CMsgFriendInfo	msg;
					IF_OK(msg.Create(_FRIEND_INFO, pTarget->GetID(), pTarget->GetLookFace(), pTarget->GetLev(), 
							pTarget->GetProfession(), pTarget->GetPk(), pTarget->GetSynID(), pTarget->GetSynRankShow(), 
							/*pTarget->GetMate()*/"111", pTarget->GetNobilityRank()))
						pUser->SendMsg(&msg);
				}
				else
					MapGroup(m_idProcess)->QueryIntraMsg()->TransmitMsg(this, GetSocketID(), GetNpcID());
			}
		}
		break;
	case actionQueryLeaveWord:
		{
			if(pUser->IsMoreLeaveWord())
			{
				LeaveWord()->ShowWords(pUser);
			}
		}
		break;
	case actionCreateBooth:
		{
			if (pUser->QueryStatus(STATUS_PVPMODE))
			{
				pUser->SendSysMsg("�д�״̬���ܰ�̯��");
				return;
			}

			if (pUser->IsItemLockedOpen())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "���ȹر��˺Ű�ȫ��");
				return;
			}

			if(pUser->QuerySquareDeal())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "�����в��ܰ�̯");
				return;
			}
			CBooth*	pBooth=NULL;
			if( pUser->GetMap()->CheckBooth( /*m_pInfo->unPosX*/pUser->GetPosX(),/*m_pInfo->unPosY*/pUser->GetPosY(),OBJ_BOOTH,IPP_OF(pBooth) ) )
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "����˵�̯λ̫����");// "����˵�̯λ̫����"
				m_pInfo->idNpc = NULL;
				pUser->SendMsg(this);
				return;
			}
// 				int posX = pUser->GetPosX();
// 				int posY = pUser->GetPosY();
// 				CGameMap* pMap = MapManager()->QueryMap(pUser->GetMapID());
// 				CHECK(pMap);
// 				if(!pMap->IsMarketRegion(posX,posY))
// 				{
// 					LOGERROR( "MsgAction::case actionCreateBooth����,��̯����Ƿ�.�û�:%s mapID:%d x:%d y:%d",
// 						pUser->GetName(),
// 						pUser->GetMapID(),
// 						posX,posY );
// 					pUser->SendSysMsg(g_objStrRes.GetStr(15084));//"�㲻Ӧ�ÿ����������.�����Ѿ���¼.�ǰ�̯����!"
// 					m_pInfo->dwData = NULL;
// 					pUser->SendMsg(this);
// 					return;
// 				}
			if(pUser->CreateBooth(pUser->GetPosX() , pUser->GetPosY(), /*m_pInfo->unDir*/6))
			{
				m_pInfo->idNpc = pUser->QueryBooth()->GetID();
				pUser->SendMsg(this);
			}
			else
				return;

			pUser->QueryBooth()->EnterMap(pUser->GetPosX(), pUser->GetPosY(), 6);
		}
		break;
// 	case actionSuspendBooth:
// 		{
// 			if(pUser && pUser->QueryBooth())
// 			{
// 				pUser->QueryBooth()->LeaveMap();
// 				pUser->DestroyBooth();
// 			}
// 		}
// 		break;
	case actionResumeBooth:
		{
			if(pUser && pUser->QueryBooth())
			{
				pUser->QueryBooth()->EnterMap(m_pInfo->unPosX, m_pInfo->unPosY, m_pInfo->unDir);
				m_pInfo->dwData = pUser->QueryBooth()->GetID();
				pUser->SendMsg(this);
			}
		}
		break;
	case actionDestroyBooth:
		{
			if(pUser && pUser->QueryBooth())
			{
				pUser->QueryBooth()->LeaveMap();
				pUser->DestroyBooth();
			}
		}
		break;
	case actionSetBooth:
		{
			if (pUser)
			{
				pUser->ClearBooth();
			}
		}
		break;
	case actionQueryCryOut:
		{
			CUser* pTarget = UserManager()->GetUser(m_pInfo->idTarget);
			if(!pTarget)
				return ;

			if(pUser && pTarget->QueryBooth())
				pTarget->QueryBooth()->SendCryOut(pUser);
		}
		break;
	case actionQueryBuffAdd:
		{
			if(!pUser)
				break;	
			
			pUser->SendAllBuff();
			pUser->CheckVerificationState();//�޸�����״̬
//modify code by �ֵ�Ԩ begin 2011.6.22
			IStatus* pStatus1 = pUser->QueryStatus(STATUS_RETORT_MDSX);
			if( pStatus1 )
			{
				pUser->SetNewValue(pStatus1->GetParam1());
				pUser->SetNewValueTrageid(pStatus1->GetParam2());
				pUser->AttachStatus(pUser,STATUS_RETORT_MDSX_1, 0, 2, 0,0,0,0,pUser->GetID());
			}
//modify code by �ֵ�Ԩ end
		}
		break;
	case actionQueryMoneyAdd:
		{
			if(!pUser)
				break;

			CMsgUserAttrib	msg;//�ƶ��ٶ��Լ�������
			if(msg.Create(pUser->GetID(), _USERATTRIB_SPEED, pUser->AdjustSpeed(pUser->GetSpeed())))
			{
				if (pUser->GetHpAdd()>0 || pUser->GetMpAdd()>0 || pUser->GetPetAdd()>0)
				{
					msg.Append(_USERATTRIB_DRUG_ADDHP, pUser->GetHpAdd());
					msg.Append(_USERATTRIB_DRUG_ADDMP, pUser->GetMpAdd());
					msg.Append(_USERATTRIB_DRUG_ADDPET, pUser->GetPetAdd());
				}
				pUser->SendMsg(&msg);
			}			
			
			CMsgAction	msgAction;
			if(msgAction.Create(pUser->GetID(), 0, 0, 0, actionQueryMoneyAdd, 0, 0))
				pUser->SendMsg(&msgAction);
		}
		break;
	case actionQueryMissList:
		{
			if(!pUser)
				break;
			
			CMsgAction	msg;
			if(msg.Create(pUser->GetID(), 0, 0, 0, actionGetMagicSet, 0, 0))
				pUser->SendMsg(&msg);
			
			int amount = pUser->sendMissionMsg();
			
			if(msg.Create(pUser->GetID(), 0, 0, 0, actionQueryMissList, amount, 0))
				pUser->SendMsg(&msg);
		}
		break;
	case actionQueryCDList:
		{
			if(!pUser)
				break;
			
			CMsgAction	msg;
			if(msg.Create(pUser->GetID(), 0, 0, 0, actionGetMagicSet, 0, 0))
				pUser->SendMsg(&msg);

			int amount = pUser->SendAllCD();

			if(msg.Create(pUser->GetID(), 0, 0, 0, actionQueryCDList, amount, 0))
				pUser->SendMsg(&msg);
		}
		break;
	case actionQueryUserkeyList:
		{
			if(!pUser)
				break;
			//goto ���Ͱ�ȫ����������
			CMsgAccLock msgLock;
			if (msgLock.Create(pUser->GetID(), _MSGACCLOCK_KEY, 0, NULL, (char*)_MSGACCLOCK_KEYCHAR))
				pUser->SendMsg(&msgLock);

			pUser->SendAllUserkey();

			CMsgAction	msg;
			if(msg.Create(pUser->GetID(), 0, 0, 0, actionQueryUserkeyList, 2, 0))
				pUser->SendMsg(&msg);
		}
		break;
	case actionQueryAuto:
		{
			if(!pUser)
				break;
			
			pUser->m_pAutoFightData.AutoFightReadDB(pUser);
			
			CMsgAction	msg;
			if(msg.Create(pUser->GetID(), 0, 0, 0, actionQueryAuto, 2, 0))
				pUser->SendMsg(&msg);
		}
		break;
	case actionQuerySkillList:
		{
			if(!pUser)
				break;
			
			CMsgAction	msg;
			if(msg.Create(pUser->GetID(), 0, 0, 0, actionGetMagicSet, 0, 0))
				pUser->SendMsg(&msg);

			int amount = pUser->SendAllMagicInfo();

			if(msg.Create(pUser->GetID(), 0, 0, 0, actionQuerySkillList, amount, 0))
				pUser->SendMsg(&msg);
		}
		break;
	case actionQueryEudemonList:
		{
			if(!pUser)
				break;
			
			int amount = pUser->SendEudemonList();
			pUser->SendPeifangList();
			
			CMsgAction	msg;
			if(msg.Create(pUser->GetID(), 0, 0, 0, actionQueryEudemonList, amount, 0))
				pUser->SendMsg(&msg);
		}
		break;
// 	case actionAbortTransform:
// 		{
// 			if(pUser)
// 				pUser->AbortTransform();
// 		}
// 		break;
//	case actionTakeOff:
//		{
//			if(pUser)
//				pUser->QueryStatusSet()->DelObj(STATUS_WING);
//		}
//		break;
//	case actionCancelKeepBow:
//		{
//			if(pUser)
//				pUser->QueryStatusSet()->DelObj(STATUS_KEEPBOW);
//		}
//		break;
// 	case actionQueryMedal:
// 	case actionDelMedal:
// 	case actionSelectMedal:
// 	case actionQueryHonorTitle:
// 	case actionDelHonorTitle:
// 	case actionSelectHonorTitle:
// 		{
// 			if(pUser)
// 				pUser->ProcessMsgAction(m_pInfo->usAction, m_pInfo->dwData, m_pInfo->idUser);
// 		}
// 		break;
// 
// 		// for ai server //////////////////////////////////////////////////////////////


	case actionQuerySynInfo:
		{
			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->dwData);
			if(pSyn)
			{
				CMsgSynInfo	msg;
				IF_OK(msg.Create(pSyn))
					pUser->SendMsg(&msg);
			}
		}
		break;
	// �����ǵ�ʦϵͳ���
// 	case actionStudentApply:  //������ͽ 
// 		{
// 			CTeam* pTeam = pUser->GetTeam();
// 			if (!pTeam || pTeam->GetLeader()!=pUser->GetID())
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "�������Ҫ�յ�ͽ�����ǰ��");
// 				return;
// 			}
// 			if (pTeam->GetMemberAmount()>2)
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "ÿ��ֻ�ܴ�һ��ͽ�������а�ʦ��ʽŶ");
// 				return;
// 			}
// 			// find target
// 			CUser* pTarget = NULL;
// 			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, pTeam->GetTeamMemberOnly(), IPP_OF(pTarget)))
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "�Է��Ѳ�����ĸ���");
// 				return;
// 			}
// 			if (pUser->GetLev() < _MIN_TUTOR_LEVEL)
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�㹦����ǳ����������60������������ͽ��");//STR_LOW_LEVEL
// 				return;
// 			}
// 
// 			//  [�½���10/25/2007]
// 			if (pUser->IsStudentFull())
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW,"����ڵ�ͽ��̫���ˣ����ܼ�����ͽ");//STR_STUDENT_FULL
// 				return;
// 			}
// 
// 			if (pTarget->GetTutor())
// 			{
// 				char szTemp[128] = {0};
// 				sprintf(szTemp, "%s�Ѿ���ʦ����", pTarget->GetName());
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, szTemp);//STR_YOUR_STUDENT_ALREADY
// 				return;
// 			}
// 
// 			if (!pTarget->IsAlive())
// 				return;
// 
// 			if(pTarget->GetLev() <10 || pTarget->GetLev()>30)
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "ͽ�ܵȼ���Ҫ��10����30��֮����ܾ��а�ʦ��ʽ");
// 				return;
// 			}
// 
// 			pUser->SetApply(CUser::APPLY_STUDENTAPPLY, pTeam->GetTeamMemberOnly());
// 			pTarget->SendMsg(this);
// 
// // 			pTarget->SendSysMsg(g_objStrRes.GetStr(13066), pUser->GetName());//STR_STUDENT_APPLY
// // 			pUser->SendSysMsg(g_objStrRes.GetStr(13067));//STR_STUDENT_APPLY_SENT
// 		}
// 		break;
// 	case actionAgreeStudentApply: //ͽ����Ӧ��ʦ
// 		{
// 			CTeam* pTeam = pUser->GetTeam();
// 			if (!pTeam)
// 				return;
// 			CUser* pTarget = NULL;
// 			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, pTeam->GetLeader(), IPP_OF(pTarget)))
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է��Ѳ�����ĸ���"); //�Է��Ѳ�����ĸ�����
// 				return;
// 			}
// 			if (m_pInfo->unPosX==2)//ͽ�ܾܾ�
// 			{
// 				char szTemp[128];
// 				sprintf(szTemp, "%s����Ը�����Ϊʦ", pUser->GetName());
// 				pTarget->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
// 				return;
// 			}
// 			pUser->ProcessAction(10022);
			/*
			if( pUser->GetLev() < 10 )
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "���ĵȼ�̫��");//"���ĵȼ�̫��!"
				return;
			}
			if (pUser->GetLev() >= NEWBIE_LEVEL)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "���ĵȼ�����");//STR_HEIGHT_LEV
				return;
			}

			if (pUser->GetTutor())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "���Ѿ���ʦ����"); //���Ѿ���ʦ����
				return;
			}
			
			// find target
			CTeam* pTeam = pUser->GetTeam();
			if (!pTeam)
				return;
			CUser* pTarget = NULL;
			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, pTeam->GetLeader(), IPP_OF(pTarget)))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է��Ѳ�����ĸ���"); //�Է��Ѳ�����ĸ�����
				return;
			}
			if (m_pInfo->unPosX==2)//ͽ�ܾܾ�
			{
				char szTemp[128];
				sprintf(szTemp, "%s����Ը�����Ϊʦ", pUser->GetName());
				pTarget->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
				return;
			}
			if (pUser->IsFirstTutor() && pUser->QueryPackage()->IsPackFull(1,0, ITEMPOSITION_BACKPACK1))
			{
				pTarget->SendSysMsg(_TXTATR_TOPSHOW,"ͽ�ܵı������������ܻ�ð�ʦ����");
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�������������ܻ�ð�ʦ����");
				return;
			}
			if (pTarget->FetchApply(CUser::APPLY_STUDENTAPPLY, pUser->GetID()))
			{
				if (pUser->AddTutor(pTarget->GetID(), pTarget->GetName(), pTarget->GetProfession()))
				{
					// synchronize all statuses
					// 					MSGBUF szMsg;
					// 					sprintf(szMsg, g_objStrRes.GetStr(13073), pTarget->GetName(), pUser->GetName());//STR_MAKE_TEACHERSTUDENT
					// 					pUser->BroadcastRoomMsg(szMsg, true);
				}
				else
					::LogSave("Error: add tutor failed.");
			}
			else
			{
				return;
				//pUser->SendSysMsg(g_objStrRes.GetStr(13071));//STR_NO_STUDENT_APPLY
			}*/
// 		}
// 		break;
// 	case actionTeacherApply:   //goto ʦ����ͽ�ܳ�ʦ idUserΪʦ��
// 		{
// 			CTeam* pTeam = pUser->GetTeam();
// 			if (!pTeam || pTeam->GetLeader()!=pUser->GetID())
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "������ͽ����Ӻ�����");
// 				return;
// 			}
// 			if (pTeam->GetMemberAmount()>2)
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "ÿ��ֻ�ܴ�һ��ͽ�������г�ʦ��ʽŶ");
// 				return;
// 			}
// 			
// 			// find target
// 			OBJID idTarget = pTeam->GetTeamMemberOnly();
// 			CUser* pStudent = NULL;
// 			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, idTarget, IPP_OF(pStudent)))
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "�Է��Ѳ�����ĸ���");
// 				return;
// 			}
// 			CTutorData* pTutor = pStudent->GetTutor();
// 			if (!pTutor || pTutor->GetTutorID()!=pUser->GetID())
// 			{
// 				char szTemp[128];
// 				sprintf(szTemp,"����Ц�ˣ�%s�������ͽ��", pStudent->GetName());
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
// 				return;
// 			}
// 			if (pStudent->GetLev() < GRAUDUATE_LEVEL)
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "���ͽ�ܹ����������Գ�ʦ��������̵�");//STR_LOW_LEV2
// 				return ;
// 			}
// 			
// 			pUser->SetApply(CUser::APPLY_GRADUATE, idTarget);
// 			pStudent->SendMsg(this);
// 		}
// 		break;
// 	case actionTeacherRequest://goto ͽ����Ӧ��ʦ
// 		{
//// 			if( pUser->GetLev() < GRAUDUATE_LEVEL )
//// 			{
//// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "���ĵȼ�̫��");//"���ĵȼ�̫��!"
//// 				return;
//// 			}
//// 			if (!pUser->GetTutor())
//// 			{
//// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "����û��ʦ��");//"���ĵȼ�̫��!"
//// 				return;
//// 			}
			// find target
// 			CTeam* pTeam = pUser->GetTeam();
// 			if (!pTeam)
// 				return;
// 			CUser* pTarget = NULL;
// 			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, pTeam->GetLeader(), IPP_OF(pTarget)))
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է��Ѳ�����ĸ���"); //�Է��Ѳ�����ĸ�����
// 				return;
// 			}
// 			if (m_pInfo->unPosX==2)//ͽ�ܾܾ�
// 			{
// 				pTarget->SendSysMsg(_TXTATR_TOPSHOW, "���ͽ����ʱ�������ʦ");
// 				return;
// 			}
// 			pUser->ProcessAction(10023);
			//pUser->Graduate();
// 		}
// 		break;
// 	case actionAgreeTeacherApply:   //ͬ����˰�ʦ
// 		{
// 			return;//goto ���޴˹���
// 			if (!pUser)
// 				break;
// 
// 			// find target
// 			CUser* pTarget = NULL;
// 			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, m_pInfo->idTarget, IPP_OF(pTarget)))
// 			{
// 				pUser->SendSysMsg(g_objStrRes.GetStr(28308)); //�Է��Ѳ�����ĸ�����
// 				break;
// 			}
// 
// 			if(pTarget->GetLev() >= pUser->GetLev())
// 			{
// 				pUser->SendSysMsg(g_objStrRes.GetStr(28305));  //�Է��������ߣ����ܵ����ͽ��
// 				return ;
// 			}
	
			//  [�½���10/25/2007]
//			if(pUser->GetTutor())
//			{
//				pUser->SendSysMsg("���Ѿ��Ǳ������£����ܳ�Ϊ����ʦ��");//STR_STUDENT_FULL2
//				return;
//			}
// 			if (pUser->IsStudentFull())
// 			{
// 				pUser->SendSysMsg(g_objStrRes.GetStr(13080));//STR_STUDENT_FULL2
// 				return;
// 			}
// 			if (pUser->GetStudent(m_pInfo->idTarget))
// 			{
// 				pUser->SendSysMsg(g_objStrRes.GetStr(13065));//STR_YOUR_STUDENT_ALREADY
// 				return;
// 			}
// 
// 			if (pTarget->FetchApply(CUser::APPLY_TEACHERAPPLY, pUser->GetID()))
// 			{
// 				if (pTarget->AddTutor(pUser/*->GetID(), pUser->GetName(), pUser->GetProfession()*/))
// 				{
// 					// synchronize all statuses
//                     if(pUser->IsStudentFull())
// 					{ 
// 						DEBUG_TRY
// 							if(Announce()->QueryAnnounceDataByOwner(pUser->GetID()))
// 							   Announce()->DeleteAnnounceByUserID(pUser->GetID(),true);
// 						DEBUG_CATCH("DeleteAnnounceByOwer(pUser->GetID()) ERROR");
// 					}
// 					MSGBUF szMsg;
// 					sprintf(szMsg, g_objStrRes.GetStr(13073), pUser->GetName(), pTarget->GetName());//STR_MAKE_TEACHERSTUDENT
// 					pTarget->BroadcastRoomMsg(szMsg, true);
// 				}
// 				else
// 					::LogSave("Error: add tutor failed.");
// 			}
// 			else
// 			{
// 				pUser->SendSysMsg(g_objStrRes.GetStr(13072));//STR_NO_TEACHER_APPLY
// 			}
// 		}
// 		break;
// 	case actionDismissStudent:  //ʦ������ɾ��ѧ��
// 		{
// 			if (pUser->GetTeam())
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "���뿪���鵥��һ��ǰ�����ʦͽ��ϵ");
// 				return;
// 			}
// 			pUser->DelStudentByTutor(m_pInfo->idTarget);
// 		}
// 		break;
// 	case actionLeaveTeacher:  //ͽ��ɾ��ʦ��
// 		{
// 			return;
// 			if (pUser->GetTeam())
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "���뿪���鵥��һ��ǰ�����ʦͽ��ϵ");
// 				return;
// 			}
// 			if (!pUser->GetTutor())
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "����Ц�ˣ���û��δ��ʦ��ʦͽ��ϵ");
// 				return;
// 			}
// 			pUser->DelTutor();
// 		}
// 		break;
// 	case actionTeamLeaveTeacher://goto Э����ʦͽ��ϵ ʦ������
// 		{
// 			CTeam* pTeam = pUser->GetTeam();
// 			if (!pTeam || pTeam->GetLeader()!=pUser->GetID())
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "������ͽ����Ӻ�����");
// 				return;
// 			}
// 			if (pTeam->GetMemberAmount()>2)
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "ÿ��ֻ�ܺ�һ��ͽ�ܽ��ʦͽ��ϵ");
// 				return;
// 			}
// 			// find target
// 			CUser* pTarget = NULL;
// 			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, pTeam->GetTeamMemberOnly(), IPP_OF(pTarget)))
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "�Է��Ѳ�����ĸ���");
// 				return;
// 			}
// 			
// 			CTutorData* pTutor = pTarget->GetTutor();
// 			if (!pTutor || pTutor->GetTutorID()!=pUser->GetID())
// 			{
// 				char szTemp[128];
// 				sprintf(szTemp,"����Ц�ˣ�%s�������ͽ��", pTarget->GetName());
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
// 				return;
// 			}
// 			if (pTutor->IsGraduated())
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "һ��Ϊʦ����Ϊ�����Ѿ���ʦ��ͽ�ܲ��ܽ��ʦͽ��ϵ");
// 				return;
// 			}
// 			if (!pTarget->IsAlive())
// 				return;
// 			
// 			pUser->SetApply(CUser::APPLY_LEAVETEACHER, pTeam->GetTeamMemberOnly());
// 			pTarget->SendMsg(this);
// 		}
// 		break;
// 	case actionStudentLeave://goto ͽ����Ӧ���ʦͽ��ϵ
// 		{
// 			if (!pUser->GetTutor())
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "����û��ʦ��");
// 				return;
// 			}
// 			// find target
// 			CTeam* pTeam = pUser->GetTeam();
// 			if (!pTeam)
// 				return;
// 			CUser* pTarget = NULL;
// 			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, pTeam->GetLeader(), IPP_OF(pTarget)))
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է��Ѳ�����ĸ���"); //�Է��Ѳ�����ĸ�����
// 				return;
// 			}
// 			if (m_pInfo->unPosX==2)//ͽ�ܾܾ�
// 			{
// 				char szTemp[128];
// 				sprintf(szTemp, "���ͽ��%s��Ը��������ʦͽ��ϵ", pUser->GetName());
// 				pTarget->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
// 				return;
// 			}
// 			if (pTarget->FetchApply(CUser::APPLY_LEAVETEACHER, pUser->GetID()))
// 			{
// 				if (pTarget->TutorDivorce(pUser->GetID()))
// 				{
// 					char szTemp[128];
// 					sprintf(szTemp, "����%s��ʦͽ��ϵ�Ѿ����",pUser->GetName());
// 					pTarget->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
// 
// 					memset(szTemp, 0L, strlen(szTemp));
// 					sprintf(szTemp, "����%s��ʦͽ��ϵ�Ѿ����",pTarget->GetName());
// 					pUser->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
// 				}
// 			}
// 		}
// 		break;
// 	case actionCancelDisStudent:  //ȡ��ɾ��ѧ��
// 		{
// 			if (!pUser)
// 				break;
// 
// 			pUser->CancelDelStudent(m_pInfo->idTarget);
// 		}
// 		break;

// 	case actionCancelLevTeacher:  //ȡ��ɾ����ʦ
// 		{
// 			if (!pUser)
// 				break;
// 			
// 			pUser->CancelLeaveTeach();
// 		}
// 		break;

// 	case actionAgrResortStudent:   //ͬ��ָ�ѧ��
// 		{
// 			pUser->RecoverStudent(m_pInfo->idTarget);
// 		}
// 		break;

// 	case actionAgrResortTeacher:   //ͬ��ָ���ʦ 
// 		{
// 			pUser->RecoverTutor();
// 		}
// 		break;
	case actionClearLockTarget:
		break;
		//---------------------------
		//20070602����:�˳�����--------------

// 	case actionDoScript: //���н����ű�
// 		{
// 			DWORD nIndex = m_pInfo->dwData%10;
// 			DWORD dwAction = m_pInfo->dwData/10;
// 			DWORD idAction = TaskSetRand()->GetActionId(dwAction);
// 
//             if(idAction)
// 			{
// 				LuaScriptMachine()->InitVar(nIndex);
// 				if(!TaskSetRand()->IsTaskOnTime(m_pInfo->dwData))//�Ƿ�Ϊ��ʱ����
//  			      GameAction()->ProcessAction(idAction, pUser);
// 			}
//               
// 			//���ڵ���
// 			if(pUser&&pUser->IsGM())
// 			{
// 				char szMsg[1024];
// 				if(idAction)
// 				{
// 				    sprintf(szMsg,"����ϵͳ:��������Action:%u",idAction);
// 				    szMsg[255] = '\0';
// 				}
// 				else
// 				{
// 					sprintf(szMsg,"��������ActionΪ��");
// 				}
// 				CMsgTalk msg;
// 				if(msg.Create(SYSTEM_NAME,pUser->GetName(),szMsg,NULL,0x00ffff00,_TXTATR_NORMAL))
// 					pUser->SendMsg(&msg);
// 			}
// 		}
// 		break;
// 	case actionDoGenScript://������������ű�
// 		{
// 			DWORD idAction = TaskSetRand()->GetActionGenerate(m_pInfo->dwData,m_pInfo->idNpc);
// 
//             if(idAction)
// 			{
// 			  if(!TaskSetRand()->IsTaskOnTime(m_pInfo->dwData))//�Ƿ�Ϊ��ʱ����
//  			    GameAction()->ProcessAction(idAction, pUser);
// 			}
//               
// 			//���ڵ���
// 			if(pUser&&pUser->IsGM())
// 			{
// 				char szMsg[1024];
// 				if(idAction)
// 				{
// 				    sprintf(szMsg,"����ϵͳ:��������Action:%u",idAction);
// 				    szMsg[255] = '\0';
// 				}
// 				else
// 				{
// 					sprintf(szMsg,"��������ActionΪ��");
// 				}
// 				CMsgTalk msg;
// 				if(msg.Create(SYSTEM_NAME,pUser->GetName(),szMsg,NULL,0x00ffff00,_TXTATR_NORMAL))
// 					pUser->SendMsg(&msg);
// 			}
// 		}
// 		break;
// 	case actionTaskOnTime: //��ʹ��ɵ�����
// 		{
// 			DWORD idAction = TaskSetRand()->GetActionId(m_pInfo->dwData);
// 
// 			//���ڵ���
// 			if(pUser&&pUser->IsGM())
// 			{
// 				char szMsg[1024];
// 				if(idAction)
// 				{
// 					sprintf(szMsg,"����ϵͳ:�������˼�ʱ����-->Action:%u",idAction);
// 					szMsg[255] = '\0';
// 				}
// 				else
// 				{
// 					sprintf(szMsg,"��������ActionΪ��");
// 				}
// 				CMsgTalk msg;
// 				if(msg.Create(SYSTEM_NAME,pUser->GetName(),szMsg,NULL,0x00ffff00,_TXTATR_NORMAL))
// 					pUser->SendMsg(&msg);
// 			}
// 			
//             if(idAction)
// 			{
// 				if(TaskSetRand()->IsTaskOnTime(m_pInfo->dwData))//�Ƿ�Ϊ��ʱ����
// 				   GameAction()->ProcessAction(idAction, pUser);
// 			}
// 		}
// 		break;
// 	case actionRequireMarry:  //�������
// 		{
// 			if(pUser)
// 			   pUser->RequireMarry(m_pInfo->dwData);
// 		}
// 		break;
// 	case actionLoverBedLevUp:            //���Ĵ�������
// 		{
// 			if(pUser)
// 			   pUser->LevUpBed(m_pInfo->dwData);
// 		}
// 		break;
// 	case actionLoverStorageLevUp:        //���Ĺ��ӵ�����
// 		{
// 			if(pUser)
// 			   pUser->LevUpStorage(m_pInfo->dwData);
// 		}
// 		break;
// 	case actionLoverFlowerLevUp:        //���Ļ��������
// 		{
// 			if(pUser)
// 			  pUser->LevUpFlower(m_pInfo->dwData);
// 		}
// 		break;
// 	case actionLoverCabinetLevUp:      //���ĳ���������
// 		{
// 			if(pUser)
// 			  pUser->LevUpCabinet(m_pInfo->dwData);
// 		}
// 		break;
// 	case actionMarryBed:
// 		{
// 			if (pUser)
// 			{
// 				const  OBJID moonidNpc = 1811; //����NPC
// 				
// 				CNpc* pNpc = NULL;
// 				CGameMap* pMap = pUser->GetMap();
// 				if(!pMap)
// 					return;
// 				
// 				if(!pMap->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_NPC, moonidNpc, IPP_OF(pNpc)))
// 				{
// 					pUser->SendSysMsg("��������̫Զ,�޷�ʹ�ô�!");
// 					return;
// 				}
// 				if (!pUser->IsMarried())
// 				{
// 					pUser->SendSysMsg("�㲢û�о��л���!");
// 					return;
// 				}
// // 				float  nPer = 0.0f;
// // 				int nMaxHP  = 0;
// // 				int nMaxMP  = 0;
// 				if(pUser->Get_BedLevel() == 1)
// 				{
// 					GameAction()->ProcessAction(1712001, pUser);
// 					//nPer = 0.3f;
// 				}
// 				else if(pUser->Get_BedLevel() == 2)
// 				{
// 					GameAction()->ProcessAction(1712002/*idAction*/, pUser);
// 					//nPer = 0.6f;
// 				}
// 				else if(pUser->Get_BedLevel() >= 3)
// 				{
// 					GameAction()->ProcessAction(1712003/*idAction*/, pUser);
// 					//nPer = 0.8f;
// 				}
// 
// // 				int nCurrLife = pUser->GetLife(); //��õ�ǰѪ��
// // 				int nCurrMana = pUser->GetMana(); //��õ�ǰMP
// // 				int nDetalHP  = pUser->GetMaxLife() * nPer;
// // 				int nDetalMp  = pUser->GetMaxMana() * nPer;
// // 
// // 				if(nCurrLife + nDetalHP > pUser->GetMaxLife())
// // 					nCurrLife = pUser->GetMaxLife();
// // 				else
// // 					nCurrLife += nDetalHP;
// // 
// // 				if(nCurrMana + nDetalMp > pUser->GetMaxMana())
// // 					nCurrMana = pUser->GetMaxMana();
// // 				else
// // 					nCurrMana += nDetalMp;
// // 
// // 				pUser->SetAttrib(_USERATTRIB_LIFE, nCurrLife/*pUser->GetMaxLife()*/, SYNCHRO_TRUE);
// // 				pUser->SetAttrib(_USERATTRIB_MANA, nCurrMana/*pUser->GetMaxMana()*/, SYNCHRO_TRUE);
// 			}
// 		}	
// 		break;
// 	case actionResetGift:
// 		{
// 			if(pUser)
// 			{
// 				pUser->ProcessGift();
// 			}
// 		}
// 		break;
	case actionLearnMagicLearn:
		{
			if(pUser)
			{
				if(m_pInfo->idNpc <= 0 )
					m_pInfo->idNpc = 1;

				m_pInfo->unDir += m_pInfo->idNpc;
				CHECK(m_pInfo->unDir > 0 && m_pInfo->unDir <= 150);

				pUser->QueryMagic()->LearnMagic(m_pInfo->dwData, m_pInfo->unDir,true,EM_NOT_GIFT_MAGIC,NULL, m_pInfo->idNpc);//ѧϰ���������ܶ������ﴦ��


// 				if (m_pInfo->idNpc==0)
// 					pUser->QueryMagic()->LearnMagic(m_pInfo->dwData, m_pInfo->unDir,true,EM_NOT_GIFT_MAGIC,NULL, m_pInfo->idNpc);//ѧϰ���������ܶ������ﴦ��
// 				else
// 				{
// 					int nLev = m_pInfo->unDir;
// 					for (int i=0; i<m_pInfo->idNpc; i++)//��һ�����,��N�μ�,��������
// 					{
// 						if (!pUser->QueryMagic()->LearnMagic(m_pInfo->dwData, nLev++,true,EM_NOT_GIFT_MAGIC,NULL, m_pInfo->idNpc))//ѧϰ���������ܶ������ﴦ��
// 							break;
// 					}
// 				}
			}
			
		}
		break;
	case actionSychroVas:
		{
			CMsgVAS MsgVAS;
			if (MsgVAS.Create(0, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_QUERY_BALANCE))
			{
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVAS );
			}
		}
		break;
	case actionVasToCheck://Ԫ����Ʊ
		{
			if (!pUser)
				return;
			if (m_pInfo->dwData<10 || m_pInfo->dwData>2000)//Ԫ��Ʊ�޶�
				return;

			if (m_pInfo->dwData > pUser->GetMoneyByType(5))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "Ԫ������");
				return;
			}
			if (pUser->GetVasTicketAmount()>20000)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "ÿ��ɶһ���Ԫ������Ϊ20000�����Ѵﵽ���޵����޷��ٶһ�");
				return;
			}
			CItemTypeData* pItemTypeData = ItemType()->QueryItemType(_SPCITEM_VASEXCHANGE);
			if (!pItemTypeData)
			{
				::LogSave("��ƷԪ��Ʊ����%uδ�ҵ�", _SPCITEM_VASEXCHANGE);
				return;
			}
// 			if (m_pInfo->dwData > 2000/*pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT)*/)
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "�һ�����Ԫ��Ʊ�ɴ洢����");
// 				return;
// 			}
			if(!pUser->QueryPackage())
				return;
			int nPosition = pUser->QueryPackage()->GetAvailablePosition(_SPCITEM_VASEXCHANGE);
			if (pUser->QueryPackage()->IsPackFull(1,_SPCITEM_VASEXCHANGE,nPosition))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "��������");
				return;
			}
			//Ԥ������λ��
			pUser->QueryPackage()->SetIndexOrderAmount(nPosition, 1);

			CMsgVAS msg;
			if (msg.Create(m_pInfo->dwData, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_SPEND_VAS, _SPCITEM_VASEXCHANGE, 0, m_pInfo->dwData))
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );
		}
		break;
// 	case actionEXPToCheck://���黻��
// 		{
// 			if (!pUser)
// 				return;
// 			if (m_pInfo->dwData<=0)
// 				return;
// 			CItemTypeData* pItemTypeData = ItemType()->QueryItemType(_SPCITEM_EXPEXCHANGE);
// 			if (!pItemTypeData)
// 			{
// 				::LogSave("��Ʒ���鵤����%uδ�ҵ�", _SPCITEM_EXPEXCHANGE);
// 				return;
// 			}
// 			if (m_pInfo->dwData > pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT))
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "�һ����鳬�����鵤�ɴ洢����");
// 				return;
// 			}
// 			if (pUser->GetExp()<m_pInfo->dwData)
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "��ɫ���鲻��");
// 				return;
// 			}
// 			int nPosition = pUser->QueryPackage()->GetAvailablePosition(_SPCITEM_EXPEXCHANGE);
// 			if (pUser->QueryPackage()->IsPackFull(1,_SPCITEM_EXPEXCHANGE,nPosition))
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "��������");
// 				return;
// 			}
// 			CItem* pItem = pUser->AwardItem(_SPCITEM_EXPEXCHANGE, true,false,false,false,m_pInfo->dwData);
// 			if (pItem)
// 			{
// 				pUser->SetAttrib(_USERATTRIB_EXP, pUser->GetExp()-m_pInfo->dwData, SYNCHRO_TRUE);//ͬ�����ﾭ��
// 			}
// 		}
// 		break;
	case actionMine:
		{
			if(pUser->Mine())
			{
				CMsgAction msg;
				if (msg.Create(pUser->GetID(), 0, 0, 0, actionMine))
					this->SendMsg(&msg);
			}
			else
			{
				CMsgAction msg;
				if (msg.Create(pUser->GetID(), 0, 0, 0, actionStopMine))
					this->SendMsg(&msg);
			}
		}
		break;
	case actionQueryNewGive:
		{
			CMsgUserAttrib	msg;
			if(msg.Create(pUser->GetID(), _USERATTRIB_NEWGIVEMASKLOW, pUser->GetNewGiveMask()& 0x00000000FFFFFFFF)) //�ɾ�����
			{
				msg.Append(_USERATTRIB_NEWGIVEMASKLOW2,pUser->GetNewGiveMask2()& 0x00000000FFFFFFFF);
				msg.Append(_USERATTRIB_CREATEROLETIME,pUser->GetPassedTime(pUser->GetCreateTime(),1));
				CSynAttr* pSynAtt = pUser->QuerySynAttr();
				if(pSynAtt && (pSynAtt->GetSynID() != ID_NONE))
					msg.Append(_USERATTRIB_JOINSYNTIME,pUser->GetPassedTime(pSynAtt->GetInt(SYNATTRDATA_jointime),1));
				else
					msg.Append(_USERATTRIB_JOINSYNTIME,0);
				pUser->SendMsg(&msg);
			}
		}
		break;
	case actionGetNewGive:
		{
			if(m_pInfo->idNpc > 32)
				return;

			if(pUser->GetNewGive(m_pInfo->idNpc))
			{
				CMsgUserAttrib	msg;
				if(msg.Create(pUser->GetID(), _USERATTRIB_NEWGIVEMASKLOW, pUser->GetNewGiveMask()& 0x00000000FFFFFFFF)) //�ɾ�����
				{
					pUser->SendMsg(&msg);
				}
				pUser->SendSysMsg("��ȡ�ɹ�");
			}
			else
				pUser->SendSysMsg("��ȡʧ��");
		}
		break;
		
	case actionQueryOtherInfo :    //�䳤��Ϣ����Ҫ��������ֱ������Ϣ�����
		{
			DWORD majorcon=pUser->GetJobCon(1);
			DWORD assistcon=pUser->GetJobCon(2);
			DWORD viptime=pUser->GetVipLeftMinites();
			DWORD frozentime=pUser->GetFrozenDoubleTime();
			DWORD nationCon=pUser->GetNationCon();
			DWORD chivalrous=pUser->GetChivalrous();
			DWORD dwJinmeiValue = pUser->GetJinmeiValue();
		
		
			CMsgUserAttrib msg_att;
			msg_att.Create(pUser->GetID(),_USERATTRIB_UserInfo_MajorCon,majorcon);
			msg_att.Append(_USERATTRIB_UserInfo_AssistCon,assistcon);
			msg_att.Append(_USERATTRIB_UserInfo_FrozenDoubleTime,frozentime);
			msg_att.Append(_USERATTRIB_UserInfo_VipLeft,viptime);
			msg_att.Append(_USERATTRIB_UserInfo_XIAYIZHI,chivalrous);
			msg_att.Append(_USERATTRIB_UserInfo_RONGYUZHI,nationCon);
			msg_att.Append(_USERATTRIB_UserInfo_JimeiValue, dwJinmeiValue);
			msg_att.Append(_USERATTRIB_ARENA_SCORE, pUser->GetArenaScore());
			pUser->SendMsg(&msg_att);
	
		}
		break;

	case actionGetAward :
		{
			CHECK(pUser);
			CHECK(pUser->CanBeSee());

			int nStep = 0;
			DEBUG_TRY
			if(!pUser->IsAwardTimeOut())
				return;	
			nStep = 1;
			DWORD ntimes=pUser->GetAward();
			if(ntimes>9)
				ntimes-=10;
			if(ntimes<0)
			{
				pUser->SetAward(8);
				pUser->SendSysMsg("��ȡʧ��");
				break;
			}
			if (ntimes>7)
			{
				pUser->SetAward(8);
				pUser->SendSysMsg("���Ľ����Ѿ�����");
				break;
			}
			nStep = 2;
			switch(ntimes%10)
			{

			case 0:
				{   
					nStep = 3;
					DEBUG_TRY
					if(pUser->ProcessAction(2100))
					{
						ntimes++;
						pUser->SetNextAwardTime(120);
						pUser->SetAward(ntimes);
						pUser->SendSysMsg("�ɹ���ȡ�˵�һ�ݽ���");
						break;
					}
					else
						pUser->SendSysMsg("��ȡʧ��");
					DEBUG_CATCH("pUser->ProcessAction(2100)");
				}
				break;
			case 1:
				{
					nStep = 4;
					DEBUG_TRY
						if(pUser->ProcessAction(2101))
						{
							ntimes++;							
							pUser->SetNextAwardTime(180);
							pUser->SetAward(ntimes);
							pUser->SendSysMsg("�ɹ���ȡ�˵ڶ��ݽ���");
							break;
						}
						else
							pUser->SendSysMsg("��ȡʧ��");
					DEBUG_CATCH("pUser->ProcessAction(2101)");
				}
				break;
            case 2:
				{
					nStep = 5;
					DEBUG_TRY
						if(pUser->ProcessAction(2102))
						{
							ntimes++;
							pUser->SetNextAwardTime(300);
							pUser->SetAward(ntimes);
							pUser->SendSysMsg("�ɹ���ȡ�˵����ݽ���");
							break;
						}
						else
							pUser->SendSysMsg("��ȡʧ��");
					DEBUG_CATCH("pUser->ProcessAction(2102)");
					
				}
				break;
			case 3:
				{
					nStep = 6;
					DEBUG_TRY
						if(pUser->ProcessAction(2103))
						{
							ntimes++;
							pUser->SetNextAwardTime(600);
							pUser->SetAward(ntimes);
							pUser->SendSysMsg("�ɹ���ȡ�˵��ķݽ���");
							break;
						}
						else
							pUser->SendSysMsg("��ȡʧ��");
					DEBUG_CATCH("pUser->ProcessAction(2103)");
					
				}
				break;
			case 4:
				{
					nStep = 7;
					DEBUG_TRY
						if(pUser->ProcessAction(2104))
						{
							ntimes++;
							pUser->SetNextAwardTime(900);
							pUser->SetAward(ntimes);
							pUser->SendSysMsg("�ɹ���ȡ�˵���ݽ���");
							break;
						}
						else
							pUser->SendSysMsg("��ȡʧ��");
					DEBUG_CATCH("pUser->ProcessAction(2104)");
				
				}
				break;
			case 5:
				{
					nStep = 8;
					DEBUG_TRY
						if(pUser->ProcessAction(2105))
						{
							ntimes++;
							pUser->SetNextAwardTime(1200);
							pUser->SetAward(ntimes);
							pUser->SendSysMsg("�ɹ���ȡ�˵����ݽ���");
							break;
						}
						else
							pUser->SendSysMsg("��ȡʧ��");
					DEBUG_CATCH("pUser->ProcessAction(2105)");
				}
				break;
			case 6:
				{
					nStep = 9;
					DEBUG_TRY
						if(pUser->ProcessAction(2106))
						{
							ntimes++;
							pUser->SetNextAwardTime(1800);
							pUser->SetAward(ntimes);
							pUser->SendSysMsg("�ɹ���ȡ�˵��߷ݽ���");
							break;
						}
						else
							pUser->SendSysMsg("��ȡʧ��");
					DEBUG_CATCH("pUser->ProcessAction(2106)");
				}
				break;
			case 7:
				{
					nStep = 10;
					DEBUG_TRY
						if(pUser->ProcessAction(2107))
						{
							ntimes++;
							pUser->SetAward(ntimes);
							pUser->SendSysMsg("�ɹ���ȡ�˵ڰ˷ݽ���");
							break;
						}
						else
							pUser->SendSysMsg("��ȡʧ��");
					DEBUG_CATCH("pUser->ProcessAction(2107)");
				}
				break;
			default:
				break;
			 }
			 DEBUG_CATCH2("actionGetAward error! step = %u",nStep)
		 }
		 break;
	case actionStopAutoAtk://goto �������Ѿ���Ϲ����ˣ����ﲻ������ֻ��Ϊ�˱���д��־
		break;
	case actionGetEudemonMagicCD://��ȡ���＼��CD
		{
			CMonster* pWarPet = pUser->QueryCallPet();
			if (!pWarPet)
				return;
			if (pWarPet->GetID()!=m_pInfo->dwTimeStamp)
				return;
			pWarPet->CreateAllCD();
		}
		break;
	case actionGetTeamMemPos://��ȡ���ѵ�����
		{
			CTeam* pTeam = pUser->GetTeam();
			if (!pTeam)
				return;
			if (m_pInfo->idNpc!=ID_NONE)
			{
				if (!pTeam->IsTeamMember(m_pInfo->idNpc))
					return;
				CUser* pMember = UserManager()->GetUser(m_pInfo->idNpc);
				if (pMember && pMember->GetMap()==pUser->GetMap())
				{
					CMsgTeamPos msg;
					if (msg.Create(_TEAMPOS_GETPOS, pUser->GetID()))
					{
						msg.Append(pMember);
						pUser->SendMsg(&msg);
					}
				}
			}
			else
			{
				CMsgTeamPos msg;
				if (!msg.Create(_TEAMPOS_GETPOS, pUser->GetID()))
					return;
				for(int i = 0; i < pTeam->GetMemberAmount(); i++)
				{
					OBJID idMember = pTeam->GetMemberByIndex(i);
					CUser* pMember = UserManager()->GetUser(idMember);
					if(pMember && pMember->GetID()!=pUser->GetID() && pMember->GetMap()==pUser->GetMap())
					{
						msg.Append(pMember);
					}
				}
				pUser->SendMsg(&msg);
			}
		}
		break;
	case actionSendPosToTeam:
		{
			CMsgTeamPos msg;
			if (!msg.Create(_TEAMPOS_GETPOS, m_pInfo->idNpc))
				return;
			msg.Append(pUser);
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, m_pInfo->idNpc);
		}
		break;

	case actionSetDress:
		{
			DEBUG_TRY
			pUser->SetDress(m_pInfo->unDir);
			CMsgUserAttrib	msg;
			IF_OK(msg.Create(pUser->GetID(), _USERATTRIB_UserInfo_IsDress, m_pInfo->unDir))
				pUser->BroadcastRoomMsg(&msg, INCLUDE_SELF);
			DEBUG_CATCH2("%d actionSetDress crash!", pUser->GetID())
		}
		break;

	case actionQueryDress:
		{
			DWORD dwIsDress = pUser->GetDress();
			CMsgAction	msg;
			if (msg.Create(dwIsDress, actionQueryDress))
			{
				pUser->SendMsg(&msg);
			}
		}
		break;
	case actionQueryTitleList:
		{
			//goto------begin ���ƺ�����ǰ����Ҳ��ϳƺ�,�Ժ��·��Ͳ���Ҫ�����
			if (pUser->IsVipActive())//vip�ƺ�
			{
				int nVipLev = pUser->GetVipLev()==4 ? 1 : pUser->GetVipLev();
				if (!pUser->FindTitleByType(_TITLE_VIP))//��vip����û�ƺ�
					pUser->AddHoldTitle(_TITLE_VIP, nVipLev);
			}
			//goto------end
			pUser->SendAllTitle();//ͬ���ƺŵ��ͻ���
			if (pUser->GetSynID()!=0)//���ɳƺ�
			{
				int nRank = pUser->QuerySynRank();
				if (nRank >= RANK_FEELLEADER && !pUser->FindTitleByType(_TITLE_SYNDICATE))//����ݵİ��ɳ�Ա
				{
					switch(nRank)
					{
						case RANK_LEADER:			pUser->AddHoldTitle(_TITLE_SYNDICATE, 14);break;//����
						case RANK_SUBLEADER:		pUser->AddHoldTitle(_TITLE_SYNDICATE, 13);break;//������
						case RANK_FEELLEADER:		pUser->AddHoldTitle(_TITLE_SYNDICATE, 15);break;///////////��Ӣ�������ر�ע�⣡��
						default:break;
					}
				}
				else if(pUser->FindTitleByType(_TITLE_SYNDICATE))
				{
					switch(nRank)
					{
						case RANK_LEADER:
							if(pUser->GetTitleLev(_TITLE_SYNDICATE)!=14)
								pUser->SetTitleLevel(_TITLE_SYNDICATE, 14);
							break;//����
						case RANK_SUBLEADER:
							if(pUser->GetTitleLev(_TITLE_SYNDICATE)!=13)
								pUser->SetTitleLevel(_TITLE_SYNDICATE, 13);
							break;//������
						case RANK_FEELLEADER:
							if(pUser->GetTitleLev(_TITLE_SYNDICATE)!=15)
								pUser->SetTitleLevel(_TITLE_SYNDICATE, 15);
							break;///////////��Ӣ�������ر�ע�⣡��
						default:
							if(pUser->GetTitleLev(_TITLE_SYNDICATE)!=0)
								pUser->SetTitleLevel(_TITLE_SYNDICATE,0);
					}
				}
			}
			else
			{
				if (pUser->FindTitleByType(_TITLE_SYNDICATE))//����ݵİ��ɳ�Ա
				{
					pUser->DelHoldTitle(_TITLE_SYNDICATE);
				}
			}

		}
		break;
	case actionSetUseTitle:
		{
			pUser->SetUseTitle(m_pInfo->unPosX, m_pInfo->unPosY);
		}
		break;
	case actionQueryTreasure:
		{
			pUser->SendAllTreasure();
		}
		break;
	case actionLottery://goto ��Ʊ������
		{
			if (!pUser->QueryTreasure())
				return;
			if (pUser->IsItemLockedOpen())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "���ȹر��˺Ű�ȫ��");
				return;
			}
			if (m_pInfo->idNpc<1 || m_pInfo->idNpc>3)//�����ӷ�����
				return;
			if (m_pInfo->dwData<1 || m_pInfo->dwData>50)//�����Ӵ������
				return;
			int nNeedVas = pUser->QueryTreasure()->GetTreasurePrice(m_pInfo->idNpc, m_pInfo->dwData);
			if (nNeedVas<=0)
				return;
			
			if (!pUser->QueryTreasure()->HasEnoughSpace(m_pInfo->dwData))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "�������������");
				return;
			}
			if (pUser->GetMoneyByType(5)<nNeedVas)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "Ԫ������");
				return;
			}
			CMsgVAS msg;
			if (msg.Create(nNeedVas, pUser->GetAccountID(), pUser->GetID(), _MSGVAS_BUY_LOTTERY, m_pInfo->dwData, m_pInfo->idNpc))
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );//�������̷߳���Ϣ
		}
		break;
	case actionGetTreasure:
		{
			if (!pUser->QueryTreasure())
				return;
			pUser->QueryTreasure()->GetTreasure(m_pInfo->dwData);
		}
		break;
	case actionDropTreasure:
		{
			if (!pUser->QueryTreasure())
				return;
			CHECK(m_pInfo->dwData);
			pUser->QueryTreasure()->DelTreasure(m_pInfo->dwData);
		}
		break;
 	case actionEveryDayWorkPresent:
 		{
			if (!LuaScriptMachine())
			{
				return;
			}
			CHECK(pUser);
			pUser->InitScript();
 			LuaScriptMachine()->RunScriptFile("action/rcblq.lua");
 		}
 		break;
	case actionQuickDayWork:
		{
			char szlua[32] = "";
//			LuaScriptMachine()->InitMapOntimerScript(m_pInfo->dwData);
			if (!LuaScriptMachine())
			{
				return;
			}
			CHECK(pUser);
			pUser->InitScript();
			LuaScriptMachine()->InitVar(m_pInfo->dwData);
			sprintf(szlua, "action/rcbwc.lua");
			LuaScriptMachine()->RunScriptFile(szlua);   
		}
		break;
	case actionQueryInsInfo:
		{
			pUser->SendInsMemInfo(m_pInfo->dwData);
		}
		break;
	case actionChoseTimeBox://����ʱ�ޱ���
		{
			if(m_pInfo->idNpc<=100 || m_pInfo->dwData==ID_NONE)
				return;

			int nType = m_pInfo->idNpc / 100;
			int nIndex = m_pInfo->idNpc % 100;

			if(nType==1 || nType==2)			//ʯͷ,ʯͷx3
				CHECK(nIndex>0 && nIndex<7);
			else if(nType==3)					//��������
				CHECK(nIndex>0 && nIndex<5);
			else
				return;

			CItem* pItem = pUser->GetItem(m_pInfo->dwData, false, false);
			if(!pItem)
				return;

			OBJID idAction = 500400 + m_pInfo->idNpc - 1;
			if(pUser->ProcessAction(idAction))
				pUser->SpendItem(pItem);
		}
		break;
	case actionVipGetVas://vip��ȡ�鱦
		{
			if(pUser->IsVipActive())
				pUser->ProcessAction(630006);
		}
		break;
	case actionEudemonPlayAdd:
		{
			CTeam* pTeam = pUser->GetTeam();
			if(!pTeam)
				return;
			pTeam->EudemonPlayChange(pUser->GetID(),m_pInfo->idTarget);
		}
		break;
	case actionEudemonPlayLock:
		{
			CTeam* pTeam = pUser->GetTeam();
			if(!pTeam)
				return;
			pTeam->EudemonPlayChange(pUser->GetID(),true);
		}
		break;
	case actionEudemonPlayCancel:
		{
			CTeam* pTeam = pUser->GetTeam();
			if(!pTeam)
				return;
			pTeam->EudemonPlayCancel();
		}
		break;
	case actionEudemonPlay:
		{
			CTeam* pTeam = pUser->GetTeam();
			if(!pTeam)
				return;
			pTeam->EudemonPlay();
		}
		break;
	default:
		{
			char strTemp[128];
			sprintf(strTemp,"switch(m_pInfo->usAction) default...m_pInfo->usAction=%d",m_pInfo->usAction);
			//ASSERT(!strTemp);
			::LogSave(strTemp);
			break;
		}
	}
	DEBUG_CATCH3("switch(MSGACTION) [%d] [%d]", m_pInfo->usAction,nStep)		// AAAAAAAAAAAAAAAA
}
