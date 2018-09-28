// Role.cpp: implementation of the CRole class.
//
//////////////////////////////////////////////////////////////////////

#include "Role.h"
#include "StatusOnce.h"
#include "StatusMore.h"
#include "MsgAction.h"
#include "User.h"
#include "MapGroup.h"
#include "MsgTeam.h"
#include "MsgTeamMember.h"
#include "MsgWalkEx.h"
#include "MsgUserAttrib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRole::CRole()
	: m_setStatus(NULL)
{
	m_idProcess		= ID_NONE;
	m_pMap			= NULL;
	m_nPosX			= 0;	//DEFAULT_LOGIN_POSX;
	m_nPosY			= 0;	//DEFAULT_LOGIN_POSY;
	m_nDir			= 0;
	
}

CRole::~CRole()
{
	// �ɸ������ഴ���ģ���ô���ɸ������ฺ���ͷţ�����ֻ���ظ�����������ڴ�й©
	if(m_setStatus)
	{
		::LogSave("CRole::~CRole() %u %u",GetID(),GetObjType());
	}
	SAFE_RELEASE (m_setStatus);
}

//////////////////////////////////////////////////////////////////////
bool CRole::IsInFan(POINT pos, POINT posSource, int nRange, int nWidth, POINT posCenter)
{
	CHECKF(nWidth > 0 && nWidth < 360);
	
	if(posCenter.x == GetPosX() && posCenter.y == GetPosY())
		return false;
	if(pos.x == posSource.x && pos.y == posSource.y)
		return false;
	
	if(GetDistance(pos.x, pos.y) > nRange)
		return false;
	
	float PI = static_cast<float>(3.1415926535);
	float fRadianDelta	= (PI * nWidth / 180) / 2;
	float fCenterLine	= GetRadian(GetPosX(), GetPosY(), posCenter.x, posCenter.y);
	float fTargetLine	= GetRadian(posSource.x, posSource.y, pos.x, pos.y);
	float fDelta	= fabs(fCenterLine - fTargetLine);
	if(fDelta <= fRadianDelta || fDelta >= 2*PI - fRadianDelta)
		return true;
	
	return false;
}

//////////////////////////////////////////////////////////////////////
int	 CRole::AdjustDataEx(int nData, int nAdjust, int nMaxData/*=0*/)
{
	if(nAdjust>=ADJUST_PERCENT)
		return MulDiv(nData, nAdjust-ADJUST_PERCENT, 100);
	
	if(nAdjust==ADJUST_FULL)
	{
		ASSERT(nMaxData != 0);
		return nMaxData;
	}

	if(nAdjust<=ADJUST_SET)
		return -1*nAdjust + ADJUST_SET;
	

	return nData + nAdjust;
}
///////////////////////////////////////////////////////////////////////
//06.11.4
//����
//�ж�״̬�ǲ��ǿ���ϵ���µ�״̬
//��̬����
//07.4.18���񲨵�һ���޸ģ�ʹ��ID�ж�
bool CRole::IsControlStatus( int nStatus )
{
	int nIdent = nStatus / 1000 % 10;
	if( DATAIDENT_CONTROL == nIdent )
		return true;
	return false;
}
//////////////////////////////////////////////////////////////////////
//06.11.4�����޸�
bool CRole::AttachStatus( IRole* pRole, int nStatus, int nPower, int nSecs, int nTimes, int nParam1, int nParam2,int nParam3 ,OBJID idOwner)//����ʩ���ߵ�ID
{
	CGameMap* pMap = pRole->GetMap();
	IF_NOT (pMap)
		return false;

	if( STATUS_INVALID == nStatus )
	{
		return true;
	}
	if (IsBadlyStatus(nStatus))			//������Ҫ�����DOT�������⴦���˴�Ԥ��
		//////////////////////////////////
	{
//modify code by �ֵ�Ԩ begin 2011.6.22
		if( !(nStatus == STATUS_RETORT_MDSX || nStatus == STATUS_RETORT_MDSX_BUFF2 || nStatus == STATUS_RETORT_MDSX_1) )
		{
			if (pRole->QueryStatus( STATUS_PK_PROTECT ))
			{
				DetachBadlyStatus(pRole);
				return true;
			}
		}
//modify code by �ֵ�Ԩ end
	}
	IStatus* pStatus = pRole->QueryStatus( nStatus );
	if( pStatus )				
	{
		bool bChange	= false;
		StatusInfoStruct info;
		pStatus->GetInfo( &info );

		if( nPower > info.nPower ) //
		{
			bChange = true;
		}
		else if( nPower == info.nPower )
		{
			if( nTimes )//��ε�
			{
				if( nTimes > info.nTimes )
					bChange = true;
			}
			else//����
			{
				if(nStatus == STATUS_GETEXPUP_PERCENT || nStatus == STATUS_GETEXPUP_PERCENT_WT || nStatus == STATUS_GETEXPUP_PERCENT_SYNINS)
				{
					nSecs += info.nSecs;
					if(nSecs > 10 * 60 * 60)
					{
						pRole->SendSysMsg("˫������ʱ���Ѵ�����");
						return false;
					}
					bChange = true;
				}
				else if( nSecs > info.nSecs )
					bChange = true;
//modify code by �ֵ�Ԩ begin 2011.5.9
				else if( STATUS_STOPSAY ==  nStatus && 0 == nSecs )
				{
					pRole->QueryStatusSet()->DelObj( nStatus );
					pRole->ClsStatus( nStatus );
					pRole->ClsBuff(nStatus);
				}
//modify code by �ֵ�Ԩ end
			}
		}
		if( bChange )
		{
			pStatus->ChangeData( nPower, nSecs, nTimes, nParam1, nParam2,nParam3 ,idOwner);
			pRole->UpdateBuff( nStatus,nSecs,nTimes,nPower,nParam1,nParam2,nParam3 );
			//goto �޸�˫�����ƣ���ֹ����ʱ���˫����ʧ
			if(nStatus == STATUS_GETEXPUP_PERCENT || nStatus == STATUS_GETEXPUP_PERCENT_WT || nStatus == STATUS_GETEXPUP_PERCENT_SYNINS)
			{
				CUser* pUser = NULL;
				if (pRole->QueryObj(OBJ_USER,(void**)&pUser))
				{
					pUser->WriteStateToDB(pStatus);
				}
			}
		}
		else if (2 != int(nStatus/10000) && 3 != int(nStatus/10000)) 
		{
			pRole->SendSysMsg("��ӵ�и�Ϊǿ��ı���buff");
			return false;
		}
		return true;
	}
	else
	{
		if(nTimes)		// ��δ�����״̬
		{
			CStatusMore*	pNewStatus = CStatusMore::CreateNew();
			IF_OK(pNewStatus)
			{
				IF_OK(pNewStatus->Create( pRole,nStatus,nPower,nSecs,nTimes,nParam1,nParam2,nParam3,idOwner ) )
				{
					pRole->QueryStatusSet()->AddObj(pNewStatus->QueryInterface());
					pRole->SetStatus(nStatus, SYNCHRO_TRUE, nPower);

					//����Ҽ�״̬
					pRole->AddBuff( nStatus,nSecs,nTimes,nPower,nParam1,nParam2,nParam3 );
					return true;
				}
				else
					pNewStatus->ReleaseByOwner();
			}
		}
		else
		{
			CStatusOnce*	pNewStatus = CStatusOnce::CreateNew();
			IF_OK(pNewStatus)
			{
				IF_OK( pNewStatus->Create( pRole, nStatus, nPower, nSecs,nParam1,nParam2,nParam3,idOwner ) )
				{
					pRole->QueryStatusSet()->AddObj(pNewStatus->QueryInterface());
					pRole->SetStatus(nStatus, SYNCHRO_TRUE, nPower);

					//����Ҽ�״̬
					pRole->AddBuff( nStatus,nSecs,0,nPower,nParam1,nParam2,nParam3 );
					//goto �޸�˫�����ƣ���ֹ����ʱ���˫����ʧ
					if(nStatus == STATUS_GETEXPUP_PERCENT || nStatus == STATUS_GETEXPUP_PERCENT_WT || nStatus == STATUS_GETEXPUP_PERCENT_SYNINS)
					{
						CUser* pUser = NULL;
						if (pRole->QueryObj(OBJ_USER,(void**)&pUser))
						{
							pUser->WriteStateToDB(pNewStatus);
						}
					}
					return true;
				}
				else
					pNewStatus->ReleaseByOwner();
			}
		}

	}
	
	
	return false;
}

