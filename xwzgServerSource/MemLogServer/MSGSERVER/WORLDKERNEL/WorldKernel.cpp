// WorldKernel.cpp: implementation of the CWorldKernel class.
//
//////////////////////////////////////////////////////////////////////
#define	_WINSOCKAPI_		// 阻止加载winsock.h
#include "inifile.h"
#include "protocol.h"
#include "windows.h"
#include "Winuser.h"
#include "BaseFunc.h"
#include "I_MessagePort.h"
#include "AllMsg.h"
#include "NetMsg.h"
#include "WorldKernel.h"
#include "AutoPtr.h"
#include "StrRes.h"

using namespace world_kernel;
CWorldKernel*	CWorldKernel::m_pWorld = NULL;

// interface
//////////////////////////////////////////////////////////////////////
bool CWorldKernel::Create(IMessagePort* pPort)
{
	m_pMsgPort		= pPort;
	m_idProcess		= m_pMsgPort->GetID();		// process id == msg_port_id
	m_pMsgPort->Open();
	ASSERT(m_idProcess == MSGPORT_WORLD);

	m_pDb	= ::CreateDatabase("Database");
	if(!m_pDb)
	{
		PrintText("数据库连接失败!");
		return false;
	}

	m_bCanWork = true;
	m_pWorld	= this;
	m_WorldTime	= ::GetTickCount();


	m_dataSet.clear();
	m_dataDbSet.clear();
	CIniFile	ini(CONFIG_FILENAME, "System");
	int nAmount = ini.GetInt("AMOUNT");
	
	char szTemp[128];
	for(int i = 0 ; i < nAmount ; i ++)
	{
		sprintf(szTemp,"LOG%u",i + 1);
		ini.SetSection(szTemp);
		
		int nLogType = ini.GetInt("LOGTYPE");
		if(nLogType == 0) //日志的
		{
			stLogServerData* pData = new stLogServerData;
			ini.GetString(pData->arrayLog, "MEMLOGFILE", 63);
			if(strlen(pData->arrayLog) == 0)
				return false;
		
		
			ini.GetString(pData->arrayFile, "LOGDIR", 63);
			if(strlen(pData->arrayFile) == 0)
				return false;
		
			CreateDirectory(pData->arrayFile, NULL);
		
			sprintf(szTemp,"%s\\syslog",pData->arrayFile);
			CreateDirectory(szTemp, NULL);
			sprintf(szTemp,"%s\\valuableslog",pData->arrayFile);
			CreateDirectory(szTemp, NULL);
		
			pData->m_logInfo.clear();
			pData->m_hMapFile = ::OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,pData->arrayLog);
		
			if(pData->m_hMapFile)
			{
				pData->m_bRun = true;
			}
			else
			{
				pData->m_hMapFile = CreateFileMapping( (HANDLE)0xFFFFFFFF,NULL,
					PAGE_READWRITE,0,sizeof(stLogData),pData->arrayLog);
				
				if(pData->m_hMapFile)
				{
					stLogData* pLogData = (stLogData*)MapViewOfFile(pData->m_hMapFile,FILE_MAP_ALL_ACCESS,
						0,0,0);
					
					pLogData->unFirst = 0;
					pLogData->unLast = 0;
					FlushViewOfFile(pLogData,sizeof(stLogData));
					UnmapViewOfFile(pLogData);
					pData->m_bRun = true;
				}
			}

			m_dataSet.push_back(pData);
			sprintf(szTemp,"日志服务器 %s 开启.",pData->arrayLog);
			GameWorld()->PrintText(szTemp);
		}
		else //数据库
		{
			stLogServerData_db* pData = new stLogServerData_db;
			pData->m_pDef = NULL;
			pData->m_nDbData = 0;
			ini.GetString(pData->arrayLog, "MEMLOGFILE", 63);
			if(strlen(pData->arrayLog) == 0)
				return false;
			
			pData->m_hMapFile = ::OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,pData->arrayLog);
			
			if(pData->m_hMapFile)
			{
				pData->m_bRun = true;
			}
			else
			{
				pData->m_hMapFile = CreateFileMapping( (HANDLE)0xFFFFFFFF,NULL,
					PAGE_READWRITE,0,sizeof(stLogData_db),pData->arrayLog);
				
				if(pData->m_hMapFile)
				{
					stLogData_db* pLogData = (stLogData_db*)MapViewOfFile(pData->m_hMapFile,FILE_MAP_ALL_ACCESS,
						0,0,0);
					
					pLogData->unFirst = 0;
					pLogData->unLast = 0;
					FlushViewOfFile(pLogData,sizeof(stLogData_db));
					UnmapViewOfFile(pLogData);
					pData->m_bRun = true;
				}
			}
			
			m_dataDbSet.push_back(pData);
			sprintf(szTemp,"数据库服务器 %s 开启.",pData->arrayLog);
			GameWorld()->PrintText(szTemp);
		}
	}

	return true;	
}

