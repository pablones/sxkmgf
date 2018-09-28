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

///״̬������,�ɽ�ɫӵ��,����ɫ��ֵ״̬д�����ݿ�,���ڽ�ɫ�´�����ʱ��ԭ״̬
class CStatusManager  
{
public:
	CStatusManager();
	~CStatusManager();

	BOOL WriteStatusDB(IRole* pRole,IDatabase* pDB,IStatus* pStatus);///д�����ݿ�
	bool  RestoreStatusDB(CUser* pRole,IDatabase* pDB);///�����ݿ��ڻָ���ɫ״̬
//	BOOL ClearStatusDB(IRole* pRole,IDatabase* pDB);///�ڱ����ɫ״̬֮ǰ���������
	BOOL IsValueStatus(IStatus* pStatus);///�жϸ�״̬�Ƿ���Ҫд�����ݿ�
	BOOL IsShutup(CUser* pRole);///���û��Ƿ񱻽�ֹ����
	BOOL IsDownLineGoOnStatus( UINT nStatusID );// �Ƿ�Ϊ�����Ժ�Ҳ������ʱ ��� [6/28/2007]
	BOOL RestoreFreezeStatusDB(CUser* pRole,IDatabase* pDB, UINT uStateID);///�����ݿ��ڻָ�����Ľ�ɫ״̬ by goto
	BOOL CheckRestoreFreezeStatusDB(CUser* pRole,IDatabase* pDB, UINT uStateID);
private:
//	CMyRecordset*		m_pRes;///��¼��������ڷ������ݿ�
	//char				m_szSQL[256];
//	CString	m_strSQL;///SQL���
};

#endif // !defined(AFX_STATUSMANAGER_H__4AB4E0FC_05D7_4D69_964F_FD4B8FB2807C__INCLUDED_)
