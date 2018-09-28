// GiftManager.cpp: implementation of the CGiftManager class.
//
//////////////////////////////////////////////////////////////////////

#include "GiftManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//���½�����������ȷ��������-------
const int _MAXGIFT_PROF = 511;//����츳ְҵ.ͨ��
const int _MAXGIFT_SORT = _MAXGIFT_PROF*10 + 3;//����츳ϵ
const int _MAXGIFT_TYPE = _MAXGIFT_SORT*100 + 99;//����츳����

MYHEAP_IMPLEMENTATION(CGiftManager,s_heap)
//char	szGiftTable[] = _TBL_GIFT;
char	szGiftTreeTable[] = _TBL_GIFTTREE;
//global
CGiftManager* g_pGiftManager=NULL;


CGiftManager::CGiftManager()
{
//	m_setGift = NULL;
	m_setGiftTree = NULL;
	m_pGiftTempleteManager = NULL;
}

CGiftManager::~CGiftManager()
{
//	if(m_setGift)
//		m_setGift->Release();

	if(m_setGiftTree)
		m_setGiftTree->Release();
	if(m_pGiftTempleteManager)
		m_pGiftTempleteManager->Release();
}


//-----------------------------
bool CGiftManager::Create (IDatabase* pDb)
{

	CHECKF(pDb);
	CHECKF(!m_setGiftTree);
	CHECKF(!m_pGiftTempleteManager);

	char szSQL[1024];

	//sk_gift_tree
	m_setGiftTree	= CGiftTreeSet::CreateNew(true);
	CHECKF(m_setGiftTree);

	sprintf(szSQL, "SELECT * FROM %s", _TBL_GIFTTREE);
	IF_NOT_(m_setGiftTree->Create(szSQL, pDb))
		return false;


	//sk_gift_templet_manager
	m_pGiftTempleteManager = CGiftTempleteManager::CreateNew();
	CHECKF(m_pGiftTempleteManager);
	m_pGiftTempleteManager->Create(pDb);

	return true;
}

//---------------------------------------
CGiftTreeData* CGiftManager::GetGiftData(OBJID giftType)//ͨ��type��lev�õ�������Ϣ
{
	CHECKF(giftType>=0 && giftType<=_MAXGIFT_TYPE);
	return m_setGiftTree->GetObj(giftType);
}

//-----------------------------------------
//[!��Ŀ]����㱣֤�ͷ�IRecord
IRecord* CGiftManager::GetGiftTemplete(OBJID giftType)
{
	CHECKF(giftType>=0 && giftType<=_MAXGIFT_TYPE);

	CGiftTreeData* pGiftTreeDataTemp = GetGiftData(giftType);
	CHECKF(pGiftTreeDataTemp);

	OBJID tmpType = pGiftTreeDataTemp->GetInt(GIFTTREEDATA_temType);
	OBJID tmpIdx = pGiftTreeDataTemp->GetInt(GIFTTREEDATA_temID);
	return GetGiftTempleteInfo(tmpType,tmpIdx);
}
