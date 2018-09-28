// Booth.cpp: implementation of the CBooth class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "Booth.h"
#include "User.h"
#include "MapGroup.h"
#include "AllMsg.h"

//////////////////////////////////////////////////////////////////////
const int	BOOTH_SIZE		= 24;
const int	EUDEMON_SIZE		= 7;

MYHEAP_IMPLEMENTATION(CBooth,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBooth::CBooth()
{
	m_id			= ID_NONE;
	m_pMap			= NULL;
	m_bEnterMap		= false;
	m_szCryOut[0]	= 0;
	m_idLook = BOOTH_LOOK;

	for(int i =0; i< BOOTH_SIZE ; i++)
		bHasItem[i] = 0;
}

//////////////////////////////////////////////////////////////////////
CBooth::~CBooth()
{
	for(int i = 0; i < m_setGoods.size(); i++)
	{
		if(m_setGoods[i].idItem == ID_NONE)
			continue;
		CItem * pItem = m_pUser->GetItem(m_setGoods[i].idItem,false);
		if(pItem)
			pItem->setLocked(false);
	}

	for(i = 0; i < m_setEudemon.size(); i++)
	{
		m_pUser->UnLockEudemon(m_setEudemon[i].idEudemon);
	}

	if(m_pMap)
	{
		LeaveMap();
	}

	if(m_id != ID_NONE)
		MapManager()->RecycleDynaNpcID(m_id);
}

CBooth*	CBooth::CreateNew(PROCESS_ID idProcess, CUser* pUser, int nPosX, int nPosY, int nDir)
{
	CBooth* pBooth = new CBooth;
	if(pBooth)
	{
		if(pBooth->Create(idProcess, pUser, nPosX, nPosY, nDir))
		{
			return pBooth;
		}
		else
			delete pBooth;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////
bool CBooth::Create(PROCESS_ID idProcess, CUser* pUser, int nPosX, int nPosY, int nDir)
{
	m_idProcess	= idProcess;
	m_pUser		= pUser;
	CHECKF(pUser);

	//if(pUser->GetMountTypeID() != ID_NONE )
	if(pUser->GetMountState() )
	{
		pUser->SendSysMsg( _TXTATR_TOPSHOW, "坐骑上不能摆摊" );//"坐骑上不能摆摊!"
		return false;
	}

	m_pMap		= m_pUser->GetMap();
	CHECKF(m_pMap);
	m_id		= MapManager()->SpawnNewNpcID();

	sprintf(m_szName,"杂货摊位");
	strcpy(m_szCryOut,"还没有介绍信息");
	ClrLeaveWord();

	EnterMap(nPosX, nPosY, nDir);
	CRole::AttachStatus(m_pUser, STATUS_BOOTH_ADDEXP, 1, 60, 1, 0, 0, 0, m_pUser->GetID());//摆摊时加经验
	return true;
}

//////////////////////////////////////////////////////////////////////
// IMapThing
//////////////////////////////////////////////////////////////////////
void CBooth::SendShow(IRole* pRole)
{
	CHECK(m_pUser);
	CMsgPlayer msg;
	if (msg.CreateBooth(GetID(),m_pMap->GetDocID(),m_nPosX,m_nPosY,m_szName,m_pUser->GetID()))
		pRole->SendMsg(&msg);
}

//////////////////////////////////////////////////////////////////////
// application
//////////////////////////////////////////////////////////////////////
bool CBooth::AddEudemon(OBJID idEudemon, int nMoney)
{
	CHECKF(idEudemon != ID_NONE && nMoney > 0);
	if(m_setEudemon.size() >= EUDEMON_SIZE)
	{
		m_pUser->SendSysMsg( "数量已满" );
		return false;
	}
	
	if(m_pUser->IsLockedEudemon(idEudemon))
	{
		m_pUser->SendSysMsg( "宠物绑定中，不能进行其他操作" );
		return false;
	}
		

	EudemonData eudeData;
	if(!m_pUser->LoadEudemon(idEudemon,eudeData,2))
	{
		m_pUser->SendSysMsg( "宠物错误" );
		return false;
	}
	
	if(m_setEudemon.size() >= 7)
	{
		m_pUser->SendSysMsg( "宠物数量已满，不能再添加" );
		return false;
	}

	for(int i = 0; i < m_setEudemon.size(); i++)
	{
		if(m_setEudemon[i].idEudemon == idEudemon)
		{
			m_pUser->LockEudemon(idEudemon);
			m_pUser->SendSysMsg( "宠物已经上架，不能重复操作" );
			return false;
		}
	}

	m_pUser->LockEudemon(idEudemon);

	EUDEMON_ST	info;
	info.idEudemon		= idEudemon;
	info.nMoney			= nMoney;
	info.gettype		= eudeData.gettype;
	info.lookface		= eudeData.dwLookface;
	info.needlev		= eudeData.dwNeedlev;
	info.failtime		= eudeData.failtime;
	info.sex			= eudeData.sex;
	info.habit			= eudeData.habit;
	info.lev			= eudeData.level;
	info.gift			= eudeData.gift;
	info.bronTime		= eudeData.borntime;
	info.understand		= eudeData.understand;
	SafeCopy(info.szName,eudeData.name,16);
	m_setEudemon.push_back(info);
	
	CMsgEudemon msg;
	if(msg.Create(PETACTION_AddBoothPet,NULL,m_pUser->GetID()))
	{
		msg.Append(eudeData,nMoney);
		m_pUser->SendMsg(&msg);
	}
	return true;
}

bool CBooth::AddItem(OBJID idItem, int nMoney, DWORD dwType)
{
	CHECKF(idItem != ID_NONE && nMoney > 0);
	if(m_setGoods.size() >= BOOTH_SIZE)
	{
		m_pUser->SendSysMsg( g_objStrRes.GetStr( 14046 ) );
		return false;
	}

	CItem* pItem = m_pUser->GetItem(idItem,false);
	if(!pItem)
		return false;

	if(pItem->GetKidnap() == 1)
	{
		m_pUser->SendSysMsg( "该物品已绑定，不能摆摊" );
		return false;
	}

	if (!pItem->IsExchangeEnable())
	{
		m_pUser->SendSysMsg( "此物品不能交易"  );
		return false;
	}

	GOODS_ST	info;
	info.idItem		= idItem;
	info.nType		= pItem->GetInt(ITEMDATA_TYPE);
	info.nMoney		= nMoney;
	info.dwColor    = pItem->GetColour();
	info.nindex     = FindIndex();
	if(info.nindex == -1)
	{
		m_pUser->SendSysMsg( g_objStrRes.GetStr( 14046 ) );
		return false;
	}
	SetIndex(info.nindex,1);
	if (CItem::IsCountable(pItem->GetTypeID()))
	{
		info.dwNumber	= pItem->GetItemAmount();
	}
	else
	{
		if(pItem->GetTypeID() / 1000 == 301 || pItem->GetTypeID() / 1000 == 311 || pItem->GetTypeID() / 1000 == 321
			|| pItem->GetTypeID() == _SPCITEM_VASEXCHANGE
			|| pItem->GetTypeID() == _SPCITEM_EXPEXCHANGE
			|| pItem->GetTypeID() == _SPCITEM_MONEYFORSHOP)
		{
			info.dwNumber	= pItem->GetItemAmount();
		}
		else
			info.dwNumber	= 1;
	}
	for(int i = 0; i < m_setGoods.size(); i++)
	{
		if(m_setGoods[i].idItem == idItem)
			return false;
	}

	pItem->setLocked(true);
	m_setGoods.push_back(info);

	CMsgItem msg;
	if (msg.Create(ITEMACT_ADD,0))
	{
		if (msg.Add(info.idItem,info.nType,info.dwNumber,pItem->GetInt(ITEMDATA_AMOUNTLIMIT), /*pItem->GetPosition()*/200,pItem->GetKidnap(),info.nindex,info.nMoney,info.dwColor))
			m_pUser->SendMsg(&msg);
	}
	return true;
}

bool CBooth::ChgEudemon(OBJID idEudemon, int nMoney)
{
	CHECKF(idEudemon != ID_NONE && nMoney > 0);
	EudemonData eudemonData;

	if(!m_pUser->LoadEudemon(idEudemon,eudemonData,2))
		return false;

	
	for(int i = 0; i < m_setEudemon.size(); i++)
	{
		if(m_setEudemon[i].idEudemon == idEudemon)
		{
			m_setEudemon[i].nMoney = nMoney;
			
			CMsgEudemon msg;
			if(msg.Create(PETACTION_AddBoothPet,NULL,m_pUser->GetID()))
			{
				msg.Append(eudemonData,nMoney);
				m_pUser->SendMsg(&msg);
			}
			
			return true;
		}
	}

	return false;
}

bool CBooth::ChgItem(OBJID idItem, int nMoney, DWORD dwType)
{
	CHECKF(idItem != ID_NONE && nMoney > 0);

	CItem* pItem = m_pUser->GetItem(idItem,false);
	if(!pItem)
		return false;
	
	if (!pItem->IsExchangeEnable())
	{
		m_pUser->SendSysMsg( g_objStrRes.GetStr( 11007 ) );
		return false;
	}

	int nAmount = 0;
	if (CItem::IsCountable(pItem->GetTypeID()))
	{
		nAmount	= pItem->GetItemAmount();
	}
	else
	{
		nAmount	= 1;
	}

	for(int i = 0; i < m_setGoods.size(); i++)
	{
		if(m_setGoods[i].idItem == idItem)
		{
			m_setGoods[i].dwNumber = nAmount;
			m_setGoods[i].nMoney = nMoney;

			CMsgItem msg;
			if (msg.Create(ITEMACT_ADD,0))
			{
				if (msg.Add(m_setGoods[i].idItem,m_setGoods[i].nType,m_setGoods[i].dwNumber,pItem->GetInt(ITEMDATA_AMOUNTLIMIT), /*pItem->GetPosition()*/200,pItem->GetKidnap(),m_setGoods[i].nindex,m_setGoods[i].nMoney,m_setGoods[i].dwColor))
					m_pUser->SendMsg(&msg);
			}

			return true;
		}
	}
	return false;
}

int CBooth::FindIndex()
{
	for(int i = 0 ; i < BOOTH_SIZE ; i ++)
	{
		if(!bHasItem[i])
			return i + 1;
	}
	return -1;
}

void CBooth::SetIndex(int index,bool data)
{
	bHasItem[index - 1] = data;
}

bool CBooth::DelEudemon(OBJID idEudemon)
{
	bool ret = false;
	for(int i = 0; i < m_setEudemon.size(); i++)
	{
		if(m_setEudemon[i].idEudemon == idEudemon)
		{
			m_pUser->UnLockEudemon(idEudemon);
			m_setEudemon.erase(m_setEudemon.begin() + i);

			
			CMsgEudemon msg;
			EudemonData data;
			data.id = idEudemon;
			if(msg.Create(PETACTION_DelBoothPet,NULL,m_pUser->GetID()))
			{
				msg.Append(data,0);
				m_pUser->SendMsg(&msg);
			}

			ret = true;
		}
	}
	return ret;	
}

//////////////////////////////////////////////////////////////////////
bool CBooth::DelItem(OBJID idItem, DWORD dwNumber, CUser *pTarget)
{
	CHECKF(idItem);
	bool ret = false;
	for(int i = 0; i < m_setGoods.size(); i++)
	{
		if(m_setGoods[i].idItem == idItem)
		{
			CItem * pItem = m_pUser->GetItem(idItem,false);
			if(pItem)
			{
				if(pItem->GetTypeID() / 1000 == 301 || pItem->GetTypeID() / 1000 == 311 || pItem->GetTypeID() / 1000 == 321
					|| pItem->GetTypeID() == _SPCITEM_VASEXCHANGE
					|| pItem->GetTypeID() == _SPCITEM_EXPEXCHANGE
					|| pItem->GetTypeID() == _SPCITEM_MONEYFORSHOP)
				{
					pItem->setLocked(false);

					SetIndex(m_setGoods[i].nindex,0);
					m_setGoods.erase(m_setGoods.begin() + i);
					return true;
				}
			}
			
			if((dwNumber == 0 )|| (dwNumber == m_setGoods[i].dwNumber))
			{
				CItem * pItem = m_pUser->GetItem(idItem,false);
				if(pItem)
					pItem->setLocked(false);

				SetIndex(m_setGoods[i].nindex,0);
				m_setGoods.erase(m_setGoods.begin() + i);
			}
			else if(dwNumber < m_setGoods[i].dwNumber)
			{
				m_setGoods[i].dwNumber -= dwNumber;
			}
			ret = true;
		}
	}
	return ret;	
}

bool CBooth::BuyEudemon(CUser* pTarget, OBJID idEudemon, DWORD dwMoney)
{
	CHECKF(pTarget && idEudemon != ID_NONE);
	DEBUG_TRY
	OBJID idSource = m_pUser->GetID();
	OBJID idTarget = pTarget->GetID();
	if(idSource == idTarget)
	{
		pTarget->SendSysMsg(_TXTATR_TOPSHOW,"不能购买自己的物品");
		return false;
	}

	//找摆摊信息
	bool bFind = false;
	int index = 0;
	for(int i = 0; i < m_setEudemon.size(); i++)
	{
		if(m_setEudemon[i].idEudemon == idEudemon)
		{
			index = i;
			bFind = true;
			break;
		}
	}
	if(!bFind)
	{
		pTarget->SendSysMsg(_TXTATR_TOPSHOW,"摊位信息已改变，请刷新");
		return false;
	}

	if(dwMoney != m_setEudemon[index].nMoney)
	{
		pTarget->SendSysMsg(_TXTATR_TOPSHOW,"摊位信息已改变，请刷新");
		return false;
	}
	if((pTarget->GetMoneyByType(1) < dwMoney))
	{
		pTarget->SendSysMsg( _TXTATR_TOPSHOW,"银两不足，无法购买");
		return false;
	}

	if (m_pUser->GetMoneyByType(1) + dwMoney > _MAX_MONEYLIMIT)
	{
		pTarget->SendSysMsg(_TXTATR_TOPSHOW,"对方金钱超过上限,无法再进行交易");
		m_pUser->SendSysMsg(_TXTATR_TOPSHOW,"你所携带的金钱太多,导致一次交易失败请减少你所携带的金钱");
		return false;
	}

	if(pTarget->GetEudemonAmount() >= 3 + pTarget->GetPacklev(4))
	{
		pTarget->SendSysMsg(_TXTATR_TOPSHOW,"宠物背包空间不足,无法购买");
		return false;
	}
	CUserPet* pUserPetSet = m_pUser->QueryUserPetSet();
	if (!pUserPetSet)
		return false;
	CUserPetData* pUserPet = pUserPetSet->QueryData(idEudemon);
	if (!pUserPet)
		return false;
	CEudemonTypeData* pEudemonType = EudemonType()->GetObj(pUserPet->GetInt(EUDEMONDATA_typeid));
	if(!pEudemonType)
		return false;
	if (pTarget->GetLev() < pEudemonType->GetInt(EUDEMONTYPEDATA_needLev))
	{
		pTarget->SendSysMsg(_TXTATR_TOPSHOW, "宠物携带等级过高，无法携带");
		return false;
	}
	if (pUserPet->GetInt(EUDEMONDATA_level) > pTarget->GetLev()+5)
	{
		pTarget->SendSysMsg(_TXTATR_TOPSHOW, "宠物等级超过人物等级5级以上，无法携带");
		return false;
	}
	ASSERT(DelEudemon(idEudemon));

	CMsgEudemon msg;
	EudemonData data;
	data.id = idEudemon;
	if(msg.Create(PETACTION_DelBoothPet,NULL,m_pUser->GetID()))
	{
		msg.Append(data,0);
		pTarget->SendMsg(&msg);
	}

	CUserPetData* pEudemon = m_pUser->PopEudemon(idEudemon,true,false);
	if(!pEudemon)
	{
		pTarget->SendSysMsg(_TXTATR_TOPSHOW,"摊位信息已改变，请刷新");
		return false;
	}

	if(!pTarget->AddEudemon(pEudemon,true,true))
	{
		m_pUser->AddEudemon(pEudemon,true,true);
		pTarget->SendSysMsg(_TXTATR_TOPSHOW,"背包已满");
		return false;
	}
	

	BOOL bRet = FALSE;
	ASSERT(bRet = pTarget->SpendMoneyByType(dwMoney, 1, SYNCHRO_TRUE));
	ASSERT(bRet = m_pUser->GainMoney(dwMoney * 19 / 20, SYNCHRO_TRUE, 1));
	if (dwMoney>30000)
		::MyLogSave(VALUABLESLOG_FILENAME, "CBooth::BuyEudemon user:%u(%s)出售宠物%u给user:%u(%s),获得银两%d",m_pUser->GetID(),m_pUser->GetName(), idEudemon, pTarget->GetID(), pTarget->GetName(), dwMoney);

	char szTemp[1024];
	char szMoney[1024] = "";
	int nMoney = dwMoney / 10000;
	if(nMoney > 0)
	{
		sprintf(szTemp,"%d金",nMoney);
		strcat(szMoney,szTemp);
	}
	nMoney = (dwMoney % 10000) / 100;
	if(nMoney > 0)
	{
		sprintf(szTemp,"%d银",nMoney);
		strcat(szMoney,szTemp);
	}
	nMoney = dwMoney % 100;
	if(nMoney > 0)
	{
		sprintf(szTemp,"%d铜",nMoney);
		strcat(szMoney,szTemp);
	}

	sprintf(szTemp,"<%s>购买{%u_%u_%s}，总价：%d",pTarget->GetName(),pTarget->GetID(),pEudemon->GetID(),pEudemon->GetStr(EUDEMONDATA_name), dwMoney );
	m_pUser->SendSysMsg(_TXTATR_TOPSHOW,"[%s]花了%s购买你的%s", pTarget->GetName(), szMoney,pEudemon->GetStr(EUDEMONDATA_name));
	pTarget->SendSysMsg(_TXTATR_TOPSHOW,"你花了%s购买[%s]的%s", szMoney, m_pUser->GetName(),pEudemon->GetStr(EUDEMONDATA_name));
	AddLeaveWord("系统",szTemp,true,pTarget);
	::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) transfer pet %u(%d) to %u(%s)", m_pUser->GetID(), m_pUser->GetName(), pEudemon->GetID(), pEudemon->GetInt(EUDEMONDATA_typeid), pTarget->GetID(), pTarget->GetName());
	DEBUG_CATCH("CBooth::BuyEudemon()")		// VVVVVVVVVVVVVVVVVVVVV
	return true;
}
//////////////////////////////////////////////////////////////////////
bool CBooth::BuyItem(CUser* pTarget, OBJID idItem, DWORD dwMoney, DWORD dwNumber)//[游途道标 2008/12/23]新增一个数量参数
{
	CHECKF(pTarget && idItem != ID_NONE);
	DEBUG_TRY
	OBJID idSource = m_pUser->GetID();
	OBJID idTarget = pTarget->GetID();
	if(idSource == idTarget)
	{
		pTarget->SendSysMsg(_TXTATR_TOPSHOW,"不能购买自己的物品");
		return false;
	}

	//找摆摊信息
	bool bFind = false;
	int index = 0;
	for(int i = 0; i < m_setGoods.size(); i++)
	{
		if(m_setGoods[i].idItem == idItem)
		{
			index = m_setGoods[i].nindex;
			bFind = true;
			break;
		}
	}
	if(!bFind)
	{
		pTarget->SendSysMsg(_TXTATR_TOPSHOW,"摊位信息已改变，请刷新");
		return false;
	}

	if(dwNumber == 0)
		dwNumber = m_setGoods[i].dwNumber;

	if(dwMoney != m_setGoods[i].nMoney)
	{
		pTarget->SendSysMsg(_TXTATR_TOPSHOW,"摊位信息已改变，请刷新");
		return false;
	}

	DWORD dwRealMoney = m_setGoods[i].nMoney * dwNumber;
	if((pTarget->GetMoneyByType(1) < dwRealMoney))
	{
		pTarget->SendSysMsg( _TXTATR_TOPSHOW,"银两不足，无法购买");
		return false;
	}

	if (m_pUser->GetMoneyByType(1) + dwRealMoney > _MAX_MONEYLIMIT)
	{
		pTarget->SendSysMsg(_TXTATR_TOPSHOW,"对方金钱超过上限,无法再进行交易");
		m_pUser->SendSysMsg(_TXTATR_TOPSHOW,"你所携带的金钱太多,导致一次交易失败请减少你所携带的金钱");
		return false;
	}

	CItem* pItem = m_pUser->GetItem(idItem,false);
	if(!pItem)
	{
		pTarget->SendSysMsg(_TXTATR_TOPSHOW,"摊位信息已改变，请刷新");
		return false;
	}

	if(pTarget->IsItemFull(0, pItem->GetInt(ITEMDATA_TYPE), /*pItem->GetInt(ITEMDATA_AMOUNTLIMIT)*/1))
	{
		pTarget->SendSysMsg(_TXTATR_TOPSHOW,"背包已满");
		return false;
	}

	ASSERT(DelItem(idItem, dwNumber, pTarget));	
	CItemPtr pItemTemp = NULL;
	if (CItem::IsCountable(pItem->GetTypeID()))
	{
		if (dwNumber > pItem->GetItemAmount())
		{
			pTarget->SendSysMsg(_TXTATR_TOPSHOW,"摊位信息已改变，请刷新");
			return false;
		}
		else if (dwNumber != pItem->GetItemAmount())
		{
			pItemTemp = pItem->Split(dwNumber, Database());
			m_pUser->SynchroItem(pItem,ITEMACT_ADD);
			m_pUser->SynchroBoothItem(pItem,ITEMACT_ADD,index,dwMoney);
			pTarget->SynchroBoothItem(pItem,ITEMACT_ADD,index,dwMoney);
			CHECKF(pItemTemp);
		}
		else
		{
			ASSERT(pItemTemp = m_pUser->PopItem(idItem, SYNCHRO_TRUE));
			m_pUser->SynchroBoothItem(pItemTemp,ITEMACT_BOOTH_DEL,index,dwMoney);
			pTarget->SynchroBoothItem(pItemTemp,ITEMACT_BOOTH_DEL,index,dwMoney);
		}
	}
	else
	{
		if (dwNumber > 1)
		{
			pTarget->SendSysMsg(_TXTATR_TOPSHOW,"摊位信息已改变，请刷新");
			return false;
		}
		ASSERT(pItemTemp = m_pUser->PopItem(idItem, SYNCHRO_TRUE));
		m_pUser->SynchroBoothItem(pItemTemp,ITEMACT_BOOTH_DEL,index,dwMoney);
		pTarget->SynchroBoothItem(pItemTemp,ITEMACT_BOOTH_DEL,index,dwMoney);
	}	
	
	BOOL bRet = FALSE;

	ASSERT(bRet = pTarget->SpendMoneyByType(dwRealMoney, 1, SYNCHRO_TRUE));
	ASSERT(bRet = m_pUser->GainMoney(max(1,dwRealMoney * 19 / 20), SYNCHRO_TRUE, 1));
	if (dwRealMoney>30000)
		::MyLogSave(VALUABLESLOG_FILENAME, "CBooth::BuyItem user:%u(%s)出售%d个%s给user:%u(%s),获得银两%d",m_pUser->GetID(),m_pUser->GetName(), dwNumber,pItem->GetStr(ITEMDATA_NAME), pTarget->GetID(), pTarget->GetName(), dwRealMoney);

	char szTemp[1024];
	char szMoney[1024] = "";
	int nMoney = dwRealMoney / 10000;
	if(nMoney > 0)
	{
		sprintf(szTemp,"%d金",nMoney);
		strcat(szMoney,szTemp);
	}
	nMoney = (dwRealMoney % 10000) / 100;
	if(nMoney > 0)
	{
		sprintf(szTemp,"%d银",nMoney);
		strcat(szMoney,szTemp);
	}
	nMoney = dwRealMoney % 100;
	if(nMoney > 0)
	{
		sprintf(szTemp,"%d铜",nMoney);
		strcat(szMoney,szTemp);
	}

	sprintf(szTemp,"<%s>购买%d个{%u_%u_%s}，总价：%d",pTarget->GetName(),dwNumber,pTarget->GetID(), pItem->GetID(),pItem->GetStr(ITEMDATA_NAME), dwRealMoney );
	m_pUser->SendSysMsg(_TXTATR_TOPSHOW,"[%s]花了%s购买你的%d个%s", pTarget->GetName(), szMoney, dwNumber, pItem->GetStr(ITEMDATA_NAME));
	pTarget->SendSysMsg(_TXTATR_TOPSHOW,"你花了%s购买[%s]的%d个%s", szMoney, m_pUser->GetName(), dwNumber, pItem->GetStr(ITEMDATA_NAME));
	AddLeaveWord("系统",szTemp,true,pTarget);
	if (pItemTemp && pItemTemp->IsCostlyItem())//goto 贵重物品记录
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) transfer %d %u to %u(%s)", m_pUser->GetID(), m_pUser->GetName(), CItem::IsCountable(pItemTemp->GetTypeID())?pItemTemp->GetItemAmount():1, pItemTemp->GetTypeID(), pTarget->GetID(), pTarget->GetName());
	
	

	DEBUG_TRY
	pItemTemp->SetPosition( 0,false );
	pItemTemp->SetPackageIndex( 0,false );
	pItemTemp->setLocked(false);
	pItemTemp->SetEquipAddLevFailRate(0);//[2010-07-20 goto]交易后装备强化的失败加成清零
	pItemTemp->SetEquipStarFailRate(0);//[2010-07-22 goto]交易后装备升星的失败加成清零

	pTarget->AddItem(pItemTemp, SYNCHRO_TRUE);
	DEBUG_CATCH("pTarget->AddItem()");

	DEBUG_CATCH("CBooth::BuyItem()")		// VVVVVVVVVVVVVVVVVVVVV
	return true;
}

//////////////////////////////////////////////////////////////////////
void CBooth::SendGoods(CUser* pTarget)
{
	CHECK(pTarget);
	CHECK(m_pUser);

	CMsgItem msg;
	bool bSend = false;
	if (!msg.Create(ITEMACT_ADD,0))
		return;

	for( int i = 0;i < m_setGoods.size();i++ )
	{
		OBJID	idItem		= m_setGoods[i].idItem;
		int		nCost		= m_setGoods[i].nMoney /** m_setGoods[i].dwNumber*/;

		if(idItem == ID_NONE)
			continue;

		CItem* pItem = m_pUser->GetItem(idItem,false);
		if(pItem)
		{
			bSend = true;
			if (!msg.Add(m_setGoods[i].idItem,m_setGoods[i].nType,m_setGoods[i].dwNumber,pItem->GetInt(ITEMDATA_AMOUNTLIMIT), /*pItem->GetPosition()*/200,pItem->GetKidnap(),m_setGoods[i].nindex,nCost,m_setGoods[i].dwColor))
				return;
		}
		else
		{
			DelItem(idItem);
		}
	}

	if(bSend)
	{
		pTarget->SendMsg(&msg);
	}

	bSend = false;
	CMsgEudemon msgEud;
	if(!msgEud.Create(PETACTION_AddBoothPet,NULL,m_pUser->GetID()))
		return;

	for(i = 0;i < m_setEudemon.size();i++ )
	{
		OBJID	idEudemon		= m_setEudemon[i].idEudemon;
		EudemonData stEudData;
		if(m_pUser->LoadEudemon(idEudemon,stEudData,2))
		{
			bSend = true;
			msgEud.Append(stEudData,m_setEudemon[i].nMoney);
		}
		else
		{
			DelEudemon(idEudemon);
		}
	}
	
	if(bSend)
	{
		pTarget->SendMsg(&msgEud);
	}

	if(pTarget->GetID() != m_pUser->GetID())
	{
		m_pUser->SendSysMsg(_TXTATR_TOPSHOW,"%s正在查看你的摊位",pTarget->GetName());
	}

	SendInfo(pTarget);
}

//////////////////////////////////////////////////////////////////////
void CBooth::EnterMap(int nPosX, int nPosY, int nDir)
{
	if(m_bEnterMap)
		return ;

	// overlap booth
	void* pTemp;
	if(m_pUser->GetMap()->QueryObjInPos(nPosX, nPosY, OBJ_BOOTH, IPP_OF(pTemp)))
		return ;

	// enter map
	m_nPosX		= nPosX;
	m_nPosY		= nPosY;
	m_nDir		= nDir;
	m_pMap->EnterRoom(QueryMapThing());

	CHECK(m_pUser);
	CMsgPlayer msg;
	if (msg.CreateBooth(GetID(),m_pMap->GetDocID(),m_nPosX,m_nPosY,m_szName,m_pUser->GetID()))
		m_pUser->BroadcastRoomMsg(&msg,INCLUDE_SELF);
//		m_pMap->BroadcastBlockMsg(QueryMapThing(), &msg);
	m_bEnterMap	= true;
}

//////////////////////////////////////////////////////////////////////
void CBooth::LeaveMap()
{
	if(!m_bEnterMap)
		return ;

	CMsgAction msg;
	if (msg.Create(GetID(), 0, 0, 0, actionDisappear, 2))
		m_pUser->BroadcastRoomMsg(&msg,INCLUDE_SELF);
//		m_pMap->BroadcastBlockMsg(QueryMapThing(), &msg);

	m_pMap->LeaveRoom(QueryMapThing());
	m_bEnterMap	= false;
}


void CBooth::SetCryOut(LPCTSTR szWords,bool bSend)
{
	SafeCopy(m_szCryOut,szWords,1024);
	if(bSend)
	{
		CMsgTalk msg;
		if (msg.Create(m_pUser->GetName(), ALLUSERS_NAME, m_szCryOut, NULL, 0x0ffffff, _TXTATR_BOOTHDES))
			m_pUser->SendMsg(&msg);
	}
}

void CBooth::ChangeName(LPCTSTR szName,bool bSend)
{
	SafeCopy(m_szName,szName,24);
	CHECK(m_pUser);
	if(bSend)
	{
		CMsgPlayer msg;
		if (msg.CreateBooth(GetID(),m_pMap->GetDocID(),m_nPosX,m_nPosY,m_szName,m_pUser->GetID()))
			m_pUser->BroadcastRoomMsg(&msg,INCLUDE_SELF);
//			m_pMap->BroadcastBlockMsg(QueryMapThing(), &msg);
	}
}

void CBooth::ClrLeaveWord()
{
	m_LeaveTop = 0;
	m_LeaveEnd = 0;
}

void CBooth::AddLeaveWord(LPCTSTR szSender,LPCTSTR szWord,bool bSend,CUser* pNeed)
{
	if(m_LeaveTop < 0 || m_LeaveTop > 19)
		return;
	if(m_LeaveEnd < 0 || m_LeaveEnd > 19)
		return;

	if(strcmp(szSender,"系统") == 0)
		sprintf(m_szLeaveWord[m_LeaveEnd],"%s：%s",szSender,szWord);
	else
		sprintf(m_szLeaveWord[m_LeaveEnd],"<%s>：%s",szSender,szWord);
	if(bSend)
	{
		CMsgTalk msg;
		if (msg.Create(m_pUser->GetName(), ALLUSERS_NAME, m_szLeaveWord[m_LeaveEnd], NULL, 0xffffff, _TXTATR_BOOTHLEAVEWORD))
			m_pUser->SendMsg(&msg);

		if(pNeed && pNeed != m_pUser)
			pNeed->SendMsg(&msg);
	}

	bool bFull = false;
	if((m_LeaveEnd == m_LeaveTop - 1) || (m_LeaveEnd == m_LeaveTop + 19))
		bFull = true;

	if(bFull)
	{
		m_LeaveEnd ++;
		m_LeaveTop  = m_LeaveEnd + 1;

		if(m_LeaveEnd >= 20)
			m_LeaveEnd -= 20;
		if(m_LeaveTop >= 20)
			m_LeaveTop -= 20;
	}
	else
	{
		m_LeaveEnd ++;
	}
}

void CBooth::SendInfo(CUser* pUser)
{
	SendCryOut(pUser);
	SendLeaveWord(pUser);
}

void CBooth::SendLeaveWord(CUser* pUser)
{
	if(m_LeaveTop == m_LeaveEnd)
		return;
	if(m_LeaveTop < 0 || m_LeaveTop > 19)
		return;
	if(m_LeaveEnd < 0 || m_LeaveEnd > 19)
		return;
	CMsgTalk msg;
	for(int i = m_LeaveTop ; i != m_LeaveEnd ; )
	{
		if (msg.Create(m_pUser->GetName(), ALLUSERS_NAME, m_szLeaveWord[i], NULL, 0xffffff, _TXTATR_BOOTHLEAVEWORD))
			pUser->SendMsg(&msg);

		i++;
		if(i > 19)
			i = 0;
	}
}

//////////////////////////////////////////////////////////////////////
void CBooth::SendCryOut(CUser* pUser)
{
	if(strlen(m_szCryOut))
	{
		CMsgTalk msg;
		IF_OK_ (msg.Create(m_pUser->GetName(), ALLUSERS_NAME, m_szCryOut, NULL, 0xffffff, _TXTATR_BOOTHDES))
			pUser->SendMsg(&msg);			
	}
}

void CBooth::SetUserPoint(CUser *pUser)//[游途道标 2009.05.12]
{
	m_pUser = pUser;
}

bool CBooth::IsHaveItem(OBJID idItem)
{
	for (int i = 0; i < m_setGoods.size(); ++i)
	{
		if(m_setGoods[i].idItem == idItem)
		{
			return true;
		}
	}
	return  false;
}

void CBooth::Clear()
{
	m_setGoods.clear();
	m_setEudemon.clear();
	return;	
}
