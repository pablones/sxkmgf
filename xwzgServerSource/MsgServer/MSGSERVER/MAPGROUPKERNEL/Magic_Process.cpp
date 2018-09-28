// Magic.cpp: implementation of the CMagic class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning(disable:4800)
#include "network/AllMsg.h"
#include "GameObj.h"
#include "Magic.h"
#include "MapGroup.h"
#include "StatusOnce.h"
#include "StatusMore.h"
#include "MapTrap.h"
#include "User.h"
#include "CombGod.h"
#include "GiftManager.h"//20071130修罗:

/*/ 4超级的特殊状态，及超级回复
3500	3500	0002	回复术
3501	3501	0006	钢铁皮肤
3502	3502	0006	魔法护盾
3503	3503	0006	神圣祈祷
3504	3504	0006	魔力加持
*/

const int	SUPER_MAGIC_RECRUIT			= 3500;
const int	SUPER_MAGIC_DEFENCE			= 3501;
const int	SUPER_MAGIC_MGC_DEFENCE		= 3502;
const int	SUPER_MAGIC_ATTACK			= 3503;
const int	SUPER_MAGIC_MGC_ATTACK		= 3504;

const int	THROW_WEAPON_DROP_RANGE		= 1;							// 扔武器攻击可掉物品的范围

const int	SPIRITSTONE_TYPEID_BASE		= 900000;


//时空门信息---BEGIN
const int	SPACETIMEDOOR_GENE_MAPID	=	1004;
const int	GENE_XPOS	=	194;
const int	GENE_YPOS	=	390;
const int	SPACETIMEDOOR_HUMAN_MAPID	=	2020;
const int	HUMAN_XPOS	=	253;
const int	HUMAN_YPOS	=	228;
const int	SPACETIMEDOOR_DRAGON_MAPID	=	3004;	
const int	DRAGON_XPOS	=	261;
const int	DRAGON_YPOS	=	384;
//时空门信息----END

const int WAIT_REBORN_TIME	= 30;//等待复活时间，秒

const int ESPECIAL_POWER = -9999;// 陈剑飞添加，当玩家在安全区域释放魔法时，对方免疫，调用此来避免头上冒0和受击动作 [6/12/2007]
extern int g_SpecilLine;
//////////////////////////////////////////////////////////////////////
void	MovePos(POINT* pPos, int nDir)
{
	nDir %= 8;
	pPos->x	+= _DELTA_X[nDir];
	pPos->y	+= _DELTA_Y[nDir];//06.3.22王玉波修改.....注意，以后有可能改回来原来+_DELTA_X[]
}

//////////////////////////////////////////////////////////////////////
void	DDALine(int x0, int y0, int x1, int y1, int nRange, vector<POINT>& vctPoint)
{
	if (x0 == x1 && y0 == y1)
		return;

	float scale = 1.0f*nRange/sqrt((long double)(x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
	x1 = int(0.5f+scale*(x1-x0)+x0);
	y1 = int(0.5f+scale*(y1-y0)+y0);

	::DDALineEx(x0, y0, x1, y1, vctPoint);
}
//[2010-08-03 goto]目标数量超过技能攻击上限时，按距离排序，取距离小的目标
bool FartherThan(BeAttackSort target1, BeAttackSort target2)
{
	return target1.nDistance < target2.nDistance;
}
//////////////////////////////////////////////////////////////////////
// launch
//		MAGICSORT_FLASH_ATTACK		= 41,		// 移形换位
//		MAGICSORT_DANCE				= 22,		// 跳舞(only use for client)
//		MAGICSORT_VAMPIRE			= 24,		// 吸血，power is percent award. use for call pet
//		MAGICSORT_INSTEAD			= 25,		// 替身. use for call 
//	
//////////////////////////////////////////////////////////////////////
int CMagic::Launch()
{
	if(!m_pMagic)
		return 0;
	CHECKF(m_pOwner);

	if(!m_pOwner->IsAlive())
	{
		ShowMiss();
		return 0;
	}

	int ret = 0;
	int tt=0;
	switch(m_pMagic->GetInt(MAGICDATA_SORT))
	{
	case	MAGICSORT_ATTACK:
		//1攻击单人,对方去血
		ret = ProcessAttack();
		break;
	case	MAGICSORT_RECRUIT:				//2回复，加血
		ret = ProcessRecruit();
		break;
	case	MAGICSORT_CROSS:				//3十字型区域目标掉血
		//ret = ProcessCross();
		break;
	case	MAGICSORT_LINE:					 //14, support auto active(random).			
	case	MAGICSORT_LINE_PENETRABLE:		// 33 攻击者轨迹可以穿人的线性攻击
		//ret = ProcessLine();
		break;
	case	MAGICSORT_FAN:					//4扇形区域，目标去血				
		//ret = ProcessFan();
		break;
	case	MAGICSORT_BOMB:					//5圆形区域，目标去血				
		ret = ProcessBomb();
		break;
	case	MAGICSORT_BOMB_LOCKALL:			//30与MAGICSORT_BOMB处理相同，只是锁定全部目标
		//ret = ProcessBombLockAll();
		break;
	case	MAGICSORT_ATTACHSTATUS:	//6回复单人，对方加状态
	//case	MAGICSORT_TEAM_MAGIC:
		ret = ProcessAttach();	
		break;
	case	MAGICSORT_SWITCHSTATUS:	//63回复单人，状态开关,有状态则去,无状态则加
		//ret = ProcessSwitchStatus();
		break;
	case	MAGICSORT_DETACHSTATUS:	//7回复单人，对方去状态
		//ret = ProcessDetach();
		break;
	case	MAGICSORT_SQUARE:		//8.range of square, multi target, (support xp skill)(support ground type)(SizeAdd effective)
		//ret = ProcessSquare();
		break;
	case	MAGICSORT_FLYSTRIKE: //9 goto 改为瞬移后攻击
		ret = ProcessFlyStrike();
		break;
	case	MAGICSORT_RANDOMTRANS://10
		//ret = ProcessRandomTrans();
		break;
	case	MAGICSORT_DISPATCHXP://11
		//ret = ProcessDispatchXP();
		break;
	case	MAGICSORT_COLLIDE://12
		//ret = ProcessCollide();
		break;
	case	MAGICSORT_ATKSTATUS://	16 support auto active, random active.
		ret = ProcessAtkStatus();
		break;
	case	MAGICSORT_CALLTEAMMEMBER: //17
		//ret = ProcessCallTeamMember();
		break;
	case	MAGICSORT_RECORDTRANSSPELL://18
		//ret = ProcessRecordTransSpell();
		break;
// 	case	MAGICSORT_TRANSFORM://19
// 		ret = ProcessTransform();
// 		break;
	case	MAGICSORT_ADDMANA:	// = 20,support self target only.
		ret = ProcessAddMana();
		break;
	case	MAGICSORT_LAYTRAP: //21
		//ret = ProcessLayTrap();
		break;
	case	MAGICSORT_CALLPET: //23,			// 召唤兽
		//ret = ProcessCallPet();
		break;
	case	MAGICSORT_DECLIFE:// 26,扣血(当前血的比例)
		//ret = ProcessDecLife();
		break;
	case	MAGICSORT_GROUNDSTING: // 27地刺
		//ret = ProcessGroundSting();
		break;
	case	MAGICSORT_REBORN://28,			 复活 -- zlong 2004.5.14
		//ret = ProcessReborn();
		break;
	case	MAGICSORT_SORB_SOUL:	//31	吸魂魔法
		//ret = ProcessSorbSoul();
		break;
	case	MAGICSORT_STEAL:	// 32,偷盗，随机从目标身上偷取power个物品
		//ret = ProcessSteal();
		break;
	case	MAGICSORT_PASS_THROUGH:	// 43 // 冲刺
		//ret = ProcessPassThrough();
		break;
/////////////////////////////////////////////////
	case	MAGICSORT_BLAST_THUNDER://34,			// 魔雷
		//ret = ProcessBlastThunder();
		break;
	case	MAGICSORT_MULTI_ATTACHSTATUS://35,// 群体施加状态
		ret = ProcessMultiAttach();
		break;
	case	MAGICSORT_MULTI_DETACHSTATUS://36 群体解除状态
		//ret = ProcessMultiDetach();
		break;
	case	MAGICSORT_MULTI_CURE:		//37 群体补血
		//ret = ProcessMultiCure();
		break;
	case	MAGICSORT_STEAL_MONEY:	//38偷钱
		//ret = ProcessStealMoney();
		break;
	case	MAGICSORT_KO: //39,			// 必杀技，目标血小于15%自动触发
		//ret = ProcessKO();
		break;
	case	MAGICSORT_ESCAPE:	// 40	// 逃跑/救助
		//ret = ProcessEscape();
		break;
	case MAGICSORT_PRECONDITION://44,处理前置
		//ret = ProcessPreCondition();
		break;
	case MAGICSORT_SPACETIMEDOOR://55,时空门
		//ret = Process_SpaceTimeDoor();
		break;
	case MAGICSORT_STOPTRAP:		//58，静止陷阱
		//ret = Process_StopTrap();
		break;
	case MAGICSORT_DRAGONHEART:		//59,龙之心
		//ret = ProcessDragonHeart();
		break;
	case MAGICSORT_BELLOW:			//60，嘲讽
		ret = Process_Bellow();
		break;
// 	case MAGICSORT_CONGEAL_SPRITGRID://61,原魂凝结
// 		ret = Process_CongealSpiritStone();
// 		break;
	case MAGICSORT_DIMENSION_DOOR://62,瞬间移动
		ret = ProcessDimensionDoor();
		break;
	case MAGICSORT_SUMMON:
		//ret = ProcessSummon();//20080202修罗:召唤
		break;
	case MAGICSORT_CLEARSKILLCD://64,清除cd [12/21/2007 陈剑飞]
		ret = ProcessClearCD();
		break;
	case MAGICSORT_TARGET_BACK://66，单攻，目标后退
		ret = ProcessAtkBack();
		break;
	case MAGICSORT_RANDOM_MULTI://67,//随机攻击多个目标
		ret = ProcessRandomMultiAtk();
		break;
	case MAGICSORT_TIMESATK://68,//连击
		ret = ProcessTimesAtk();
		break;
	case MAGICSORT_WITHDRAWATK://69,//后退并攻击
		ret = ProcessWithDrawAtk();
		break;
	case MAGICSORT_BACK_CITY://90,回城
		ret = ProcessBackCity();
		break;
	case MAGICSORT_BACK_SYN://91回帮
		ret = ProcessBackSyn();
		break;
///////////////////////////////////////////////////////////////////////////////
	default:
		ASSERT(!"switch magic sort");
	}

	if(m_pMagic  &&  m_pOwner)
	{
		if(m_pMagic->GetInt(MAGICDATA_DELAY)/1000 > 60)//少于60秒的不记录
		{
	//		char thisSql[128];
			char thedata[24];
			
			SYSTEMTIME thisTime;
			::GetLocalTime(&thisTime);
			int nTime = AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_DELAY),m_pMagic->GetInt(MAGICDATA_TYPE),m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_PRIVATECD);
			sprintf(thedata,"%d-%d-%d-%d-%d-%d",nTime/1000,thisTime.wMonth,thisTime.wDay,thisTime.wHour,thisTime.wMinute,thisTime.wSecond);
			//宠物的CD
			if (m_pOwner->GetID() >= EUDEMON_ID_FIRST && m_pOwner->GetID() <= EUDEMON_ID_LAST)//如果是宠物，计算性格对CD的影响
			{
				CMonster* pMonster = NULL;
				if( m_pOwner->QueryObj( OBJ_MONSTER,( void** )&pMonster) )
				{
					if (m_pMagic->GetIndex()==0)
						pMonster->SetMagicLastuse(EUDEMONDATA_skill1_lastuse, thedata, 20);
	// 				else if (m_pMagic->GetIndex()==1)//只有一个主动技能
	// 					pMonster->SetMagicLastuse(EUDEMONDATA_skill2_lastuse, thedata, 20);
				}
			}
			else
			{
				m_pMagic->SetStr(MAGICDATA_LASTUSE,thedata,24);
			}
		}
	}	
	return ret;
}

//////////////////////////////////////////////////////////////////////
// magic process
//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessAttack()
{
	int nStep=0;
	DEBUG_TRY
	CHECKF(m_pMagic);
	CHECKF(m_pOwner);

	IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
	if(!pTarget || !pTarget->IsAlive() || m_idTarget==m_pOwner->GetID())
		return false;
	nStep=1;
// 	CUser* pAtkUser = NULL;
// 	if( m_pOwner->QueryObj( OBJ_USER,( void** )&pAtkUser) )//某USER施放技能
// 	{
// 		if( !pAtkUser->IsArrowPass( pTarget->GetPosX(),pTarget->GetPosY() ) )
// 		{
// 
// 			pAtkUser->SendSysMsg( g_objStrRes.GetStr( 14097 ) );//"不能施放魔法,阻挡"
// 			return false;
// 		}
// 	}

// 	if (m_pMagic->GetSTrackAmount() > 0)
// 		LockTarget(true);

	nStep=2;
	for (int k=0; k<m_nApplyTimes; k++)
	{
		nStep=3;
		DWORD dwBattleAddInfo = BATTLEADDINFO::INFO_INVALID;//无效的

		if ( this->IsImmunity(pTarget) )
		{
// 			CMonster* pMonster = NULL;
// 			if( pTarget->QueryObj( OBJ_MONSTER,( void**)&pMonster ) )//不能对本族守卫攻击
// 			{
// 				if( pMonster->IsGuard()  || pMonster->IsRealGuard() )
// 				{
// 					return false;
// 				}
// 			}
// 			CMsgMagicEffect	msg;
// 			IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_idTarget, ESPECIAL_POWER, m_pOwner->GetDir(),BATTLEADDINFO::INFO_ATK_INVALID))
// 				m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
				
			return false;//  [6/12/2007]陈剑飞
		}
		nStep=4;
		int nPower1 = AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER),m_pMagic->GetInt(MAGICDATA_TYPE),m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW);
		int nPower2 = 0;
		if (RandGet(100)<m_pMagic->GetInt(MAGICDATA_STATUS_CHANCE))//附加属性的机率
			nPower2=AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER2),m_pMagic->GetInt(MAGICDATA_TYPE),m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW2);

		int nPower = CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pTarget,nPower1,0,nPower2,m_pMagic->GetInt(MAGICDATA_ATTACK_TYPE));
		nStep=5;
		if (m_pMagic->GetInt(MAGICDATA_TYPE)==MAGIC_JGBH)//金刚不坏
		{
			float nTargetLifeRest = pTarget->GetLife()*1.0f/pTarget->GetMaxLife();
			if (nTargetLifeRest>=0.8f)
				nPower = pTarget->GetLife()*0.1;
			else if (nTargetLifeRest>=0.6f && nTargetLifeRest <0.8f)
				nPower = pTarget->GetLife()*0.15;
			else if (nTargetLifeRest<0.6f)
				nPower = pTarget->GetLife()*0.2;
		}
		nStep=6;
		// 广播法术效果
		bool bCreate = false;
		CMsgMagicEffect	msg;
		IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_idTarget, nPower, m_pOwner->GetDir(),dwBattleAddInfo))
		{
			if (MAGIC_XX==m_pMagic->GetInt(MAGICDATA_TYPE) || MAGIC_GJXX==m_pMagic->GetInt(MAGICDATA_TYPE))
			{
				if (RandGet(100)<m_pMagic->GetInt(MAGICDATA_STATUS_CHANCE))
				{
					m_pOwner->AddAttrib(_USERATTRIB_LIFE, nPower2, SYNCHRO_TRUE);
					msg.AppendRole(m_pOwner->GetID(), nPower2, BATTLEADDINFO::INFO_HALTDAMAGE);//吸血
				}
			}
			if (MAGIC_XQ==m_pMagic->GetInt(MAGICDATA_TYPE) || MAGIC_GJXQ==m_pMagic->GetInt(MAGICDATA_TYPE))//抽蓝
			{
				if (RandGet(100)<m_pMagic->GetInt(MAGICDATA_STATUS_CHANCE))
				{
					CUser* pUser = NULL;
					pTarget->QueryObj(OBJ_USER, IPP_OF(pUser));
					if (pUser)
					{
						nPower2=AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER2),m_pMagic->GetInt(MAGICDATA_TYPE),m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW2);
						pUser->AddAttrib(_USERATTRIB_MANA, -1*nPower2, SYNCHRO_TRUE);
					}
				}
			}
			if (MAGIC_DN==m_pMagic->GetInt(MAGICDATA_TYPE) || MAGIC_GJDN==m_pMagic->GetInt(MAGICDATA_TYPE))//抽怒气
			{
				if (RandGet(100)<m_pMagic->GetInt(MAGICDATA_STATUS_CHANCE))
				{
					CUser* pUser = NULL;
					pTarget->QueryObj(OBJ_USER, IPP_OF(pUser));
					if (pUser)
					{
						nPower2=AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER2),m_pMagic->GetInt(MAGICDATA_TYPE),m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW2);
						pUser->SetAttrib(_USERATTRIB_SP, __max(pUser->GetSp()-nPower2,0), SYNCHRO_TRUE);
					}
				}
			}
			if (MAGIC_LJ==m_pMagic->GetInt(MAGICDATA_TYPE) || MAGIC_GJLJ==m_pMagic->GetInt(MAGICDATA_TYPE))//连击
			{
				if (RandGet(100)<m_pMagic->GetInt(MAGICDATA_STATUS_CHANCE))
				{
					int nPowerTemp = CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pTarget,nPower1,0,nPower2,m_pMagic->GetInt(MAGICDATA_ATTACK_TYPE));
					msg.AppendRole(m_idTarget, nPowerTemp, dwBattleAddInfo);
					nPower+=nPowerTemp;
				}
			}
			//m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF); goto 客户端强烈要求先同步血再发技能效果,无奈...
			bCreate = true;
		}
		nStep=7;
		CheckCrime(pTarget);
		nStep=8;
		if (nPower)
		{
			int nLifeLost = nPower;
			nLifeLost = __min(pTarget->GetLife(), nPower);
			CUser* pUser = NULL;
			pTarget->QueryObj(OBJ_USER, IPP_OF(pUser));
			if(pUser)
			{
				nLifeLost = pUser->PVPEmendDamage(nLifeLost, m_pOwner->GetID());
			}
//modify code by 林德渊 begin 2011.6.22
			IStatus* pStatus = pTarget->QueryStatus(STATUS_RETORT_LGWS);
			if(pStatus)
			{
				m_pOwner->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
				m_pOwner->BeAttack(HitByMagic(),pTarget,nPower,0,true);
			}
//modify code by 林德渊 end
			pTarget->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
			int addi = 0;//FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
			pTarget->BeAttack(HitByMagic(), m_pOwner, nPower,addi);					
		}
		//goto ???单攻技能会抽蓝???
// 		if( nLostMana != 0 )
// 		{
// 			nLostMana = __min( pTarget->GetMana(),nLostMana );
// 			pTarget->AddAttrib( _USERATTRIB_MANA, -1*nLostMana,SYNCHRO_TRUE );
// 		}

// 		if (m_pMagic->GetSTrackAmount() > 0)
// 			AdjustPosByTrack(k);
		if (bCreate)
			m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
	}
	nStep=9;
	m_pos.x	= pTarget->GetPosX();
	m_pos.y = pTarget->GetPosY();
	nStep=10;
	if (!pTarget->IsAlive())
		m_pOwner->Kill(pTarget, GetDieMode());
//	AwardExp( 0 );
	DEBUG_CATCH2("CMagic::ProcessAttack() step=%d",nStep)
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//单体伤害，一定机率给对方加状态
bool CMagic::ProcessAtkStatus()
{
	CHECKF(m_pMagic);
	
	bool bNeedOverPVP = false;
	m_setTargetLocked.clear();
	
	IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
	if(!pTarget || !pTarget->IsAlive())
		return false;
		  
	if (this->IsImmunity(pTarget))
		return false;
	DWORD dwBattleAddInfo = 0;
	int nPower1 = AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER), 0, m_pMagic->GetMagicLevel(), MAGICLEVADDTYPE_POW);
	int nPower2 = AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER2), 0, m_pMagic->GetMagicLevel(), MAGICLEVADDTYPE_POW2);
	int nPower = CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pTarget,nPower1,0,nPower2,m_pMagic->GetInt(MAGICDATA_ATTACK_TYPE) );//30100没有技能加成		
	
	if (nPower)
	{
		CNpc* pNpc = NULL;
		int nLifeLost = nPower;
		
		nLifeLost = __min(pTarget->GetLife(), nPower);
		if (pTarget->IsUser())
		{
			CUser* pTargetTemp=NULL;
			if(pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetTemp)))
			if (pTargetTemp)
			{
				if(MapGroup(PID)->GetPVPManger()->findPVP(pTargetTemp->GetID()))
				{
					if((pTargetTemp->GetLife() - nLifeLost) <= 0)
						bNeedOverPVP = true;
					nLifeLost = pTargetTemp->PVPEmendDamage(nLifeLost, m_pOwner->GetID(), false);
				}
			}
		}
//modify code by 林德渊 begin 2011.6.22
		IStatus* pStatus = pTarget->QueryStatus(STATUS_RETORT_LGWS);
		if(pStatus)
		{
			m_pOwner->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
			m_pOwner->BeAttack(HitByMagic(),pTarget,nPower,0,true);
		}
//modify code by 林德渊 end
		pTarget->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
		CUser* pTargetUser=NULL;
		if(pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser)))
			pTargetUser->BroadcastTeamLife();
		
		pTarget->BeAttack(HitByMagic(), m_pOwner, nPower);		  
//		AwardExp( 0 );	//加技能经验.....
	}
	// 广播法术效果
	CMsgMagicEffect	msg;
	IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_idTarget, nPower, m_pOwner->GetDir(),dwBattleAddInfo))
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
	// set crime status
	CheckCrime(pTarget);

	// 把原来的先加状态后算攻击修改为先算攻击后加状态
	// 原因是例如冰冻之类的魔法会在BeAttack的时候解除，因此必须在BeAttack之后调用加状态
	
	if( pTarget->IsAlive() )
	{
		int nStatus	= m_pMagic->GetInt( MAGICDATA_STATUS );
		if( RandGet( 100 ) < m_pMagic->GetInt( MAGICDATA_STATUS_CHANCE ) )//一定机率给对方加状态
		{
			int nStatusPower = AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_STATUS_POWER),m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_STATUS_POW);
			int nSecs	= AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_STEP),m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), MAGICLEVADDTYPE_STEPSEC);
			int nTimes	= AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_TIMES),m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_TIMES);
			CRole::AttachStatus(pTarget, nStatus, nStatusPower, nSecs, nTimes,0,0,0,m_pOwner->GetID());
		}
		if (bNeedOverPVP)
		{
			MapGroup(PID)->GetPVPManger()->EndPVP(pTarget->GetID(), m_pOwner->GetID(), true);		//避免出现红名所以在加好状态之后才能结束切磋
		}
	}
	// 杀死
	else if (!pTarget->IsAlive())
		m_pOwner->Kill(pTarget, GetDieMode());
	return true;
}

//////////////////////////////////////////////////////////////////////
//加血单人,多人
bool CMagic::ProcessRecruit()
{
	CHECKF(m_pMagic);
	if (m_pMagic->GetInt(MAGICDATA_CRIME)==2 && m_pOwner->GetID()!=m_idTarget)//只能对自己使用
	{
		m_idTarget = m_pOwner->GetID();
	}
//	m_setTargetLocked.clear();
	vector<IRole*>	setRole;
	vector<int>		setPower;
	CTeam* pTeam = m_pOwner->GetTeam();
	if(m_pMagic->GetInt(MAGICDATA_MULTI))
	{
		if (pTeam)//有队伍时加队友、宠物及自己
		{
			for(int i = 0; i < pTeam->GetMemberAmount(); i++)
			{
				OBJID idMember = pTeam->GetMemberByIndex(i);
				IRole* pMember = m_pOwner->FindAroundRole(idMember);
				CUser* pMemberUser = NULL;
				if (pMember && pMember->QueryObj(OBJ_USER, IPP_OF(pMemberUser)) && CanRecurit(pMember) && pMember->IsAlive())
				{
					if (m_pOwner->GetDistance(pMemberUser->GetPosX(), pMemberUser->GetPosY())<=m_pMagic->GetInt(MAGICDATA_RANGE))
						setRole.push_back(pMemberUser);
					CMonster* pMonster = pMemberUser->QueryCallPet();
					if (pMonster && m_pOwner->GetDistance(pMonster->GetPosX(), pMonster->GetPosY())<=m_pMagic->GetInt(MAGICDATA_RANGE))
						setRole.push_back(pMonster);
				}
			}
		}
		else//没队伍时只加自己和宠物
		{
			setRole.push_back(m_pOwner);
			CMonster* pMonster = NULL;
			m_pOwner->QueryObj(OBJ_MONSTER, IPP_OF(pMonster));
			if (pMonster)
			{
				CUser* pUser = pMonster->QueryOwnerUser();
				if (pUser)
					setRole.push_back(pUser);
			}
			CUser* pUser = NULL;
			m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser));
			if (pUser)
			{
				CMonster* pMonster = pUser->QueryCallPet();
				if (pMonster)
					setRole.push_back(pMonster);
			}
			//m_idTarget = m_pOwner->GetID();
		}
	}
	else
	{
		IRole* pRole = m_pOwner->FindAroundRole(m_idTarget);
		if(!pRole || !pRole->IsAlive())
			return false;

		//goto--- 无此功能
// 		CUser* pAttackerUse = NULL;
// 		CUser* pTargetUser = NULL;
// 		if(m_pOwner->QueryObj(OBJ_USER, IPP_OF(pAttackerUse)) && pRole->QueryObj(OBJ_USER, IPP_OF(pTargetUser)))
// 		{
// 			if(CITYSYNWAR_CITYWAR == pAttackerUse->GetCitySynWarType())
// 			{	
// 				if((pAttackerUse->IsCityWarAttack(pAttackerUse->GetCityWarDoMain()) && pTargetUser->IsCityWarDefence(pAttackerUse->GetCityWarDoMain()))
// 					|| (pTargetUser->IsCityWarAttack(pAttackerUse->GetCityWarDoMain()) && pAttackerUse->IsCityWarDefence(pAttackerUse->GetCityWarDoMain())))
// 				{
// 					pAttackerUse->SendSysMsg(g_objStrRes.GetStr(28046));
// 					return false;
// 				}
// 			}
// 			else if(CITYSYNWAR_SYNWAR == pAttackerUse->GetCitySynWarType())
// 			{
// 				if(    SYNWARTYPE_NONE != pAttackerUse->GetSynWarJoinType()
// 					&& SYNWARTYPE_NONE != pTargetUser->GetSynWarJoinType()
// 					&& pAttackerUse->GetCitySynWarType() != pTargetUser->GetSynWarJoinType())
// 				{
// 					pAttackerUse->SendSysMsg(g_objStrRes.GetStr(28046));
// 					return false;
// 				}
// 			}
// 		}
		//goto---

		//  [2007-9-29] 朱斌 BUG0001614	可加血换成 CanRecurit
		if( pRole != m_pOwner && ( !CanRecurit( pRole ) ) )//可以攻击的人不能加
		{
			pRole = m_pOwner;
			m_idTarget = m_pOwner->GetID();
		//	return false;
		}
		setRole.push_back(pRole);

//		m_setTargetLocked.push_back(pRole->GetID());
	}

