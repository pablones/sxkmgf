// MsgBetOutside.cpp: implementation of the CMsgAtk class.
// �ɽ��ޣ�2002.1.8
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "basefunc.h"
#include "MapGroup.h"
#include "SynManager.h"

int levMemberAmount[] = {0,50,55,60,65,70,75,80,85,90,95};//�������Ӧ��0��0������ʼ--jay
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgSyndicate::CMsgSyndicate()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgSyndicate::~CMsgSyndicate()
{
}

//////////////////////////////////////////////////////////////////////
BOOL	CMsgSyndicate::Create(unsigned short usAction, OBJID idTarget, OBJID idFealty)
{
	// check param
	if(usAction == ACTION_NONE) //  || idTarget == ID_NONE
	{
		ASSERT(!"CMsgSyndicate::Create()");
		return false;
	}

	// init
	this->Init();

	m_pInfo->usAction	=usAction;
	m_pInfo->idTarget	=idTarget;
	m_pInfo->dwData	=idFealty;
	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_SYNDICATE;


	return true;
}
BOOL CMsgSyndicate::Create(unsigned short usAction, char* pMsgBuf)
{
	this->Init();
	m_StrPacker.AddString(pMsgBuf);
	m_pInfo->usAction = usAction;
	
	m_unMsgType = _MSG_SYNDICATE;
	m_unMsgSize = sizeof(MSG_Info)-1+m_StrPacker.GetSize();

	return true;
}
//////////////////////////////////////////////////////////////////////
BOOL	CMsgSyndicate::Create		(char* pMsgBuf, DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_SYNDICATE != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
void	CMsgSyndicate::Process		(CGameSocket* pSocket)
{
	try {
	// �����Ϣ��ʽ
	//����������ͼ���û�в���
// 	if(SYN_WAR_SET == m_pInfo->usAction)//��һ��ͼ��㲥
// 	{
// 		if(PID == MSGPORT_MAPGROUP_FIRST)
// 			return;
// 		OBJID thisID = 0,targetID = 0;
// 		int state = 0;
// 
// 		targetID = m_pInfo->idTarget;
// 		char szName[_MAX_NAMESIZE] = "";
// 		if (m_StrPacker.GetStrAmount() > 0)
// 		m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
// 		if(sscanf(szName,"%d %d",&thisID,&state) == 2)
// 		{
// 			CSynPtr pSyn = SynManager()->QuerySyndicate(thisID);
// 			if(!pSyn)
// 				return ;
// 			CSynPtr pSynTarget = SynManager()->QuerySyndicate(targetID);
// 			if(!pSynTarget)
// 				return ;
// 
// 			pSyn->SetSynWar(targetID,false);
// 			pSynTarget->SetSynWar(thisID,false);
// 
// 			pSyn->SetState(state);
// 			pSynTarget->SetState(state);
// 
// 			pSyn->SynchroInfo();
// 			pSynTarget->SynchroInfo();
// 			
// 		}		
// 
// 		if(PID < MSGPORT_MAPGROUP_INSTANCE - 1)
// 			CurrentMapGroup()->QueryIntraMsg()->TransmitMapGroupMsg(PID + 1,this);
// 		return;
// 	}
// 	else if(SYN_WAR_SEND == m_pInfo->usAction)//��һ��ͼ�����
// 	{
// 		if(PID != MSGPORT_MAPGROUP_FIRST)
// 			return;
// 
// 		OBJID thisID = 0,targetID = 0;
// 		int state = 0;
// 		
// 		targetID = m_pInfo->idTarget;
// 		char szName[_MAX_NAMESIZE] = "";
// 		if (m_StrPacker.GetStrAmount() > 0)
// 			m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
// 		if(sscanf(szName,"%d %d",&thisID,&state) == 2)
// 		{
// 			CSynPtr pSyn = SynManager()->QuerySyndicate(thisID);
// 			if(!pSyn)
// 				return ;
// 			CSynPtr pSynTarget = SynManager()->QuerySyndicate(targetID);
// 			if(!pSynTarget)
// 					return ;
// 
// 			if(0 == state)//��ʼ
// 			{
// 				pSyn->SetSynWar(targetID);
// 				pSynTarget->SetSynWar(thisID);
// 				MSGBUF szMsg;
// 				sprintf(szMsg, "%s����%s��ῪʼԼս!", pSyn->GetStr(SYNDATA_NAME), pSynTarget->GetStr(SYNDATA_NAME));
// 				UserManager()->BroadcastMsg(szMsg, NULL, NULL, 0xffffff, _TXTATR_SYSTEM);
// 			}
// 			else if(1 == state)//ɱ��
// 			{
// 				int synKill = pSynTarget->GetKill();
// 				pSynTarget->SetKill(synKill + 1);
// 
// 			}
// 			
// 			
// 		}
// 		return;
// 	}
	char szName[_MAX_NAMESIZE] = "";
	char szWord[256] = "";
	if (m_StrPacker.GetStrAmount() > 0)
		m_StrPacker.GetString(0, szName, _MAX_NAMESIZE);
	if (m_StrPacker.GetStrAmount() > 1)
		m_StrPacker.GetString(1, szWord, 256);

	// ״̬���
	CUserPtr pUser = UserManager()->GetUser(this);
 	if(pUser==NULL)
	{
		///���ﴦ�����Ϣ�������û�ָ��
		switch(m_pInfo->usAction)
		{
			///�����Ǳ���߷������İ��ɷ��ͳɹ�����Ϣ
			case SYN_CREATE_OK:
				{
					///�ȿ������Ƿ��Ѵ���������ɣ����û���ٳ��Դ����ݿ��д���
					///����Ѵ���ʵ������и��²���
					SynManager()->Create(m_pInfo->idTarget);///������ɹ������ڱ��ش���һ������ʵ��
					///�����յ������Ϣ���߾�����������Ϊ����״̬
					CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
	// 				if(pSyn)
	// 				{
	// 					pSyn->SetOnline(pSyn->GetLeaderID(),true);
	// 				}
				}	
				break;
			case SYN_ANNONCE_OTHERLINE:///�����յ����޸���Ϣ
//			case SYN_ANNONCE:
				{
//  					CUserPtr pMasterUser = UserManager()->GetUser(this);
// 					if(pMasterUser==NULL)
//  					{
						///��������Ķ���Ϊ�����ʾ�����Ϣ�Ǵ������߹�����
						CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
						if(pSyn)
						{
							pSyn->SetStr(SYNDATA_ANNOUNCE,szWord,256,true);
							CMsgTalk	msg;
							IF_OK (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szWord, NULL, 0xFFFFFF, _TXTATR_SYNANNOUNCE))
								pSyn->BroadcastSynMsg(&msg);
						}
	//  					MyStackWalker sw;
	//  					sw.ShowCallstack();
// 					}
				}
				break;
			case SYN_CREATECOMPLETE:
				{
					CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
					if(pSyn==NULL)///����Ҳ���������ɳ������¹���һ��
					{
						SynManager()->Create(m_pInfo->idTarget);///������ɹ������ڱ��ش���һ������ʵ��
						pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
					}
					if(pSyn)
						pSyn->BroadcastSynMsg(this);
				}
				break;

				
			case	SYN_UPLEV_OTHERLINE:
				{
					CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
					if (pSyn)
					{
							pSyn->UpdateLevel(m_pInfo->dwData);
						if (pUser)
						{
							if (m_pInfo->dwData!=0)
								pSyn->SendSciToC(pUser);
							else
								pSyn->SendAllInfoToClient(pUser);
						}
					}
				}
				break;
			case	SYN_UPSKILLSUB_OTHERLINE:
				{
// 					int nSub = 0;
// 					int nSkill = 0;
// 					nSub = m_pInfo->dwData / 10;
// 					nSkill = m_pInfo->dwData % 10;
// 					CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 					if (pSyn)
// 					{
// 						pSyn->UpSubSkill(nSub, nSkill);
// 						CMsgTalk	msg;
// 						sprintf(szWord, "%d", m_pInfo->dwData);
// 						IF_OK (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szWord, NULL, 0xFFFFFF, _TXTATR_SYNUPSKILL))
// 						pSyn->BroadcastSynMsg(&msg);
// 					}
				}
				break;
			case SYN_SETBUILDOTHERLINE:
				{
					UINT Money = 0;
					UINT Build = 0;
					int nType0 = 0;
					int nType = m_pInfo->dwData % 10;
					int idSyn = m_pInfo->dwData / 10;
					CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
					if (pSyn)
					{
						pSyn->SetGrow(nType, m_pInfo->idTarget, false);
					}
				}
				break;
			case	SYN_INVITE_OTHERLINE:
				{
					CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->dwData);
					if(!pSyn)
						return;
					OBJID pTargetid = m_pInfo->idTarget;
					CUser* pTarget	= UserManager()->GetUser(pTargetid);
					if (pTarget)
					{
						this->Create(SYN_INVITE, m_pInfo->dwData, pUser->GetName(), 0, "");
						pTarget->SendMsg(this);
						return;
					}
				}
				break;
			case 	SYN_SYNTALKMSG:
				{
					CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->dwData);
					if(!pSyn)
						return;
					CMsgTalk	msg;
					if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_SYNDICATE))
						pSyn->BroadcastSynMsg(&msg);					
				}
				break;
			case SYN_UPDATEOTHERNLINE:
				{	
						CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->dwData);
						if(!pSyn)
							return;
						CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->idTarget);
						if(pThisUser)
						{
							if (pThisUser->GetMap()==pThisUser->GetSynMap())
								pThisUser->LeaveSynMap();
							pThisUser->QuerySynAttr()->LeaveSyn(true);
							pThisUser->SendSysMsg("�㱻�߳�����");
						}
						else if(pSyn->Hasmember( m_pInfo->idTarget))
						{
							::MemberInfo* info =pSyn->GetMemberFromList(m_pInfo->idTarget,1);
							if(info)
							{
								sprintf(szWord, "%s���߳�����", info->szName);
								CMsgTalk msg;
								if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME , szWord, NULL, 0xff0000, _TXTATR_SYNDICATE))
									pSyn->BroadcastSynMsg(&msg);
								pSyn->DelMemberList(m_pInfo->idTarget);
							}
						}
					}

				break;
			case SYN_SETSYNMAPID:
				{
					CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
					if(!pSyn)
							return;
					if (m_idProcess!=MSGPORT_MAPGROUP_FIRST)
							pSyn->SetSynMapID(m_pInfo->dwData);
				}
				break;
			default:
				break;
		}
		return;
	}
	// �������
