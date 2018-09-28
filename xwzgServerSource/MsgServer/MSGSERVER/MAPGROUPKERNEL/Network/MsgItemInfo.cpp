// MsgItemInfo.cpp: interface for the CMsgItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgItemInfo.h"
#include "MapGroup.h"
#include "ItemType.h"
#pragma	warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgItemInfo::CMsgItemInfo()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgItemInfo::~CMsgItemInfo()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgItemInfo::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_ITEMINFO != this->GetType())
		return false;

	return true;
}
// 
// BOOL CMsgItemInfo::Create(int nAction, OBJID idUser)
// {
// 	this->Init();
// 	
// 	m_pInfo->ucAction			= nAction;
// 	m_pInfo->id					= idUser;
// 
// 	m_unMsgType	= _MSG_ITEMINFO;
// 	m_unMsgSize	= sizeof(MSG_Info);
// 
// 	return true;
// }

BOOL CMsgItemInfo::Create(CItem* pItem, int nAction /*= ITEMINFO_ADDITEM*/,OBJID idUser/*=ID_NONE*/)
{
	CHECKF(pItem);
	ItemInfoStruct info;
	memset(&info,0,sizeof(ItemInfoStruct));
	CHECKF(pItem->GetInfo(&info));
	
	// init
	this->Init();

	m_pInfo->id				= info.id;
	m_pInfo->dwType			= info.idType;
	m_pInfo->dwAmount		= info.nAmount;
	m_pInfo->dwAmountLimit	= info.nAmountLimit;
	m_pInfo->usLevel		= info.dwAddLev;
	m_pInfo->usQuality		= info.dwQual;
	m_pInfo->usStar			= info.dwStr;
	m_pInfo->usColor		= info.dwColor;

	m_pInfo->usActiveType	= info.nNeedCheck;
	m_pInfo->dwLifeTime		= info.uLifeTime;

	m_pInfo->usKind			= info.nKidnap;

	m_pInfo->dwGemType[0]	= info.GemTypeID[0];
	m_pInfo->dwGemType[1]	= info.GemTypeID[1];
	m_pInfo->dwGemType[2]	= info.GemTypeID[2];
	m_pInfo->dwGemType[3]	= info.GemTypeID[3];

	m_pInfo->dwBasic[0]		= info.BasicAddID[0];
	m_pInfo->dwBasic[1]		= info.BasicAddID[1];

	m_pInfo->dwAddtion[0]	= info.AdditionAddID[0];
	m_pInfo->dwAddtion[1]	= info.AdditionAddID[1];
	m_pInfo->dwAddtion[2]	= info.AdditionAddID[2];
	m_pInfo->dwAddtion[3]	= info.AdditionAddID[3];
	m_pInfo->dwAddtion[4]	= info.AdditionAddID[4];
	m_pInfo->dwAddtion[5]	= info.AdditionAddID[5];
	m_pInfo->dwAddtion[6]	= info.AdditionAddID[6];
	m_pInfo->dwAddtion[7]	= info.AdditionAddID[7];
	
	m_pInfo->usEquipSoulLev	= info.usEquipSoulLev;
	m_pInfo->usEquipSoulType= info.usEquipSoulType;
	m_pInfo->nEquipSoulPoint=info.nEquipSoulPoint;

	bool bOK = true;
	bOK &= m_StrPacker.AddString(info.szName);
	if(strlen(info.szMakerName) > 0)
		bOK &= m_StrPacker.AddString(info.szMakerName);
	else
		bOK &= m_StrPacker.AddString("нч");
	
	m_unMsgType	=_MSG_ITEMINFO;
	m_unMsgSize	=sizeof(MSG_Info)-1 + m_StrPacker.GetSize();

	return bOK;
}

//////////////////////////////////////////////////////////////////////
void CMsgItemInfo::Process(void *pInfo)
{
	ASSERT(!"CMsgItemInfo::Process()");
	return;

#ifdef _MSGDEBUG
	::LogMsg("Process CMsgItemInfo, id:%u", m_info.id);
#endif
}
