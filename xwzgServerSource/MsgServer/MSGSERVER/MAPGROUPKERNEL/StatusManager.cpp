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

bool CStatusManager::RestoreStatusDB(CUser* pRole,IDatabase* pDB)///�����ݿ��ڻָ���ɫ״̬
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
			Status_ID=pNewRecordset->LoadInt("Status_ID");///��ȡ״̬
			Status_Power=pNewRecordset->LoadInt("Status_Power");///״̬����
			nSecs=pNewRecordset->LoadInt("RestTime");///ʣ��ʱ��,��
			nTimes=pNewRecordset->LoadInt("Status_nTimes");///ʣ�����
			Param1=pNewRecordset->LoadInt("Param1");///ʣ��ʱ��,����
			Param2=pNewRecordset->LoadInt("Param2");///�����ж���Ч��ͼ
			Param3=pNewRecordset->LoadInt("Param3");
			if (Param3==1)//�����״̬
			{
				pNewRecordset->MoveNext();
				continue;
			}
			if (nTimes || nSecs)///�жϲ�������Ϊ0ʱ�ż�buffer
			{
				// ���ϵ����ӡ� [6/28/2007]
				/*
				ʹ�ã�COleDateTime start_time = COleDateTime::GetCurrentTime();
				COleDateTimeSpan end_time = COleDateTime::GetCurrentTime()-start_time;
				While(end_time.GetTotalSeconds() < 2)
				{
					// ������ʱ��ʱ�ڼ��ܴ�����������Ϣ
					DoSomething()
					end_time = COleDateTime::GetCurrentTime-start_time;
				}
				*/
				// ���������߼�ʱ����Ʒ�� [6/28/2007]
				if( IsDownLineGoOnStatus( Status_ID ) )
				{
					CString LastRecordTime;
					double nInterval;
					pNewRecordset->LoadString( LastRecordTime.GetBuffer(0), "LastRecordTime", LastRecordTime.GetLength() );//�õ���ʱд��ʱ��
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
					IF_OK( nInterval > -60 )//goto 0�ĳ�60,��ͬ��������ʱ�������
					{
						if (nTimes>0)//���Ч����buff
						{
							int nLeftSecs = nSecs*nTimes - nInterval;//ʣ��ʱ�����Ч����������Ч�����ʱ��
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
			if (Status_ID!=STATUS_GETEXPUP_PERCENT && Status_ID!=STATUS_GETEXPUP_PERCENT_WT && Status_ID!=STATUS_GETEXPUP_PERCENT_SYNINS)//˫���Ĳ�ɾ
				pNewRecordset->DeleteRecord();///ɾ���ѻָ���״̬
			pNewRecordset->MoveNext();
		}
		pNewRecordset->Release();
	}
	return true;
}

BOOL CStatusManager::WriteStatusDB(IRole* pRole,IDatabase* pDB,IStatus* pStatus)///д�����ݿ�
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

BOOL CStatusManager::IsShutup(CUser* pRole)///���û��Ƿ񱻽�ֹ����
{
	IStatus* pStatusSet=pRole->QueryStatusSet()->GetObj( STATUS_STOPSAY );
	return pStatusSet!=NULL;
}

BOOL CStatusManager::IsValueStatus(IStatus* pStatus)///�жϸ�״̬�Ƿ���Ҫд�����ݿ�
{
	DWORD idStatus=pStatus->GetID();
	BOOL bResult=true;
	switch(idStatus)
	{
		case STATUS_PK_PROTECT:///PK����״̬������
			bResult=false;
			break;
		case STATUS_PHYATTACKUP_TUTOR_PERCENT:// ʦ���ӳ�BUFF pengfeng [10/27/2007]
			bResult=false;
			break;
		case STATUS_MAGATTACKUP_TUTOR_PERCENT://  [�½���10/27/2007]
			bResult=false;
			break;
		case STATUS_NEW_PK_PROTECT:	//  [2/15/2008 ���]	��PK����
			bResult = FALSE;
			break;

		case STATUS_CITYWAR_WEAPON :        //��ս���״̬������
		case STATUS_CITYWAR_SAFETIME :             
		case STATUS_CITYWAR_ATTACKMARK :            
		case STATUS_CITYWAR_DEFENCEMARK : 
			bResult=false;
			break;

		case STATUS_PVPMODE :      //�������buff      
		case STATUS_BIGBLOOD_USERNOT : 
			bResult=false;
			break;

		case STATUS_YABIAO :         
		case STATUS_JIEBIAO : 
			bResult=false;
			break;
		case STATUS_MOUNTSPEED_NUMBER://����״̬������
			bResult=false;
			break;
		case STATUS_BOOTH_ADDEXP://��̯�ļӾ���BUFF������
			bResult=false;
			break;
//		case STATUS_SYNWORKTIME://�򹤵�ʱ���¼
	//		bResult=false;
		//	break;
		case STATUS_SOUL_CRI://����			���
		case STATUS_SOUL_HITRATE://����			���
		case STATUS_SOUL_DODGE://����			���
		case STATUS_SOUL_INNERATT://�ڹ�			���
		case STATUS_SOUL_OUTERATT://�⹦			���
		case STATUS_SOUL_INNERDEF://�ڷ�			���
		case STATUS_SOUL_OUTERDEF://���			���
		case STATUS_SOUL_REBLOOD://��Ѫ			���
		case STATUS_SOUL_REBLOOD_PER:
		case STATUS_SOUL_REMP://��ħ         ״̬���߲�����
			 bResult=false;
			 break;
//modify code by �ֵ�Ԩ begin 2011.6.22
		case STATUS_RETORT_TENSEC:
		case STATUS_RETORT_LGWS:
		case STATUS_RETORT_ZHF:
//		case STATUS_RETORT_MDSX:
//		case STATUS_RETORT_MDSX_BUFF2:
		case STATUS_RETORT_MDSX_1:
			bResult=false;
			break;
//modify code by �ֵ�Ԩ end
		case STATUS_WARGAME://ս��buff
		case STATUS_DOT_WARGAMEEXP:
			bResult = false;
			break;
		default:
			bResult=true;
	}
	if (pStatus->GetSecLeft()<=60 && idStatus!=STATUS_GETEXPUP_PERCENT && idStatus!=STATUS_GETEXPUP_PERCENT_WT && idStatus != STATUS_GETEXPUP_PERCENT_SYNINS)//ʣ��ʱ�䲻��һ���ӵ�״̬����¼
		bResult=false;

//modify code by �ֵ�Ԩ begin 2011.6.22
	if( idStatus == STATUS_RETORT_MDSX || idStatus == STATUS_RETORT_MDSX_BUFF2 )
		bResult = true;
//modify code by �ֵ�Ԩ end

	if (idStatus/100 == 127)//����buf������
		bResult=false;
	return bResult;
}

BOOL CStatusManager::IsDownLineGoOnStatus( UINT nStatusID )// �Ƿ�Ϊ�����Ժ�Ҳ������ʱ ��� [6/28/2007]
{
	if (STATUS_GETEXPUP_PERCENT==nStatusID || STATUS_GETEXPUP_PERCENT_WT==nStatusID ||STATUS_VERIFICATION_RUNNING==nStatusID || STATUS_VERIFICATION_WAIT==nStatusID || STATUS_GETEXPUP_PERCENT_SYNINS == nStatusID)//˫���������߲���ʱ
		return false;
	else
		return true;
// 	if( 11018 == nStatusID 
// 		|| nStatusID == STATUS_WARGAME)	//  [2007-10-20] ���	ս���״̬���߼�ʱ
// 	{
// 		return true;
// 	}
// 	else
// 	{
// 		return false;
// 	}
}

BOOL CStatusManager::CheckRestoreFreezeStatusDB(CUser* pRole,IDatabase* pDB, UINT uStateID)///�����ݿ��ڻָ�����Ľ�ɫ״̬ by goto
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

BOOL CStatusManager::RestoreFreezeStatusDB(CUser* pRole,IDatabase* pDB, UINT uStateID)///�����ݿ��ڻָ�����Ľ�ɫ״̬ by goto
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
			Status_ID=pNewRecordset->LoadInt("Status_ID");///��ȡ״̬
			Status_Power=pNewRecordset->LoadInt("Status_Power");///״̬����
			nSecs=pNewRecordset->LoadInt("RestTime");///ʣ��ʱ��,��
			nTimes=pNewRecordset->LoadInt("Status_nTimes");///ʣ�����
			Param1=pNewRecordset->LoadInt("Param1");///ʣ��ʱ��,����
			if (nTimes || nSecs)///�жϲ�������Ϊ0ʱ�ż�buffer
			{
				if (!pRole->AttachStatus(pRole,Status_ID,Status_Power,nSecs,nTimes,Param1))
				{
					SAFE_RELEASE(pNewRecordset);
					return false;
				}
			}
			pNewRecordset->DeleteRecord();///ɾ���ѻָ���״̬
			pNewRecordset->MoveNext();
		}
		pNewRecordset->Release();
		return true;
	}
	pRole->SendSysMsg(_TXTATR_TOPSHOW, "��û�ж���Ĵ���״̬");
	return false;
}
