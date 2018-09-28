// Item.cpp: implementation of the CItem class.
//
//////////////////////////////////////////////////////////////////////

#include "Item.h"
#include "ItemType.h"
#include "MapGroup.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include "User.h"
#include "MsgEudemonAttrib.h"
#include "MyArray.h"
//---jinggy---2004-11-12---Begin
#include "netmsg.h"
#include "AllMsg.h"
#include "Console.h"
//---jinggy---2004-11-12---End

MYHEAP_IMPLEMENTATION(CItem,s_heap)

//---tcp2005---9-17-add---begin
const _MAX_GEM_AMOUNT=13;
const unsigned int nGemType[_MAX_GEM_AMOUNT]= {0,
                                               1038000,
                                               1038010,
											   1038020,
											   1038030,
											   1038040,
											   1038050,
											   1038060,
											   1038070,
											   1038080,
											   1038090,
											   1038100,
											   1038110											   
};
const CMyArray<unsigned int,_MAX_GEM_AMOUNT>m_nGemTypeArray(nGemType,_MAX_GEM_AMOUNT);
//---tcp2005---9-17-add---end


//20070825����:��װ��ħ���
const int _DRESSENCHANTMAXCOUNT = 3;//��װ���ħ��
const int _DRESSENCHANTGEMPOS[_DRESSENCHANTMAXCOUNT]={
	EM_GEMPOS_GEM1,
	EM_GEMPOS_GEM2,
	EM_GEMPOS_GEM3};//��װ��ħGEMλ

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CItem::CItem()
{
	SetObjType(OBJ_ITEM);
	m_isLocked = false;
	m_pWpSlData = NULL;
}

//////////////////////////////////////////////////////////////////////
CItem::~CItem()
{
	DeletWpSlData();
}

//////////////////////////////////////////////////////////////////////
int	CItem::CalcRepairMoney()
{
	int nRecoverDurability = __max(0, GetInt(ITEMDATA_AMOUNTLIMIT) - GetInt(ITEMDATA_AMOUNT));
	if (nRecoverDurability == 0)
		return 0;

	// ����۸�=(�־�����-��ǰ�־�)/�־�����*�̵���ۼ۸�
	double nRepairCost	= 0;
	if (GetInt(ITEMDATA_AMOUNTLIMIT) > 0)
		nRepairCost	= (double)GetInt(ITEMDATA_PRICE) * nRecoverDurability / GetInt(ITEMDATA_AMOUNTLIMIT)/* * 0.1*/;
	
	return __max(1, nRepairCost);
}

//////////////////////////////////////////////////////////////////////
DWORD CItem::GetSellPrice()
{
	if( GetInt(ITEMDATA_AMOUNTLIMIT) == 0 )
		return 0;
	int nAmount = this->GetInt(ITEMDATA_AMOUNT);
	if (nAmount > this->GetInt(ITEMDATA_AMOUNTLIMIT))
		nAmount = this->GetInt(ITEMDATA_AMOUNTLIMIT);

	DWORD dwPrice = GetInt(ITEMDATA_PRICEOUT);

	if( CItem::IsCountable( GetInt( ITEMDATA_TYPE ) ) )	//������Ʒ
	{
		//dwPrice = dwPrice / this->GetInt( ITEMDATA_AMOUNT_INIT );
		DWORD dwTotal = dwPrice * nAmount;
		return dwTotal;
	}

	dwPrice = dwPrice;
	return dwPrice;
}

//////////////////////////////////////////////////////////////////////
CItem*	CItem::Split(int nNum, IDatabase* pDb)
{
	CHECKF(pDb);
	CHECKF(IsPileEnable());

	int nCurrNum = GetInt(ITEMDATA_AMOUNT);
	CHECKF(nNum > 0 && nNum < nCurrNum);

	int nStep = 0;
	DEBUG_TRY ////////
	CItemPtr pItem = Clone(pDb);
	CHECKF(pItem);
	nStep = 1;
	pItem->SetInt(ITEMDATA_BUYPRICE,this->GetInt(ITEMDATA_BUYPRICE));
	nStep = 2;
	pItem->SetInt(ITEMDATA_AMOUNT, nNum);
	nStep = 3;
	this->SetInt(ITEMDATA_AMOUNT, nCurrNum - nNum);

	return pItem;
	DEBUG_CATCH2("item split!! nstep = %u",nStep) //////////

	return NULL;
}

//////////////////////////////////////////////////////////////////////
CItem*	CItem::Clone(IDatabase* pDb)		// ����Ʒ�����ݿ�
{
	CHECKF (pDb);
	CItemPtr pItem = CreateNew();
	IF_NOT(pItem)
		return NULL;

	ItemInfoStruct	info;
	memset(&info,0,sizeof(ItemInfoStruct));//20071115����:
	GetInfo(&info);
	info.id		= ID_NONE;		// new record


	IRecord* pWpSlRecord=NULL;
	if(IsWeaponSoul() && m_pWpSlData)
	{
		pWpSlRecord = m_pWpSlData->GetRecord();
	}

	if(pItem->Create(pDb, 
		m_pData->GetRecord(), 
		&info, 
		NULL,
		pWpSlRecord,
		true))
		return pItem;


	pItem->ReleaseByOwner();
	return NULL;
}

//////////////////////////////////////////////////////////////////////
int	CItem::GetItemSubType()
{
	// ȡʮ����ǧλ
	return (GetInt(ITEMDATA_TYPE)%1000000)/1000;
}

//////////////////////////////////////////////////////////////////////
int	CItem::GetLevel(void)
{
	return /*GetInt( ITEMDATA_LEVEL )*/0;
}

//////////////////////////////////////////////////////////////////////
/*
 �Ա� cq_item �е� type, �ı� type ��ĩβ���Ǹı�װ����Ʒ�� ;
 ͨ�� type �� magic3 �Ϳɶ�λ��  �� cq_itemaddition (��ͬƷ�ʼ�׷�����Ե�װ����Ӧ����������)
*/
bool CItem::ChangeType	(OBJID idNewType) //
{
	// type
	CItemTypeData *pType = ItemType()->QueryItemType(idNewType);
	if(!pType)
	{
		LOGERROR("ChangeType����[%d]����!", idNewType);

		return false;
	}
	
	m_pType = pType;
	this->SetInt(ITEMDATA_TYPE, idNewType);

//	m_pAddition = ItemAddition()->QueryItemAddition(idNewType, GetInt(ITEMDATA_ADDITION));

	return true;
}


