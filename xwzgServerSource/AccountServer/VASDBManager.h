// VASDBManager.h: interface for the CVASDBManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VASDBMANAGER_H__C2B1E812_971D_4C10_BA86_68C4A3B61EF4__INCLUDED_)
#define AFX_VASDBMANAGER_H__C2B1E812_971D_4C10_BA86_68C4A3B61EF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Define.h"
#include "GameBaseCode//mydb.h"

///VAS数据库访问管理器，修改增值点数和生成交易记录,查询帐户余额
#include "string"
#include "MsgVAS.h"
using namespace std;

class CVASDBManager  
{
public:
	BOOL SetVotCard(const char* pszCard);
	BOOL CheckVotCard(const char* pszCard);
	BOOL SetVotCard2(const char* pszCard);
	BOOL CheckVotCard2(const char* pszCard);
	BOOL SetVotCard3(const char* pszCard);
	BOOL CheckVotCard3(const char* pszCard);
	__int64 ExecChg(const CString& cstrServerName, __int64 iTradePoint);//[游途道标 2008/12/3]修改增值币数量,返回余额.
	UINT ExecItemLifeContinue(UINT nTradePoint, OBJID UserID, OBJID ItemID, UINT ItemAmount, const CString& strServerName, UINT nType);
	CVASDBManager();
	~CVASDBManager();

	///如果没有该帐号的VAS记录,则失败返回0,新建记录的工作由充值程序承担
	BOOL GetAccountID(const CString& strAccountName);///得到帐号ID
	BOOL InitServer(ULONG idAccount);
	BOOL InitMananger(ULONG idAccount,const CString& strServerName);///初始化VAS数据库管理器,定位记录位置
	BOOL CheckNewCard(const char* pszCard);
	BOOL SetNewCard(const char* pszCard);
	BOOL CheckNewCard2(ULONG idAccount,const char* pszCard);
	BOOL SetNewCard2(ULONG idAccount,const char* pszCard);
	UINT GetBalance();///得到帐户增值点余额
	UINT GetBalanceJuan();
	UINT ExecTrade(UINT nTradePoint,OBJID UserID,OBJID ItemID,UINT ItemAmount,const CString& strServerName,int nJuan);///进行增值交易,余额不足或交易失败返回-1,成功返回交易后余额
	UINT ExecTrade(UINT nTradePoint,OBJID UserID,const CString& strServerName, CMsgVAS::MsgTraffic* pGoods, int nSize);//[2010-07-27 goto]重载此函数，一次可以交易多种物品
	long ExecChg(UINT nTradePoint,const CString& strServerName,int nTradeType);
	int  ExecServerChk(int server = 1,int action = 1,int data = 0);
	int	 ExecMaskDo(int data,int action ,int check);
	UINT ExecSupply(UINT nTradePoint,const CString& strServerName,int idWebSite,int nTradeType);///充值,结果和同上
	UINT ExecSupply(UINT nTradePoint,const CString& strServerName,int idWebSite,int nTradeType,UINT nMoney);///充值,带游戏币兑换,结果和同上
//protected:
	BOOL WriteConsumeLog(const CString& strAccountName,OBJID UserID,OBJID ItemID,UINT ItemAmount,UINT nTradePoint,const CString& strServerName);///写消费记录表,由ExecTrade调用
	BOOL WriteSupplyLog(UINT idWebSite,const CString& strAccountName,const CString& strServerName,int nSupplyType,UINT nTradePoint);///写充值记录表,由ExecSupply调用
	BOOL WriteSupplyLog(UINT idWebSite,const CString& strAccountName,const CString& strServerName,int nSupplyType,UINT nTradePoint,UINT nMoney);///写充值记录表,由ExecSupply调用,带游戏币兑换
	BOOL WriteLifeTimeContinueLog(const CString& strAccountName,OBJID UserID,OBJID ItemID,UINT ItemAmount,UINT nTradePoint,const CString& strServerName, UINT uType);	//  [2007-9-15] 朱斌	
	BOOL WriteVasTicketLog(const CString& strAccountName,int nOperType,OBJID ItemTypeID,UINT nTradePoint,OBJID UserID,const CString& strServerName);///元宝票记录
private:
	OBJID	m_idAccount;///帐号
	OBJID	m_idPlayer;///角色ID
	CString	m_strServerName;///游戏服务器名称
	CMyRecordset*		m_pRes;///记录集组件用于访问数据库
	//char				m_szSQL[256];
	CString	m_strSQL;///SQL语句
};

#endif // !defined(AFX_VASDBMANAGER_H__C2B1E812_971D_4C10_BA86_68C4A3B61EF4__INCLUDED_)
