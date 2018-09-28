// UserPackage.cpp: implementation of the CUserPackage class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "UserPackage.h"
#include "User.h"
#include "MapGroup.h"
#include "Network/allmsg.h"
#include "Network/netmsg.h"


MYHEAP_IMPLEMENTATION(CUserPackage, s_heap)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserPackage::CUserPackage()
{
	m_pOwner	= NULL;
	for (int i=0; i<_MAX_PACK_TYPE; i++)
	{
		m_setItem[i] = CItemSet::CreateNew();
		if (!m_setItem[i])
		{
			::LogSave("error create UserPackage, CItemSet::CreateNew failed.");
		}
			//06.9.21�������
		memset( &m_bIndexHasItem[ i ],0,sizeof( int ) * _MAX_USERITEMSIZE );
	}
	for (i=0; i<4; i++)
		m_nIndexOrdered[i] = 0;
}

CUserPackage::~CUserPackage()
{
	for (int i=0; i<_MAX_PACK_TYPE; i++)
	{
		if (m_setItem[i])
			SAFE_RELEASE (m_setItem[i]);
	}
}

//////////////////////////////////////////////////////////////////////
bool CUserPackage::Create(CUser* pUser, IDatabase* pDb,bool bLogin)
{
	CHECKF(pUser);
	CHECKF(pDb);

	ClearAll();

	int nPack = pUser->GetPacklev();
	_MAX_PACK_SIZES[0] = 24 + nPack % 100;
	_MAX_PACK_SIZES[1] = 24 + (nPack / 100) % 100;
	_MAX_PACK_SIZES[2] = 24 + (nPack / 10000) % 100;
	_MAX_PACK_SIZES[3] = 10;

	m_pOwner = pUser;
	if(!bLogin)
		return true;

	bool bAddSuc = false;

	SQLBUF		szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE owner_id=%u && position>=%u && position<%u", _TBL_ITEM, m_pOwner->GetID(), ITEMPOSITION_PACK_BEGIN, ITEMPOSITION_PACK_END);
	IRecordset*	pRes = pDb->CreateNewRecordset(szSQL);
	if(pRes)
	{
		for(int i = 0; i < pRes->RecordCount(); i++)
		{
			CItemPtr	pItem = CItem::CreateNew();
			if(pItem)
			{
				bAddSuc = false;
				if(pItem->Create(pRes, pDb))
				{
					//���������ı�̬����
// 					DEBUG_TRY
// 					if(pItem->GetTypeID() == 144000)
// 					{
// 						SQLBUF		szSQLDel;
// 						sprintf(szSQLDel, "DELETE FROM %s WHERE id=%u", _TBL_ITEM, pItem->GetID());
// 						pRes->MoveNext();
// 						pDb->ExecuteSQL(szSQLDel);
// 						pItem->ReleaseByOwner();
// 						continue;
// 					}

					//m_pOwner->ReEquipAttribute(pItem);

					if(pItem->GetPosition() < ITEMPOSITION_PACK_BEGIN || pItem->GetPosition() >= ITEMPOSITION_PACK_END)
					{
						LOGWARNING("���棬������Ʒ�����ܵ��»��� itemid = %u userid = %u",pItem->GetID(),m_pOwner->GetID());
						pRes->MoveNext();
						continue;
					}
					bAddSuc = AddItem(pItem);
					if(bAddSuc)
					{
						if(pItem->GetPosition() < ITEMPOSITION_PACK_BEGIN || pItem->GetPosition() >= ITEMPOSITION_PACK_END)
						{
							LOGWARNING("���棬���뱳������Ʒ�����⣬���ܵ��»��� itemid = %u userid = %u",pItem->GetID(),m_pOwner->GetID());
							pRes->MoveNext();
							continue;
						}
					}
//					DEBUG_CATCH("ɾ����̬����ERROR!!!")
				}

				if(!bAddSuc)
				{
					LOGERROR("���[%s]�޷�������Ʒ[%u]", m_pOwner->GetName(), pItem->GetID());
					pItem->ReleaseByOwner();
				}
			}

			pRes->MoveNext();
		}
		pRes->Release();
	}

// 	sprintf(szSQL, "SELECT * FROM %s WHERE owner_id=%u && position=%u", _TBL_ITEM, m_pOwner->GetID(), ITEMPOSITION_PETPACK);
// 	pRes = pDb->CreateNewRecordset(szSQL);
// 	if(pRes)
// 	{
// 		for(int i = 0; i < pRes->RecordCount(); i++)
// 		{
// 			CItemPtr	pItem = CItem::CreateNew();
// 			if(pItem)
// 			{
// 				if(pItem->Create(pRes, pDb))
// 				{
// 					m_pOwner->ReEquipAttribute(pItem);
// 					AddItem(pItem);
// 				}
// 				else
// 				{
// 					LOGERROR("���[%s]�޷�������Ʒ[%u]", m_pOwner->GetName(), pItem->GetID());
// 					pItem->ReleaseByOwner();
// 				}
// 			}
// 			
// 			pRes->MoveNext();
// 		}
// 		pRes->Release();
// 	}
	return true;
}

bool CUserPackage::AddItemByChangeMap(CItem* pItem)
{
	int nPosition = pItem->GetInt( ITEMDATA_POSITION );
	int nPackType = GetPackType(nPosition);
	SetIndexState( nPosition,pItem->GetInt( ITEMDATA_PACKAGE_INDEX ),1 );
	m_setItem[nPackType]->AddObj(pItem);
	return true;
}
//////////////////////////////////////////////////////////////////////
bool CUserPackage::AddItem(CItem* pItem, bool bUpdate /* = false */)
{
	int nStep = 1;
	DEBUG_TRY
	CHECKF(pItem);
	CHECKF(m_pOwner);
//06.2.16�����޸�
 	int nPosition = pItem->GetInt( ITEMDATA_POSITION );
// 	if( nPosition < ITEMPOSITION_PACK_BEGIN || nPosition > ITEMPOSITION_PACK_END )
// 	{
	nStep = 2;

	int nCheckPosition = this->GetAvailablePosition(pItem->GetTypeID());
	if(nCheckPosition == -1)
	{
		//LOGERROR("���������������Ʒ�޷����أ���ҿ������ˣ����磬userid = %u itemid = %u position = %u",m_pOwner->GetID(),pItem->GetID(),nPosition);
		m_pOwner->SendSysMsg("��������");
		return false;
	}

	nStep = 3;

	if(nPosition != nCheckPosition)
	{
		//LOGERROR("��ʼλ�ò��ԣ��ѵ�������?userid = %u itemid = %u",m_pOwner->GetID(),pItem->GetID());
		nPosition = nCheckPosition;
		pItem->SetPosition( nPosition);
	}

	nStep = 4;
//	}
	///�жϻ�ʯ
//	if(pItem->IsSpiritStone())
//		pItem->SetPosition(ITEMPOSITION_MAGICSTONEBOX,false);
/*	if(!pItem->IsSpiritStone() && nPosition==ITEMPOSITION_MAGICSTONEBOX)
	{
		m_pOwner->SendSysMsg(g_objStrRes.GetStr(15069));// �½��ɵ��� [6/29/2007]"�������ʧ��,ֻ��ħ�����ܷ���ħ����!"
		return false;
	}*/

	if ( IsPackFull( 1,0,nPosition ) ) //�������..
	{
		//�ҵ�һ�����õ�POS
// 		nPosition = GetAvailablePosition(pItem->GetTypeID());
// 		pItem->SetPackageIndex(0,false);
// 		if( -1 == nPosition )
// 		{
			//LOGERROR("���������������Ʒ�޷����أ���ҿ������ˣ����磬userid = %u itemid = %u",m_pOwner->GetID(),pItem->GetID());
			m_pOwner->SendSysMsg("��������");
			return false;
//		}
	}
	nStep = 5;

	//06.9.21�������,����package_index--------------------begin-----------------------------------------
	int  nPackageIndex = pItem->GetPackageIndex();
	int nPackType = GetPackType(nPosition);
// 	if(nPosition == ITEMPOSITION_PETPACK)
// 		nPackType = 3;

	nStep = 6;

	if (nPackType < 0 || nPackType >= 3)
		return false;

	nStep = 7;

	if( ( 0 >= nPackageIndex ) || nPackageIndex > _MAX_PACK_SIZES[nPackType] ||( GetIndexState( nPosition,nPackageIndex ) > 0 ))//Ϊ�������һ��λ���Ѿ�����Ʒ
	{
		nPackageIndex = GetFreeIndex( nPosition );
		if( nPackageIndex <= 0 )
		{
			//LOGERROR("�����������Ҳ�������Ʒ��λ�ã������Ʒ�޷����أ���ҿ������ˣ����磬userid = %u itemid = %u",m_pOwner->GetID(),pItem->GetID());
			m_pOwner->SendSysMsg("��������");
			return false;
		}
	}
	//-------------------------------------------------------endl------------------------------------------

	nStep = 8;
	pItem->SetPackageIndex( nPackageIndex );
	//�����Ǹ�λ������Ʒ
/*	if(nPosition!=ITEMPOSITION_MAGICSTONEBOX)*/
	if(!SetIndexState( nPosition,nPackageIndex,1))
		return false;

	nStep = 9;
	if (m_setItem[nPackType]->AddObj(pItem))
	{
		if(bUpdate)
		{
			nStep = 10;
			pItem->SaveInfo();

			DEBUG_TRY
			m_pOwner->UpdateMissionMsgByItemType(pItem->GetTypeID());
			DEBUG_CATCH("m_pOwner->UpdateMissionMsgByItemType1")
		}
		return true;
	}
	return false;

	DEBUG_CATCH3("CUserPackage::AddItem(CItem* pItem) userid = %u , nStep = %u",m_pOwner->GetID(),nStep)
	return false;
}

