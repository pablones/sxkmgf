// StatusManager.cpp: implementation of the CStatusManager class.
// 
//////////////////////////////////////////////////////////////////////
//#include "..\stdafx.h"
//#include "Mapgroup.h"
//#include "../../GameBaseCodeMT/GameData.h"
//#include "GameObj.h"
//#include <afxwin.h>         // MFC core and standard components
//#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls

#include "MAPGROUP.h"
#include "StatusManager.h"
#include "MsgPlayer.h"
#include "MsgUserAttrib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//class CMyDatabase;
//extern CMyDatabase		g_db;
//
CStatusManager::CStatusManager()
{
//	m_pRes=new CMyRecordset(&g_db);
}

CStatusManager::~CStatusManager()
{
//	if(m_pRes)
//	{
//		m_pRes->Close();
//		delete m_pRes;
//		m_pRes=NULL;
//	}

}

bool CStatusManager::RestoreStatusDB(CUser* pRole,IDatabase* pDB)///从数据库内恢复角色状态
{
	CString m_strSQL;
	m_strSQL.Format("select * from %s where USER_ID=%d",_TBL_USERSTATUS,pRole->GetID());
	m_strSQL.MakeUpper();
	IRecordset* pNewRecordset=pDB->CreateNewRecordset(m_strSQL);
	if (pNewRecordset)
	{
		DWORD Status_ID=0,Status_Power=0,nTimes=0,Param1=0,Param2=0,Param3=0;
		int nSecs=0;
		for (int i=0;i<pNewRecordset->RecordCount();++i)
		{
			Status_ID=pNewRecordset->LoadInt("Status_ID");///读取状态
			Status_Power=pNewRecordset->LoadInt("Status_Power");///状态主参
			nSecs=pNewRecordset->LoadInt("RestTime");///剩余时间,秒
			nTimes=pNewRecordset->LoadInt("Status_nTimes");///剩余次数
			Param1=pNewRecordset->LoadInt("Param1");///剩余时间,毫秒
			Param2=pNewRecordset->LoadInt("Param2");///用于判断生效地图
			Param3=pNewRecordset->LoadInt("Param3");
			if (Param3==1)//冻结的状态
			{
				pNewRecordset->MoveNext();
				continue;
			}
			if (nTimes || nSecs)///判断参数都不为0时才加buffer
			{
				// 网上的例子。 [6/28/2007]
				/*
				使用：COleDateTime start_time = COleDateTime::GetCurrentTime();
				COleDateTimeSpan end_time = COleDateTime::GetCurrentTime()-start_time;
				While(end_time.GetTotalSeconds() < 2)
				{
					// 处理延时或定时期间能处理其他的消息
					DoSomething()
					end_time = COleDateTime::GetCurrentTime-start_time;
				}
				*/
				// 彭峰添加下线计时的新品种 [6/28/2007]
				if( IsDownLineGoOnStatus( Status_ID ) )
				{
					CString LastRecordTime;
					double nInterval;
					pNewRecordset->LoadString( LastRecordTime.GetBuffer(0), "LastRecordTime", LastRecordTime.GetLength() );//得到当时写的时间
					if( LastRecordTime == "" )
					{
						pNewRecordset->MoveNext();
						continue;
					}
					COleDateTime lastDate;
					COleDateTimeSpan newDate;
					lastDate.ParseDateTime( LastRecordTime );
					newDate = COleDateTime::GetCurrentTime() - lastDate;
					nInterval = newDate.GetTotalSeconds();
					IF_OK( nInterval > -60 )//goto 0改成60,不同服务器间时间有误差
					{
						if (nTimes>0)//多次效果的buff
						{
							int nLeftSecs = nSecs*nTimes - nInterval;//剩余时间等于效果次数乘以效果间隔时间
							int nLeftTimes = 0;
							if (nLeftSecs>0)
								nLeftTimes = __max(nLeftSecs / nSecs, 1);
							if (nLeftTimes>0)
								pRole->AttachStatus(pRole,Status_ID,Status_Power,nSecs,nLeftTimes,Param1,Param2);
						}
						else
						{
							if (nInterval>0)
								nSecs = nSecs - nInterval;
							if( nSecs > 0/* || Status_ID == 30000 */)
							{
								pRole->AttachStatus(pRole,Status_ID,Status_Power,nSecs,nTimes,Param1,Param2);
							}
						}
					}
				}
				else
				{
// 					if(12015 != Status_ID)
// 					{
						pRole->AttachStatus(pRole,Status_ID,Status_Power,nSecs,nTimes,Param1,Param2);
//					}
// 					if (Status_ID == 30000)
// 					{
// 						pRole->SetMount();
// 						CMsgPlayer msgPlayer;
// 						if (msgPlayer.Create(pRole->QueryRole()))
// 							pRole->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
// 						
// 						pRole->AttachStatus(pRole,Status_ID,Status_Power,-1,nTimes,Param1);
// 
// 						pRole->SetAttrib(_USERATTRIB_SPEED, pRole->AdjustSpeed(pRole->GetSpeed()), SYNCHRO_TRUE);
// 					}
				}
			}
				//pRole->AddBuff(Status_ID,nSecs,Status_Power,Param1);
			if (Status_ID!=STATUS_GETEXPUP_PERCENT && Status_ID!=STATUS_GETEXPUP_PERCENT_WT && Status_ID!=STATUS_GETEXPUP_PERCENT_SYNINS)//双倍的不删
				pNewRecordset->DeleteRecord();///删除已恢复的状态
			pNewRecordset->MoveNext();
		}
		pNewRecordset->Release();
	}
	return true;
}

