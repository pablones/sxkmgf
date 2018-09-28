// MsgSchoolMember.cpp: implementation of the CMsgSchoolMember class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "MapGroup.h"
#include "UserManager.h"
//#pragma	warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgSchoolMember::CMsgSchoolMember()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgSchoolMember::~CMsgSchoolMember()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgSchoolMember::Create	(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_SCHOOLMEMBER != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgSchoolMember::Create(OBJID idUser, UCHAR ucAction, ST_MEMBERINFO* pMember, UCHAR ucAmount, UCHAR ucPage)
{
	if (sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + ucAmount*sizeof(ST_MEMBERINFO) >= MAX_PACKETSIZE)
		return false;

	if (idUser==ID_NONE)
		return false;
	// init
	this->Init();

	// TODO: init data
	m_pInfo->idUser		= idUser;
	m_pInfo->ucAction	= ucAction;
	m_pInfo->ucPage		= ucPage;
	if (pMember)
	{
		m_pInfo->ucAmount	= ucAmount;
		memcpy(m_pInfo->setMember, pMember, sizeof(ST_MEMBERINFO)*ucAmount);
	}
	else
		m_pInfo->ucAmount	= 0;

	m_unMsgType	=_MSG_SCHOOLMEMBER;
	m_unMsgSize	=sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + m_pInfo->ucAmount*sizeof(ST_MEMBERINFO);

	return true;
}

//////////////////////////////////////////////////////////////////////
// BOOL CMsgSchoolMember::Append(UCHAR ucRelation, UCHAR ucStatus, OBJID idMember, const char* pszName,OBJID idTutor)
// {
// 	if (sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + (m_pInfo->ucAmount+1)*sizeof(ST_MEMBERINFO) >= MAX_PACKETSIZE)
// 		return false;
// 
// 	memset(&m_pInfo->setMember[m_pInfo->ucAmount], 0L, sizeof(ST_MEMBERINFO));
// 	m_pInfo->setMember[m_pInfo->ucAmount].ucRelation	= ucRelation;
// 	m_pInfo->setMember[m_pInfo->ucAmount].idMember		= idMember;
// 	if (pszName)
// 		::SafeCopy(m_pInfo->setMember[m_pInfo->ucAmount].szName, pszName, _MAX_NAMESIZE);
// 
// 	++m_pInfo->ucAmount;
// 
// 	m_unMsgSize	=sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + m_pInfo->ucAmount*sizeof(ST_MEMBERINFO);
// 
// 	return true;
// }

