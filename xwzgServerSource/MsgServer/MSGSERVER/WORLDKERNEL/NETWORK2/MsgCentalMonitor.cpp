// MsgCentalMonitor.cpp: implementation of the CMsgCentralMonitor class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgCentalMonitor.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgCentralMonitor::CMsgCentralMonitor()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgCentralMonitor::~CMsgCentralMonitor()
{

}

void CMsgCentralMonitor::Process(void *pInfo)
{
}


BOOL CMsgCentralMonitor::Create(char *pszServerName, char *pszServerIP, int nType, int nSerialNumber, BOOL bLastOne, char *pszTrace, int nLength)
{
	// init
	this->Init();

	// fill info now
	m_pInfo->unMsgSize	= sizeof(MSG_Info) + nLength - 1;
	m_pInfo->unMsgType	=_MSG_MONITORSERVER_MONITOR;

	strcpy(m_pInfo->szServerName, pszServerName);
	strcpy(m_pInfo->szServerIP, pszServerIP);
	m_pInfo->nTraceType = nType;
	m_pInfo->nSerialNumber = nSerialNumber;
	m_pInfo->bLastOne = bLastOne;
	strncpy(m_pInfo->szText, pszTrace, nLength);
	m_pInfo->nTextSize = nLength;

	return true;
}
