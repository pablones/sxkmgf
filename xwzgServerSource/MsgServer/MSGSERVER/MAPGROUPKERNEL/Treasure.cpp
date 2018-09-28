// Treasure.cpp: implementation of the CTreasure class.
//
//////////////////////////////////////////////////////////////////////

#include "Treasure.h"
#include "MapGroup.h"
#include "ItemType.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTreasure::CTreasure(PROCESS_ID idProcess, CUser* pUser)
{
	ASSERT (pUser);
	
	m_idProcess = idProcess;
	m_pUser		= pUser;
	
	m_setData	= CTreasureSet::CreateNew(true);
}

CTreasure::~CTreasure()
{
	if(m_setData)
		SAFE_RELEASE(m_setData);
}

CTreasure* CTreasure::CreateNew(PROCESS_ID idProcess, CUser* pUser)
{
	CHECKF (pUser);
	
	CTreasure* pTreasure = new CTreasure(idProcess, pUser);
	CHECKF(pTreasure);
	//�ṹ�������Ѿ�����ֵ��
// 	pTreasure->m_idProcess = idProcess;
// 	pTreasure->m_pUser		= pUser;
	
	return pTreasure;
}

// login
//////////////////////////////////////////////////////////////////////
bool CTreasure::CreateAll()
{
	CHECKF(CMapGroup::IsValidMapGroupProcessID(m_idProcess));
	CHECKF(m_setData);
	CHECKF(m_pUser);
	
	char szSQL[256]="";
	sprintf(szSQL,"SELECT * FROM %s WHERE ownerid=%u LIMIT %d", _TBL_TREASURE, m_pUser->GetID(), _MAXTREASURE_COUNT);
	CHECKF(m_setData->Create(szSQL, Database()));
	
	return true;
}
//////////////////////////////////////////////////////////////////////
bool CTreasure::SaveInfo()
{
	for(int i = 0; i < m_setData->GetAmount(); i++)
	{
		CTreasureData* pData = m_setData->GetObjByIndex(i);
		IF_OK(pData)
		{
			pData->Update();
		}
	}
	return true;
}
//////////////////////////////////////////////////////////////////////
int	CTreasure::SendInfoToClient()
{
	int nAmount = 0;
	CMsgLottery msg;
	if (!msg.Create(_LOTTERY_ADD, m_pUser->GetID()))
		return 0;
	for(int i = m_setData->GetAmount()-1; i >= 0; i--)
	{
		CTreasureData* pData = m_setData->GetObjByIndex(i);
		if(pData && pData->GetTypeID())
		{
			//send to client
			msg.Append(pData->GetID(), pData->GetTypeID(), pData->GetAmount(), pData->GetDivide());
			nAmount ++;
			if ((nAmount % 50)==0)//��ֹ���,���50��
			{
				m_pUser->SendMsg(&msg);
				msg.Init();
				if (!msg.Create(_LOTTERY_ADD, m_pUser->GetID()))
					return 0;
			}
		}
	}
	if (msg.m_pInfo->uAmount>0)
		m_pUser->SendMsg(&msg);
	return nAmount;
}
//////////////////////////////////////////////////////////////////////
//���ӿ����ӽ�Ʒ,�Զ�����
OBJID CTreasure::AddTreasure(int nType, int nAmount, int nDivide, CMsgLottery* msgAdd, CMsgLottery* msgSynchor)
{
	CHECKF(m_pUser);
	CHECKF(msgAdd);
	CHECKF(msgSynchor);

	if (nAmount <= 0)
		return ID_NONE;
	if (nType<=0)
		return ID_NONE;

	CItemTypeData* pType = ItemType()->QueryItemType(nType);
	if (!pType)
		return ID_NONE;

	OBJID idTreasure = ID_NONE;
	//��һ�����Զ����ӵĴ���
	if (CItem::IsCountable(nType))
	{
		int nAmountLimit = pType->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT);
		TREASURE_SET setTreasure;
		if (!CollectTreasureSet(setTreasure, nType))
			return ID_NONE;
		for (int i=0; i<setTreasure.size(); i++)
		{
			CTreasureData* pDataTemp = setTreasure[i];
			if (pDataTemp)
			{
				if (pDataTemp->GetAmount() < nAmountLimit)
				{
					idTreasure = pDataTemp->GetID();
					if (pDataTemp->GetAmount() + nAmount <= nAmountLimit)
					{
						pDataTemp->SetAmount(pDataTemp->GetAmount()+nAmount);
						nAmount = 0;
						msgSynchor->Append(idTreasure, nType, pDataTemp->GetAmount(), nDivide);
					}
					else
					{
						nAmount -= (nAmountLimit - pDataTemp->GetAmount());
						pDataTemp->SetAmount(nAmountLimit);
						msgSynchor->Append(idTreasure, nType, pDataTemp->GetAmount(), nDivide);
					}
				}
				if (nAmount == 0)
					break;
			}
		}
	}
	if (nAmount == 0)
		return idTreasure;
	////////////////////////
	
	CTreasureData* pData = NULL;
	pData = CTreasureData::CreateNew();
	CHECKF(pData);
	IF_NOT(pData->Create(GameDataDefault()->GetTreasureData(), idTreasure))
		return ID_NONE;
	
	pData->SetInt(TREASUREDATA_OWNERID, m_pUser->GetID());
	pData->SetInt(TREASUREDATA_ITEMTYPE, nType);
	pData->SetInt(TREASUREDATA_AMOUNT, nAmount);
	pData->SetInt(TREASUREDATA_DIVIDE, nDivide);
	
	idTreasure = pData->InsertRecord();
	m_setData->AddObj(pData);
	msgAdd->Append(idTreasure, nType, nAmount, nDivide);
	return idTreasure;
}
//////////////////////////////////////////////////////////////////////
int	CTreasure::GetFreeSpaceCount()
{
	if (m_setData)
		return _MAXTREASURE_COUNT - m_setData->GetAmount();
	return 0;
}
//////////////////////////////////////////////////////////////////////
//�жϿ����ӵİ����ռ��Ƿ��㹻
bool CTreasure::HasEnoughSpace(int nLotteryCount)
{
	return GetFreeSpaceCount() > nLotteryCount;//Ҫ���ǽ���,���Բ��ܵ���
}
//////////////////////////////////////////////////////////////////////
//�����nAmount�������ӽ���
bool CTreasure::AwardLotteryItem(int nAmount, int nGroup)
{
	if (!m_pUser)
	{
		::LogSave("������ʱ��Ҳ�����");
		return false;
	}
	if (nAmount==0)
		return false;
	CMsgLottery msgShow, msgAdd, msgSynchro;//������Ϣ,�ֱ��ǿͻ���չʾ��ý�Ʒ,��Ʒ��������Ʒ,��Ʒ����Ʒͬ������
	if (!msgShow.Create(_LOTTERY_SHOW, m_pUser->GetID()))
		return false;
	if (!msgAdd.Create(_LOTTERY_ADD, m_pUser->GetID()))
		return false;
	if (!msgSynchro.Create(_LOTTERY_AMOUNT, m_pUser->GetID()))
		return false;

	vector<OBJID> setBroadcast;//��Ҫ���й㲥�Ľ�Ʒ�б�
	int nRand = 0;//0-10000֮��
	CTreasureType* pType = NULL;
	char szText[1024] = "";
	char szTemp[256] = "";
	for(int i = 0; i < nAmount; i++)
	{
		nRand = ::RandGet(10000);
		pType = CTreasure::FindTreasureType(nGroup, nRand);
		if (pType)
		{
			//----����֮������⴦����һ���һ��Ԫ��ûҡ������Ʒ����������һ�°�
			if (pType->GetTypeID()==610054)//ҡ��������֮��
				m_pUser->SetTreasureCost(0);
			else if (i==nAmount-1 && nGroup==3)//���һ�λ���, ֻ���ǽ����Ÿ�
			{
				if (m_pUser->GetTreasureCost()>150000)//�Ѿ�����һ��Ԫ��
				{
					pType = FindTreasureTypeByType(610054, nGroup);
					if (!pType)//������붼�����˰�
					{
						pType = CTreasure::FindTreasureType(nGroup, nRand);//���Ǹ���ԭ���Ķ�����
						::LogSave("CTreasure::AwardLotteryItem!The typeid of kylin maybe wrong");
					}
					else
						m_pUser->SetTreasureCost(0);
				}
			}
			//----
			if (AddTreasure(pType->GetTypeID(), pType->GetAmount(), pType->GetDivide(), &msgAdd, &msgSynchro))//�������������Ʒ
			{
				msgShow.Append(ID_NONE, pType->GetTypeID(), pType->GetAmount(), pType->GetDivide());
				if (pType->isBroadcast())
					setBroadcast.push_back(pType->GetTypeID());
				//::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from lottery into box", m_pUser->GetID(), m_pUser->GetName(), pType->GetAmount(), pType->GetTypeID());
				sprintf(szTemp, "%u(%s) get %d %u from lottery into box", m_pUser->GetID(), m_pUser->GetName(), pType->GetAmount(), pType->GetTypeID());
				strcat(szText, szTemp);
				if(strlen(szText)>=900 || i==nAmount-1)
				{
					::MyLogSave(VALUABLESLOG_FILENAME, szText);
					memset(szText,0L,1024);
				}
				else
					strcat(szText, "\n");
			}
			else
				::LogSave("CTreasure::AwardLotteryItem ����Ʒʧ��.user=%u,itemType=%u,amount=%d", m_pUser->GetID(), pType->GetTypeID(), pType->GetAmount());
		}
		else
			::LogSave("CTreasure::AwardLotteryItem �����û�ж�Ӧ��Ʒ.user=%u,group=%d,nRand=%d", m_pUser->GetID(), nGroup, nRand);
	}

	//���ĺ���id 530001
	if(m_pUser->ProcessAction(1151))
	{
		int crystleAmount =0;
		int crystleDivide =5;
		int crystleId=530001;
		//crystleId=505001;

		if (nGroup==1)
		{
			if(nAmount==10) 
			{
				crystleAmount=4;
			}
			else if(nAmount==50) 
			{
				crystleAmount=22;
			}
		}
		else if (nGroup==2)		
		{
			if(nAmount==10) 
			{
				crystleAmount=6;
			}
			else if(nAmount==50) 
			{
				crystleAmount=32;
			}
		}
		else if (nGroup==3)
		{
			if(nAmount==10) 
			{
				crystleAmount=8;
			}
			else if(nAmount==50) 
			{
				crystleAmount=42;
			}
		}
		if(crystleAmount>0)
		{
			if (AddTreasure(crystleId, crystleAmount,crystleDivide, &msgAdd, &msgSynchro))//����
			{
				msgShow.Append(ID_NONE, crystleId, crystleAmount, 5);
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from lottery into box", m_pUser->GetID(), m_pUser->GetName(), crystleAmount, crystleId);
			}
			else
				::LogSave("CTreasure::AwardLotteryItem ������ʧ��.user=%u,itemtype=%d,amount=%d", m_pUser->GetID(), crystleId, crystleAmount);			
		}

		
	}



	//����
	int nMetalDivide = 5;//���µ����
	OBJID metalType = ID_NONE;
	if (nGroup==1)
		metalType = 610049;
	else if (nGroup==2)
		metalType = 610050;
	else if (nGroup==3)
		metalType = 610051;

	if (AddTreasure(metalType, nAmount, nMetalDivide, &msgAdd, &msgSynchro))//����
	{
		msgShow.Append(ID_NONE, metalType, nAmount, nMetalDivide);
		::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from lottery into box", m_pUser->GetID(), m_pUser->GetName(), nAmount, metalType);
	}
	else
		::LogSave("CTreasure::AwardLotteryItem ������ʧ��.user=%u,itemtype=%d,amount=%d", m_pUser->GetID(), metalType, nAmount);

	SaveInfo();//����֮�󱣴�
	m_pUser->SendMsg(&msgShow);
	if (msgAdd.m_pInfo->uAmount>0)
		m_pUser->SendMsg(&msgAdd);
	if (msgSynchro.m_pInfo->uAmount>0)
		m_pUser->SendMsg(&msgSynchro);

	//����д�,���й㲥
	DEBUG_TRY
	for (int k=setBroadcast.size()-1; k>=0; k--)
	{
		BroadCastTreasure(m_pUser->GetName(), setBroadcast[k], nGroup);
	}
	DEBUG_CATCH("CTreasure::AwardLotteryItem �����ӹ㲥�쳣")
	return true;
}
//////////////////////////////////////////////////////////////////////
//ͨ��ID
CTreasureData* CTreasure::FindTreasureData(OBJID idTreasure)
{
	for(int i = 0; i < m_setData->GetAmount(); i++)
	{
		CTreasureData* pData = m_setData->GetObjByIndex(i);
		if(pData && pData->GetID() == idTreasure)
			return pData;
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////
//����ĳһ�ཱƷ�ļ���
bool CTreasure::CollectTreasureSet(TREASURE_SET& setTreasure, OBJID itemType)
{
	if (itemType == ID_NONE)
		return false;
	setTreasure.clear();
	for(int i = 0; i < m_setData->GetAmount(); i++)
	{
		CTreasureData* pData = m_setData->GetObjByIndex(i);
		if(pData && pData->GetInt(TREASUREDATA_ITEMTYPE) == itemType)
			setTreasure.push_back(pData);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////
//��Ʒ������Ʒȡ��������
bool CTreasure::GetTreasure(OBJID idTreasure)
{
	if (!m_pUser->QueryPackage())
		return false;
	if (idTreasure>=0 && idTreasure<6)//ȡ��ȫ��
	{
		int nPak1=0, nPak2=0,nPak3=0;//,nPakPet=0;
		for(int i = m_setData->GetAmount()-1; i >=0; i--)
		{
			CTreasureData* pData = m_setData->GetObjByIndex(i);
			if(pData)
			{
				if (idTreasure!=0 && pData->GetDivide()!=idTreasure)
					continue;
				int nPosition = m_pUser->QueryPackage()->GetAvailablePosition(pData->GetTypeID());
				if (nPosition==ITEMPOSITION_BACKPACK1)
					nPak1++;
				else if (nPosition==ITEMPOSITION_BACKPACK2)
					nPak2++;
				else if (nPosition==ITEMPOSITION_BACKPACK3)
					nPak3++;
				else
				{
					m_pUser->SendSysMsg(_TXTATR_TOPSHOW, "�����ռ䲻��");
					return false;
				}
// 				else if (nPosition==ITEMPOSITION_PETPACK)
// 					nPakPet++;
			}
		}
		if ( (nPak1>0 && m_pUser->QueryPackage()->IsPackFull(nPak1, 0, ITEMPOSITION_BACKPACK1))
			|| (nPak2>0 && m_pUser->QueryPackage()->IsPackFull(nPak2, 0, ITEMPOSITION_BACKPACK2))
			|| (nPak3>0 && m_pUser->QueryPackage()->IsPackFull(nPak3, 0, ITEMPOSITION_BACKPACK3)))
//			|| m_pUser->QueryPackage()->IsPackFull(nPakPet, 0, ITEMPOSITION_PETPACK))
		{
			m_pUser->SendSysMsg(_TXTATR_TOPSHOW, "�����ռ䲻��");
			return false;
		}
		char szText[1024] = "";
		char szTemp[256] = "";
		OBJID itemType = 0;
		int nAmount = 0;
		for(i = m_setData->GetAmount()-1; i >=0; i--)
		{
			CTreasureData* pData = m_setData->GetObjByIndex(i);
			if(pData)
			{
				if (idTreasure!=0 && pData->GetDivide()!=idTreasure)
					continue;

				itemType = pData->GetTypeID();
				nAmount = pData->GetAmount();
				if (m_pUser->AwardItem(itemType, true, false, true, false, nAmount))
					DelTreasure(pData->GetID());
//modify code by �ֵ�Ԩ begin 2011.5.21
				CItem *pitem_data = m_pUser->GetItemByType(itemType);
				if( pitem_data )
					sprintf(szTemp, "%u(%s) get %d %u from treasure box into package %u total (%d)", m_pUser->GetID(), m_pUser->GetName(), nAmount, itemType,itemType,pitem_data->GetItemAmount());
				else
					sprintf(szTemp, "%u(%s) get %d %u from treasure box into package %u total (%d)", m_pUser->GetID(), m_pUser->GetName(), nAmount, itemType,itemType,0);
//modify code by �ֵ�Ԩ end
				strcat(szText, szTemp);
				if(strlen(szText)>=900 || i==0)
				{
					::MyLogSave(VALUABLESLOG_FILENAME, szText);
					memset(szText,0L,1024);
				}
				else
					strcat(szText, "\n");
			}
		}
		return true;
	}
	else
	{
		CTreasureData* pData = FindTreasureData(idTreasure);
		if (!pData)
			return false;
		if (m_pUser->QueryPackage()->IsPackFull(1, pData->GetTypeID()))
		{
			m_pUser->SendSysMsg(_TXTATR_TOPSHOW, "�����ռ䲻��");
			return false;
		}
		if (m_pUser->AwardItem(pData->GetTypeID(), true, false, true, false, pData->GetAmount()))
		{
//modify code by �ֵ�Ԩ begin 2011.5.21
			CItem *pitem_data = m_pUser->GetItemByType(pData->GetTypeID());
			if( pitem_data )
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from treasure box into package %u total (%d)", m_pUser->GetID(), m_pUser->GetName(), pData->GetAmount(), pData->GetTypeID() ,pData->GetTypeID() ,pitem_data->GetItemAmount());
			else
				::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from treasure box into package %u total (%d)", m_pUser->GetID(), m_pUser->GetName(), pData->GetAmount(), pData->GetTypeID() ,pData->GetTypeID() ,0);
//modify code by �ֵ�Ԩ end
			DelTreasure(idTreasure);
			return true;
		}
		return false;
	}
}
//////////////////////////////////////////////////////////////////////
//ɾ����Ʒ���е���Ʒ
bool CTreasure::DelTreasure(OBJID idTreasure)
{
	if (idTreasure==ID_NONE)
		return false;
	CTreasureData* pData = FindTreasureData(idTreasure);
	if(!pData)
		return false;

	ASSERT(pData->DeleteRecord());
//modify code by �ֵ�Ԩ begin 2011.5.21	
	::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) DelTreasure %d %u from box", m_pUser->GetID(), m_pUser->GetName(), pData->GetAmount(), pData->GetTypeID());
//modify code by �ֵ�Ԩ end	
	m_setData->DelObj(pData->GetID());
	
	CMsgLottery msg;
	if (msg.Create(_LOTTERY_DEL, m_pUser->GetID()))
	{
		msg.Append(idTreasure, 0, 0, 0);
		m_pUser->SendMsg(&msg);
	}

	return true;
}
//////////////////////////////////////////////////////////////////////
CTreasureType* CTreasure::FindTreasureType(int nGroup, int nRand)
{
	for(int i = 0; i < TreasureTypeSet()->GetAmount(); i++)
	{
		CTreasureType* pType = TreasureTypeSet()->GetObjByIndex(i);
		if(pType && pType->GetInt(TREASURETYPE_GROUP) == nGroup)
		{
			if (nRand >= pType->GetInt(TREASURETYPE_START) && nRand < pType->GetInt(TREASURETYPE_END))
				return pType;
		}
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////
CTreasureType* CTreasure::FindTreasureTypeByType(OBJID nType, int nGroup)
{
	for(int i = 0; i < TreasureTypeSet()->GetAmount(); i++)
	{
		CTreasureType* pType = TreasureTypeSet()->GetObjByIndex(i);
		if(pType && pType->GetTypeID()==nType && pType->GetInt(TREASURETYPE_GROUP)==nGroup)
		{
			return pType;
		}
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////
//���ҡ�д�,ȫ���㲥һ��
void CTreasure::BroadCastTreasure(LPCTSTR szUserName, OBJID itemType, int nGroup)
{
	CHECK(m_pUser);
	CHECK(nGroup);

	DEBUG_TRY
	if (!szUserName || strlen(szUserName)<=0 || itemType==0)
		return;
	char szMsg[512]	   = "";
	char itemName[256] = "";
	char UserName[64]  = "";
	char Monster[64]   = "";

	CItemTypeData* pType = ItemType()->QueryItemType(itemType);
	if (!pType)
		return;
	int nColor = pType->GetInt(ITEMTYPEDATA_COLOR_INIT);

	sprintf(itemName,  "%d_[%s]_%d_%d_%d_%d_%d", SNAPSHOTOBJ,  pType->GetStr(ITEMTYPEDATA_NAME), 0, itemType, m_pUser->GetID(), 0, nColor);
	sprintf(UserName, "%d_[%s]_7",  SNAPSHOTUSER, m_pUser->GetName());
	
	if (nGroup == 1)
		sprintf(Monster, "%d_[%s]_%d_%d_%d_%d_%d", SNAPSHOTOBJ, "��ͽ", 0,0,0,0, COLORRANGE);
	else if (nGroup == 2)
		sprintf(Monster, "%d_[%s]_%d_%d_%d_%d_%d", SNAPSHOTOBJ, "����", 0,0,0,0, COLORRANGE);
	else if (nGroup == 3)
		sprintf(Monster, "%d_[%s]_%d_%d_%d_%d_%d", SNAPSHOTOBJ, "����", 0,0,0,0, COLORRANGE);
	else
		return;

	sprintf(szMsg, "<%s>��ս<%s>�����������������<%s>��", UserName, Monster, itemName);

	UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, nGroup, _TXTATR_TREASURE, NULL, 0, itemType, NULL, ALLUSERS_NAME);
	DEBUG_CATCH3("CTreasure::BroadCastTreasure error! userid=%u,itemtype=%u", m_pUser->GetID(), itemType);
}
//////////////////////////////////////////////////////////////////////
//�����ӵļ۸�
int CTreasure::GetTreasurePrice(int nGroup, int nAmount)
{
	if (nGroup<1 || nGroup>3)
		return -1;
	if (nAmount<1 || nAmount>50)
		return -1;
	int nGroupIndex = nGroup-1;
	int nAmountIndex = -1;
	if (nAmount == 1)
		nAmountIndex = 0;
	else if (nAmount == 10)
		nAmountIndex = 1;
	else if (nAmount == 50)
		nAmountIndex = 2;

	if (nGroupIndex>=0 && nGroupIndex<3 && nGroupIndex>=0 && nGroupIndex<3)//��ֹ�±�Խ��
		return _TREASURE_PRICE[nGroupIndex][nAmountIndex];
	
	return -1;
}

bool CTreasure::GetInfoByIndex(TreasureInfoStruct* pInfo, int idx)
{
	CHECKF(pInfo);
	CHECKF(idx >= 0 && idx < m_setData->GetAmount());
	
	CTreasureData* pData = m_setData->GetObjByIndex(idx);
	IF_OK(pData)
	{
		pInfo->id			= pData->GetID();
		pInfo->idOwner		= m_pUser->GetID();
		pInfo->itemType		= pData->GetTypeID();
		pInfo->nAmount		= pData->GetAmount();
		pInfo->nDivide		= pData->GetDivide();
		return true;
	}
	return false;
}

bool CTreasure::AppendTreasure(const TreasureInfoStruct* pInfo)
{
	CHECKF(pInfo);

	CTreasureData* pData = CTreasureData::CreateNew();
	CHECKF(pData);
	IF_NOT(pData->Create(GameDataDefault()->GetTreasureData(), pInfo->id))
		return false;

	pData->SetInt(TREASUREDATA_OWNERID, pInfo->idOwner);
	pData->SetInt(TREASUREDATA_ITEMTYPE, pInfo->itemType);
	pData->SetInt(TREASUREDATA_AMOUNT, pInfo->nAmount);
	pData->SetInt(TREASUREDATA_DIVIDE, pInfo->nDivide);
	pData->ClearUpdateFlags();
	m_setData->AddObj(pData);
	return true;
}