///////////////////////////////////////////////////////////////////
// BOOL CMsgSchoolMember::Append(UCHAR ucRelation, UCHAR ucStatus, OBJID idMember, UCHAR ucProfession,UCHAR ucLevel,UINT ucSex,const char* pszName)
// {
// 	if (sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + (m_pInfo->ucAmount+1)*sizeof(ST_MEMBERINFO) >= MAX_PACKETSIZE)
// 		return false;
// 
// 	memset(&m_pInfo->setMember[m_pInfo->ucAmount], 0L, sizeof(ST_MEMBERINFO));
// 	m_pInfo->setMember[m_pInfo->ucAmount].ucRelation	= ucRelation;
// 	m_pInfo->setMember[m_pInfo->ucAmount].idMember		= idMember;
// 
// 	if (pszName)
// 		::SafeCopy(m_pInfo->setMember[m_pInfo->ucAmount].szName, pszName, _MAX_NAMESIZE);
// //	if(pszTeacherName)
// //  		::SafeCopy(m_pInfo->setMember[m_pInfo->ucAmount].szTeacherName, pszTeacherName, _MAX_NAMESIZE);
// 
// 	++m_pInfo->ucAmount;
// 
// 	m_unMsgSize	=sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + m_pInfo->ucAmount*sizeof(ST_MEMBERINFO);
// 
// 	return true;
// }
BOOL CMsgSchoolMember::Append(ST_MEMBERINFO stMember)
{
	if (sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + (m_pInfo->ucAmount+1)*sizeof(ST_MEMBERINFO) >= MAX_PACKETSIZE)
		return FALSE;
	
	memcpy(&m_pInfo->setMember[m_pInfo->ucAmount], &stMember, sizeof(ST_MEMBERINFO));
	++m_pInfo->ucAmount;
	
	m_unMsgSize	=sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + m_pInfo->ucAmount*sizeof(ST_MEMBERINFO);
	
	return TRUE;
}
BOOL CMsgSchoolMember::Append(TUTORLIST stInfo)
{
	if (sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + (m_pInfo->ucAmount+1)*sizeof(ST_MEMBERINFO) >= MAX_PACKETSIZE)
		return FALSE;

	memset(&m_pInfo->setMember[m_pInfo->ucAmount], 0L, sizeof(ST_MEMBERINFO));
	m_pInfo->setMember[m_pInfo->ucAmount].idMember = stInfo.idUser;
	m_pInfo->setMember[m_pInfo->ucAmount].ucInTeam	= stInfo.ucGender;
	m_pInfo->setMember[m_pInfo->ucAmount].ucVipLev = stInfo.ucVipLev;
	m_pInfo->setMember[m_pInfo->ucAmount].usLev = stInfo.usLev;
	m_pInfo->setMember[m_pInfo->ucAmount].usMainLev = stInfo.dwProfession;
	m_pInfo->setMember[m_pInfo->ucAmount].dwMaxScore = stInfo.dwBattleNum;
	m_pInfo->setMember[m_pInfo->ucAmount].nImpart = stInfo.dwImpart;
	m_pInfo->setMember[m_pInfo->ucAmount].dwExitTime = stInfo.dwLastLogoutTime;
	SafeCopy(m_pInfo->setMember[m_pInfo->ucAmount].szName, stInfo.szName, _MAX_NAMESIZE);

	++m_pInfo->ucAmount;	
	m_unMsgSize	=sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + m_pInfo->ucAmount*sizeof(ST_MEMBERINFO);
	
	return TRUE;
}
//////////////////////////////////////////////////////////////////////
BOOL CMsgSchoolMember::Append(UCHAR ucRelation, CUser* pUser)
{
	CHECKF (pUser);
	if (sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + (m_pInfo->ucAmount+1)*sizeof(ST_MEMBERINFO) >= MAX_PACKETSIZE)
		return false;

	memset(&m_pInfo->setMember[m_pInfo->ucAmount], 0L, sizeof(ST_MEMBERINFO));
	m_pInfo->setMember[m_pInfo->ucAmount].idMember		= pUser->GetID();
	m_pInfo->setMember[m_pInfo->ucAmount].usMainJob		= pUser->GetProfession(1);
	m_pInfo->setMember[m_pInfo->ucAmount].usSubJob		= pUser->GetProfession(2);
	m_pInfo->setMember[m_pInfo->ucAmount].usLev			= pUser->GetLev();
	m_pInfo->setMember[m_pInfo->ucAmount].usMainLev		= pUser->GetJobLev(1);
	m_pInfo->setMember[m_pInfo->ucAmount].usSubLev		= pUser->GetJobLev(2);
	m_pInfo->setMember[m_pInfo->ucAmount].usLookFace	= pUser->GetLookFace();
	m_pInfo->setMember[m_pInfo->ucAmount].nImpart		= pUser->GetImpart();
	m_pInfo->setMember[m_pInfo->ucAmount].ucVipLev	= pUser->IsVipActive() ? pUser->GetVipLev() : 0;
	m_pInfo->setMember[m_pInfo->ucAmount].ucInTeam	= pUser->GetTeam() ? 1 : 0;
	char szServer[32] = "";
	UCHAR nLine = 0;
	strcpy(szServer, MapGroup(pUser->GetProcessID())->GetServerName());
	if (strcmp(szServer, "一线") == 0)
		nLine = 1;
	else if (strcmp(szServer, "二线") == 0)
		nLine = 2;
	else if (strcmp(szServer, "三线") == 0)
		nLine = 3;
	else if (strcmp(szServer, "四线") == 0)
		nLine = 4;
	else if (strcmp(szServer, "五线") == 0)
		nLine = 5;
	else if (strcmp(szServer, "六线") == 0)
		nLine = 6;
	m_pInfo->setMember[m_pInfo->ucAmount].ucLine	= nLine;
	m_pInfo->setMember[m_pInfo->ucAmount].ucRelation	= ucRelation;
	::SafeCopy(m_pInfo->setMember[m_pInfo->ucAmount].szName, pUser->GetName(), _MAX_NAMESIZE);
	
	++m_pInfo->ucAmount;

	m_unMsgSize	=sizeof(MSG_Info) - sizeof(ST_MEMBERINFO) + m_pInfo->ucAmount*sizeof(ST_MEMBERINFO);

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgSchoolMember::Process(CGameSocket* pSocket)
{
#ifdef _MSGDEBUG
	::LogMsg("Process CMsgSchoolMember");
#endif
	DEBUG_TRY
	CUser* pUser = UserManager()->GetUser(this);
	if (!pUser)
	{
		switch(m_pInfo->ucAction)
		{
		case MESSAGEBOARD_SYNLINE://收到其他线路的列表更新信息
			{
				switch(m_pInfo->ucPage)
				{
				case SYNTUTOR_ONLINE:
					{
						if (m_pInfo->idUser==0)
							return;

						TUTORLIST info = {0};
						info.idUser = m_pInfo->setMember[0].idMember;
						info.ucGender = m_pInfo->setMember[0].ucInTeam;
						info.ucVipLev = m_pInfo->setMember[0].ucVipLev;
						info.usLev = m_pInfo->setMember[0].usLev;
						info.dwProfession = m_pInfo->setMember[0].usMainLev;
						info.dwBattleNum = m_pInfo->setMember[0].dwMaxScore;
						info.dwImpart = m_pInfo->setMember[0].nImpart;
						info.dwLastLogoutTime = 0;
						info.uIPAddress = m_pInfo->setMember[0].dwExitTime;
						SafeCopy(info.szName, m_pInfo->setMember[0].szName, _MAX_NAMESIZE);

						g_TutorListSystem.TutorLogin(&info);
					}
					break;
				case SYNTUTOR_OFFLINE:
					{
						if (m_pInfo->idUser==0)
							return;
						if (m_pInfo->setMember[0].dwExitTime==0)
							return;

						TUTORLIST info = {0};
						info.idUser = m_pInfo->setMember[0].idMember;
						info.ucGender = m_pInfo->setMember[0].ucInTeam;
						info.ucVipLev = m_pInfo->setMember[0].ucVipLev;
						info.usLev = m_pInfo->setMember[0].usLev;
						info.dwProfession = m_pInfo->setMember[0].usMainLev;
						info.dwBattleNum = m_pInfo->setMember[0].dwMaxScore;
						info.dwImpart = m_pInfo->setMember[0].nImpart;
						info.dwLastLogoutTime = m_pInfo->setMember[0].dwExitTime;
						SafeCopy(info.szName, m_pInfo->setMember[0].szName, _MAX_NAMESIZE);

						g_TutorListSystem.TutorLogout(&info);
					}
					break;
				case SYNTUTOR_ADD:
					{
						if (m_pInfo->ucAmount==0)
							return;
						TUTORLIST info = {0};
						info.idUser = m_pInfo->setMember[0].idMember;
						info.ucGender = m_pInfo->setMember[0].ucInTeam;
						info.ucVipLev = m_pInfo->setMember[0].ucVipLev;
						info.usLev = m_pInfo->setMember[0].usLev;
						info.dwProfession = m_pInfo->setMember[0].usMainLev;
						info.dwBattleNum = m_pInfo->setMember[0].dwMaxScore;
						info.dwImpart = m_pInfo->setMember[0].nImpart;
						info.dwLastLogoutTime = m_pInfo->setMember[0].dwExitTime;
						info.uIPAddress = m_pInfo->idUser;
						SafeCopy(info.szName, m_pInfo->setMember[0].szName, _MAX_NAMESIZE);
						g_TutorListSystem.AddTutorToList(&info);
					}
					break;
				case SYNTUTOR_DEL:
					{
						if (m_pInfo->idUser == ID_NONE)
							return;
						g_TutorListSystem.DeleteTutorFromList(m_pInfo->idUser);
					}
					break;
				}
			}
			break;
		case MESSAGEBOARD_LINEBROAD://其他线路过来的消息
			{
				switch(m_pInfo->ucPage)
				{
				case LINEBROAD_STUREBEL://徒弟叛出了师门
					{
						if (m_pInfo->setMember[0].idMember==ID_NONE)
							return;
						CUser* pUserTutor = UserManager()->GetUser(m_pInfo->setMember[0].idMember);
						if (!pUserTutor)
							return;
						CTutorData* pData = pUserTutor->GetStudent(m_pInfo->idUser);
						if (!pData)
							return;
						pUserTutor->DelStudentByTutor(m_pInfo->idUser);

						char szMsg[256] = "";
						sprintf(szMsg, "由于你对徒弟教导不善，%s已经叛离了你的师门。", m_pInfo->setMember[0].szName);
						pUserTutor->SendEmail(pUserTutor->GetName(), szMsg, pUserTutor);
					}
					break;
				case LINEBROAD_TUTORKICKOUT://被师父踢出了师门
					{
						if (m_pInfo->setMember[0].idMember==ID_NONE)
							return;
						CUser* pUserStu = UserManager()->GetUser(m_pInfo->setMember[0].idMember);
						if (!pUserStu)
							return;
						CTutorData* pData = pUserStu->GetTutor();
						if (!pData)
							return;
						pUserStu->DelTutorByStudent(true);
						
						char szMsg[256] = "";
						sprintf(szMsg, "你的师傅%s对你的表现很不满意，已经将你踢出了师门。", m_pInfo->setMember[0].szName);
						pUserStu->SendEmail(pUserStu->GetName(), szMsg, pUserStu);
					}
					break;
				case LINEBROAD_APPLYTUTOR://拜师请求
					{
						if (m_pInfo->idUser==ID_NONE)
							return;
						CUser* pTutor = UserManager()->GetUser(m_pInfo->idUser);
						if (!pTutor)
							return;
						if (m_pInfo->ucAmount==0)
							return;

						if (!pTutor->GetTutorManager())
							return;
						if (pTutor->GetTutorManager()->FindStudentByID(m_pInfo->setMember[0].idMember))
							return;
						char szMsg[256] = "";
						sprintf(szMsg, "玩家%s向你提出了拜师申请，请到师徒界面查看吧", m_pInfo->setMember[0].szName);
						if (pTutor->GetTutorManager()->AddNewToTutorApply(m_pInfo->setMember[0].dwExitTime,m_pInfo->setMember,TUTORSTATE_APPLYREF, NULL))
						{
							pTutor->SendEmail(pTutor->GetName(), szMsg, pTutor);
							CMsgSchoolMember msg;
							if (msg.Create(pTutor->GetID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_STATE))
							{
								ST_MEMBERINFO info = {0};
								info.idMember = m_pInfo->setMember[0].idMember;
								info.usMainJob = TUTORSTATE_APPLYREF;
								msg.Append(info);
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
							}
						}
					}
					break;
				case LINEBROAD_STATE://更改师徒状态
					{
						if (m_pInfo->setMember[0].idMember==ID_NONE)
							return;
						CUser* pTutor = UserManager()->GetUser(m_pInfo->setMember[0].idMember);
						if (!pTutor)
							return;
						if (!pTutor->GetTutorManager())
							return;
						CTutorData* pData = pTutor->GetTutorManager()->FindTutorByID(m_pInfo->idUser);
						if (!pData)
							pData = pTutor->GetTutorManager()->FindStudentByID(m_pInfo->idUser);
						if (!pData)
							return;
						CHECK(m_pInfo->setMember[0].usMainJob>=TUTORSTATE_BEGIN && m_pInfo->setMember[0].usMainJob<TUTORSTATE_END);
						pData->SetState((TUTORSTATE)m_pInfo->setMember[0].usMainJob);
					}
					break;
				case LINEBROAD_AGREEAPPLY://师父同意收徒
					{
						if (m_pInfo->setMember[0].idMember==ID_NONE)
							return;
						CUser* pStu = UserManager()->GetUser(m_pInfo->setMember[0].idMember);
						if (!pStu)
							return;
						if (!pStu->GetTutorManager())
							return;
						
						CTutorData* pData = pStu->GetTutorManager()->FindTutorByID(m_pInfo->idUser);
						if (!pData)
							return;

						//同意的时候不做IP判断了，只判断拜师的时候
// 						if (pStu->GetIPAddress()==m_pInfo->setMember[0].usMainJob)
// 						{
// 							//通知对方收徒失败，把状态改回去
// 							CMsgSchoolMember msg2;
// 							if (msg2.Create(pStu->GetID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_STATE))
// 							{
// 								ST_MEMBERINFO info = {0};
// 								info.idMember = m_pInfo->idUser;
// 								info.usMainJob = TUTORSTATE_APPLYREF;
// 								msg2.Append(info);
// 								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg2,OBJID(0));
// 							}
// 							CMsgTalk msg;
// 							if (msg.Create(SYSTEM_NAME, m_pInfo->setMember[0].szName, "该玩家登陆IP与你相同，无法结成师徒关系", NULL, 0xff0000, _TXTATR_TOPSHOW))
// 								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,m_pInfo->idUser);
// 
// 							return;
// 						}
						if (pStu->GetTutor())//已经有师父了
						{
							pStu->GetTutorManager()->DelTutorApply(m_pInfo->idUser);
							pData->DeleteRecord();
							CMsgSchoolMember msg;
							if (msg.Create(pStu->GetID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_STATE))
							{
								ST_MEMBERINFO info = {0};
								info.idMember = m_pInfo->idUser;
								info.usMainJob = TUTORSTATE_TUTORKICKOUT;
								msg.Append(info);
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
							}
							return;
						}
						pData->SetState(TUTORSTATE_NORMAL);
						pData->SetTeam(m_pInfo->setMember[0].ucInTeam);
						pData->SetLine(m_pInfo->setMember[0].ucLine);
						char szMsg[256] = "";
						sprintf(szMsg, "玩家%s接受了你的拜师申请，现在你们正式成为师徒了", m_pInfo->setMember[0].szName);
						pStu->SendEmail(pStu->GetName(), szMsg, pStu);

						CMsgSchoolMember msg;
						if (msg.Create(pStu->GetID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_STATE))
						{
							ST_MEMBERINFO info = {0};
							info.idMember = m_pInfo->idUser;
							info.usMainJob = TUTORSTATE_NORMAL;
							msg.Append(info);
							MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
						}

						int nGroup = 0;
						if (pStu->GetFriendGroupAmount(1)<MAX_GROUP_FRIEND)
							nGroup = 1;
						else if (pStu->GetFriendGroupAmount(2)<MAX_GROUP_FRIEND)
							nGroup = 2;
						else if (pStu->GetFriendGroupAmount(3)<MAX_GROUP_FRIEND)
							nGroup = 3;
						else if (pStu->GetFriendGroupAmount(4)<MAX_GROUP_FRIEND)
							nGroup = 4;
						if (nGroup!=0)
						{
							CMsgFriend pFriendMsg;
							OBJID dwFriendID = pStu->AddLeaveFriend(pData->GetTutorName(), nGroup, &pFriendMsg);
							if(dwFriendID)
							{
								pFriendMsg.m_pInfo->idFriend=dwFriendID;
								
								pFriendMsg.m_pInfo->ucAction=_FRIEND_ADDSUCCESS_PASSIVE;
								///离线加好友时向关系服务器求助查询好友在线状态
								pFriendMsg.SetTransData(pStu->GetID());
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&pFriendMsg);
								
								char szTemp[128];
								sprintf(szTemp,"<font color=\"#ffff00\">添加</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">为好友成功!</font>",pData->GetTutorName());
								pStu->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
							}
						}
					}
					break;
				case LINEBROAD_REFUSEAPPLY://师父拒绝收徒
					{
						if (m_pInfo->setMember[0].idMember==ID_NONE)
							return;
						CUser* pStu = UserManager()->GetUser(m_pInfo->setMember[0].idMember);
						if (!pStu)
							return;
						if (!pStu->GetTutorManager())
							return;
						CTutorData* pData = pStu->GetTutorManager()->FindTutorByID(m_pInfo->idUser);
						if (!pData)
							return;
						pData->DeleteRecord();
						pStu->GetTutorManager()->DelTutorApply(m_pInfo->idUser);
						char szMsg[256] = "";
						sprintf(szMsg, "很遗憾，玩家%s拒绝了你的拜师申请", m_pInfo->setMember[0].szName);
						pStu->SendEmail(pStu->GetName(), szMsg, pStu);
					}
					break;
				case LINEBROAD_ONLINESTATE://在线状态
					{
						if (m_pInfo->idUser==ID_NONE)
							return;
						CUser* pUserTemp = UserManager()->GetUser(m_pInfo->idUser);
						if (!pUserTemp)
							return;

						CTutorData* pData = NULL;
						if (m_pInfo->setMember[0].ucRelation == RELATION_STUDENT)
							pData = pUserTemp->GetStudent(m_pInfo->setMember[0].idMember);
						else if (m_pInfo->setMember[0].ucRelation == RELATION_TEACHER)
							pData = pUserTemp->GetTutor();

						if (!pData)
							return;
						pData->SetTeam(m_pInfo->setMember[0].ucInTeam);
						pData->SetLine(m_pInfo->setMember[0].ucLine);
						pData->SetInt(TUTORDATA_LEV, m_pInfo->setMember[0].usLev);
						pData->SetInt(TUTORDATA_MAINLEV, m_pInfo->setMember[0].usMainLev);
						pData->SetInt(TUTORDATA_SUBLEV, m_pInfo->setMember[0].usSubLev);
						pData->SetInt(TUTORDATA_IMPART,m_pInfo->setMember[0].nImpart);
						pData->SetInt(TUTORDATA_EXITTIME, 0);
						//把自己的状态返回
						int nReleation = m_pInfo->setMember[0].ucRelation == RELATION_TEACHER ? RELATION_STUDENT : RELATION_TEACHER;
						CMsgSchoolMember msg;
						if (msg.Create(pData->GetTutorID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_ONLINESTATEREF))
						{
							msg.Append(nReleation, pUserTemp);
							msg.m_pInfo->setMember[0].dwExitTime = 0;
							MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
						}
					}
					break;
				case LINEBROAD_ONLINESTATEREF://在线状态,不需要返回
					{
						if (m_pInfo->idUser==ID_NONE)
							return;
						CUser* pUserTemp = UserManager()->GetUser(m_pInfo->idUser);
						if (!pUserTemp)
							return;
						
						CTutorData* pData = NULL;
						if (m_pInfo->setMember[0].ucRelation == RELATION_STUDENT)
							pData = pUserTemp->GetStudent(m_pInfo->setMember[0].idMember);
						else if (m_pInfo->setMember[0].ucRelation == RELATION_TEACHER)
							pData = pUserTemp->GetTutor();
						
						if (!pData)
							return;
						pData->SetTeam(m_pInfo->setMember[0].ucInTeam);
						pData->SetLine(m_pInfo->setMember[0].ucLine);
						pData->SetInt(TUTORDATA_LEV, m_pInfo->setMember[0].usLev);
						pData->SetInt(TUTORDATA_MAINLEV, m_pInfo->setMember[0].usMainLev);
						pData->SetInt(TUTORDATA_SUBLEV, m_pInfo->setMember[0].usSubLev);
						pData->SetInt(TUTORDATA_IMPART,m_pInfo->setMember[0].nImpart);
						pData->SetInt(TUTORDATA_EXITTIME, m_pInfo->setMember[0].dwExitTime);
					}
					break;
				case LINEBROAD_SYNGIFTMASK:
					{
						if (m_pInfo->setMember[0].idMember==ID_NONE)
							return;
						CUser* pUserTemp = UserManager()->GetUser(m_pInfo->setMember[0].idMember);
						if (!pUserTemp)
							return;
						CTutorData* pData = NULL;
						if (m_pInfo->setMember[0].ucRelation==RELATION_STUDENT)
							pData = pUserTemp->GetStudent(m_pInfo->idUser);
						else if (m_pInfo->setMember[0].ucRelation==RELATION_TEACHER)
							pData = pUserTemp->GetTutor();
						if (!pData)
							return;
						pData->SetTutorGiftMask(m_pInfo->setMember[0].nImpart);
					}
					break;
				case LINEBROAD_CALLSTU://召唤徒弟
					{
						if (m_pInfo->setMember[0].idMember==ID_NONE)
							return;

						CUser* pUserTemp = UserManager()->GetUser(m_pInfo->setMember[0].idMember);
						if (!pUserTemp)
							return;

						CTutorData* pData = pUserTemp->GetTutor();
						if (!pData || pData->GetTutorID()!=m_pInfo->idUser)
							return;

						if (!pUserTemp->GetMap())
							return;
						if (pUserTemp->GetMap()->IsDynaMap())
						{
							CMsgTalk msg;
							if (msg.Create(SYSTEM_NAME, m_pInfo->setMember[0].szName, "对方在副本中，无法召唤！", NULL, 0xff0000, _TXTATR_TOPSHOW))
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,m_pInfo->idUser);
							return;
						}

						if (pUserTemp->GetMap()->GetDocID()==MAP_PRISON || pUserTemp->GetMap()->GetDocID()==1026 || pUserTemp->GetMap()->IsPkGameMap() || g_SpecilLine==1)
						{
							CMsgTalk msg;
							if (msg.Create(SYSTEM_NAME, m_pInfo->setMember[0].szName, "对方所在的场景不能召唤！", NULL, 0xff0000, _TXTATR_TOPSHOW))
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,m_pInfo->idUser);
							return;
						}
						if (pUserTemp->GetItemByType(_SPCITEM_MONEYFORSHOP))
						{
							CMsgTalk msg;
							if (msg.Create(SYSTEM_NAME, m_pInfo->setMember[0].szName, "对方在跑商中，无法召唤！", NULL, 0xff0000, _TXTATR_TOPSHOW))
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,m_pInfo->idUser);
							return;
						}
						if (!pUserTemp->GetTutorManager())
							return;

						pUserTemp->GetTutorManager()->SetBeCalled(true);
						pUserTemp->GetTutorManager()->SetCalledPos(m_pInfo->setMember[0].usLev, m_pInfo->setMember[0].usMainLev, m_pInfo->setMember[0].usSubLev, m_pInfo->setMember[0].ucLine);

						//通知徒弟，征求徒弟意见
						CMsgSchoolMember msg;
						if (msg.Create(pUserTemp->GetID(), MESSAGEBOARD_CALLSTU))
						{
							m_pInfo->setMember[0].idMember = pData->GetTutorID();
							msg.Append(m_pInfo->setMember[0]);
							pUserTemp->SendMsg(&msg);
						}
					}
					break;
				case LINEBROAD_CALLSTUREF://徒弟响应召唤
					{
						//召唤不需要扣道具，所以注释掉
// 						if (m_pInfo->setMember[0].idMember==ID_NONE)
// 							return;
// 						
// 						CUser* pUserTemp = UserManager()->GetUser(m_pInfo->setMember[0].idMember);
// 						if (!pUserTemp)
// 							return;

// 						CItem* pItem = pUserTemp->GetItemByType(506002);
// 						if (!pItem)
// 						{
// 							return;
// 						}
// 
// 						CHECK(pUserTemp->SpendItem(pItem));
					}
					break;
				case LINEBROAD_ADDEDLEV://带徒弟升过的等级
					{
						if (m_pInfo->setMember[0].idMember==ID_NONE)
							return;
						CUser* pUserTemp = UserManager()->GetUser(m_pInfo->setMember[0].idMember);
						if (!pUserTemp)
							return;
						CTutorData* pData = NULL;
						if (m_pInfo->setMember[0].ucRelation==RELATION_STUDENT)
							pData = pUserTemp->GetStudent(m_pInfo->idUser);
						else if (m_pInfo->setMember[0].ucRelation==RELATION_TEACHER)
							pData = pUserTemp->GetTutor();
						if (!pData)
							return;
						pData->SetInt(TUTORDATA_ADDEDLEV, m_pInfo->setMember[0].nImpart);
						pData->SetInt(TUTORDATA_LEV, m_pInfo->setMember[0].usLev);
					}
					break;
				case LINEBROAD_APPLYBYNAME://按名字拜师
					{
						if (m_pInfo->setMember[0].idMember==ID_NONE)
							return;
						CUser* pTarget = UserManager()->GetUser(m_pInfo->setMember[0].idMember);
						if (!pTarget)
							return;

						if(!pTarget->GetTutorManager())
							return;

						if (pTarget->GetLev() - m_pInfo->setMember[0].usLev < 10 )//必须比对方的等级要低10
						{
							CMsgTalk msg;
							if (msg.Create(SYSTEM_NAME, m_pInfo->setMember[0].szName, "只能拜等级超过你10级的玩家为师", NULL, 0xff0000, _TXTATR_TOPSHOW))
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,m_pInfo->idUser);

							return;
						}
						#if (!defined(_DEBUG) && !defined(_ASHRELEASE))
						if (m_pInfo->setMember[0].dwMaxScore==pTarget->GetIPAddress())
						{
							CMsgTalk msg;
							if (msg.Create(SYSTEM_NAME, m_pInfo->setMember[0].szName, "该玩家登陆IP与你相同，无法结成师徒关系", NULL, 0xff0000, _TXTATR_TOPSHOW))
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,m_pInfo->idUser);

							return;
						}
						#endif
						if (pTarget->IsStudentFull())
						{
							CMsgTalk msg;
							if (msg.Create(SYSTEM_NAME, m_pInfo->setMember[0].szName, "对方徒弟已满", NULL, 0xff0000, _TXTATR_TOPSHOW))
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,m_pInfo->idUser);

							return;
						}
						if (pTarget->GetTutorApplyAmount()>=20)
						{
							CMsgTalk msg;
							if (msg.Create(SYSTEM_NAME, m_pInfo->setMember[0].szName, "对方申请列表已满", NULL, 0xff0000, _TXTATR_TOPSHOW))
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,m_pInfo->idUser);

							return;
						}

						CTutorData* pData = pTarget->GetStudent(m_pInfo->idUser);
						if(pData)//曾经的徒弟
						{
							if(pData->GetState()!=TUTORSTATE_GRADUATED)
								return;

							pData->SetState(TUTORSTATE_APPLYREF);

							CMsgSchoolMember msg;
							if (msg.Create(pTarget->GetID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_STATE))
							{
								ST_MEMBERINFO info = {0};
								info.idMember = m_pInfo->idUser;
								info.usMainJob = TUTORSTATE_APPLYREF;
								msg.Append(info);
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
							}
						}
						else
						{
							OBJID id = ID_NONE;
							DEBUG_TRY							
							CTutorData* pData = CTutorData::CreateNew();
							CHECK(pData);
							IF_NOT(pData->Create(GameDataDefault()->GetTutorData(), id))
								return;
							
							pData->SetInt(TUTORDATA_TUTORID, pTarget->GetID());
							pData->SetStr(TUTORDATA_TUTORNAME, pTarget->GetName(), _MAX_NAMESIZE);
							pData->SetInt(TUTORDATA_STUID, m_pInfo->idUser);
							pData->SetStr(TUTORDATA_STUNAME, m_pInfo->setMember[0].szName, _MAX_NAMESIZE);
							pData->SetTutorGiftMask(pTarget->CalcTutorGiftMask(m_pInfo->setMember[0].usLev));
							pData->SetState(TUTORSTATE_APPLYREF, false);

							id = pData->InsertRecord();
							pData->ReleaseByOwner();

							pData = pTarget->GetTutorManager()->AddNewToLeaveTutorApply(m_pInfo->idUser, pTarget->GetID());
							CHECK(pData);
							pData->SetTeam(m_pInfo->setMember[0].ucInTeam);
							pData->SetLine(m_pInfo->setMember[0].ucLine);
							DEBUG_CATCH("CMsgSchoolMember::Process LINEBROAD_APPLYBYNAME error!")

							CMsgSchoolMember msg;
							if (msg.Create(pTarget->GetID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_APPLYBYNAMEREF))
							{
								msg.Append(RELATION_TEACHER, pTarget);
								msg.m_pInfo->setMember[0].idMember = m_pInfo->idUser;
								msg.m_pInfo->setMember[0].dwExitTime = id;
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
							}
						}
						char szMsg[256] = "";
						sprintf(szMsg, "玩家%s向你提出了拜师申请，请到师徒界面查看吧", m_pInfo->setMember[0].szName);
						pTarget->SendEmail(pTarget->GetName(), szMsg, pTarget);

						CMsgTalk msg;
						if (msg.Create(SYSTEM_NAME, m_pInfo->setMember[0].szName, "你的拜师申请已经发送", NULL, 0xff0000, _TXTATR_TOPSHOW))
							MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,m_pInfo->idUser);
					}
					break;
				case LINEBROAD_APPLYBYNAMEREF:
					{
						if (m_pInfo->setMember[0].idMember==ID_NONE)
							return;
						CUser* pTarget = UserManager()->GetUser(m_pInfo->setMember[0].idMember);
						if (!pTarget)
							return;
						
						if(!pTarget->GetTutorManager())
							return;

						CTutorData* pData = pTarget->GetTutorManager()->AddLeaveTutor(pTarget->GetID(), m_pInfo->idUser);
						CHECK(pData);
						pData->SetTeam(m_pInfo->setMember[0].ucInTeam);
						pData->SetLine(m_pInfo->setMember[0].ucLine);
					}
					break;
				}
			}
			break;
		}
		return;
	}

	if(IsNpcMsg())
		return;

	// TransmitMsg
	switch(m_pInfo->ucAction)
	{
	case MESSAGEBOARD_DELMEMBER:
		{
			//goto 临时测试用
// 				CMsgSchoolMember msg;
// 				msg.Create(pUser->GetID(), MESSAGEBOARD_ADDMEMBER, NULL, 0);
// 				msg.Append(123456789, RELATION_TEACHER, 0, "test", 1);
// 				pUser->SendMsg(&msg);
		}
		break;
	case MESSAGEBOARD_ADDMEMBER:
		{
// 			CMsgSchoolMember msg;
// 			msg.Create(pUser->GetID(), MESSAGEBOARD_ADDMEMBER, NULL, 0);
// 			for (int i=0; i<m_pInfo->ucAmount; i++)
// 			{
// 				CUser* pTarget = UserManager()->GetUser(m_pInfo->setMember[i].idMember);
// 				if (pTarget)
// 					msg.Append(m_pInfo->setMember[i].ucRelation, pTarget);
// 			}
// 			SendMsg(&msg);
		}
		break;
	case MESSAGEBOARD_UPDATEMEMBER:
		{
		}
		break;
	case MESSAGEBOARD_RECALL:
		{

		}
		break;
	case MESSAGEBOARD_QUERYINFO://查询详细
		{
// 			CTutorData* pTutor = NULL;
// 			DWORD dwGiftMask = 0;
// 			if (m_pInfo->setMember[0].ucRelation==RELATION_TEACHER)
// 				pTutor = pUser->GetTutor();
// 			else if (m_pInfo->setMember[0].ucRelation==RELATION_STUDENT)
// 				pTutor= pUser->GetStudent(m_pInfo->setMember[0].idMember);
// 
// 			if (!pTutor)
// 				return;
// 
// 			if (m_pInfo->setMember[0].ucRelation==RELATION_TEACHER)
// 				dwGiftMask = 0;
// 			else
// 				dwGiftMask = pTutor->GetTutorGiftMask();
// 
// 			CMsgSchoolMember msg;
// 			if (msg.Create(pUser->GetID(), MESSAGEBOARD_QUERYINFO, NULL, 0))
// 			{
// //				msg.Append(m_pInfo->setMember[0].idMember, m_pInfo->setMember[0].ucRelation, pTutor->GetTutorImpart(), pTutor->GetTutorName(), pTutor->GetTutorLev(), pTutor->GetTutorImpart(), dwGiftMask);
// 				pUser->SendMsg(&msg);
// 			}
		}
		break;
	case MESSAGEBOARD_QUERYTOTURLIST://查询师父列表
		{
			pUser->QueryTutorList( m_pInfo->ucPage, m_pInfo->ucAmount );
		}
		break;
	case MESSAGEBOARD_ADDTOLIST:
		{
			if (g_TutorListSystem.GetTutorAmount() >= _TUTOR_LIST_NUM)//列表人数已经达到上限
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "师父列表登记人数已达到上限！");
				return;
			}
			if (pUser->GetLev() < _MIN_TUTOR_LEVEL)
				return;
			if (g_TutorListSystem.FindTutorByID(pUser->GetID()))
			{
				if (!pUser->IsInTutorList())
					pUser->AddTutorListMask();
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "您已经登记过了！");
				return;
			}
			if (pUser->IsStudentFull())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "您的徒弟数量已满，不需要再登记收徒！");
				return;
			}
			pUser->JoinToTutorList();
			CMsgSchoolMember msg;
			if (msg.Create(pUser->GetID(), MESSAGEBOARD_ADDTOLIST, NULL, 0, 0))
			{
				pUser->SendMsg(&msg);
			}
		}
		break;
	case MESSAGEBOARD_APPLYADDTUTOR://申请拜师
		{
			if (pUser->GetLev()<10)//人物等级不符合条件
				return;
			if (pUser->GetTutor())//已经有师父了
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"您已经有师父了！");
				return;
			}
			if (pUser->GetID()==m_pInfo->setMember[0].idMember || strcmp(m_pInfo->setMember[0].szName, pUser->GetName())==0)
				return;
			if (!pUser->GetTutorManager())
				return;
			CTutorData* pTutorData = pUser->GetTutorManager()->FindTutorByID(m_pInfo->setMember[0].idMember);
			if (pTutorData)
			{
				if (pTutorData->GetState()==TUTORSTATE_APPLY || pTutorData->GetState()==TUTORSTATE_APPLYREF)
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "您已经在对方的申请列表中了！");
					return;
				}
