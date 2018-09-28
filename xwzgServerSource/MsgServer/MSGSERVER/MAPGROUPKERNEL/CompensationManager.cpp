// CompensationManager.cpp: implementation of the CCompensationManager class.
//
//////////////////////////////////////////////////////////////////////
#include <afx.h>
#include "MapGroup.h"
#include "CompensationManager.h"
#include "AllMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char	szCompensationTable[]	= _TBL_CPSATION;
char	szCompensationMsgTable[]	= _TBL_CPSATIONMSG;

CCompensationManager::CCompensationManager()
{
	m_setCompensation.clear();
	m_setCompensationMsg.clear();
	m_pOwner = NULL;
}

CCompensationManager::~CCompensationManager()
{
	for(list<CCompensation*>::iterator it = m_setCompensation.begin(); it != m_setCompensation.end(); it ++)
	{
		CCompensation* pUserData = *it;
		SAFE_RELEASE(pUserData);;
	}
	m_setCompensation.clear();

	for(list<CCompensationMsg*>::iterator itMsg = m_setCompensationMsg.begin(); itMsg != m_setCompensationMsg.end(); itMsg ++)
	{
		CCompensationMsg* pUserData = *itMsg;
		SAFE_RELEASE(pUserData);;
	}
	m_setCompensationMsg.clear();
}

bool CCompensationManager::Create (IDatabase* pDb,  PROCESS_ID idProcess,CUser* pUser)
{
	CHECKF(pDb);
	CHECKF(pUser);

	m_pDB = pDb;
	m_idProcess = idProcess;
	m_pOwner = pUser;
	
	//wup
	char szSQL[1024];
	sprintf(szSQL, "SELECT * FROM %s WHERE owner_id = %u", _TBL_CPSATION, pUser->GetID());
	IRecordset* pRes = m_pDB->CreateNewRecordset(szSQL, false);
	if(pRes)
	{
		for(int i = 0; i < pRes->RecordCount(); i++)
		{
			CCompensation *pCompensation = CCompensation::CreateNew();
			IF_OK(pCompensation)
			{
				if(pCompensation->Create(pRes))
				{
					m_setCompensation.push_back(pCompensation);
				}
				else
					pCompensation->Release();
			}
			pRes->MoveNext();
		}
		pRes->Release();
	}

	sprintf(szSQL, "SELECT * FROM %s WHERE owner_id = %u order by id DESC", _TBL_CPSATIONMSG, pUser->GetID());
	pRes = m_pDB->CreateNewRecordset(szSQL, false);
	if(pRes)
	{
		for(int i = 0; i < pRes->RecordCount(); i++)
		{
			CCompensationMsg *pCompensationMsg = CCompensationMsg::CreateNew();
			IF_OK(pCompensationMsg)
			{
				if(pCompensationMsg->Create(pRes))
				{
					m_setCompensationMsg.push_back(pCompensationMsg);
				}
				else
					pCompensationMsg->Release();
			}
			pRes->MoveNext();
		}
		pRes->Release();
	}
	return true;
}

void CCompensationManager::AddMsg(OBJID idUser,int nType,const char* szTime,const char* szMsg,bool bSendMail/* = false*/)
{
	CHECK(m_pOwner);
	CHECK(szTime);
	CHECK(szMsg);

	//先看消息满了没
	if(m_setCompensationMsg.size() >= 20)
	{
		int nCount = 0;
		for(list<CCompensationMsg*>::iterator it = m_setCompensationMsg.begin(); it != m_setCompensationMsg.end() ; it ++ )
		{
			nCount ++;
			if(nCount >= 20)
			{
				CCompensationMsg* pMsg = *it;
				if(pMsg)
				{
					pMsg->Destroy();
					SAFE_RELEASE(pMsg);
				}
				m_setCompensationMsg.erase(it);
				break;
			}
		}
	}

	CCompensationMsg *pCompensationMsg = CCompensationMsg::CreateNew();
	IF_OK(pCompensationMsg)
	{
		if(pCompensationMsg->Create(GameDataDefault()->GetCPSMData(),idUser,nType,szMsg,szTime))
		{
			if(bSendMail)
			{
				char szTemp[1024] = "";
				sprintf(szTemp,"%s,%s",szTime,szMsg);
				m_pOwner->SendEmail(m_pOwner->GetName(), szMsg, m_pOwner);
			}
			m_setCompensationMsg.push_front(pCompensationMsg);
		}
		else
			pCompensationMsg->Release();
	}
}

