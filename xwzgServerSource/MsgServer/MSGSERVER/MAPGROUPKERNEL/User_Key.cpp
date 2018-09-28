#include "..\stdafx.h"
#include "Network/allmsg.h"
#include "User.h"
#include "MapGroup.h"
#include "Network/netmsg.h"
#include "ItemType.h"
#include "MapItem.h"
// 
// bool  CUser::IniUserKey()
// {
// 	IF_NOT(m_pUserKey.Create(Database(),this))
// 		return false;
// 
//     if(!m_pUserKey.SendKey(this)) //送快捷键给客户端
// 		return false;
// 
// 	return true;
// }

bool   CUser::SetUserKey(int nData,bool bUpdate/* = false*/)
{
	return false;
	UserKey keyInfo;
	if (keyInfo.key1 == 95070)
	{
		keyInfo.key1 = nData;
		this->m_pUserKey.SetInt( SYSKEY_KEY1,keyInfo.key1);
		this->IniUserKey();
		return true;
	}
	else if (keyInfo.key2 == 95070)
	{
		keyInfo.key2 = nData;
		this->m_pUserKey.SetInt( SYSKEY_KEY2,keyInfo.key2);
		this->IniUserKey();
		return true;
	}
	else if (keyInfo.key3 == 95070)
	{
		keyInfo.key3 = nData;
		this->m_pUserKey.SetInt( SYSKEY_KEY3,keyInfo.key3);
		this->IniUserKey();
		return true;
	}
	else if (keyInfo.key4 == 95070)
	{
		keyInfo.key4 = nData;
		this->m_pUserKey.SetInt( SYSKEY_KEY4,keyInfo.key4);
		this->IniUserKey();
		return true;
	}
	else if (keyInfo.key5 == 95070)
	{
		keyInfo.key5 = nData;
		this->m_pUserKey.SetInt( SYSKEY_KEY5,keyInfo.key5);
		this->IniUserKey();
		return true;
	}
	else if (keyInfo.key6 == 95070)
	{
		keyInfo.key6 = nData;
		this->m_pUserKey.SetInt( SYSKEY_KEY6,keyInfo.key6);
		this->IniUserKey();
		return true;
	}
	else if (keyInfo.key7 == 95070)
	{
		keyInfo.key7 = nData;
		this->m_pUserKey.SetInt( SYSKEY_KEY7,keyInfo.key7);
		this->IniUserKey();
		return true;
	}
	else if (keyInfo.key8 == 95070)
	{
		keyInfo.key8 = nData;
		this->m_pUserKey.SetInt( SYSKEY_KEY8,keyInfo.key8);
		this->IniUserKey();
		return true;
	}
	else if (keyInfo.keyF1 == 95070)
	{
		keyInfo.key1 = nData;
		this->m_pUserKey.SetInt( SYSKEY_KEYF1,keyInfo.keyF1);
		this->IniUserKey();
		return true;
	}
	else if (keyInfo.keyF2 == 95070)
	{
		keyInfo.key2 = nData;
		this->m_pUserKey.SetInt( SYSKEY_KEYF2,keyInfo.keyF2);
		this->IniUserKey();
		return true;
	}
	else if (keyInfo.keyF3 == 95070)
	{
		keyInfo.key3 = nData;
		this->m_pUserKey.SetInt( SYSKEY_KEYF3,keyInfo.keyF3);
		this->IniUserKey();
		return true;
	}
	else if (keyInfo.keyF4 == 95070)
	{
		keyInfo.key4 = nData;
		this->m_pUserKey.SetInt( SYSKEY_KEYF4,keyInfo.keyF4);
		this->IniUserKey();
		return true;
	}
	else if (keyInfo.keyF5 == 95070)
	{
		keyInfo.key5 = nData;
		this->m_pUserKey.SetInt( SYSKEY_KEYF5,keyInfo.keyF5);
		this->IniUserKey();
		return true;
	}
	else if (keyInfo.keyF6 == 95070)
	{
		keyInfo.key6 = nData;
		this->m_pUserKey.SetInt( SYSKEY_KEYF6,keyInfo.keyF6);
		this->IniUserKey();
		return true;
	}
	else if (keyInfo.keyF7 == 95070)
	{
		keyInfo.key7 = nData;
		this->m_pUserKey.SetInt( SYSKEY_KEYF7,keyInfo.keyF7);
		this->IniUserKey();
		return true;
	}
	else if (keyInfo.keyF8 == 95070)
	{
		keyInfo.key8 = nData;
		this->m_pUserKey.SetInt( SYSKEY_KEYF8,keyInfo.keyF8);
		this->IniUserKey();
		return true;
	}
	else
		return false;
}

bool CUser::SaveUserKey(struct UserKey &KeyInfo,bool bUpdate/* = false*/)
{
	return m_pUserKey.SaveKey(KeyInfo,bUpdate,true);
}

bool CUser::SetInviteUserID(OBJID id)
{
	for(int i = 0 ; i < 5 ; i ++)
	{
		if(m_idInviteTeamID[i] == id)
			return true;
		if(m_idInviteTeamID[i] == ID_NONE)
		{
			m_idInviteTeamID[i] = id;
			return true;
		}
	}
	return false;
}

OBJID CUser::GetInviteUserID(int index)
{
	if(index < 0 || index > 4)
		return ID_NONE;
	else
		return m_idInviteTeamID[index];
}

bool CUser::IsInviteUserID(OBJID id)
{
	for(int i = 0 ; i < 5 ; i ++)
	{
		if(m_idInviteTeamID[i] == id)
			return true;
	}
	return false;
}

void CUser::ClrInviteUserID(OBJID id )
{
	if(id == ID_NONE)
	{
		for(int i = 0 ; i < 5 ; i ++)
		{
			m_idInviteTeamID[i] = 0;
		}
	}
	else
	{
		for(int i = 0 ; i < 5 ; i ++)
		{
			if(m_idInviteTeamID[i] == id)
				m_idInviteTeamID[i] = ID_NONE;
		}
	}
}

void CUser::ExchangePet()
{
	if(QueryPackage()->IsPackFull(2))//背包是否已经满了
	{
		this->SendSysMsg(/*g_objStrRes.GetStr(28338)*/"背包空间不足，请整理背包并至少留两格空间后，再来试试");
		return ;
	}

    DWORD dwLpet = 0;
    DWORD dwRpet = 0;

// 	CItem* pEquip = GetEquipItemByPos(ITEMPOSITION_EUEDMONL);
// 	if(pEquip)
// 	{
// 		dwLpet = pEquip->GetID();
// 	}
// 
// 	CItem* pEquip2 = GetEquipItemByPos(ITEMPOSITION_EUEDMONR);
// 	if(pEquip2)
// 	{
// 		dwRpet = pEquip2->GetID();
// 	}

// 	if( dwRpet != 0 || dwLpet != 0)
// 	{
// 		UnEquipItem(ITEMPOSITION_EUEDMONR, SYNCHRO_TRUE);
// 		UnEquipItem(ITEMPOSITION_EUEDMONL, SYNCHRO_TRUE);
// 		
// 		UseItem(dwRpet, ITEMPOSITION_EUEDMONL, SYNCHRO_TRUE);
// 		UseItem(dwLpet, ITEMPOSITION_EUEDMONR, SYNCHRO_TRUE);
// 	}
}
