// MsgGmMac.cpp: implementation of the CMsgGmMac class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgGmMac.h"
#include "AllMsg.h"
#include "../mapgroup.h"
#include "../transformation.h"
#include "../Agent.h"
#include "../User.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgGmMac::CMsgGmMac()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgGmMac::~CMsgGmMac()
{

}
BOOL CMsgGmMac::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_GMMAC != this->GetType())
		return false;

	return true;
}
BOOL CMsgGmMac::Create(char* usMac, USHORT usAction,USHORT usSuc)
{
	this->Init();

	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_GMMAC;
	strcpy(m_pInfo->usMac,usMac);
	m_pInfo->usSuc  = usSuc;
	m_pInfo->usAction = usAction;
	return true;
}
void CMsgGmMac::Process(void* pInfo)
{
	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE addressmac='%s'", _TBL_GMMAC, m_pInfo->usMac );
	IRecordset* pRes = Database()->CreateNewRecordset(szSQL);	
	if (pRes)
	{
		char szBuf[32] ={0};	
		SafeCopy(szBuf, pRes->GetStr(1), _MAX_NAMESIZE);
		CMsgGmMac msg;
		if (msg.Create(szBuf,ACTION_CHECKGMMACSUC,1))
		{
			SendMsg(&msg);
		}
		pRes->Release();
	}
	else
	{	
		char szBuf[32] ={0};
		CMsgGmMac msg;
		if (msg.Create(szBuf,ACTION_CHECKGMMACFAIL,0))
		{
			SendMsg(&msg);
		}
	}
}