//	ASSERT(pSocket);
	if(!pSocket)
		return;

	IF_NOT(pUser)
		return;

	OBJID idUser = pUser->GetID();
	IF_NOT(idUser != ID_NONE)
		return;

	if(!pUser->IsAlive())
	{
		pUser->SendSysMsg("���Ѿ�����");//STR_DIE
		return ;
	}


	switch(m_pInfo->usAction)
	{
	case SYN_CREATE:
		{
			CHECK(m_pInfo->idTarget);
			CItem* pItem = pUser->GetItemByType(630007);
			if(!pItem)
				return;

			if(pUser->GetMoney() < 1000000)
				return;
		
			CSyndicate* pSynOld = pUser->GetSyndicate();
			if(pSynOld )
			{
				if(pSynOld->GetInt(SYNDATA_STATE) == 1)
				{
					if(pUser->QuerySynAttr()->GetSynRank() == 100)
					{
						pUser->SendSysMsg("���İ����ڴ����У����ܽ�����İ���");
						return;
					}
					else
					{
						if(!pUser->QuerySynAttr()->LeaveSyn())
						{
							pUser->SendSysMsg("���İ����ڴ����У����ܽ�����İ���");
							return;
						}
					}
				}
				else
				{
					pUser->SendSysMsg("���Ѿ��ڰ����У��˳�����ܼ�����������");
					return;
				}
			}
			CHECK(pUser->QuerySynAttr());
			OBJID idSyn = pUser->QuerySynAttr()->CreateSyndicate(szName, szWord);
			if(idSyn != ID_NONE)
			{
				if (pUser->SpendItem(pItem))
				{
					if(pUser->SpendMoney(1000000,true))
					{
						this->Create(SYN_CREATE_OK,idSyn);
					}
					else
					{
						LogSave("ERROR: ���ɿ۳��ʽ����! idSyn=%u , iduser=%u",idSyn,pUser->GetID());
						this->Create(SYN_CREATE_ERROR,pUser->GetID());
					}
				}
				else
				{
					LogSave("ERROR: ���ɿ۳���Ʒ����! itemtype=%u , idSyn=%u , iduser=%u",pItem->GetItemType(),idSyn,pUser->GetID());
					this->Create(SYN_CREATE_ERROR,pUser->GetID());
				}
			}
			else
			{
				this->Create(SYN_CREATE_ERROR,pUser->GetID());
			}
			pUser->SendMsg(this);
		}
		break;
	case SYN_QUERY_CREATE:
		{
			SynManager()->SendCreateSynToUser(pUser,m_pInfo->dwData);
		}
		break;
	case SYN_JOIN:
		{
// 			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			if(!pSyn)
// 				return;
// 			if(pSyn->GetInt(SYNDATA_STATE) == 1) //��Ӧ������Ҫ�������� �Զ����ϴ���Ӧ
// 			{
// 				CSyndicate* pSynOld = pUser->GetSyndicate();
// 				if(pSynOld && (pSynOld->GetInt(SYNDATA_STATE) == 1))
// 				{
// 					if(pUser->QuerySynAttr()->GetSynRank() == 100)
// 					{
// 						pUser->SendSysMsg("���İ����ڴ����У�������Ӧ���˵İ���");
// 					}
// 					else
// 					{
// 						if(!pUser->QuerySynAttr()->LeaveSyn())
// 							pUser->SendSysMsg("���İ����ڴ����У�������Ӧ���˵İ���");
// 
// 					}
// 
// 				}
// 				pUser->QuerySynAttr()->JoinSyn(m_pInfo->idTarget);
// 				pUser->SendSysMsg("��Ӧ�ɹ�");
// 
// 				///��Ӧ�ɹ�,��������Ϣ�㲥��������,���ߴ����ɹ�������
// //  				CMsgSyndicate msg;
// //  				if(msg.Create(SYN_CREATE_OK,m_pInfo->idTarget,pUser->GetID()))
// //  					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg, ALLUSERS_NAME);
// 			}
// 			else
// 			{
// 				pUser->SendSysMsg("��Ӧ�����Ѿ������ɹ�");
// 			}
// 
// 			CMsgSyndicateList msg;
// 			if (msg.Create(SYNLISTACTION_UPDATA,pSyn->GetID(),pSyn->GetStr(SYNDATA_NAME),pSyn->GetStr(SYNDATA_LEADERNAME),pSyn->GetInt(SYNDATA_LEVEL),pSyn->GetInt(SYNDATA_AMOUNT)))
// 				pUser->SendMsg(&msg);
		}
		break;
	case SYN_QUERY_INFO:
		{
			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
			if(m_pInfo->idTarget == ID_NONE)
				pSyn = pUser->GetSyndicate();
			else
				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);

			if(!pSyn)
				return;
			pSyn->SendSynInfoToClient(pUser);
		}
		break;

	case SYN_EVERYSUB:   
		{
			CSyndicate* pSyn = NULL/*SynManager()->QuerySyndicate(m_pInfo->idTarget)*/;
			if(m_pInfo->idTarget == ID_NONE)
				pSyn = pUser->GetSyndicate();
			else
				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
			
			if(!pSyn)
				return;
			pSyn->SendSubToC(pUser);
		}
		break;
	case SYN_CONLIST://�ﹱ�б�
	case SYN_MEMBERLIST://��Ա�б�
		{
			CSyndicate* pSyn;
			if(m_pInfo->idTarget == ID_NONE)
				pSyn = pUser->GetSyndicate();
			else
				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
			
			if(!pSyn)
				return;
			
			if(pSyn->GetInt(SYNDATA_STATE) == 1) 
				return;
			
			pSyn->SendMenberListToClient(pUser);

		}
		break;
	case SYN_QUERYLIST://�����б�
		{
			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
			if(m_pInfo->idTarget == ID_NONE)
				pSyn = pUser->GetSyndicate();
			else
				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
			
			if(!pSyn)
				return;
			
			if(pSyn->GetInt(SYNDATA_STATE) == 1) 
				return;
			
			pSyn->SendQueryListToClient(pUser,m_pInfo->dwData);

		}
		break;
	case SYN_QUERYSYNLIST:		//�����б�
		{
			SynManager()->SendSynList(pUser,m_pInfo->dwData);
		}
		break;
	case SYN_SYNINFO:		//������Ϣ
		{
			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
			if(m_pInfo->idTarget == ID_NONE)
				pSyn = pUser->GetSyndicate();
			else
				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
			
			if(!pSyn)
				return;
			pSyn->SendAllInfoToClient(pUser);
		}
		break;

	case 		SYN_LEVELUP://����
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
			{
				if (pUser->IsGM())
					pUser->SendSysMsg("û�ҵ���");
				return;
			}
			if(strcmp(szName,pUser->GetName())!=0)
			{
				CMsgSyndicate msg;
				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
				return;
			}
			int nRank = pUser->QuerySynRank();
			if (nRank < RANK_SUBLEADER)
			{
				pUser->SendSysMsg("����Ȩ�޲��㣡");
				return;
			}
			if(pSyn->UpLevel(pUser, m_pInfo->dwData))
			{
				CMsgSyndicate msgSyndicate;
				m_pInfo->idTarget = pSyn->GetID();
				msgSyndicate.Create(SYN_UPLEV_OTHERLINE,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData,szWord);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msgSyndicate,(OBJID)0);
				if (m_pInfo->dwData!=0)
					pSyn->SendSciToC(pUser);
				else
					pSyn->SendAllInfoToClient(pUser);                      
			}
			else
				pUser->SendSysMsg("����ʧ�ܣ�");
		}
		break;
	case SYN_SCIINFO:   //��ȡ�Ƽ���Ϣ
		{
			CSyndicate* pSyn = NULL/*SynManager()->QuerySyndicate(m_pInfo->idTarget)*/;
			if(m_pInfo->idTarget == ID_NONE)
				pSyn = pUser->GetSyndicate();
			else
				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
			if(!pSyn)
				return;
			pSyn->SendSciToC(pUser);
		}
		break;
	case SYN_ACTION_ENTERSYN:
		{
			//�߼���������!
		}
		break;
	case SYN_SUB1_ACTION://����
		{
			pUser->ProcessAction(1018);
		}
		break;
	case SYN_SUB2_ACTION://�׻�
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
			{
				pUser->SendSysMsg("�㻹û����!");
				return;
			}
			if(!pSyn->GetCallTask())
			{
				if (m_pInfo->idTarget>10 || m_pInfo->idTarget <= 0)
					return;
				LuaScriptMachine()->InitVar(m_pInfo->idTarget);//�����ű�
				if(pUser->ProcessAction(1011))
					pSyn->SetCallTask(true);
			}
			else
			{
				pUser->SendSysMsg("���ε�Ϯ�ѽ���");
			}
		}
		break;
	case SYN_SUB3_ACTION://����
		{
			pUser->ProcessAction(2904604);
		}
		break;
	case SYN_SUB4_ACTION://��ȸ
		{
			pUser->ProcessAction(1016);
		}
		break;
	case SYN_ACTION_WELFARE:		//��ȡ����
		{
			
		}
		break;
	case SYN_ACTION_SHOP:		//�̵�
		{
			pUser->ProcessAction(1017);
		}
		break;
