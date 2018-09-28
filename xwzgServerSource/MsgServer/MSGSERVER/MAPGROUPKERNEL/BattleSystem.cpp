// BattleSystem.cpp: implementation of the CBattleSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "mapgroup.h"
#include "I_Role.h"
#include "BattleSystem.h"
#include "Npc.h"
#include "GiftTempleteManager.h"

const int	BOW_SHIELD_HITRATE_PERCENT			= 12;			// 盾牌对箭矢的遮挡率


const int _USERATTRIB_MANA = 113;//2;
const int _USERATTRIB_LIFE = 112;//0;//  [12/24/2007 陈剑飞]


//暴击-----------------begin--------------------------------------
const int	WOULDEDPHYDAMAGE			= 1;	//普通	
const float	PHYSICAL_CRITICALHIT		= 2;	//致命一击
const float MAGIC_CRITICALHIT			= 2;	//法术爆发
//---------------------end-----------------------------------------


MYHEAP_IMPLEMENTATION(CBattleSystem,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char	szZcxKey[]			= "level";
char	szLevupexpTable[]	= _TBL_LEVEXP;

//////////////////////////////////////////////////////////////////////
CBattleSystem::CBattleSystem(PROCESS_ID idProcess, IRole* pOwner)
{
	m_idProcess	= idProcess;
	m_pOwner	= pOwner;
	m_idTarget	= ID_NONE;
}

//////////////////////////////////////////////////////////////////////
CBattleSystem::~CBattleSystem()
{

}

//////////////////////////////////////////////////////////////////////
//06.4.19王玉波修改
BOOL CBattleSystem::checkMonsterTag(int monsterID)
{
	BOOL bRe = false;
	if(m_idTarget == ID_NONE)
		return false;

	IRole* pTarget	= m_pOwner->GetMap()->QueryRole(m_pOwner->GetPosX(), m_pOwner->GetPosY(), m_idTarget);
	if (pTarget)
	{
		if(pTarget->IsMonster())
		{
			CMonsterPtr pMonster=NULL;
			if(pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
			{
				if(pMonster->GetType() == monsterID)
					bRe = true;
			}	
		}
	}
	return bRe;	
}

BOOL CBattleSystem::ProcAttack_Hand2Hand()
{//战斗消息也在本函数中发送给客户端

	DEBUG_TRY
	if (!m_pOwner || m_idTarget == ID_NONE)
		return false;

	if (!IsBattleMaintain())
		return false;

	DEBUG_CATCH( "ProcAttack_Hand2Hand IsBattleMain()" );

	IRole* pTarget	= m_pOwner->GetMap()->QueryRole(m_pOwner->GetPosX(), m_pOwner->GetPosY(), m_idTarget);
	if (!pTarget)
	{
		this->ResetBattle();
		return true;
	}
	bool bOldIsAlive = pTarget->IsAlive();

	if (!m_pOwner->IsAtkable(pTarget))
	{
		this->ResetBattle();
		return true;
	}
	// 自动(被动)技能触发
	if(m_pOwner->AutoSkillAttack(pTarget))
		return true;
	DEBUG_TRY
	if(m_pOwner->IsUser())
		m_pOwner->CheckCrime(pTarget);
	DEBUG_CATCH("ProcAttack_Hand2Hand CheckCrime");
// 	CNpc* pNpc;
// 	pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc));
// 	CUser* pOwnerUser = m_pOwner->QueryOwnerUser();

	DEBUG_TRY
	// 躲避失败，计算伤害值。
	int nTagLife	= pTarget->GetLife();//还未减血
	DWORD dwBattleAddInfo = BATTLEADDINFO::INFO_INVALID;//无效的
	
	int nDamage		= m_pOwner->Attack( pTarget,dwBattleAddInfo );

	m_pOwner->SendDamageMsg( pTarget->GetID(),nDamage,dwBattleAddInfo );

	if( nDamage == 0 )
		return true;
	DEBUG_CATCH( "ProcAttack_Hand2Hand::m_pOwner->Attack and m_pOwner->AdditionMagic!" );

	//杀死
	DEBUG_TRY
	if ( !pTarget->IsAlive() && bOldIsAlive)
	{
		m_idTarget = ID_NONE;
		m_pOwner->Kill( pTarget,m_pOwner->IsBowman() ? DIE_ARROWSHOOT : DIE_NORMAL );
	}
	DEBUG_CATCH( "ProcAttack_Hand2Hand::m_pOwner->Kill!" );
	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CBattleSystem::IsBattleMaintain()
{
	if(m_idTarget == ID_NONE)
		return false;

	IRole* pTarget	= RoleManager()->QueryRole(m_idTarget);
	if(!pTarget)
	{
		this->ResetBattle();
		return false;
	}

	if(!m_pOwner->IsAtkable(pTarget))
	{
		this->ResetBattle();
		return false;
	}

	if(pTarget->IsWing() && !m_pOwner->IsWing() && !(m_pOwner->IsBowman() || m_pOwner->IsSimpleMagicAtk()))
	{
		this->ResetBattle();
		return false;
	}

	int nDistance = 0;
	if (m_pOwner->IsMonster())
	{
		nDistance = m_pOwner->GetDistance(pTarget->QueryMapThing());
	}
	else
	{
		int nPosX = 0, nPosY = 0;
		pTarget->GetFootPrint(nPosX, nPosY);

		nDistance = m_pOwner->GetDistance(nPosX, nPosY);
	}

	if (nDistance > m_pOwner->GetAttackRange(pTarget->GetSizeAdd()))
	{
		this->ResetBattle();
		return false;
	}

	if (!pTarget->IsAlive())
	{
		this->ResetBattle();
		return false;
	}

	if (!m_pOwner->IsAlive())
	{
		this->ResetBattle();
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CBattleSystem::CreateBattle(OBJID idTarget)
{
	if (idTarget == ID_NONE)
		return false;

	m_idTarget	= idTarget;
	return true;
}

//////////////////////////////////////////////////////////////////////
void CBattleSystem::ResetBattle()
{
	if(m_idTarget)//20070614修罗:加一个判断
	{
		m_idTarget	= ID_NONE;

		//20070429修罗:取消lockAtk标志-------------
		//20070614修罗:放到最底层
		CUser *pUserTemp=NULL;
		if(m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUserTemp)))
		{
			pUserTemp->SynchroLockAtkEnd();
		}
		//-----------------------------------------
	}
}
int CBattleSystem::AdjustExp(int nDamage, int nAtkLev, int nDefLev)
{
	if(nAtkLev > USERMAXLEVEL)
		nAtkLev = USERMAXLEVEL;

	int nExp = nDamage;

	int nDeltaLev = nAtkLev - nDefLev;
	if(nDeltaLev <= -21 )
		nExp *= 0.1f;
	else if(nDeltaLev > -21 && nDeltaLev <= -11)
		nExp *= 0.5f;
	else if(nDeltaLev > -11 && nDeltaLev <= 2)
		;//100%
	else if(nDeltaLev > 2 && nDeltaLev <= 5)
		nExp *= 0.8f;
	else if(nDeltaLev > 5 && nDeltaLev <= 10)
		nExp *= 0.5f;
	else if(nDeltaLev > 10 && nDeltaLev <= 20)
		nExp *= 0.2f;
	else if(nDeltaLev > 20)
		nExp *= 0.1f;
	return ::CutTrail(0, nExp);
}

//////////////////////////////////////////////////////////////////////
int CBattleSystem::AdjustDrop(int nDrop, int nAtkLev, int nDefLev)
{
	return nDrop;
}

int CBattleSystem::CalcDamage( DWORD& dwBattleAddInfoRef,const ATK_TYPE& type,IRole* pAtker,IRole* pTargeter,int nPower,int nSkillHitRate, int nPower2, int nAtackType )
{
	CHECKF( pAtker && pTargeter );
	
	int nDamage = 0;
	int nAtkType = 0;// 区分攻击的类型 [12/19/2007 陈剑飞]
	if( ATKTYPE_PHYSICAL_COMMON == type )//物理普通攻击
	{
		nDamage = CBattleSystem::CalcDamage_PhysicalCommon( pAtker,pTargeter,dwBattleAddInfoRef );
		nAtkType = 1;
	}
	else if( ATKTYPE_MAGIC_SKILL == type )//魔法技能攻击
	{
		nDamage = CBattleSystem::CalcDamage_MagicSkill( pAtker,pTargeter,nPower,dwBattleAddInfoRef ,nPower2, nAtackType);
		nAtkType = 3;
	}	

	if((pAtker->IsUser() || pAtker->QueryOwnerUser()) && (pTargeter->IsUser() || pTargeter->QueryOwnerUser()))
	{
		nDamage /= 10;
	}

	nDamage = __max(0, nDamage);

	//--------------end----------------------------------------------------
	if( nDamage > 0 )
	{
		//定制装备处理
		if (pAtker->IsUser())
		{
			CItem *pWeapon = pAtker->GetWeaponR();
			if(pWeapon)
			{
				int nWeaponType = pWeapon->GetTypeID();
				int nWeaponAddLev = pWeapon->GetEquipAddLev();//强化等级
			
				int nStatPower = 0, nSec = 6;
				if (nWeaponType>=145900 && nWeaponType<=145904 && ::RandGet(100)<2)//锤
				{
					nStatPower = 0;
					if (nWeaponType%10==0 && nWeaponAddLev>=3) nStatPower=1000;
					else if (nWeaponType%10==1 && nWeaponAddLev>=4) nStatPower=2000;
					else if (nWeaponType%10==2 && nWeaponAddLev>=5) nStatPower=4000;
					else if (nWeaponType%10==3 && nWeaponAddLev>=6) nStatPower=6000;
					else if (nWeaponType%10==4 && nWeaponAddLev>=7) nStatPower=8000;
					if (nStatPower)
						CRole::AttachStatus(pAtker, STATUS_EQUIP_DEFUP, nStatPower, nSec, 0, 0, 0, 0, pAtker->GetID());
				}
				else if (nWeaponType>=144900 && nWeaponType<=144904 && ::RandGet(100)<2)//弓
				{
					nStatPower = 0;
					if (nWeaponType%10==0 && nWeaponAddLev>=3) nStatPower=140;
					else if (nWeaponType%10==1 && nWeaponAddLev>=4) nStatPower=160;
					else if (nWeaponType%10==2 && nWeaponAddLev>=5) nStatPower=170;
					else if (nWeaponType%10==3 && nWeaponAddLev>=6) nStatPower=190;
					else if (nWeaponType%10==4 && nWeaponAddLev>=7) nStatPower=200;
					if (nStatPower)
						nDamage = nDamage*nStatPower/100;
				}
				else if (nWeaponType>=143900 && nWeaponType<=143904 && ::RandGet(100)<2)//刀
				{
					nStatPower = 0;
					if (nWeaponType%10==0 && nWeaponAddLev>=3) nStatPower=100;
					else if (nWeaponType%10==1 && nWeaponAddLev>=4) nStatPower=200;
					else if (nWeaponType%10==2 && nWeaponAddLev>=5) nStatPower=300;
					else if (nWeaponType%10==3 && nWeaponAddLev>=6) nStatPower=400;
					else if (nWeaponType%10==4 && nWeaponAddLev>=7) nStatPower=500;
					if (nStatPower)
						CRole::AttachStatus(pAtker, STATUS_EQUIP_CRIUP, nStatPower, nSec, 0, 0, 0, 0, pAtker->GetID());
				}
				else if (nWeaponType>=142900 && nWeaponType<=142904 && ::RandGet(100)<2)//剑
				{
					nStatPower = 0;
					if (nWeaponType%10==0 && nWeaponAddLev>=3) nStatPower=5;
					else if (nWeaponType%10==1 && nWeaponAddLev>=4) nStatPower=7;
					else if (nWeaponType%10==2 && nWeaponAddLev>=5) nStatPower=8;
					else if (nWeaponType%10==3 && nWeaponAddLev>=6) nStatPower=9;
					else if (nWeaponType%10==4 && nWeaponAddLev>=7) nStatPower=10;
					if (nStatPower)
						CRole::AttachStatus(pTargeter, STATUS_EQUIP_BEDAMAGEUP, nStatPower, nSec, 0, 0, 0, 0, pAtker->GetID());
				}
				else if (nWeaponType>=141900 && nWeaponType<=141904 && ::RandGet(100)<2)//杖
				{
					nStatPower = 0;
					if (nWeaponType%10==0 && nWeaponAddLev>=3) nStatPower=15;
					else if (nWeaponType%10==1 && nWeaponAddLev>=4) nStatPower=30;
					else if (nWeaponType%10==2 && nWeaponAddLev>=5) nStatPower=40;
					else if (nWeaponType%10==3 && nWeaponAddLev>=6) nStatPower=50;
					else if (nWeaponType%10==4 && nWeaponAddLev>=7) nStatPower=55;
					if (nStatPower)
						pAtker->AddAttrib(_USERATTRIB_LIFE, pAtker->GetMaxLife()*nStatPower/1000, SYNCHRO_TRUE);
				}
				else if (nWeaponType>=146900 && nWeaponType<=146904 && ::RandGet(100)<2)//扇
				{
					nStatPower = 0;
					if (nWeaponType%10==0 && nWeaponAddLev>=3) nStatPower=500;
					else if (nWeaponType%10==1 && nWeaponAddLev>=4) nStatPower=1000;
					else if (nWeaponType%10==2 && nWeaponAddLev>=5) nStatPower=2000;
					else if (nWeaponType%10==3 && nWeaponAddLev>=6) nStatPower=3000;
					else if (nWeaponType%10==4 && nWeaponAddLev>=7) nStatPower=4000;
					if (nStatPower)
						CRole::AttachStatus(pTargeter, STATUS_EQUIP_DEFDOWN, nStatPower, nSec, 0, 0, 0, 0, pAtker->GetID());
				}
			}
			//帮派技能处理
			CUser* pAtkUser = NULL;
			if( pAtker->QueryObj( OBJ_USER,( void** )&pAtkUser) )
			{
//				if (pAtkUser->GetSynID() != ID_NONE)
				{
					if (pAtkUser->QueryMagic())
						nDamage += pAtkUser->QueryMagic()->ProcessSynMagic(pAtkUser, pTargeter);
				}
				//宠物附体技能
				if (pAtkUser->QueryMagic())
					nDamage += pAtkUser->QueryMagic()->ProcessSoulEudemonMagic(pAtkUser, pTargeter);
			}
		}
		//师徒任务特殊怪物的处理
		CMonster* pTargetMonster = NULL;
		if (pTargeter->QueryObj(OBJ_MONSTER, (void**)&pTargetMonster))
		{
			if (!pTargetMonster->IsEudemon() && pTargetMonster->IsTutorBoss())
			{
				if (!pAtker->QueryStatus(STATUS_TUTOR_BOSS))
				{
					nDamage=0;
					dwBattleAddInfoRef = BATTLEADDINFO::INFO_DODGE;
					return nDamage;
				}
			}
		}
		IStatus* pStatus = NULL;
		pStatus = pTargeter->QueryStatus( STATUS_SHIELD ); //真气护体
		if( pStatus )	//李京哲0413
		{	
			int	nShieldPower = pStatus->GetPower();
			if( nDamage >= nShieldPower )	//本次伤害大于魔法盾可抵消的伤害
			{
				nDamage -= nShieldPower;
				CRole::DetachStatus( pTargeter,STATUS_SHIELD );
			}
			else
			{
				pStatus->SetPower( nShieldPower - nDamage );	
				nDamage = 0;
				dwBattleAddInfoRef = BATTLEADDINFO::INFO_LOWDAMAGE;
				return 0;
			}
		}
		pStatus = pTargeter->QueryStatus( STATUS_SYN_SHIELD );
		if( pStatus )
		{	
			int	nShieldPower = pStatus->GetPower();
			if( nDamage >= nShieldPower )	//本次伤害大于魔法盾可抵消的伤害
			{
				nDamage -= nShieldPower;
				CRole::DetachStatus( pTargeter,STATUS_SYN_SHIELD );
			}
			else
			{
				pStatus->SetPower( nShieldPower - nDamage );	
				nDamage = 0;
				dwBattleAddInfoRef = BATTLEADDINFO::INFO_LOWDAMAGE;
				return 0;
			}
		}
		pStatus = pTargeter->QueryStatus( STATUS_SHIELD_1 ); //三味真火，无敌
		if( pStatus )	//李京哲0413
		{		
			nDamage = 0;
			dwBattleAddInfoRef = BATTLEADDINFO::INFO_LOWDAMAGE;
			return 0;
		}
		pStatus = pTargeter->QueryStatus( STATUS_SHIELD_2 ); //无敌
		if( pStatus )	//李京哲0413
		{		
			nDamage = 0;
			dwBattleAddInfoRef = BATTLEADDINFO::INFO_LOWDAMAGE;
			return 0;
		}
		int nChangeData = 0;
		pStatus = pTargeter->QueryStatus( STATUS_MAGICDAMAGEUP_PERCENT );//魔法伤害增大
		if( pStatus )
		{
			nChangeData += ( -1 ) * CRole::CalcStatusInfluence( STATUS_MAGICDAMAGEUP_PERCENT,nDamage,pStatus->GetPower() );
		}
		pStatus = pTargeter->QueryStatus( STATUS_MAGICDAMAGEDOWN_PERCENT );
		if( pStatus )
		{
			nChangeData += ( -1 ) * CRole::CalcStatusInfluence( STATUS_MAGICDAMAGEDOWN_PERCENT,nDamage,pStatus->GetPower() );
		}
		pStatus = pTargeter->QueryStatus( STATUS_DAMAGEUP_PERCENT );
		if( pStatus )
		{
			nChangeData += ( -1 ) * CRole::CalcStatusInfluence( STATUS_DAMAGEUP_PERCENT,nDamage,pStatus->GetPower() );
		}
		pStatus = pTargeter->QueryStatus( STATUS_DAMAGEDOWN_PERCENT );
		if( pStatus )
		{
			nChangeData += ( -1 ) * CRole::CalcStatusInfluence( STATUS_DAMAGEDOWN_PERCENT,nDamage,pStatus->GetPower() );
		}
		//2007827修罗:-----------------
		pStatus = pTargeter->QueryStatus( STATUS_DAMAGEDOWN_PERCENT_TN );
		if( pStatus )
		{
			nChangeData += ( -1 ) * CRole::CalcStatusInfluence( STATUS_DAMAGEDOWN_PERCENT_TN,nDamage,pStatus->GetPower() );
		}
		//-----------------------------
		pStatus = pTargeter->QueryStatus( STATUS_FRENZY );//狂暴
		if( pStatus )
		{
			nChangeData += ( -1 ) * CRole::CalcStatusInfluence( STATUS_FRENZY,nDamage,pStatus->GetPower() );
		}
		pStatus = pTargeter->QueryStatus( STATUS_EQUIP_BEDAMAGEUP );//定制装备
		if( pStatus )
		{
			nChangeData += nDamage*pStatus->GetPower()/100;
		}
		//攻击者的状态
		pStatus = pAtker->QueryStatus(STATUS_SELF_DAMAGEUP);//孤注一掷
		if (pStatus)
		{
			nChangeData += CRole::CalcStatusInfluence( STATUS_SELF_DAMAGEUP,nDamage,pStatus->GetPower() );
		}
		pStatus = pAtker->QueryStatus(STATUS_SELF_DAMAGEUP_PERCENT);//火神下凡
		if (pStatus)
		{
			nChangeData += CRole::CalcStatusInfluence( STATUS_SELF_DAMAGEUP_PERCENT,nDamage,pStatus->GetPower() );
		}
		pStatus = pAtker->QueryStatus(STATUS_SELF_DAMAGEUP_NUMBER);
		if (pStatus)
		{
			nChangeData += CRole::CalcStatusInfluence( STATUS_SELF_DAMAGEUP_NUMBER,nDamage,pStatus->GetPower() );
		}
		nDamage += nChangeData;
		
		CUser* pTagUser = NULL;
		if(pTargeter->QueryObj( OBJ_USER,( void** )&pTagUser ) )//一定是user才有此状态.只有玩家有Mana;
		{
			if(pTagUser && pTagUser->QueryMagic())
				pTagUser->QueryMagic()->ProcessSoulEudemonMagic(pTagUser, pAtker);
			pStatus = pTargeter->QueryStatus(STATUS_MAGIC_SHIELD);//魔法盾,用MP抵消伤害
			if (pStatus)
			{
				int i64Data = __max(1, nDamage);
				int nPower = pStatus->GetPower();
				if (nPower >= i64Data)
				{
					if(i64Data <= pTargeter->GetMana())
					{
						pTargeter->AddAttrib(_USERATTRIB_MANA, -1*i64Data, SYNCHRO_TRUE);
						dwBattleAddInfoRef = BATTLEADDINFO::INFO_LOWDAMAGE;
						return 0;
					}
					else
					{
						int nMana = pTargeter->GetMana();
						int nLostLife = i64Data-nMana;
						pTargeter->AddAttrib(_USERATTRIB_MANA, -1*nMana, SYNCHRO_TRUE);
						//pTargeter->AddAttrib(_USERATTRIB_LIFE, -1*nLostLife, SYNCHRO_TRUE);
						nDamage = nLostLife;
					}
					nPower -= i64Data;
					if (nPower==0)
						CRole::DetachStatus(pTargeter,STATUS_MAGIC_SHIELD);
					else
						pStatus->SetPower(nPower);
				}
				else
				{
					int nLostLife = i64Data-nPower;
					if (nPower <= pTargeter->GetMana())
						pTargeter->AddAttrib(_USERATTRIB_MANA, -1*nPower, SYNCHRO_TRUE);
					else
					{
						int nMana = pTargeter->GetMana();
						nLostLife += nPower-nMana;
						pTargeter->AddAttrib(_USERATTRIB_MANA, -1*nMana, SYNCHRO_TRUE);
					}
					nDamage = nLostLife;
					//pTargeter->AddAttrib(_USERATTRIB_LIFE, -1*nLostLife, SYNCHRO_TRUE);
					CRole::DetachStatus(pTargeter,STATUS_MAGIC_SHIELD);
				}
			}
			// 天赋被动技能 [12/25/2007 陈剑飞]
// 			if(pAtkUser)
// 			{
// 				int nDamageGift = Adjust_Damage_GiftPassivityMagic( pAtkUser,pTagUser,nDamage,nAtkType );
// 				nDamage = nDamageGift;
// 			}
			//-------------------------------------------------------------------------------------
// 			pStatus = pTargeter->QueryStatus( STATUS_CRAZE );//狂热，用MP抵消物理系伤害
// 			if( pStatus )
// 			{
// 				int nPower = pStatus->GetPower();//还有多少点数
// 				if( nPower <= 0 )
// 				{
// 					CRole::DetachStatus( pTargeter,STATUS_CRAZE );
// 					return nDamage;
// 				}
// 				int nCanWithStandPow = __min( nPower,pTagUser->GetMana() );//可以抵消的点数
// 				if( nDamage >= nCanWithStandPow )//抵消一部分
// 				{
// 					nDamage -= nCanWithStandPow;
// 					pTagUser->AddAttrib( _USERATTRIB_MANA,( -1 ) * nCanWithStandPow,true );//减MANA
// 					nPower -= nCanWithStandPow;
// 					if( nPower <= 0 )
// 					{
// 						CRole::DetachStatus( pTargeter,STATUS_CRAZE );
// 					}
// 					else
// 					{
// 						pStatus->SetPower( nPower );
// 					}
// 					
// 				}
// 				else//全部抵消了
// 				{
// 					pStatus->SetPower( nPower - nDamage );
// 					pTagUser->AddAttrib( _USERATTRIB_MANA,( -1 ) * nDamage,true );
// 					nDamage = 0;
// 				}
// 			}
			// 被动技能，精神领袖 [12/26/2007 陈剑飞]
// 			if( pTagUser->QueryMagic( MAGIC_JSLX ) )
// 			{
// 				IMagicData* pData=pTagUser->QueryMagic(MAGIC_JSLX);
// 				if (pData)
// 				{
// 					int nPercent = pData->GetInt(MAGICDATA_PERCENT);
// 					int nLife = pTagUser->GetLife();
// 					if( nDamage >= nLife )
// 					{
// 						if( RandGet(100) <= nPercent )
// 						{
// 							nDamage = 0;
// 						}
// 					}	
// 				}
// 			}
		//------------------------------------------------------------------------
		}
	}

    int nAddDamageRate = 0;
	int nDecDamageRate = 0;
	CUser* pAtkUser = NULL;
	if (pAtker->QueryObj(OBJ_USER, ( void** )&pAtkUser))
	{
		nAddDamageRate = pAtkUser->GetJinmeiIncDamageRate();
	}

	CUser* pTargetUser = NULL;
	if(pTargeter->QueryObj( OBJ_USER,( void** )&pTargetUser ))
	{
		nDecDamageRate = pTargetUser->GetJinmeiDecDamageRate();
	}

	nDamage = nDamage + nDamage * (float)(nAddDamageRate / 1000.0f);
	nDamage = nDamage * (float)((1000 - nDecDamageRate) / 1000.0f);

	return nDamage;
}
//-----------------------------------------------------------------------------------------------------------
//07.1.30
//王玉波
//等级差修正
/*说明
LevModifyImpossible = 1.5%
LevModifyHard = 2%
LevModifyCommon = 5%
LevModifyEasily = 20%
LevModifyDisdain = 10%

若等级差 ＞10			等级差修正为[1+（攻方等级-守方等级）* ImpossibleLevModify]
若10≥等级差 ＞5		等级差修正为[1+（攻方等级-守方等级）* LevModifyHard]
若5≥等级差 ≥-5		等级差修正为[1+（攻方等级-守方等级）* LevModifyCommon]
若-5＞等级差＞-10		等级差修正为[1+（攻方等级-守方等级）*LevModifyEasily]
若等级差≤-10			等级差修正恒定为LevModifyDisdain
-------------------------------------------------------------------------------------------------------*/
float CBattleSystem::GetLevelDifference( int nAtkLeve,int nTagLevel )
{
	return 1.0;			//07.3.29王玉波修改,因为都是乘法，现屏蔽等级差，return 1变通实现
	const float fLevModifyImpossible	= 0.015f;
	const float fLevModifyHard			= 0.02f;
	const float fLevModifyCommon		= 0.05f;
//	const float fLevModifyEasily		= 0.2f;//暂时改为0.1,-5 ~-10时会秒怪
	const float fLevModifyEasily		= 0.1f;
	const float fLevModifyDisdain		= 0.1f;
	
	int nLevelDiff = nAtkLeve - nTagLevel;
	if( nLevelDiff > 10 )
	{
		return nLevelDiff * fLevModifyImpossible + 1;
	}
	else if( nLevelDiff <= 10 && nLevelDiff > 5 )
	{
		return nLevelDiff * fLevModifyHard + 1;
	}
	else if( nLevelDiff <=5 && nLevelDiff >= -5 )
	{
		return nLevelDiff * fLevModifyCommon + 1;
	}
	else if( nLevelDiff < -5 && nLevelDiff > -10 )
	{
		return nLevelDiff * fLevModifyEasily + 1;
	}
	else
	{
		return fLevModifyDisdain;
	}
			
}
 
//---------------------------------------------------------------------------------------------------
//10.6.3
//判断普通物理攻击是否命中
//返回值:true命中,false未打中
/*
命中率= 命中基数+自己的命中/（自己的命中+对方躲闪）*命中系数
命中基数 = 25%
命中系数 = 1
--------------------------------------------------------------------------------------------------*/
bool CBattleSystem::IsHitRatePhysicalCommon( IRole* pAtker,IRole* pTargeter )
{
	DWORD	dwAttackHitRate = pAtker->GetAtkHitRate();
	DWORD	dwBeAttackDodge = pTargeter->GetDdg();

	DWORD	dwResult = 40 + ( float )dwAttackHitRate / ( float )( dwAttackHitRate + dwBeAttackDodge )* 100;//转化为1-100之间的数
	if( RandGet( 100 ) <= dwResult)
		return true;
	return false;
}
//---------------------------------------------------------------------------------------------------
//10.6.3
//判断普通物理攻击是否致命一击
/*
非技能攻击
命中率= 自己的暴击/（自己的暴击+对方的坚韧）
--------------------------------------------------------------------------------------------------*/
bool CBattleSystem::IsCriticalHit(IRole* pRole,bool bMagic ,IRole* pTarget /* = NULL */)
{
	CUser* pUser = NULL; 
	CUser* pTargetTmp = NULL;
	DWORD dwCriticalHitRate = pRole->GetCrit();
	DWORD dwTough = pTarget->GetTough();
	DWORD dwResult = 500 + ( float )dwCriticalHitRate / ( float )( dwCriticalHitRate + 5 * dwTough )* 10000;//转化为1-100之间的数

	if( RandGet( 10000 ) <= dwResult )
	{	
		return true;
	}
	return false;
}

const float PHYSICAL_ATKTYPEADD = 1.1f;
const float PHYSICAL_ATKTYPENOR = 1.0f;
float CBattleSystem::CheckATKType(DWORD atkType,DWORD defType)
{
	float fret = PHYSICAL_ATKTYPENOR;
	if(atkType != 0)
	{
		if(defType == 0)
			fret = PHYSICAL_ATKTYPEADD;
		else
		{
			switch(atkType)
			{
			case 1:
				if(defType == 2)
					fret = PHYSICAL_ATKTYPEADD;
				break;
			case 2:
				if(defType == 3)
					fret = PHYSICAL_ATKTYPEADD;
				break;
			case 3:
				if(defType == 4)
					fret = PHYSICAL_ATKTYPEADD;
				break;
			case 4:
				if(defType == 1)
					fret = PHYSICAL_ATKTYPEADD;
				break;
			}
		}
	}
	return fret;
}

//------------------------------------------------------------------------------------------------
//07.1.31
//王玉波
//判断物理系技能是否命中
//参数说明：pAtker攻击者指针，pTargeter被攻击者指针，nPercentValue数据库中的百分比字段
//20070713:其中命中率若大于95%，则取值为95%
bool CBattleSystem::IsHitRatePhysicalSkill( IRole* pAtker,IRole* pTargeter,int nPercentValue )
{
	float	fDifference = GetLevelDifference( pAtker->GetLev(),pTargeter->GetLev() );
	DWORD	dwAttackHitRate = pAtker->GetAtkHitRate();
	DWORD	dwBeAttackDodge = pTargeter->GetDdg();
	DWORD	dwResult = ( ( float )dwAttackHitRate / ( float )( dwAttackHitRate + dwBeAttackDodge ) + ( float )nPercentValue / 100.0 ) *  fDifference * 100;//转化为1-100之间的数
	//if( RandGet( 100 ) <= dwResult )
	if(dwResult >= 100)
	{
		dwResult = 60 + RandGet( 30 );
	}
	if( RandGet( 100 ) <= __min(95,dwResult) )
		return true;
	return false;
}

//---------------------------------------------------------------------------------------------------------
//07.1.31
//王玉波
//计算普通物理攻击伤害
//07.4.2王玉波第一次修改:考虑buff与debuff
//07.4.18王玉波第二次修改：增加状态“识破”相关功能，如果这一次躲避，下一次必然暴击
//07.7.9 王玉波第三次修改，增加DWORD& dwBattleAddInfoRef参数，取得战斗附加信息
int CBattleSystem::CalcDamage_PhysicalCommon( IRole* pAtker,IRole* pTargeter,DWORD& dwBattleAddInfoRef )
{
	CUser* pAtkUser = NULL;
	pAtker->QueryObj( OBJ_USER,( void** )&pAtkUser ); //攻击者是玩家

	if( pAtker->IgnoreTargetDodge() || IsHitRatePhysicalCommon( pAtker,pTargeter ) )//是否命中
	{
		DWORD dwAtk = 0;
		BOOL bCritHit = false;//致命一击标志
		if( IsCriticalHit( pAtker,false ,pTargeter) )//造成了致命一击
		{
			bCritHit = true;
		}

		//结算最后伤害,Physical Common Damage = 自己的外攻^2/（自己的外攻+对方外防）

		//外攻
		dwAtk = pAtker->GetAtk();
		//几率外功伤害
		if(pAtkUser)
			dwAtk += pAtkUser->GetRandPhyAtk(dwAtk);
		//攻击随即
		int nRan = RandGet(10);
		dwAtk = (DWORD)((double)dwAtk * ( 95.0f + nRan) / 100.0f);
		DWORD dwTagDefence = pTargeter->GetDef();
		//削弱外功防御
		if(pAtkUser)
			dwTagDefence -= pAtkUser->GetDecPhyDef(dwTagDefence);
		DWORD nDamage = ((double)dwAtk /(double) (dwAtk + dwTagDefence)) * dwAtk;

		//内攻
		dwAtk = pAtker->GetMgcAtk();
		//几率内功伤害
		if(pAtkUser)
			dwAtk += pAtkUser->GetRandMagAtk(dwAtk);
		//攻击随即
		nRan = RandGet(10);
		dwAtk = (DWORD)((double)dwAtk * ( 95.0f + nRan) / 100.0f);
		dwTagDefence = pTargeter->GetMagicDef();
		//削弱内功防御
		if(pAtkUser)
			dwTagDefence -= pAtkUser->GetDecPhyDef(dwTagDefence);
		nDamage += ((double)dwAtk /(double) (dwAtk + dwTagDefence)) * dwAtk;

		int dwThisDamage = 0;
		//冰
		dwAtk = pAtker->GetAddAttByType(2);
		if(pAtkUser)
			dwAtk += pAtkUser->GetRandAtt2(dwAtk);
		dwTagDefence = pTargeter->GetAddAttByType(12);
		if(pAtkUser)
			dwTagDefence -= pAtkUser->GetDecDef2(dwTagDefence);
		dwThisDamage = (dwAtk-dwTagDefence) * 5;
		nDamage += dwThisDamage >0 ? dwThisDamage : 0 ;
		
		//火
		dwThisDamage = 0;
		dwAtk = pAtker->GetAddAttByType(1);
		if(pAtkUser)
			dwAtk += pAtkUser->GetRandAtt1(dwAtk);
		dwTagDefence = pTargeter->GetAddAttByType(11);
		if(pAtkUser)
			dwTagDefence -= pAtkUser->GetDecDef1(dwTagDefence);
		dwThisDamage = (dwAtk-dwTagDefence) * 5;
		nDamage += dwThisDamage >0 ? dwThisDamage : 0 ;
		
		//毒
		dwThisDamage = 0;
		dwAtk = pAtker->GetAddAttByType(4);
		if(pAtkUser)
			dwAtk += pAtkUser->GetRandAtt4(dwAtk);
		dwTagDefence = pTargeter->GetAddAttByType(14);
		if(pAtkUser)
			dwTagDefence -= pAtkUser->GetDecDef4(dwTagDefence);
		dwThisDamage = (dwAtk-dwTagDefence) * 5;
		nDamage += dwThisDamage >0 ? dwThisDamage : 0 ;
		
		//玄
		dwThisDamage = 0;
		dwAtk = pAtker->GetAddAttByType(3);
		if(pAtkUser)
			dwAtk += pAtkUser->GetRandAtt3(dwAtk);
		dwTagDefence = pTargeter->GetAddAttByType(13);
		if(pAtkUser)
			dwTagDefence -= pAtkUser->GetDecDef3(dwTagDefence);
		dwThisDamage = (dwAtk-dwTagDefence) * 5;
		nDamage += dwThisDamage >0 ? dwThisDamage : 0 ;

		//相性攻击
		if(pAtkUser && pAtkUser->GetAtkType())
		{
			nDamage *= CheckATKType(pAtkUser->GetAtkType(),pTargeter->GetAtkType());
		}
		

		if(bCritHit)//致命一击
		{
			nDamage = nDamage*PHYSICAL_CRITICALHIT;
			dwBattleAddInfoRef = BATTLEADDINFO::INFO_CRITICALHIT;
		}
		
		nDamage = pAtker->AdjustDamage( nDamage,1 );

		if(pAtkUser && pAtkUser->GetWeaponR())
		{
			if( pAtkUser->GetWeaponR()->GetTypeID() == 144000 )
			{
				int nRanSuperWeapen = RandGet(1000);
				if(nRanSuperWeapen < 800)
				{
					nDamage *= 3;
				}
			}
		}
		
		return __max(1, nDamage);
	}
	else//未击中，躲避了
	{
		dwBattleAddInfoRef = BATTLEADDINFO::INFO_DODGE;	
		return 0;
	}
	
}

//-----------------------------------------------------------------------------------------------------------
//07.1.31
//王玉波
//计算魔法技能伤害
//参数说明
//Arcane Skill Damage = 魔法攻击力*[魔法攻击力/（魔法攻击力*技能加成%+魔法防御力）]）*等级差修正
//07.4.2王玉波第一次修改:考虑buff与debuff
//07.4.16王玉波第二次修改：应策划要求，更改战斗公式
//根据加成的是百份比或者点数，如果是点数，加到攻击力上去
//07.7.9 王玉波第三次修改，增加DWORD& dwBattleAddInfoRef参数，取得战斗附加信息

//最终外攻伤害=(自己的外攻+技能外攻)^2/（自己的外攻+技能外攻+对方外防）
int CBattleSystem::CalcDamage_MagicSkill( IRole* pAtker,IRole* pTargeter,int nMagicSkillPower,DWORD& dwBattleAddInfoRef, int nPower2, int nAtackType)
{
	if (nAtackType==0)
		return 0;
	if (nAtackType==9999)//直接伤血的技能
		return nMagicSkillPower;
	BOOL bCriticalHit = false;
	if( IsCriticalHit( pAtker,true ,pTargeter ) )//法术爆发
	{
		bCriticalHit = true;
	}
	else
	{
		
	}
	int nAttackType = nAtackType;
	int nAttArray[2] = {0};
	nAttArray[0] = nAttackType/100;//属性1的攻击类型
	nAttArray[1] = nAttackType%100;//属性2的攻击类型
	int nAttPower[2] = {0};
	nAttPower[0] = nMagicSkillPower;//属性1的威力
	nAttPower[1] = nPower2;//属性2的威力

	DWORD dwAtkCom = pAtker->GetAtk();//外攻
	DWORD dwAtkIn = pAtker->GetMgcAtk();//内攻
	DWORD dwAtkIce = pAtker->GetAddAttByType(2);//冰
	DWORD dwAtkFire = pAtker->GetAddAttByType(1);//火
	DWORD dwAtkNoxious = pAtker->GetAddAttByType(4);//毒
	DWORD dwAtkMystery = pAtker->GetAddAttByType(3);//玄
	for (int i=0; i<2; i++)
	{
		if (nAttArray[i]==ATKTYPEMODE_NONE)//无效的
			continue;
		else if(nAttArray[i]==ATKTYPEMODE_COMMON)//外攻
			dwAtkCom += nAttPower[i];
		else if(nAttArray[i]==ATKTYPEMODE_INNNER)//内攻
			dwAtkIn += nAttPower[i];
		else if(nAttArray[i]==ATKTYPEMODE_ICE)//冰
			dwAtkIce += nAttPower[i];
		else if(nAttArray[i]==ATKTYPEMODE_FIRE)//火
			dwAtkFire += nAttPower[i];
		else if(nAttArray[i]==ATKTYPEMODE_NOXIOUS)//毒
			dwAtkNoxious += nAttPower[i];
		else if(nAttArray[i]==ATKTYPEMODE_MYSTERY)//玄
			dwAtkMystery += nAttPower[i];
	}

	//攻击随机
	int nRan = RandGet(10);
	int nDamage = 0;//最终伤害

	//外攻
	dwAtkCom = (DWORD)((double)dwAtkCom * ( 95.0f + nRan) / 100.0f);
	DWORD dwTagDefence = pTargeter->GetDef();
	nDamage += ((double)dwAtkCom /(double) (dwAtkCom + dwTagDefence)) * dwAtkCom;
	
	//内攻
	dwAtkIn = (DWORD)((double)dwAtkIn * ( 95.0f + nRan) / 100.0f);
	dwTagDefence = pTargeter->GetMagicDef();
	nDamage += ((double)dwAtkIn /(double) (dwAtkIn + dwTagDefence)) * dwAtkIn;

	int nThisDamage = 0;
	//冰
	dwTagDefence = pTargeter->GetAddAttByType(12);
	nThisDamage = (dwAtkIce-dwTagDefence) * 5;
	nDamage += nThisDamage>0 ? nThisDamage : 0 ;

	//火
	nThisDamage = 0;
	dwTagDefence = pTargeter->GetAddAttByType(11);
	nThisDamage = (dwAtkFire-dwTagDefence) * 5;
	nDamage += nThisDamage>0 ? nThisDamage : 0 ;

	//毒
	nThisDamage = 0;
	dwTagDefence  = pTargeter->GetAddAttByType(14);
	nThisDamage = (dwAtkNoxious-dwTagDefence) * 5;
	nDamage += nThisDamage>0 ? nThisDamage : 0 ;

	//玄
	nThisDamage = 0;
	dwTagDefence = pTargeter->GetAddAttByType(13);
	nThisDamage = (dwAtkMystery-dwTagDefence) * 5;
	nDamage += nThisDamage>0 ? nThisDamage : 0 ;

	if(bCriticalHit)//暴击时伤害加倍
	{
		nDamage = nDamage*MAGIC_CRITICALHIT;
		dwBattleAddInfoRef = BATTLEADDINFO::INFO_CRITICALHIT;
	}

	return __max(1, nDamage);
}