// 				else if (pTutorData->GetState()==TUTORSTATE_GRADUATED)//允许出师后再拜同一个人为师
// 					pUser->SendSysMsg(_TXTATR_TOPSHOW, "您已经在对方门下出师了！");
			}

			CUser* pTutor = NULL;
			if(m_pInfo->setMember[0].idMember!=ID_NONE)
				pTutor = UserManager()->GetUser(m_pInfo->setMember[0].idMember);

			if(!pTutor && strlen(m_pInfo->setMember[0].szName)>0)//为了支持聊天栏拜师
				pTutor = UserManager()->GetUser(m_pInfo->setMember[0].szName);

			if (pTutor)
			{
				if (pTutor->GetLev()-pUser->GetLev() < 10 )//必须比对方的等级要低10
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "只能拜等级超过你10级的玩家为师");
					return;
				}
				if (pTutor->IsStudentFull())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方徒弟已满！");
					return;
				}
				#if (!defined(_DEBUG) && !defined(_ASHRELEASE))//方便测试
				if (pTutor->GetIPAddress()==pUser->GetIPAddress())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "该玩家登陆IP与你相同，无法结成师徒关系");
					return;
				}
				#endif
				if (pTutor->GetTutorApplyAmount()>=20)
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方申请列表已满");
					return;
				}
				ST_MEMBERINFO info = {0};
				info.idMember = pTutor->GetID();
				info.usLev = pTutor->GetLev();
				info.usMainJob = pTutor->GetProfession(1);
				info.usMainLev = pTutor->GetJobLev(1);
				info.usSubJob = pTutor->GetProfession(2);
				info.usSubLev = pTutor->GetJobLev(2);
				info.usLookFace = pTutor->GetVipface()>0 ? pTutor->GetVipface() : pTutor->GetLookFace();
				info.nImpart = pTutor->GetImpart();
				info.ucInTeam = pTutor->GetSex();
				if (pTutor->IsVipActive())
					info.ucVipLev = pTutor->GetVipLev();

				//info.dwMorality = pTutor->GetMorality();
				SafeCopy(info.szName, pTutor->GetName(), _MAX_NAMESIZE);

				//附加信息，一个装不下，再弄一个
				ST_MEMBERINFO infoExt = {0};
				infoExt.dwMaxScore = pTutor->GetMaxScore();
				infoExt.dwExitTime = 0;
				infoExt.nImpart = pTutor->GetMorality();
				OBJID id = pUser->ApplyAddTutor(&info, TUTORSTATE_APPLYREF, &infoExt);
				CHECK(id);
				char szMsg[256] = "";
				sprintf(szMsg, "玩家%s向你提出了拜师申请，请到师徒界面查看吧", pUser->GetName());
				
				int nVipLev = 0;
				if (pUser->IsVipActive())
					nVipLev = pUser->GetVipLev();

				ST_MEMBERINFO infoStu = {0};
				infoStu.idMember = pUser->GetID();
				infoStu.usLev = pUser->GetLev();
				infoStu.usMainJob = pUser->GetProfession(1);
				infoStu.usMainLev = pUser->GetJobLev(1);
				infoStu.dwExitTime = 0;
				infoStu.usSubLev = pUser->GetJobLev(2);
				infoStu.usLookFace = pUser->GetVipface()>0 ? pUser->GetVipface() : pUser->GetLookFace();
				infoStu.nImpart = pUser->GetMaxScore();
				infoStu.ucInTeam = pUser->GetSex();
				if (pUser->IsVipActive())
					infoStu.ucVipLev = pUser->GetVipLev();
				
				char szServer[32] = "";
				UCHAR nLine = 0;
				strcpy(szServer, MapGroup(pUser->GetProcessID())->GetServerName());
				if (strcmp(szServer, "一线") == 0)
					nLine = 1;
				else if (strcmp(szServer, "二线") == 0)
					nLine = 2;
				else if (strcmp(szServer, "三线") == 0)
					nLine = 3;
				else if (strcmp(szServer, "四线") == 0)
					nLine = 4;
				else if (strcmp(szServer, "五线") == 0)
					nLine = 5;
				else if (strcmp(szServer, "六线") == 0)
					nLine = 6;
				infoStu.ucLine = nLine;
				SafeCopy(infoStu.szName, pUser->GetName(), _MAX_NAMESIZE);

				ST_MEMBERINFO infoStuExt = {0};
				infoStuExt.usSubLev = pUser->GetProfession(2);
				infoStuExt.nImpart = pUser->GetImpart();
				infoStuExt.dwMaxScore = pUser->GetMorality();
				infoStuExt.usSubJob = pUser->CalcTutorGiftMask(pUser->GetLev());
				infoStuExt.ucInTeam = pUser->GetTeam()!=NULL ? 1 : 0;

				if (pTutor->GetTutorManager()->AddNewToTutorApply(id,&infoStu,TUTORSTATE_APPLYREF,&infoStuExt))
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "你的拜师申请已经发送");
					pTutor->SendEmail(pTutor->GetName(), szMsg, pTutor);
				}
			}
			else
			{
				TUTORLIST* pTutorInfo = NULL;
				if(m_pInfo->setMember[0].idMember!=ID_NONE)
					pTutorInfo = g_TutorListSystem.FindTutorByID(m_pInfo->setMember[0].idMember);
				else if(strlen(m_pInfo->setMember[0].szName)>0)
					pTutorInfo = g_TutorListSystem.FindTutorByName(m_pInfo->setMember[0].szName);

				if (!pTutorInfo)//不是师父列表上的
				{
					if(strlen(m_pInfo->setMember[0].szName)<=0)
						return;
					//pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方已不在师父列表中，请刷新列表！");
					CMsgSchoolMember msg;
					if (msg.Create(pUser->GetID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_APPLYBYNAME))
					{
						msg.Append(0,pUser);
						msg.m_pInfo->setMember[0].dwMaxScore = pUser->GetIPAddress();
						SafeCopy(msg.m_pInfo->setMember[0].szName, m_pInfo->setMember[0].szName, _MAX_NAMESIZE);
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
					}
					return;
				}
				if (pTutorInfo->usLev - pUser->GetLev() < 10 )//必须比对方的等级要低10
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "只能拜等级超过你10级的玩家为师");
					return;
				}
				#if (!defined(_DEBUG) && !defined(_ASHRELEASE))
				if (pTutorInfo->uIPAddress==pUser->GetIPAddress())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "该玩家登陆IP与你相同，无法结成师徒关系");
					return;
				}
				#endif
				int nRet = g_TutorListSystem.GetTutorApplyAmount(m_pInfo->setMember[0].idMember, pTutorInfo->dwImpart);
				if (nRet == 1)
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方徒弟已满！");
					return;
				}
				else if (nRet == 2)
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方申请列表已满");
					return;
				}
				
				ST_MEMBERINFO info = {0};
				info.idMember = pTutorInfo->idUser;
				info.usLev = pTutorInfo->usLev;
				info.usMainJob = pTutorInfo->dwProfession;
				info.usMainLev = 0;
				info.usSubJob = 0;
				info.usSubLev = 0;
				info.usLookFace = 0;
				info.nImpart = pTutorInfo->dwImpart;
				info.ucInTeam = pTutorInfo->ucGender;
				info.ucVipLev = pTutorInfo->ucVipLev;
				
				//info.dwMorality = pTutor->GetMorality();
				SafeCopy(info.szName, pTutorInfo->szName, _MAX_NAMESIZE);

				OBJID id = pUser->ApplyAddTutor(&info, TUTORSTATE_APPLY, NULL);
				CHECK(id);

				int nVipLev = 0;
				if (pUser->IsVipActive())
					nVipLev = pUser->GetVipLev();
				
				ST_MEMBERINFO infoStu = {0};
				infoStu.idMember = pUser->GetID();
				infoStu.usLev = pUser->GetLev();
				infoStu.usMainJob = pUser->GetProfession(1);
				infoStu.usMainLev = pUser->GetJobLev(1);
				infoStu.dwExitTime = id;
				infoStu.usSubLev = pUser->GetJobLev(2);
				infoStu.usLookFace = pUser->GetVipface()>0 ? pUser->GetVipface() : pUser->GetLookFace();
				infoStu.nImpart = pUser->GetMaxScore();
				infoStu.ucInTeam = pUser->GetTeam()!=NULL ? 1 : 0;
				char szServer[32] = "";
				UCHAR nLine = 0;
				strcpy(szServer, MapGroup(pUser->GetProcessID())->GetServerName());
				if (strcmp(szServer, "一线") == 0)
					nLine = 1;
				else if (strcmp(szServer, "二线") == 0)
					nLine = 2;
				else if (strcmp(szServer, "三线") == 0)
					nLine = 3;
				else if (strcmp(szServer, "四线") == 0)
					nLine = 4;
				else if (strcmp(szServer, "五线") == 0)
					nLine = 5;
				else if (strcmp(szServer, "六线") == 0)
					nLine = 6;
				infoStu.ucLine = nLine;
				if (pUser->IsVipActive())
					infoStu.ucVipLev = pUser->GetVipLev();
				
				SafeCopy(infoStu.szName, pUser->GetName(), _MAX_NAMESIZE);

				pUser->SendSysMsg(_TXTATR_TOPSHOW, "你的拜师申请已经发送");

				CMsgSchoolMember msg;
				if (msg.Create(pTutorInfo->idUser, MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_APPLYTUTOR))//与其他地方不同，这里idUser放的是对方的ID
				{
					msg.Append(infoStu);
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
				}
			}
		}
		break;
	case MESSAGEBOARD_AGREESTU://同意拜师
		{
			int nStep = 0;
			DEBUG_TRY
			if (pUser->GetLev()<35)
				return;
			if (pUser->IsStudentFull())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "徒弟已满！");
				return;
			}
			if (m_pInfo->setMember[0].idMember==0 || pUser->GetID()==m_pInfo->setMember[0].idMember)
				return;
			if (!pUser->GetTutorManager())
				return;
			nStep=1;
			CTutorData* pDataTutor = pUser->GetStudent(m_pInfo->setMember[0].idMember);//对方并没有申请过
			if (!pDataTutor)
				return;
			if (pDataTutor->GetState()!=TUTORSTATE_APPLY && pDataTutor->GetState()!=TUTORSTATE_APPLYREF)
				return;
			CUser* pUserStu = UserManager()->GetUser(m_pInfo->setMember[0].idMember);
			nStep=2;
			if (pUserStu)
			{
				//同意的时候不做IP判断了，只判断拜师的时候
// 				if (pUserStu->GetIPAddress()==pUser->GetIPAddress())
// 				{
// 					pUser->SendSysMsg(_TXTATR_TOPSHOW, "该玩家登陆IP与你相同，无法结成师徒关系");
// 					return;
// 				}
				nStep=3;
				CTutorData* pDataStu = pUserStu->GetTutorManager()->FindTutorByID(pUser->GetID());
				CHECK(pDataStu);

				if (pUserStu->GetLev()+10 > pUser->GetLev())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "只能收等级比自己小10级的玩家为徒弟！");
					return;
				}
				if (pUserStu->GetTutor())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方已经有师父了！");
					return;
				}
				nStep=4;
				pDataTutor->SetState(TUTORSTATE_NORMAL);
				pDataTutor->SetTutorGiftMask(pUser->CalcTutorGiftMask(pUserStu->GetLev()));
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "你成功收了一名徒弟");
				CMsgSchoolMember msg;
				if (msg.Create(pUser->GetID(), MESSAGEBOARD_DELAPPLYLIST))
				{
					msg.m_pInfo->ucAmount = 1;
					msg.m_pInfo->setMember[0].idMember = pUserStu->GetID();
					pUser->SendMsg(&msg);
				}
				nStep=5;
				pDataStu->SetState(TUTORSTATE_NORMAL);
				char szMsg[256] = "";
				sprintf(szMsg, "玩家%s接受了你的拜师申请，现在你们正式成为师徒了", pUser->GetName());
				pUserStu->SendEmail(pUserStu->GetName(), szMsg, pUserStu);
				nStep=6;
				//双方加为好友
				int nGroup = 0;
				if (pUser->GetFriendGroupAmount(1)<MAX_GROUP_FRIEND)
					nGroup = 1;
				else if (pUser->GetFriendGroupAmount(2)<MAX_GROUP_FRIEND)
					nGroup = 2;
				else if (pUser->GetFriendGroupAmount(3)<MAX_GROUP_FRIEND)
					nGroup = 3;
				else if (pUser->GetFriendGroupAmount(4)<MAX_GROUP_FRIEND)
					nGroup = 4;
				if (nGroup!=0)
				{
					if(pUser->AddFriend(pUserStu->GetID(), pUserStu->GetName(),nGroup,pUserStu->GetMate()))
					{ 
						CMsgFriend pMsg;
						pMsg.Create(_FRIEND_ADDSUCCESS,
							pUserStu->GetID(),
							pUserStu->GetName(),
							CMsgFriend::ONLINE_STATUS,
							pUserStu->GetLookFace(),
							pUserStu->GetLev(),
							pUserStu->GetProfession(),
							0,//friendship
							pUserStu->GetSex(),
							nGroup,//frinedgroup
							pUserStu->GetFeel(),
							0);//relation
						pUser->SendMsg(&pMsg);
						char szTemp[128];
						sprintf(szTemp,"<font color=\"#ffff00\">添加</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">为好友成功!</font>",pUserStu->GetName());
						pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
						pUserStu->AddBeFriend(0,pUser->GetID(),nGroup,pUser->GetName());
				    }
				}
				nGroup = 0;
				if (pUserStu->GetFriendGroupAmount(1)<MAX_GROUP_FRIEND)
					nGroup = 1;
				else if (pUserStu->GetFriendGroupAmount(2)<MAX_GROUP_FRIEND)
					nGroup = 2;
				else if (pUserStu->GetFriendGroupAmount(3)<MAX_GROUP_FRIEND)
					nGroup = 3;
				else if (pUserStu->GetFriendGroupAmount(4)<MAX_GROUP_FRIEND)
					nGroup = 4;
				if (nGroup!=0)
				{
					if(pUserStu->AddFriend(pUser->GetID(), pUser->GetName(),nGroup,pUser->GetMate()))
					{ 
						CMsgFriend pMsg;
						pMsg.Create(_FRIEND_ADDSUCCESS,
							pUser->GetID(),
							pUser->GetName(),
							CMsgFriend::ONLINE_STATUS,
							pUser->GetLookFace(),
							pUser->GetLev(),
							pUser->GetProfession(),
							0,//friendship
							pUser->GetSex(),
							nGroup,//frinedgroup
							pUser->GetFeel(),
							0);//relation
						pUserStu->SendMsg(&pMsg);
						char szTemp[128];
						sprintf(szTemp,"<font color=\"#ffff00\">添加</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">为好友成功!</font>",pUser->GetName());
						pUserStu->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
						pUser->AddBeFriend(0,pUserStu->GetID(),nGroup,pUserStu->GetName());
				    }
				}
				nStep=7;
				if (pUserStu->IsFirstTutor())
				{
					//给首次拜师奖励
					pUserStu->ProcessAction(2011710);
					pUserStu->SetNotFirstTutor();
				}
			}
			else
			{
				nStep=8;
				if (pDataTutor->GetStuID()==pUser->GetID())
					return;
				if (g_TutorListSystem.HasTutor(m_pInfo->setMember[0].idMember))
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方已经有师父了！");
					return;
				}
				nStep=9;
				pDataTutor->SetState(TUTORSTATE_AGREED);
				pDataTutor->SetTutorGiftMask(pUser->CalcTutorGiftMask(pDataTutor->GetInt(TUTORDATA_LEV)));
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "你成功收了一名徒弟");
				CMsgSchoolMember msg1;
				if (msg1.Create(pUser->GetID(), MESSAGEBOARD_DELAPPLYLIST))
				{
					msg1.m_pInfo->ucAmount = 1;
					msg1.m_pInfo->setMember[0].idMember = m_pInfo->setMember[0].idMember;
					pUser->SendMsg(&msg1);
				}
				nStep=10;
				CMsgSchoolMember msg;
				if (msg.Create(pUser->GetID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_AGREEAPPLY))
				{
					ST_MEMBERINFO info = {0};
					info.idMember = m_pInfo->setMember[0].idMember;
					info.usMainJob = pUser->GetIPAddress();
					SafeCopy(info.szName, pUser->GetName(), _MAX_NAMESIZE);

					info.ucInTeam = pUser->GetTeam()!=NULL ? 1 : 0;
					char szServer[32] = "";
					UCHAR nLine = 0;
					strcpy(szServer, MapGroup(pUser->GetProcessID())->GetServerName());
					if (strcmp(szServer, "一线") == 0)
						nLine = 1;
					else if (strcmp(szServer, "二线") == 0)
						nLine = 2;
					else if (strcmp(szServer, "三线") == 0)
						nLine = 3;
					else if (strcmp(szServer, "四线") == 0)
						nLine = 4;
					else if (strcmp(szServer, "五线") == 0)
						nLine = 5;
					else if (strcmp(szServer, "六线") == 0)
						nLine = 6;
					info.ucLine = nLine;
					msg.Append(info);
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
				}
				nStep=11;
				//加对方为好友
				int nGroup = 0;
				if (pUser->GetFriendGroupAmount(1)<MAX_GROUP_FRIEND)
					nGroup = 1;
				else if (pUser->GetFriendGroupAmount(2)<MAX_GROUP_FRIEND)
					nGroup = 2;
				else if (pUser->GetFriendGroupAmount(3)<MAX_GROUP_FRIEND)
					nGroup = 3;
				else if (pUser->GetFriendGroupAmount(4)<MAX_GROUP_FRIEND)
					nGroup = 4;
				if (nGroup!=0)
				{
					CMsgFriend pFriendMsg;
					OBJID dwFriendID = pUser->AddLeaveFriend(pDataTutor->GetStuName(), nGroup, &pFriendMsg);
					if(dwFriendID)
					{
						pFriendMsg.m_pInfo->idFriend=dwFriendID;
						
						pFriendMsg.m_pInfo->ucAction=_FRIEND_ADDSUCCESS_PASSIVE;
						///离线加好友时向关系服务器求助查询好友在线状态
						pFriendMsg.SetTransData(pUser->GetID());
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&pFriendMsg);
						
						char szTemp[128];
						sprintf(szTemp,"<font color=\"#ffff00\">添加</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">为好友成功!</font>",pDataTutor->GetStuName());
						pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
					}
				}
			}
			nStep=12;
			if (pUser->IsStudentFull())
			{
				pUser->DelFromTutorList();
				return;
			}
			DEBUG_CATCH2("CMsgSchoolMember MESSAGEBOARD_AGREESTU step=%d", nStep);
		}
		break;
	case MESSAGEBOARD_REFUSESTU://拒绝拜师请求
		{
			if (m_pInfo->setMember[0].idMember==0 || pUser->GetID()==m_pInfo->setMember[0].idMember)
				return;
			if (!pUser->GetTutorManager())
				return;
			CTutorData* pData = pUser->GetTutorManager()->FindStudentByID(m_pInfo->setMember[0].idMember);
			if (!pData)
				return;
			CUser* pUserStu = UserManager()->GetUser(m_pInfo->setMember[0].idMember);
			if (pUserStu && pUserStu->GetTutorManager())
			{
				if (pUser->GetTutorManager()->DelStuApply(m_pInfo->setMember[0].idMember))
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "你删除了一个申请");

				pUserStu->GetTutorManager()->DelTutorApply(pUser->GetID());
				char szMsg[256] = "";
				sprintf(szMsg, "很遗憾，玩家%s拒绝了你的拜师申请", pUser->GetName());
				pUserStu->SendEmail(pUserStu->GetName(), szMsg, pUserStu);

				CMsgSchoolMember msg1;
				if (msg1.Create(pUser->GetID(), MESSAGEBOARD_DELAPPLYLIST))
				{
					msg1.m_pInfo->ucAmount = 1;
					msg1.m_pInfo->setMember[0].idMember = m_pInfo->setMember[0].idMember;
					pUser->SendMsg(&msg1);
				}
			}
			else
			{
				pData->SetState(TUTORSTATE_REFUSED);
				pUser->GetTutorManager()->DelStudentByTutor(m_pInfo->setMember[0].idMember, false);
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "你删除了一个申请");

				CMsgSchoolMember msg1;
				if (msg1.Create(pUser->GetID(), MESSAGEBOARD_DELAPPLYLIST))
				{
					msg1.m_pInfo->ucAmount = 1;
					msg1.m_pInfo->setMember[0].idMember = m_pInfo->setMember[0].idMember;
					pUser->SendMsg(&msg1);
				}
				CMsgSchoolMember msg;
				if (msg.Create(pUser->GetID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_REFUSEAPPLY))
				{
					ST_MEMBERINFO info = {0};
					info.idMember = m_pInfo->setMember[0].idMember;
					SafeCopy(info.szName, pUser->GetName(), _MAX_NAMESIZE);
					msg.Append(info);
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
				}
			}
		}
		break;
	case MESSAGEBOARD_TEAMADD://师徒组队建立师徒关系
		{
			pUser->ProcessAction(2011714);
		}
		break;
	case MESSAGEBOARD_TEAMADDREF://徒弟响应拜师
		{
			CTeam* pTeam = pUser->GetTeam();
			if (!pTeam)
				return;
			CUser* pTutor = NULL;
			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, pTeam->GetLeader(), IPP_OF(pTutor)))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"对方已不在你的附近"); //对方已不在你的附近！
				return;
			}
			if(!pTutor->GetTutorManager())
				return;
			if (m_pInfo->ucPage==2)//徒弟拒绝
			{
				char szTemp[128] = "";
				sprintf(szTemp, "%s不愿意拜你为师", pUser->GetName());
				pTutor->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
				pTutor->FetchApply(CUser::APPLY_TEACHERAPPLY, pUser->GetID());
				return;
			}
			if(pUser->GetTutor())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"你已经有师父了"); //对方已不在你的附近！
				return;
			}
			if(pTutor->GetLev()-pUser->GetLev()<10)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "只能拜等级超过你10级的玩家为师");
				return;
			}
			if (pTutor->IsStudentFull())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方徒弟名额已满");
				return;
			}
			#if (!defined(_DEBUG) && !defined(_ASHRELEASE))
			if (pTutor->GetIPAddress()==pUser->GetIPAddress())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "该玩家登陆IP与你相同，无法结成师徒关系");
				return;
			}
			#endif
			
			if (pTutor->FetchApply(CUser::APPLY_TEACHERAPPLY, pUser->GetID()))
			{
				ST_MEMBERINFO info = {0};
				info.idMember = pTutor->GetID();
				info.usLev = pTutor->GetLev();
				info.usMainJob = pTutor->GetProfession(1);
				info.usMainLev = pTutor->GetJobLev(1);
				info.usSubJob = pTutor->GetProfession(2);
				info.usSubLev = pTutor->GetJobLev(2);
				info.usLookFace = pTutor->GetVipface()>0 ? pTutor->GetVipface() : pTutor->GetLookFace();
				info.nImpart = pTutor->GetImpart();
				info.ucInTeam = pTutor->GetSex();
				if (pTutor->IsVipActive())
					info.ucVipLev = pTutor->GetVipLev();
				
				//info.dwMorality = pTutor->GetMorality();
				SafeCopy(info.szName, pTutor->GetName(), _MAX_NAMESIZE);
				
				//附加信息，一个装不下，再弄一个
				ST_MEMBERINFO infoExt = {0};
				infoExt.dwMaxScore = pTutor->GetMaxScore();
				infoExt.dwExitTime = 0;
				infoExt.nImpart = pTutor->GetMorality();
				OBJID id = pUser->ApplyAddTutor(&info, TUTORSTATE_NORMAL, &infoExt);
				CHECK(id);

				int nVipLev = 0;
				if (pUser->IsVipActive())
					nVipLev = pUser->GetVipLev();
				
				ST_MEMBERINFO infoStu = {0};
				infoStu.idMember = pUser->GetID();
				infoStu.usLev = pUser->GetLev();
				infoStu.usMainJob = pUser->GetProfession(1);
				infoStu.usMainLev = pUser->GetJobLev(1);
				infoStu.dwExitTime = 0;
				infoStu.usSubLev = pUser->GetJobLev(2);
				infoStu.usLookFace = pUser->GetVipface()>0 ? pUser->GetVipface() : pUser->GetLookFace();
				infoStu.nImpart = pUser->GetMaxScore();
				infoStu.ucInTeam = pUser->GetSex();
				if (pUser->IsVipActive())
					infoStu.ucVipLev = pUser->GetVipLev();
				
				char szServer[32] = "";
				UCHAR nLine = 0;
				strcpy(szServer, MapGroup(pUser->GetProcessID())->GetServerName());
				if (strcmp(szServer, "一线") == 0)
					nLine = 1;
				else if (strcmp(szServer, "二线") == 0)
					nLine = 2;
				else if (strcmp(szServer, "三线") == 0)
					nLine = 3;
				else if (strcmp(szServer, "四线") == 0)
					nLine = 4;
				else if (strcmp(szServer, "五线") == 0)
					nLine = 5;
				else if (strcmp(szServer, "六线") == 0)
					nLine = 6;
				infoStu.ucLine = nLine;
				SafeCopy(infoStu.szName, pUser->GetName(), _MAX_NAMESIZE);
				
				ST_MEMBERINFO infoStuExt = {0};
				infoStuExt.usSubLev = pUser->GetProfession(2);
				infoStuExt.nImpart = pUser->GetImpart();
				infoStuExt.dwMaxScore = pUser->GetMorality();
				infoStuExt.usSubJob = pUser->CalcTutorGiftMask(pUser->GetLev());
				infoStuExt.ucInTeam = pUser->GetTeam()!=NULL ? 1 : 0;
				
				CTutorData* pData = pUser->GetTutor();
				if(pData)
				{
					pData->SetTeam(pTutor->GetTeam()!=NULL ? 1 : 0);
					pData->SetLine(nLine);
				}
				if (pTutor->GetTutorManager()->AddNewToTutorApply(id,&infoStu,TUTORSTATE_NORMAL,&infoStuExt))
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "拜师完成，现在你们正式成为师徒了");
					pTutor->SendSysMsg(_TXTATR_TOPSHOW, "你成功收了一名徒弟");
				}

				//双方加为好友
				int nGroup = 0;
				if (pUser->GetFriendGroupAmount(1)<MAX_GROUP_FRIEND)
					nGroup = 1;
				else if (pUser->GetFriendGroupAmount(2)<MAX_GROUP_FRIEND)
					nGroup = 2;
				else if (pUser->GetFriendGroupAmount(3)<MAX_GROUP_FRIEND)
					nGroup = 3;
				else if (pUser->GetFriendGroupAmount(4)<MAX_GROUP_FRIEND)
					nGroup = 4;
				if (nGroup!=0)
				{
					if(pUser->AddFriend(pTutor->GetID(), pTutor->GetName(),nGroup,pTutor->GetMate()))
					{ 
						CMsgFriend pMsg;
						pMsg.Create(_FRIEND_ADDSUCCESS,
							pTutor->GetID(),
							pTutor->GetName(),
							CMsgFriend::ONLINE_STATUS,
							pTutor->GetLookFace(),
							pTutor->GetLev(),
							pTutor->GetProfession(),
							0,//friendship
							pTutor->GetSex(),
							nGroup,//frinedgroup
							pTutor->GetFeel(),
							0);//relation
						pUser->SendMsg(&pMsg);
						char szTemp[128];
						sprintf(szTemp,"<font color=\"#ffff00\">添加</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">为好友成功!</font>",pTutor->GetName());
						pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
						pTutor->AddBeFriend(0,pUser->GetID(),nGroup,pUser->GetName());
					}
				}
				nGroup = 0;
				if (pTutor->GetFriendGroupAmount(1)<MAX_GROUP_FRIEND)
					nGroup = 1;
				else if (pTutor->GetFriendGroupAmount(2)<MAX_GROUP_FRIEND)
					nGroup = 2;
				else if (pTutor->GetFriendGroupAmount(3)<MAX_GROUP_FRIEND)
					nGroup = 3;
				else if (pTutor->GetFriendGroupAmount(4)<MAX_GROUP_FRIEND)
					nGroup = 4;
				if (nGroup!=0)
				{
					if(pTutor->AddFriend(pUser->GetID(), pUser->GetName(),nGroup,pUser->GetMate()))
					{ 
						CMsgFriend pMsg;
						pMsg.Create(_FRIEND_ADDSUCCESS,
							pUser->GetID(),
							pUser->GetName(),
							CMsgFriend::ONLINE_STATUS,
							pUser->GetLookFace(),
							pUser->GetLev(),
							pUser->GetProfession(),
							0,//friendship
							pUser->GetSex(),
							nGroup,//frinedgroup
							pUser->GetFeel(),
							0);//relation
						pTutor->SendMsg(&pMsg);
						char szTemp[128];
						sprintf(szTemp,"<font color=\"#ffff00\">添加</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">为好友成功!</font>",pUser->GetName());
						pTutor->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
						pUser->AddBeFriend(0,pTutor->GetID(),nGroup,pTutor->GetName());
					}
				}
				if (pUser->IsFirstTutor())
				{
					//给首次拜师奖励
					pUser->ProcessAction(2011710);
					pUser->SetNotFirstTutor();
				}
			}
		}
		break;
	case MESSAGEBOARD_GRADUATEAPPLY://师父带徒弟出师
		{
// 			CTeam* pTeam = pUser->GetTeam();
// 			if (!pTeam || pTeam->GetLeader()!=pUser->GetID())
// 			{
// 				return;
// 			}
// 			
// 			// find target
// 			OBJID idTarget = pTeam->GetTeamMemberOnly();
// 			CUser* pStudent = NULL;
// 			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, idTarget, IPP_OF(pStudent)))
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方已不在你的附近");
// 				return;
// 			}
			pUser->ProcessAction(2011713);
		}
		break;
	case MESSAGEBOARD_GRADUATEREF://徒弟响应出师
		{
			// find target
			CTeam* pTeam = pUser->GetTeam();
			if (!pTeam)
				return;
			CUser* pTarget = NULL;
			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, pTeam->GetLeader(), IPP_OF(pTarget)))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"对方已不在你的附近"); //对方已不在你的附近！
				return;
			}
			if (m_pInfo->ucPage==2)//徒弟拒绝
			{
				char szTemp[128] = "";
				sprintf(szTemp, "你的徒弟%s还不愿意出师", pUser->GetName());
				pTarget->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
				pTarget->FetchApply(CUser::APPLY_GRADUATE, pUser->GetID());
				return;
			}
			if (pTarget->FetchApply(CUser::APPLY_GRADUATE, pUser->GetID()))
				pTarget->ProcessAction(2011701);
		}
		break;
	case MESSAGEBOARD_DIVORCE://协议解除师徒关系
		{
			pUser->ProcessAction(2011702);
		}
		break;
	case MESSAGEBOARD_REQDIVORCE://徒弟响应解除关系
		{
			if (!pUser->GetTutor())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "您还没有师父");
				return;
			}
			// find target
			CTeam* pTeam = pUser->GetTeam();
			if (!pTeam)
				return;
			CUser* pTarget = NULL;
			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, pTeam->GetLeader(), IPP_OF(pTarget)))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"对方已不在你的附近"); //对方已不在你的附近！
				return;
			}
			if (m_pInfo->ucPage==2)//徒弟拒绝
			{
				char szTemp[128] = "";
				sprintf(szTemp, "你的徒弟%s不愿意与你解除师徒关系", pUser->GetName());
				pTarget->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
				pTarget->FetchApply(CUser::APPLY_LEAVETEACHER, pUser->GetID());
				return;
			}
			if (pTarget->FetchApply(CUser::APPLY_LEAVETEACHER, pUser->GetID()))
			{
				IF_NOT (pUser->TutorDivorce())
					return;
				IF_NOT (pTarget->DelStudentByTutor(pUser->GetID(), false))
					return;

				char szTemp[128] = "";
				sprintf(szTemp, "你与%s的师徒关系已经解除",pUser->GetName());
				pTarget->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
				
				sprintf(szTemp, "你与%s的师徒关系已经解除",pTarget->GetName());
				pUser->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
			}
		}
		break;
	case MESSAGEBOARD_STUREBEL://徒弟叛师
		{
			pUser->ProcessAction(2011703);
		}
		break;
	case MESSAGEBOARD_TUTORKICKOUTSTU://师父踢出徒弟
		{
			if (m_pInfo->setMember[0].idMember==ID_NONE || m_pInfo->setMember[0].idMember==pUser->GetID())
				return;
			LuaScriptMachine()->InitVar(m_pInfo->setMember[0].idMember);
			pUser->ProcessAction(2011704);
		}
		break;
	case MESSAGEBOARD_QUERYSTUAPPLYLIST://查询徒弟申请列表
		{
			if (!pUser->GetTutorManager())
				return;

			pUser->GetTutorManager()->SendStuApplyList(m_pInfo->ucPage, m_pInfo->ucAmount);
		}
		break;