//////////////////////////////////////////////////////////////////////
bool CItem::IsNonsuchItem(void)
{
	DWORD dwType = GetTypeID();
	// precious gem
	if (this->IsGem() && (dwType%10) >= 2)
		return true;

	// other type
	if (CItem::GetItemSort(dwType) == ITEMSORT_OTHER || CItem::GetItemSort(dwType) >= ITEMSORT_EXPEND)
		return false;

	// ....
	int nGem1 = this->GetInt(ITEMDATA_GEM1)%10;
	int nGem2 = this->GetInt(ITEMDATA_GEM2)%10;

	bool bIsNonsuch = false;
	if(IsWeapon())
	{
		if ((nGem1 != GEM_HOLE && nGem1 >= 2)
				|| (nGem2 != GEM_HOLE && nGem2 >= 2))
			bIsNonsuch = true;
	}
	else if(IsFinery1())
	{
		if (nGem1 != GEM_NONE || nGem2 != GEM_NONE)
			bIsNonsuch = true;
	}

	return bIsNonsuch;
}

//////////////////////////////////////////////////////////////////////
bool CItem::ChangeColor(int nColor)
{
	if (!(nColor >=3 && nColor <= 9))
		return false;

	if(IsHelmet() || IsArmor())
	{
		DWORD idType = this->GetInt(ITEMDATA_TYPE);
		DWORD idNewType = idType-((idType/100)%10)*100+nColor*100;
		return this->ChangeType(idNewType);
	}

	return false;
}



//������һ����..
bool CItem::CreateHole()
{
	for(int i=EM_GEMPOS_BEGIN; i<=EM_GEMPOS_END; i++)
	{
		if (GetGemStatus(i)==_NOT_HOLE)
		{
			SetGemStatus( i,_HOLE ); 
			return true;
		}
	}
	return false;
}
//���װ��ǿ�������Ǻ�����Լӳ�ϵ��
float CItem::GetEquipAddRate(int nLev)
{
	float nRate = 0;
	switch(nLev)
	{
	case 1:	nRate=1.1f;	break;
	case 2:	nRate=1.2f;	break;
	case 3:	nRate=1.3f;	break;
	case 4:	nRate=1.4f;	break;
	case 5:	nRate=1.55f;	break;
	case 6:	nRate=1.7f;	break;
	case 7:	nRate=1.85f;	break;
	case 8:	nRate=2.05f;	break;
	case 9:	nRate=2.25f;	break;
	case 10:nRate=2.6f;	break;
	}
	return nRate;
}
//[2010-07-22 goto]ǿ�������װ����������
//goto �¼Ӳ���nType��ʾ��ǿ�����ǻ�ӡ��0=ǿ����1=��ӡ
void CItem::UpdateEquipBaseProperty(int nOldLev, int nType/*=0*/)
{
	int nLev = GetEquipAddLev();
	if (nLev == nOldLev && nType==0)//ǿ���ȼ��ޱ仯ʱ����Ҫ����ǿ����������
		return;
	if (nLev==0 && !IsSoulMark())
		return;
	float nRate = GetEquipAddRate(nLev);
	float nLastRate=1;
	if (nLev>1)//�����1�����ϵ�Ҫ�Ȼ�ԭ��0���������ٳ���ϵ��
	{
		nLastRate = GetEquipAddRate(nOldLev);
	}
	if (IsSoulMark())//goto ��ӡ +10%
	{
		nRate += 0.1f;
		if (nRate==0.1f)
			nRate=1.1f;
		if (nType!=1)//������ǻ�ӡ��������ԣ����뻹ԭ�ɻ�ӡǰ������
			nLastRate += 0.1f;
	}
	int nSort1 = GetInt( ITEMDATA_BASE1 ) / 10000;
	int nSort2 = GetInt( ITEMDATA_BASE2 ) / 10000;
	int nOldBase1 = GetInt( ITEMDATA_BASE1 )%10000*1.0/nLastRate+0.5;//�������룬��С���
	int nOldBase2 = GetInt( ITEMDATA_BASE2 )%10000*1.0/nLastRate+0.5;

	SetInt(ITEMDATA_BASE1, nSort1*10000+nOldBase1*nRate+0.5);
	SetInt(ITEMDATA_BASE2, nSort2*10000+nOldBase2*nRate+0.5);
}
//[2010-07-22 goto]���Ǻ����װ����������
void CItem::UpdateEquipAdditionProperty(int nOldLev)
{
	int nLev = GetEquipStar();
	if (nLev == nOldLev)
		return;
	if (nLev==0)
		return;
	float nRate = GetEquipAddRate(nLev);
	float nLastRate=1;
	if (nLev>1)//�����1�����ϵ�Ҫ�Ȼ�ԭ��0���������ٳ���ϵ��
	{
		nLastRate = GetEquipAddRate(nOldLev);
	}
	int nSort1 = GetInt( ITEMDATA_BASIC_ADDITION1 ) / 10000;
	int nSort2 = GetInt( ITEMDATA_BASIC_ADDITION2 ) / 10000;
	int nSort3 = GetInt( ITEMDATA_BASIC_ADDITION3 ) / 10000;
	int nSort4 = GetInt( ITEMDATA_BASIC_ADDITION4 ) / 10000;
	int nSort5 = GetInt( ITEMDATA_BASIC_ADDITION5 ) / 10000;
	int nSort6 = GetInt( ITEMDATA_BASIC_ADDITION6 ) / 10000;
	int nSort7 = GetInt( ITEMDATA_BASIC_ADDITION7 ) / 10000;
	int nSort8 = GetInt( ITEMDATA_BASIC_ADDITION8 ) / 10000;
	//��ԭ��0��ʱ������
	int nOldAddition1 = GetInt( ITEMDATA_BASIC_ADDITION1 )%10000*1.0/nLastRate+0.5;//�������룬��С���
	int nOldAddition2 = GetInt( ITEMDATA_BASIC_ADDITION2 )%10000*1.0/nLastRate+0.5;
	int nOldAddition3 = GetInt( ITEMDATA_BASIC_ADDITION3 )%10000*1.0/nLastRate+0.5;
	int nOldAddition4 = GetInt( ITEMDATA_BASIC_ADDITION4 )%10000*1.0/nLastRate+0.5;
	int nOldAddition5 = GetInt( ITEMDATA_BASIC_ADDITION5 )%10000*1.0/nLastRate+0.5;
	int nOldAddition6 = GetInt( ITEMDATA_BASIC_ADDITION6 )%10000*1.0/nLastRate+0.5;
	int nOldAddition7 = GetInt( ITEMDATA_BASIC_ADDITION7 )%10000*1.0/nLastRate+0.5;
	int nOldAddition8 = GetInt( ITEMDATA_BASIC_ADDITION8 )%10000*1.0/nLastRate+0.5;
	//����Ϊ�µȼ�����
	SetInt(ITEMDATA_BASIC_ADDITION1, nSort1*10000+nOldAddition1*nRate+0.5);
	SetInt(ITEMDATA_BASIC_ADDITION2, nSort2*10000+nOldAddition2*nRate+0.5);
	SetInt(ITEMDATA_BASIC_ADDITION3, nSort3*10000+nOldAddition3*nRate+0.5);
	SetInt(ITEMDATA_BASIC_ADDITION4, nSort4*10000+nOldAddition4*nRate+0.5);
	SetInt(ITEMDATA_BASIC_ADDITION5, nSort5*10000+nOldAddition5*nRate+0.5);
	SetInt(ITEMDATA_BASIC_ADDITION6, nSort6*10000+nOldAddition6*nRate+0.5);
	SetInt(ITEMDATA_BASIC_ADDITION7, nSort7*10000+nOldAddition7*nRate+0.5);
	SetInt(ITEMDATA_BASIC_ADDITION8, nSort8*10000+nOldAddition8*nRate+0.5);
}

