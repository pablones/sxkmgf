// MsgVipList.h: interface for the CMsgVipList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGVIPLIST_H__B7D0DBDE_C50D_44DB_BEC7_53D397352E55__INCLUDED_)
#define AFX_MSGVIPLIST_H__B7D0DBDE_C50D_44DB_BEC7_53D397352E55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum{
	VIPLIST_NONE	  = 0,
	VIPLIST_QUERYINFO = 1,//查询vip列表
	VIPLIST_LINEBORAD//线路广播
};

enum{
	VIPLINEBORAD_NONE	= 0,
	VIPLINEBORAD_ONLINE = 1,//vip上线或转场
	VIPLINEBORAD_OFFLINE= 2,//vip下线

	VIPLINEBORAD_VASLIMIT = 101//商城限量商品，借用此协议进行线路广播
};
typedef struct 
{
	OBJID	idUser;
	OBJID	idMap;
	USHORT	usLev;
	UCHAR	ucLine;
	UCHAR	ucVipLev;
	NAMESTR szName;
}ST_VIPLIST;

class CMsgVipList : public CNetMsg  
{
public:
	CMsgVipList();
	virtual ~CMsgVipList();

public:
	BOOL	Create(char* pbufMsg, DWORD dwMsgSize);
	BOOL	Create(OBJID idUser, USHORT usAction, UCHAR ucPage);

	BOOL	Append(OBJID idUser, OBJID idMap, USHORT usLev, UCHAR ucLine, UCHAR ucVipLev, LPCTSTR szName);
	BOOL	Append(ST_VIPLIST pInfo);

	void	Process(void* pInfo);
	void	ProcessLineMsg(void* pInfo);

public:
	typedef struct
	{
		MSGHEAD_DEFINE
			
		OBJID		idUser;
		USHORT		usAction;		//action
		UCHAR		ucPage;
		UCHAR		ucAmount;		//数量
		ST_VIPLIST	setMember[1];	//vip列表
	}MSG_Info;

	MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGVIPLIST_H__B7D0DBDE_C50D_44DB_BEC7_53D397352E55__INCLUDED_)