// 	case MESSAGEBOARD_QUERYSTU://徒弟列表
// 		{
// 			if (!pUser->GetTutorManager())
// 				return;
// 			
// 			pUser->GetTutorManager()->SendStuList();
// 		}
// 		break;
	case MESSAGEBOARD_ALLLIST://师徒列表
		{
			if (!pUser->GetTutorManager())
				return;
			
			pUser->GetTutorManager()->SendAllList();
		}
		break;
	case MESSAGEBOARD_DELAPPLYLIST://删除徒弟申请列表中的徒弟
		{
			if (m_pInfo->setMember[0].idMember==0 || pUser->GetID()==m_pInfo->setMember[0].idMember)
				return;
			if (!pUser->GetTutorManager())
				return;
			CTutorData* pData = pUser->GetTutorManager()->FindStudentByID(m_pInfo->setMember[0].idMember);
			if (!pData)
				return;

			if (pUser->GetTutorManager()->DelStuApply(m_pInfo->setMember[0].idMember))
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "你删除了一个申请");

			CMsgSchoolMember msg;
			if (msg.Create(pUser->GetID(), MESSAGEBOARD_DELAPPLYLIST))
			{
				msg.m_pInfo->ucAmount = 1;
				msg.m_pInfo->setMember[0].idMember = m_pInfo->setMember[0].idMember;
				pUser->SendMsg(&msg);
			}
		}
		break;
	case MESSAGEBOARD_CALLSTU://召唤徒弟
		{
			if (m_pInfo->setMember[0].idMember==0 || pUser->GetID()==m_pInfo->setMember[0].idMember)
				return;

			//判断有没有召唤道具
// 			if (!pUser->GetItemByType(506002))
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "缺少召唤需要的道具！");
// 				return;
// 			}
			CTutorData* pData = pUser->GetStudent(m_pInfo->setMember[0].idMember);
			if (!pData)
				return;

			if (!pUser->GetMap())
				return;
			if (pUser->GetMap()->IsDynaMap())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "副本中不能召唤！");
				return;
			}
			if (pUser->GetMap()->IsSynMap())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "帮派中不能召唤！");
				return;
			}
			if (pUser->GetMap()->GetDocID()==MAP_PRISON || pUser->GetMap()->GetDocID()==1026 || pUser->GetMap()->IsPkGameMap() || g_SpecilLine == 1 )//监狱和地府不能召唤
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "该场景无法召唤！");
				return;
			}
			CUser* pStudent = UserManager()->GetUser(m_pInfo->setMember[0].idMember);
			if (pStudent)
			{
				if (!pStudent->GetTutorManager())
					return;
				if (!pStudent->GetMap())
					return;
				if (pStudent->GetMap()->IsDynaMap())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方在副本中，无法召唤！");
					return;
				}
				if (pStudent->GetItemByType(_SPCITEM_MONEYFORSHOP))
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方在跑商中，无法召唤！");
					return;
				}
				if (pStudent->GetMap()->GetDocID()==MAP_PRISON || pStudent->GetMap()->GetDocID()==1026 || pStudent->GetMap()->IsPkGameMap())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方所在的场景不能召唤！");
					return;
				}
				//通知徒弟，征求徒弟意见
				CMsgSchoolMember msg;
				if (msg.Create(pStudent->GetID(), MESSAGEBOARD_CALLSTU))
				{
					msg.Append(RELATION_TEACHER, pUser);
					msg.m_pInfo->setMember[0].usLev = pUser->GetMap()->GetDocID();
					msg.m_pInfo->setMember[0].usMainLev = pUser->GetPosX();
					msg.m_pInfo->setMember[0].usSubLev = pUser->GetPosY();
					pStudent->SendMsg(&msg);
				}

				if (pStudent->GetTutorManager())
				{
					pStudent->GetTutorManager()->SetBeCalled(true);
					pStudent->GetTutorManager()->SetCalledPos(pUser->GetMap()->GetDocID(), pUser->GetPosX(), pUser->GetPosY(), msg.m_pInfo->setMember[0].ucLine);
				}
			}
			else
			{
				CMsgSchoolMember msg;
				if (msg.Create(pUser->GetID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_CALLSTU))
				{
					msg.Append(RELATION_TEACHER, pUser);
					msg.m_pInfo->setMember[0].idMember = pData->GetStuID();
					msg.m_pInfo->setMember[0].usLev = pUser->GetMap()->GetDocID();
					msg.m_pInfo->setMember[0].usMainLev = pUser->GetPosX();
					msg.m_pInfo->setMember[0].usSubLev = pUser->GetPosY();
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
				}
			}
		}
		break;
	case MESSAGEBOARD_CALLSTUREF://徒弟响应召唤
		{
			if (!pUser->GetTutorManager())
				return;
			if (!pUser->GetTutorManager()->HasBeCalled())
				return;

			BeCalledPos* pCallPos = pUser->GetTutorManager()->GetCalledPos();
			if (!pCallPos)
				return;

			CTutorData* pData = pUser->GetTutor();
			if (!pData)
				return;

			if (!pUser->GetMap())
				return;
			
			CUser* pTutor = UserManager()->GetUser(pData->GetTutorID());
			if (pTutor)
			{
				if (m_pInfo->ucPage==2)
				{
					pTutor->SendSysMsg(_TXTATR_TOPSHOW, "对方拒绝了召唤！");
					return;
				}
// 				CItem* pItem = pTutor->GetItemByType(506002);
// 				if (!pItem)
// 					return;

				if (!pTutor->GetMap())
					return;

				if (pUser->FlyMap(pCallPos->nMapID, pCallPos->nPosX, pCallPos->nPosY)==-1)
					return;
				pUser->GetTutorManager()->SetBeCalled(false);
				//扣除师父道具
//				CHECK(pTutor->SpendItem(pItem));

			}
			else
			{
				if (m_pInfo->ucPage==2)
				{
					CMsgTalk msg;
					if (msg.Create(SYSTEM_NAME, pData->GetTutorName(), "对方拒绝了召唤！", NULL, 0xff0000, _TXTATR_TOPSHOW))
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pData->GetTutorID());
					return;
				}
				
				if (pUser->FlyMap(pCallPos->nMapID, pCallPos->nPosX, pCallPos->nPosY)==-1)
					return;
				pUser->GetTutorManager()->SetBeCalled(false);
				//通知师父删除道具
// 				CMsgSchoolMember msg;
// 				if (msg.Create(pUser->GetID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_CALLSTUREF))
// 				{
// 					ST_MEMBERINFO info = {0};
// 					info.idMember = pData->GetTutorID();
// 					msg.Append(info);
// 					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
// 				}
				//通知客户端换线
				CMsgSchoolMember msg2;
				if (msg2.Create(pUser->GetID(), MESSAGEBOARD_CALLSTUREF))
				{
					ST_MEMBERINFO info = {0};
					info.idMember = pData->GetTutorID();
					info.usLev = pCallPos->nMapID;
					info.usMainLev = pCallPos->nPosX;
					info.usSubLev = pCallPos->nPosY;
					info.ucLine = pCallPos->nLine;
					msg2.Append(info);
					pUser->SendMsg(&msg2);
				}
			}
		}
		break;
	case MESSAGEBOARD_GETGIFT://师父领取徒弟的成长奖励
		{
			if (m_pInfo->setMember[0].idMember==ID_NONE || m_pInfo->setMember[0].idMember==pUser->GetID())
				return;

			if (!pUser->GetTutorManager())
				return;

			CTutorData* pData = pUser->GetTutorManager()->FindStudentByID(m_pInfo->setMember[0].idMember);
			if (!pData)
				return;

			LuaScriptMachine()->InitVar(m_pInfo->setMember[0].idMember);
			if (pData->GetState()==TUTORSTATE_GRADUATED)
				pUser->ProcessAction(1007);
			else if (pData->GetState()==TUTORSTATE_AGREED || pData->GetState()==TUTORSTATE_NORMAL)
				pUser->ProcessAction(1006);
		}
		break;
	case MESSAGEBOARD_HASGRADUATE://是否拥有出过师的徒弟
		{
			if (!pUser->GetTutorManager())
				return;

			int nGradu = pUser->GetTutorManager()->GetGraduatedAmount();
			int bHas = nGradu>0 ? 1 : 0;
			CMsgSchoolMember msg;
			if (msg.Create(pUser->GetID(), MESSAGEBOARD_HASGRADUATE, NULL, 0, bHas))
				pUser->SendMsg(&msg);
		}
		break;
	case MESSAGEBOARD_GRADUATESTU://请求已出师的徒弟列表
		{
			if (!pUser->GetTutorManager())
				return;
			
			pUser->GetTutorManager()->SendGraduateStuList(m_pInfo->ucPage, m_pInfo->ucAmount);
		}
		break;
	case MESSAGEBOARD_ANSWERQUESTION://师徒答题
		{
			if(pUser->GetTutorQuestion()==0)
				return;

			CTeam* pTeam = pUser->GetTeam();
			if (!pTeam)
				return;

			int nTeamAmount = pTeam->GetMemberAmount();
			if(nTeamAmount<2)
				return;

			CHECK(m_pInfo->ucPage);

			CUser* pTempUser = NULL;
			CTutorData* pData = NULL;
			//把队伍里的师徒全找出来
			vector<CUser*> vTutor;
			CUser* pTutor = NULL;
			if(!pUser->FindAroundObj( OBJ_USER,pTeam->GetLeader(),( void** )&pTutor ))//必须是师父当队长
				return;
			if(!pTutor)
				return;
			OBJID idQuestion = pTutor->GetTutorQuestion();
			if(!idQuestion)
				return;

			vTutor.push_back(pTutor);
			for(int i = 0; i < nTeamAmount; i++)
			{
				OBJID idMember = pTeam->GetMemberByIndex(i);
				if(idMember==pTutor->GetID())
					continue;

				if(!pUser->FindAroundObj( OBJ_USER,idMember,( void** )&pTempUser ))
					continue;

				if(pTempUser && pTempUser->IsAlive())
				{
					if(idQuestion == pTempUser->GetTutorQuestion())
						vTutor.push_back(pTempUser);
				}
			}

			if(vTutor.size()<2)
				return;

			pUser->SetTutorAnswer(m_pInfo->ucPage);

			//检查是否所有人都已经提交答案,答案是否全部相同
			int bWrong = 0;
			for(i = vTutor.size()-1; i>=0; i--)
			{
				if(vTutor[i]->GetTutorAnswer()==0)//有人还没答
					return;
				else if(vTutor[i]->GetTutorAnswer()!=pTutor->GetTutorAnswer())//有人答案不一致
					bWrong = 1;
			}

			CMsgSchoolMember msg;
			if(msg.Create(pUser->GetID(), MESSAGEBOARD_ANSWERQUESTION, NULL, 0, bWrong))
			{
				for(int i = vTutor.size()-1; i>=0; i--)
				{
					if(bWrong==0)
						vTutor[i]->ProcessAction(1010);

					msg.m_pInfo->idUser = vTutor[i]->GetID();
					vTutor[i]->SendMsg(&msg);
					vTutor[i]->SetTutorAnswer(0);
					vTutor[i]->SetTutorQuestion(0);
				}
			}
		}
		break;
	default:
		{
			char strTemp[128];
			sprintf(strTemp,"CMsgSchoolMember::Process::switch(m_pInfo->usAction) default...m_pInfo->usAction=%d,userid:%u,username:%s",m_pInfo->ucAction,pUser->GetID(), pUser->GetName());
			::LogSave(strTemp);
		}
		break;
	}
	DEBUG_CATCH2("CMsgSchoolMember::Process action=%d", m_pInfo->ucAction)
}