void CCompensationManager::ClearAllThing()
{
	CHECK(m_pOwner);
	for(list<CCompensation*>::iterator it = m_setCompensation.begin(); it != m_setCompensation.end() ; it ++ )
	{
		CCompensation* pData = *it;
		if(pData)
		{
			pData->Destroy();
			SAFE_RELEASE(pData);
		}
	}
	m_setCompensation.clear();

	CTime tNowCheck(CTime::GetCurrentTime());
	AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_TIMEMSG,tNowCheck.Format("%Y-%m-%d %H:%M:%S"),"你超过7天没有领取补偿奖励，补偿仓库已被清空.",true);
}

void CCompensationManager::UpdateAll()//上线整理
{
	CHECK(m_pOwner);
	char szTemp[1024] = "";
	//先把没处理的处理
	bool bDo = false;
	int nLastTime = 24 * 10;
	for(list<CCompensation*>::iterator it = m_setCompensation.begin(); it != m_setCompensation.end() ; it ++ )
	{
		CCompensation* pData = *it;
		if(pData)
		{
			if(pData->GetInt(COMPENSATIONDATA_STATUS) == COMPENSATIONSTATUS_NEW)
			{
				int nTypeOp = pData->GetInt(COMPENSATIONDATA_TYPE);
				int nTypeId = pData->GetInt(COMPENSATIONDATA_TYPEID);
				switch(nTypeOp)
				{
				case COMPENSATIONTYPE_ITEM:
					{
						ItemInfoStruct stInfo;
						if(!ItemType()->GetInfo(nTypeId,&stInfo))
							break;
						
						sprintf(szTemp,"你获得了官方奖励补偿:%s x %d。",stInfo.szName,pData->GetInt(COMPENSATIONDATA_NUM));
						AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_OPMSG,pData->GetStr(COMPENSATIONDATA_WRITETIME),szTemp);

						sprintf(szTemp,"%s,你获得了官方奖励补偿:%s x %d,请前往江陵礼物仙子(62,99)补偿仓库领取。",pData->GetStr(COMPENSATIONDATA_WRITETIME),stInfo.szName,pData->GetInt(COMPENSATIONDATA_NUM));
						m_pOwner->SendEmail(m_pOwner->GetName(), szTemp, m_pOwner);

						pData->SetInt(COMPENSATIONDATA_STATUS,COMPENSATIONSTATUS_WAITE);
						nLastTime = min(nLastTime,GetCurrentTimePass(pData->GetStr(COMPENSATIONDATA_WRITETIME)));
						bDo = true;
					}
					break;
				case COMPENSATIONTYPE_EUDEMON:
					{
						CEudemonTypeData*	pEudemonType = EudemonType()->GetObj(nTypeId);
						if(!pEudemonType)
							break;
						
						sprintf(szTemp,"你获得了官方奖励补偿:%s。",pEudemonType->GetStr(EUDEMONTYPEDATA_name));
						AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_OPMSG,pData->GetStr(COMPENSATIONDATA_WRITETIME),szTemp);

						sprintf(szTemp,"%s,你获得了官方奖励补偿:%s,请前往江陵礼物仙子(62,99)补偿仓库领取。",pData->GetStr(COMPENSATIONDATA_WRITETIME),pEudemonType->GetStr(EUDEMONTYPEDATA_name));
						m_pOwner->SendEmail(m_pOwner->GetName(), szTemp, m_pOwner);

						pData->SetInt(COMPENSATIONDATA_STATUS,COMPENSATIONSTATUS_WAITE);
						nLastTime = min(nLastTime,GetCurrentTimePass(pData->GetStr(COMPENSATIONDATA_WRITETIME)));
						bDo = true;
					}
					break;
				case COMPENSATIONTYPE_GOLD1:
					{
						sprintf(szTemp,"你获得了官方奖励补偿:$%d银两$。",pData->GetInt(COMPENSATIONDATA_NUM));
						AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_OPMSG,pData->GetStr(COMPENSATIONDATA_WRITETIME),szTemp);

						sprintf(szTemp,"%s,你获得了官方奖励补偿:%d银两,请前往江陵礼物仙子(62,99)补偿仓库领取。",pData->GetStr(COMPENSATIONDATA_WRITETIME),pData->GetInt(COMPENSATIONDATA_NUM));
						m_pOwner->SendEmail(m_pOwner->GetName(), szTemp, m_pOwner);

						pData->SetInt(COMPENSATIONDATA_STATUS,COMPENSATIONSTATUS_WAITE);
						nLastTime = min(nLastTime,GetCurrentTimePass(pData->GetStr(COMPENSATIONDATA_WRITETIME)));
						bDo = true;
					}
					break;
				case COMPENSATIONTYPE_GOLD2:
					{
						sprintf(szTemp,"你获得了官方奖励补偿:$%d碎银$。",pData->GetInt(COMPENSATIONDATA_NUM));
						AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_OPMSG,pData->GetStr(COMPENSATIONDATA_WRITETIME),szTemp);

						sprintf(szTemp,"%s,你获得了官方奖励补偿:%d碎银,请前往江陵礼物仙子(62,99)补偿仓库领取。",pData->GetStr(COMPENSATIONDATA_WRITETIME),pData->GetInt(COMPENSATIONDATA_NUM));
						m_pOwner->SendEmail(m_pOwner->GetName(), szTemp, m_pOwner);

						pData->SetInt(COMPENSATIONDATA_STATUS,COMPENSATIONSTATUS_WAITE);
						nLastTime = min(nLastTime,GetCurrentTimePass(pData->GetStr(COMPENSATIONDATA_WRITETIME)));
						bDo = true;
					}
					break;
				case COMPENSATIONTYPE_GOLD3:
					{
						sprintf(szTemp,"你获得了官方奖励补偿:$%d珠宝$。",pData->GetInt(COMPENSATIONDATA_NUM));
						AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_OPMSG,pData->GetStr(COMPENSATIONDATA_WRITETIME),szTemp);

						sprintf(szTemp,"%s,你获得了官方奖励补偿:%d珠宝,请前往江陵礼物仙子(62,99)补偿仓库领取。",pData->GetStr(COMPENSATIONDATA_WRITETIME),pData->GetInt(COMPENSATIONDATA_NUM));
						m_pOwner->SendEmail(m_pOwner->GetName(), szTemp, m_pOwner);

						pData->SetInt(COMPENSATIONDATA_STATUS,COMPENSATIONSTATUS_WAITE);
						nLastTime = min(nLastTime,GetCurrentTimePass(pData->GetStr(COMPENSATIONDATA_WRITETIME)));
						bDo = true;
					}
					break;
				case COMPENSATIONTYPE_GOLD5:
					{
						sprintf(szTemp,"你获得了官方奖励补偿:$%d元宝$。",pData->GetInt(COMPENSATIONDATA_NUM));
						AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_OPMSG,pData->GetStr(COMPENSATIONDATA_WRITETIME),szTemp);

						sprintf(szTemp,"%s,你获得了官方奖励补偿:%d元宝,请前往江陵礼物仙子(62,99)补偿仓库领取。",pData->GetStr(COMPENSATIONDATA_WRITETIME),pData->GetInt(COMPENSATIONDATA_NUM));
						m_pOwner->SendEmail(m_pOwner->GetName(), szTemp, m_pOwner);

						pData->SetInt(COMPENSATIONDATA_STATUS,COMPENSATIONSTATUS_WAITE);
						nLastTime = min(nLastTime,GetCurrentTimePass(pData->GetStr(COMPENSATIONDATA_WRITETIME)));
						bDo = true;
					}
					break;
				case COMPENSATIONTYPE_PORFFER:
					{
						sprintf(szTemp,"你获得了官方奖励补偿:%d荣誉。",pData->GetInt(COMPENSATIONDATA_NUM));
						AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_OPMSG,pData->GetStr(COMPENSATIONDATA_WRITETIME),szTemp);

						sprintf(szTemp,"%s,你获得了官方奖励补偿:%d荣誉,请前往江陵礼物仙子(62,99)补偿仓库领取。",pData->GetStr(COMPENSATIONDATA_WRITETIME),pData->GetInt(COMPENSATIONDATA_NUM));
						m_pOwner->SendEmail(m_pOwner->GetName(), szTemp, m_pOwner);

						pData->SetInt(COMPENSATIONDATA_STATUS,COMPENSATIONSTATUS_WAITE);
						nLastTime = min(nLastTime,GetCurrentTimePass(pData->GetStr(COMPENSATIONDATA_WRITETIME)));
						bDo = true;
					}
					break;
				case COMPENSATIONTYPE_SYNPORFFER:
					{
						sprintf(szTemp,"你获得了官方奖励补偿:%d帮贡。",pData->GetInt(COMPENSATIONDATA_NUM));
						AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_OPMSG,pData->GetStr(COMPENSATIONDATA_WRITETIME),szTemp);

						sprintf(szTemp,"%s,你获得了官方奖励补偿:%d帮贡,请前往江陵礼物仙子(62,99)补偿仓库领取。",pData->GetStr(COMPENSATIONDATA_WRITETIME),pData->GetInt(COMPENSATIONDATA_NUM));
						m_pOwner->SendEmail(m_pOwner->GetName(), szTemp, m_pOwner);

						pData->SetInt(COMPENSATIONDATA_STATUS,COMPENSATIONSTATUS_WAITE);
						nLastTime = min(nLastTime,GetCurrentTimePass(pData->GetStr(COMPENSATIONDATA_WRITETIME)));
						bDo = true;
					}
					break;
				case COMPENSATIONTYPE_EXP:
					{
						sprintf(szTemp,"你获得了官方奖励补偿:%d经验。",pData->GetInt(COMPENSATIONDATA_NUM));
						AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_OPMSG,pData->GetStr(COMPENSATIONDATA_WRITETIME),szTemp);

						sprintf(szTemp,"%s,你获得了官方奖励补偿:%d经验,请前往江陵礼物仙子(62,99)补偿仓库领取。",pData->GetStr(COMPENSATIONDATA_WRITETIME),pData->GetInt(COMPENSATIONDATA_NUM));
						m_pOwner->SendEmail(m_pOwner->GetName(), szTemp, m_pOwner);

						pData->SetInt(COMPENSATIONDATA_STATUS,COMPENSATIONSTATUS_WAITE);
						nLastTime = min(nLastTime,GetCurrentTimePass(pData->GetStr(COMPENSATIONDATA_WRITETIME)));
						bDo = true;
					}
					break;
				}
			}
		}
	}
	
	if(bDo)
	{
		int nPassDay = nLastTime / 24;
		if(nPassDay >= 7)
		{
			ClearAllThing();
		}
		else if(nPassDay >= 6)
		{
			CTime tNowCheck(CTime::GetCurrentTime());
			AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_TIMEMSG,tNowCheck.Format("%Y-%m-%d %H:%M:%S"),"补偿仓库内的补偿奖励已存放了6天，将在第1天后清空，请尽快领取.",true);
		}
		else if(nPassDay >= 5)
		{
			CTime tNowCheck(CTime::GetCurrentTime());
			AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_TIMEMSG,tNowCheck.Format("%Y-%m-%d %H:%M:%S"),"补偿仓库内的补偿奖励已存放了5天，将在第2天后清空，请尽快领取.",true);
		}
		else if(nPassDay >= 4)
		{
			CTime tNowCheck(CTime::GetCurrentTime());
			AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_TIMEMSG,tNowCheck.Format("%Y-%m-%d %H:%M:%S"),"补偿仓库内的补偿奖励已存放了4天，将在第3天后清空，请尽快领取.",true);
		}
		return;
	}

	//没有
	if(m_setCompensationMsg.size() < 1)
		return;

	//看最后一次是干嘛
	list<CCompensationMsg*>::iterator itMsg = m_setCompensationMsg.begin();
	CCompensationMsg* pMsg = *itMsg;
	if(pMsg)
	{
		if(pMsg->GetInt(COMPENSATIONMSGDATA_TYPE) == COMPENSATIONMSGTYPE_CLRMSG) //最后是清理掉不用看了
		{

		}
		else if (pMsg->GetInt(COMPENSATIONMSGDATA_TYPE) == COMPENSATIONMSGTYPE_OPMSG) //最后是操作，判断是否过4天
		{
			//看看，还有东西没
			if(m_setCompensation.size() >= 1)
			{
				int nLastOp = GetCurrentTimePass(pMsg->GetStr(COMPENSATIONMSGDATA_WRITETIME));
				int nPassDay = nLastOp / 24;
				if(nPassDay >= 7)
				{
					ClearAllThing();
				}
				else if(nPassDay >= 6)
				{
					CTime tNowCheck(CTime::GetCurrentTime());
					AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_TIMEMSG,tNowCheck.Format("%Y-%m-%d %H:%M:%S"),"补偿仓库内的补偿奖励已存放了6天，将在第1天后清空，请尽快领取.",true);
				}
				else if(nPassDay >= 5)
				{
					CTime tNowCheck(CTime::GetCurrentTime());
					AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_TIMEMSG,tNowCheck.Format("%Y-%m-%d %H:%M:%S"),"补偿仓库内的补偿奖励已存放了5天，将在第2天后清空，请尽快领取.",true);
				}
				else if(nPassDay >= 4)
				{
					CTime tNowCheck(CTime::GetCurrentTime());
					AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_TIMEMSG,tNowCheck.Format("%Y-%m-%d %H:%M:%S"),"补偿仓库内的补偿奖励已存放了4天，将在第3天后清空，请尽快领取.",true);
				}
			}
		}
		else if(pMsg->GetInt(COMPENSATIONMSGDATA_TYPE) == COMPENSATIONMSGTYPE_TIMEMSG)//最后是留言，看是否过了一天
		{
			if(GetCurrentTimePass(pMsg->GetStr(COMPENSATIONMSGDATA_WRITETIME)) >= 24)//过一天了  找最后操作时间
			{
				int nLastOp = 24 * 10;
				for(list<CCompensationMsg*>::iterator itFind = m_setCompensationMsg.begin();itFind != m_setCompensationMsg.end();itFind ++)
				{
					CCompensationMsg* pMsgThis = *itFind;
					if(pMsgThis)
					{
						if(pMsgThis->GetInt(COMPENSATIONMSGDATA_TYPE) == COMPENSATIONMSGTYPE_CLRMSG)
							break;
						if(pMsgThis->GetInt(COMPENSATIONMSGDATA_TYPE) == COMPENSATIONMSGTYPE_OPMSG)
						{
							nLastOp = GetCurrentTimePass(pMsgThis->GetStr(COMPENSATIONMSGDATA_WRITETIME));
							break;
						}
					}
				}

				int nPassDay = nLastOp / 24;
				if(nPassDay >= 7)
				{
					ClearAllThing();
				}
				else if(nPassDay >= 6)
				{
					CTime tNowCheck(CTime::GetCurrentTime());
					AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_TIMEMSG,tNowCheck.Format("%Y-%m-%d %H:%M:%S"),"补偿仓库内的补偿奖励已存放了6天，将在第1天后清空，请尽快领取.",true);
				}
				else if(nPassDay >= 5)
				{
					CTime tNowCheck(CTime::GetCurrentTime());
					AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_TIMEMSG,tNowCheck.Format("%Y-%m-%d %H:%M:%S"),"补偿仓库内的补偿奖励已存放了5天，将在第2天后清空，请尽快领取.",true);
				}
			}
		}
	}
}

