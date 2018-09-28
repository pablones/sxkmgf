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
	if (strcmp(szServer, "һ��") == 0)
		nLine = 1;
	else if (strcmp(szServer, "����") == 0)
		nLine = 2;
	else if (strcmp(szServer, "����") == 0)
		nLine = 3;
	else if (strcmp(szServer, "����") == 0)
		nLine = 4;
	else if (strcmp(szServer, "����") == 0)
		nLine = 5;
	else if (strcmp(szServer, "����") == 0)
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
		case MESSAGEBOARD_SYNLINE://�յ�������·���б������Ϣ
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
		case MESSAGEBOARD_LINEBROAD://������·��������Ϣ
			{
				switch(m_pInfo->ucPage)
				{
				case LINEBROAD_STUREBEL://ͽ���ѳ���ʦ��
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
						sprintf(szMsg, "�������ͽ�̵ܽ����ƣ�%s�Ѿ����������ʦ�š�", m_pInfo->setMember[0].szName);
						pUserTutor->SendEmail(pUserTutor->GetName(), szMsg, pUserTutor);
					}
					break;
				case LINEBROAD_TUTORKICKOUT://��ʦ���߳���ʦ��
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
						sprintf(szMsg, "���ʦ��%s����ı��ֺܲ����⣬�Ѿ������߳���ʦ�š�", m_pInfo->setMember[0].szName);
						pUserStu->SendEmail(pUserStu->GetName(), szMsg, pUserStu);
					}
					break;
				case LINEBROAD_APPLYTUTOR://��ʦ����
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
						sprintf(szMsg, "���%s��������˰�ʦ���룬�뵽ʦͽ����鿴��", m_pInfo->setMember[0].szName);
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
				case LINEBROAD_STATE://����ʦͽ״̬
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
				case LINEBROAD_AGREEAPPLY://ʦ��ͬ����ͽ
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

						//ͬ���ʱ����IP�ж��ˣ�ֻ�жϰ�ʦ��ʱ��
