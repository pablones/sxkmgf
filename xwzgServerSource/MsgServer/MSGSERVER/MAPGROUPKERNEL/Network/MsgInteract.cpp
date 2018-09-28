// MsgInteract.cpp: interface for the CMsgInteract class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgInteract.h"
#include "MsgAction.h"
#include "MapGroup.h"
#include "MsgUserAttrib.h"//20080110修罗:为使用AddAttrib()
#include "MsgTalk.h"
#include "MsgPlayer.h"
#include "MsgCoolDown.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgInteract::CMsgInteract()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgInteract::~CMsgInteract()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgInteract::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_INTERACT != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgInteract::Create2(USHORT unType, OBJID idSender, OBJID idTarget, USHORT unPosX, USHORT unPosY, DWORD dwData /*= 0*/,DWORD dwBattleAddInfo )
{
	DEBUG_CREATEMSG("Interact",idSender,unType,"",idTarget,dwData)
	CHECKF(unType && idSender);		// && idTarget

	// init
	this->Init();
	// fill info now
	m_pInfo->unType			= unType;

	m_pInfo->idSender		= idSender;
	m_pInfo->idTarget		= idTarget;
	m_pInfo->usPosX			= unPosX;
	m_pInfo->usPosY			= unPosY;
	m_pInfo->dwData			= dwData;
	m_pInfo->dwBattleAddInfo = dwBattleAddInfo;

	m_unMsgType	=_MSG_INTERACT;
	m_unMsgSize	=sizeof(MSG_Info);

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgInteract::Create(USHORT unType, OBJID idSender, OBJID idTarget, USHORT unPosX, USHORT unPosY, USHORT usMagicType, USHORT usMagicLev , DWORD nIntoneTime)
{
	DEBUG_CREATEMSG("Interact",idSender,unType,"",idTarget,dwData)
	CHECKF(unType && idSender);		// && idTarget

	// init
	this->Init();
	// fill info now
	m_pInfo->unType			= unType;
	m_pInfo->idSender		= idSender;
	m_pInfo->idTarget		= idTarget;
	m_pInfo->usPosX			= unPosX;
	m_pInfo->usPosY			= unPosY;
	m_pInfo->usMagicType	= usMagicType;
	m_pInfo->usMagicLev		= usMagicLev;
	m_pInfo->dwData			= nIntoneTime;
	m_unMsgType	=_MSG_INTERACT;
	m_unMsgSize	=sizeof(MSG_Info);

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgInteract::Process(void *pInfo)
{
	IRole* pRole = RoleManager()->QueryRole(this, m_pInfo->idSender);
	if(!pRole)
		return;
	
	CUser* pUser = NULL;
	pRole->QueryObj(OBJ_USER, IPP_OF(pUser));

	if(pUser && !pUser->IsAlive())
	{
		pUser->SendSysMsg("你已经死亡!");//STR_DIE
		return ;
	}
	IRole* pTarget = RoleManager()->QueryRole(m_pInfo->idTarget);
	if(!pTarget && INTERACT_MAGICATTACK != m_pInfo->unType && INTERACT_MOUNT != m_pInfo->unType)//需要目标的技能判断
		return;

	if ( m_pInfo->unType == INTERACT_ATTACK 
		|| m_pInfo->unType == INTERACT_SHOOT)
	{
		if( pTarget && 	(pTarget->QueryStatus( STATUS_IMMUNE_ALL ) || pTarget->QueryStatus( STATUS_RESOURCEWAR_PROTECT ) ))
		{
			if( pUser )
				pUser->SendSysMsg( "目标是无敌的!" );//"目标是无敌的!"
			return;
		}
		
	}

	// fill id
	m_pInfo->idSender	= pRole->GetID();


	char szDebugAction[32];
	char szDebugStep[32];
	sprintf(szDebugAction,"%d",m_pInfo->unType);
//modify code by 林德渊 begin 2011.5.19
// 	if( INTERACT_MOUNT != m_pInfo->unType && g_StatisticalData.Addinteract((unsigned long) m_pInfo->idSender) )
// 	{
// 		UserManager()->KickOutSocket(pUser->GetSocketID(), "> maxinteract");
// 		g_StatisticalData.EraseMaxInte();
// 		char szTemp[256] = {0};
// 		if( pUser )
// 			sprintf(szTemp,"KickOutSocket [%s] > maxinteract",pUser->GetName());
// 		::LogSave(szTemp);
// 	}
//modify code by 林德渊 end
	DEBUG_TRY
	try{
	switch(m_pInfo->unType)
	{
	case	INTERACT_ATTACK:
	case	INTERACT_SHOOT:
		{
			if (m_pInfo->idSender == m_pInfo->idTarget)
				return;
			if (pUser)
			{
				if(!pUser->CheckMsgData(CHECKDATA_ATTACK,800))
					return;

				sprintf(szDebugStep,"%d",1);
				if (!pUser->SynPosition(m_pInfo->usPosX, m_pInfo->usPosY))
				{
					UserManager()->KickOutSocket(m_idSocket, "CMsgInteract SynPosition:Shoot");
					return;
				}

				sprintf(szDebugStep,"%d",3);
				if(!pTarget)
				{
					return;
				}

			}


			sprintf(szDebugStep,"%d",4);
			BOOL bSuc = pRole->SetAttackTarget(pTarget);
			if(bSuc && pUser)
			{
				sprintf(szDebugStep,"%d",5);
				pUser->SynchroLockAtkBegin();
			}

			sprintf(szDebugStep,"%d",6);
			if(pUser)
				pUser->ProcessAutoAttack();
		}
		break;
	case	INTERACT_MAGICATTACK:
		{
			if (!this->IsNpcMsg())
			{
				USHORT usLev  = m_pInfo->usMagicLev;
				USHORT usType = m_pInfo->usMagicType;

				//下面要对技能进行判断
				if (pUser)
				{
					if (pUser->QueryMagic())
					{
						IMagicData* pMagicData = pUser->QueryMagic()->FindMagic(usType);
						if (pMagicData)
						{
// 							if ((pMagicData->GetInt(MAGICDATA_AUTO_ACTIVE) & AUTOACTIVE_RANDOM) != 0)
// 							{
// 								return;
// 							}
						}
					}
				}
			}

			if(pUser)
			{
				if(!pUser->CheckMsgData(CHECKDATA_MAGICATT,1000))
					return;
				pUser->ProcessOnAttack();
			}
// 			if (m_pInfo->usMagicType==1136)//瞬移，需要知道人物方向
// 			{
// 				if (m_pInfo->dwData<1 || m_pInfo->dwData>9 || m_pInfo->dwData==5)//方向不对
// 					return;
// 				pUser->SetDir(m_pInfo->dwData);
// 			}
			pRole->MagicAttack(m_pInfo->usMagicType, m_pInfo->idTarget, m_pInfo->usPosX, m_pInfo->usPosY);

		}
		break;
	case INTERACT_MOUNT://[2010-08-09 goto]坐骑
		{
			if (!pUser)
				return;			
			CItem* pMount = pUser->GetEquipItemByPos(ITEMPOSITION_MOUNT);
			if (!pMount || pMount->GetInt(ITEMDATA_POSITION)!=ITEMPOSITION_MOUNT)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "您还没有装配坐骑");
				return;
			}
			if(pUser->QueryStatus(STATUS_WARGAME))//竞技场
				return;

			if (!pMount->IsEnableMount())
				return;
			int aa = pMount->GetInt(ITEMDATA_REQ_PROF);
			int bb = pUser->GetProfession();
			if ( (pUser->GetProfession() & pMount->GetInt(ITEMDATA_REQ_PROF))==0 )
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "职业不符");
				return;
			}
			if (pUser->GetMountDelay()<10000 && !pUser->GetItemByType(_SPCITEM_MONEYFORSHOP))//距离上次使用坐骑时间太短,如果在跑商时不考虑CD，要让玩家下坐骑
				return;
			if (pUser->GetMountState())//下坐骑
			{
				pUser->SetUnMount();
				CMsgPlayer msgPlayer;
				if (msgPlayer.Create(pUser->QueryRole()))
					pUser->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);

				CRole::DetachStatus(pUser,STATUS_MOUNTSPEED_NUMBER);
			}
			else//上坐骑
			{
				if(pUser->QueryBooth())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "摆摊中不能上坐骑");
					return;
				}
				if (pUser->GetItemByType(_SPCITEM_MONEYFORSHOP))
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "跑商中不能上坐骑");
					return;
				}
				if (pUser->GetMap() && pUser->GetMap()->IsDynaMap())
				{
					pUser->SendSysMsg("副本中不能上坐骑！");
					LOGWARNING("该死的家伙绕过了客户端,企图在副本中上坐骑iduser = %u", pUser->GetID());
					return;
				}
				int nSpeed = pMount->GetInt(ITEMDATA_BASE1);
				if (nSpeed/10000!=GENADD_SORT_SPEED)
					return;
				//坐上坐骑时把别的加速BUF清除
				if (pUser->QueryStatus(STATUS_MOVESPEEDUP_PERCENT))
					CRole::DetachStatus(pUser,STATUS_MOVESPEEDUP_PERCENT);
				if (pUser->QueryStatus(STATUS_MOUNTSPEED_NUMBER))
					CRole::DetachStatus(pUser,STATUS_MOUNTSPEED_NUMBER);

				pUser->SetMount();
				CMsgPlayer msgPlayer;
				if (msgPlayer.Create(pUser->QueryRole()))
					pUser->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);

				CRole::AttachStatus(pUser,STATUS_MOUNTSPEED_NUMBER, nSpeed%1000, -1, 0,0,0,0,pUser->GetID());
			}
			pUser->SetAttrib(_USERATTRIB_SPEED, pUser->AdjustSpeed(pUser->GetSpeed()), SYNCHRO_TRUE);
			pUser->UpdateMountDelay();
			CMsgCoolDown msgCD;//通知客户端进行10秒CD，以免用户连续点击,暂时只在客户端CD，服务端不CD
			if( msgCD.Create( pUser->GetID(),MAGICCOOLDOWN_BEGIN,MAGIC_INFO,9508,10000,0 ) )
			{
				pUser->SendMsg( &msgCD );
			}
		}
		break;
	default:
		{
			LOGERROR("INVALID MSG INTERACT TYPE %d", m_pInfo->unType);
		}
		break;
	} // switch
		// AAAAAAAAAAAAAAAA/*	
