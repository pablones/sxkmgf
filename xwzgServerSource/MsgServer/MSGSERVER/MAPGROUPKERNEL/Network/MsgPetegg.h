// MsgPetegg.h: interface for the CMsgPetegg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGPETEGG_H__85DA23F1_F46E_4813_8F3F_4D346E205D52__INCLUDED_)
#define AFX_MSGPETEGG_H__85DA23F1_F46E_4813_8F3F_4D346E205D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum{
	PETEGG_NONE	  = 0,
	PETEGG_QUERYINFO = 1,  //查询可选宠物
	PETEGG_QUICKFLUSH = 2, //使用道具刷新
	PETEGG_GETPET	= 3,   //获取宠物
	PETEGG_FLUSHED	= 4,
};

typedef struct 
{
	OBJID	idData;
	UCHAR	ucGetType;
	UCHAR	ucLookFace;
	USHORT	usGrow;
	USHORT	usStr;
	USHORT	usInt;
	USHORT	usSta;
	USHORT	usSpi;
	USHORT	usAgi;
	USHORT	usMaxStr;
	USHORT	usMaxInt;
	USHORT	usMaxSta;
	USHORT	usMaxSpi;
	USHORT	usMaxAgi;
}ST_PETEGG;

class CMsgPetegg : public CNetMsg  
{
public:
	CMsgPetegg();
	virtual ~CMsgPetegg();

public:
	BOOL	Create(char* pbufMsg, DWORD dwMsgSize);
	BOOL	Create(OBJID idUser, OBJID idItem, UCHAR ucAction, USHORT usType, LONG nNeedSec, LPCTSTR szName);
	
	BOOL	Append(ST_PETEGG pInfo);
	
	void	Process(void* pInfo);

public:
	typedef struct
	{
		MSGHEAD_DEFINE
			
		OBJID		idUser;
		OBJID		idItem;
		LONG		nNeedSec;
		USHORT		usType;
		UCHAR		ucAction;
		UCHAR		ucAmount;
		char		szName[_MAX_NAMESIZE];
		ST_PETEGG	setPet[1];
	}MSG_Info;
	
	MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGPETEGG_H__85DA23F1_F46E_4813_8F3F_4D346E205D52__INCLUDED_)
