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
const char* sqlGetFCM = "select * from account where id = '%d'";		//获得防沉迷标识


// CFatiGueManager::CFatiGueManager()
// {
//     
// }
// 
// CFatiGueManager::~CFatiGueManager()
// {
// 	if(m_pRes)
// 	{
// 		m_pRes->Close();///关闭记录集
// 		delete m_pRes;///销毁对象
// 		m_pRes=NULL;
// 	}
// }

int CFatiGueManager::GetFCM(int AccountId)
{
	CMyRecordset Res(&g_db);
	///得到帐号ID
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