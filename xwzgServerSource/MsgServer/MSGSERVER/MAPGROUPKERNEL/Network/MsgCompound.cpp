// MsgCompound.cpp: implementation of the CMsgCompound class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "MsgCompound.h"
#include "FecthData.h"
#include "mapgroup.h"
#include "MsgItem.h"
#include <set>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgCompound::CMsgCompound()
{
	Init();
	m_pInfo	=( MSG_Info * )m_bufMsg;
}

CMsgCompound::~CMsgCompound()
{

}
bool CMsgCompound::CheckSameItemID(int* array,int ArraySize)
{
	for (int i = 0; i < ArraySize;i++)
	{
		int nCheckNum = array[i];
		for (int j = 0; j<ArraySize;j++)
		{
			if (j == i
				|| !array[i]
				|| !array[j])
			{
				continue;
			}
			if (nCheckNum == array[j])
			{
				return true;
			}
		}
	}
	return false;
}
BOOL CMsgCompound::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_COMPOUND != this->GetType())
		return false;
										
	return true;
}

BOOL CMsgCompound::Create(int nAction,OBJID idItem, int data)
{
	// param check
	m_pInfo->nItemTypeID = idItem;
	m_pInfo->nAction = nAction;
	m_pInfo->nMaterialSize=data;
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_COMPOUND;//������2007/02/16��Ȼû��дsize��type�ĸ�ֵ����а��
	
	return true;
}

BOOL CMsgCompound::AppendPeifang(OBJID idPeifang)
{
	m_pInfo->Material[m_pInfo->nMaterialSize].nItemID = idPeifang;
	m_pInfo->Material[m_pInfo->nMaterialSize].nAmount = 0;
	m_pInfo->nMaterialSize ++;
	if(m_pInfo->nMaterialSize > 1)
		m_unMsgSize	+=sizeof(MATERIAL);
	CHECKF(m_unMsgSize < 1024);
	return true;
}

