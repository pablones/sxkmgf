// MsgJinmei.h: interface for the CMsgJinmei class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGJINMEI_H__38999DB7_A78D_4675_A369_0ECD1713D52D__INCLUDED_)
#define AFX_MSGJINMEI_H__38999DB7_A78D_4675_A369_0ECD1713D52D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "NetMsg.h"

typedef struct 
{
	int nType;
	int nLev;
	int nValue;
	int nLeaveTime;
	int nStrengthLev;
	int nAddRate;
	int nStatu;
	int nAddQueueTime;
}UserJinmeiInfoStruct;


class CMsgJinmei: public CNetMsg
{
public:
	CMsgJinmei();
	virtual ~CMsgJinmei();
	
public:		//接口函数
	BOOL	Create(OBJID idUser, int nType, int nLev, int nValue, int nLeaveTime, int nStatu, int nAddQueueTime, int nStrengthLev, int nAddRate,
		      int nAllLevGrade , int nAllStrengthLevAdd);
	
	BOOL	Append(int nType, int nLev, int nValue, int nLeaveTime, int nStrengthLev, int nAddRate, int nStatu, int nAddQueueTime);
	
public:
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(CGameSocket* pSocket);
	
private:
	typedef struct
	{
		MSGHEAD_DEFINE

		OBJID idUser;	
		int nAmount;
		int nAllLevGrade;
		int nAllStrengthLevAdd;
		UserJinmeiInfoStruct	setJinmeiAttr[1];			// 变长
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGJINMEI_H__38999DB7_A78D_4675_A369_0ECD1713D52D__INCLUDED_)
