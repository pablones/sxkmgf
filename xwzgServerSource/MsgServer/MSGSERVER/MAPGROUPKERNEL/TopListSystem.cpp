////////////////////////////////////////////////////////////////////////////////////////////////////
//File:		TopListSystem.cpp
//Time:		2006.9.8
//Author:	����
//Mark:		���а�ϵͳ
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "TopListSystem.h"
#include "SKHonorMurderList.h"
#include "DataInspectManage.h"
#include "MAPGROUP.h"//20070530����
#include "AdjustRate.h"
#include "MsgTopList.h"
#include "VariantManager.h"
#include "MapGroup.h"
//#include "../SKPostOffice.h"
#include "SKPostOffice.h"
//#include "User.h"
//#include "../share/I_Shell.h"

CTopListSystem	g_TopListSystem;			//���а�ϵͳ,ȫ�ֶ���
/////////////////////////////////////////////////////////////////////////////////////////////////////
CTopListSystem::CTopListSystem()
{
	m_pDb = NULL;
	m_TopListUpMin = RandGet(60,true);

	//����ʱ�������˲�ѯ����
	m_UpdateCompleteEvent = CreateEvent( NULL,true,true,NULL );//��������̳�,�ֶ���λ,��ʼΪ��λ״̬,��ָ���¼���

	Init();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
CTopListSystem::~CTopListSystem()
{
	CloseHandle(m_UpdateCompleteEvent);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void CTopListSystem::Init()			//�������
{
	memset( &m_WorldTopList,0L,sizeof( TOPLIST ) );
	memset( &m_LastUpdateTime,0L,sizeof( SYSTEMTIME ) );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTopListSystem::CreateData( IDatabase* pDb )
{
	if( !pDb )
		return false;
	m_pDb = pDb;

	int i;
	char szSQL[ 1024 ];

	//�ȼ�����
	sprintf( szSQL, "SELECT u.name,u.level,s.syn_id FROM  %s as s RIGHT JOIN %s AS u on s.id = u.id WHERE u.id IS NOT NULL and u.identity <> 3 ORDER BY u.level DESC LIMIT 200",_TBL_SYNATTR,_TBL_USER);
	IRecordset* pRes = pDb->CreateNewRecordset( szSQL, false );
	if( pRes )
	{
// 		IRecordset* pResult;
		for( i = 0; i < pRes->RecordCount(); i++ )
		{
			strcpy( m_WorldTopList.LevelList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.LevelList[ i ].nLev		= pRes->GetInt( 1 );
			int nID = pRes->GetInt( 2 );
			CheckSynName(m_WorldTopList.LevelList[ i ].szSynName, nID, pDb);
// 			if(nID != ID_NONE)
// 			{
// 				sprintf(szSQL,"SELECT name,level FROM %s WHERE id = %d",
// 					_TBL_SYNDICATE,nID);	
// 				CAutoPtr<IRecordset> pResult = pDb->CreateNewRecordset( szSQL, false );
// 				if (pResult && (pResult->RecordCount() > 0))
// 				{
// 					if(pResult->GetStr(1) > 0)	//��������еȼ�
// 						strcpy(m_WorldTopList.LevelList[ i ].szSynName,pResult->GetStr(0));	
// 					else 
// 						strcpy(m_WorldTopList.LevelList[ i ].szSynName,"δ������");
// // 					pResult->Release();
// 					
// 				}
// 				else
// 					strcpy(m_WorldTopList.LevelList[ i ].szSynName,"δ������");
// 			}
// 			else
//				strcpy(m_WorldTopList.LevelList[ i ].szSynName,"δ������");
			pRes->MoveNext();
		}
		pRes->Release();  
	}

	//������
	//sprintf( szSQL, "SELECT u.name,u.Gold1,s.syn_id FROM %s as s RIGHT JOIN %s AS u on s.id = u.id WHERE u.id IS NOT NULL and u.identity <> 3 ORDER BY u.Gold1 DESC LIMIT 200",_TBL_SYNATTR,_TBL_USER);
	sprintf( szSQL, "SELECT u.name,u.Gold1+u.Gold_Saved,s.syn_id FROM %s as s RIGHT JOIN %s AS u on s.id = u.id WHERE u.id IS NOT NULL and u.identity <> 3 ORDER BY u.Gold1+u.Gold_Saved DESC LIMIT 200",_TBL_SYNATTR,_TBL_USER);
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
// 		IRecordset* pResult;
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			strcpy( m_WorldTopList.MoneyList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.MoneyList[ i ].dwMoney		= pRes->GetInt( 1 );
			int nID = pRes->GetInt( 2 );
			CheckSynName(m_WorldTopList.MoneyList[ i ].szSynName, nID, pDb);
// 			if(nID != ID_NONE)
// 			{
// 				sprintf(szSQL,"SELECT name FROM %s WHERE id = %d",
// 					_TBL_SYNDICATE,nID);	
// 				CAutoPtr<IRecordset> pResult = pDb->CreateNewRecordset( szSQL, false );
// 				if (pResult && (pResult->RecordCount() > 0))
// 				{
// 					strcpy(m_WorldTopList.MoneyList[ i ].szSynName,pResult->GetStr(0));	
// // 					pResult->Release();
// 					
// 				}
// 				else
// 					strcpy(m_WorldTopList.MoneyList[ i ].szSynName,"δ������");
// 			}
// 			else
// 				strcpy(m_WorldTopList.MoneyList[ i ].szSynName,"δ������");
			pRes->MoveNext();
		}
		pRes->Release();
	}

	//��ְҵ��
	sprintf( szSQL,"SELECT name,Fir_Job,Fir_JobLv FROM %s where identity <> 3 ORDER BY Fir_JobLv DESC LIMIT 200",_TBL_USER );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			DWORD nProf = pRes->GetInt( 1 );
			int nLev = pRes->GetInt( 2 );
			if( nProf == 0)
			{
				pRes->MoveNext();
				continue;
			}
			if(nLev < 1)
				break;
			strcpy( m_WorldTopList.FirJobList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.FirJobList[ i ].dwProf		= nProf;
			m_WorldTopList.FirJobList[ i ].nData		= nLev;
			pRes->MoveNext();
		}
		pRes->Release(); 
	}

	//��ְҵ��
	sprintf( szSQL,"SELECT name,Sec_Job,Sec_JobLv FROM %s where identity <> 3 ORDER BY Sec_JobLv DESC LIMIT 200",_TBL_USER );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			DWORD nProf = pRes->GetInt( 1 );
			int nLev = pRes->GetInt( 2 );
			if( nProf == 0)
			{
				pRes->MoveNext();
				continue;
			}
			if(nLev < 1)
				break;
			strcpy( m_WorldTopList.SecJobList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.SecJobList[ i ].dwProf		= nProf;
			m_WorldTopList.SecJobList[ i ].nData			= nLev;
			pRes->MoveNext();
		}
		pRes->Release(); 
	}

	//���ְ�
	sprintf( szSQL,"SELECT name,Fir_Job,max_score FROM %s where identity <> 3 ORDER BY max_score DESC LIMIT 200",_TBL_USER );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			strcpy( m_WorldTopList.ScoreList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.ScoreList[ i ].dwProf		= pRes->GetInt( 1 );
			m_WorldTopList.ScoreList[ i ].nData			= pRes->GetInt( 2 );
			pRes->MoveNext();
		}
		pRes->Release(); 
	}

	//�⹦��
	sprintf( szSQL,"SELECT name,Fir_Job,max_phyatt FROM %s where identity <> 3 ORDER BY max_phyatt DESC LIMIT 200",_TBL_USER );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			strcpy( m_WorldTopList.PhyAttList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.PhyAttList[ i ].dwProf		= pRes->GetInt( 1 );
			m_WorldTopList.PhyAttList[ i ].nData		= pRes->GetInt( 2 );
			pRes->MoveNext();
		}
		pRes->Release(); 
	}

	//�ڹ���
	sprintf( szSQL,"SELECT name,Fir_Job,max_magatt FROM %s where identity <> 3 ORDER BY max_magatt DESC LIMIT 200",_TBL_USER );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			strcpy( m_WorldTopList.MagAttList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.MagAttList[ i ].dwProf		= pRes->GetInt( 1 );
			m_WorldTopList.MagAttList[ i ].nData		= pRes->GetInt( 2 );
			pRes->MoveNext();
		}
		pRes->Release(); 
	}

	//��������
	sprintf( szSQL,"SELECT name,Fir_Job,max_maxlife FROM %s where identity <> 3 ORDER BY max_maxlife DESC LIMIT 200",_TBL_USER );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			strcpy( m_WorldTopList.LifeList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.LifeList[ i ].dwProf		= pRes->GetInt( 1 );
			m_WorldTopList.LifeList[ i ].nData		= pRes->GetInt( 2 );
			pRes->MoveNext();
		}
		pRes->Release(); 
	}

	//������
	sprintf( szSQL,"SELECT name,Fir_Job,max_crit FROM %s where identity <> 3 ORDER BY max_crit DESC LIMIT 200",_TBL_USER );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			strcpy( m_WorldTopList.CritList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.CritList[ i ].dwProf		= pRes->GetInt( 1 );
			m_WorldTopList.CritList[ i ].nData		= pRes->GetInt( 2 );
			pRes->MoveNext();
		}
		pRes->Release(); 
	}

	//���а�
	sprintf( szSQL,"SELECT name,Fir_Job,max_hitrate FROM %s where identity <> 3 ORDER BY max_hitrate DESC LIMIT 200", _TBL_USER );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			strcpy( m_WorldTopList.HitRateList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.HitRateList[ i ].dwProf		= pRes->GetInt( 1 );
			m_WorldTopList.HitRateList[ i ].nData		= pRes->GetInt( 2 );
			pRes->MoveNext();
		}
		pRes->Release(); 
	}

	//�������
	sprintf( szSQL,"SELECT name,level,amount FROM %s where level>0 ORDER BY amount DESC LIMIT 200",_TBL_SYNDICATE  );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			strcpy( m_WorldTopList.SynMemList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.SynMemList[ i ].dwProf		= pRes->GetInt( 1 );
			m_WorldTopList.SynMemList[ i ].nData		= pRes->GetInt( 2 );
			pRes->MoveNext();
		}
		pRes->Release(); 
	}


	//���Ǯ
	sprintf( szSQL,"SELECT name,amount,money FROM %s where level>0 ORDER BY money DESC LIMIT 200",_TBL_SYNDICATE );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			strcpy( m_WorldTopList.SynMoneyList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.SynMoneyList[ i ].dwProf		= pRes->GetInt( 1 );
			m_WorldTopList.SynMoneyList[ i ].nData		= pRes->GetInt( 2 );
			pRes->MoveNext();
		}
		pRes->Release(); 
	}


	//������������
	sprintf( szSQL, "SELECT owner_id,name,maxscore FROM %s ORDER BY maxscore DESC LIMIT 200",_TBL_EUDEMON);
	pRes = pDb->CreateNewRecordset( szSQL, false );
	if( pRes )
	{
// 		IRecordset* pResult;
		for( i = 0; i < pRes->RecordCount(); i++ )
		{
			strcpy( m_WorldTopList.EudemonScoreList[ i ].szSynName,pRes->GetStr( 1 ) );
			m_WorldTopList.EudemonScoreList[ i ].nLev		= pRes->GetInt( 2 );
			int nID = pRes->GetInt( 0 );
			if(nID != ID_NONE)
			{
				sprintf( szSQL,"SELECT name FROM %s WHERE id = %u LIMIT 1",_TBL_USER ,nID);
				::CAutoPtr<IRecordset> pResult = pDb->CreateNewRecordset( szSQL, false );
				if (pResult && pResult->GetStr(0) != NULL)
				{
					strcpy(m_WorldTopList.EudemonScoreList[ i ].szName,pResult->GetStr(0));	
// 					pResult->Release();
				}
				else
					strcpy(m_WorldTopList.EudemonScoreList[ i ].szSynName,"��");
			}
			else
				strcpy(m_WorldTopList.EudemonScoreList[ i ].szSynName,"��");
			pRes->MoveNext();
		}
		pRes->Release();  
	}

	::GetLocalTime( &m_LastUpdateTime );//����������ʱ��������
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CTopListSystem::UpdateWorldData()
{
	if( !m_pDb )
		return;
	int i;
	IDatabase* pDb = m_pDb;
	char szSQL[ 1024 ];


	//�ȼ�����
	sprintf( szSQL, "SELECT u.name,u.level,s.syn_id FROM  %s as s RIGHT JOIN %s AS u on s.id = u.id WHERE u.id IS NOT NULL and u.identity <> 3 ORDER BY u.level DESC LIMIT 200",_TBL_SYNATTR,_TBL_USER);
	IRecordset* pRes = pDb->CreateNewRecordset( szSQL, false );
	if( pRes )
	{
		//IRecordset* pResult;
		for( i = 0; i < pRes->RecordCount(); i++ )
		{
			strcpy( m_WorldTopList.LevelList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.LevelList[ i ].nLev		= pRes->GetInt( 1 );
			int nID = pRes->GetInt( 2 );
			CheckSynName(m_WorldTopList.LevelList[ i ].szSynName, nID, pDb);
// 			if(nID != ID_NONE)
// 			{
// 				sprintf(szSQL,"SELECT name FROM %s WHERE id = %d",
// 					_TBL_SYNDICATE,nID);	
// 				CAutoPtr<IRecordset> pResult = pDb->CreateNewRecordset( szSQL, false );
// 				if (pResult && (pResult->RecordCount() > 0))
// 				{
// 					strcpy(m_WorldTopList.LevelList[ i ].szSynName,pResult->GetStr(0));	
// // 					pResult->Release();
// 					
// 				}
// 				else
// 					strcpy(m_WorldTopList.LevelList[ i ].szSynName,"δ������");
// 			}
// 			else
// 				strcpy(m_WorldTopList.LevelList[ i ].szSynName,"δ������");
			pRes->MoveNext();
		}
		pRes->Release();  
	}

	//������
	sprintf( szSQL, "SELECT u.name,u.Gold1+u.Gold_Saved,s.syn_id FROM %s as s RIGHT JOIN %s AS u on s.id = u.id WHERE u.id IS NOT NULL and u.identity <> 3 ORDER BY u.Gold1+u.Gold_Saved DESC LIMIT 200",_TBL_SYNATTR,_TBL_USER);
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
// 		IRecordset* pResult;
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			strcpy( m_WorldTopList.MoneyList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.MoneyList[ i ].dwMoney		= pRes->GetInt( 1 );
			int nID = pRes->GetInt( 2 );
			CheckSynName(m_WorldTopList.MoneyList[ i ].szSynName, nID, pDb);
// 			if(nID != ID_NONE)
// 			{
// 				sprintf(szSQL,"SELECT name FROM %s WHERE id = %d",
// 					_TBL_SYNDICATE,nID);	
// 				CAutoPtr<IRecordset> pResult = pDb->CreateNewRecordset( szSQL, false );
// 				if (pResult && (pResult->RecordCount() > 0))
// 				{
// 					strcpy(m_WorldTopList.MoneyList[ i ].szSynName,pResult->GetStr(0));	
// // 					pResult->Release();
// 					
// 				}
// 				else
// 					strcpy(m_WorldTopList.MoneyList[ i ].szSynName,"δ������");
// 			}
// 			else
// 				strcpy(m_WorldTopList.MoneyList[ i ].szSynName,"δ������");
			pRes->MoveNext();
		}
		pRes->Release();
	}

	//��ְҵ��
	sprintf( szSQL,"SELECT name,Fir_Job,Fir_JobLv FROM %s where identity <> 3 ORDER BY Fir_JobLv DESC LIMIT 200",_TBL_USER );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			DWORD nProf = pRes->GetInt( 1 );
			int nLev = pRes->GetInt( 2 );
			if( nProf == 0)
			{
				pRes->MoveNext();
				continue;
			}
			if(nLev < 1)
				break;
			strcpy( m_WorldTopList.FirJobList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.FirJobList[ i ].dwProf		= nProf;
			m_WorldTopList.FirJobList[ i ].nData		= nLev;
			pRes->MoveNext();
		}
		pRes->Release(); 
	}

	//��ְҵ��
	sprintf( szSQL,"SELECT name,Sec_Job,Sec_JobLv FROM %s where identity <> 3 ORDER BY Sec_JobLv DESC LIMIT 200",_TBL_USER );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			DWORD nProf = pRes->GetInt( 1 );
			int nLev = pRes->GetInt( 2 );
			if( nProf == 0)
			{
				pRes->MoveNext();
				continue;
			}
			if(nLev < 1)
				break;
			strcpy( m_WorldTopList.SecJobList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.SecJobList[ i ].dwProf		= nProf;
			m_WorldTopList.SecJobList[ i ].nData			= nLev;
			pRes->MoveNext();
		}
		pRes->Release(); 
	}

	//���ְ�
	sprintf( szSQL,"SELECT name,Fir_Job,max_score FROM %s where identity <> 3 ORDER BY max_score DESC LIMIT 200",_TBL_USER );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			strcpy( m_WorldTopList.ScoreList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.ScoreList[ i ].dwProf		= pRes->GetInt( 1 );
			m_WorldTopList.ScoreList[ i ].nData			= pRes->GetInt( 2 );
			pRes->MoveNext();
		}
		pRes->Release(); 
	}

	//�⹦��
	sprintf( szSQL,"SELECT name,Fir_Job,max_phyatt FROM %s where identity <> 3 ORDER BY max_phyatt DESC LIMIT 200",_TBL_USER );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			strcpy( m_WorldTopList.PhyAttList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.PhyAttList[ i ].dwProf		= pRes->GetInt( 1 );
			m_WorldTopList.PhyAttList[ i ].nData		= pRes->GetInt( 2 );
			pRes->MoveNext();
		}
		pRes->Release(); 
	}

	//�ڹ���
	sprintf( szSQL,"SELECT name,Fir_Job,max_magatt FROM %s where identity <> 3 ORDER BY max_magatt DESC LIMIT 200",_TBL_USER );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			strcpy( m_WorldTopList.MagAttList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.MagAttList[ i ].dwProf		= pRes->GetInt( 1 );
			m_WorldTopList.MagAttList[ i ].nData		= pRes->GetInt( 2 );
			pRes->MoveNext();
		}
		pRes->Release(); 
	}

	//��������
	sprintf( szSQL,"SELECT name,Fir_Job,max_maxlife FROM %s where identity <> 3 ORDER BY max_maxlife DESC LIMIT 200",_TBL_USER );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			strcpy( m_WorldTopList.LifeList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.LifeList[ i ].dwProf		= pRes->GetInt( 1 );
			m_WorldTopList.LifeList[ i ].nData		= pRes->GetInt( 2 );
			pRes->MoveNext();
		}
		pRes->Release(); 
	}

	//������
	sprintf( szSQL,"SELECT name,Fir_Job,max_crit FROM %s where identity <> 3 ORDER BY max_crit DESC LIMIT 200",_TBL_USER );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			strcpy( m_WorldTopList.CritList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.CritList[ i ].dwProf		= pRes->GetInt( 1 );
			m_WorldTopList.CritList[ i ].nData		= pRes->GetInt( 2 );
			pRes->MoveNext();
		}
		pRes->Release(); 
	}

	//���а�
	sprintf( szSQL,"SELECT name,Fir_Job,max_hitrate FROM %s where identity <> 3 ORDER BY max_hitrate DESC LIMIT 200", _TBL_USER );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			strcpy( m_WorldTopList.HitRateList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.HitRateList[ i ].dwProf		= pRes->GetInt( 1 );
			m_WorldTopList.HitRateList[ i ].nData		= pRes->GetInt( 2 );
			pRes->MoveNext();
		}
		pRes->Release(); 
	}

	//�������
	sprintf( szSQL,"SELECT name,level,amount FROM %s where level>0 ORDER BY amount DESC LIMIT 200",_TBL_SYNDICATE  );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			strcpy( m_WorldTopList.SynMemList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.SynMemList[ i ].dwProf		= pRes->GetInt( 1 );
			m_WorldTopList.SynMemList[ i ].nData		= pRes->GetInt( 2 );
			pRes->MoveNext();
		}
		pRes->Release(); 
	}


	//���Ǯ
	sprintf( szSQL,"SELECT name,amount,money FROM %s where level>0  ORDER BY money DESC LIMIT 200",_TBL_SYNDICATE );//�Ƹ���������
	pRes = pDb->CreateNewRecordset( szSQL,false );
	if( pRes )
	{
		for( i = 0;i < pRes->RecordCount();i++ )
		{
			strcpy( m_WorldTopList.SynMoneyList[ i ].szName,pRes->GetStr( 0 ) );
			m_WorldTopList.SynMoneyList[ i ].dwProf		= pRes->GetInt( 1 );
			m_WorldTopList.SynMoneyList[ i ].nData		= pRes->GetInt( 2 );
			pRes->MoveNext();
		}
		pRes->Release(); 
	}


	//������������
	sprintf( szSQL, "SELECT owner_id,name,maxscore FROM %s ORDER BY maxscore DESC LIMIT 200",_TBL_EUDEMON);
	pRes = pDb->CreateNewRecordset( szSQL, false );
	if( pRes )
	{
// 		IRecordset* pResult;
		for( i = 0; i < pRes->RecordCount(); i++ )
		{
			strcpy( m_WorldTopList.EudemonScoreList[ i ].szSynName,pRes->GetStr( 1 ) );
			m_WorldTopList.EudemonScoreList[ i ].nLev		= pRes->GetInt( 2 );
			int nID = pRes->GetInt( 0 );
			if(nID != ID_NONE)
			{
				sprintf( szSQL,"SELECT name FROM %s WHERE id = %u LIMIT 1",_TBL_USER ,nID);
				::CAutoPtr<IRecordset> pResult = pDb->CreateNewRecordset( szSQL, false );
				if (pResult && pResult->GetStr(0) != NULL)
				{
					strcpy(m_WorldTopList.EudemonScoreList[ i ].szName,pResult->GetStr(0));	
// 					pResult->Release();
				}
				else
					strcpy(m_WorldTopList.EudemonScoreList[ i ].szSynName,"��");
			}
			else
				strcpy(m_WorldTopList.EudemonScoreList[ i ].szSynName,"��");
			pRes->MoveNext();
		}
		pRes->Release();  
	}

	::GetLocalTime( &m_LastUpdateTime );//����������ʱ��������
}

