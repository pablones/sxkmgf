// MsgPlayer.cpp: implementation of the CMsgPlayer class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "mapgroup.h"
//#include "UserData.h"
#include "User.h"
#include "AiNpc.h"
//#include "Booth.h"

#include "CombGod.h"
const int	STATUARY_LIFE_SCALE					= 100;			//? 补丁：雕像下传LIFE的比例
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgPlayer::CMsgPlayer()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;

	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgPlayer::~CMsgPlayer()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgPlayer::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_PLAYER != this->GetType())
		return false;

	return true;
}

BOOL CMsgPlayer::CreateBooth(OBJID idBooth,OBJID idMap,USHORT x,USHORT y,char* szName,OBJID idOwner)
{
	this->Init();
	m_pInfo->id				= idBooth;
	m_pInfo->usLookFace     = 30000;
	m_pInfo->usMapID		= idMap;
	m_pInfo->usPosX			= x;
	m_pInfo->usPosY			= y;

	m_pInfo->dwLife         = idOwner;//100;
	m_pInfo->dwMaxLife      = 100;

	BOOL bSucMake	=true;
	bSucMake	&=m_StrPacker.AddString(szName);

	m_unMsgType	=_MSG_PLAYER;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();

	return bSucMake;
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgPlayer::Create(IRole* pRole,bool justCome/* = false*/)
{
	DEBUG_CREATEMSG("CMsgPlayer",pRole->GetID(),pRole->IsUser(),"",pRole->GetPosX(),pRole->GetPosY())
	// param check
	if (!pRole)
		return false;
	BOOL bSucMake	=true;

	int nStep = 0;
	DEBUG_TRY
	// init
	this->Init();
	nStep = 1;
	m_pInfo->id				= pRole->GetID();
	nStep = 11;
	if(pRole->IsUser())
	{
		CUser* pUser;
		pRole->QueryObj(OBJ_USER, IPP_OF(pUser));
		if (pUser && pUser->GetVipface())
		{
			m_pInfo->usLookFace		= pUser->GetVipface();
		}
		else
		{
			m_pInfo->usLookFace		= pRole->GetLookFace();
		}
	}
	else
		m_pInfo->usLookFace		= pRole->GetLookFace();
	nStep = 12;
	m_pInfo->dwProfession	= pRole->GetProfession();

	nStep = 13;
	m_pInfo->dwWeapenType	= pRole->GetWeaponRTypeID()*10+pRole->GetWeaponSpecialView();
	nStep = 14;
	m_pInfo->dwDressType	= pRole->GetDressTypeID();
	nStep = 15;
	
	m_pInfo->dwMaxLife		= pRole->GetMaxLife();
	nStep = 16;
	m_pInfo->dwLife			= pRole->GetLife();
	nStep = 17;
	m_pInfo->dwMaxMana		= pRole->GetMaxMana();
	nStep = 18;
	m_pInfo->dwMana			= pRole->GetMana();
	nStep = 19;
	m_pInfo->dwMaxSp		= 0;
	m_pInfo->dwSp			= 0;
	m_pInfo->usLevel		= pRole->GetLev();
	m_pInfo->nSex			= 0;
	m_pInfo->dwIsVip		= 0;

	nStep = 2;

	if(pRole->GetMap())
		m_pInfo->usMapID = pRole->GetMap()->GetID(); 
	m_pInfo->usPosX		= pRole->GetPosX();
	m_pInfo->usPosY		= pRole->GetPosY();
	m_pInfo->ucDir		= pRole->GetDir();

	if(m_pInfo->ucDir < 1 || m_pInfo->ucDir > 9 || m_pInfo->ucDir == 5)
		m_pInfo->ucDir = 2;

	m_pInfo->idBooth    = ID_NONE;

	bSucMake	&=m_StrPacker.AddString((char*)pRole->GetName());

	nStep = 3;
	CMonster* pMonster = NULL;
	if(pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
	{
		nStep = 4;
		m_pInfo->dwAttackUser = pMonster->GetAttackUser();
		m_pInfo->idMonsterType = pMonster->GetType();
		m_pInfo->dwSp          = pMonster->GetExp();

		if(pMonster->IsEudemon() && pMonster->QueryOwnerUser())
		{
			nStep = 5;
			m_pInfo->idOwner	= pMonster->QueryOwnerUser()->GetID();
			m_pInfo->dwIsDress = pMonster->GetUnderstand();
			m_pInfo->idBooth	= pMonster->GetGetType();
			bSucMake	&=m_StrPacker.AddString("无");
			bSucMake	&=m_StrPacker.AddString("无");
			bSucMake	&=m_StrPacker.AddString((char*)pMonster->GetOwnerName());
		}
	}

	nStep = 6;
	CNpc* pNpc = NULL;
	if (pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)))
	{
		if(pNpc->IsDynaNpc())
			m_pInfo->idMonsterType = pNpc->GetNpcType();
		else
			m_pInfo->idMonsterType = m_pInfo->id;
	}

	nStep = 7;
	CUser* pUser = NULL;
	if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
	{
		m_pInfo->bIsPMHide	= pUser->IsHide();
		m_pInfo->nPK		= pUser->GetPk();
		m_pInfo->dwMaxSp	= pUser->GetMaxSp();
		m_pInfo->dwSp		= pUser->GetSp();
		m_pInfo->nSex       = pUser->GetSex();

		if(pUser->IsVipActive())
		{
			m_pInfo->dwIsVip            = pUser->GetVipLev();
		}
		else
		m_pInfo->dwIsVip			= 0;

		nStep = 8;
		if (pUser->GetMountState())
			m_pInfo->dwMountType	= pUser->GetMountTypeID();
		else
			m_pInfo->dwMountType	= 0;

		m_pInfo->idSyn              = ID_NONE;
		m_pInfo->nSynRank			= 0;
		if(pUser->GetSyndicate())
		{
			m_pInfo->idSyn			= pUser->GetSyndicate()->GetID();
			m_pInfo->nSynRank       = pUser->QuerySynRank();
			if(pUser->GetSyndicate()->GetInt(SYNDATA_STATE) == 1)
				m_pInfo->nSynRank += 1000;
		}


		nStep = 9;
		if (pUser->QueryBooth())
		{
			m_pInfo->idBooth = pUser->QueryBooth()->GetID();
		}
	
		bSucMake	&=m_StrPacker.AddString(pUser->GetFeel());

		CTeam*pTeam = pUser->GetTeam();
		if(pTeam)
		{
			m_pInfo->idTeam = pTeam->GetID();
			m_pInfo->idTeamLeader = pTeam->GetLeader();
			bSucMake	&=m_StrPacker.AddString((char*)pTeam->GetTeamName());
		}
		else
		{
			bSucMake	&=m_StrPacker.AddString("无");
		}
		bSucMake	&=m_StrPacker.AddString("无");

		nStep = 11;
		m_pInfo->dwIsDress = pUser->GetDress();
	}

	nStep = 10;
	m_unMsgType	=_MSG_PLAYER;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();

	DEBUG_CATCH3(" CMsgPlayer::Create(IRole* pRole,bool justCome/* = false*/) %u %u",nStep,m_pInfo->id	)
	return bSucMake;

}

