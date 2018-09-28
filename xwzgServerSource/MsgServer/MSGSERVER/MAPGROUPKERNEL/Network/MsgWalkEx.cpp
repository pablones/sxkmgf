// MsgAction.cpp: implementation of the CMsgWalkEx class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "mapgroup.h"
#include "transformation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgWalkEx::CMsgWalkEx()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgWalkEx::~CMsgWalkEx()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgWalkEx::Create(OBJID idPlayer, int nDir, int nMode, int x, int y,int walkInt/* = 0*/,int walkEnd/* = 0*/)
{
	// param check
	if (idPlayer == ID_NONE)
		return false;

	// init
	this->Init();

	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_WALKEX;

	m_pInfo->idUser		=idPlayer;
	m_pInfo->ucDir		=nDir;
	m_pInfo->ucMode		=nMode;
	m_pInfo->usPosX		=x;
	m_pInfo->usPosY		=y;

	m_pInfo->walkEnd	=walkEnd;
	m_pInfo->walkInt	=walkInt;
	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgWalkEx::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_WALKEX != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgWalkEx::Process(void *pInfo)
{
	DEBUG_TRY
	const int WALK_MASK = 1234;
	bool bMonster = false;
	// get obj
	IRole* pRole	= RoleManager()->QueryRole(this, m_pInfo->idUser);	
	if(!pRole)
	{
		return ;
	}

	int nDir = m_pInfo->ucDir%10;
	int nNewX = pRole->GetPosX()+_DELTA_X[nDir];
	int nNewY = pRole->GetPosY()+_DELTA_Y[nDir];
	CMonster* pMonster = NULL;
	CUser* pUser = NULL;
	if(pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
	{
		if(!pUser->CheckMsgData(CHECKDATA_WALK,150))
			return;
//modify code by 林德渊 begin 2011.5.19
// 		if( g_StatisticalData.Addwalkcount((unsigned long) m_pInfo->idUser) )
// 		{
// 			UserManager()->KickOutSocket(pUser->GetSocketID(), "> maxwalkcount");
// 			g_StatisticalData.EraseMaxWalk();
// 			char szTemp[256] = {0};
// 			if( pUser )
// 				sprintf(szTemp,"KickOutSocket [%s] > maxwalkcount",pUser->GetName());
// 			::LogSave(szTemp);
// 		}
//modify code by 林德渊 end
		///转场景途中不处理walk消息
		if(pUser->CanBeSee()==false)
			return;

		//摆摊 不许走动
		if(pUser->QueryBooth())
		{
			pUser->KickBack();
			return;
		}
		if (pUser->IsVerificationCoding())//跑商验证
		{
			pUser->KickBack();
			return;
		}
		if(!pUser->IsAlive())
		{
			pUser->KickBack();
			return;
		}

		if(pUser->GetPosX() != m_pInfo->usPosX || pUser->GetPosY() != m_pInfo->usPosY)
		{
			pUser->KickBack();
			return;
		}

		if(!pUser->GetMap()->IsStandEnable(nNewX, nNewY))
		{
			pUser->KickBack();
			return;
		}
 		// 跑步模式的阻挡判断
 		if (m_pInfo->ucMode >= MOVEMODE_RUN_DIR0 && m_pInfo->ucMode <= MOVEMODE_RUN_DIR7)
 		{
 			nNewX += _DELTA_X[m_pInfo->ucMode - MOVEMODE_RUN_DIR0];
 			nNewY += _DELTA_Y[m_pInfo->ucMode - MOVEMODE_RUN_DIR0];

		
			if(!pUser->GetMap()->IsStandEnable(nNewX, nNewY))
			{
				pUser->KickBack();
				return;
			}
		}
	}
	else if(pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
	{
		if(pMonster->IsEudemon())
		{
			bool bRunMode = (m_pInfo->ucMode >= MOVEMODE_RUN_DIR0 && m_pInfo->ucMode <= MOVEMODE_RUN_DIR7);
			if (!bRunMode)
			{
				if(!pMonster->GetMap()->IsValidPoint(nNewX, nNewY))
				{
					pMonster->KickBack();
					return;
				}
				
				pRole->MoveToward(nDir);
			}
			else
			{
				if(!pMonster->GetMap()->IsValidPoint(nNewX, nNewY))
				{
					pMonster->KickBack();
					return;
				}

				nNewX += _DELTA_X[m_pInfo->ucMode - MOVEMODE_RUN_DIR0];
 				nNewY += _DELTA_Y[m_pInfo->ucMode - MOVEMODE_RUN_DIR0];

				if(!pMonster->GetMap()->IsValidPoint(nNewX, nNewY))
				{
					pMonster->KickBack();
					return;
				}

				pRole->MoveToward(nDir, false);
			}

			if(bRunMode)
				pRole->MoveToward(m_pInfo->ucMode - MOVEMODE_RUN_DIR0);

			CMsgWalkEx msg;
			IF_OK(msg.Create(m_pInfo->idUser, m_pInfo->ucDir, m_pInfo->ucMode, nNewX, nNewY,m_pInfo->walkInt,m_pInfo->walkEnd))
			{
				pRole->BroadcastRoomMsg(&msg, INCLUDE_SELF);
			}
			return;
		}

		bMonster = true;
	}

	DEBUG_TRY
		pRole->ProcessOnMove(m_pInfo->ucMode);
	DEBUG_CATCH("ProcessOnMove")

	int step = 0;
	DEBUG_TRY
		//房间同步		
		CMsgWalkEx msg;
		IF_OK(msg.Create(m_pInfo->idUser, m_pInfo->ucDir, m_pInfo->ucMode, nNewX, nNewY,m_pInfo->walkInt,m_pInfo->walkEnd))
		{
			step = 1;
			if(pRole->IsUser())
			{
				step = 2;
				pRole->BroadcastRoomMsg(&msg, INCLUDE_SELF);
			}
			else
			{
				step = 3;
				pRole->BroadcastRoomMsg(&msg,INCLUDE_SELF,EM_SENDNPCFLAG_FALSE);
			}
		}
		
	
	
		step = 4;
		bool bRunMode = (m_pInfo->ucMode >= MOVEMODE_RUN_DIR0 && m_pInfo->ucMode <= MOVEMODE_RUN_DIR7);
		if (bMonster || !bRunMode)
		{
			if(pUser)
			{
				if(pUser->CanBeSee())
					pRole->MoveToward(nDir);
			}
			else
			{
				pRole->MoveToward(nDir);
			}
		}
		else
		{
			if(pUser)
			{
				if(pUser->CanBeSee())
					pRole->MoveToward(nDir, false);
			}
			else
			{
				pRole->MoveToward(nDir, false);
			}
			
		}
		step = 5;
 		if (!bMonster && bRunMode)
 		{
			if(pUser)
			{
				if(pUser->CanBeSee())
					pRole->MoveToward(m_pInfo->ucMode - MOVEMODE_RUN_DIR0);
			}
			else
			{
				pRole->MoveToward(m_pInfo->ucMode - MOVEMODE_RUN_DIR0);
			}
		}
		step = 7;

	}
	catch(...)
	{
		char szTemp[256];
		sprintf(szTemp,"MoveTowardEX %d,%d,%d,%d,%d,%d",
			m_pInfo->idUser,
			m_pInfo->ucDir,
			m_pInfo->ucMode,
			m_pInfo->usPosX,
			m_pInfo->usPosY,
			step);
		::LogSave(szTemp);
	}

// #ifdef	PALED_DEBUG_X
// 	MSGBUF	szMsg;
// 	if(IsNpcID(pRole->GetID()))
// 		sprintf(szMsg, "%s%06dWALK: (%d,%d)", pRole->GetName(), pRole->GetID(), pRole->GetPosX(), pRole->GetPosY());
// 	else
// 		sprintf(szMsg, "%sWALK: (%d,%d)", pRole->GetName(), pRole->GetPosX(), pRole->GetPosY());
// 	//LOGWARNING(szMsg);
// 	DebugSave(szMsg);
// #endif
		
	DEBUG_CATCH("CMsgWalk::Process")
}