BOOL CStatusManager::WriteStatusDB(IRole* pRole,IDatabase* pDB,IStatus* pStatus)///写入数据库
{
	CString m_strSQL;
	StatusInfoStruct statusinfo={0};
	pStatus->GetInfo(&statusinfo);
	OBJID idRole=pRole->GetID();
	UINT  idStatus=statusinfo.nStatus;
	m_strSQL.Format("delete from %s where USER_ID=%d and Status_ID=%d and Param3!=1",_TBL_USERSTATUS,idRole,idStatus);	
	m_strSQL.MakeUpper();
	pDB->ExecuteSQL((LPSTR)(LPCSTR)m_strSQL);
	m_strSQL.Format("insert %s set USER_ID=%d,Status_ID=%d,Status_Power=%d,RestTime=%d,LastRecordTime='%s',Status_nTimes=%d,Param1=%d,Param2=%d,Param3=%d",
		_TBL_USERSTATUS,idRole,idStatus,statusinfo.nPower,statusinfo.nSecs,COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),statusinfo.nTimes,statusinfo.nParam1,statusinfo.nParam2,statusinfo.nParam3);
	m_strSQL.MakeUpper();
	return pDB->ExecuteSQL((LPSTR)(LPCSTR)m_strSQL);
}

BOOL CStatusManager::IsShutup(CUser* pRole)///该用户是否被禁止发言
{
	IStatus* pStatusSet=pRole->QueryStatusSet()->GetObj( STATUS_STOPSAY );
	return pStatusSet!=NULL;
}

BOOL CStatusManager::IsValueStatus(IStatus* pStatus)///判断该状态是否需要写入数据库
{
	DWORD idStatus=pStatus->GetID();
	BOOL bResult=true;
	switch(idStatus)
	{
		case STATUS_PK_PROTECT:///PK保护状态不保存
			bResult=false;
			break;
		case STATUS_PHYATTACKUP_TUTOR_PERCENT:// 师傅加成BUFF pengfeng [10/27/2007]
			bResult=false;
			break;
		case STATUS_MAGATTACKUP_TUTOR_PERCENT://  [陈剑飞10/27/2007]
			bResult=false;
			break;
		case STATUS_NEW_PK_PROTECT:	//  [2/15/2008 朱斌]	新PK保护
			bResult = FALSE;
			break;

		case STATUS_CITYWAR_WEAPON :        //城战相关状态不保存
		case STATUS_CITYWAR_SAFETIME :             
		case STATUS_CITYWAR_ATTACKMARK :            
		case STATUS_CITYWAR_DEFENCEMARK : 
			bResult=false;
			break;

		case STATUS_PVPMODE :      //决斗相关buff      
		case STATUS_BIGBLOOD_USERNOT : 
			bResult=false;
			break;

		case STATUS_YABIAO :         
		case STATUS_JIEBIAO : 
			bResult=false;
			break;
		case STATUS_MOUNTSPEED_NUMBER://坐骑状态不保存
			bResult=false;
			break;
		case STATUS_BOOTH_ADDEXP://摆摊的加经验BUFF不保存
			bResult=false;
			break;
//		case STATUS_SYNWORKTIME://打工的时间记录
	//		bResult=false;
		//	break;
		case STATUS_SOUL_CRI://暴击			武魂
		case STATUS_SOUL_HITRATE://命中			武魂
		case STATUS_SOUL_DODGE://闪避			武魂
		case STATUS_SOUL_INNERATT://内功			武魂
		case STATUS_SOUL_OUTERATT://外功			武魂
		case STATUS_SOUL_INNERDEF://内防			武魂
		case STATUS_SOUL_OUTERDEF://外防			武魂
		case STATUS_SOUL_REBLOOD://回血			武魂
		case STATUS_SOUL_REBLOOD_PER:
		case STATUS_SOUL_REMP://回魔         状态下线不保存
			 bResult=false;
			 break;
//modify code by 林德渊 begin 2011.6.22
		case STATUS_RETORT_TENSEC:
		case STATUS_RETORT_LGWS:
		case STATUS_RETORT_ZHF:
//		case STATUS_RETORT_MDSX:
//		case STATUS_RETORT_MDSX_BUFF2:
		case STATUS_RETORT_MDSX_1:
			bResult=false;
			break;
//modify code by 林德渊 end
		case STATUS_WARGAME://战争buff
		case STATUS_DOT_WARGAMEEXP:
			bResult = false;
			break;
		default:
			bResult=true;
	}
	if (pStatus->GetSecLeft()<=60 && idStatus!=STATUS_GETEXPUP_PERCENT && idStatus!=STATUS_GETEXPUP_PERCENT_WT && idStatus != STATUS_GETEXPUP_PERCENT_SYNINS)//剩余时间不到一分钟的状态不记录
		bResult=false;

//modify code by 林德渊 begin 2011.6.22
	if( idStatus == STATUS_RETORT_MDSX || idStatus == STATUS_RETORT_MDSX_BUFF2 )
		bResult = true;
//modify code by 林德渊 end

	if (idStatus/100 == 127)//宠物buf不保存
		bResult=false;
	return bResult;
}

