// NpcMagicManager.cpp: implementation of the CNpcMagicManager class.
//
//////////////////////////////////////////////////////////////////////

#include "NpcMagicManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNpcMagicManager*	g_pNpcMagicManager=NULL;


CNpcMagicManager::CNpcMagicManager()
{

}

CNpcMagicManager::~CNpcMagicManager()
{
	//����,delete
	for(MAP_NPCMAGICDATA::iterator iter = m_mNpcMagicData.begin(); iter!=m_mNpcMagicData.end(); iter++)
	{
		VCT_NPCMAGICDATA* pVctDataTemp = iter->second;
		IF_NOT(pVctDataTemp)
		{
			continue;
		}

		for(int i=0;i<pVctDataTemp->size();i++)
		{
			SAFE_DELETE((*pVctDataTemp)[i]);
		}
		pVctDataTemp->clear();
		SAFE_DELETE(pVctDataTemp);
	}
	m_mNpcMagicData.clear();


	for(int i=0;i<m_mEudemonMagicData.size();i++)
	{
		SAFE_DELETE(m_mEudemonMagicData[i]);
	}
	m_mEudemonMagicData.clear();

	for(i=0;i<m_mSoulMagicData.size();i++)
	{
		SAFE_DELETE(m_mSoulMagicData[i]);
	}
	m_mSoulMagicData.clear();
}


//----------------------------------
BOOL CNpcMagicManager::CreateData( IDatabase* pDb )//�����ݿ��ж����� 
{
	CHECKF(pDb);

	char szSQL[1024]={0};
	sprintf( szSQL, "SELECT * FROM %s",_TBL_MONTERMAGIC);
	IRecordset* pRes = pDb->CreateNewRecordset( szSQL, false );
	if(pRes)
	{
		m_mNpcMagicData.clear();
		for(int i = 0; i < pRes->RecordCount(); i++)
		{
			int nOwnerType = pRes->GetInt(EM_NPCMAGICDATA_ownertype);
			CNpcMagicData* pMagicData = new CNpcMagicData;
			CHECKF(pMagicData);
			IF_NOT(pMagicData->Create(pRes))
			{
				//����ʧ��
				pMagicData->Release();
				pRes->MoveNext();
				continue;
			}
			
			//�����type�Ѿ������
			MAP_NPCMAGICDATA::iterator itFind = m_mNpcMagicData.find(nOwnerType);
			if(itFind != m_mNpcMagicData.end())
			{
				VCT_NPCMAGICDATA* pVctDataTemp = itFind->second;
				CHECKF(pVctDataTemp);
				pVctDataTemp->push_back(pMagicData);
			}
			else//�²���
			{
				VCT_NPCMAGICDATA* pVctDataTemp = new VCT_NPCMAGICDATA;
				pVctDataTemp->push_back(pMagicData);//ѹ��vector
				//����map
				m_mNpcMagicData.insert(MAP_NPCMAGICDATA::value_type(nOwnerType,pVctDataTemp));
			}
			pRes->MoveNext();
		}
		pRes->Release();
		//return true;
	}

	szSQL[0]=0;
	sprintf( szSQL, "SELECT * FROM %s",_TBL_EUDEMONMAGIC);
	pRes = pDb->CreateNewRecordset( szSQL, false );
	if(pRes)
	{
		m_mEudemonMagicData.clear();
		for(int i = 0; i < pRes->RecordCount(); i++)
		{
			CEudemonMagicData* pMagicData = new CEudemonMagicData;
			CHECKF(pMagicData);
			IF_NOT(pMagicData->Create(pRes))
			{
				//����ʧ��
				pMagicData->Release();
				pRes->MoveNext();
				continue;
			}
			m_mEudemonMagicData.push_back(pMagicData);
			
			pRes->MoveNext();
		}
		pRes->Release();
	}

	szSQL[0]=0;
	sprintf( szSQL, "SELECT * FROM %s",_TBL_SOULMAGIC);
	pRes = pDb->CreateNewRecordset( szSQL, false );
	if(pRes)
	{
		m_mSoulMagicData.clear();
		for(int i = 0; i < pRes->RecordCount(); i++)
		{
			CSoulMagicData* pMagicData = new CSoulMagicData;
			CHECKF(pMagicData);
			IF_NOT(pMagicData->Create(pRes))
			{
				//����ʧ��
				pMagicData->Release();
				pRes->MoveNext();
				continue;
			}
			m_mSoulMagicData.push_back(pMagicData);
			
			pRes->MoveNext();
		}
		pRes->Release();
		return true;
	}
	return false;
}

