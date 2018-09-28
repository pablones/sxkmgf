// StatusOnce.h: interface for the CStatusOnce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUSONCE_H__D1AD5A1E_58D9_49C0_ACE9_B9A0C4B20336__INCLUDED_)
#define AFX_STATUSONCE_H__D1AD5A1E_58D9_49C0_ACE9_B9A0C4B20336__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int FOREVER_TIME	= -1;			//07.4.26王玉波添加，表示永久的持续时间
#include "StatusMore.h"

class CStatusOnce : public IStatus
{
protected:
	CStatusOnce();
	virtual ~CStatusOnce();
public:
	static CStatusOnce* CreateNew()			{ return new CStatusOnce; }
	ULONG	ReleaseByOwner()				{ delete this; return 0; }
	OBJID	GetID()							{ return m_nStatus; }
	bool	Create(IRole* pRole, int nStatus, int nPower, int nSecs, int nParam1, int nParam2,int nParam3,OBJID idOwner=0 )
			{
				m_pOwner	= pRole;
				m_nStatus	= nStatus;
				m_nData		= nPower;
				m_nParam1	= nParam1;
				m_nParam2	= nParam2;
				m_nParam3	= nParam3;
				m_tKeep.Startup(nSecs*1000);
				m_idAttker	= idOwner;
//				m_tInterval.Startup(1000);	//07.4.25王玉波注释
				return true;
			}
	IStatus*	QueryInterface()			{ return (IStatus*)this; } 

public: // application
	int			GetSec();
	bool		IsValid();					
	void		SetPower(int data);
	int			GetPower()					{ return m_nData; }
	int			GetParam1	()				{ return m_nParam1; }
	int			GetParam2	()				{ return m_nParam2; }
	int			GetParam3	()				{ return m_nParam3;	}
	bool		GetInfo(StatusInfoStruct* pInfo);
	bool		OnTimer(DWORD tCurr);
	bool		ChangeData( int nPower, int nSecs, int nTimes=0, int nParam1=0, int nParam2=0,int nParam3 = 0 ,OBJID idOwner=0);		// =0 : status once
	bool		IncTime(int nMilliSecs, int nLimit);
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
	int			m_nParam3;		//06.11.4王玉波添加
	CTimeOutMS	m_tKeep;
	auto_long	m_bFlash;
	OBJID		m_idAttker;
//	CTimeOutMS	m_tInterval;	//07.4.25王玉波注释

	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_STATUSONCE_H__D1AD5A1E_58D9_49C0_ACE9_B9A0C4B20336__INCLUDED_)