// 						if (pStu->GetIPAddress()==m_pInfo->setMember[0].usMainJob)
// 						{
// 							//֪ͨ�Է���ͽʧ�ܣ���״̬�Ļ�ȥ
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
// 							if (msg.Create(SYSTEM_NAME, m_pInfo->setMember[0].szName, "����ҵ�½IP������ͬ���޷����ʦͽ��ϵ", NULL, 0xff0000, _TXTATR_TOPSHOW))
// 								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,m_pInfo->idUser);
// 
// 							return;
// 						}
						if (pStu->GetTutor())//�Ѿ���ʦ����
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
						sprintf(szMsg, "���%s��������İ�ʦ���룬����������ʽ��Ϊʦͽ��", m_pInfo->setMember[0].szName);
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
								///���߼Ӻ���ʱ���ϵ������������ѯ��������״̬
								pFriendMsg.SetTransData(pStu->GetID());
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&pFriendMsg);
								
								char szTemp[128];
								sprintf(szTemp,"<font color=\"#ffff00\">���</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">Ϊ���ѳɹ�!</font>",pData->GetTutorName());
								pStu->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
							}
						}
					}
					break;
				case LINEBROAD_REFUSEAPPLY://ʦ���ܾ���ͽ
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
						sprintf(szMsg, "���ź������%s�ܾ�����İ�ʦ����", m_pInfo->setMember[0].szName);
						pStu->SendEmail(pStu->GetName(), szMsg, pStu);
					}
					break;
				case LINEBROAD_ONLINESTATE://����״̬
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
						//���Լ���״̬����
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
				case LINEBROAD_ONLINESTATEREF://����״̬,����Ҫ����
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
				case LINEBROAD_CALLSTU://�ٻ�ͽ��
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
							if (msg.Create(SYSTEM_NAME, m_pInfo->setMember[0].szName, "�Է��ڸ����У��޷��ٻ���", NULL, 0xff0000, _TXTATR_TOPSHOW))
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,m_pInfo->idUser);
							return;
						}

						if (pUserTemp->GetMap()->GetDocID()==MAP_PRISON || pUserTemp->GetMap()->GetDocID()==1026 || pUserTemp->GetMap()->IsPkGameMap() || g_SpecilLine==1)
						{
							CMsgTalk msg;
							if (msg.Create(SYSTEM_NAME, m_pInfo->setMember[0].szName, "�Է����ڵĳ��������ٻ���", NULL, 0xff0000, _TXTATR_TOPSHOW))
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,m_pInfo->idUser);
							return;
						}
						if (pUserTemp->GetItemByType(_SPCITEM_MONEYFORSHOP))
						{
							CMsgTalk msg;
							if (msg.Create(SYSTEM_NAME, m_pInfo->setMember[0].szName, "�Է��������У��޷��ٻ���", NULL, 0xff0000, _TXTATR_TOPSHOW))
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,m_pInfo->idUser);
							return;
						}
						if (!pUserTemp->GetTutorManager())
							return;

						pUserTemp->GetTutorManager()->SetBeCalled(true);
						pUserTemp->GetTutorManager()->SetCalledPos(m_pInfo->setMember[0].usLev, m_pInfo->setMember[0].usMainLev, m_pInfo->setMember[0].usSubLev, m_pInfo->setMember[0].ucLine);

						//֪ͨͽ�ܣ�����ͽ�����
						CMsgSchoolMember msg;
						if (msg.Create(pUserTemp->GetID(), MESSAGEBOARD_CALLSTU))
						{
							m_pInfo->setMember[0].idMember = pData->GetTutorID();
							msg.Append(m_pInfo->setMember[0]);
							pUserTemp->SendMsg(&msg);
						}
					}
					break;
				case LINEBROAD_CALLSTUREF://ͽ����Ӧ�ٻ�
					{
						//�ٻ�����Ҫ�۵��ߣ�����ע�͵�
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
				case LINEBROAD_ADDEDLEV://��ͽ�������ĵȼ�
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
				case LINEBROAD_APPLYBYNAME://�����ְ�ʦ
					{
						if (m_pInfo->setMember[0].idMember==ID_NONE)
							return;
						CUser* pTarget = UserManager()->GetUser(m_pInfo->setMember[0].idMember);
						if (!pTarget)
							return;

						if(!pTarget->GetTutorManager())
							return;

						if (pTarget->GetLev() - m_pInfo->setMember[0].usLev < 10 )//����ȶԷ��ĵȼ�Ҫ��10
						{
							CMsgTalk msg;
							if (msg.Create(SYSTEM_NAME, m_pInfo->setMember[0].szName, "ֻ�ܰݵȼ�������10�������Ϊʦ", NULL, 0xff0000, _TXTATR_TOPSHOW))
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,m_pInfo->idUser);

							return;
						}
						#if (!defined(_DEBUG) && !defined(_ASHRELEASE))
						if (m_pInfo->setMember[0].dwMaxScore==pTarget->GetIPAddress())
						{
							CMsgTalk msg;
							if (msg.Create(SYSTEM_NAME, m_pInfo->setMember[0].szName, "����ҵ�½IP������ͬ���޷����ʦͽ��ϵ", NULL, 0xff0000, _TXTATR_TOPSHOW))
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,m_pInfo->idUser);

							return;
						}
						#endif
						if (pTarget->IsStudentFull())
						{
							CMsgTalk msg;
							if (msg.Create(SYSTEM_NAME, m_pInfo->setMember[0].szName, "�Է�ͽ������", NULL, 0xff0000, _TXTATR_TOPSHOW))
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,m_pInfo->idUser);

							return;
						}
						if (pTarget->GetTutorApplyAmount()>=20)
						{
							CMsgTalk msg;
							if (msg.Create(SYSTEM_NAME, m_pInfo->setMember[0].szName, "�Է������б�����", NULL, 0xff0000, _TXTATR_TOPSHOW))
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,m_pInfo->idUser);

							return;
						}

						CTutorData* pData = pTarget->GetStudent(m_pInfo->idUser);
						if(pData)//������ͽ��
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
						sprintf(szMsg, "���%s��������˰�ʦ���룬�뵽ʦͽ����鿴��", m_pInfo->setMember[0].szName);
						pTarget->SendEmail(pTarget->GetName(), szMsg, pTarget);

						CMsgTalk msg;
						if (msg.Create(SYSTEM_NAME, m_pInfo->setMember[0].szName, "��İ�ʦ�����Ѿ�����", NULL, 0xff0000, _TXTATR_TOPSHOW))
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
			//goto ��ʱ������
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
	case MESSAGEBOARD_QUERYINFO://��ѯ��ϸ
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
	case MESSAGEBOARD_QUERYTOTURLIST://��ѯʦ���б�
		{
			pUser->QueryTutorList( m_pInfo->ucPage, m_pInfo->ucAmount );
		}
		break;
	case MESSAGEBOARD_ADDTOLIST:
		{
			if (g_TutorListSystem.GetTutorAmount() >= _TUTOR_LIST_NUM)//�б������Ѿ��ﵽ����
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "ʦ���б�Ǽ������Ѵﵽ���ޣ�");
				return;
			}
			if (pUser->GetLev() < _MIN_TUTOR_LEVEL)
				return;
			if (g_TutorListSystem.FindTutorByID(pUser->GetID()))
			{
				if (!pUser->IsInTutorList())
					pUser->AddTutorListMask();
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "���Ѿ��Ǽǹ��ˣ�");
				return;
			}
			if (pUser->IsStudentFull())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "����ͽ����������������Ҫ�ٵǼ���ͽ��");
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
	case MESSAGEBOARD_APPLYADDTUTOR://�����ʦ
		{
			if (pUser->GetLev()<10)//����ȼ�����������
				return;
			if (pUser->GetTutor())//�Ѿ���ʦ����
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"���Ѿ���ʦ���ˣ�");
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
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "���Ѿ��ڶԷ��������б����ˣ�");
					return;
				}
