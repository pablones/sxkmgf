// OnlineTable.cpp: implementation of the COnlineTable class.
//
//////////////////////////////////////////////////////////////////////

#include "AllHeads.h"
#include "OnlineTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const DWORD	INVALID_SERIAL			= 1234567890;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// COnlineAccount

COnlineAccount::COnlineAccount()
{
	Init();
}

COnlineAccount::~COnlineAccount()
{
}

void COnlineAccount::Init()
{
	m_nState			= c_stateOnlineNone;
	m_idAccount			= 0;			// ����
	m_szLoginName[0]	= 0;
	m_szPassword[0]		= 0;
//? ��Ϸ������δ�ϴ�������	m_idRole			= 0;
	m_nAuthenID			= 0;
	m_szClientIP[0]		= 0;
	m_szServerName[0]	= 0;
	m_nFeeType			= 0;
	m_idFeeAccount		= ID_NONE;
	m_szFeeAccount[0]	= 0;
	m_tStartPoint		= 0;
	m_tLastPoint		= 0;
	m_tHeartbeat		= 0;
	m_tTimeWait			= 0;
	m_tEndPoint			= 0;
	m_nEndType			= c_endNone;
	m_nPointCount		= 0;
	m_dwSerial			= INVALID_SERIAL;
}

void COnlineAccount::Init(int nState, 
						  OBJID		idAccount, 
						  DWORD		nAuthenID, 
						  LPCTSTR	pszLoginName, 
						  LPCTSTR	pszPassword, 
						  LPCTSTR	pszClientIP, 
						  LPCTSTR	pszServerName, 
						  int		nFeeType,
						  OBJID		idFeeAccount, 
						  LPCTSTR	szFeeAccount)
{
	ASSERT(nState);
	ASSERT(idAccount);
// Ŀǰ���ݿ����пմ��ʺ�	ASSERT(pszLoginName[0]);

	m_nState			= nState;

	m_idAccount			= idAccount;
	m_nAuthenID			= nAuthenID;
	SafeCopy(m_szLoginName, pszLoginName, _MAX_ACCOUNT_NAMESIZE);
	SafeCopy(m_szPassword, pszPassword, _MAX_NAMESIZE);

	SafeCopy(m_szClientIP, pszClientIP, IPSTRSIZE);
	SafeCopy(m_szServerName, pszServerName, _MAX_NAMESIZE);

	m_nFeeType			= nFeeType;
	m_idFeeAccount		= idFeeAccount;
	SafeCopy(m_szFeeAccount, szFeeAccount, _MAX_ACCOUNT_NAMESIZE);
	m_tStartPoint		= 0;
	m_tLastPoint		= 0;
	m_tEndPoint			= 0;

	m_nEndType			= c_endNone;
	m_nPointCount		= 0;
	m_tHeartbeat		= clock();
	m_tTimeWait			= 0;

	m_dwSerial			= INVALID_SERIAL;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// COnlineTable
COnlineTable::COnlineTable(int nSize)
{ 
	LOCKTHREAD;

	m_pAccount = new COnlineAccount[nSize]; 

	m_nCount = 0; 

//	for(int i = 0; i < MAXGAMESERVERS; i++)
//		m_nCountList[i] = 0; 
}

COnlineTable::~COnlineTable()
{
	LOCKTHREAD;

	LOGMSG("���߱�����ʱʣ��[%d]��Ԫ��", m_nCount);
#ifdef	TEST_INDEX
	ASSERT(m_idxID.Size() == m_nCount);
	ASSERT(m_idxName.Size() == m_nCount);
#endif
	for(int nIndex = 1; nIndex < ONLINETABLESIZE; nIndex++)		// 1: ��1��ʼ
	{
		if(m_pAccount[nIndex].m_nState)
		{
			m_pAccount[nIndex].m_tEndPoint	= time(NULL);
			LogFee(nIndex, COnlineAccount::c_endAccountCloseKick);
			Remove_0(nIndex);
		}
	}
#ifdef	TEST_INDEX
	ASSERT(m_idxID.Size() == m_nCount);
	ASSERT(m_idxName.Size() == m_nCount);
#endif

	LOGMSG("���߱���պ�ʣ��[%d]��Ԫ��", m_nCount);

	if(m_nCount)
		LOGWARNING("~COnlineTable() �쳣��m_nCount��Ϊ0[%d]", m_nCount);
#ifdef	TEST_INDEX
	if(m_idxName.Size())
	{
		LOGWARNING("idxName.Size() : %d", m_idxName.Size());
		for(INDEXNAME::iterator j = m_idxName.Begin(); j != m_idxName.End(); j++)
		{
			LOGWARNING("---> name[%s], index[%d]", m_idxName.Key(j), m_idxName.Data(j));
		}
	}
	if(m_idxID.Size())
	{
		LOGWARNING("idxID.Size() : %d", m_idxID.Size());
		for(INDEXID::iterator i = m_idxID.Begin(); i != m_idxID.End(); i++)
		{
			LOGWARNING("---> id[%d], index[%d]", m_idxID.Key(i), m_idxID.Data(i));
		}
	}
#endif

	m_aPlayerCount.ClearAll();
	if(STAT_SERIAL)
		m_aSerialCount.ClearAll();
	delete [] m_pAccount; 
}
////////////////////////////////////////////////////////////////////////////////////////////////
// COnlineTable
bool	COnlineTable::GetAttr(const char * szAccount, OBJID &idAccount, char * szPassword, 
					  DWORD &nAuthenID, char * szServerName, char* szFeeAccount, OBJID &idFeeAccount, char* szOldIP)
{
	LOCKTHREAD;

	int nIndex = FindIndex(szAccount);		// return 0: ERROR
	if(nIndex && m_pAccount[nIndex].m_nState)
	{
		idAccount		= m_pAccount[nIndex].m_idAccount;
		SafeCopy(szPassword, m_pAccount[nIndex].m_szPassword, _MAX_NAMESIZE);
		nAuthenID		= m_pAccount[nIndex].m_nAuthenID;
		SafeCopy(szServerName, m_pAccount[nIndex].m_szServerName, _MAX_NAMESIZE);
		SafeCopy(szFeeAccount, m_pAccount[nIndex].m_szFeeAccount, _MAX_ACCOUNT_NAMESIZE);
		idFeeAccount	= m_pAccount[nIndex].m_idFeeAccount;
		SafeCopy(szOldIP, m_pAccount[nIndex].m_szClientIP, _MAX_NAMESIZE);
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
bool	COnlineTable::GetAttr2(OBJID idAccount, int &nFeeType, char * szServerName, 
												OBJID &idFeeAccount, char* szFeeAccount)
{
	LOCKTHREAD;

	int nIndex = FindIndex(idAccount);		// return 0: ERROR
	if(nIndex && m_pAccount[nIndex].m_nState)
	{
		nFeeType		= m_pAccount[nIndex].m_nFeeType;
		SafeCopy(szServerName, m_pAccount[nIndex].m_szServerName, _MAX_NAMESIZE);
		idFeeAccount	= m_pAccount[nIndex].m_idFeeAccount;
		SafeCopy(szFeeAccount, m_pAccount[nIndex].m_szFeeAccount, _MAX_ACCOUNT_NAMESIZE);
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
#define		IDXASSERT(x,y,z)	{if(!(x)) LOGERROR("ASSERT(" #x ") -> [%s][%d] in %d line of %s", y, z, __LINE__, __FILE__); }
////////////////////////////////////////////////////////////////////////////////////////////////////////
int 	COnlineTable::AddNew(OBJID idAccount, DWORD nAuthenID, const char * pClientIP, const char * szLoginName
			, const char * szPassword, int nPointType, const char * szServerName, OBJID	idFeeAccount, LPCTSTR szFeeAccount)			// return 0: error
{
	LOCKTHREAD;

	int	nFreeIndex = FindFree(idAccount);		// return 0: ��������д��ʺ�
	if(nFreeIndex)
	{
		m_nCount++;
		m_aPlayerCount.Add(szServerName, m_aPlayerCount[szServerName] + 1);
		m_pAccount[nFreeIndex].Init(COnlineAccount::c_stateOnlineNormal,
									idAccount,
									nAuthenID,
									szLoginName,
									szPassword,
									pClientIP,
									szServerName,
									nPointType, 
									idFeeAccount,
									szFeeAccount);
#ifdef	TEST_INDEX
		int	nOldIDCount = m_idxID.Size();
		int	nOldNameCount = m_idxName.Size();
		IDXASSERT(!m_idxID.IsIn(idAccount), szLoginName, idAccount);
		IDXASSERT(!m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif
		m_idxID.Add(idAccount, nFreeIndex);
		m_idxName.Add(szLoginName, nFreeIndex);
#ifdef	TEST_INDEX
		IDXASSERT(m_idxID.Size() == nOldIDCount+1, szLoginName, idAccount);
		IDXASSERT(m_idxName.Size() == nOldNameCount+1, szLoginName, idAccount);
		IDXASSERT(m_idxID.IsIn(idAccount), szLoginName, idAccount);
		IDXASSERT(m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif

		return nFreeIndex;
	}
	else
	{
		LOGWARNING("ADDNEW()ʱ�����ͻ���ʺ�[%s]���ܻ��ظ���¼����ΪREJOIN", szLoginName);

		// ���Ǿ�����
		//20070702����:��Ȼ֮ǰ��int	nFreeIndex = FindFree(idAccount);�õ�
		//�˴�Ҳ��Ϊ��idAccount�õ�.
		//�����������Կ���,nIndexΪ0,������Ч
		//--------------------------------------------------------------------------
		//int		nIndex = FindIndex(szLoginName);
		//ASSERT(nIndex == FindIndex(idAccount));
		int		nIndex = FindIndex(idAccount);
		ASSERT(nIndex == FindIndex(szLoginName));
		//ASSERT(nIndex);
		if(!nIndex)
		{
			//δ�鵽ʱ����.����
			ASSERT(!"nIndex == 0");
			return 0;
		}
		//--------------------------------------------------------------------------
		if(m_pAccount[nIndex].m_tStartPoint)
		{
			m_pAccount[nIndex].m_tHeartbeat = clock();
			m_pAccount[nIndex].m_tEndPoint	= time(NULL);
			LogFee(nIndex, COnlineAccount::c_endAccountRejoinKick);
		}

		ASSERT(m_pAccount[nIndex].m_idAccount == idAccount);
		ASSERT(strcmp(m_pAccount[nIndex].m_szLoginName, szLoginName) == 0);
		m_pAccount[nIndex].Init(COnlineAccount::c_stateOnlineRejoin,
									idAccount,
									nAuthenID,
									szLoginName,
									szPassword,
									pClientIP,
									szServerName,
									nPointType, 
									idFeeAccount,
									szFeeAccount);
#ifdef	TEST_INDEX
		int	nOldIDCount = m_idxID.Size();
		int	nOldNameCount = m_idxName.Size();
		IDXASSERT(m_idxID.IsIn(idAccount), szLoginName, idAccount);
		IDXASSERT(m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif
		ASSERT(m_idxID.IsIn(idAccount));
		ASSERT(m_idxName.IsIn(szLoginName));
		m_idxID.Add(idAccount, nIndex);
		m_idxName.Add(szLoginName, nIndex);
#ifdef	TEST_INDEX
		IDXASSERT(m_idxID.Size() == nOldIDCount, szLoginName, idAccount);
		IDXASSERT(m_idxName.Size() == nOldNameCount, szLoginName, idAccount);
		IDXASSERT(m_idxID.IsIn(idAccount), szLoginName, idAccount);
		IDXASSERT(m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif

		return nIndex;
	}
}

// ����һ���ʺ�
int 	COnlineTable::AppendNew(OBJID idAccount, DWORD nAuthenID, const char * pClientIP, const char * szLoginName
			, const char * szPassword, int nPointType, const char * szServerName, OBJID	idFeeAccount, LPCTSTR szFeeAccount)	// return 0: error
{
	LOCKTHREAD;

//��ΪPUBLIC	RemoveOvertime();	//????? ��û������ҵ�¼ʱ����Զ�����鳬ʱ����ʱ��Ŀ��ֻ�ǻ������߱���Դ���ӳټ���ϵ����
	int	nFreeIndex = FindFree(idAccount);		// return 0: ��������д��ʺ�
	if(nFreeIndex)
	{
		m_nCount++;
		m_aPlayerCount.Add(szServerName, m_aPlayerCount[szServerName] + 1);
		m_pAccount[nFreeIndex].Init(COnlineAccount::c_stateOnlineRejoin,
									idAccount,
									nAuthenID,
									szLoginName,
									szPassword,
									pClientIP,
									szServerName,
									nPointType, 
									idFeeAccount,
									szFeeAccount);
#ifdef	TEST_INDEX
		int	nOldIDCount = m_idxID.Size();
		int	nOldNameCount = m_idxName.Size();
		IDXASSERT(!m_idxID.IsIn(idAccount), szLoginName, idAccount);
		IDXASSERT(!m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif
		m_idxID.Add(idAccount, nFreeIndex);
		m_idxName.Add(szLoginName, nFreeIndex);
#ifdef	TEST_INDEX
		IDXASSERT(m_idxID.Size() == nOldIDCount+1, szLoginName, idAccount);
		IDXASSERT(m_idxName.Size() == nOldNameCount+1, szLoginName, idAccount);
		IDXASSERT(m_idxID.IsIn(idAccount), szLoginName, idAccount);
		IDXASSERT(m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif

		return nFreeIndex;
	}
	else
	{
		LOGWARNING("APPENDNEW()ʱ�����ͻ���ʺ�[%s]����REJOIN", szLoginName);

		// ���Ǿ�����


		//20070702����:��Ȼ֮ǰ��int	nFreeIndex = FindFree(idAccount);�õ�
		//�˴�Ҳ��Ϊ��idAccount�õ�.
		//�����������Կ���,nIndexΪ0,������Ч
		//--------------------------------------------------------------------------
		//int		nIndex = FindIndex(szLoginName);
		//ASSERT(nIndex == FindIndex(idAccount));
		int		nIndex = FindIndex(idAccount);
		ASSERT(nIndex == FindIndex(szLoginName));
		//ASSERT(nIndex);
		if(!nIndex)
		{
			//δ�鵽ʱ����.����
			ASSERT(!"nIndex == 0");
			return 0;
		}
		//--------------------------------------------------------------------------
		if(m_pAccount[nIndex].m_tStartPoint)
		{
			m_pAccount[nIndex].m_tHeartbeat = clock();
			m_pAccount[nIndex].m_tEndPoint	= time(NULL);
			LogFee(nIndex, COnlineAccount::c_endAccountRejoinKick);
		}

		ASSERT(m_pAccount[nIndex].m_idAccount == idAccount);
		ASSERT(strcmp(m_pAccount[nIndex].m_szLoginName, szLoginName) == 0);
		m_pAccount[nIndex].Init(COnlineAccount::c_stateOnlineRejoin,
									idAccount,
									m_pAccount[nIndex].m_nAuthenID,
									szLoginName,
									m_pAccount[nIndex].m_szPassword,
									m_pAccount[nIndex].m_szClientIP,
									szServerName,
									nPointType, 
									idFeeAccount,
									szFeeAccount);
#ifdef	TEST_INDEX
		int	nOldIDCount = m_idxID.Size();
		int	nOldNameCount = m_idxName.Size();
		IDXASSERT(m_idxID.IsIn(idAccount), szLoginName, idAccount);
		IDXASSERT(m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif
		m_idxID.Add(idAccount, nIndex);
		m_idxName.Add(szLoginName, nIndex);
#ifdef	TEST_INDEX
		IDXASSERT(m_idxID.Size() == nOldIDCount, szLoginName, idAccount);
		IDXASSERT(m_idxName.Size() == nOldNameCount, szLoginName, idAccount);
		IDXASSERT(m_idxID.IsIn(idAccount), szLoginName, idAccount);
		IDXASSERT(m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif

		return nIndex;
	}
}

int 	COnlineTable::Rejoin(OBJID idAccount, DWORD nAuthenID, const char * pClientIP, const char * szLoginName
				, const char * szPassword, int nPointType, const char * szServerName, OBJID	idFeeAccount, LPCTSTR szFeeAccount)			// return 0: error
{
	LOCKTHREAD;

	int		nIndex = FindIndex(szLoginName);
	ASSERT(nIndex == FindIndex(idAccount));
	if(nIndex)
	{
		if(m_pAccount[nIndex].m_tStartPoint)
		{
			m_pAccount[nIndex].m_tHeartbeat = clock();
			m_pAccount[nIndex].m_tEndPoint	= time(NULL);
			LogFee(nIndex, COnlineAccount::c_endAccountRejoinKick);
		}

		ASSERT(strcmp(m_pAccount[nIndex].m_szPassword, szPassword) == 0);

		ASSERT(m_pAccount[nIndex].m_idAccount == idAccount);
		ASSERT(strcmp(m_pAccount[nIndex].m_szLoginName, szLoginName) == 0);
		m_pAccount[nIndex].Init(COnlineAccount::c_stateOnlineRejoin,
									idAccount,
									nAuthenID,
									szLoginName,
									szPassword,
									pClientIP,
									szServerName,
									nPointType, 
									idFeeAccount,
									szFeeAccount);
#ifdef	TEST_INDEX
		int	nOldIDCount = m_idxID.Size();
		int	nOldNameCount = m_idxName.Size();
		IDXASSERT(m_idxID.IsIn(idAccount), szLoginName, idAccount);
		IDXASSERT(m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif
		m_idxID.Add(idAccount, nIndex);
		m_idxName.Add(szLoginName, nIndex);
#ifdef	TEST_INDEX
		IDXASSERT(m_idxID.Size() == nOldIDCount, szLoginName, idAccount);
		IDXASSERT(m_idxName.Size() == nOldNameCount, szLoginName, idAccount);
		IDXASSERT(m_idxID.IsIn(idAccount), szLoginName, idAccount);
		IDXASSERT(m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif

		return nIndex;
	}
	else
	{
		LOGWARNING("REJOIN()ʱ�����ͻ�����[%s]��ΪADDNEW", szLoginName);

		int	nFreeIndex = FindFree(idAccount);		// return 0: ��������д��ʺ�
		if(nFreeIndex)
		{
			m_nCount++;
			m_aPlayerCount.Add(szServerName, m_aPlayerCount[szServerName] + 1);
			m_pAccount[nFreeIndex].Init(COnlineAccount::c_stateOnlineNormal,
										idAccount,
										nAuthenID,
										szLoginName,
										szPassword,
										pClientIP,
										szServerName,
										nPointType, 
										idFeeAccount,
										szFeeAccount);
#ifdef	TEST_INDEX
		int	nOldIDCount = m_idxID.Size();
		int	nOldNameCount = m_idxName.Size();
		IDXASSERT(!m_idxID.IsIn(idAccount), szLoginName, idAccount);
		IDXASSERT(!m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif
			m_idxID.Add(idAccount, nFreeIndex);
			m_idxName.Add(szLoginName, nFreeIndex);
#ifdef	TEST_INDEX
		IDXASSERT(m_idxID.Size() == nOldIDCount+1, szLoginName, idAccount);
		IDXASSERT(m_idxName.Size() == nOldNameCount+1, szLoginName, idAccount);
		IDXASSERT(m_idxID.IsIn(idAccount), szLoginName, idAccount);
		IDXASSERT(m_idxName.IsIn(szLoginName), szLoginName, idAccount);
#endif

			return nFreeIndex;
		}
		else
		{
			LOGERROR("Rejoin()�ڲ��쳣�����߱�����[%d/%d]", GetPlayerCount(), ONLINETABLESIZE);
			return 0;
		}
	}
}

//?? ����������ǰ�������ߡ��Ƶ��̸߳�Ϊ���߳�ʱ��BUG
int  COnlineTable::StartFee(OBJID idAccount, const char* pszServerName)		// return -n: ��������ƥ��
{
	LOCKTHREAD;

	int nIndex = FindIndex(idAccount);		// return 0: ERROR
	ASSERT(nIndex);
	if(!nIndex)
		return -1;

	// ���˫�ص�¼
	ASSERT(m_pAccount[nIndex].m_idAccount == idAccount);
	if(strcmp(m_pAccount[nIndex].m_szServerName, pszServerName))
	{
		LOGERROR("�����STARTFEE�����[%s][%d]��¼��[%s]�������յ�[%s]��STARTFEE��Ϣ", 
					m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount, 
					m_pAccount[nIndex].m_szServerName, pszServerName);
		return -1;
	}

	ASSERT(m_pAccount[nIndex].m_nState != COnlineAccount::c_stateOnlineNone);
	m_pAccount[nIndex].m_tHeartbeat = clock();
	if(m_pAccount[nIndex].m_tStartPoint)
	{
#ifdef	DUMP
		LOGERROR("�ظ����յ�MSG_STARTFEE��Ϣ����Ϸ������Ϊ[%s], ����ʺ�Ϊ[%s]", 
				m_pAccount[nIndex].m_szServerName, m_pAccount[nIndex].m_szLoginName);
#endif
	}
	else
	{
		if(m_pAccount[nIndex].m_nPointCount)
		{
			LOGERROR("�����쳣��[%s]��ҽ��յ�MSG_STARTFEE��Ϣʱ���߱��еĵ���δ���[%d]��", 
				m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_nPointCount);
			m_pAccount[nIndex].m_nPointCount = 0;
		}
		m_pAccount[nIndex].m_tStartPoint	= time(NULL);
	}

	return 0;
}
		
//?? ����������ǰ�������ߡ��Ƶ��̸߳�Ϊ���߳�ʱ��BUG
int  COnlineTable::EndFee(OBJID idAccount, const char* pszServerName, bool bOffline)		// return -n: ��������ƥ��
{
	LOCKTHREAD;

	int nIndex = FindIndex(idAccount);
	if(!nIndex)
	{
		if(g_bEnableLogin && !bOffline)
			LOGWARNING("�����ENDFEE, ��Ϸ������[%s]�ʺ�ID[%d]", pszServerName, idAccount);
		return -1;
	}

	// ���˵��Ǽ�¼��������END_FEE
	ASSERT(m_pAccount[nIndex].m_idAccount == idAccount);
	if(strcmp(pszServerName, m_pAccount[nIndex].m_szServerName))
	{
		if(!bOffline)
			LOGERROR("�����END_FEE�����ԡ����[%s][%d]��[%s]�������յ�[%s]����Ϣ��", 
						m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount, 
						m_pAccount[nIndex].m_szServerName, pszServerName);
		return -1;
	}

//������գ���������	m_pAccount[nIndex].m_nState			= COnlineAccount::c_stateOnlineNone;	// ����
	m_pAccount[nIndex].m_tHeartbeat = clock();
	m_pAccount[nIndex].m_tEndPoint		= time(NULL);
//-------------------07.5.26����ע��----------begin-----------------------------
//-----------��������ֻ��������߱�-----------------
//	if(m_pAccount[nIndex].m_tStartPoint)
//	{
//		if(bOffline)
//			LogFee(nIndex, COnlineAccount::c_endGameServerLose);
//		else if(m_pAccount[nIndex].m_nState == COnlineAccount::c_stateOnlineNormal)
//			LogFee(nIndex, COnlineAccount::c_endNormal);
//		else if(m_pAccount[nIndex].m_nState == COnlineAccount::c_stateOnlineRejoin)
//			LogFee(nIndex, COnlineAccount::c_endRejoin);
//		else
//			LOGERROR("COnlineTable::EndFee(OBJID idAccount) �ڲ��쳣");
//	}
//	else
//	{
//		LogFee(nIndex, COnlineAccount::c_endGameServerKick);
//	}
//
//	int nCount = (m_pAccount[nIndex].m_tEndPoint - m_pAccount[nIndex].m_tStartPoint) / (POINTFEEINTERVALSECS*1000);
//	if(m_pAccount[nIndex].m_tStartPoint && nCount - m_pAccount[nIndex].m_nPointCount > 0)		//? 3: �����
//	{
//		char szStartTime[20];
//		char szPointTime[20];
//		char szEndTime[20];
//		char szStamp[15];
//		DateTime(szStartTime, m_pAccount[nIndex].m_tStartPoint);
//		DateTime(szPointTime, m_pAccount[nIndex].m_tLastPoint);
//		DateTime(szEndTime, m_pAccount[nIndex].m_tEndPoint);
//		time_t	tCurr = time(NULL);
//		tm*	pTm = localtime(&tCurr);
//		sprintf(szStamp, "%04d%02d%02d%02d%02d%02d", pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
//		ASSERT(c_typeAll == 7);
//		ASSERT(COnlineAccount::c_endAll == 8);
//		const char *	aFeeType[] = { "δ֪", "�Ƶ�", "�¿�", "���ɼƵ�", "�����¿�", "ISP", "���", "Error", "Error"  };
//		const char *	aEndType[] = { "δ֪", "�����˳�", "��ʱ�˳�", "��Ϸ��������", "�ظ���¼", 
//								"��ʱ�˳�", "�ʺŷ������ر�", "��¼��Ϣ��ʧ", "Error", "Error" };
//		const char *	pFeeType = aFeeType[0];
//		const char *	pEndType = aEndType[0];
//		if(m_pAccount[nIndex].m_nFeeType >= c_typeNone && m_pAccount[nIndex].m_nFeeType < c_typeAll)
//			pFeeType = aFeeType[m_pAccount[nIndex].m_nFeeType];
//		if(m_pAccount[nIndex].m_nEndType > COnlineAccount::c_endNone && m_pAccount[nIndex].m_nEndType < COnlineAccount::c_endAll)
//			pEndType = aEndType[m_pAccount[nIndex].m_nEndType];
//		LOGERROR("�Ʒ�������[%s]�ϴ�[%d]�Ľ����Ʒ���Ϣ\n"
//					"��ʼʱ��[%s]�����Ƶ�[%s]������ʱ��[%s]��\n"
//					"�Ʒ�����[%s], �Ƶ�����[%d], ��������[%s]", 
//					m_pAccount[nIndex].m_szServerName, m_pAccount[nIndex].m_idAccount, 
//					szStartTime, szPointTime, szEndTime, 
//					pFeeType, m_pAccount[nIndex].m_nPointCount, pEndType);
//	}
//----------------------------------------------------end------------------------------
	// ͬʱɾ����Ա
	ASSERT(m_pAccount[nIndex].m_nState);
	if(m_pAccount[nIndex].m_nState == COnlineAccount::c_stateOnlineNormal || bOffline || REJOINTIMEWAITSECS == 0)
		Remove_0(nIndex);
	else
		m_pAccount[nIndex].m_tTimeWait = clock() + REJOINTIMEWAITSECS*CLOCKS_PER_SEC;

	return 0;
}

//?? ����������ǰ�������ߡ��Ƶ��̸߳�Ϊ���߳�ʱ��BUG																									
int COnlineTable::PointFee(OBJID idAccount, const char* pszServerName)		// return -n: ��������ƥ��
{
	LOCKTHREAD;

	int nIndex = FindIndex(idAccount);
	if(!nIndex)
	{
		LOGERROR("COnlineTable::PointFee(OBJID idAccount) �ڲ��쳣");
		return	-1;		// û�и���ҵ����߱�(�ڲ��쳣)
	}

	// �������POINT_FEE
	ASSERT(m_pAccount[nIndex].m_idAccount == idAccount);
	if(strcmp(m_pAccount[nIndex].m_szServerName, pszServerName))
	{
		LOGERROR("�����POINT_FEE�����[%s][%d]��¼��[%s]�������յ�[%s]����Ϣ��IP��ַ[%s]", 
					m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount, 
					m_pAccount[nIndex].m_szServerName, pszServerName, 
					m_pAccount[nIndex].m_szClientIP);
		return -1;		// -1: ����
	}

	ASSERT(m_pAccount[nIndex].m_nState != COnlineAccount::c_stateOnlineNone);
	m_pAccount[nIndex].m_tHeartbeat = clock();
	if(!m_pAccount[nIndex].m_tStartPoint)
	{
#ifdef	DUMP
		LOGWARNING("���յ�POINTFEE��Ϣ֮ǰû���յ�STARTFEE��Ϣ���ʺ�ID[%d]�������ǲ�����ʺ�", m_pAccount[nIndex].m_idAccount);
#endif
		m_pAccount[nIndex].m_tStartPoint	= time(NULL);
	}
	m_pAccount[nIndex].m_tLastPoint	= time(NULL);
	m_pAccount[nIndex].m_nPointCount++;
//	LOGPOINT("���[%d]�ı��μƵ������ۼƵ�[%d]", m_pAccount[nIndex].m_idAccount, m_pAccount[nIndex].m_nPointCount);

	int nRet = m_pAccount[nIndex].m_nPointCount;
	if(nRet > 0)
		return nRet;
	else
		return 0;
}

// ��鲢ɾ�����г�ʱ�����߱�
bool	COnlineTable::RemoveOvertime()
{
	LOCKTHREAD;

	bool	ret = false;
	for(int nIndex = 1; nIndex < ONLINETABLESIZE; nIndex++)	// 1: 0�ŵ�Ԫ����
	{
		// ���TIME_WAIT
		if(m_pAccount[nIndex].m_nState && m_pAccount[nIndex].m_tTimeWait && clock() > m_pAccount[nIndex].m_tTimeWait)
		{
			Remove_0(nIndex);
			ret = true;
		}

		// ��鳬ʱ
		if(m_pAccount[nIndex].m_nState 
					&& clock() > m_pAccount[nIndex].m_tHeartbeat+ONLINEHEARTBEATSECS*CLOCKS_PER_SEC)
		{
			m_pAccount[nIndex].m_tEndPoint	= time(NULL);
			LogFee(nIndex, COnlineAccount::c_endAccountOvertimeKick);
			Remove_0(nIndex);
			ret = true;
		}
	}
	return ret;
}

bool	COnlineTable::GetServerName(OBJID idAccount, char* bufServer) 
{ 
	LOCKTHREAD; 

	int		nIndex = FindIndex(idAccount);
	if(nIndex)
	{
		SafeCopy(bufServer, m_pAccount[nIndex].m_szServerName, _MAX_NAMESIZE);
		return true;
	}
	else
		return false;
}

//? ���Ż�
int	COnlineTable::GetFeeCount(OBJID idFeeAccount)
{
	LOCKTHREAD;

	int 	nCount = 0;
	for(int nIndex = 1; nIndex < ONLINETABLESIZE; nIndex++)	// 1: 0�ŵ�Ԫ����
	{
		if(m_pAccount[nIndex].m_nState && m_pAccount[nIndex].m_idFeeAccount == idFeeAccount)
			nCount++;
	}
	return nCount;
}

bool COnlineTable::CheckUniqueIP(LPCTSTR pClientIP, OBJID idAccount)
{
	LOCKTHREAD;

	ASSERT(!( pClientIP == 0 || *pClientIP == 0 ));
	if(pClientIP == 0 || *pClientIP == 0)
		return false;

	for(int nIndex = 1; nIndex < ONLINETABLESIZE; nIndex++)	// 1: 0�ŵ�Ԫ����
	{
		if(m_pAccount[nIndex].m_nFeeType == c_typeISP 
				&& strcmp(m_pAccount[nIndex].m_szClientIP, pClientIP) == 0 
				&& m_pAccount[nIndex].m_idAccount != idAccount)
			return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// private

//?? �ú�����Ҫ�Ż�
int		COnlineTable::FindIndex(const char * szLoginName)		// return 0: ERROR
{
// �ڲ�����	LOCKTHREAD;

#ifdef	INDEXOPTIMIZE
	int	nIndex = m_idxName[szLoginName];
	if(!nIndex || strcmp(m_pAccount[nIndex].m_szLoginName, szLoginName) == 0)
		return nIndex;
	LOGERROR("���߱�������ͬ�����ʺ���[%s]������Ϊ[%d]��������[%d]���ʺ���Ϊ[%s]��������ɾ������", 
					szLoginName, nIndex, nIndex, m_pAccount[nIndex].m_szLoginName);
	m_idxName.Del(szLoginName);
	return 0;
#endif

	for(int i = 1; i < ONLINETABLESIZE; i++)	// 1: 0�ŵ�Ԫ����
	{
		if(m_pAccount[i].m_nState && strcmp(m_pAccount[i].m_szLoginName, szLoginName) == 0)
		{
			if(i != m_idxName[szLoginName])
				LOGERROR("TEST: ��¼��[%s]������ͬ��[%d][%d]���ڲ��쳣", szLoginName, i, m_idxName[szLoginName]);

			return i;
		}
	}

	if(m_idxName[szLoginName])
		LOGERROR("TEST: ��¼��[%s]������ͬ��[%d]���ڲ��쳣.", szLoginName, m_idxName[szLoginName]);

	return 0;
}

//?? �ú�����Ҫ�Ż�
int		COnlineTable::FindIndex(OBJID idAccount)		// return 0: ERROR
{
// �ڲ�����	LOCKTHREAD;

#ifdef	INDEXOPTIMIZE
	int	nIndex = m_idxID[idAccount];
	if(!nIndex || m_pAccount[nIndex].m_idAccount == idAccount)
		return nIndex;
	LOGERROR("���߱�������ͬ�����ʺ�ID[%d]������Ϊ[%d]��������[%d]���ʺ�IDΪ[%d]��������ɾ������", 
					idAccount, nIndex, nIndex, m_pAccount[nIndex].m_idAccount);
	m_idxID.Del(idAccount);
	return 0;
#endif

	for(int i = 1; i < ONLINETABLESIZE; i++)	// 1: 0�ŵ�Ԫ����
	{
		if(m_pAccount[i].m_nState && m_pAccount[i].m_idAccount == idAccount)
		{
			if(i != m_idxID[idAccount])
				LOGERROR("TEST: �ʺ�ID[%d]������ͬ��[%d][%d]���ڲ��쳣", idAccount, i, m_idxID[idAccount]);

			return i;
		}
	}

	if(m_idxID[idAccount])
		LOGERROR("TEST: �ʺ�ID[%d]������ͬ��[%d]���ڲ��쳣.", idAccount, m_idxID[idAccount]);

	return 0;
}

//?? �ú�����Ҫ�Ż�
int		COnlineTable::FindFree(OBJID idAccount)		// return 0: ��������д��ʺ�
{
// �ڲ�����	LOCKTHREAD;

	int	nFreeIndex = 0;
	for(int i = 1; i < ONLINETABLESIZE; i++)	// 1: 0�ŵ�Ԫ����
	{
		if(m_pAccount[i].m_nState && m_pAccount[i].m_idAccount == idAccount)
		{
			LOGERROR("��ͼ�ظ����������ұ�[%d]�������ͻ��", idAccount);
			return 0;
		}
		else if(!m_pAccount[i].m_nState && !nFreeIndex)
		{
			nFreeIndex = i;
//??? �����أ���������ظ����			break;
		}
	}

	if(nFreeIndex)
	{
		return nFreeIndex;
	}
	else
	{
		LOGERROR("������ұ����[%d]�����޸�config.ini����������������", m_nCount);
		return 0;
	}
}

// ˽�е�
void	COnlineTable::Remove_0(int nIndex)
{
	if(nIndex && m_pAccount[nIndex].m_nState)
	{ 
		LPCTSTR	szServerName = m_pAccount[nIndex].m_szServerName;
		ASSERT(m_aPlayerCount.IsIn(szServerName));
		if(m_aPlayerCount[szServerName] > 0)
			m_aPlayerCount.Add(szServerName, m_aPlayerCount[szServerName] - 1);
		if(m_nCount > 0)
			m_nCount--;
		if(STAT_SERIAL)
		{
			DWORD dwSerial = m_pAccount[nIndex].m_dwSerial;
			if(dwSerial != INVALID_SERIAL && m_aSerialCount[dwSerial] > 0)
				m_aSerialCount.Add(dwSerial, m_aSerialCount[dwSerial] - 1);
		}

#ifdef	TEST_INDEX
		int	nOldIDCount = m_idxID.Size();
		int	nOldNameCount = m_idxName.Size();
		IDXASSERT(m_idxID.IsIn(m_pAccount[nIndex].m_idAccount), m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount);
		IDXASSERT(m_idxName.IsIn(m_pAccount[nIndex].m_szLoginName), m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount);
#endif
		m_idxID.Del(m_pAccount[nIndex].m_idAccount);
		m_idxName.Del(m_pAccount[nIndex].m_szLoginName);
#ifdef	TEST_INDEX
		IDXASSERT(m_idxID.Size() == nOldIDCount-1, m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount);
		IDXASSERT(m_idxName.Size() == nOldNameCount-1, m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount);
		IDXASSERT(!m_idxID.IsIn(m_pAccount[nIndex].m_idAccount), m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount);
		IDXASSERT(!m_idxName.IsIn(m_pAccount[nIndex].m_szLoginName), m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount);
#endif
		m_pAccount[nIndex].Init();
	}
	else
	{
		LOGERROR("Remove_0()�ڲ��쳣��Index[%d], State[%d]", nIndex, m_pAccount[nIndex].m_nState);
	}
}

// ˽�е�
void	COnlineTable::LogFee(int nIndex, enum COnlineAccount::EEndType nEndType)
{
	
	m_pAccount[nIndex].m_nEndType	= nEndType;
	char szStartTime[20];
	char szPointTime[20];
	char szEndTime[20];
	char szStamp[15];
	DateTime(szStartTime, m_pAccount[nIndex].m_tStartPoint);
	DateTime(szPointTime, m_pAccount[nIndex].m_tLastPoint);
	DateTime(szEndTime, m_pAccount[nIndex].m_tEndPoint);
	time_t	tCurr = time(NULL);
	tm*	pTm = localtime(&tCurr);
	sprintf(szStamp, "%04d%02d%02d%02d%02d%02d", pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
	ASSERT(c_typeAll == 7);
	ASSERT(COnlineAccount::c_endAll == 8);
	const char *	aFeeType[] = { "δ֪", "�Ƶ�", "�¿�", "���ɼƵ�", "�����¿�", "ISP", "���", "Error", "Error" };
	const char *	aEndType[] = { "δ֪", "�����˳�", "��ʱ�˳�", "��Ϸ��������", "�ظ���¼", 
							"��ʱ�˳�", "�ʺŷ������ر�", "��¼��Ϣ��ʧ", "Error", "Error" };
	const char *	pFeeType = aFeeType[0];
	const char *	pEndType = aEndType[0];
	if(m_pAccount[nIndex].m_nFeeType >= c_typeNone && m_pAccount[nIndex].m_nFeeType < c_typeAll)
		pFeeType = aFeeType[m_pAccount[nIndex].m_nFeeType];
	if(m_pAccount[nIndex].m_nEndType > COnlineAccount::c_endNone && m_pAccount[nIndex].m_nEndType < COnlineAccount::c_endAll)
		pEndType = aEndType[m_pAccount[nIndex].m_nEndType];
	if(m_pAccount[nIndex].m_nPointCount)
	{
		LOGPOINT("[%s]��[%s][%d]�˳�����ʼʱ��[%s]�����Ƶ�[%s]������ʱ��[%s]��"
					"�Ʒ��ʺ�[%s][%d], �Ʒ�����[%s], �Ƶ�����[%d], ��������[%s], IP��ַ[%s]\n"
			"INSERT fee VALUES ('%s','%s','%s',   %d,'%s','%s','%s',   '%s',%d,%d,   %d,%d,'%s');"
					, m_pAccount[nIndex].m_szServerName, m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount
					, szStartTime, szPointTime, szEndTime
					, m_pAccount[nIndex].m_szFeeAccount, m_pAccount[nIndex].m_idFeeAccount
					, pFeeType, m_pAccount[nIndex].m_nPointCount, pEndType, m_pAccount[nIndex].m_szClientIP
			, szStamp, m_pAccount[nIndex].m_szServerName, m_pAccount[nIndex].m_szLoginName
			, m_pAccount[nIndex].m_idAccount, szStartTime, szPointTime, szEndTime
			, m_pAccount[nIndex].m_szFeeAccount, m_pAccount[nIndex].m_idFeeAccount, m_pAccount[nIndex].m_nFeeType
			, m_pAccount[nIndex].m_nPointCount, m_pAccount[nIndex].m_nEndType, m_pAccount[nIndex].m_szClientIP
			);
		if(m_pAccount[nIndex].m_nFeeType == c_typeISP)
		{
			LOGISPPOINT(m_pAccount[nIndex].m_szFeeAccount, "[%s]��[%s][%d]�˳�����ʼʱ��[%s]�����Ƶ�[%s]������ʱ��[%s]��"
						"�Ʒ��ʺ�[%s][%d], �Ʒ�����[%s], �Ƶ�����[%d], ��������[%s], IP��ַ[%s]\n"
				"INSERT fee VALUES ('%s','%s','%s',   %d,'%s','%s','%s',   '%s',%d,%d,   %d,%d,'%s');"
						, m_pAccount[nIndex].m_szServerName, m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount
						, szStartTime, szPointTime, szEndTime
						, m_pAccount[nIndex].m_szFeeAccount, m_pAccount[nIndex].m_idFeeAccount
						, pFeeType, m_pAccount[nIndex].m_nPointCount, pEndType, m_pAccount[nIndex].m_szClientIP
				, szStamp, m_pAccount[nIndex].m_szServerName, m_pAccount[nIndex].m_szLoginName
				, m_pAccount[nIndex].m_idAccount, szStartTime, szPointTime, szEndTime
				, m_pAccount[nIndex].m_szFeeAccount, m_pAccount[nIndex].m_idFeeAccount, m_pAccount[nIndex].m_nFeeType
				, m_pAccount[nIndex].m_nPointCount, m_pAccount[nIndex].m_nEndType, m_pAccount[nIndex].m_szClientIP
				);
		}
	}
	else
	{
		LOGPOINT("[%s]��[%s]�˳����ʺ�ID[%d], ��ʼʱ��[%s]�����Ƶ�[%s]������ʱ��[%s]��"
					"�Ʒ��ʺ�[%s], �Ʒ�����[%s], �Ƶ�����[%d], ��������[%s], IP��ַ[%s]"
					, m_pAccount[nIndex].m_szServerName, m_pAccount[nIndex].m_szLoginName, m_pAccount[nIndex].m_idAccount
					, szStartTime, szPointTime, szEndTime
					, m_pAccount[nIndex].m_szFeeAccount, pFeeType, m_pAccount[nIndex].m_nPointCount
					, pEndType, m_pAccount[nIndex].m_szClientIP
			);
	}
	return;					//07.5.26������ӣ������ٴ���Ʒ������Ϣ...���������ݿ�

	char szSQL[1024];
	sprintf(szSQL, "INSERT fee VALUES ('%s','%s','%s',   %d,'%s','%s','%s',   '%s',%d,%d,   %d,%d,'%s');"
			, szStamp, m_pAccount[nIndex].m_szServerName, m_pAccount[nIndex].m_szLoginName
			, m_pAccount[nIndex].m_idAccount, szStartTime, szPointTime, szEndTime
			, m_pAccount[nIndex].m_szFeeAccount, m_pAccount[nIndex].m_idFeeAccount, m_pAccount[nIndex].m_nFeeType
			, m_pAccount[nIndex].m_nPointCount, m_pAccount[nIndex].m_nEndType, m_pAccount[nIndex].m_szClientIP
			);
	g_db.ExecuteSQL(szSQL);
}

void COnlineTable::RemoveByServerName( char* pServerName )
{
	if( !pServerName || strlen( pServerName ) <= 0 )
	{
		return;
	}
	LOCKTHREAD
	for( int i = 1;i < ONLINETABLESIZE;i++ )	// 1: 0�ŵ�Ԫ����
	{
		if( m_pAccount[ i ].m_nState && 0 == strcmp( m_pAccount[ i ].m_szServerName,pServerName ) )
		{
			Remove_0( i );
		}
	}

}