int CCompensationManager::GetCurrentTimePass(const char* pText)
{	
	CHECKF(pText);
	int y;	
	int m;
	int d;
	int hh;
	int mm;
	int ss;
	if(sscanf(pText,"%u-%u-%u %u:%u:%u",&y,&m,&d,&hh,&mm,&ss) != 6)
		return 0;

	CTime tCreate(y,m,d,hh,mm,ss,0);
	
// 	int yc = 2011;	
// 	int mc = 7;
// 	int dc = 6;
// 	int hhc = 13;
// 	int mmc = 11;
// 	int ssc = 11;
// 	CTime tNowCheck(yc,mc,dc,hhc,mmc,ssc);
	CTime tNowCheck(CTime::GetCurrentTime());
	CTimeSpan tInterval = tNowCheck - tCreate;
	return tInterval.GetTotalHours();
}

void CCompensationManager::SendCompentationInfo()
{
	CHECK(m_pOwner);
	CMsgCompensation msg;
	msg.Create(COMPOSITONACTION_ADD,0,0,0);

	for(list<CCompensation*>::iterator it = m_setCompensation.begin(); it != m_setCompensation.end() ; it ++ )
	{
		CCompensation* pData = *it;
		if(pData)
		{
			int nType = pData->GetInt(COMPENSATIONDATA_TYPE) * 10000000 + pData->GetInt(COMPENSATIONDATA_TYPEID);
			if(!msg.Add(pData->GetInt(COMPENSATIONDATA_ID),nType,pData->GetInt(COMPENSATIONDATA_NUM)))
				break;
		}
	}
	m_pOwner->SendMsg(&msg);
}