// 	if (m_pMagic->GetSTrackAmount() > 0)
// 		LockTarget(true);

	for (int k=0; k<m_nApplyTimes; k++)
	{
		// 广播法术效果
		bool bCreate = false;
		CMsgMagicEffect	msg;
		IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_idTarget, m_pOwner->GetDir()))
		{
			for(int i = 0; i < setRole.size(); i++)
			{
				IRole* pRole = setRole[i];
				if (pRole && pRole->IsAlive())
				{
					int nPower = m_pMagic->GetInt(MAGICDATA_POWER);
					if (nPower > 300000)
					{
						int nPercent = AdjustMagicPower((nPower-300000)/100, m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), MAGICLEVADDTYPE_POW);
						nPower = pRole->GetMaxLife()*(nPercent/100.0);
					}
					else
						nPower=AdjustMagicPower(nPower, m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), MAGICLEVADDTYPE_POW);///对回血POWER进行修正

					setPower.push_back(nPower);
					msg.AppendRole( setRole[i]->GetID(),nPower,BATTLEADDINFO::INFO_INVALID );
				}
				else
					setPower.push_back(0);
			}
			//m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
			bCreate = true;
		}

		for(int i = 0; i < setRole.size(); i++)
		{
			if (!setRole[i]->IsAlive())
				continue;

			int nAddLife = ::CutOverflow(setPower[i], ::CutTrail(0, (int)(setRole[i]->GetMaxLife()-setRole[i]->GetLife())));
			
// 			CUser* pTmpUser = NULL;
// 			if( setRole[i]->QueryObj( OBJ_USER,( void** )&pTmpUser) )
// 			{
// 				if(pTmpUser->QueryMagic(MAGIC_GMXN))
// 				{
// 					IMagicData* pData=pTmpUser->QueryMagic(MAGIC_GMXN);
// 					if(pData)
// 					{
// 						nAddLife = nAddLife * (100 + pData->GetInt(MAGICDATA_POWER))/100;
// 						nAddLife = ::CutOverflow(nAddLife, ::CutTrail(0, (int)(pTmpUser->GetMaxLife()-pTmpUser->GetLife())));
// 					}
// 				}
// 			}//12.26 光明信念

			if(nAddLife > 0)
			{
				setRole[i]->AddAttrib(_USERATTRIB_LIFE, nAddLife, SYNCHRO_TRUE);
				CUser* pUser=NULL;
				if(setRole[i]->QueryObj(OBJ_USER, IPP_OF(pUser)))
					pUser->BroadcastTeamLife();
			}

		}
		if (bCreate)
			m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
// 		if (m_pMagic->GetSTrackAmount() > 0)
// 			AdjustPosByTrack(k);
	}

//	AwardExp( 0 );	//加技能经验.....

	return true;
}
//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessAddMana()
{
	CHECKF(m_pMagic);
	if (m_pMagic->GetInt(MAGICDATA_CRIME)==2 && m_pOwner->GetID()!=m_idTarget)//只能对自己使用
	{
		m_idTarget = m_pOwner->GetID();
	}
//	m_setTargetLocked.clear();
	vector<IRole*>	setRole;
	vector<int>		setPower;
	CTeam* pTeam = m_pOwner->GetTeam();
	if(m_pMagic->GetInt(MAGICDATA_MULTI))
	{
		if (pTeam)//有队伍时加队友、宠物及自己
		{
			for(int i = 0; i < pTeam->GetMemberAmount(); i++)
			{
				OBJID idMember = pTeam->GetMemberByIndex(i);
				IRole* pMember = m_pOwner->FindAroundRole(idMember);
				CUser* pMemberUser = NULL;
				if (pMember && pMember->QueryObj(OBJ_USER, IPP_OF(pMemberUser)) && CanRecurit(pMember) && pMember->IsAlive())
				{
					if (m_pOwner->GetDistance(pMemberUser->GetPosX(), pMemberUser->GetPosY())<=m_pMagic->GetInt(MAGICDATA_RANGE))
						setRole.push_back(pMemberUser);
					//宠物没有蓝
// 					CMonster* pMonster = pMemberUser->QueryCallPet();
// 					if (pMonster && m_pOwner->GetDistance(pMonster->GetPosX(), pMonster->GetPosY())<=m_pMagic->GetInt(MAGICDATA_RANGE))
// 						setRole.push_back(pMonster);
				}
			}
		}
		else//没队伍时只加自己和宠物
		{
			setRole.push_back(m_pOwner);
			CMonster* pMonster = NULL;
			m_pOwner->QueryObj(OBJ_MONSTER, IPP_OF(pMonster));
			if (pMonster)
			{
				CUser* pUser = pMonster->QueryOwnerUser();
				if (pUser)
					setRole.push_back(pUser);
			}
			//宠物没有蓝
// 			CUser* pUser = NULL;
// 			m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser));
// 			if (pUser)
// 			{
// 				CMonster* pMonster = pUser->QueryCallPet();
// 				if (pMonster)
// 					setRole.push_back(pMonster);
// 			}
			//m_idTarget = m_pOwner->GetID();
		}
	}
	else
	{
		IRole* pRole = m_pOwner->FindAroundRole(m_idTarget);
		if(!pRole || !pRole->IsAlive())
			return false;

		//goto--- 目前无此功能
// 		CUser* pAttackerUse = NULL;
// 		CUser* pTargetUser = NULL;
// 		if(m_pOwner->QueryObj(OBJ_USER, IPP_OF(pAttackerUse)) && pRole->QueryObj(OBJ_USER, IPP_OF(pTargetUser)))
// 		{
// 			if(CITYSYNWAR_CITYWAR == pAttackerUse->GetCitySynWarType())
// 			{	
// 				if((pAttackerUse->IsCityWarAttack(pAttackerUse->GetCityWarDoMain()) && pTargetUser->IsCityWarDefence(pAttackerUse->GetCityWarDoMain()))
// 					|| (pTargetUser->IsCityWarAttack(pAttackerUse->GetCityWarDoMain()) && pAttackerUse->IsCityWarDefence(pAttackerUse->GetCityWarDoMain())))
// 				{
// 					pAttackerUse->SendSysMsg(g_objStrRes.GetStr(28046));
// 					return false;
// 				}
// 			}
// 			else if(CITYSYNWAR_SYNWAR == pAttackerUse->GetCitySynWarType())
// 			{
// 				if(    SYNWARTYPE_NONE != pAttackerUse->GetSynWarJoinType()
// 					&& SYNWARTYPE_NONE != pTargetUser->GetSynWarJoinType()
// 					&& pAttackerUse->GetCitySynWarType() != pTargetUser->GetSynWarJoinType())
// 				{
// 					pAttackerUse->SendSysMsg(g_objStrRes.GetStr(28046));
// 					return false;
// 				}
// 			}
// 		}
		//goto---

		//  [2007-9-29] 朱斌 BUG0001614	可加血换成 CanRecurit
		if( pRole != m_pOwner && ( !CanRecurit( pRole ) ) )//可以攻击的人不能加
		{
			pRole = m_pOwner;
			m_idTarget = m_pOwner->GetID();
		//	return false;
		}
		setRole.push_back(pRole);

//		m_setTargetLocked.push_back(pRole->GetID());
	}

// 	if (m_pMagic->GetSTrackAmount() > 0)
// 		LockTarget(true);

	for (int k=0; k<m_nApplyTimes; k++)
	{
		// 广播法术效果
		bool bCreate = false;
		CMsgMagicEffect	msg;
		IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_idTarget, m_pOwner->GetDir()))
		{
			for(int i = 0; i < setRole.size(); i++)
			{
				IRole* pRole = setRole[i];
				if (pRole && pRole->IsAlive())
				{
					int nPower = m_pMagic->GetInt(MAGICDATA_POWER);
					if (nPower > 30000)
					{
						int nPercent = AdjustMagicPower(nPower-30000, m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), MAGICLEVADDTYPE_POW);
						nPower = pRole->GetMaxMana()*nPercent/100;
					}
					else
						nPower=AdjustMagicPower(nPower, m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), MAGICLEVADDTYPE_POW);///对回血POWER进行修正

					setPower.push_back(nPower);
					msg.AppendRole( setRole[i]->GetID(),nPower,BATTLEADDINFO::INFO_INVALID );
				}
				else
					setPower.push_back(0);
			}
			//m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
			bCreate = true;
		}

		for(int i = 0; i < setRole.size(); i++)
		{
			if (!setRole[i]->IsAlive())
				continue;

			int nAddMana = ::CutOverflow(setPower[i], ::CutTrail(0, (int)(setRole[i]->GetMaxMana()-setRole[i]->GetMana())));

			if(nAddMana > 0)
			{
				setRole[i]->AddAttrib(_USERATTRIB_MANA, nAddMana, SYNCHRO_TRUE);
				CUser* pUser=NULL;
				if(setRole[i]->QueryObj(OBJ_USER, IPP_OF(pUser)))
					pUser->BroadcastTeamMana();
			}

		}
		if (bCreate)
			m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
// 		if (m_pMagic->GetSTrackAmount() > 0)
// 			AdjustPosByTrack(k);
	}

//	AwardExp( 0 );	//加技能经验.....

	return true;
}
//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessCross()
{
	CHECKF(m_pMagic);


	int	nRange1	= m_pMagic->GetInt(MAGICDATA_RANGE) % 100;
	int	nRange2	= m_pMagic->GetInt(MAGICDATA_RANGE) / 100;

	int	nSize		= nRange1*2 + 1;
	int	nBufSize	= nSize * nSize;
	MAPTHING_SET	setTarget;
	vector<IRole*>	setRole;
	vector<int>		setPower;
	POINT	pos;



	int nPowerSum = 0;

	for (int k=0; k<m_nApplyTimes; k++)
	{
		// 搜索目标集
		{
			m_setTargetLocked.clear();
			setTarget.clear();
			setRole.clear();
			setPower.clear();

			int nDir	= ::GetDirByPos(m_pOwner->GetPosX(), m_pOwner->GetPosY(), m_pos.x, m_pos.y);
			pos.x		= m_pOwner->GetPosX();
			pos.y		= m_pOwner->GetPosY();

			m_pOwner->GetMap()->CollectMapThing(setTarget, pos, nRange1, OBJ_USER|OBJ_MONSTER|OBJ_NPC);

			int nThisDir = nDir;
			for(int i = 0; i < 4; i++)
			{
				POINT	posThis = pos;
				for(int j = 0; j < (i%2 ? nRange1 : nRange2); j++)
				{
					MovePos(&posThis, nThisDir);
					if(!m_pOwner->GetMap()->IsValidPoint(posThis))
						continue;

					int idx = POS2INDEX(posThis.x - pos.x + nRange1, posThis.y - pos.y + nRange1, nSize, nSize);
					ASSERT(idx>=0 && idx<setTarget.size());
					//20080112修罗:从2维变为3维------
					//if(setTarget[idx])
					for(int zIdx=0;zIdx<setTarget[idx].size();zIdx++)
					{
						IRole* pRole = NULL;
						//if(setTarget[idx]->QueryRole(IPP_OF(pRole)) && pRole->IsAlive() && !IsImmunity(pRole))
						if(setTarget[idx][zIdx]->QueryRole(IPP_OF(pRole)) && pRole->IsAlive() && !IsImmunity(pRole))
						{	
							setRole.push_back(pRole);
							// 只锁定一个攻击目标
							if (pRole->GetID() == m_idTarget)
								m_setTargetLocked.push_back(pRole->GetID());
							
						}
					}
					//-------------------------------
				}
				nThisDir = (nThisDir+2) % 8;
			}
		}

		
		// 广播法术效果
		CMsgMagicEffect	msg;
		IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), pos.x, pos.y, m_pOwner->GetDir()))
		{
			for(int i = 0; i < setRole.size(); i++)
			{
				IRole* pRole = setRole[i];
				if (pRole && pRole->IsAlive())
				{
// 					DWORD dwBattleAddInfo = 0;
// 					int nPower = CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pRole,GetPower(k),m_pMagic->GetInt( MAGICDATA_PERCENT ) );
// 					setPower.push_back(nPower);
// 					msg.AppendRole(pRole->GetID(), nPower,dwBattleAddInfo );
				}
				else
					setPower.push_back(0);
			}
			m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
		}

		// set crime status
		CheckCrime(&setRole);

		int addi = 0;//FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
		
		for (int i = 0; i < setRole.size(); i++)
		{
			if (!setRole[i]->IsAlive())
				continue;
			
			nPowerSum += setPower[i];
			CNpc* pNpc = NULL;
			int nLifeLost = setPower[i];
			
			nLifeLost = __min(setRole[i]->GetLife(), setPower[i]);
			CUser* pUser = NULL;
			setRole[i]->QueryObj(OBJ_USER, IPP_OF(pUser));
			if(pUser)
			{
				nLifeLost = pUser->PVPEmendDamage(nLifeLost, m_pOwner->GetID());
			}
			setRole[i]->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
			if(setRole[i]->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
				m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
			
			setRole[i]->BeAttack(HitByMagic(), m_pOwner, setPower[i],addi);
			
			
		}

// 		if (m_pMagic->GetSTrackAmount() > 0)
// 			AdjustPosByTrack(k);

		// 必须先发轨迹后发kill消息
		for (i = 0; i < setRole.size(); i++)
		{
			if(!setRole[i]->IsAlive())
				m_pOwner->Kill(setRole[i], GetDieMode());
		}
	}
//	AwardExp( 0 );	//加技能经验.....

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessLine()
{
	CHECKF(m_pMagic);

	vector<IRole*>	setTarget;
	vector<int>		setPower;
	vector<POINT>	setPoint;

	int nPowerSum = 0;
	bool bAtkerPenetrable	= (m_pMagic->GetInt(MAGICDATA_SORT) == MAGICSORT_LINE_PENETRABLE);
	for (int k=0; k<m_nApplyTimes; k++)
	{
		// 搜索目标集
		{
			m_setTargetLocked.clear();
			setTarget.clear();
			setPower.clear();
			setPoint.clear();

			POINT pos;
			pos.x	= m_pOwner->GetPosX();
			pos.y	= m_pOwner->GetPosY();

			int nRange = m_pMagic->GetInt(MAGICDATA_RANGE);
		
			DDALine(pos.x, pos.y, m_pos.x, m_pos.y, nRange, setPoint);
		//	DDALine(pos.x, pos.y, m_pos.x, m_pos.y, m_pMagic->GetInt(MAGICDATA_RANGE), setPoint);

			for(int i = 0; i < setPoint.size(); i++)
			{
				POINT posThis = setPoint[i];
				if(!m_pOwner->GetMap()->IsValidPoint(posThis))
					continue;
				IRole* pTarget = m_pOwner->GetMap()->QueryRole(setPoint[i].x, setPoint[i].y, ID_NONE);
				if(!pTarget || !pTarget->IsAlive() || this->IsImmunity(pTarget))
					continue;

				// attack now

				setTarget.push_back(pTarget);
				if (m_pOwner->GetID() != pTarget->GetID())
					m_setTargetLocked.push_back(pTarget->GetID());
			}
		}

		// 广播法术效果
		CMsgMagicEffect	msg;
		IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_pos.x, m_pos.y, m_pOwner->GetDir()))
		{
			for(int i = 0; i < setTarget.size(); i++)
			{
				IRole* pRole = setTarget[i];
				if (pRole && pRole->IsAlive())
				{
// 					DWORD dwBattleAddInfo = 0;//无效的
// 					int nPower = CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pRole,GetPower(k),m_pMagic->GetInt( MAGICDATA_PERCENT ) );
// 					setPower.push_back(nPower);
// 					msg.AppendRole( pRole->GetID(),nPower,dwBattleAddInfo );

				}
				else
					setPower.push_back(0);
			}
			m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
		}

		// set crime status
		CheckCrime(&setTarget);

		int addi = 0;//FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
		
		for (int i = 0; i < setTarget.size(); i++)
		{
			if (!setTarget[i]->IsAlive())
				continue;
			
			nPowerSum += setPower[i];
			CNpc* pNpc = NULL;
			int nLifeLost = setPower[i];
			
			nLifeLost = __min(setTarget[i]->GetLife(), setPower[i]);
			setTarget[i]->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
			
			if(setTarget[i]->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
				m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
			
			setTarget[i]->BeAttack(HitByMagic(), m_pOwner, setPower[i],addi);
			
			
		}

// 		if (m_pMagic->GetSTrackAmount() > 0)
// 			AdjustPosByTrack(k, bAtkerPenetrable);

		// 必须先发轨迹后发kill消息
		for (i = 0; i < setTarget.size(); i++)
		{
			if(!setTarget[i]->IsAlive())
				m_pOwner->Kill(setTarget[i], GetDieMode());
		}
	}


//	AwardExp( 0 );
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessFan()
{
	CHECKF(m_pMagic);

	MAPTHING_SET	setTarget;
	vector<IRole*>	setRole;
	vector<int>		setPower;
	POINT	pos;	// source of fan

	int	nRange	= m_pMagic->GetInt(MAGICDATA_RANGE)+MAX_SIZEADD;

	int nWidth	= DEFAULT_MAGIC_FAN;//m_pMagic->GetInt(MAGICDATA_WIDTH);
// 	if(!nWidth)
// 		nWidth = DEFAULT_MAGIC_FAN;

	int	nSize		= nRange*2 + 1;
	int	nBufSize	= nSize * nSize;

	int nPowerSum = 0;
	for (int k=0; k<m_nApplyTimes; k++)
	{
		// 搜索目标集
		{
			m_setTargetLocked.clear();
			setTarget.clear();
			setRole.clear();
			setPower.clear();

			if(m_pMagic->GetInt(MAGICDATA_GROUND))
			{
				pos.x		= m_pOwner->GetPosX();
				pos.y		= m_pOwner->GetPosY();
			}
			else
			{
				IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
				if(!pTarget || !pTarget->IsAlive())
					return false;

				pos.x		= pTarget->GetPosX();
				pos.y		= pTarget->GetPosY();

				// 只锁定一个攻击目标
				setRole.push_back(pTarget);
				m_setTargetLocked.push_back(pTarget->GetID());
			}

			m_pOwner->GetMap()->CollectMapThing(setTarget, pos, nRange, OBJ_USER|OBJ_MONSTER|OBJ_NPC);

			for(int i = ::CutTrail((int)pos.x-nRange, 0); i <= pos.x+nRange && i < m_pOwner->GetMap()->GetWidth(); i++)
			{
				for(int j = ::CutTrail((int)pos.y-nRange, 0); j <= pos.y+nRange && j < m_pOwner->GetMap()->GetHeight(); j++)
				{
					POINT posThis;
					posThis.x	= i;
					posThis.y	= j;
					if(!m_pOwner->IsInFan(posThis, pos, nRange, nWidth, m_pos))	// m_pos: target pos
						continue;

					int idx = POS2INDEX(posThis.x - pos.x + nRange, posThis.y - pos.y + nRange, nSize, nSize);
					ASSERT(idx>=0 && idx<setTarget.size());
					//20080112修罗:从2维变为3维------
					//if(setTarget[idx])
					for(int zIdx=0;zIdx<setTarget[idx].size();zIdx++)
					{
						IRole* pRole = NULL;
//						if(setTarget[idx]->QueryRole(IPP_OF(pRole)) && pRole->IsAlive() && !IsImmunity(pRole)
//								)	//&& m_pOwner->GetDistance(pRole->QueryMapThing()) <= m_pOwner->GetAttackRange(pRole->GetSizeAdd()))
						if(setTarget[idx][zIdx]->QueryRole(IPP_OF(pRole)) && pRole->IsAlive() && !IsImmunity(pRole))
						{
							if (pRole->GetID() != m_idTarget )	// m_idTarget如果存在前面已经push到setRole中了
								setRole.push_back(pRole);
						}
					}
					//-------------------------------
				}
			}
		}

		// 广播法术效果
		CMsgMagicEffect	msg;
		IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(),
			pos.x, pos.y, m_pOwner->GetDir()))
		{
			for(int i = 0; i < setRole.size(); i++)
			{
				IRole* pRole = setRole[i];
				if (pRole && pRole->IsAlive())
				{
					DWORD dwBattleAddInfo = 0;
					int nPower = 0;//CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pRole,GetPower(k),m_pMagic->GetInt( MAGICDATA_PERCENT ) );
					setPower.push_back(nPower);
					msg.AppendRole( pRole->GetID(), nPower,dwBattleAddInfo );

					// 榨取 [12/24/2007 陈剑飞]
// 					CUser* pTarget = NULL;
// 					if( pRole->QueryObj( OBJ_USER,IPP_OF( pTarget )) )
// 					{
// 						if( pTarget->QueryMagic(MAGIC_ZQ) )
// 						{
// 							int nPer = 0;// 计算几率 [12/20/2007 陈剑飞]
// 							int nMagicType = m_pMagic->GetInt( MAGICDATA_TYPE );
// 							IMagicData* pData=pTarget->QueryMagic(MAGIC_ZQ);
// 							if (pData)
// 							{
// //								nPer = pData->GetInt(MAGICDATA_PERCENT);
// 							}
// 							if( pData && ( nMagicType == MAGIC_YJHL) )
// 							{
// 								if( RandGet( 100 ) <= nPer )
// 								{
// 									pTarget->AddAttrib(_USERATTRIB_LIFE, nPower, SYNCHRO_TRUE);
// 								}
// 								
// 							}
// 						}
// 					}
					//-----------------------------------------	
					
				}
				else
					setPower.push_back(0);
			}
			m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
		}

		// set crime status
		CheckCrime(&setRole);

		int addi = 0;//FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
		
		for (int i = 0; i < setRole.size(); i++)
		{
			if (!setRole[i]->IsAlive())
				continue;
			
			nPowerSum += setPower[i];
			CNpc* pNpc = NULL;
			int nLifeLost = setPower[i];
			
			nLifeLost = __min(setRole[i]->GetLife(), setPower[i]);

			//----------------------------------回避
// 			CUser* pTmpUser = NULL;
// 			if( setRole[i]->QueryObj( OBJ_USER,( void** )&pTmpUser) )
// 			{
// 				if(pTmpUser->QueryMagic(MAGIC_HB))
// 				{
// 					IMagicData* pData=pTmpUser->QueryMagic(MAGIC_HB);
// 					if(pData)
// 					{
// 						nLifeLost = nLifeLost * (100 - pData->GetInt(MAGICDATA_POWER))/100;
// 					}
// 				}
// 			}
			//--------------------------------------12.26.07 王萌
			
			setRole[i]->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
			
			if(setRole[i]->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
				m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
			
			setRole[i]->BeAttack(HitByMagic(), m_pOwner, setPower[i],addi);
			
			
		}

// 		if (m_pMagic->GetSTrackAmount() > 0)
// 			AdjustPosByTrack(k);

		// 必须先发轨迹后发kill消息
		for (i = 0; i < setRole.size(); i++)
		{
			if(!setRole[i]->IsAlive())
				m_pOwner->Kill(setRole[i], GetDieMode());
		}
	}
//	IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
//	if (pStatus)
//	{
//		int nLifeGot = ::CutTrail(0, MulDiv(nPowerSum, pStatus->GetPower(), 100));
//		if (nLifeGot > 0)
//			m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
//	}
//	AwardExp( 0 );
	return true;
}