OBJID CItem::GetGemStatus( int nPos )
{
	CHECKF( nPos >=EM_GEMPOS_BEGIN && nPos <= EM_GEMPOS_END );

	OBJID nResult = 0;
	switch( nPos )
	{
	case EM_GEMPOS_GEM1:
		nResult = GetInt( ITEMDATA_GEM1 );
		break;
	case EM_GEMPOS_GEM2:
		nResult = GetInt( ITEMDATA_GEM2 );
		break;
	case EM_GEMPOS_GEM3:
		nResult = GetInt( ITEMDATA_GEM3 );
		break;
	default:
		break;
	}
	return nResult;
}
OBJID CItem::GetAddStatus( int nPos )
{
	CHECKF( nPos >=EM_ADDPOS_BEGIN && nPos <= EM_ADDPOS_END );
	
	OBJID nResult = 0;
	switch( nPos )
	{
	case EM_ADDPOS_GEM1:
		nResult = GetInt( ITEMDATA_BASIC_ADDITION1 );
		break;
	case EM_ADDPOS_GEM2:
		nResult = GetInt( ITEMDATA_BASIC_ADDITION2 );
		break;
	case EM_ADDPOS_GEM3:
		nResult = GetInt( ITEMDATA_BASIC_ADDITION3 );
		break;
	case EM_ADDPOS_GEM4:
		nResult = GetInt( ITEMDATA_BASIC_ADDITION4 );
		break;
	case EM_ADDPOS_GEM5:
		nResult = GetInt( ITEMDATA_BASIC_ADDITION5 );
		break;
	case EM_ADDPOS_GEM6:
		nResult = GetInt( ITEMDATA_BASIC_ADDITION6 );
		break;
	case EM_ADDPOS_GEM7:
		nResult = GetInt( ITEMDATA_BASIC_ADDITION7 );
		break;
	case EM_ADDPOS_GEM8:
		nResult = GetInt( ITEMDATA_BASIC_ADDITION8 );
		break;
	default:
		break;
	}
	return nResult;
}
OBJID CItem::GetBasicStatus( int nPos )
{
	CHECKF( nPos >=EM_BASEPOS_BEGIN && nPos <= EM_BASEPOS_END );
	
	OBJID nResult = 0;
	switch( nPos )
	{
	case EM_BASEPOS_GEM1:
		nResult = GetInt( ITEMDATA_BASE1 );
		break;
	case EM_BASEPOS_GEM2:
		nResult = GetInt( ITEMDATA_BASE2 );
		break;
	default:
		break;
	}
	return nResult;
}

