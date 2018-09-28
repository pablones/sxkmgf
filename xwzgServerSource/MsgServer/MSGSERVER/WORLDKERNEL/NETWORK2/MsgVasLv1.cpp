//#include "MsgVasLv1.h"
#include "AllMsg.h"
#include "WorldKernel.h"
using namespace world_kernel;

CMsgVasLv3::CMsgVasLv3()
{
	Init();
	m_pInfo	=(MSG_Info* )m_bufMsg;
}

CMsgVasLv3::~CMsgVasLv3()
{
	
}

BOOL CMsgVasLv3::Create(unsigned short usAction,OBJID id,DWORD vastotal,DWORD lvtotal,DWORD dwcount,DWORD vaslv,DWORD awarditem,DWORD viscou,DWORD maxvis,DWORD vascount,OBJID questionid,DWORD questionan)
{
	if(usAction == ACTION_NONE_)
	{
		ASSERT(!"CMsgVasLv3::Create()");
		return false;
	}
	
	this->Init();
	
	m_pInfo->usAction	=	usAction;
	m_pInfo->id			=	id;
	m_pInfo->vastotal	=	vastotal;
	m_pInfo->lvtotal	=	lvtotal;
	m_pInfo->dwcount	=	dwcount;
	m_pInfo->vaslv		=	vaslv;
	m_pInfo->awarditem	=	awarditem;
	
	m_pInfo->viscou		=	viscou;
	m_pInfo->maxvis		=	maxvis;
	m_pInfo->vascount	=	vascount;
	m_pInfo->questionid	=	questionid;
	m_pInfo->questionan	=	questionan;
	
	m_unMsgType	=_MSG_VASLV;
	m_unMsgSize	=sizeof(MSG_Info);
	
	return true;
}

CMsgVasLv1::CMsgVasLv1()
{
	Init();
	m_pInfo	=(MSG_Info* )m_bufMsg;
}

CMsgVasLv1::~CMsgVasLv1()
{
	
}

BOOL CMsgVasLv1::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if(!pbufMsg)
		return false;
	Init();
	memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
	return true;
}

BOOL CMsgVasLv1::Create(unsigned short usAction,OBJID id,DWORD vastotal,DWORD lvtotal,DWORD dwcount,DWORD vaslv,DWORD awarditem,DWORD viscou,DWORD maxvis,DWORD vascount,OBJID questionid,DWORD questionan)
{
	if(usAction == ACTION_NONE_)
	{
		ASSERT(!"CMsgVasLv1::Create()");
		return false;
	}
	
	this->Init();
	
	m_pInfo->usAction	=	usAction;
	m_pInfo->id			=	id;
	m_pInfo->vastotal	=	vastotal;
	m_pInfo->lvtotal	=	lvtotal;
	m_pInfo->dwcount	=	dwcount;
	m_pInfo->vaslv		=	vaslv;
	m_pInfo->awarditem	=	awarditem;

	m_pInfo->viscou		=	viscou;
	m_pInfo->maxvis		=	maxvis;
	m_pInfo->vascount	=	vascount;
	m_pInfo->questionid	=	questionid;
	m_pInfo->questionan	=	questionan;
	
	m_unMsgType	= m_pInfo->unMsgType = _MSG_VASLV;
	m_unMsgSize	= m_pInfo->unMsgSize = sizeof(MSG_Info);
	
	return true;
}

BOOL CMsgVasLv1::Create(unsigned short usAction,OBJID id,DWORD vastotal,DWORD lvtotal,DWORD dwcount,DWORD vaslv,DWORD awarditem)
{
	if(usAction == ACTION_NONE_)
	{
		ASSERT(!"CMsgVasLv1::Create()");
		return false;
	}
	
	this->Init();
	
	m_pInfo->usAction	=	usAction;
	m_pInfo->id			=	id;
	m_pInfo->vastotal	=	vastotal;
	m_pInfo->lvtotal	=	lvtotal;
	m_pInfo->dwcount	=	dwcount;
	m_pInfo->vaslv		=	vaslv;
	m_pInfo->awarditem	=	awarditem;
	
	m_unMsgType	= m_pInfo->unMsgType = _MSG_VASLV;
	m_unMsgSize	= m_pInfo->unMsgSize = sizeof(MSG_Info);
	
	return true;
}

BOOL CMsgVasLv1::Create(unsigned short usAction,OBJID id,DWORD viscou,DWORD maxvis,DWORD vascount,OBJID questionid)
{
	if(usAction == ACTION_NONE_)
	{
		ASSERT(!"CMsgVasLv1::Create()");
		return false;
	}
	
	this->Init();
	
	m_pInfo->usAction	=	usAction;
	m_pInfo->id			=	id;
	m_pInfo->viscou		=	viscou;
	m_pInfo->maxvis		=	maxvis;
	m_pInfo->vascount	=	vascount;
	m_pInfo->questionid	=	questionid;
	
	m_unMsgType	= m_pInfo->unMsgType = _MSG_VASLV;
	m_unMsgSize	= m_pInfo->unMsgSize = sizeof(MSG_Info);
	
	return true;
}

BOOL CMsgVasLv1::Create(unsigned short usAction,OBJID id,OBJID questionid,DWORD questionan)
{
	if(usAction == ACTION_NONE_)
	{
		ASSERT(!"CMsgVasLv1::Create()");
		return false;
	}
	
	this->Init();
	
	m_pInfo->usAction	=	usAction;
	m_pInfo->id			=	id;
	m_pInfo->questionid	=	questionid;
	m_pInfo->questionan	=	questionan;
	
	m_unMsgType	= m_pInfo->unMsgType = _MSG_VASLV;
	m_unMsgSize	= m_pInfo->unMsgSize = sizeof(MSG_Info);
	
	return true;
}

