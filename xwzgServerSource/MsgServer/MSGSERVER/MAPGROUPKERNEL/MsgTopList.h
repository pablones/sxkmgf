#ifndef __MSGTOPLIST_H
#define __MSGTOPLIST_H

enum
{
	MSGTOPLIST_WORLDLEVEL		= 0,		//世界等级
	MSGTOPLIST_WORLDFIRJOB,
	MSGTOPLIST_WORLDSECJOB,
	MSGTOPLIST_WORLDMONEY,
	MSGTOPLIST_WORLDPHYATT,
	MSGTOPLIST_WORLDMAGATT,
	MSGTOPLIST_WORLDMAXLIFE,
	MSGTOPLIST_WORLDCRIT,
	MSGTOPLIST_WORLDHITRATE,
	MSGTOPLIST_WORLDSCORE,
	MSGTOPLIST_SYNMEM,
	MSGTOPLIST_SYNMONEY,
	MSGTOPLIST_EUDEMONSCORE,

 	MSGTOPLIST_QUERYTOTAL		= 20,		//查询请求
};

struct toplistMemData
{
	unsigned int data1;
	unsigned int data2;
	char szName[16];
	char szSyn[16];
};

#include "NetMsg.h"

class CMsgTopList:public CNetMsg
{
public:
	CMsgTopList();
	virtual ~CMsgTopList();
public:
	BOOL	Create		( char* pMsgBuf, DWORD dwSize );
	BOOL	Create		(  USHORT usType,toplistMemData* pData,int nSize,USHORT usPage,USHORT myPos);
	void	Process		( void* pInfo );
protected:
	typedef struct
	{
		MSGHEAD_DEFINE
		USHORT		usAction;
		USHORT		usPage;
		USHORT		usAmount;
		USHORT		usData;
		toplistMemData topData[1];				//数据
	}MSG_Info;
	MSG_Info*	m_pInfo;
};




#endif //__MSGTOPLIST_H