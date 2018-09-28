// VipList.cpp: implementation of the CVipList class.
//
//////////////////////////////////////////////////////////////////////

#include "VipList.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVipList g_VipListMgr;

CVipList::CVipList()
{
	m_vetVipList.clear();
}

CVipList::~CVipList()
{
	m_vetVipList.clear();
}

int CVipList::GetVipAmount()
{
	return m_vetVipList.size();
}

ST_VIPLIST* CVipList::GetVipUser(OBJID idUser)
{
	CHECKF(idUser);

	for(int i=m_vetVipList.size()-1; i>=0; i--)
	{
		if(m_vetVipList[i].idUser == idUser)
			return &m_vetVipList[i];
	}

	return NULL;
}

bool CVipList::AddVipUser(CUser* pUser)
{
	CHECKF(pUser);
	CHECKF(pUser->GetMap());
	
	ST_VIPLIST info = {0};
	info.idUser = pUser->GetID();
	info.idMap = pUser->GetMap()->GetDocID();
	info.usLev = pUser->GetLev();
	info.ucLine = GetLineByName();
	if(pUser->IsVipActive())
		info.ucVipLev = pUser->GetVipLev();
	else
		info.ucVipLev = 0;
	SafeCopy(info.szName, pUser->GetName(), _MAX_NAMESIZE);
		
	return AddVipUser(info);
}

bool CVipList::AddVipUser(ST_VIPLIST Info)
{
	CHECKF(Info.idUser);

	DEBUG_TRY
	ST_VIPLIST* pInfo = GetVipUser(Info.idUser);
	if(pInfo)
	{
		int nOldVipLev = pInfo->ucVipLev;
		pInfo->idUser = Info.idUser;
		pInfo->idMap = Info.idMap;
		pInfo->usLev = Info.usLev;
		pInfo->ucLine = Info.ucLine;
		pInfo->ucVipLev = Info.ucVipLev;
		SafeCopy(pInfo->szName, Info.szName, _MAX_NAMESIZE);

		if(nOldVipLev!=Info.ucVipLev)
			sort(m_vetVipList.begin(), m_vetVipList.end(), CVipList::CompareVipLev);
		return true;
	}

	m_vetVipList.push_back(Info);
	sort(m_vetVipList.begin(), m_vetVipList.end(), CVipList::CompareVipLev);
	DEBUG_CATCH2("CVipList::AddVipUser error!idUser=%d", Info.idUser)

	return true;
}

bool CVipList::DelVipUser(OBJID idUser)
{
	CHECKF(idUser);

	DEBUG_TRY
	VET_VIPLIST::iterator it = m_vetVipList.begin();
	for(; it!=m_vetVipList.end(); it++)
	{
		if(it->idUser == idUser)
		{
			m_vetVipList.erase(it);
			return true;
		}
	}
	DEBUG_CATCH2("CVipList::DelVipUser error!idUser=%d", idUser)

	return true;
}

int CVipList::QueryInfo(ST_VIPLIST* pData, int nPage, int nPageSize, OBJID idSelf/*=0*/)
{
	CHECKF(pData);
	CHECKF(nPage);
	CHECKF(nPageSize>0 && nPageSize<=20);

	if(GetVipAmount()<=0)
		return 0;
	
	int nAmount = 0;
	DEBUG_TRY
	//计算本页数据的起始和结束下标
	int nIndexStart = (nPage-1)*nPageSize;
	int nIndexEnd = nPage*nPageSize;

	if(nIndexStart >= GetVipAmount())
		return 0;

	for(int i=nIndexStart; i<nIndexEnd && i<GetVipAmount(); i++)
	{
		if(m_vetVipList[i].idUser == idSelf)
		{
			nIndexEnd++;
			continue;
		}
		pData[nAmount].idUser = m_vetVipList[i].idUser;
		pData[nAmount].idMap  = m_vetVipList[i].idMap;
		pData[nAmount].usLev  = m_vetVipList[i].usLev;
		pData[nAmount].ucLine = m_vetVipList[i].ucLine;
		pData[nAmount].ucVipLev = m_vetVipList[i].ucVipLev;
		SafeCopy(pData[nAmount].szName, m_vetVipList[i].szName, _MAX_NAMESIZE);
		
		nAmount++;
	}
	DEBUG_CATCH3("CVipList::QueryInfo error!amount=%d,page=%d", GetVipAmount(), nPage)
	return nAmount;
}

bool CVipList::CompareVipLev(ST_VIPLIST info1, ST_VIPLIST info2)
{
	if(info1.ucVipLev == 4)//周卡
		return false;
	else if(info2.ucVipLev == 4)
		return true;
	else
		return info1.ucVipLev > info2.ucVipLev;
}

int CVipList::GetLineByName(LPCTSTR szLine)
{
	int nLine = 0;
	char szServer[32] = "";
	if(szLine && strlen(szLine)>0)
		strcpy(szServer, szLine);
	else
		strcpy(szServer, MapGroup(MSGPORT_MAPGROUP_FIRST)->GetServerName());

	if (strcmp(szServer, "一线") == 0)
		nLine = 1;
	else if (strcmp(szServer, "二线") == 0)
		nLine = 2;
	else if (strcmp(szServer, "三线") == 0)
		nLine = 3;
	else if (strcmp(szServer, "四线") == 0)
		nLine = 4;
	else if (strcmp(szServer, "五线") == 0)
		nLine = 5;
	else if (strcmp(szServer, "六线") == 0)
		nLine = 6;

	return nLine;
}