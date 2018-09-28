// StatusOnce.cpp: implementation of the CStatusMore class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "windows.h"
#include "../share/define.h"
#include "I_Role.h"
#include "TimeOut.h"
#include "StatusMore.h"
#include "magic.h"



MYHEAP_IMPLEMENTATION(CStatusMore,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CStatusMore::CStatusMore()
{
	m_pOwner	= NULL;
	m_nStatus	= 0;
	m_idAttker	= 0;
	m_nQuestionAmount = 1;
}

//////////////////////////////////////////////////////////////////////
CStatusMore::~CStatusMore()
{
//	if(m_pOwner)
//	{
//		m_pOwner->ClsStatus( m_nStatus );
//		m_pOwner->ClsBuff( m_nStatus );
//	}
}

//////////////////////////////////////////////////////////////////////
int CStatusMore::GetPower()
{
	return m_nData;
}

//////////////////////////////////////////////////////////////////////
bool CStatusMore::GetInfo(StatusInfoStruct* pInfo)
{
	pInfo->nPower	= m_nData;
	pInfo->nStatus	= m_nStatus;
	pInfo->nSecs	= m_tKeep.GetInterval();
	pInfo->nTimes	= m_nTimes;			//? mast >0
	pInfo->nParam1	= m_tKeep.GetRemain();
	pInfo->nParam2	= GetParam2();
	pInfo->nParam3	= GetParam3();

	return IsValid();		// m_nTimes > 0
}

//////////////////////////////////////////////////////////////////////
//����true��ʾû��ɾ��buff,����false��ʾ��ɾ��buff
bool CStatusMore::OnTimer(DWORD tCurr)
{
	bool bRet = true;
	DEBUG_TRY
	if(!IsValid())
		return true;
	if( m_tKeep.ToNextTime() )
	{
		if (m_nStatus != STATUS_BOOTH_ADDEXP)//��̯�Ӿ���BUFF�޴������ƣ�ֻ����̯ʱɾ��
			m_nTimes--;
		if (m_nStatus == STATUS_BOOTH_ADDEXP)
		{
			CUser* pUser = NULL;
			if (m_pOwner->QueryObj(OBJ_USER, (void**)&pUser))
			{
				if (pUser->QueryBooth())
				{
					int nAwardExp = pUser->GetLev()*10;//��õľ���Ϊ����ȼ���10��
					if (pUser->AwardBattleExp(nAwardExp))
						pUser->SendSysMsg(_TXTATR_TOPSHOW, "������%u����", nAwardExp);
				}
			}
		}
		if (m_nStatus == STATUS_ITEM_ADDEXP)
		{
			CUser* pUser = NULL;
			if (m_pOwner->QueryObj(OBJ_USER, (void**)&pUser))
			{
				//m_nParam2���ڴ���Ч�ĵ�ͼ
				if (pUser->GetMap() && (pUser->GetMapID() == m_nParam2))
				{
					float addFactor=pUser->CalItemAddExpTeamFactor();///�õ�������֮���buff�ӳ�ϵ��

// 					char szWords[128] = "";
// 					sprintf(szWords, "����ӳ�ϵ��%f",addFactor);
// 					UserManager()->BroadCastMsgAndReceiveName(szWords, /*pUser*/NULL, 0, 0xffffff, _TXTATR_WORLDSERVER,pUser,0,0,"",ALLUSERS_NAME);

//					pUser->SendSysMsg(_TXTATR_NORMAL, "����ӳ�ϵ��%f",addFactor);

					if(addFactor>2.5 || addFactor<0)///����ϵͳ�������
					{
						::LogSave("CalItemAddExpTeamFactor error value >maxvalue 2.4 fix it!userid:%d,addFactor:%d",pUser->GetID(),addFactor);
						addFactor=0;
					}
					int nAwardExp = GetPower()+(GetPower()*addFactor);//��õľ���Ϊ����ȼ���10��
					if (pUser->AwardBattleExp(nAwardExp))
						pUser->SendSysMsg(_TXTATR_TOPSHOW, "������%u����", nAwardExp);
				}
			}
		}
		if (m_nStatus == STATUS_ACTIVATE_ADDEXP)
		{
			CUser* pUser = NULL;
			if (m_pOwner->QueryObj(OBJ_USER, (void**)&pUser))
			{
				//�����پ������
				int nAwardExp = pUser->GetLev()*180;
				if (pUser->AwardBattleExp(nAwardExp))
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "������%u����", nAwardExp);
			}
		}
		if (m_nStatus == STATUS_DOT_SYNEXP)
		{
			CUser* pUser = NULL;
			if (m_pOwner->QueryObj(OBJ_USER, (void**)&pUser))
			{
				//�����پ������--
				if (pUser->GetMap()==pUser->GetSynMap())
				{
					if (pUser->AwardBattleExp(m_nData*pUser->GetSyndicate()->GetOnlineAmount()))
						pUser->SendSysMsg(_TXTATR_TOPSHOW, "������%u����", m_nData*pUser->GetSyndicate()->GetOnlineAmount());
				}
			}
		}
		if (m_nStatus == STATUS_DOT_WARGAMEEXP)
		{
			if(m_pOwner->IsAlive())
			{
				CUser* pUser = NULL;
				if (m_pOwner->QueryObj(OBJ_USER, (void**)&pUser))
				{
					if (m_nParam2==0)
					{
						int nAwardExp = GetPower();
						if (pUser->AwardBattleExp(nAwardExp))
							pUser->SendSysMsg(_TXTATR_TOPSHOW, "������%u����", nAwardExp);
					}
					else if (pUser->GetMap() && (pUser->GetMap()->GetDocID() == m_nParam2))
					{
						int nAwardExp = GetPower();
						if (pUser->AwardBattleExp(nAwardExp))
							pUser->SendSysMsg(_TXTATR_TOPSHOW, "������%u����", nAwardExp);
					}
				}
			}
		}
		if (m_nStatus == STATUS_DOT_TUTOREXP)
		{
			CUser* pUser = NULL;
			if (m_pOwner->QueryObj(OBJ_USER, (void**)&pUser))
			{
				if (m_nParam2==0)
				{
					int nAwardExp = GetPower();
					if (pUser->AwardBattleExp(nAwardExp))
						pUser->SendSysMsg(_TXTATR_TOPSHOW, "������%u����", nAwardExp);
				}
				else if (pUser->GetMap() && (pUser->GetMap()->GetDocID() == m_nParam2))
				{
					int nAwardExp = GetPower();
					if (pUser->AwardBattleExp(nAwardExp))
						pUser->SendSysMsg(_TXTATR_TOPSHOW, "������%u����", nAwardExp);
				}
				//ʦͽ����
				if(m_tQuestion.ToNextTime())
				{
					CTeam* pTeam = pUser->GetTeam();
					if (pTeam && pTeam->GetLeader()==pUser->GetID())
					{
						int nTeamAmount = pTeam->GetMemberAmount();
						CTutorData* pData = NULL;
						int nRad = RandGet(999999);
						if(nRad==0)
							nRad=1;
						bool bHasStuInTeam = false;

						CMsgSchoolMember msg;
						if(!msg.Create(pUser->GetID(), MESSAGEBOARD_QUESTION, NULL, 0, 0))
							return true;
						ST_MEMBERINFO info = {0};
						info.idMember = nRad;
						info.dwMaxScore = m_nQuestionAmount;
						msg.Append(info);

						for(int i = 0; i < nTeamAmount; i++)
						{
							OBJID idMember = pTeam->GetMemberByIndex(i);
							CUser* pTempUser = NULL;
							pUser->FindAroundObj( OBJ_USER,idMember,( void** )&pTempUser );
							if(pTempUser && pTempUser->IsAlive() && (pTempUser->GetID() != pUser->GetID()))
							{
								if(!pTempUser->QueryStatus(STATUS_DOT_TUTOREXP))
									continue;
								pData = pUser->GetStudent(pTempUser->GetID());
								if (pData && pData->GetState()==TUTORSTATE_NORMAL)//��ͽ��
								{
									pTempUser->SetTutorAnswer(0);
									pTempUser->SetTutorQuestion(nRad);
									bHasStuInTeam = true;
									msg.m_pInfo->idUser = pTempUser->GetID();
									pTempUser->SendMsg(&msg);
								}
							}
						}

						if(bHasStuInTeam)
						{
							pUser->SetTutorAnswer(0);//����ǰ��ѡ����Ϊ0
							pUser->SetTutorQuestion(nRad);
							msg.m_pInfo->idUser = pUser->GetID();
							msg.m_pInfo->ucPage = 1;
							pUser->SendMsg(&msg);
							m_nQuestionAmount++;
						}
					}
				}
			}
		}
		if(m_nStatus == STATUS_DOT_ADDEXP)
		{
			CUser* pUser = NULL;
			if (m_pOwner->QueryObj(OBJ_USER, (void**)&pUser))
			{
				if (m_nParam2==0)
				{
					int nAwardExp = GetPower();
					if (pUser->AwardBattleExp(nAwardExp))
						pUser->SendSysMsg(_TXTATR_TOPSHOW, "������%u����", nAwardExp);//"������%u����"
				}
				else if (pUser->GetMap() && (pUser->GetMap()->GetDocID() == m_nParam2))
				{
					int nAwardExp = GetPower();
					if (pUser->AwardBattleExp(nAwardExp))
						pUser->SendSysMsg(_TXTATR_TOPSHOW,"������%u����", nAwardExp);//"������%u����"
				}
			}
		}
		//��ʱ�����ж���Ѫ��Ч
		if ( STATUS_DOT_LIFEMISSING == m_nStatus || STATUS_DOT_BURNING == m_nStatus || STATUS_DOT_SUFFERING == m_nStatus || STATUS_DOT_POISONING == m_nStatus
			|| STATUS_DOT_LIFEMISS1 == m_nStatus)
		{
			if(m_pOwner->IsAlive())
			{
				//ʦͽ�����������Ĵ���
				CMonster* pTargetMonster = NULL;
				if (m_pOwner->QueryObj(OBJ_MONSTER, (void**)&pTargetMonster))
				{
					if (!pTargetMonster->IsEudemon() && pTargetMonster->IsTutorBoss())
					{
						return bRet;
					}
				}
				int nPower = m_nData;
				if (m_pOwner->IsUser())
					nPower = __max(1, m_nData/10);
				//int nLoseLife = ::CutOverflow(m_nData, (int)m_pOwner->GetLife() - 1);		// ʣ��1��Ѫ
				int nLoseLife = ::CutOverflow(nPower, (int)m_pOwner->GetLife());//goto �߻�Ҫ������ж���
				/*-----------------------------------------------------------------*/
//                 if(STATUS_DOT_ENMITY == m_nStatus)
// 				    nLoseLife = 0;
				/*-----------------------------------------------------------------*/
				// effect
				if (m_pOwner->IsUser())
				{
					CUser* pTargetUser = NULL;
					if (m_pOwner->QueryObj(OBJ_USER, (void**)&pTargetUser))
					{
						if(!pTargetUser)
							return bRet;
						nLoseLife = pTargetUser->PVPEmendDamage(nLoseLife, m_idAttker);
						bRet = false;
					}
				}
				CMsgMagicEffect	msg;
				IF_OK(msg.Create(m_idAttker, m_nStatus, 0, m_pOwner->GetID(), nLoseLife, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
					m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
				
				m_pOwner->AddAttrib(_USERATTRIB_LIFE, -1*nLoseLife, SYNCHRO_TRUE);
				if(!m_pOwner->IsAlive())
				{
					if (m_pOwner->IsUser() && m_idAttker!=ID_NONE)
					{
						CUser* pUser = NULL;
						if (m_pOwner->QueryObj(OBJ_USER, (void**)&pUser))
						{
							//if(pUser->IsPVPStuts() && pUser->GetPVPTargetID() == m_idAttker)
							if(pUser)
								pUser->BeKillByDot(m_idAttker);
						}
					}
               		else
					{
						CMsgInteract msg;
						if(msg.Create( INTERACT_KILL,m_idAttker,m_pOwner->GetID(),m_pOwner->GetPosX(),m_pOwner->GetPosY(),DIE_NORMAL,0 ) )
							m_pOwner->BroadcastRoomMsg(&msg, true,EM_SENDNPCFLAG_ENFORCE);
						if (m_idAttker)//ɱ����������Ҫ����
						{
							CMonster* pMonster = NULL;
							if (m_pOwner->QueryObj(OBJ_MONSTER, (void**)&pMonster))
							{
								//if(pUser->IsPVPStuts() && pUser->GetPVPTargetID() == m_idAttker)
								if(pMonster)
									pMonster->BeKillByDot(m_idAttker);
							}
						}
						else
							m_pOwner->BeKill(NULL);		// ����PK
					}
					bRet = false;//������ɾ��buff
				}
			}
			
		}
		else if ( STATUS_DOT_REDMANA == m_nStatus )//����
		{
			if(m_pOwner->IsAlive() && m_pOwner->IsUser())//ֻ��������
			{
				int nPower = m_nData;
				if (m_pOwner->IsUser())
					nPower = __max(1, m_nData/10);
				int nLoseMana = ::CutOverflow(m_nData, (int)m_pOwner->GetMana());

				CMsgMagicEffect	msg;
				IF_OK(msg.Create(m_idAttker, m_nStatus, 0, m_pOwner->GetID(), nLoseMana, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
					m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
				
				m_pOwner->AddAttrib(_USERATTRIB_MANA, -1*nLoseMana, SYNCHRO_TRUE);
			}
		}
		else if ( STATUS_DOT_REDSP == m_nStatus )//��ŭ��
		{
			if(m_pOwner->IsAlive() && m_pOwner->IsUser())//ֻ������ŭ��
			{
				int nPower = m_nData;
				if (m_pOwner->IsUser())
					nPower = __max(1, m_nData/10);
				int nLoseSp = ::CutOverflow(m_nData, (int)((CUser*)m_pOwner)->GetSp());
				
				CMsgMagicEffect	msg;
				IF_OK(msg.Create(m_idAttker, m_nStatus, 0, m_pOwner->GetID(), nLoseSp, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
					m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
				
				m_pOwner->AddAttrib(_USERATTRIB_SP, -1*nLoseSp, SYNCHRO_TRUE);
			}
		}
// 		else if ( STATUS_DOT_SUFFERING == m_nStatus )//����
// 		{
// 			if(m_pOwner->IsAlive())
// 			{
// 				int nLoseLife = ::CutOverflow(m_nData, (int)m_pOwner->GetLife() - 1);		// ʣ��1��Ѫ
// 				// effect
// 				CMsgMagicEffect	msg;
// 				IF_OK(msg.Create(m_pOwner->GetID(), m_nStatus, 0, m_pOwner->GetID(), nLoseLife, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
// 					m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
// 				
// 				m_pOwner->AddAttrib(_USERATTRIB_LIFE, -1*nLoseLife, SYNCHRO_TRUE);
// 				if(!m_pOwner->IsAlive())
// 					m_pOwner->BeKill(NULL);		// ����PK
// 			}
// 			
// 		}
// 		else if ( STATUS_DOT_SUFFERING_PRE == m_nStatus )// ʹ�����2 [12/13/2007 �½���]
// 		{
// 			if(m_pOwner->IsAlive())
// 			{
// 				int nLoseLife = ::CutOverflow(m_nData, (int)m_pOwner->GetLife() - 1);		// ʣ��1��Ѫ
// 				// effect
// 				CMsgMagicEffect	msg;
// 				IF_OK(msg.Create(m_pOwner->GetID(), m_nStatus, 0, m_pOwner->GetID(), nLoseLife, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
// 					m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
// 				
// 				m_pOwner->AddAttrib(_USERATTRIB_LIFE, -1*nLoseLife, SYNCHRO_TRUE);
// 				if(!m_pOwner->IsAlive())
// 					m_pOwner->BeKill(NULL);		// ����PK
// 			}
// 			
// 		}
// 		else if( STATUS_DOT_BEATING == m_nStatus ) //���
// 		{
// 			if(m_pOwner->IsAlive())
// 			{
// 				//int nli = m_pOwner->GetLife();
// 				int nLoseLife = ::CutOverflow(m_nData, (int)m_pOwner->GetLife() - 1);		// ʣ��1��Ѫ
// 				// effect
// 				CMsgMagicEffect	msg;
// 				IF_OK(msg.Create(m_pOwner->GetID(), m_nStatus, 0, m_pOwner->GetID(), nLoseLife, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
// 					m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
// 				
// 				m_pOwner->AddAttrib(_USERATTRIB_LIFE, -1*nLoseLife, SYNCHRO_TRUE);
// 				if(!m_pOwner->IsAlive())
// 					m_pOwner->BeKill(NULL);		// ����PK
// 			}
// 
// 
// 		}
		//-----------------------------�������2--�½���---------------------------
// 		else if( STATUS_DOT_BEATINGPRE == m_nStatus )
// 		{
// 			if( m_pOwner->IsAlive() )
// 			{
// 				//int nPower = ::MulDiv( m_pOwner->GetLife(),m_nData,100 );//��ǰѪ���İٷֱ�
// 				int nLoseLife = ::CutOverflow(m_nData, (int)m_pOwner->GetLife() - 1);		// ʣ��1��Ѫ
// 				
// 				CMsgMagicEffect	msg;
// 				IF_OK(msg.Create(m_pOwner->GetID(), m_nStatus, 0, m_pOwner->GetID(), nLoseLife, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
// 					m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
// 				
// 				m_pOwner->AddAttrib(_USERATTRIB_LIFE, -1*nLoseLife, SYNCHRO_TRUE);
// 				if(!m_pOwner->IsAlive())
// 					m_pOwner->BeKill(NULL);		// ����PK
// 			}
// 		}
		//-------------------------------------------------------------------
// 		else if( STATUS_DOT_MANAMISSING == m_nStatus )
// 		{
// 			void* p = NULL;
// 			if( m_pOwner->QueryObj( OBJ_USER,&p ) )//ֻ�������MP
// 			{
// 				CUser* pUser = ( CUser* )m_pOwner;
// 				if( pUser->IsAlive() )
// 				{
// 					int nLoseMana = ::CutOverflow( m_nData, (int)pUser->GetMana() - 1);// ʣ��1��ħ��
// 					// effect
// 					CMsgMagicEffect	msg;
// 					IF_OK(msg.Create( pUser->GetID(), m_nStatus, 0, pUser->GetID(), nLoseMana, pUser->GetDir(),BATTLEADDINFO::INFO_INVALID ) )
// 						pUser->BroadcastRoomMsg( &msg, INCLUDE_SELF );
// 					pUser->AddAttrib( _USERATTRIB_MANA, -1*nLoseMana, SYNCHRO_TRUE );
// 				}
// 				
// 			}
// 			
// 		}
// 		else if( STATUS_DOT_STRENGTHMISSING == m_nStatus )
// 		{
// 			void* p = NULL;
// 			if( m_pOwner->QueryObj( OBJ_USER,&p ) )//ֻ�������SP
// 			{
// 				CUser* pUser = ( CUser* )m_pOwner;
// 				if( pUser->IsAlive() )
// 				{
// 					int nLoseSp = ::CutOverflow( m_nData, (int)pUser->GetSp() - 1 );		// ʣ��1������
// 					// effect
// 					CMsgMagicEffect	msg;
// 					IF_OK(msg.Create( pUser->GetID(), m_nStatus, 0, pUser->GetID(), nLoseSp, pUser->GetDir(),BATTLEADDINFO::INFO_INVALID ))
// 						pUser->BroadcastRoomMsg( &msg, INCLUDE_SELF );
// 					pUser->AddAttrib( _USERATTRIB_SP, -1*nLoseSp, SYNCHRO_TRUE );
// 				}
// 			}
// 		}
		//2007827����:-------------
		else if(STATUS_DOT_ADDLIFE == m_nStatus || STATUS_SOUL_REBLOOD == m_nStatus )
// 			|| STATUS_DOT_ADDLIFE_RB == m_nStatus
// 			|| STATUS_DOT_QUICK_ADDLIFE == m_nStatus 
// 			|| STATUS_DOT_ADDLIFE_L == m_nStatus)// ���ٻָ� [12/25/2007 �½���]
		//-------------------------
		{
			if( m_pOwner->IsAlive() )
			{
				int nAddLife = __min(m_nData, m_pOwner->GetMaxLife()-m_pOwner->GetLife());
				// effect
				CMsgMagicEffect	msg;
				IF_OK(msg.Create(m_pOwner->GetID(), m_nStatus, 0, m_pOwner->GetID(), nAddLife, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
					m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
				
				m_pOwner->AddAttrib( _USERATTRIB_LIFE,nAddLife,SYNCHRO_TRUE );
			}

		}
		else if(STATUS_SOUL_REBLOOD_PER == m_nStatus)
		{
			if( m_pOwner->IsAlive() )
			{
				int nAddLife = __min(m_pOwner->GetMaxLife() * (float)(m_nData / 1000.0f),m_pOwner->GetMaxLife()-m_pOwner->GetLife() );
				// effect
				CMsgMagicEffect	msg;
				IF_OK(msg.Create(m_pOwner->GetID(), m_nStatus, 0, m_pOwner->GetID(), nAddLife, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
					m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
				
				m_pOwner->AddAttrib( _USERATTRIB_LIFE,nAddLife,SYNCHRO_TRUE );
			}
			
		}
		//2007827����:---------------
		//else if( STATUS_DOT_ADDMANA == m_nStatus )//����
		else if(STATUS_DOT_ADDMANA == m_nStatus || STATUS_SOUL_REMP == m_nStatus)
// 			|| STATUS_DOT_ADDMANA_MC == m_nStatus
// 			|| STATUS_DOT_ADDMANA_L == m_nStatus )
		//---------------------------
		{
			if( m_pOwner->IsAlive() )
			{
				int nAddMana = m_nData;
				// effect
				CMsgMagicEffect	msg;
				IF_OK(msg.Create(m_pOwner->GetID(), m_nStatus, 0, m_pOwner->GetID(), nAddMana, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
					m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
				
				m_pOwner->AddAttrib( _USERATTRIB_MANA,nAddMana,SYNCHRO_TRUE );
			}			
		}
// 		else if (STATUS_BUFF_HPMP == m_nStatus) //ͬʱ��Ѫ�ͼ���
// 		{
// 			if(m_pOwner->IsAlive())
// 			{
// 				int nAddMana = m_nData;
// 				int nAddLife = m_nData;
// 
// 				// effect
// 				CMsgMagicEffect	msg;
// 				IF_OK(msg.Create(m_pOwner->GetID(), m_nStatus, 0, m_pOwner->GetID(), nAddLife, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
// 					m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
// 				m_pOwner->AddAttrib( _USERATTRIB_LIFE,nAddLife,SYNCHRO_TRUE );
// 
// 				// effect
// 	/*			CMsgMagicEffect	msg;*/
// // 				IF_OK(msg.Create(m_pOwner->GetID(), m_nStatus, 0, m_pOwner->GetID(), nAddMana, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
// // 					m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
// 				
// 				m_pOwner->AddAttrib( _USERATTRIB_MANA,nAddMana,SYNCHRO_TRUE );
// 			}
// 		}
	}
	DEBUG_CATCH2("CStatusMore::OnTimer!m_nStatus=%d", m_nStatus)
	return bRet;

}

//////////////////////////////////////////////////////////////////////
bool CStatusMore::ChangeData( int nPower,int nSecs,int nTimes,int nParam1,int nParam2,int nParam3 ,OBJID idOwner)
{
	CHECKF(nSecs > 0 && nTimes > 0);
	
	m_nData		= nPower;
	m_nParam1	= nParam1;
	m_nParam2	= nParam2;
	m_nParam3	= nParam3;
	m_tKeep.SetInterval(nSecs);
	m_nTimes	= nTimes;
	m_idAttker	= idOwner;
	
	return true;	
}

//////////////////////////////////////////////////////////////////////
bool CStatusMore::ToFlash()
{
	if(!IsValid())
		return false;

	if(!m_bFlash && m_nTimes <= (STATUS_FLASH_REMAIN_MS/1000) / m_tKeep.GetInterval())
	{
		m_bFlash = true;
		return true;
	}

	return false;
}







//  [3/31/2008 ���]
//	�ó���ʱ��
int CStatusMore::GetSec()
{
	return m_tKeep.GetInterval()/* / 1000*/;//goto ��λ���룬 ���ó���1000
}

int CStatusMore::GetSecLeft()
{
	return m_nTimes*GetSec()+m_tKeep.GetRemain();
}

//////////////////////////////////////////////////////////////////////
void CStatusMore::SetFreeze()
{
	if (m_nParam3==1)
		return;
	m_nParam3 = 1;
}
//////////////////////////////////////////////////////////////////////
void CStatusMore::SetUnFreeze()
{
	if (m_nParam3==0)
		return;
	m_nParam3 = 0;
}