extern struct STAT_STRUCT	g_stat;
//ÿ������5�㿪ʼ��������
void CTopListSystem::OnTimer( time_t tCurr )
{
	//  [1/15/2008 ���]	��������
	if(!g_bEnableTopListRefresh)
		return;
 
	SYSTEMTIME curTime;
	::GetLocalTime( &curTime );
	if( curTime.wDay != m_LastUpdateTime.wDay)//���컹û�и��¹�
	{
		if(m_TopListUpMin != curTime.wMinute)
			return;

		ResetEvent( m_UpdateCompleteEvent );//���ڸ���,�¼�Ϊ��λ״̬
		Init();
		UpdateWorldData();

// 		UINT uDay = GetPrivateProfileInt("���а�", "��������", 0, ".\\��ʱ����.ini");
// 		if (1 == curTime.wDayOfWeek && uDay != curTime.wDay)
// 		{
// 			//GiveAward();
// 			g_honorMurderList.DeleteHonor(m_pDb);//���2007/02/14ɾ������ɱ�����ݣ����sk_userhonor���ݿ⣩
// 			char buf[4];
// 			WritePrivateProfileString("���а�", "��������", itoa(curTime.wDay, buf, 10), ".\\��ʱ����.ini");
// 		}
		SetEvent( m_UpdateCompleteEvent );
	}
}