void CMsgCompound::Process(void* pInfo)
{
	DEBUG_TRY	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
	CUserPtr pUser = UserManager()->GetUser(this);
	if(!pUser)
		return;

	if(!pUser->IsAlive())
	{
		return ;
	}
	
	switch(m_pInfo->nAction)
	{
	case MSGCOMPOUND_EQUIPCREATE://װ������
		{
			int materialSize = m_pInfo->nMaterialSize;
			
			pUser->EquipCreateNew(m_pInfo->nItemTypeID,m_pInfo->nMaterialSize);

			CMsgCompound msg;
			msg.Create(MSGCOMPOUND_EQUIPCREATE_SUC, m_pInfo->nItemTypeID);
			this->SendMsg(&msg);
		}
		break;
	//[2010-07-19 goto] �û�����Ҫǿ����װ������ʱ���ͻ��˷���װ����ʧ�ܼӳ�
	case MSGCOMPOUND_PUT_EQUIP:
		{
			CItem* pEquip = pUser->GetItem(m_pInfo->nItemTypeID,false);
			if(!pEquip)
				pEquip = pUser->GetEquipItem(m_pInfo->nItemTypeID);

			if (!pEquip || !pEquip->IsCanUpLevel())
				return;

			if (pEquip->GetTypeID()==144000)//տ¬��
				return;
			if (pEquip->IsWeaponSoul())
				return;

			int nFailRate = 0;
			if (m_pInfo->nMaterialSize==1)//ǿ��
				nFailRate = pEquip->GetEquipAddLevFailRate()*10+1;
			else if (m_pInfo->nMaterialSize==2)//����
				nFailRate = pEquip->GetEquipStarFailRate()*10+2;
			else
				return;

			CMsgCompound msg;//����װ����ʧ�ܼӳ�
			IF_OK( msg.Create( MSGCOMPOUND_EQUIP_FAILRATE, m_pInfo->nItemTypeID, nFailRate ) )//�ӳ��ֶεĸ�λ��ʾ����(1,ǿ��;2,����)������10�Ժ�ó��Ĳ��Ǽӳ���
			pUser->SendMsg(&msg);
		}
		break;
	//[[2010-07-19 goto]]װ��ǿ��
	case MSGCOMPOUND_EQUIP_LEVUP:
		{
// 			if (m_pInfo->nMaterialSize==0 || m_pInfo->nMaterialSize>4)
// 				return;
// 			OBJID data[4] = { 0 };//����ĸ�ǿ����ʯ
// 			for( int i=0; i<m_pInfo->nMaterialSize && i<4; i++ )
// 			{
// 				data[i] = m_pInfo->Material[i].nItemID;
// 			}
			int nGemCount = m_pInfo->Material[0].nItemID;//�Զ��ŷ��ĸ���
			bool bUseLuckyCharm = m_pInfo->Material[0].nAmount > 0 ? true : false;//�Ƿ��Զ����
			if (nGemCount < 1 || nGemCount > 4)
				return;
			pUser->WeaponLevUp( m_pInfo->nItemTypeID,nGemCount,bUseLuckyCharm );//ͬʱ����Ϣ

			CMsgCompound msg;//�ͻ�����Ҫ����,���ܳɹ�����ʧ��
			IF_OK( msg.Create( MSGCOMPOUND_EQUIP_LEVUP, m_pInfo->nItemTypeID, 0 ) )
				pUser->SendMsg(&msg);
		}
		break;
	case MSGCOMPOUND_EQUIP_ADDSTAR://װ������
		{
// 			if (m_pInfo->nMaterialSize==0 || m_pInfo->nMaterialSize>4)
// 				return;
// 			OBJID data[4] = { 0 };//����ĸ����Ǳ�ʯ
// 			for( int i=0; i<m_pInfo->nMaterialSize && i<4; i++ )
// 			{
// 				data[i] = m_pInfo->Material[i].nItemID;
// 			}
			int nGemCount = m_pInfo->Material[0].nItemID;//�Զ��ŷ��ĸ���
			bool bUseLuckyCharm = m_pInfo->Material[0].nAmount > 0 ? true : false;//�Ƿ��Զ����
			if (nGemCount < 1 || nGemCount > 4)
				return;
			pUser->EquipAddStar( m_pInfo->nItemTypeID,nGemCount,bUseLuckyCharm );//ͬʱ����Ϣ

			CMsgCompound msg;//�ͻ�����Ҫ����,���ܳɹ�����ʧ��
			IF_OK( msg.Create( MSGCOMPOUND_EQUIP_ADDSTAR, m_pInfo->nItemTypeID, 0 ) )
				pUser->SendMsg(&msg);
		}
		break;
	case MSGCOMPOUND_MAKEHOLE://װ�����
		{
			if(m_pInfo->nItemTypeID == ID_NONE || m_pInfo->nMaterialSize == ID_NONE)
				return;

			CItem* pEquipItem = pUser->GetItem( m_pInfo->nItemTypeID ,true);
			if(!pEquipItem)
				pEquipItem = pUser->GetEquipItem(m_pInfo->nItemTypeID);
			if(!pEquipItem)
				return;

			if(!pEquipItem->IsWeaponSoul())
			{
				if(!pEquipItem->IsCanUpLevel() || pEquipItem->IsLocked())
					return;
			}

			int nHoleCount = pEquipItem->GetEmbedCountLimit();
			BOOL bRet = FALSE;

			if(nHoleCount==3)//�Ѿ���3�����ˣ����ĸ������
			{
				if(pEquipItem->GetChaosHoleState() != 0)//���ĸ���Ҳ����
					return;

				bRet = pUser->EquipMakeChaosHole(m_pInfo->nItemTypeID, m_pInfo->nMaterialSize);
			}
			else
				bRet = pUser->EquipMakeHole( m_pInfo->nItemTypeID, m_pInfo->nMaterialSize);

			if(bRet)
			{
				// ���������ɹ���Ϣ
				CMsgCompound msg;
				IF_OK( msg.Create( MSGCOMPOUND_MAKEHOLE_SUC,m_pInfo->nItemTypeID,bRet ) )
					pUser->SendMsg( &msg );
// 				pUser->SetCompLastDickQqu(m_pInfo->nItemTypeID); //��¼���ڽ�������action
// 				pUser->ProcessAction(COMP_DICK);
			}
			else
			{
				CMsgCompound msg;
				IF_OK( msg.Create( MSGCOMPOUND_MAKEHOLE_FAL,m_pInfo->nItemTypeID,0 ) )
					pUser->SendMsg( &msg );
			}
		}
		break;
	case MSGCOMPOUND_EMBEDEQUIP://װ����Ƕ
		{
			if(m_pInfo->nItemTypeID == ID_NONE)
				return;

			OBJID idStone = m_pInfo->Material[ 0 ].nItemID;
			if (idStone==0)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "ȱ����Ƕ����");
				return;
			}

			CItem* pEquip = pUser->GetItem( m_pInfo->nItemTypeID ,true);
			if(!pEquip)
				pEquip = pUser->GetEquipItem(m_pInfo->nItemTypeID);
			if(!pEquip)
				return;

			if(!pEquip->IsCanEmbedEquip() || pEquip->IsLocked())
				return;

			if(pEquip->GetInt(ITEMDATA_GEM4)==_HOLE && pEquip->GetEmbedPos()==0)//������ͨ�׶����ˣ�ֻʣ�����
			{
				if (!pUser->EmbedGemInChaos( m_pInfo->nItemTypeID, idStone, m_pInfo->nMaterialSize))
				{
					CMsgCompound msg;
					IF_OK( msg.Create( MSGCOMPOUND_EMBED_FAL,m_pInfo->nItemTypeID,0 ) )
						pUser->SendMsg( &msg );
				}
			}
			else
			{
				if (!pUser->EmbedGem( m_pInfo->nItemTypeID, idStone, m_pInfo->nMaterialSize))
				{
					CMsgCompound msg;
					IF_OK( msg.Create( MSGCOMPOUND_EMBED_FAL,m_pInfo->nItemTypeID,0 ) )
						pUser->SendMsg( &msg );
				}
			}
		}
		break;
	case MSGCOMPOUND_EMBED_TAKEOUTGEM://װ��ȡ����ʯ
		{
			OBJID idStone = m_pInfo->Material[ 0 ].nItemID;
			if (idStone==0)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "ȱ��ȡ������");
				return;
			}
			if (m_pInfo->nMaterialSize<1 || m_pInfo->nMaterialSize>4)
			{
				return;
			}
			if (!pUser->TakeOutGem(m_pInfo->nItemTypeID, idStone, m_pInfo->nMaterialSize))
			{
				CMsgCompound msg;//����ʧ����Ϣ
				IF_OK( msg.Create( MSGCOMPOUND_EMBED_TAKEOUTGEM_FAIL, m_pInfo->nItemTypeID, 0 ) )
					pUser->SendMsg(&msg);
			}
		}
		break;
	case MSGCOMPOUND_GEM_LEVUP://[2010-08-04 goto] ��ʯ�ϳɣ�3-5���ͼ���ʯ�ϳ�һ���߼���ʯ
		{
			if (m_pInfo->nMaterialSize>5)
				return;

			if (m_pInfo->nMaterialSize<3)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "������Ҫ������ʯ���ܺϳ�");
				return;
			}
			set<OBJID> setGem;
			for( int i=0; i<m_pInfo->nMaterialSize && i<5; i++ )
			{
				if (setGem.count(m_pInfo->Material[i].nItemID))
					continue;
				setGem.insert(m_pInfo->Material[i].nItemID);
			}
			if (!pUser->GemLevUp(m_pInfo->nItemTypeID,setGem,m_pInfo->nMaterialSize))
			{
				CMsgCompound msg;//����ʧ����Ϣ
				IF_OK( msg.Create( MSGCOMPOUND_GEM_LEVUP_FAIL, 0, 0 ) )
					pUser->SendMsg(&msg);
			}
		}
		break;
	case MSGCOMPOUND_EQUIP_SOULMARK://goto ��ӡ(����İ�)
		{
			if (!pUser->EquipSoulMark(m_pInfo->nItemTypeID))
			{
				CMsgCompound msg;//����ʧ����Ϣ
				IF_OK( msg.Create( MSGCOMPOUND_EQUIP_SOULMARK_FAIL, m_pInfo->nItemTypeID, 0 ) )
					pUser->SendMsg(&msg);
			}
		}
		break;
	case MSGCOMPOUND_GEM_CUTWORK://��ʯ����
		{
			if(!m_pInfo->nItemTypeID)
				return;
			if(!m_pInfo->Material[0].nItemID)
				return;

			OBJID idNewGem = pUser->GemCutwork(m_pInfo->nItemTypeID, m_pInfo->Material[0].nItemID);
			if(idNewGem)
			{
				CMsgCompound msg;
				IF_OK( msg.Create( MSGCOMPOUND_GEM_CUTWORK_SUC, idNewGem, 0 ) )
					pUser->SendMsg(&msg);
			}
			else
			{
				CMsgCompound msg;
				IF_OK( msg.Create( MSGCOMPOUND_GEM_CUTWORK_FAIL, 0, 0 ) )
					pUser->SendMsg(&msg);
			}
		}
		break;
	case MSGCOMPOUND_EQUIP_SOULADD://����
		{
			if(!m_pInfo->nItemTypeID)
				return;
			if(!m_pInfo->nMaterialSize)
				return;
			CItem* pEquip = pUser->GetItem(m_pInfo->nItemTypeID,false);
			if (!pEquip || !pEquip->IsCanUpLevel())
				return;
			if (pEquip->GetTypeID()==144000)//տ¬��
				return;
			if (pEquip->IsWeaponSoul())
				return;
			
			bool bUseBook=false;
			CItem* pBook =pUser->GetItem(m_pInfo->nMaterialSize);
			if(pBook && pBook->IsSoulBook())
				bUseBook=true;
			if(pEquip->GetEquipSoulLev()<=0)
				bUseBook=true;
			CMsgCompound msg;//������Ϣ
			if(!pUser->EquipSoulAdd(m_pInfo->nItemTypeID,m_pInfo->nMaterialSize,bUseBook))//ͬʱ����Ϣ
			{
				if(!msg.Create( MSGCOMPOUND_EQUIP_SOULADD_FAIL, pEquip->GetEquipSoulPoint(), pEquip->GetEquipSoulLev()))
					return;
			}		
			else
			{
				if(!msg.Create( MSGCOMPOUND_EQUIP_SOULADD_SUC, pEquip->GetEquipSoulPoint(), pEquip->GetEquipSoulLev()))
					return;
			}
			pUser->SendMsg(&msg);
		}
		break;
	case MSGCOMPOUND_EQUIP_SOULEXCHANGE://����ת��
		{
			if(!m_pInfo->nItemTypeID)
				return;
			if(!m_pInfo->nMaterialSize)
			return;
			CItem* pEquip = pUser->GetItem(m_pInfo->nItemTypeID,false);
			CItem* pEquip2= pUser->GetItem(m_pInfo->nMaterialSize,false);
			if (!pEquip || !pEquip->IsCanUpLevel() || !pEquip2 || !pEquip2->IsCanUpLevel())
				return;
			if (pEquip->GetTypeID()==144000 || pEquip2->GetTypeID()==144000)//տ¬��
				return;
			if (pEquip->IsWeaponSoul()||pEquip2->IsWeaponSoul())
				return;
			CMsgCompound msg;//������Ϣ
			if(!pUser->EquipSoulExchange(m_pInfo->nItemTypeID,m_pInfo->nMaterialSize))//ͬʱ����Ϣ
			{
				if(!msg.Create( MSGCOMPOUND_EQUIP_SOULEXCHANGE_FAIL, m_pInfo->nItemTypeID, m_pInfo->nMaterialSize))
					return;
			}
			else
			{
				if(!msg.Create( MSGCOMPOUND_EQUIP_SOULEXCHANGE_SUC, m_pInfo->nItemTypeID,m_pInfo->nMaterialSize))
					return;
			}
				pUser->SendMsg(&msg);
		}
		break;