//////////////////////////////////////////////////////////////////////
//06.3.13����
//�����Ʒ����ͨ��Ʒ����(4 ���е�1�����ĸ��пռ����ӵ��ĸ�����û�пռ䷵��false)
//��ͨ��Ʒ��POSΪ:47,48,49,50
bool CUserPackage::AddItemToNormalBag( CItem* pItem,bool bUpdate /*= false*/)
{
	return false;
	CHECKF(pItem);
	CHECKF(m_pOwner);

	int npos = 0;
	if( !IsPackFull( 0,0,ITEMPOSITION_BACKPACK1 ) )
		npos = 47;
	else if( !IsPackFull(0,0, ITEMPOSITION_BACKPACK2 ) )
		npos = 48;
	else if( !IsPackFull(0,0, ITEMPOSITION_BACKPACK3 ) )
		npos = 49;
	else
		return false;
	int nPackType = GetPackType( npos );

	if ( pItem->GetPosition() != npos )
	{
		pItem->SetPosition( npos);
	}

	if ( m_setItem[ nPackType ]->AddObj( pItem ) )
	{
		if( bUpdate )
			pItem->SaveInfo();
		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////
//[2010-06-17 goto]���ݱ���λ�û�ȡ��Ʒ
CItem* CUserPackage::GetItemByPackageIndex(int nPosition, int nIndex)
{
	int nPackCount=/*m_pOwner->GetPackSize()*/3;
	for (int i=0; i<nPackCount; i++)
	{
		int nSize = m_setItem[i]->GetAmount();
		for (int j=nSize-1; j>=0; j--)
		{
			CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
			if (pItem && pItem->GetPosition()==nPosition && pItem->GetPackageIndex()==nIndex)
			{
				if(!GetIndexState(nPosition,nIndex))
					LOGWARNING("ȡ��һ����Ʒ��ȴ��Ӧλ��Ϊ��");
				return m_setItem[i]->GetObjByIndex(j);
			}
		}
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////
//[2010-06-17 goto]�����Ʒ��ָ���ı���λ��
bool CUserPackage::AddItemToPosition(CUser* pUser, CItem* pItem, int nPosition, int nIndex, bool bUpdate/* = false*/)
{
	return false;
	CHECKF(pItem);
	CHECKF(m_pOwner);

	if ( nPosition < ITEMPOSITION_PACK_BEGIN || nPosition > ITEMPOSITION_PACK_END )
		return false;

	int nMaxItemSize;
	if (nPosition == 47)
	{
		nMaxItemSize = _MAX_PACK_SIZES[0];
	}
	if (nPosition == 48)
	{
		nMaxItemSize = _MAX_PACK_SIZES[1];
	}
	else if (nPosition == 49)
	{
		nMaxItemSize = _MAX_PACK_SIZES[2];
	}
	else if(nPosition == 99)
	{
		nMaxItemSize = _MAX_PACK_SIZES[3];
	}

	if (nIndex < 1 || nIndex>nMaxItemSize)
		return false;

	int nPackType = GetPackType(nPosition);
	if (nPackType < 0)
		return false;

	pItem->SetPosition( nPosition);
	pItem->SetPackageIndex( nIndex );
	//�����Ǹ�λ������Ʒ
	if(!SetIndexState( nPosition,nIndex,1))
		return false;
	if (m_setItem[nPackType]->AddObj(pItem))
	{
		if(bUpdate)
			pItem->SaveInfo();
		
		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////
int CUserPackage::IsEmpty(int nPosition, int nIndex)
{
	return GetIndexState(nPosition,nIndex);
}
//goto Ԥ���������ӣ�����Ԫ������������Ʒʱʹ��
//���û���Ԫ��ָ������Ʒʱ���ͻ���ѭ�����͹�����Ʒ����Ϣ��������
//����ʱ��������Ҫ����Ϣһ��һ�����͵��˺ŷ�����
//���˺ŷ��������سɹ�֮��Ÿ����������Ʒ�����ø���״̬
//��ô���п������˺ŷ�����������Ϣǰ�������յ��˵ڶ���������Ʒ����Ϣ
//��ʱ�Ա������������жϾͻ�������Ϊδ����֮ǰ�����Ʒռ�õĸ���
void CUserPackage::SetIndexOrderAmount(int nPosition, int nAmount)
{
	int nIndex = 0;
	if (nPosition==ITEMPOSITION_PETPACK)
		nIndex = 3;
	else
		nIndex=nPosition-ITEMPOSITION_PACK_BEGIN;
	if (nIndex>=0 && nIndex<4)
		m_nIndexOrdered[nIndex]+=nAmount;
	else
		::LogSave("CUserPackage::SetIndexOrderAmount() Invalid Param!nPosition(%d),nIndex(%d)", nPosition, nIndex);
}
int CUserPackage::GetIndexOrderAmount(int nPosition)
{
	int nIndex = 0;
	if (nPosition==ITEMPOSITION_PETPACK)
		nIndex = 3;
	else
		nIndex=nPosition-ITEMPOSITION_PACK_BEGIN;
	if (nIndex>=0 && nIndex<4)
		return m_nIndexOrdered[nIndex];
	else
		::LogSave("CUserPackage::GetIndexOrderAmount() Invalid Param!nPosition(%d),nIndex(%d)", nPosition, nIndex);
	return 0;
}
//////////////////////////////////////////////////////////////////////
int	CUserPackage::GetFreeSpaceCount( int nPosition, bool bIncludeOrder )
{
	ReSetPackSize();
	int nCount = 0;
	int nIndex = 0;
// 	if (nPosition==ITEMPOSITION_PETPACK)//������
// 		nIndex=3;
// 	else
	nIndex=nPosition-ITEMPOSITION_PACK_BEGIN;
	CHECKF(nIndex >= 0 && nIndex < 3);

	for (int i=1; i<=_MAX_PACK_SIZES[nIndex]; i++)
	{
		if (!IsEmpty(nPosition, i))
			nCount++;
	}
	if (bIncludeOrder)
		nCount -= m_nIndexOrdered[nIndex];
	return nCount>0 ? nCount : 0;
}
//////////////////////////////////////////////////////////////////////
bool CUserPackage::DelItem(OBJID idItem)
{
//---------06.9.21�������-------begin----------------
	CItem* pItem = GetItem( idItem );
	if( pItem )
	{
		SetIndexState( pItem->GetPosition(),pItem->GetPackageIndex(),0 );//������һ������Ϊfalse
	}
//----------------------------------end-----------------
	int nPackCount=_MAX_PACK_TYPE;///m_pOwner->GetPackSize();///����Ӳ����Ϊ��󱳰�������ȷ������ɾ��ħ�����е�item,softworms 2007-05-09
	for (int i=0; i<nPackCount; i++)
	{
		if (m_setItem[i]->DelObj(idItem))
			return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////////////
//06.10.28�������
//��ָ������������ȡ��һ��ITEM,����Ч��
//����˵��:idItem,Ҫ�ҵ���Ʒid,int nPackArrayIndex,������������,>=0 && <=_MAX_PACK_TYPE
CItem* CUserPackage::GetItemFromSpecificPack( OBJID idItem,int nPackArrayIndex )
{
	if( idItem == ID_NONE )
	{
		LogSave( "CUserPackage::GetItemFromSpecificPack() Error,idItem = 0" );
		return NULL;
	}
	else if( nPackArrayIndex < 0 || nPackArrayIndex >= _MAX_PACK_TYPE )
	{
		LogSave( "CUserPackage::GetItemFromSpecificPack() Error,nPackArrayIndex=%d",nPackArrayIndex );
		return NULL;
	}
	CItemPtr pItem = m_setItem[ nPackArrayIndex ]->GetObj( idItem );
	if ( pItem )
		return pItem;
	else
		return NULL;

}
//////////////////////////////////////////////////////////////////////
CItem* CUserPackage::GetItem(OBJID idItem)
{
	CHECKF(m_pOwner);
	CHECKF(idItem);
	DEBUG_TRY
	int nPackCount=/*m_pOwner->GetPackSize()*/3;
	for (int i=0; i<nPackCount; i++)
	{
		CItemPtr pItem = m_setItem[i]->GetObj(idItem);
		if (pItem)
			return pItem;
	}
	DEBUG_CATCH3("CUserPackage::GetItem(%u),userid = %u",idItem,m_pOwner->GetID())
	return NULL;
}

CItem* CUserPackage::GetPetTrainItem()
{
	int nPackCount=3;
	CItemPtr pItemRet =NULL;
	int nHappy = 0;
	for (int i=0; i<nPackCount; i++)
	{
		int nSize = m_setItem[i]->GetAmount();
		for (int j=nSize-1; j>=0; j--)
		{
			CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
			if (pItem && !pItem->IsLocked() && (pItem->GetTypeID() / 10000 == 33))
			{
				if(nHappy == 0 || ((pItem->GetInt(ITEMDATA_MANA) > 0) && (nHappy > pItem->GetInt(ITEMDATA_MANA)) ) )
				{
					pItemRet = pItem;
					nHappy = pItem->GetInt(ITEMDATA_MANA);
				}
			}
		}
	}
	return pItemRet;
}


CItem* CUserPackage::GetPetLifeSpanItem()
{
	int nPackCount=3;
	CItemPtr pItemRet =NULL;
	int nLife = 0;
	for (int i=0; i<nPackCount; i++)
	{
		int nSize = m_setItem[i]->GetAmount();
		for (int j=nSize-1; j>=0; j--)
		{
			CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
			if (pItem && !pItem->IsLocked() && (pItem->GetTypeID() / 10000 == 34))
			{
				if(nLife == 0 || ((pItem->GetLife() > 0) && (nLife > pItem->GetLife()) ) )
				{
					pItemRet = pItem;
					nLife = pItem->GetLife();
				}
			}
		}
	}
	return pItemRet;
}

CItem* CUserPackage::GetPetEatItem()
{
	int nPackCount=3;
	CItemPtr pItemRet =NULL;
	int nLife = 0;
	for (int i=0; i<nPackCount; i++)
	{
		int nSize = m_setItem[i]->GetAmount();
		for (int j=nSize-1; j>=0; j--)
		{
			CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
			if (pItem && !pItem->IsLocked() && (pItem->GetTypeID() / 1000 == 320))
			{
				if(nLife == 0 || ((pItem->GetLife() > 0) && (nLife > pItem->GetLife()) ) )
				{
					pItemRet = pItem;
					nLife = pItem->GetLife();
				}
			}
		}
	}
	return pItemRet;
}

//////////////////////////////////////////////////////////////////////
CItem* CUserPackage::GetItemByType(OBJID idType)
{
	int nPackIndex= GetPositionByItemType(idType) - ITEMPOSITION_BACKPACK1;
	CBooth *pBooth = m_pOwner->QueryBooth();

	int nFirstIndex = -1;
	int nSize = m_setItem[nPackIndex]->GetAmount();
	for (int j=nSize-1; j>=0; j--)
	{
		CItemPtr pItem = m_setItem[nPackIndex]->GetObjByIndex(j);
		if (pItem && pItem->GetInt(ITEMDATA_TYPE) == idType)
		{
			if (pBooth && pBooth->IsHaveItem(pItem->GetID()))
			{
				continue;
			}

			if(pItem->GetKidnap() == 1)
			{
				return pItem;
			}
			else if(nFirstIndex == -1)
				nFirstIndex = j;
		}
	}
	if(nFirstIndex == -1)
		return NULL;
	else
		return m_setItem[nPackIndex]->GetObjByIndex(nFirstIndex);
}

int CUserPackage::GetItemNumByType(OBJID idType)
{
	int nResult = 0;

	int nPackIndex=GetPositionByItemType(idType) - ITEMPOSITION_BACKPACK1;

	int nSize = m_setItem[nPackIndex]->GetAmount();
	for (int j=nSize-1; j>=0; j--)
	{
		CItemPtr pItem = m_setItem[nPackIndex]->GetObjByIndex(j);
		if (pItem && pItem->GetInt(ITEMDATA_TYPE) == idType && pItem->IsLocked()==false)///softworms �¼����ж�,�����Ʒ���ڱ�����״̬����Ϊ��Ҳ�ӵ�д���Ʒ
		{
			if(CItem::IsCountable(idType))
				nResult += pItem->GetItemAmount();
			else
				nResult++;
		}
	}
	return nResult;
}
//20061010����.������ѯby TypeID.��ָ���������idItem[]
//�����ò�������ʵ�ʵ�size.�ɺ�������amount
//����-1ʱ��ʾidItem[]��ʵ��size����maxSize.��������������maxSize֮�ڵ�����
int	CUserPackage::GetItemByTypeMulti(OBJID idType,int &maxSize,OBJID *idItem, bool isKidnap)
{
	int amount=0;

	int nPackageIndex = this->GetPositionByItemType(idType);
	if(nPackageIndex < ITEMPOSITION_BACKPACK1 || nPackageIndex >= ITEMPOSITION_PACK_END)
		return -1;

	int nPack =  nPackageIndex - ITEMPOSITION_BACKPACK1;
	int nSize = m_setItem[nPack]->GetAmount();
	int nMaxIndex = 0;
	for (int i=nSize-1; i>=0; i--)
	{
		CItem *pItemTemp = m_setItem[nPack]->GetObjByIndex(i);
		if (!pItemTemp)
		{
			continue;
		}

		if(pItemTemp->GetInt(ITEMDATA_TYPE) != idType)
		{
			continue;
		}

		if(!isKidnap && pItemTemp->GetKidnap())
		{
			continue;
		}

		if(CItem::IsCountable(idType))//�ɵ���
		{
			amount+=pItemTemp->GetInt(ITEMDATA_AMOUNT);//���Ӹ���
		}
		else//���ɵ���
		{
			amount++;
		}

		DEBUG_TRY
		if(maxSize > 0)
		{
			if(nMaxIndex >= maxSize)//����Ԥ��size
			{
				LOGWARNING("GetItemByTypeMulti �������󣬷���-1 userid - %u",m_pOwner->GetID());
				maxSize=nMaxIndex;//����ʵ��size
				return -1;
			}
			else
			{
				idItem[nMaxIndex++]=pItemTemp->GetID();
			}
		}
		DEBUG_CATCH("GetItemByTypeMulti ���")
	}
	maxSize=nMaxIndex;//����ʵ��size
	return amount;
}
//���ñ��������������ŵ�������[08.9.27]�������
void CUserPackage::ReSetPackSize()
{
	CHECK(m_pOwner);
	int nPack = m_pOwner->GetPacklev();
	_MAX_PACK_SIZES[0] = 24 + (nPack / 10000) % 100;
	_MAX_PACK_SIZES[1] = 24 + (nPack / 100) % 100;
	_MAX_PACK_SIZES[2] = 24 + nPack % 100;
	_MAX_PACK_SIZES[3] = 3 + m_pOwner->GetPacklev(4);
}
//////////////////////////////////////////////////////////////////////
//�жϱ����Ƿ���������nPosiion ��ʾ������һ������
//����,return true
//��û,return false;
bool CUserPackage::IsPackFull(int nNeed,OBJID idItemType,int nPosition,bool bIncludeOrder)
{
	int nIndex = -1;
	int nDec = 0;

	ReSetPackSize();
		
	if(nNeed < 1)
		nNeed = 1;

	if(idItemType != ID_NONE)
		nPosition = this->GetPositionByItemType(idItemType);
	
	if(nPosition == -1)
		return true;

	if(!(nPosition >= ITEMPOSITION_PACK_BEGIN && nPosition <= ITEMPOSITION_PACK_END) )
	{
		::LogSave("nPosition >= ITEMPOSITION_PACK_BEGIN && nPosition <= ITEMPOSITION_PACK_END");
		::LogSave("CUserPackage::IsPackFull().nPosition =%d",nPosition);
		return true;
	}

	nIndex = nPosition - ITEMPOSITION_PACK_BEGIN;
	if (bIncludeOrder)
		nDec = m_nIndexOrdered[nIndex];
	//int nAmount = m_setItem[ nIndex ]->GetAmount();
	int nAmount = m_setItem[ nIndex ]->GetAmount();//goto Ԥ��λ�ã�����Ԫ������ʱ�����ڹ������ǰ������
	if ( m_setItem[ nIndex ]->GetAmount() + nNeed > (_MAX_PACK_SIZES[ nIndex ] - nDec))
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////
bool CUserPackage::IsPackSpare(int nNum, OBJID idItemType, DWORD dwData)
{
	if (idItemType == ID_NONE)	// �����ָ��itemtype����dwData��ʾҪ���ı���
		return IsPackSpare( nNum, dwData );
	else
	{
		bool bOK = !IsPackFull( nNum);
		if (!bOK && idItemType != 131010)
		{
			ItemInfoStruct info;
			memset(&info,0,sizeof(ItemInfoStruct));
			ASSERT(ItemType()->GetInfo(idItemType, &info));
			
			if(CItem::IsCountable(idItemType))
			{
				if(/*!CItem::IsNeedIdent(info.ucIdent) && */nNum < info.nAmountLimit)
				{
					const int _maxSize=120;
					int sourceItemAmount = 0;
					int sourceItemSize=_maxSize;
					OBJID sourceItemID[_maxSize];
					sourceItemAmount = this->GetItemByTypeMulti(idItemType, sourceItemSize, sourceItemID);
					for (int i = 0; i < sourceItemSize; ++i)
					{
						CItem* pItem = GetItem(sourceItemID[i]);
						if(pItem && info.nKidnap ==  pItem->GetKidnap())
						{
							if(pItem->GetInt(ITEMDATA_AMOUNT) + nNum > info.nAmountLimit)
							{
								continue;
							}
							bOK = true;
							break;
						}
					}
				}
			}
		}
		return bOK;
	}
}
//////////////////////////////////////////////////////////////////////
bool CUserPackage::IsPackSpare(int nNum, int nPosition)
{
	ReSetPackSize();
	//20061025����.ashulanDebug
	//CHECKF(nPosition >= ITEMPOSITION_PACK_BEGIN && nPosition <= ITEMPOSITION_PACK_END);
	if(!(nPosition >= ITEMPOSITION_PACK_BEGIN && nPosition <= ITEMPOSITION_PACK_END) )
	{
		::LogSave("nPosition >= ITEMPOSITION_PACK_BEGIN && nPosition <= ITEMPOSITION_PACK_END");
		::LogSave("CUserPackage::IsPackSpare().nPosition =%d",nPosition);
	}

	int nPackType = GetPackType(nPosition);
	if (nPackType == -1)
		return false;

	if (m_setItem[nPackType]->GetAmount() + nNum > _MAX_PACK_SIZES[nPackType])
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
void CUserPackage::ClearAll()
{
	for (int i=0; i<_MAX_PACK_TYPE; i++)
	{
		int nStep = 1;
		DEBUG_TRY
		int nSize = m_setItem[i]->GetAmount();
		for (int j=nSize-1; j>=0; j--)
		{
			nStep = 2;
			DEBUG_TRY
			CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
			if (pItem)
			{
				nStep = 3;
				m_setItem[i]->DelObj(pItem->GetID());
			}
			else
				::LogSave("CGameObjSet::GetObjByIndex return NULL in CUserPackage::ClearAll");
			DEBUG_CATCH2("catch error in CUserPackage::ClearAll in for step=%d", nStep)
		}
		nStep = 4;
		//06.9.21�������
		memset( &m_bIndexHasItem[ i ],0,sizeof( int ) * _MAX_USERITEMSIZE );
		DEBUG_CATCH2("catch error in CUserPackage::ClearAll step=%d", nStep)
	}

}

//////////////////////////////////////////////////////////////////////
void CUserPackage::SaveAllInfo()
{
	int nPackCount=/*m_pOwner->GetPackSize()*/4;
	for (int i=0; i<nPackCount; i++)
	{
		int nSize = m_setItem[i]->GetAmount();
		for (int j=nSize-1; j>=0; j--)
		{
			CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
			if (pItem)
				pItem->SaveInfo();
		}
	}
	///����ħ����
// 	int nSize = m_setItem[4]->GetAmount();
// 	for (int j=0; j<nSize; j++)
// 	{
// 		CItemPtr pItem = m_setItem[4]->GetObjByIndex(j);
// 		if (pItem)
// 		{
// 			///����ħ������
// 			pItem->SetPackageIndex(j,false);
// 			pItem->SaveInfo();
// 		}
// 	}
}

//////////////////////////////////////////////////////////////////////
int CUserPackage::SendAllItemInfo()
{
	if (!m_pOwner)
		return 0;

	CMsgItem msg;
	msg.Create(ITEMACT_ADD,2);
	int allAmount = 0;
	int nAmount = 0;	

	int nPackCount=/*m_pOwner->GetPackSize()*/4;
	for (int i=0; i<nPackCount; i++)
	{
		int nSize = m_setItem[i]->GetAmount();
		for (int j=0; j<nSize; j++)
		{
			CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
			if (pItem)
			{
				if(!msg.Add(pItem->GetID(),pItem->GetTypeID(),pItem->GetItemAmount(),pItem->GetInt(ITEMDATA_AMOUNTLIMIT), pItem->GetPosition(),pItem->GetKidnap(),pItem->GetPackageIndex(),pItem->GetInt(ITEMDATA_BUYPRICE),pItem->GetColour()))
				{
					m_pOwner->SendMsg(&msg);
					msg.Create(ITEMACT_ADD,2);
					msg.Add(pItem->GetID(),pItem->GetTypeID(),pItem->GetItemAmount(),pItem->GetInt(ITEMDATA_AMOUNTLIMIT), pItem->GetPosition(),pItem->GetKidnap(),pItem->GetPackageIndex(),pItem->GetInt(ITEMDATA_BUYPRICE),pItem->GetColour());
					allAmount += nAmount;
					nAmount = 0;
				}
				nAmount ++;
			}
		}
	}
	if(nAmount > 0)
	{
		allAmount += nAmount;
		m_pOwner->SendMsg(&msg);
	}
	return allAmount;
}


bool  CUser::IniUserKey(bool bSend ,bool bLogin)
{
	if(m_pUserKey.bInit)
		return true;

	IF_NOT(m_pUserKey.Create(Database(),this,bLogin,GameDataDefault()->GetKeyData()))
		return false;
	
	m_pUserKey.bInit = true;

	if(!bSend )
		return true;

	if(!bLogin)
		return true;

    if(!m_pUserKey.SendKey(this)) //�Ϳ�ݼ����ͻ���
		return false;
	
	return true;
}
//////////////////////////////////////////////////////////////////////
CItem* CUserPackage::GetTaskItem(LPCTSTR szItemName)
{
	int nPackCount=/*m_pOwner->GetPackSize()*/4;
	for (int i=0; i<nPackCount; i++)
	{
		int nSize = m_setItem[i]->GetAmount();
		for (int j=nSize-1; j>=0; j--)
		{
			CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
			if (pItem && strcmp(pItem->GetStr(ITEMDATA_NAME), szItemName) == 0)
				return pItem;
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
CItem* CUserPackage::GetTaskItemByType(OBJID idType, int nAmount)
{
	int nPackCount=/*m_pOwner->GetPackSize()*/4;
	for (int i=0; i<nPackCount; i++)
	{
		int nSize = m_setItem[i]->GetAmount();
		for (int j=nSize-1; j>=0; j--)
		{
			CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
			if (pItem && pItem->GetInt(ITEMDATA_TYPE) == idType && (!nAmount || pItem->GetInt(ITEMDATA_AMOUNT) >= nAmount))
				return pItem;
		}
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////
CItem* CUserPackage::PopItem(OBJID idItem)
{
	if (ID_NONE == idItem)
		return NULL;

	int nPackCount=/*m_pOwner->GetPackSize()*/3;
	for (int i=0; i<nPackCount; i++)
	{
		CItemPtr pItem = m_setItem[i]->PopObj(idItem);
		if (pItem)
		{
			SetIndexState( pItem->GetPosition(),pItem->GetPackageIndex(),0 );//06.9.21�������

			DEBUG_TRY
			m_pOwner->UpdateMissionMsgByItemType(pItem->GetTypeID());
			DEBUG_CATCH("m_pOwner->UpdateMissionMsgByItemType2")
			return pItem;
		}
	}
//	CItemPtr pItem = m_setItem[4]->PopObj(idItem);
//	if (pItem)
//	{
//		SetIndexState(pItem->GetPosition(),pItem->GetPackageIndex(),0 );//06.9.21�������
//		return pItem;
//	}
	
	return NULL;
}

bool CUserPackage::PureMagicStoneBox()//���ħ����
{
	return false;
	int nSize = m_setItem[4]->GetAmount();
	for (int j=nSize-1; j>=0; j--)
	{
		CItem *pItemTemp = m_setItem[4]->GetObjByIndex(j);
		if (pItemTemp) 
		{
			//////////////////////////////////////////////////////////////////////////
			//	20070710	���	��¼װ����Ϣ
			BOOL bIsCostlyItem = pItemTemp->IsCostlyItem();
			int nItemTypeID = 0, nItemID = 0, nItemAmount = 0;
			ItemInfoStruct info;
			NAMESTR szItemName;
			BOOL	bIsItemCountable = FALSE;
			if(bIsCostlyItem)
			{
				nItemTypeID = pItemTemp->GetTypeID();
				nItemID = pItemTemp->GetID();
				ItemType()->GetInfo(pItemTemp->GetTypeID(), &info);		
				strcpy(szItemName, info.szName);
				nItemAmount = pItemTemp->GetInt(ITEMDATA_AMOUNT);
				bIsItemCountable = CItem::IsCountable(pItemTemp->GetTypeID());
			}				
			//	���� 20070710 ���
			//////////////////////////////////////////////////////////////////////////
			BOOL bRet = m_pOwner->EraseItem(pItemTemp->GetID(),true);
			
			//////////////////////////////////////////////////////////////////////////
			//	20070709	���	����������Ʒ������־	����
			if(bRet && bIsCostlyItem)
			{
				MyLogSaveCostlyItem(NULL, m_pOwner->GetAccountID(), m_pOwner->GetID(),
					m_pOwner->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_DROP, 
					nItemTypeID, nItemID, szItemName,   
					bIsItemCountable ? nItemAmount : 1);  
			}
			//	���� 20070709	���	
			//////////////////////////////////////////////////////////////////////////
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////
//�õ�һ�����Է����Ӧtype��Ʒ���пո񱳰���POS������ֻ����������POS ��47 48 49
//����Ҳ�����ӦPOS����-1
//������ҵ�������Ӧ����
int CUserPackage::GetAvailablePosition(OBJID idType, bool bOrdered)
{
	int pos = -1;
	switch(idType / 100000)
	{
	case 1:
	case 2:
	case 3:
	case 5:
	case 8:
		if( false == this->IsPackFull(1,0, ITEMPOSITION_BACKPACK1,bOrdered ) ) 
			pos = ITEMPOSITION_BACKPACK1;
		break;
	case 4:
		if( false == this->IsPackFull( 1,0,ITEMPOSITION_BACKPACK2,bOrdered ) ) 
			pos = ITEMPOSITION_BACKPACK2;
		break;
	case 6:
		{
			if (idType/10000==60 || idType/10000==64 || idType/10000==65)
			{
				if( false == this->IsPackFull(1,0, ITEMPOSITION_BACKPACK1,bOrdered ) ) 
					pos = ITEMPOSITION_BACKPACK1;
			}
			else if (idType/10000==61 || idType/10000==63  || idType/10000==66 || idType/1000==626)
			{
				if( false == this->IsPackFull( 1,0,ITEMPOSITION_BACKPACK2,bOrdered ) ) 
					pos = ITEMPOSITION_BACKPACK2;
			}
			else if (idType/10000==62)
			{
				if( false == this->IsPackFull( 1,0,ITEMPOSITION_BACKPACK3,bOrdered ) ) 
					pos = ITEMPOSITION_BACKPACK3;
			}
			else
			{
				LogSave("GetAvailablePosition(OBJID idType) error typeid = %u",idType);
			}
		}
		break;
	case 9:
		if( false == this->IsPackFull( 1,0,ITEMPOSITION_BACKPACK3,bOrdered ) ) 
			pos = ITEMPOSITION_BACKPACK3;
		break;
	default:
		LogSave("GetAvailablePosition(OBJID idType) error typeid = %u",idType);
		break;
// 	case 7:
// 		if( false == this->IsPackFull( 0,0,ITEMPOSITION_PETPACK ) ) 
// 			pos = ITEMPOSITION_PETPACK;
// 		break;
	}

	
	return pos;
}

//////////////////////////////////////////////////////////////////////
//�õ�һ����Ӧtype��Ʒ��Ӧ�ı�����POS������ֻ����������POS ��47 48 49
//����Ҳ�����ӦPOS���ص�һ������
//������ҵ�������Ӧ����
int CUserPackage::GetPositionByItemType(OBJID idItemType)
{
	int pos = ITEMPOSITION_BACKPACK1;
	switch(idItemType / 100000)
	{
	case 1:
	case 2:
	case 3:
	case 5:
	case 8:		
		pos = ITEMPOSITION_BACKPACK1;
		break;
	case 4:
		pos = ITEMPOSITION_BACKPACK2;
		break;
	case 6:
		{
			if (idItemType/10000==60 || idItemType/10000==64 || idItemType/10000==65)
				pos = ITEMPOSITION_BACKPACK1;
			else if (idItemType/10000==61 || idItemType/10000==63  || idItemType/10000==66 || idItemType/1000==626)
				pos = ITEMPOSITION_BACKPACK2;
			else if (idItemType/10000==62)
				pos = ITEMPOSITION_BACKPACK3;
			else
				LogSave("GetAvailablePosition(OBJID idItemType) error typeid = %u",idItemType);
		}
		break;
	case 9:
		pos = ITEMPOSITION_BACKPACK3;
		break;
	default:
		//STACK_DUMP();
		LogSave("GetAvailablePosition(OBJID idItemType) error typeid = %u",idItemType);
		break;
		// 	case 7:
		// 		if( false == this->IsPackFull( 0,0,ITEMPOSITION_PETPACK ) ) 
		// 			pos = ITEMPOSITION_PETPACK;
		// 		break;
	}
	
	
	return pos;
}

//////////////////////////////////////////////////////////////////////
CItem* CUserPackage::FindCombineItem(OBJID idType)
{
	CItemTypeData* pTypeData = ItemType()->QueryItemType(idType);
	if(NULL == pTypeData)
	{
		return NULL;
	}

	int nBind = pTypeData->GetInt( ITEMTYPEDATA_MONOPOLY ) & KIDNAP_PICKUP_MASK ;

	int nPackCount=/*m_pOwner->GetPackSize()*/4;
	for (int i=0; i<nPackCount; i++)
	{
		int nSize = m_setItem[i]->GetAmount();
		for (int j=nSize-1; j>=0; j--)
		{
			CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
			if (pItem && (pItem->GetInt(ITEMDATA_TYPE) == idType && pItem->GetKidnap() == nBind && pItem->GetInt(ITEMDATA_AMOUNT) < pItem->GetInt(ITEMDATA_AMOUNTLIMIT)))
				return pItem;
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
int CUserPackage::GetAmount()
{
	int nLoop = /*m_pOwner->GetPackSize()*/4;
	assert( nLoop >= 1 && nLoop <= 4 );
	int nCount = 0;
	for (int i=0; i< nLoop; i++ )
	{
		int nSize = m_setItem[i]->GetAmount();
		for (int j=nSize-1; j>=0; j--)
		{
			CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
			if (pItem)
				++nCount;
		}
	}
	return nCount;

}

//////////////////////////////////////////////////////////////////////
int CUserPackage::MultiGetItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum, OBJID* pId, int nSize)
{
	CHECKF(pId);

	int nCount = 0;
	int nPackCount=/*m_pOwner->GetPackSize()*/4;
	for (int i=0; i<nPackCount; i++)
	{
		int nAmount = m_setItem[i]->GetAmount();
		for (int j=nAmount-1; j>=0; j--)
		{
			CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
			if (pItem
					&& pItem->GetInt(ITEMDATA_TYPE) >= idTypeFirst
					&& pItem->GetInt(ITEMDATA_TYPE) <= idTypeLast)
			{
				ASSERT(nCount+1 < nSize);
				pId[nCount++] = pItem->GetID();
				if (nCount >= nNum)
					return nCount;
			}
		}
	}
	return nCount;
}

//////////////////////////////////////////////////////////////////////
bool CUserPackage::MultiCheckItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum)
{
	int nCount = 0;
	int nPackCount=/*m_pOwner->GetPackSize()*/4;
	for (int i=0; i<nPackCount; i++)
	{
		int nAmount = m_setItem[i]->GetAmount();
		for (int j=nAmount-1; j>=0; j--)
		{
			CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
			if (pItem
					&& pItem->GetInt(ITEMDATA_TYPE) >= idTypeFirst
					&& pItem->GetInt(ITEMDATA_TYPE) <= idTypeLast)
			{
				if (++nCount >= nNum)
					return true;
			}
		}
	}
	return false;
}
bool CUserPackage::HasLockedItem(int package)
{
	int nPack=package - 47;
	int nSize = m_setItem[nPack]->GetAmount();
	for (int j=nSize-1; j>=0; j--)
	{
		CItemPtr pItem = m_setItem[nPack]->GetObjByIndex(j);
		if (pItem->IsLocked())
			return true;
	}
	return false;
}
bool CUserPackage::OrderPackage(int package)
{	
	CHECKF(m_pOwner);
	if(package < ITEMPOSITION_PACK_BEGIN || package >= ITEMPOSITION_PACK_END)
		return false;
	if (HasLockedItem(package))
	{
		m_pOwner->SendSysMsg(_TXTATR_TOPSHOW, "����Ʒ��������״̬,�޷�����");
		return false;
	}
	//��ʼ����
	//1.ȫ��Ϊ��
	package -= ITEMPOSITION_PACK_BEGIN;
	for (int i = 1;i < 40; i ++)
	{
		SetIndexState( package + ITEMPOSITION_PACK_BEGIN,i,0 );
	}
	

	int nSize = m_setItem[package]->GetAmount();
	if(nSize == 0)
		return true;
	bool bOK = false;
	bool* pHasOrder = new bool[nSize];
	int curIndex = 1;
	OBJID delIndexArray[40];
	int delAmount = 0;

	int indexArray[40];
	int hasAmount = 0;
	OBJID nMaxType = ID_NONE;
	CItemPtr pItem = NULL;
	CItemPtr pItemBin = NULL;
	CItemPtr pItemNoBin = NULL;
	int itemNum = 0;
	int itemMaxNum = 0;

	for (i = 0;i < nSize; i ++)
	{
		pHasOrder[i] = false;
	}

	while(!bOK)
	{
		//�����type
		hasAmount = 0;
		nMaxType = ID_NONE;
		pItemBin = NULL;
		pItemNoBin = NULL;
		
		for (i=0; i < nSize; i++)
		{
			//�����ǹ���
			if(pHasOrder[i])
				continue;

			//�������Ʒ����
			pItem = m_setItem[package]->GetObjByIndex(i);
			if(!pItem)
			{
				LOGWARNING("�������Ʒ");
				pHasOrder[i] = true;
				continue;
			}

			if(pItem->GetPosition() != package + ITEMPOSITION_PACK_BEGIN)
			{
				LOGWARNING("�������Ʒ,��ֵ�λ��");
				pHasOrder[i] = true;
				continue;
			}


			//����type
			if(pItem->GetTypeID() > nMaxType)
			{
				nMaxType = pItem->GetTypeID();
				indexArray[0] = i;
				hasAmount = 1;				
			}
			else if(pItem->GetTypeID() == nMaxType)
			{
				indexArray[hasAmount++] = i;
			}
		}

		//���type������
		if(hasAmount > 1)
		{
			//��Ҫ���Ǻϲ�
			for(i=0; i < hasAmount; i++)
			{
				pHasOrder[indexArray[i]] = true;

				pItem = m_setItem[package]->GetObjByIndex(indexArray[i]);
				if(!pItem)
				{
					LOGWARNING("�������Ʒ�����!");
					continue;
				}
				if (pItem->GetTypeID()/1000 == 301 || pItem->GetTypeID()/1000 == 321)//�ɵĴ��ʹ����Ƚ����⣬ʹ���˵���������ΪҩƷ����������Ҫ���⴦��һ��
					itemMaxNum = pItem->GetInt(ITEMDATA_LIFE);
				else if (pItem->GetTypeID()/1000 == 311)
					itemMaxNum = pItem->GetInt(ITEMDATA_MANA);
				else if (pItem->GetTypeID() == _SPCITEM_VASEXCHANGE)//Ԫ��Ʊ���ϲ�
					itemMaxNum = pItem->GetInt(ITEMDATA_AMOUNT);
				else
					itemMaxNum = pItem->GetInt(ITEMDATA_AMOUNT_LIMIT_INIT);

				if(pItem->GetKidnap() == 1)
				{
					if(pItemBin)
					{
						//���
						int itemNum = pItem->GetItemAmount() + pItemBin->GetItemAmount();
						if(itemNum <= itemMaxNum)
						{
							pItemBin->SetItemAmount(itemNum);
							delIndexArray[delAmount++] = pItem->GetID();							
						}
						else
						{
							pItemBin->SetItemAmount(itemMaxNum);
							if(itemMaxNum != pItemBin->GetItemAmountLimit())
								pItemBin->SetInt(ITEMDATA_AMOUNTLIMIT,itemMaxNum);
							pItemBin->SetPackageIndex(curIndex);						
							curIndex ++;

							pItemBin = pItem;
							pItemBin->SetItemAmount(itemNum - itemMaxNum);
						}
					}
					else 
						pItemBin = pItem;
				}
				else
				{
					if(pItemNoBin)
					{
						//���
						int itemNum = pItem->GetItemAmount() + pItemNoBin->GetItemAmount();
						if(itemNum <= itemMaxNum)
						{
							pItemNoBin->SetItemAmount(itemNum);
							delIndexArray[delAmount++] = pItem->GetID();
						}
						else
						{
							pItemNoBin->SetItemAmount(itemMaxNum);
							if(itemMaxNum != pItemNoBin->GetItemAmountLimit())
								pItemNoBin->SetInt(ITEMDATA_AMOUNTLIMIT,itemMaxNum);
							pItemNoBin->SetPackageIndex(curIndex);						
							curIndex ++;
							
							pItemNoBin = pItem;
							pItemNoBin->SetItemAmount(itemNum - itemMaxNum);
						}
					}
					else 
						pItemNoBin = pItem;
				}
			}

			if(pItemBin)
			{
				pItemBin->SetPackageIndex(curIndex);
				if(itemMaxNum != pItemBin->GetItemAmountLimit())
					pItemBin->SetInt(ITEMDATA_AMOUNTLIMIT,itemMaxNum);
				curIndex ++;
			}
			if(pItemNoBin)
			{
				pItemNoBin->SetPackageIndex(curIndex);	
				if(itemMaxNum != pItemNoBin->GetItemAmountLimit())
					pItemNoBin->SetInt(ITEMDATA_AMOUNTLIMIT,itemMaxNum);
				curIndex ++;
			}
		}
		else if(hasAmount == 1)
		{
			pHasOrder[indexArray[0]] = true;

			//���ϲ�
			pItem = m_setItem[package]->GetObjByIndex(indexArray[0]);

			pItem->SetPackageIndex(curIndex);	
			if (pItem->GetTypeID()/1000 == 301 || pItem->GetTypeID()/1000 == 321)//�ɵĴ��ʹ����Ƚ����⣬ʹ���˵���������ΪҩƷ����������Ҫ���⴦��һ��
				itemMaxNum = pItem->GetInt(ITEMDATA_LIFE);
			else if (pItem->GetTypeID()/1000 == 311)
				itemMaxNum = pItem->GetInt(ITEMDATA_MANA);
			else
				itemMaxNum = pItem->GetInt(ITEMDATA_AMOUNT_LIMIT_INIT);
			if(itemMaxNum != pItem->GetItemAmountLimit())
				pItem->SetInt(ITEMDATA_AMOUNTLIMIT,itemMaxNum);
			curIndex ++;
		}
		else
			bOK = true;
	}

	delete []pHasOrder;

	//ɾ��
	for (i=0; i<delAmount; i++)
	{
		m_pOwner->EraseItem(delIndexArray[i], SYNCHRO_FALSE);
	}

	//��λ��
	if(curIndex > 0 && curIndex < 41)
	{
		for(i=0; i<curIndex; i++)
		{
			SetIndexState( package + ITEMPOSITION_PACK_BEGIN,i,1 );
		}
	}

	//ͬ���ͻ���
	nSize = m_setItem[package]->GetAmount();
	if(nSize == 0)
		return true;

	CMsgItem msg;
	if (!msg.Create(ITEMACT_ADD,0))
		return false;
	int nAmount = 0;
	
	for (i=0; i<nSize; i++)
	{
		pItem = m_setItem[package]->GetObjByIndex(i);
		if (pItem)
		{
			if(!msg.Add(pItem->GetID(),pItem->GetTypeID(),pItem->GetItemAmount(),pItem->GetInt(ITEMDATA_AMOUNTLIMIT), pItem->GetPosition(),pItem->GetKidnap(),pItem->GetPackageIndex(),pItem->GetInt(ITEMDATA_BUYPRICE),pItem->GetColour()))
			{
				m_pOwner->SendMsg(&msg);
				if (!msg.Create(ITEMACT_ADD,0))
					return false;
				if (!msg.Add(pItem->GetID(),pItem->GetTypeID(),pItem->GetItemAmount(),pItem->GetInt(ITEMDATA_AMOUNTLIMIT), pItem->GetPosition(),pItem->GetKidnap(),pItem->GetPackageIndex(),pItem->GetInt(ITEMDATA_BUYPRICE),pItem->GetColour()))
					return false;
				nAmount = 1;
			}
			nAmount ++;
		}
	}
	if(nAmount > 0)
		m_pOwner->SendMsg(&msg);
	return true;
}
//////////////////////////////////////////////////////////////////////
int CUserPackage::RandDropItem(int nMapType, int nChance)
{
	CHECKF(m_pOwner);

	const int	DROP_ITEM_PACK_TYPES	= ITEMPOSITION_PACK_END - ITEMPOSITION_PACK_BEGIN + 1;

	int nDropNum = 0;
	for (int i=0; i<DROP_ITEM_PACK_TYPES; i++)
	{
		///ħ�����еĵ�������������
	/*	if (i==ITEMPOSITION_MAGICSTONEBOX)
			continue;*/
		int nSize = m_setItem[i]->GetAmount();
		for (int j=nSize-1; j>=0; j--)
		{
			CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
			if( pItem->GetKidnap() || pItem->IsKidnapWhenPickup())//-------���---2007/04/09---�жϰ�װ�����������--
				continue;

			if (pItem && rand()%100 < nChance)
			{
				switch (nMapType)
				{
				case _MAP_NONE:
					break;
				case _MAP_PKFIELD:
				case _MAP_SYN:
					if (!pItem->IsEatEnable() && !pItem->IsArmor())
						continue;
					break;
				case _MAP_PRISON:
					break;
				}

				POINT	pos;
				pos.x	= m_pOwner->GetPosX();
				pos.y	= m_pOwner->GetPosY();
				if (m_pOwner->GetMap()->FindDropItemCell(USERDROPITEM_RANGE, &pos))
				{
					if (m_pOwner->DropItem(pItem->GetID()))
						++nDropNum;
				}
			}
		}
	}

	return nDropNum;
}

//////////////////////////////////////////////////////////////////////
int CUserPackage::RandDropItem(int nDropNum,bool isPlayerKill)
{
	CHECKF(m_pOwner);
	CHECKF(nDropNum > 0);

	const int	DROP_ITEM_PACK_TYPES	=1;// /*ITEMPOSITION_PACK_END - ITEMPOSITION_PACK_BEGIN + */1;
	int nRealDropNum = 0;
	bool flag = false;
	std::vector<OBJID>	setItems;
	for (int i=0; i<DROP_ITEM_PACK_TYPES; i++)
	{
		for (int j=0; j<m_setItem[i]->GetAmount(); j++)
		{
			CItem* pItem = m_setItem[i]->GetObjByIndex(j);
			if (pItem
				&& !pItem->IsNeverDropWhenDead()
				/*&& !pItem->GetKidnap()*/  //�󶨣� ƭ�˵�
				&& !pItem->IsKidnapWhenPickup())		// ��Щ��Ʒ������Ҳ���������//-------���---2007/04/09-----�ж��Ƿ�Ϊ����Ʒ
			{
				CBooth *pBooth = m_pOwner->QueryBooth();
				if (pBooth && pBooth->IsHaveItem(pItem->GetID()))
				{
					continue;
				}
				if (pItem->IsSoulMark())//goto ��ӡ�Ĳ��ܵ�
					continue;
				setItems.push_back(pItem->GetID());
			}
		}
	}

	int nTotalItemCount = setItems.size();
	{


// ���䱳�����е�ȫ�������ص���Ʒ [�½���9/13/2007]---begin-------------------
	//	for (int j=0; j<nTotalItemCount; j++)
//		for (int j=nTotalItemCount - 1; j >= 0; j--)
//		{
//			if( setItems.empty() )
//				break;
//			OBJID idItem = setItems[j];
//						
//			CItem* pItem = this->GetItem(idItem);
//			if (pItem && pItem->IsDropWhenDead())
//			{
////				if( pItem->GetKidnap() || pItem->IsKidnapWhenPickup())//-------���---2007/04/09---�жϰ�װ�����������--
////					continue;
//				int ntypeID = pItem->GetTypeID();
//				POINT	pos;
//				pos.x	= m_pOwner->GetPosX();
//				pos.y	= m_pOwner->GetPosY();
//				if (m_pOwner->GetMap()->FindDropItemCell(USERDROPITEM_RANGE, &pos))
//				{
//					if (m_pOwner->DropItem(pItem->GetID(), pos.x, pos.y))
//					{
//						setItems.erase(setItems.begin()+j);
//					//	++nRealDropNum;
//					}
//				}
//				else
//					break;
//			}
//		}
//------------------------end---------------------------------------------------------


		//srand( ( unsigned )time( NULL ) );
		// ���ѡ���__min(nDropNum, nTotalItemCount)����Ʒ
		for (int i=0; i<__min(nDropNum, nTotalItemCount); i++)
		{
			
			if( setItems.empty() )
				break;
			int nIdx = rand() % setItems.size();
			OBJID idItem = setItems[nIdx];
			setItems.erase(setItems.begin()+nIdx);
			
			CItem* pItem = this->GetItem(idItem);
			if (pItem)
			{
				if( /*pItem->GetKidnap() ||*/ pItem->IsKidnapWhenPickup())//-------���---2007/04/09---�жϰ�װ�����������--
					return 0;
				if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
				{
					int nAmount = 1;
					if (CItem::IsCountable(pItem->GetTypeID()))
						nAmount = pItem->GetItemAmount();
					::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lost %d %u from red_killed", m_pOwner->GetID(), m_pOwner->GetName(), nAmount, pItem->GetTypeID());
				}
				if (m_pOwner->DropItem(pItem->GetID(), true))
				{
					++nRealDropNum;
	//						m_pOwner->SendSysMsg("��Ϊһ�������ɱ�����ﷸ���㽫����������ϵ�һ���ǰ󶨵���Ʒ");
				}
				else
					break;
			}
			
		}	// end of for loop
	}

	return nRealDropNum;
}
//////////////////////////////////////////////////////////////////////
int	CUserPackage::RanDropEquip(int nDropNum,bool isPlayerKill)
{
	return -1;									//�Ƶ�CUSER��ȥ
	CHECKF(m_pOwner);
	
	const int	DROP_ITEM_EQUIP_TYPES	= ITEMPOSITION_EQUIPEND - ITEMPOSITION_EQUIPBEGIN+ 1;
	int nRealDropNum = 0;
	bool flag = false;
	std::vector<OBJID>	setItems;
	for (int i=0; i<DROP_ITEM_EQUIP_TYPES; i++)
	{
		for (int j=0; j<m_setItem[i]->GetAmount(); j++)
		{
			CItem* pItem = m_setItem[i]->GetObjByIndex(j);
			if (pItem
				&& !pItem->IsNeverDropWhenDead()
				/*&& !pItem->GetKidnap()*/
				&& !pItem->IsKidnapWhenPickup())		// ��Щ��Ʒ������Ҳ���������//-------���---2007/04/09-----�ж��Ƿ�Ϊ����Ʒ
			{
				CBooth *pBooth = m_pOwner->QueryBooth();
				if (pBooth && pBooth->IsHaveItem(pItem->GetID()))
				{
					continue;
				}
				setItems.push_back(pItem->GetID());
			}
		}
	}
	
	int nTotalItemCount = setItems.size();
	{
		//srand( ( unsigned )time( NULL ) );
		// ���ѡ���__min(nDropNum, nTotalItemCount)����Ʒ
		for (int i=0; i<__min(nDropNum, nTotalItemCount); i++)
		{
			
			if( setItems.empty() )
				break;
			int nIdx = rand() % setItems.size();
			OBJID idItem = setItems[nIdx];
			setItems.erase(setItems.begin()+nIdx);
			
			CItem* pItem = this->GetItem(idItem);
			if (pItem)
			{
				if( /*pItem->GetKidnap() ||*/ pItem->IsKidnapWhenPickup())//-------���---2007/04/09---�жϰ�װ�����������--
					return 0;
				POINT	pos;
				pos.x	= m_pOwner->GetPosX();
				pos.y	= m_pOwner->GetPosY();
				if (m_pOwner->GetMap()->FindDropItemCell(USERDROPITEM_RANGE, &pos))
				{
					if (m_pOwner->DropItem(pItem->GetID()))
					{
						+nRealDropNum;
//						m_pOwner->SendSysMsg("��Ϊһ�������ɱ�����ﷸ���㽫����������ϵ�һ���ǰ󶨵���Ʒ");
					}
				}
				else
					break;
			}
			
		}	// end of for loop
	}
	
	return nRealDropNum;
}
//////////////////////////////////////////////////////////////////////
void CUserPackage::SendAllObjInfo(PROCESS_ID idProcess)
{
	CHECK(m_pOwner);

	int nPackCount=/*m_pOwner->GetPackSize()*/3;
	for (int i=0; i<nPackCount; i++)
	{
		int nSize = m_setItem[i]->GetAmount();
		for (int j=nSize-1; j>=0; j--)
		{
			CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
			if(pItem)
			{
				ItemInfoStructSend info;
				pItem->GetInfo(&info);
				m_pOwner->SendObjInfo(idProcess, m_pOwner->GetID(), INFO_ITEM, &info, sizeof(info));
			}
		}
	}

	///����ħ����
// 	int nSize = m_setItem[4]->GetAmount();
// 	for (int j=0; j<nSize; j++)
// 	{
// 		CItemPtr pItem = m_setItem[4]->GetObjByIndex(j);
// 		if (pItem)
// 		{
// 		
// 			ItemInfoStruct info;
// 			pItem->GetInfo(&info);
// 			pMapGroup->QueryIntraMsg()->SendObjInfo(idProcess, m_pOwner->GetID(), INFO_ITEM, &info, sizeof(info));
// 		}
// 	}
}


//////////////////////////////////////////////////////////////////////
OBJID CUserPackage::RandGetItem()
{
	CHECKF(m_pOwner);

	int nRealDropNum = 0;
	int nItemCount = this->GetAmount();
	if (nItemCount <= 0)
		return ID_NONE;

	typedef std::vector<int>	PACKTYPE_SET;
	PACKTYPE_SET	setPackType;
	int nPackCount=/*m_pOwner->GetPackSize()*/4;
	for (int i=0; i<nPackCount; i++)
	{
		if (m_setItem[i]->GetAmount() > 0)
			setPackType.push_back(i);
	}

	int nPackType = setPackType[::RandGet(setPackType.size())];
	int nAmount = m_setItem[nPackType]->GetAmount();
	if (nAmount > 0)
	{
		int nIndex = ::RandGet(nAmount);
		CItemPtr pItem = m_setItem[nPackType]->GetObjByIndex(nIndex);
		if (pItem)
			return pItem->GetID();
	}

	return ID_NONE;
}

/////////////////////////////////////////////////////////////
int CUserPackage::GetIndexState( UCHAR usPosition,UCHAR ucIndex )	
{
// 	if(usPosition == ITEMPOSITION_PETPACK)
// 	{
// 		return m_bIndexHasItem[ 3 ][ ucIndex - 1 ]; 
// 	}

	if( usPosition < ITEMPOSITION_PACK_BEGIN 
		|| usPosition > ITEMPOSITION_PACK_END //47---50
		|| ucIndex < 1
		|| ucIndex > _MAX_USERITEMSIZE )//1---40
		return -1;
	return m_bIndexHasItem[ usPosition - ITEMPOSITION_PACK_BEGIN ][ ucIndex - 1 ]; 
}

bool CUserPackage::SetIndexState( UCHAR ucPosition,UCHAR ucIndex,int nStatus )	
{
// 	if(ucPosition == ITEMPOSITION_PETPACK)
// 	{
// 		m_bIndexHasItem[ 3 ][ ucIndex - 1 ] = nStatus; 
// 		return true;
// 	}

	if( ucPosition < ITEMPOSITION_PACK_BEGIN 
		|| ucPosition > ITEMPOSITION_PACK_END 
		|| ucIndex < 1 
		|| ucIndex > _MAX_USERITEMSIZE )
		return false;
	m_bIndexHasItem[ ucPosition - ITEMPOSITION_PACK_BEGIN ][ ucIndex - 1 ] = nStatus; 
	return true;
}

int	CUserPackage::GetFreeIndex( UCHAR ucPosition )
{
	UCHAR usIndex = -1;
// 	if( ucPosition == ITEMPOSITION_PETPACK)
// 		usIndex = 3;
	if( ucPosition >= ITEMPOSITION_PACK_BEGIN || ucPosition < ITEMPOSITION_PACK_END )
	{
		usIndex = ucPosition - ITEMPOSITION_PACK_BEGIN;
	}
	else
	{
		return -1;
	}

	ASSERT( usIndex >= 0 && usIndex < 3 );
	//-----------�������[08.9.26]�޸�-------------------//
	int nMaxItemSize = 0;
	if (ucPosition == 47)
	{
		nMaxItemSize = _MAX_PACK_SIZES[0];
	}
	if (ucPosition == 48)
	{
		 nMaxItemSize = _MAX_PACK_SIZES[1];
	}
	else if (ucPosition == 49)
	{
		nMaxItemSize = _MAX_PACK_SIZES[2];
	}
// 	else if(ucPosition == 99)
// 	{
// 		nMaxItemSize = _MAX_PACK_SIZES[3];
// 	}
	//------------------------------------------------------//
	//�˱�����û�������ﲻ���ж�,Ӧ���ڵ��ô˺���֮ǰ�ж�
	for( int i = 0;i < nMaxItemSize/*_MAX_USERITEMSIZE*/;i++ )
	{
// 		if(i >= 36)
// 		{
// 			LOGWARNING("�����ң��������˰ɣ�userid = %u",m_pOwner->GetID());
// 			break;
// 		}
		if( 0 == m_bIndexHasItem[ usIndex ][ i ] ) //�ҵ���
		{
			return ( i + 1 );
		}
		
	}
//	if (ucPosition==ITEMPOSITION_MAGICSTONEBOX)
//	{
//		get
//	}
	return -1;//Ҳ����
}

void CUserPackage::CheckItemLifeTime(BOOL bUpdateToClient, BOOL bSaveToDatabase)
{
	CHECK(m_pOwner);
	int nPackCount = /*m_pOwner->GetPackSize()*/3;
	for(int packageIndex = 0; packageIndex < nPackCount; packageIndex++)
	{		
		int itemAmount = m_setItem[packageIndex]->GetAmount();
		//	for(int itemIndex = 0; itemIndex < itemAmount; itemIndex++)
		for(int itemIndex = itemAmount - 1; itemIndex >= 0; itemIndex--)	//  [���10/22/2007] BUG0001946 ���ܻ�ɾ����Ʒ���ý���������
		{
			CItem *pItemTemp = m_setItem[packageIndex]->GetObjByIndex(itemIndex);
			if(pItemTemp != NULL)
				m_pOwner->CheckItemLifeTime(pItemTemp, bUpdateToClient, bSaveToDatabase);
		}
	}	
}

// �������������ص���Ʒ [�½���10/13/2007]
void CUserPackage::DropItemWhenIsDead()
{
	return;
	CHECK(m_pOwner);
	
	const int	DROP_ITEM_WHEN_DEAD	= ITEMPOSITION_PACK_END - ITEMPOSITION_PACK_BEGIN + 1;
	int nRealDropNum = 0;
	
	std::vector<OBJID>	setItemdead;
	for (int i=0; i<DROP_ITEM_WHEN_DEAD; i++)
	{
		for (int j=0; j<m_setItem[i]->GetAmount(); j++)
		{
			CItem* pItem = m_setItem[i]->GetObjByIndex(j);
			if (pItem)	
				setItemdead.push_back(pItem->GetID());
		}
	}
	
	int nTotalItemCount = setItemdead.size();
	{
		// ���䱳�����е�ȫ�������ص���Ʒ [�½���9/13/2007]---begin-------------------
		for (int j=nTotalItemCount - 1; j >= 0; j--)
		{
			if( setItemdead.empty() )
				break;
			OBJID idItem = setItemdead[j];
			
			CItem* pItem = this->GetItem(idItem);
			if (pItem && pItem->IsDropWhenDead())
			{
				//				if( pItem->GetKidnap() || pItem->IsKidnapWhenPickup())//-------���---2007/04/09---�жϰ�װ�����������--
				//					continue;
				int ntypeID = pItem->GetTypeID();
				POINT	pos;
				pos.x	= m_pOwner->GetPosX();
				pos.y	= m_pOwner->GetPosY();
				if (m_pOwner->GetMap()->FindDropItemCell(USERDROPITEM_RANGE, &pos))
				{
					if (m_pOwner->DropItem(pItem->GetID()))
					{
						setItemdead.erase(setItemdead.begin()+j);
						//	++nRealDropNum;
					}
				}
				else
					break;
			}
		}
	}

}