/*//////////////////////////////////////////////////////////////////////
BOOL CMsgPlayer::Create((CBooth* pBooth))
{
	// param check
	if (idPlayer == ID_NONE)
		return false;

	// init
	this->Init();

	// fill info now
	m_pInfo->id					= pBooth->GetID();
	m_pInfo->dwLookFace			= pBooty->GetLook();
	m_pInfo->dwEffect			= 0;
	m_pInfo->dwSynID_Rank		= 0;

	m_pInfo->dwHelmetType		= pInfo->ucFace;
	m_pInfo->dwArmorType		= pInfo->dwExp;
	m_pInfo->dwWeaponRType		= pInfo->nRepute;
	m_pInfo->dwWeaponLType		= pInfo->nRepute;
	m_pInfo->dwMountType		= pInfo->ucRank;

	m_pInfo->dwMedalSelect		= 0;

	m_pInfo->usPosX				= pBooth->GetPosX();
	m_pInfo->usPosY				= pBooth->GetPosY();
	m_pInfo->usHair				= 0;
	m_pInfo->ucDir				= pBooth->GetDir();
	m_pInfo->ucPose				= 0;

	if(!m_StrPacker.AddString(pBooth->GetOwnerName())
		return false;

	m_unMsgType	=_MSG_PLAYER;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();

	return true;
}*/

//////////////////////////////////////////////////////////////////////
void CMsgPlayer::Process(void *pInfo)
{
	return;
	CUser* pUser = g_UserManager.GetUser(this);
	pUser->BroadcastRoomMsg(this, EXCLUDE_SELF);

#ifdef _MSGDEBUG
	::LogMsg("Process CMsgPlayer, id:%u", m_pInfo->id);
#endif

	char szName[_MAX_NAMESIZE]		="";
	char szMate[_MAX_NAMESIZE]		="";
	char szSyndicate[_MAX_NAMESIZE]	="";
	char szTitle[_MAX_NAMESIZE]		="";

	m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
	m_StrPacker.GetString(1, szMate, _MAX_NAMESIZE);
	m_StrPacker.GetString(2, szSyndicate, _MAX_NAMESIZE);
	m_StrPacker.GetString(3, szTitle, _MAX_NAMESIZE);
}