//[2010-08-02 goto]修改此函数为技能等级加成计算，包括消耗蓝，CD时间等加成
int CMagic::AdjustMagicPower(int nPower,int nMagicType, int nlev, MAGICLEVADDTYPE nAddType)///调整玩家魔法值强度值,softworms-2007-09-05
{
	CHECKF(m_pOwner);	

	int nStep = 0;
	DEBUG_TRY
	if (nAddType==MAGICLEVADDTYPE_PRIVATECD/* || nAddType==MAGICLEVADDTYPE_PUBLICCD*/)
	{
		nStep = 101;
		if (m_pOwner->GetID() >= EUDEMON_ID_FIRST && m_pOwner->GetID() <= EUDEMON_ID_LAST)//如果是宠物，计算性格对CD的影响
		{
			nStep = 102;
			CMonster* pMonster = NULL;
			if( m_pOwner->QueryObj( OBJ_MONSTER,( void** )&pMonster) )
			{
				nStep = 103;
				CHECKF(m_pMagic);
				CEudemonMagicData* pMagicData = g_pNpcMagicManager->QueryEudemonDataByType(m_pMagic->GetInt(MAGICDATA_TYPE));
				nStep = 104;
				if (pMagicData && pMonster->GetHabit()==pMagicData->GetInt(EM_EUDEMONMAGIC_character))
				{
					nStep = 105;
					nPower += pMagicData->GetInt(EM_EUDEMONMAGIC_affect);
				}
			}
		}
	}
	nStep = 2;
	if (nlev==1)//1级的技能没有加成
		return nPower;
	if (!m_pMagic)
		return nPower;
	nStep = 3;
	int nPowerAdd = 0;
	switch(nAddType)
	{
	case MAGICLEVADDTYPE_POW:
		nStep = 4;
		nPowerAdd = m_pMagic->GetInt(MAGICDATA_POWER_ADD);
		break;
	case MAGICLEVADDTYPE_MP:
		nStep = 5;
		nPowerAdd = m_pMagic->GetInt(MAGICDATA_USE_MP_ADD);
		break;
	case MAGICLEVADDTYPE_PRIVATECD:
		nStep = 6;
		nPowerAdd = m_pMagic->GetInt(MAGICDATA_DELAY_ADD);
		break;
	case MAGICLEVADDTYPE_PUBLICCD:
		nStep = 7;
		nPowerAdd = m_pMagic->GetInt(MAGICDATA_PUBLICDELAY_ADD);
		break;
	case MAGICLEVADDTYPE_STEPSEC:
		nStep = 8;
		nPowerAdd = m_pMagic->GetInt(MAGICDATA_STEP_ADD);
		break;
	case MAGICLEVADDTYPE_TIMES:
		nStep = 9;
		nPowerAdd = m_pMagic->GetInt(MAGICDATA_TIMES_ADD);
		break;
	case MAGICLEVADDTYPE_TARGET:
		nStep = 10;
		nPowerAdd = m_pMagic->GetInt(MAGICDATA_TARGET_ADD);
		break;
	case MAGICLEVADDTYPE_POW2:
		nStep = 11;
		nPowerAdd = m_pMagic->GetInt(MAGICDATA_POWER2_ADD);
		break;
	case MAGICLEVADDTYPE_STATUS_POW:
		nStep = 12;
		nPowerAdd = m_pMagic->GetInt(MAGICDATA_STATUS_ADD);
		break;
	default:
		break;
	}
	nStep = 13;
	if (nPowerAdd==0)
		return nPower;
	nStep = 14;
	if (nAddType==MAGICLEVADDTYPE_PRIVATECD || nAddType==MAGICLEVADDTYPE_PUBLICCD)
	{
		nStep = 15;
		nPower = nPower + nPowerAdd*(nlev-1);
		return nPower;
	}
	if (nPowerAdd/100000==1)//数值类
	{
		nStep = 16;
		int nAddition = nPowerAdd%100000;
		return nPower+(nAddition*1.0f/10)*(nlev-1);
	}
	else if (nPowerAdd/100000==2)//百分比
	{
		nStep = 17;
		float nAddition = nPowerAdd%100000*1.0f/10000;
		return nPower+nPower*nAddition*(nlev-1);
	}
	else if (nPowerAdd/100000==3)//数值，扩大了100倍
	{
		nStep = 18;
		int nAddition = nPowerAdd%100000;
		return nPower+(nAddition*1.0f/100)*(nlev-1);
	}
	return nPower;
// 	if (m_pOwner->IsUser())
// 	{
// 		CUserPtr pUser=(CUserPtr)m_pOwner;
// 
// 		IMagicData* pData=pUser->QueryMagic(MAGIC_PASSIVITY_EASYTAKE);
// 		if (pData && (nMagicType==MAGIC_BARB || ///穿刺
// 			nMagicType==MAGIC_DEATHCHAINS ||///死亡枷锁
// 			nMagicType==MAGIC_DEATHHANDSHAKE) ) ///死亡之握
// 		{
// 			nPower=AdditionPower(nPower,pData->GetInt(MAGICDATA_POWER));
// 		}
// 
// 		pData=pUser->QueryMagic(MAGIC_PASSIVITY_BEASTRECOVERY);///是否有野兽恢复被动技能
// 		if (pData && (nMagicType==MAGIC_HEALMAGIC || ///治愈术
// 			nMagicType==MAGIC_COLONYHEAL))///群体治愈术
// 		{
// 			nPower=AdditionPower(nPower,pData->GetInt(MAGICDATA_POWER));
// 		}
// 
// 		pData=pUser->QueryMagic(MAGIC_PASSIVITY_PAINSUPPRESS);///是否有痛苦压制被动技能
// 		if (pData && (nMagicType==MAGIC_ENERGYSHIELD || ///能量罩
// 			nMagicType==MAGIC_CRAZEAFFLATUS) )///狂热灵感
// 		{
// 			nPower=AdditionPower(nPower,pData->GetInt(MAGICDATA_POWER));
// 		}
// /*	MAGIC_DZAY = 4101,
// 	MAGIC_KBHJ = 4102,
// 	MAGIC_CEJX = 6803,
// 	MAGIC_YQGH = 6804,
// 	MAGIC_ZJ   = 4107,
// 	MAGIC_TQTB = 6811,
// */
// 		pData=pUser->QueryMagic(MAGIC_PASSIVITY_ZLFK);///被动-龙族--战士--振聋发聩
// 		if (pData && (nMagicType==MAGIC_DZAY || 
// 			nMagicType==MAGIC_KBHJ ||
// 			nMagicType==MAGIC_CEJX ||
// 			nMagicType==MAGIC_YQGH ||
// 			nMagicType==MAGIC_ZJ   ||
// 			nMagicType==MAGIC_TQTB) )
// 		{
// 			nPower=AdditionPower(nPower,pData->GetInt(MAGICDATA_POWER));
// 		}
// 		
// 		pData=pUser->QueryMagic(MAGIC_PASSIVITY_XLSH);///被动-龙族--弓手--心领神会
// 		if (pData && (nMagicType==MAGIC_DZAY || 
// 			nMagicType==MAGIC_YJWX ||
// 			nMagicType==MAGIC_TZLW ||
// 			nMagicType==MAGIC_JYPF)  )
// 		{
// 			nPower=AdditionPower(nPower,pData->GetInt(MAGICDATA_POWER));
// 		}
// 
// 		pData=pUser->QueryMagic(MAGIC_PASSIVITY_GGZH);///被动-龙族-法师-亘古智慧
// 		if (pData && (nMagicType==MAGIC_MLZF || 
// 			nMagicType==MAGIC_FSBH ||
// 			nMagicType==MAGIC_FL ||
// 			nMagicType==MAGIC_QNBF)  )
// 		{
// 			nPower=AdditionPower(nPower,pData->GetInt(MAGICDATA_POWER));
// 		}
// 			 
// 		pData=pUser->QueryMagic(MAGIC_PASSIVITY_TZBD);///被动-基因--战士--特种部队
// 		if (pData && (nMagicType==MAGIC_SSZ || 
// 			nMagicType==MAGIC_FJCY) )
// 		{
// 			nPower=AdditionPower(nPower,pData->GetInt(MAGICDATA_POWER));
// 		}
// 
// 		pData=pUser->QueryMagic(MAGIC_PASSIVITY_DWXF);///被动-基因--枪手--弹无虚发
// 		if (pData && (nMagicType==MAGIC_SS || 
// 			nMagicType==MAGIC_QLSJ ||
// 			nMagicType==MAGIC_MZSJ) )
// 		{
// 			nPower=AdditionPower(nPower,pData->GetInt(MAGICDATA_POWER));
// 		}
//  
// 	
// 		pData=pUser->QueryMagic(MAGIC_PASSIVITY_JSMX);	///被动-基因--法师--静思冥想
// 		if (pData && (nMagicType==MAGIC_CND || 
// 			nMagicType==MAGIC_JNTX ||
// 			nMagicType==MAGIC_JSGJ) )
// 		{
// 			nPower=AdditionPower(nPower,pData->GetInt(MAGICDATA_POWER));
// 		}
// 
// 	}
//	return nPower;
	DEBUG_CATCH2("CMagic::AdjustMagicPower() nStep = %d",nStep)
	return nPower;
}

//////////////////////////////////////////////////////////////////////
//60 嘲讽
bool CMagic::Process_Bellow()
{
	CHECKF(m_pMagic);

	ROLE_SET		setRole;
	POINT	pos;
	int nPower =  m_pMagic->GetInt(MAGICDATA_POWER);

	int nPowerSum = 0;
// 	for (int k=0; k<m_nApplyTimes; k++)
// 	{
	// 搜索目标集
	setRole.clear();
	if (m_pMagic->GetInt(MAGICDATA_MULTI)==1)
		CollectTargetSet_Bomb(setRole, pos, LOCK_NONE);
	else
	{
		IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
		if(!pTarget || !pTarget->IsAlive() || m_idTarget==m_pOwner->GetID() || IsImmunity(pTarget))
			return false;
		setRole.push_back(pTarget);
	}

	// 广播法术效果，计算伤害
	CMsgMagicEffect	msg;
	IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), pos.x, pos.y, m_pOwner->GetDir()))
	{
		
		for(int i = 0; i < setRole.size(); i++)
		{
			IRole* pRole = setRole[i];
			if (pRole && pRole->IsAlive())
			{
				msg.AppendRole(pRole->GetID(),0,BATTLEADDINFO::INFO_INVALID);
			}
		}
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
	}

	// set crime status
	CheckCrime(&setRole);

	int nPower1 = AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER),m_pMagic->GetInt(MAGICDATA_TYPE),m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW);
	int nPower2 = 0;
//	if (RandGet(100)<m_pMagic->GetInt(MAGICDATA_STATUS_CHANCE))//附加属性的机率
		nPower2=AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER2),m_pMagic->GetInt(MAGICDATA_TYPE),m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW2);

	// 伤害
	for (int i = 0; i < setRole.size(); i++)
	{
		if (!setRole[i]->IsAlive())
			continue;
// 		CNpc* pNpc = NULL;
// 		CRole::AttachStatus(setRole[i],STATUS_SLEEP,0,nPower,0,m_pOwner->GetID());
		
		CMonster* pMonster = NULL;
		if( setRole[i]->QueryObj( OBJ_MONSTER,( void** )&pMonster ))
		{
			pMonster->AddEnmitySynNPC(m_pOwner->GetID(), nPower1, 0, nPower2);
			//pMonster->BeEnmityAttack(HitByMagic(), m_pOwner, nPower2);
		}
	}
//	}
//	CRole::AttachStatus(m_pOwner,STATUS_PALSY,0,nPower,0,0);
//	AwardExp( 0 );
	return true;
}
//07.4.16
//王玉波第一次修改
//增加功能，如果填写了status,一定机率使目标加一个状态，增加的状态暂时只支持不需要power字段的状态。因为技能已经使用了power字段
bool CMagic::ProcessBomb()
{
	int nStep = 0;
	DEBUG_TRY
	CHECKF(m_pMagic);

	ROLE_SET		setRole;
	vector<int>		setPower;
	POINT	pos;


	//特殊群攻耗自己百分比血量，造成同等伤害
	int nSpcDem = 0;
	if(MAGIC_SSQY == m_pMagic->GetInt(MAGICDATA_TYPE))
	{
		int nPowerSpc=AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER),m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW);
		nSpcDem = m_pOwner->GetLife() * (nPowerSpc / 10000.0f);
		if(m_pOwner->GetLife() < nSpcDem)
		{
			m_pOwner->SendSysMsg("血量不足,无法使用该技能");
			return false;
		}
		else
		{
			m_pOwner->AddAttrib(_USERATTRIB_LIFE, -1*nSpcDem, SYNCHRO_TRUE);
		}
	}

	

	int nPowerSum = 0;
	for (int k=0; k<m_nApplyTimes; k++)
	{
		// 搜索目标集
		{
			nStep = 1;
			setRole.clear();
			setPower.clear();

			CollectTargetSet_Bomb(setRole, pos, LOCK_NONE);
		}
		nStep = 2;
		
		// 广播法术效果，计算伤害
		nStep = 3;
		bool bCreate = false;
		CMsgMagicEffect	msg;
		IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), pos.x, pos.y, m_pOwner->GetDir()))
		{
			for(int i = 0; i < setRole.size(); i++)
			{
				IRole* pRole = setRole[i];
				if (pRole && pRole->IsAlive())
				{
					nStep = 4;
					int nTimes = AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_STEP), 0, m_pMagic->GetMagicLevel(), MAGICLEVADDTYPE_STEPSEC);					
					DWORD dwBattleAddInfo = 0;//无效
					//int nPower = GetPower(k);
					int att = 0;
					int nPower = 0;
// 					if(7004 == m_pMagic->GetInt(MAGICDATA_TYPE) )
// 					{						
// 						att = CBattleSystem::CalcDamage( dwBattleAddInfo,ATKTYPE_PHYSICAL_COMMON,m_pOwner,pRole);
// 						setPower.push_back(att);
// 
// 						nPower	= GetPower();
// 						nPower=AdjustMagicPower(nPower,m_pMagic->GetInt(MAGICDATA_TYPE));
// 					}
//					else if(7015 == m_pMagic->GetInt(MAGICDATA_TYPE) || 7005 == m_pMagic->GetInt(MAGICDATA_TYPE))
//					{
//						int nAddStatusTime = 0;
//						CUser* pGiftTmpAtkUser = NULL;
//						if( m_pOwner->QueryObj( OBJ_USER,( void** )&pGiftTmpAtkUser) )
//						{
//							nAddStatusTime = pGiftTmpAtkUser->ProcGiftMagicAddDuration(m_pMagic);
//						}
//						nTimes += nAddStatusTime;
//						setPower.push_back(nPower);
//					}
					nStep = 5;
					if(MAGIC_SSQY == m_pMagic->GetInt(MAGICDATA_TYPE))
					{
						if(setRole[i]->IsUser() || setRole[i]->QueryOwnerUser()) //对方人或宠物
						{
							setPower.push_back(nSpcDem);
						}
						else
						{
							setPower.push_back(nSpcDem * 3);
						}
					}
					else if (MAGIC_JGBRZ==m_pMagic->GetInt(MAGICDATA_TYPE))//金刚般若掌,物理攻击伤害*power%
					{
						int nPower1=AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER),m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW);
						nPower=CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pRole,0,0,0,m_pMagic->GetInt(MAGICDATA_ATTACK_TYPE));
						nPower = nPower * nPower1*1.0f/100;
						setPower.push_back(nPower);
					}
					else
					{
						int nPower1=AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER),m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW);///对应POWER进行修正
						int nPower2=AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER2),m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW2);
						nPower=CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pRole,nPower1,0,nPower2,m_pMagic->GetInt(MAGICDATA_ATTACK_TYPE));
//modify code by 林德渊 begin 2011.6.22
						if( MAGIC_FJCY_2 == m_pMagic->GetInt(MAGICDATA_TYPE) )
						{
							CUser* pTmpUser = NULL;
							if( setRole[i]->QueryObj( OBJ_USER,( void** )&pTmpUser) )
							{
								nPower = pTmpUser->GetMaxLife()/100*75 + 2000;
								//火防
								nPower = nPower - setRole[i]->GetAddAttByType(11) * 5;
								nPower = nPower >0 ? nPower : 0 ;
//								nPower = __min(pTmpUser->GetLife(), nPower);
								
								IStatus* pStatus = setRole[i]->QueryStatus(STATUS_MAGIC_SHIELD);//魔法盾,用MP抵消伤害
								if( pStatus )
								{
									int i64Data = __max(0, setRole[i]->GetMana());
									if( nPower >= i64Data )
									{
										setRole[i]->AddAttrib(_USERATTRIB_MANA, -1*i64Data, SYNCHRO_TRUE);
										CRole::DetachStatus(setRole[i],STATUS_MAGIC_SHIELD);
										nPower = nPower - i64Data;
									}
									else
									{
										int nLostLife = i64Data - nPower;
										nPower = 0;
										setRole[i]->AddAttrib(_USERATTRIB_MANA, -1*nLostLife, SYNCHRO_TRUE);
									}
								}
							}
						}
//modify code by 林德渊 end
						setPower.push_back(nPower);
					}
					
// 					if(7004 == m_pMagic->GetInt(MAGICDATA_TYPE) )
// 					{
// 						msg.AppendRole( pRole->GetID(),att,dwBattleAddInfo );
// 					}
// 					else
// 					{
						msg.AppendRole( pRole->GetID(),nPower,dwBattleAddInfo );
//					}
						
// 					CUser* pAtkUser = NULL;
// 					if( m_pOwner->QueryObj( OBJ_USER, IPP_OF( pAtkUser ) ) )
// 					{
// 						if( pAtkUser->QueryMagic( MAGIC_HM ) )
// 						{
// 							int nProbability = 0;
// 							int nMagicType = m_pMagic->GetInt(MAGICDATA_TYPE);
// 							IMagicData* pData=pAtkUser->QueryMagic(MAGIC_HM);
// 							if (pData)
// 								nProbability = 0;//pData->GetInt( MAGICDATA_PERCENT );
// 							
// // 							if( pData && ( nMagicType == MAGIC_TBDL ))
// // 							{
// // 								if( RandGet( 100 ) <= nProbability )
// // 									CRole::AttachStatus( setRole[ i ],STATUS_SLEEP,100,pData->GetInt(MAGICDATA_STEP) );
// // 							}
// 						}
// 					}
// 					if( 7015 == m_pMagic->GetInt(MAGICDATA_TYPE) || 7005 == m_pMagic->GetInt(MAGICDATA_TYPE))
// 					{
// 						CRole::AttachStatus( setRole[ i ],m_pMagic->GetInt(MAGICDATA_STATUS),20,nTimes );						
// 					}
// 					else if( 7004 == m_pMagic->GetInt(MAGICDATA_TYPE) )
// 					{
// 						CRole::AttachStatus( setRole[ i ],m_pMagic->GetInt(MAGICDATA_STATUS),nPower,nTimes,m_pMagic->GetInt(MAGICDATA_TIMES) );
// 					}

               //------------------------------------------------------
 				}
				else
					setPower.push_back(0);
			}
			//m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
			bCreate = true;
		}
		nStep = 6;
		// set crime status
		CheckCrime(&setRole);

//		int addi = FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
		int addi = 0;
		if (setPower.size()!=setRole.size())
			return false;
		nStep = 7;
		// 伤害
		for (int i = 0; i < setRole.size(); i++)
		{
			if (!setRole[i] || !setRole[i]->IsAlive())
				continue;
			
			nPowerSum += setPower[i];
			
			int nLifeLost = setPower[i];
			
			
			nLifeLost = __min(setRole[i]->GetLife(), setPower[i]);

			//----------------------------------回避
			CUser* pTmpUser = NULL;
			if( setRole[i]->QueryObj( OBJ_USER,( void** )&pTmpUser) )
			{
				if(pTmpUser->QueryMagic(MAGIC_HB))
				{
					IMagicData* pData=pTmpUser->QueryMagic(MAGIC_HB);
					if(pData)
					{
						nLifeLost = nLifeLost * (100 - pData->GetInt(MAGICDATA_POWER))/100;
					}
				}
				nLifeLost = pTmpUser->PVPEmendDamage(nLifeLost, m_pOwner->GetID());
			}
			//--------------------------------------12.26.07 王萌
//modify code by 林德渊 begin 2011.6.22
			IStatus* pStatus = setRole[i]->QueryStatus(STATUS_RETORT_LGWS);
			if(pStatus)
			{
				m_pOwner->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
				m_pOwner->BeAttack(HitByMagic(),setRole[i],setPower[i],0,true);
			}
//modify code by 林德渊 end
			setRole[i]->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
			
			setRole[i]->BeAttack(HitByMagic(), m_pOwner, setPower[i],addi);
// 			nStep = 8;
// 			if(!setRole[i]->IsAlive())
// 			{
// 				m_pOwner->Kill(setRole[i], GetDieMode());
// 			}
		}

		if (bCreate)
			m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
		// 调整移动轨迹
// 		if (m_pMagic->GetSTrackAmount() > 0)
// 			AdjustPosByTrack(k);

		// 必须先发轨迹后发kill消息
		nStep = 8;
		for (i = 0; i < setRole.size(); i++)
		{
			if(setRole[i] && !setRole[i]->IsAlive())
			{
				CMonster* pMonster = NULL;
				if(setRole[i]->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))//宠物和怪物先死，因为人死的时候会收回宠物，这里的宠物指针就成了野指针
				{
					m_pOwner->Kill(setRole[i], GetDieMode());
					setRole[i] = NULL;//置为空，下一轮就不再处理
				}
			}
		}
		for (i = 0; i < setRole.size(); i++)
		{
			if(setRole[i] && !setRole[i]->IsAlive())
			{
				m_pOwner->Kill(setRole[i], GetDieMode());
			}
		}
// 			else
// 			{
// 				DWORD dwStatus	= m_pMagic->GetInt( MAGICDATA_STATUS );
// 				if( dwStatus != ID_NONE )
// 				{
// // 					int nPercent = m_pMagic->GetInt( MAGICDATA_PERCENT );
// // 					if( RandGet( 100 ) < nPercent )
// // 					{
// // 						CRole::AttachStatus( setRole[ i ],dwStatus,100,m_pMagic->GetInt( MAGICDATA_STEP ),m_pMagic->GetInt( MAGICDATA_TIMES ) );
// // 						//power 100 无意义，随便填
// // 					}
// 				}
// 
// 			}
// 		}
	}
	DEBUG_CATCH2("bool CMagic::ProcessBomb() step=%d",nStep);
//	AwardExp( 0 );
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessBombLockAll()
{
	CHECKF(m_pMagic);

	POINT	pos;
	ROLE_SET		setRole;
	vector<int>		setPower;

	// 收集目标集
	CollectTargetSet_Bomb(setRole, pos, LOCK_ALL);

	
	int nPowerSum = 0;
	for (int k=0; k<m_nApplyTimes; k++)
	{
		// 广播法术效果
		CMsgMagicEffect	msg;
		IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), pos.x, pos.y, m_pOwner->GetDir()))
		{
			for(int i = 0; i < setRole.size(); i++)
			{
				IRole* pRole = setRole[i];
				if (pRole && pRole->IsAlive())
				{
					DWORD dwBattleAddInfo = 0;//无效
					int nPower = 0;//CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pRole,GetPower( k ),m_pMagic->GetInt( MAGICDATA_PERCENT ) );
					setPower.push_back(nPower);
					msg.AppendRole( pRole->GetID(), nPower,dwBattleAddInfo );

				}
				else
					setPower.push_back(0);
			}
			m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
		}

		// set crime status
		CheckCrime(&setRole);

		int addi = 0;//FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
		
		for (int i = 0; i < setRole.size(); i++)
		{
			if (!setRole[i]->IsAlive())
				continue;
			
			nPowerSum += setPower[i];
			CNpc* pNpc = NULL;
			int nLifeLost = setPower[i];
			
			nLifeLost = __min(setRole[i]->GetLife(), setPower[i]);
			setRole[i]->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
			
			if(setRole[i]->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
				m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
			
			setRole[i]->BeAttack(HitByMagic(), m_pOwner, setPower[i],addi);
				
		}

// 		if (m_pMagic->GetSTrackAmount() > 0)
// 			AdjustPosByTrack(k);

		// 必须先发轨迹后发kill消息
		for (i = 0; i < setRole.size(); i++)
		{
			if(!setRole[i]->IsAlive())
				m_pOwner->Kill(setRole[i], GetDieMode());
		}
	}

//	IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
//	if (pStatus)
//	{
//		int nLifeGot = ::CutTrail(0, MulDiv(nPowerSum, pStatus->GetPower(), 100));
//		if (nLifeGot > 0)
//			m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
//	}

//	AwardExp( 0 );
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessSquare()
{
	CHECKF(m_pMagic);

	MAPTHING_SET	setTarget;
	vector<IRole*>	setRole;
	vector<int>		setPower;
	POINT	pos;

	int	nRange	= m_pMagic->GetInt(MAGICDATA_RANGE);
	
	if(HitByWeapon())
		nRange	= m_pOwner->GetAttackRange(MAX_SIZEADD);

	int	nSize		= nRange*2 + 1;
	int	nBufSize	= nSize * nSize;

	int nPowerSum = 0;
	for (int k=0; k<m_nApplyTimes; k++)
	{
		// 搜索目标集
		{
			m_setTargetLocked.clear();
			setTarget.clear();
			setRole.clear();
			setPower.clear();

			int nDir	= m_pOwner->GetDir();
			if(m_pMagic->GetInt(MAGICDATA_GROUND))
			{
				pos.x		= m_pos.x;
				pos.y		= m_pos.y;
			}
			else
			{
				IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
				if(!pTarget || !pTarget->IsAlive())
					return false;

				pos.x		= pTarget->GetPosX();
				pos.y		= pTarget->GetPosY();
			}

			m_pOwner->GetMap()->CollectMapThing(setTarget, pos, nRange, OBJ_USER|OBJ_MONSTER|OBJ_NPC);

			for(int i = ::CutTrail((int)pos.x-nRange, 0); i <= pos.x+nRange && i < m_pOwner->GetMap()->GetWidth(); i++)
			{
				for(int j = ::CutTrail((int)pos.y-nRange, 0); j <= pos.y+nRange && j < m_pOwner->GetMap()->GetHeight(); j++)
				{
					POINT posThis;
					posThis.x	= i;
					posThis.y	= j;
					int idx = POS2INDEX(posThis.x - pos.x + nRange, posThis.y - pos.y + nRange, nSize, nSize);
					ASSERT(idx>=0 && idx<setTarget.size());
					//20080112修罗:从2维变为3维------
					//if(setTarget[idx])
					for(int zIdx=0;zIdx<setTarget[idx].size();zIdx++)
					{
						IRole* pRole = NULL;
						//if(setTarget[idx]->QueryRole(IPP_OF(pRole)) && pRole->IsAlive() && !IsImmunity(pRole))
						if(setTarget[idx][zIdx]->QueryRole(IPP_OF(pRole)) && pRole->IsAlive() && !IsImmunity(pRole))
						{
							setRole.push_back(pRole);
							// 只锁定一个攻击目标
							if (pRole->GetID() == m_idTarget)
								m_setTargetLocked.push_back(pRole->GetID());
						}
					}
					//-------------------------------
				}
			}
		}


		// 广播法术效果
		CMsgMagicEffect	msg;
		IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), pos.x, pos.y, m_pOwner->GetDir()))
		{
			for(int i = 0; i < setRole.size(); i++)
			{
				IRole* pRole = setRole[i];
				if (pRole && pRole->IsAlive())
				{
					DWORD dwBattleAddInfo = 0;//无效
					int nPower = 0;//CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pRole,GetPower(k),m_pMagic->GetInt( MAGICDATA_PERCENT ) );
					setPower.push_back(nPower);
					msg.AppendRole( pRole->GetID(),nPower,dwBattleAddInfo );

				}
				else
					setPower.push_back(0);
			}
			m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
		}

		// set crime status
		CheckCrime(&setRole);

		int addi = 0;//FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
			
		for (int i = 0; i < setRole.size(); i++)
		{
			if (!setRole[i]->IsAlive())
				continue;
			
			nPowerSum += setPower[i];
			CNpc* pNpc = NULL;
			int nLifeLost = setPower[i];
			
			
			nLifeLost = __min(setRole[i]->GetLife(), setPower[i]);
			setRole[i]->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
			
			if(setRole[i]->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
				m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
			
			setRole[i]->BeAttack(HitByMagic(), m_pOwner, setPower[i],addi);
			
		}

// 		if (m_pMagic->GetSTrackAmount() > 0)
// 			AdjustPosByTrack(k);

		// 必须先发轨迹后发kill消息
		for (i = 0; i < setRole.size(); i++)
		{
			if(!setRole[i]->IsAlive())
				m_pOwner->Kill(setRole[i], GetDieMode());
		}
	}

//	IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
//	if (pStatus)
//	{
//		int nLifeGot = ::CutTrail(0, MulDiv(nPowerSum, pStatus->GetPower(), 100));
//		if (nLifeGot > 0)
//			m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
//	}
//	AwardExp( 0 );


	return true;
}


