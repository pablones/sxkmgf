// PVPManager.cpp: implementation of the CPVPManager class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "MAPGROUP.h"
#include "User.h"
#include "AllMsg.h"
#include "PVPManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPVPManager::CPVPManager(PROCESS_ID id)
{
	thisChkTime = 0;
	m_idProcess = id;
}

CPVPManager::~CPVPManager()
{
	for(PVPDataSet::iterator it = m_pAllPVP.begin(); it != m_pAllPVP.end() ; ++ it)
	{		
		PVPData* pData = *it;
		CMonster* pMonster = NULL;
		IRole* pRole = RoleManager()->QuerySet()->GetObj(pData->flagID);

		if (pRole && pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
		{
			pMonster->DelMonster(true);
		}
		else
		{
			::LogSave("删除决斗旗时决斗旗无效1");
		}
		delete pData;
	} 
	m_pAllPVP.clear();
}

void CPVPManager::doPVP()
{
	DWORD thisTime = GetTickCount() / 1000;
	bool bEnd	= false;
	if(thisTime <= thisChkTime)
		return;
	for(PVPDataSet::iterator it = m_pAllPVP.begin(); it != m_pAllPVP.end() ; )
	{
		if (*it)
		{
			PVPData* pData = *it;
// 			if ((1 == pData->nMode || 2  == pData->nMode) && thisTime - pData->nBegainTime < 3) //4秒后切磋开始
// 			{
// 				if (pData->userID1 ==0 || pData->userID2 == 0)
// 				{
// 					return;
// 				}
// 				CUser* pUser1 = UserManager()->GetUser(pData->userID1);
// 				CUser* pUser2 = UserManager()->GetUser(pData->userID2);
// 				if (pUser2)
// 				{te
// 					pUser2->AttachStatus()
// 				}
// 				if (pUser1)
// 				{
// 					pUser1->AddBuff(STATUS_IMMUNE_ALL, (3-thisTime + pData->nBegainTime), 0, 0, 0, 0, 0);
// 				}
// 			}
			if ((1 == pData->nMode || 2  == pData->nMode) && thisTime - pData->nBegainTime > 3) //4秒后切磋开始
			{
// 				if (pData->userID1 ==0 || pData->userID2 == 0)
// 				{
// 					///
// 					return;
// 				}
				CUser* pUser1 = UserManager()->GetUser(pData->userID1);
				CUser* pUser2 = UserManager()->GetUser(pData->userID2);
				if(pUser1 && pUser1)
				{  //玩家在  开始切磋
					pUser1->SetPVPStuts(true);
					pUser2->SetPVPStuts(true);
					
					// 				CMsgFriend msg;
					// 				IF_OK(msg.Create(_FRIEND_PVP, pUser1->GetID(), pUser1->GetName(), true,0,0,0,0,0,6))
					// 					pUser2->SendMsg(&msg);
					// 				IF_OK(msg.Create(_FRIEND_PVP, pUser2->GetID(), pUser2->GetName(), true,0,0,0,0,0,6))
					// 					pUser1->SendMsg(&msg);
					pData->nMode += 2;
					it ++;
					continue;
				}
				else
				{   //玩家不在  给在的清场
					if(pUser1)
 					{
						pUser1->SendSysMsg("切磋结束！");
						
						IStatus* pStatus = pUser1->QueryStatus( STATUS_PVPMODE );
						if( pStatus )
						{
							CRole::DetachStatus(pUser1,STATUS_PVPMODE);
						}
						pStatus = pUser1->QueryStatus( STATUS_BIGBLOOD_USERNOT );
						if( pStatus )
						{
							CRole::DetachStatus(pUser1,STATUS_BIGBLOOD_USERNOT);
						}
						bEnd = true;
 					}
 					if(pUser2)
 					{
						pUser2->SetPVPStuts(false);
						pUser2->SendSysMsg("切磋结束！");
						
						IStatus* pStatus = pUser2->QueryStatus( STATUS_PVPMODE );
						if( pStatus )
						{
							CRole::DetachStatus(pUser2,STATUS_PVPMODE);
						}
						pStatus = pUser2->QueryStatus( STATUS_BIGBLOOD_USERNOT );
						if( pStatus )
						{
							CRole::DetachStatus(pUser2,STATUS_BIGBLOOD_USERNOT);
						}
						bEnd = true;
					}
					CMonster* pMonster = NULL;
					IRole* pRole = RoleManager()->QuerySet()->GetObj(pData->flagID);
					if (pRole && pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
					{
						pMonster->DelMonster(true);
					}
					else
					{
						::LogSave("删除决斗旗时决斗旗无效2");
					}
					EndPVP(pData->userID1);
				}
			}	
			
			else if((3 == pData->nMode || 4  == pData->nMode) && thisTime - pData->nBegainTime > 3*60 + 3)//时间到海没完的结束
			{
				CUser* pUser1 = UserManager()->GetUser(pData->userID1);
				CUser* pUser2 = UserManager()->GetUser(pData->userID2);
				if(pUser1 && pUser2)
				{	//玩家在  结束切磋
					pUser1->SetPVPStuts(false);
					pUser2->SetPVPStuts(false);
					
					pUser1->SetPVPLong(false);
					pUser2->SetPVPLong(false);
					
					pUser1->SendSysMsg("切磋时间到，结果：平。");
					pUser2->SendSysMsg("切磋时间到，结果：平。");

 					CMsgFriend msg;
// 					IF_OK(msg.Create(_FRIEND_PVP, pUser1->GetID(), pUser1->GetName(), true,0,0,0,0,0,300))
// 						pUser2->SendMsg(&msg);
// 					IF_OK(msg.Create(_FRIEND_PVP, pUser2->GetID(), pUser2->GetName(), true,0,0,0,0,0,300))
// 						pUser1->SendMsg(&msg);
					IF_OK(msg.Create(_FRIEND_PVP, pUser1->GetID(), pUser1->GetName(), true,0,0,0,0,0,0))
						pUser2->SendMsg(&msg);
					IF_OK(msg.Create(_FRIEND_PVP, pUser2->GetID(), pUser2->GetName(), true,0,0,0,0,0,0))
						pUser1->SendMsg(&msg);	

					EndPVP(pUser1->GetID());
					
				}
				else //结束还没人。。。
				{
					if(pUser1)
					{
// 						pUser1->SetPVPStuts(false);
// 						pUser1->SetPVPLong(false);
// 						pUser1->SendSysMsg("切磋结束！");
// 						
// 						IStatus* pStatus = pUser1->QueryStatus( STATUS_PVPMODE );
// 						if( pStatus )
// 						{
// 							CRole::DetachStatus(pUser1,STATUS_PVPMODE);
// 						}
// 						pStatus = pUser1->QueryStatus( STATUS_BIGBLOOD_USERNOT );
// 						if( pStatus )
// 						{
// 							CRole::DetachStatus(pUser1,STATUS_BIGBLOOD_USERNOT);
// 						}
// 
// 						pUser1->SetPVPReady(false);
// 						CMsgFriend msg;
// 						IF_OK(msg.Create(_FRIEND_PVP, pUser2->GetID(), pUser2->GetName(), true,0,0,0,0,0,0))
// 							pUser1->SendMsg(&msg);
						EndPVP(pUser1->GetID());
						bEnd = true;
					}
					if(pUser2)
					{
// 						pUser2->SetPVPStuts(false);
// 						pUser2->SetPVPLong(false);
// 						pUser2->SendSysMsg("切磋结束！");
// 						
// 						IStatus* pStatus = pUser2->QueryStatus( STATUS_PVPMODE );
// 						if( pStatus )
// 						{
// 							CRole::DetachStatus(pUser2,STATUS_PVPMODE);
// 						}
// 						pStatus = pUser2->QueryStatus( STATUS_BIGBLOOD_USERNOT );
// 						if( pStatus )
// 						{
// 							CRole::DetachStatus(pUser2,STATUS_BIGBLOOD_USERNOT);
// 						}
// 
// 						pUser2->SetPVPReady(false);
// 						CMsgFriend msg;
// 						IF_OK(msg.Create(_FRIEND_PVP, pUser1->GetID(), pUser1->GetName(), true,0,0,0,0,0,0))
// 							pUser2->SendMsg(&msg);	
						EndPVP(pUser2->GetID());
						bEnd = true;
					}
				}
				bEnd = true;
				CMonster* pMonster = NULL;
				IRole* pRole = RoleManager()->QuerySet()->GetObj(pData->flagID);
				if (pRole && pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
				{
					pMonster->DelMonster(true);
				}
				else
				{
					::LogSave("删除决斗旗时决斗旗无效3");
				}
				
//				NpcManager()->DelMonsterByID(pData->flagID);
// 				PVPDataSet::iterator itTemp = it;
// 				it ++;
// 				delete pData;
// 				m_pAllPVP.erase(itTemp);
// 				continue;
			}
			else //战斗中 距离判断
			{
				bool bOut	= false;
				CUser* pUser1 = UserManager()->GetUser(pData->userID1);
				CUser* pUser2 = UserManager()->GetUser(pData->userID2);
				if(pUser1 && !bEnd)
				{
					
					
					if(pUser1->GetMapID() != pUser1->GetMapID()
						|| abs(pUser1->GetPosX() - pUser1->GetPVPX()) > 20
						|| abs(pUser1->GetPosY() - pUser1->GetPVPY()) > 20)	
					{
						if (!pData->nOutTime1 && (pUser1->GetMapID() == pUser2->GetMapID()))
						{
							pData->nOutTime1 = thisTime;
							CMsgFriend Cmsg;
							IF_OK(Cmsg.Create(_FRIEND_PVP, pUser1->GetID(), pUser1->GetName(), true,0,0,0,0,0,9))			//告诉客户端 我出去了
								pUser1->SendMsg(&Cmsg);
						}
						if (thisTime - pData->nOutTime1 >= 10)
						{
						//	bEnd = true;
							pUser1->SendSysMsg("切磋结束");
							pUser2->SendSysMsg("切磋结束");
							if (findPVP(pData->userID1))
							{
								EndPVP(pUser1->GetID(), pUser2->GetID());
								bEnd = true;
							}
						}
					}
					else if(pData->nOutTime1 && !bEnd)
					{
						pData->nOutTime1 = 0;
						CMsgFriend Cmsg;
						IF_OK(Cmsg.Create(_FRIEND_PVP, pUser1->GetID(), pUser1->GetName(), true,0,0,0,0,0,8))			//告诉客户端 我回来了
							pUser1->SendMsg(&Cmsg);
					}
				}
				else
				{
					if (findPVP(pData->userID1))
					{
						EndPVP(pData->userID1);
						bEnd = true;
					}
				}
				
				if(pUser2 && !bEnd)
				{
		
					if(pUser2->GetMapID() != pUser2->GetMapID()
						|| abs(pUser2->GetPosX() - pUser2->GetPVPX()) > 20
						|| abs(pUser2->GetPosY() - pUser2->GetPVPY()) > 20)	
					{
						if (!pData->nOutTime2 && (pUser1->GetMapID() == pUser2->GetMapID()))
						{
							pData->nOutTime2 = thisTime;
							CMsgFriend Cmsg;
							IF_OK(Cmsg.Create(_FRIEND_PVP, pUser2->GetID(), pUser2->GetName(), true,0,0,0,0,0,9))
								pUser2->SendMsg(&Cmsg);
						}
						if (thisTime - pData->nOutTime2 >= 10)
						{
						//	bEnd = true;
							pUser1->SendSysMsg("切磋结束");
							pUser2->SendSysMsg("切磋结束");
							if (findPVP(pData->userID1))
							{
								EndPVP(pUser2->GetID(), pUser1->GetID());
								bEnd = true;
							}
						}
					}
					else if(pData->nOutTime2 && !bEnd)
					{
						pData->nOutTime2 = 0;
						CMsgFriend Cmsg;
						IF_OK(Cmsg.Create(_FRIEND_PVP, pUser2->GetID(), pUser2->GetName(), true,0,0,0,0,0,8))			//告诉客户端 我回来了
							pUser2->SendMsg(&Cmsg);
					}
		
				}
				else
				{
					if (findPVP(pData->userID1))
					{
						EndPVP(pData->userID1);
						bEnd = true;
					}
				}			
			}
			if(bEnd)
			{
				PVPDataSet::iterator itTemp = it;
				it++;
				delete pData;
				m_pAllPVP.erase(itTemp);
			}
			else
				it++;
		}
	}

}                       

void CPVPManager::AddPVP(OBJID id1,OBJID id2,int mode,OBJID flagID)
{
	PVPData* pData = new PVPData;
	if(pData)
	{
		pData->nMode = mode;
		pData->userID1 = id1;
		pData->userID2 = id2;
		pData->flagID = flagID;
		pData->nBegainTime = GetTickCount() / 1000;
		pData->nOutTime1 = 0;
		pData->nOutTime2 = 0;

		m_pAllPVP.push_back(pData);
	}
}

//终止 winner为胜利方
void CPVPManager::EndPVP(OBJID id1,OBJID winner, bool bKill)
{
	for(PVPDataSet::iterator it = m_pAllPVP.begin(); it != m_pAllPVP.end() ;)
	{
		PVPData* pData = *it;		
		if(pData->userID1 == id1 || pData->userID2 == id1)
		{
			if(winner != 0)
			{
				if(pData->userID1 == winner || pData->userID2 == winner)
				{
					CUser* pUser1 = NULL;
					CUser* pUser2 = NULL;
					if(pData->userID1 == winner)
					{
						pUser1 = UserManager()->GetUser(pData->userID1);
						pUser2 = UserManager()->GetUser(pData->userID2);
						if(pUser1 && pUser2) //人全在
						{
							MSGBUF	szMsg;
							CMsgTalk MapMsg;
							sprintf(szMsg,"玩家%s技高一筹在切磋中战胜了玩家%s。", pUser1->GetName(), pUser2->GetName());//STR_MAKE_FRIEND
							MapMsg.Create( SYSTEM_NAME , ALLUSERS_NAME ,szMsg,NULL,0xfefeefef,_TXTATR_MAP );
							UserManager()->BroadcastMapMsg( pUser1->GetMapID() , &MapMsg,NULL);

						}
					}
					else if(pData->userID2 == winner)
					{
						pUser1 = UserManager()->GetUser(pData->userID1);
						pUser2 = UserManager()->GetUser(pData->userID2);
						if(pUser1 && pUser2) //人全在
						{
							MSGBUF	szMsg;
							CMsgTalk MapMsg;
							sprintf(szMsg,"玩家%s技高一筹在切磋中战胜了玩家%s。", pUser2->GetName(), pUser1->GetName());//STR_MAKE_FRIEND
							MapMsg.Create( SYSTEM_NAME , ALLUSERS_NAME ,szMsg,NULL,0xfefeefef,_TXTATR_MAP );
							UserManager()->BroadcastMapMsg( pUser1->GetMapID() , &MapMsg,NULL);
						}
					}
						
					if(pUser1 && pUser2) //人全在
					{
//						MSGBUF	szMsg;
//						sprintf(szMsg,"玩家 %s 技高一筹在切磋中战胜了玩家 %s。", pUser1->GetName(), pUser2->GetName());//STR_MAKE_FRIEND
//						pUser1->BroadcastRoomMsg(szMsg,true);
//						pUser1->SendSysMsg(_TXTATR_MAP, szMsg);
						
//						CMsgFriend msg;
// 						IF_OK(msg.Create(_FRIEND_PVP, pUser1->GetID(), pUser1->GetName(), true,0,0,0,0,0,200))
// 							pUser2->SendMsg(&msg);
// 						IF_OK(msg.Create(_FRIEND_PVP, pUser2->GetID(), pUser2->GetName(), true,0,0,0,0,0,100))
// 							pUser1->SendMsg(&msg);
					}
					else //有人不在？？
					{
						if(pUser1)
						{
							pUser1->SendSysMsg("切磋结束！");
						}
						if(pUser2)
						{
							pUser2->SendSysMsg("切磋结束！");
						}
					}
				}
			}
			else //结束没胜利者
			{
				CUser* pUser1 = NULL;
				CUser* pUser2 = NULL;
				if(pData->userID1 == id1)
				{
					pUser2 = UserManager()->GetUser(pData->userID1);
					pUser1 = UserManager()->GetUser(pData->userID2);
				}
				else if(pData->userID2 == id1)
				{
					pUser1 = UserManager()->GetUser(pData->userID1);
					pUser2 = UserManager()->GetUser(pData->userID2);
				}
				
				if(pUser1 && pUser2) //人全在
				{
// 					MSGBUF	szMsg;
					CMsgTalk MapMsg;
//					sprintf(szMsg,"玩家%和玩家%s在切磋中战成了平手！", pUser2->GetName(), pUser1->GetName());//STR_MAKE_FRIEND
//					MapMsg.Create( SYSTEM_NAME , ALLUSERS_NAME ,szMsg,NULL,0xfefeefef,_TXTATR_MAP );
//					UserManager()->BroadcastMapMsg( pUser1->GetMapID() , &MapMsg,NULL);
						
					CMsgFriend msg;
					IF_OK(msg.Create(_FRIEND_PVP, pUser1->GetID(), pUser1->GetName(), true,0,0,0,0,0,0))
						pUser2->SendMsg(&msg);
					IF_OK(msg.Create(_FRIEND_PVP, pUser2->GetID(), pUser2->GetName(), true,0,0,0,0,0,0))
						pUser1->SendMsg(&msg);
				}
				else //有人不在？？
				{
					if(pUser1)
					{
						pUser1->SendSysMsg("切磋结束！");
					}
					if(pUser2)
					{
						pUser2->SendSysMsg("切磋结束！");
					}
				}
			}

			CUser* pUser1 = UserManager()->GetUser(pData->userID1); //结束后清场
			if(pUser1)
			{
				pUser1->SetPVPLong(false);
				pUser1->SetPVPStuts(false);
				IStatus* pStatus = pUser1->QueryStatus( STATUS_PVPMODE );
				if( pStatus )
				{
					CRole::DetachStatus(pUser1,STATUS_PVPMODE);
				}
				pStatus = pUser1->QueryStatus( STATUS_BIGBLOOD_USERNOT );
				if( pStatus )
				{
					CRole::DetachStatus(pUser1,STATUS_BIGBLOOD_USERNOT);
				}
				CMsgFriend msg;
				IF_OK(msg.Create(_FRIEND_PVP, pUser1->GetID(), pUser1->GetName(), true,0,0,0,0,0,0))
					pUser1->SendMsg(&msg);
			}
			
			CUser* pUser2 = UserManager()->GetUser(pData->userID2);
			if(pUser2)
			{
				pUser2->SetPVPLong(false);
				pUser2->SetPVPStuts(false);
				IStatus* pStatus = pUser2->QueryStatus( STATUS_PVPMODE );
				if( pStatus )
				{
					CRole::DetachStatus(pUser2,STATUS_PVPMODE);
				}
				pStatus = pUser2->QueryStatus( STATUS_BIGBLOOD_USERNOT );
				if( pStatus )
				{
					CRole::DetachStatus(pUser2,STATUS_BIGBLOOD_USERNOT);
				}
				CMsgFriend msg;
				IF_OK(msg.Create(_FRIEND_PVP, pUser2->GetID(), pUser2->GetName(), true,0,0,0,0,0,0))
						pUser2->SendMsg(&msg);
			}

//			NpcManager()->DelMonsterByID(pData->flagID);
	
			CMonster* pMonster = NULL;
			IRole* pRole = RoleManager()->QuerySet()->GetObj(pData->flagID);
			if (pRole && pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
			{
				pMonster->DelMonster(true);
			}
			else
			{
				::LogSave("删除决斗旗时决斗旗无效4");
			}

// 			PVPDataSet::iterator itTemp = it;
// 			it ++;
// 			delete pData;
// 			m_pAllPVP.erase(itTemp);
			if (bKill)
			{
				PVPDataSet::iterator itTemp = it;
				it ++;
				delete pData;
				m_pAllPVP.erase(itTemp);
			}
			return;
		}
		else
		{
			it ++;
		}
		
	}
}

bool CPVPManager::findPVP(OBJID id1)
{
	for(PVPDataSet::iterator it = m_pAllPVP.begin(); it != m_pAllPVP.end() ; ++ it)
	{
		PVPData* pData = *it;		
		if(pData->userID1 == id1 || pData->userID2 == id1)
		{
			return true;
		}
	}
	return false;
}

void CPVPManager::SetLong(OBJID id1)
{
	CUser* pUser = UserManager()->GetUser(id1);
	if(pUser)
	{
		if(!pUser->IsPVPLong())
		{
			pUser->SetPVPLong(true);
			CMsgFriend msg;
			IF_OK(msg.Create(_FRIEND_PVP, pUser->GetID(), pUser->GetName(), true,0,0,0,0,0,10))
				pUser->SendMsg(&msg);
		}
	}
}
void CPVPManager::ClrLong(OBJID id1)
{
	CUser* pUser = UserManager()->GetUser(id1);
	if(pUser)
	{
		if(pUser->IsPVPLong())
		{
			pUser->SetPVPLong(false);
			CMsgFriend msg;
			IF_OK(msg.Create(_FRIEND_PVP, pUser->GetID(), pUser->GetName(), true,0,0,0,0,0,11))
				pUser->SendMsg(&msg);
		}
	}

}
