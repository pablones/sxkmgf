// MsgFriend.cpp: interface for the CMsgFriend class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgFriend.h"
#include "../MapGroup.h"
#pragma	warning(disable:4786)
#include "MsgFriend.h"
#include "MsgPlayer.h"
#include "I_MessagePort.h"
#include "MsgUserAttrib.h"
#include "SKPostOffice.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgFriend::CMsgFriend()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;

	m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE-sizeof(MSG_Info)+1);
}

CMsgFriend::~CMsgFriend()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgFriend::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_FRIEND != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
//20061012����.������������
BOOL	CMsgFriend::Create	(int nAction, 
				 OBJID idFriend, 
				 LPCTSTR szName, 
				 short bOnline/* = false*/, 
				 DWORD nLookFace/*=0*/, 
				 int nLevel/*=0*/,
				 unsigned int profession/*ְҵ=0*/,
				 unsigned int friendship/*�����=0*/,
				 unsigned int nSex,
				 unsigned int nFriendType,
				 LPCTSTR szFeel,//[2010-06-05 goto]���Ӻ�������
				 int nRelation,//[2010-06-05 goto]��ϵ,ʦͽ������...
				 LPCTSTR szFaction,//����
				 LPCTSTR szTitle,//�ƺ�
				 LPCTSTR szMateName,//��ż
				 LPCTSTR szTeamName,//����
				 OBJID idMap,//λ��
				 LPCTSTR szServerName//��·
				 )
{
	//20070528����:��ǿ��־
	//CHECKF(idFriend && nAction && szName);
	//IF_NOT(idFriend && nAction && szName)
	IF_NOT(nAction && szName)
	{
		char szLog[512];
		sprintf(szLog,"nAction:%d,idFriend:%d.",nAction,idFriend);
		return false;
	}
	
	// init
	this->Init();
	// fill info now
	m_pInfo->idFriend		= idFriend;
	m_pInfo->dwLookFace		= nLookFace;
	m_pInfo->ucAction		= nAction;
	m_pInfo->ucOnlineFlag	= bOnline;
	m_pInfo->ucLevel		= nLevel;
	//20061012����.������������
	//------------------------------
	m_pInfo->ucProfession	= profession;
	m_pInfo->ucFriendship	= friendship;
	m_pInfo->ucSex          = nSex;
	//------------------------------
	m_pInfo->ucFriendType	= nFriendType;
	m_pInfo->ucRelation		= nRelation;
	SafeCopy(m_pInfo->szName, szName, _MAX_NAMESIZE);

	BOOL bSucMake	=true;
	if(szFeel)//[2010-06-05 goto]��������
		bSucMake &= m_StrPacker.AddString(szFeel);
	else
		bSucMake &= m_StrPacker.AddString("��");
	if(szFaction)//����
		bSucMake &= m_StrPacker.AddString(szFaction);
	else
		bSucMake &= m_StrPacker.AddString("��");
	if(szTitle)//�ƺ�
		bSucMake &= m_StrPacker.AddString(szTitle);
	else
		bSucMake &= m_StrPacker.AddString("��");
	if(szMateName)//��ż
		bSucMake &= m_StrPacker.AddString(szMateName);
	else
		bSucMake &= m_StrPacker.AddString("��");
	if(szTeamName)//����
		bSucMake &= m_StrPacker.AddString(szTeamName);
	else
		bSucMake &= m_StrPacker.AddString("��");
	if(idMap && szServerName && strlen(szServerName)>0)//λ��,��ͼ����·Ҫ�оͶ���
	{
		char szMap[20];
		sprintf(szMap,"%s_%u", strcmp(szServerName,"����")==0?"ר��":szServerName, idMap);
		bSucMake &= m_StrPacker.AddString(szMap);
	}
	else
		bSucMake &= m_StrPacker.AddString("δ֪");

	m_unMsgType	=_MSG_FRIEND;
	m_unMsgSize	=sizeof(MSG_Info)-1+m_StrPacker.GetSize();

	return bSucMake;
}