//////////////////////////////////////////////////////////////////////
void CRole::DetachWellStatus(IRole* pRole)
{
	CHECK(pRole);

	for(int i = pRole->QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
	{
		IStatus* pStatus =  pRole->QueryStatusSet()->GetObjByIndex(i);
		if(pStatus && pStatus->GetID() != STATUS_CRIME)
		{
			if (CRole::IsWellStatus(pStatus->GetID()))
			{
			//	pRole->QueryStatusSet()->DelObj(pStatus->GetID());
				CRole::DetachStatus( pRole,pStatus->GetID() );
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
void CRole::DetachBadlyStatus(IRole* pRole)
{
	CHECK(pRole);

	for(int i =  pRole->QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
	{
		IStatus* pStatus =  pRole->QueryStatusSet()->GetObjByIndex(i);
		if(pStatus && pStatus->GetID() != STATUS_CRIME)
		{
			if (CRole::IsBadlyStatus(pStatus->GetID()))
			{
			//	pRole->QueryStatusSet()->DelObj(pStatus->GetID());
				CRole::DetachStatus( pRole,pStatus->GetID() );

			}
		}
	}
}
//-------------------------------------------------------------------------------------------
//  [12/26/2007 �½���]�Ӵ��̶������Ĳ���BUF
void CRole::DetachSomeBadStatus(IRole* pRole,int nCout )
{
	CHECK(pRole);

	int nNum = nCout;
	for(int i =  pRole->QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
	{
		IStatus* pStatus =  pRole->QueryStatusSet()->GetObjByIndex(i);
		if(pStatus && pStatus->GetID() != STATUS_CRIME)
		{
			if (CRole::IsBadlyStatus(pStatus->GetID()))
			{
			//	pRole->QueryStatusSet()->DelObj(pStatus->GetID());
				CRole::DetachStatus( pRole,pStatus->GetID() );
				nNum --;

				if( nNum <= 0 )
					break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
bool CRole::IsWellStatus(int nStatus)
{
	int nBigType	= nStatus / 10000;
	if( BIGTYPE_BUFF == nBigType )
		return true;
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CRole::IsBadlyStatus(int nStatus)
{
	int nBigType	= nStatus / 10000;	
	if (nStatus == STATUS_BOOTH_ADDEXP ||
		nStatus == STATUS_MOUNTSPEED_NUMBER ||
		nStatus == STATUS_WARGAME ||
		nStatus == STATUS_STOPSAY ||//������BUFF�������徻
		nStatus == STATUS_ACTIVATE_ADDEXP ||
		nStatus == STATUS_ITEM_ADDEXP||
//		nStatus == STATUS_SYNWORKTIME ||
		nStatus == STATUS_DOT_TUTOREXP ||
		nStatus == STATUS_DOT_SYNEXP||
		nStatus == STATUS_TUTOR_BOSS ||
		nStatus == STATUS_DOT_WARGAMEEXP||
		nStatus == STATUS_DOT_ADDEXP
		)
		return false;
	if( BIGTYPE_DEBUFF == nBigType || BIGTYPE_DOT == nBigType)
		return true;
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CRole::DetachStatus(IRole* pRole, int nStatus)
{
	CHECKF(pRole);
	if (nStatus == STATUS_VERIFICATION_WAIT)
	{
		CUser* pUser = NULL;
		if (pRole->QueryObj(OBJ_USER,(void**)&pUser))
		{
			if (!pUser->GetVerificationPass()&&pUser->GetVerificationNeedChk())
			{
				pUser->KickOutUser();
			}
		}
	}
	//goto �޸�˫�����ƣ���ֹ����ʱ���˫����ʧ
	if(nStatus == STATUS_GETEXPUP_PERCENT || nStatus == STATUS_GETEXPUP_PERCENT_WT || nStatus == STATUS_GETEXPUP_PERCENT_SYNINS)
	{
		CUser* pUser = NULL;
		if (pRole->QueryObj(OBJ_USER,(void**)&pUser))
		{
			pUser->DelStateFromDB(nStatus);
		}
	}
	bool hr=false;
	hr=pRole->QueryStatusSet()->DelObj( nStatus );
	pRole->ClsStatus( nStatus );
	pRole->ClsBuff(nStatus);
	return hr;
}

//////////////////////////////////////////////////////////////////////
void CRole::DetachAllStatus(IRole* pRole)
{
	CHECK(pRole);

	for(int i =  pRole->QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
	{
		IStatus* pStatus =  pRole->QueryStatusSet()->GetObjByIndex(i);
		if( pStatus && pStatus->GetID() != STATUS_CRIME 
			&& pStatus->GetID() != STATUS_GETEXPUP_PERCENT 
			&& pStatus->GetID() != STATUS_MOUNTSPEED_NUMBER//�����޸ģ���ʱ�ӵ������ɢ�����˫�����������BUF
			//2007827����:��������buff���ᱻ��ɢ-------------------
			&& pStatus->GetID() != STATUS_PHYSICALATTACKUP_PERCENT_WS
			&& pStatus->GetID() != STATUS_MAGICATTACKUP_PERCENT_ED
			&& pStatus->GetID() != STATUS_DAMAGEDOWN_PERCENT_TN
			&& pStatus->GetID() != STATUS_GETEXPUP_PERCENT_WT
			&& pStatus->GetID() != STATUS_PHYSICALDEFENCEUP_NUMBER_SF
			&& pStatus->GetID() != STATUS_MAGICDEFENCEUP_NUMBER_EM
			&& pStatus->GetID() != STATUS_CRITRATEUP_NUMBER_SW
// 			&& pStatus->GetID() != STATUS_DOT_ADDLIFE_RB
// 			&& pStatus->GetID() != STATUS_DOT_ADDMANA_MC
			&& pStatus->GetID() != STATUS_STRUGGLE_RELIVE
			//-----------------------------------------------------
			&& pStatus->GetID() != STATUS_GETEXPUP_PERCENT_SYNINS
			//  [2007-10-11] ���	���� ʱ��֮�� ������ʯ ս��� ������ɢ
			&& pStatus->GetID() != STATUS_ENEMYFIRE
			&& pStatus->GetID() != STATUS_DAMAGEDOWN_PERCENT
			&& pStatus->GetID() != STATUS_SKZL
			&& pStatus->GetID() != STATUS_WARGAME
			&& pStatus->GetID() != STATUS_RESOURCEWAR_PROTECT// pengfeng DEBUG [10/15/2007]
			&& pStatus->GetID() != STATUS_PHYATTACKUP_TUTOR_PERCENT//  [�½���10/26/2007]
			&& pStatus->GetID() != STATUS_MAGATTACKUP_TUTOR_PERCENT//  [�½���10/27/2007]
			&& pStatus->GetID() != STATUS_NEW_PK_PROTECT	//  [1/22/2008 ���]	��ȥPK����
			&& pStatus->GetID() != STATUS_STOPSAY     //2008.1.17 ����  DEBUG:��ɢ��������GM����BUFF
			&& pStatus->GetID() != STATUS_LIFE_UP   // ���HP���� [1/31/2008 �½���]
			&& pStatus->GetID() != STATUS_VERIFICATION_RUNNING
			&& pStatus->GetID() != STATUS_VERIFICATION_WAIT
			)
			DetachStatus(pRole,pStatus->GetID());
//			 pRole->QueryStatusSet()->DelObj(pStatus->GetID());
	}
}

// ��������ʱ�����������BUF [�½���8/8/2007]
//void CRole::DetachSpeedStatus( IRole* pRole )
//{
//	CHECK(pRole);
//
//	for(int i =  pRole->QueryStatusSet()->GetAmount() - 1; i >= 0; i--)
//	{
//		IStatus* pStatus =  pRole->QueryStatusSet()->GetObjByIndex(i);
//		if( pStatus->GetID() == STATUS_MOVESPEEDUP_PERCENT)//
//			DetachStatus(pRole,pStatus->GetID());
//	}
//}

//////////////////////////////////////////////////////////////////////
// ����������˲�Ƶ�����
void CRole::ActiveTransPos(int nPosX, int nPosY)
{
	if (!GetMap()/* || !GetMap()->IsStandEnable(nPosX, nPosY)*/)//���������赲
		return;

	//this->ClrAttackTarget();//20080124����:��ͼ˲��,����ֹͣ����.�Ա�����Ծ֮���޷���������
	
	CUser* pUser = NULL;
	if (this->QueryObj(OBJ_USER, IPP_OF(pUser)))
	{
		if(!GetMap()->IsStandEnable(nPosX, nPosY))
			return;

		pUser->StopMine();
		if (!pUser->IsLuxuryBooth())
		{
			pUser->DestroyBooth();
		}
	}
	else
	{
		if(!GetMap()->IsValidPoint(nPosX, nPosY))
			return;
	}

	ProcessOnMove(MOVEMODE_SYNCHRO);
	TransPos(nPosX,nPosY) ;	
	
// 	CMsgAction	msg;
// 	if(msg.Create(GetID(), GetPosX(), GetPosY(), GetDir(), actionTransPos, GetPosX(), GetPosY()))
// 		SendMsg(&msg);


// 	CMsgWalkEx msgWalk;
//     if(msgWalk.Create(GetID(), GetDir(), MOVEMODE_SYNCHRO, GetPosX(), GetPosY()))
// 	{
// 		MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(ID_NONE, &msgWalk);
// 	}
	return;
}

//////////////////////////////////////////////////////////////////////
int	CRole::MoveToward(int nDir, bool bSync)
{
	m_nDir	= nDir%10;
	int	nDirX	= _DELTA_X[m_nDir];
	int	nDirY	= _DELTA_Y[m_nDir];
	
	CMapPtr pMap = this->GetMap();
	CHECKF(pMap);

	int nNewPosX = GetPosX() + nDirX;
	int nNewPosY = GetPosY() + nDirY;

	if(IsUser() && !GetMap()->IsValidPoint(nNewPosX, nNewPosY))
	{
		::LogSave("IsUser() && GetMap()->IsValidPoint(nNewPosX, nNewPosY) %u %u %u %u",GetID(),GetMap()->GetID(),nNewPosX,nNewPosY);
		return 0;
	}
// 	IF_NOT(GetMap()->IsValidPoint(nNewPosX, nNewPosY))
// 	{
// 		//zeng
// 
// 		char szlog[1024];
// 		sprintf(szlog,"MapID=%d,nNewPosX=%d,nNewPosY=%d,MapWidth=%d,MapHeight=%d,UserName=%s,UserID=%d,UserPosX=%d,UserPosY=%d,nDir=%d,IsAlive=%d",pMap->GetID(),nNewPosX,nNewPosY,pMap->GetWidth(),
// 			pMap->GetHeight(),GetName(),GetID(),GetPosX(),GetPosY(),nDir,IsAlive());
// 
// 		LOGERROR(szlog);
// 
// 		//MyStackWalker sw;
// 		//sw.ShowCallstack();
// 
// 		//zeng
// 
// 		return 0;
// 	}

	CUser* pUser = NULL;
	this->QueryObj(OBJ_USER, IPP_OF(pUser));
// 	if(GetMap() && !GetMap()->IsStandEnable(nNewPosX, nNewPosY) && this->QueryObj(OBJ_USER, IPP_OF(pUser)))
//  	{
// 		SendSysMsg(g_objStrRes.GetStr( 12030 ));//STR_INVALID_COORDINATE
// //		MSGBUF	szMsg;
// //		sprintf(szMsg, "���[%s]�ߵ��Ƿ�����[%d,%d,%d]��", GetName(), GetMapID(), nNewPosX, nNewPosY);
// 		//		::GmLogSave(szMsg);
// 		pUser->KickBack();
// 		//UserManager()->KickOutSocket(GetSocketID(), szMsg);
// 		return false;
// 	}

	pMap->MoveTo(this, nNewPosX, nNewPosY, WITH_BLOCK, WITH_BLOCK);
	m_nPosX = nNewPosX;
	m_nPosY = nNewPosY;

	if (pUser)
	{
//		pUser->StandRestart();
		if(pMap->ChangeUserMapData(pUser, nNewPosX, nNewPosY))
		{
			return 2;
		}
	}
	

	// ����MapThing	-- !!!����Ҳ��Ҫ����
	{
/*
		int x,y,z;
		IRole*	pRole = NULL;
		FOR_9_BLOCKTHINGS(pMap, GetPosX(), GetPosY())
		{
			IMapThing* pTarget = pMap->QueryThingByIndex(x,y,z);
			if(pTarget)
			{
				int	nDistX	= (pTarget->GetPosX() - this->GetPosX()) * nDirX;			// ǰ�����Ϊ������Ϊ����
				int	nDistY	= (pTarget->GetPosY() - this->GetPosY()) * nDirY;			// ǰ�����Ϊ������Ϊ����
				if(nDirX && nDistX == CELLS_PER_VIEW || nDirY && nDistY == CELLS_PER_VIEW)
				{
					pTarget->SendShow(this);
					//				this->SendSysMsg("����");//@
					
					if(bSync && pTarget->QueryRole(IPP_OF(pRole)))		// �������ǽ�ɫ
						this->SendShow(pRole);
				}
			}
		}
*/
	}
	if (bSync)
		UpdateBroadcastSet();

	ProcessAfterMove();
	return 1;
}

//////////////////////////////////////////////////////////////////////
void CRole::TransPos(int nPosX, int nPosY)
{
	if(nPosX == GetPosX() && nPosY == GetPosY())
		return;

	CMapPtr pMap = this->GetMap();

	CUser* pUser = NULL;
	bool bRect = this->QueryObj(OBJ_USER, IPP_OF(pUser));

	if(pUser && bRect)
	{
		IF_NOT(pMap && pMap->IsStandEnable(nPosX, nPosY))			//zeng��IsValidPoint��ΪIsStandEnable
		{
			if(pMap)
				::LogSave("ERROR:Role.cpp Ln390. pMap->IsStandEnable(%d,%d)||mapID=%d",nPosX,nPosY,pMap->GetID());
			else
				::LogSave("ERROR:Role.cpp Ln393.pMap is NULL");
			
			if(pUser)
				pUser->KickBack();
			
			return;
		}
	}
	else//�����������ֿ�����Ϊ���������赲
	{
		IF_NOT(pMap && pMap->IsValidPoint(nPosX, nPosY))			//zeng��IsValidPoint��ΪIsStandEnable
		{
			if(pMap)
				::LogSave("ERROR:Role.cpp Ln390. pMap->IsValidPoint(%d,%d)||mapID=%d",nPosX,nPosY,pMap->GetID());
			else
				::LogSave("ERROR:Role.cpp Ln393.pMap is NULL");
			
			return;
		}
	}
	
	if(pUser && !pUser->CanBeSee())
	{
		pUser->KickBack();
		return;
	}

	pMap->MoveTo(this, nPosX, nPosY, WITH_BLOCK, WITH_BLOCK);
	
	m_nPosX = nPosX;
	m_nPosY = nPosY;

	CMonster* pMonster = NULL;
	if(QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
		pMonster->ClearPath();
	ClrBroadcastSet(false);
	UpdateBroadcastSet();
	
	ProcessAfterMove();
}

//////////////////////////////////////////////////////////////////////
void CRole::JumpPos(int nPosX, int nPosY)
{
	CHECK(!"void CRole::JumpPos(int nPosX, int nPosY)");
	if(nPosX == GetPosX() && nPosY == GetPosY())
		return;
	
	CMapPtr pMap = this->GetMap();
	IF_NOT(pMap && pMap->IsValidPoint(nPosX, nPosY))
		return;

	CUser* pUser = NULL;
	if (this->QueryObj(OBJ_USER, IPP_OF(pUser)))
	{
		int	nDeltaX	= abs(nPosX - m_nPosX);
		int	nDeltaY	= abs(nPosY - m_nPosY);
		
		// ������
		if(nDeltaX > CELLS_PER_BLOCK || nDeltaY > CELLS_PER_BLOCK)
		{
			//		::GmLogSave("���[%s]���ľ���Ƿ�[%d][%d]��", GetName(), nDeltaX, nDeltaY);
			return ;
		}
		if(GetMap() && !GetMap()->IsStandEnable(nPosX, nPosY))
		{
			SendSysMsg(g_objStrRes.GetStr( 12030 ));//STR_INVALID_COORDINATE
//			MSGBUF	szMsg;
//			sprintf(szMsg, "���[%s]�����Ƿ�����[%d,%d,%d]��", GetName(), GetMapID(), nPosX, nPosY);
//			::GmLogSave(szMsg);
			pUser->KickBack();
			//UserManager()->KickOutSocket(GetSocketID(), szMsg);
			return ;
		}
	}
//	if( pStatus->GetID() != STATUS_MOUNTSPEED_NUMBER )//  [�½���8/3/2007]
//		CRole::DetachStatus(this, STATUS_HIDDEN);

	pMap->MoveTo(this, nPosX, nPosY, WITH_BLOCK, WITH_BLOCK);
	if (pUser)
	{
		//pUser->StandRestart();
		//���ʹ��� satan******************************
		//pMap->ChangeRegion(pUser, nPosX, nPosY);
	}
//	int	nOldX	= m_nPosX;
//	int	nOldY	= m_nPosY;
	m_nPosX = nPosX;
	m_nPosY = nPosY;
	
	// �������
/*
	int x,y,z;
	IRole*	pRole = NULL;
	FOR_9_BLOCKTHINGS(pMap, GetPosX(), GetPosY())
	{
		IMapThing* pTarget = pMap->QueryThingByIndex(x,y,z);
		CUser* pTargetUser = NULL;
		if(pTarget && 
			(pUser													// ��ң�һ��Ҫ��
			|| pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser))))	// ����ң����ڿ�����Ҳŷ�
		{
			if(abs(pTarget->GetPosX() - nOldX) > CELLS_PER_VIEW  
				&& abs(pTarget->GetPosX() - this->GetPosX()) <= CELLS_PER_VIEW
				|| abs(pTarget->GetPosY() - nOldY) > CELLS_PER_VIEW 
				&& abs(pTarget->GetPosY() - this->GetPosY()) <= CELLS_PER_VIEW )
			{
				pTarget->SendShow(this);
				//				this->SendSysMsg("����");//@

				if(pTarget->QueryRole(IPP_OF(pRole)))
					this->SendShow(pRole);
			}
		}
	}
*/

	ClrBroadcastSet(false);
	UpdateBroadcastSet();
	
	ProcessAfterMove();
}

//////////////////////////////////////////////////////////////////////
bool CRole::SyncTrackTo(int nPosX, int nPosY, int nDir, DWORD dwAction)
{
	CHECKF(!"bool CRole::SyncTrackTo(int nPosX, int nPosY, int nDir, DWORD dwAction)");
	m_nDir	= nDir%8;
	
	if (GetPosX() != nPosX || GetPosY() != nPosY)
	{
		CMapPtr pMap = this->GetMap();
		IF_NOT(pMap && pMap->IsValidPoint(nPosX, nPosY))
			return false;
		
		if(GetMap() && !GetMap()->IsStandEnable(nPosX, nPosY))
		{
			//SendSysMsg(STR_INVALID_COORDINATE);
			//::LogSave("Invalid coordinate (%d, %d)", nPosX, nPosY);
			return false;
		}
		
		CRole::DetachStatus(this, STATUS_HIDDEN);
		
		pMap->MoveTo(this, nPosX, nPosY, WITH_BLOCK, WITH_BLOCK);
		CUser* pUser = NULL;
		if (this->QueryObj(OBJ_USER, IPP_OF(pUser)))
		{
			//pUser->StandRestart();
			//���ʹ��� satan******************************
			//pMap->ChangeRegion(pUser, nPosX, nPosY);
		}

//		int	nOldX	= m_nPosX;
//		int	nOldY	= m_nPosY;
		m_nPosX	= nPosX;
		m_nPosY	= nPosY;

/*
		int x,y,z;
		IRole*	pRole = NULL;
		FOR_9_BLOCKTHINGS(pMap, GetPosX(), GetPosY())
		{
			IMapThing* pTarget = pMap->QueryThingByIndex(x,y,z);
			CUser* pTargetUser = NULL;
			if(pTarget && 
				(pUser													// ��ң�һ��Ҫ��
				|| pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser))))	// ����ң����ڿ�����Ҳŷ�
			{
				if(abs(pTarget->GetPosX() - nOldX) > CELLS_PER_VIEW  
					&& abs(pTarget->GetPosX() - this->GetPosX()) <= CELLS_PER_VIEW
					|| abs(pTarget->GetPosY() - nOldY) > CELLS_PER_VIEW 
					&& abs(pTarget->GetPosY() - this->GetPosY()) <= CELLS_PER_VIEW )
				{
					pTarget->SendShow(this);

					if(pTarget->QueryRole(IPP_OF(pRole)))		// �������ǽ�ɫ
						this->SendShow(pRole);
				}
			}
		}
*/
	}

	UpdateBroadcastSet();

	//	ProcessAfterMove();
	
	// ͬ���켣
	CMsgAction	msg;
	IF_OK (msg.Create(GetID(), GetPosX(), GetPosY(), GetDir(), actionMagicTrack, dwAction))
		BroadcastRoomMsg(&msg, true);
	
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CRole::UpdateBroadcastSet(bool bClearSet/*=false*/)
{
	CHECKF (GetMap());

	int step = 0;
	DEBUG_TRY

	CMapItem* pMapItem = NULL;
	CBooth* pBooth = NULL;
	CUser* pUser = NULL;
	QueryObj(OBJ_USER, IPP_OF(pUser));

	step = 1;
	bool	bIsCallPet = false;
	CMonster* pMonster;
	if (QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
	{
		if (pMonster->IsCallPet() || pMonster->IsEudemon())
		{
		    bIsCallPet = true;
		}
//			bIsCallPet = true;
	}

	step = 2;
	// Ѱ���µ�Ŀ�꼯
	typedef std::vector<IRole*>		ROLE_SET;
	ROLE_SET		setNewRole;
	BROADCAST_SET	setNewMapItem;
	BROADCAST_SET	setNewComeMapThing;
	int x,y,z;
	IRole*	pRole = NULL;
	FOR_9_BLOCKTHINGS(GetMap(), GetPosX(), GetPosY())
	{
		step = 3;
		IMapThing* pTarget = GetMap()->QueryThingByIndex(x,y,z);
		step = 31;
		if(!pTarget)
			continue;
#ifdef MYDEBUG
// 		OBJID idObjType = ID_NONE;
// 		DEBUG_TRY
// 		idObjType = pTarget->GetObjType();
// 		DEBUG_CATCH("Invalit pt pTarget")
// 		switch (idObjType)
// 		{
// 		case OBJ_USER:
// 			{
// 				if (!CUser::IsValidPt(pTarget))
// 				{
// 					// invalid pt check and remove it...
// 					continue;
// 				}
// 			}
// 			break;
// 		case OBJ_MONSTER:
// 			{
// 
// 			}
// 			break;
// 		case OBJ_NPC:
// 			{
// 
// 			}
// 			break;
// 		default:
// 			break;
// 		}
#endif
//		CUser* pTargetUser = NULL;
	
		step = 4;

		if (abs(pTarget->GetPosX() - GetPosX()) <= CELLS_PER_VIEW
			&& abs(pTarget->GetPosY() - GetPosY()) <= CELLS_PER_VIEW)
		{
			if (pTarget->QueryRole(IPP_OF(pRole)))
			{
				step = 5;
				CUser* pTargetUser = NULL;
				CMonster* pTargetMonster = NULL;
				OBJID id = pRole->GetID();
				if (pRole->GetID() != this->GetID() //�����Լ�
					&& ( pUser || bIsCallPet || pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser)) 
					|| (pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pTargetMonster))&& pTargetMonster && pTargetMonster->IsEudemon())))
				{
					step = 6;
					setNewRole.push_back(pRole);
				}
			}
			else if (pUser && pTarget->QueryObj(OBJ_MAPITEM, IPP_OF(pMapItem)))	
			{
				step = 7;
				if(pMapItem && pMapItem->GetOwnerID() == pUser->GetID())
					setNewMapItem.push_back(pTarget->GetID());
			}
			else if (pUser && pTarget->QueryObj(OBJ_BOOTH, IPP_OF(pBooth)) ) 
			{
				step = 8;
				setNewComeMapThing.push_back(pTarget->GetID());
				//pTarget->SendShow(this);
			}
		}
	}
// 
// 	if(pUser)
// 	{
		//���ѵľ����ÿͻ���ȥ����
//		CTeam* pTeam=pUser->GetTeam();///,*pTargetTeam=NULL;
//		CMsgTeamMember msgMember;
// 		if(pTeam)
// 		{
// 			if(pUser->GetMap())
// 			{
// 				msgMember.Create(_MSG_TEAMMEMBER_CHGMAP, pUser->GetMap()->GetDocID(), pUser->GetID());
// 				pTeam->BroadcastTeamMsg(&msgMember);
// 			}
// 		}

// 		CUser* pTargetMember=NULL;
// 		for(int i=0;pTeam && i<pTeam->GetMemberAmount();++i)
// 		{
// 			CMsgTeam msg;
// 			OBJID idMember =pTeam->GetMemberByIndex(i);
// 			if (idMember!=pUser->GetID())
// 			{
// 				pTargetMember=UserManager()->GetUser(idMember);
// 				if (pTargetMember)
// 				{
// 					if (pUser->GetDistance(pTargetMember->GetPosX(),pTargetMember->GetPosY())<=_RANGE_EXPSHARE)				
// 					{
// 						msg.Create(idMember,_MSGTEAM_CHG_SHARESTATUS_IN);
// 						pTeam->GetShareStatusSet()[pTargetMember->GetID()]=true;
// 						pUser->SendMsg(&msg);			
// 					}
// 					else if(pTeam->GetShareStatusSet()[idMember]==true)
// 					{
// 						msg.Create(idMember,_MSGTEAM_CHG_SHARESTATUS_OUT);
// 						pTeam->GetShareStatusSet()[idMember]=false;
// 						pUser->SendMsg(&msg);
// 					}
// 				}
// 				else
// 				{
// 					msg.Create(idMember,_MSGTEAM_CHG_SHARESTATUS_OUT);
// 					pTeam->GetShareStatusSet()[idMember]=false;
// 					pUser->SendMsg(&msg);
// 				}
// 			}
// 		}
// 	}

		// ����ԭĿ�꼯��ɾ��������Ŀ�꼯�е�id��ѹ����ɾ������
		// ͬʱҲɾ����Ŀ�꼯��ԭ���Ѿ����ڵ�id
	step = 9;
	BROADCAST_SET::iterator it = m_setBCRole.begin();
	for (; it!=m_setBCRole.end(); )
	{
		OBJID id = *it;
		bool  bMatch = false;
		ROLE_SET::iterator iter = setNewRole.begin();
		for (; iter!=setNewRole.end(); iter++)
		{
			IRole* pRole = *iter;
			if (pRole && pRole->GetID() == id)
			{
				//δ�ܳ�BC
				bMatch = true;
				setNewRole.erase(iter);
				break;
			}
		}

		//BC�����  ������
		BROADCAST_SET::iterator itErase = it;
		++it;
		if (!bMatch)
		{
			m_setBCRole.erase(itErase);

			// ֪ͨ�Լ��Է���ʧ
			DEBUG_TRY
			RemoveFromBCRoleSet(id);
			DEBUG_CATCH("RemoveFromBCRoleSet(id);");
			// ֪ͨ�Է��Լ���ʧ
			IRole* pRole = RoleManager()->QueryRole(id);
			if (pRole)
			{
				DEBUG_TRY
				pRole->RemoveFromBCRoleSet(this->GetID());
				DEBUG_CATCH("RemoveFromBCRoleSet(id);2");
			}
		}
	}
	step = 10;

	for (it=m_setBCMapItem.begin(); it!=m_setBCMapItem.end(); )
	{
		OBJID id = *it;
		bool bMatch = false;
		BROADCAST_SET::iterator iter = find(setNewMapItem.begin(), setNewMapItem.end(), id);
		if (iter != setNewMapItem.end())
		{
			setNewMapItem.erase(iter);
			bMatch = true;
		}

		BROADCAST_SET::iterator itErase = it;
		++it;

		if (!bMatch)
		{
			m_setBCMapItem.erase(itErase);

			DEBUG_TRY
			RemoveFromBCMapItemSet(id);
			DEBUG_CATCH("RemoveFromBCMapItemSet(id);");
		}
	}

	step = 11;
	for(it = m_setAnotherMapThing.begin(); it != m_setAnotherMapThing.end();/* ++it*/)
	{
		OBJID idObj = *it;
		bool bMatch = false;
		BROADCAST_SET::iterator i = find(setNewComeMapThing.begin(), setNewComeMapThing.end(), idObj);
		if(i != setNewComeMapThing.end()) // Find it
		{
			setNewComeMapThing.erase(i);
			bMatch = true;
		}
		
		BROADCAST_SET::iterator itErase = it;
		++it;

		if(!bMatch)
		{
			m_setAnotherMapThing.erase(itErase);
			
			DEBUG_TRY
			DisappearMapThing(idObj);
			DEBUG_CATCH("DisappearMapThing(idObj);");

		}
	}

	step = 12;
	// ���������¾�Ŀ�꼯�ϲ�
	CUser* pTargetUser = NULL;
	if (!setNewRole.empty())
	{
		ROLE_SET::iterator it=setNewRole.begin();
		for (; it!=setNewRole.end(); it++)
		{
			IRole* pRole = *it;
			if (pRole)
			{
				//��Ҷ��ܿ���
				//�����NPCʲô��������
				//���￴�˺ͳ���

				pRole->QueryObj(OBJ_USER, IPP_OF(pTargetUser));
				pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster));

				bool bSendSelf = false;
				if(pUser)
					bSendSelf = true;
				else if(IsMonster() && (pTargetUser || (pMonster && pMonster->IsEudemon())))
					bSendSelf = true;
// 				bool bSendSelf = (pUser || 
// 									(
// 									   !bIsCallPet && !IsNpc()
// 									   &&   (     pRole->QueryObj(OBJ_USER, IPP_OF(pTargetUser))  
// 												  || (	pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster))
// 														&& (pMonster->IsCallPet() || pMonster->IsEudemon())
// 													)
// 											)
// 									));

			
				DEBUG_TRY
				AddToBCRoleSet(pRole->GetID(), bSendSelf);
				DEBUG_CATCH3("AddToBCRoleSet(pRole->GetID(), bSendSelf); %u %u",GetID(),pRole->GetID());

				// �Է����ǻ��ޣ��ң��Լ�����һ���޻��߶Է�����ҵ�ʱ�����Ҫ����
				

				bool bSendTarget = false;
				if(pTargetUser)
					bSendTarget = true;
				else if((pMonster && !pMonster->IsEudemon()) && (pUser || bIsCallPet))
					bSendTarget = true;

// 				bool bSendTarget = ((!pMonster || (!pMonster->IsCallPet() && !pMonster->IsEudemon()))
// 										&& (pUser || bIsCallPet || pRole->QueryObj(OBJ_USER, IPP_OF(pTargetUser))));

			
				DEBUG_TRY
				pRole->AddToBCRoleSet(GetID(), bSendTarget);
				DEBUG_CATCH3("pRole->AddToBCRoleSet(GetID(), bSendTarget); %u %u",GetID(),pRole->GetID());

				
			}
		}
	}

	step = 13;
	if (!setNewMapItem.empty())
	{
		BROADCAST_SET::iterator it=setNewMapItem.begin();
		for (; it!=setNewMapItem.end(); it++)
		{
			DEBUG_TRY
			AddToBCMapItemSet(*it);	
			DEBUG_CATCH("AddToBCMapItemSet(*it);");
		}
	}

	step = 14;

	if (!setNewComeMapThing.empty())
	{
		BROADCAST_SET::iterator it=setNewComeMapThing.begin();
		for (; it!=setNewComeMapThing.end(); it++)
		{
			DEBUG_TRY
			SeeMapThing(*it);
			DEBUG_CATCH("SeeMapThing(*it);");
		}
	}

	DEBUG_CATCH2("UpdateBroadcastSet %u",step);

	return true;
}

//////////////////////////////////////////////////////////////////////
void CRole::ClrBroadcastSet(bool bForce/* = true*/)
{

	//zeng

	bool bMonster = false;
	CMonster* pMonster;
	if(QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
		bMonster = true;

	//zeng

	BROADCAST_SET::iterator it = m_setBCRole.begin();
	for (; it!=m_setBCRole.end(); it++)
	{
		OBJID id = *it;
		// ֪ͨ�Է��Լ���ʧ
		IRole* pRole = RoleManager()->QueryRole(id);
		if (pRole)
			pRole->RemoveFromBCRoleSet(this->GetID());
	}

// 	if(IsUser() || (bMonster && pMonster && pMonster->IsEudemon()))
// 	{
// 		if(MapGroup(m_idProcess)->RemoveFromNpcBigSet(this->GetID(),bForce))
// 		{
// 			CMsgAction msg;
// 			if (msg.Create(GetID(), 0, 0, 0, actionDisappear, 0))
// 				MapGroup(m_idProcess)->QueryIntraMsg()->SendNpcMsg(ID_NONE, &msg);
// 		}
// 	}
	m_setBCRole.clear();

	m_setBCMapItem.clear();
}

//////////////////////////////////////////////////////////////////////
void CRole::AddToBCRoleSet(OBJID idRole, bool bSendMsg)
{
	BROADCAST_SET::iterator it=m_setBCRole.begin();
	for (; it!=m_setBCRole.end(); it++)
	{
		if (idRole == *it)
			return;
	}
	m_setBCRole.push_back(idRole);

	//zeng
	CMonster* pMonster;
	if(QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
	{
		if(pMonster && !pMonster->IsEudemon())
		{
			pMonster->AddSeeUserNum();
// 			if(MapGroup(m_idProcess)->FindNpcBigSet(idRole))
// 				bSendMsg = false;
// 			
// 			MapGroup(m_idProcess)->AddToNpcBigSet(idRole);
		}
	}

	//zeng

	if (bSendMsg)
	{
		IRole* pRole =  RoleManager()->QueryRole(idRole);
		if (pRole)
		{
			pRole->QueryMapThing()->SendShow((IRole*)this);
		}
	}
}

//////////////////////////////////////////////////////////////////////
void CRole::RemoveFromBCRoleSet(OBJID idRole,bool bClrBigBc/* = false*/)
{
	//NPC������������ʱ���ģ�����ֻ��֪ͨ�ͻ������
	CUser* pUser = NULL;
	if (QueryObj(OBJ_USER, IPP_OF(pUser)))
	{
		// ֪ͨ�Լ��Է���ʧ
		CMsgAction msg;
		if (msg.Create(idRole, 0, 0, 0, actionDisappear, 0))
			SendMsg(&msg);
	}

	BROADCAST_SET::iterator it=m_setBCRole.begin();
	for (; it!=m_setBCRole.end(); it++)
	{
		if (idRole == *it)
		{
			m_setBCRole.erase(it);
			break;
		}
	}


	CMonster* pMonster;
	if(QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && !pMonster->IsEudemon())
	{
		pMonster->DecSeeUserNum();
// 		MapGroup(m_idProcess)->RemoveFromNpcBigSet(idRole);
// 
// 		if(bClrBigBc || !MapGroup(m_idProcess)->FindNpcBigSet(idRole))
// 		{
// 			CMsgAction msg;
// 			if (msg.Create(idRole, 0, 0, 0, actionDisappear, 0))
// 				SendMsg(&msg);
// 		}
	}
	
	//zeng
}

//////////////////////////////////////////////////////////////////////
void CRole::AddToBCMapItemSet(OBJID idMapItem)
{
	BROADCAST_SET::iterator it=m_setBCMapItem.begin();
	for (; it!=m_setBCMapItem.end(); it++)
	{
		if (idMapItem == *it)
			return;
	}
	m_setBCMapItem.push_back(idMapItem);
	
	CMapItem* pMapItem = MapManager()->QueryMapItem(idMapItem);
	if (pMapItem)
	{
		pMapItem->QueryMapThing()->SendShow(this);
	}
}

//////////////////////////////////////////////////////////////////////
void CRole::RemoveFromBCMapItemSet(OBJID idMapItem)
{
	//NPC������������ʱ���ģ�����ֻ��֪ͨ�ͻ���
	CUser* pUser = NULL;
	if (QueryObj(OBJ_USER, IPP_OF(pUser)))
	{
		CMsgAction msg;
		if (msg.Create(idMapItem, 0, 0, 0, actionDisappear, 1))
			SendMsg(&msg);
	}

	BROADCAST_SET::iterator it=m_setBCMapItem.begin();
	for (; it!=m_setBCMapItem.end(); it++)
	{
		if (idMapItem == *it)
		{
			m_setBCMapItem.erase(it);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////
void CRole::SeeMapThing(OBJID idMapThing)
{
	BROADCAST_SET::iterator it=m_setAnotherMapThing.begin();
	for (; it!=m_setAnotherMapThing.end(); it++)
	{
		if (idMapThing == *it)
			return;
	}
	m_setAnotherMapThing.push_back(idMapThing);
	
	CBooth* pBooth = MapManager()->QueryBooth(idMapThing);
	if (pBooth)
	{
		pBooth->SendShow(this);
	}
}

//////////////////////////////////////////////////////////////////////
void CRole::DisappearMapThing(OBJID idMapThing)
{
	CUser* pUser = NULL;
	if (QueryObj(OBJ_USER, IPP_OF(pUser)))
	{
		CMsgAction msg;
		if (msg.Create(idMapThing, 0, 0, 0, actionDisappear, 2))
			SendMsg(&msg);
	}
	
	BROADCAST_SET::iterator it=m_setAnotherMapThing.begin();
	for (; it!=m_setAnotherMapThing.end(); it++)
	{
		if (idMapThing == *it)
		{
			m_setAnotherMapThing.erase(it);
			break;
		}
	}
}

OBJID CRole::FindAttackUser(int nRange)
{
	if(!IsMonster())
		return ID_NONE;
	BROADCAST_SET::iterator it=m_setBCRole.begin();
	for (; it!=m_setBCRole.end(); it++)
	{
		OBJID idRole = *it;
		IRole* pRole = RoleManager()->QueryRole(idRole);
		if (pRole && pRole->IsAlive())
		{
			if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
			{
				if(GetDistance(pUser) < nRange)
					return idRole;
			}
		}
	}
	return ID_NONE;
}

//////////////////////////////////////////////////////////////////////
void CRole::BroadcastRoomMsg(CNetMsg* pMsg, bool bSendSelf,EM_SENDNPCFLAG sendNpc/* = EM_SENDNPCFLAG_TRUE*/)
{
	CHECK(pMsg);
	CGameMap* pMap = GetMap();
	CHECK (pMap);

	CUser*	pUser		= NULL;
	IRole*	pRole		= NULL;
	CMonster* pMonster	= NULL;
// 	bool bSendToNpc	= (QueryObj(OBJ_USER, IPP_OF(pUser))
// 						|| (QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && (pMonster->IsCallPet() || pMonster->IsEudemon()))
// 						|| bSendSelf);
	//20080111����:����npc��־---
//	bSendToNpc = (bSendToNpc && (sendNpc==EM_SENDNPCFLAG_TRUE));
	//---------------------------

	if (bSendSelf)
		m_setBCRole.push_back(this->GetID());
	DEBUG_TRY
	BROADCAST_SET::iterator it=m_setBCRole.begin();
	for (; it!=m_setBCRole.end(); it++)
	{
		OBJID idRole = *it;
		IRole* pRole = RoleManager()->QueryRole(idRole);
		if (pRole)
		{
			if (pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster))/* && bSendToNpc*/)
			{
// 				if (/*!pMonster->IsCallPet() && */!pMonster->IsEudemon())
// 				{
// 					pRole->SendMsg(pMsg);
// 					bSendToNpc = false;
// 				}
				continue;
			}
			if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
			{
  				if(pUser && pUser->CanBeSee()==false)
					continue;
				pRole->SendMsg(pMsg);
			}
		}
	}
	DEBUG_CATCH2("CRole::BroadcastRoomMsg",pRole->GetID())
	if (bSendSelf)
		m_setBCRole.pop_back();

	//20080111����:����npc��־---
	//ǿ�Ʒ���
// 	if(sendNpc==EM_SENDNPCFLAG_ENFORCE)
// 	{
// 		MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(ID_NONE, pMsg);
// 	}
	//---------------------------
}

OBJID CRole::GetMaxEnmityAndActiveUserID()
{
	CMonster* pThisMonster = NULL;
	OBJID idUser = ID_NONE;
	int nMaxPower = 0;
	if( this->QueryObj( OBJ_MONSTER,( void** )&pThisMonster ) )
	{						
		for( int i = 0;i < m_vEnmity.size();i++ )
		{
			if( m_vEnmity[ i ].enmity > nMaxPower )
			{
				CUser* pTempUser = NULL;
				pThisMonster->FindAroundObj( OBJ_USER,m_vEnmity[ i ].userID,( void** )&pTempUser );
				if(pTempUser)
				{
					nMaxPower = m_vEnmity[ i ].enmity;
					idUser	= m_vEnmity[ i ].userID;
				}
				
				CMonster* pTempMonster = NULL;
				pThisMonster->FindAroundObj( OBJ_MONSTER,m_vEnmity[ i ].userID,( void** )&pTempMonster );
				if(pTempMonster)
				{
					if(pTempMonster->QueryOwnerUser())
					{
						nMaxPower = m_vEnmity[ i ].enmity;
						idUser	= pTempMonster->QueryOwnerUser()->GetID();
					}
				}
			}			
		}	
	}	
	return idUser;
}
//06.12.7
//����
//��ó���б������������ID
//20070209����:�ŵ�role��ȥ
OBJID CRole::GetMaxEnmityUserID()
{
	OBJID idUser = ID_NONE;
	int nMaxPower = 0;			
	for( int i = 0;i < m_vEnmity.size();i++ )
	{
		if( m_vEnmity[ i ].enmity > nMaxPower )
		{
			nMaxPower = m_vEnmity[ i ].enmity;
			idUser	= m_vEnmity[ i ].userID;
		}

	}
	return idUser;

}
//07.4.6
//����
//�¼Ӽ���״̬POWER
//07.4.9���񲨵�һ���޸�
//���ĺ�������ֵDWORDΪint,���ٷ��ؼ�����ֵ����Ϊ�����һ����������������ͬ��״̬��ȡ��������������߼�
//���ڷ������ӣ�����٣��˵ĵ���
int CRole::CalcStatusInfluence( DWORD dwStatus,DWORD dwInitValue,int nPower )
{

	int nBigType	= dwStatus / 10000;			//��һλ
	int nDataIdent	= dwStatus / 1000 % 10;		//�ڶ�λ
	if( BIGTYPE_BUFF == nBigType  )
	{
		if( DATAIDENT_PERCENT == nDataIdent )
			//return MulDiv( dwInitValue,nPower,150 );
			return MulDiv( dwInitValue,nPower,100 );
		else if( DATAIDENT_NUMBER == nDataIdent )
			return nPower;
	}
	else if( BIGTYPE_DEBUFF == nBigType )
	{
		if( DATAIDENT_PERCENT == nDataIdent )
			return MulDiv( dwInitValue,nPower,100 ) * ( -1 );
		else if( DATAIDENT_NUMBER == nDataIdent )
			return nPower * ( -1 );
	}
	else if( 30000 == dwStatus)
	{
		//return MulDiv( dwInitValue,nPower,100 );
		return dwInitValue*nPower*1.0f/100;
	}
	
	//�Ŷ�--�޸�BUF���Ե�������
	return 0/*dwInitValue*/;
}
//------------------------------------------
void CRole::ClsEnmityList()//20070428����:��ճ�ޱ�
{
	m_vEnmity.clear();
}
//-----------------------------------------
//  [2007-8-30] ���
bool greater_than(const ENMITY& info1, const ENMITY& info2)
{
	return info1.enmity >= info2.enmity;
}
//------------------------------------------
//20080105����:��ޱ�������.���ǳ�޼ӳ�
bool greater_than_enmAddi(const ENMITY& info1, const ENMITY& info2)
{
	if(info1.nMax == 1)
		return true;
	else
		if(info2.nMax== 1)
			return false;
		else
	        return info1.enmity+info1.nEnmAddi >= info2.enmity+info2.nEnmAddi;
}
//-------------------------------------------
const SET_ENMITY& CRole::GetSortedEnmityListByMaxEnmity()
{
	//stable_sort(m_vEnmity.begin(), m_vEnmity.end(), greater_than);
	sort(m_vEnmity.begin(), m_vEnmity.end(), greater_than);
	return m_vEnmity;
}

//--------------------------------------------
//20080105����:����б�������.���ǵ���޼�Ȩ
const SET_ENMITY& CRole::GetSortedEnmityListByMaxEnmity_enmAddi()
{
	//stable_sort(m_vEnmity.begin(), m_vEnmity.end(), greater_than_enmAddi);
	sort(m_vEnmity.begin(), m_vEnmity.end(), greater_than_enmAddi);
	return m_vEnmity;
}

void CRole::ClsEnmity(OBJID id)
{
	for(vector<ENMITY>::iterator it = m_vEnmity.begin(); it != m_vEnmity.end(); it ++)
	{
		if(it->userID == id)
		{
			m_vEnmity.erase(it);
			return;
		}
	}
}

bool CRole::CheckTargetEnmity(OBJID id)
{
	if(!IsMonster())
		return true;
//modify code by �ֵ�Ԩ begin 2011.6.22
	if( IsMonster() )
	{
		CMonster* pMonster = NULL;
		if(QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && pMonster)
		{
			if( pMonster->GetType() > 6100 && pMonster->GetType() <= 6130 )
			{
				IStatus* pStatus = this->QueryStatus(STATUS_RETORT_TENSEC);
				if( pStatus )
					return true;
				else
					return false;
			}
		}
	}
//modify code by �ֵ�Ԩ end
	OBJID idNewTarget = NULL;
	//�ҵ�����б�
	const SET_ENMITY& setEnmity = GetSortedEnmityListByMaxEnmity_enmAddi();
	
	if(setEnmity.size()==0)
	{
		return false;
	}

	if(setEnmity[0].userID == id)
		return true;
	
	if(setEnmity.size() == 1)
		return false;

	DWORD nNownmity = 0;
	for(int i = 1; i < setEnmity.size(); i++)
	{
		if(setEnmity[i].userID == id)
		{
			nNownmity = setEnmity[i].enmity + setEnmity[i].nEnmAddi;
			break;
		}
	}
	
	if((setEnmity[0].enmity + setEnmity[0].nEnmAddi) >= nNownmity * 1.01 )
	{
		return false;		
	}
	return true;
}

//------------------------------------------
//20080104����:	���ӳ��ֵ.
//�����¼�,���ܸ���.����ֵΪ��role�ڳ���б��е���ֵ.
int	CRole::AddEnmity(OBJID id,
		int nEnmity,
		int nEnmAddi/* = NULL*/)
{

	int newEnmity = 0;
	DWORD curTime = ::TimeGet();

	int amount = m_vEnmity.size();
	for(int i = 0;i< amount;i++)		//�ڳ���б����������˺��������ˣ��ۼ�
	{
		if(m_vEnmity[i].userID == id)
		{
			m_vEnmity[i].enmity += nEnmity;
			m_vEnmity[i].lastUpdateTime = curTime;//ʱ���
			m_vEnmity[i].nEnmAddi = __min(_MAX_ENMITYADDITION,
			__max(_MIN_ENMITYADDITION,m_vEnmity[i].nEnmAddi+nEnmAddi));//��޼�Ȩֵ
			newEnmity = m_vEnmity[i].enmity;
			break;
		}	
	}
	if(i == amount)		//һ���µĵ��ˣ����
	{
		ENMITY enmity;
		memset(&enmity,0,sizeof(ENMITY));
		enmity.userID = id;
		enmity.enmity = nEnmity;
		enmity.lastUpdateTime = curTime;//ʱ���
		enmity.nEnmAddi = __min(_MAX_ENMITYADDITION,
			__max(_MIN_ENMITYADDITION,nEnmAddi));//��޼�Ȩֵ
		enmity.nMax = 0;
		newEnmity = enmity.enmity;
		m_vEnmity.push_back(enmity);
#ifdef _DEBUG
		//���ǵ�size��С
		const int _warningSize = 50;
		int enmitySize = m_vEnmity.size();
		if(enmitySize > _warningSize)
		{
			LOGERROR( "�����ޱ�������,�������Ĺ�:%s enmitySize:%d",
				this->GetName(),enmitySize );
		}
#endif
	}

	return newEnmity;
}

//  [12/19/2007 ���]	ǿ��Ŀ��ֻ�ܹ����Լ�������3�롣���Ƕ�Ŀ��ʹ�õ�Debuff��
//	��Ҫ�µ�status��ʹ�Լ���Ϊ���˵�Ŀ�꣬�ҵ��˲��ܸı�Ŀ�ꡣֻ�������Ч��
//	�д�DEBUF��Ŀ�겻��ͬ����FALSE,���򷵻�TRUE
BOOL CRole::CheckDebuffLockTarget(IRole *pRole, OBJID idTarget)
{
	CHECKF(pRole);
	
	if(idTarget == 0)	//	û��Ŀ��
		return TRUE;

	IStatus* pLockTargetStatus = pRole->QueryStatus(STATUS_LOCK_TARGET);
	if(pLockTargetStatus)
	{
		//	powerֵ����Ŀ��ID
		int nTargetID = pLockTargetStatus->GetPower();
		if(nTargetID != idTarget)
			return FALSE;
	}

	return TRUE;
}