bool CMagic::ProcessSwitchStatus()
{
	bool hr=false;
	CHECKF(m_pMagic);
	DWORD nStatus	= m_pMagic->GetInt(MAGICDATA_STATUS);///得到状态ID
	m_idTarget = m_pOwner->GetID();// 仇视，把对象ID设置为施法者，防止创建消息时idTarget为空写日志 [陈剑飞11/9/2007]
	if(m_pOwner->QueryStatus(nStatus)!=NULL)
		hr=ProcessDetach();
	else
		hr=ProcessAttach();
	return hr;
}

//////////////////////////////////////////////////////////////////////
//06.3.3王玉波修改
//回复单人，加状态的处理函数
bool CMagic::ProcessAttach()
{
	CHECKF(m_pMagic);

//	m_setTargetLocked.clear();
	IRole* pRole = m_pOwner->FindAroundRole(m_idTarget);
//modify code by 林德渊 begin 2011.6.22
	if( MAGIC_ZHF == m_pMagic->GetInt(MAGICDATA_TYPE) )
	{
		ROLE_SET		setRole;
		POINT	pos;
		setRole.clear();
		CollectTargetSet_BuffBomb(setRole, pos, LOCK_ALL,m_pMagic->GetInt( MAGICDATA_RANGE ));// 搜索目标集
		
		if( setRole.size() > 0 )
		{
			if( setRole.size() > 1 )
			{
				srand(unsigned(time(NULL)));//产生随机数
				int k = (int)setRole.size();
				int autonum = rand()%k;
				if( !setRole[autonum] || !setRole[autonum]->IsAlive() )
				{
					for (int i = 0; i < setRole.size(); i++)
					{
						if (!setRole[i] || !setRole[i]->IsAlive())
							continue;
						
						autonum = i;
						break;
					}
				}
				
				if( !(!setRole[autonum] || !setRole[autonum]->IsAlive()) )
					pRole = setRole[autonum];
			}
			else
				pRole = setRole[0];
		}
	}
//modify code by 林德渊 end
	if(!pRole || !pRole->IsAlive())
		return false;

	//  [2007-9-30] 朱斌	检查活动
// 	if(!HasSameWargameStatus(pRole))
// 		return false;

	if( m_pMagic->GetInt( MAGICDATA_CRIME ) ==1)//debuff
	{
		if( this->IsImmunity( pRole ) )
			return false;
	}
	else if (m_pMagic->GetInt( MAGICDATA_CRIME ) ==0)//buff,最好改为只能给队友加
	{
		if( m_pOwner != pRole && m_pOwner->IsAtkable( pRole ) )
		{
		// 	return false;
			pRole = m_pOwner;
		}
		//不能给守卫加BUFF，暂时这样处理,否则守卫会攻击你
		CMonster* pMonster = NULL;
		if( pRole->QueryObj( OBJ_MONSTER,( void** )&pMonster ) && pMonster->IsGuard() && pMonster->IsRealGuard())
		{
			pRole = m_pOwner;
			//return false;
		}
	}
	else if (m_pMagic->GetInt( MAGICDATA_CRIME ) ==2)//只能给自己或主人加
	{
		if (m_pOwner->GetID() >= EUDEMON_ID_FIRST && m_pOwner->GetID() <= EUDEMON_ID_LAST)
		{
			pRole = m_pOwner->QueryOwnerUser();
			if (!pRole || !pRole->IsAlive())
				return false;
			m_idTarget = pRole->GetID();
		}
		else
		{
			pRole = m_pOwner;
			m_idTarget = m_pOwner->GetID();
		}
	}
	
//	int nPower	= GetPower();
	int nPower=AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER),m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW);//计算等级加成

	int nSecs	= AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_STEP),m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), MAGICLEVADDTYPE_STEPSEC);
	int	nTimes	= AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_TIMES),m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_TIMES);
	DWORD nStatus	= m_pMagic->GetInt(MAGICDATA_STATUS);

//	m_setTargetLocked.push_back(m_idTarget);
	// 广播法术效果
	CMsgMagicEffect	msg;
//	IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_idTarget, 0, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
//		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);

	//  [11/3/2007 朱斌]	加强日志
	if(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), pRole->GetID(), 0, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
	else
	{
		char szTemp[50];
		char szMsg[255];
		if(m_pOwner)
		{
			sprintf(szMsg, "m_pOwner.ID = %d ", m_pOwner->GetID());
		}
		else
		{
			sprintf(szMsg, "m_pOwner = NULL ");
		}

		if(m_pMagic)
		{
			sprintf(szTemp, "m_pMagic.TYPE = %d ", m_pMagic->GetInt(MAGICDATA_TYPE));
		}
		else
		{
			sprintf(szTemp, "m_pMagic = NULL ");
		}
		strcat(szMsg, szTemp);
		
		sprintf(szTemp, "m_idTarget = %d", m_idTarget);
		strcat(szMsg, szTemp);

		log_assert("IF_NOT_DEBUG", szMsg, __FILE__, __LINE__);
	}

	// set crime status
	CheckCrime(pRole);

	// user status only
	void* pTemp;
// 	if( !pRole->QueryObj( OBJ_USER, IPP_OF( pTemp ) ) && ( nStatus == STATUS_LURK  ) )//给非玩家加潜行,return false
// 		return false;
	int addi = 0;//FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
	if( pRole->QueryObj(OBJ_USER, IPP_OF(pTemp)) )
		pRole->BeAttack(HitByMagic(), m_pOwner, 0,addi);
	else
		pRole->BeAttack(HitByMagic(), m_pOwner, 3,addi);//如果目标不是玩家，需要加上仇恨

// 	if( STATUS_FRENZY == nStatus )//狂暴技能使用Power字段
// 	{
// 		//受到的伤害加成,power
// 		//移动速度加成,parma1,
// 		//攻击速度加成,param2,
// 		//对目标的伤害加成param3
// 		//07.1.20王玉波修改,param1用潜能字段,param2用power字段,param3用EP字段
// 		int nParam1 = m_pMagic->GetInt( MAGICDATA_USE_POTENTIAL );
// 		int nParam2 = nPower;
// 		int nParam3 = m_pMagic->GetInt( MAGICDATA_USE_EP );
// 		CRole::AttachStatus( pRole, nStatus, nPower, nSecs,0,nParam1,nParam2,nParam3 );
// 	}
// 	if( STATUS_PARRY == nStatus )//招架技能
// 	{
// 		CRole::AttachStatus( pRole,STATUS_DAMAGEDOWN_PERCENT,nPower,nSecs,nTimes );//伤害减少
// 		CRole::AttachStatus( pRole,STATUS_PHYSICALATTACKDOWN_PERCENT,nPower,nSecs,nTimes );//物理攻击力减少
// 	}
// 	else if( STATUS_ATTACKMORE == nStatus )//重视进攻技能
// 	{
// 		CRole::AttachStatus( pRole,STATUS_DAMAGEUP_PERCENT,nPower,nSecs,nTimes );//伤害增加
// 		CRole::AttachStatus( pRole,STATUS_PHYSICALATTACKUP_PERCENT,nPower,nSecs,nTimes );//物理攻击力增加
// 	}
// 	else if( STATUS_GLARE == nStatus )//眩目之刺技能
// 	{
// 		CRole::AttachStatus( pRole,STATUS_PHYSICALDEFENCEDOWN_PERCENT,nPower,nSecs,nTimes );//防御减少
// 		CRole::AttachStatus( pRole,STATUS_HITRATEDOWN_PERCENT,nPower,nSecs,nTimes );//命中减少
// 
// 	}
// 	else if(  STATUS_DOT_POISON_ATK == nStatus )// 致命打击2,毒袭 [12/13/2007 陈剑飞]
// 	{
// 		int nStatusPower = GetPower();
// 		int nMinAtk = m_pOwner->GetAtk();
// 		int nMaxAtk = m_pOwner->GetAtk();
// 		
// 		int nRand = rand() % (nMaxAtk - nMinAtk);
// 		nRand += nMinAtk;
// 		int nCostLife = nRand;
// // 		int nSecs	= m_pMagic->GetInt(MAGICDATA_STEP);
// // 		int nTimes	= m_pMagic->GetInt(MAGICDATA_TIMES);
// 		CRole::AttachStatus(pRole, nStatus, nCostLife, nSecs, nTimes);
// 	}
// 	else if( STATUS_DOT_SUFFERING_PRE == nStatus )// 痛苦缠绕2 [12/13/2007 陈剑飞]
// 	{
// 		int nStatusPower = GetPower();
// 		int nMagicAtk = m_pOwner->GetMgcAtk();
// 		
// 		int nRand = nMagicAtk;
// 		nRand += nMagicAtk;
// 		int nCostLife = nRand;
// 		CRole::AttachStatus(pRole, nStatus, nCostLife, nSecs, nTimes);
// 		
// 	}
// 	else if (STATUS_LOCK_TARGET == nStatus)	//  [12/19/2007 朱斌]	挑战 强迫目标只能攻击自己，持续3秒。这是对目标使用的Debuff，需要新的status。使自己成为敌人的目标，且敌人不能改变目标。只对玩家有效。
// 	{
// 		int nOwnerID = m_pOwner->GetID();
// 		CRole::AttachStatus(pRole, nStatus, nOwnerID, nSecs, nTimes);	//	让目标只能攻击自己
// 	}
// 	// 快速恢复 [12/25/2007 陈剑飞]
// 	else if( STATUS_DOT_QUICK_ADDLIFE == nStatus )
// 	{
// 		CUser* pUser = NULL;
// 		if( pRole->QueryObj( OBJ_USER, IPP_OF( pUser ) ) )
// 		{
// 			int nLife = pUser->GetMaxSp();
// 			CRole::AttachStatus( pRole,nStatus,nLife,nSecs,nTimes );
// 		}
// 	}
// 	else if (7002 == m_pMagic->GetInt( MAGICDATA_TYPE )) 
// 	{
// 		int nStatusPower = GetPower();
// 		CRole::AttachStatus(pRole, STATUS_MOVESPEEDUP_PERCENT, nStatusPower, nSecs, nTimes);
// 		CRole::AttachStatus(pRole, STATUS_DAMAGEUP_PERCENT, nStatusPower, nSecs, nTimes);
// 	}
// 	else if (7018 == m_pMagic->GetInt( MAGICDATA_TYPE )) 
// 	{
// 		int nStatusPower = GetPower();
// 		CRole::AttachStatus(pRole, STATUS_PHYSICALATTUP_NUMBER, nStatusPower, nSecs, nTimes, 0, 0, 0, m_pOwner->GetID());
// 		CRole::AttachStatus(pRole, STATUS_DEFENCEDOWN_NUMBER, nStatusPower, nSecs, nTimes, 0, 0, 0, m_pOwner->GetID());
// 		CRole::AttachStatus(pRole, STATUS_MAXLIFEUP_PERCENT, nStatusPower, nSecs, nTimes, 0, 0, 0, m_pOwner->GetID());//goto添加,测试用
// 	}
// 	else if (7019 == m_pMagic->GetInt( MAGICDATA_TYPE )) 
// 	{
// 		int nStatusPower = GetPower();
// 		CRole::AttachStatus(pRole, STATUS_PHYSICALATTDOWN_NUMBER, nStatusPower, nSecs, nTimes, 0, 0, 0, m_pOwner->GetID());
// 		CRole::AttachStatus(pRole, STATUS_DEFENCEUP_NUMBER, nStatusPower, nSecs, nTimes, 0, 0, 0, m_pOwner->GetID());
// 	}
// 	if (MAGIC_TBS == m_pMagic->GetInt( MAGICDATA_TYPE ))
// 	{
// 		int nPowerDe=AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER2),m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW2);
// 		CRole::AttachStatus(pRole, STATUS_PHYSICALDEFENCEDOWN_NUMBER, nPowerDe, nSecs, nTimes, 0, 0, 0, m_pOwner->GetID());
// 		CRole::AttachStatus(pRole, STATUS_PHYSICALATTUP_NUMBER, nPower, nSecs, nTimes, 0, 0, 0, m_pOwner->GetID());
// 	}
	if (MAGIC_ZYHT == m_pMagic->GetInt( MAGICDATA_TYPE ))
	{
		CRole::AttachStatus(pRole, STATUS_SELF_DAMAGEUP, nPower, nSecs, nTimes, 0, 0, 0, m_pOwner->GetID());
		CRole::AttachStatus(pRole, STATUS_SPENDMP_UP, 20, nSecs, nTimes, 0, 0, 0, m_pOwner->GetID());
	}
	//-----------------------------------------------------------
	else
		CRole::AttachStatus(pRole, nStatus, nPower, nSecs, nTimes, 0, 0, 0, m_pOwner->GetID());
	
	
//	AwardExp( 0 );		// false: not award battle exp

	return true;
}

//////////////////////////////////////////////////////////////////////
//解除目标的一个状态
//07.3.31
//王玉波第一次修改
//原来的机制，去掉状态的实现方法为改变状态数据，使power字段为0.实际上还是只有等待状态结束条件（单次状态为持续时间超时多次为次数为0）发生
//实现方法不能满足需求，故更改
bool CMagic::ProcessDetach()
{
	CHECKF(m_pMagic);

	m_setTargetLocked.clear();
	IRole* pRole = m_pOwner->FindAroundRole(m_idTarget);
	if(!pRole)
		return false;

//-----------07.5.18王玉波添加------begin------------------------
	if( m_pMagic->GetInt( MAGICDATA_CRIME ) )//debuff
	{
		if( this->IsImmunity( pRole ) )
			return false;
	}
	else//buff
	{
		if( m_pOwner != pRole && m_pOwner->IsAtkable( pRole ) )
		{
			pRole = m_pOwner;
		//	return false;
		}
	}
//--------------------------------end---------------------------
//----------begin----------------------------------------------------------
//07.3.31
//王玉波注释
//	int nPower	= 0;		//解状态 GetPower();
//	int nSecs	= m_pMagic->GetInt(MAGICDATA_STEP);
//	int	nTimes	= m_pMagic->GetInt(MAGICDATA_TIMES);
//	int nStatus	= m_pMagic->GetInt(MAGICDATA_STATUS);
//	ASSERT(nPower == 0);	// 0 : 减少
//-----------end-----------------------------------------------------------
	int nStatus	= m_pMagic->GetInt(MAGICDATA_STATUS);
	if (!pRole->IsAlive())
	{
		if (STATUS_DIE != nStatus)
			return false;
		void* pTemp;
		if (!pRole->QueryObj(OBJ_USER, IPP_OF(pTemp)))
			return false;
	}

	// pk field will disable reborn magic
	if (STATUS_DIE == nStatus)
	{
		CGameMap* pMap = m_pOwner->GetMap();
		if (pMap && pMap->IsPkField())
			return false;		
	}

	if(nStatus == STATUS_DETACH_ALL)
	{
		CRole::DetachAllStatus(pRole);
	}
	else if(nStatus == STATUS_DETACH_BADLY)
	{
		CRole::DetachBadlyStatus(pRole);
	}
	// 技能净化术，去除最多为2个的不利状态 [12/26/2007 陈剑飞]
	else if( STATUS_DETACH_SOMEBAD == nStatus )
	{
		int nPowerTmp = m_pMagic->GetInt( MAGICDATA_POWER );
		int nCount = RandGet( nPowerTmp );
		if( nCount == 0 )
			nCount = 1;
		CRole::DetachSomeBadStatus( pRole, nCount );
	}
	else if( STATUS_RECOVER == nStatus )//恢复术,去掉中毒，与流血
	{
//		CRole::DetachStatus( pRole,STATUS_DOT_POISONING );
		CRole::DetachStatus( pRole,STATUS_DOT_LIFEMISSING );
//		CRole::DetachStatus( pRole,STATUS_DOT_ENMITY );
	}
	else
	{
//------------begin----------王玉波注释并添加CRole::DetachStatus( pRole,nStatus )----------------
//07.3.31
//		IStatus* pStatus = pRole->QueryStatus(nStatus);
//		if(pStatus)
//			pStatus->ChangeData(nPower, nSecs, nTimes);
		CRole::DetachStatus( pRole,nStatus );
//------------end--------------------------------------------------------------------------------
	}

//	m_setTargetLocked.push_back(m_idTarget);
	// 广播法术效果
	CMsgMagicEffect	msg;
	IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_idTarget, 0, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);

	// set crime status
	CheckCrime(pRole);

//	pRole->ClsStatus(nStatus, SYNCHRO_TRUE);		//07.3.31 王玉波注释

	if (STATUS_DIE == nStatus)
	{	// reborn
		CUser* pUser = NULL;
		pRole->QueryObj(OBJ_USER, IPP_OF(pUser));
		if (pUser)
		{
			bool bChgPos = false;
			pUser->Reborn(bChgPos);
		}
	}

//	AwardExp(0);

	return true;
}
//////////////////////////////////////////////////////////////////////
//06.11.13
//王玉波
//龙之心技能处理
//解除目标一切异常状态,并且免疫一切伤害，但自身也无法移动。持续短暂数秒

bool CMagic::ProcessDragonHeart()
{
	CHECKF(m_pMagic);

	IRole* pRole = m_pOwner->FindAroundRole(m_idTarget);
	if(!pRole)
		return false;

	int  nSecs	= m_pMagic->GetInt(MAGICDATA_STEP);

	if (!pRole->IsAlive())	
	{
		void* pTemp;
		if (!pRole->QueryObj(OBJ_USER, IPP_OF(pTemp)))//目标必须是玩家
			return false;
	}
	const int nNoMoveSenc = 3;//自身龙心状态时间

	CRole::DetachBadlyStatus(pRole);								//对方去掉不良状态
	CRole::AttachStatus( pRole,STATUS_IMMUNE_ALL,0,nSecs );				//对方+无敌状态
	CRole::AttachStatus( m_pOwner,STATUS_DREAMING,0,nNoMoveSenc );	//
	// 广播法术效果
	CMsgMagicEffect	msg;
	IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_idTarget,STATUS_DREAMING,m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);

//	AwardExp( 0 );	//加技能经验.....
	return true;
}
//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessJumpAttackFaild(int x, int y)
{
	CHECKF(m_pMagic);

	m_setTargetLocked.clear();
	int		nPower		= 0;
	OBJID	idTarget	= ID_NONE;

	// 广播法术效果
	CMsgMagicEffect	msg;
	IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), x, y, idTarget, nPower, m_pOwner->GetDir()))
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);

	m_pOwner->ProcessOnMove(MOVEMODE_JUMPMAGICATTCK);
	m_pOwner->JumpPos(x, y);

	return true;
}

//////////////////////////////////////////////////////////////////////
//goto瞬移并攻击
bool CMagic::ProcessFlyStrike()
{
	CHECKF ( m_pMagic );
	CHECKF ( m_pOwner );
	IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
	if(!pTarget || !pTarget->IsAlive() || m_idTarget==m_pOwner->GetID())
		return false;
	if ( this->IsImmunity(pTarget) )
	{
		return false;//  [6/12/2007]陈剑飞
	}
	//assert m_pOwner is a User
	CUser* pUser = NULL;
	if( !m_pOwner->QueryObj( OBJ_USER,( void** )&pUser ) )
	{
		return false;
	}
	//查找瞬移目标点
	CMapPtr pMap = m_pOwner->GetMap();
	if (pMap)
	{
		//int nDistance = m_pMagic->GetInt(MAGICDATA_DISTANCE);//移动的距离,前面已经检查过攻击距离，这里不再判断
		int nDir = GetDirByPos(m_pOwner->GetPosX(), m_pOwner->GetPosY(), pTarget->GetPosX(), pTarget->GetPosY());
		m_pos.x = pTarget->GetPosX();
		m_pos.y = pTarget->GetPosY();

		if (pMap->IsStandEnable(m_pos.x+_DELTA_X[nDir], m_pos.y+_DELTA_Y[nDir]))
		{
			m_pos.x += _DELTA_X[nDir];
			m_pos.y += _DELTA_Y[nDir];
		}
		
// 		if (pUser->GetDistance(nPosX,nPosY)>nDistance)
// 		{
// 			m_pOwner->SendSysMsg(_TXTATR_TOPSHOW, "距离太远");
// 			return false;
// 		}
		//pUser->SetPosX(m_pos.x);
		//pUser->SetPosY(m_pos.y);
		pUser->TransPos(m_pos.x, m_pos.y);
// 		CMsgWalkEx msgWalk;
// 		if(msgWalk.Create(m_pOwner->GetID(), m_pOwner->GetDir(), MOVEMODE_SYNCHRO, m_pOwner->GetPosX(), m_pOwner->GetPosY()))
// 		{
// 			MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(ID_NONE, &msgWalk);
// 		}
	}
	else
		return false;
	//计算攻击
	DWORD dwBattleAddInfo = BATTLEADDINFO::INFO_INVALID;//无效的
	
	int nPower1 = AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER),m_pMagic->GetInt(MAGICDATA_TYPE),m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW);
	int nPower2 = 0;
	if (RandGet(100)<m_pMagic->GetInt(MAGICDATA_STATUS_CHANCE))//附加属性的机率
		nPower2=AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER2),m_pMagic->GetInt(MAGICDATA_TYPE),m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW2);

	int nPower = CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pTarget,nPower1,0,nPower2,m_pMagic->GetInt(MAGICDATA_ATTACK_TYPE));
	
	// 广播法术效果
	bool bCreate = false;
	CMsgMagicEffect	msg;
	IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_pos.x, m_pos.y, m_pOwner->GetDir()))
	{
		if (msg.AppendRole(m_idTarget, nPower, dwBattleAddInfo))
			bCreate = true;
			//m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
	}

	CheckCrime(pTarget);

	if (nPower)
	{
		int nLifeLost = nPower;
		nLifeLost = __min(pTarget->GetLife(), nPower);
		CUser* pUser = NULL;
		pTarget->QueryObj(OBJ_USER, IPP_OF(pUser));
		if(pUser)
		{
			nLifeLost = pUser->PVPEmendDamage(nLifeLost, m_pOwner->GetID());
		}
//modify code by 林德渊 begin 2011.6.22
		IStatus* pStatus = pTarget->QueryStatus(STATUS_RETORT_LGWS);
		if(pStatus)
		{
			m_pOwner->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
			m_pOwner->BeAttack(HitByMagic(),pTarget,nPower,0,true);
		}
//modify code by 林德渊 end
		pTarget->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
		int addi = 0;//FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
		pTarget->BeAttack(HitByMagic(), m_pOwner, nPower,addi);					
	}
	if (bCreate)
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
	if (!pTarget->IsAlive())
		m_pOwner->Kill(pTarget, GetDieMode());

	return true;
}
//////////////////////////////////////////////////////////////////////
//goto 69 //后退并攻击
bool CMagic::ProcessWithDrawAtk()
{
	CHECKF ( m_pMagic );
	CHECKF ( m_pOwner );
	IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
	if(!pTarget || !pTarget->IsAlive() || m_idTarget==m_pOwner->GetID())
		return false;
	if ( this->IsImmunity(pTarget) )
	{
		return false;//  [6/12/2007]陈剑飞
	}
	//assert m_pOwner is a User
	CUser* pUser = NULL;
	if( !m_pOwner->QueryObj( OBJ_USER,( void** )&pUser ) )
	{
		return false;
	}
	//查找瞬移目标点
	CMapPtr pMap = m_pOwner->GetMap();
	if (pMap)
	{
		int nDistance = m_pMagic->GetInt(MAGICDATA_RANGE);//移动的距离
		int nDir = GetDirByPos(m_pOwner->GetPosX(), m_pOwner->GetPosY(), pTarget->GetPosX(), pTarget->GetPosY());
		m_pos.x = pUser->GetPosX()-_DELTA_X[nDir]*nDistance;
		m_pos.y = pUser->GetPosY()-_DELTA_Y[nDir]*nDistance;
		
		while (!pMap->IsStandEnable(m_pos.x, m_pos.y))
		{
			m_pos.x += _DELTA_X[nDir];
			m_pos.y += _DELTA_Y[nDir];
		}
		
		if (pUser->GetDistance(m_pos.x,m_pos.y)>nDistance)
		{
			return false;
		}
		//pUser->SetPosX(m_pos.x);
		//pUser->SetPosY(m_pos.y);
		if (!(pUser->GetPosX()==m_pos.x && pUser->GetPosY()==m_pos.y))//如果目标点是原地，就不用瞬移
		{
			pUser->TransPos(m_pos.x, m_pos.y);
// 			CMsgWalkEx msgWalk;
// 			if(msgWalk.Create(pUser->GetID(), pUser->GetDir(), MOVEMODE_SYNCHRO, pUser->GetPosX(), pUser->GetPosY()))
// 			{
// 				MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(ID_NONE, &msgWalk);
// 			}
		}
	}
	else
		return false;
	//计算攻击
	DWORD dwBattleAddInfo = BATTLEADDINFO::INFO_INVALID;//无效的
	
	int nPower1 = AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER),m_pMagic->GetInt(MAGICDATA_TYPE),m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW);
	int nPower2 = 0;
	if (RandGet(100)<m_pMagic->GetInt(MAGICDATA_STATUS_CHANCE))//附加属性的机率
		nPower2=AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER2),m_pMagic->GetInt(MAGICDATA_TYPE),m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW2);
	
	int nPower = CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pTarget,nPower1,0,nPower2,m_pMagic->GetInt(MAGICDATA_ATTACK_TYPE));
	
	CheckCrime(pTarget);
	
	if (nPower)
	{
		int nLifeLost = nPower;
		nLifeLost = __min(pTarget->GetLife(), nPower);
		CUser* pUser = NULL;
		pTarget->QueryObj(OBJ_USER, IPP_OF(pUser));
		if(pUser)
		{
			nLifeLost = pUser->PVPEmendDamage(nLifeLost, m_pOwner->GetID());
		}
//modify code by 林德渊 begin 2011.6.22
		IStatus* pStatus = pTarget->QueryStatus(STATUS_RETORT_LGWS);
		if(pStatus)
		{
			m_pOwner->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
			m_pOwner->BeAttack(HitByMagic(),pTarget,nPower,0,true);
		}
//modify code by 林德渊 end
		pTarget->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
		int addi = 0;//FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
		pTarget->BeAttack(HitByMagic(), m_pOwner, nPower,addi);					
	}
	// 广播法术效果
	CMsgMagicEffect	msg;
	IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_pos.x, m_pos.y, m_pOwner->GetDir()))
	{
		if (msg.AppendRole(m_idTarget, nPower, dwBattleAddInfo))
			m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
	}
	if (!pTarget->IsAlive())
		m_pOwner->Kill(pTarget, GetDieMode());
	
	return true;
}
//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessRandomTrans()
{
	CHECKF(m_pMagic);

	m_setTargetLocked.clear();
	int nWidth = GetPower() * 2 + 1;

//	AwardExp(0);
	
	for(int i = 0; i < RANDOMTRANS_TRY_TIMES; i++)
	{
		int x = (int)m_pOwner->GetPosX()+nWidth/2-::RandGet(nWidth);
		int y = (int)m_pOwner->GetPosY()+nWidth/2-::RandGet(nWidth);
		if(m_pOwner->GetMap()->IsStandEnable(x, y))
		{
			// 广播法术效果
			CMsgMagicEffect	msg;
			IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), x, y, m_pOwner->GetDir()))
				m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);


			// move!!! 攻击方需要锁定坐标
			m_pOwner->ProcessOnMove(MOVEMODE_TRANS);
			m_pOwner->TransPos(x, y);		//? may be optimize to JumpPos(,)
			m_pOwner->Synchro();

			return true;
		}
	}

	m_pOwner->SendSysMsg(g_objStrRes.GetStr( 12017 ));//STR_MAGIC_FAILED
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessDispatchXP()
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////
// 清除CD [12/21/2007 陈剑飞]
bool CMagic::ProcessClearCD()
{
	CHECKF( m_pMagic );
	ClearDelay();
	
	CMsgMagicEffect msg;
	IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), 0,0))
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessCollideFail(int x, int y, int nDir)
{
//	m_setTargetLocked.clear();
//	// target data is dir
//	int nTargetX	= m_pos.x+_DELTA_X[nDir];
//	int nTargetY	= m_pos.y+_DELTA_Y[nDir];
//	int	nPower		= 0;
//	OBJID	idTarget	= ID_NONE;
//
//	IF_NOT(m_pOwner->GetMap()->IsStandEnable(nTargetX, nTargetY))
//	{
//		m_pOwner->SendSysMsg(STR_INVALID_MSG);
//		CUser* pUser = NULL;
//		if (m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
//			UserManager()->KickOutSocket(pUser->GetSocketID(), "COLLIDE coord!");
//		return false;
//	}
//
//	CMsgInteract msg;
//	IF_OK (msg.Create(INTERACT_BUMP, m_pOwner->GetID(), idTarget, nTargetX, nTargetY, nDir*0x01000000+nPower))
//		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
//
//	// move now...
//	// if (!(m_pOwner->GetPosX() == m_pos.x && m_pOwner->GetPosY() == m_pos.y))
//	{
//		m_pOwner->ProcessOnMove(MOVEMODE_COLLIDE);
//		m_pOwner->MoveToward(nDir);
//	}

	return true;
}

