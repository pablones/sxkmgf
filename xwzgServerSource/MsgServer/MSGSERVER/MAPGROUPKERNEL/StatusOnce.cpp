// StatusOnce.cpp: implementation of the CStatusOnce class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "../share/define.h"
#include "I_Role.h"
#include "TimeOut.h"
#include "myheap.h"
#include "StatusMore.h"
#include "StatusOnce.h"
#include "MapGroup.h"
#include "MsgUserAttrib.h"

MYHEAP_IMPLEMENTATION(CStatusOnce,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CStatusOnce::CStatusOnce()
{
	m_pOwner	= NULL;
	m_nStatus	= 0;
}

//////////////////////////////////////////////////////////////////////
CStatusOnce::~CStatusOnce()
{
//	if(m_pOwner)
//	{
//		m_pOwner->ClsStatus( m_nStatus );
//		m_pOwner->ClsBuff( m_nStatus ); //王玉波添加，07.4.27
//	}
}


void CStatusOnce::SetPower(int data)
{
	m_nData = data;
}
//////////////////////////////////////////////////////////////////////
bool CStatusOnce::GetInfo(StatusInfoStruct* pInfo)
{
	pInfo->nPower	= m_nData;
	pInfo->nStatus	= m_nStatus;
	pInfo->nSecs	= m_tKeep.GetRemain()/1000;
	pInfo->nTimes	= 0;					// Status once
	pInfo->nParam1	= m_nParam1;
	pInfo->nParam2	= m_nParam2;
	pInfo->nParam3	= m_nParam3;
	
	return IsValid();
}

//////////////////////////////////////////////////////////////////////
bool CStatusOnce::ChangeData( int nPower, int nSecs, int nTimes, int nParam1, int nParam2,int nParam3 ,OBJID idOwner)
{
	if(!(nSecs > 0 && nTimes == 0))
	{
		LOGWARNING("CStatusOnce::ChangeData() status = %u nSecs = %u nTimes = %u",GetID(),nSecs,nTimes);
		return false;
	}

//	if(nPower)
//	{
//		// 时间多则复盖
//		if(nSecs*1000 >= m_tKeep.GetRemain())
//		{
			m_nData		= nPower;
			m_nParam1	= nParam1;
			m_nParam2	= nParam2;
			m_nParam3	= nParam3;
			m_tKeep.SetInterval(nSecs*1000);
			m_tKeep.Update();
			m_idAttker	= idOwner;
//		}
		return true;
//	}
//	else // power == 0: 减时间
//	{
//		// 减时间
//		m_tKeep.DecInterval(nSecs*1000);
//	}
//	return false;
}
//////////////////////////////////////////////////////////////////////
void CStatusOnce::SetFreeze()
{
	if (m_nParam3==1)
		return;
	m_nParam3 = 1;
}
//////////////////////////////////////////////////////////////////////
void CStatusOnce::SetUnFreeze()
{
	if (m_nParam3==0)
		return;
	m_nParam3 = 0;
}
//////////////////////////////////////////////////////////////////////
bool CStatusOnce::IncTime(int nMilliSecs, int nLimit)
{
	int nInterval = __min(nMilliSecs+m_tKeep.GetRemain(), nLimit);
	m_tKeep.SetInterval(nInterval);
	m_tKeep.Update();
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CStatusOnce::ToFlash()
{
	if(!IsValid())
		return false;

	if(!m_bFlash && m_tKeep.GetRemain() <= STATUS_FLASH_REMAIN_MS)
	{
		m_bFlash = true;
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////
bool CStatusOnce::OnTimer(DWORD tCurr)
{
	return true;
}
//07.4.26
//王玉波
bool CStatusOnce::IsValid()
{ 
	if( m_tKeep.IsActive() )
	{
		if( m_tKeep.GetInterval() == FOREVER_TIME * 1000 )//永久时间
		{
			return true;
		}
		if( m_tKeep.IsTimeOut() )
		{
			return false;
		}
		else
		{
			return true;
		}
		
	}
	else
	{
		return false;
	}
}

//  [3/31/2008 朱斌]
//	得持续时间
int CStatusOnce::GetSec()
{
	return m_tKeep.GetInterval() / 1000;
}

int CStatusOnce::GetSecLeft()
{
	return m_tKeep.GetRemain()/1000;
}