//----------------------------------------
const VCT_NPCMAGICDATA* CNpcMagicManager::QueryDataByOwnerType(OBJID monsterType)//ͨ��monsterType�õ�magic�ļ���
{
	MAP_NPCMAGICDATA::iterator itFind = m_mNpcMagicData.find(monsterType);
	if(itFind != m_mNpcMagicData.end())
	{
		CHECKF(itFind->second);
		return itFind->second;
	}

	return NULL;
}

//-----------------------------------------
const CNpcMagicData* CNpcMagicManager::QueryData(OBJID monsterType,
												 OBJID magicType)//ͨ��monstertype��magicType����þ��弼����Ϣ
{
	const VCT_NPCMAGICDATA* pVtcDataTemp = QueryDataByOwnerType(monsterType);
	if(pVtcDataTemp)
	{
		for(int i=0;i<pVtcDataTemp->size();i++)
		{
			CNpcMagicData* pDataTemp = (*pVtcDataTemp)[i];
			CHECKF(pDataTemp);
			if(pDataTemp->GetInt(EM_NPCMAGICDATA_magictype) == magicType)
			{
				//�����ҵ���.�����ǵڶ���ƥ��.��ʱ����
				return pDataTemp;
			}
		}
	}

	return NULL;
}

//-----------------------------------------
CEudemonMagicData* CNpcMagicManager::QueryEudemonData(int id)
{
	for(int i=0;i<m_mEudemonMagicData.size();i++)
	{
		CEudemonMagicData* pDataTemp = m_mEudemonMagicData[i];
		CHECKF(pDataTemp);
		if(pDataTemp->GetInt(EM_EUDEMONMAGIC_id) == id)
		{
			return pDataTemp;
		}
	}	
	return NULL;
}

//goto ������type������
CEudemonMagicData* CNpcMagicManager::QueryEudemonDataByType(int nType)
{
	for(int i=0;i<m_mEudemonMagicData.size();i++)
	{
		CEudemonMagicData* pDataTemp = m_mEudemonMagicData[i];
		CHECKF(pDataTemp);
		if(pDataTemp->GetInt(EM_EUDEMONMAGIC_dataID) == nType)
		{
			return pDataTemp;
		}
	}	
	return NULL;
}

int CNpcMagicManager::QueryEudemonSkill(int skillType)
{
	for(int i=0;i<m_mEudemonMagicData.size();i++)
	{
		CEudemonMagicData* pDataTemp = m_mEudemonMagicData[i];
		CHECKF(pDataTemp);
		if(pDataTemp->GetInt(EM_EUDEMONMAGIC_dataID) == skillType)
		{
			return pDataTemp->GetInt(EM_EUDEMONMAGIC_basicLev);
		}
	}	
	return 0;
}

int CNpcMagicManager::GetEudSkillIDByType(int skillType)
{
	for(int i=0;i<m_mEudemonMagicData.size();i++)
	{
		CEudemonMagicData* pDataTemp = m_mEudemonMagicData[i];
		CHECKF(pDataTemp);
		if(pDataTemp->GetInt(EM_EUDEMONMAGIC_dataID) == skillType)
		{
			return pDataTemp->GetInt(EM_EUDEMONMAGIC_id);
		}
	}	
	return 0;
}

CSoulMagicData* CNpcMagicManager::QuerySoulData(int id)
{
	for(int i=0;i<m_mSoulMagicData.size();i++)
	{
		CSoulMagicData* pDataTemp = m_mSoulMagicData[i];
		CHECKF(pDataTemp);
		if(pDataTemp->GetInt(EM_SOULMAGIC_id) == id)
		{
			return pDataTemp;
		}
	}	
	return NULL;
}

CSoulMagicData* CNpcMagicManager::QuerySoulDataByType(int nType)
{
	for(int i=0;i<m_mSoulMagicData.size();i++)
	{
		CSoulMagicData* pDataTemp = m_mSoulMagicData[i];
		CHECKF(pDataTemp);
		if(pDataTemp->GetInt(EM_SOULMAGIC_dataID) == nType)
		{
			return pDataTemp;
		}
	}	
	return NULL;
}