bool CTopListSystem::GetInfo( USHORT usType,toplistMemData* ppData,int* pDataSize, USHORT nPage )
{

	int nRet = ::WaitForSingleObject( m_UpdateCompleteEvent,1000 );
	if( WAIT_TIMEOUT == nRet )
	{
		return false;
	}
	switch( usType )
	{
	case MSGTOPLIST_WORLDLEVEL:
		{
			for(int i = 0 ; i < 20 ; i ++)
			{
				if(m_WorldTopList.LevelList[ (nPage - 1) * 20 + i ].szName[0] != 0)
				{
					strcpy(ppData[i].szName,m_WorldTopList.LevelList[ (nPage - 1) * 20  + i].szName);
					strcpy(ppData[i].szSyn,m_WorldTopList.LevelList[ (nPage - 1) * 20  + i].szSynName);
					ppData[i].data1 = m_WorldTopList.LevelList[ (nPage - 1) * 20  + i].nLev;
					ppData[i].data2 = 0;
				}
				else
					break;
			}
			*pDataSize = i;
			return true;
		}
		break;
	case MSGTOPLIST_WORLDFIRJOB:
		{
			for(int i = 0 ; i < 20 ; i ++)
			{
				if(m_WorldTopList.FirJobList[ (nPage - 1) * 20 + i ].szName[0] != 0)
				{
					strcpy(ppData[i].szName,m_WorldTopList.FirJobList[ (nPage - 1) * 20+ i ].szName);
					strcpy(ppData[i].szSyn,"");
					ppData[i].data1 = m_WorldTopList.FirJobList[ (nPage - 1) * 20+ i ].dwProf;
					ppData[i].data2 = m_WorldTopList.FirJobList[ (nPage - 1) * 20+ i ].nData;
				}
				else
					break;
			}
			*pDataSize = i;
			return true;
		}
		break;
	case MSGTOPLIST_WORLDSECJOB:
		{
			for(int i = 0 ; i < 20 ; i ++)
			{
				if(m_WorldTopList.SecJobList[ (nPage - 1) * 20 + i ].szName[0] != 0)
				{
					strcpy(ppData[i].szName,m_WorldTopList.SecJobList[ (nPage - 1) * 20+ i ].szName);
					strcpy(ppData[i].szSyn,"");
					ppData[i].data1 = m_WorldTopList.SecJobList[ (nPage - 1) * 20+ i ].dwProf;
					ppData[i].data2 = m_WorldTopList.SecJobList[ (nPage - 1) * 20+ i ].nData;
				}
				else
					break;
			}
			*pDataSize = i;
			return true;
		}
		break;
	case MSGTOPLIST_WORLDMONEY:
		{
			for(int i = 0 ; i < 20 ; i ++)
			{
				if(m_WorldTopList.MoneyList[ (nPage - 1) * 20 + i ].szName[0] != 0)
				{
					strcpy(ppData[i].szName,m_WorldTopList.MoneyList[ (nPage - 1) * 20+ i ].szName);
					strcpy(ppData[i].szSyn,m_WorldTopList.MoneyList[ (nPage - 1) * 20+ i ].szSynName);
					ppData[i].data1 = m_WorldTopList.MoneyList[ (nPage - 1) * 20+ i ].dwMoney;
					ppData[i].data2 = 0;
				}
				else
					break;
			}
			*pDataSize = i;
			return true;
		}
		break;
	case MSGTOPLIST_WORLDPHYATT:
		{
			for(int i = 0 ; i < 20 ; i ++)
			{
				if(m_WorldTopList.PhyAttList[ (nPage - 1) * 20 + i ].szName[0] != 0)
				{
					strcpy(ppData[i].szName,m_WorldTopList.PhyAttList[ (nPage - 1) * 20+ i ].szName);
					strcpy(ppData[i].szSyn,"");
					ppData[i].data1 = m_WorldTopList.PhyAttList[ (nPage - 1) * 20+ i ].dwProf;
					ppData[i].data2 = m_WorldTopList.PhyAttList[ (nPage - 1) * 20+ i ].nData;
				}
				else
					break;
			}
			*pDataSize = i;
			return true;
		}
		break;
	case MSGTOPLIST_WORLDMAGATT:
		{
			for(int i = 0 ; i < 20 ; i ++)
			{
				if(m_WorldTopList.MagAttList[ (nPage - 1) * 20 + i ].szName[0] != 0)
				{
					strcpy(ppData[i].szName,m_WorldTopList.MagAttList[ (nPage - 1) * 20+ i ].szName);
					strcpy(ppData[i].szSyn,"");
					ppData[i].data1 = m_WorldTopList.MagAttList[ (nPage - 1) * 20+ i ].dwProf;
					ppData[i].data2 = m_WorldTopList.MagAttList[ (nPage - 1) * 20+ i ].nData;
				}
				else
					break;
			}
			*pDataSize = i;
			return true;
		}
		break;
	case MSGTOPLIST_WORLDMAXLIFE:
		{
			for(int i = 0 ; i < 20 ; i ++)
			{
				if(m_WorldTopList.LifeList[ (nPage - 1) * 20 + i ].szName[0] != 0)
				{
					strcpy(ppData[i].szName,m_WorldTopList.LifeList[ (nPage - 1) * 20+ i ].szName);
					strcpy(ppData[i].szSyn,"");
					ppData[i].data1 = m_WorldTopList.LifeList[ (nPage - 1) * 20+ i ].dwProf;
					ppData[i].data2 = m_WorldTopList.LifeList[ (nPage - 1) * 20+ i ].nData;
				}
				else
					break;
			}
			*pDataSize = i;
			return true;
		}
		break;
	case MSGTOPLIST_WORLDCRIT:
		{
			for(int i = 0 ; i < 20 ; i ++)
			{
				if(m_WorldTopList.CritList[ (nPage - 1) * 20 + i ].szName[0] != 0)
				{
					strcpy(ppData[i].szName,m_WorldTopList.CritList[ (nPage - 1) * 20+ i ].szName);
					strcpy(ppData[i].szSyn,"");
					ppData[i].data1 = m_WorldTopList.CritList[ (nPage - 1) * 20+ i ].dwProf;
					ppData[i].data2 = m_WorldTopList.CritList[ (nPage - 1) * 20+ i ].nData;
				}
				else
					break;
			}
			*pDataSize = i;
			return true;
		}
		break;
	case MSGTOPLIST_WORLDHITRATE:
		{
			for(int i = 0 ; i < 20 ; i ++)
			{
				if(m_WorldTopList.HitRateList[ (nPage - 1) * 20 + i ].szName[0] != 0)
				{
					strcpy(ppData[i].szName,m_WorldTopList.HitRateList[ (nPage - 1) * 20+ i ].szName);
					strcpy(ppData[i].szSyn,"");
					ppData[i].data1 = m_WorldTopList.HitRateList[ (nPage - 1) * 20+ i ].dwProf;
					ppData[i].data2 = m_WorldTopList.HitRateList[ (nPage - 1) * 20+ i ].nData;
				}
				else
					break;
			}
			*pDataSize = i;
			return true;
		}
		break;
	case MSGTOPLIST_WORLDSCORE:
		{
			for(int i = 0 ; i < 20 ; i ++)
			{
				if(m_WorldTopList.ScoreList[ (nPage - 1) * 20 + i ].szName[0] != 0)
				{
					strcpy(ppData[i].szName,m_WorldTopList.ScoreList[ (nPage - 1) * 20+ i ].szName);
					strcpy(ppData[i].szSyn,"");
					ppData[i].data1 = m_WorldTopList.ScoreList[ (nPage - 1) * 20+ i ].dwProf;
					ppData[i].data2 = m_WorldTopList.ScoreList[ (nPage - 1) * 20+ i ].nData;
				}
				else
					break;
			}
			*pDataSize = i;
			return true;
		}
		break;
	case MSGTOPLIST_SYNMEM:
		{
			for(int i = 0 ; i < 20 ; i ++)
			{
				if(m_WorldTopList.SynMemList[ (nPage - 1) * 20 + i ].szName[0] != 0)
				{
					strcpy(ppData[i].szName,m_WorldTopList.SynMemList[ (nPage - 1) * 20+ i ].szName);
					strcpy(ppData[i].szSyn,"");
					ppData[i].data1 = m_WorldTopList.SynMemList[ (nPage - 1) * 20+ i ].dwProf;
					ppData[i].data2 = m_WorldTopList.SynMemList[ (nPage - 1) * 20+ i ].nData;
				}
				else
					break;
			}
			*pDataSize = i;
			return true;
		}
		break;
	case MSGTOPLIST_SYNMONEY:
		{
			for(int i = 0 ; i < 20 ; i ++)
			{
				if(m_WorldTopList.SynMoneyList[ (nPage - 1) * 20 + i ].szName[0] != 0)
				{
					strcpy(ppData[i].szName,m_WorldTopList.SynMoneyList[ (nPage - 1) * 20+ i ].szName);
					strcpy(ppData[i].szSyn,"");
					ppData[i].data1 = m_WorldTopList.SynMoneyList[ (nPage - 1) * 20+ i ].dwProf;
					ppData[i].data2 = m_WorldTopList.SynMoneyList[ (nPage - 1) * 20+ i ].nData;
				}
				else
					break;
			}
			*pDataSize = i;
			return true;
		}
		break;
	case MSGTOPLIST_EUDEMONSCORE:
		{
			for(int i = 0 ; i < 20 ; i ++)
			{
				if(m_WorldTopList.EudemonScoreList[ (nPage - 1) * 20 + i ].szName[0] != 0)
				{
					strcpy(ppData[i].szName,m_WorldTopList.EudemonScoreList[ (nPage - 1) * 20+ i ].szName);
					strcpy(ppData[i].szSyn,m_WorldTopList.EudemonScoreList[ (nPage - 1) * 20+ i ].szSynName);
					ppData[i].data1 = m_WorldTopList.EudemonScoreList[ (nPage - 1) * 20+ i ].nLev;
					ppData[i].data2 = 0;
				}
				else
					break;
			}
			*pDataSize = i;
			return true;
		}
		break;
	default:
		return false;
 	}
	return true;
}