BOOL CStatusManager::IsDownLineGoOnStatus( UINT nStatusID )// 是否为下线以后也继续计时 彭峰 [6/28/2007]
{
	if (STATUS_GETEXPUP_PERCENT==nStatusID || STATUS_GETEXPUP_PERCENT_WT==nStatusID ||STATUS_VERIFICATION_RUNNING==nStatusID || STATUS_VERIFICATION_WAIT==nStatusID || STATUS_GETEXPUP_PERCENT_SYNINS == nStatusID)//双倍经验下线不计时
		return false;
	else
		return true;
// 	if( 11018 == nStatusID 
// 		|| nStatusID == STATUS_WARGAME)	//  [2007-10-20] 朱斌	战争活动状态下线计时
// 	{
// 		return true;
// 	}
// 	else
// 	{
// 		return false;
// 	}
}

BOOL CStatusManager::CheckRestoreFreezeStatusDB(CUser* pRole,IDatabase* pDB, UINT uStateID)///从数据库内恢复冻结的角色状态 by goto
{
	CString m_strSQL;
	m_strSQL.Format("select * from %s where USER_ID=%d and Param3=1 and Status_ID=%u",_TBL_USERSTATUS,pRole->GetID(),uStateID);
	m_strSQL.MakeUpper();
	IRecordset* pNewRecordset=pDB->CreateNewRecordset(m_strSQL);
	if (pNewRecordset)
	{
		pNewRecordset->Release();
		return false;
	}
	return true;
}

BOOL CStatusManager::RestoreFreezeStatusDB(CUser* pRole,IDatabase* pDB, UINT uStateID)///从数据库内恢复冻结的角色状态 by goto
{
	CString m_strSQL;
	m_strSQL.Format("select * from %s where USER_ID=%d and Param3=1 and Status_ID=%u",_TBL_USERSTATUS,pRole->GetID(),uStateID);
	m_strSQL.MakeUpper();
	IRecordset* pNewRecordset=pDB->CreateNewRecordset(m_strSQL);
	if (pNewRecordset)
	{
		DWORD Status_ID=0,Status_Power=0,nTimes=0,Param1=0;
		int nSecs=0;
		for (int i=0;i<pNewRecordset->RecordCount();++i)
		{
			Status_ID=pNewRecordset->LoadInt("Status_ID");///读取状态
			Status_Power=pNewRecordset->LoadInt("Status_Power");///状态主参
			nSecs=pNewRecordset->LoadInt("RestTime");///剩余时间,秒
			nTimes=pNewRecordset->LoadInt("Status_nTimes");///剩余次数
			Param1=pNewRecordset->LoadInt("Param1");///剩余时间,毫秒
			if (nTimes || nSecs)///判断参数都不为0时才加buffer
			{
				if (!pRole->AttachStatus(pRole,Status_ID,Status_Power,nSecs,nTimes,Param1))
				{
					SAFE_RELEASE(pNewRecordset);
					return false;
				}
			}
			pNewRecordset->DeleteRecord();///删除已恢复的状态
			pNewRecordset->MoveNext();
		}
		pNewRecordset->Release();
		return true;
	}
	pRole->SendSysMsg(_TXTATR_TOPSHOW, "您没有冻结的此类状态");
	return false;
}