// 	case SYN_QUERY_MEMBER:
// 		{
// 			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			if(m_pInfo->idTarget == ID_NONE)
// 				pSyn = pUser->GetSyndicate();
// 			else
// 				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			
// 			if(!pSyn)
// 				return;
// 
// 			if(pSyn->GetInt(SYNDATA_STATE) == 1) 
// 				return;
// 			
// //			pSyn->SendMenberToClient(pUser,m_pInfo->dwData);
// 		}
// 		break;
	case SYN_QUERY_LIST:
		{
			SynManager()->SendSynList(pUser,m_pInfo->dwData);
		}
		break;
	case SYN_JOIN_QUERY:
		{
			CSyndicate* pSynOld = pUser->GetSyndicate();
			if(pSynOld )
			{
				if(pSynOld->GetInt(SYNDATA_STATE) == 1)
				{
					if(pUser->QuerySynAttr()->GetSynRank() == 100)
					{
						pUser->SendSysMsg("���İ����ڴ����У����ܼ����İ���");
						return;
					}
					else
					{
						if(!pUser->QuerySynAttr()->LeaveSyn())
						{
							pUser->SendSysMsg("���İ����ڴ����У����ܼ����İ���");
							return;
						}
					}
				}
				else
				{
					pUser->SendSysMsg("���Ѿ��ڰ����У��˳�����ܼ�����������");
					return;
				}
			}


			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
			if(m_pInfo->idTarget == ID_NONE)
				pSyn = pUser->GetSyndicate();
			else
				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
			
			if(!pSyn)
				return;
			
			if(pSyn->GetInt(SYNDATA_STATE) != 2) 
				return;

			if(pSyn->GetMemberAmount() >= levMemberAmount[pSyn->GetInt(SYNDATA_LEVEL)])
			{
				pUser->SendSysMsg("�����������");
				return;
			}
			
			MemberInfo info={0};
			info.id = pUser->GetID();
			info.level = pUser->GetLev();
			info.firJob = pUser->GetProfession(1);
			info.sex=pUser->GetSex();
			info.maxpower=pUser->GetMaxScore();
			if (pUser->IsVipActive())
				info.vip=pUser->GetVipLev();
			else
				info.vip=0;
			info.line=CVipList::GetLineByName(MapGroup(PID)->GetServerName());
			SafeCopy(info.szName,pUser->GetName());
			if(pSyn->GetMemberFromList(info.id,2)==NULL)
			{
				if(pSyn->AddMemberList(info,2) == RANK_NORMAL)
				{
					///����������֪ͨ�����߸�������
					CMsgSynMemberInfo msg;
					if (msg.Create(SynMemberAdd,2,2))
					{
						msg.Append(pSyn->GetID(),info.szName,info.level,info.firJob,info.sex,10,info.line,info.vip,info.lastlogin,0,0,0,info.maxpower);///����Ϊ�˲��޸�Э�飬ֻ�ж��һ����Ч������������ID
						msg.Append(info.id,info.szName,info.level,info.firJob,info.sex,10,info.line,info.vip,info.lastlogin,0,0,0,info.maxpower);
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
					}
					pUser->SendSysMsg("���뷢�ͳɹ�");
				}
			}
			else
			{
				pUser->SendSysMsg("���Ѿ��ڶԷ��������б��У��벻Ҫ�ظ�����");
			}
		}
		break;
 	case SYN_QUERY_QUERYMEMBER:
	{
// 			CSyndicate* pSyn = SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			if(m_pInfo->idTarget == ID_NONE)
// 				pSyn = pUser->GetSyndicate();
// 			else
// 				pSyn =SynManager()->QuerySyndicate(m_pInfo->idTarget);
// 			
// 			if(!pSyn)
// 				return;
// 			
// 			if(pSyn->GetInt(SYNDATA_STATE) == 1) 
// 				return;
// 			
// //			pSyn->SendQueryMenberToClient(pUser,m_pInfo->dwData);
		}
		break;
	case SYN_ANSERR_OK:
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
				return;
			
			///�ذ��ں��ý�ɫ����������˰�Ϊ������
			if(strcmp(szName,pUser->GetName())!=0)
			{
				CMsgSyndicate msg;
				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
				return;
			}			

			if(pSyn->GetInt(SYNDATA_STATE) == 1) 
				return;

			int nRank = pUser->QuerySynRank();
			if(nRank < RANK_FEELLEADER)
			{
				pUser->SendSysMsg("���Ȩ�޲���");
				return;
			}

			if(pSyn->GetMemberAmount() >= levMemberAmount[pSyn->GetInt(SYNDATA_LEVEL)])
			{
				pUser->SendSysMsg("�����������");
				return;
			}

			if(!pSyn->AllowMenber(m_pInfo->idTarget,pUser))
			{
				pUser->SendSysMsg("�Է��Ѿ������˰���");
				return;
			}
			OBJID idTarget=m_pInfo->idTarget;
			CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->idTarget);
			if(pThisUser)
			{
				pThisUser->SendSysMsg("����%sͨ��",pSyn->GetStr(SYNDATA_NAME));

				this->Create(SYN_UPDATE,pThisUser->GetID(),SYN_UPDATE_ALLOW);
				pThisUser->SendMsg(this);
				pUser->SendMsg(this);
			}
			else
			{
				this->Create(SYN_UPDATE,pUser->GetID(),SYN_UPDATE_ALLOW);
				pUser->SendMsg(this);
			}
		}
		break;
	case SYN_ANSERR_NO:
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
				return;
			
			///�ذ��ں��ý�ɫ����������˰�Ϊ������
			if(strcmp(szName,pUser->GetName())!=0)
			{
				CMsgSyndicate msg;
				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
				return;
			}			

			if(pSyn->GetInt(SYNDATA_STATE) == 1) 
				return;

			int nRank = pUser->QuerySynRank();
			if(nRank < RANK_FEELLEADER)
			{
				pUser->SendSysMsg("���Ȩ�޲���");
				return;
			}
			OBJID idTarget=m_pInfo->idTarget;
			pSyn->DelMemberList(m_pInfo->idTarget,2);
			CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->idTarget);
			if(pThisUser)
			{
				pThisUser->SendSysMsg("��%s�ܾ�",pSyn->GetStr(SYNDATA_NAME));
				this->Create(SYN_UPDATE,pThisUser->GetID(),SYN_UPDATE_NO);
				pThisUser->SendMsg(this);
			}
			///��׼������֪ͨ�����߸�������
			CMsgSynMemberInfo msg;
			if (msg.Create(SynMemberAnswerNO,2,2))
			{
				msg.Append(pSyn->GetID(),"",0,0,0,0,0,0,0,0,0,0,0);///����Ϊ�˲��޸�Э�飬ֻ�ж��һ����Ч������������ID
				msg.Append(idTarget,"",0,0,0,0,0,0,0,0,0,0,0);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
			}
		}
		break;
	case SYN_KICKOUT:
		{
			bool bIgnorData = false;
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
			{
				pUser->SendSysMsg("û�ҵ�����");
				return;
			}
			///�ذ��ں��ý�ɫ����������˰�Ϊ������
			if(strcmp(szName,pUser->GetName())!=0)
			{
				CMsgSyndicate msg;
				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
				return;
			}			
			int nMyRank = pUser->QuerySynRank();
			OBJID idTarget=m_pInfo->idTarget;
			if(nMyRank < RANK_FEELLEADER)
			{
				pUser->SendSysMsg("���Ȩ�޲���");
				return;
			}
			CUserPtr pThisUser = UserManager()->GetUser(m_pInfo->idTarget);
			if(pThisUser&&pThisUser->QuerySynAttr()!=NULL)
			{
				if ( pThisUser->QuerySynRank() == RANK_NONE)
				{
					pUser->SendSysMsg("�Է��Ѿ��뿪�˰���");
					return;
				}
				if ((pThisUser->GetSyndicate()->GetID() != pSyn->GetID()) || nMyRank < pThisUser->QuerySynRank() )
				{
					pUser->SendSysMsg("���Ȩ�޲���");
					return;
				}
				if (pUser->GetID() == idTarget)		//ȷ�������Լ�T�Լ�
				{
					pUser->SendSysMsg("�������Լ�");
					return;
				}
				if (pThisUser->QuerySynAttr()->LeaveSyn(true,true,true))
				{
					pThisUser->SendSysMsg("�㱻�߳�����");
					this->Create(SYN_UPDATE,pThisUser->GetID(),SYN_UPDATE_KICK);
					pThisUser->SendMsg(this);
					pThisUser->DelHoldTitle(_TITLE_SYNDICATE);//goto ɾ���ƺ�
					CMsgSynMemberInfo msgtoc;
					if (msgtoc.Create(DelMember,1,1))
					{
						msgtoc.Append(pThisUser->GetID(),"",0,0,0,0,0,0,0,0,0,0,0);
						pSyn->BroadcastSynMsg(&msgtoc);
					}
					pUser->SendSysMsg("�߳��ɹ�");
				}
				else
					pUser->SendSysMsg("�߳�ʧ��");
			}
			else
			{
				int nThisRank = pUser->QuerySynAttr()->GetRankByID(m_pInfo->idTarget,pSyn->GetID());
				if(nThisRank == RANK_NONE)
				{
					pUser->SendSysMsg("�Է��Ѿ��뿪�˰���");
					return;
				}
				if ((pUser->GetSyndicate()->GetID() != pSyn->GetID()) || nMyRank < nThisRank)
				{
					pUser->SendSysMsg("���Ȩ�޲���");
					return;
				}
				if (nThisRank == RANK_NONE)//�쳣����
				{
					bIgnorData = true;
				}
				if (pUser->GetID() != idTarget)		//ȷ�������Լ�T�Լ�
				{
					if(pUser->QuerySynAttr()->KickoutMember(m_pInfo->idTarget, bIgnorData))
						pUser->SendSysMsg("�߳��ɹ�");
					else
						pUser->SendSysMsg("�߳�ʧ��");
// 					if (nThisRank >= RANK_FEELLEADER)
// 						CUserTitle::DelOffLineUserTitle(idTarget, _TITLE_SYNDICATE, Database());
					CMsgSynMemberInfo msgtoc;
					if (msgtoc.Create(DelMember,1,1))
					{
						msgtoc.Append(idTarget,"",0,0,0,0,0,0,0,0,0,0,0);
						pSyn->BroadcastSynMsg(&msgtoc);
					}
				}
				else
					pUser->SendSysMsg("�������Լ�");

			}
			///�����˰֪ͨ�����߸�������
			CMsgSynMemberInfo msg;
			if (msg.Create(SynMemberKickout,1,2))
			{
				msg.Append(pSyn->GetID(),"",0,0,0,0,0,0,0,0,0,0,0);///����Ϊ�˲��޸�Э�飬ֻ�ж��һ����Ч������������ID
				msg.Append(idTarget,"",0,0,0,0,0,0,0,0,0,0,0);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
			}
		
		}
		break;
	case SYN_LEAVER:
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
				return;
			///�ذ��ں��ý�ɫ����������˰�Ϊ������
			if(strcmp(szName,pUser->GetName())!=0)
			{
				CMsgSyndicate msg;
				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
				return;
			}			

			int nMyRank = pUser->QuerySynRank();
			if(nMyRank == RANK_LEADER)
				return;
			
			if (pUser->QuerySynAttr()->LeaveSyn())	
			{
				pUser->DelHoldTitle(_TITLE_SYNDICATE);//goto ɾ���ƺ�
				pUser->SendSysMsg("�뿪�ɹ�");
			}
			else
			{
				pUser->SendSysMsg("�뿪ʧ��");
				return;
			}
			///�����˰֪ͨ�����߸�������
			CMsgSynMemberInfo msg;
			if (msg.Create(SynMemberRemove,1,2))
			{
				msg.Append(pSyn->GetID(),"",0,0,0,0,0,0,0,0,0,0,0);///����Ϊ�˲��޸�Э�飬ֻ�ж��һ����Ч������������ID
				msg.Append(pUser->GetID(),pUser->GetName(),0,0,0,0,0,0,0,0,0,0,0);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
			}
			this->Create(SYN_UPDATE,pUser->GetID(),SYN_UPDATE_LEAVE);
			pUser->SendMsg(this);
		}
		break;
	case SYN_SETRANK:
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
				return;
		
			///�ذ��ں��ý�ɫ����������˰�Ϊ������
			if(strcmp(szName,pUser->GetName())!=0)
			{
				CMsgSyndicate msg;
				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
				return;
			}			

			int nMyRank = pUser->QuerySynRank();
			if(nMyRank <= m_pInfo->dwData)//�����߼����ܶ���ְλ
			{
				pUser->SendSysMsg("���Ȩ�޲���");
				return;
			}

			int nThisRank = pUser->QuerySynAttr()->GetRankByID(m_pInfo->idTarget,pSyn->GetID());
			if(nThisRank == RANK_NONE)
			{
				pUser->SendSysMsg("�Է��Ѿ��뿪�˰���");
				return;
			}

			if(m_pInfo->dwData < RANK_NORMAL || m_pInfo->dwData > RANK_SUBLEADER)
			{
				pUser->SendSysMsg("ְλ����");
				return;
			}
			if(pUser->QuerySynAttr()->SetIdRank(m_pInfo->idTarget,pSyn->GetID(),m_pInfo->dwData,true))
			{
				///�����ɹ�֪ͨ�����߸���
				///��׼������֪ͨ�����߸�������
				CMsgSynMemberInfo msg;
				if (msg.Create(SynMemberSetRank,2,2))
				{
					msg.Append(pSyn->GetID(),"",0,0,0,m_pInfo->dwData,0,0,0,0,0,0,0);///����Ϊ�˲��޸�Э�飬ֻ�ж��һ����Ч������������ID
					msg.Append(m_pInfo->idTarget,"",0,0,0,m_pInfo->dwData,0,0,0,0,0,0,0);
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
				}
				CMsgSynMemberInfo msgtoc;
				if (msgtoc.Create(SetRank,1,1))
				{
					msgtoc.Append(m_pInfo->idTarget,"",0,0,0,m_pInfo->dwData,0,0,0,0,0,0,0);
					pSyn->BroadcastSynMsg(&msgtoc);
				}
				pUser->SendSysMsg("�����ɹ�");
			}
		
		}
		break;
	case SYN_SETLEADER:		//��������
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if (!pUser->QuerySynAttr())
				return;
			if(!pSyn)
			{
				pUser->SendSysMsg("�㻹û�а");
				return;
			}
			///�ذ��ں��ý�ɫ����������˰�Ϊ������
			if(strcmp(szName,pUser->GetName())!=0)
			{
				CMsgSyndicate msg;
				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
				return;
			}	
			if (pUser->QuerySynRank() != RANK_LEADER)
			{
				pUser->SendSysMsg("��ûȨ�ޣ�");
				return;
			}
			if(m_pInfo->idTarget != pSyn->GetSubLeader2ID())
			{ 
				if(m_pInfo->idTarget != pSyn->GetSubLeaderID())
				{
					pUser->SendSysMsg("ֻ������������Ϊ����");
					return;
				}
			}
			CMsgSynMemberInfo msg;
			if(pUser->QuerySynAttr()->SetIdRank(m_pInfo->idTarget,pSyn->GetID(),RANK_LEADER,true))
			{
				if (msg.Create(SynMemberSetRank,2,2))
				{
					msg.Append(pSyn->GetID(),"",0,0,0,RANK_LEADER,0,0,0,0,0,0,0);///����Ϊ�˲��޸�Э�飬ֻ�ж��һ����Ч������������ID
					msg.Append(m_pInfo->idTarget,"",0,0,0,RANK_LEADER,0,0,0,0,0,0,0);
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
					CMsgSynMemberInfo msgtoc;
					if (msgtoc.Create(SetRank,1,1))
					{
						msgtoc.Append(m_pInfo->idTarget,"",0,0,0,RANK_LEADER,0,0,0,0,0,0,0);
						pSyn->BroadcastSynMsg(&msgtoc);
					}
				}
				else
					LogSave("ERROR: ������Ϣ����! idsyn=%u,iduser=%u,idleader=%u",pSyn->GetID(),pUser->GetID(),pSyn->GetLeaderID());
				if (pUser->QuerySynAttr()->SetIdRank(pUser->GetID(),pSyn->GetID(),RANK_SUBLEADER,true))
				{
					if (msg.Create(SynMemberSetRank,2,2))
					{
						msg.Append(pSyn->GetID(),"",0,0,0,RANK_SUBLEADER,0,0,0,0,0,0,0);///����Ϊ�˲��޸�Э�飬ֻ�ж��һ����Ч������������ID
						msg.Append(pUser->GetID(),"",0,0,0,RANK_SUBLEADER,0,0,0,0,0,0,0);
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
					}
					CMsgSynMemberInfo msgtoc;
					if (msgtoc.Create(SetRank,1,1))
					{
						msgtoc.Append(pUser->GetID(),"",0,0,0,RANK_SUBLEADER,0,0,0,0,0,0,0);
						pSyn->BroadcastSynMsg(&msgtoc);
					}
					pUser->SendSysMsg("�����ɹ�");
					CMsgSynInfo msgup;
					if(msgup.CreateNew(pSyn,pUser))
						pSyn->BroadcastSynMsg(&msgup);
				}
				else
					LogSave("ERROR: ��������������! idsyn=%u,iduser=%u,idleader=%u",pSyn->GetID(),pUser->GetID(),pSyn->GetLeaderID());
			}
			else
				pUser->SendSysMsg("����ʧ��!");
		}
		break;
	case SYN_DEMISE:
		{
// 			CSyndicate* pSyn = pUser->GetSyndicate();
// 			if(!pSyn)
// 				return;
// 			
// 			///�ذ��ں��ý�ɫ����������˰�Ϊ������
// 			if(strcmp(szName,pUser->GetName())!=0)
// 			{
// 				CMsgSyndicate msg;
// 				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
// 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
// 				return;
// 			}			
// 
// 			int nMyRank = pUser->QuerySynRank();
// 			if(nMyRank != RANK_LEADER)
// 			{
// 				pUser->SendSysMsg("���Ȩ�޲���");
// 				return;
// 			}
// 
// 			int nThisRank = pUser->QuerySynAttr()->GetRankByID(m_pInfo->idTarget,pSyn->GetID());
// 			if(nThisRank == RANK_NONE)
// 			{
// 				pUser->SendSysMsg("�Է��Ѿ��뿪�˰���");
// 				return;
// 			}
// 
// 			if(pUser->QuerySynAttr()->SetIdRank(m_pInfo->idTarget,pSyn->GetID(),RANK_LEADER),true)
// 			{
// 				pUser->SendSysMsg("��λ�ɹ�");
// 				///�����ɹ�֪ͨ�����߸���
// 				///��׼������֪ͨ�����߸�������
// 				CMsgSynMemberInfo msg;
// 				if (msg.Create(SynMemberSetRank,1,1))
// 				{
// 				
// 					msg.Append(m_pInfo->idTarget,"",RANK_LEADER,0,0,0,0,0,0,0,0,0,0);
// 					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,(OBJID)0);
// 				}
// 			}
		}
		break;
	case SYN_IMPACH:
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if (!pUser->QuerySynAttr())
			{
				return;
			}
			if(!pSyn)
			{
				pUser->SendSysMsg("�㻹û�а");
				return;
			}
			///�ذ��ں��ý�ɫ����������˰�Ϊ������
			if(strcmp(szName,pUser->GetName())!=0)
			{
				CMsgSyndicate msg;
				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
				return;
			}	
			if (pUser->QuerySynRank() != RANK_SUBLEADER)
			{
				pUser->SendSysMsg("��ûȨ�ޣ�");
				return;
			}
			if(pUser->QuerySynRank() == RANK_LEADER)
			{
				pUser->SendSysMsg("�������ܵ����Լ���");
				return;
			}
			CUserPtr pThisUser = UserManager()->GetUser(pSyn->GetLeaderID());
			if(pThisUser)
			{
				pUser->SendSysMsg("�����������첻���߲��ܵ�������");
				return;
			}
			stTempUser* pTempData = UserManager()->QueryTempUser(pSyn->GetLeaderID());
			if(!pTempData)
				return;
			DWORD passdays=pUser->GetPassedTime(pTempData->Exit_Time,3);
			if (passdays < 3)
			{
				pUser->SendSysMsg("�����������첻���߲��ܵ�������");
				return;
			}
			OBJID idoldleader=pSyn->GetLeaderID();
			if (pUser->GetSynCon() >= 250)
			{
				CMsgSynMemberInfo msg;
				if(pUser->QuerySynAttr()->SetIdRank(idoldleader,pSyn->GetID(),RANK_NORMAL,true))
				{
					if (msg.Create(SynMemberSetRank,2,2))
					{
						msg.Append(pSyn->GetID(),"",0,0,0,RANK_NORMAL,0,0,0,0,0,0,0);///����Ϊ�˲��޸�Э�飬ֻ�ж��һ����Ч������������ID
						msg.Append(idoldleader,"",0,0,0,RANK_NORMAL,0,0,0,0,0,0,0);
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
						CMsgSynMemberInfo msgtoc;
						if (msgtoc.Create(SetRank,1,1))
						{
							msgtoc.Append(idoldleader,"",0,0,0,RANK_NORMAL,0,0,0,0,0,0,0);
							pSyn->BroadcastSynMsg(&msgtoc);
						}
					}
					else
						LogSave("ERROR: ������Ϣ����! idsyn=%u,iduser=%u,idleader=%u",pSyn->GetID(),pUser->GetID(),pSyn->GetLeaderID());
					if (pUser->QuerySynAttr()->SetIdRank(pUser->GetID(),pSyn->GetID(),RANK_LEADER,true))
					{
						if (msg.Create(SynMemberSetRank,2,2))
						{
							msg.Append(pSyn->GetID(),"",0,0,0,RANK_LEADER,0,0,0,0,0,0,0);///����Ϊ�˲��޸�Э�飬ֻ�ж��һ����Ч������������ID
							msg.Append(pUser->GetID(),"",0,0,0,RANK_LEADER,0,0,0,0,0,0,0);
							MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msg,(OBJID)0);
							CMsgSynMemberInfo msgtoc;
							if (msgtoc.Create(SetRank,1,1))
							{
								msgtoc.Append(pUser->GetID(),"",0,0,0,RANK_LEADER,0,0,0,0,0,0,0);
								pSyn->BroadcastSynMsg(&msgtoc);
							}
						}
						pUser->SetSynCon(pUser->GetSynCon()-250);
						pUser->SendSysMsg("�����ɹ�");
						CMsgSynInfo msgup;
						if(msgup.CreateNew(pSyn,pUser))
							pSyn->BroadcastSynMsg(&msgup);
					}
					else
						LogSave("ERROR: ����������! idsyn=%u,iduser=%u,idleader=%u",pSyn->GetID(),pUser->GetID(),pSyn->GetLeaderID());
				}
				else
					pUser->SendSysMsg("����ʧ��!");
			}
			else
			{
				pUser->SendSysMsg("�ﹱ����");
			}
		}
		break;
	case SYN_ANNONCE:
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
				return;
			
			///�ذ��ں��ý�ɫ����������˰�Ϊ������
			if(strcmp(szName,pUser->GetName())!=0)
			{
				CMsgSyndicate msg;
				msg.Create(m_pInfo->usAction,pSyn->GetID(),pUser->GetName(),m_pInfo->dwData,szWord);
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
				return;
			}			

			int nMyRank = pUser->QuerySynRank();
			if(nMyRank != RANK_LEADER && nMyRank != RANK_SUBLEADER)
			{
				pUser->SendSysMsg("���Ȩ�޲���");
				return;
			}
			
			pSyn->SetStr(SYNDATA_ANNOUNCE,szWord,256,true);


			CMsgTalk	msg;
			IF_OK (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szWord, NULL, 0xFFFFFF, _TXTATR_SYNANNOUNCE))
				pSyn->BroadcastSynMsg(&msg);

			pUser->SendSysMsg("�޸ĳɹ�");

			///�޸Ĺ���ʱ����������
			m_pInfo->idTarget=pSyn->GetID();
			CMsgSyndicate msgSyndicate;
			msgSyndicate.Create(SYN_ANNONCE_OTHERLINE,m_pInfo->idTarget,"",0,szWord);
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msgSyndicate,0);
		}
		break;