//////////////////////////////////////////////////////////////////////
void CMsgFriend::Process(void *pInfo)
{
#ifdef _MSGDEBUG
	::LogMsg("Process CMsgFriend, id:%u", m_pInfo->id);
#endif

	char szDebugAction[32];
    char szDebugStep[32];
  // try
   {
	   sprintf(szDebugAction,"ucAction:%d ",m_pInfo->ucAction);


	CUserPtr pUser = UserManager()->GetUser(this);
	DEBUG_TRY
	if (!pUser)
	{
		if(IsNpcMsg())
			return;

		// TransmitMsg
		switch(m_pInfo->ucAction)
		{
		case _FRIEND_GETINFO:
			{
				OBJID idFriend = m_pInfo->idFriend;
				CUser* pTarget = UserManager()->GetUser(idFriend);
				if(pTarget)
				{
					sprintf(szDebugStep,"%d",1);
					//20061020����.����ְҵ----------------
					IF_OK(Create(_FRIEND_ONLINE, 
						idFriend, 
						pTarget->GetName(), 
						ONLINE_STATUS, 
						pTarget->GetLookFace(), 
						pTarget->GetLev(),
						pTarget->GetProfession(),
						0,
						pTarget->GetSex() ,m_pInfo->ucFriendType))
					//IF_OK(Create(_FRIEND_ONLINE, idFriend, pTarget->GetName(), ONLINE_STATUS, pTarget->GetLookFace(), pTarget->GetLev()))
						SendMsg(this);
					//-------------------------------------
					sprintf(szDebugStep,"%d",2);
				}
			}
			break;
		case _ENEMY_ADD:
			return;//goto ����δʹ�õĹ��ܣ�������Ϣ�����·���������
			{
				OBJID idFriend = m_pInfo->idFriend;
				CUser* pTarget = UserManager()->GetUser(idFriend);
				if(pTarget)
				{
					sprintf(szDebugStep,"%d",1);
					IF_OK(Create(_ENEMY_ONLINE, idFriend, m_pInfo->szName, ONLINE_STATUS, pTarget->GetLookFace(), pTarget->GetLev(), pTarget->GetProfession(), 0, pTarget->GetSex()))
						SendMsg(this);
					sprintf(szDebugStep,"%d",2);
				}
			}
			break;
		case _FRIEND_BREAK:
			return;//goto ����δʹ�õĹ��ܣ�������Ϣ�����·���������
			{
				OBJID idFriend = m_pInfo->idFriend;
				CUser* pTarget = UserManager()->GetUser(idFriend);
				if(pTarget)
				{
					sprintf(szDebugStep,"%d",1);
					pTarget->DelFriend(m_pInfo->idFriend);
					sprintf(szDebugStep,"%d",2);
					if (pUser)
						pUser->DelBeFriend(pTarget->GetID());//�������[2008/12/17]
					sprintf(szDebugStep,"%d",3);
				}
			}
			break;
		}

		return;
	}
	DEBUG_CATCH2("MsgFriendChannel:%d ERROR! (When !pUser)", m_pInfo->ucAction);

// 	if (!pUser->IsAlive())
// 	{
// 		pUser->SendSysMsg(g_objStrRes.GetStr(12069));//STR_DIE
// 		return;
// 	}
	DEBUG_TRY
	switch(m_pInfo->ucAction)
	{
	case _FRIEND_ADDSUCCESS_PASSIVE:
		{
			///�����ӳɹ�ֻ��Ҫ�ӱ������Ѽ���
			pUser->AddBeFriend(0,GetTransData(),m_pInfo->ucFriendType,m_pInfo->szName);//���A�Ѿ���B�ĺ��ѣ�����Ҫ��֪ͨB
		}
		break;
	case _FRIEND_ADDSUCCESS:///OK,�����ﱻ����ʱֻ�п������յ���ķ����������ĺ�����ӳɹ���֪ͨ
		{
			///����ͬ�����Ϊ�����ˣ����ڱ��ؿ����Ƚ�����Ϊ���ߺ���

			CMsgFriend pFriendMsg;
			OBJID dwFriendID = pUser->AddLeaveFriend(m_pInfo->szName, m_pInfo->ucFriendType, &pFriendMsg);
			if(dwFriendID)
			{
				m_pInfo->idFriend=dwFriendID;
				pUser->SendMsg(this);

				///���߼Ӻ���ʱ���ϵ������������ѯ��������״̬
// 				pFriendMsg.SetTransData(pUser->GetID());
// 				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&pFriendMsg);
				
				///�������߳��鷢��Ϣ�������и������߳���
				//					pUser->SendMsg(&pFriendMsg);
				char szTemp[128];
				sprintf(szTemp,"<font color=\"#ffff00\">���</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">Ϊ���ѳɹ�!</font>",m_pInfo->szName);
				pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
			}
			else
			{
				LOGERROR("_FRIEND_WHETHER_ADDFRIEND �����ߺ���ʧ��");
				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_ADDFAIL,m_pInfo->idFriend,m_pInfo->szName,0,0,0,0,0,0,0,NULL,0))
					pUser->SendMsg(&pMsg);
			}

// 			CMsgFriend pMsg;
// 			pUser = UserManager()->GetUser(m_pInfo->idFriend);
// 			
// 			if (pUser && pUser->AddBeFriend(0,GetTransData(),m_pInfo->ucFriendType) && m_pInfo->ucFriendType!=TEMP_FRIEND_GROUP)//���A�Ѿ���B�ĺ��ѣ�����Ҫ��֪ͨB
// 			{
// 				if (pUser->GetFriend(GetTransData()))
// 				{
// 					char szTemp[128];
// //					sprintf(szTemp,"<font color=\"#ffff00\">�û�</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">�����Ϊ�˺���!</font>",pUser->GetName());
// 					sprintf(szTemp,"<font color=\"#ffff00\">�û�</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">�����Ϊ�˺���!</font>",m_pInfo->szName);
// 					pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
// 				}
// 				else
// 				{
// 				//	pMsg.Create(_FRIEND_ADDFRIEND,pUser->GetID(),pUser->GetName(),0,0,0,0,0,0,0,NULL,0);
// 					pMsg.Create(_FRIEND_ADDFRIEND,GetTransData(),m_pInfo->szName,0,0,0,0,0,0,0,NULL,0);
// 					pUser->SendMsg(&pMsg);
// 				}
// 			}
			
		}
		break;
	case _FRIEND_CONTACTHISTORY_APPLY:
		return;//goto ����δʹ�õĹ��ܣ�������Ϣ�����·���������		
	case _FRIEND_ADDFRIEND://��A���͵�����  [2010-06-07 goto]����ҪB��ͬ�⣬ֱ�Ӽ�Ϊ���ѣ�ͬʱ֪ͨB
		{			
			if (pUser->GetID()==m_pInfo->idFriend || stricmp(pUser->GetName(),m_pInfo->szName)==0)
				return;
			if (stricmp("ϵͳ",m_pInfo->szName)==0)
				return;
			if( m_pInfo->ucFriendType>MAX_GROUP || m_pInfo->ucFriendType<=0 )//���ѷ���,1-4�飬5��ʱ���ѣ�6������
				return;

			if(m_pInfo->ucFriendType!=TEMP_FRIEND_GROUP && pUser->GetFriendGroupAmount(m_pInfo->ucFriendType)>=MAX_GROUP_FRIEND )
			{
				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_ADDFAIL,m_pInfo->idFriend,m_pInfo->szName,0,0,0,0,0,0,0,NULL,0))
					pUser->SendMsg(&pMsg);

				pUser->SendSysMsg("�˺��ѷ����Ѵﵽ��������");
				return;
			}

			CFriend* pFriend= pUser->GetFriendByName(m_pInfo->szName);
			if (pFriend)//B�Ѿ���A�ĺ���
			{
				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_ADDFAIL,m_pInfo->idFriend,m_pInfo->szName,0,0,0,0,0,0,0,NULL,0))
					pUser->SendMsg(&pMsg);
				char szTemp[128];
				sprintf(szTemp,"%s�Ѿ�����ĺ���",m_pInfo->szName);
				pUser->SendSysMsg(szTemp);
				return;
			}
			CUserPtr	pTarget = NULL;
			pTarget = UserManager()->GetUser(m_pInfo->szName);
			if (pTarget)
			{
				CMsgFriend pMsg;
				if(m_pInfo->ucFriendType==TEMP_FRIEND_GROUP)
				{
					pMsg.Create(_FRIEND_ADDSUCCESS,
						pTarget->GetID(),
						m_pInfo->szName,
						ONLINE_STATUS,
						pTarget->GetLookFace(),
						pTarget->GetLev(),
						pTarget->GetProfession(),
						0,//friendship
						pTarget->GetSex(),
						m_pInfo->ucFriendType,//frinedgroup
						pTarget->GetFeel(),
								 0);
					pUser->SendMsg(&pMsg);
					pUser->AddFriend(pTarget->GetID(), m_pInfo->szName,m_pInfo->ucFriendType,pTarget->GetMate());
					pTarget->AddBeFriend(0,pUser->GetID(),m_pInfo->ucFriendType,pUser->GetName());
					pUser->SetTempFriend(true);
                }
				else
				{
					if (!pTarget->IsAgreeFriendApply())
					{
						pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է��Ѵ򿪾ܾ��Ӻ��ѿ���");//STR_TEAM_FULL
					   return;
					}
					pMsg.Create(_FRIEND_ADDFRIEND,pUser->GetID(),pUser->GetName(),0,0,0,0,0,0,m_pInfo->ucFriendType,NULL,0);
                    pTarget->SendMsg(&pMsg);
				}
				
			} 
			else
			{ 
				CMsgFriend msg;
				if(m_pInfo->ucFriendType==TEMP_FRIEND_GROUP)
				{
// 					pMsg.Create(_FRIEND_ADDSUCCESS,
// 						pTarget->GetID(),
// 						m_pInfo->szName,
// 						ONLINE_STATUS,
// 						pTarget->GetLookFace(),
// 						pTarget->GetLev(),
// 						pTarget->GetProfession(),
// 						0,//friendship
// 						pTarget->GetSex(),
// 						m_pInfo->ucFriendType,//frinedgroup
// 						pTarget->GetFeel(),
// 						0);
// 					pUser->SendMsg(&pMsg);
// 					pUser->AddFriend(pTarget->GetID(), m_pInfo->szName,m_pInfo->ucFriendType,pTarget->GetMate());
//					OBJID dwFriendID = pUser->AddLeaveFriend(m_pInfo->szName, m_pInfo->ucFriendType, &pFriendMsg);
                }
				else
				{
					msg.Create(_FRIEND_ADDFRIEND,pUser->GetID(),m_pInfo->szName,0,0,0,0,0,0,m_pInfo->ucFriendType,NULL,0);
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
				}
				//pUser->SendSysMsg("�Է������ߣ����ܼ�Ϊ����");
			}
		}
		break;

    case _FRIEND_WHETHER_ADDFRIEND:
		{
			if(!pUser)
				return;
			if (stricmp(pUser->GetName(),m_pInfo->szName)==0)
				return;
			if(m_pInfo->ucSex<1||m_pInfo->ucSex>MAX_GROUP)
				return;
			
			DWORD optnumber=m_pInfo->dwLookFace;
			if(optnumber<0||optnumber>3)
				return;
            CUserPtr	pTarget = NULL;
			pTarget = UserManager()->GetUser(m_pInfo->szName);
			if (!pTarget)
			{
				switch(optnumber)
				{
					case 1:
					case 2:
					{
						CMsgFriend pMsg;
						pMsg.Create(_FRIEND_ADDSUCCESS,
							pUser->GetID(),
							m_pInfo->szName,
							ONLINE_STATUS,
							pUser->GetLookFace(),
							pUser->GetLev(),
							pUser->GetProfession(),
							0,//friendship
							pUser->GetSex(),
							m_pInfo->ucSex,//frinedgroup
							pUser->GetFeel(),
						0);//relation
						///���߼Ӻ���ʱ���ϵ������������ѯ��������״̬
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&pMsg,OBJID(0));
						
						///�����ѯһ�����ݿ⽫�û���ת���û�id
						if(pUser->AddBeFriend(0,m_pInfo->idFriend,m_pInfo->ucSex,m_pInfo->szName))//���A�Ѿ���B�ĺ��ѣ�����Ҫ��֪ͨB
						{
							char szTemp[128]="";
							sprintf(szTemp,"<font color=\"#ffff00\">�û�</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">�����Ϊ�˺���!</font>",m_pInfo->szName);
							pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
						}

						///ͬ��Ӻ���ͬʱ�ӶԷ�Ϊ����
						if(optnumber==2)
						{
							if(m_pInfo->ucFriendType<1||m_pInfo->ucFriendType>MAX_GROUP)
								return;
							///��ֹ�ظ���Ӻ���
							if(pUser->GetFriend(m_pInfo->idFriend))
							{
								char szTemp[128]="";
								sprintf(szTemp,"<font color=\"#ffff00\">�û�</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">�Ѿ�����ĺ���,�����ظ����!</font>",m_pInfo->szName);
								pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
								break;
							}
							CMsgFriend pFriendMsg;
							OBJID dwFriendID = pUser->AddLeaveFriend(m_pInfo->szName, m_pInfo->ucFriendType, &pFriendMsg);
							if(dwFriendID)
							{
								pFriendMsg.m_pInfo->idFriend=dwFriendID;

								
// 								pUser->SendMsg(&pFriendMsg);
								pFriendMsg.m_pInfo->ucAction=_FRIEND_ADDSUCCESS_PASSIVE;
								///���߼Ӻ���ʱ���ϵ������������ѯ��������״̬
								pFriendMsg.SetTransData(pUser->GetID());
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&pFriendMsg);
								
								///�������߳��鷢��Ϣ�������и������߳���
								//					pUser->SendMsg(&pFriendMsg);
								char szTemp[128];
								sprintf(szTemp,"<font color=\"#ffff00\">���</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">Ϊ���ѳɹ�!</font>",m_pInfo->szName);
								pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
							}
							else
								LOGERROR("_FRIEND_WHETHER_ADDFRIEND �����ߺ���ʧ��");

							
						}
							///�������߳��鷢��Ϣ�������и������߳���
							//					pUser->SendMsg(&pFriendMsg);
// 							char szTemp[128];
// 							sprintf(szTemp,"<font color=\"#ffff00\">���</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">Ϊ���ѳɹ�!</font>",m_pInfo->szName);
// 							pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
					}
						break;
// 					case 2:
// 						{
// 
// 						}
// 						break;
					case 3:///�ܾ�����
						{
							CMsgFriend pMsg;
							pMsg.Create(_FRIEND_ADDFAIL,pUser->GetID(),m_pInfo->szName,0,0,0,0,0,0,0,NULL,0);
							MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&pMsg,OBJID(0));
//							pTarget->SendMsg(&pMsg);
// 							char szTemp[128];
// 							sprintf(szTemp,"<font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">�ܾ������ĺ�������!</font>",pUser->GetName());
// 							pTarget->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
						}
						break;
					default:
						break;
				}
				return;
			}
			if (optnumber==1)
			{
				if(pTarget->AddFriend(pUser->GetID(), pUser->GetName(),m_pInfo->ucSex,pUser->GetMate()))
				{ CMsgFriend pMsg;
				  pMsg.Create(_FRIEND_ADDSUCCESS,
						pUser->GetID(),
						pUser->GetName(),
						ONLINE_STATUS,
						pUser->GetLookFace(),
						pUser->GetLev(),
						pUser->GetProfession(),
						0,//friendship
						pUser->GetSex(),
						m_pInfo->ucSex,//frinedgroup
						pUser->GetFeel(),
						0);//relation
				  pTarget->SendMsg(&pMsg);
				  char szTemp[128];
				  sprintf(szTemp,"<font color=\"#ffff00\">���</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">Ϊ���ѳɹ�!</font>",pUser->GetName());
				  pTarget->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
				  pUser->AddBeFriend(0,m_pInfo->idFriend,m_pInfo->ucSex,pTarget->GetName());
                }
				else
				{
					CMsgFriend pMsg;
					pMsg.Create(_FRIEND_ADDFAIL,pUser->GetID(),pUser->GetName(),0,0,0,0,0,0,0,NULL,0);
					pTarget->SendMsg(&pMsg);
					return;
				}
			}
			else if (optnumber==2)
			{  
				///��ֹ�ظ���Ӻ���
				if(m_pInfo->ucFriendType<1||m_pInfo->ucFriendType>MAX_GROUP)
					return;
				if(pUser->GetFriend(m_pInfo->idFriend))
				{
					char szTemp[128]="";
					sprintf(szTemp,"<font color=\"#ffff00\">�û�</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">�Ѿ�����ĺ���,�����ظ����!</font>",m_pInfo->szName);
					pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
					break;
				}
				if(pTarget->AddFriend(pUser->GetID(), pUser->GetName(),m_pInfo->ucSex,pUser->GetMate()))
				{ CMsgFriend pMsg;
				  pMsg.Create(_FRIEND_ADDSUCCESS,
					pUser->GetID(),
					pUser->GetName(),
					ONLINE_STATUS,
					pUser->GetLookFace(),
					pUser->GetLev(),
					pUser->GetProfession(),
					0,//friendship
					pUser->GetSex(),
					m_pInfo->ucSex,//frinedgroup
					pUser->GetFeel(),
					0);//relation
				pTarget->SendMsg(&pMsg);
				char szTemp[128];
				sprintf(szTemp,"<font color=\"#ffff00\">���</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">Ϊ���ѳɹ�!</font>",pUser->GetName());
				pTarget->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
				pUser->AddBeFriend(0,m_pInfo->idFriend,m_pInfo->ucSex,pTarget->GetName());
                }
				else
				{
					CMsgFriend pMsg;
					pMsg.Create(_FRIEND_ADDFAIL,pUser->GetID(),pUser->GetName(),0,0,0,0,0,0,0,NULL,0);
					pTarget->SendMsg(&pMsg);
					return;
				}
			   if(pUser->AddFriend(pTarget->GetID(), m_pInfo->szName,m_pInfo->ucFriendType,pTarget->GetMate()))
				{ CMsgFriend pMsg;
				pMsg.Create(_FRIEND_ADDSUCCESS,
					pTarget->GetID(),
					m_pInfo->szName,
					ONLINE_STATUS,
					pTarget->GetLookFace(),
					pTarget->GetLev(),
					pTarget->GetProfession(),
					0,//friendship
					pTarget->GetSex(),
					m_pInfo->ucFriendType,//frinedgroup
					pTarget->GetFeel(),
					0);//relation
				pUser->SendMsg(&pMsg);
				char szTemp[128];
				sprintf(szTemp,"<font color=\"#ffff00\">���</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">Ϊ���ѳɹ�!</font>",m_pInfo->szName);
				pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
				pTarget->AddBeFriend(0,pUser->GetID(),m_pInfo->ucFriendType,pUser->GetName());
                }
				else
				{
					CMsgFriend pMsg;
					pMsg.Create(_FRIEND_ADDFAIL,pTarget->GetID(),m_pInfo->szName,0,0,0,0,0,0,0,NULL,0);
					pUser->SendMsg(&pMsg);
					return;
				}
			}
			else if (optnumber==3)
			{
				CMsgFriend pMsg;
			    pMsg.Create(_FRIEND_ADDFAIL,pUser->GetID(),pUser->GetName(),0,0,0,0,0,0,0,NULL,0);
				pTarget->SendMsg(&pMsg);
                char szTemp[128];
				sprintf(szTemp,"<font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">�ܾ������ĺ�������!</font>",pUser->GetName());
				pTarget->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
			}
		}
		break;

	case _FRIEND_CHANGEGROUP://[2010-06-07 goto]�������ѷ���
		{
			if( m_pInfo->ucFriendType>(MAX_GROUP+1) || m_pInfo->ucFriendType<=0 )//���ѷ���,1-4�飬5��ʱ���ѣ�6������
				return;
			
			if( pUser->GetFriendGroupAmount(m_pInfo->ucFriendType)>=MAX_GROUP_FRIEND )
			{
				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_CHANGEFAIL,m_pInfo->idFriend,m_pInfo->szName,0,0,0,0,0,0,0,NULL,0))
					pUser->SendMsg(&pMsg);
				
				pUser->SendSysMsg("�˺��ѷ����Ѵﵽ��������");
				return;
			}

			CFriend* pFriend= pUser->GetFriendByName(m_pInfo->szName);
			if(!pFriend)
				return;
			
			unsigned int uoldType = pFriend->GetFriendType();
			//pFriend->SetFriendType(m_pInfo->ucFriendType);//�޸ĺ��ѷ���
			if( pUser->MoveFriend(pFriend,m_pInfo->ucFriendType) )//�޸ĺ��ѷ���
			{
				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_CHANGEGROUP,m_pInfo->idFriend,m_pInfo->szName,0,0,0,0,uoldType,0,m_pInfo->ucFriendType,NULL,0))
					pUser->SendMsg(&pMsg);
			}
			CUserPtr	pTarget = NULL;
			pTarget = UserManager()->GetUser(m_pInfo->szName);
			if(pTarget)
			{
				CFriend* pTargetFriend = pTarget->GetBeFriend(pUser->GetID());
				if (pTargetFriend)
					pTargetFriend->SetFriendType(m_pInfo->ucFriendType);
			}
		}
		break;
	case _FRIEND_DELFRIEND://[2010-06-08 goto]ɾ������
		{
			CFriend* pFriend= pUser->GetFriendByName(m_pInfo->szName);
			if(!pFriend)///����ڱ����Ҳ����������,�ͳ���ɾ����������
			{
				pUser->DelBeFriend(GetTransData());
			}
			if (!pFriend)
				return;

			OBJID nFriendID=pFriend->GetFriendID();
			int nFriendType=pFriend->GetFriendType();
			if(pUser->DelFriend(pFriend->GetFriendID()))
			{
				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_DELFRIEND,nFriendID,m_pInfo->szName,0,0,0,0,0,0,nFriendType,NULL,0))
					pUser->SendMsg(&pMsg);
			}
			else
			{
				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_DELFRIENDFAIL,pFriend->GetFriendID(),pFriend->GetFriendName(),0,0,0,0,0,0,0,NULL,0))
					pUser->SendMsg(&pMsg);
				return;
			}

			CUserPtr	pTarget = NULL;
			pTarget = UserManager()->GetUser(m_pInfo->szName);
			if (pTarget)
			{
				pTarget->DelBeFriend(pUser->GetID());
			}
			else
			{
				///�Ҳ�������ʱ����ת����ϵ����������
				SetTransData(pUser->GetID());
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this);

