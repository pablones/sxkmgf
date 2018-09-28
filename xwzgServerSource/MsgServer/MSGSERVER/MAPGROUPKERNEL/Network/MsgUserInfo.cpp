// MsgUserInfo.cpp: interface for the CMsgUserInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "UserData.h"
#include "Rename.h"
#pragma	warning(disable:4786)
extern CRename* g_pRename;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgUserInfo::CMsgUserInfo()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;

	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgUserInfo::~CMsgUserInfo()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgUserInfo::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_USERINFO != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgUserInfo::Create(CUser* pUser)
{
	CHECKF(pUser);
	int n = sizeof(m_pInfo);
	const UserInfoStruct* pInfo = pUser->GetInfo();
	this->Init();
	
	m_pInfo->id					= pInfo->id;
	if(pUser->GetVipface())
	{
		m_pInfo->ucLookFace			= pUser->GetVipface();
	}
	else
		m_pInfo->ucLookFace			= pInfo->dwLookface;
	m_pInfo->ucFeel			    = g_pRename->ISCanRename(pUser->GetID())?1:0;///能否改人名
	m_pInfo->Canrename			=	g_pRename->ISCanRename(pUser->GetSynID())?1:0;///能否改帮名
	m_pInfo->ucCurJob			= pInfo->CurJob;
	m_pInfo->nSex				= pInfo->sex;
	m_pInfo->usLevel			= pInfo->usLevel;

	m_pInfo->usHp				= pInfo->nLife;
	m_pInfo->usMaxHp			= pUser->GetMaxLife();
	m_pInfo->usMp				= pInfo->nMana;
	m_pInfo->usMaxMp			= pUser->GetMaxMana();
	m_pInfo->usSp				= pInfo->nSp;
	m_pInfo->usMaxSp			= pUser->GetMaxSp();

	m_pInfo->usFirJob			= pInfo->FirJob;
	m_pInfo->usFirJobLev		= pInfo->FirJobLev;
	m_pInfo->usFirJobPhase		= pInfo->FirJobPhase;
	m_pInfo->usSecJob			= pInfo->SecJob;
	m_pInfo->usSecJobLev		= pInfo->SecJobLev;
	m_pInfo->usSecJobPhase		= pInfo->SecJobPhase;

	//m_pInfo->nMount				= pUser->GetMountTypeID();
	if (pUser->GetMountState())
		m_pInfo->nMount	= pUser->GetMountTypeID();
	else
		m_pInfo->nMount	= 0;

	m_pInfo->nDress				= pUser->GetDressTypeID();
	m_pInfo->nWeapen			= pUser->GetWeaponRTypeID()*10+pUser->GetWeaponSpecialView();
	m_pInfo->Vit				= pInfo->Vit;
	m_pInfo->MaxVit				= pInfo->MaxVit;
	m_pInfo->Ene				= pInfo->Ene;
	m_pInfo->MaxEne				= pInfo->MaxEne;
	m_pInfo->uExp			    = pInfo->i64Exp;
	m_pInfo->uHoldTitle		    = pInfo->i64HoldTitle;
	m_pInfo->uUseTitle			= pInfo->iUseTitle;
	m_pInfo->nPk				= pInfo->nPk;
	m_pInfo->PkProc				= pInfo->ucPkMode;
	m_pInfo->nMoney1			= pInfo->money1;
	m_pInfo->nMoney2			= pInfo->money2;
	m_pInfo->nMoney3			= pInfo->money3;
	m_pInfo->nMoney4			= pInfo->money4;
	m_pInfo->nMoney5			= pInfo->money5;

	m_pInfo->taskMaskHi         = pInfo->i64TaskMask>>32;
	m_pInfo->taskMaskLow        = pInfo->i64TaskMask&0x00000000ffffffff;
	m_pInfo->DaytaskMaskHi      = pInfo->i64DayTaskMask>>32;
	m_pInfo->DaytaskMaskLow     = pInfo->i64DayTaskMask&0x00000000ffffffff;
	m_pInfo->DaytaskMaskHi2     = pInfo->i64DayTaskMask2>>32;
	m_pInfo->DaytaskMaskLow2    = pInfo->i64DayTaskMask2&0x00000000ffffffff;

	m_pInfo->idSyn              = ID_NONE;
	m_pInfo->nSynRank			= 0;
	m_pInfo->conSyn				= 0;
	if(pUser->GetSyndicate())
	{
		m_pInfo->idSyn = pUser->GetSyndicate()->GetID();
		m_pInfo->nSynRank           = pUser->QuerySynRank();
		if(pUser->GetSyndicate()->GetInt(SYNDATA_STATE) == 1)
			m_pInfo->nSynRank += 1000;
		m_pInfo->conSyn					= pUser->QuerySynAttr()->GetProffer();
	}

	m_pInfo->dwBagLev           = pInfo->nPackageLev;
	m_pInfo->dwBankLev          = pInfo->nBankLev;
	m_pInfo->dwIsDress			= pUser->GetDress();

	if(pUser->IsVipActive())
	{
		m_pInfo->dwIsVip            = pUser->GetVipLev();
	}
	else
		m_pInfo->dwIsVip			= 0;


	BOOL bSucMake	=true;

	bSucMake	&=m_StrPacker.AddString((char*)pInfo->szName);
	bSucMake	&=m_StrPacker.AddString((char*)"111");
	bSucMake	&=m_StrPacker.AddString((char*)pInfo->szFeel);//[2010-06-04 goto] 玩家心情改为字符串
	m_unMsgType	=_MSG_USERINFO;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();

   m_pInfo->onlineaward=     pInfo->onlineaward;
	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgUserInfo::Process(void *pInfo)
{
	ASSERT(!"CMsgUserInfo::Process()");
	return;

#ifdef _MSGDEBUG
	::LogMsg("Process CMsgUserInfo, id:%u", m_pInfo->id);
#endif

	char szName[_MAX_NAMESIZE]		="";
	char szMate[_MAX_NAMESIZE]		="";
	char szSyndicate[_MAX_NAMESIZE]	="";
	char szTitle[_MAX_NAMESIZE]		="";

	m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
	m_StrPacker.GetString(1, szMate, _MAX_NAMESIZE);
}