//modify code by �ֵ�Ԩ begin 2011.7.13
	case MSGCOMPOUND_ARTIFIC://װ������
		{
			if(!m_pInfo->nItemTypeID)
				return;
			
			if(pUser->EquipArtific(m_pInfo->nItemTypeID))
			{
				CMsgCompound msg;
				IF_OK( msg.Create( MSGCOMPOUND_ARTIFIC_SUC, m_pInfo->nItemTypeID, 0 ) )
					pUser->SendMsg(&msg);
			}
			else
			{
				CMsgCompound msg;
				IF_OK( msg.Create( MSGCOMPOUND_ARTIFIC_FAIL, 0, 0 ) )
					pUser->SendMsg(&msg);
			}
		}
		break;
	case MSGCOMPOUND_EQUIP_MOVE://ǿ��ת��
		{
			if(!m_pInfo->nItemTypeID)
				return;
			if(!m_pInfo->Material[0].nItemID)
				return;
			int firstaddlv = 0;
			int secondaddlv = 0;
			if(pUser->EquipMove(m_pInfo->nItemTypeID, m_pInfo->Material[0].nItemID,firstaddlv,secondaddlv))
			{
				CMsgCompound msg;
				IF_OK( msg.Create( MSGCOMPOUND_EQUIP_MOVE_SUC, m_pInfo->nItemTypeID, firstaddlv ) )
					pUser->SendMsg(&msg);
				
				CMsgCompound msg1;
				IF_OK( msg1.Create( MSGCOMPOUND_EQUIP_MOVE_SUC, m_pInfo->Material[0].nItemID, secondaddlv ) )
					pUser->SendMsg(&msg1);
			}
			else
			{
				CMsgCompound msg;
				IF_OK( msg.Create( MSGCOMPOUND_EQUIP_MOVE_FAIL, 0, 0 ) )
					pUser->SendMsg(&msg);
			}
		}
		break;
//modify code by �ֵ�Ԩ end
	default:
		{
			//warning
		}
		break;
	}
	DEBUG_CATCH2("switch(MSGCOMPOUND) [%d]", m_pInfo->nAction)		// AAAAAAAAAAAAAAAA
	return;
}