//////////////////////////////////////////////////////////////////////

bool CWorldKernel::ProcessMsg(OBJID idPacket, void* buf, int nType, int nFrom)
{
	switch(idPacket)
	{
	case	WORLD_CLIENTKERMSG:
		{
			CLIENTTOWORLDMSG_PACKET0*	pMsg = (CLIENTTOWORLDMSG_PACKET0*)buf;
			OBJID	idMsg	= pMsg->idPacket;
			char*	pBuf	= pMsg->buf;
			int		nMsgLen	= pMsg->nSize;
			CLIENTTOWORLDMSG_ACTION* pActionData = (CLIENTTOWORLDMSG_ACTION*)pMsg->buf;
			ProcessClientKerAction( pActionData->nAction);
		}
 		break;
	case	WORLD_SERVERMSG:
		{
			CLIENTTOWORLDMSG_PACKET0*	pMsg	= (CLIENTTOWORLDMSG_PACKET0*)buf;
			OBJID	idMsg	= pMsg->idPacket;
			char*	pBuf	= pMsg->buf;
			int		nMsgLen	= pMsg->nSize;
			ProcessClientMsg(idMsg,pBuf,nMsgLen);
		}
		break;
	case	WORLD_SOCKETMSG:
		{
			SOCKETTOWORLDMSG_PACKET0*	pMsg	= (SOCKETTOWORLDMSG_PACKET0*)buf;
			OBJID	idMsg	= pMsg->idPacket;
			char*	pBuf	= pMsg->buf;
			int		nMsgLen	= pMsg->nSize;
			SOCKETTOWORLDMSG_ACTION* pActionData = (SOCKETTOWORLDMSG_ACTION*)pMsg->buf;
			ProcessSocketKerAction( pActionData->nSocket,pActionData->nAction);
		}
		break;
	case	WORLD_CLIENTMSG:
		{
			SOCKETTOWORLDMSG_PACKET0*	pMsg	= (SOCKETTOWORLDMSG_PACKET0*)buf;
			OBJID	idSocket = pMsg->idSocket;
			OBJID	idMsg	= pMsg->idPacket;
			char*	pBuf	= pMsg->buf;
			int		nMsgLen	= pMsg->nSize;
			ProcessSocketMsg(idSocket,idMsg,pBuf,nMsgLen);
		}
		break;
	default:
		ASSERT(!"CWorldKernel::ProcessMsg()");
	}

	return true;		// return false : 消息处理异常，程序关闭。
}


