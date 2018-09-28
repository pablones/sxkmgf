// VipList.h: interface for the CVipList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIPLIST_H__13178A81_9708_4596_9666_107CB90FCC67__INCLUDED_)
#define AFX_VIPLIST_H__13178A81_9708_4596_9666_107CB90FCC67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../share/define.h"
#include "Network/MsgVipList.h"
#include "User.h"
#include <vector>

class CVipList
{
public:
	CVipList();
	virtual ~CVipList();

public:
	int GetVipAmount();
	ST_VIPLIST* GetVipUser(OBJID idUser);
	bool AddVipUser(ST_VIPLIST Info);
	bool AddVipUser(CUser* pUser);
	bool DelVipUser(OBJID idUser);
	int	QueryInfo(ST_VIPLIST* pData, int nPage, int nPageSize, OBJID idSelf=0);//获得指定页的VIP列表信息,如果自己是vip,就把自己的ID带进来，否则就不需要

	static int GetLineByName(LPCTSTR szLine=NULL);
	static bool CompareVipLev(ST_VIPLIST info1, ST_VIPLIST info2);
private:
	typedef std::vector<ST_VIPLIST> VET_VIPLIST;
	VET_VIPLIST m_vetVipList;
};

extern CVipList g_VipListMgr;

#endif // !defined(AFX_VIPLIST_H__13178A81_9708_4596_9666_107CB90FCC67__INCLUDED_)
