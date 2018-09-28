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

///VAS���ݿ���ʹ��������޸���ֵ���������ɽ��׼�¼,��ѯ�ʻ����
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
	OBJID	m_idAccount;///�ʺ�
	OBJID	m_idPlayer;///��ɫID
	CString	m_strServerName;///��Ϸ����������
	CMyRecordset*		m_pRes;///��¼��������ڷ������ݿ�
	//char				m_szSQL[256];
	CString	m_strSQL;///SQL���
};

#endif // !defined(AFX_FATIGUEMANAGER_H__2C1B8BCC_6056_4E89_838B_A636669F7204__INCLUDED_)