// 				else if (pTutorData->GetState()==TUTORSTATE_GRADUATED)//�����ʦ���ٰ�ͬһ����Ϊʦ
// 					pUser->SendSysMsg(_TXTATR_TOPSHOW, "���Ѿ��ڶԷ����³�ʦ�ˣ�");
			}

			CUser* pTutor = NULL;
			if(m_pInfo->setMember[0].idMember!=ID_NONE)
				pTutor = UserManager()->GetUser(m_pInfo->setMember[0].idMember);

			if(!pTutor && strlen(m_pInfo->setMember[0].szName)>0)//Ϊ��֧����������ʦ
				pTutor = UserManager()->GetUser(m_pInfo->setMember[0].szName);

			if (pTutor)
			{
				if (pTutor->GetLev()-pUser->GetLev() < 10 )//����ȶԷ��ĵȼ�Ҫ��10
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "ֻ�ܰݵȼ�������10�������Ϊʦ");
					return;
				}
				if (pTutor->IsStudentFull())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "�Է�ͽ��������");
					return;
				}
				#if (!defined(_DEBUG) && !defined(_ASHRELEASE))//�������
				if (pTutor->GetIPAddress()==pUser->GetIPAddress())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "����ҵ�½IP������ͬ���޷����ʦͽ��ϵ");
					return;
				}
				#endif
				if (pTutor->GetTutorApplyAmount()>=20)
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "�Է������б�����");
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

				//������Ϣ��һ��װ���£���Ūһ��
				ST_MEMBERINFO infoExt = {0};
				infoExt.dwMaxScore = pTutor->GetMaxScore();
				infoExt.dwExitTime = 0;
				infoExt.nImpart = pTutor->GetMorality();
				OBJID id = pUser->ApplyAddTutor(&info, TUTORSTATE_APPLYREF, &infoExt);
				CHECK(id);
				char szMsg[256] = "";
				sprintf(szMsg, "���%s��������˰�ʦ���룬�뵽ʦͽ����鿴��", pUser->GetName());
				
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
				if (strcmp(szServer, "һ��") == 0)
					nLine = 1;
				else if (strcmp(szServer, "����") == 0)
					nLine = 2;
				else if (strcmp(szServer, "����") == 0)
					nLine = 3;
				else if (strcmp(szServer, "����") == 0)
					nLine = 4;
				else if (strcmp(szServer, "����") == 0)
					nLine = 5;
				else if (strcmp(szServer, "����") == 0)
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
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "��İ�ʦ�����Ѿ�����");
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

				if (!pTutorInfo)//����ʦ���б��ϵ�
				{
					if(strlen(m_pInfo->setMember[0].szName)<=0)
						return;
					//pUser->SendSysMsg(_TXTATR_TOPSHOW, "�Է��Ѳ���ʦ���б��У���ˢ���б�");
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
				if (pTutorInfo->usLev - pUser->GetLev() < 10 )//����ȶԷ��ĵȼ�Ҫ��10
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "ֻ�ܰݵȼ�������10�������Ϊʦ");
					return;
				}
				#if (!defined(_DEBUG) && !defined(_ASHRELEASE))
				if (pTutorInfo->uIPAddress==pUser->GetIPAddress())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "����ҵ�½IP������ͬ���޷����ʦͽ��ϵ");
					return;
				}
				#endif
				int nRet = g_TutorListSystem.GetTutorApplyAmount(m_pInfo->setMember[0].idMember, pTutorInfo->dwImpart);
				if (nRet == 1)
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "�Է�ͽ��������");
					return;
				}
				else if (nRet == 2)
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "�Է������б�����");
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
				if (strcmp(szServer, "һ��") == 0)
					nLine = 1;
				else if (strcmp(szServer, "����") == 0)
					nLine = 2;
				else if (strcmp(szServer, "����") == 0)
					nLine = 3;
				else if (strcmp(szServer, "����") == 0)
					nLine = 4;
				else if (strcmp(szServer, "����") == 0)
					nLine = 5;
				else if (strcmp(szServer, "����") == 0)
					nLine = 6;
				infoStu.ucLine = nLine;
				if (pUser->IsVipActive())
					infoStu.ucVipLev = pUser->GetVipLev();
				
				SafeCopy(infoStu.szName, pUser->GetName(), _MAX_NAMESIZE);

				pUser->SendSysMsg(_TXTATR_TOPSHOW, "��İ�ʦ�����Ѿ�����");

				CMsgSchoolMember msg;
				if (msg.Create(pTutorInfo->idUser, MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_APPLYTUTOR))//�������ط���ͬ������idUser�ŵ��ǶԷ���ID
				{
					msg.Append(infoStu);
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
				}
			}
		}
		break;
	case MESSAGEBOARD_AGREESTU://ͬ���ʦ
		{
			int nStep = 0;
			DEBUG_TRY
			if (pUser->GetLev()<35)
				return;
			if (pUser->IsStudentFull())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "ͽ��������");
				return;
			}
			if (m_pInfo->setMember[0].idMember==0 || pUser->GetID()==m_pInfo->setMember[0].idMember)
				return;
			if (!pUser->GetTutorManager())
				return;
			nStep=1;
			CTutorData* pDataTutor = pUser->GetStudent(m_pInfo->setMember[0].idMember);//�Է���û�������
			if (!pDataTutor)
				return;
			if (pDataTutor->GetState()!=TUTORSTATE_APPLY && pDataTutor->GetState()!=TUTORSTATE_APPLYREF)
				return;
			CUser* pUserStu = UserManager()->GetUser(m_pInfo->setMember[0].idMember);
			nStep=2;
			if (pUserStu)
			{
				//ͬ���ʱ����IP�ж��ˣ�ֻ�жϰ�ʦ��ʱ��
// 				if (pUserStu->GetIPAddress()==pUser->GetIPAddress())
// 				{
// 					pUser->SendSysMsg(_TXTATR_TOPSHOW, "����ҵ�½IP������ͬ���޷����ʦͽ��ϵ");
// 					return;
// 				}
				nStep=3;
				CTutorData* pDataStu = pUserStu->GetTutorManager()->FindTutorByID(pUser->GetID());
				CHECK(pDataStu);

				if (pUserStu->GetLev()+10 > pUser->GetLev())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "ֻ���յȼ����Լ�С10�������Ϊͽ�ܣ�");
					return;
				}
				if (pUserStu->GetTutor())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "�Է��Ѿ���ʦ���ˣ�");
					return;
				}
				nStep=4;
				pDataTutor->SetState(TUTORSTATE_NORMAL);
				pDataTutor->SetTutorGiftMask(pUser->CalcTutorGiftMask(pUserStu->GetLev()));
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "��ɹ�����һ��ͽ��");
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
				sprintf(szMsg, "���%s��������İ�ʦ���룬����������ʽ��Ϊʦͽ��", pUser->GetName());
				pUserStu->SendEmail(pUserStu->GetName(), szMsg, pUserStu);
				nStep=6;
				//˫����Ϊ����
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
						sprintf(szTemp,"<font color=\"#ffff00\">���</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">Ϊ���ѳɹ�!</font>",pUserStu->GetName());
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
						sprintf(szTemp,"<font color=\"#ffff00\">���</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">Ϊ���ѳɹ�!</font>",pUser->GetName());
						pUserStu->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
						pUser->AddBeFriend(0,pUserStu->GetID(),nGroup,pUserStu->GetName());
				    }
				}
				nStep=7;
				if (pUserStu->IsFirstTutor())
				{
					//���״ΰ�ʦ����
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
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "�Է��Ѿ���ʦ���ˣ�");
					return;
				}
				nStep=9;
				pDataTutor->SetState(TUTORSTATE_AGREED);
				pDataTutor->SetTutorGiftMask(pUser->CalcTutorGiftMask(pDataTutor->GetInt(TUTORDATA_LEV)));
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "��ɹ�����һ��ͽ��");
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
					if (strcmp(szServer, "һ��") == 0)
						nLine = 1;
					else if (strcmp(szServer, "����") == 0)
						nLine = 2;
					else if (strcmp(szServer, "����") == 0)
						nLine = 3;
					else if (strcmp(szServer, "����") == 0)
						nLine = 4;
					else if (strcmp(szServer, "����") == 0)
						nLine = 5;
					else if (strcmp(szServer, "����") == 0)
						nLine = 6;
					info.ucLine = nLine;
					msg.Append(info);
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
				}
				nStep=11;
				//�ӶԷ�Ϊ����
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
						///���߼Ӻ���ʱ���ϵ������������ѯ��������״̬
						pFriendMsg.SetTransData(pUser->GetID());
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&pFriendMsg);
						
						char szTemp[128];
						sprintf(szTemp,"<font color=\"#ffff00\">���</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">Ϊ���ѳɹ�!</font>",pDataTutor->GetStuName());
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
	case MESSAGEBOARD_REFUSESTU://�ܾ���ʦ����
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
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "��ɾ����һ������");

				pUserStu->GetTutorManager()->DelTutorApply(pUser->GetID());
				char szMsg[256] = "";
				sprintf(szMsg, "���ź������%s�ܾ�����İ�ʦ����", pUser->GetName());
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
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "��ɾ����һ������");

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
	case MESSAGEBOARD_TEAMADD://ʦͽ��ӽ���ʦͽ��ϵ
		{
			pUser->ProcessAction(2011714);
		}
		break;
	case MESSAGEBOARD_TEAMADDREF://ͽ����Ӧ��ʦ
		{
			CTeam* pTeam = pUser->GetTeam();
			if (!pTeam)
				return;
			CUser* pTutor = NULL;
			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, pTeam->GetLeader(), IPP_OF(pTutor)))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է��Ѳ�����ĸ���"); //�Է��Ѳ�����ĸ�����
				return;
			}
			if(!pTutor->GetTutorManager())
				return;
			if (m_pInfo->ucPage==2)//ͽ�ܾܾ�
			{
				char szTemp[128] = "";
				sprintf(szTemp, "%s��Ը�����Ϊʦ", pUser->GetName());
				pTutor->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
				pTutor->FetchApply(CUser::APPLY_TEACHERAPPLY, pUser->GetID());
				return;
			}
			if(pUser->GetTutor())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"���Ѿ���ʦ����"); //�Է��Ѳ�����ĸ�����
				return;
			}
			if(pTutor->GetLev()-pUser->GetLev()<10)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "ֻ�ܰݵȼ�������10�������Ϊʦ");
				return;
			}
			if (pTutor->IsStudentFull())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "�Է�ͽ����������");
				return;
			}
			#if (!defined(_DEBUG) && !defined(_ASHRELEASE))
			if (pTutor->GetIPAddress()==pUser->GetIPAddress())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "����ҵ�½IP������ͬ���޷����ʦͽ��ϵ");
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
				
				//������Ϣ��һ��װ���£���Ūһ��
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
				if (strcmp(szServer, "һ��") == 0)
					nLine = 1;
				else if (strcmp(szServer, "����") == 0)
					nLine = 2;
				else if (strcmp(szServer, "����") == 0)
					nLine = 3;
				else if (strcmp(szServer, "����") == 0)
					nLine = 4;
				else if (strcmp(szServer, "����") == 0)
					nLine = 5;
				else if (strcmp(szServer, "����") == 0)
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
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "��ʦ��ɣ�����������ʽ��Ϊʦͽ��");
					pTutor->SendSysMsg(_TXTATR_TOPSHOW, "��ɹ�����һ��ͽ��");
				}

				//˫����Ϊ����
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
						sprintf(szTemp,"<font color=\"#ffff00\">���</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">Ϊ���ѳɹ�!</font>",pTutor->GetName());
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
						sprintf(szTemp,"<font color=\"#ffff00\">���</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">Ϊ���ѳɹ�!</font>",pUser->GetName());
						pTutor->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
						pUser->AddBeFriend(0,pTutor->GetID(),nGroup,pTutor->GetName());
					}
				}
				if (pUser->IsFirstTutor())
				{
					//���״ΰ�ʦ����
					pUser->ProcessAction(2011710);
					pUser->SetNotFirstTutor();
				}
			}
		}
		break;
	case MESSAGEBOARD_GRADUATEAPPLY://ʦ����ͽ�ܳ�ʦ
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
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "�Է��Ѳ�����ĸ���");
// 				return;
// 			}
			pUser->ProcessAction(2011713);
		}
		break;
	case MESSAGEBOARD_GRADUATEREF://ͽ����Ӧ��ʦ
		{
			// find target
			CTeam* pTeam = pUser->GetTeam();
			if (!pTeam)
				return;
			CUser* pTarget = NULL;
			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, pTeam->GetLeader(), IPP_OF(pTarget)))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է��Ѳ�����ĸ���"); //�Է��Ѳ�����ĸ�����
				return;
			}
			if (m_pInfo->ucPage==2)//ͽ�ܾܾ�
			{
				char szTemp[128] = "";
				sprintf(szTemp, "���ͽ��%s����Ը���ʦ", pUser->GetName());
				pTarget->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
				pTarget->FetchApply(CUser::APPLY_GRADUATE, pUser->GetID());
				return;
			}
			if (pTarget->FetchApply(CUser::APPLY_GRADUATE, pUser->GetID()))
				pTarget->ProcessAction(2011701);
		}
		break;
	case MESSAGEBOARD_DIVORCE://Э����ʦͽ��ϵ
		{
			pUser->ProcessAction(2011702);
		}
		break;
	case MESSAGEBOARD_REQDIVORCE://ͽ����Ӧ�����ϵ
		{
			if (!pUser->GetTutor())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "����û��ʦ��");
				return;
			}
			// find target
			CTeam* pTeam = pUser->GetTeam();
			if (!pTeam)
				return;
			CUser* pTarget = NULL;
			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, pTeam->GetLeader(), IPP_OF(pTarget)))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW,"�Է��Ѳ�����ĸ���"); //�Է��Ѳ�����ĸ�����
				return;
			}
			if (m_pInfo->ucPage==2)//ͽ�ܾܾ�
			{
				char szTemp[128] = "";
				sprintf(szTemp, "���ͽ��%s��Ը��������ʦͽ��ϵ", pUser->GetName());
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
				sprintf(szTemp, "����%s��ʦͽ��ϵ�Ѿ����",pUser->GetName());
				pTarget->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
				
				sprintf(szTemp, "����%s��ʦͽ��ϵ�Ѿ����",pTarget->GetName());
				pUser->SendSysMsg(_TXTATR_TOPSHOW, szTemp);
			}
		}
		break;
	case MESSAGEBOARD_STUREBEL://ͽ����ʦ
		{
			pUser->ProcessAction(2011703);
		}
		break;
	case MESSAGEBOARD_TUTORKICKOUTSTU://ʦ���߳�ͽ��
		{
			if (m_pInfo->setMember[0].idMember==ID_NONE || m_pInfo->setMember[0].idMember==pUser->GetID())
				return;
			LuaScriptMachine()->InitVar(m_pInfo->setMember[0].idMember);
			pUser->ProcessAction(2011704);
		}
		break;
	case MESSAGEBOARD_QUERYSTUAPPLYLIST://��ѯͽ�������б�
		{
			if (!pUser->GetTutorManager())
				return;

			pUser->GetTutorManager()->SendStuApplyList(m_pInfo->ucPage, m_pInfo->ucAmount);
		}
		break;
