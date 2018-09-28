// GiftManager.cpp: implementation of the CGiftManager class.
//
//////////////////////////////////////////////////////////////////////

#include "GiftManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//以下仅用做不甚精确的冗余检查-------
const int _MAXGIFT_PROF = 511;//最大天赋职业.通用
const int _MAXGIFT_SORT = _MAXGIFT_PROF*10 + 3;//最大天赋系
const int _MAXGIFT_TYPE = _MAXGIFT_SORT*100 + 99;//最大天赋类型

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
CGiftTreeData* CGiftManager::GetGiftData(OBJID giftType)//通过type和lev得到具体信息
{
	CHECKF(giftType>=0 && giftType<=_MAXGIFT_TYPE);
	return m_setGiftTree->GetObj(giftType);
}

//-----------------------------------------
//[!醒目]由外层保证释放IRecord
IRecord* CGiftManager::GetGiftTemplete(OBJID giftType)
{
	CHECKF(giftType>=0 && giftType<=_MAXGIFT_TYPE);

	CGiftTreeData* pGiftTreeDataTemp = GetGiftData(giftType);
	CHECKF(pGiftTreeDataTemp);

	OBJID tmpType = pGiftTreeDataTemp->GetInt(GIFTTREEDATA_temType);
	OBJID tmpIdx = pGiftTreeDataTemp->GetInt(GIFTTREEDATA_temID);
	return GetGiftTempleteInfo(tmpType,tmpIdx);
}