BOOL CTopListSystem::CheckTopListPlace(int nTopListType, int nID, int nOper, int nData)
{
	CHECKF(nData <= 100);
	CHECKF(nID);

	CHECKF(nOper >= GREATER && nOper <= EQUAL);

/*	CHECKF(nTopListType == MSGTOPLIST_WORLDLEVEL || nTopListType == MSGTOPLIST_WORLDFACTION
		|| nTopListType == MSGTOPLIST_WORLDHONORKILL);*/

// 	switch(nTopListType)
// 	{
// 	case MSGTOPLIST_WORLDLEVEL:
// 		{
// 			switch(nOper)
// 			{
// 			case GREATER:
// 				{
// 					for(int i = nData - 1; i < 100; i++)
// 					{
// 					//	if(strcmp(m_WorldTopList.LevelList[i].Name, szName) == 0)
// 						if(m_WorldTopList.LevelList[i].nID == nID)
// 							return TRUE;
// 					}
// 				}
// 				break;
// 			case LESSER:
// 				{
// 					for(int i = 0; i < nData - 1; i++)
// 					{
// 					//	if(strcmp(m_WorldTopList.LevelList[i].Name, szName) == 0)
// 						if(m_WorldTopList.LevelList[i].nID == nID)
// 							return TRUE;
// 					}
// 				}
// 				break;
// 			case EQUAL_OR_GREATER:
// 				{
// 					for(int i = nData - 1; i < 100; i++)
// 					{
// 					//	if(strcmp(m_WorldTopList.LevelList[i].Name, szName) == 0)
// 						if(m_WorldTopList.LevelList[i].nID == nID)
// 							return TRUE;
// 					}
// 				}
// 				
// 				return FALSE;
// 				break;
// 			case EQUAL_OR_LESSER:
// 				{
// 					for(int i = 0; i <= nData - 1; i++)
// 					{
// 					//	if(strcmp(m_WorldTopList.LevelList[i].Name, szName) == 0)
// 						if(m_WorldTopList.LevelList[i].nID == nID)
// 							return TRUE;
// 					}
// 				}
// 				
// 				return FALSE;
// 			    break;
// 			case EQUAL:
// 			//	if(strcmp(m_WorldTopList.LevelList[nData - 1].Name, szName) == 0)
// 				if(m_WorldTopList.LevelList[nData - 1].nID == nID)
// 					return TRUE;
// 				
// 				return FALSE;
// 			    break;
// 			default:
// 			    break;
// 			}
// 		}
// 		break;
// 	case MSGTOPLIST_WORLDFACTION:
// 		{
// 			switch(nOper)
// 			{
// 			case GREATER:
// 				{
// 					for(int i = nData - 1; i < 100; i++)
// 					{
// 					//	if(strcmp(m_WorldTopList.FactionList[i]., szName) == 0)
// 						if(m_WorldTopList.FactionList[i].nSynID == nID)
// 							return TRUE;
// 					}
// 				}
// 				break;
// 			case LESSER:
// 				{
// 					for(int i = 0; i < nData - 1; i++)
// 					{
// 					//	if(strcmp(m_WorldTopList.FactionList[i].Name, szName) == 0)
// 						if(m_WorldTopList.FactionList[i].nSynID == nID)
// 							return TRUE;
// 					}
// 				}
// 				break;
// 			case EQUAL_OR_GREATER:
// 				{
// 					for(int i = nData - 1; i < 100; i++)
// 					{
// 					//	if(strcmp(m_WorldTopList.FactionList[i].Name, szName) == 0)
// 						if(m_WorldTopList.FactionList[i].nSynID == nID)
// 							return TRUE;
// 					}
// 				}
// 				
// 				return FALSE;
// 				break;
// 			case EQUAL_OR_LESSER:
// 				{
// 					for(int i = 0; i <= nData - 1; i++)
// 					{
// 					//	if(strcmp(m_WorldTopList.FactionList[i].Name, szName) == 0)
// 						if(m_WorldTopList.FactionList[i].nSynID == nID)
// 							return TRUE;
// 					}
// 				}
// 				
// 				return FALSE;
// 			    break;
// 			case EQUAL:
// 				//	if(strcmp(m_WorldTopList.FactionList[nData - 1].Name, szName) == 0)
// 				if(m_WorldTopList.FactionList[nData - 1].nSynID == nID)
// 					return TRUE;
// 				
// 				return FALSE;
// 			    break;
// 			default:
// 			    break;
// 			}
// 		}
// 	    break;
// 	case MSGTOPLIST_WORLDHONORKILL:
// 		{
// 			switch(nOper)
// 			{
// 			case GREATER:
// 				{
// 					for(int i = nData - 1; i < 100; i++)
// 					{
// 					//	if(strcmp(m_WorldTopList.LevelList[i].Name, szName) == 0)
// 						if(m_WorldTopList.HonorKillList[i].nID == nID)
// 							return TRUE;
// 					}
// 				}
// 				break;
// 			case LESSER:
// 				{
// 					for(int i = 0; i < nData - 1; i++)
// 					{
// 					//	if(strcmp(m_WorldTopList.LevelList[i].Name, szName) == 0)
// 						if(m_WorldTopList.HonorKillList[i].nID == nID)
// 							return TRUE;
// 					}
// 				}
// 				break;
// 			case EQUAL_OR_GREATER:
// 				{
// 					for(int i = nData - 1; i < 100; i++)
// 					{
// 					//	if(strcmp(m_WorldTopList.LevelList[i].Name, szName) == 0)
// 						if(m_WorldTopList.HonorKillList[i].nID == nID)
// 							return TRUE;
// 					}
// 				}
// 				
// 				return FALSE;
// 				break;
// 			case EQUAL_OR_LESSER:
// 				{
// 					for(int i = 0; i <= nData - 1; i++)
// 					{
// 					//	if(strcmp(m_WorldTopList.LevelList[i].Name, szName) == 0)
// 						if(m_WorldTopList.HonorKillList[i].nID == nID)
// 							return TRUE;
// 					}
// 				}
// 				
// 				return FALSE;
// 			    break;
// 			case EQUAL:
// 			//	if(strcmp(m_WorldTopList.LevelList[nData - 1].Name, szName) == 0)
// 				if(m_WorldTopList.HonorKillList[nData - 1].nID == nID)
// 					return TRUE;
// 				
// 				return FALSE;
// 			    break;
// 			default:
// 			    break;
// 			}
// 		}
// 		break;
// 
// 	default:
// 
// 		return FALSE;
// 	    break;
// 	}

	return FALSE;
}

