// CompensationMsg.h: interface for the CCompensationMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPENSATIONMSG_H__AD512E52_4BE4_4D1B_ADA2_60631DFE34E8__INCLUDED_)
#define AFX_COMPENSATIONMSG_H__AD512E52_4BE4_4D1B_ADA2_60631DFE34E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "../../GameBaseCodeMT/ConstGameData.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"

enum COMPENSATIONMSGDATA{
	COMPENSATIONMSGDATA_ID = 0,
	COMPENSATIONMSGDATA_USERID,
	COMPENSATIONMSGDATA_TYPE, //1.²Ù×÷ 2.ÏûÏ¢
	COMPENSATIONMSGDATA_WRITETIME,
	COMPENSATIONMSGDATA_MSG,
};

enum COMPENSATIONMSGTYPE
{
	COMPENSATIONMSGTYPE_OPMSG = 1,
	COMPENSATIONMSGTYPE_TIMEMSG,
	COMPENSATIONMSGTYPE_CLRMSG,
};

char	szCompensationMsgTable[];
typedef		CGameData<COMPENSATIONMSGDATA, szCompensationMsgTable, szID>	CCompensationMsgData;

class CCompensationMsg  
{
public:
	CCompensationMsg();
	virtual ~CCompensationMsg();

	static CCompensationMsg* CreateNew() { CCompensationMsg* ptr=new CCompensationMsg; return ptr;}
	
	bool	Create(IRecordset* pRes);
	bool	Create(IRecord* pDefault,OBJID idOwner, DWORD dwMsgType,const char* szMsg,const char* szTime, bool bInsert=true);
	ULONG	Release()				{ delete this; return 0; }
	
	//	void    GetInfo(InstanceUseTimeInfo* pInfo);
	void    Destroy();
public:
	int		GetInt(COMPENSATIONMSGDATA idx)                  { return m_pData->GetInt(idx); }
	LPCTSTR	GetStr(COMPENSATIONMSGDATA idx)		   	      { return m_pData->GetStr(idx); }
	
private:	
	CCompensationMsgData*    m_pData;
};

#endif // !defined(AFX_COMPENSATIONMSG_H__AD512E52_4BE4_4D1B_ADA2_60631DFE34E8__INCLUDED_)