// 		if(pRole->IsUser() && pTarget)
// 		{
// 			
// 			if (pTarget->IsUser() && pRole->IsAtkable(pTarget))
// 			{
// 				CUser* pTargetUser = NULL;
// 				pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser));
// 				//			if (pUser->IsWhiteName() && (!pTargetUser->IsEvil()))
// 				if(pTargetUser && !pTargetUser->IsEvil())
// 				{
// 					pUser->SetCrimeStatus();
// 				}
// 			}
// 			else
// 			{
// 				CMonster* pTargetPet = NULL;
// 				pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pTargetPet));
// 				if (pTargetPet && pTargetPet->IsEudemon() && pTargetPet->QueryOwnerUser())
// 				{
// 					//				if (!(pTargetPet->QueryOwnerUser()->IsEvil()) && pUser->IsWhiteName())
// 					if(!pTargetPet->QueryOwnerUser()->IsEvil() && pUser->IsAtkable(pTargetPet))
// 					{
// 						pUser->SetCrimeStatus();
// 					}
// 				}
// 			}
// 			
// 		}
// 		if (!pRole->IsUser() && pTarget)
// 		{
// 			CMonster* pUserPet = NULL;
// 			pRole->QueryObj(OBJ_MONSTER, IPP_OF(pUserPet));	
// 			if(pUserPet && pUserPet->IsAtkable(pTarget) && pUserPet->IsEudemon())
// 			{
// 				if( pTarget->IsUser() && pUserPet->IsAtkable(pTarget))
// 				{
// 					
// 					if (pUserPet->IsEudemon() && pUserPet->QueryOwnerUser())
// 					{
// 						CUser* pTargetUser = NULL;
// 						pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser));
// 						//				if (!(pTargetUser->IsEvil()) && (pUserPet->QueryOwnerUser()->IsWhiteName()))
// 						if(!pTargetUser->IsEvil())
// 						{
// 							pUserPet->QueryOwnerUser()->SetCrimeStatus();
// 						}
// 					}
// 				}
// 			}
// 			
// 			else
// 			{	
// 				CMonster* pTargetPet = NULL;
// 				pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pTargetPet));
// 				
// 				if (pTargetPet && pUserPet->IsEudemon() && pTargetPet->IsEudemon())
// 				{
// 					//				if ((pUserPet->QueryOwnerUser()->IsWhiteName()) && (!pTargetPet->QueryOwnerUser()->IsEvil()) )
// 					if(!pTargetPet->QueryOwnerUser()->IsEvil() && pRole->IsAtkable(pTargetPet))
// 					{
// 						pUserPet->QueryOwnerUser()->SetCrimeStatus();
// 					}
// 				}
// 				
// 			}
// 			
// 		}
 	}
	catch(...)
	{
		LOGERROR("MSGINTERACT Action[%s] Step[%s]",szDebugAction,szDebugStep);
	}
	DEBUG_CATCH2("switch(MSGINTERACT) [%d]", m_pInfo->unType)
		
	return;

#ifdef _MSGDEBUG
	::LogMsg("Process CMsgInteract, id:%u", m_pInfo->id);
#endif
}