int CTopListSystem::GetTopListPlace(int nTopListType, int nID,const char* szName)
{
	if(!nID && (strlen(szName) == 0))
		return 9999;

	int nRet = ::WaitForSingleObject( m_UpdateCompleteEvent,1000 );

	if( WAIT_TIMEOUT == nRet )
	{
		return 9999;
	}

	switch( nTopListType )
	{
	case MSGTOPLIST_WORLDLEVEL:
		{
			for(int i = 0 ; i < 200 ; i ++)
			{
				if(strcmp(m_WorldTopList.LevelList[ i ].szName,szName) == 0)
				{
					return i;
				}
			}
		}
		break;
	case MSGTOPLIST_WORLDFIRJOB:
		{
			for(int i = 0 ; i < 200 ; i ++)
			{
				if(strcmp(m_WorldTopList.FirJobList[ i ].szName,szName) == 0)
				{
					return i;
				}
			}
		}
		break;
	case MSGTOPLIST_WORLDSECJOB:
		{
			for(int i = 0 ; i < 200 ; i ++)
			{
				if(strcmp(m_WorldTopList.SecJobList[ i ].szName,szName) == 0)
				{
					return i;
				}
			}
		}
		break;
	case MSGTOPLIST_WORLDMONEY:
		{
			for(int i = 0 ; i < 200 ; i ++)
			{
				if(strcmp(m_WorldTopList.MoneyList[ i ].szName,szName) == 0)
				{
					return i;
				}
			}
		}
		break;
	case MSGTOPLIST_WORLDPHYATT:
		{
			for(int i = 0 ; i < 200 ; i ++)
			{
				if(strcmp(m_WorldTopList.PhyAttList[ i ].szName,szName) == 0)
				{
					return i;
				}
			}
		}
		break;
	case MSGTOPLIST_WORLDMAGATT:
		{
			for(int i = 0 ; i < 200 ; i ++)
			{
				if(strcmp(m_WorldTopList.MagAttList[ i ].szName,szName) == 0)
				{
					return i;
				}
			}
		}
		break;
	case MSGTOPLIST_WORLDMAXLIFE:
		{
			for(int i = 0 ; i < 200 ; i ++)
			{
				if(strcmp(m_WorldTopList.LifeList[ i ].szName,szName) == 0)
				{
					return i;
				}
			}
		}
		break;
	case MSGTOPLIST_WORLDCRIT:
		{
			for(int i = 0 ; i < 200 ; i ++)
			{
				if(strcmp(m_WorldTopList.CritList[ i ].szName,szName) == 0)
				{
					return i;
				}
			}
		}
		break;
	case MSGTOPLIST_WORLDHITRATE:
		{
			for(int i = 0 ; i < 200 ; i ++)
			{
				if(strcmp(m_WorldTopList.HitRateList[ i ].szName,szName) == 0)
				{
					return i;
				}
			}
		}
		break;
	case MSGTOPLIST_WORLDSCORE:
		{
			for(int i = 0 ; i < 200 ; i ++)
			{
				if(strcmp(m_WorldTopList.ScoreList[ i ].szName,szName) == 0)
				{
					return i;
				}
			}
		}
		break;
	case MSGTOPLIST_SYNMEM:
		{
			for(int i = 0 ; i < 200 ; i ++)
			{
				if(strcmp(m_WorldTopList.SynMemList[ i ].szName,szName) == 0)
				{
					return i;
				}
			}
		}
		break;
	case MSGTOPLIST_SYNMONEY:
		{
			for(int i = 0 ; i < 200 ; i ++)
			{
				if(strcmp(m_WorldTopList.SynMoneyList[ i ].szName,szName) == 0)
				{
					return i;
				}
			}
		}
		break;
	case MSGTOPLIST_EUDEMONSCORE:
		{
			for(int i = 0 ; i < 200 ; i ++)
			{
				if(strcmp(m_WorldTopList.EudemonScoreList[ i ].szName,szName) == 0)
				{
					return i;
				}
			}
		}
		break;
	default:
		return 9999;
 	}
	return 9999;
}

void CTopListSystem::CheckSynName(char *szSynName, int SynID, IDatabase* pDb)
{
	char szSQL[ 1024 ];
	if(SynID == ID_NONE)
	{
		strcpy(szSynName ,"δ������");
		return;
	}
	sprintf(szSQL,"SELECT name,level FROM %s WHERE id = %d",
		_TBL_SYNDICATE,SynID);	
	::CAutoPtr<IRecordset> pResult = pDb->CreateNewRecordset( szSQL, false );
	if (pResult && (pResult->RecordCount() > 0))
	{
		if(pResult->GetInt(1) > 0)	//��������еȼ�
			strcpy(szSynName ,pResult->GetStr(0));	
		else 
			strcpy(szSynName ,"δ������");		 					
	}
	else
		strcpy(szSynName ,"δ������");
}
