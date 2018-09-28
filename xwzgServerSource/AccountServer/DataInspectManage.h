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

const int  AWARDEX_TYPE_BEGIN    = 100; //领奖扩展起始类型
const int  AWARDEX_TYPE_END      = 200; //领奖扩展结束类型

typedef vector<GainAwardInfo> VECAWARD;
typedef VECAWARD::iterator ITAWARD;

class CDataInspectManage  
{
public:
	CDataInspectManage();
	virtual ~CDataInspectManage();
	BOOL GetNewPlayerExamState( UINT nAccountID, char* state,int saveSize, USHORT& size );//  [9/22/2007 Administrator]
	BOOL WriteMananger( const CString& strAccountName, UINT nAccountID );///直接纪录登陆
	BOOL UpdateAccoutOnlineTime(OBJID idAccount,const CString& strServerName);
	int  ManageGeneralize( UINT nAccountID, int nAction ,int nType ,int nData = 0 );///初始化ser_loginlog数据库管理器,直接写数据。试卷的套数ExamNum
	BOOL WriteMananger(const CString& strServerName,const CString& strAccountName, UINT nAccountID,const CString& strServerIP,const CString& strMac="N/A");///支持记录客户端IP和网卡物理地址.
	BOOL  StopAccountByGMtool( UINT nAccountID );///通过GM来封号
	BOOL  ActiveAccountByGMtool( UINT nAccountID );///通过GM来封号
	USHORT GetAccountStatus( UINT nAccountID );///[2009.2.2--张冬]获得账号状态
	BOOL WatchToolInfo(OBJID idAccount , const char * pGameServer,const short type = 0);///[2009.3.18]监控工具信息
	BOOL ReadAwardEx( UINT nAccountID, int nType, const char* szServerName, VECAWARD& vecAward);
	BOOL WriteAwardeEx( UINT nAccountID, int nType, const char* szServerName, int nData);
	int  ManageTopListAward( UINT nAccountID, int nAction, int nType,int nData = 0 );
	int GetMobChargeAward(UINT nAccountID,int nType,int nData=0);
	int setMobChargeAward(UINT nAccountID,int nType,int nData=0);
private:
	CString	m_strServerName;///游戏服务器名称
	CMyRecordset*		m_pRes;///记录集组件用于访问数据库
	CString	m_strSQL;///SQL语句
};

#endif // !defined(AFX_DATAINSPECTMANAGE_H__F95C1853_D8C1_4B93_B7B8_74B62F9BD72E__INCLUDED_)