//				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(this, nFriendID);
			}
		}
		break;
	case _FRIEND_SETFEEL://[2010-06-08 goto]�޸�����
		{
			if(m_pInfo->szName)
			{
				if(strlen(m_pInfo->szName)>_MAX_FEELSIZE)
					return;

				pUser->SetFeel(m_pInfo->szName);

				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_SETFEEL,pUser->GetID(),pUser->GetName(),ONLINE_STATUS,0,0,0,0,0,0,m_pInfo->szName,0))
					pUser->SendMsg(&pMsg);

				CMsgPlayer msgPlayer;
				if (msgPlayer.Create(pUser->QueryRole()))
					pUser->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
			}
		}
		break;
	case _FRIEND_QUERYFRIENDINFO:///�յ����������Ĳ�ѯ��Ϣ
		{
			///Ŀǰ��Чʱ�Ŵ���
			if(pUser && pUser->CanBeSee()==false)
				return;

			char szUserTitle[10];
			sprintf(szUserTitle, "%u", pUser->GetUseTitle());
			int nFriendShip=0,nMapID=0;
			char szTeamName[32] = "";
			sprintf(szTeamName, "%s_%d", "��", m_pInfo->ucSex%10);
			CTeam* pTeam = pUser->GetTeam();
			if(pTeam)
				sprintf(szTeamName, "%s_%d", pTeam->GetTeamName(), m_pInfo->ucSex%10);//��������߿ͻ����Ƿ�Ҫ������ϸ���ϴ��ڣ����������ʱ������ϸ��Ϣ����Ҫ����
			
			CSyndicate* pSyndicate = pUser->GetSyndicate();
			CFriend* pFriend= pUser->GetFriend(m_pInfo->dwLookFace);
			if(pFriend)
				nFriendShip=pFriend->GetFriendShip();
			if(pUser->GetMap())
				nMapID=pUser->GetMap()->GetDocID();
			CMsgFriend pMsg;
			if (pMsg.Create(_FRIEND_REPLYFRIENDINFO,
				pUser->GetID(),
				pUser->GetName(),
				pUser->GetProfession(2),//��ְҵ
				pUser->GetLookFace(),//lookface
				pUser->GetLev(),//����ȼ�
				pUser->GetProfession(1),//��ְҵ
				nFriendShip,
				pUser->GetJobLev(1)*10,//����ȥ��ʱ����10,����Ҫ��һ��
				pUser->GetJobLev(2),//��ְҵ�ȼ�
				pUser->GetFeel(),
				0,//��ϵ
				pSyndicate==NULL? "��" : pSyndicate->GetStr(SYNDATA_NAME),//����
				szUserTitle,//�ƺ�
				pUser->GetMate(),//��ż
				szTeamName,//����
				nMapID,//λ��
				::MapGroup(PID)->GetServerName()//��·
				))
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&pMsg,m_pInfo->dwLookFace);
		}
		break;
	case _FRIEND_GETFRIENDINFO://��ȡ���ѵ���ϸ��Ϣ
		{
			if (stricmp("ϵͳ",m_pInfo->szName)==0)
				return;
			CFriend* pFriend= pUser->GetFriendByName(m_pInfo->szName);
			CUserPtr	pTarget = NULL;
			pTarget = UserManager()->GetUser(m_pInfo->szName);

			///Ŀǰ��Чʱ�Ŵ���
			if(pTarget && pTarget->CanBeSee()==false)
				return;

			if (!pFriend)//�Ǻ���
			{
				if(!pTarget)
				{
// 					CMsgFriend pMsg;
// 					pMsg.Create(_FRIEND_GETFRIENDINFO,m_pInfo->idFriend,m_pInfo->szName,
// 						0,//��ְҵ
// 						0,//lookface
// 						0,//����ȼ�
// 						0,//��ְҵ
// 						0,
// 						0,//��ְҵ�ȼ�
// 						0,//��ְҵ�ȼ�
// 						NULL,
// 						0);//��ϵ
// 					pUser->SendMsg(&pMsg);
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(this,pUser->GetID());

//					pUser->SendSysMsg(_TXTATR_TOPSHOW, "���˲����ߣ����ڸ�����");
				}
				else
				{
					char szUserTitle[10];
					sprintf(szUserTitle, "%u", pTarget->GetUseTitle());
					
					char szTeamName[32] = "";
					sprintf(szTeamName, "%s_%d", "��", m_pInfo->ucSex%10);
					CTeam* pTeam = pTarget->GetTeam();
					if(pTeam)
						sprintf(szTeamName, "%s_%d", pTeam->GetTeamName(), m_pInfo->ucSex%10);//��������߿ͻ����Ƿ�Ҫ������ϸ���ϴ��ڣ����������ʱ������ϸ��Ϣ����Ҫ����
// 					CTeam* pTeam = pTarget->GetTeam();
// 					if(pTeam)
// 						sprintf(szTeamName,pTeam->GetTeamName());

					CSyndicate* pSyndicate = pTarget->GetSyndicate();
					UINT nMapID=0;
					if(pTarget->GetMap())
						nMapID=pTarget->GetMap()->GetDocID();
					CMsgFriend pMsg;
					if (pMsg.Create(_FRIEND_GETFRIENDINFO,
						pTarget->GetID(),
						pTarget->GetName(),
						pTarget->GetInfo()->SecJob,//��ְҵ
						pTarget->GetLookFace(),//lookface
						pTarget->GetLev(),//����ȼ�
						pTarget->GetInfo()->FirJob,//��ְҵ
						0,//�Ѻö�
						pTarget->GetInfo()->FirJobLev,//��ְҵ�ȼ�
						pTarget->GetInfo()->SecJobLev,//��ְҵ�ȼ�
						pTarget->GetFeel(),
						0,//��ϵ
						pSyndicate==NULL? "��" : pSyndicate->GetStr(SYNDATA_NAME),//����
						szUserTitle,//�ƺ�
						pTarget->GetMate(),//��ż
						szTeamName,//����
						nMapID,//λ��
						::MapGroup(PID)->GetServerName()//��·
						))
					pUser->SendMsg(&pMsg);
				}
				return;
			}
			if (!pTarget)//������ʱ������Ϣ�޷���ȡ
			{
				char szUserTitle[10];
				sprintf(szUserTitle, "%u",pFriend->GetFriendTitle());
				OBJID idSyn = pFriend->GetFriendSynID();
				char szSyn[16] = "��";
				if (idSyn!=ID_NONE)
				{
					CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
					if (pSyn)
						sprintf(szSyn, pSyn->GetStr(SYNDATA_NAME));
				}
				char szTeam[32] = "";
				sprintf(szTeam, "%s_%d", "��", m_pInfo->ucSex);//goto���ڱ����ǿ϶�û�ӵģ�����Ϊ�˴���һ����Ϣ��ȥֻ�ü�����
				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_GETFRIENDINFO,
							pFriend->GetFriendID(),
							pFriend->GetFriendName(),
							pFriend->GetFriendSecJob(),//��ְҵ
							pFriend->GetFriendLookFace(),//lookface
							pFriend->GetFriendLevel(),//����ȼ�
							pFriend->GetFriendFirJob(),//��ְҵ
							pFriend->GetFriendShip(),
							pFriend->GetFriendFirJobLev()*10+m_pInfo->ucSex,//��ְҵ�ȼ�,��λ�Ǹ��߿ͻ����Ƿ�Ҫ�����Ի���
							pFriend->GetFriendSecJobLev(),//��ְҵ�ȼ�
							pFriend->GetFriendFeel(),
							0,//��ϵ
							szSyn,//"����",
							szUserTitle,
							pFriend->GetFriendMateName(),
							szTeam))

				///���Ѳ�����ʱҲ��Ҫת������ϵ������ȥ����
				MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg2(&pMsg,pUser->GetID());
