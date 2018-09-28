// FatiGueManager.h: interface for the FatiGueManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FATIGUEMANAGER_H__2C1B8BCC_6056_4E89_838B_A636669F7204__INCLUDED_)
#define AFX_FATIGUEMANAGER_H__2C1B8BCC_6056_4E89_838B_A636669F7204__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Define.h"
#include "GameBaseCode//mydb.h"

///VAS数据库访问管理器，修改增值点数和生成交易记录,查询帐户余额
#include "string"
#include "MsgFatigue.h"
using namespace std;


class CFatiGueManager  
{
public:
	int GetFCM(int AccountId);
// 	CFatiGueManager();
// 	virtual ~CFatiGueManager();
private:
	OBJID	m_idAccount;///帐号
	OBJID	m_idPlayer;///角色ID
	CString	m_strServerName;///游戏服务器名称
	CMyRecordset*		m_pRes;///记录集组件用于访问数据库
	//char				m_szSQL[256];
	CString	m_strSQL;///SQL语句
};

#endif // !defined(AFX_FATIGUEMANAGER_H__2C1B8BCC_6056_4E89_838B_A636669F7204__INCLUDED_)
