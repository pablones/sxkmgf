// StatusManager.h: interface for the CStatusManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUSMANAGER_H__4AB4E0FC_05D7_4D69_964F_FD4B8FB2807C__INCLUDED_)
#define AFX_STATUSMANAGER_H__4AB4E0FC_05D7_4D69_964F_FD4B8FB2807C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../share/define.h"
//#include "../../common/common.h"
//#include "../../GameBaseCodeMT/I_mydb.h"

///状态管理器,由角色拥有,将角色价值状态写入数据库,并在角色下次上线时还原状态
class CStatusManager  
{
public:
	CStatusManager();
	~CStatusManager();

	BOOL WriteStatusDB(IRole* pRole,IDatabase* pDB,IStatus* pStatus);///写入数据库
	bool  RestoreStatusDB(CUser* pRole,IDatabase* pDB);///从数据库内恢复角色状态
//	BOOL ClearStatusDB(IRole* pRole,IDatabase* pDB);///在保存角色状态之前必须先清空
	BOOL IsValueStatus(IStatus* pStatus);///判断该状态是否需要写入数据库
	BOOL IsShutup(CUser* pRole);///该用户是否被禁止发言
	BOOL IsDownLineGoOnStatus( UINT nStatusID );// 是否为下线以后也继续计时 彭峰 [6/28/2007]
	BOOL RestoreFreezeStatusDB(CUser* pRole,IDatabase* pDB, UINT uStateID);///从数据库内恢复冻结的角色状态 by goto
	BOOL CheckRestoreFreezeStatusDB(CUser* pRole,IDatabase* pDB, UINT uStateID);
private:
//	CMyRecordset*		m_pRes;///记录集组件用于访问数据库
	//char				m_szSQL[256];
//	CString	m_strSQL;///SQL语句
};

#endif // !defined(AFX_STATUSMANAGER_H__4AB4E0FC_05D7_4D69_964F_FD4B8FB2807C__INCLUDED_)
