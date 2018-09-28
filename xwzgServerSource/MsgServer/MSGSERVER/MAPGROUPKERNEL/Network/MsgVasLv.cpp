#include "MsgVasLv.h"
#include "MapGroup.h"

CMsgVasLv::CMsgVasLv()
{
	Init();
	m_pInfo	=(MSG_Info* )m_bufMsg;
}

CMsgVasLv::~CMsgVasLv()
{
	
}

BOOL CMsgVasLv::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;
	
	if(_MSG_VAS != this->GetType())
		return false;
	
	return true;
}

BOOL CMsgVasLv::Create(unsigned short usAction,OBJID id,DWORD vastotal,DWORD lvtotal,DWORD dwcount,DWORD vaslv,DWORD awarditem,DWORD viscou,DWORD maxvis,DWORD vascount,OBJID questionid,DWORD questionan)
{
	if(usAction == ACTION_NONE_)
	{
		ASSERT(!"CMsgVasLv::Create()");
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

BOOL CMsgVasLv::Create(unsigned short usAction,OBJID id,DWORD vastotal,DWORD lvtotal,DWORD dwcount,DWORD vaslv,DWORD awarditem)
{
	if(usAction == ACTION_NONE_)
	{
		ASSERT(!"CMsgVasLv::Create()");
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
	
	m_unMsgType	=_MSG_VASLV;
	m_unMsgSize	=sizeof(MSG_Info);
	
	return true;
}

BOOL CMsgVasLv::Create(unsigned short usAction,OBJID id,DWORD viscou,DWORD maxvis,DWORD vascount,OBJID questionid)
{
	if(usAction == ACTION_NONE_)
	{
		ASSERT(!"CMsgVasLv::Create()");
		return false;
	}
	
	this->Init();
	
	m_pInfo->usAction	=	usAction;
	m_pInfo->id			=	id;
	m_pInfo->viscou		=	viscou;
	m_pInfo->maxvis		=	maxvis;
	m_pInfo->vascount	=	vascount;
	m_pInfo->questionid	=	questionid;
	
	m_unMsgType	=_MSG_VASLV;
	m_unMsgSize	=sizeof(MSG_Info);
	
	return true;
}

BOOL CMsgVasLv::Create(unsigned short usAction,OBJID id,OBJID questionid,DWORD questionan)
{
	if(usAction == ACTION_NONE_)
	{
		ASSERT(!"CMsgVasLv::Create()");
		return false;
	}
	
	this->Init();
	
	m_pInfo->usAction	=	usAction;
	m_pInfo->id			=	id;
	m_pInfo->questionid	=	questionid;
	m_pInfo->questionan	=	questionan;
	
	m_unMsgType	=_MSG_VASLV;
	m_unMsgSize	=sizeof(MSG_Info);
	
	return true;
}

void CMsgVasLv::Process(void* pbufMsg)
{
	DEBUG_TRY
	switch(m_pInfo->usAction)
	{
	case ACTION_QUERYVASLVTOSER:
		{
			CUserPtr pUser	= UserManager()->GetUser(m_pInfo->id);
			if( pUser )
			{
				CMsgVasLv2  MsgVASLv2;
				if( MsgVASLv2.Create(ACTION_QUERYVASLVTOACC,m_pInfo->id,0,0,0,0,0,0,0,0,pUser->GetAccountID(),0) )
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVASLv2 );
			}
		}
		break;
	case ACTION_QUERYVASLVTOACC:
		break;
	case ACTION_QUERYVASLVTOMSG:
		{
			CUserPtr pUser = UserManager()->GetUser(m_pInfo->id);
			if( pUser )
			{
				CVasLvData *pRes = pUser->QueryVaslvData();;
				int awarditem = 0;
				if( pRes )
				{
					awarditem = pRes->GetInt(VASLVDATA_AWARDITEM);
					
					if( m_pInfo->awarditem > 0 )
					{
						int maxvis = pRes->GetInt(VASLVDATA_MAXVIS);
						char sqlstr[128] = {0};
						awarditem = awarditem | m_pInfo->awarditem;
						if( (m_pInfo->awarditem & 512) )
							maxvis = 30;
						else if( (m_pInfo->awarditem & 256) )
							maxvis = 25;
						else if( (m_pInfo->awarditem & 128) )
							maxvis = 20;
						else if( (m_pInfo->awarditem & 64) )
							maxvis = 16;
						else if( (m_pInfo->awarditem & 32) )
							maxvis = 12;
						else if( (m_pInfo->awarditem & 16) )
							maxvis = 9;
						else if( (m_pInfo->awarditem & 8) )
							maxvis = 6;
						else if( (m_pInfo->awarditem & 4) )
							maxvis = 4;
						else if( (m_pInfo->awarditem & 2) )
							maxvis = 2;
						else if( (m_pInfo->awarditem & 1) )
							maxvis = 1;

						pRes->SetInt(VASLVDATA_AWARDITEM,awarditem);
						pRes->SetInt(VASLVDATA_MAXVIS,maxvis);
					}
					
					CMsgVasLv2 MsgVASLv;
					if (MsgVASLv.Create(ACTION_QUERYVASLVTOCLI,m_pInfo->id,m_pInfo->vastotal,m_pInfo->lvtotal,m_pInfo->dwcount,m_pInfo->vaslv,awarditem,0,0,0,0,m_pInfo->questionan))
						pUser->SendMsg(&MsgVASLv);
				}
				else
				{//数据库操作失败
					::LogSave("CMSGVASLV1 ACTION_QUERYVASLVTOMSG插入数据库操作失败");
				}
			}
			else
			{//玩家不在线啦做个记录
				::LogSave("CMSGVASLV1 ACTION_QUERYVASLVTOMSG玩家不在线啦做个记录");
			}
		}
		break;
	case ACTION_QUERYVASLVTOCLI:
		break;
	case ACTION_GETITEM:
		{
			CUserPtr pUser	= UserManager()->GetUser(m_pInfo->id);
			if( pUser )
			{
				CVasLvData *pRes = pUser->QueryVaslvData();;
				if( pRes )
				{
					bool flag = false;
					int awitem = 0;
					if( m_pInfo->awarditem >0 && m_pInfo->awarditem < 11 )
					{
						flag = true;
						if( 1 == m_pInfo->awarditem )
							awitem = 1;
						else if( 2 == m_pInfo->awarditem )
							awitem = 2;
						else if( 3 == m_pInfo->awarditem )
							awitem = 4;
						else if( 4 == m_pInfo->awarditem )
							awitem = 8;
						else if( 5 == m_pInfo->awarditem )
							awitem = 16;
						else if( 6 == m_pInfo->awarditem )
							awitem = 32;
						else if( 7 == m_pInfo->awarditem )
							awitem = 64;
						else if( 8 == m_pInfo->awarditem )
							awitem = 128;
						else if( 9 == m_pInfo->awarditem )
							awitem = 256;
						else if( 10 == m_pInfo->awarditem )
							awitem = 512;
					}
					int awarditem = pRes->GetInt(VASLVDATA_AWARDITEM);
					if( flag && (awitem & awarditem) )
					{
						//调用脚本,脚本范围4010000-4019999
						pUser->SetVasLvid(m_pInfo->awarditem);
						if( pUser->ProcessAction(4010001) )
						{
							pRes->SetInt(VASLVDATA_AWARDITEM,~((~awarditem)|awitem));
							if( m_pInfo->awarditem >= 3 && m_pInfo->awarditem <= 10 )
							{
								char dchar[126] = {0};
								OBJID item_id = 0;
								if( 3 == m_pInfo->awarditem )
									item_id = 500112;
								else if( 4 == m_pInfo->awarditem )
									item_id = 500113;
								else if( 5 == m_pInfo->awarditem )
									item_id = 500114;
								else if( 6 == m_pInfo->awarditem )
									item_id = 500115;
								else if( 7 == m_pInfo->awarditem )
									item_id = 500116;
								else if( 8 == m_pInfo->awarditem )
									item_id = 500117;
								else if( 9 == m_pInfo->awarditem )
									item_id = 500118;
								else if( 10 == m_pInfo->awarditem )
									item_id = 500119;
								CItem* pItem = pUser->GetItemByType(item_id);
								if( NULL != pItem )
									pUser->IgotAnOrangeWeapen(item_id, pItem);
							}
						}
						//回发信息
						CMsgVasLv2  MsgVASLv2;
						if( MsgVASLv2.Create(ACTION_QUERYVASLVTOACC,m_pInfo->id,0,0,0,0,0,0,0,0,pUser->GetAccountID(),0) )
							MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVASLv2 );
					}
					else
					{
						;//不能领取奖励
					}
				}
			}
		}
		break;
	case ACTION_QUERYVISITTOSER:
		{
			CUserPtr pUser	= UserManager()->GetUser(m_pInfo->id);
			if( pUser )
			{
				CVasLvData *pRes = pUser->QueryVaslvData();;
				if( pRes )
				{
					int viscou = pRes->GetInt(VASLVDATA_VISCOU);
					int maxvis = pRes->GetInt(VASLVDATA_MAXVIS);
					int vascount = pRes->GetInt(VASLVDATA_VASCOUNT);
					int questionid = pRes->GetInt(VASLVDATA_QUESTIONID);
					CMsgVasLv2 msgvaslv;
					vascount = vascount + 1;
					if( 0 == questionid )
						questionid = -1;
					if( msgvaslv.Create(ACTION_QUERYVISITTOCLI,m_pInfo->id,maxvis - viscou,maxvis,10 * vascount - 5,questionid) )
						pUser->SendMsg(&msgvaslv);
				}
			}
		}
		break;
	case ACTION_VISITFREETOSER:
		{
			CUserPtr pUser	= UserManager()->GetUser(m_pInfo->id);
			if( pUser )
			{
				CVasLvData *pRes = pUser->QueryVaslvData();;
				if( pRes )
				{
					int viscou = pRes->GetInt(VASLVDATA_VISCOU);
					int maxvis = pRes->GetInt(VASLVDATA_MAXVIS);
					int vascount = pRes->GetInt(VASLVDATA_VASCOUNT);
					int questionid = pRes->GetInt(VASLVDATA_QUESTIONID);
					if( viscou < maxvis )
					{
// 						char sqlstr[128];
						viscou = viscou + 1;
						srand(time(NULL));//调用脚本产生随机数
						if( rand()%101 > 65 )
							questionid = rand()%101;
						else
							questionid = 0;
						pRes->SetInt(VASLVDATA_VISCOU,viscou);
						pRes->SetInt(VASLVDATA_QUESTIONID,questionid);
						pUser->ProcessAction(4010002);
					}
					
					CMsgVasLv2 msgvaslv;
					vascount = vascount + 1;
					if( msgvaslv.Create(ACTION_QUERYVISITTOCLI,m_pInfo->id,maxvis - viscou,maxvis,10 * vascount - 5,questionid) )
						pUser->SendMsg(&msgvaslv);
				}
			}
		}
		break;
	case ACTION_VISITVASTOSER:
		{
			CUserPtr pUser	= UserManager()->GetUser(m_pInfo->id);
			if( pUser )
			{
				CVasLvData *pRes = pUser->QueryVaslvData();;
				if( pRes )
				{
					int viscou = pRes->GetInt(VASLVDATA_VISCOU);
					int maxvis = pRes->GetInt(VASLVDATA_MAXVIS);
					int vascount = pRes->GetInt(VASLVDATA_VASCOUNT);
					int questionid = pRes->GetInt(VASLVDATA_QUESTIONID);
					
					vascount = vascount + 1;
					if( questionid > 0 )
					{
						pUser->SendSysMsg("您还没有答完题目");
						CMsgVasLv2 msgvaslv;
						if( msgvaslv.Create(ACTION_QUERYVISITTOCLI,m_pInfo->id,maxvis - viscou,maxvis,10 * vascount - 5,questionid) )
							pUser->SendMsg(&msgvaslv);
					}
					else if( (pUser->GetMoneyByType(5) >= (10 * vascount - 5)) )
					{
						if( (10 * vascount - 5) > 2000 )
						{
							pUser->SendSysMsg("您太过频繁操作，明天5点后再来操作吧！");
							CMsgVasLv2 msgvaslv;
							if( msgvaslv.Create(ACTION_QUERYVISITTOCLI,m_pInfo->id,maxvis - viscou,maxvis,10 * vascount - 5,-1) )
								pUser->SendMsg(&msgvaslv);
						}
						else
						{
							CMsgVasLv2  MsgVASLv2;
							if( MsgVASLv2.Create(ACTION_REDUCEVAS,m_pInfo->id,10 * vascount - 5,0,0,0,0,0,0,0,pUser->GetAccountID(),0) )
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVASLv2 );
						}
					}
					else
					{
						pUser->SendSysMsg("您没有足够的元宝");
						CMsgVasLv2 msgvaslv;
						if( msgvaslv.Create(ACTION_QUERYVISITTOCLI,m_pInfo->id,maxvis - viscou,maxvis,10 * vascount - 5,-1) )
							pUser->SendMsg(&msgvaslv);
					}
				}
			}
		}
		break;
	case ACTION_SENDANSWER:
		{
			CUserPtr pUser	= UserManager()->GetUser(m_pInfo->id);
			if( pUser )
			{//根据题号调用脚本,脚本范围4010000-4019999
//				int qid = 4010000;
				if( m_pInfo->questionid > 0 && m_pInfo->questionid < 200 )
				{
//					qid = qid + m_pInfo->questionid;
					CVasLvData *pRes = pUser->QueryVaslvData();;
					if( pRes )
					{
						if( pRes->GetInt(VASLVDATA_QUESTIONID) == m_pInfo->questionid )
						{
							pUser->SetVasLvQuestionAnswer(m_pInfo->questionan,m_pInfo->questionid);
							if( pUser->ProcessAction(4010000) )
								pRes->SetInt(VASLVDATA_QUESTIONID,0);
						}
						int viscou = pRes->GetInt(VASLVDATA_VISCOU);
						int maxvis = pRes->GetInt(VASLVDATA_MAXVIS);
						int vascount = pRes->GetInt(VASLVDATA_VASCOUNT);
						int questionid = pRes->GetInt(VASLVDATA_QUESTIONID);
						CMsgVasLv2 msgvaslv;
						if( 0 == questionid )
							questionid = -2;
						vascount = vascount + 1;
						if( msgvaslv.Create(ACTION_QUERYVISITTOCLI,m_pInfo->id,maxvis - viscou,maxvis,10 * vascount - 5,questionid) )
							pUser->SendMsg(&msgvaslv);
					}
				}
			}
			else
			{//没有给角色奖励做个记录
				::LogSave("user=%u ACTION_SENDANSWER FAIL questionid[%d] [%d]", m_pInfo->id,m_pInfo->questionid,m_pInfo->questionan);
			}
		}
		break;
	case ACTION_UNEQU6:
		{
			CUserPtr pUser	= UserManager()->GetUser(m_pInfo->id);
			if( pUser )
			{
				pUser->ProcessAction(4010003);
				CMsgVasLv2  MsgVASLv2;
				if( MsgVASLv2.Create(ACTION_UNEQU6,m_pInfo->id,0,0,0,0,0,0,0,0,pUser->GetAccountID(),0) )
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &MsgVASLv2 );
			}
		}
		break;
	case ACTION_REDUCEVASUPDATE:
		{
			CUserPtr pUser	= UserManager()->GetUser(m_pInfo->id);
			if( pUser )
			{
				CVasLvData *pRes = pUser->QueryVaslvData();;
				if( pRes )
				{
					int viscou = pRes->GetInt(VASLVDATA_VISCOU);
					int maxvis = pRes->GetInt(VASLVDATA_MAXVIS);
					int vascount = pRes->GetInt(VASLVDATA_VASCOUNT);
					int questionid = pRes->GetInt(VASLVDATA_QUESTIONID);
					
// 					char sqlstr[128];
					vascount = vascount + 1;
					if( m_pInfo->questionan )
					{//
						srand(time(NULL));//调用脚本产生随机数
						if( rand()%101 > 65 )
							questionid = rand()%101;
						else
							questionid = 0;
						pRes->SetInt(VASLVDATA_VASCOUNT,vascount);
						pRes->SetInt(VASLVDATA_QUESTIONID,questionid);
						pUser->ProcessAction(4010002);
						vascount += 1;
					}
					CMsgVasLv2 msgvaslv;
					if( msgvaslv.Create(ACTION_QUERYVISITTOCLI,m_pInfo->id,maxvis - viscou,maxvis,10 * vascount - 5,questionid) )
						pUser->SendMsg(&msgvaslv);
				}
			}
			else
			{//没有给角色奖励做个记录
				::LogSave("user=%u MsgVasLv ACTION_REDUCEVASUPDATE Get CUserPtr  FAIL [%d] [%d]", m_pInfo->id,m_pInfo->vastotal,m_pInfo->questionan);
			}
		}
		break;
	case ACTION_UNEQU3:
		{
			CUserPtr pUser	= UserManager()->GetUser(m_pInfo->id);
			if( pUser )
			{
				CVasLvData *pRes = pUser->QueryVaslvData();;
				int awarditem = 0;
				if( pRes )
				{
					awarditem = pRes->GetInt(VASLVDATA_AWARDITEM);
					
					if( m_pInfo->awarditem > 0 )
					{
						int maxvis = pRes->GetInt(VASLVDATA_MAXVIS);
						char sqlstr[128] = {0};
						awarditem = awarditem | m_pInfo->awarditem;
						if( (m_pInfo->awarditem & 512) )
							maxvis = 30;
						else if( (m_pInfo->awarditem & 256) )
							maxvis = 25;
						else if( (m_pInfo->awarditem & 128) )
							maxvis = 20;
						else if( (m_pInfo->awarditem & 64) )
							maxvis = 16;
						else if( (m_pInfo->awarditem & 32) )
							maxvis = 12;
						else if( (m_pInfo->awarditem & 16) )
							maxvis = 9;
						else if( (m_pInfo->awarditem & 8) )
							maxvis = 6;
						else if( (m_pInfo->awarditem & 4) )
							maxvis = 4;
						else if( (m_pInfo->awarditem & 2) )
							maxvis = 2;
						else if( (m_pInfo->awarditem & 1) )
							maxvis = 1;
						pRes->SetInt(VASLVDATA_AWARDITEM,awarditem);
						pRes->SetInt(VASLVDATA_MAXVIS,maxvis);
					}
				}
			}
			else
			{
				::LogSave("user=%u CMsgVasLv ACTION_UNEQU3 FAIL awarditem[%d]", m_pInfo->id,m_pInfo->awarditem);
			}
		}
		break;
	default:break;
	}
	DEBUG_CATCH("CMsgVasLv::Process exit")
}

CMsgVasLv2::CMsgVasLv2()
{
	Init();
	m_pInfo	=(MSG_Info* )m_bufMsg;
}

CMsgVasLv2::~CMsgVasLv2()
{
	
}

BOOL CMsgVasLv2::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;
	
	if(_MSG_VAS != this->GetType())
		return false;
	
	return true;
}

BOOL CMsgVasLv2::Create(unsigned short usAction,OBJID id,DWORD vastotal,DWORD lvtotal,DWORD dwcount,DWORD vaslv,DWORD awarditem,DWORD viscou,DWORD maxvis,DWORD vascount,OBJID questionid,DWORD questionan)
{
	if(usAction == ACTION_NONE_)
	{
		ASSERT(!"CMsgVasLv2::Create()");
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

BOOL CMsgVasLv2::Create(unsigned short usAction,OBJID id,DWORD viscou,DWORD maxvis,DWORD vascount,OBJID questionid)
{
	if(usAction == ACTION_NONE_)
	{
		ASSERT(!"CMsgVasLv::Create()");
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