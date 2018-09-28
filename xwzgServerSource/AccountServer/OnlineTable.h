// OnlineTable.h: interface for the COnlineTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ONLINETABLE_H__81A9486E_D360_4B8A_9028_CF9E67FF50EF__INCLUDED_)
#define AFX_ONLINETABLE_H__81A9486E_D360_4B8A_9028_CF9E67FF50EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "T_Index.h"
#include "string"

using namespace std;

enum { c_typeNone = 0, c_typePoint, c_typeTime, c_typeNetBarPoint, c_typeNetBarTime, c_typeISP, c_typeFree, c_typeAll };

class COnlineAccount
{
public:
	COnlineAccount();
	virtual ~COnlineAccount();
public:
	// �ޡ�����Ԫ����
protected:
	void	Init();
	void	Init(int nState, 
				  OBJID idAccount, 
				  DWORD nAuthenID, 
				  LPCTSTR	pszLoginName, 
				  LPCTSTR	pszPassword, 
				  LPCTSTR	pszClientIP, 
				  LPCTSTR	pszServerName, 
				  int		nPointType, 
				  OBJID		idFeeAccount, 
				  LPCTSTR	szFeeAccount);
protected:
	enum { c_stateOnlineNone = 0, c_stateOnlineNormal, c_stateOnlineRejoin };
	int		m_nState;				// ��0��ʾ����

	OBJID	m_idAccount;			// ����
	DWORD	m_nAuthenID;
	char 	m_szLoginName[_MAX_ACCOUNT_NAMESIZE];
	char 	m_szPassword[IPSTRSIZE];
//? ��Ϸ������δ�ϴ�������	OBJID	m_idRole;

	char	m_szClientIP[IPSTRSIZE];
	char	m_szServerName[SERVERNAMESIZE];

	int		m_nFeeType;			// �ﲹ�ǵı�������Ϊc_typeNone��enum { c_typeNone = 0, c_typePoint, ..., c_typeAll };
	OBJID	m_idFeeAccount;						// �Ʒ��ʺš���������ɵ�¼��Ϊ�����ʺ���������ͬ����ʺ�����
	char	m_szFeeAccount[IPSTRSIZE];			// �Ʒ��ʺš���������ɵ�¼��Ϊ�����ʺ���������ͬ����ʺ�����
	time_t	m_tStartPoint;
	time_t	m_tLastPoint;
	time_t	m_tEndPoint;

	enum EEndType { c_endNone = 0, c_endNormal, c_endRejoin, c_endGameServerKick, c_endAccountRejoinKick, 
					c_endAccountOvertimeKick, c_endAccountCloseKick, c_endGameServerLose, c_endAll } m_nEndType;
	int		m_nPointCount;
	time_t	m_tHeartbeat;
	time_t	m_tTimeWait;

	DWORD	m_dwSerial;
	
	UINT	m_AccountTimeout;///���ʺű��ߺ����ó�ʱ,Ĭ��Ϊ1����
	friend	class COnlineTable;
};