//////////////////////////////////////////////////////////////////////
//顺势斩？冲撞？王玉波添加
bool CMagic::ProcessCollide()
{
//	CHECKF(m_pMagic);
//	CHECKF(m_pOwner);
//
//	m_setTargetLocked.clear();
//	// check syn of user
//	IF_NOT(m_pOwner->SynPosition(m_pos.x, m_pos.y))
//	{
//		CUser* pUser = NULL;
//		if (m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
//			UserManager()->KickOutSocket(pUser->GetSocketID(), "ProcessCollide SynPosition");
//		return false;
//	}
//
//	// target data is dir
//	int nDir		= m_nData;
//	int nTargetX	= m_pos.x+_DELTA_X[nDir];
//	int nTargetY	= m_pos.y+_DELTA_Y[nDir];
//
//	IF_NOT(m_pOwner->GetMap()->IsStandEnable(nTargetX, nTargetY))
//	{
//		m_pOwner->SendSysMsg(STR_INVALID_MSG);
//		CUser* pUser = NULL;
//		if (m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
//			UserManager()->KickOutSocket(pUser->GetSocketID(), "COLLIDE coord!");
//		return false;
//	}
//
//	// search the target
//	bool bSuc = true;
//	IRole* pTarget = m_pOwner->GetMap()->QueryRole(nTargetX, nTargetY, ID_NONE);
//	if(!pTarget || !pTarget->IsAlive())
//		bSuc = false;
//
//	if (!pTarget || this->IsImmunity(pTarget))
//		bSuc = false;
//
//	//if (m_pOwner->GetDistance(nTargetX, nTargetY) > m_pMagic->GetInt(MAGICDATA_DISTANCE))
//	//	bSuc = false;
//
//	int	nPower	= 0;
//	bool bBackEnable = false;
//	OBJID idTarget = ID_NONE;
//
//	CNpc* pNpc = NULL;
//	if (pTarget)
//	{
//		pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc));
//
////		int nHitRate = 100;
////		if (::RandGet(100) >= nHitRate)
////			bSuc = false;
//
//		if (bSuc)
//		{
//			bBackEnable	= pTarget->GetMap()->IsStandEnable(nTargetX+_DELTA_X[nDir], nTargetY+_DELTA_Y[nDir]);
//			if(pNpc)
//				bBackEnable = false;
//
//			if (!bBackEnable)
//				nDir = MAX_DIRSIZE;
//
//			if (HitByWeapon())
//			{
//				nPower += CBattleSystem::CalcDamage_PhysicalCommon( m_pOwner,pTarget );
//
//				if(bBackEnable || pNpc)
//					nPower	= MulDiv(nPower, COLLIDE_POWER_PERCENT, 100);
//				else
//					nPower	= m_pOwner->AdjustData(nPower, GetPower());
//			}
//
//	//		if(!m_pOwner->GetMap()->IsTrainMap())
//		//		m_pOwner->AddEquipmentDurability(ITEMPOSITION_AMMO, -1*COLLIDE_SHIELD_DURABILITY);
//		}
//
//		idTarget = pTarget->GetID();
////		m_setTargetLocked.push_back(idTarget);
//	}
//
//	CMsgInteract msg;
//	DWORD dwData = nDir*0x01000000+nPower;
//	if (!bBackEnable)
//		dwData += 1*0x10000000;
//
//	IF_OK (msg.Create(INTERACT_BUMP, m_pOwner->GetID(), idTarget, nTargetX, nTargetY, dwData))
//		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
//
//	if (m_pOwner->IsPM())
//	{
//		CUser* pUser = NULL;
//		if (m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
//			pUser->SendSysMsg(_TXTATR_NORMAL, "bump move:(%u, %u)->(%u, %u)", 
//								pUser->GetPosX(), pUser->GetPosY(),
//								nTargetX, nTargetY);
//	}
//						
//	// move now...
//	// if (!(m_pOwner->GetPosX() == m_pos.x && m_pOwner->GetPosY() == m_pos.y))
//	if (bBackEnable)
//	{
//		m_pOwner->ProcessOnMove(MOVEMODE_COLLIDE);
//		m_pOwner->MoveToward(nDir);
//	}
//
//	if (pTarget && bBackEnable && bSuc)
//	{
//		pTarget->ProcessOnMove(MOVEMODE_COLLIDE);
//		pTarget->MoveToward(nDir);
//
//		// paled - set not synchro yet
//		CUser* pTargetUser;
//		if(pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser)))
//		{
//			// syn pos of client
//			pTargetUser->SetSynchro(false);
//		}
//	}
//
//	// set crime status
//	if(pTarget)
//		CheckCrime(pTarget);
//
//	if (nPower && pTarget)
//	{
//		CNpc* pNpc = NULL;
//		int nLifeLost = nPower;
//		
//		nLifeLost = __min(pTarget->GetLife(), nPower);
//		pTarget->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
//		
//		if(pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
//			m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
//		
//		pTarget->BeAttack(HitByMagic(), m_pOwner, nPower);
//		
//		if (!pTarget->IsAlive())
//			m_pOwner->Kill(pTarget, GetDieMode());
//		
////		IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
////		if (pStatus)
////		{
////			int nLifeGot = ::CutTrail(0, MulDiv(nPower, pStatus->GetPower(), 100));
////			if (nLifeGot > 0)
////				m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
////		}
//	}
//	AwardExp( 0 );	//加技能经验.....
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessSerialCut()
{
	CHECKF(m_pMagic);

	m_setTargetLocked.clear();
	POINT pos;
	pos.x	= m_pos.x;
	pos.y	= m_pos.y;

	int	nRange	= 1;

	int	nSize		= nRange*2 + 1;
	int	nBufSize	= nSize * nSize;
	vector<IRole*>	setRole;
	vector<int>		setPower;
	vector<DWORD>	setBattleAddInfo;

	for(int i = 0; i < MAX_SERIALCUTSIZE; i++)
	{
		MAPTHING_SET	setTarget;
		m_pOwner->GetMap()->CollectMapThing(setTarget, pos, nRange, OBJ_USER|OBJ_MONSTER|OBJ_NPC);
		IRole* pTarget = NULL;
		for(int j = 0; j < setTarget.size(); j++)
		{
			int idx = j;
			ASSERT(idx>=0 && idx<setTarget.size());
			//20080112修罗:从2维变为3维------
			//if(setTarget[idx])
			for(int zIdx=0;zIdx<setTarget[idx].size();zIdx++)
			{
				IRole* pRole = NULL;
				//if(setTarget[idx]->QueryRole(IPP_OF(pRole)) && pRole->IsAlive() && !IsImmunity(pRole))
				if(setTarget[idx][zIdx]->QueryRole(IPP_OF(pRole)) && pRole->IsAlive() && !IsImmunity(pRole))
				{
					DWORD dwBattleAddInfo = BATTLEADDINFO::INFO_INVALID;
					int	nPower = CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pRole, GetPower());
					
					setRole.push_back(pRole);
					setPower.push_back(nPower);
					setBattleAddInfo.push_back( dwBattleAddInfo );
					// 只锁定一个攻击目标
					//						if (pRole->GetID() == m_idTarget)
					//							m_setTargetLocked.push_back(pRole->GetID());
					
					if(nPower >= pRole->GetLife())
						pTarget = pRole;			// continue next cut
					break;

					//if(nLoseLife)
					{
						setRole.push_back(pRole);
						setPower.push_back(nPower);
						setBattleAddInfo.push_back( dwBattleAddInfo );
						// 只锁定一个攻击目标
//						if (pRole->GetID() == m_idTarget)
//							m_setTargetLocked.push_back(pRole->GetID());

						if(nPower >= pRole->GetLife())
							pTarget = pRole;			// continue next cut
						break;
					}
				}
			}
			//-----------------------------------
		}
		if(!pTarget)
			break;
		pos.x = pTarget->GetPosX();
		pos.y = pTarget->GetPosY();
	}

	// 广播法术效果
	CMsgMagicEffect	msg;
	IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), pos.x, pos.y, m_pOwner->GetDir()))
	{
		for(int i = 0; i < setRole.size(); i++)
			msg.AppendRole( setRole[ i ]->GetID(), setPower[ i ],setBattleAddInfo[ i ] );
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
	}


    int addi = 0;//FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
		
	int nPowerSum = 0;
	for (i = 0; i < setRole.size(); i++)
	{
		nPowerSum += setPower[i];
		CNpc* pNpc = NULL;
		int nLifeLost = setPower[i];
		
		nLifeLost = __min(setRole[i]->GetLife(), setPower[i]);
		setRole[i]->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
		
		if(setRole[i]->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
			m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
		
		setRole[i]->BeAttack(HitByMagic(), m_pOwner, setPower[i],addi);
		
		if(!setRole[i]->IsAlive())
			m_pOwner->Kill(setRole[i], GetDieMode());
	}

//	IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
//	if (pStatus)
//	{
//		int nLifeGot = ::CutTrail(0, MulDiv(nPowerSum, pStatus->GetPower(), 100));
//		if (nLifeGot > 0)
//			m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
//	}
//	AwardExp( 0 );

	//? 未增加武器熟练度，也未损耗双方装备。

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessCallTeamMember()
{
	CHECKF(m_pMagic);

	m_setTargetLocked.clear();
	CTeam* pTeam = m_pOwner->GetTeam();
	if(!pTeam)
		return false;
	if (!m_pOwner->GetMap())
		return false;

	// 广播法术效果
	CMsgMagicEffect	msg;
	IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_pOwner->GetID(), 0, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);

	for(int i = 0; i < pTeam->GetMemberAmount(); i++)
	{
		OBJID idTarget = pTeam->GetMemberByIndex(i);
		if(idTarget != ID_NONE && idTarget != m_pOwner->GetID())
		{
			CUser* pTarget = UserManager()->GetUser(idTarget);
			if(pTarget)
			{
				if (!pTarget->IsAlive())
					return false;

//				m_setTargetLocked.push_back(pTarget->GetID());
				pTarget->FlyMap(m_pOwner->GetMap()->GetID(), m_pOwner->GetPosX(), m_pOwner->GetPosY());
			}
			else
				MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_FLY, m_pOwner->GetMap()->GetID(), idTarget, MAKELONG(m_pOwner->GetPosX(), m_pOwner->GetPosY()));
		}
	}

//	AwardExp(0);

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessRecordTransSpell()
{
	CHECKF(m_pMagic);

	m_setTargetLocked.clear();
	// 广播法术效果
	CMsgMagicEffect	msg;
	IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_idTarget, 0, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);

	CItem* pItem = m_pOwner->FindSpaceTransSpell();
	if(pItem)
	{
		OBJID idMap = ID_NONE;
		if (m_pOwner->GetMap())
			idMap = m_pOwner->GetMap()->GetID();
//		pItem->SetInt(ITEMDATA_DATA, idMap, UPDATE_FALSE);
		pItem->SetInt(ITEMDATA_AMOUNT, m_pOwner->GetPosX());
		pItem->SetInt(ITEMDATA_AMOUNTLIMIT, m_pOwner->GetPosY());
/*
		CMsgItemInfo	msg;
		msg.Create(pItem, ITEMINFO_UPDATE);
		m_pOwner->SendMsg(&msg);*/
		CUser* pUser = NULL;
		if (m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
		{
			pUser->SynchroItem(pItem, ITEMINFO_UPDATE);
		}
	}
//	AwardExp(0);

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessTransform()
{
// 	CHECKF(m_pMagic);
// 
// 	m_setTargetLocked.clear();
// 	// 广播法术效果
// 	CMsgMagicEffect	msg;
// 	IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_idTarget, 0, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
// 		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
// 
// 	m_pOwner->Transform(GetPower(), GetPower());
// 
// //	this->AwardExp(0);

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessLayTrap()
{
	return false;//无此功能，屏蔽
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessCallPet()
{
	CHECKF(m_pMagic);
	CUser* pUser = NULL;
	if (!m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
		return false;

	m_setTargetLocked.clear();
	// 广播法术效果
	CMsgMagicEffect	msg;
	IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_pos.x, m_pos.y, m_pOwner->GetDir()))
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);

	pUser->CallPet(GetPower(), m_pos.x, m_pos.y, m_pMagic->GetInt(MAGICDATA_STEP));

//	this->AwardExp(0);

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessDecLife()
{
	CHECKF(m_pMagic);

	m_setTargetLocked.clear();
	IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
	if(!pTarget || !pTarget->IsAlive())
		return false;

//	if(m_pOwner->GetDistance(pTarget->GetPosX(), pTarget->GetPosY()) > m_pMagic->GetInt(MAGICDATA_DISTANCE))
//		return false;

	if (this->IsImmunity(pTarget))
		return false;

	int	nPower	= ::CutTrail(0, CRole::AdjustDataEx(pTarget->GetLife(), GetPower()));

//	m_setTargetLocked.push_back(m_idTarget);
	// 广播法术效果
	CMsgMagicEffect	msg;
	IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_idTarget, nPower, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);

	// set crime status
	CheckCrime(pTarget);

	int addi = 0;//FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
		
	if (nPower)
	{
		int nLifeLost = nPower;
		{
			nLifeLost = __min(pTarget->GetLife(), nPower);
			pTarget->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);

			pTarget->BeAttack(HitByMagic(), m_pOwner, nPower,addi);
		}


		if (!pTarget->IsAlive())
			m_pOwner->Kill(pTarget, GetDieMode());
		
//		IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
//		if (pStatus)
//		{
//			int nLifeGot = ::CutTrail(0, MulDiv(nPower, pStatus->GetPower(), 100));
//			if (nLifeGot > 0)
//				m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
//		}
	}
	
	//? 未增加武器熟练度，也未损耗双方装备。
//	AwardExp( 0 );	//加技能经验.....
	return true;
}

//////////////////////////////////////////////////////////////////////
//地刺
bool CMagic::ProcessGroundSting()
{
	CHECKF(m_pMagic);

	vector<IRole*>	setRole;
	vector<int>		setPower;
	POINT	pos;

	int	nCount = ::CutOverflow(4, m_pMagic->GetMagicLevel() + 2);

	int	nExp = 0;
	int nPowerSum = 0;
	for (int k=0; k<m_nApplyTimes; k++)
	{
		// 搜索目标集
		{
			m_setTargetLocked.clear();
			setRole.clear();
			setPower.clear();

			if(m_pMagic->GetInt(MAGICDATA_GROUND))
			{
				pos.x		= m_pos.x;
				pos.y		= m_pos.y;
			}
			else
			{
				IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
				if(!pTarget || !pTarget->IsAlive())
					return false;

				pos.x		= pTarget->GetPosX();
				pos.y		= pTarget->GetPosY();
			}

			IRole* pRole = NULL;
			switch(nCount)
			{
			case 2:
				if(::RandGet(2) == 0)
				{
					pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x-1, pos.y);
					if(pRole && pRole->IsAlive() && !IsImmunity(pRole))
					{
						m_setTargetLocked.push_back(pRole->GetID());
						setRole.push_back(pRole);
					}
					pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x+1, pos.y);
					if(pRole && pRole->IsAlive() && !IsImmunity(pRole))
					{
						m_setTargetLocked.push_back(pRole->GetID());
						setRole.push_back(pRole);
					}
				}
				else
				{
					pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x, pos.y-1);
					if(pRole && pRole->IsAlive() && !IsImmunity(pRole))
					{
						m_setTargetLocked.push_back(pRole->GetID());
						setRole.push_back(pRole);
					}
					pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x, pos.y+1);
					if(pRole && pRole->IsAlive() && !IsImmunity(pRole))
					{
						m_setTargetLocked.push_back(pRole->GetID());
						setRole.push_back(pRole);
					}
				}
				break;

			case 3:
				{
					int nRand = ::RandGet(4);
					if(nRand != 0)
					{
						pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x-1, pos.y);
						if(pRole && pRole->IsAlive() && !IsImmunity(pRole))
						{
							m_setTargetLocked.push_back(pRole->GetID());
							setRole.push_back(pRole);
						}
					}
					if(nRand != 1)
					{
						pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x+1, pos.y);
						if(pRole && pRole->IsAlive() && !IsImmunity(pRole))
						{
							m_setTargetLocked.push_back(pRole->GetID());
							setRole.push_back(pRole);
						}
					}
					if(nRand != 2)
					{
						pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x, pos.y-1);
						if(pRole && pRole->IsAlive() && !IsImmunity(pRole))
						{
							m_setTargetLocked.push_back(pRole->GetID());
							setRole.push_back(pRole);
						}
					}
					if(nRand != 3)
					{
						pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x, pos.y+1);
						if(pRole && pRole->IsAlive() && !IsImmunity(pRole))
						{
							m_setTargetLocked.push_back(pRole->GetID());
							setRole.push_back(pRole);
						}
					}
				}
				break;

			case 4:
				{
					pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x-1, pos.y);
					if(pRole && pRole->IsAlive() && !IsImmunity(pRole))
					{
						m_setTargetLocked.push_back(pRole->GetID());
						setRole.push_back(pRole);
					}
					pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x+1, pos.y);
					if(pRole && pRole->IsAlive() && !IsImmunity(pRole))
					{
						m_setTargetLocked.push_back(pRole->GetID());
						setRole.push_back(pRole);
					}
					pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x, pos.y-1);
					if(pRole && pRole->IsAlive() && !IsImmunity(pRole))
					{
						m_setTargetLocked.push_back(pRole->GetID());
						setRole.push_back(pRole);
					}
					pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x, pos.y+1);
					if(pRole && pRole->IsAlive() && !IsImmunity(pRole))
					{
						m_setTargetLocked.push_back(pRole->GetID());
						setRole.push_back(pRole);
					}
				}
				break;
			}
		}

	

		// 广播法术效果
		CMsgMagicEffect	msg;
		IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), pos.x, pos.y, m_pOwner->GetDir()))
		{
			for(int i = 0; i < setRole.size(); i++)
			{
				IRole* pRole = setRole[i];
				if (pRole && pRole->IsAlive())
				{
					DWORD dwBattleAddInfo = BATTLEADDINFO::INFO_INVALID;
					int nPower = 0;//CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pRole,GetPower(k),m_pMagic->GetInt( MAGICDATA_PERCENT) );
					setPower.push_back(nPower);
					msg.AppendRole( pRole->GetID(),nPower,dwBattleAddInfo );

				}
				else
					setPower.push_back(0);
			}
			m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
		}

		// set crime status
		CheckCrime(&setRole);

		int addi = 0;//FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
		
		for (int i = 0; i < setRole.size(); i++)
		{
			if (!setRole[i]->IsAlive())
				continue;
			
			nPowerSum += setPower[i];
			CNpc* pNpc = NULL;
			int nLifeLost = setPower[i];
			
			nLifeLost = __min(setRole[i]->GetLife(), setPower[i]);
			setRole[i]->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
			
			if(setRole[i]->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
				m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
			
			setRole[i]->BeAttack(HitByMagic(), m_pOwner, setPower[i],addi);
			
			
		}

// 		if (m_pMagic->GetSTrackAmount() > 0)
// 			AdjustPosByTrack(k);

		// 必须先发轨迹后发kill消息
		for (i = 0; i < setRole.size(); i++)
		{
			if(!setRole[i]->IsAlive())
				m_pOwner->Kill(setRole[i], GetDieMode());
		}
	}

//	IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
//	if (pStatus)
//	{
//		int nLifeGot = ::CutTrail(0, MulDiv(nPowerSum, pStatus->GetPower(), 100));
//		if (nLifeGot > 0)
//			m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
//	}
//	AwardExp( 0 );

	return true;
}

//////////////////////////////////////////////////////////////////////
//复活
bool CMagic::ProcessReborn()
{
	CHECKF (m_pMagic);

//	m_setTargetLocked.clear();
	IRole* pRole = m_pOwner->FindAroundRole(m_idTarget);
	if(!pRole || pRole->IsAlive())
		return false;

	CUser* pTarget = NULL;
	if (!pRole->QueryObj(OBJ_USER, IPP_OF(pTarget)))
		return false;

	CUser* pAttackerUse = NULL;
	if(m_pOwner->QueryObj(OBJ_USER, IPP_OF(pAttackerUse)) )
	{	
	}

	///禁止重生地图,不许复活.softworms-2007-10-05-即使身上有复活卷,但在非复活地图也不能复活
	if (pTarget->GetMap()->IsNotAllowReborn())
		return false;

	int nPower = GetPower();//百分比

	// 广播法术效果
	CMsgMagicEffect	msg;
	IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_idTarget, nPower, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);

	// set crime status
	CheckCrime(pRole);

	CMsgName msg2;
	if( msg2.Create( NAMEACT_REBORN_MSG,"123",m_pOwner->GetID() ) )//此处name未用,随便填写
	{
		pTarget->SendMsg( &msg2 );
		pTarget->SetWaitRebornData( nPower,WAIT_REBORN_TIME );
	}
	
//	bool bChgPos = false;
//	pTarget->Reborn(bChgPos);

//	this->AwardExp(0);
	
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessSorbSoul()
{
	CHECKF (m_pMagic);
	m_setTargetLocked.clear();
	IRole* pRole = m_pOwner->FindAroundRole(m_idTarget);
	if (!pRole || pRole->IsAlive() || !pRole->IsMonster())
		return false;

	int nPower = GetPower();

	// 广播法术效果
	CMsgMagicEffect	msg;
	IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_idTarget, nPower, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);

	// TODO: 吸收尸体的灵魂转换为魔力
	// 通过前面的CheckCondition判断，以下条件判断已经是冗余的
	CMonster* pMonster = NULL;
	if (pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) 
		&& (pMonster->GetMaskData() & MASK_DISABLE_SORB_SOUL) == 0)
	{
		pMonster->SetMaskData(MASK_DISABLE_SORB_SOUL);
		pMonster->DelMonster(true);		// 立刻删除怪物尸体
		pMonster->SetStatus(STATUS_DISAPPEARING);

		m_pOwner->AddAttrib(_USERATTRIB_MANA, nPower, SYNCHRO_TRUE);
	}

//	this->AwardExp(0);

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessSteal()
{
	CHECKF(m_pMagic);
	CHECKF(m_pOwner);

	IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
	if(!pTarget || !pTarget->IsAlive() ||  m_idTarget==m_pOwner->GetID())
		return false;

	CUser* pTargetUser = NULL;
	if (!pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser)))
		return false;

	m_setTargetLocked.clear();
	int	nPower = GetPower();

	// 广播法术效果
	CMsgMagicEffect	msg;
	IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_idTarget, nPower, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);

	// set crime status
	CheckCrime(pTarget);

	if (nPower)
	{
		CItem* pItem = pTargetUser->GetItemByType(1034020);
		if (pItem)
		{
			pTargetUser->SendSysMsg(g_objStrRes.GetStr( 14014 ), pItem->GetStr(ITEMDATA_NAME));//STR_AUTO_USE_ANTISTEAL
			pTargetUser->SendSysMsg(g_objStrRes.GetStr( 14015 ), m_pOwner->GetName());//STR_AVOID_STEAL
			pTargetUser->EraseItem(pItem->GetID(), true);
			return false;
		}
		else
		{
			IUserPackage* pPackage = pTargetUser->QueryPackage();
			if (pPackage)
			{
				int nStealNum = __min(nPower, pPackage->GetAmount());
				for (int i=0; i<nStealNum; i++)
				{
					OBJID idItem = pPackage->RandGetItem();
					CItem* pItem = pTargetUser->PopItem(idItem, true);
					if (pItem)
					{
						pTargetUser->SendSysMsg(g_objStrRes.GetStr( 14016 ), m_pOwner->GetName(), pItem->GetStr(ITEMDATA_NAME));//STR_STOLEN
						if (!m_pOwner->AddItem(pItem, true))
						{
							pItem->DeleteRecord(Database());
							pItem->ReleaseByOwner();
						}
					}
				}
			}
		}
	}

//	this->AwardExp(0);
	
	return true;
}

