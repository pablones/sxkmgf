// DataInspectManage.h: interface for the CDataInspectManage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAINSPECTMANAGE_H__F95C1853_D8C1_4B93_B7B8_74B62F9BD72E__INCLUDED_)
#define AFX_DATAINSPECTMANAGE_H__F95C1853_D8C1_4B93_B7B8_74B62F9BD72E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Define.h"
#include "GameBaseCode//mydb.h"
#include <vector>

using std::vector;

const int  AWARDEX_TYPE_BEGIN    = 100; //�콱��չ��ʼ����
const int  AWARDEX_TYPE_END      = 200; //�콱��չ��������

typedef vector<GainAwardInfo> VECAWARD;
typedef VECAWARD::iterator ITAWARD;

class CDataInspectManage  
{
public:
	CDataInspectManage();
	virtual ~CDataInspectManage();
	BOOL GetNewPlayerExamState( UINT nAccountID, char* state,int saveSize, USHORT& size );//  [9/22/2007 Administrator]
	BOOL WriteMananger( const CString& strAccountName, UINT nAccountID );///ֱ�Ӽ�¼��½
	BOOL UpdateAccoutOnlineTime(OBJID idAccount,const CString& strServerName);
	int  ManageGeneralize( UINT nAccountID, int nAction ,int nType ,int nData = 0 );///��ʼ��ser_loginlog���ݿ������,ֱ��д���ݡ��Ծ������ExamNum
	BOOL WriteMananger(const CString& strServerName,const CString& strAccountName, UINT nAccountID,const CString& strServerIP,const CString& strMac="N/A");///֧�ּ�¼�ͻ���IP�����������ַ.
	BOOL  StopAccountByGMtool( UINT nAccountID );///ͨ��GM�����
	BOOL  ActiveAccountByGMtool( UINT nAccountID );///ͨ��GM�����
	USHORT GetAccountStatus( UINT nAccountID );///[2009.2.2--�Ŷ�]����˺�״̬
	BOOL WatchToolInfo(OBJID idAccount , const char * pGameServer,const short type = 0);///[2009.3.18]��ع�����Ϣ
	BOOL ReadAwardEx( UINT nAccountID, int nType, const char* szServerName, VECAWARD& vecAward);
	BOOL WriteAwardeEx( UINT nAccountID, int nType, const char* szServerName, int nData);
	int  ManageTopListAward( UINT nAccountID, int nAction, int nType,int nData = 0 );
	int GetMobChargeAward(UINT nAccountID,int nType,int nData=0);
	int setMobChargeAward(UINT nAccountID,int nType,int nData=0);
private:
	CString	m_strServerName;///��Ϸ����������
	CMyRecordset*		m_pRes;///��¼��������ڷ������ݿ�
	CString	m_strSQL;///SQL���
};

#endif // !defined(AFX_DATAINSPECTMANAGE_H__F95C1853_D8C1_4B93_B7B8_74B62F9BD72E__INCLUDED_)