void CMsgVasLv1::Process(void* pbufMsg)
{
	DEBUG_TRY
	switch(m_pInfo->usAction)
	{
	case ACTION_QUERYVASLVTOSER:
		break;
	case ACTION_QUERYVASLVTOACC:
		GameWorld()->SendVASLVRequest(m_pInfo->unMsgType,m_pInfo->usAction,m_pInfo->id,m_pInfo->vastotal,m_pInfo->lvtotal,m_pInfo->dwcount,m_pInfo->vaslv,m_pInfo->awarditem,m_pInfo->viscou,m_pInfo->maxvis,m_pInfo->vascount,m_pInfo->questionid,m_pInfo->questionan);
		break;
	case ACTION_QUERYVASLVTOMSG:
		{
			CMsgVasLv3 MsgVASLv;
			CPlayer* pReceiver = UserList()->GetPlayer(m_pInfo->id);
			if( pReceiver )
			{
				int nMapGroup = pReceiver->GetProcessID();
				if (MsgVASLv.Create(ACTION_QUERYVASLVTOMSG,m_pInfo->id,m_pInfo->vastotal,m_pInfo->lvtotal,m_pInfo->dwcount,m_pInfo->vaslv,m_pInfo->awarditem,m_pInfo->viscou,m_pInfo->maxvis,m_pInfo->vascount,m_pInfo->questionid,m_pInfo->questionan))
					GameWorld()->TransmitMapGroupMsg(nMapGroup,&MsgVASLv);
			}
		}
		break;
	case ACTION_QUERYVASLVTOCLI:
		break;
	case ACTION_LOGINUPDATE:
		GameWorld()->SendVASLVRequest(m_pInfo->unMsgType,m_pInfo->usAction,m_pInfo->id,m_pInfo->vastotal,m_pInfo->lvtotal,m_pInfo->dwcount,m_pInfo->vaslv,m_pInfo->awarditem,m_pInfo->viscou,m_pInfo->maxvis,m_pInfo->vascount,m_pInfo->questionid,m_pInfo->questionan);
		break;
	case ACTION_UNEQU3:
		{
			CPlayer* pReceiver = UserList()->GetPlayer(m_pInfo->id);
			if( pReceiver )
			{
				CMsgVasLv3 MsgVASLv;
				int nMapGroup = pReceiver->GetProcessID();
				if (MsgVASLv.Create(ACTION_UNEQU3,m_pInfo->id,m_pInfo->vastotal,m_pInfo->lvtotal,m_pInfo->dwcount,m_pInfo->vaslv,m_pInfo->awarditem,m_pInfo->viscou,m_pInfo->maxvis,m_pInfo->vascount,m_pInfo->questionid,m_pInfo->questionan))
					GameWorld()->TransmitMapGroupMsg(nMapGroup,&MsgVASLv);
			}
			else
			{//玩家不在线啦做个记录
				::LogSave("CMSGVASLV1 ACTION_UNEQU3玩家不在线啦做个记录vastotal[%d]vaslv[%d]awarditem[%d]",m_pInfo->vastotal,m_pInfo->vaslv,m_pInfo->awarditem);
			}
		}
		break;
	case ACTION_UNEQU1:
		break;
	case ACTION_GETITEM:
		break;
	case ACTION_UNEQU6:
		GameWorld()->SendVASLVRequest(m_pInfo->unMsgType,m_pInfo->usAction,m_pInfo->id,m_pInfo->vastotal,m_pInfo->lvtotal,m_pInfo->dwcount,m_pInfo->vaslv,m_pInfo->awarditem,m_pInfo->viscou,m_pInfo->maxvis,m_pInfo->vascount,m_pInfo->questionid,m_pInfo->questionan);
		break;
	case ACTION_REDUCEVAS:
		GameWorld()->SendVASLVRequest(m_pInfo->unMsgType,m_pInfo->usAction,m_pInfo->id,m_pInfo->vastotal,m_pInfo->lvtotal,m_pInfo->dwcount,m_pInfo->vaslv,m_pInfo->awarditem,m_pInfo->viscou,m_pInfo->maxvis,m_pInfo->vascount,m_pInfo->questionid,m_pInfo->questionan);
		break;
	case ACTION_REDUCEVASUPDATE:
		{
			CPlayer* pReceiver = UserList()->GetPlayer(m_pInfo->id);
			if( pReceiver )
			{
				CMsgVasLv3 MsgVASLv;
				int nMapGroup = pReceiver->GetProcessID();
				if (MsgVASLv.Create(ACTION_REDUCEVASUPDATE,m_pInfo->id,m_pInfo->vastotal,m_pInfo->lvtotal,m_pInfo->dwcount,m_pInfo->vaslv,m_pInfo->awarditem,m_pInfo->viscou,m_pInfo->maxvis,m_pInfo->vascount,m_pInfo->questionid,m_pInfo->questionan))
					GameWorld()->TransmitMapGroupMsg(nMapGroup,&MsgVASLv);
			}
			else
			{//玩家不在线啦做个记录
				::LogSave("CMSGVASLV1 ACTION_REDUCEVASUPDATE玩家不在线啦做个记录[%d]vascount[%d]",m_pInfo->vastotal,m_pInfo->vascount);
			}
		}
		break;
	default:break;
	}
	DEBUG_CATCH("CMsgVasLv1::Process exit")
}