//����������ĳһ���׵�״̬
//20070105����:����������ʯλ.��7,8
//07.1.8�����޸�
//20070825����:��enum
void CItem::SetGemStatus( unsigned char ucPos,OBJID idType,bool bUpdate )// bupdateĬ��ֵΪfalse
{

	CHECK( ucPos >=EM_GEMPOS_BEGIN && ucPos <= EM_GEMPOS_END );

	switch( ucPos )
	{
	case EM_GEMPOS_GEM1:SetInt( ITEMDATA_GEM1,idType,bUpdate );break;
	case EM_GEMPOS_GEM2:SetInt( ITEMDATA_GEM2,idType,bUpdate );break;
	case EM_GEMPOS_GEM3:SetInt( ITEMDATA_GEM3,idType,bUpdate );break;
	default:
		{
			LogSave( "Invalid Pos :%d in SetGemStatus()",ucPos );
			return;
		}
		break;
	}

	//20070312����:ԭupdate��д
	if(bUpdate)
	{
		this->SaveInfo();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//Time:06.8.17
//Author:�����޸�
//��̬����
//return true:��ʾ����Ʒ���Ե���,false:������
////////////////////////////////////////////////////////////////////////////////////////
bool CItem::IsCountable( int nType )
{
	DWORD dwType = nType / 100000;
	if (dwType == 1 || dwType == 2)
	{
		return false;
	}

	if(nType / 1000 == 301 || nType / 1000 == 311 || nType / 1000 == 321 || nType == _SPCITEM_VASEXCHANGE || nType == _SPCITEM_EXPEXCHANGE || nType == _SPCITEM_MONEYFORSHOP)
	{
		return false;
	}
	
	CItemTypeData* pType =ItemType()->QueryItemType(nType);
	if (!pType) 
	{
		return false;
	}
	int iLimit = pType->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT);
	if (iLimit > 1 && iLimit <= 10000)
	{
		return true;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////////////
//06.11.7
//����
//����˵��:��̬����ͨ����Ʒ������,�õ�������ƷCD��MAP�е�����
//06.11.17�����޸�,Ϊ���ڿͻ������ּ��ܵ�CD,Ϊ������һ��ƫ����
int CItem::GetDelayGroupIndex( OBJID idType )
{
	const int nRetOffset = 60000;
	int nFormatValue = idType / 10000;
	int nSort = idType/100000;
	if(IsActionItem(idType))//20070825����:action��Ʒͳһcd��
		return 1 + nRetOffset;
	else return -1;//��Чֵ
}

//---------------------------------------------
bool CItem::IsCanEmbedEquip()
{
	if(!IsWeaponSoul())
	{
		return IsCanUpLevel();
	}
	return true;
}
//[2010-07-21 goto]��ȡװ���ϵ�һ���յĿ�λ��
int	CItem::GetEmbedPos()
{
	for(int i=EM_GEMPOS_BEGIN; i<=EM_GEMPOS_END; i++)
	{
		int nStatus = GetGemStatus(i);
		if (nStatus==_HOLE)
			return i;
	}
	return 0;
}
//[2010-07-19 goto] װ�����Ƿ��Ѿ���Ƕ�˴��౦ʯ
bool CItem::IsHaveSameGemEmbed(OBJID idGemType)
{
// 	for(int i=EM_GEMPOS_BEGIN; i<=EM_GEMPOS_END; i++)
// 	{
// 		int nStatus = GetGemStatus(i);
// 		if (nStatus/100==idGemType/100)
// 			return true;
// 	}
	DEBUG_TRY
	int lastGem=ItemType()->QueryItemType(idGemType)->GetInt(ITEMTYPEDATA_BASE1_INIT)/10000;
	for(int i=EM_GEMPOS_BEGIN; i<=EM_GEMPOS_END; i++)
	{
		int nStatus = GetGemStatus(i);
		if(nStatus == 0 || nStatus==_NOT_HOLE || nStatus == _HOLE || CItem::GetItemSort(nStatus)!=4)
			continue;
		int firstGem=ItemType()->QueryItemType(nStatus)->GetInt(ITEMTYPEDATA_BASE1_INIT)/10000;

		if (firstGem==lastGem)
			return true;
		if ( (lastGem>=1 && lastGem<=2) && (firstGem>=1 && firstGem<=2) )
			return true;
		if ( (lastGem>=20 && lastGem<=23) && (firstGem>=20 && firstGem<=23) )
			return true;
		if ( (lastGem>=3 && lastGem<=4) && (firstGem>=3 && firstGem<=4) )
			return true;
		if ( (lastGem>=24 && lastGem<=27) && (firstGem>=24 && firstGem<=27) )
			return true;
		if ( (lastGem>=9 && lastGem<=13) && (firstGem>=9 && firstGem<=13) )
			return true;
	}
	DEBUG_CATCH("bool CItem::IsHaveSameGemEmbed");
	return false;
}
//Ҫ��Ƕ�ı�ʯ�����Ƿ���װ��ƥ��
bool CItem::IsRightGemType(OBJID idGemType)
{
	int nType = GetItemType();
	if (nType==ITEMTYPE_NECKLACE
		|| nType==ITEMTYPE_WEAPONSOUL)//���������ֱ�ʯ��������Ƕ
		return true;
	
	if (idGemType/10000==41)//�����౦ʯ
	{
		if (nType==ITEMTYPE_RING		//��ָ
			|| nType==IETMTYPE_ADORN	//��Ʒ
			|| nType==ITEMTYPE_BANGLE	//����
			|| nType==ITEMTYPE_WEAPONR)	//����
			return true;
	}
	else if(idGemType/10000==42)//�����౦ʯ
	{
		if (nType==ITEMTYPE_SHOULDER	//����
			|| nType==ITEMTYPE_ARMOR	//����
			|| nType==ITEMTYPE_HELMET	//ͷ��
			|| nType==ITEMTYPE_GLOVE	//����
			|| nType==ITEMTYPE_CAESTUS	//����
			|| nType==ITEMTYPE_SHOES)	//Ь��
			return true;
	}
	return false;
}

int CItem::GetEudemonNeedExp(int nlevel)
{
	CLevupexpData* pLevupexp = LevupexpSet()->GetObj(EXP_TYPE_EUDEMON * _EXP_TYPE + nlevel );
	if ( !pLevupexp )
	{
		return 0;
	}
	return pLevupexp->GetInt( LEVUPEXPDATA_EXP );//�õ���������Ҫ����
}


int	CItem::GetReqLev()//�������ȼ�,���������������ж�
{	
	int reqLev = GetInt(ITEMDATA_REQ_USERLEVEL);
	return reqLev;
}

int	CItem::GetReqJobLev()//�������ȼ�,���������������ж�
{	
	int reqLev = GetInt(ITEMDATA_REQ_JOBLEVEL);
	return reqLev;
}

//------------------------------------
DWORD CItem::GetWeaponLevExp(WORD lev)
{
	CHECKF(lev > 0 && lev <= 150);
	CLevupexpData* pLevupexp = LevupexpSet()->GetObj( EXP_TYPE_WEAPONSOUL * _EXP_TYPE + lev);
	if ( !pLevupexp )
	{
		return 0;
	}

	return pLevupexp->GetInt64(LEVUPEXPDATA_EXP);
}

//--------------------------------
//20070704����:������
BOOL CItem::CanRepair(DWORD itemType)
{
	int nItemSort = GetItemSort(itemType);//ǰһλ


	if(nItemSort == IETMSORT_FINERY1 || nItemSort == IETMSORT_FINERY2)
		return true;

	return false;
}


//	20070710 ��� �Ƿ�Ϊ������Ʒ
//goto �޸Ĺ�����Ʒ�ж�����
BOOL CItem::IsCostlyItem()
{
	if ((GetInt(ITEMDATA_MONOPOLY) & COSTLY_ITEM) != 0)
		return TRUE;
	else if (IsEquipment())
	{
		if (GetEquipAddLev()>4 || GetEquipStar()>4)//ǿ�����Ǽ�+5
			return TRUE;
		if (GetEquipSoulLev()>2)
			return TRUE;
		for(int i=EM_GEMPOS_BEGIN; i<=EM_GEMPOS_END; i++)//����3����ʯ
		{
			int nStatus = GetGemStatus(i);
			if(nStatus == 0 || nStatus==_NOT_HOLE || nStatus == _HOLE || CItem::GetItemSort(nStatus)!=4)
				continue;
			if (nStatus%100>2)
				return TRUE;
		}
	}
	return FALSE;
}

//	20070710	���	��Ǯʹ���Ƿ��¼
BOOL CItem::IsLogSaveMoney(int nMoney)
{
	if(nMoney >= LOG_SAVE_MONEY)
		return TRUE;

	return FALSE;
}
//goto ���δ˺���,������Ʒ�������ж�
// BOOL CItem::IsCostlyItem(int nItemTypeID)
// {
// 	int nTypeID = nItemTypeID;
// 	int	nFirst = nTypeID / 100000;	//	ǰһλ;
// 	int nFirstSecond = nTypeID / 10000;	//	ǰ��λ;
// 	int	nFirstToForth = nTypeID / 100;	//	ǰ4λ;
// 	int	nEnd = nTypeID - (nTypeID / 10) * 10;	//	��λ
// 	
// 	//	1��ͷ��Ʒ�У���β��3����Ʒ	����װ��
// 	if(nFirst == 1 && nEnd == 3)	
// 		return TRUE;
// 	
// 	//	4��ͷ��Ʒ�У���β��3����Ʒ	��������
// 	else if(nFirst == 4 && nEnd == 3)
// 		return TRUE;
// 	
// 	//	500001	ǿ��ʯ,500002	����ǿ��ʯ,511202	������Ҷ ����ֵ����Ҫ���۵Ĳ��ϣ�
// 	//	511301	�������֣���ֵ����Ҫ���۵Ĳ��ϣ�,511302	��Դʯ����ֵ����Ҫ���۵Ĳ��ϣ�
// 	//	704006	����,704007	��ƥ,704008	����,704000	ʱ��֮����Ƭ
// 	else if(nTypeID == 500001 || nTypeID == 500002 || nTypeID == 511202 || nTypeID == 511301 || 
// 		nTypeID == 511302 || nTypeID == 704006 || nTypeID == 704007 || nTypeID == 704008
// 		|| nTypeID == 704000)
// 		return TRUE;
// 
// 	//	5001��ͷ��������Ʒ	������ʯ
// 	else if(nFirstToForth == 5001)
// 		return TRUE;
// 
// 	//	����81-86��ͷ����Ʒ�У���β��5����Ʒ	5����ʯ
// 	else if(nFirstSecond >= 81 && nFirstSecond <= 86)
// 		return TRUE;
// 
// 	//	����9��ͷ��Ʒ	��ֵ����
// 	else if(nFirst == 9)
// 		return TRUE;
// 
// 	//20071121����:�߻���----------
// 	else if(nFirst == 4 && nEnd == 4)//��������
// 		return TRUE;
// 
// 	else if(nFirstToForth == 5114)//��ԭʯ
// 		return TRUE;
// 
// 	else if(nFirstSecond == 54)//������������Ĳ���
// 		return TRUE;
// 	//------------------------------
// 	else if(nFirstSecond == 21)	//  [1/5/2008 ���] ����
// 		return TRUE;
// 
// 	return FALSE;
// }

bool CItem::IsUserExclusive(unsigned int	uMonopoly)
{
	return (uMonopoly & USER_EXCLUSIVE_MASK ) != 0; 
}
//----------------------װ���Ͽ׵�����
int	CItem::GetEmbedCountLimit()
{
	int nHoleCount = 0;

	if (GetInt(ITEMDATA_GEM1)!=_NOT_HOLE && GetInt(ITEMDATA_GEM1)!=0)
		nHoleCount++;
	if (GetInt(ITEMDATA_GEM2)!=_NOT_HOLE && GetInt(ITEMDATA_GEM2)!=0)
		nHoleCount++;
	if (GetInt(ITEMDATA_GEM3)!=_NOT_HOLE && GetInt(ITEMDATA_GEM3)!=0)
		nHoleCount++;

	return nHoleCount;
}
//----------------------װ��������Ƕ��ʯ������
int	CItem::GetEmbedGemCount()
{
	int nGemCount = 0;
	
	for (int i=EM_GEMPOS_BEGIN; i<EM_GEMPOS_END; i++)
	{
		int nState = GetGemStatus(i);
		if (nState!=_NOT_HOLE && nState!=_HOLE && nState!=0)
			nGemCount++;
	}
	return nGemCount;
}
//----------------------
BOOL CItem::IsCanUpLevel(DWORD itemType)
{
	//[2010-07-19 goto]
	return  ( (GetItemSort(itemType) == IETMSORT_FINERY1 || GetItemSort(itemType) == IETMSORT_FINERY2)
			&& GetItemType(itemType)!=ITEMTYPE_MOUNT//���ﲻ��ǿ��
			&& GetItemType(itemType)!=ITEMTYPE_DRESS//ʱװ����ǿ��
			&& GetItemType(itemType)!=ITEMTYPE_TREASURE
			&& GetItemType(itemType)!=ITEMTYPE_WEAPONSOUL);//��겻��ǿ��
// 	if( 4 == itemType / 100000 && itemType % 10 != 9 ) 
// 		return true; 
// 	else
//		return false;
}
//[2010-07-19 goto]��ȡװ��ǿ���������Ҫ��ǿ����ʯ�ȼ�
int CItem::GetNeedGemLev(int nEquipLev)
{
	int nNeedLev = 0;
	switch(nEquipLev)
	{
	case 0:
	case 1:
	case 2:
		nNeedLev = 1;
		break;
	case 3:
	case 4:
	case 5:
		nNeedLev = 2;
		break;
	case 6:
	case 7:
	case 8:
	case 9:
		nNeedLev = 3;
		break;
// 	case 9://�߻�Ҫ��ȥ��4���������3��
// 		nNeedLev = 4;
// 		break;
	default:
		break;
	}
	return nNeedLev;
}
//[2010-08-24 goto]��ȡװ��ǿ������ǵ������ĳɹ���
float CItem::GetSucRate(short nType)
{
	int nLev = 0;
	float fSucRate = 0.0f;
	if (nType==1)//ǿ��
		nLev = GetEquipAddLev();
	else if (nType==2)
		nLev = GetEquipStar();
	switch(nLev)
	{
	case 0:fSucRate=0.25f;break;
	case 1:fSucRate=0.21f;break;
	case 2:fSucRate=0.1725f;break;
	case 3:fSucRate=0.1375f;break;
	case 4:fSucRate=0.1f;break;
	case 5:fSucRate=0.075f;break;
	case 6:fSucRate=0.05f;break;
	case 7:fSucRate=0.025f;break;
	case 8:fSucRate=0.005f;break;
	case 9:fSucRate=0.0025f;break;
	default:
		break;
	}
	return fSucRate*100;
}
//[2010-07-19 goto]��ȡװ��ǿ���������Ҫ������
int CItem::GetEquipAddLevCost(int nType)
{
	int nCost = 0;
	int nSwitchLev = 0;
	if (nType==1)
	{
		nSwitchLev = GetEquipAddLev();
		switch(nSwitchLev)
		{
		case 0:
		case 1:
		case 2:
			nCost=80;
			break;
		case 3:
		case 4:
		case 5:
			nCost=150;
			break;
		case 6:
		case 7:
		case 8:
		case 9:
			nCost=250;
			break;
		default:
			break;
		}
	}
	else if (nType==2)
	{
		nSwitchLev = GetEquipStar();
		switch(nSwitchLev)
		{
		case 0:
		case 1:
		case 2:
			nCost=50;
			break;
		case 3:
		case 4:
		case 5:
			nCost=100;
			break;
		case 6:
		case 7:
		case 8:
		case 9:
			nCost=150;
			break;
		default:
			break;
		}
	}
	return nCost;
}

//-----------------------------------------------
BOOL CItem::IsValidEquip(CUser* pUser)
{
	if(IsActivateEquip() || IsActivatePickup())
	{
		if(GetInt(ITEMDATA_NEEDCHECK) == 2)
			return FALSE;
	}
	if(IsEquipEnable())
	{
		if(IsWeaponSoul())
		{
			CWeaponSoulData* pData = QueryWeapensoulData();
			if(pData)
			{
				if(pData->GetInt(WEAPONSOULDATA_LIFESPAN) >= 5)
					return true;
			}
			return false;
		}

		//�־�
		if(GetItemAmount()/100==0) 
			return false;

		if(pUser)
		{
			//����ȼ�
			if(GetReqLev() > pUser->GetLev())
				return false;
			//ְҵ�ȼ�
			if(GetReqJobLev() > pUser->GetJobLev())
				return false;
			
// 			if (GetInt(ITEMDATA_REQ_SEX) != pUser->GetSex())
// 				return false;
			
			if (GetInt(ITEMDATA_REQ_PROF) && ((GetInt(ITEMDATA_REQ_PROF) & (pUser->GetProfession())) == 0))
				return false;			
		}

		return true;
	}
	else 
		return false;
}


//////////////////////////////////////////////////////////////////////////
//  [2007-9-14] ���
bool CItem::IsActivatePickup(unsigned int uMonopoly)	//  [2007-9-14] ���	��ü���
{
	return (uMonopoly & ACTIVATE_PICKUP ) != 0;
}

//  [2007-9-14] ���	װ������
bool CItem::IsActivateEquip(unsigned int uMonopoly)
{
	return (uMonopoly & ACTIVATE_EQUIP ) != 0;
}

//  [2007-9-14] ���	��������
bool CItem::IsLifeTimeContinue(unsigned int uMonopoly)
{
	return (uMonopoly & LIFETIME_CONTINUE ) != 0;
}

//  [2007-9-14] ���	��������
bool CItem::IsLifeTimeNonContinue(unsigned int uMonopoly)
{
	return (uMonopoly & LIFETIME_NON_CONTINUE ) != 0;
}

//  [2007-9-15] ��� ���ϴμ��ʱ��,���˵Ϊ����δ����
const char* CItem::GetLastCheckTime()
{
	return GetStr(ITEMDATA_LAST_CHECK_TIME);
}

//  [2007-9-15] ��� ��ʣ����������,��λ��
unsigned int CItem::GetLifeTime()
{
	return GetInt(ITEMDATA_LIFE_TIME);
}


//-----------------------------------
//ͨ���ڴ洴��.�������ݿ����
bool CItem::Create(IDatabase* pDb, 
		IRecord* pDefault, 
		const ItemInfoStruct* pInfo, 
		IRecord* pDefaultEudemonData, 
		IRecord* pDefaultWpSl,//��� 
		bool bInsert/*=true*/, 
		OBJID idNew/*=ID_NONE*/)
{
	CHECKF(CItemData::Create(pDb, 
		pDefault, 
		pInfo, 
		pDefaultEudemonData, 
		bInsert, 
		idNew));

	if(IsWeaponSoul())
	{
		ST_WEAPONSOULDATA data;
		CWeaponSoulData::GetInfoByDefault(data);
		data.idItem = GetID();
		CreateWpSlData(&data,pDefaultWpSl,pDb,bInsert);
	}

	return true;
}

bool CItem::CreateSend(IDatabase* pDb, 
				   IRecord* pDefault, 
				   const ItemInfoStructSend* pInfo, 
				   IRecord* pDefaultWpSl,//��� 
				   bool bInsert/*=true*/, 
				   OBJID idNew/*=ID_NONE*/)
{
	CHECKF(CItemData::CreateSend(pDb, 
		pDefault, 
		pInfo, 
		NULL, 
		bInsert, 
		idNew));
	
	if(IsWeaponSoul())
	{
		CreateWpSlData(&(pInfo->stWeapenSoul),pDefaultWpSl,pDb,bInsert);
	}
	SetEquipSoulPoint(pInfo->nEquipSoulPoint);
	SetEquipSoulLev(pInfo->usEquipSoulLev);
	SetEquipSoulType(pInfo->usEquipSoulType);

	return true;
}

bool CItem::CreateSoul(IDatabase* pDb,
				   IRecord* pDefault,
				   const ItemInfoStruct* pInfo,
				   IRecord* pDefaultWpSl,
				   ST_WEAPONSOULDATA stData,
				   bool bInsert,
				   OBJID idNew)
{
	CHECKF(CItemData::Create(pDb, 
		pDefault, 
		pInfo, 
		NULL, 
		bInsert, 
		idNew));

	if(!IsWeaponSoul())
	{
		DeleteRecord(pDb);
		CHECKF(!"if(!IsWeaponSoul())");
	}

	stData.idItem = GetID();
	if(!CreateWpSlData(&stData,pDefaultWpSl,pDb,bInsert))
	{
		DeleteRecord(pDb);
		CHECKF(!"CreateWpSlData(&stData,pDefaultWpSl,pDb,bInsert)");
	}
	return true;
}

//ͨ�����ݿⴴ��
bool CItem::Create(OBJID idItem, IDatabase* pDb)
{
	//����CItemData��ͬ������
	CHECKF(CItemData::Create(idItem,pDb));
	CreateWpSlData(pDb);
	return true;
}

bool CItem::Create(IRecordset* pRes, IDatabase* pDb)
{
	//����CItemData��ͬ������
	CHECKF(CItemData::Create(pRes,pDb));
	CHECKF(CreateWpSlData(pDb));
	return true;
}

//-------------------------------
BOOL CItem::CreateWpSlData(IDatabase* pDb)//�������
{
	CHECKF(pDb);

	if( !IsWeaponSoul())//��������������.������
		return true;

	OBJID idSoul = GetInt(ITEMDATA_BASE1);
	if(idSoul == ID_NONE)
	{
		LOGWARNING("CItem::CreateWpSlData(IDatabase* pDb) ITEMID = %u",GetID());
		return false;
	}

	//�����ԭ�е��������
	DeletWpSlData();

	m_pWpSlData = new CWeaponSoulData;
	CHECKF(m_pWpSlData);
	IF_NOT(m_pWpSlData->Create(idSoul,pDb))
	{
		//����ʧ��
		SAFE_RELEASE(m_pWpSlData);
	}

	return true;
}

//-------------------------------
BOOL CItem::CreateWpSlData(const ST_WEAPONSOULDATA* pstWpSlData,IRecord* pDefaultWpSl,IDatabase* pDb,bool bInsert /*= true*/)
{
	CHECKF(pstWpSlData);
	CHECKF(pDefaultWpSl);
	CHECKF(pDb);
	CHECKF(IsWeaponSoul());//��������������.������		

	//�������ǰ��
	DeletWpSlData();

	m_pWpSlData = new CWeaponSoulData;
	CHECKF(m_pWpSlData);
	IF_NOT(m_pWpSlData->Create(pDb,
			pDefaultWpSl,
			pstWpSlData,
			bInsert,
			ID_NONE))
		{
			//����ʧ��
			SAFE_RELEASE(m_pWpSlData);
			return false;
		}

	this->SetInt(ITEMDATA_BASE1,m_pWpSlData->GetID(),true);//���ID
	return true;
}

//------------------------------------
//��װһ��.�ڵ���CItemDataͬ������
bool CItem::GetInfo	(ItemInfoStructSend* pInfo)
{
	CItemData::GetInfo(pInfo); 
	if(IsWeaponSoul())
	{
		m_pWpSlData->GetInfo(pInfo->stWeapenSoul,false);
	}
	pInfo->usEquipSoulLev=GetEquipSoulLev();
	pInfo->usEquipSoulType=GetEquipSoulType();
	pInfo->nEquipSoulPoint=GetEquipSoulPoint();
	return true;
}


bool CItem::GetInfo	(ItemInfoStruct* pInfo)
{
	CItemData::GetInfo(pInfo); 
	pInfo->usEquipSoulLev=GetEquipSoulLev();
	pInfo->usEquipSoulType=GetEquipSoulType();
	pInfo->nEquipSoulPoint=GetEquipSoulPoint();
	return true;
}


//------------------------------------
void CItem::DeletWpSlData()//��������������.����ʱ����
{
	SAFE_RELEASE(m_pWpSlData);
}

void CItem::EraseWpSl(IDatabase* pDb)
{
	if(IsWeaponSoul() && m_pWpSlData)
	{
		m_pWpSlData->DeleteRecord(pDb);
		DeletWpSlData();
	}
}

BOOL CItem::GetWpSlInfo(ST_WEAPONSOULDATA& stWpSl)
{
	if(IsWeaponSoul() && m_pWpSlData)
	{
		return m_pWpSlData->GetInfo(stWpSl);
	}
	return false;
}

DWORD CItem::GetWpSlLev()
{
	if(IsWeaponSoul() && m_pWpSlData)
	{
		return m_pWpSlData->GetInt(WEAPONSOULDATA_MIXLEV);
	}
	return 0;
}

DWORD CItem::GetWpSlType()
{
	if(IsWeaponSoul() && m_pWpSlData)
	{
		return m_pWpSlData->GetInt(WEAPONSOULDATA_TYPE) / 10;
	}
	return 0;
}


void CItem::OnSaveInfo()
{
	if(IsWeaponSoul() && m_pWpSlData)
	{
		m_pWpSlData->SaveInfo();
	}
}

bool CItem::IsChanceDropEquipWhenDead(unsigned int uMonopoly)
{
	return (uMonopoly & CHANCE_DROP_EQUIP_WHEN_DEAD ) != 0;
}

bool CItem::IsUnkidnap(unsigned int uMonopoly)
{
	return (uMonopoly & UNKIDNAP) != 0;
}

int	CItem::GetWeaponSpecialView()
{
	if (GetItemType() != ITEMTYPE_WEAPONR)
		return 0;

	int nViewLev = 0;
	if (GetEquipAddLev()>6)
		nViewLev += 2;
// 	if (GetEquipStar()>6)
// 		nViewLev++;
	for(int i=EM_GEMPOS_BEGIN; i<=EM_GEMPOS_END; i++)
	{
		int nStatus = GetGemStatus(i);
		if(nStatus == 0 || nStatus==_NOT_HOLE || nStatus == _HOLE || CItem::GetItemSort(nStatus)!=4)
			continue;
		if (nStatus%100 > 3)
		{
			nViewLev++;
			break;
		}
	}
	return nViewLev;
}

bool CItem::IsNewRemedy()
{
	return CItem::IsNewRemedyByItemType(GetTypeID());
}

bool CItem::IsNewRemedyByItemType( int typeID )
{
	if((301 == typeID/1000)
		|| (311 == typeID/1000)
		|| (321 == typeID/1000)
		|| (302 == typeID/1000)
		|| (312 == typeID/1000)
		|| (322 == typeID/1000))
		return true;
	else
		return false;
}

int	CItem::GetItemAdditionData(int nIndex)//�����Ʒĳ������Ե�ֵ,������1��8
{
	nIndex = nIndex + ITEMDATA_BASIC_ADDITION1 - 1;
	if (nIndex>=ITEMDATA_BASIC_ADDITION1 && nIndex<=ITEMDATA_BASIC_ADDITION8)
	{
		return GetInt((ITEMDATA)nIndex);
	}
	return 0;
}

bool CItem::CanCutWork()//��ʯ�ܷ����
{
	if(GetTypeID()/100 < 4105 || GetTypeID()/100 > 4108)//ֻ�к�ˮ������ˮ������ˮ������ˮ�����Ե���
		return false;
	int nLev = GetTypeID()%100;
	if(nLev<=10)
		return true;

	return false;
}
//�ǲ��ǳ��ﵰ
bool CItem::IsPetEgg()
{
	OBJID typeID = GetTypeID();
	if(typeID > 505000 && typeID < 506000)
		return true;

	return false;
}
//�������
short CItem::GetEquipSoulLev()//�õ�����ȼ�
{
	int nSoulState=GetInt(ITEMDATA_SOULSTATE);
	if (nSoulState == 0 || nSoulState >= 1000000000)//����10λΪ����
		return 0;
	short nState=nSoulState/10000000;
	short nLev=(nSoulState-nState*10000000)/100000;
	return max(0,nLev);
}
short CItem::GetEquipSoulType()//�õ���������
{
	int nSoulState=GetInt(ITEMDATA_SOULSTATE);
	if (nSoulState == 0|| nSoulState >= 1000000000)//����10λΪ����
		return 0;
	short nState=nSoulState/10000000;
	return max(0,nState);
}
DWORD CItem::GetEquipSoulPoint()//�õ�ħ����
{
	int nSoulState=GetInt(ITEMDATA_SOULSTATE);
	if (nSoulState == 0|| nSoulState >= 1000000000)//����10λΪ����
		return 0;
	DWORD nPoint=nSoulState%100000;
	return max(0,nPoint);
}
void CItem::SetEquipSoulLev(short nLev)//�õ�����ȼ�
{
	if (nLev>99 || nLev <0)
		return;
	int nSoulState=GetInt(ITEMDATA_SOULSTATE);
	short noldState=nSoulState/10000000;
	short noldLev=(nSoulState-noldState*10000000)/100000;
	int   noldPoint=nSoulState%100000;

	SetInt(ITEMDATA_SOULSTATE,noldState*10000000+nLev*100000+noldPoint);

}
void CItem::SetEquipSoulType(short nType)//�õ���������
{
	if (nType>99 || nType <0)
		return;
	int nSoulState=GetInt(ITEMDATA_SOULSTATE);
	short noldState=nSoulState/10000000;
	short noldLev=(nSoulState-noldState*10000000)/100000;
	int   noldPoint=nSoulState%100000;
	
	SetInt(ITEMDATA_SOULSTATE,nType*10000000+noldLev*100000+noldPoint);
}
void CItem::SetEquipSoulPoint(OBJID nPoint)//�õ�ħ����
{
	if (nPoint>99999 || nPoint <0)
		return;
	int nSoulState=GetInt(ITEMDATA_SOULSTATE);
	short noldState=nSoulState/10000000;
	short noldLev=(nSoulState-noldState*10000000)/100000;
	int   noldPoint=nSoulState%100000;
	
	SetInt(ITEMDATA_SOULSTATE,noldState*10000000+noldLev*100000+nPoint);
}
short CItem::GetSoulBookType()
{
	if (!IsSoulBook()) 
		return 0;
	else
	{
		int nType=(GetInt(ITEMDATA_TYPE)-(ITEMTYPE_SOULBOOK*10000))/100;
		if (nType<=0)
			return 0;
		return nType;
	}
}

bool CItem::IsActSoulBook()
{
	if (!IsSoulBook())
		return false;
	if(GetSoulBookType()==EM_SOULTYPE_BING || GetSoulBookType()==EM_SOULTYPE_HUO || 
	   GetSoulBookType()==EM_SOULTYPE_XUAN || GetSoulBookType()==EM_SOULTYPE_DU)
		return true;
	return false;
}
bool CItem::IsDefSoulBook()
{
	if (!IsSoulBook())
		return false;
	if(GetSoulBookType()==EM_SOULTYPE_LI || GetSoulBookType()==EM_SOULTYPE_LING || GetSoulBookType()==EM_SOULTYPE_DING || 
	   GetSoulBookType()==EM_SOULTYPE_SHEN || GetSoulBookType() == EM_SOULTYPE_TI)
		return true;
	return false;
}