// 	case MESSAGEBOARD_QUERYSTU://ͽ���б�
// 		{
// 			if (!pUser->GetTutorManager())
// 				return;
// 			
// 			pUser->GetTutorManager()->SendStuList();
// 		}
// 		break;
	case MESSAGEBOARD_ALLLIST://ʦͽ�б�
		{
			if (!pUser->GetTutorManager())
				return;
			
			pUser->GetTutorManager()->SendAllList();
		}
		break;
	case MESSAGEBOARD_DELAPPLYLIST://ɾ��ͽ�������б��е�ͽ��
		{
			if (m_pInfo->setMember[0].idMember==0 || pUser->GetID()==m_pInfo->setMember[0].idMember)
				return;
			if (!pUser->GetTutorManager())
				return;
			CTutorData* pData = pUser->GetTutorManager()->FindStudentByID(m_pInfo->setMember[0].idMember);
			if (!pData)
				return;

			if (pUser->GetTutorManager()->DelStuApply(m_pInfo->setMember[0].idMember))
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "��ɾ����һ������");

			CMsgSchoolMember msg;
			if (msg.Create(pUser->GetID(), MESSAGEBOARD_DELAPPLYLIST))
			{
				msg.m_pInfo->ucAmount = 1;
				msg.m_pInfo->setMember[0].idMember = m_pInfo->setMember[0].idMember;
				pUser->SendMsg(&msg);
			}
		}
		break;
	case MESSAGEBOARD_CALLSTU://�ٻ�ͽ��
		{
			if (m_pInfo->setMember[0].idMember==0 || pUser->GetID()==m_pInfo->setMember[0].idMember)
				return;

			//�ж���û���ٻ�����
// 			if (!pUser->GetItemByType(506002))
// 			{
// 				pUser->SendSysMsg(_TXTATR_TOPSHOW, "ȱ���ٻ���Ҫ�ĵ��ߣ�");
// 				return;
// 			}
			CTutorData* pData = pUser->GetStudent(m_pInfo->setMember[0].idMember);
			if (!pData)
				return;

			if (!pUser->GetMap())
				return;
			if (pUser->GetMap()->IsDynaMap())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "�����в����ٻ���");
				return;
			}
			if (pUser->GetMap()->IsSynMap())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "�����в����ٻ���");
				return;
			}
			if (pUser->GetMap()->GetDocID()==MAP_PRISON || pUser->GetMap()->GetDocID()==1026 || pUser->GetMap()->IsPkGameMap() || g_SpecilLine == 1 )//�����͵ظ������ٻ�
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "�ó����޷��ٻ���");
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
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "�Է��ڸ����У��޷��ٻ���");
					return;
				}
				if (pStudent->GetItemByType(_SPCITEM_MONEYFORSHOP))
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "�Է��������У��޷��ٻ���");
					return;
				}
				if (pStudent->GetMap()->GetDocID()==MAP_PRISON || pStudent->GetMap()->GetDocID()==1026 || pStudent->GetMap()->IsPkGameMap())
				{
					pUser->SendSysMsg(_TXTATR_TOPSHOW, "�Է����ڵĳ��������ٻ���");
					return;
				}
				//֪ͨͽ�ܣ�����ͽ�����
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
	case MESSAGEBOARD_CALLSTUREF://ͽ����Ӧ�ٻ�
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
					pTutor->SendSysMsg(_TXTATR_TOPSHOW, "�Է��ܾ����ٻ���");
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
				//�۳�ʦ������
