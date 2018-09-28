// FatiGueManager.cpp: implementation of the FatiGueManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FatiGueManager.h"
#include "Logfile.h"	//paled
#include "AllHeads.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const char* sqlGetFCM = "select * from account where id = '%d'";		//��÷����Ա�ʶ


// CFatiGueManager::CFatiGueManager()
// {
//     
// }
// 
// CFatiGueManager::~CFatiGueManager()
// {
// 	if(m_pRes)
// 	{
// 		m_pRes->Close();///�رռ�¼��
// 		delete m_pRes;///���ٶ���
// 		m_pRes=NULL;
// 	}
// }

int CFatiGueManager::GetFCM(int AccountId)
{
	CMyRecordset Res(&g_db);
	///�õ��ʺ�ID
	int bReulst=0;
	m_strSQL.Format(sqlGetFCM,AccountId);
	m_strSQL.MakeUpper();
	bReulst=Res.Open(m_strSQL);
	if (bReulst && Res.RecordCount())
		bReulst=Res.Fields("fcm");
	else
		bReulst=-1;
	Res.Close();
	return bReulst;
}