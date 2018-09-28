// UserTutor.cpp: implementation of the CUserTutor class.
//
//////////////////////////////////////////////////////////////////////

#include "UserTutor.h"
#include "MapGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserTutor::CUserTutor(PROCESS_ID idProcess, CUser* pUser)
{
	ASSERT (pUser);
	
	m_idProcess = idProcess;
	m_pUser		= pUser;
	
	m_setStu	= CTutorSet::CreateNew(true);
	m_setTutor  = CTutorSet::CreateNew(true);

	m_bCalled	= false;//是否被师父召唤了
	
	memset(&m_stCalledPos, 0L, sizeof(m_stCalledPos));
}

CUserTutor::~CUserTutor()
{
	if(m_setStu)
		SAFE_RELEASE(m_setStu);
	if(m_setTutor)
		SAFE_RELEASE(m_setTutor);
}
//////////////////////////////////////////////////////////////////////////
CUserTutor* CUserTutor::CreateNew(PROCESS_ID idProcess, CUser* pUser)
{
	CHECKF (pUser);
	
	CUserTutor* pUserTutor = new CUserTutor(idProcess, pUser);
	CHECKF(pUserTutor);
	
	return pUserTutor;
}
//////////////////////////////////////////////////////////////////////
bool CUserTutor::CreateAll()
{
	CHECKF(CMapGroup::IsValidMapGroupProcessID(m_idProcess));
	CHECKF(m_setTutor);
	CHECKF(m_setStu);
	CHECKF(m_pUser);
	
	//加载师父数据
	char szSQL[256]="";
	char szSQL2[256] = "";
	sprintf(szSQL,"SELECT *,0,0,0,0,0,0,0,0,0,0,0,0,0 FROM %s WHERE stu_id=%u AND state!=%d LIMIT 100", _TBL_TUTOR, m_pUser->GetID(),TUTORSTATE_STUREBEL);
	
	IRecordset* pRes = Database()->CreateNewRecordset( szSQL, true );
	if( pRes )
	{
		OBJID idTutor = 0;
		for( int i = 0; i < pRes->RecordCount(); i++ )
		{
			idTutor = pRes->LoadDWord("tutor_id");
			if(!idTutor)
			{
				::LogSave("CUserTutor::CreateAll() tutor id error!sql='%s'", szSQL);
				pRes->MoveNext();
				continue;
			}


			CUser* pTutor = UserManager()->GetUser(idTutor);
			if(pTutor)
			{
				CTutorData* pData = CTutorData::CreateNew();
				if(!pData)
				{
					pRes->Release();
					return false;
				}

				if(!pData->Create(pRes, pTutor))
				{
					pData->ReleaseByOwner();
					pRes->Release();
					return false;
				}

				m_setTutor->AddObj(pData);
				pRes->MoveNext();
				continue;
			}
			else
			{
				stTempUser* pTempTutor = UserManager()->QueryTempUser(idTutor);
				if(pTempTutor)
				{
					CTutorData* pData = CTutorData::CreateNew();
					if(!pData)
					{
						pRes->Release();
						return false;
					}
					
					if(!pData->Create(pRes, pTempTutor))
					{
						pData->ReleaseByOwner();
						pRes->Release();
						return false;
					}
					
					m_setTutor->AddObj(pData);
					pRes->MoveNext();
					continue;
				}
			}
			pRes->MoveNext();
		}
		pRes->Release();
	}

	//加载徒弟数据
	sprintf(szSQL,"SELECT *,0,0,0,0,0,0,0,0,0,0,0,0,0 FROM %s WHERE tutor_id=%u AND state!=%d LIMIT 100", _TBL_TUTOR, m_pUser->GetID(),TUTORSTATE_TUTORKICKOUT);
	pRes = Database()->CreateNewRecordset( szSQL, true );
	if( pRes )
	{
		OBJID idStu = 0;
		for( int i = 0; i < pRes->RecordCount(); i++ )
		{
			idStu = pRes->LoadDWord("stu_id");
			if(!idStu)
			{
				::LogSave("CUserTutor::CreateAll() student id error!sql='%s'", szSQL);
				pRes->MoveNext();
				continue;
			}
			CUser* pStu = UserManager()->GetUser(idStu);
			if(pStu)
			{
				CTutorData* pData = CTutorData::CreateNew();
				if(!pData)
				{
					pRes->Release();
					return false;
				}
				
				if(!pData->Create(pRes, pStu))
				{
					pData->ReleaseByOwner();
					pRes->Release();
					return false;
				}
				
				m_setStu->AddObj(pData);
				pRes->MoveNext();
				continue;
			}
			else
			{
				stTempUser* pTempStu = UserManager()->QueryTempUser(idStu);
				if(pTempStu)
				{
					CTutorData* pData = CTutorData::CreateNew();
					if(!pData)
					{
						pRes->Release();
						return false;
					}
					
					if(!pData->Create(pRes, pTempStu))
					{
						pData->ReleaseByOwner();
						pRes->Release();
						return false;
					}
					
					m_setStu->AddObj(pData);
					pRes->MoveNext();
					continue;
				}
			}
			pRes->MoveNext();
		}
		pRes->Release();
	}
	return true;
}