bool CWorldKernel::OnTimer(time_t tCurr)
{
	for(LOGDATASET::iterator it = m_dataSet.begin(); it != m_dataSet.end() ; it ++)
	{
		stLogServerData* pData = *it;
		if(pData)
		{
			if(pData->m_bRun)
			{
				ProcessLogFile(pData);
			}
		}
	}
	for(LOGDATADBSET::iterator itDb = m_dataDbSet.begin(); itDb != m_dataDbSet.end() ; itDb ++)
	{
		stLogServerData_db* pData = *itDb;
		if(pData)
		{
			if(pData->m_bRun)
			{
				ProcessLogDb(pData);
			}
		}
	}
	return true;		// return false : 消息处理异常，程序关闭。
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::Release()
{
	m_pMsgPort->Close();
	if(m_pDb)
	{
		m_pDb->Release();
		m_pDb = NULL;
	}
	delete this;
	return true;		// return false : 无意义。
}

bool CWorldKernel::ProcessClientKerAction(const int nAction)
{
	return true;
}

bool CWorldKernel::SendClientKerAction(const int nAction)
{
	MESSAGESTR	bufAction;
	WORLDTOCLIENTMSG_ACTION* pActionData = (WORLDTOCLIENTMSG_ACTION*)bufAction;
	pActionData->nAction = nAction;
	return SendClientKerMsg(CLIENT_WORLDMSG,bufAction,sizeof(CLIENTTOWORLDMSG_ACTION));
}

bool CWorldKernel::SendClientKerMsg(OBJID idMsg, const char *pBuf, int nMsgLen)
{
	if (!pBuf)
	{
		::LogSave("Error: null msg point found in SendWorldMsg::SendMsg.");
		return false;
	}
	
	if (idMsg <= CLIENT_BASE || idMsg >= ALL_END)
	{
		::LogSave("Error: invalid msg type in SendWorldMsg::SendMsg().");
		return false;
	}
	
	MESSAGESTR	bufPacket;
	WORLDTOCLIENTMSG_PACKET0* pPacket = (WORLDTOCLIENTMSG_PACKET0*)bufPacket;
	pPacket->idPacket	= idMsg;
	pPacket->nSize		= nMsgLen;
	memcpy(pPacket->buf, pBuf, nMsgLen);
	m_pMsgPort->Send(MSGPORT_CLIENT, idMsg, STRUCT_TYPE(bufPacket), &bufPacket);
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::ProcessClientMsg(OBJID idMsg, const char *pbufMsg, int nSize)
{
	if(idMsg == _MSG_NONE || !pbufMsg || nSize <= 0 || nSize >_MAX_MSGSIZE)
        return false;

	CNetMsg* pMsg	=CNetMsg::CreateClientMsg(m_idProcess, 0, idMsg, pbufMsg, nSize);
	if(pMsg)
	{
		DEBUG_TRY
		pMsg->Process(this);
		DEBUG_CATCH("exception catch at CWorldKernel::ProcessClientMsg!")
		delete pMsg;
	}
	return true;
}

bool CWorldKernel::ProcessSocketKerAction(SOCKET_ID nSocket,const int nAction)
{
	return true;
}

bool CWorldKernel::SendSocketKerAction(SOCKET_ID nSocket,const int nAction)
{
	MESSAGESTR	bufAction;
	WORLDTOSOCKETMSG_ACTION* pActionData = (WORLDTOSOCKETMSG_ACTION*)bufAction;
	pActionData->nAction = nAction;
	return SendSocketKerMsg(SOCKET_SOCKETWORLDACTION,bufAction,sizeof(WORLDTOSOCKETMSG_ACTION),nSocket);
}

bool CWorldKernel::SendSocketKerMsg(OBJID idMsg, const char *pBuf, int nMsgLen ,SOCKET_ID nSocket)
{
 	return true;
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::ProcessSocketMsg(SOCKET_ID idSocket, OBJID idMsg, const char *pbufMsg, int nSize)
{
	if(idMsg == _MSG_NONE || !pbufMsg || nSize <= 0 || nSize >_MAX_MSGSIZE)
        return false;
	
	CNetMsg* pMsg	=CNetMsg::CreateSocketMsg(m_idProcess, idSocket, idMsg, pbufMsg, nSize);
	if(pMsg)
	{
		DEBUG_TRY
		pMsg->Process(this);
		DEBUG_CATCH("exception catch at CWorldKernel::ProcessSocketMsg!")
		delete pMsg;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendClientMsg(SOCKET_ID idSocket,CNetMsg* pNetMsg)
{
	ASSERT(pNetMsg);
	if(!pNetMsg)
		return false;

	pNetMsg->AppendInfo(idSocket);
	
	return SendClientMsg(idSocket, pNetMsg->GetType(), pNetMsg->GetBuf(), pNetMsg->GetSize());
}


//////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendClientMsg(SOCKET_ID idSocket, OBJID idMsg, const char *pBuf, int nMsgLen)
{
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendServerMsg(CNetMsg* pNetMsg)
{
	ASSERT(pNetMsg);
	if(!pNetMsg)
		return false;
	
	return SendServerMsg( pNetMsg->GetType(), pNetMsg->GetBuf(), pNetMsg->GetSize());
}


//////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendServerMsg( OBJID idMsg, const char *pBuf, int nMsgLen)
{
	// TODO: 请在此添加NPC服务器下传消息的代码
	if (!pBuf)
	{
		::LogSave("Error: null msg point found in CGameSocket::SendMsg.");
		return false;
	}
	
	if (_MSG_NONE == idMsg)
	{
		::LogSave("Error: invalid msg type in CGameSocket::SendMsg().");
		return false;
	}
	
	DEBUG_TRY
	MESSAGESTR	bufPacket;
	WORLDTOCLIENTMSG_PACKET0* pPacket = (WORLDTOCLIENTMSG_PACKET0*)bufPacket;
	pPacket->idPacket	= idMsg;
	pPacket->nSize		= nMsgLen;
	memcpy(pPacket->buf, pBuf, nMsgLen);
	
	m_pMsgPort->Send(MSGPORT_CLIENT, CLIENT_SENDSERVERMSG, STRUCT_TYPE(bufPacket), &bufPacket);
	DEBUG_CATCHF("Error: exception catched in CGameSocket::SendMsg().")
		
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::PrintText			(LPCTSTR szText)
{
	return m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(szText), szText);
}

void CWorldKernel::ProcessLogFile(stLogServerData* pData)
{
	char szFile[128] = "";
	char szTemp[2048] = "";

	if(!pData)
		return;

	if(!pData->m_bRun)
		return;

	
	stLogData* pLogData = (stLogData*)MapViewOfFile(pData->m_hMapFile,FILE_MAP_ALL_ACCESS,
		0,0,0);
	
	if(pLogData->unLast > pLogData->unFirst)
	{
		pData->m_nAllLog = pLogData->unLast - pLogData->unFirst;
	}
	else if(pLogData->unLast == pLogData->unFirst)
	{
		pData->m_nAllLog = 0;
	}
	else
	{
		pData->m_nAllLog = pLogData->unLast + 100 - pLogData->unFirst;
	}
	
	if(pData->m_nAllLog > 5)
		pData->m_nProcesslog = 5;
	else
		pData->m_nProcesslog = pData->m_nAllLog;
	
	if(pData->m_nProcesslog > 0)
	{
		
		for(int i = 0; i < pData->m_nProcesslog ; i ++)
		{
			int nIndex = (pLogData->unFirst + i) % 100;
			sprintf(szFile,"%s\\%s",pData->arrayFile,pLogData->arrayFile[nIndex]);
			log_Save(pData,szFile,pLogData->arrayLog[nIndex]);
		}
		
		pLogData->unFirst = (pLogData->unFirst + pData->m_nProcesslog) % 100;
		FlushViewOfFile(pLogData,sizeof(stLogData));
	}
	UnmapViewOfFile(pLogData);
}

void CWorldKernel::ProcessLogDb(stLogServerData_db* pData)
{
	char szFile[128] = "";
	char szTemp[2048] = "";

	if(!pData)
		return;
	
	if(!pData->m_bRun)
		return;
	
// 	stDataSave_db stInfoaa;
// 	stInfoaa.usOp = 1;
// 	stInfoaa.usOpType = 2;
// 	stInfoaa.usGetType = 3;
// 	stInfoaa.usNum = 1;
// 	strcpy(stInfoaa.szTarget,"111");
// 	stInfoaa.idTarget = 10003775;
// 	strcpy(stInfoaa.szOwner,"222");
// 	stInfoaa.idOwner = 10003776;
// 	strcpy(stInfoaa.szTime,"2011-07-07 77:56:00");
// 	strcpy(stInfoaa.szMsg,"2134234345345345");
// 	log_SaveDb(pData,stInfoaa);

	stLogData_db* pLogData = (stLogData_db*)MapViewOfFile(pData->m_hMapFile,FILE_MAP_ALL_ACCESS,
		0,0,0);
	
	if(pLogData->unLast > pLogData->unFirst)
	{
		pData->m_nAllLog = pLogData->unLast - pLogData->unFirst;
	}
	else if(pLogData->unLast == pLogData->unFirst)
	{
		pData->m_nAllLog = 0;
	}
	else
	{
		pData->m_nAllLog = pLogData->unLast + 100 - pLogData->unFirst;
	}
	
	if(pData->m_nAllLog > 5)
		pData->m_nProcesslog = 5;
	else
		pData->m_nProcesslog = pData->m_nAllLog;
	
	if(pData->m_nProcesslog > 0)
	{
		for(int i = 0; i < pData->m_nProcesslog ; i ++)
		{
			int nIndex = (pLogData->unFirst + i) % 100;
			stDataSave_db stInfo;
			stInfo.usOp = pLogData->usOp[nIndex];
			stInfo.usOpType = pLogData->usOpType[nIndex];
			stInfo.usGetType = pLogData->usGetType[nIndex];
			stInfo.usNum = pLogData->usNum[nIndex];
			strcpy(stInfo.szTarget,pLogData->szTarget[nIndex]);
			stInfo.idTarget = pLogData->idTarget[nIndex];
			strcpy(stInfo.szOwner,pLogData->szOwner[nIndex]);
			stInfo.idOwner = pLogData->idOwner[nIndex];
			strcpy(stInfo.szTime,pLogData->szTime[nIndex]);
			strcpy(stInfo.szMsg,pLogData->szMsg[nIndex]);

			log_SaveDb(pData,stInfo);
		}
		
		pLogData->unFirst = (pLogData->unFirst + pData->m_nProcesslog) % 100;
		FlushViewOfFile(pLogData,sizeof(stLogData));
	}
	UnmapViewOfFile(pLogData);
}

void CWorldKernel::log_Save(stLogServerData* pData ,const char * pFilename, const char * pBuf)
{
	char	szLogFile[1024] = "";
	
	time_t tCurrTime;
	time( &tCurrTime );
	tm *	pTmFile = localtime(&tCurrTime);
	int nTime = (pTmFile->tm_year+1900)*10000 +  (pTmFile->tm_mon + 1) * 100 + pTmFile->tm_mday;
	
	LOGNAME_SET::iterator it = pData->m_logInfo.find(pFilename);
	if(it != pData->m_logInfo.end())
	{
		if(it->second.nTime != nTime)
		{
			fclose(it->second.pFile);
			it->second.unFileIndex = 0;
			it->second.unHas = 0;
			it->second.nTime = nTime;
			
			
			sprintf(szLogFile, "%s %04d-%02d-%02d (%d).LOG", pFilename, it->second.nTime / 10000, it->second.nTime % 10000 / 100, it->second.nTime % 100, it->second.unFileIndex);
			it->second.pFile = fopen(szLogFile, "a");
			if(it->second.pFile)
				fseek(it->second.pFile, 0, SEEK_END);
		}
		if(it->second.unHas > 10000)
		{
			fclose(it->second.pFile);
			it->second.unFileIndex ++;
			it->second.unHas = 0;
			
			
			sprintf(szLogFile, "%s %04d-%02d-%02d (%d).LOG", pFilename, it->second.nTime / 10000, it->second.nTime % 10000 / 100, it->second.nTime % 100, it->second.unFileIndex);
			it->second.pFile = fopen(szLogFile, "a");
			if(it->second.pFile)
				fseek(it->second.pFile, 0, SEEK_END);
		}
		
		
		if(it->second.pFile)
		{
			fputs(pBuf, it->second.pFile);
			fflush(it->second.pFile);
			it->second.unHas ++;
		}
	}
	else
	{
		stLogInfo stData;
		stData.unFileIndex = 0;
		stData.unHas = 0;
		stData.nTime = nTime;
		
		sprintf(szLogFile, "%s %04d-%02d-%02d (%d).LOG", pFilename, stData.nTime / 10000, stData.nTime % 10000 / 100, stData.nTime % 100, stData.unFileIndex);
		stData.pFile = fopen(szLogFile, "a");
		
		if(stData.pFile)
		{
			fseek(stData.pFile, 0, SEEK_END);
			fputs(pBuf, stData.pFile);
			fflush(stData.pFile);
			
			stData.unHas ++;
			pData->m_logInfo[pFilename] = stData;
		}
	}
}

void CWorldKernel::log_SaveDb(stLogServerData_db* pData ,stDataSave_db stInfo)
{
	if(!m_pDb)
		return;

	char szTemp[128] = "";
	char szSQL[4096] = "";

	SYSTEMTIME curTime;
	::GetLocalTime( &curTime );
	int nDbData = curTime.wYear * 10000 + curTime.wMonth * 100 + curTime.wDay / 8 + 1;
	sprintf(szTemp,"log_%d_%d_%d",curTime.wYear,curTime.wMonth,curTime.wDay / 8 + 1);
	if(nDbData != pData->m_nDbData) //到下一个了
	{		
		sprintf(szSQL, "SELECT * FROM %s WHERE id=0 LIMIT 1", szTemp);
		pData->m_pDef	= m_pDb->CreateNewRecordset(szSQL, false);
		if(!pData->m_pDef)//库不存在
		{
			static char szSQLNewdb[] = "CREATE TABLE `%s` ("
				"`Id` int(11) NOT NULL AUTO_INCREMENT,"
				"`op` int(11) unsigned NOT NULL DEFAULT '0',"
				"`optype` int(11) unsigned NOT NULL DEFAULT '0',"
				"`gettype` int(11) unsigned NOT NULL DEFAULT '0',"
				"`optarget` char(64) NOT NULL DEFAULT 'system',"
				"`optargetid` int(11) NOT NULL DEFAULT '0',"
				"`opowner` char(64) NOT NULL DEFAULT '0',"
				"`opownerid` int(11) NOT NULL DEFAULT '0',"
				"`optime` char(64) NOT NULL DEFAULT '0',"
				"`opnum` int(11) NOT NULL DEFAULT '0',"
				"`opmsg` varchar(255) NOT NULL DEFAULT '0',"
				"PRIMARY KEY (`Id`)"
				") ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=gbk";
			char szSQLNew[1024] = "";
			sprintf(szSQLNew, szSQLNewdb, szTemp);
			m_pDb->ExecuteSQL(szSQLNew);

			pData->m_pDef	= m_pDb->CreateNewRecordset(szSQL, false);
			if(!pData->m_pDef)
				return;
		}
		pData->m_nDbData = nDbData;
	}
	else
	{
		if(!pData->m_pDef)
			return;
	}

	static char szSQLNewData[] = "INSERT INTO %s SET "
		"`op` = %d,"
		"`optype` = %d,"
		"`gettype` = %d,"
		"`opnum` = %d,"
		"`optarget` = '%s',"
		"`optargetid` = %u,"
		"`opowner` = '%s',"
		"`opownerid` = %u,"
		"`optime` = '%s',"
		"`opmsg` = '%s'";

	sprintf(szSQL, szSQLNewData, szTemp
		,stInfo.usOp
		,stInfo.usOpType
		,stInfo.usGetType
		,stInfo.usNum
		,stInfo.szTarget
		,stInfo.idTarget
		,stInfo.szOwner
		,stInfo.idOwner
		,stInfo.szTime
		,stInfo.szMsg);
	m_pDb->ExecuteSQL(szSQL);
}

///////////////////////////////////////////////////////////////////////////////////////
// global entry
///////////////////////////////////////////////////////////////////////////////////////
IWorld* IWorld::CreateNew()
{
	return new CWorldKernel;
}
////////////////////////////////////////////////////////////////////////////////