// 	case	SYN_UPLEV:
// 		{
// 			///�ذ��ں��ý�ɫ����������˰�Ϊ������
// 			CSyndicate* pSyn = pUser->GetSyndicate();
// 			if(!pSyn)
// 				return;
// 			if(strcmp(szName,pUser->GetName())!=0)
// 			{
// 				CMsgSyndicate msg;
// 				msg.Create(m_pInfo->usAction,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData);
// 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pSyn->GetID());
// 				return;
// 			}
// 			int nRank = pUser->QuerySynRank();
// 			if (nRank < RANK_SUBLEADER)
// 			{
// 				pUser->SendSysMsg("����Ȩ�޲��㣡");
// 				return;
// 			}
// 			if(pSyn->UpLevel(pUser, m_pInfo->dwData))
// 			{
// 				CMsgSyndicate msgSyndicate;
// 				m_pInfo->idTarget = pSyn->GetID();
// 				msgSyndicate.Create(SYN_UPLEV_OTHERLINE,m_pInfo->idTarget,pUser->GetName(),m_pInfo->dwData,szWord);
// 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msgSyndicate,0);
// 			}
// 			else
// 				pUser->SendSysMsg("����ʧ�ܣ�");
// 
// 		}
// 		break;

	case	SYN_CONTRIBUTE:
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
				return;
			UINT	Addup	= m_pInfo->idTarget / 10;
			int		Endflag	= m_pInfo->idTarget % 10;
			if (m_pInfo->dwData)
			{
				pUser->ConItem(Addup, Endflag);
			}
			else
			{
				pUser->ConMoney(Addup, true, Endflag);
			}
		}
		break;
	case	SYN_INVITE:
		{
			CSyndicate* pSyn = pUser->GetSyndicate();
			if(!pSyn)
				return;
			if (pUser->GetSynRank() < RANK_FEELLEADER1)
			{
				return;
			}
			CUser* pTarget	= UserManager()->GetUser(szName);
			if (pTarget)
			{
				return;
			}
			else
			{
				CMsgSyndicate msgSyndicate;
				m_pInfo->idTarget = pTarget->GetID();
				msgSyndicate.Create(SYN_INVITE_OTHERLINE ,m_pInfo->idTarget ,pUser->GetName() ,pSyn->GetID() ,szWord);
				pUser->SendSysMsg("���������Ѿ���������ȴ��Է���Ӧ��");
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&msgSyndicate,0);
			}
		}
		break;
	default:
		ASSERT(!"switch MsgSyndicate");
		break;
	}

	}
	catch(...)
	{	
		::LogSave("MsgSyndicate error: %d,%d,%d",m_pInfo->usAction,m_pInfo->idTarget,m_pInfo->dwData);//20061025����.������.�Լ��ֶ����⿪����
	}
}
//--------------------------------------------------
//20070403����:��������Ŀ������
BOOL CMsgSyndicate::Create(unsigned short usAction, OBJID idTarget, const char *szTargetName,unsigned short dwData,const char *szWords)
{
	// check param
	if(usAction == ACTION_NONE) //  || idTarget == ID_NONE
	{
		ASSERT(!"CMsgSyndicate::Create()");
		return false;
	}

	// init
	this->Init();
	
	m_pInfo->usAction	=usAction;
	m_pInfo->idTarget	=idTarget;
	m_pInfo->dwData		=dwData;

	m_StrPacker.AddString( szTargetName );
	m_StrPacker.AddString(	szWords		);
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();
	m_unMsgType	=_MSG_SYNDICATE;


	return true;
}

BOOL CMsgSyndicate::Create(unsigned short usAction, unsigned short dwData, OBJID idTarget, char *szBuf)
{
	if(usAction == ACTION_NONE) //  || idTarget == ID_NONE
	{
		ASSERT(!"CMsgSyndicate::Create()");
		return false;
	}
	
	// init
	this->Init();
	
	m_pInfo->usAction	=usAction;
	m_pInfo->idTarget	=idTarget;
	m_pInfo->dwData		=dwData;
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_SYNDICATE;
	return true;
}
