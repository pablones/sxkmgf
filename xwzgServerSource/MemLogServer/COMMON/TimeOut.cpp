// TimeOut.cpp: implementation of the CTimeOut class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "TimeOut.h"

//////////////////////////////////////////////////////////////////////
// CTimeOut
//////////////////////////////////////////////////////////////////////
// ��ʱ����۵Ķ�ʱʱ��
bool CTimeOut::ToNextTick(int nSecs)
{
	if(IsTimeOut(nSecs))
	{
		if(time(NULL) >= m_tUpdateTime + nSecs*2)
			return Update(),true;
		else
			return (m_tUpdateTime+=nSecs),true;
	}
	else
		return false;
}

//////////////////////////////////////////////////////////////////////
// CTimeOutMS
//////////////////////////////////////////////////////////////////////
// ��ʱ����۵Ķ�ʱʱ��
bool CTimeOutMS::ToNextTick(int nMilliSecs)
{
	if(IsTimeOut(nMilliSecs))
	{
		if(clock() >= m_tUpdateTime + nMilliSecs*2)
			return Update(),true;
		else
			return (m_tUpdateTime+=nMilliSecs),true;
	}
	else
		return false;
}

int CTimeOutMS::GetRemain()  // ���������������Ϊ����BUFʱ��Ϊ-1 [�½���8/4/2007]
{
	if(m_nInterval < 0) 
		return m_nInterval; 
	else 
		return m_tUpdateTime ? ::CutRange(m_nInterval - ((int)clock()-(int)m_tUpdateTime), 0, m_nInterval) : 0; 
}