void CCompensationManager::SendCompentationInfoMsg()
{
	CHECK(m_pOwner);
	
	char szTemp[1024] = "";
	for(list<CCompensationMsg*>::iterator it = m_setCompensationMsg.begin(); it != m_setCompensationMsg.end() ; it ++ )
	{
		CCompensationMsg* pMsg = *it;
		if(pMsg)
		{
			sprintf(szTemp,"%s,%s",pMsg->GetStr(COMPENSATIONMSGDATA_WRITETIME),pMsg->GetStr(COMPENSATIONMSGDATA_MSG));
			CMsgCompensationMsg msg;
			msg.Create(COMPOSITONACTION_ADD,szTemp);
			m_pOwner->SendMsg(&msg);
		}
	}
}

bool CCompensationManager::PickCompentation(OBJID id,bool& bOther)
{
	CHECKF(m_pOwner);
	char szTemp[1024] = "";
	bOther = false;
	list<CCompensation*>::iterator it;
	for( it = m_setCompensation.begin(); it != m_setCompensation.end() ; it ++ )
	{
		CCompensation* pData = *it;
		if(pData)
		{
			if(pData->GetInt(COMPENSATIONDATA_ID) == id)
			{
				int nType = pData->GetInt(COMPENSATIONDATA_TYPE);
				if(nType == COMPENSATIONTYPE_ITEM)
				{
					//领物品
					ItemInfoStruct info;
					memset(&info,0,sizeof(ItemInfoStruct));					
					CHECKF(ItemType()->GetInfo(pData->GetInt(COMPENSATIONDATA_TYPEID), &info));

					if (m_pOwner->IsItemFull(0,pData->GetInt(COMPENSATIONDATA_TYPEID),1))
					{
						m_pOwner->SendSysMsg("背包已满");
						return false;
					}

					info.idOwner	= m_pOwner->GetID();
					info.idPlayer	= m_pOwner->GetID();
					//"kidnap quality star addlev itembase1 itembase1 itemAddition1 itemAddition2 itemAddition3 itemAddition4 itemAddition5 itemAddition6 itemAddition7 itemAddition8 itemGem1 itemGem2 itemGem3"
					if(sscanf(pData->GetStr(COMPENSATIONDATA_PARAM),"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",&info.nKidnap,&info.dwQual,&info.dwStr,&info.dwAddLev
						,&info.BasicAddID[0],&info.BasicAddID[1]
						,&info.AdditionAddID[0],&info.AdditionAddID[1],&info.AdditionAddID[2],&info.AdditionAddID[3],&info.AdditionAddID[4],&info.AdditionAddID[5],&info.AdditionAddID[6],&info.AdditionAddID[7]
						,&info.GemTypeID[0],&info.GemTypeID[1],&info.GemTypeID[2]) != 17)
						return false;

					info.dwPerPrice = 0;
					info.nAmount = pData->GetInt(COMPENSATIONDATA_NUM);
					CItem* pItem = m_pOwner->AwardItem(&info,true,true);
					if(!pItem)
						return false;

					CTime tNowCheck(CTime::GetCurrentTime());
					sprintf(szTemp,"你领取了%s x %d.",pItem->GetStr(ITEMDATA_NAME),pData->GetInt(COMPENSATIONDATA_NUM));
					AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_OPMSG,tNowCheck.Format("%Y-%m-%d %H:%M:%S"),szTemp);

					pData->Destroy();
					SAFE_RELEASE(pData);
					m_setCompensation.erase(it);
					return true;
				}
				else if(nType == COMPENSATIONTYPE_EUDEMON)
				{
					//领宠物
					//"level exp sex gettype understand habit gift borntime grow strchg intchg stachg spichg agichg skill1 skill2 skill3 skill4 skill5 skill6 skill7 skill8 skill9 skill10"
					EudemonData data;
					if(sscanf(pData->GetStr(COMPENSATIONDATA_PARAM),"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",&data.level,&data.exp,&data.sex,&data.gettype,&data.understand,&data.habit
						,&data.gift,&data.borntime,&data.grow,&data.strchg,&data.intchg,&data.stachg,&data.spichg,&data.agichg
						,&data.skill[0],&data.skill[1],&data.skill[2],&data.skill[3],&data.skill[4],&data.skill[5],&data.skill[6],&data.skill[7],&data.skill[8],&data.skill[9]) != 24)
						return false;

					if(!m_pOwner->AwardEudemon(pData->GetInt(COMPENSATIONDATA_TYPEID),data,false,false,0,true))
						return false;

					sprintf(szTemp,"你获得%s宠物",data.name);
					m_pOwner->SendSysMsg(szTemp);

					CTime tNowCheck(CTime::GetCurrentTime());
					sprintf(szTemp,"你领取了%s.",data.name);
					AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_OPMSG,tNowCheck.Format("%Y-%m-%d %H:%M:%S"),szTemp);

					pData->Destroy();
					SAFE_RELEASE(pData);
					m_setCompensation.erase(it);
					return true;
				}
				else
				{
					//其他
					bOther = true;
					break;
				}
			}
		}
	}

	bool bRet = false;
	if(bOther)
	{
		bool bOk = false;
		for(it = m_setCompensation.begin(); it != m_setCompensation.end() ; )
		{
			CCompensation* pData = *it;
			if(pData)
			{
				char szOptype[64];
				int nType = pData->GetInt(COMPENSATIONDATA_TYPE);
				if(nType != COMPENSATIONTYPE_ITEM && nType != COMPENSATIONTYPE_EUDEMON)
				{
					bool bGet = false;
					list<CCompensation*>::iterator itThis = it;
					it ++;
					
					switch(nType)
					{
					case COMPENSATIONTYPE_GOLD1:
						{
							bGet = m_pOwner->GainMoney(pData->GetInt(COMPENSATIONDATA_NUM),true,1);
							strcpy(szOptype,"银两");
						}
						break;
					case COMPENSATIONTYPE_GOLD2:
						{
							bGet = m_pOwner->GainMoney(pData->GetInt(COMPENSATIONDATA_NUM),true,2);
							strcpy(szOptype,"碎银");
						}
						break;
					case COMPENSATIONTYPE_GOLD3:
						{
							bGet = m_pOwner->GainMoney(pData->GetInt(COMPENSATIONDATA_NUM),true,4);
							strcpy(szOptype,"珠宝");
						}
						break;
					case COMPENSATIONTYPE_GOLD5:
						{
							if(pData->GetInt(COMPENSATIONDATA_NUM) > 0 || pData->GetInt(COMPENSATIONDATA_NUM) <= 200000)
							{
								m_pOwner->GainVasCPST(pData->GetInt(COMPENSATIONDATA_NUM));
								bGet = true;
							}
							else
							{
								m_pOwner->SendSysMsg("补偿元宝超额，请联系GM");
							}
							strcpy(szOptype,"元宝");
						}
						break;
					case COMPENSATIONTYPE_PORFFER:
						{
							bGet = m_pOwner->AddAttrib(_USERATTRIB_ARENA_SCORE,pData->GetInt(COMPENSATIONDATA_NUM),true);
							strcpy(szOptype,"荣誉");
						}
						break;
					case COMPENSATIONTYPE_SYNPORFFER:
						{
							int nOldJobCon = m_pOwner->GetSynCon();
							m_pOwner->SetSynCon(max(0,nOldJobCon + pData->GetInt(COMPENSATIONDATA_NUM)));
							bGet = true;
							strcpy(szOptype,"帮贡");
						}
						break;
					case COMPENSATIONTYPE_EXP: //经验特殊处理
						{
							int nGetData = m_pOwner->AwardBattleExp(pData->GetInt(COMPENSATIONDATA_NUM));
							if( nGetData > 0)
							{
								m_pOwner->SendSysMsg(_TXTATR_TOPSHOW, "你获得了%u经验", nGetData);

								bOk = true;
								strcpy(szOptype,"经验");
								CTime tNowCheck(CTime::GetCurrentTime());
								sprintf(szTemp,"你领取了%s%d.",szOptype,nGetData);
								AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_OPMSG,tNowCheck.Format("%Y-%m-%d %H:%M:%S"),szTemp);

								int nLeave = pData->GetInt(COMPENSATIONDATA_NUM) - nGetData;

								CMsgCompensation msg;
								msg.Create(COMPOSITONACTION_PICK_SUC,pData->GetInt(COMPENSATIONDATA_ID),pData->GetInt(COMPENSATIONDATA_TYPEID),nLeave);
								m_pOwner->SendMsg(&msg);

								if(nLeave > 0)
								{
									pData->SetInt(COMPENSATIONDATA_NUM,nLeave);
									pData->Update();
								}
								else
								{
									pData->Destroy();
									SAFE_RELEASE(pData);
									m_setCompensation.erase(itThis);
								}
							}
							
						}
						break;
					}

					if(bGet)
					{
						bOk = true;
						CTime tNowCheck(CTime::GetCurrentTime());
						sprintf(szTemp,"你领取了%s%d.",szOptype,pData->GetInt(COMPENSATIONDATA_NUM));
						AddMsg(m_pOwner->GetID(),COMPENSATIONMSGTYPE_OPMSG,tNowCheck.Format("%Y-%m-%d %H:%M:%S"),szTemp);


						if(pData->GetInt(COMPENSATIONDATA_ID) != id)
						{
							CMsgCompensation msg;
							msg.Create(COMPOSITONACTION_PICK_SUC,pData->GetInt(COMPENSATIONDATA_ID),pData->GetInt(COMPENSATIONDATA_TYPEID),0);
							m_pOwner->SendMsg(&msg);
						}
						else
							bRet = true;

						pData->Destroy();
						SAFE_RELEASE(pData);
						m_setCompensation.erase(itThis);
						continue;
					}
					else
						continue;
				}
			}
			it ++;
		}

		if(!bOk)
		{
			CMsgCompensation msg;
			msg.Create(COMPOSITONACTION_PICK_FAIL,id,0,0);
			m_pOwner->SendMsg(&msg);
		}
	}
	return bRet;
}