//////////////////////////////////////////////////////////////////////
//魔雷
bool CMagic::ProcessBlastThunder()
{
	CHECKF(m_pMagic);
	CHECKF(m_pOwner);

	vector<IRole*>		setRole;
	vector<int>			setPower;
	
	POINT	pos;

	m_setTargetLocked.clear();
	IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
	if(!pTarget || !pTarget->IsAlive())
		return false;
	pos.x = m_pOwner->GetPosX();
	pos.y = m_pOwner->GetPosY();

	// 搜索目标集
	while (pTarget)
	{
		if (m_pOwner->GetDistance(pos.x, pos.y) > m_pMagic->GetInt(MAGICDATA_DISTANCE)
			|| !pTarget->IsAlive()
			|| IsImmunity(pTarget))
		{
			break;
		}
		setRole.push_back(pTarget);
		pTarget = NULL;

		int nDirX = ::RandGet(MAX_DIRSIZE);
		int nDirY = ::RandGet(MAX_DIRSIZE);
		POINT pt = {pos.x, pos.y};
		for (int i=0; i<m_pMagic->GetInt(MAGICDATA_DISTANCE); i++)
		{
			pt.x += _DELTA_X[nDirX];
			pt.y += _DELTA_Y[nDirY];
			IRole* pRole = m_pOwner->GetMap()->QueryRoleByPos(pt.x, pt.y);
			if (pRole && pRole->IsAlive() && !IsImmunity(pRole) 
				&& m_pOwner->FindAroundRole(pRole->GetID()))	// 必须在自己的广播区内，这里暂时这么实现，效率不够高 -- zlong
			{
				// 避免重复
				bool bInTargetSet = false;
				for (int k=0; k<setRole.size(); k++)
				{
					if (setRole[k] && setRole[k]->GetID() == pRole->GetID())
					{
						bInTargetSet = true;
						break;
					}
				}
				if (!bInTargetSet)
					pTarget = pRole;
			}
		}
	}	// end of while

	// 广播法术效果，计算伤害
	CMsgMagicEffect	msg;
	IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_pOwner->GetPosX(), m_pOwner->GetPosY(), m_pOwner->GetDir()))
	{
		for(int i = 0; i < setRole.size(); i++)
		{
			IRole* pRole = setRole[i];
			if (pRole && pRole->IsAlive())
			{
				DWORD dwBattleAddInfo = BATTLEADDINFO::INFO_INVALID;
				int nPower = 0;//CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pRole,GetPower(),m_pMagic->GetInt( MAGICDATA_PERCENT) );
				setPower.push_back(nPower);
				msg.AppendRole(pRole->GetID(), nPower,dwBattleAddInfo);
				
			}
			else
				setPower.push_back(0);
		}
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
	}

	CheckCrime(&setRole);

	int addi = 0;//FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
		
	int nPowerSum = 0;
	for (int i = 0; i < setRole.size(); i++)
	{
		if (!setRole[i]->IsAlive())
			continue;
		
		nPowerSum += setPower[i];
		CNpc* pNpc = NULL;
		int nLifeLost = setPower[i];
		
		nLifeLost = __min(setRole[i]->GetLife(), setPower[i]);
		setRole[i]->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
		
		if(setRole[i]->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
			m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
		
		setRole[i]->BeAttack(HitByMagic(), m_pOwner, setPower[i],addi);
		
		
		if(!setRole[i]->IsAlive())
			m_pOwner->Kill(setRole[i], GetDieMode());
	}
	
//	IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
//	if (pStatus)
//	{
//		int nLifeGot = ::CutTrail(0, MulDiv(nPowerSum, pStatus->GetPower(), 100));
//		if (nLifeGot > 0)
//			m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
//	}
	
//	AwardExp( 0 );
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::CollectTargetSetByPos(ROLE_SET& setRole, POINT& pos)
{
	CHECKF (m_pMagic);
	
	int	nRange	=  6; //m_pMagic->GetInt(MAGICDATA_RANGE);
	int	nSize		= nRange*2 + 1;
	int	nBufSize	= nSize * nSize;
	MAPTHING_SET	setTarget;
	
	setRole.clear();
	
	int nDir	= m_pOwner->GetDir();
	if (m_pMagic->GetInt(MAGICDATA_GROUND))
	{
		pos.x		= m_pOwner->GetPosX(); //m_pos.x;
		pos.y		= m_pOwner->GetPosY(); //m_pos.y;		
	}
	else
	{
		IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
		if(!pTarget || !pTarget->IsAlive())
			return false;
		pos.x		= pTarget->GetPosX();
		pos.y		= pTarget->GetPosY();
	}
	
	m_pOwner->GetMap()->CollectMapThing(setTarget, pos, nRange, OBJ_USER|OBJ_MONSTER|OBJ_NPC);
	
	for(int i = ::CutTrail((int)pos.x-nRange, 0); i <= pos.x+nRange && i < m_pOwner->GetMap()->GetWidth(); i++)
	{
		for(int j = ::CutTrail((int)pos.y-nRange, 0); j <= pos.y+nRange && j < m_pOwner->GetMap()->GetHeight(); j++)
		{
			POINT posThis;
			posThis.x	= i;
			posThis.y	= j;
			if (!CUser::IsInCircle(posThis, pos, nRange))
				continue;
			
			int idx = POS2INDEX(posThis.x - pos.x + nRange, posThis.y - pos.y + nRange, nSize, nSize);
			ASSERT(idx>=0 && idx<setTarget.size());
			//20080112修罗:从2维变为3维------
			//if(setTarget[idx])
			for(int zIdx=0;zIdx<setTarget[idx].size();zIdx++)
			{
				IRole* pRole = NULL;
				//if(setTarget[idx]->QueryRole(IPP_OF(pRole)) && pRole->IsAlive() && !IsImmunity(pRole))
				if(setTarget[idx][zIdx]->QueryRole(IPP_OF(pRole)) && pRole->IsAlive() && !IsImmunity(pRole))
				{
					//守卫对白名攻击忽视
					CMonster* pMonster =	NULL;
					if(m_pOwner->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
					{
						if(pMonster && pMonster->IsRealGuard())
						{
							CUser* pUserTarget = NULL;
							pRole->QueryObj(OBJ_USER, IPP_OF(pUserTarget));
							
							if(!pUserTarget || !pUserTarget->IsRedName())
								continue;
						}
					}

					setRole.push_back(pRole);
				}
			}
			//------------------------------
		}
	}
	return (!setRole.empty());	
}

//////////////////////////////////////////////////////////////////////
bool CMagic::CollectTargetSet_Bomb(ROLE_SET& setRole, POINT& pos, int nLockType)
{
	CHECKF (m_pMagic);
	
	int	nRange	= m_pMagic->GetInt(MAGICDATA_RANGE);

	int	nSize		= nRange*2 + 1;
	int	nBufSize	= nSize * nSize;
	MAPTHING_SET	setTarget;

	m_setTargetLocked.clear();
	setRole.clear();

	int nDir	= m_pOwner->GetDir();
	if (m_pMagic->GetInt(MAGICDATA_GROUND))
	{
		if(m_pOwner->GetDistance(m_pos.x,m_pos.y) > m_pMagic->GetInt(MAGICDATA_DISTANCE))
			return false;
		pos.x		= m_pos.x;
		pos.y		= m_pos.y;
	}
	else
	{
		pos.x		= m_pOwner->GetPosX();
		pos.y		= m_pOwner->GetPosY();
	}
	if (m_pMagic->GetInt(MAGICDATA_TYPE)==MAGIC_LY)
	{
		IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
		if(!pTarget || !pTarget->IsAlive())
			return false;
		pos.x		= pTarget->GetPosX();
		pos.y		= pTarget->GetPosY();
	}
//modify code by 林德渊 begin 2011.6.22
	if( m_pMagic->GetInt(MAGICDATA_TYPE) == MAGIC_FJCY_2 )
	{
		IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
		if(!pTarget || !pTarget->IsAlive())
			return false;
		pos.x		= m_pOwner->GetPosX();
		pos.y		= m_pOwner->GetPosY();
		nLockType = LOCK_ALL;
	}
//modify code by 林德渊 end

	BeAttackSort target;
	vector<BeAttackSort> setTargetDistance;
//modify code by 林德渊 begin 2011.6.22
//	m_pOwner->GetMap()->CollectMapThing(setTarget, pos, nRange, OBJ_USER|OBJ_MONSTER|OBJ_NPC);
	if( m_pMagic->GetInt(MAGICDATA_TYPE) == MAGIC_FJCY_2 )
		m_pOwner->GetMap()->CollectMapThing(setTarget, pos, nRange, OBJ_USER);
	else
		m_pOwner->GetMap()->CollectMapThing(setTarget, pos, nRange, OBJ_USER|OBJ_MONSTER|OBJ_NPC);
//modify code by 林德渊 end
    for(int i = ::CutTrail((int)pos.x-nRange, 0); i <= pos.x+nRange && i < m_pOwner->GetMap()->GetWidth(); i++)
	{
		for(int j = ::CutTrail((int)pos.y-nRange, 0); j <= pos.y+nRange && j < m_pOwner->GetMap()->GetHeight(); j++)
		{
			POINT posThis;
			posThis.x	= i;
			posThis.y	= j;
			if (!CUser::IsInCircle(posThis, pos, nRange))
				continue;
//modify code by 林德渊 begin 2011.6.22
			if( m_pMagic->GetInt(MAGICDATA_TYPE) == MAGIC_FJCY_2 )
			{
				if( ((i >= ((int)pos.x - 3)) && (i <= ((int)pos.x + 3))) && ((j >= ((int)pos.y - 3)) && (j <= ((int)pos.y + 3))) )
					continue;
			}
//modify code by 林德渊 end
			
			int idx = POS2INDEX(posThis.x - pos.x + nRange, posThis.y - pos.y + nRange, nSize, nSize);
			ASSERT(idx>=0 && idx<setTarget.size());
			//20080112修罗:从2维变为3维------
			//if(setTarget[idx])	
			for(int zIdx=0;zIdx<setTarget[idx].size();zIdx++)
			{		
				IRole* pRole = NULL;
				//if(setTarget[idx]->QueryRole(IPP_OF(pRole)) && pRole->IsAlive() && !IsImmunity(pRole))
				if(setTarget[idx][zIdx]->QueryRole(IPP_OF(pRole)) 
					&& pRole->IsAlive()
					&& pRole->GetID()!=m_pOwner->GetID()
					&& !IsImmunity(pRole, false))
				{
					//守卫对白名攻击忽视
// 					CMonster* pMonster =	NULL;
// 					if(m_pOwner->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
// 					{
// 						if(pMonster && pMonster->IsRealGuard())
// 						{
// 							CUser* pUserTarget = NULL;
// 							pRole->QueryObj(OBJ_USER, IPP_OF(pUserTarget));
// 
// 							if(!pUserTarget || !pUserTarget->IsRedName())
// 								continue;
// 						}
// 					}
//modify code by 林德渊 begin 2011.6.22
					IStatus* pStatus = pRole->QueryStatus(STATUS_RETORT_ZHF);
					if(pStatus)
						continue;
//modify code by 林德渊 end
					if (pRole->QueryOwnerUser()!=m_pOwner || m_pMagic->GetInt(MAGICDATA_CRIME)==0)//宠物不能攻击主人
					{
						//goto 优化
						//setRole.push_back(pRole);
						target.pRole = pRole;
						target.nDistance = m_pOwner->GetDistance(pRole->GetPosX(), pRole->GetPosY());
						setTargetDistance.push_back(target);
					}
					// 锁定攻击目标
					if ((nLockType == LOCK_ONE && pRole->GetID() == m_idTarget)
						|| nLockType == LOCK_ALL)
					{
						m_setTargetLocked.push_back(pRole->GetID());
					}
				}
			}			
			//--------------------------------
		}
	}
	//[2010-08-02 goto]影响目标上限控制
	int nTargetCount = AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_TARGET),0,m_pMagic->GetMagicLevel(), MAGICLEVADDTYPE_TARGET);
	if (nTargetCount!=0 && setTargetDistance.size()>nTargetCount && m_pMagic->GetInt(MAGICDATA_SORT)!=MAGICSORT_RANDOM_MULTI)
	{
		//goto 优化
// 		vector<BeAttackSort> setTargetDistance;
// 		for (int i=0; i<setRole.size(); i++)
// 		{
// 			BeAttackSort target;
// 			target.pRole = setRole[i];
// 			target.nDistance = m_pOwner->GetDistance(setRole[i]->GetPosX(), setRole[i]->GetPosY());
// 			setTargetDistance.push_back(target);
// 		}
		sort(setTargetDistance.begin(), setTargetDistance.end(), FartherThan);
// 		while(setTargetDistance.size()>nTargetCount)
// 			setTargetDistance.pop_back();
// 		setRole.clear();
	}
	for (i=0 ;i<nTargetCount && i<setTargetDistance.size(); i++)
		setRole.push_back(setTargetDistance[i].pRole);
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::CollectTargetSet_Team(ROLE_SET& setRole)
{
	// 如果是幻兽则需要先获得主人
	CUser* pUser = NULL;
	CMonster* pMonster = NULL;
	if (m_pOwner->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
		pUser = pMonster->QueryOwnerUser();
	else
		m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser));
	if (!pUser)
		return false;

	int		nRange	= m_pMagic->GetInt(MAGICDATA_RANGE);
	POINT	pos = { m_pOwner->GetPosX(), m_pOwner->GetPosY() };

	// 先搜索玩家集合
	vector<CUser*>	setUser;
	setUser.push_back(pUser);

	CTeam* pTeam = pUser->GetTeam();
	if (pTeam)
	{
		for (int i=0; i<pTeam->GetMemberAmount(); i++)
		{
			OBJID idMember = pTeam->GetMemberByIndex(i);
			IRole* pMember = m_pOwner->FindAroundRole(idMember);
			CUser* pMemberUser = NULL;
			if (pMember && pMember->GetID() != pUser->GetID()
				&& pMember->QueryObj(OBJ_USER, IPP_OF(pMemberUser)))
			{
				setUser.push_back(pMemberUser);
			}
		}
	}

	// 然后搜索目标集合—玩家..
	POINT	posThis;
	for (int i=0; i<setUser.size(); i++)
	{
		CUser* pTarget = setUser[i];
		posThis.x = pTarget->GetPosX();
		posThis.y = pTarget->GetPosY();
		if (pTarget->IsAlive() /*&& CUser::IsInCircle(posThis, pos, nRange)*/)
			setRole.push_back(pTarget->QueryRole());
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessMultiAttach()
{
	CHECKF (m_pMagic);
	CHECKF (m_pOwner);

	ROLE_SET	setRole;
	POINT		pos;

	// 搜索目标集
	if (m_pMagic->GetInt(MAGICDATA_CRIME)==1)
	{
		if (!CollectTargetSet_Bomb(setRole, pos, LOCK_NONE))
			return false;
	}
	else if (m_pMagic->GetInt(MAGICDATA_CRIME)==0)//buff 只能给队友加
	{
		pos.x = m_pOwner->GetPosX();
		pos.y = m_pOwner->GetPosY();
		if (!CollectTargetSet_Team(setRole))
			return false;
	}

//	int nPower	= GetPower();
	int nAtt = 0;
	int nPower=AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER),m_pMagic->GetInt(MAGICDATA_TYPE),m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW);///对应POWER进行修正

	int nSecs	= AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_STEP),m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), MAGICLEVADDTYPE_STEPSEC);
	int nTimes	= AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_TIMES),m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_TIMES);
	DWORD nStatus = m_pMagic->GetInt(MAGICDATA_STATUS);

	//------------------------------------------------------------------------
	// 毒云喷射单独处理 [12/13/2007 陈剑飞]
// 	if( nStatus == STATUS_DOT_POISON_ATK )
// 	{
// 		//int nStatusPower = GetPower();
// 		int nMinAtk = ::MulDiv( m_pOwner->GetAtk(),nPower,100 );
// 		int nMaxAtk = ::MulDiv( m_pOwner->GetAtk(),nPower,100 );
// 		
// 		int nRand = rand() % (nMaxAtk - nMinAtk);
// 		nRand += nMinAtk;
// 		nPower = nRand;
// 	}
	
	CMsgMagicEffect	msg;
	IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), pos.x, pos.y, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))

	{
		for (int i=0; i<setRole.size(); i++)
		{
			IRole* pRole = setRole[i];
			if (pRole && pRole->IsAlive())
			{
				msg.AppendRole(pRole->GetID(), 0,BATTLEADDINFO::INFO_INVALID);
			}
		}
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
	}

	CheckCrime(&setRole);

	for (int i=0; i<setRole.size(); i++)
	{
		IRole* pRole = setRole[i];
		if (!pRole || !pRole->IsAlive())
			continue;

		void* pTemp = NULL;
		if(!pRole->QueryObj(OBJ_USER, IPP_OF(pTemp))&& (nStatus == STATUS_LURK ))//给非玩家加潜行.continue
			continue;
//		if(nAtt > 0)
//		{
//			CMsgMagicEffect	msg;
//			IF_OK(msg.Create(pRole->GetID(), 34002, 0, pRole->GetID(), nAtt, pRole->GetDir(),BATTLEADDINFO::INFO_INVALID))
//				pRole->BroadcastRoomMsg(&msg, INCLUDE_SELF);
//				
//			pRole->AddAttrib(_USERATTRIB_LIFE, -1*nAtt, SYNCHRO_TRUE);
//			if(!pRole->IsAlive())
//				pRole->BeKill(m_pOwner);
//		}
		CMonster* pMonster = NULL;
		if( pRole->QueryObj( OBJ_MONSTER,( void** )&pMonster )/* && (nStatus == STATUS_SLEEP || nStatus == STATUS_DREAMING ) */)
		{
			pMonster->AddEnmitySynNPC(m_pOwner->GetID(), 3);
		}

		if (m_pMagic->GetInt(MAGICDATA_TYPE) == 9006)//峨嵋群体加防
		{
			CRole::AttachStatus(pRole, STATUS_PHYSICALDEFENCEUP_NUMBER, nPower, nSecs, nTimes);
			CRole::AttachStatus(pRole, STATUS_MAGICDEFENCEUP_NUMBER, nPower, nSecs, nTimes);
		}
		else
			CRole::AttachStatus(pRole, nStatus, nPower, nSecs, nTimes,0,0,0,m_pOwner->GetID());

	}
//	AwardExp(0);		// false: not award battle exp
	
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessMultiDetach()
{
	CHECKF (m_pMagic);
	CHECKF (m_pOwner);

	int nPower	= 0;
	int nSecs	= m_pMagic->GetInt(MAGICDATA_STEP);
	int nTimes	= m_pMagic->GetInt(MAGICDATA_TIMES);
	DWORD nStatus = m_pMagic->GetInt(MAGICDATA_STATUS);

	// PK场不能使用复活魔法
	if (STATUS_DIE == nStatus)
	{
		if (m_pOwner->GetMap() && m_pOwner->GetMap()->IsPkField())
			return false;
	}

	
	ROLE_SET	setRole;
	POINT		pos;
	
	// 搜索目标集
	if (m_pMagic->GetInt(MAGICDATA_CRIME))
	{
		if (!CollectTargetSet_Bomb(setRole, pos, LOCK_NONE))
			return false;
	}
	else
	{
		pos.x = m_pOwner->GetPosX();
		pos.y = m_pOwner->GetPosY();
		if (!CollectTargetSet_Team(setRole))
			return false;
	}
	
	CMsgMagicEffect	msg;
	IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), pos.x, pos.y, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
	{
		for (int i=0; i<setRole.size(); i++)
		{
			IRole* pRole = setRole[i];
			if (!pRole)
				continue;
			if (!pRole->IsAlive())
			{
				// 对于死亡的对象，只能使用复活魔法，并且只能对玩家使用
				if (STATUS_DIE !=nStatus)
					continue;
				void* pTemp;
				if (!pRole->QueryObj(OBJ_USER, IPP_OF(pTemp)))
					continue;
			}
			msg.AppendRole(pRole->GetID(),0,BATTLEADDINFO::INFO_INVALID);
		}
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
	}
	
	CheckCrime(&setRole);
	
	for (int i=0; i<setRole.size(); i++)
	{
		IRole* pRole = setRole[i];
		if (!pRole || !pRole->IsAlive())
			continue;

// 		if (STATUS_DETACH_ALL == nStatus)
// 		{
// 			CRole::DetachAllStatus(pRole);
// 		}
// 		else if (STATUS_DETACH_BADLY == nStatus)
// 		{
// 			CRole::DetachBadlyStatus(pRole);
// 		}
// 		else if( STATUS_RECOVER == nStatus )//恢复术,去掉中毒，与流血
// 		{
// 			CRole::DetachStatus( pRole,STATUS_DOT_POISONING );
// 			CRole::DetachStatus( pRole,STATUS_DOT_LIFEMISSING );
// 			CRole::DetachStatus( pRole,STATUS_DOT_ENMITY );
// 			CRole::DetachStatus( pRole,21005 );
// 		}
// 		// 群体净化术，去除1个的不利状态 [12/26/2007 陈剑飞]
// 		else if( STATUS_DETACH_SOMEBAD == nStatus )
// 		{
// 			int nCount = m_pMagic->GetInt( MAGICDATA_POWER );
// 			CRole::DetachSomeBadStatus( pRole, nCount );
// 		}
		// ---------------------------------------------------------------------
//		else
//		{
//			IStatus* pStatus = pRole->QueryStatus(nStatus);
//			if (pStatus)
//				pStatus->ChangeData(nPower, nSecs, nTimes);
//		}
		pRole->ClsStatus(nStatus, SYNCHRO_TRUE);
		if (STATUS_DIE == nStatus)
		{
			CUser* pUser = NULL;
			if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
			{
				bool bChgPos = false;
				pUser->Reborn(bChgPos);
			}
		}
	}

//	AwardExp(0);

	return true;
}

//////////////////////////////////////////////////////////////////////
//群体治愈术
bool CMagic::ProcessMultiCure()
{
	CHECKF(m_pMagic);
//	MAPTHING_SET	setTarget;
	ROLE_SET	setRole;
	vector<int>	setPower;
	POINT		pos;




	int	nExp = 0;
//--------------07.4.11王玉波添加----begin------------------------------------
	pos.x = m_pOwner->GetPosX();
	pos.y = m_pOwner->GetPosY();
	if ( !CollectTargetSet_Team( setRole ) )
		return false;
//--------------07.4.11王玉波添加----end------------------------------------
//--------------07.4.11王玉波注释----begin------------------------------------
	// 搜索目标集
//	{
//		int	nRange	= m_pMagic->GetInt(MAGICDATA_RANGE);
//		int	nSize		= nRange * 2 + 1;
//		setTarget.clear();
//		setRole.clear();
//		setPower.clear();
//
//		int nDir	= m_pOwner->GetDir();
//
//		pos.x		= m_pos.x;
//		pos.y		= m_pos.y;
//	
//		m_pOwner->GetMap()->CollectMapThing(setTarget, pos, nRange, OBJ_USER);
//		
//		for(int i = ::CutTrail(pos.x-nRange, 0); i <= pos.x+nRange && i < m_pOwner->GetMap()->GetWidth(); i++)
//		{
//			for(int j = ::CutTrail(pos.y-nRange, 0); j <= pos.y+nRange && j < m_pOwner->GetMap()->GetHeight(); j++)
//			{
//				POINT posThis;
//				posThis.x	= i;
//				posThis.y	= j;
//				int idx = POS2INDEX(posThis.x - pos.x + nRange, posThis.y - pos.y + nRange, nSize, nSize);
//				ASSERT(idx>=0 && idx<setTarget.size());
//				if(setTarget[idx])
//				{
//					IRole* pRole = NULL;
//					if(setTarget[idx]->QueryRole(IPP_OF(pRole)) && pRole->IsAlive()	&& IsImmunity(pRole) && m_pOwner->GetDistance(pRole->QueryMapThing()) <= m_pOwner->GetAttackRange(pRole->GetSizeAdd()))
//							setRole.push_back(pRole);
//				}
//			}
//		}
//	}
//----------------07.4.11王玉波注释----end---------------------------------------------
	for (int k=0; k<m_nApplyTimes; k++)
	{
		// 广播法术效果
		CMsgMagicEffect	msg;
		IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_pos.x, m_pos.y, m_pOwner->GetDir()))
		{
			for(int i = 0; i < setRole.size(); i++)
			{
				IRole* pRole = setRole[i];
				//  [2007-9-29] 朱斌 BUG0001614	 CanRecurit 能否加血
				//	if (pRole && pRole->IsAlive())
				if (pRole && pRole->IsAlive() && HasSameWargameStatus(pRole))
				{
					int nPower = GetPower();
//					if (nPower == ADJUST_FULL)
//						nPower = pRole->GetMaxLife() - pRole->GetLife();
					setPower.push_back(nPower);
					msg.AppendRole(setRole[i]->GetID(), nPower,BATTLEADDINFO::INFO_INVALID);
				}
				else
					setPower.push_back(0);
			}
			m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
		}
		
		for(int i = 0; i < setRole.size(); i++)
		{
			if (!setRole[i]->IsAlive())
				continue;
			
			int nAddLife = ::CutOverflow(setPower[i], ::CutTrail(0, (int)(setRole[i]->GetMaxLife()-setRole[i]->GetLife())));
			
// 			CUser* pTmpUser = NULL;
// 			if( setRole[i]->QueryObj( OBJ_USER,( void** )&pTmpUser) )
// 			{
// 				if(pTmpUser->QueryMagic(MAGIC_GMXN))
// 				{
// 					IMagicData* pData=pTmpUser->QueryMagic(MAGIC_GMXN);
// 					if(pData)
// 					{
// 						nAddLife = nAddLife * (100 + pData->GetInt(MAGICDATA_POWER))/100;
// 						nAddLife = ::CutOverflow(nAddLife, ::CutTrail(0, (int)(pTmpUser->GetMaxLife()-pTmpUser->GetLife())));
// 					}
// 				}
// 			}//12.26 光明信念

			if(nAddLife > 0)
			{
				setRole[i]->AddAttrib(_USERATTRIB_LIFE, nAddLife, SYNCHRO_TRUE);
				CUser* pUser=NULL;
				if(setRole[i]->QueryObj(OBJ_USER, IPP_OF(pUser)))
					pUser->BroadcastTeamLife();
			}
		}
		
// 		if (m_pMagic->GetSTrackAmount() > 0)
// 			AdjustPosByTrack(k);
	}
	