//				pUser->SendMsg(&pMsg);
			}
			else
			{
				char szUserTitle[10];
				sprintf(szUserTitle, "%u", pTarget->GetUseTitle());
				
				char szTeamName[32] = "";
				sprintf(szTeamName, "%s_%d", "��", m_pInfo->ucSex%10);
				CTeam* pTeam = pTarget->GetTeam();
				if(pTeam)
					sprintf(szTeamName, "%s_%d", pTeam->GetTeamName(), m_pInfo->ucSex%10);//��������߿ͻ����Ƿ�Ҫ������ϸ���ϴ��ڣ����������ʱ������ϸ��Ϣ����Ҫ����
// 				CTeam* pTeam = pTarget->GetTeam();
// 				if(pTeam)
// 					sprintf(szTeamName, pTeam->GetTeamName());

				CSyndicate* pSyndicate = pTarget->GetSyndicate();
				UINT nMapID=0;
				if(pTarget->GetMap())
					nMapID=pTarget->GetMap()->GetDocID();
				CMsgFriend pMsg;
				if (pMsg.Create(_FRIEND_GETFRIENDINFO,
							pTarget->GetID(),
							pTarget->GetName(),
							pTarget->GetProfession(2),//��ְҵ
							pTarget->GetLookFace(),//lookface
							pTarget->GetLev(),//����ȼ�
							pTarget->GetProfession(1),//��ְҵ
							pFriend->GetFriendShip(),
							pTarget->GetJobLev(1),//��ְҵ�ȼ�
							pTarget->GetJobLev(2),//��ְҵ�ȼ�
							pTarget->GetFeel(),
							0,//��ϵ
							pSyndicate==NULL? "��" : pSyndicate->GetStr(SYNDATA_NAME),//����
							szUserTitle,//�ƺ�
							pTarget->GetMate(),//��ż
							szTeamName,//����
							nMapID,//λ��
							::MapGroup(PID)->GetServerName()//��·
							))
				pUser->SendMsg(&pMsg);
			}
		}
		break;
	case _FRIEND_DELCONTACT://ɾ���Ѿ�����������
		{
			if(pUser->GetID()!=m_pInfo->idFriend)
				return;
			pUser->DelFriendLeaveWord(m_pInfo->szName,m_pInfo->dwLookFace);
		}
		break;
	case _FRIEND_HAVETEAM://�鿴���Ѷ�����Ϣ
		{
			CUserPtr pFriend = UserManager()->GetUser(m_pInfo->idFriend);
			if (!pFriend)
			{
				CMsgFriend msg;
				if (msg.Create(_FRIEND_HAVETEAM,0,pUser->GetName(),ONLINE_STATUS,0,0,0,0,0,0))
				{
					pUser->SendMsg(&msg);
				}
				return;
			}

			int nStep = 1;
			DEBUG_TRY
			CTeam* pTeam = pFriend->GetTeam();
			nStep = 2;
			if (!pTeam)//�����޶���
			{
				nStep = 3;
				CMsgFriend msg;
				if (msg.Create(_FRIEND_HAVETEAM,0,pFriend->GetName(),ONLINE_STATUS,0,0,0,0,0,0))
					pUser->SendMsg(&msg);
			}
			else//�����ж���
			{
				nStep = 4;
				OBJID idTeam = pTeam->GetID();
				OBJID idTeamLeader = pTeam->GetLeader();
				LPCTSTR szTeam = pTeam->GetTeamName();

				nStep = 5;
				CMsgFriend msg;
				if (msg.Create(_FRIEND_HAVETEAM,idTeam,szTeam,0,idTeamLeader,0,0,0,0,0))
					pUser->SendMsg(&msg);
			}
			DEBUG_CATCH2("_FRIEND_HAVETEAM %u",nStep);
		}
		break;
	case _FRIEND_HAVETEAM_MT://�����̲߳�����ҵķ��ؽ��
		{
// 			CUserPtr pFriend = UserManager()->GetUser(m_pInfo->idFriend);
// 			if (!pFriend)
// 			{
// 				CMsgFriend msg;
// 				if (msg.Create(_FRIEND_HAVETEAM,0,pUser->GetName(),ONLINE_STATUS,0,0,0,0,0,0))
// 					pUser->SendMsg(&msg);
// 				return;
// 			}
// 			
// 			CTeam* pTeam = pFriend->GetTeam();
// 			if (!pTeam)//�����޶���
// 			{
// 				CMsgFriend msg;
// 				if (msg.Create(_FRIEND_HAVETEAM,0,pFriend->GetName(),ONLINE_STATUS,0,0,0,0,0,0))
// 					pUser->SendMsg(&msg);
// 			}
// 			else//�����ж���
// 			{
// 				OBJID idTeam = pTeam->GetID();
// 				OBJID idTeamLeader = pTeam->GetLeader();
// 				LPCTSTR szTeam = pTeam->GetTeamName();
// 				CMsgFriend msg;
// 				msg.Create(_FRIEND_HAVETEAM,idTeam,szTeam,0,idTeamLeader,0,0,0,0,0);
// 				pUser->SendMsg(&msg);
// 			}
		}
		break;
	case _FRIEND_FEEL_SWITH:
		{
			pUser->SetFeelSwitch(m_pInfo->idFriend);
			CMsgUserAttrib msg;
			msg.Create(pUser->GetID(), _USERATTRIB_FEEL_SWITH, m_pInfo->idFriend);
			pUser->BroadcastRoomMsg(&msg, INCLUDE_SELF);
		}
		break;
	case _FRIEND_MOVETOGOODFRIENDACCEPT://Bͬ��A������
		return;//goto ����δʹ�õĹ��ܣ�������Ϣ�����·���������
		{
			if (m_pInfo->ucFriendType != 1)
				return;
			CUserPtr	pTarget = NULL;
			pTarget = UserManager()->GetUser(m_pInfo->szName);
			if (!pTarget)
				return;
			sprintf(szDebugStep,"%d",1);
			CFriend* pBeFriend = pTarget->GetFriend(pUser->GetID());//�ұ�Ȼ�Ѿ�����ĳ�ֺ���
			if (!pBeFriend)
				return;
			CFriend* pFriend= pUser->GetFriend(/*m_pInfo->idFriend*/pTarget->GetID());//���Ƿ�Ҳ���ҵ�ĳ�ֺ���
			sprintf(szDebugStep,"%d",2);
			if (!pFriend)//����
			{
				sprintf(szDebugStep,"%d",3);
				if (pUser->AddFriend(/*m_pInfo->idFriend*/pTarget->GetID(), m_pInfo->szName,m_pInfo->ucFriendType)
					&& pTarget->MoveFriend(pBeFriend,1))
				{
					sprintf(szDebugStep,"%d",4);
					//-----A����BΪ����,����B�б���A����Ϣ----------
					pFriend = pUser->GetFriend(pTarget->GetID()/*m_pInfo->idFriend�����m_pInfo->idFriend����Ϊ��*/);
					pTarget->AddBeFriend(pFriend->GetID(),pUser->GetID(),pFriend->GetFriendType(),pUser->GetName());
					sprintf(szDebugStep,"%d",5);
					//----------------------------
					if(this->Create(_FRIEND_MOVETOGOODFRIENDACCEPT, 
						pTarget->GetID(), 
						pTarget->GetName(), 
						ONLINE_STATUS, 
						pTarget->GetLookFace(), 
						pTarget->GetLev(),
						pTarget->GetProfession(),
						0,
						pTarget->GetSex(),
						m_pInfo->ucFriendType) )
						pUser->SendMsg(this);
					if(this->Create(_FRIEND_MOVETOGOODFRIENDACCEPT, 
						pUser->GetID(), 
						pUser->GetName(), 
						ONLINE_STATUS, 
						pUser->GetLookFace(), 
						pUser->GetLev(),
						pUser->GetProfession(),
						0,
						pUser->GetSex(),
						m_pInfo->ucFriendType) )
						pTarget->SendMsg(this);
					MSGBUF	szMsg;
						sprintf(szMsg,g_objStrRes.GetStr(13057), pUser->GetName(), pTarget->GetName());//STR_MAKE_FRIEND
						pUser->BroadcastRoomMsg(szMsg, true);
						sprintf(szDebugStep,"%d",6);
				}
				sprintf(szDebugStep,"%d",7);
			}
			else//��
			{
				sprintf(szDebugStep,"%d",8);
				if (pUser->MoveFriend(pFriend,1) && pTarget->MoveFriend(pBeFriend,1))
				{
					if(this->Create(_FRIEND_MOVETOGOODFRIENDACCEPT, 
						pTarget->GetID(), 
						pTarget->GetName(), 
						ONLINE_STATUS, 
						pTarget->GetLookFace(), 
						pTarget->GetLev(),
						pTarget->GetProfession(),
						0,
						pTarget->GetSex(),
						m_pInfo->ucFriendType) )
						pUser->SendMsg(this);
					if(this->Create(_FRIEND_MOVETOGOODFRIENDACCEPT, 
						pUser->GetID(), 
						pUser->GetName(), 
						ONLINE_STATUS, 
						pUser->GetLookFace(), 
						pUser->GetLev(),
						pUser->GetProfession(),
						0,
						pUser->GetSex(),
						m_pInfo->ucFriendType) )
						pTarget->SendMsg(this);
					MSGBUF	szMsg;
						sprintf(szMsg,g_objStrRes.GetStr(13057), pUser->GetName(), pTarget->GetName());//STR_MAKE_FRIEND
						pUser->BroadcastRoomMsg(szMsg, true);
				}
				sprintf(szDebugStep,"%d",9);
			}
		}
		break;
	case _FRIEND_MOVEAPPLY:
		return;//goto ����δʹ�õĹ��ܣ�������Ϣ�����·���������
		{
			CUserPtr	pTarget = NULL;
			pTarget = UserManager()->GetUser(m_pInfo->szName);
			if (!pTarget)
			{
				pUser->SendSysMsg(g_objStrRes.GetStr(28023));
				return;
			}
			sprintf(szDebugStep,"%d",1);
			CFriend* pFriend= pUser->GetFriendByName(m_pInfo->szName);
			if (pFriend == NULL || pFriend->GetFriendType() == m_pInfo->ucFriendType)
				return;
			if (m_pInfo->ucFriendType == 1)//�������ΪGOOD FRIEND,����Ҫ�����Է�ȷ��,�൱�ڷ�����һ������Ӻ�����Ϣ
			{
			}
			sprintf(szDebugStep,"%d",2);
			if (pUser->MoveFriend(pFriend,m_pInfo->ucFriendType))
			{
				sprintf(szDebugStep,"%d",3);
				if(this->Create(_FRIEND_MOVESUCESS, 
					pFriend->GetFriendID(), 
					pFriend->GetFriendName(), 
					pTarget?true:false, 
					pTarget?pTarget->GetLookFace():0, 
					pTarget?pTarget->GetLev():0,
					pTarget?pTarget->GetProfession():0,
					0,
					pTarget?pTarget->GetSex():0,
					m_pInfo->ucFriendType) )
						pUser->SendMsg(this);
				sprintf(szDebugStep,"%d",4);
			}
		}
		break;
	case _FRIEND_PVP:  //Satan2009.08.07 ������ȫ�����д�
		{			
			OBJID idFriend = m_pInfo->idFriend;
			CUser* pTarget = UserManager()->GetUser(idFriend);
			if (!pUser->GetMap())
			{
				return;
			}
			if (pUser->QuerySquareDeal())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "�����в��ܽ��д˲���");
				pUser->SetPVPTargetID(0);
				return;
			}
			if (pUser->QueryBooth())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "��̯�в��ܽ��д˲���");
				pUser->SetPVPTargetID(0);
				return;
			}
			if (pUser->GetMap()->isDisableStruggle())
			{
				pUser->SendSysMsg("�õ�ͼ���ܷ����д�����");
				return;
			}
			if (pUser->GetMap()->IsPkGameMap())
			{
				pUser->SendSysMsg("�õ�ͼ���ܷ����д�����");
				return;
			}
			if (pUser && pUser->IsRedName())
			{
				pUser->SendSysMsg("�㲻�ܷ����д裡");
				return;
			}
			if (pTarget && pTarget->IsRedName())
			{
				pUser->SendSysMsg("�Է����ܳ�Ϊ�д�Ķ���");
				return;
			}

			if (pUser->GetMapID() > DYNAMIC_MAP_ID)
			{
				pUser->SendSysMsg("����֮�ڽ�ֹ�д裡");
				return;				
			}
			if(!pTarget)
			{
//				pUser->SendSysMsg("�Ҳ���Ŀ��!");
				return;
			}
			if (pTarget->QuerySquareDeal())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "�Է����ڽ�����");
				pUser->SetPVPTargetID(0);
				pTarget->SetPVPTargetID(0);
				return;
			}
			if (pTarget->QueryBooth())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "�Է����ڰ�̯��");
				pUser->SetPVPTargetID(0);
				pTarget->SetPVPTargetID(0);
				return;
			}
			if(100 == m_pInfo->ucFriendType) //������� �����Ľ��
			{				
				MapGroup(PID)->GetPVPManger()->EndPVP(idFriend);
				return;
			}
			if (99 == m_pInfo->ucFriendType)
			{
				pTarget->SendSysMsg("���ܷ����д�");
				return;
			}

			if(4 == m_pInfo->ucFriendType) //�Է��ܾ�
			{
				pTarget->FetchApply(CUser::APPLY_PVP);
				char temp[128];
				sprintf(temp,"��� %s �ܾ�������д�����",pUser->GetName());
				pTarget->SendSysMsg(temp);
				pTarget->SetPVPTargetID(0);
				return;
			}
			
			IStatus* pStatus_Self = pUser->QueryStatus( STATUS_PVPMODE );
			IStatus* pStatus = pTarget->QueryStatus( STATUS_PVPMODE );
			if( pStatus_Self )
			{
				pUser->SendSysMsg("�������д���!");
				return;
			}
			if( pStatus )
			{
				pUser->SendSysMsg("�Է������д���!");
				return;
			}
			if(m_pInfo->ucFriendType == 1)
			{
				if (pUser->GetPVPTargetID() == 2 || pTarget->GetPVPTargetID() == 1 || 
					pUser->GetPVPTargetID() == 1 || pTarget->GetPVPTargetID() == 2)
				{
					pUser->SendSysMsg("���Ѿ��������д�������߶Է������д��С�");
					return;
				}
			}

			if(1 == m_pInfo->ucFriendType)
			{
				if (pTarget->QueryBooth())
				{
					pUser->SendSysMsg("�Է����ڰ�̯�У�");
					return;
				}
				if (!pTarget->IsAgreePKApply())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է��Ѵ򿪾ܾ��д迪��");//STR_TEAM_FULL
					return;
				}
				pUser->SetPVPTargetID(m_pInfo->ucFriendType);
				pUser->SetApply(CUser::APPLY_PVP, pTarget->GetID());
				IF_OK(Create(_FRIEND_PVP, pUser->GetID(), pUser->GetName(), true,0,0,0,0,0,m_pInfo->ucFriendType))
					pTarget->SendMsg(this);
				pUser->SendSysMsg("�㷢�����д�����");
				return;
			}
			
			if(2 == m_pInfo->ucFriendType)
			{
				pUser->SetPVPTargetID(m_pInfo->ucFriendType);
				pUser->SetApply(CUser::APPLY_PVP, pTarget->GetID());
				IF_OK(Create(_FRIEND_PVP, pUser->GetID(), pUser->GetName(), true,0,0,0,0,0,m_pInfo->ucFriendType))
					pTarget->SendMsg(this);
				return;
			}

			if(pUser->FetchApply(CUser::APPLY_PVP) != pTarget->GetID())
			{		
				pUser->SendSysMsg("�д������Ѿ����ڣ�");
				return;
// 				pUser->SetPVPTargetID(m_pInfo->ucFriendType);
// 				pUser->SetApply(CUser::APPLY_PVP, pTarget->GetID());
// 				IF_OK(Create(_FRIEND_PVP, pUser->GetID(), pUser->GetName(), true,0,0,0,0,0,m_pInfo->ucFriendType))
// 					pTarget->SendMsg(this);
			}
			else if(3 == m_pInfo->ucFriendType)
			{
				pUser->FetchApply(CUser::APPLY_PVP);

				if(pUser->GetMapID() != pTarget->GetMapID()               //ͬ��ͼ
					|| abs(pUser->GetPosX()- pTarget->GetPosX()) > 10     //X����
					|| abs(pUser->GetPosY()- pTarget->GetPosY()) > 10)    //Y����       
				{
					pUser->SetPVPTargetID(0);
					pTarget->SetPVPTargetID(0);
					pUser->SendSysMsg("�д�˫������̫Զ!");
					pTarget->SendSysMsg("�д�˫������̫Զ!");
					return;
				}

				CGameMap* pMap = pUser->GetMap();
//				if(!pMap || !pMap->IsPVPEnable())
//				{
//					pUser->SendSysMsg("��ǰ��ͼ�������д�!");
//					pTarget->SendSysMsg("��ǰ��ͼ�������д�!");
//					return;
//				}

// 				if(pUser->GetLife() != pUser->GetMaxLife()
// 					||pUser->GetMana() != pUser->GetMaxMana()
// 					||pTarget->GetLife() != pTarget->GetMaxLife()
// 					||pTarget->GetMana() != pTarget->GetMaxMana())
// 				{
// 					pUser->SendSysMsg("�д�ǰ����˫����ȫ�ָ�!");
// 					pTarget->SendSysMsg("�д�ǰ����˫����ȫ�ָ�!");
// 					return;
// 				}

				if(MapGroup(PID)->GetPVPManger()->findPVP(pUser->GetID()))
				{
					//MapGroup(PID)->GetPVPManger()->EndPVP(pUser->GetID());
				}
				if(MapGroup(PID)->GetPVPManger()->findPVP(pTarget->GetID()))
				{
					//MapGroup(PID)->GetPVPManger()->EndPVP(pTarget->GetID());
				}				
				//��ʼ����������������������

				pUser->SendSysMsg("�д迪ʼ!");
				pTarget->SendSysMsg("�д迪ʼ!");

				IF_OK(Create(_FRIEND_PVP, pUser->GetID(), pUser->GetName(), true,0,0,0,0,0,5))
					pTarget->SendMsg(this);

				IF_OK(Create(_FRIEND_PVP, pTarget->GetID(), pTarget->GetName(), true,0,0,0,0,0,5))
					pUser->SendMsg(this);

				

				//ˢ����
				int flagX = (pUser->GetPosX() + pTarget->GetPosX()) / 2;
				int flagY = (pUser->GetPosY() + pTarget->GetPosY()) / 2;
				ST_CREATENEWNPC	info;
				memset(&info, 0, sizeof(ST_CREATENEWNPC));
				info.idMap			= pUser->GetMapID();
				info.idData			= 0;
				info.idOwner		= 0;		
				info.usPosX			= flagX;
				info.usPosY			= flagY;
				info.usType			= 9999;
				info.ucOwnerType	= 0;
				info.nData			= 0;
				int nType = pTarget->GetPVPTargetID();
				OBJID flagID = NpcManager()->CreateSynPetReturnID(&info);

				CRole::AttachStatus( pUser, STATUS_PVPMODE, 100, 3 * 60 + 5);
				CRole::AttachStatus( pTarget, STATUS_PVPMODE, 100, 3 * 60 + 5);

				if(2 == nType )
				{
					CRole::AttachStatus( pUser, STATUS_BIGBLOOD_USERNOT, 100, 3 * 60 + 5);
					CRole::AttachStatus( pTarget, STATUS_BIGBLOOD_USERNOT, 100, 3 * 60 + 5);
				}

				//Ϊ��Ҽ�װ��������
				pUser->SetPVPTargetID(pTarget->GetID());
				pUser->SetPVPX(flagX);
				pUser->SetPVPY(flagY);
				pUser->SetPVPMap(pUser->GetMapID());

				pTarget->SetPVPTargetID(pUser->GetID());
				pTarget->SetPVPX(flagX);
				pTarget->SetPVPY(flagY);
				pTarget->SetPVPMap(pUser->GetMapID());
				MapGroup(PID)->GetPVPManger()->AddPVP(pUser->GetID(),pTarget->GetID(),nType,flagID);
			}
		}
		break;
		}
		DEBUG_CATCH2("MsgFriendChannel:%d ERROR!", m_pInfo->ucAction);
	}

// 	   catch (...)
// 	   {
// 		   ::LogSave("CMsgFriend %s %s %d",szDebugAction,szDebugStep, m_pInfo->ucAction);
// 		   
// 	   }
}