bool CUserTutor::SendLeaveMsg()
{
	CHECKF(m_setTutor);
	CHECKF(m_setStu);
	CHECKF(m_pUser);
	
	DEBUG_TRY
	int i = 0;
	for(i = m_setTutor->GetAmount()-1; i >= 0; i--)
	{
		CTutorData* pData = m_setTutor->GetObjByIndex(i);
		if(pData)
		{
			if (pData->GetState()==TUTORSTATE_TUTORKICKOUT)
			{
				char szMsg[256] = "";
				sprintf(szMsg, "你的师傅%s对你的表现很不满意，已经将你踢出了师门。", pData->GetTutorName());
				m_pUser->SendEmail(m_pUser->GetName(), szMsg, m_pUser);
				pData->DeleteRecord();
				m_setTutor->DelObj(pData->GetID());
			}
			else if (pData->GetState()==TUTORSTATE_AGREED)
			{
				char szMsg[256] = "";
				sprintf(szMsg, "玩家%s接受了你的拜师申请，现在你们正式成为师徒了", pData->GetTutorName());
				m_pUser->SendEmail(m_pUser->GetName(), szMsg, m_pUser);
				pData->SetState(TUTORSTATE_NORMAL);
				//加对方为好友
				int nGroup = 0;
				if (m_pUser->GetFriendGroupAmount(1)<MAX_GROUP_FRIEND)
					nGroup = 1;
				else if (m_pUser->GetFriendGroupAmount(2)<MAX_GROUP_FRIEND)
					nGroup = 2;
				else if (m_pUser->GetFriendGroupAmount(3)<MAX_GROUP_FRIEND)
					nGroup = 3;
				else if (m_pUser->GetFriendGroupAmount(4)<MAX_GROUP_FRIEND)
					nGroup = 4;
				if (nGroup!=0)
				{
					CUser* pTarget = UserManager()->GetUser(pData->GetTutorID());
					if (pTarget)
					{
						if(m_pUser->AddFriend(pTarget->GetID(), pTarget->GetName(),nGroup,pTarget->GetMate()))
						{ 
							CMsgFriend pMsg;
							pMsg.Create(_FRIEND_ADDSUCCESS,
								pTarget->GetID(),
								pTarget->GetName(),
								CMsgFriend::ONLINE_STATUS,
								pTarget->GetLookFace(),
								pTarget->GetLev(),
								pTarget->GetProfession(),
								0,//friendship
								pTarget->GetSex(),
								nGroup,//frinedgroup
								pTarget->GetFeel(),
								0);//relation
							m_pUser->SendMsg(&pMsg);
							char szTemp[128];
							sprintf(szTemp,"<font color=\"#ffff00\">添加</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">为好友成功!</font>",pTarget->GetName());
							m_pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
							pTarget->AddBeFriend(0,m_pUser->GetID(),nGroup,m_pUser->GetName());
						}
					}
					else
					{
						CMsgFriend pFriendMsg;
						OBJID dwFriendID = m_pUser->AddLeaveFriend(pData->GetTutorName(), nGroup, &pFriendMsg);
						if(dwFriendID)
						{
							pFriendMsg.m_pInfo->idFriend=dwFriendID;
							
							pFriendMsg.m_pInfo->ucAction=_FRIEND_ADDSUCCESS_PASSIVE;
							///离线加好友时向关系服务器求助查询好友在线状态
							pFriendMsg.SetTransData(m_pUser->GetID());
							MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&pFriendMsg);
							
							char szTemp[128];
							sprintf(szTemp,"<font color=\"#ffff00\">添加</font><font color=\"#ff0000\">[%s]</font><font color=\"#ffff00\">为好友成功!</font>",pData->GetTutorName());
							m_pUser->SendSysMsg(_TXTATR_TOPSHOW,szTemp);
						}
					}
				}
				if (m_pUser->IsFirstTutor())
				{
					//给首次拜师奖励
					m_pUser->ProcessAction(2011710);
					m_pUser->SetNotFirstTutor();
				}
			}
			else if (pData->GetState()==TUTORSTATE_REFUSED)
			{
				char szMsg[256] = "";
				sprintf(szMsg, "很遗憾，玩家%s拒绝了你的拜师申请", pData->GetTutorName());
				m_pUser->SendEmail(m_pUser->GetName(), szMsg, m_pUser);
				pData->DeleteRecord();
				m_setTutor->DelObj(pData->GetID());
			}
		}
	}
	
	for(i = m_setStu->GetAmount()-1; i >=0; i--)
	{
		CTutorData* pData = m_setStu->GetObjByIndex(i);
		if (pData->GetState()==TUTORSTATE_APPLY)
		{
			char szMsg[256] = "";
			sprintf(szMsg, "玩家%s向你提出了拜师申请，请到师徒界面查看吧", pData->GetStuName());
			m_pUser->SendEmail(m_pUser->GetName(), szMsg, m_pUser);
			pData->SetState(TUTORSTATE_APPLYREF);
		}
		else if (pData->GetState()==TUTORSTATE_STUREBEL)
		{
			char szMsg[256] = "";
			sprintf(szMsg, "由于你对徒弟教导不善，%s已经叛离了你的师门。", pData->GetStuName());
			m_pUser->SendEmail(m_pUser->GetName(), szMsg, m_pUser);
			pData->DeleteRecord();
			m_setStu->DelObj(pData->GetID());
		}
	}
	DEBUG_CATCH("CUserTutor::SendLeaveMsg()")
	return true;
}
//////////////////////////////////////////////////////////////////////
bool CUserTutor::SaveInfo()
{
	CHECKF(m_setTutor);
	CHECKF(m_setStu);
	CHECKF(m_pUser);

	int i = 0;
	for(i = 0; i < m_setTutor->GetAmount(); i++)
	{
		CTutorData* pData = m_setTutor->GetObjByIndex(i);
		IF_OK(pData)
		{
			pData->Update();
		}
	}

	for(i = 0; i < m_setStu->GetAmount(); i++)
	{
		CTutorData* pData = m_setStu->GetObjByIndex(i);
		IF_OK(pData)
		{
			pData->Update();
		}
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
CTutorData* CUserTutor::GetTutor()
{
	CHECKF(m_setTutor);

	for(int i = 0; i < m_setTutor->GetAmount(); i++)
	{
		CTutorData* pData = m_setTutor->GetObjByIndex(i);
		if(pData)
		{
			if(pData->GetState()==TUTORSTATE_NORMAL || pData->GetState()==TUTORSTATE_AGREED)
				return m_setTutor->GetObjByIndex(i);
		}
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////
//获得玩家徒弟申请列表数量
int	CUserTutor::GetTutorApplyAmount()
{
	CHECKF(m_setStu);
	
	int nAmount = 0;
	for(int i = 0; i < m_setStu->GetAmount(); i++)
	{
		CTutorData* pData = m_setStu->GetObjByIndex(i);
		if(pData)
		{
			if(pData->GetState()==TUTORSTATE_APPLY || pData->GetState()==TUTORSTATE_APPLYREF)
				nAmount++;
		}
	}
	return nAmount;
}
//////////////////////////////////////////////////////////////////////////
//玩家发出拜师请求，此时请求接受者还未收到
OBJID CUserTutor::ApplyAddTutor(ST_MEMBERINFO* pInfo, TUTORSTATE nState, ST_MEMBERINFO* pInfoExt)
{
	CHECKF(pInfo);
	CHECKF(m_pUser);
	CHECKF(m_setTutor);

	OBJID id = ID_NONE;
	DEBUG_TRY
	CTutorData *pTempData = FindTutorByID(pInfo->idMember);
	if(pTempData)
	{
		if(pTempData->GetState()==TUTORSTATE_GRADUATED)
		{
			pTempData->SetState(nState);
			return pTempData->GetID();
		}
		else
			return ID_NONE;
	}

	CTutorData* pData = CTutorData::CreateNew();
	CHECKF(pData);
	IF_NOT(pData->Create(GameDataDefault()->GetTutorData(), id))
		return ID_NONE;
	
	pData->SetInt(TUTORDATA_TUTORID, pInfo->idMember);
	pData->SetStr(TUTORDATA_TUTORNAME, pInfo->szName, _MAX_NAMESIZE);
	pData->SetInt(TUTORDATA_STUID, m_pUser->GetID());
	pData->SetStr(TUTORDATA_STUNAME, m_pUser->GetName(), _MAX_NAMESIZE);
	pData->SetInt(TUTORDATA_ADDEDLEV, 0);
	pData->SetTutorGiftMask(m_pUser->CalcTutorGiftMask(m_pUser->GetLev()));
	pData->SetState(nState, false);//这里一定要写false,因为这时数据库还没这条记录
	if(pInfoExt)
	{
		pData->SetInt(TUTORDATA_SEX, pInfo->ucInTeam);
		pData->SetInt(TUTORDATA_LOOKFACE, pInfo->usLookFace);
		pData->SetInt(TUTORDATA_VIPFACE, 0);
		pData->SetInt(TUTORDATA_LEV, pInfo->usLev);
		pData->SetInt(TUTORDATA_MAINJOB, pInfo->usMainJob);
		pData->SetInt(TUTORDATA_MAINLEV, pInfo->usMainLev);
		pData->SetInt(TUTORDATA_SUBJOB, pInfo->usSubJob);
		pData->SetInt(TUTORDATA_SUBLEV, pInfo->usSubLev);
		pData->SetInt(TUTORDATA_IMPART, pInfo->nImpart);
		pData->SetInt(TUTORDATA_MAXSCORE, pInfoExt->dwMaxScore);
		pData->SetInt(TUTORDATA_VIPLEV, pInfo->ucVipLev);
		pData->SetInt(TUTORDATA_EXITTIME, pInfoExt->dwExitTime);
		pData->SetInt(TUTORDATA_MORALITY, pInfoExt->nImpart);
	}
	id = pData->InsertRecord();
	if(pInfoExt)
		m_setTutor->AddObj(pData);
	else
	{
		pData->ReleaseByOwner();
		AddLeaveTutor(m_pUser->GetID(), pInfo->idMember);
	}
	DEBUG_CATCH("CUserTutor::ApplyAddTutor")
	return id;
}
CTutorData* CUserTutor::AddLeaveTutor(OBJID idStu, OBJID idTUtor)
{
	CTutorData* pData = NULL;
	DEBUG_TRY
	char szSQL[256]="";
	sprintf(szSQL,"SELECT *,0,0,0,0,0,0,0,0,0,0,0,0,0 FROM %s WHERE stu_id=%u AND tutor_id=%u LIMIT 1", _TBL_TUTOR, idStu,idTUtor);
	IRecordset* pRes = Database()->CreateNewRecordset( szSQL, true );
	if( pRes)
	{
		stTempUser* pTempTutor = UserManager()->QueryTempUser(idTUtor);
		if(pTempTutor)
		{
			pData = CTutorData::CreateNew();
			if(!pData)
			{
				pRes->Release();
				return NULL;
			}
			
			if(!pData->Create(pRes, pTempTutor))
			{
				pData->ReleaseByOwner();
				pRes->Release();
				return NULL;
			}
			
			m_setTutor->AddObj(pData);
		}
		pRes->Release();
	}
	DEBUG_CATCH("CUserTutor::AddLeaveTutor()")
	return pData;
}
//////////////////////////////////////////////////////////////////////////
//师父收到一个拜师请求，不用写数据库，徒弟请求时已经写过了
bool CUserTutor::AddNewToTutorApply(OBJID id, ST_MEMBERINFO* pInfo, TUTORSTATE nState, ST_MEMBERINFO* pInfoStuExt)
{
	CHECKF(m_pUser);
	CHECKF(m_setStu);

	DEBUG_TRY
	CTutorData* pTempData = FindStudentByID(pInfo->idMember);
	if(pTempData)
	{
		if(pTempData->GetState()==TUTORSTATE_GRADUATED)
		{
			pTempData->SetState(nState);
			return true;
		}
		else
			return false;
	}

	if(pInfoStuExt)
	{
		CTutorData* pData = CTutorData::CreateNew();
		CHECKF(pData);
		IF_NOT(pData->Create(GameDataDefault()->GetTutorData(), id))
			return false;
		
		pData->SetInt(TUTORDATA_TUTORID, m_pUser->GetID());
		pData->SetStr(TUTORDATA_TUTORNAME, m_pUser->GetName(), _MAX_NAMESIZE);
		pData->SetInt(TUTORDATA_STUID, pInfo->idMember);
		pData->SetStr(TUTORDATA_STUNAME, pInfo->szName, _MAX_NAMESIZE);
		pData->SetInt(TUTORDATA_ADDEDLEV, 0);
		pData->SetTutorGiftMask(pInfoStuExt->usSubJob);
		pData->SetState(nState);
		
		pData->SetInt(TUTORDATA_SEX, pInfo->ucInTeam);
		pData->SetInt(TUTORDATA_LOOKFACE, pInfo->usLookFace);
		pData->SetInt(TUTORDATA_VIPFACE, 0);
		pData->SetInt(TUTORDATA_LEV, pInfo->usLev);
		pData->SetInt(TUTORDATA_MAINJOB, pInfo->usMainJob);
		pData->SetInt(TUTORDATA_MAINLEV, pInfo->usMainLev);
		pData->SetInt(TUTORDATA_SUBJOB, pInfoStuExt->usSubLev);
		pData->SetInt(TUTORDATA_SUBLEV, pInfo->usSubLev);
		pData->SetInt(TUTORDATA_IMPART, pInfoStuExt->nImpart);
		pData->SetInt(TUTORDATA_MAXSCORE, pInfo->nImpart);
		pData->SetInt(TUTORDATA_VIPLEV, pInfo->ucVipLev);
		pData->SetInt(TUTORDATA_EXITTIME, pInfo->dwExitTime);
		pData->SetInt(TUTORDATA_MORALITY, pInfoStuExt->dwMaxScore);

		pData->SetLine(pInfo->ucLine);
		pData->SetTeam(pInfoStuExt->ucInTeam);
		
		m_setStu->AddObj(pData);
	}
	else
	{
		CTutorData* pData = AddNewToLeaveTutorApply(pInfo->idMember, m_pUser->GetID());
		if (!pData)
			return false;
		if (pInfo->ucLine!=0)
		{
			pData->SetLine(pInfo->ucLine);
			pData->SetInt(TUTORDATA_EXITTIME, 0);
		}
		pData->SetTeam(pInfo->ucInTeam);
	}
	DEBUG_CATCH("CUserTutor::AddNewToTutorApply")
	return true;
}
CTutorData* CUserTutor::AddNewToLeaveTutorApply(OBJID idStu, OBJID idTUtor)
{
	CTutorData* pData = NULL;
	DEBUG_TRY
	char szSQL[256]="";
	sprintf(szSQL,"SELECT *,0,0,0,0,0,0,0,0,0,0,0,0,0 FROM %s WHERE stu_id=%u AND tutor_id=%u LIMIT 1", _TBL_TUTOR, idStu,idTUtor);
	IRecordset* pRes = Database()->CreateNewRecordset( szSQL, true );
	if( pRes)
	{
		stTempUser* pTempStu = UserManager()->QueryTempUser(idStu);
		if(pTempStu)
		{
			pData = CTutorData::CreateNew();
			if(!pData)
			{
				pRes->Release();
				return NULL;
			}
			
			if(!pData->Create(pRes, pTempStu))
			{
				pData->ReleaseByOwner();
				pRes->Release();
				return NULL;
			}
			
			m_setStu->AddObj(pData);
		}
		pRes->Release();
	}
	DEBUG_CATCH("CUserTutor::AddNewToLeaveTutorApply()")
	return pData;
}
bool CUserTutor::DelStuApply(OBJID idStu)//删除师父收到的拜师请求
{
	CTutorData* pData = FindStudentByID(idStu);
	if(!pData)
		return false;

	if (pData->GetState()!=TUTORSTATE_APPLY && pData->GetState()!=TUTORSTATE_APPLYREF)
		return false;

	CHECKF(pData->DeleteRecord());

	return m_setStu->DelObj(pData->GetID());
}
bool CUserTutor::DelTutorApply(OBJID idTutor)//删除徒弟发出的拜师请求，不删数据库，师父删除时已经删过了
{
	CTutorData* pData = FindTutorByID(idTutor);
	if(!pData)
		return false;

	if (pData->GetState()!=TUTORSTATE_APPLY && pData->GetState()!=TUTORSTATE_APPLYREF)
		return false;
	
	return m_setTutor->DelObj(pData->GetID());
}
//////////////////////////////////////////////////////////////////////////
int CUserTutor::GetStudentAmount(bool bIncludeGraduated)
{
	CHECKF(m_setStu);

	if (bIncludeGraduated)//包括出师的徒弟
		return m_setStu->GetAmount();

	int nAmount = 0;
	for(int i = 0; i < m_setStu->GetAmount(); i++)
	{
		CTutorData* pData = m_setStu->GetObjByIndex(i);
		if(pData)
		{
			if(pData->GetState()==TUTORSTATE_NORMAL || pData->GetState()==TUTORSTATE_AGREED)
				nAmount++;
			else if (pData->IsGraduated() && bIncludeGraduated)
				nAmount++;
		}
	}
	return nAmount;
}
//////////////////////////////////////////////////////////////////////////
CTutorData* CUserTutor::FindStudentByID(OBJID idStudent)
{
	CHECKF(idStudent);
	CHECKF(m_setStu);

	for(int i = 0; i < m_setStu->GetAmount(); i++)
	{
		CTutorData* pData = m_setStu->GetObjByIndex(i);
		if(pData && pData->GetStuID()==idStudent)
			return m_setStu->GetObjByIndex(i);
	}
	return NULL;
}
CTutorData* CUserTutor::FindTutorByID(OBJID idTutor)
{
	CHECKF(idTutor);
	CHECKF(m_setTutor);
	
	for(int i = 0; i < m_setTutor->GetAmount(); i++)
	{
		CTutorData* pData = m_setTutor->GetObjByIndex(i);
		if(pData && pData->GetTutorID()==idTutor)
			return m_setTutor->GetObjByIndex(i);
	}
	return NULL;
}
//师徒协议解除关系
bool CUserTutor::TutorDivorce()
{
	CTutorData* pTutor = GetTutor();
	if (!pTutor)
		return false;

	ASSERT(pTutor->DeleteRecord());
	m_setTutor->DelObj(pTutor->GetID());

	return true;
}
//////////////////////////////////////////////////////////////////////////
bool CUserTutor::DelStudentByTutor(OBJID idStudent, bool bDelDBRec/*=true*/)
{
	CTutorData* pStu = FindStudentByID(idStudent);
	if (!pStu)
		return false;

	if (bDelDBRec)
		pStu->DeleteRecord();

	m_setStu->DelObj(pStu->GetID());

	return  true;
}
bool CUserTutor::DelTutorByStudent(bool bDelDBRec)
{
	CTutorData* pTutor = GetTutor();
	if (!pTutor)
		return false;

	if (bDelDBRec)
		pTutor->DeleteRecord();

	m_setTutor->DelObj(pTutor->GetID());

	return true;
}
//////////////////////////////////////////////////////////////////////////
DWORD CUserTutor::GetTutorGiftMask(OBJID idStudent)
{
	CTutorData* pStudent = FindStudentByID(idStudent);
	if (!pStudent)
		return 0;

	return pStudent->GetTutorGiftMask();
}
//////////////////////////////////////////////////////////////////////////
void CUserTutor::SetTutorGiftMask(OBJID idStudent, DWORD dwMask)
{
	CTutorData* pStudent = FindStudentByID(idStudent);
	if (!pStudent)
		return;
	
	pStudent->SetTutorGiftMask(dwMask);
}
//////////////////////////////////////////////////////////////////////////
//师父带徒弟出师
bool CUserTutor::Graduate(OBJID idStudent)
{
	DEBUG_TRY
	CUser* pUserStu = UserManager()->GetUser(idStudent);
	if (!pUserStu)
		return false;
	if (pUserStu->HasGraduated())
	{
		pUserStu->SendSysMsg(_TXTATR_TOPSHOW,"您已经出过一次师了！");
		m_pUser->SendSysMsg(_TXTATR_TOPSHOW,"对方已经出过一次师了！");
		return false;
	}

	CTutorData* pStu = FindStudentByID(idStudent);
	CTutorData* pTutor = pUserStu->GetTutor();
	if (!pStu || !pTutor)
		return false;

	if ( (pStu->GetState()!=TUTORSTATE_NORMAL || pTutor->GetState()!=TUTORSTATE_NORMAL)
		&& (pStu->GetState()!=TUTORSTATE_AGREED || pTutor->GetState()!=TUTORSTATE_AGREED) )
		return false;

	pStu->SetState(TUTORSTATE_GRADUATED);
	pTutor->SetState(TUTORSTATE_GRADUATED);

	char UserName[128] = "";
	char TarName[128]  = "";
	sprintf(UserName, "%d_[%s]_%d",  SNAPSHOTUSER, m_pUser->GetName(), COLORYELLOW);
	sprintf(TarName, "%d_[%s]_%d",  SNAPSHOTUSER, pUserStu->GetName(), COLORYELLOW);

	char szMsg[512] = "";
	sprintf(szMsg, "恭喜<%s>在<%s>的辛苦教导下成功出师，双方领取了丰厚的师徒奖励。", TarName, UserName);
	UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, m_pUser, 0, 0, NULL , ALLUSERS_NAME);

	pUserStu->SetGraduated();
	DEBUG_CATCH("CUserTutor::Graduate(OBJID idStudent)")
	return true;
}
//徒弟叛师
bool CUserTutor::StuRebel()
{
	CHECKF(m_pUser);
	CHECKF(m_setTutor);

	DEBUG_TRY
	CTutorData* pTutor = GetTutor();
	if (!pTutor)
		return false;

	CUser* pTarget = UserManager()->GetUser(pTutor->GetTutorID());
	if (pTarget)
	{
		DelTutorByStudent(true);
		pTarget->DelStudentByTutor(m_pUser->GetID(), false);
		char szMsg[256] = "";
		sprintf(szMsg, "你已叛离了师傅%s。", pTarget->GetName());
		m_pUser->SendSysMsg(_TXTATR_TOPSHOW, szMsg);
		
		sprintf(szMsg, "由于你对徒弟教导不善，%s已经叛离了你的师门。", m_pUser->GetName());
		pTarget->SendEmail(pTarget->GetName(), szMsg, pTarget);
	}
	else
	{
		pTutor->SetState(TUTORSTATE_STUREBEL);
		char szMsg[256] = "";
		sprintf(szMsg, "你已叛离了师傅%s。", pTutor->GetTutorName());
		m_pUser->SendSysMsg(_TXTATR_TOPSHOW, szMsg);
		CMsgSchoolMember msg;
		if (msg.Create(m_pUser->GetID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_STUREBEL))
		{
			TUTORLIST info = {0};
			info.idUser = pTutor->GetTutorID();
			SafeCopy(info.szName, m_pUser->GetName(), _MAX_NAMESIZE);
			msg.Append(info);
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
		}
		m_setTutor->DelObj(pTutor->GetID());
	}
	DEBUG_CATCH("CUserTutor::StuRebel()")
	return true;
}
//踢出徒弟
bool CUserTutor::KickOutStu(OBJID idStudent)
{
	CHECKF(m_pUser);
	CHECKF(m_setStu);

	DEBUG_TRY
	CTutorData* pStudent = FindStudentByID(idStudent);
	if (!pStudent)
		return false;
	if (pStudent->GetState()!=TUTORSTATE_NORMAL && pStudent->GetState()!=TUTORSTATE_AGREED)
		return false;

	CUser* pTarget = UserManager()->GetUser(idStudent);
	if (pTarget)
	{
		DelStudentByTutor(idStudent, true);
		pTarget->DelTutorByStudent(false);
		char szMsg[256] = "";
		sprintf(szMsg, "你已踢出了徒弟%s。", pTarget->GetName());
		m_pUser->SendSysMsg(_TXTATR_TOPSHOW, szMsg);
		
		sprintf(szMsg, "你的师傅%s对你的表现很不满意，已经将你踢出了师门。", m_pUser->GetName());
		pTarget->SendEmail(pTarget->GetName(), szMsg, pTarget);
	}
	else
	{
		pStudent->SetState(TUTORSTATE_TUTORKICKOUT);
		char szMsg[256] = "";
		sprintf(szMsg, "你已踢出了徒弟%s。", pStudent->GetStuName());
		m_pUser->SendSysMsg(_TXTATR_TOPSHOW, szMsg);
		CMsgSchoolMember msg;
		if (msg.Create(m_pUser->GetID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_TUTORKICKOUT))
		{
			TUTORLIST info = {0};
			info.idUser = pStudent->GetStuID();
			SafeCopy(info.szName, m_pUser->GetName(), _MAX_NAMESIZE);
			msg.Append(info);
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
		}
		DelStudentByTutor(idStudent,false);
	}

	CMsgSchoolMember msg1;
	if (msg1.Create(m_pUser->GetID(), MESSAGEBOARD_DELSTU))
	{
		ST_MEMBERINFO info = {0};
		info.idMember = idStudent;
		msg1.Append(info);
		m_pUser->SendMsg(&msg1);
	}
	DEBUG_CATCH("CUserTutor::KickOutStu(OBJID idStudent)")
	return true;
}
void CUserTutor::SetState(OBJID idTutor, TUTORSTATE nState)
{
	CTutorData* pData = FindTutorByID(idTutor);
	if(pData)
		pData->SetState(nState, true);//即时更新数据库，防止读数据库判断时出bug
}
//发送徒弟申请列表
bool CUserTutor::SendStuApplyList(USHORT usPage, int nPageSize)
{
	CHECKF(nPageSize>0 && nPageSize<=20);//每页最多只允许20个
	DEBUG_TRY
	CMsgSchoolMember msg;
	int nTotalAmount = GetTutorApplyAmount();
	UCHAR nPages = nTotalAmount%nPageSize==0 ? nTotalAmount/nPageSize : (nTotalAmount/nPageSize) + 1;
	if (!msg.Create(m_pUser->GetID(), MESSAGEBOARD_QUERYSTUAPPLYLIST, NULL, 0, nPages))
		return false;

	TUTORLIST info = {0};
//	int nIndex = 0;
	int nAmount = 0;
	for(int i = 0; i < m_setStu->GetAmount(); i++)
	{
// 		nIndex = (usPage-1)*nPageSize + i;
// 		if (nIndex >= m_setStu->GetAmount())
// 			break;
		CTutorData* pData = m_setStu->GetObjByIndex(i);
		if(pData)
		{
			if (pData->GetState()==TUTORSTATE_APPLY || pData->GetState()==TUTORSTATE_APPLYREF)
			{
				nAmount++;
				if(nAmount <= nPageSize*(usPage-1))//不是本页的
					continue;
				info.idUser = pData->GetStuID();
				info.ucGender = pData->GetInt(TUTORDATA_SEX);
				info.usLev = pData->GetInt(TUTORDATA_LEV);
				info.dwProfession = pData->GetInt(TUTORDATA_MAINJOB);
				info.dwBattleNum = pData->GetInt(TUTORDATA_MAXSCORE);
				info.ucVipLev = pData->GetInt(TUTORDATA_VIPLEV);
				if (pData->GetLine()==0)
					info.dwLastLogoutTime = pData->GetInt(TUTORDATA_EXITTIME);
				else
					info.dwLastLogoutTime = 0;
				SafeCopy(info.szName, pData->GetStuName(), _MAX_NAMESIZE);
				msg.Append(info);
			}
		}
		if (msg.m_pInfo->ucAmount>=nPageSize)
			break;
	}
	m_pUser->SendMsg(&msg);
	DEBUG_CATCH("CUserTutor::SendStuApplyList()")
	return true;
}
//发送徒弟列表
bool CUserTutor::SendStuList()
{
	DEBUG_TRY
	CMsgSchoolMember msg;
	if (!msg.Create(m_pUser->GetID(), MESSAGEBOARD_QUERYSTU))
		return false;
	
	TUTORLIST info = {0};
	for(int i = 0; i < m_setStu->GetAmount(); i++)
	{
		CTutorData* pData = m_setStu->GetObjByIndex(i);
		if(pData)
		{
			if (pData->GetState()==TUTORSTATE_NORMAL || pData->GetState()==TUTORSTATE_AGREED)
			{
				info.idUser = pData->GetStuID();
				info.ucGender = pData->GetInt(TUTORDATA_SEX);
				info.usLev = pData->GetInt(TUTORDATA_LEV);
				info.dwProfession = pData->GetInt(TUTORDATA_MAINJOB);
				info.dwBattleNum = pData->GetInt(TUTORDATA_MAXSCORE);
				info.ucVipLev = pData->GetInt(TUTORDATA_VIPLEV);
				info.dwImpart = pData->GetTutorGiftMask();
				if (pData->GetLine()==0)
					info.dwLastLogoutTime = pData->GetInt(TUTORDATA_EXITTIME);
				else
					info.dwLastLogoutTime = 0;
				SafeCopy(info.szName, pData->GetStuName(), _MAX_NAMESIZE);
				msg.Append(info);
			}
		}
	}
	m_pUser->SendMsg(&msg);
	DEBUG_CATCH("CUserTutor::SendStuList()")
	return true;
}
//发送出师的徒弟列表
int CUserTutor::SendGraduateStuList(int nPage, int nPageSize)
{
	CHECKF(m_setStu);
	CHECKF(nPageSize>0 && nPageSize<=20);//每页最多只允许20个
	int nAmount = 0;
	DEBUG_TRY
	CMsgSchoolMember msg;
	int nTotalAmount = GetGraduatedAmount();
	UCHAR nPages = nTotalAmount%nPageSize==0 ? nTotalAmount/nPageSize : (nTotalAmount/nPageSize) + 1;
	if (!msg.Create(m_pUser->GetID(), MESSAGEBOARD_GRADUATESTU, NULL, 0, nPages))
		return false;
	
	TUTORLIST info = {0};
//	int nIndex = 0;
	int nCount = 0;
	for(int i = 0; i < m_setStu->GetAmount(); i++)
	{
// 		nIndex = (nPage-1)*nPageSize + i;
// 		if (nIndex >= m_setStu->GetAmount())
// 			break;

		CTutorData* pData = m_setStu->GetObjByIndex(i);
		if(pData)
		{
			if (pData->GetState()==TUTORSTATE_GRADUATED)
			{
				nCount++;
				if(nCount <= nPageSize*(nPage-1))//不是本页的
					continue;
				info.idUser = pData->GetStuID();
				info.ucGender = pData->GetInt(TUTORDATA_SEX);
				info.usLev = pData->GetInt(TUTORDATA_LEV);
				info.dwProfession = pData->GetInt(TUTORDATA_MAINJOB);
				info.dwBattleNum = pData->GetInt(TUTORDATA_MAXSCORE);
				info.ucVipLev = pData->GetInt(TUTORDATA_VIPLEV);
				info.dwImpart = pData->GetTutorGiftMask();
				if (pData->GetLine()==0)
					info.dwLastLogoutTime = pData->GetInt(TUTORDATA_EXITTIME);
				else
					info.dwLastLogoutTime = 0;
				SafeCopy(info.szName, pData->GetStuName(), _MAX_NAMESIZE);
				msg.Append(info);
				nAmount++;
			}
			if (nAmount>=nPageSize)
				break;
		}
	}
	m_pUser->SendMsg(&msg);
	DEBUG_CATCH("CUserTutor::SendStuList()")
	return nAmount;
}
//发送师徒列表
bool CUserTutor::SendAllList()
{
	DEBUG_TRY
	CMsgSchoolMember msg;
	if (!msg.Create(m_pUser->GetID(), MESSAGEBOARD_ALLLIST))
		return false;
	
	ST_MEMBERINFO info = {0};
	//自己的信息
	info.idMember = m_pUser->GetID();
	info.nImpart = m_pUser->GetImpart();
	info.dwExitTime = m_pUser->GetMorality();
	info.usMainJob = GetGraduatedAmount();//出徒数量
	if (m_pUser->IsInTutorList())
		info.usLev = 1;
	info.ucRelation	= RELATION_SELF;
	msg.Append(info);
	memset(&info, 0L, sizeof(ST_MEMBERINFO));
	//师父的信息
	CTutorData* pTutor = GetTutor();
	if (pTutor)
	{
		info.idMember = pTutor->GetTutorID();
		info.usLookFace = pTutor->GetLookFace();
		info.usLev = pTutor->GetInt(TUTORDATA_LEV);
		info.usMainJob = pTutor->GetInt(TUTORDATA_MAINJOB);
		info.usMainLev = pTutor->GetInt(TUTORDATA_MAINLEV);
		info.usSubJob = pTutor->GetInt(TUTORDATA_SUBJOB);
		info.usSubLev = pTutor->GetInt(TUTORDATA_SUBLEV);
		info.nImpart = pTutor->GetInt(TUTORDATA_IMPART);
		SafeCopy(info.szName, pTutor->GetTutorName(), _MAX_NAMESIZE);
		info.ucRelation	= RELATION_TEACHER;
		info.ucLine = pTutor->GetLine();
		info.ucInTeam = pTutor->HasTeam();
		msg.Append(info);
		memset(&info, 0L, sizeof(ST_MEMBERINFO));
	}

	for(int i = 0; i < m_setStu->GetAmount(); i++)
	{
		CTutorData* pData = m_setStu->GetObjByIndex(i);
		if(pData)
		{
			if (pData->GetState()==TUTORSTATE_NORMAL || pData->GetState()==TUTORSTATE_AGREED)
			{
				info.idMember = pData->GetStuID();
				info.usLookFace = pData->GetLookFace();
				info.usLev = pData->GetInt(TUTORDATA_LEV);
				info.usMainJob = pData->GetInt(TUTORDATA_MAINJOB);
				info.usMainLev = pData->GetInt(TUTORDATA_MAINLEV);
				info.usSubJob = pData->GetInt(TUTORDATA_SUBJOB);
				info.usSubLev = pData->GetInt(TUTORDATA_SUBLEV);
				info.nImpart = pData->GetTutorGiftMask();
				info.ucRelation	= RELATION_STUDENT;
				SafeCopy(info.szName, pData->GetStuName(), _MAX_NAMESIZE);
				info.ucLine = pData->GetLine();
				info.ucInTeam = pData->HasTeam();
				msg.Append(info);
			}
		}
	}
	m_pUser->SendMsg(&msg);
	DEBUG_CATCH("CUserTutor::SendAllList()")
	return true;
}
//出徒数量
int CUserTutor::GetGraduatedAmount()
{
	CHECKF(m_setStu);

	int nAmount = 0;
	for(int i = 0; i < m_setStu->GetAmount(); i++)
	{
		CTutorData* pData = m_setStu->GetObjByIndex(i);
		if(pData)
		{
			if (pData->GetState()==TUTORSTATE_GRADUATED)
				nAmount++;
		}
	}
	return nAmount;
}

int CUserTutor::GetTutorLevByImpart(UINT nImpart)
{
	int nLev = 10;

	CLevupexpData* pLevupexp = NULL;
	__int64 i64LevupExp = 0;
	for(; nLev>0; nLev--)
	{
		pLevupexp = LevupexpSet()->GetObj(EXP_TYPE_TUTOR * _EXP_TYPE + nLev);
		if (!pLevupexp)
			return 1;
		i64LevupExp	= pLevupexp->GetInt64(LEVUPEXPDATA_EXP);

		if (nImpart>=i64LevupExp)
		{
			if (nLev<10)
				nLev++;
			break;
		}
	}
	if (nLev>0 && nLev<11)
		return nLev;
	else
		return 1;
}
int CUserTutor::GetMaxStuAmount(UINT nImpart)
{
	return GetTutorLevByImpart(nImpart) + 1;
}
//通知师徒自己上线，并获取所有师徒状态
void CUserTutor::SendTutorOnLineInfo()
{
	CHECK(m_setStu);
	CHECK(m_setTutor);
	CHECK(m_pUser);
	
	DEBUG_TRY
	int i=0;
	CUser* pTarget = NULL;
	for(i = 0; i < m_setStu->GetAmount(); i++)
	{
		CTutorData* pData = m_setStu->GetObjByIndex(i);
		if(pData)
		{
			if (pData->GetState()==TUTORSTATE_NORMAL)
			{
				pTarget = UserManager()->GetUser(pData->GetStuID());
				if (pTarget)
				{
					pData->SetLine(MapGroup(PID)->GetServerName());
					if (pTarget->GetTeam())
						pData->SetTeam(pTarget->GetTeam()->GetID());
					else
						pData->SetTeam(0);

					CTutorData* pTemp = pTarget->GetTutor();
					if (pTemp)
					{
						pTemp->SetLine(MapGroup(PID)->GetServerName());
						if (m_pUser->GetTeam())
							pTemp->SetTeam(m_pUser->GetTeam()->GetID());
						else
							pTemp->SetTeam(0);

						pTemp->SetInt(TUTORDATA_LEV, m_pUser->GetLev());
						pTemp->SetInt(TUTORDATA_MAINLEV, m_pUser->GetJobLev(1));
						pTemp->SetInt(TUTORDATA_SUBLEV, m_pUser->GetJobLev(2));
						pTemp->SetInt(TUTORDATA_IMPART, m_pUser->GetImpart());
						pTemp->SetInt(TUTORDATA_EXITTIME, 0);
					}
				}
				else
				{
					CMsgSchoolMember msg;
					if (msg.Create(pData->GetStuID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_ONLINESTATE))
					{
						msg.Append(RELATION_TEACHER, m_pUser);
						msg.m_pInfo->setMember[0].dwExitTime = 0;
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
					}
				}
			}
		}
	}

	for(i = 0; i < m_setTutor->GetAmount(); i++)
	{
		CTutorData* pData = m_setTutor->GetObjByIndex(i);
		if(pData)
		{
			if (pData->GetState()==TUTORSTATE_NORMAL || pData->GetState()==TUTORSTATE_APPLY || pData->GetState()==TUTORSTATE_APPLYREF || pData->GetState()==TUTORSTATE_GRADUATED)
			{
				pTarget = UserManager()->GetUser(pData->GetTutorID());
				if (pTarget)
				{
					pData->SetLine(MapGroup(PID)->GetServerName());
					if (pTarget->GetTeam())
						pData->SetTeam(pTarget->GetTeam()->GetID());
					else
						pData->SetTeam(0);

					CTutorData* pTemp = pTarget->GetStudent(m_pUser->GetID());
					if (pTemp)
					{
						pTemp->SetLine(MapGroup(PID)->GetServerName());
						if (m_pUser->GetTeam())
							pTemp->SetTeam(m_pUser->GetTeam()->GetID());
						else
							pTemp->SetTeam(0);

						pTemp->SetInt(TUTORDATA_LEV, m_pUser->GetLev());
						pTemp->SetInt(TUTORDATA_MAINLEV, m_pUser->GetJobLev(1));
						pTemp->SetInt(TUTORDATA_SUBLEV, m_pUser->GetJobLev(2));
						pTemp->SetInt(TUTORDATA_IMPART, m_pUser->GetImpart());
						pTemp->SetInt(TUTORDATA_EXITTIME, 0);
					}
				}
				else
				{
					CMsgSchoolMember msg;
					if (msg.Create(pData->GetTutorID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_ONLINESTATE))
					{
						msg.Append(RELATION_STUDENT, m_pUser);
						msg.m_pInfo->setMember[0].dwExitTime = 0;
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
					}
				}
			}
		}
	}
	DEBUG_CATCH("CUserTutor::SendTutorOnLineInfo()")
}
//通知师徒自己下线
void CUserTutor::SendTutorOffLineInfo()
{
	CHECK(m_setStu);
	CHECK(m_setTutor);
	CHECK(m_pUser);
	
	DEBUG_TRY
	int i=0;
	CUser* pTarget = NULL;
	for(i = 0; i < m_setStu->GetAmount(); i++)
	{
		CTutorData* pData = m_setStu->GetObjByIndex(i);
		if(pData)
		{
			if (pData->GetState()==TUTORSTATE_NORMAL)
			{
				pTarget = UserManager()->GetUser(pData->GetStuID());
				if (pTarget)
				{
					CTutorData* pTemp = pTarget->GetTutor();
					if (pTemp)
					{
						pTemp->SetLine((UCHAR)0);
						pTemp->SetTeam(0);
						pTemp->SetInt(TUTORDATA_EXITTIME, m_pUser->GetExitTime());
						pTemp->SetInt(TUTORDATA_LEV, m_pUser->GetLev());
						pTemp->SetInt(TUTORDATA_MAINLEV, m_pUser->GetJobLev(1));
						pTemp->SetInt(TUTORDATA_SUBLEV, m_pUser->GetJobLev(2));
						pTemp->SetInt(TUTORDATA_IMPART, m_pUser->GetImpart());
					}
				}
				else
				{
					CMsgSchoolMember msg;
					if (msg.Create(pData->GetStuID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_ONLINESTATEREF))
					{
						msg.Append(RELATION_TEACHER, m_pUser);
						msg.m_pInfo->setMember[0].ucLine = 0;
						msg.m_pInfo->setMember[0].ucInTeam = 0;
						msg.m_pInfo->setMember[0].dwExitTime = m_pUser->GetExitTime();
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
					}
				}
			}
		}
	}
	
	for(i = 0; i < m_setTutor->GetAmount(); i++)
	{
		CTutorData* pData = m_setTutor->GetObjByIndex(i);
		if(pData)
		{
			if (pData->GetState()==TUTORSTATE_NORMAL || pData->GetState()==TUTORSTATE_APPLY || pData->GetState()==TUTORSTATE_APPLYREF || pData->GetState()==TUTORSTATE_GRADUATED)
			{
				pTarget = UserManager()->GetUser(pData->GetTutorID());
				if (pTarget)
				{
					CTutorData* pTemp = pTarget->GetStudent(m_pUser->GetID());
					if (pTemp)
					{
						pTemp->SetLine((UCHAR)0);
						pTemp->SetTeam(0);
						pTemp->SetInt(TUTORDATA_EXITTIME, m_pUser->GetExitTime());
					}
				}
				else
				{
					CMsgSchoolMember msg;
					if (msg.Create(pData->GetTutorID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_ONLINESTATEREF))
					{
						msg.Append(RELATION_STUDENT, m_pUser);
						msg.m_pInfo->setMember[0].ucLine = 0;
						msg.m_pInfo->setMember[0].ucInTeam = 0;
						msg.m_pInfo->setMember[0].dwExitTime = m_pUser->GetExitTime();
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
					}
				}
			}
		}
	}
	DEBUG_CATCH("CUserTutor::SendTutorOffLineInfo()")
}
//通知师父和徒弟自己状态改变了
void CUserTutor::BroadCastTutorState()
{
	CHECK(m_setStu);
	CHECK(m_setTutor);
	CHECK(m_pUser);

	DEBUG_TRY
	int i=0;
	CUser* pTarget = NULL;
	for(i = 0; i < m_setStu->GetAmount(); i++)
	{
		CTutorData* pData = m_setStu->GetObjByIndex(i);
		if(pData)
		{
			if (pData->GetState()==TUTORSTATE_NORMAL)
			{
				pTarget = UserManager()->GetUser(pData->GetStuID());
				if (pTarget)
				{
					CTutorData* pTemp = pTarget->GetTutor();
					if (pTemp)
					{
						pTemp->SetLine(MapGroup(PID)->GetServerName());
						if (m_pUser->GetTeam())
							pTemp->SetTeam(m_pUser->GetTeam()->GetID());
						else
							pTemp->SetTeam(0);
					}
				}
				else
				{
					CMsgSchoolMember msg;
					if (msg.Create(pData->GetStuID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_ONLINESTATEREF))
					{
						msg.Append(RELATION_TEACHER, m_pUser);
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
					}
				}
			}
		}
	}
	
	for(i = 0; i < m_setTutor->GetAmount(); i++)
	{
		CTutorData* pData = m_setTutor->GetObjByIndex(i);
		if(pData)
		{
			if (pData->GetState()==TUTORSTATE_NORMAL || pData->GetState()==TUTORSTATE_APPLYREF || pData->GetState()==TUTORSTATE_GRADUATED)
			{
				pTarget = UserManager()->GetUser(pData->GetTutorID());
				if (pTarget)
				{
					CTutorData* pTemp = pTarget->GetStudent(m_pUser->GetID());
					if (pTemp)
					{
						pTemp->SetLine(MapGroup(PID)->GetServerName());
						if (m_pUser->GetTeam())
							pTemp->SetTeam(m_pUser->GetTeam()->GetID());
						else
							pTemp->SetTeam(0);
					}
				}
				else
				{
					CMsgSchoolMember msg;
					if (msg.Create(pData->GetTutorID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_ONLINESTATEREF))
					{
						msg.Append(RELATION_STUDENT, m_pUser);
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
					}
				}
			}
		}
	}
	DEBUG_CATCH("CUserTutor::BroadCastTutorState()")
}
//师徒奖励掩码处理
void CUserTutor::ProcessGiftMask(int nLev)
{
	DEBUG_TRY
	if (nLev%5!=0)
		return;

	int nIndex = (nLev-15)/5;//掩码的第一位是15级的奖励,第二位是20级...以此类推

	if (nIndex<0 || nIndex>=32)
		return;

	for(int i = 0; i < m_setTutor->GetAmount(); i++)
	{
		CTutorData* pData = m_setTutor->GetObjByIndex(i);
		if(!pData)
			continue;

		if(nLev%10==0 && nLev!=50)//除50级出师以外，每升10级是必定有奖励领的
			return;

		if (pData->GetState()==TUTORSTATE_NORMAL || pData->GetState()==TUTORSTATE_AGREED || pData->GetState()==TUTORSTATE_GRADUATED)
		{
			if(nLev < 50)//50以前是每5级可以领一次
				return;
			else
			{
				DWORD dwMask = pData->GetTutorGiftMask() | (1<<nIndex);
				pData->SetTutorGiftMask(dwMask);
				CUser* pUser = UserManager()->GetUser(pData->GetTutorID());
				if (pUser)
				{
					CTutorData* pStu = pUser->GetStudent(m_pUser->GetID());
					if (pStu)
						pStu->SetTutorGiftMask(dwMask);
				}
				else
				{
					pData->SetTutorGiftMask(dwMask);
					CMsgSchoolMember msg;
					if (msg.Create(m_pUser->GetID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_SYNGIFTMASK))
					{
						ST_MEMBERINFO info = {0};
						info.idMember = pData->GetTutorID();
						info.nImpart = dwMask;
						info.ucRelation = RELATION_STUDENT;
						msg.Append(info);
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
					}
				}
			}
		}
		else//还在申请中的，一律不能领
		{
			DWORD dwMask = pData->GetTutorGiftMask() | (1<<nIndex);
			pData->SetTutorGiftMask(dwMask);
			CUser* pUser = UserManager()->GetUser(pData->GetTutorID());
			if (pUser)
			{
				CTutorData* pStu = pUser->GetStudent(m_pUser->GetID());
				if (pStu)
					pStu->SetTutorGiftMask(dwMask);
			}
			else
			{
				CMsgSchoolMember msg;
				if (msg.Create(m_pUser->GetID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_SYNGIFTMASK))
				{
					ST_MEMBERINFO info = {0};
					info.idMember = pData->GetTutorID();
					info.nImpart = dwMask;
					info.ucRelation = RELATION_STUDENT;
					msg.Append(info);
					MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
				}
			}
		}
	}
	DEBUG_CATCH("CUserTutor::ProcessGiftMask(int nLev)")
}
//判断某个等级的奖励是否为可领取
bool CUserTutor::CanGetGift(OBJID idStu, int nLev)
{
	CTutorData* pData = FindStudentByID(idStu);
	if (!pData)
		return false;

	if (nLev<15 || pData->GetInt(TUTORDATA_LEV)<nLev)
		return false;
	int nIndex = (nLev-15)/5;
	if (nIndex<0 || nIndex>31)
		return false;

	DWORD dwMask = pData->GetTutorGiftMask();
	return ( dwMask & (1<<nIndex) ) == 0;
}
//设置某个等级的奖励为不可领取状态
bool CUserTutor::AddGiftMask(OBJID idStu, int nLev)
{
	CTutorData* pData = FindStudentByID(idStu);
	if (!pData)
		return false;
	
	if (nLev<15)
		return false;
	int nIndex = (nLev-15)/5;
	if (nIndex<0 || nIndex>31)
		return false;

	DWORD dwMask = pData->GetTutorGiftMask();
	dwMask |= (1<<nIndex);
	pData->SetTutorGiftMask(dwMask);

	CUser* pStu = UserManager()->GetUser(idStu);
	if (pStu)
	{
		CTutorData* pData = pStu->GetTutor();
		if (pData)
			pData->SetTutorGiftMask(dwMask);
	}
	else
	{
		CMsgSchoolMember msg;
		if (msg.Create(m_pUser->GetID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_SYNGIFTMASK))
		{
			ST_MEMBERINFO info = {0};
			info.idMember = idStu;
			info.nImpart = dwMask;
			info.ucRelation = RELATION_TEACHER;
			msg.Append(info);
			MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
		}
	}
	return true;
}

void CUserTutor::SetCalledPos(int nMapID, int nPosX, int nPosY, int nLine)
{
	CHECK(nMapID);
	CHECK(nLine);

	m_stCalledPos.nMapID = nMapID;
	m_stCalledPos.nPosX = nPosX;
	m_stCalledPos.nPosY = nPosY;
	m_stCalledPos.nLine = nLine;
}

void CUserTutor::AddTutorAddedLev(int nLev)
{
	if(nLev==0)
		return;

	for(int i = 0; i < m_setTutor->GetAmount(); i++)
	{
		CTutorData* pData = m_setTutor->GetObjByIndex(i);
		if(pData)
		{
			if(pData->GetState()==TUTORSTATE_NORMAL || pData->GetState()==TUTORSTATE_AGREED)
			{
				int nNewAddLev = pData->GetInt(TUTORDATA_ADDEDLEV) + 1;
				pData->SetInt(TUTORDATA_ADDEDLEV, nNewAddLev);
				
				CUser* pUser = UserManager()->GetUser(pData->GetTutorID());
				if(pUser)
				{
					CTutorData* pDataTemp = pUser->GetStudent(m_pUser->GetID());
					if(pDataTemp)
					{
						pDataTemp->SetInt(TUTORDATA_ADDEDLEV, nNewAddLev);
						pDataTemp->SetInt(TUTORDATA_LEV, m_pUser->GetLev());
					}
				}
				else
				{
					CMsgSchoolMember msg;
					if (msg.Create(m_pUser->GetID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_ADDEDLEV))
					{
						ST_MEMBERINFO info = {0};
						info.idMember = pData->GetTutorID();
						info.nImpart = nNewAddLev;
						info.usLev = m_pUser->GetLev();
						info.ucRelation = RELATION_STUDENT;
						msg.Append(info);
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
					}
				}
			}
			else if(pData->GetState()==TUTORSTATE_GRADUATED)//已经出师的不用记成长等级，但需要计算成长奖励
			{
				int nNewAddLev = pData->GetInt(TUTORDATA_ADDEDLEV);
				pData->SetInt(TUTORDATA_ADDEDLEV, nNewAddLev);
				
				CUser* pUser = UserManager()->GetUser(pData->GetTutorID());
				if(pUser)
				{
					CTutorData* pDataTemp = pUser->GetStudent(m_pUser->GetID());
					if(pDataTemp)
					{
						pDataTemp->SetInt(TUTORDATA_ADDEDLEV, nNewAddLev);
						pDataTemp->SetInt(TUTORDATA_LEV, m_pUser->GetLev());
					}
				}
				else
				{
					CMsgSchoolMember msg;
					if (msg.Create(m_pUser->GetID(), MESSAGEBOARD_LINEBROAD, NULL, 0, LINEBROAD_ADDEDLEV))
					{
						ST_MEMBERINFO info = {0};
						info.idMember = pData->GetTutorID();
						info.nImpart = nNewAddLev;
						info.usLev = m_pUser->GetLev();
						info.ucRelation = RELATION_STUDENT;
						msg.Append(info);
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
					}
				}
			}
		}
	}
}

bool CUserTutor::GetStuInfoByIndex(TutorInfoStruct* pInfo, int idx)
{
	CHECKF(pInfo);
	CHECKF(idx >= 0 && idx < m_setStu->GetAmount());
	
	CTutorData* pData = m_setStu->GetObjByIndex(idx);
	IF_OK(pData)
	{
		pInfo->id			= pData->GetID();
		pInfo->idTutor		= pData->GetInt(TUTORDATA_TUTORID);
		SafeCopy(pInfo->szTutorName, pData->GetStr(TUTORDATA_TUTORNAME), _MAX_NAMESIZE);
		pInfo->idStudent	= pData->GetInt(TUTORDATA_STUID);
		SafeCopy(pInfo->szStudentName, pData->GetStr(TUTORDATA_STUNAME), _MAX_NAMESIZE);
		pInfo->dwGiftMask	= pData->GetInt(TUTORDATA_TUTORGIFTMASK);
		pInfo->nState		= pData->GetInt(TUTORDATA_STATE);
		pInfo->nAddLev		= pData->GetInt(TUTORDATA_ADDEDLEV);
		pInfo->nLine		= pData->GetLine();
		pInfo->idTeam		= pData->HasTeam();
		memcpy(&pInfo->detailInfo, pData->GetDetailInfo(), sizeof(stDetailInfo));

		return true;
	}
	return false;
}
bool CUserTutor::AppendStu(const TutorInfoStruct* pInfo)
{
	CHECKF(pInfo);
	
	CTutorData* pData = CTutorData::CreateNew();
	CHECKF(pData);
	IF_NOT(pData->Create(GameDataDefault()->GetTutorData(), pInfo->id))
		return false;
	
	pData->SetInt(TUTORDATA_TUTORID, pInfo->idTutor);
	pData->SetStr(TUTORDATA_TUTORNAME, pInfo->szTutorName, _MAX_NAMESIZE);
	pData->SetInt(TUTORDATA_STUID, pInfo->idStudent);
	pData->SetStr(TUTORDATA_STUNAME, pInfo->szStudentName, _MAX_NAMESIZE);
	pData->SetInt(TUTORDATA_TUTORGIFTMASK, pInfo->dwGiftMask);
	pData->SetInt(TUTORDATA_STATE, pInfo->nState);
	pData->SetInt(TUTORDATA_ADDEDLEV, pInfo->nAddLev);
	pData->SetTeam(pInfo->idTeam);
	pData->SetLine(pInfo->nLine);
	pData->SetDetailInfo(&pInfo->detailInfo);
	pData->ClearUpdateFlags();

	m_setStu->AddObj(pData);
	return true;
}

bool CUserTutor::GetTutorInfoByIndex(TutorInfoStruct* pInfo, int idx)
{
	CHECKF(pInfo);
	CHECKF(idx >= 0 && idx < m_setTutor->GetAmount());
	
	CTutorData* pData = m_setTutor->GetObjByIndex(idx);
	IF_OK(pData)
	{
		pInfo->id			= pData->GetID();
		pInfo->idTutor		= pData->GetInt(TUTORDATA_TUTORID);
		SafeCopy(pInfo->szTutorName, pData->GetStr(TUTORDATA_TUTORNAME), _MAX_NAMESIZE);
		pInfo->idStudent	= pData->GetInt(TUTORDATA_STUID);
		SafeCopy(pInfo->szStudentName, pData->GetStr(TUTORDATA_STUNAME), _MAX_NAMESIZE);
		pInfo->dwGiftMask	= pData->GetInt(TUTORDATA_TUTORGIFTMASK);
		pInfo->nState		= pData->GetInt(TUTORDATA_STATE);
		pInfo->nAddLev		= pData->GetInt(TUTORDATA_ADDEDLEV);
		pInfo->nLine		= pData->GetLine();
		pInfo->idTeam		= pData->HasTeam();
		memcpy(&pInfo->detailInfo, pData->GetDetailInfo(), sizeof(stDetailInfo));
		
		return true;
	}
	return false;
}
bool CUserTutor::AppendTutor(const TutorInfoStruct* pInfo)
{
	CHECKF(pInfo);
	
	CTutorData* pData = CTutorData::CreateNew();
	CHECKF(pData);
	IF_NOT(pData->Create(GameDataDefault()->GetTutorData(), pInfo->id))
		return false;
	
	pData->SetInt(TUTORDATA_TUTORID, pInfo->idTutor);
	pData->SetStr(TUTORDATA_TUTORNAME, pInfo->szTutorName, _MAX_NAMESIZE);
	pData->SetInt(TUTORDATA_STUID, pInfo->idStudent);
	pData->SetStr(TUTORDATA_STUNAME, pInfo->szStudentName, _MAX_NAMESIZE);
	pData->SetInt(TUTORDATA_TUTORGIFTMASK, pInfo->dwGiftMask);
	pData->SetInt(TUTORDATA_STATE, pInfo->nState);
	pData->SetInt(TUTORDATA_ADDEDLEV, pInfo->nAddLev);
	pData->SetTeam(pInfo->idTeam);
	pData->SetLine(pInfo->nLine);
	pData->SetDetailInfo(&pInfo->detailInfo);
	pData->ClearUpdateFlags();
	
	m_setTutor->AddObj(pData);
	return true;
}
void	CUserTutor::ResetTutorName(const char* szName)
{
	CHECK(m_setTutor);
	for(int i = 0; i < m_setTutor->GetAmount(); i++)
	{
		CTutorData* pData = m_setTutor->GetObjByIndex(i);
		if(pData)
		{
			pData->SetStr(TUTORDATA_STUNAME,szName,_MAX_NAMESIZE,true);
		}
	}

}
void	CUserTutor::ResetStudentName(const char* szName)
{
	CHECK(m_setStu);
	
	for(int i = 0; i < m_setStu->GetAmount(); i++)
	{
		CTutorData* pData = m_setStu->GetObjByIndex(i);
		if(pData)
		{
			pData->SetStr(TUTORDATA_TUTORNAME,szName,_MAX_NAMESIZE,true);
		}
	}
}