//				CHECK(pTutor->SpendItem(pItem));

			}
			else
			{
				if (m_pInfo->ucPage==2)
				{
					CMsgTalk msg;
					if (msg.Create(SYSTEM_NAME, pData->GetTutorName(), "�Է��ܾ����ٻ���", NULL, 0xff0000, _TXTATR_TOPSHOW))
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,pData->GetTutorID());
					return;
				}
				
				if (pUser->FlyMap(pCallPos->nMapID, pCallPos->nPosX, pCallPos->nPosY)==-1)
					return;
				pUser->GetTutorManager()->SetBeCalled(false);
				//֪ͨʦ��ɾ������
// 				CMsgSchoolMember msg;
// 				if (msg.Create(pUser->GetID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_CALLSTUREF))
// 				{
// 					ST_MEMBERINFO info = {0};
// 					info.idMember = pData->GetTutorID();
// 					msg.Append(info);
// 					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
// 				}
				//֪ͨ�ͻ��˻���
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
	case MESSAGEBOARD_GETGIFT://ʦ����ȡͽ�ܵĳɳ�����
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
	case MESSAGEBOARD_HASGRADUATE://�Ƿ�ӵ�г���ʦ��ͽ��
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
	case MESSAGEBOARD_GRADUATESTU://�����ѳ�ʦ��ͽ���б�
		{
			if (!pUser->GetTutorManager())
				return;
			
			pUser->GetTutorManager()->SendGraduateStuList(m_pInfo->ucPage, m_pInfo->ucAmount);
		}
		break;
	case MESSAGEBOARD_ANSWERQUESTION://ʦͽ����
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
			//�Ѷ������ʦͽȫ�ҳ���
			vector<CUser*> vTutor;
			CUser* pTutor = NULL;
			if(!pUser->FindAroundObj( OBJ_USER,pTeam->GetLeader(),( void** )&pTutor ))//������ʦ�����ӳ�
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

			//����Ƿ������˶��Ѿ��ύ��,���Ƿ�ȫ����ͬ
			int bWrong = 0;
			for(i = vTutor.size()-1; i>=0; i--)
			{
				if(vTutor[i]->GetTutorAnswer()==0)//���˻�û��
					return;
				else if(vTutor[i]->GetTutorAnswer()!=pTutor->GetTutorAnswer())//���˴𰸲�һ��
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
