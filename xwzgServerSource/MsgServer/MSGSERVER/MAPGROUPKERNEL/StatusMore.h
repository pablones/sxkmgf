// StatusOnce.h: interface for the CStatusMore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUSONCE_H__D1AD5A1E_58D9_49C0_ACE9_B9A0C4B20336__INCLUDED_2)
#define AFX_STATUSONCE_H__D1AD5A1E_58D9_49C0_ACE9_B9A0C4B20336__INCLUDED_2

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "I_Role.h"

const int	STATUS_FLASH_REMAIN_MS		= 5000;			// 提前5秒闪烁

struct StatusInfoStruct
{
	int	nStatus;
	int nPower;
	int	nSecs;
	int	nTimes;				//  一次性状态 nTimes = 0;
	int nParam1;
	int nParam2;
	int nParam3;	//06.11.4王玉波添加
};

struct StatusInfoStruct;
class CStatusMore : IStatus
{
protected:
	CStatusMore();
	virtual ~CStatusMore();
public:
	static CStatusMore* CreateNew()			{ return new CStatusMore; }
	ULONG	ReleaseByOwner()				{ delete this; return 0; }
	OBJID	GetID()							{ return m_nStatus; }
	bool	Create( IRole* pRole, int nStatus, int nPower, int nSecs, int nTimes, int nParam1, int nParam2,int nParam3 ,OBJID idOwner=0)
			{ 
				m_pOwner	= pRole; 
				m_nStatus	= nStatus;
				if(m_nStatus == STATUS_DOT_TUTOREXP)
					m_tQuestion.Startup(30);
				m_nData		= nPower; 
				m_nParam1	= nParam1;
				m_nParam2	= nParam2;
				m_nParam3	= nParam3;
				m_tKeep.Startup(nSecs); 
				m_nTimes	= nTimes;
				m_idAttker	= idOwner;
				if (ID_NONE == m_idAttker)
					m_idAttker = pRole->GetID();
				return true;
			}
	IStatus*	QueryInterface()			{ return (IStatus*)this; } 

public: // application
	int			GetSec();
	bool		IsValid()					{ return m_nTimes > 0; }
	int			GetPower();	//					{ return m_nData; }
	int			GetParam1()					{ return m_nParam1;	}
	int			GetParam2()					{ return m_nParam2; }
	int			GetParam3()					{ return m_nParam3;	}
	bool		GetInfo(StatusInfoStruct* pInfo);
	bool		OnTimer(DWORD tCurr);
	bool		ChangeData( int nPower,int nSecs,int nTimes = 0,int nParam1 = 0,int nParam2 = 0,int nParam3 = 0 ,OBJID idOwner=0);		// =0 : status once
	bool		IncTime(int nMilliSecs, int nLimit)			{ return false; }
	bool		ToFlash();
	int			GetSecLeft();
	void		SetFreeze();
	void		SetUnFreeze();
protected:
	IRole*		m_pOwner;
	int			m_nData;
	int			m_nStatus;
	int			m_nParam1;
	int			m_nParam2;
	int			m_nParam3;
	CTimeOut	m_tKeep;
	int			m_nTimes;
	auto_long	m_bFlash;
	OBJID		m_idAttker;
	CTimeOut	m_tQuestion;
	int			m_nQuestionAmount;

	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_STATUSONCE_H__D1AD5A1E_58D9_49C0_ACE9_B9A0C4B20336__INCLUDED_2)
