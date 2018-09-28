// UserData.cpp: implementation of the CUserData class.
//
//////////////////////////////////////////////////////////////////////

#include "UserData.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include "UserManager.h"
#include "string.h"

//////////////////////////////////////////////////////////////////////
char	szUserTable[] = _TBL_USER;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUserData::CUserData()
{
	memset(&m_Info, 0, sizeof(UserInfoStruct));
	m_pRes	= NULL;
	m_Info.newKardState[0] = -1;
	m_Info.newKardState[1] = -1;
	m_Info.newKardState[2] = -1;
	m_Info.newKardState[3] = -1;
	m_Info.newKardState[4] = -1;
	m_bNeedUpdate		   = false;
}

CUserData::~CUserData()
{
	if(m_pRes)
		m_pRes->Release();
}

//////////////////////////////////////////////////////////////////////
bool CUserData::Create(OBJID idUser, IDatabase* pDb)
{
	CHECKF(!m_pRes);
	CHECKF(pDb);

	SQLBUF	szSQL;
	sprintf(szSQL, "SELECT * FROM %s WHERE id=%u LIMIT 1", _TBL_USER, idUser);
	IRecordset* pRes = pDb->CreateNewRecordset(szSQL);	
	if(pRes)
	{
		m_pRes = pRes->CreateNewRecord();
		pRes->Release();		

		return LoadInfo();
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CUserData::LoadInfo(void)
{
	if (!m_pRes) 
		return false;

	// load again?
	if (m_Info.id != ID_NONE)
		return true;
	m_Info.id						= m_pRes->LoadDWord( "id" );
	m_Info.idAccount				= m_pRes->LoadDWord( "account_id" );
	m_Info.dwLast_login				= m_pRes->LoadDWord( "last_login" );
	m_Info.dwCreateTime				= m_pRes->LoadDWord( "Creat_Time" );
	m_Info.dwExitTime				= m_pRes->LoadDWord( "Exit_Time" );
	m_Info.dwAllOnlineTime			= m_pRes->LoadDWord( "all_online_time" );
	m_Info.dwAllAutoTime			= m_pRes->LoadDWord( "all_auto_time" );
	m_Info.dwAllDoubleTime			= m_pRes->LoadDWord( "all_double_time" );
	m_Info.idMate                   = m_pRes->LoadDWord( "mate" );
	m_Info.sex						= m_pRes->LoadInt( "sex" );	
	m_Info.dwLookface				= m_pRes->LoadDWord( "lookface" );
	//m_Info.dwFeel					= m_pRes->LoadDWord( "lookface" );
	
	m_Info.i64HoldTitle				= m_pRes->GetInt64("hold_title");
	m_Info.iUseTitle				= m_pRes->LoadUInt("use_title");
	m_Info.Vit						= m_pRes->LoadDWord( "Vit" );
	m_Info.MaxVit					= m_pRes->LoadDWord( "Max_Vit" );
	m_Info.Ene						= m_pRes->LoadDWord( "Ene" );
	m_Info.MaxEne					= m_pRes->LoadDWord( "Max_Ene" );
	m_Info.usLevel					= m_pRes->LoadInt( "level" );
	m_Info.i64Exp					= m_pRes->GetInt64("exp");
	m_Info.CurJob                   = m_pRes->LoadDWord("Cur_job");

	m_Info.FirJob                   = m_pRes->LoadDWord("Fir_Job");
	m_Info.FirJobLev                = m_pRes->LoadDWord("Fir_JobLv");
	m_Info.FirJobPhase              = m_pRes->LoadDWord("Fir_JobPhase");
	m_Info.nLife					= m_pRes->LoadDWord("Fir_Hp");
	m_Info.nMana					= m_pRes->LoadDWord("Fir_Mp");
	m_Info.nSp						= m_pRes->LoadDWord("Fir_Sp");
	m_Info.FirJobStr                = m_pRes->LoadDWord("Fir_Str");
	m_Info.FirJobInt                = m_pRes->LoadDWord("Fir_Int");
	m_Info.FirJobSta                = m_pRes->LoadDWord("Fir_Sta");
	m_Info.FirJobSpi                = m_pRes->LoadDWord("Fir_Spi");
	m_Info.FirJobAgi                = m_pRes->LoadDWord("Fir_Agi");
	m_Info.FirJobPot                = m_pRes->LoadDWord("Fir_Pot");
	m_Info.FirJobCon                = m_pRes->LoadDWord("FirJob_Con");
	m_Info.SecJob                   = m_pRes->LoadDWord("Sec_Job");
	m_Info.SecJobLev                = m_pRes->LoadDWord("Sec_JobLv");
	m_Info.SecJobPhase              = m_pRes->LoadDWord("Sec_JobPhase");
	m_Info.SecJobStr                = m_pRes->LoadDWord("Sec_Str");
	m_Info.SecJobInt                = m_pRes->LoadDWord("Sec_Int");
	m_Info.SecJobSta                = m_pRes->LoadDWord("Sec_Sta");
	m_Info.SecJobSpi                = m_pRes->LoadDWord("Sec_Spi");
	m_Info.SecJobAgi                = m_pRes->LoadDWord("Sec_Agi");
	m_Info.SecJobPot                = m_pRes->LoadDWord("Sec_Pot");
	m_Info.SecJobCon                = m_pRes->LoadDWord("SecJob_Con");
	m_Info.idRecordmap				= m_pRes->LoadDWord( "recordmap_id" );
	m_Info.usRecordX				= m_pRes->LoadDWord( "recordx" );	
	m_Info.usRecordY				= m_pRes->LoadDWord( "recordy" );
	m_Info.idFamily					= m_pRes->LoadDWord( "Family_ID" );
	m_Info.idSyn					= m_pRes->LoadDWord( "Syn_ID" );
	m_Info.idNation				    = m_pRes->LoadDWord( "Nation_ID" );
	m_Info.nPk						= m_pRes->LoadDWord( "pk" );
	m_Info.GoodEvil					= m_pRes->LoadDWord( "GoodEvil" );
	m_Info.ucPkMode					= m_pRes->LoadDWord( "Protect_PK" ); 
	m_Info.FamilyCon				= m_pRes->LoadDWord( "Fam_Con" );
	m_Info.SynCon					= m_pRes->LoadDWord( "Syn_Con" );
	m_Info.NationCon				= m_pRes->LoadDWord( "Nat_Con" );
	m_Info.SceCon					= m_pRes->LoadDWord( "Sce_Con" );
	m_Info.ucTutorLev				= m_pRes->LoadDWord( "Tutor_Lv" );
	m_Info.idTutor					= m_pRes->LoadDWord( "Tutor_Mask" );
	m_Info.nPackageLev				= m_pRes->LoadDWord( "Bag_Lv" );
	m_Info.nBankLev					= m_pRes->LoadDWord( "Bank_Lv" );
	m_Info.money1					= m_pRes->LoadDWord( "Gold1" );
	m_Info.money2					= m_pRes->LoadDWord( "Gold2" );
	m_Info.money3					= m_pRes->LoadDWord( "Gold3" );
	m_Info.money4					= m_pRes->LoadDWord( "Gold4" );
	m_Info.money5					= 0; //请求账号服务器
	m_Info.moneySave				= m_pRes->LoadDWord( "Gold_Saved" );
	m_Info.i64TaskMask				= m_pRes->GetInt64("task_mask");
	m_Info.i64DayTaskMask			= m_pRes->GetInt64("daytask_mask");
	m_Info.i64DayTaskMask2			= m_pRes->GetInt64("daytask2_mask");
	m_Info.i64WeekTaskMask			= m_pRes->GetInt64("weektask_mask");
	m_Info.i64NewGiveMask			= m_pRes->GetInt64("newgivemask");
	m_Info.i64NewGiveMask2			= m_pRes->GetInt64("newgivemask2");

	m_Info.dwMaxScore			= m_pRes->GetInt64("max_score");
	m_Info.dwMaxPhyatt			= m_pRes->GetInt64("max_phyatt");
	m_Info.dwMaxMagatt			= m_pRes->GetInt64("max_magatt");
	m_Info.dwMaxMaxlife			= m_pRes->GetInt64("max_maxlife");
	m_Info.dwMaxCrit			= m_pRes->GetInt64("max_crit");
	m_Info.dwMaxHitrate			= m_pRes->GetInt64("max_hitrate");

	m_Info.vipState				= m_pRes->GetInt64("vipstate");
	m_Info.vipLev				= m_pRes->GetInt64("viplev");
	m_Info.vipTime				= m_pRes->GetInt64("viptime");
	//[2010-08-27 goto]特殊大红大蓝的增量，最后一位表示类型
	m_Info.dwHpAdd				= m_pRes->GetInt64("hp_add");
	m_Info.dwMpAdd				= m_pRes->GetInt64("mp_add");
	m_Info.dwPetAdd				= m_pRes->GetInt64("pet_mp_add");
	m_Info.Identity				= m_pRes->GetInt64( "identity" );	
	m_Info.FatiTime				= m_pRes->GetInt64("fatitime");
	m_Info.Vipface				= m_pRes->GetInt64("vipface");	//VIP脸
	m_Info.VipfaceTime			= m_pRes->GetInt64("vipfacetime");
	m_Info.EverydayOL			= m_pRes->GetInt64("EverydayOL");

	m_Info.dwLeaveExp			= m_pRes->LoadDWord( "leaveExp" );
	m_Info.MonsterKill			= m_pRes->LoadDWord( "KillMonseters" );
	m_pRes->LoadString( m_Info.szName,"name", _MAX_NAMESIZE );
	m_pRes->LoadString( m_Info.szFeel, "Feel", _MAX_FEELSIZE );//[2010-06-04 goto] 好友心情改为可自定义的字符串
    m_pRes->LoadString( m_Info.depotCode,"depotcode", 36 );	
	m_pRes->LoadString( m_Info.description,"Info_Des",128 );
	m_pRes->LoadString( m_Info.szItemLockPwd, "lockpwd", 24);//goto 物品锁密码
	
// 	m_pRes->LoadString( m_Info.qq,"QQ", 21 );
// 	m_pRes->LoadString( m_Info.prov,"prov", 21 );
// 	m_pRes->LoadString( m_Info.city,"city", 21 );
// 	m_pRes->LoadString( m_Info.old,"old", 21 );
// 	m_pRes->LoadString( m_Info.star,"star", 21 );
// 	m_pRes->LoadString(m_Info.btSex,"btSex",21);

	m_Info.onlineaward 				= m_pRes->GetInt64( "award" );	
	m_Info.bClosing					= false;
	m_Info.usSpeed                  = 100;
	m_Info.i64BuyASales				= m_pRes->GetInt64("buyvasitem");
	m_Info.nJinmeiValue             = m_pRes->LoadInt("jinmeivalue");
	m_Info.nMorality				= m_pRes->GetInt("TutorMorality");
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CUserData::SaveInfo(void)
{
	if (!m_pRes)
		return false;

	if(m_bNeedUpdate)
	{
		m_pRes->UpdateRecord();
		m_bNeedUpdate = false;
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////
void CUserData::SetMate(OBJID idMate, BOOL bUpdate)
{
	if(m_Info.idMate == idMate)
		return;

	m_Info.idMate = idMate;
	if (m_pRes)
	{
		m_pRes->SetDWord("mate", m_Info.idMate);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetDepotCode(LPCTSTR szDepotCode, BOOL bUpdate)
{
	if(strcmp(m_Info.depotCode,szDepotCode) == 0)
		return;

	SafeCopy(m_Info.depotCode, szDepotCode, 36);
	if (m_pRes)
	{
		m_pRes->SetString("depotcode", m_Info.depotCode, 36);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

//////////////////////////////////////////////////////////////////////
void CUserData::SetLife(int nLife, BOOL bUpdate)
{
	if(m_Info.nLife == nLife)
		return;

	m_Info.nLife	= nLife;
	if (m_pRes)
	{
		m_pRes->SetDWord("Fir_Hp", m_Info.nLife);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

DWORD	CUserData::GetLife()
{
	return m_Info.nLife;
}

//////////////////////////////////////////////////////////////////////
void CUserData::SetMana(int nPower, BOOL bUpdate)
{
	if(m_Info.nMana == nPower)
		return;

	m_Info.nMana	= nPower;
	if (m_pRes)
	{
		m_pRes->SetDWord("Fir_Mp", m_Info.nMana);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

DWORD	CUserData::GetMana()
{
	return m_Info.nMana;
}
//////////////////////////////////////////////////////////////////////
void CUserData::SetSp( int data,bool bUpdate/* = false*/ )
{
	if(m_Info.nSp == data)
		return;

	m_Info.nSp	= data;
	if (m_pRes)
	{
		m_pRes->SetDWord("Fir_Sp", m_Info.nSp);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

DWORD	CUserData::GetSp ()
{
	return m_Info.nSp;
}

void CUserData::SetProfession(DWORD nProff,int nType, BOOL bUpdate)
{
	if(nType == 1)
	{
		if(m_Info.FirJob == nProff)
			return;

		m_Info.FirJob	= nProff;
		if (m_pRes)
		{
			m_pRes->SetDWord("Fir_Job", m_Info.FirJob);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
	else
	{
		if(m_Info.SecJob == nProff)
			return;

		m_Info.SecJob	= nProff;
		if (m_pRes)
		{
			m_pRes->SetDWord("Sec_Job", m_Info.SecJob);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
}

DWORD CUserData::GetJobByType(DWORD nType)
{
	if(nType == 1)
	{
		return m_Info.FirJob;
	}
	else
	{
		return m_Info.SecJob;
	}
	return 0;
}

DWORD CUserData::GetJobCon(int  nType)
{
	if(nType == 0)
		nType = m_Info.CurJob;
	
	if(nType == 1)
	{
		return m_Info.FirJobCon;
	}
	else
	{
		return m_Info.SecJobCon;
	}
	return 0;
}

void CUserData::SetJobCon(int nData,int type, BOOL bUpdate) 
{
	if(type == 0)
		type = m_Info.CurJob;
	
	if(type == 1)
	{
		if(m_Info.FirJobCon	== nData)
			return;

		m_Info.FirJobCon	= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("FirJob_Con", m_Info.FirJobCon);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
	else
	{
		if(m_Info.SecJobCon	== nData)
			return;

		m_Info.SecJobCon	= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("SecJob_Con", m_Info.SecJobCon);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
}

void	CUserData::SetIdentity(int IdNum)
{
	if(m_Info.Identity == IdNum)
		return;

	m_Info.Identity = IdNum;
	if(m_pRes)
	{
		m_pRes->SetDWord("identity", m_Info.Identity);
	}
}

DWORD CUserData::GetJobLev(int  nType)
{
	if(nType == 0)
		nType = m_Info.CurJob;

	if(nType == 1)
	{
		return m_Info.FirJobLev;
	}
	else
	{
		return m_Info.SecJobLev;
	}
	return 0;
}

void CUserData::SetJobLev(int nData,int type, BOOL bUpdate) 
{
	if(type == 0)
		type = m_Info.CurJob;

	if(type == 1)
	{
		if(m_Info.FirJobLev	== nData)
			return;

		m_Info.FirJobLev	= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Fir_JobLv", m_Info.FirJobLev);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
	else
	{
		if(m_Info.SecJobLev	== nData)
			return;

		m_Info.SecJobLev	= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Sec_JobLv", m_Info.SecJobLev);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
}

void CUserData::AddStr(int nData,int type, BOOL bUpdate) 
{
	if(type == 0)
		type = m_Info.CurJob;
	if(nData == 0)
		return;

	if(type == 1)
	{
		m_Info.FirJobStr	+= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Fir_Str", m_Info.FirJobStr);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
	else
	{
		m_Info.SecJobStr	+= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Sec_Str", m_Info.SecJobStr);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
}

void CUserData::SetStr(int nData,int type, BOOL bUpdate) 
{
	if(type == 1)
	{
		if(m_Info.FirJobStr	== nData)
			return;

		m_Info.FirJobStr	= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Fir_Str", m_Info.FirJobStr);
			if (bUpdate)
				SaveInfo();
			else
			m_bNeedUpdate = true;
		}
	}
	else
	{
		if(m_Info.SecJobStr	== nData)
			return;

		m_Info.SecJobStr	= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Sec_Str", m_Info.SecJobStr);
			if (bUpdate)
				SaveInfo();
			else
			m_bNeedUpdate = true;
		}
	}
}

DWORD CUserData::GetStr(int type)
{
	if(type == 0)
		type = m_Info.CurJob;
	if(type == 1)
	{
		return m_Info.FirJobStr / 100;
	}
	else
	{
		return m_Info.SecJobStr / 100;
	}
}

void CUserData::AddIntex(int nData,int type, BOOL bUpdate) 
{
	if(type == 0)
		type = m_Info.CurJob;
	if(nData == 0)
		return;

	if(type == 1)
	{
		m_Info.FirJobInt	+= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Fir_Int", m_Info.FirJobInt);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
	else
	{
		m_Info.SecJobInt	+= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Sec_Int", m_Info.SecJobInt);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
}

void CUserData::SetIntex(int nData,int type, BOOL bUpdate) 
{
	if(type == 0)
		type = m_Info.CurJob;
	if(type == 1)
	{
		if(m_Info.FirJobInt	== nData)
			return;

		m_Info.FirJobInt	= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Fir_Int", m_Info.FirJobInt);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
	else
	{
		if(m_Info.SecJobInt	== nData)
			return;

		m_Info.SecJobInt	= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Sec_Int", m_Info.SecJobInt);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
}

DWORD CUserData::GetIntex(int type)
{
	if(type == 0)
		type = m_Info.CurJob;
	if(type == 1)
	{
		return m_Info.FirJobInt / 100;
	}
	else
	{
		return m_Info.SecJobInt / 100;
	}
}

void CUserData::AddSta(int nData,int type, BOOL bUpdate) 
{
	if(type == 0)
		type = m_Info.CurJob;
	if(nData == 0)
		return;

	if(type == 1)
	{
		m_Info.FirJobSta	+= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Fir_Sta", m_Info.FirJobSta);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
	else
	{
		m_Info.SecJobSta	+= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Sec_Sta", m_Info.SecJobSta);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
}

void CUserData::SetSta(int nData,int type, BOOL bUpdate) 
{
	if(type == 0)
		type = m_Info.CurJob;
	if(type == 1)
	{
		if(m_Info.FirJobSta	== nData)
			return;

		m_Info.FirJobSta	= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Fir_Sta", m_Info.FirJobSta);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
	else
	{
		if(m_Info.SecJobSta	== nData)
			return;

		m_Info.SecJobSta	= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Sec_Sta", m_Info.SecJobSta);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
}


DWORD CUserData::GetSta(int type)
{
	if(type == 0)
		type = m_Info.CurJob;
	if(type == 1)
	{
		return m_Info.FirJobSta / 100;
	}
	else
	{
		return m_Info.SecJobSta / 100;
	}
}

void CUserData::AddSpi(int nData,int type, BOOL bUpdate) 
{
	if(type == 0)
		type = m_Info.CurJob;
	if(nData == 0)
		return;

	if(type == 1)
	{
		m_Info.FirJobSpi	+= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Fir_Spi", m_Info.FirJobSpi);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
	else
	{
		m_Info.SecJobSpi	+= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Sec_Spi", m_Info.SecJobSpi);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
}

void CUserData::SetSpi(int nData,int type, BOOL bUpdate) 
{
	if(type == 0)
		type = m_Info.CurJob;
	if(type == 1)
	{
		if(m_Info.FirJobSpi == nData)
			return;

		m_Info.FirJobSpi	= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Fir_Spi", m_Info.FirJobSpi);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
	else
	{
		if(m_Info.SecJobSpi	== nData)
			return;

		m_Info.SecJobSpi	= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Sec_Spi", m_Info.SecJobSpi);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
}

DWORD CUserData::GetSpi(int type)
{
	if(type == 0)
		type = m_Info.CurJob;
	if(type == 1)
	{
		return m_Info.FirJobSpi / 100;
	}
	else
	{
		return m_Info.SecJobSpi / 100;
	}
}

void CUserData::AddAgi(int nData,int type, BOOL bUpdate) 
{
	if(type == 0)
		type = m_Info.CurJob;
	if(nData == 0)
		return;

	if(type == 1)
	{
		m_Info.FirJobAgi	+= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Fir_Agi", m_Info.FirJobAgi);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
	else
	{
		m_Info.SecJobAgi	+= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Sec_Agi", m_Info.SecJobAgi);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
}

void CUserData::SetAgi(int nData,int type, BOOL bUpdate) 
{
	if(type == 0)
		type = m_Info.CurJob;
	if(type == 1)
	{
		if(m_Info.FirJobAgi == nData)
			return;

		m_Info.FirJobAgi	= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Fir_Agi", m_Info.FirJobAgi);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
	else
	{
		if(m_Info.SecJobAgi	== nData)
			return;

		m_Info.SecJobAgi	= nData;
		if (m_pRes)
		{
			m_pRes->SetDWord("Sec_Agi", m_Info.SecJobAgi);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
}

DWORD CUserData::GetAgi(int type)
{
	if(type == 0)
		type = m_Info.CurJob;
	if(type == 1)
	{
		return m_Info.FirJobAgi / 100;
	}
	else
	{
		return m_Info.SecJobAgi / 100;
	}
}

void CUserData::AddPot(int nData,int type, BOOL bUpdate)
{
	if(type == 0)
		type = m_Info.CurJob;
	if(nData == 0)
		return;

	if(type == 1)
	{
		m_Info.FirJobPot	+= nData;
		if(m_Info.FirJobPot < 0)
		{
			::LogSave("AddPot1() error [%s]",m_Info.id);
			m_Info.FirJobPot = 0;
		}
		
		if (m_pRes)
		{
			m_pRes->SetDWord("Fir_Pot", m_Info.FirJobPot);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
	else
	{
		m_Info.SecJobPot	+= nData;

		if(m_Info.SecJobPot < 0)
		{
			::LogSave("AddPot2() error [%s]",m_Info.id);
			m_Info.SecJobPot = 0;
		}

		if (m_pRes)
		{
			m_pRes->SetDWord("Sec_Pot", m_Info.SecJobPot);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
}

void CUserData::SetPot(int nData,int type, BOOL bUpdate) 
{
	if(type == 0)
		type = m_Info.CurJob;
	if(type == 1)
	{
		if(m_Info.FirJobPot == nData)
			return;

		m_Info.FirJobPot	= nData;

		if(m_Info.FirJobPot < 0)
		{
			::LogSave("AddPot1() error [%s]",m_Info.id);
			m_Info.FirJobPot = 0;
		}

		if (m_pRes)
		{
			m_pRes->SetDWord("Fir_Pot", m_Info.FirJobPot);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
	else
	{
		if(m_Info.SecJobPot	== nData)
			return;

		m_Info.SecJobPot	= nData;

		if(m_Info.SecJobPot < 0)
		{
			::LogSave("AddPot2() error [%s]",m_Info.id);
			m_Info.SecJobPot = 0;
		}
		if (m_pRes)
		{
			m_pRes->SetDWord("Sec_Pot", m_Info.SecJobPot);
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
}

DWORD CUserData::GetPot(int type)
{
	if(type == 0)
		type = m_Info.CurJob;
	if(type == 1)
	{
		return m_Info.FirJobPot;
	}
	else
	{
		return m_Info.SecJobPot;
	}
}

DWORD CUserData::GetMoney ( int moneyTye)
{
	switch(moneyTye)
	{
	case 1:
		{
			return m_Info.money1;
		}
		break;
	case 2:
		{
			return m_Info.money2;
		}
		break;
	case 3:
		{
			return m_Info.money3;
		}
		break;
	case 4:
		{
			return m_Info.money4;
		}
		break;
	case 5:
		{
			return m_Info.money5;
		}
		break;
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////
void CUserData::SetMoney(int nMoney,int moneyTye, BOOL bUpdate)
{
	switch(moneyTye)
	{
	case 1:
		{
			if(m_Info.money1 == nMoney)
				return;

			m_Info.money1 = nMoney;
			if (m_pRes)
			{
				m_pRes->SetDWord("Gold1", m_Info.money1);
				if (bUpdate)
					SaveInfo();
				else
					m_bNeedUpdate = true;
			}
		}
		break;
	case 2:
		{
			if(m_Info.money2 == nMoney)
				return;

			m_Info.money2 = nMoney;
			if (m_pRes)
			{
				m_pRes->SetDWord("Gold2", m_Info.money2);
				if (bUpdate)
					SaveInfo();
				else
					m_bNeedUpdate = true;
			}
		}
		break;
	case 3:
		{
			if(m_Info.money3 == nMoney)
				return;

			m_Info.money3 = nMoney;
			if (m_pRes)
			{
				m_pRes->SetDWord("Gold3", m_Info.money3);
				if (bUpdate)
					SaveInfo();
				else
					m_bNeedUpdate = true;
			}
		}
		break;
	case 4:
		{
			if(m_Info.money4 == nMoney)
				return;

			m_Info.money4 = nMoney;
			if (m_pRes)
			{
				m_pRes->SetDWord("Gold4", m_Info.money4);
				if (bUpdate)
					SaveInfo();
				else
					m_bNeedUpdate = true;
			}
		}
		break;
	case 5:
		{
			m_Info.money5 = nMoney;
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////
void CUserData::SetMoneySaved(int nMoney, BOOL bUpdate)
{
	if(m_Info.moneySave	== nMoney)
		return;

	m_Info.moneySave	= nMoney;
	if (m_pRes)
	{
		m_pRes->SetDWord("Gold_Saved", m_Info.moneySave);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetExp(_int64 i64Exp, BOOL bUpdate)
{
	if(m_Info.i64Exp == i64Exp)
		return;

	m_Info.i64Exp	= i64Exp;
	if (m_pRes)
	{
	//	m_pRes->SetDWord("exp", nExp);
		m_pRes->SetInt64("exp", m_Info.i64Exp);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetVit(int nVit, BOOL bUpdate)
{
	if(m_Info.Vit == nVit)
		return;

	m_Info.Vit	= nVit;
	if(m_Info.Vit > m_Info.MaxVit)
		m_Info.Vit = m_Info.MaxVit;
	if (m_pRes)
	{
		m_pRes->SetInt64("Vit", m_Info.Vit);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}
//goto 体力
void CUserData::SetEne(int nEne, BOOL bUpdate)
{
	if(m_Info.Ene == nEne)
		return;

	m_Info.Ene	= nEne;
	if(m_Info.Ene > m_Info.MaxEne)
		m_Info.Ene = m_Info.MaxEne;
	if (m_pRes)
	{
		m_pRes->SetInt64("Ene", m_Info.Ene);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetCurJob(int curJob, BOOL bUpdate)
{
	if(m_Info.CurJob == curJob)
		return;

	m_Info.CurJob	= curJob;
	if (m_pRes)
	{
		//	m_pRes->SetDWord("exp", nExp);
		m_pRes->SetInt64("Cur_job", m_Info.CurJob);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetNationCon	(int nNationCon, BOOL bUpdate)	
{
	if(m_Info.NationCon	== nNationCon)
		return;

	m_Info.NationCon	= nNationCon;
	if (m_pRes)
	{
		m_pRes->SetDWord("Nat_Con", nNationCon);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetGoodEvil		(int nGoodEvil, BOOL bUpdate)	
{
	if(m_Info.GoodEvil	== nGoodEvil)
		return;

	m_Info.GoodEvil	= nGoodEvil;
	if (m_pRes)
	{
		m_pRes->SetDWord("GoodEvil", nGoodEvil);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

//////////////////////////////////////////////////////////////////////
void CUserData::SetPk(int nPk, BOOL bUpdate)
{
	if(m_Info.nPk == nPk)
		return;

	m_Info.nPk	= nPk;
	if (m_pRes)
	{
		m_pRes->SetDWord("pk", nPk);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}
///////////////////////////////////////////////////////////////////

void CUserData::SetMaxScore	(DWORD dwMaxScore, BOOL bUpdate)
{
	if(m_Info.dwMaxScore == dwMaxScore)
		return;

	m_Info.dwMaxScore	= dwMaxScore;
	if (m_pRes)
	{
		m_pRes->SetDWord("max_score", m_Info.dwMaxScore);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetMaxPhyatt	(DWORD dwMaxPhyatt, BOOL bUpdate)
{
	if(m_Info.dwMaxPhyatt == dwMaxPhyatt)
		return;

	m_Info.dwMaxPhyatt	= dwMaxPhyatt;
	if (m_pRes)
	{
		m_pRes->SetDWord("max_phyatt", m_Info.dwMaxPhyatt);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetMaxMagatt	(DWORD dwMaxMagatt, BOOL bUpdate)
{
	if(m_Info.dwMaxMagatt == dwMaxMagatt)
		return;

	m_Info.dwMaxMagatt	= dwMaxMagatt;
	if (m_pRes)
	{
		m_pRes->SetDWord("max_magatt", m_Info.dwMaxMagatt);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetMaxMaxlife	(DWORD dwMaxMaxlife, BOOL bUpdate)
{
	if(m_Info.dwMaxMaxlife == dwMaxMaxlife)
		return;

	m_Info.dwMaxMaxlife	= dwMaxMaxlife;
	if (m_pRes)
	{
		m_pRes->SetDWord("max_maxlife", m_Info.dwMaxMaxlife);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetMaxCrit	(DWORD dwMaxCrit, BOOL bUpdate)
{
	if(m_Info.dwMaxCrit == dwMaxCrit)
		return;

	m_Info.dwMaxCrit	= dwMaxCrit;
	if (m_pRes)
	{
		m_pRes->SetDWord("max_crit", m_Info.dwMaxCrit);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetMaxHitrate	(DWORD dwMaxHitrate, BOOL bUpdate)
{
	if(m_Info.dwMaxHitrate == dwMaxHitrate)
		return;

	m_Info.dwMaxHitrate	= dwMaxHitrate;
	if (m_pRes)
	{
		m_pRes->SetDWord("max_hitrate", m_Info.dwMaxHitrate);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetRecordPos(OBJID idMap, DWORD dwX, DWORD dwY, BOOL bUpdate)
{
	bool bChange = false;
	if(m_Info.idRecordmap != idMap)
	{
		m_Info.idRecordmap	= idMap;
		bChange = true;
		if (m_pRes)
		{	
			m_pRes->SetDWord("recordmap_id", m_Info.idRecordmap);
		}
	}
	if(m_Info.usRecordX != dwX)
	{
		m_Info.usRecordX	= dwX;
		bChange = true;
		if (m_pRes)
		{	
			m_pRes->SetDWord("recordx", m_Info.usRecordX);
		}
	}
	if(m_Info.usRecordY != dwY)
	{
		m_Info.usRecordY	= dwY;
		bChange = true;
		if (m_pRes)
		{	
			m_pRes->SetDWord("recordy", m_Info.usRecordY);
		}
	}


	if (m_pRes)
	{
		if(bChange)
		{
			if (bUpdate)
				SaveInfo();
			else
				m_bNeedUpdate = true;
		}
	}
}

void CUserData::SetLastLogout()
{
	m_Info.dwExitTime	= ::TimeGet(TIME_MINUTE);
	if (m_pRes)
	{
		m_pRes->SetDWord("Exit_Time", m_Info.dwExitTime);
		m_bNeedUpdate = true;
	}
}

void CUserData::SetLeaveExp(DWORD dwData)
{
	if(m_Info.dwLeaveExp == dwData)
		return;

	m_Info.dwLeaveExp	= dwData;
	if (m_pRes)
	{
		m_pRes->SetDWord("leaveExp", m_Info.dwLeaveExp);
		m_bNeedUpdate = true;
	}
}

//////////////////////////////////////////////////////////////////////
void CUserData::SetLastLogin(int nTime/* = 0*/)
{
	if(nTime == 0)
		m_Info.dwLast_login	= ::DateStamp();
	else
		m_Info.dwLast_login = nTime;
	if (m_pRes)
	{
		m_pRes->SetDWord("last_login", m_Info.dwLast_login);
		m_bNeedUpdate = true;
	}
}
// void CUserData::SetLastChangeDescTime( DWORD time,BOOL bUpdate )
// {
// 	m_Info.dwLastChangeDescTime = time;
// 	if( m_pRes )
// 	{
// 		m_pRes->SetDWord( "last_changedesc",m_Info.dwLastChangeDescTime );
// 		if( bUpdate )
// 			SaveInfo();
// 	}
// }
//////////////////////////////////////////////////////////////////////
void CUserData::SetName(const char* pszName)
{
#if defined(_DEBUG) || defined(_ASHRELEASE)//20071010修罗:
	
	if (!pszName || strlen(pszName) >= _MAX_NAMESIZE)
		return;

	strcpy(m_Info.szName, pszName);
	if (m_pRes)
	{
		m_pRes->SetString("name", m_Info.szName, _MAX_NAMESIZE);
	}
#endif
}

void CUserData::SetDescription( char* desc,BOOL bUpdate )
{
	if( !desc || strlen( desc ) >= 128 )//保留一个\0,所以>=..
		return;
	if(strcmp(m_Info.description,desc) == 0)
		return;

	strcpy( m_Info.description,desc );
	if( m_pRes )
	{
		m_pRes->SetString( "description",m_Info.description,128 );
		if( bUpdate )
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

//////////////////////////////////////////////////////////////////////
void CUserData::SetLev(int nLev, BOOL bUpdate)
{
	if(m_Info.usLevel == nLev)
		return;

	m_Info.usLevel	= nLev;
	if (m_pRes)
	{
		m_pRes->SetUInt("level", m_Info.usLevel);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

//////////////////////////////////////////////////////////////////////
// void CUserData::SetForce(int nForce , BOOL bUpdate)
// {
// 	m_Info.usForce	= nForce;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("force_", nForce);
// 		if (bUpdate)
// 			SaveInfo();
// 	}
// }

//////////////////////////////////////////////////////////////////////
// void CUserData::SetAddPoint(int nAddPoint, BOOL bUpdate)
// {
// 	m_Info.usAdd_point	= nAddPoint;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("add_point", nAddPoint);
// 		if (bUpdate)
// 			SaveInfo();
// 	}
//}

//////////////////////////////////////////////////////////////////////
// void CUserData::SetDexterity(DWORD dwDex, BOOL bUpdate)
// {
// 	m_Info.usDexterity	= dwDex;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("dexterity", dwDex);
// 		if (bUpdate)
// 			SaveInfo();
// 	}
// }

//////////////////////////////////////////////////////////////////////
void CUserData::SetSpeed(DWORD dwSpeed, BOOL bUpdate)
{
	m_Info.usSpeed	= dwSpeed;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("speed", dwSpeed);
// 		if (bUpdate)
// 			SaveInfo();
// 	}
}

//////////////////////////////////////////////////////////////////////
// void CUserData::SetHealth(DWORD dwHealth, BOOL fUpdate)
// {
// 	m_Info.usHealth	= dwHealth;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("health", dwHealth);
// 		if (fUpdate)
// 			SaveInfo();
// 	}
// }

//////////////////////////////////////////////////////////////////////
// void CUserData::SetSoul(DWORD dwSoul, BOOL fUpdate)
// {
// 	m_Info.usSoul	= dwSoul;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("soul", dwSoul);
// 		if (fUpdate)
// 			SaveInfo();
// 	}
// }

//////////////////////////////////////////////////////////////////////
void CUserData::SetLookFace(DWORD dwLookFace, BOOL fUpdate)
{
	if(m_Info.dwLookface == dwLookFace)
		return;

	m_Info.dwLookface	= dwLookFace;
	if (m_pRes)
	{
		m_pRes->SetUInt("lookface", m_Info.dwLookface);
		if (fUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}
//[2010-06-04 goto] 用户心情，在好友中显示
void CUserData::SetFeel(char* szFeel, BOOL bUpdate/*=false*/)
{	
	if( !szFeel || strlen( szFeel ) >= _MAX_FEELSIZE )//保留一个\0,所以>=..
		return;
	if(strcmp(m_Info.szFeel,szFeel) == 0)
		return;

	strcpy( m_Info.szFeel,szFeel );
	if( m_pRes )
	{
		m_pRes->SetString( "Feel", m_Info.szFeel, _MAX_FEELSIZE );
		if( bUpdate )
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}
//////////////////////////////////////////////////////////////////////
// void CUserData::SetProfession (DWORD dwProfession, BOOL bUpdate)
// {
// 	m_Info.nProfession	= dwProfession;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("profession", dwProfession);
// 		if (bUpdate)
// 			SaveInfo();
// 	}
//}
void CUserData::SetTaskMask (_int64 i64Task)
{
	if(m_Info.i64TaskMask == i64Task)
		return;

	m_Info.i64TaskMask	= i64Task;
	if (m_pRes)
	{
		m_pRes->SetInt64("task_mask", m_Info.i64TaskMask);
		m_bNeedUpdate = true;
	}
}

bool CUserData::SetNewGiveMask (_int64 i64Task)
{
	if(m_Info.i64NewGiveMask == i64Task)
		return true;

	m_Info.i64NewGiveMask	= i64Task;
	if (m_pRes)
	{
		m_pRes->SetInt64("newgivemask", m_Info.i64NewGiveMask);
		m_bNeedUpdate = true;
		return true;
	}
	return false;
}

bool CUserData::SetNewGiveMask2 (_int64 i64Task)
{
	if(m_Info.i64NewGiveMask2 == i64Task)
		return true;

	m_Info.i64NewGiveMask2	= i64Task;
	if (m_pRes)
	{
		m_pRes->SetInt64("newgivemask2", m_Info.i64NewGiveMask2);
		m_bNeedUpdate = true;
		return true;
	}
	return false;
}

void CUserData::SetDayTaskMask (_int64 i64DayMask)
{
	if(m_Info.i64DayTaskMask == i64DayMask)
		return;

	m_Info.i64DayTaskMask	= i64DayMask;
	if (m_pRes)
	{
		m_pRes->SetInt64("daytask_mask", m_Info.i64DayTaskMask);
		m_bNeedUpdate = true;
	}
}

void CUserData::SetDayTaskMask2 (_int64 i64DayMask2)
{
	if(m_Info.i64DayTaskMask2 == i64DayMask2)
		return;

	m_Info.i64DayTaskMask2	= i64DayMask2;
	if (m_pRes)
	{
		m_pRes->SetInt64("daytask2_mask", m_Info.i64DayTaskMask2);
		m_bNeedUpdate = true;
	}
}


void CUserData::SetWeekTaskMask (_int64 i64WeekMask)
{
	if(m_Info.i64WeekTaskMask == i64WeekMask)
		return;

	m_Info.i64WeekTaskMask	= i64WeekMask;
	if (m_pRes)
	{
		m_pRes->SetInt64("weektask_mask", m_Info.i64WeekTaskMask);
		m_bNeedUpdate = true;
	}
}

// void CUserData::SetCredit( DWORD dwCredit,BOOL bUpdate )
// {
// 	m_Info.dwCredit = dwCredit;
// 	if( m_pRes )
// 	{
// 		m_pRes->SetDWord( "credit",dwCredit );
// 		if( bUpdate )
// 			SaveInfo();
// 	}
// }
//////////////////////////////////////////////////////////////////////
// void CUserData::SetHomeMap( OBJID idMap, BOOL bUpdate )
// {
// 	m_Info.idHomemap = idMap;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetDWord("homemap", idMap );
// 		if (bUpdate)
// 			SaveInfo();
// 	}
// }

//////////////////////////////////////////////////////////////////////
// void CUserData::SetNobilityRank(DWORD dwNobilityRank, BOOL bUpdate)
// {
// 	m_Info.ucNobilityRank = dwNobilityRank;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetDWord("nobility_rank", dwNobilityRank);
// 		if(bUpdate)
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetLoveliness(int nLoveliness, BOOL bUpdate)
// {
// 	m_Info.usLoveliness	= nLoveliness;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("loveliness", nLoveliness);
// 		if (bUpdate)
// 			SaveInfo();
// 	}
// }

void CUserData::SetTutorLevel(DWORD ucLevel, BOOL bUpdate)
{
	if(m_Info.ucTutorLev == ucLevel)
		return;

	m_Info.ucTutorLev = ucLevel;
	if (m_pRes)
	{
		m_pRes->SetUInt("Tutor_Lv", m_Info.ucTutorLev);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetTutorMask( int nMask, BOOL bUpdate)
{
	if(m_Info.idTutor == nMask)
		return;

	m_Info.idTutor = nMask;
	if (m_pRes)
	{
		m_pRes->SetUInt("Tutor_Mask", m_Info.idTutor);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetPackLev( DWORD ucLevel,bool bUpdate)
{
	if(m_Info.nPackageLev == ucLevel)
		return;

	m_Info.nPackageLev = ucLevel;
	if (m_pRes)
	{
		m_pRes->SetUInt("Bag_Lv", m_Info.nPackageLev);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetBankLev(DWORD ucLevel, BOOL bUpdate)
{
	if(m_Info.nBankLev == ucLevel)
		return;

	m_Info.nBankLev = ucLevel;
	if (m_pRes)
	{
		m_pRes->SetUInt("Bank_Lv", 	m_Info.nBankLev);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetSex( UINT nSex,BOOL bUpdate)
{
	if(m_Info.sex == nSex)
		return;

	m_Info.sex	= nSex;
	if (m_pRes)
	{
		m_pRes->SetUInt("sex", m_Info.sex);
		if( bUpdate )
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

// void CUserData::SetStoneLev(UINT nStoneLev,BOOL bUpdate /* = true  */)
// {
// 	m_Info.stoneLev	= nStoneLev;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("StoneLev", nStoneLev);
// 		if( bUpdate )
// 			SaveInfo();
// 	}
// }

// void CUserData::SetGrassLev(UINT nGrassLev,BOOL bUpdate /* = true  */)
// {
// 	m_Info.grassLev	= nGrassLev;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("GrassLev", nGrassLev);
// 		if( bUpdate )
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetBodyLev(UINT nBodyLev,BOOL bUpdate /* = true  */)
// {
// 	m_Info.bodyLev	= nBodyLev;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("BodyLev", nBodyLev);
// 		if( bUpdate )
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetBoxLev(UINT nBoxLev,BOOL bUpdate /* = true  */)
// {
// 	m_Info.boxLev	= nBoxLev;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("BoxLev", nBoxLev);
// 		if( bUpdate )
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetGoodKillTime( UINT nGoodKillTime, BOOL bUpdate )
// {
// 	m_Info.nGoodKillTime	= nGoodKillTime;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("kill_good", nGoodKillTime);
// 		if( bUpdate )
// 		SaveInfo();
// 	}
// }
// 
// void CUserData::SetBadKillTime( UINT nBadKillTime, BOOL bUpdate )
// {
// 	m_Info.nBadKillTime	= nBadKillTime;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("kill_bad", nBadKillTime);
// 		if( bUpdate )
// 		SaveInfo();
// 	}
// }


// void CUserData::SetMagicStoneBoxLimit( int MagicStoneBoxLimit, BOOL bUpdate)////仓库的最大存放数,单位格数
// {
// 	m_Info.nMagicStoneBoxLimit	= MagicStoneBoxLimit;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("magicstonebox_limit", MagicStoneBoxLimit);
// 		if( bUpdate )
// 			SaveInfo();
// 	}
// }

// void CUserData::SetStorageLimit( int StorageLimit, BOOL bUpdate)////仓库的最大存放数,单位格数
// {
// 	m_Info.nStorageLimit	= StorageLimit;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("storage_limit", StorageLimit);
// 		if( bUpdate )
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetStoreStatus( _int64 nStoreStatus, BOOL bUpdate)////彭峰2007/02/08直接修改DB里的非荣誉击杀次数(kill_bad)
// {
// 	m_Info.nStoreStatus	= nStoreStatus;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("storestatus", nStoreStatus);
// 		if( bUpdate )
// 		SaveInfo();
// 	}
// }
// 
// void CUserData::SetKillGoodAction(int KillGoodAction, BOOL bUpdate /* = true  */)//  [4/10/2007]哈雷，修改SKUSER中的KILLGOODACTION
// {
// 	m_Info.nKillGoodAction = KillGoodAction;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("kill_good_action", KillGoodAction);
// 		if( bUpdate )
// 		SaveInfo();
// 	}
// }
// 
// void CUserData::SetKillBadAction(int KillBadAction, BOOL bUpdate /* = true  */)//  [4/10/2007]哈雷，修改SKUSER中的KILLbadACTION
// {
// 	m_Info.nKillBadAction = KillBadAction;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("kill_bad_action", KillBadAction);
// 		if( bUpdate )
// 		SaveInfo();
// 	}
// }
// 
// void CUserData::SetKillDeadAction(int KillDeadAction, BOOL bUpdate /* = true  */)//  [4/10/2007]哈雷，修改SKUSER中的KILLdeadACTION
// {
// 	m_Info.nKillDeadAction = KillDeadAction;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetUInt("pk_dead_action", KillDeadAction);
// 		if( bUpdate )
// 		SaveInfo();
// 	}
// }

void	CUserData::SetCreateTime( DWORD dwData, BOOL bUpdate)
{
// 	m_Info.dwCreateTime = dwData;
// 	if(m_pRes)
// 	{
// 		m_pRes->SetUInt("Creat_Time", m_Info.dwCreateTime);
// 		if( bUpdate )
// 			SaveInfo();
// 	}
}

void	CUserData::SetExitTime( DWORD dwData, BOOL bUpdate)
{
	if(m_Info.dwExitTime == dwData)
		return;

	m_Info.dwExitTime = dwData;
	if(m_pRes)
	{
		m_pRes->SetUInt("Exit_Time", m_Info.dwExitTime);
		if( bUpdate )
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void	CUserData::SetAllOnlineTime( DWORD dwData, BOOL bUpdate )
{
	if(m_Info.dwAllOnlineTime == dwData)
		return;

	m_Info.dwAllOnlineTime = dwData;
	if(m_pRes)
	{
		m_pRes->SetUInt("all_online_time", m_Info.dwAllOnlineTime);
		if( bUpdate )
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void	CUserData::SetAllAddExpTime( DWORD nAllAddExpTime, BOOL bUpdate/* = true  */)// 设置累积经验值[9/12/2007 %PENGFENG%]
{
	if(m_Info.dwAllDoubleTime == nAllAddExpTime)
		return;

	m_Info.dwAllDoubleTime = nAllAddExpTime;
	if(m_pRes)
	{
		m_pRes->SetUInt("all_add_exp_time", m_Info.dwAllDoubleTime);
		if( bUpdate )
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}



// void CUserData::SetGiftPoint(int nGiftPoint,BOOL bUpdate)
// {
// 	m_Info.giftpoint = nGiftPoint;
// 	if(m_pRes)
// 	{
// 		m_pRes->SetInt("giftpoint",nGiftPoint);
// 		if(bUpdate)
// 			SaveInfo();
// 	}
// }


// void CUserData::SetQq(const char* pszQq,BOOL bUpdate)
// {
// 	if( !pszQq || strlen( pszQq ) >= 20 )//保留一个\0,所以>=..
// 		return;
// 	SafeCopy( m_Info.qq,pszQq,20 );
// 	if( m_pRes )
// 	{
// 		m_pRes->SetString( "QQ",m_Info.qq,20 );
// 		if( bUpdate )
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetProv(const char* pszProv,BOOL bUpdate)
// {
// 	if( !pszProv || strlen( pszProv ) >= 20 )//保留一个\0,所以>=..
// 		return;
// 	SafeCopy( m_Info.prov,pszProv ,20);
// 	if( m_pRes )
// 	{
// 		m_pRes->SetString( "prov",m_Info.prov,20 );
// 		if( bUpdate )
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetCity(const char* pszCity,BOOL bUpdate)
// {
// 	if( !pszCity || strlen( pszCity ) >= 20 )//保留一个\0,所以>=..
// 		return;
// 	SafeCopy( m_Info.city,pszCity,20 );
// 	if( m_pRes )
// 	{
// 		m_pRes->SetString( "city",m_Info.city,11 );
// 		if( bUpdate )
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetStar(const char* pszStar,BOOL bUpdate)
// {
// 	if( !pszStar || strlen( pszStar ) >= 20 )//保留一个\0,所以>=..
// 		return;
// 	SafeCopy( m_Info.star,pszStar,20 );
// 	if( m_pRes )
// 	{
// 		m_pRes->SetString( "star",m_Info.star,20 );
// 		if( bUpdate )
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetOld(const char* pasOld,BOOL bUpdate)
// {
// 	if( !pasOld || strlen( pasOld ) >= 20 )//保留一个\0,所以>=..
// 		return;
// 	//strcpy( m_Info.old,pasOld );
// 	SafeCopy(m_Info.old, pasOld, 20);
// 	if( m_pRes )
// 	{
// 		m_pRes->SetString( "old",m_Info.old,20 );
// 		if( bUpdate )
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetBtSex(const char* pasBtSex,BOOL bUpdate)
// {
// 	if( !pasBtSex || strlen( pasBtSex ) >= 12 )//保留一个\0,所以>=..
// 		return;
// 	SafeCopy(m_Info.btSex, pasBtSex, 20);
// 	if( m_pRes )
// 	{
// 		m_pRes->SetString( "btSex",m_Info.btSex,20 );
// 		if( bUpdate )
// 			SaveInfo();
// 	}
// }

void CUserData::SetHoldTitle(UINT nPower, BOOL bUpdate, IDatabase* pDb)
{
	if(m_Info.i64HoldTitle == nPower)
		return;
	
	m_Info.i64HoldTitle = nPower;
	if(m_pRes)
	{
		m_pRes->SetInt64("hold_title", m_Info.i64HoldTitle);
		if( bUpdate )
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
// 	__int64 i64OldTitle = NULL;
// 	if (pDb)
// 	{
// 		char szSQL[128];
// 		sprintf(szSQL, "SELECT hold_title FROM %s WHERE id = %d LIMIT 1", _TBL_USER ,GetID());
// 		IRecordset* pRes = pDb->CreateNewRecordset(szSQL, false);
// 		if (pRes)
// 		{
// 			i64OldTitle = pRes->GetInt(0);
// 			pRes->Release();
// 		}
// 	}
// 	if (i64OldTitle != NULL && i64OldTitle != m_Info.i64HoldTitle)
// 	{
// 		 m_Info.i64HoldTitle = i64OldTitle;
// 	}
// 
// 	UINT iIDH = ((nPower / 100) - 1) * 4;
// 	UINT iIDL= nPower % 100;
// 	UINT iOld = (m_Info.i64HoldTitle >> iIDH) & 0xF;
// 
// 
// 	i64OldTitle = static_cast<__int64>(0xF) << iIDH;
// 	i64OldTitle ^= 0xFFFFFFFFFFFFFFFF;
// 	i64OldTitle &= m_Info.i64HoldTitle;
// 	i64OldTitle |= static_cast<__int64>(iIDL) << iIDH;
// 	m_Info.i64HoldTitle = i64OldTitle;
// 
// 	if (m_pRes)
// 	{
// 		m_pRes->SetInt64("hold_title", m_Info.i64HoldTitle);
// 		if (bUpdate)
// 		{
// 			SaveInfo();
// 		}
// 	}
}

void CUserData::AddHoldTitle(UINT nPower, BOOL bUpdate, IDatabase* pDb)
{
// 	__int64 i64OldTitle = NULL;
// 	if (pDb)
// 	{
// 		char szSQL[128];
// 		sprintf(szSQL, "SELECT hold_title FROM %s WHERE id = %d LIMIT 1", _TBL_USER ,GetID());
// 		IRecordset* pRes = pDb->CreateNewRecordset(szSQL, false);
// 		if (pRes)
// 		{
// 			i64OldTitle = pRes->GetInt(0);
// 			pRes->Release();
// 		}
// 	}
// 	if (i64OldTitle != NULL && i64OldTitle != m_Info.i64HoldTitle)
// 	{
// 		m_Info.i64HoldTitle = i64OldTitle;
// 	}
// 	
// 	UINT iIDH = ((nPower / 100) - 1) * 4;
// 	UINT iIDL= nPower % 100;
// 	UINT iOld = (m_Info.i64HoldTitle >> iIDH) & 0xF;
// 
// 	
// 	i64OldTitle = static_cast<__int64>(0xF) << iIDH;
// 	i64OldTitle ^= 0xFFFFFFFFFFFFFFFF;
// 	i64OldTitle &= m_Info.i64HoldTitle;
// 	iIDL = _MIN((int)(iOld + iIDL), 15);
// 	i64OldTitle |= static_cast<__int64>(iIDL) << iIDH;
// 	m_Info.i64HoldTitle = i64OldTitle;
// 	
// 	if (m_pRes)
// 	{
// 		m_pRes->SetInt64("hold_title", m_Info.i64HoldTitle);
// 		if (bUpdate)
// 		{
// 			SaveInfo();
// 		}
// 	}
}

void CUserData::SubHoldTitle(UINT nPower, BOOL bUpdate, IDatabase* pDb)
{
// 	__int64 i64OldTitle = NULL;
// 	if (pDb)
// 	{
// 		char szSQL[128];
// 		sprintf(szSQL, "SELECT hold_title FROM %s WHERE id = %d LIMIT 1", _TBL_USER ,GetID());
// 		IRecordset* pRes = pDb->CreateNewRecordset(szSQL, false);
// 		if (pRes)
// 		{
// 			i64OldTitle = pRes->GetInt(0);
// 			pRes->Release();
// 		}
// 	}
// 	if (i64OldTitle != NULL && i64OldTitle != m_Info.i64HoldTitle)
// 	{
// 		m_Info.i64HoldTitle = i64OldTitle;
// 	}
// 	
// 	UINT iIDH = ((nPower / 100) - 1) * 4;
// 	UINT iIDL= nPower % 100;
// 	UINT iOld = (m_Info.i64HoldTitle >> iIDH) & 0xF;
// 	
// 	
// 	i64OldTitle = static_cast<__int64>(0xF) << iIDH;
// 	i64OldTitle ^= 0xFFFFFFFFFFFFFFFF;
// 	i64OldTitle &= m_Info.i64HoldTitle;
// 	iIDL = _MAX((int)(iOld - iIDL), 0);
// 	i64OldTitle |= static_cast<__int64>(iIDL) << iIDH;
// 	m_Info.i64HoldTitle = i64OldTitle;
// 	
// 	if (m_pRes)
// 	{
// 		m_pRes->SetInt64("hold_title", m_Info.i64HoldTitle);
// 		if (bUpdate)
// 		{
// 			SaveInfo();
// 		}
// 	}
}

int CUserData::CmpHoldTitle(UINT nPower)
{
	return 0;
// 	UINT iIDH = ((nPower / 100) - 1) * 4;
// 	UINT iIDL= nPower % 100;
// 	UINT iOld = (m_Info.i64HoldTitle >> iIDH) & 0xF;
// 	
// 	return iOld - iIDL;
}
//[游途道标 2008/10/13]设置当前使用的称号
void CUserData::SetUseTitle(int nPower, BOOL bUpdate)
{
	if(m_Info.iUseTitle == nPower)
		return;

	m_Info.iUseTitle = nPower;
	if (m_pRes)
	{
		m_pRes->SetDWord("use_title", m_Info.iUseTitle);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
} 

// void CUserData::SetFishExp(DWORD dwVal, BOOL bUpdate /* = TRUE */)
// {
// 	if(dwVal > 150)
// 		dwVal = 150;
// 
// 	m_Info.fishExp = dwVal;
// 	if( m_pRes )
// 	{
// 		m_pRes->SetDWord( "fishexp",dwVal );
// 		if( bUpdate )
// 			SaveInfo();
// 	}
// }

// void CUserData::SetHouseStor1(DWORD dwVal, BOOL bUpdate /* = TRUE */)
// {
// 	m_Info.houseStor1 = dwVal;
// 
// 	if(m_pRes)
// 	{
// 		m_pRes->SetDWord("housestore1", dwVal);
// 		if(bUpdate)
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetHouseStor2(DWORD dwVal, BOOL bUpdate /* = TRUE */)
// {
// 	m_Info.houseStor2 = dwVal;
// 
// 	if(m_pRes)
// 	{
// 		m_pRes->SetDWord("housestore2", dwVal);
// 		if(bUpdate)
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetHouseStor3(DWORD dwVal, BOOL bUpdate /* = TRUE */)
// {
// 	m_Info.houseStor3 = dwVal;
// 
// 	if(m_pRes)
// 	{
// 		m_pRes->SetDWord("housestore3", dwVal);
// 		if(bUpdate)
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetHouseStor4(DWORD dwVal, BOOL bUpdate /* = TRUE */)
// {
// 	m_Info.houseStor4 = dwVal;
// 
// 	if(m_pRes)
// 	{
// 		m_pRes->SetDWord("housestore4", dwVal);
// 		if(bUpdate)
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetHouseStor5(DWORD dwVal, BOOL bUpdate /* = TRUE */)
// {
// 	m_Info.houseStor5 = dwVal;
// 
// 	if(m_pRes)
// 	{
// 		m_pRes->SetDWord("housestore5", dwVal);
// 		if(bUpdate)
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetHouseStor6(DWORD dwVal, BOOL bUpdate /* = TRUE */)
// {
// 	m_Info.houseStor6 = dwVal;
// 
// 	if(m_pRes)
// 	{
// 		m_pRes->SetDWord("housestore6", dwVal);
// 		if(bUpdate)
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetHouseStor7(DWORD dwVal, BOOL bUpdate /* = TRUE */)
// {
// 	m_Info.houseStor7 = dwVal;
// 
// 	if(m_pRes)
// 	{
// 		m_pRes->SetDWord("housestore7", dwVal);
// 		if(bUpdate)
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetHouseStor8(DWORD dwVal, BOOL bUpdate /* = TRUE */)
// {
// 	m_Info.houseStor8 = dwVal;
// 
// 	if(m_pRes)
// 	{
// 		m_pRes->SetDWord("housestore8", dwVal);
// 		if(bUpdate)
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetHouseStor9(DWORD dwVal, BOOL bUpdate /* = TRUE */)
// {
// 	m_Info.houseStor9 = dwVal;
// 
// 	if(m_pRes)
// 	{
// 		m_pRes->SetDWord("housestore9", dwVal);
// 		if(bUpdate)
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetStockNum(DWORD dwVal,BOOL bUpdate /* = TRUE */)
// {
// 	m_Info.StockNum = dwVal;
// 	if(m_pRes)
// 	{
// 		m_pRes->SetDWord("StockNum",dwVal);
// 		if(bUpdate)
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetArenaRating(UINT nVal,BOOL bUpdate /* = TRUE */)
// {
// 	m_Info.nArenaRating = nVal;
// 	if(m_pRes)
// 	{
// 		m_pRes->SetDWord("ArenaRating",nVal);
// 		if(bUpdate)
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetPassError(UINT data, BOOL update /* = TRUE */)
// {
// 	m_Info.nPassError = data;
// 	if(m_pRes)
// 	{
// 		m_pRes->SetDWord("pass_error",data);
// 		if(update)
// 			SaveInfo();
// 	}
// }
// 
// void CUserData::SetMonsterNum(int nTargetNum, BOOL bUpdate /* = true  */)
// {
// 	m_Info.m_nKillMonsterTargetNum = nTargetNum;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetDWord("monsternum",nTargetNum);
// 		if (bUpdate)
// 		{
// 			SaveInfo();
// 		}
// 	}
// }
// 
// void CUserData::SetMonsterTimeInterval(int nTimeInterval, BOOL bUpdate /* = true  */)
// {
// 	m_Info.m_nKillMonsterTimeInterval = nTimeInterval;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetDWord("timeinterval",nTimeInterval);
// 		if(bUpdate)
// 		{
//           SaveInfo();
// 		}
// 	}
// }

void CUserData::SetPkProc(int data,BOOL bUpdate /* = true */)
{
	if(m_Info.ucPkMode == data)
		return;

	m_Info.ucPkMode = data;
	if(m_pRes)
	{
		m_pRes->SetDWord("Protect_PK",m_Info.ucPkMode);
		if(bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetVipTime(DWORD dwMin)
{
	if(m_Info.vipTime == dwMin)
		return;

	m_Info.vipTime = dwMin;
	if(m_pRes)
	{	
		m_pRes->SetDWord("viptime",m_Info.vipTime);
		m_bNeedUpdate = true;
	}
}

void CUserData::SetVipLev(DWORD dwMin)
{
	if(m_Info.vipLev == dwMin)
		return;

	m_Info.vipLev = dwMin;
	if(m_pRes)
	{	
		m_pRes->SetDWord("viplev",m_Info.vipLev);
		m_bNeedUpdate = true;
	}
}

void CUserData::ActiveVipTime()
{
	if(m_Info.vipState == 1)
		return;

	m_Info.vipState = 1;
	if(m_pRes)
	{	
		m_pRes->SetDWord("vipstate",m_Info.vipState);
		m_bNeedUpdate = true;
	}
}

void CUserData::ClrVipTime()
{
	if(m_Info.vipState == 0)
		return;

	m_Info.vipState = 0;
	if(m_pRes)
	{	
		m_pRes->SetDWord("vipstate",m_Info.vipState);
		m_bNeedUpdate = true;
	}
}
void CUserData::SetHpAdd(DWORD dwHpAdd, bool bUpdate)
{
	if(m_Info.dwHpAdd == dwHpAdd)
		return;

	m_Info.dwHpAdd = dwHpAdd;
	if (m_pRes)
	{
		m_pRes->SetDWord("hp_add", m_Info.dwHpAdd);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}
void CUserData::SetMpAdd(DWORD dwMpAdd, bool bUpdate)
{
	if(m_Info.dwMpAdd == dwMpAdd)
		return;

	m_Info.dwMpAdd = dwMpAdd;
	if (m_pRes)
	{
		m_pRes->SetDWord("mp_add", m_Info.dwMpAdd);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}
void CUserData::SetPetAdd(DWORD dwPetAdd, bool bUpdate)
{
	if(m_Info.dwPetAdd == dwPetAdd)
		return;

	m_Info.dwPetAdd = dwPetAdd;
	if (m_pRes)
	{
		m_pRes->SetDWord("pet_mp_add", m_Info.dwPetAdd);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}
// void CUserData::SetUpLevelTime(UINT nTime,BOOL bUpdate /*= true*/)
// {
// 	m_Info.nUpLevelTime = nTime;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetDWord("upleveltime",nTime);
// 		if(bUpdate)
// 		{
//           SaveInfo();
// 		}
// 	}
// }


// void  CUserData::SetGiftReset(UINT nVal,BOOL bUpdata /*= TRUE*/)
// {
// 	m_Info.giftReset = nVal;
// 	if (m_pRes)
// 	{
// 		m_pRes->SetDWord("gift_reset",nVal);
// 		if(bUpdata)
// 		{
// 			SaveInfo();
// 		}
// 	}
// }
// 
// void CUserData::SetFishTired(int nVal, BOOL bUpdate /* = TRUE */)
// {
// 	m_Info.nFishTired = nVal;
// 	if(m_pRes)
// 	{
// 		m_pRes->SetDWord("fishtired", nVal);
// 		if(bUpdate)
// 		{
// 			SaveInfo();
// 		}
// 	}
// }
// 
// //-------赠送物品记录
// void       CUserData::SetAwardItemTime(int nValu,BOOL bUpdate)
// {
// 	m_Info.nAwardItemTime = nValu;
// 	if(bUpdate==FALSE)
// 		return ;
// 	if(m_pRes)
// 	{
// 		m_pRes->SetDWord("award_Item_time",nValu);
// 		SaveInfo();
// 	
// 	}
// 	
// }

void CUserData::SetOnlineAward(int ntime,bool bUpdate)
{
	if(m_Info.onlineaward == ntime)
		return;

	if(ntime < 0 || ntime > 18||ntime==9)
		m_Info.onlineaward=8;

	m_Info.onlineaward=ntime;

	if (m_pRes)
	{
		m_pRes->SetDWord("award", m_Info.onlineaward);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

DWORD CUserData::GetAwardTime()
{
	return m_Info.onlineaward;
}

void CUserData::setFati(int iFatigue)
{
	m_Info.Fatigue = iFatigue;
}

int CUserData::getFati()
{ 
	return m_Info.Fatigue;
}

void CUserData::SetFatiTime(int FatiTime, bool bUpdate /*= false*/)
{
	if(m_Info.FatiTime == FatiTime)
		return;

	m_Info.FatiTime = FatiTime;
	if (m_pRes)
	{
		m_pRes->SetUInt("fatitime", m_Info.FatiTime);
		if(bUpdate == true)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}

	return;
}

void CUserData::SetItemLockPwd(char* szPwd, bool bUpdate/*=false*/)
{
	if( !szPwd || strlen(szPwd) >= 24 )
		return;
	if(strcmp(m_Info.szItemLockPwd,szPwd) == 0)
		return;

	strcpy( m_Info.szItemLockPwd,szPwd );
	if( m_pRes )
	{
		m_pRes->SetString( "lockpwd", m_Info.szItemLockPwd, 24);
		if( bUpdate )
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetVipface(int nData)
{
	if(m_Info.Vipface == nData)
		return;

	m_Info.Vipface = nData;
	if (m_pRes)
	{
		m_pRes->SetDWord( "vipface", nData );
		m_bNeedUpdate = true;
	}
}

void CUserData::SetVipfaceTime(OBJID nTime,bool bUpdate /*= false*/)
{
	if(m_Info.VipfaceTime == nTime)
		return;

	m_Info.VipfaceTime = nTime;
	if (m_pRes)
	{
		m_pRes->SetDWord( "vipfacetime", nTime );
		if( bUpdate )
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}

}

OBJID CUserData::GetVipfaceTime()
{
	return m_Info.VipfaceTime;
}

void CUserData::SetFati(int nData)
{
	m_Info.Fatigue = nData;
}

void CUserData::SetMonsterkill	(DWORD nMonsterkill, BOOL bUpdate/* = false*/)
{
	if(m_Info.MonsterKill == nMonsterkill)
		return;

	m_Info.MonsterKill = nMonsterkill;
	if (m_pRes)
	{
		m_pRes->SetDWord( "KillMonseters", nMonsterkill );
		if(bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetEverydayOL	(int nTime, BOOL bUpdate/* = false*/)
{
	if(m_Info.EverydayOL == nTime)
		return;

	m_Info.EverydayOL = nTime;
	if (m_pRes)
	{
		m_pRes->SetDWord( "EverydayOL", nTime );
		if(bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

DWORD CUserData::GetSynCon()
{
	return m_Info.SynCon;
}

void CUserData::SetSynCon(int nData, int bUpdate)
{
	if(m_Info.SynCon == nData)
		return;

	m_Info.SynCon	= nData;
	if (m_pRes)
	{
		m_pRes->SetDWord("Syn_Con", m_Info.SynCon);
		if(bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

_int64 CUserData::GetASales()
{
	return m_Info.i64BuyASales;
}

void CUserData::SetASales(_int64 nData, bool bUpdate)
{
	if(m_Info.i64BuyASales == nData)
		return;
	
	m_Info.i64BuyASales	= nData;
	if (m_pRes)
	{
		m_pRes->SetInt64("buyvasitem", m_Info.i64BuyASales);
		if(bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

void CUserData::SetJinmeiValue(int nData, bool bUpdate /* = false */)
{
	if (m_Info.nJinmeiValue == nData)
		return;
    
	m_Info.nJinmeiValue	= nData;
	if (m_pRes)
	{
		m_pRes->SetUInt("jinmeivalue", m_Info.nJinmeiValue);
		if (bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}

int CUserData::GetJinmeiValue()
{
	return m_Info.nJinmeiValue;
}

int CUserData::GetMorality()//师德
{
	return m_Info.nMorality;
}
void CUserData::SetMorality(int nData, bool bUpdate)
{
	if (m_Info.nMorality == nData)
		return;

	m_Info.nMorality = nData;
	if(m_pRes)
	{
		m_pRes->SetInt("TutorMorality", m_Info.nMorality);
		if(bUpdate)
			SaveInfo();
		else
			m_bNeedUpdate = true;
	}
}
