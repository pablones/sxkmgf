// MsgDialog.cpp: implementation of the CMsgDialog class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#ifdef	WORLD_KERNEL
#include "userlist.h"
using namespace world_kernel;
#else
#pragma warning(disable:4786)
#include "mapgroup.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgDialog::CMsgDialog()
{
	Init();
	m_pInfo	=(MSG_Info* )m_bufMsg;

	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgDialog::~CMsgDialog()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgDialog::Create	(int nAction, LPCTSTR szText, int idxTask)
{
	CHECKF(szText);

	// fill
	this->Init();

	// fill structure
	m_pInfo->idxTask	=idxTask;
	m_pInfo->ucAction	=nAction;

	BOOL ret = m_StrPacker.AddString(szText);

	m_unMsgType	=_MSG_DIALOG;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();

	return ret;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgDialog::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if(!pbufMsg)
		return false;

	memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgDialog::Process(void *pInfo)
{
#ifdef _MSGDEBUG
	::LogMsg("Process MsgDialog, Sender:%s, Receiver:%s, Words:%s", 
						szSender, 
						szReceiver, 
						szWords);
#endif

	ASSERT(!"CMsgDialog::Process");
}
