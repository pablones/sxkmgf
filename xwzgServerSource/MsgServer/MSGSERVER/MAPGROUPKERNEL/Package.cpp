// Package.cpp: implementation of the CPackage class.
//
//////////////////////////////////////////////////////////////////////

#include "Network/AllMsg.h"
#include "Package.h"
#include "User.h"
#include "MapGroup.h"
//#include "mapgroup.h"
MYHEAP_IMPLEMENTATION(CPackage,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPackage::CPackage()
{
	m_nType		= _STORE_NONE;
	m_setItem	= NULL;
}

//////////////////////////////////////////////////////////////////////
CPackage::~CPackage()
{
	if(m_setItem)
		m_setItem->Release();
}

//////////////////////////////////////////////////////////////////////
//goto �޸�Ϊ����ʱ����һ�Σ����治�ٴ���
bool CPackage::Create(int nType, OBJID idOwner, OBJID idPlayer, int nIndex, IDatabase* pDb,bool bLogin)		// ���ظ�����
{
	CHECKF(nType != _STORE_NONE);

	m_nType		= nType;
	m_idOwner	= idOwner;
	m_idPlayer	= idPlayer;

	// add by zlong 2003-11-24 ---- �ж�ucType
	switch (m_nType)
	{
	case ITEMPOSITION_STORAGE:
		m_ucType	= MSGPACKAGE_TYPE_STORAGE;
		break;
	case ITEMPOSITION_TRUNK:
		m_ucType	= MSGPACKAGE_TYPE_TRUNK;
		break;
	case ITEMPOSITION_CHEST:
		m_ucType	= MSGPACKAGE_TYPE_CHEST;
		break;
/*	case ITEMPOSITION_MAGICSTONEBOX:
		m_ucType	= PACKAGE_MAGICSTONEBOX;
		break;*/
	default:
		m_ucType = MSGPACKAGE_TYPE_NONE;
		break;
	};      
	
	
	if(m_setItem)
		m_setItem->Release();
	m_setItem	= CItemSet::CreateNew(true);
	CHECKF(m_setItem);
	if(!bLogin)
		return false;
	
	int	nPosition = m_nType;		// ��ͬ
	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE owner_id=%u && player_id=%u && position=%d LIMIT %d",
					_TBL_ITEM, idOwner, idPlayer, nPosition, PACKAGE_LIMIT*3);
	IRecordset* pRes = pDb->CreateNewRecordset(szSQL, false);
	if(!pRes)
		return true;
	for(int i = 0; i < pRes->RecordCount(); i++,pRes->MoveNext())
	{
		CItemPtr pItem = CItem::CreateNew();
		ASSERT(pItem);
		if(!pItem)
			continue;
		
		if(pItem->Create(pRes, pDb))
		{
			m_setItem->AddObj(pItem);
			// m_setItemEx->AddObj(pItem);
		}
		else
			pItem->ReleaseByOwner();
	}
	pRes->Release();
	
	return true;
}
//////////////////////////////////////////////////////////////////////
bool CPackage::IsEmpty(int nIndex)
{
	for(int i=0; i<m_setItem->GetAmount(); i++)
	{
		CItemPtr pItem = m_setItem->GetObjByIndex(i);
		if (pItem->GetPackageIndex()==nIndex)
			return false;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////
CItem* CPackage::GetItemByIndex(int index)
{
	for(int i=0;i<m_setItem->GetAmount();i++)
	{
		CItemPtr pItem = m_setItem->GetObjByIndex(i);
		if (pItem->GetPackageIndex()==index)
			return m_setItem->GetObjByIndex(i);
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////
bool CPackage::DelItem(CUser* pUser, CItem* pItem)
{
	m_setItem->PopObj(pItem->GetID());
	pItem->DeleteRecord(::MapGroup(pUser->GetProcessID())->GetDatabase(), SYNCHRO_TRUE);
	pItem->ReleaseByOwner();
	return true;
}

bool CPackage::AddItem( CItem* pItem)
{
	CHECKF(m_nType != _STORE_NONE);
	CHECKF(pItem);

	m_setItem->AddObj(pItem);
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CPackage::CheckIn(CUser* pUser, CItem* pItem, int nIndex, bool bUpdate /*= true*/)
{
	DEBUG_TRY
	CHECKF(m_nType != _STORE_NONE);
	CHECKF(pUser);
	CHECKF(pItem);	

	bool bIsDefault = false;//�Ƿ���˫����Ʒ�洢
	CItem* pTargetItem = NULL;//Ŀ��λ�õ���Ʒ
	int nPackageSize = PACKAGE_ORIGIN+pUser->GetBanklev(1)*6;

	if(m_nType == ITEMPOSITION_CHEST && !pItem->IsExchangeEnable())
	{                              
		//		pUser->SendSysMsg("���󣺴���Ʒ���˴��!");
		return false;
	}
	if (nIndex>=PACKAGE_DEFAULTINDEX && nIndex<=PACKAGE_DEFAULTINDEX+3)//�����˫����Ʒʱ�Ĵ���,�ֿ����ֻ��3ҳ
	{
		bIsDefault = true;
		bool bHasFound = false;
		int nPage = nIndex%10;
		int nMaxIndex = nPage*PACKAGE_LIMIT;//[2010-06-16 goto]����ҳ�����ֿ���һҳ���ӱ������
		int nMinIndex = nMaxIndex+1-PACKAGE_LIMIT;//[2010-06-16 goto]����ҳ�����ֿ���һҳ���ӱ������

		if (nMinIndex>nPackageSize)
			return false;

		if (CItem::IsCountable(pItem->GetTypeID()))
		{
			for (int i=nMinIndex; i<=__min(nMaxIndex,nPackageSize); i++)//����û�п��Ժϲ�����Ʒ
			{
				pTargetItem = GetItemByIndex(i);
				if (pTargetItem && pItem->GetInt(ITEMDATA_TYPE)==pTargetItem->GetInt(ITEMDATA_TYPE)
					&& pItem->GetItemAmount() + pTargetItem->GetItemAmount() <= pTargetItem->GetItemAmountLimit()
					&& pTargetItem->GetKidnap()==pItem->GetKidnap())//�󶨵ĺ�δ�󶨵Ĳ��ܵ���
				{
					nIndex = i;
					bHasFound = true;
					break;
				}
			}
		}
		if (!bHasFound)
		{
			pTargetItem = NULL;
			for (int i=nMinIndex; i<=__min(nMaxIndex,nPackageSize); i++)
			{
				if (IsEmpty(i))
				{
					nIndex=i;
					break;
				}
			}
		}
	}
	if (nIndex>=PACKAGE_DEFAULTINDEX && nIndex<=PACKAGE_DEFAULTINDEX+3)//��ǰҳû�п�λ
	{
		pUser->SendSysMsg(_TXTATR_TOPSHOW, "�ֿ⵱ǰҳ����");
		return false;
	}
	if (nIndex>nPackageSize)
		return false;
	OBJID idItem=pItem->GetID();
	if (!bIsDefault)
		pTargetItem = GetItemByIndex(nIndex);
	if (pTargetItem)//Ŀ��λ������Ʒʱ���ϲ����߽���λ��
	{
		CHECKF(pItem->GetID() != pTargetItem->GetID());
		if (pItem->GetInt(ITEMDATA_TYPE)==pTargetItem->GetInt(ITEMDATA_TYPE)
			&& CItem::IsCountable(pTargetItem->GetTypeID())
			&& pItem->GetKidnap()==pTargetItem->GetKidnap()
			&& pItem->GetItemAmount() + pTargetItem->GetItemAmount() <= pTargetItem->GetItemAmountLimit())//�ϲ�
		{
			int nIndex1 = pItem->GetPackageIndex();
			int nIndex2 = pTargetItem->GetPackageIndex();

			int nPosition1 = pItem->GetPosition();
			int nPosition2 = pTargetItem->GetPosition();
// 			if (pItem->GetItemAmount() + pTargetItem->GetItemAmount() <= pTargetItem->GetItemAmountLimit())
// 			{
				pTargetItem->SetInt(ITEMDATA_AMOUNT, pItem->GetItemAmount() + pTargetItem->GetItemAmount());
				//�ֿ���Ʒ�������浵
				pTargetItem->SaveInfo();
				
				CMsgPackage msg;
				msg.Create(MSGPACKAGE_CHECKOUT,pTargetItem->GetID(),pTargetItem->GetInt(ITEMDATA_TYPE),pTargetItem->GetItemAmount(),pTargetItem->GetItemAmountLimit(),nIndex2,MSGPACKAGE_TYPE_STORAGE,pTargetItem->GetKidnap());
				pUser->SendMsg(&msg);

				msg.Create(MSGPACKAGE_CHECKIN,pTargetItem->GetID(),pTargetItem->GetInt(ITEMDATA_TYPE),pTargetItem->GetItemAmount(),pTargetItem->GetItemAmountLimit(),nIndex2,MSGPACKAGE_TYPE_STORAGE,pTargetItem->GetKidnap());
				pUser->SendMsg(&msg);

				CMsgItem msgItem;
				if(msgItem.Create( ITEMACT_DROP,0,0,0,0,idItem ))
					pUser->SendMsg(&msgItem);
				pUser->QueryPackage()->PopItem(idItem);
				pItem->DeleteRecord(::MapGroup(pUser->GetProcessID())->GetDatabase(), SYNCHRO_TRUE);
				pItem->ReleaseByOwner();
				return true;
//			}
// 			else
// 			{
// 				int nMoveSize = pTargetItem->GetItemAmountLimit() - pTargetItem->GetItemAmount();
// 				pTargetItem->SetInt(ITEMDATA_AMOUNT, pTargetItem->GetItemAmountLimit(), true);
// 				pItem->SetInt(ITEMDATA_AMOUNT, pItem->GetItemAmount() - nMoveSize, true);
// 				
// 				CMsgPackage msg;
// 				msg.Create(MSGPACKAGE_CHECKOUT,pTargetItem->GetID(),pTargetItem->GetInt(ITEMDATA_TYPE),pTargetItem->GetItemAmount(),pTargetItem->GetItemAmountLimit(),nIndex2,MSGPACKAGE_TYPE_STORAGE,pTargetItem->GetKidnap());
// 				pUser->SendMsg(&msg);
// 
// 				msg.Create(MSGPACKAGE_CHECKIN,pTargetItem->GetID(),pTargetItem->GetInt(ITEMDATA_TYPE),pTargetItem->GetItemAmount(),pTargetItem->GetItemAmountLimit(),nIndex2,MSGPACKAGE_TYPE_STORAGE,pTargetItem->GetKidnap());
// 				pUser->SendMsg(&msg);
// 
// 				CMsgItem	msgItem;
// 				if(msgItem.Create(ITEMACT_SYNCHRO_AMOUNT,0,0,0,0,pItem->GetID(),0,pItem->GetItemAmount()))
// 					pUser->SendMsg(&msgItem);
// 				pUser->QueryPackage()->PopItem(idItem);
// 				CMsgItem msgItem;
// 				if(msgItem.Create( ITEMACT_DROP,0,0,0,0,idItem ))
// 					pUser->SendMsg(&msgItem);
// 				
// 				msgItem.Create(ITEMACT_ADD,0);
// 				if (msgItem.Add(pItem->GetID(),pItem->GetTypeID(),pItem->GetItemAmount(),pItem->GetInt(ITEMDATA_AMOUNTLIMIT), pItem->GetPosition(),pItem->GetKidnap(),pItem->GetPackageIndex()))
// 					pUser->SendMsg(&msgItem);
// 				pUser->QueryPackage()->AddItemToPosition(pUser, pItem, nPosition1,nIndex1);
//			}
 		}
		else if (pItem->GetInt(ITEMDATA_TYPE)!=pTargetItem->GetInt(ITEMDATA_TYPE)
				|| pItem->GetKidnap()!=pTargetItem->GetKidnap())//����λ��
		{
			int nPosition = pUser->QueryPackage()->GetPositionByItemType(pTargetItem->GetTypeID());
			if (pUser->QueryPackage()->IsPackFull(1,0, nPosition))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "����ı���ҳ����,�޷�����λ��");
				return false;
			}
			if (!pUser->QueryPackage()->PopItem(idItem))
				return false;			

			int nIndex1 = pItem->GetPackageIndex();
			int nIndex2 = pTargetItem->GetPackageIndex();

			int nPosition1 = pItem->GetPosition();
			int nPosition2 = pTargetItem->GetPosition();
			
			pItem->SetPosition(nPosition2);
			pItem->SetPackageIndex(nIndex2);
			
			pTargetItem->SetPosition(nPosition1);
			pTargetItem->SetPackageIndex(1);
			CMsgPackage msg;
			msg.Create(MSGPACKAGE_CHECKOUT,pTargetItem->GetID(),pTargetItem->GetInt(ITEMDATA_TYPE),pTargetItem->GetItemAmount(),pTargetItem->GetItemAmountLimit(),nIndex2,MSGPACKAGE_TYPE_STORAGE,pTargetItem->GetKidnap());
			pUser->SendMsg(&msg);
			m_setItem->PopObj(pTargetItem->GetID());

			msg.Create(MSGPACKAGE_CHECKIN,idItem,pItem->GetInt(ITEMDATA_TYPE),pItem->GetItemAmount(),pItem->GetItemAmountLimit(),nIndex2,MSGPACKAGE_TYPE_STORAGE,pItem->GetKidnap());
			pUser->SendMsg(&msg);
	
			m_setItem->AddObj(pItem);
			//��ֿ��浵	
			pItem->SaveInfo();

			if (!pUser->QueryPackage()->AddItem(pTargetItem))
				return false;
			//���뱳������Ʒ�ɲ�������
			if(bUpdate)
				pTargetItem->SaveInfo();

			CMsgItem msgItem;
			if(msgItem.Create( ITEMACT_DROP,0,0,0,0,idItem ))
				pUser->SendMsg(&msgItem);
			
			msgItem.Create(ITEMACT_ADD,0);
			if (msgItem.Add(pTargetItem->GetID(),pTargetItem->GetTypeID(),pTargetItem->GetItemAmount(),
						pTargetItem->GetInt(ITEMDATA_AMOUNTLIMIT), pTargetItem->GetPosition(),pTargetItem->GetKidnap(),pTargetItem->GetPackageIndex(),0,pTargetItem->GetColour()))
				pUser->SendMsg(&msgItem);
// 			pUser->QueryPackage()->AddItemToPosition(pUser, pTargetItem, nPosition1,nIndex1);
			return true;
		}
		else
			return false;
	}
	
	
	int		nPosition = m_nType;		// ��ͬ
	OBJID	idPlayer = (IsPlayerPackage() ? pUser->GetID() : ID_NONE);
// 	pItem->SetInt(ITEMDATA_OWNERID, m_idOwner, UPDATE_FALSE);//[2010-06-16 goto]ֻ�ı�λ�ã����ı�owner
// 	pItem->SetInt(ITEMDATA_PLAYERID, idPlayer, UPDATE_FALSE);//[2010-06-16 goto]ֻ�ı�λ�ã����ı�owner
// 	pItem->SetInt(ITEMDATA_POSITION, nPosition, bUpdate);
// 	pItem->SetInt( ITEMDATA_PACKAGE_INDEX,nIndex,UPDATE_TRUE );//06.9.22�������
	if (!IsEmpty(nIndex))
		return false;
	pUser->QueryPackage()->PopItem(idItem);

	pItem->SetPosition(nPosition);
	pItem->SetPackageIndex(nIndex);

	//��ֿ��浵
	m_setItem->AddObj(pItem);
	pItem->SaveInfo();
	
	CMsgPackage msgPackage;
	msgPackage.Create(MSGPACKAGE_CHECKIN,
		pItem->GetID(),
		pItem->GetTypeID(),
		pItem->GetItemAmount(),
		pItem->GetItemAmountLimit(),
		pItem->GetPackageIndex(),
		MSGPACKAGE_TYPE_STORAGE,
		pItem->GetKidnap());//isbind
	pUser->SendMsg(&msgPackage);
	
	
	CMsgItem msg;
	if(msg.Create( ITEMACT_DROP,0,0,0,0,idItem ))
		pUser->SendMsg(&msg);

	//[��;���� 2009.04.25]������Ժϲ��ͺϲ���,��ȻЧ�ʵ���
// 	if (CItem::IsCountable(pItem->GetTypeID()))
// 	{
// 		for (int i = 0; i < m_setItem->GetAmount(); ++i)
// 		{
// 			CItem *pOldItem = m_setItem->GetObjByIndex(i);
// 			if (pOldItem->GetTypeID() == pItem->GetTypeID() 
// 				&& pOldItem->GetKidnap() == pItem->GetKidnap()
// 				&& pOldItem->GetInt(ITEMDATA_AMOUNT) + pItem->GetInt(ITEMDATA_AMOUNT) <= pItem->GetInt(ITEMDATA_AMOUNTLIMIT))
// 			{
// 				m_setItem->PopObj(pOldItem->GetID());
// 				CMsgPackage msg;
// 				CHECKF(msg.Create(MSGPACKAGE_CHECKOUT, m_idOwner, pOldItem->GetID(), m_ucType));
// 					pUser->SendMsg(&msg);
// 				pItem->SetInt(ITEMDATA_AMOUNT, pOldItem->GetInt(ITEMDATA_AMOUNT) + pItem->GetInt(ITEMDATA_AMOUNT), true);
// 				pOldItem->DeleteRecord(::MapGroup(pUser->GetProcessID())->GetDatabase(), SYNCHRO_TRUE);
// 				pOldItem->ReleaseByOwner();
// 				break;
// 			}
// 		}
// 	}

	// synchro
// 	MsgPackageItemInfo	info;
// 	info.id		= pItem->GetID();
// 	info.dwType	= pItem->GetInt(ITEMDATA_TYPE);
// 	info.usAmount	= pItem->GetInt(ITEMDATA_AMOUNT);
// 	info.usAmountLimit	= pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
// 	info.ucIdent	= pItem->GetInt(ITEMDATA_IDENT);
// 	info.GemTypeID[ 0 ] = pItem->GetInt(ITEMDATA_GEM1);
// 	info.GemTypeID[ 1 ] = pItem->GetInt(ITEMDATA_GEM2);
// 	info.GemTypeID[ 2 ] = pItem->GetInt(ITEMDATA_GEM3);
// 	info.BasicAdditionID1 = pItem->GetInt( ITEMDATA_BASIC_ADDITION1 );
// 	info.BasicAdditionID2 = pItem->GetInt( ITEMDATA_BASIC_ADDITION2 );
// 	info.BasicAdditionID3 = pItem->GetInt( ITEMDATA_BASIC_ADDITION3 );//[��;���� 2008/11/10]
	
// 	info.nData			= pItem->GetInt(ITEMDATA_DATA);
//     info.ucSpecialEffect=  pItem->GetInt(ITEMDATA_SPECIAL_EFFECT);
// 	info.usLevel		= pItem->GetInt( ITEMDATA_LEVEL );
// 	info.dwExp			= pItem->GetInt( ITEMDATA_EXP );
// 	info.ucAttrib		= pItem->GetInt( ITEMDATA_ATTRIBUTE );
// 	
// 	info.usMinPhysicalAttack	= pItem->GetInt( ITEMDATA_MIN_ATTACK );
// 	info.usMaxPhysicalAttack	= pItem->GetInt( ITEMDATA_MAX_ATTACK );
// 	info.usMinMagicAttack	= pItem->GetInt( ITEMDATA_MIN_MAGICATTACK );
// 	info.usMaxMagicAttack	= pItem->GetInt( ITEMDATA_MAX_MAGICATTACK );
// 	info.usStrengthanCount	= pItem->GetInt( ITEMDATA_STRENGTHAN_COUNT );	//ǿ������ +��
// 	info.usPhysicalDefence	= pItem->GetInt( ITEMDATA_NEW_PHYSICALDEF );	//�������
// 	info.usMagicDefence		= pItem->GetInt( ITEMDATA_NEW_MAGICDEF );			//ħ������
// 	info.ucEmbedCountLimit	= pItem->GetInt( ITEMDATA_EMBEDCOUNT_LIMIT );		//����Ƕ�������ƣ��������6
// 	info.usCapability		= pItem->GetInt( ITEMDATA_CAPABILITY );			//���������
// 	info.usEmbedPos			= pItem->GetInt( ITEMDATA_EMBED_POS );				//��ʯ����Ƕλ��
// 	SafeCopy(info.szName,pItem->GetStr(ITEMDATA_FORGENAME),16);			//  [5/24/2007] ����������...halei
	// modified by zlong 2003-11-24 ---- ָ��ucType���
	
	//  [2007-10-9] ��� �Ƿ񼤻�
// 	if(strlen(pItem->GetLastCheckTime()) == 0 || stricmp(pItem->GetLastCheckTime(), "NULL") == 0)
// 		info.bIsActivated = FALSE;
// 	else
// 		info.bIsActivated = TRUE;

	//  [2007-10-9] ��� ��������
/*	info.uLifeTime = pItem->GetLifeTime();*/

	//  [3/4/2008 ���]
// 	info.WPSL_State = pItem->GetStateWPSL();
// 	info.WPSL_ID1 = pItem->GetIdWPSLByIndex(0);
// 	info.WPSL_ID2 = pItem->GetIdWPSLByIndex(1);
// 	info.WPSL_ID3 = pItem->GetIdWPSLByIndex(2);
// 	info.WPSL_ID4 = pItem->GetIdWPSLByIndex(3);

// 	CMsgPackage msg;
// 	CHECKF(msg.Create(MSGPACKAGE_CHECKIN, m_idOwner, &info, 1, m_ucType));
// 	pUser->SendMsg(&msg);
	DEBUG_CATCH("CPackage::CheckIn ERROR!")
		return true;
}
//----------------------------------------------------------------------
//CItem*  CPackage::CheckOut(OBJID idPlayer, OBJID idItem, bool bUpdate)
//LW 2005-01-12
//�����ȡ������ֿ����Ʒʱ��ɾ���ڣΣ������ֿ��е�Item�ڴ����������
//�������ȡ����Ʒʱ���޸ģ����Բ��ò������ݿ�
//----------------------------------------------------------------------
// CItem*	CPackage::CheckOut(OBJID idItem)
// {
// 	CItemPtr pItem = m_setItem->GetObj(idItem);
// 	if(!pItem)
// 		return false;
// 	m_setItem->PopObj(idItem);
// 	return pItem;
// }
//////////////////////////////////////////////////////////////////////
CItem* CPackage::CheckOut(CUser* pUser, OBJID idItem, int nIndex, bool bUpdate)
{
	CHECKF(pUser);
	CHECKF(m_nType != _STORE_NONE);
	
	CItemPtr pItem = m_setItem->GetObj(idItem);
	if(!pItem)
		return NULL;

	int nPosition = (nIndex-1)/PACKAGE_LIMIT + ITEMPOSITION_PACK_BEGIN;
	int nPackageIndex = nIndex%PACKAGE_LIMIT ? nIndex%PACKAGE_LIMIT : PACKAGE_LIMIT;
// 	pItem->SetInt(ITEMDATA_OWNERID, pUser->GetID(), UPDATE_FALSE);
// 	pItem->SetInt(ITEMDATA_PLAYERID, pUser->GetID(), UPDATE_FALSE);
	CHECKF(nPosition>=ITEMPOSITION_PACK_BEGIN && nPosition<=ITEMPOSITION_PACK_END);
	CHECKF(nPackageIndex>=1 && nPackageIndex<=36);

	CItem* pTargetItem = pUser->QueryPackage()->GetItemByPackageIndex(nPosition, nPackageIndex);
	if (pTargetItem)
	{
		CHECKF(pTargetItem->GetID() != pItem->GetID());
		if (pItem->GetInt(ITEMDATA_TYPE)==pTargetItem->GetInt(ITEMDATA_TYPE)
			&& CItem::IsCountable(pTargetItem->GetTypeID())
			&& pItem->GetKidnap()==pTargetItem->GetKidnap()
			&& pItem->GetItemAmount() + pTargetItem->GetItemAmount() <= pTargetItem->GetItemAmountLimit())//�ϲ�
		{
			//�����װ���Ļ�������type��ͬ���������Բ�ͬ����Ȼ��Ҫ����
// 			if (pTargetItem->GetItemAmount()==pTargetItem->GetItemAmountLimit())
// 				return pItem;
			int nIndex1 = pItem->GetPackageIndex();
			int nIndex2 = pTargetItem->GetPackageIndex();
			int nPosition1 = pItem->GetPosition();
			int nPosition2 = pTargetItem->GetPosition();

// 			if (pItem->GetItemAmount() + pTargetItem->GetItemAmount() <= pTargetItem->GetItemAmountLimit())//û�дﵽ�ϲ�����
// 			{
				pTargetItem->SetInt(ITEMDATA_AMOUNT, pItem->GetItemAmount() + pTargetItem->GetItemAmount());
				//������Ʒ
				if(bUpdate)
					pTargetItem->SaveInfo();
				CMsgItem	msgItem;
				if(msgItem.Create(ITEMACT_SYNCHRO_AMOUNT,0,0,0,0,pTargetItem->GetID(),0,pTargetItem->GetItemAmount()))
 					pUser->SendMsg(&msgItem);
				
// 				pUser->QueryPackage()->PopItem(pTargetItem->GetID());
// 				CMsgItem msgItem;
// 				if(msgItem.Create( ITEMACT_DROP,0,0,0,0,pTargetItem->GetID() ))
// 					pUser->SendMsg(&msgItem);
// 				
// 				pUser->QueryPackage()->AddItemToPosition(pUser, pTargetItem, nPosition2,nIndex2);
// 				msgItem.Create(ITEMACT_ADD,0);
// 				if (msgItem.Add(pTargetItem->GetID(),pTargetItem->GetTypeID(),pTargetItem->GetItemAmount(),
// 					pTargetItem->GetInt(ITEMDATA_AMOUNTLIMIT), pTargetItem->GetPosition(),pTargetItem->GetKidnap(),pTargetItem->GetPackageIndex()))
// 					pUser->SendMsg(&msgItem);
				
				CMsgPackage msg;
				if (msg.Create(MSGPACKAGE_CHECKOUT,pItem->GetID(),pItem->GetInt(ITEMDATA_TYPE),pItem->GetItemAmount(),pItem->GetItemAmountLimit(),nIndex1,MSGPACKAGE_TYPE_STORAGE,pItem->GetKidnap()))
					pUser->SendMsg(&msg);
				m_setItem->PopObj(idItem);
				pItem->DeleteRecord(::MapGroup(pUser->GetProcessID())->GetDatabase(), SYNCHRO_TRUE);
				pItem->ReleaseByOwner();
				return pTargetItem;
//			}
// 			else//�����ϲ�����
// 			{
// 				int nMoveSize = pTargetItem->GetItemAmountLimit() - pTargetItem->GetItemAmount();
// 				pTargetItem->SetInt(ITEMDATA_AMOUNT, pTargetItem->GetItemAmountLimit(), true);
// 				pItem->SetInt(ITEMDATA_AMOUNT, pItem->GetItemAmount() - nMoveSize, true);
// 				
// 				CMsgPackage msg;
// 				msg.Create(MSGPACKAGE_CHECKOUT,pItem->GetID(),pItem->GetInt(ITEMDATA_TYPE),pItem->GetItemAmount(),pItem->GetItemAmountLimit(),nIndex1,MSGPACKAGE_TYPE_STORAGE,pItem->GetKidnap());
// 				pUser->SendMsg(&msg);
// 				
// 				msg.Create(MSGPACKAGE_CHECKIN,pItem->GetID(),pItem->GetInt(ITEMDATA_TYPE),pItem->GetItemAmount(),pItem->GetItemAmountLimit(),nIndex1,MSGPACKAGE_TYPE_STORAGE,pItem->GetKidnap());
// 				pUser->SendMsg(&msg);
// 
// 				CMsgItem msgItem;
// 				if(msgItem.Create( ITEMACT_DROP,0,0,0,0,pTargetItem->GetID() ))
// 					pUser->SendMsg(&msgItem);
// 				pUser->QueryPackage()->PopItem(pTargetItem->GetID());
// 				
// 				msgItem.Create(ITEMACT_ADD,0);
// 				if (msgItem.Add(pTargetItem->GetID(),pTargetItem->GetTypeID(),pTargetItem->GetItemAmount(),
// 					pTargetItem->GetInt(ITEMDATA_AMOUNTLIMIT), pTargetItem->GetPosition(),pTargetItem->GetKidnap(),pTargetItem->GetPackageIndex()))
// 					pUser->SendMsg(&msgItem);
// 				pUser->QueryPackage()->AddItemToPosition(pUser, pTargetItem, nPosition2,nIndex2);
// 			}
		}
// 		else//���ܺϲ�,�ұ�����λ�÷���
// 		{
// 			int nIndex1 = pItem->GetPackageIndex();
// 			int nPosition1 = pItem->GetPosition();
// 
// 			int nIndex2 = pTargetItem->GetPackageIndex();
// 			int nPosition2 = pTargetItem->GetPosition();
// 			
// 			pItem->SetPosition(nPosition2,true);
// 			pItem->SetPackageIndex(nIndex2,true);
			
// 			pTargetItem->SetPosition(nPosition1, true);
// 			pTargetItem->SetPackageIndex(nIndex1, true);
// 			CMsgPackage msg;
// 			msg.Create(MSGPACKAGE_CHECKOUT,pItem->GetID(),pItem->GetInt(ITEMDATA_TYPE),pItem->GetItemAmount(),pItem->GetItemAmountLimit(),nIndex1,MSGPACKAGE_TYPE_STORAGE,0);
// 			pUser->SendMsg(&msg);
// 			m_setItem->PopObj(pItem->GetID());
// 			
// 			msg.Create(MSGPACKAGE_CHECKIN,pTargetItem->GetID(),pTargetItem->GetInt(ITEMDATA_TYPE),pTargetItem->GetItemAmount(),pTargetItem->GetItemAmountLimit(),nIndex1,MSGPACKAGE_TYPE_STORAGE,0);
// 			pUser->SendMsg(&msg);
// 			m_setItem->AddObj(pTargetItem);
// 			
// 			CMsgItem msgItem;
// 			if(msgItem.Create( ITEMACT_DROP,0,0,0,0,pTargetItem->GetID() ))
// 				pUser->SendMsg(&msgItem);
// 			pUser->QueryPackage()->PopItem(pTargetItem->GetID());

// 			msgItem.Create(ITEMACT_ADD,0);
// 			msgItem.Add(pItem->GetID(),pItem->GetTypeID(),pItem->GetItemAmount(),pItem->GetInt(ITEMDATA_AMOUNTLIMIT), pItem->GetPosition(),pItem->GetKidnap(),pItem->GetPackageIndex());
// 			pUser->SendMsg(&msgItem);
// 			pUser->QueryPackage()->AddItemToPosition(pUser, pItem, nPosition2,nIndex2);
			
// 			if (pUser->QueryPackage()->AddItem(pItem, SYNCHRO_TRUE))
// 			{
// 				CMsgPackage msg;
// 				msg.Create(MSGPACKAGE_CHECKOUT,pItem->GetID(),pItem->GetInt(ITEMDATA_TYPE),pItem->GetItemAmount(),pItem->GetItemAmountLimit(),nIndex1,MSGPACKAGE_TYPE_STORAGE,0);
// 				pUser->SendMsg(&msg);
// 				m_setItem->PopObj(pItem->GetID());
// 			}
// 			else//���뱳��ʧ��,��λ�øĻ���
// 			{
// 				pItem->SetPosition(nPosition1,true);
// 				pItem->SetPackageIndex(nIndex1,true);
// 			}
// 			return pItem;
// 		}
	}
	
	//if (!pUser->QueryPackage()->AddItemToPosition(pUser ,pItem, nPosition, nPackageIndex, bUpdate))
	int nOldPosition = pItem->GetPosition();
	int nOldIndex    = pItem->GetPackageIndex();
	if (nPosition != pUser->QueryPackage()->GetPositionByItemType(pItem->GetTypeID()))
	{
		nPosition = pUser->QueryPackage()->GetPositionByItemType(pItem->GetTypeID());
		nPackageIndex = 1;
	}

	if(pUser->QueryPackage()->IsPackFull(1,0,nPosition))
	{
		pUser->SendSysMsg("��������");
		return NULL;
	}

//		nPackageIndex=1;
	pItem->SetPosition( nPosition );
	pItem->SetPackageIndex( nPackageIndex);
	if (!pUser->QueryPackage()->AddItem(pItem, SYNCHRO_TRUE))
	{
		LOGWARNING("�ֿ�ȡ����Ʒʧ�ܣ�userid = %u",pUser->GetID());
 		pItem->SetPosition(nOldPosition);
 		pItem->SetPackageIndex(nOldIndex);
		//�ֿ���Ʒ�������浵
		pItem->SaveInfo();
		return NULL;
 	}

	//������Ʒ
	if(bUpdate)
		pItem->SaveInfo();

	CMsgPackage msgPackage;
	msgPackage.Create(MSGPACKAGE_CHECKOUT,
		pItem->GetID(),
		pItem->GetTypeID(),
		pItem->GetItemAmount(),
		pItem->GetItemAmountLimit(),
		nOldIndex,//pItem->GetPackageIndex(),
		MSGPACKAGE_TYPE_STORAGE,
		pItem->GetKidnap());//isbind
	pUser->SendMsg(&msgPackage);

	CMsgItem msg;
	if (msg.Create(ITEMACT_ADD,0)
		&& msg.Add(pItem->GetID(),pItem->GetTypeID(),pItem->GetItemAmount(),pItem->GetInt(ITEMDATA_AMOUNTLIMIT), pItem->GetPosition(),pItem->GetKidnap(),pItem->GetPackageIndex(),0,pItem->GetColour()))
		pUser->SendMsg(&msg);

	m_setItem->PopObj(idItem);
	// modified by zlong 2003-11-24 ---- ָ��ucType���
	// synchro
// 	CMsgPackage msg;
// 	CHECKF(msg.Create(MSGPACKAGE_CHECKOUT, m_idOwner, idItem, m_ucType));
// 	pUser->SendMsg(&msg);
	return pItem;
}
//////////////////////////////////////////////////////////////////////
//�ֿ�����Ʒ����λ��
bool CPackage::ExChangeIndex(CUser* pUser, CItem* pItem1, CItem* pItem2)
{
	CHECKF(pUser);
	int nIndex1 = pItem1->GetPackageIndex();
	int nIndex2 = pItem2->GetPackageIndex();

	pItem1->SetPackageIndex(nIndex2);
	pItem2->SetPackageIndex(nIndex1);
	CMsgPackage msg;
	msg.Create(MSGPACKAGE_CHECKOUT,pItem1->GetID(),pItem1->GetInt(ITEMDATA_TYPE),pItem1->GetItemAmount(),pItem1->GetItemAmountLimit(),nIndex1,MSGPACKAGE_TYPE_STORAGE,pItem1->GetKidnap());
	pUser->SendMsg(&msg);

	msg.Create(MSGPACKAGE_CHECKOUT,pItem2->GetID(),pItem2->GetInt(ITEMDATA_TYPE),pItem2->GetItemAmount(),pItem2->GetItemAmountLimit(),nIndex2,MSGPACKAGE_TYPE_STORAGE,pItem2->GetKidnap());
	pUser->SendMsg(&msg);

	msg.Create(MSGPACKAGE_CHECKIN,pItem1->GetID(),pItem1->GetInt(ITEMDATA_TYPE),pItem1->GetItemAmount(),pItem1->GetItemAmountLimit(),nIndex2,MSGPACKAGE_TYPE_STORAGE,pItem1->GetKidnap());
	pUser->SendMsg(&msg);

	msg.Create(MSGPACKAGE_CHECKIN,pItem2->GetID(),pItem2->GetInt(ITEMDATA_TYPE),pItem2->GetItemAmount(),pItem2->GetItemAmountLimit(),nIndex1,MSGPACKAGE_TYPE_STORAGE,pItem2->GetKidnap());
	pUser->SendMsg(&msg);

	pItem1->SaveInfo();
	pItem2->SaveInfo();
	return true;
}
//////////////////////////////////////////////////////////////////////
//�ֿ�����Ʒ�ϲ�
bool CPackage::CombineItem(CUser* pUser, CItem* pItem1, CItem* pItem2)
{
	CHECKF(pUser);
	if (pItem1->GetKidnap()!=pItem2->GetKidnap())
		return false;
	if (!CItem::IsCountable(pItem1->GetTypeID()))//���ɵ���ʱ����λ��
	{
		ExChangeIndex(pUser, pItem1, pItem2);
		return true;
	}
	int nIndex1 = pItem1->GetPackageIndex();//Ŀ��λ��
	int nIndex2 = pItem2->GetPackageIndex();
	

	int nMaxNum = pItem1->GetInt(ITEMDATA_AMOUNT_LIMIT_INIT);
	if (pItem1->GetItemAmount()==nMaxNum)
	{
		return true;
	}
	if (pItem1->GetItemAmount() + pItem2->GetItemAmount() <= nMaxNum)
	{
		pItem1->SetInt(ITEMDATA_AMOUNT, pItem1->GetItemAmount() + pItem2->GetItemAmount());
		if(pItem1->GetItemAmountLimit() != nMaxNum)
			pItem1->SetInt(ITEMDATA_AMOUNTLIMIT,nMaxNum);
		pItem1->SaveInfo();

		CMsgPackage msg;
		msg.Create(MSGPACKAGE_CHECKOUT,pItem1->GetID(),pItem1->GetInt(ITEMDATA_TYPE),pItem1->GetItemAmount(),pItem1->GetItemAmountLimit(),nIndex1,MSGPACKAGE_TYPE_STORAGE,pItem1->GetKidnap());
		pUser->SendMsg(&msg);

		msg.Create(MSGPACKAGE_CHECKOUT,pItem2->GetID(),pItem2->GetInt(ITEMDATA_TYPE),pItem2->GetItemAmount(),pItem2->GetItemAmountLimit(),nIndex2,MSGPACKAGE_TYPE_STORAGE,pItem2->GetKidnap());
		pUser->SendMsg(&msg);

		msg.Create(MSGPACKAGE_CHECKIN,pItem1->GetID(),pItem1->GetInt(ITEMDATA_TYPE),pItem1->GetItemAmount(),pItem1->GetItemAmountLimit(),nIndex1,MSGPACKAGE_TYPE_STORAGE,pItem1->GetKidnap());
		pUser->SendMsg(&msg);
		
		m_setItem->PopObj(pItem2->GetID());
		pItem2->DeleteRecord(::MapGroup(pUser->GetProcessID())->GetDatabase(), SYNCHRO_TRUE);
		pItem2->ReleaseByOwner();
	}
	else
	{
		int nMoveSize = nMaxNum - pItem1->GetItemAmount();
		pItem1->SetInt(ITEMDATA_AMOUNT, nMaxNum);
		if(pItem1->GetItemAmountLimit() != nMaxNum)
			pItem1->SetInt(ITEMDATA_AMOUNTLIMIT,nMaxNum);

		pItem2->SetInt(ITEMDATA_AMOUNT, pItem2->GetItemAmount() - nMoveSize);
		if(pItem2->GetItemAmountLimit() != nMaxNum)
			pItem2->SetInt(ITEMDATA_AMOUNTLIMIT,nMaxNum);

		pItem1->SaveInfo();
		pItem2->SaveInfo();
		CMsgPackage msg;
		msg.Create(MSGPACKAGE_CHECKOUT,pItem1->GetID(),pItem1->GetInt(ITEMDATA_TYPE),pItem1->GetItemAmount(),pItem1->GetItemAmountLimit(),nIndex1,MSGPACKAGE_TYPE_STORAGE,pItem1->GetKidnap());
		pUser->SendMsg(&msg);
		
		msg.Create(MSGPACKAGE_CHECKOUT,pItem2->GetID(),pItem2->GetInt(ITEMDATA_TYPE),pItem2->GetItemAmount(),pItem2->GetItemAmountLimit(),nIndex2,MSGPACKAGE_TYPE_STORAGE,pItem2->GetKidnap());
		pUser->SendMsg(&msg);
		
		msg.Create(MSGPACKAGE_CHECKIN,pItem1->GetID(),pItem1->GetInt(ITEMDATA_TYPE),pItem1->GetItemAmount(),pItem1->GetItemAmountLimit(),nIndex1,MSGPACKAGE_TYPE_STORAGE,pItem1->GetKidnap());
		pUser->SendMsg(&msg);

		msg.Create(MSGPACKAGE_CHECKIN,pItem2->GetID(),pItem2->GetInt(ITEMDATA_TYPE),pItem2->GetItemAmount(),pItem2->GetItemAmountLimit(),nIndex2,MSGPACKAGE_TYPE_STORAGE,pItem2->GetKidnap());
		pUser->SendMsg(&msg);
	}

	return true;
}
//////////////////////////////////////////////////////////////////////
//�ֿ��е���Ʒ�ڲֿ����ƶ�λ��
bool CPackage::MoveStorageItem(CUser* pUser, CItem* pItem, int nIndex)
{
	CHECKF(pUser);

	if (!IsEmpty(nIndex))
		return false;

	CMsgPackage msg;
	msg.Create(MSGPACKAGE_CHECKOUT,pItem->GetID(),pItem->GetInt(ITEMDATA_TYPE),pItem->GetItemAmount(),pItem->GetItemAmountLimit(),pItem->GetPackageIndex(),MSGPACKAGE_TYPE_STORAGE,pItem->GetKidnap());
		pUser->SendMsg(&msg);

	pItem->SetPackageIndex(nIndex);
	pItem->SaveInfo();
	msg.Create(MSGPACKAGE_CHECKIN,pItem->GetID(),pItem->GetInt(ITEMDATA_TYPE),pItem->GetItemAmount(),pItem->GetItemAmountLimit(),pItem->GetPackageIndex(),MSGPACKAGE_TYPE_STORAGE,pItem->GetKidnap());
		pUser->SendMsg(&msg);

	return true;
}
//////////////////////////////////////////////////////////////////////
bool CPackage::SendInfo(CUser* pUser, int nIndex)//goto ���Ӳֿ�ҳ�������ֻ��ָ��һҳ����Ʒ
{
	CHECKF(m_nType != _STORE_NONE);
	CHECKF(pUser);
	
// 	int nAction = MSGPACKAGE_QUERYLIST;
// 	MsgPackageItemInfo	buf[PACKAGE_LIMIT];
// 	int		nCount = 0;
	
	for(int i = 0; i < m_setItem->GetAmount(); i++)
	{
		//[2010-06-12 goto]ͬ����Ʒʱ��������Ʒ��ϸ��Ϣ
		CItemPtr pItem = m_setItem->GetObjByIndex(i);
		if(!pItem)
			continue;
		int nMaxIndex = nIndex*PACKAGE_LIMIT;//[2010-06-16 goto]����ҳ�����ֿ���һҳ���ӱ������
		int nMinIndex = nMaxIndex+1-PACKAGE_LIMIT;//[2010-06-16 goto]����ҳ�����ֿ���һҳ���ӱ������

		if (nMinIndex<1 || nMaxIndex>3*PACKAGE_LIMIT)
			continue;

		if (pItem->GetPackageIndex()<nMinIndex || pItem->GetPackageIndex()>nMaxIndex)
			continue;

		CMsgPackage msg;
		msg.Create(MSGPACKAGE_QUERYLIST,
				   pItem->GetID(),
				   pItem->GetInt(ITEMDATA_TYPE),
				   pItem->GetInt(ITEMDATA_AMOUNT),
				   pItem->GetInt(ITEMDATA_AMOUNTLIMIT),
				   pItem->GetPackageIndex(),
				   MSGPACKAGE_TYPE_STORAGE,
				   pItem->GetKidnap());//isbind
		pUser->SendMsg(&msg);
////////////////////////////////////////////////////////////////
// 		CItemPtr pItem = m_setItem->GetObjByIndex(i);
// 		if(pItem && nCount < PACKAGE_LIMIT)
// 		{
// 			buf[nCount].id			= pItem->GetID();
// 			buf[nCount].dwType		= pItem->GetInt(ITEMDATA_TYPE);
// 			buf[nCount].usAmount	= pItem->GetInt(ITEMDATA_AMOUNT);
// 			buf[nCount].usAmountLimit	= pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
//			buf[nCount].ucIdent		= pItem->GetInt(ITEMDATA_IDENT);
// 			buf[nCount].GemTypeID[0]		= pItem->GetInt(ITEMDATA_GEM1);
// 			buf[nCount].GemTypeID[1]		= pItem->GetInt(ITEMDATA_GEM2);
// 			buf[nCount].GemTypeID[2]		= pItem->GetInt(ITEMDATA_GEM3);
// 			buf[ nCount ].BasicAdditionID1 = pItem->GetInt( ITEMDATA_BASIC_ADDITION1 );
// 			buf[ nCount ].BasicAdditionID2 = pItem->GetInt( ITEMDATA_BASIC_ADDITION2 );
// 			buf[ nCount ].BasicAdditionID3 = pItem->GetInt( ITEMDATA_BASIC_ADDITION3 );//[��;���� 2008/11/10]
		
// 			buf[nCount].nData		= pItem->GetInt(ITEMDATA_DATA);
// 			buf[nCount].ucSpecialEffect=  pItem->GetInt(ITEMDATA_SPECIAL_EFFECT);
// 			buf[ nCount ].usLevel		= pItem->GetInt( ITEMDATA_LEVEL );
// 			buf[ nCount ].dwExp			= pItem->GetInt( ITEMDATA_EXP );
// 			buf[ nCount ].ucAttrib		= pItem->GetInt( ITEMDATA_ATTRIBUTE );
// 			
// 			buf[ nCount ].usMinPhysicalAttack	= pItem->GetInt( ITEMDATA_MIN_ATTACK );
// 			buf[ nCount ].usMaxPhysicalAttack	= pItem->GetInt( ITEMDATA_MAX_ATTACK );
// 			buf[ nCount ].usMinMagicAttack	= pItem->GetInt( ITEMDATA_MIN_MAGICATTACK );
// 			buf[ nCount ].usMaxMagicAttack	= pItem->GetInt( ITEMDATA_MAX_MAGICATTACK );
// 			buf[ nCount ].usStrengthanCount	= pItem->GetInt( ITEMDATA_STRENGTHAN_COUNT );	//ǿ������ +��
// 			buf[ nCount ].usPhysicalDefence	= pItem->GetInt( ITEMDATA_NEW_PHYSICALDEF );	//�������
// 			buf[ nCount ].usMagicDefence		= pItem->GetInt( ITEMDATA_NEW_MAGICDEF );			//ħ������
// 			buf[ nCount ].ucEmbedCountLimit	= pItem->GetInt( ITEMDATA_EMBEDCOUNT_LIMIT );		//����Ƕ�������ƣ��������6
// 			buf[ nCount ].usCapability		= pItem->GetInt( ITEMDATA_CAPABILITY );			//���������
// 			buf[ nCount ].usEmbedPos		= pItem->GetInt( ITEMDATA_EMBED_POS );				//��ʯ����Ƕλ��
// 			SafeCopy( buf[ nCount ].szName,pItem->GetStr( ITEMDATA_FORGENAME ),16 );	
			
			//  [2007-10-9] ��� �Ƿ񼤻�
// 			if(strlen(pItem->GetLastCheckTime()) == 0 || stricmp(pItem->GetLastCheckTime(), "NULL") == 0)
// 				buf[ nCount ].bIsActivated = FALSE;
// 			else
// 				buf[ nCount ].bIsActivated = TRUE;
			
			//  [2007-10-9] ��� ��������
// 			buf[ nCount ].uLifeTime = pItem->GetLifeTime();

			//  [3/4/2008 ���]
// 			buf[nCount].WPSL_State = pItem->GetStateWPSL();
// 			buf[nCount].WPSL_ID1 = pItem->GetIdWPSLByIndex(0);
// 			buf[nCount].WPSL_ID2 = pItem->GetIdWPSLByIndex(1);
// 			buf[nCount].WPSL_ID3 = pItem->GetIdWPSLByIndex(2);
// 			buf[nCount].WPSL_ID4 = pItem->GetIdWPSLByIndex(3);
// 			
// 			++nCount;
			// Ϊ�˱���һ�����ݹ��࣬�ֶ�η���
// 			const int	ITEMS_PER_PAGE	= 6;	//  [2007-10-10] ���	����޸�MsgPackageItemInfo����Ӧ�����ֵ���ⳬ����Ϣ����
// 			if (nCount >= ITEMS_PER_PAGE)
// 			{
// 				CMsgPackage msg;
// 				CHECKF(msg.Create(nAction, m_idOwner, buf, nCount, m_ucType));
// 				pUser->SendMsg(&msg);
// 				nCount = 0;
// 				nAction = MSGPACKAGE_QUERYLIST2;
// 			}
// 		}
// 		else
// 			break;
	}
	
	// modified by zlong 2003-11-24 ---- ָ��ucType���
	// synchro
// 	if (nCount > 0)
// 	{
// 		CMsgPackage msg;
// 		CHECKF(msg.Create(nAction, m_idOwner, buf, nCount, m_ucType));
// 		pUser->SendMsg(&msg);
// 	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////
int CPackage::GetItemType(int index)
{
	CHECKF(m_nType != _STORE_NONE);

	if(index >= 0 && index < m_setItem->GetAmount())
	{
		CItemPtr pItem = m_setItem->GetObjByIndex(index);
		if(pItem)
			return  pItem->GetInt(ITEMDATA_TYPE);
	}

	return ID_NONE;
}


//////////////////////////////////////////////////////////////////////
OBJID CPackage::GetItemTypeByID(OBJID idItem)
{
	CHECKF(m_nType != _STORE_NONE);

	CItemPtr pItem = m_setItem->GetObj(idItem);
	if(!pItem)
		return ID_NONE;

	return pItem->GetInt(ITEMDATA_TYPE);
}

DWORD CPackage::GetItemAmountLimitByID(OBJID idItem)
{
	CHECKF(m_nType != _STORE_NONE);

	CItemPtr pItem = m_setItem->GetObj(idItem);
	if(!pItem)
		return 0;

	return pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
}

//	20070712	���	������ID������Ʒ
CItemPtr CPackage::GetItemByTypeID(OBJID idType)
{
	CHECKF(m_nType != _STORE_NONE);

	for(int i = 0; i < m_setItem->GetAmount(); i++)
	{
		CItemPtr pItem = m_setItem->GetObjByIndex(i);
		if(!pItem)
			continue;

		if(pItem->GetTypeID() == idType)
			return pItem;
	}

	return NULL;
}

void CPackage::CheckItemLifeTime(CUserPtr pOwner, BOOL bUpdateToClient, BOOL bSaveToDatabase)
{
	CHECK(pOwner);

	int nCount = m_setItem->GetAmount();
	//for(int i = 0; i < m_setItem->GetAmount(); i++)
	for(int i = nCount - 1; i >= 0; i--)	//  [���10/22/2007] BUG0001946 �����������CheckItemLifeTime���ܻ�ɾ����Ʒ
	{
		CItemPtr pItem = m_setItem->GetObjByIndex(i);
		if(!pItem)
			continue;

		pOwner->CheckItemLifeTime(pItem, bUpdateToClient, bSaveToDatabase);
	}
}

//////////////////////////////////////////////////////////////////////
void CPackage::SendAllObjInfo(CUserPtr pOwner,PROCESS_ID idProcess)
{
	CHECK(pOwner);
	
	int nCount = m_setItem->GetAmount();
	for(int i = nCount - 1; i >= 0; i--)	//  [���10/22/2007] BUG0001946 �����������CheckItemLifeTime���ܻ�ɾ����Ʒ
	{
		CItemPtr pItem = m_setItem->GetObjByIndex(i);
		if(pItem)
		{
			ItemInfoStructSend info;
			pItem->GetInfo(&info);
			pOwner->SendObjInfo(idProcess, pOwner->GetID(), INFO_ITEM, &info, sizeof(info));
		}
	}
}
