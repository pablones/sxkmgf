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

///VAS���ݿ���ʹ��������޸���ֵ���������ɽ��׼�¼,��ѯ�ʻ����
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
	__int64 ExecChg(const CString& cstrServerName, __int64 iTradePoint);//[��;���� 2008/12/3]�޸���ֵ������,�������.
	UINT ExecItemLifeContinue(UINT nTradePoint, OBJID UserID, OBJID ItemID, UINT ItemAmount, const CString& strServerName, UINT nType);
	CVASDBManager();
	~CVASDBManager();

	///���û�и��ʺŵ�VAS��¼,��ʧ�ܷ���0,�½���¼�Ĺ����ɳ�ֵ����е�
	BOOL GetAccountID(const CString& strAccountName);///�õ��ʺ�ID
	BOOL InitServer(ULONG idAccount);
	BOOL InitMananger(ULONG idAccount,const CString& strServerName);///��ʼ��VAS���ݿ������,��λ��¼λ��
	BOOL CheckNewCard(const char* pszCard);
	BOOL SetNewCard(const char* pszCard);
	BOOL CheckNewCard2(ULONG idAccount,const char* pszCard);
	BOOL SetNewCard2(ULONG idAccount,const char* pszCard);
	UINT GetBalance();///�õ��ʻ���ֵ�����
	UINT GetBalanceJuan();
	UINT ExecTrade(UINT nTradePoint,OBJID UserID,OBJID ItemID,UINT ItemAmount,const CString& strServerName,int nJuan);///������ֵ����,�������ʧ�ܷ���-1,�ɹ����ؽ��׺����
	UINT ExecTrade(UINT nTradePoint,OBJID UserID,const CString& strServerName, CMsgVAS::MsgTraffic* pGoods, int nSize);//[2010-07-27 goto]���ش˺�����һ�ο��Խ��׶�����Ʒ
	long ExecChg(UINT nTradePoint,const CString& strServerName,int nTradeType);
	int  ExecServerChk(int server = 1,int action = 1,int data = 0);
	int	 ExecMaskDo(int data,int action ,int check);
	UINT ExecSupply(UINT nTradePoint,const CString& strServerName,int idWebSite,int nTradeType);///��ֵ,�����ͬ��
	UINT ExecSupply(UINT nTradePoint,const CString& strServerName,int idWebSite,int nTradeType,UINT nMoney);///��ֵ,����Ϸ�Ҷһ�,�����ͬ��
//protected:
	BOOL WriteConsumeLog(const CString& strAccountName,OBJID UserID,OBJID ItemID,UINT ItemAmount,UINT nTradePoint,const CString& strServerName);///д���Ѽ�¼��,��ExecTrade����
	BOOL WriteSupplyLog(UINT idWebSite,const CString& strAccountName,const CString& strServerName,int nSupplyType,UINT nTradePoint);///д��ֵ��¼��,��ExecSupply����
	BOOL WriteSupplyLog(UINT idWebSite,const CString& strAccountName,const CString& strServerName,int nSupplyType,UINT nTradePoint,UINT nMoney);///д��ֵ��¼��,��ExecSupply����,����Ϸ�Ҷһ�
	BOOL WriteLifeTimeContinueLog(const CString& strAccountName,OBJID UserID,OBJID ItemID,UINT ItemAmount,UINT nTradePoint,const CString& strServerName, UINT uType);	//  [2007-9-15] ���	
	BOOL WriteVasTicketLog(const CString& strAccountName,int nOperType,OBJID ItemTypeID,UINT nTradePoint,OBJID UserID,const CString& strServerName);///Ԫ��Ʊ��¼
private:
	OBJID	m_idAccount;///�ʺ�
	OBJID	m_idPlayer;///��ɫID
	CString	m_strServerName;///��Ϸ����������
	CMyRecordset*		m_pRes;///��¼��������ڷ������ݿ�
	//char				m_szSQL[256];
	CString	m_strSQL;///SQL���
};

#endif // !defined(AFX_VASDBMANAGER_H__C2B1E812_971D_4C10_BA86_68C4A3B61EF4__INCLUDED_)