//	this->AwardExp(0);
	
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessStealMoney()
{
	CHECKF (m_pMagic);
	CHECKF (m_pOwner);

	m_setTargetLocked.clear();
	IRole* pRole = m_pOwner->FindAroundRole(m_idTarget);
	if (!(pRole && pRole->IsAlive()))
		return false;

	// 广播法术效果
	CMsgMagicEffect	msg;
	IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_idTarget, 0, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);

	int nMoneyStolen = 0;
	CMonster* pMonster = NULL;
	CUser* pUser = NULL;
	if (pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
	{
		if ((pMonster->GetMaskData() & MASK_DISABLE_STEAL_MONEY) == 0)
		{
			pMonster->SetMaskData(MASK_DISABLE_STEAL_MONEY);

			nMoneyStolen = pMonster->GetDropMoneyAvg()*(::RandGet(50)+1)/100;
		}
	}
	else if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
	{
		nMoneyStolen = pUser->GetMoney()*(::RandGet(10)+1)/100;
		pUser->SpendMoney(nMoneyStolen, true);
	}

	if (nMoneyStolen > 0)
	{
		if (!m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
			pUser = m_pOwner->QueryOwnerUser();
		if (pUser)
			pUser->AddMoney(nMoneyStolen);
	}
//	this->AwardExp(0);
	
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessKO()
{
	CHECKF (m_pMagic);
	CHECKF (m_pOwner);

	IRole* pRole = m_pOwner->FindAroundRole(m_idTarget);
	if (!(pRole && pRole->IsAlive()))
		return false;

	if (pRole->GetLife()*100 / pRole->GetMaxLife() > MAGIC_KO_LIFE_PERCENT)
		return false;

	int nPower = pRole->GetLife();
	int nExp = nPower;

	if (nPower)
	{

			pRole->AddAttrib(_USERATTRIB_LIFE, -1*nPower, SYNCHRO_TRUE);

			pRole->BeAttack(HitByMagic(), m_pOwner, nPower);
			if (!pRole->IsAlive())
				m_pOwner->Kill(pRole, GetDieMode());
	

//		IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
//		if (pStatus)
//		{
//			int nLifeGot = ::CutTrail(0, MulDiv(nPower, pStatus->GetPower(), 100));
//			if (nLifeGot > 0)
//				m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
//		}
	}
//	AwardExp( 0 );	//加技能经验.....

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessEscape()
{
	CHECKF (m_pMagic);
	CHECKF (m_pOwner);

	IRole* pRole = m_pOwner->FindAroundRole(m_idTarget);
	if(!pRole || !pRole->IsAlive())
		return false;


	const int	MIN_TRANS_DISTANCE	= 5;
	const int	MAX_TRANS_DISTANCE	= 20;
	const int	DIV_DIR[]	= {-1, 1};
	const int	MAX_TRY_TIMES	= 20;

	// find random pos
	int nPosX = 0;
	int nPosY = 0;
	int i=0;
	for (i=0; i<MAX_TRY_TIMES; i++)
	{
		int nDivX = ::RandGet(MAX_TRANS_DISTANCE - MIN_TRANS_DISTANCE + 1) + MIN_TRANS_DISTANCE;
		int nDivY = ::RandGet(MAX_TRANS_DISTANCE - MIN_TRANS_DISTANCE + 1) + MIN_TRANS_DISTANCE;
		nPosX = pRole->GetPosX() + DIV_DIR[::RandGet(2)] * nDivX;
		nPosY = pRole->GetPosY() + DIV_DIR[::RandGet(2)] * nDivY;

		if (pRole->GetMap() && pRole->GetMap()->IsStandEnable(nPosX, nPosY))
			break;
	}
	if (i >= MAX_TRY_TIMES)
		return false;

	pRole->ActiveTransPos(nPosX, nPosY);

//	AwardExp(0);

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessPassThrough()
{
	CHECKF(m_pMagic);

	m_setTargetLocked.clear();

	POINT	ptTarget;
	IRole* pTarget	= NULL;
	if(m_pMagic->GetInt(MAGICDATA_GROUND))
	{
		ptTarget.x		= m_pos.x;
		ptTarget.y		= m_pos.y;
	}
	else
	{
		pTarget = m_pOwner->FindAroundRole(m_idTarget);
		if(!pTarget || !pTarget->IsAlive())
			return false;

		if(pTarget->GetDistance(m_pos.x, m_pos.y) > m_pMagic->GetInt(MAGICDATA_DISTANCE)/*m_pOwner->GetAttackRange(pTarget->GetSizeAdd())*/)
			return false;

		if (this->IsImmunity(pTarget))
			return false;

		ptTarget.x		= pTarget->GetPosX();
		ptTarget.y		= pTarget->GetPosY();
	}

	vector<POINT> setPOS;
	
	POINT ptSrc;
	ptSrc.x	= m_pOwner->GetPosX();
	ptSrc.y	= m_pOwner->GetPosY();
	
	DDALine(ptSrc.x, ptSrc.y, ptTarget.x, ptTarget.y, m_pMagic->GetInt(MAGICDATA_RANGE), setPOS);

//	if(m_pOwner->GetDistance(pos.x, pos.y) > m_pMagic->GetInt(MAGICDATA_DISTANCE))
//		return false;

	int	nRange	= m_pMagic->GetInt(MAGICDATA_RANGE);

	int	nPower	= 0;
	DWORD dwBattleAddInfo = BATTLEADDINFO::INFO_INVALID;
	if(pTarget)
	{
		nPower = 0;//CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pTarget,GetPower(),m_pMagic->GetInt( MAGICDATA_PERCENT ) );

	}

//	m_setTargetLocked.push_back(m_idTarget);
	// 广播法术效果
	CMsgMagicEffect	msg;
	IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_pos.x, m_pos.y, m_idTarget, nPower, m_pOwner->GetDir()))
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);

	// set crime status
	if(pTarget)
		CheckCrime(pTarget);
	
	// Find stand pos
	int pos;
	CMapPtr pMap = m_pOwner->GetMap();
	CHECKF(pMap);
	for(pos = setPOS.size()-1; pos>=0; pos--)
	{
		if(pMap->IsStandEnable(setPOS[pos].x, setPOS[pos].y))
			break;
	}

	// Not find.
	if(pos == -1)
		return false;

	// move!!! 攻击方需要锁定坐标
	m_pOwner->ProcessOnMove(MOVEMODE_JUMPMAGICATTCK);
	m_pOwner->ActiveTransPos(setPOS[pos].x, setPOS[pos].y);

	if (nPower && pTarget)
	{
		CNpc* pNpc = NULL;
		int nLifeLost = nPower;
		
		nLifeLost = __min(pTarget->GetLife(), nPower);
		pTarget->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
		
		if(pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
			m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
		
		int addi = 0;//FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
		pTarget->BeAttack(HitByMagic(), m_pOwner, nPower);
		
		
		
		if (!pTarget->IsAlive())
			m_pOwner->Kill(pTarget, GetDieMode());
		
//		IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
//		if (pStatus)
//		{
//			int nLifeGot = ::CutTrail(0, MulDiv(nPower, pStatus->GetPower(), 100));
//			if (nLifeGot > 0)
//				m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
//		}
	}
	
	//? 未增加武器熟练度，也未损耗双方装备。
//	AwardExp( 0 );	//加技能经验.....
	return true;
}
//技能：时空门
//07.4.16王玉波第一次修改

bool CMagic::Process_SpaceTimeDoor()
{
	return false;//无此功能，屏蔽
}

bool CMagic::Process_StopTrap()
{
	return false;//无此功能，屏蔽
}

//王玉波
//瞬间移动技能
//玩家瞬间移动到屏幕上任意一点（可以解除定身、减速、战斗不能的debuff）
bool CMagic::ProcessDimensionDoor()
{
	CHECKF ( m_pMagic );
	CHECKF ( m_pOwner );
	//assert m_pOwner is a User
	CUser* pUser = NULL;
	if( !m_pOwner->QueryObj( OBJ_USER,( void** )&pUser ) )
	{
		return false;
	}

	CMapPtr pMap = m_pOwner->GetMap();
	if (pMap)
	{
		int nDistance = AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER), 0, m_pMagic->GetMagicLevel(), MAGICLEVADDTYPE_POW);//移动的距离
		//策划要求，改成飞到玩家指定的地点
// 		int nDir = m_pOwner->GetDir();//人物方向
// 		int nPosX = m_pOwner->GetPosX();
// 		int nPosY = m_pOwner->GetPosY();
// 		while (pMap->IsStandEnable(nPosX,nPosY) && m_pOwner->GetDistance(nPosX,nPosY)<=nDistance)
// 		{
// 			nPosX += _DELTA_X[nDir];
// 			nPosY += _DELTA_Y[nDir];
// 		}
// 		m_pos.x = nPosX - _DELTA_X[nDir];
// 		m_pos.y = nPosY - _DELTA_Y[nDir];
// 
// 		if (m_pOwner->GetDistance(m_pos.x,m_pos.y)==0)
// 		{
// 			m_pOwner->SendSysMsg(_TXTATR_TOPSHOW, "被阻挡，无法瞬移");
// 			return false;
// 		}
		if (m_pOwner->GetDistance(m_pos.x,m_pos.y)>nDistance)
		{
			pUser->SendSysMsg(_TXTATR_TOPSHOW, "距离太远");//STR_CAN_STAND
			return false;
		}
		if (!pMap->IsStandEnable(m_pos.x, m_pos.y))
		{
			pUser->SendSysMsg(_TXTATR_TOPSHOW, "目标位置不允许站立");//STR_CAN_STAND
			return false;
		}
		IMapThing* pMapThing = NULL;
		//20070321修罗:返回值改为指针,检查安全性
		CGameBlock* gameBlockTemp = pMap->QueryBlock( m_pos.x,m_pos.y );
		CHECKF(gameBlockTemp);
		IThingSet* pSet = gameBlockTemp->QuerySet();
		//-------------------------------------
		//如果有NPC在此位置，就不能飞
// 		for( int i = 0;i < pSet->GetAmount();i++ )
// 		{
// 			IMapThing* pTheMapThing = pSet->GetObjByIndex( i );
// 			if ( pTheMapThing && 									
// 				pTheMapThing->GetObjType() == OBJ_NPC &&
// 				pTheMapThing->GetPosX() == m_pos.x && pTheMapThing->GetPosY() == m_pos.y )
// 			{
// 				pMapThing = pTheMapThing;
// 				break;		
// 			}
// 		}
		if ( !pMapThing )
		{
			//	pUser->FlyMap( ID_NONE,m_pos.x,m_pos.y );
			// 广播法术效果
			CMsgMagicEffect	msg;
			IF_OK( msg.CreateByPos( pUser->GetID(),m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel(),m_pos.x,m_pos.y,pUser->GetDir() ) )
				pUser->BroadcastRoomMsg(&msg, INCLUDE_SELF);
			
			pUser->ActiveTransPos( m_pos.x,m_pos.y );
		
			//CRole::DetachStatus( m_pOwner,STATUS_PALSY );
			//CRole::DetachStatus( m_pOwner,STATUS_MOVESPEEDDOWN_PERCENT );
			
		}
		else
		{
			pUser->SendSysMsg(_TXTATR_TOPSHOW, "目标位置不允许站立");//STR_CAN_STAND
			return false;
		}
	}
	else
		return false;
	
//	AwardExp( 0 );	//加技能经验.....

	return true;
}
//07.4.16
//王玉波
//处理需要有前置条件的技能
bool CMagic::ProcessPreCondition()
{
	CHECKF(m_pMagic);
	CHECKF(m_pOwner);

	IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
	if(!pTarget || !pTarget->IsAlive())
		return false;

	if (this->IsImmunity(pTarget))
		return false;

	CUser* pAtkUser = NULL;
	if( m_pOwner->QueryObj( OBJ_USER,( void** )&pAtkUser) )//某USER施放技能
	{
		if( !pAtkUser->IsArrowPass( pTarget->GetPosX(),pTarget->GetPosY() ) )
		{
			pAtkUser->SendSysMsg( g_objStrRes.GetStr( 14098 ) );//"不能施放魔法,阻挡"
			return false;
		}
	}
	DWORD dwStatus	= m_pMagic->GetInt( MAGICDATA_STATUS );
	if( ID_NONE == dwStatus )
		return false;
	if( !pTarget->QueryStatus( dwStatus ) )//前置
	{
		if( pAtkUser )
		{
			pAtkUser->SendSysMsg( g_objStrRes.GetStr( 14099 ) );//"目标无指定状态！"

		}
		return false;
	}


	for (int k=0; k<m_nApplyTimes; k++)
	{
		DWORD dwBattleAddInfo = BATTLEADDINFO::INFO_INVALID;
		int	nPower = CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pTarget,GetPower(k));

		// 广播法术效果
		CMsgMagicEffect	msg;
		IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_idTarget, nPower, m_pOwner->GetDir(),dwBattleAddInfo))
			m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);

		// set crime status
		CheckCrime(pTarget);

		int addi = 0;//FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
			
		if (nPower)
		{
			int nLifeLost = nPower;
			nLifeLost = __min(pTarget->GetLife(), nPower);
			pTarget->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
			int addi = 0;//FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
			pTarget->BeAttack(HitByMagic(), m_pOwner, nPower,addi);
			//--------------------------------------------------------------------------------
			// 精通抹煞 [12/27/2007 陈剑飞]
// 			if( pAtkUser->QueryMagic( MAGIC_JTMS ) )
// 			{
// 				int nType = m_pMagic->GetInt( MAGICDATA_TYPE );
// 				IMagicData* pData = pAtkUser->QueryMagic(MAGIC_JTMS);
// 				if( pData && nType == MAGIC_MOS2 )
// 				{
// 					int nPow = pData->GetInt(MAGICDATA_POWER);	
// 					int AddLife = (nLifeLost*( 100 + RandGet(nPow) ))/100;
// 					m_pOwner->AddAttrib( _USERATTRIB_LIFE,AddLife,SYNCHRO_TRUE );
// 				
// 				}
// 			}
			//----------------------------------------------------------------------------------
		}

// 		if (m_pMagic->GetSTrackAmount() > 0)
// 			AdjustPosByTrack(k);
	}

	if (!pTarget->IsAlive())
	{
		m_pOwner->Kill(pTarget, GetDieMode());
	}
	else
	{
		CRole::DetachStatus( pTarget,dwStatus );
// 		if (7011 == m_pMagic->GetInt( MAGICDATA_TYPE ) 
// 			|| 7013 == m_pMagic->GetInt( MAGICDATA_TYPE )
// 			//[游途道标 2008/9/12]添加7022魔法在目标有34006状态的情况下去掉其原有状态添加战斗不能的状态.
// 			|| 7022 == m_pMagic->GetInt( MAGICDATA_TYPE)) 
// 		{
// 			int nPower	= GetPower();
// 			int nSecs	= m_pMagic->GetInt(MAGICDATA_STEP);
// 			int	nTimes	= m_pMagic->GetInt(MAGICDATA_TIMES);
// 
// 			CRole::AttachStatus(pTarget, STATUS_DREAMING, nPower, nSecs, nTimes);
// 	
// 		}
		
	}

//	AwardExp( 0 );	
	return true;
}

//---------------------------------------
bool CMagic::ProcessSummon()//20080202修罗:召唤
{
	return true;
}

//satan 单嘲技能的处理
bool CMagic::MyProcessDo()
{
	CHECKF(m_pMagic);

	IRole* pRole = m_pOwner->FindAroundRole(m_idTarget);
	if(!pRole || !pRole->IsAlive())
		return false;

    if(!HasSameWargameStatus(pRole))
		return false;

	if( m_pMagic->GetInt( MAGICDATA_CRIME ) )//debuff
	{
		if( this->IsImmunity( pRole ) )
			return false;
	}
	else
	{
		if( m_pOwner != pRole && m_pOwner->IsAtkable( pRole ) )
		{
			return false;
		}
		CMonster* pMonster = NULL;
		if( pRole->QueryObj( OBJ_MONSTER,( void** )&pMonster ) && pMonster->IsGuard() && pMonster->IsRealGuard())
		{
			return false;
		}
	}

	int   nPower	= 0;
	int   nSecs	= m_pMagic->GetInt(MAGICDATA_STEP);
	int   nTimes	= m_pMagic->GetInt(MAGICDATA_TIMES);
	DWORD nStatus	= m_pMagic->GetInt(MAGICDATA_STATUS);


	CMsgMagicEffect	msg;
	if(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_idTarget, 0, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);	
	
	CheckCrime(pRole);
	
	void* pTemp;
	if(  !pRole->QueryObj( OBJ_USER, IPP_OF( pTemp ) ) && ( nStatus == STATUS_LURK  ) )//给非玩家加潜行,return false
		return false;


	CRole::AttachStatus(pRole, nStatus, nPower, nSecs, nTimes);
	int addi = 0;//FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
	pRole->BeEnmityAttack(HitByMagic(), m_pOwner,addi);//此函数处理嘲讽

	CUser* hitUser;
	if( pRole->QueryObj( OBJ_USER, (void**)&hitUser ) )
	{
		CMsgMagicInfo msgInfo;
		if (msgInfo.Create(_MAGICINFO_SNEER, m_pOwner->GetID()))
			hitUser->SendMsg(&msgInfo);
	}

//	AwardExp( 0 );
	return true;
}
//[2010-08-05 goto]击退技能
bool CMagic::ProcessAtkBack()
{
	CHECKF(m_pMagic);
	CHECKF(m_pOwner);
	
	IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
	if(!pTarget || !pTarget->IsAlive() || m_idTarget==m_pOwner->GetID())
		return false;
	
// 	CUser* pAtkUser = NULL;
// 	if( m_pOwner->QueryObj( OBJ_USER,( void** )&pAtkUser) )//某USER施放技能
// 	{
// 		if( !pAtkUser->IsArrowPass( pTarget->GetPosX(),pTarget->GetPosY() ) )
// 		{
// 			pAtkUser->SendSysMsg( g_objStrRes.GetStr( 14097 ) );//"不能施放魔法,阻挡"
// 			return false;
// 		}
// 	}
	
// 	if (m_pMagic->GetSTrackAmount() > 0)
// 		LockTarget(true);

	bool bBack = true;
	
	for (int k=0; k<m_nApplyTimes; k++)
	{
		DWORD dwBattleAddInfo = BATTLEADDINFO::INFO_INVALID;//无效的

		if ( this->IsImmunity(pTarget) )
		{
// 			CMonster* pMonster = NULL;
// 			if( pTarget->QueryObj( OBJ_MONSTER,( void**)&pMonster ) )//不能对本族守卫攻击
// 			{
// 				if( pMonster->IsGuard()  || pMonster->IsRealGuard() )
// 				{
// 					return false;
// 				}
// 			}
// 			CMsgMagicEffect	msg;
// 			IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_idTarget, ESPECIAL_POWER, m_pOwner->GetDir(),BATTLEADDINFO::INFO_ATK_INVALID))
// 				m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
			
			return false;// [6/12/2007]陈剑飞
		}
		//计算后退坐标
		CMapPtr pMap = pTarget->GetMap();
		if (pMap)
		{
			int nDistance = AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER2), 0, m_pMagic->GetMagicLevel(), MAGICLEVADDTYPE_POW2);//移动的距离
			int nDir = GetDirByPos(m_pOwner->GetPosX(), m_pOwner->GetPosY(), pTarget->GetPosX(), pTarget->GetPosY());//人物方向,往攻击者的方向后退
			int nPosX = pTarget->GetPosX();
			int nPosY = pTarget->GetPosY();
			if (pTarget->IsUser())//如果是用户要考虑地图阻挡
			{
				while (pMap->IsStandEnable(nPosX,nPosY) && pTarget->GetDistance(nPosX,nPosY)<=nDistance)
				{
					nPosX += _DELTA_X[nDir];
					nPosY += _DELTA_Y[nDir];
				}
			}
			else
			{
				while (pMap->IsValidPoint(nPosX,nPosY) && pTarget->GetDistance(nPosX,nPosY)<=nDistance && nPosX>0 && nPosY>0)
				{
					nPosX += _DELTA_X[nDir];
					nPosY += _DELTA_Y[nDir];
				}
			}
			
			m_pos.x = nPosX - _DELTA_X[nDir];
			m_pos.y = nPosY - _DELTA_Y[nDir];
			
			if (pTarget->GetDistance(m_pos.x,m_pos.y)==0)
			{
				bBack = false;
			}
		}
		if (!bBack)//无点可退，把坐标还原
		{
			m_pos.x = pTarget->GetPosX();
			m_pos.y = pTarget->GetPosY();
		}
		int nPower1 = AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER),m_pMagic->GetInt(MAGICDATA_TYPE),m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW);
		int nPower = CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pTarget,nPower1,0,0,m_pMagic->GetInt(MAGICDATA_ATTACK_TYPE));
		
		CheckCrime(pTarget);
		
		if (nPower)
		{
			int nLifeLost = nPower;
			nLifeLost = __min(pTarget->GetLife(), nPower);
			CUser* pUser = NULL;
			pTarget->QueryObj(OBJ_USER, IPP_OF(pUser));
			if(pUser)
			{
				nLifeLost = pUser->PVPEmendDamage(nLifeLost, m_pOwner->GetID());
			}
//modify code by 林德渊 begin 2011.6.22
			IStatus* pStatus = pTarget->QueryStatus(STATUS_RETORT_LGWS);
			if(pStatus)
			{
				m_pOwner->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
				m_pOwner->BeAttack(HitByMagic(),pTarget,nPower,0,true);
			}
//modify code by 林德渊 end
			pTarget->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
			int addi = 0;//FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
			pTarget->BeAttack(HitByMagic(), m_pOwner, nPower,addi);					
		}
		// 广播法术效果
		CMsgMagicEffect msg;
		IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_pos.x, m_pos.y, m_pOwner->GetDir()))
		{
			if (msg.AppendRole(m_idTarget, nPower, dwBattleAddInfo))
				m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
		}
	}
	