class COnlineTable
{
public:
	COnlineTable(int nSize);
	~COnlineTable();
public:
//	COnlineAccount &	operator [](int nIndex);
	typedef	CIndex<DWORD, int, 0>	SERIAL_INDEX;
	void	SetAccountTimeout(OBJID idAccount);///��������ʺų�ʱɾ������ֹ��Ϸ�������������Կ��Ե�¼
	UINT	GetAccountTimeout(OBJID idAccount);///��������ʺų�ʱɾ������ֹ��Ϸ�������������Կ��Ե�¼
	bool	IsAccountTimeout(OBJID idAccount);///�ж�����ʺ��Ƿ��Ѿ��߳���ʱ
 	void	RemoveAccount(OBJID idAccount);///ɾ���ʺ�
	// ����OnlineIndex
	bool	GetAttr(const char * szAccount, OBJID &idAccount, char * szPassword, 
					DWORD &nAuthenID, char * szServerName, char * szFeeAccount, OBJID &idFeeAccount, char* szOldIP);
	bool	GetAttr2(OBJID idAccount, int &nFeeType, char * szServerName, 
												OBJID &idFeeAccount, char* szFeeAccount);
	int 	AddNew(OBJID idAccount, DWORD nAuthenID, const char * pClientIP, const char * szLoginName
				, const char * szPassword, int nPointType, const char * szServerName, OBJID	idFeeAccount, LPCTSTR szFeeAccount);		// return 0: error
	int 	AppendNew(OBJID idAccount, DWORD nAuthenID, const char * pClientIP, const char * szLoginName
				, const char * szPassword, int nPointType, const char * szServerName, OBJID	idFeeAccount, LPCTSTR szFeeAccount);			// return 0: error
	int 	Rejoin(OBJID idAccount, DWORD nAuthenID, const char * pClientIP, const char * szLoginName
				, const char * szPassword, int nPointType, const char * szServerName, OBJID	idFeeAccount, LPCTSTR szFeeAccount);			// return 0: error
	int		StartFee(OBJID idAccount, const char* pszServerName);		// return -n: ��������ƥ��
	int 	EndFee(OBJID idAccount, const char* pszServerName, bool bOffline);			//? ͬʱɾ����Ա	// return -n: ��������ƥ��
	int		PointFee(OBJID idAccount, const char* pszServerName);		// return -n: ��������ƥ��
	int		UseTick(OBJID idAccount, const char* pszServerName);		// �ͻ����Ѿ�ʹ��������Ʊ��
	int		GetPlayerCount(LPCTSTR pszServerName = NULL){
		LOCKTHREAD; 
		if(pszServerName == NULL) return m_nCount;
		else return m_aPlayerCount[pszServerName];
	}
	SERIAL_INDEX*	GetSerialSet()			{ return &m_aSerialCount; }
	void	SetPassword(OBJID idAccount, const char * szPassword)		// ���ڲ��ǵ�����ص�¼ʱ��ӿ���
	{ 
		LOCKTHREAD; 
		int	nIndex = FindIndex(idAccount);
		if(nIndex && strlen(szPassword) < _MAX_PSWSIZE)
			strcpy(m_pAccount[nIndex].m_szPassword, szPassword); 
		else
			LOGERROR("SetPassword([%s]) �ڲ��쳣", szPassword);
	}
	void	SetSerial(OBJID idAccount, DWORD dwSerial)		// ���ڲ��ǵ�����ص�¼ʱ��ӿ���
	{ 
		LOCKTHREAD; 
		int	nIndex = FindIndex(idAccount);
		if(nIndex && dwSerial < 10000)				// serial ������9999
		{
			m_pAccount[nIndex].m_dwSerial	= dwSerial; 
			m_aSerialCount.Add(dwSerial, m_aSerialCount[dwSerial] + 1);
		}
		else
			LOGERROR("SetSerial([%d]) �쳣", dwSerial);
	}
	bool	IsOnline(OBJID idAccount) { LOCKTHREAD; return FindIndex(idAccount) != 0; }
	bool	IsTimeWait(OBJID idAccount) {
		LOCKTHREAD;
		int nIndex = FindIndex(idAccount);
		return nIndex && m_pAccount[nIndex].m_tTimeWait;
	}
	bool	RemoveOvertime();
	bool	GetServerName(OBJID idAccount, char* bufServer);
	int		GetFeeCount(OBJID idFeeAccount);
	bool	CheckUniqueIP(LPCTSTR pClientIP, OBJID idAccount);

	void	RemoveByServerName( char* pServerName );// ������� [5/30/2007]
protected:
	COnlineAccount *	m_pAccount;
	typedef	CIndex<OBJID, int, 0>	INDEXID;
	INDEXID		m_idxID;		// ��������ע�������߱�ͬ��
	typedef	CIndex<string, int, 0>	INDEXNAME;
	INDEXNAME	m_idxName;
	int					m_nCount;
	CIndex<string, int, 0>	m_aPlayerCount;
	SERIAL_INDEX	m_aSerialCount;
	CCriticalSection	m_xCtrl;
private:
	int		FindIndex(const char * szLoginName);		// return 0: ERROR
	int		FindIndex(OBJID idAccount);					// return 0: ERROR
	int		FindFree(OBJID idAccount);					// return 0: ERROR
	void	Remove_0(int nIndex);
	void	LogFee(int nIndex, enum COnlineAccount::EEndType nEndType);
};

#endif // !defined(AFX_ONLINETABLE_H__81A9486E_D360_4B8A_9028_CF9E67FF50EF__INCLUDED_)
