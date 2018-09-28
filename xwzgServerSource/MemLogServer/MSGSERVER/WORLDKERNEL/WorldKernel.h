// WorldKernel.h: interface for the CWorldKernel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORLDKERNEL_H__9A393F65_C167_402E_8A17_7EA70AC5879E__INCLUDED_)
#define AFX_WORLDKERNEL_H__9A393F65_C167_402E_8A17_7EA70AC5879E__INCLUDED_

#include <time.h>
#include <winsock2.h>
#include "../../GameBaseCodeMT/I_mydb.h"
#include "NetMsg.h"
#include "I_Shell.h"
#include "T_Singlemap.h"
//#include "ClientSocket.h"
#include "AutoPtr.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "protocol.h"
#include <vector>
using namespace std;
using namespace world_kernel;
//#include "AccountManage.h"
//#include "GameDataManager.h"

struct stLogData 
{
	unsigned short unFirst;
	unsigned short unLast;
	char arrayFile[100][128];
	char arrayLog[100][1024];
};

struct stLogData_db
{
	unsigned short unFirst;
	unsigned short unLast;
	unsigned short usOp[100];
	unsigned short usOpType[100];
	unsigned short usGetType[100];
	unsigned short usNum[100];
	char szTarget[100][32];
	DWORD idTarget[100];
	char szOwner[100][32];
	DWORD idOwner[100];
	char szTime[100][64];
	char szMsg[100][512];
};

struct stDataSave_db
{
	unsigned short usOp;
	unsigned short usOpType;
	unsigned short usGetType;
	unsigned short usNum;
	char szTarget[32];
	DWORD idTarget;
	char szOwner[32];
	DWORD idOwner;
	char szTime[64];
	char szMsg[512];
};

struct stLogInfo 
{
	unsigned short unHas;
	unsigned short unFileIndex;
	int nTime;
	FILE	* pFile;
};
typedef	map<String,stLogInfo> LOGNAME_SET;

struct stLogServerData 
{
	bool m_bRun;
	int m_nAllLog;
	int m_nProcesslog;
	LOGNAME_SET m_logInfo;
	char arrayFile[64];
	char arrayLog[64];
	HANDLE m_hMapFile;
};

typedef vector<stLogServerData*> LOGDATASET;

struct stLogServerData_db
{
	bool m_bRun;
	int m_nAllLog;
	int m_nProcesslog;
	IRecordset* m_pDef;
	int m_nDbData;
	char arrayLog[64];
	HANDLE m_hMapFile;
};

typedef vector<stLogServerData_db*> LOGDATADBSET;

class CWorldKernel : public IWorld, ISocket
{
public:
	CWorldKernel() 
	{
		m_nState=0;
		m_pDb = NULL;
	}
	virtual ~CWorldKernel() {}		// 建议留空
public://功能接口相关
	IWorld*		GetInterface()				{ return (IWorld*)this; }
	ISocket*	GetSocketInterface()	{ return (ISocket*)this; }
//	CGameDataManager*	QueryGameDataManager()			{ ASSERT(m_pGameDataManager); return m_pGameDataManager; }

protected: // IWorld
	virtual bool	Create(IMessagePort* pPort);
	virtual bool	ProcessMsg(OBJID idPacket, void* buf, int nType, int nFrom);
	virtual bool	OnTimer(time_t tCurr);
	virtual bool	Release();

protected: // ISocket	
	virtual bool SendServerMsg			(OBJID idMsg, const char* pBuf, int nMsgLen);	
	virtual bool SendClientMsg			(SOCKET_ID idSocket, OBJID idMsg, const char* pBuf, int nMsgLen);
public:// send
	virtual bool SendServerMsg			(CNetMsg* pNetMsg);
	virtual bool SendClientMsg			(SOCKET_ID idSocket,CNetMsg* pNetMsg);

public: 
	virtual bool PrintText			(LPCTSTR szText);
	void ProcessLogFile(stLogServerData* pData);
	void ProcessLogDb(stLogServerData_db* pData);
	void log_Save(stLogServerData* pData ,const char * pFilename, const char * pBuf);
	void log_SaveDb(stLogServerData_db* pData ,stDataSave_db stInfo);
protected:
	bool SendSocketKerMsg(OBJID idMsg, const char *pBuf, int nMsgLen, SOCKET_ID nSocket);
	bool SendSocketKerAction(SOCKET_ID nSocketconst, int nAction);
	
	bool ProcessSocketMsg	(SOCKET_ID idSocket, OBJID idMsg, const char* pBuf, int nMsgLen);
	bool ProcessSocketKerAction(SOCKET_ID nSocket,const int nAction);

	bool SendClientKerMsg(OBJID idMsg, const char *pBuf, int nMsgLen);
	bool SendClientKerAction(const int nAction);
	
	bool ProcessClientMsg	( OBJID idMsg, const char* pBuf, int nMsgLen);
	bool ProcessClientKerAction(const int nAction);
	PROCESS_ID	GetProcessID() { return m_idProcess; }
public:
	IMessagePort*	GetMsgPort()							{ return m_pMsgPort; }
	IDatabase*		GetDatabase()					{ return m_pDb;}

protected:
	PROCESS_ID		m_idProcess;
	IDatabase*		m_pDb;
	IMessagePort*	m_pMsgPort;
	static CWorldKernel*	m_pWorld;
protected:
	enum { STATE_NONE, STATE_CONNECTOK, STATE_ACCOUNTOK, STATE_NORMAL, STATE_STATEOK };
	int			m_nState;
	int			m_WorldTime;
	int			m_nIndex;
protected:
	LOGDATASET m_dataSet;
	LOGDATADBSET m_dataDbSet;
	//stLogServerData* m_pServerData;
protected: 
	bool m_bWork;
	bool m_bCanWork;
	friend CWorldKernel* GameWorld();
};

inline CWorldKernel* GameWorld()			{ ASSERT(CWorldKernel::m_pWorld); return CWorldKernel::m_pWorld; }

#endif // !defined(AFX_WORLDKERNEL_H__9A393F65_C167_402E_8A17_7EA70AC5879E__INCLUDED_)