// 	m_pos.x	= pTarget->GetPosX();
// 	m_pos.y = pTarget->GetPosY();
	
	if (!pTarget->IsAlive())
	{
		m_pOwner->Kill(pTarget, GetDieMode());
		return true;//目标死亡就不再后退了
	}
	if (bBack)
		pTarget->ActiveTransPos( m_pos.x,m_pos.y );
	return true;
}
//[2010-08-11 goto] 90 回城
bool CMagic::ProcessBackCity()
{
	CHECKF(m_pMagic);
	CHECKF(m_pOwner);

	CUser* pUser = NULL;
	if(!m_pOwner->QueryObj( OBJ_USER,( void** )&pUser))
		return false;

	if (!pUser->GetMap())
	{
		return false;
	}
	if (pUser->GetMap()->IsPkGameMap())
	{
		pUser->SendSysMsg("该地图不能使用回城！");
		return false;
	}
	if (!pUser || !pUser->IsAlive())
		return false;
	if (pUser->GetProfession() & MALE_NEWHAND)
	{
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "你还没有加入门派");
		return false;
	}
	if (pUser->QueryBooth())
	{
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "请先收摊");
		return false;
	}
	if (pUser->GetAttackTargetID()!=ID_NONE)
	{
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "战斗中不能回城");
		return false;
	}
	if (pUser->GetMap()->IsDynaMap() && g_SpecilLine ==1)
	{
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "该地图不能使用回城！");
		return false;
	}
	CMsgMagicEffect	msg;
	IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_pOwner->GetID(), 0, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
	if (m_pMagic->GetInt(MAGICDATA_TYPE)!=MAGIC_BACKSYN)
	{
		if (pUser->FlyMap(1001, 90, 49)==-1)
		return false;
	}

	return true;
}
//
//[2010-08-11 goto] 91 回帮
bool CMagic::ProcessBackSyn()
{
	CHECKF(m_pMagic);
	CHECKF(m_pOwner);
	CUser* pUser = NULL;
	if(!m_pOwner->QueryObj( OBJ_USER,( void** )&pUser))
		return false;
	
	if (pUser->GetProfession() & MALE_NEWHAND || pUser->GetSynID()==0)
	{
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "你还没有加入帮派");
		return false;
	}

	
	if (!pUser->GetMap())
	{
		return false;
	}
	if(pUser->GetMap()==pUser->GetSynMap())
	{
		pUser->SendSysMsg("你已经在帮派中");
		return false;
	}
	if (pUser->GetMap()->GetDocID()==MAP_PRISON)
	{
		pUser->SendSysMsg("监狱中不能快速回帮");
		return false;
	}
	if(pUser->GetMap()->GetDocID()==1026)
	{
		pUser->SendSysMsg("地府中不能快速回帮");
		return false;
	}
	if (pUser->GetMap()->IsPkGameMap())
	{
		pUser->SendSysMsg("该地图不能使用回帮！");
		return false;
	}
	if (!pUser || !pUser->IsAlive())
		return false;
	if (pUser->QueryBooth())
	{
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "请先收摊");
		return false;
	}
	if (pUser->GetAttackTargetID()!=ID_NONE)
	{
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "战斗中不能回帮");
		return false;
	}
	if (pUser->GetMap()->IsDynaMap())
	{
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "该地图不能使用回帮！");
		return false;
	}
	CMsgMagicEffect	msg;
	IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_pOwner->GetID(), 0, m_pOwner->GetDir(),BATTLEADDINFO::INFO_INVALID))
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
	if (m_pMagic->GetInt(MAGICDATA_TYPE)==MAGIC_BACKSYN)
	{
		if (!pUser->EnterSynMap(70,100))
			return false;
	}
	
	return true;
}
//goto 67 随机攻击多个目标N次,可能会攻击同一个目标多次
bool CMagic::ProcessRandomMultiAtk()
{
	CHECKF(m_pMagic);

	IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
	if(!pTarget || !pTarget->IsAlive() || this->IsImmunity(pTarget))
		return false;

	ROLE_SET		setRole;
	ROLE_SET		setBeAtkRole;
	vector<int>		setPower;
	POINT	pos;

	int nPowerSum = 0;
	
	pos.x = m_pOwner->GetPosX();
	pos.y = m_pOwner->GetPosY();
	// 搜索目标集
	setRole.clear();
	setPower.clear();
	setBeAtkRole.clear();
	CollectTargetSet_Bomb(setRole, pos, LOCK_ONE);
	if (setRole.size()<=0)
	{
		m_pOwner->SendSysMsg(_TXTATR_TOPSHOW, "攻击范围内没有可攻击的目标");
		return false;
	}

	// 锁定目标
// 	if (m_pMagic->GetSTrackAmount() > 0)
// 		LockTarget(true);	// 可以重复调用

	int nAtkTimes = AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_TARGET),0,m_pMagic->GetMagicLevel(), MAGICLEVADDTYPE_TARGET);
	// 广播法术效果，计算伤害
	bool bCreate = false;
	CMsgMagicEffect	msg;
	IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), pos.x, pos.y, m_pOwner->GetDir()))
	{
		for(int i = 0; i < nAtkTimes;)//攻击次数
		{
			short sTargetIndex = RandGet(setRole.size());
			IRole* pRole = setRole[sTargetIndex];
			if (i==0)//第一下打选中的目标
				pRole = pTarget;
			if (!pRole || !pRole->IsAlive())
			{
				if (i==0)//防止第一个目标有问题时出现死循环
				{
					i++;
					continue;
				}
				vector<IRole*>::iterator it = setRole.begin()+sTargetIndex;
				setRole.erase(it);
				if (setRole.size()<=0)
					break;
				else
					continue;
			}
			
			DWORD dwBattleAddInfo = 0;//无效
			int nPower = 0;
			int nPower1=AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER),m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW);///对应POWER进行修正
			int nPower2=AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER2),m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW2);
			nPower=CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pRole,nPower1,0,nPower2,m_pMagic->GetInt(MAGICDATA_ATTACK_TYPE));
			setBeAtkRole.push_back(pRole);
			setPower.push_back(nPower);
			i++;//攻击到有效目标时才增加计数器

			msg.AppendRole( pRole->GetID(),nPower,dwBattleAddInfo );
		}
		//m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
		bCreate = true;
	}

	// set crime status
	CheckCrime(&setBeAtkRole);
	if (setBeAtkRole.size()!=setPower.size())
		return false;
	// 伤害
	for (int i = 0; i < setBeAtkRole.size(); i++)
	{
		if (!setBeAtkRole[i]->IsAlive())
			continue;
		
		nPowerSum += setPower[i];
		
		int nLifeLost = setPower[i];
		
		nLifeLost = __min(setBeAtkRole[i]->GetLife(), setPower[i]);

		CUser* pUser = NULL;
		setBeAtkRole[i]->QueryObj(OBJ_USER, IPP_OF(pUser));
		if(pUser)
		{
			nLifeLost = pUser->PVPEmendDamage(nLifeLost, m_pOwner->GetID());
		}
//modify code by 林德渊 begin 2011.6.22
		IStatus* pStatus = setBeAtkRole[i]->QueryStatus(STATUS_RETORT_LGWS);
		if(pStatus)
		{
			m_pOwner->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
			m_pOwner->BeAttack(HitByMagic(),setBeAtkRole[i],setPower[i],0,true);
		}
//modify code by 林德渊 end
		setBeAtkRole[i]->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
		
		setBeAtkRole[i]->BeAttack(HitByMagic(), m_pOwner, setPower[i],0);
		
	}
	if (bCreate)
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
	for (i = 0; i < setBeAtkRole.size(); i++)
	{
		if(setBeAtkRole[i] && !setBeAtkRole[i]->IsAlive())
		{
			CMonster* pMonster = NULL;
			if(setBeAtkRole[i]->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))//宠物和怪物先死，因为人死的时候会收回宠物，这里的宠物指针就成了野指针
			{
				m_pOwner->Kill(setBeAtkRole[i], GetDieMode());
				setBeAtkRole[i] = NULL;//置为空，下一轮就不再处理
			}
		}
	}
	for (i = 0; i < setBeAtkRole.size(); i++)
	{
		if(setBeAtkRole[i] && !setBeAtkRole[i]->IsAlive())
		{
			m_pOwner->Kill(setBeAtkRole[i], GetDieMode());
		}
	}
	return true;
}
//goto 68 连击
bool CMagic::ProcessTimesAtk()
{
	CHECKF(m_pMagic);
	CHECKF(m_pOwner);

	IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
	if(!pTarget || !pTarget->IsAlive() || m_idTarget==m_pOwner->GetID())
		return false;

// 	CUser* pAtkUser = NULL;
// 	if( m_pOwner->QueryObj( OBJ_USER,( void** )&pAtkUser) )//某USER施放技能
// 	{
// 		if( !pAtkUser->IsArrowPass( pTarget->GetPosX(),pTarget->GetPosY() ) )
// 		{
// 
// 			pAtkUser->SendSysMsg( g_objStrRes.GetStr( 14097 ) );//"不能施放魔法,阻挡"
// 			return false;
// 		}
// 	}
	if ( this->IsImmunity(pTarget) )			
		return false;//  [6/12/2007]陈剑飞
// 	if (m_pMagic->GetSTrackAmount() > 0)
// 		LockTarget(true);

	DWORD dwBattleAddInfo = BATTLEADDINFO::INFO_INVALID;//无效的

	int nPower1 = AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER),m_pMagic->GetInt(MAGICDATA_TYPE),m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW);
	int nPower2 = 0;
	if (RandGet(100)<m_pMagic->GetInt(MAGICDATA_STATUS_CHANCE))//附加属性的机率
		nPower2=AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_POWER2),m_pMagic->GetInt(MAGICDATA_TYPE),m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_POW2);

	int nPower = CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pTarget,nPower1,0,nPower2,m_pMagic->GetInt(MAGICDATA_ATTACK_TYPE));

	int nActiveTimes = AdjustMagicPower(m_pMagic->GetInt(MAGICDATA_TIMES),m_pMagic->GetInt(MAGICDATA_TYPE),m_pMagic->GetMagicLevel(),MAGICLEVADDTYPE_TIMES);
	// 广播法术效果
	bool bCreate = false;
	CMsgMagicEffect	msg;
	IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetMagicLevel(), m_idTarget, nPower, m_pOwner->GetDir(),dwBattleAddInfo))
	{
		for(int i=0;i<nActiveTimes-1;i++)
		{
			int nPowerTemp = CBattleSystem::CalcDamage( dwBattleAddInfo,GetAtkType(),m_pOwner,pTarget,nPower1,0,nPower2,m_pMagic->GetInt(MAGICDATA_ATTACK_TYPE));
			msg.AppendRole(m_idTarget, nPowerTemp, dwBattleAddInfo);
			nPower+=nPowerTemp;
		}
		//m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
		bCreate = true;
	}

	CheckCrime(pTarget);

	if (nPower)
	{
		int nLifeLost = nPower;
		nLifeLost = __min(pTarget->GetLife(), nPower);
		CUser* pUser = NULL;
		pTarget->QueryObj(OBJ_USER, IPP_OF(pUser));
		if(pUser)
		{
			nLifeLost = pUser->PVPEmendDamage(nLifeLost, m_pOwner->GetID());
		}
//modify code by 林德渊 begin 2011.6.22
		IStatus* pStatus = pTarget->QueryStatus(STATUS_RETORT_LGWS);
		if(pStatus)
		{
			m_pOwner->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
			m_pOwner->BeAttack(HitByMagic(),pTarget,nPower,0,true);
		}
//modify code by 林德渊 end
		pTarget->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
		int addi = 0;//FindMagicType(m_pMagic->GetInt( MAGICDATA_TYPE ),m_pMagic->GetMagicLevel())->GetInt(MAGICTYPEDATA_ENMITY_ADDI);
		pTarget->BeAttack(HitByMagic(), m_pOwner, nPower,addi);					
	}
	if (bCreate)
		m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
	m_pos.x	= pTarget->GetPosX();
	m_pos.y = pTarget->GetPosY();

	if (!pTarget->IsAlive())
		m_pOwner->Kill(pTarget, GetDieMode());

	return true;
}
int CMagic::ProcessSynMagic(CUser* pUser, IRole* pTargeter, int nSynRank)//帮派技能,在BattleSystem中调用
{
	CHECKF(pUser);
	CHECKF(pTargeter);
	if (nSynRank==RANK_NONE)
		return 0;

	int nDamageAdd = 0;
	int nMagicType = 0;
	int nStep = 0;
	DEBUG_TRY
	IMagicData* pData = NULL;
	if (nSynRank%10==1 || nSynRank>=90)//青龙堂
	{
		nMagicType = 2102;
		pData = FindMagic(nMagicType);
		nStep = 1;
		if (pData && ::RandGet(100)<2)
		{
			nDamageAdd += AdjustMagicPower(pData->GetInt(MAGICDATA_POWER),pData->GetInt(MAGICDATA_TYPE),pData->GetMagicLevel(),MAGICLEVADDTYPE_POW);
			nStep = 2;
			int nStatPower = AdjustMagicPower(pData->GetInt(MAGICDATA_STATUS_POWER),pData->GetInt(MAGICDATA_TYPE),pData->GetMagicLevel(),MAGICLEVADDTYPE_STATUS_POW);
			nStep = 3;
			int nSecs = AdjustMagicPower(pData->GetInt(MAGICDATA_STEP),pData->GetInt(MAGICDATA_TYPE), pData->GetMagicLevel(), MAGICLEVADDTYPE_STEPSEC);
			nStep = 4;
			CRole::AttachStatus(pTargeter,pData->GetInt(MAGICDATA_STATUS), nStatPower, nSecs, 0, 0, 0, 0, pUser->GetID());
			//return nDamageAdd;//有多个堂的帮派技能一次也只给一种效果,所以要return
		}
	}
	if (nSynRank%10==2 || nSynRank>=90)//白虎堂
	{
		nMagicType = 2202;
		nStep = 5;
		pData = FindMagic(nMagicType);
		nStep = 6;
		if (pData && ::RandGet(100)<2)
		{
			nDamageAdd += AdjustMagicPower(pData->GetInt(MAGICDATA_POWER),pData->GetInt(MAGICDATA_TYPE),pData->GetMagicLevel(),MAGICLEVADDTYPE_POW);
			nStep = 7;
			int nAddSp = AdjustMagicPower(pData->GetInt(MAGICDATA_POWER2),pData->GetInt(MAGICDATA_TYPE),pData->GetMagicLevel(),MAGICLEVADDTYPE_POW2);
			nStep = 8;
			pUser->AddAttrib(_USERATTRIB_SP, nAddSp, SYNCHRO_TRUE);
			//return nDamageAdd;
		}
	}
	if (nSynRank%10==3 || nSynRank>=90)//玄武堂
	{
		nMagicType = 2302;
		nStep = 9;
		pData = FindMagic(nMagicType);
		nStep = 10;
		if (pData && ::RandGet(100)<2)
		{
			int nStatPower = AdjustMagicPower(pData->GetInt(MAGICDATA_POWER),pData->GetInt(MAGICDATA_TYPE),pData->GetMagicLevel(),MAGICLEVADDTYPE_POW);
			nStep = 11;
			int nSecs = AdjustMagicPower(pData->GetInt(MAGICDATA_STEP),pData->GetInt(MAGICDATA_TYPE), pData->GetMagicLevel(), MAGICLEVADDTYPE_STEPSEC);
			nStep = 12;
			CRole::AttachStatus(pUser,pData->GetInt(MAGICDATA_STATUS), nStatPower, nSecs, 0, 0, 0, 0, pUser->GetID());
			//return nDamageAdd;
		}
	}
	if (nSynRank%10==4 || nSynRank>=90)//朱雀堂
	{
		nMagicType = 2402;
		nStep = 13;
		pData = FindMagic(nMagicType);
		nStep = 14;
		if (pData && ::RandGet(100)<2)
		{
			int nAddHp = AdjustMagicPower(pData->GetInt(MAGICDATA_POWER),pData->GetInt(MAGICDATA_TYPE),pData->GetMagicLevel(),MAGICLEVADDTYPE_POW);
			nStep = 15;
			int nAddMp = AdjustMagicPower(pData->GetInt(MAGICDATA_POWER2),pData->GetInt(MAGICDATA_TYPE),pData->GetMagicLevel(),MAGICLEVADDTYPE_POW2);
			nStep = 16;
			pUser->AddAttrib(_USERATTRIB_LIFE, nAddHp, SYNCHRO_TRUE);
			nStep = 17;
			pUser->AddAttrib(_USERATTRIB_MANA, nAddMp, SYNCHRO_TRUE);
			//return nDamageAdd;
		}
	}
	DEBUG_CATCH4("CMagic::ProcessSynMagic error!user=%u,magictype=%d,step=%u",pUser->GetID(),nMagicType,nStep)
	return nDamageAdd;
}

//modify code by 林德渊 begin 2011.6.22
int CMagic::ProcessBuffBomb(IRole *m_pOwner1,int flag)
{
	int ret = 0;
	int nStep = 0;
	DEBUG_TRY

	ROLE_SET		setRole;
	POINT	pos;
	nStep = 1;
	setRole.clear();
	CMonster* pMonster = NULL;
	int irange = 5;
	if( 2 == flag )
	{
		irange = 14;
		m_pOwner1->QueryObj(OBJ_MONSTER, IPP_OF(pMonster));
		if( !pMonster )
			return ret;
	}
	else if( 3 == flag )
		irange = 2;
	else if( 4 == flag )
		irange = 300;
	CollectTargetSet_BuffBomb(setRole, pos, LOCK_ALL,irange);// 搜索目标集
	ret = 1;
	if( 2 == flag )
	{
		nStep = 5;
		bool flag = false;
		int num = 0;
		int total = 0;
		for (int i = 0; i < setRole.size(); i++)
		{
			if (!setRole[i] || !setRole[i]->IsAlive())
				continue;
			
			CUser* pUser = NULL;
			setRole[i]->QueryObj(OBJ_USER, IPP_OF(pUser));
			if( pUser )
			{
				total++;
				IStatus* pStatus = pUser->QueryStatus(STATUS_RETORT_MDSX);
				if(pStatus)
					flag = true;
				pStatus = pUser->QueryStatus(STATUS_RETORT_MDSX_BUFF2);
				if(pStatus)
					num++;
			}
		}
		nStep = 6;
		if( !flag )//没有buff1
		{
			srand(unsigned(time(NULL)));//产生随机数
			int autonum = rand()%(int)setRole.size();
			if( !setRole[autonum] || !setRole[autonum]->IsAlive() )
			{
				for (int i = 0; i < setRole.size(); i++)
				{
					if (!setRole[i] || !setRole[i]->IsAlive())
						continue;

					autonum = i;
					break;
				}
			}

			if( !(!setRole[autonum] || !setRole[autonum]->IsAlive()) )
			{
				CUser* pUser = NULL;
				setRole[autonum]->QueryObj(OBJ_USER, IPP_OF(pUser));
				if( pUser )
				{
					if( (total == num) )//全是buff2//消一个buff2，加buff1
						pUser->DetachStatus(pUser,STATUS_RETORT_MDSX_BUFF2);
					pUser->SetNewValue(3000);
					pUser->SetNewValueTrageid(pMonster->GetID());
					pUser->AttachStatus(pUser,STATUS_RETORT_MDSX, 0, 20, 0,0,0,0,pMonster->GetID());
					pUser->AttachStatus(pUser,STATUS_RETORT_MDSX_1, 0, 2, 0,0,0,0,pMonster->GetID());
					CMsgMagicEffect	msg;
					if(msg.Create(pMonster->GetID(), MAGIC_MDSX, 1, pUser->GetID(), 0, pMonster->GetDir(),BATTLEADDINFO::INFO_INVALID))
						pMonster->BroadcastRoomMsg(&msg, INCLUDE_SELF);
				}
			}
		}
		nStep = 7;
	}
	else if( 1 == flag )
	{//追魂斧伤害
		nStep = 2;
		for (int i = 0; i < setRole.size(); i++)
		{
			if (!setRole[i] || !setRole[i]->IsAlive())
				continue;
			
			int nLifeLost = setRole[i]->GetMaxLife()/10*6 - 1000;
			if( nLifeLost < 0 )
				nLifeLost = setRole[i]->GetMaxLife()/10*6;
			//火防
			nLifeLost = nLifeLost - setRole[i]->GetAddAttByType(11) * 5;
			nLifeLost = nLifeLost >0 ? nLifeLost : 0 ;

			nLifeLost = __min(setRole[i]->GetLife(), nLifeLost);
			setRole[i]->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
			setRole[i]->BeAttack(true, m_pOwner1, nLifeLost,0);

			CUser* pMuser = NULL;
			setRole[i]->QueryObj(OBJ_USER, IPP_OF(pMuser));
			if( pMuser )
			{
				CMsgMagicEffect	msg1;
				IF_OK(msg1.Create(m_pOwner1->GetID(), MAGIC_ZHF, 0, pMuser->GetID(), nLifeLost, m_pOwner1->GetDir(),0))
				{
					pMuser->BroadcastRoomMsg(&msg1, INCLUDE_SELF);
				}
			}
		}
		
		nStep = 3;
		for (i = 0; i < setRole.size(); i++)
		{
			if(setRole[i] && !setRole[i]->IsAlive())
			{
				CMonster* pMonster = NULL;
				if(setRole[i]->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))//宠物和怪物先死，因为人死的时候会收回宠物，这里的宠物指针就成了野指针
				{
					m_pOwner1->Kill(setRole[i], DIE_NORMAL);
					setRole[i] = NULL;//置为空，下一轮就不再处理
				}
			}
		}
		nStep = 4;
		for (i = 0; i < setRole.size(); i++)
		{
			if(setRole[i] && !setRole[i]->IsAlive())
			{
				CUser* pMuser = NULL;
				setRole[i]->QueryObj(OBJ_USER, IPP_OF(pMuser));
				if( pMuser )
					pMuser->BeKillBySys();
			}
		}
	}
	else if( 3 == flag )
	{//迷毒噬心伤害
		CUser* pMuser = NULL;
		m_pOwner1->QueryObj(OBJ_USER, IPP_OF(pMuser));
		if( pMuser )
		{
			if( pMuser->GetProcessID() == MSGPORT_MAPGROUP_INSTANCE )
			{
				int nLifeLost = pMuser->GetNewValue();
				int nnextlifelost = 0;
				bool fl1 = false;
				if( 3000 == nLifeLost )
					nnextlifelost = 5000;
				else if( 5000 == nLifeLost )
					nnextlifelost = 8000;
				else if( 8000 == nLifeLost )
					nnextlifelost = 12000;
				else if( 12000 == nLifeLost )
					nnextlifelost = 15000;
				else if( 15000 == nLifeLost )
					nnextlifelost = 21000;
				else if( 21000 == nLifeLost )
					nnextlifelost = 25000;
				else if( 25000 == nLifeLost )
					nnextlifelost = 28000;
				else if( 28000 == nLifeLost )
					nnextlifelost = 31000;
				else if( 31000 == nLifeLost )
					nnextlifelost = 35000;
				else if( 35000 == nLifeLost )
					nnextlifelost = 0;

				//毒防
				nLifeLost = nLifeLost - pMuser->GetAddAttByType(14) * 5;
				nLifeLost = nLifeLost >0 ? nLifeLost : 0 ;
				
				nLifeLost = __min(pMuser->GetLife(), nLifeLost);
				pMuser->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);
				pMuser->BeAttack(true, pMuser, nLifeLost,0);

				if( pMuser->GetNewValueTrageid() )
				{
					CMsgInteract msg;
					IF_OK(msg.Create2(INTERACT_ATTACK, pMuser->GetNewValueTrageid(), pMuser->GetID(), pMuser->GetPosX(), pMuser->GetPosY(), nLifeLost,0))
						pMuser->BroadcastRoomMsg(&msg, INCLUDE_SELF);
				}

				if( !pMuser->IsAlive() )
				{
					ret = 4;
					pMuser->SetNewValue(0);
//					pMuser->DetachStatus(pMuser,STATUS_RETORT_MDSX);
//					pMuser->BeKillBySys();
				}

				for (int i = 0; i < setRole.size(); i++)
				{
					if (!setRole[i] || !setRole[i]->IsAlive())
						continue;
					
					CUser* pUser = NULL;
					setRole[i]->QueryObj(OBJ_USER, IPP_OF(pUser));
					if( pUser )
					{
						IStatus* pStatus1 = pUser->QueryStatus(STATUS_RETORT_MDSX);
						IStatus* pStatus2 = pUser->QueryStatus(STATUS_RETORT_MDSX_BUFF2);
						if( !(pStatus1 || pStatus2) )
						{
							fl1 = true;
							pUser->SetNewValue(3000);
							pUser->SetNewValueTrageid(pMuser->GetNewValueTrageid());
							pUser->AttachStatus(pUser,STATUS_RETORT_MDSX, 0, 20, 0,0,0,0,pUser->GetID());
							pUser->AttachStatus(pUser,STATUS_RETORT_MDSX_1, 0, 2, 0,0,0,0,pUser->GetID());
						}
					}
				}
				if( pMuser->IsAlive() )
				{
					if( fl1 )
					{
						pMuser->SetNewValue(0);
						ret = 5;//pMuser->DetachStatus(pMuser,STATUS_RETORT_MDSX);
						pMuser->AttachStatus(pMuser,STATUS_RETORT_MDSX_BUFF2, 0, 40, 0,0,0,0,pMuser->GetID());
					}
					else
					{
						pMuser->SetNewValue(nnextlifelost);
						if( 0 == nnextlifelost )
							pMuser->AttachStatus(pMuser,STATUS_RETORT_MDSX_BUFF2, 0, 40, 0,0,0,0,pMuser->GetID());
						else
							pMuser->AttachStatus(pMuser,STATUS_RETORT_MDSX_1, 0, 2, 0,0,0,0,pMuser->GetID());
					}
				}
			}
			else
			{
				ret = 5;
				pMuser->SetNewValue(0);
			}
		}
	}
	else if( 4 == flag )
	{//BOSS死后清楚所有迷毒噬心BUFF
		for (int i = 0; i < setRole.size(); i++)
		{
			if (!setRole[i] || !setRole[i]->IsAlive())
				continue;
			
			CUser* pUser = NULL;
			setRole[i]->QueryObj(OBJ_USER, IPP_OF(pUser));
			if( pUser )
			{
				bool fl1 = false;
				IStatus* pStatus1 = pUser->QueryStatus(STATUS_RETORT_MDSX);
				IStatus* pStatus2 = pUser->QueryStatus(STATUS_RETORT_MDSX_BUFF2);
				if( pStatus1 )
				{
					pUser->SetNewValue(0);
					pUser->DetachStatus(pUser,STATUS_RETORT_MDSX);
				}
				if( pStatus2 )
					pUser->DetachStatus(pUser,STATUS_RETORT_MDSX_BUFF2);
			}
		}
	}
	return ret;
	DEBUG_CATCH2("bool CMagic::ProcessBuffBomb() step=%d",nStep);
	return ret;
}

bool CMagic::CollectTargetSet_BuffBomb(ROLE_SET& setRole, POINT& pos, int nLockType,int irange)
{
	int	nRange	= irange;

	int	nSize		= nRange*2 + 1;
	int	nBufSize	= nSize * nSize;
	MAPTHING_SET	setTarget;

	m_setTargetLocked.clear();
	setRole.clear();

	int nDir	= m_pOwner->GetDir();
	pos.x		= m_pOwner->GetPosX();
	pos.y		= m_pOwner->GetPosY();

	BeAttackSort target;
	vector<BeAttackSort> setTargetDistance;
	m_pOwner->GetMap()->CollectMapThing(setTarget, pos, nRange, OBJ_USER);
	
    for(int i = ::CutTrail((int)pos.x-nRange, 0); i <= pos.x+nRange && i < m_pOwner->GetMap()->GetWidth(); i++)
	{
		for(int j = ::CutTrail((int)pos.y-nRange, 0); j <= pos.y+nRange && j < m_pOwner->GetMap()->GetHeight(); j++)
		{
			POINT posThis;
			posThis.x	= i;
			posThis.y	= j;
			if (!CUser::IsInCircle(posThis, pos, nRange))
				continue;
			
			int idx = POS2INDEX(posThis.x - pos.x + nRange, posThis.y - pos.y + nRange, nSize, nSize);
			ASSERT(idx>=0 && idx<setTarget.size());
			for(int zIdx=0;zIdx<setTarget[idx].size();zIdx++)
			{		
				IRole* pRole = NULL;
				if(setTarget[idx][zIdx]->QueryRole(IPP_OF(pRole)) 
					&& pRole->IsAlive()
					&& pRole->GetID()!=m_pOwner->GetID())
				{
					if (pRole->QueryOwnerUser()!=m_pOwner)//宠物不能攻击主人
					{
						target.pRole = pRole;
						target.nDistance = m_pOwner->GetDistance(pRole->GetPosX(), pRole->GetPosY());
						setTargetDistance.push_back(target);
						m_setTargetLocked.push_back(pRole->GetID());
					}
				}
			}
		}
	}
	for (i=0 ; i<setTargetDistance.size(); i++)
		setRole.push_back(setTargetDistance[i].pRole);
	return true;
}
//modify code by 林德渊 end
int	CMagic::ProcessSoulEudemonMagic(CUser* pUser, IRole* pTargeter)
{
	CHECKF(pUser);
	CHECKF(pTargeter);

	int nDamageAdd = 0;
	clock_t tCurr = clock();
	int nCdms = 120000;//cd2分钟
	DEBUG_TRY
	if(pUser->m_eudemonSoulAddHp > 0)
	{
		if(pUser->GetLife() < pUser->GetMaxLife() * 0.1)
		{
			if(pUser->m_tLastAddHp==0 || tCurr - pUser->m_tLastAddHp > nCdms)
			{
				pUser->AddAttrib(_USERATTRIB_LIFE, pUser->m_eudemonSoulAddHp, SYNCHRO_TRUE);
				pUser->m_tLastAddHp = tCurr;
			}
		}
	}
	if(pUser->m_eudemonSoulAddMp > 0)
	{
		if(pUser->GetMana() < pUser->GetMaxMana())
		{
			if(pUser->m_tLastAddMp==0 || tCurr - pUser->m_tLastAddMp > nCdms)
			{
				pUser->AddAttrib(_USERATTRIB_MANA, pUser->m_eudemonSoulAddMp, SYNCHRO_TRUE);
				pUser->m_tLastAddMp = tCurr;
			}
		}
	}
	//打人的和挨打的都可能加
	CUser* pTargetUser = NULL;
	if( pTargeter->QueryObj( OBJ_USER,( void** )&pTargetUser) )
	{
		if(pTargetUser->GetSoulEudemon() != ID_NONE)
		{
			if(pTargetUser->m_eudemonSoulAddHp > 0)
			{
				if(pTargetUser->GetLife() < pTargetUser->GetMaxLife() * 0.1)
				{
					if(pTargetUser->m_tLastAddHp==0 || tCurr - pTargetUser->m_tLastAddHp > nCdms)
					{
						pTargetUser->AddAttrib(_USERATTRIB_LIFE, pTargetUser->m_eudemonSoulAddHp, SYNCHRO_TRUE);
						pTargetUser->m_tLastAddHp = tCurr;
					}
				}
			}
			if(pTargetUser->m_eudemonSoulAddMp > 0)
			{
				if(pTargetUser->GetMana() < pTargetUser->GetMaxMana() * 0.1)
				{
					if(pTargetUser->m_tLastAddMp==0 || tCurr - pTargetUser->m_tLastAddMp > nCdms)
					{
						pTargetUser->AddAttrib(_USERATTRIB_MANA, pTargetUser->m_eudemonSoulAddMp, SYNCHRO_TRUE);
						pTargetUser->m_tLastAddMp = tCurr;
					}
				}
			}
		}
	}
	DEBUG_CATCH("CMagic::ProcessSoulEudemonMagic")
	return nDamageAdd;
}
