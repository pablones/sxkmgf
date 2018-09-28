// MapGroupKernel.cpp: implementation of the CMapGroupKernel class.
//
//////////////////////////////////////////////////////////////////////

#include "MessagePort.h"
#include "inifile.h"
#include "protocol.h"
#include "MapGroupKernel.h"
#include "NetMsg.h"
#include "BaseFunc.h"
#include "MapGroup.h"
#include "Npc.h"
#include "Agent.h"
#include "MsgSyndicate.h"
#include "DeadLoop.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include "ResourceWar.h"
#include "MsgWargameSync.h"
#include "WargameManager.h"

#include "AllMsg.h"
#include "SKPostOffice.h"//[��;���� 2009.01.09]
#include "MsgPlayerNpc.h"
MYHEAP_IMPLEMENTATION(CMapGroupKernel,s_heap)
extern int g_MainGameServer;
extern int g_MainServerData;
extern int g_SpecilLine;
//////////////////////////////////////////////////////////////////////
// interface
//////////////////////////////////////////////////////////////////////
bool CMapGroupKernel::Create(IMessagePort* pPort)
{
	m_pMsgPort		= pPort;
	m_idProcess		= m_pMsgPort->GetID();		// process id == msg_port_id
	m_pMsgPort->Open();
	ASSERT(m_idProcess >= MSGPORT_MAPGROUP_FIRST);

	// ��ʼ�����ݿ�
	char	DB_IP		[DBSTR_SIZE] = "";
	char	DB_USER		[DBSTR_SIZE] = "";
	char	DB_PW		[DBSTR_SIZE] = "";
	char	DB_DB		[DBSTR_SIZE] = "";
	CIniFile	ini(CONFIG_FILENAME, "Database");
	ini.GetString(DB_IP,		"DB_IP",	DBSTR_SIZE);
	ini.GetString(DB_USER,		"DB_USER",	DBSTR_SIZE);
	ini.GetString(DB_PW,		"DB_PW",	DBSTR_SIZE);
	ini.GetString(DB_DB,		"DB_DB",	DBSTR_SIZE);
//	strupr()
	m_pDb = ::CreateDatabase(DB_IP, DB_USER, DB_PW, DB_DB);
	if(!m_pDb)
	{
		LOGERROR("���ݿ�����ʧ��!");
		return false;
	}

	// TODO: ���ڴ���ӳ�ʼ������
	CMapGroup::AddMapGroup(m_pMsgPort->GetID());
	if(!MapGroup(m_pMsgPort->GetID())->Create(pPort->GetID(), this, m_pDb, m_pMsgPort))
	{
		MapGroup(m_pMsgPort->GetID())->Destroy();
		return false;
	}

	//  [1/15/2008 ���]
	::GetLocalTime(&m_stLastDatabaseReconnectTime);

	return true;		// return false : ����ʧ�ܣ�����رա�
}

//////////////////////////////////////////////////////////////////////
bool CMapGroupKernel::Release()
{
	// TODO: ���ڴ���Ӵ���
	MapGroup(m_pMsgPort->GetID())->Destroy();
	SAFE_RELEASE(m_pDb);

	delete this;
	return true;		// return false : �����塣
}
//////////////////////////////////////////////////////////////////////
bool CMapGroupKernel::ProcessMsg(OBJID idPacket, void* buf, int nType, int nFrom)
{
	switch(idPacket)
	{
	case	KERNEL_CLIENTMSG:
		{
			CLIENTMSG_PACKET0*	pMsg = (CLIENTMSG_PACKET0*)buf;
			SOCKET_ID	idSocket= pMsg->idSocket;
			OBJID	idMsg	= pMsg->idPacket;
			char*	pBuf	= pMsg->buf;
			int		nMsgLen	= pMsg->nSize;

			DEADLOOP_CHECK(PID, "KERNEL_CLIENTMSG: ")

			DEBUG_TRY
 			ProcessClientMsg(idSocket, idMsg, pBuf, nMsgLen);
			DEBUG_CATCH2("ProcessClientMsg", idMsg)
		}
		break;
	case MAPGROUP_TRANSMITMSG_FORCHATROOM://��������Ϣ
		{
			TRANSMSG_PACKET0*	pMsg = (TRANSMSG_PACKET0*)buf;
			SOCKET_ID	idSocket= pMsg->idSocket;
			OBJID		idNpc	= pMsg->idNpc;
			OBJID	idMsg	= pMsg->idPacket;
			char*	pBuf	= pMsg->buf;
			int		nMsgLen	= pMsg->nSize;
			int		nTrans	= pMsg->nTrans;
			
			DEADLOOP_CHECK(PID, "MAPGROUP_TRANSMITMSG: ")
			if(idSocket != SOCKET_NONE)
			{
				ProcessClientMsg(idSocket, idMsg, pBuf, nMsgLen, nTrans);
			}
		}
		break;
	case MAPGROUP_PHYLEMSG://�㲥����������Ϣ
		{
			TRANSPHYMSG_PACK0* pMsg = ( TRANSPHYMSG_PACK0* )buf;
			DEADLOOP_CHECK(PID, "BroadcastPhyleMapGroupMsg " );
			BroadcastPhyleMapGroupMsg( pMsg->nPhyle,pMsg->idSocket, pMsg->idPacket, pMsg->buf, pMsg->nSize );
		}
		break;
	case	MAPGROUP_TRANSMITMSG:
		{
			TRANSMSG_PACKET0*	pMsg = (TRANSMSG_PACKET0*)buf;
			SOCKET_ID	idSocket= pMsg->idSocket;
			OBJID		idNpc	= pMsg->idNpc;
			OBJID	idMsg	= pMsg->idPacket;
			char*	pBuf	= pMsg->buf;
			int		nMsgLen	= pMsg->nSize;
			int		nTrans	= pMsg->nTrans;
			
			DEADLOOP_CHECK(PID, "MAPGROUP_TRANSMITMSG: ")
// 			if(idSocket == SOCKET_NONE)
// 			{
// 				if(UserManager()->GetUser(idNpc) == NULL)
// 					ProcessNpcMsg(idNpc, idMsg, pBuf, nMsgLen, nTrans);
// 			}
			if(idSocket != SOCKET_NONE)
			{
				if(UserManager()->GetUserBySocketID(idSocket) == NULL)		
					ProcessClientMsg(idSocket, idMsg, pBuf, nMsgLen, nTrans);
			}
		}
		break;
	case    MAPGROUP_SENDMSG:///�õ���һ����ͼ�鷢������Ϣ-softworms-2008-1-4
		{
			TRANSMSG_PACKET0*	pMsg = (TRANSMSG_PACKET0*)buf;
			SOCKET_ID	idSocket= pMsg->idSocket;
			OBJID		idNpc	= pMsg->idNpc;
			OBJID	idMsg	= pMsg->idPacket;
			char*	pBuf	= pMsg->buf;
			int		nMsgLen	= pMsg->nSize;
			int		nTrans	= pMsg->nTrans;
			CNetMsg* ptrMsg=CNetMsg::CreateClientMsg(m_idProcess,idSocket,idMsg,pBuf,nMsgLen,nTrans);
			CHECKF(ptrMsg);
			ptrMsg->Process(this);
			ptrMsg->Release();
		}
		break;
	case MAPGROUP_MAINSERVER:
		{
			DEBUG_TRY
			TRANSMSG_PACKET0*	pMsg = (TRANSMSG_PACKET0*)buf;
			SOCKET_ID	idSocket= pMsg->idSocket;
			OBJID		idNpc	= pMsg->idNpc;
			OBJID	idMsg	= pMsg->idPacket;
			char*	pBuf	= pMsg->buf;
			int		nMsgLen	= pMsg->nSize;
			int		nTrans	= pMsg->nTrans;
			if (idMsg==_MSG_MAINSERVER)
			{
				int nMainServer = nTrans%10;
				if (nMainServer==2 && MainServerFlag()!=1)//����2��ʾͬ��������������
				{
					g_MainServerData = nTrans/10;
					int nRad = g_MainServerData/100;
					int nhour = g_MainServerData%100;
					char szLuaName[64];
					if (nRad < 1666)//���
						sprintf(szLuaName, "ontimer/ontimer1.lua");
					else if (nRad < 3333)//ȫ��
						sprintf(szLuaName, "ontimer/ontimer2.lua");
					else if (nRad < 5000)//����
						sprintf(szLuaName, "ontimer/ontimer3.lua");
					else if (nRad < 6666)//����
						sprintf(szLuaName, "ontimer/ontimer4.lua");
					else if (nRad < 8333)//ؤ��
						sprintf(szLuaName, "ontimer/ontimer5.lua");
					else//����
						sprintf(szLuaName, "ontimer/ontimer6.lua");
					LuaScriptMachine()->InitMapOntimerScript(nhour);
					if (strlen(szLuaName)>0)
						LuaScriptMachine()->RunScriptFile(szLuaName);
				}
				else if (nMainServer==3)//ˢ���ؼ���Ʒ
					VASItem()->CreateASaleItem();
				else if (nMainServer==4)//�����ؼ���Ʒ����
				{
					vasItem* pVasItem = VASItem()->GetASaleItemByType(nTrans/10);
					if (pVasItem)
						VASItem()->SetASaleItemAmount(nTrans/10, pVasItem->aSaleCount-1, false);
				}
				else//������������
					g_MainGameServer = nMainServer;//��������Ϸ������
				break;
			}
			DEBUG_CATCH("CMapGroupKernel::ProcessMsg case MAPGROUP_MAINSERVER")
		}
		break;
	case	MAPGROUP_REMOTECALL:
		{
			REMOTE_CALL0*	pInfo = (REMOTE_CALL0*)buf;
			CUser*	pUser = UserManager()->GetUser(pInfo->idUser);
			if(pUser)
			{
				DEADLOOP_CHECK(PID, "MAPGROUP_REMOTECALL: ")
				pUser->RemoteCall(pInfo);
			}
		}
		break;
	case MAPGROUP_GMFLYPLAYER:
		{
			GM_FLYPLAYER*	pInfo = (GM_FLYPLAYER*)buf;
			CUser*	pUser = UserManager()->GetUser(pInfo->UserID);
			if(pUser)
			{
				DEADLOOP_CHECK(PID, "MAPGROUP_GMFLYPLAYER: ")
				pUser->FlyMap( pInfo->MapData, pInfo->MapX, pInfo->MapY );
			}
		}
		break;
	case MAPGROUP_GMSTOPSAY:
		{
			GM_STOPSAY*	  pInfo = (GM_STOPSAY*)buf;
			CUser* pUser = UserManager()->GetUser(pInfo->szname);
			if (pUser)
			{
				DEADLOOP_CHECK(PID,"MAPGROUP_GMSTOPSAY: ")
				CRole::AttachStatus( pUser->QueryRole(),STATUS_STOPSAY,pInfo->sec,pInfo->sec );//power��ʱ������
	
			}
		}
		break;
// 	case	KERNEL_NPCMSG:
// 		{
// 			NPCMSG_PACKET0*	pMsg = (NPCMSG_PACKET0*)buf;
// 			OBJID	idNpc	= pMsg->idNpc;
// 			OBJID	idMsg	= pMsg->idPacket;
// 			char*	pBuf	= pMsg->buf;
// 			int		nMsgLen	= pMsg->nSize;
// 
// 			DEADLOOP_CHECK(PID, "KERNEL_NPCMSG: ")
// 
// 			DEBUG_TRY
// 			ProcessNpcMsg(idNpc, idMsg, pBuf, nMsgLen);
// 			DEBUG_CATCH2("ProcessNpcMsg", idMsg)
// 		}
// 		break;
	case	KERNEL_CLOSEKERNEL:
		{
			SOCKET_ID	idSocket = *(SOCKET_ID*)buf;
			LOGDEBUG("DEBUG����ͼ�����[%d]�յ��رպ�����Ϣ��SOCKET_ID[%u]", PID, idSocket);

			DEBUG_TRY
			DEADLOOP_CHECK(PID, "KERNEL_CLOSEKERNEL: ")
#ifdef	NEW_LOGOUT
			if(UserManager()->IsLoginMapGroup(idSocket))	  // readme.txt(3-1), ���򲻲���
			{
				g_UserManager.LogoutUser(idSocket);
				m_pMsgPort->Send(MSGPORT_WORLD, KERNEL_CLOSEKERNEL, VARTYPE_INT, &idSocket);	  // readme.txt(3-2)
			}
			else
			{
				m_pMsgPort->Send(m_idProcess-1, KERNEL_CLOSEKERNEL, VARTYPE_INT, &idSocket);	  // readme.txt(3-1)
			}
#else
			if(UserManager()->IsLoginMapGroup(idSocket))	  // readme.txt(3-1), ���򲻲���
			{
				g_UserManager.LogoutUser(idSocket);
				m_pMsgPort->Send(MSGPORT_WORLD, WORLD_CLOSESOCKET, VARTYPE_INT, &idSocket);	  // readme.txt(3-2)
			}
#endif
			DEBUG_CATCH("KERNEL_CLOSEKERNEL")
		}
		break;
	case MAPGROUP_LOGOUTALLUSER:
		{
			g_UserManager.KickOutAllSocket();
		}
		break;
	case	MAPGROUP_LOGIN:
		{
			ST_LOGIN* pPacket = (ST_LOGIN*)buf;

			if(pPacket->idSocket == SOCKET_NONE)	
				break;

			int nCatchDebugStep = 0;

			DEBUG_TRY	// VVVVVVVVVVVVVV				
			DEADLOOP_CHECK(PID, "MAPGROUP_LOGIN pUser: ")
			CUserPtr pUser = UserManager()->CreateUser(pPacket->idSocket, pPacket->idUser, pPacket->nIP);
			IF_NOT(pUser)
				break;

			nCatchDebugStep = 1;
			DEBUG_TRY
			//? ������תPROCESS_ID
			CHANGE_USERDATA	obj;
			obj.idSocket	= pPacket->idSocket;
			obj.nData		= m_idProcess;
			if(!m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_SETPROCESSID, STRUCT_TYPE(CHANGE_USERDATA), &obj))		// readme.txt(3-6)
			{
				::LogSave("m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_SETPROCESSID, STRUCT_TYPE(CHANGE_USERDATA), &obj) error!");
			}
			//@ ����Ҫ��֪ͨWORLDKERNEL(�Ѿ��޸Ĺ���)
			DEBUG_CATCH( "m_pMsgPort->Send(MSGPORT_SOCKET......" );
			nCatchDebugStep = 2;
			g_UserManager.LoginUser(pPacket->idSocket, pUser);
			pUser->SetStatus(STATUS_NORMAL);

			DEBUG_CATCH4("LoginUser..idSocket:%d.idUser:%d nCatchDebugStep %d",	pPacket->idSocket, pPacket->idUser, nCatchDebugStep)
		}
		break;
	case	MAPGROUP_BROADCASTMSG:
		{
			CLIENTMSG_PACKET0* pMsg = (CLIENTMSG_PACKET0*)buf;
			DEADLOOP_CHECK(PID, "MAPGROUP_BROADCASTMSG: ")

			BroadcastMapGroupMsg(pMsg->idSocket, pMsg->idPacket, pMsg->buf, pMsg->nSize);
		}
		break;
	case	MAPGROUP_SOCKETUSERINFO:			// 1: create user
		{
			ST_USERCHANGEMAPGORUP* pMsg = (ST_USERCHANGEMAPGORUP*)buf;
			DEADLOOP_CHECK(PID, "MAPGROUP_SOCKETUSERINFO: ")

			DEBUG_TRY
			UserManager()->GetForMapGroup()->JoinMapGroup(pMsg->idSocket, &pMsg->info, pMsg->nIP);
			DEBUG_CATCH("UserManager()->GetForMapGroup()->JoinMapGroup");

			//? ������֪ͨ, �ٻ���FLY��Ϣ
			CHANGE_USERDATA	st;
			st.idSocket		= pMsg->idSocket;
			st.nData		= m_idProcess;
			m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_SETPROCESSID, STRUCT_TYPE(CHANGE_USERDATA), &st);   // readme.txt (3-6)
			m_pMsgPort->Send(MSGPORT_WORLD, WORLD_SETPROCESSID, STRUCT_TYPE(CHANGE_USERDATA), &st);
		}
		break;

	case	MAPGROUP_SENDOBJINFO:				// 2: append info
		{
			CHANGEMAPGORUP_INFO0*	pInfo = (CHANGEMAPGORUP_INFO0*)buf;
			DEADLOOP_CHECK(PID, "MAPGROUP_SENDOBJINFO: ")

			CUserPtr pUser = UserManager()->GetUser(pInfo->idUser);
			if(pUser)
				pUser->AppendObjInfo((INFO_ID)pInfo->nInfoType, pInfo->info);
		}
		break;
	case	MAPGROUP_CHANGEMAPGROUP:			// 3: join map
		{
			ST_CHANGEMAPGROUP* pPacket = (ST_CHANGEMAPGROUP*)buf;
			DEADLOOP_CHECK(PID, "MAPGROUP_CHANGEMAPGROUP: ")

			CUserPtr pUser = UserManager()->GetUser(pPacket->idUser);
			//pUser->SetBoothPoint(pPacket->lpBooth);//[��;���� 2009.05.12]
			if(pUser)
				pUser->JoinMap(pPacket->idMap, pPacket->nPosX, pPacket->nPosY);
		}
		break;
	case MAPGROUP_CHANGEMAPGROUP_REQ://aim->source mapGroup ����
		{
			//�������ͼ������Ϸ���.��ʱuser���ڴ˵�ͼ��.û�����user
			ST_CHANGEMAPGROUP* pPacket = (ST_CHANGEMAPGROUP*)buf;
			DEADLOOP_CHECK(PID, "MAPGROUP_CHANGEMAPGROUP_REQ: ")
				
			CMapPtr pMap= MapManager()->QueryMap(pPacket->idMap);
			if(pMap && pMap->IsStandEnable(pPacket->nPosX,pPacket->nPosY))
			{
				//����source mapGroup ACK
				MapGroup(PID)->QueryIntraMsg()->ChangeMapGroup(pPacket->idSourceProgress, //source mapGroup id
					pPacket->idUser, 
					pPacket->idMap, 
					pPacket->nPosX, 
					pPacket->nPosY,
					ENUM_CNGMAPGROUP_MODE_ACK);
				
			}
			else//error!
			{
				//����source mapGroup NACK
				MapGroup(PID)->QueryIntraMsg()->ChangeMapGroup(pPacket->idSourceProgress, //source mapGroup id
					pPacket->idUser, 
					pPacket->idMap, 
					pPacket->nPosX, 
					pPacket->nPosY,
					ENUM_CNGMAPGROUP_MODE_NACK);
			}
			
		}
		break;
	case MAPGROUP_CHANGEMAPGROUP_ACK://aim->source mapGroup ����
		{
			//ͬ��.���ô�ͳ���л���ͼ��.��ʱuser���ڱ���ͼ��.
			ST_CHANGEMAPGROUP* pPacket = (ST_CHANGEMAPGROUP*)buf;
			DEADLOOP_CHECK(PID, "MAPGROUP_CHANGEMAPGROUP_ACK: ")

			CUserPtr pUser = UserManager()->GetUser(pPacket->idUser);
			if(pUser)
			{
				//����е�ͼ����
				pUser->ChangeMapGroup(pPacket->idSourceProgress, //aim mapGroup id
					pPacket->idMap, 
					pPacket->nPosX, 
					pPacket->nPosY,
					ENUM_CNGMAPGROUP_MODE_PROC);					// call - delete this;
			}
		}
		break;
	case MAPGROUP_CHANGEMAPGROUP_NACK://aim->source mapGroup ����
		{
			//�ܾ�.����������ʾ.��ʱuser���ڱ���ͼ��
			ST_CHANGEMAPGROUP* pPacket = (ST_CHANGEMAPGROUP*)buf;
			DEADLOOP_CHECK(PID, "MAPGROUP_CHANGEMAPGROUP_NACK: ")

			CUserPtr pUser = UserManager()->GetUser(pPacket->idUser);
			if(pUser)
			{
				//����������ʾ
				pUser->SendSysMsg( "�л���ͼ��ʱ����,Ŀ��㲻�����");
				//pUser->SendSysMsg("�л���ͼ��ʱ����,Ŀ��㲻�����");
			}
		}
		break;
		//--------------------------------------------------
		//-----------------------------------------------
	case MAPGROUP_USERCLOSEONTIMER:
		{
			int idSocketTemp = *((int*)buf);
			DEADLOOP_CHECK(PID, "MAPGROUP_USERCLOSEONTIMER: ")
			CUserPtr pUser = UserManager()->GetUserBySocketID(idSocketTemp);
			if(pUser)
			{
				pUser->SetClosing();
			}
		}
		break;
		//-----------------------------------------------
// 	case	MAPGROUP_DELALLMONSTER:
// 		{
// 			DEADLOOP_CHECK(PID, "DelAllMonster")
// 			NpcManager()->DelAllMonster();
// 			MapGroup(PID)->ClearNpcBigSet();
// 		}
// 		break;
	case	MAPGROUP_CREATENEWNPC:
		{
			DEADLOOP_CHECK(PID, "MAPGROUP_CREATENEWNPC")
			ST_CREATENEWNPC* pPacket = (ST_CREATENEWNPC*)buf;
			//20071017����:debug info----------------
			clock_t clBegin = clock();
			NpcManager()->CreateMonster(pPacket);
//			///�½ӿ�֧��ˢ�ֵ�����
//			NpcManager()->CreatePowerMonster(pPacket);

			
			clock_t clUsed = clock() - clBegin;

#ifdef _DEBUG
			if(clUsed > 30)//overTime
			{
				//log
				::LogSave("overTime(%d)::when CreateMonster:"
							"id:%d "
							"idMap:%d "
							"idData:%d "
							"idOwner:%d "
							"nData:%d "
							"usPosX:%d "
							"usPosY:%d "
							"usAction:%d "
							"usType:%d "
							"ucOwnerType:%d ",
							clUsed,
							pPacket->id,
							pPacket->idMap,
							pPacket->idData,
							pPacket->idOwner,
							pPacket->nData,
							pPacket->usPosX,
							pPacket->usPosY,
							pPacket->usAction,
							pPacket->usType,
							pPacket->ucOwnerType
							);
			}
#endif
			extern struct STAT_STRUCT	g_stat;
			InterlockedExchangeAdd(&g_stat.setMapGroupCrMonTime[GetProcessID()-MSGPORT_MAPGROUP_FIRST],
				clUsed);
			InterlockedIncrement(&g_stat.setMapGroupCrMonCount[GetProcessID()-MSGPORT_MAPGROUP_FIRST]);
			//---------------------------------------
		}
		break;
	case	MAPGROUP_CHANGETEAM:
		{
			ST_CHANGETEAM* pPacket = (ST_CHANGETEAM*)buf;
			switch(pPacket->nAction)
			{
			case	CHANGETEAM_ADD:
				{
					DEADLOOP_CHECK(PID, "MAPGROUP_CHANGETEAM: CHANGETEAM_ADD")
					CTeam *pUserTeam=UserManager()->GetTeamById(pPacket->idTeam);
					if(pUserTeam)
					{
						TeamMemberInfo info;
						info.id = pPacket->idUser;
						info.dwLookFace = pPacket->nData1;
						info.dwLev = pPacket->nData2;
						info.usProfession = pPacket->nData3;
						info.usProLev = pPacket->nData4;
						SafeCopy(info.szName,pPacket->teamName,_MAX_NAMESIZE);
						pUserTeam->AddMemberOnly(info);
					}
				}
				break;
			case	CHANGETEAM_CREATE:
				{
					DEADLOOP_CHECK(PID, "MAPGROUP_CHANGETEAM: CHANGETEAM_ADD")
					CTeam *pUserTeam=UserManager()->GetTeamById(pPacket->idTeam);
					if(!pUserTeam)
					{
						pUserTeam	= CTeam::CreateNew(m_idProcess, pPacket->idTeam, pPacket->idUser,false);
						if (pUserTeam)
						{
							UserManager()->AddTeam(pUserTeam);
						}
					}
					else
					{
						pUserTeam->SetTeamLeaderOnly(pPacket->idUser);
					}
				}
				break;
			case	CHANGETEAM_DES:
				{
					DEADLOOP_CHECK(PID, "MAPGROUP_CHANGETEAM: CHANGETEAM_DEL")
					CTeam *pUserTeam=UserManager()->GetTeamById(pPacket->idTeam);
					if(pUserTeam)
					{
						UserManager()->DelTeam(pUserTeam);
					}
				}
				break;
			case CHANGETEAM_SETLOCATION:
				{
					DEADLOOP_CHECK(PID, "MAPGROUP_CHANGETEAM: CHANGETEAM_SETTEAMMAP")
					DEBUG_TRY
					CTeam *pUserTeam=UserManager()->GetTeamById(pPacket->idTeam);
					if(pUserTeam)
						pUserTeam->SetLocation(pPacket->idUser,pPacket->nData1,pPacket->nData2,pPacket->nData3,false);
					DEBUG_CATCH4("CHANGETEAM_SETLOCATION crash! idTeam:%d,idUser:%d,nData1:%d",pPacket->idTeam,pPacket->idUser,pPacket->nData1)
				}
				break;
			case CHANGETEAM_CLRLOCATION:
				{
					DEADLOOP_CHECK(PID, "MAPGROUP_CHANGETEAM: CHANGETEAM_CLRLOCATION")
					DEBUG_TRY
					CTeam *pUserTeam=UserManager()->GetTeamById(pPacket->idTeam);
					if(pUserTeam)
						pUserTeam->ResetLocation(false);
					DEBUG_CATCH4("CHANGETEAM_CLRLOCATION crash! idTeam:%d,idUser:%d,nData1:%d",pPacket->idTeam,pPacket->idUser,pPacket->nData1)
				}
				break;
			case CHANGETEAM_ONLINESTATUSCHANGE:
				{
					DEADLOOP_CHECK(PID, "MAPGROUP_CHANGETEAM: CHANGETEAM_ONLINESTATUSCHANGE")
					DEBUG_TRY
					CTeam *pUserTeam=UserManager()->GetTeamById(pPacket->idTeam);
					if(pUserTeam)
						pUserTeam->SetOnlineStatus(pPacket->idUser,pPacket->nData1,false);
					DEBUG_CATCH4("CHANGETEAM_ONLINESTATUSCHANGE crash! idTeam:%d,idUser:%d,nData1:%d",pPacket->idTeam,pPacket->idUser,pPacket->nData1)
				}
				break;
			case	CHANGETEAM_DEL:
				{
					DEADLOOP_CHECK(PID, "MAPGROUP_CHANGETEAM: CHANGETEAM_DEL")
					DEBUG_TRY
					CTeam *pUserTeam=UserManager()->GetTeamById(pPacket->idTeam);
					if(pUserTeam)
					{
						///�縱���߶�Ա
						CUser *pUser=UserManager()->GetUser(pPacket->idUser);
						if(pUser)
						{
							CMsgTeam msg;
							if (msg.Create(pUser->GetID(),pUser->GetName(),_MSGTEAM_KICKOUT ))
								pUser->SendMsg(&msg);
							pUser->ClrTeam(true,false);
							
							CMsgPlayer msgPlayer;
							if (msgPlayer.Create(pUser->QueryRole()))
								pUser->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
						}
						pUserTeam->DelMemberOnly(pPacket->idUser);
					}
					DEBUG_CATCH3("CHANGETEAM_DEL crash! idTeam:%d,idUser:%d",pPacket->idTeam,pPacket->idUser)
				}				
				break;
			case	CHANGETEAM_DISMISS:
				{
					DEADLOOP_CHECK(PID, "MAPGROUP_CHANGETEAM: CHANGETEAM_DISMISS")
// 					CUserManager::Iterator pUser = UserManager()->NewEnum();
// 					while(pUser.Next())
// 					{
// 						if(pUser && pUser->GetTeam() && pUser->GetTeam()->GetID() == pPacket->idTeam)
// 							pUser->QueryChangeTeam()->DelTeam();
// 					}
				}
				break;
			case	CHANGETEAM_INFO:
				{
					DEADLOOP_CHECK(PID, "MAPGROUP_CHANGETEAM: CHANGETEAM_INFO")
// 					SOCKET_ID	idSocket	= pPacket->nData;
// 					OBJID		idAgent		= pPacket->idUser;
// 					CUserManager::Iterator pUser = UserManager()->NewEnum();
// 					while(pUser.Next())
// 					{
// 						if(pUser && pUser->GetTeam() && pUser->GetTeam()->GetID() == pPacket->idTeam)
// 							pUser->QueryChangeTeam()->SendInfo(idSocket, idAgent);
// 					}
				}
				break;
			case	CHANGETEAM_FLY:
				{
					DEADLOOP_CHECK(PID, "MAPGROUP_CHANGETEAM: CHANGETEAM_FLY")
// 					OBJID	idMap = pPacket->idTeam;
// 					CUser* pUser = UserManager()->GetUser(pPacket->idUser);
// 					if(pUser && pUser->IsAlive())
// 						pUser->FlyMap(idMap, LOWORD(pPacket->nData), HIWORD(pPacket->nData));
				}
				break;
				//20070202����:С�Ӹ���--------------------------
			case CHANGETEAM_CHANGENAME:
				{
					DEADLOOP_CHECK(PID, "MAPGROUP_CHANGETEAM: CHANGETEAM_CHANGENAME")
// 					CUserManager::Iterator pUser = UserManager()->NewEnum();
// 					while(pUser.Next())
// 					{
// 						if(pUser 
// 							&& pUser->GetTeam() 
// 							&& pUser->GetTeam()->GetID() == pPacket->idTeam
// 							&& pUser->GetID() == pPacket->idUser)
// 							pUser->ChangeTeamNameOnly(pPacket->teamName);
// 					}

				}
				break;

			case CHANGETEAM_TEAMTEXT:
				{
					DEADLOOP_CHECK(PID, "MAPGROUP_CHANGETEAM: CHANGETEAM_TEAMTEXT")
// 					CUser *pUser = UserManager()->GetUser(pPacket->idUser);
// 					if(pUser && pUser->GetTeam())
// 					{
// 						pUser->GetTeam()->SetTeamText(pPacket->teamName);
// 					}
				}
				break;
			case CHANGETEAM_REQUESTCHANGLEADER:
				{
					DEADLOOP_CHECK(PID, "MAPGROUP_CHANGETEAM: CHANGETEAM_REQUESTCHANGLEADER")
					DEBUG_TRY
					CTeam *pUserTeam=UserManager()->GetTeamById(pPacket->idTeam);
					if(pUserTeam)
					{
						OBJID idLeader=pUserTeam->GetNextTeamLeader();//GetOnlineTeamMemberOnly();
						if (idLeader!=ID_NONE)
						{
							pUserTeam->ChangeTeamLeader(idLeader);
							CUserPtr pLeader = UserManager()->GetUser(idLeader);
							if(pLeader)
							{
								CMsgTeam msg;
								if(msg.Create(idLeader,pLeader->GetName(), _MSGTEAM_CHG_LEADER_APPLY))
								{
									pUserTeam->BroadcastTeamMsg(&msg);
								}
								
								CMsgPlayer msgPlayer;
								if (msgPlayer.Create(pLeader->QueryRole()))
									pLeader->BroadcastRoomMsg(&msgPlayer, INCLUDE_SELF);
							}
						}

					}
					DEBUG_CATCH("CMapGroupKernel::CHANGETEAM_REQUESTCHANGLEADER");
				}
				break;
			case CHANGETEAM_CHANGLEADER:
				{
					DEADLOOP_CHECK(PID, "MAPGROUP_CHANGETEAM: CHANGETEAM_CHANGLEADER")
					CTeam *pUserTeam=UserManager()->GetTeamById(pPacket->idTeam);
					if(pUserTeam)
						pUserTeam->SetTeamLeaderOnly(pPacket->idUser);
				}
				break;
			case CHANGETEAM_CHANGLEADER_CLSEFFECT:
				{
					DEADLOOP_CHECK(PID, "MAPGROUP_CHANGETEAM: CHANGETEAM_CHANGLEADER_CLSEFFECT")
// 					CUserManager::Iterator pUser = UserManager()->NewEnum();
// 					while(pUser.Next())
// 					{
// 						if(pUser 
// 							&& pUser->GetTeam() 
// 							&& pUser->GetTeam()->GetID() == pPacket->idTeam
// 							&& pUser->GetID() == pPacket->idUser)
// 						{
// 							pUser->ClsStatus(STATUS_TEAMLEADER);
// 						}
// 					}
				}
				break;
			case CHANGETEAM_CHANGLEADER_SETEFFECT:
				{
					DEADLOOP_CHECK(PID, "MAPGROUP_CHANGETEAM: CHANGETEAM_CHANGLEADER_SETEFFECT")
// 					CUserManager::Iterator pUser = UserManager()->NewEnum();
// 					while(pUser.Next())
// 					{
// 						if(pUser 
// 							&& pUser->GetTeam() 
// 							&& pUser->GetTeam()->GetID() == pPacket->idTeam
// 							&& pUser->GetID() == pPacket->idUser)
// 						{
// 							pUser->SetStatus(STATUS_TEAMLEADER);
// 						}
// 					}
				}
				break;
				//------------------------------------------------------------
				//20070207����:С�����ñ��
			case CHANGETEAM_CHANGCONFIG:
				{
					DEADLOOP_CHECK(PID, "MAPGROUP_CHANGETEAM: CHANGETEAM_CHANGCONFIG")
// 					CUserManager::Iterator pUser = UserManager()->NewEnum();
// 					while(pUser.Next())
// 					{
// 						if(pUser 
// 							&& pUser->GetTeam() 
// 							&& pUser->GetTeam()->GetID() == pPacket->idTeam
// 							&& pUser->GetID() == pPacket->idUser)
// 						{
// 							pUser->GetTeam()->ChangeTeamInfoOnly(pPacket->nData);
// 						}
// 					}
				}
				break;
				//  [2007-9-27] ���	����״̬
			case CHANGETEAM_UPDATE_BROADCAST_SET:
				{
					DEADLOOP_CHECK(PID, "MAPGROUP_CHANGETEAM: CHANGETEAM_UPDATE_BROADCAST_SET")
// 					CUserManager::Iterator pUser = UserManager()->NewEnum();
// 					while(pUser.Next())
// 					{
// 						if(pUser && pUser->GetTeam() && pUser->GetTeam()->GetID() == pPacket->idTeam)
// 							pUser->UpdateBroadcastSet();
// 					}
				}
				break;


			case CHANGETEAM_PICKMONEY:
				{
					DEADLOOP_CHECK(PID, "MAPGROUP_CHANGETEAM: CHANGETEAM_PICKMONEY")
// 					if(pPacket->nData <= 0)
// 						return true;
// 
// 					CUser* pUser = UserManager()->GetUser(pPacket->idUser);
// 					if(pUser)
// 					{
// 						if (pUser->GetMoney() + pPacket->nData > _MAX_MONEYLIMIT)
// 						{
// 							pUser->SendSysMsg(g_objStrRes.GetStr(14043));//STR_TOOMUCH_MONEY
// 							return true;
// 						}
// 						//��ý�Ǯ
// 						BOOL bRet = pUser->GainMoney(pPacket->nData, SYNCHRO_TRUE);
// 						//////////////////////////////////////////////////////////////////////////
// 						//	20070710	���	������Ƚ�Ǯ��¼
// 						if(bRet && CItem::IsLogSaveMoney(pPacket->nData))
// 						{
// 							MyLogSaveCostlyItem(NULL, pUser->GetAccountID(), pUser->GetID(),
// 								pUser->GetName(), 0, 0, "!=", COSTLY_ITEM_OPER_TYPE_PICK, 
// 								0, 0, "ʱ�ձ�", pPacket->nData);
// 						}
// 					
// 						if(pPacket->nData >= PICK_MORE_MONEY)
// 						{
// 							CMsgAction	msg;
// 							IF_OK(msg.Create(pUser->GetID(), 
// 								pUser->GetPosX(), 
// 								pUser->GetPosY(), 
// 								pUser->GetDir(), 
// 								actionGetMoney, 
// 								pPacket->nData))
// 							{
// 								pUser->SendMsg(&msg);
// 							}
// 						}
// 						pUser->SendSysMsg(g_objStrRes.GetStr(12048), pPacket->nData);//STR_PICK_MONEY
// 					}
				}
				break;

			}
		}
		break;
	// syndicate -------------------------------------------------------------------------------
	case	KERNEL_CREATESYNDICATE:
		{
			DEADLOOP_CHECK(PID, "KERNEL_CREATESYNDICATE")
			CreateSyndicateInfo* pPacket = (CreateSyndicateInfo*)buf;
//			SynManager()->QuerySynchro()->CreateSyndicate(pPacket);
		}
		break;
	case	KERNEL_DESTROYSYNDICATE:
		{
			DEADLOOP_CHECK(PID, "KERNEL_DESTROYSYNDICATE")
			OBJID*	pId		= (OBJID*)buf;
			OBJID	idSyn	= pId[0];
			OBJID	idTarget= pId[1];
//			SynManager()->QuerySynchro()->DestroySyndicate(idSyn, idTarget);
		}
		break;
	case	KERNEL_CHANGESYNDICATE:
		{
			DEADLOOP_CHECK(PID, "KERNEL_CHANGESYNDICATE")
			SynFuncInfo0* pPacket = (SynFuncInfo0*)buf;
			CSynPtr pSyn = SynManager()->QuerySyndicate(pPacket->idSyn);
			if(pSyn)
				pSyn->ChangeSyndicate(pPacket);
		}
		break;
	case	MAPGROUP_CHANGENPC:
		{
			DEADLOOP_CHECK(PID, "MAPGROUP_CHANGENPC")
			char* ptr = (char*)buf;
			OBJID idNpc = *(int*)ptr;
			char* pszField = ptr + sizeof(int);
			char* pszData = pszField + strlen(pszField) + 1;
			int	  nData = atol(pszData);
			bool	bUpdate = UPDATE_FALSE;

			IRole*	pRole = RoleManager()->QueryRole(idNpc);
			if(!pRole)
				return false;

			CNpc*	pTarget;
			IF_NOT(pRole->QueryObj(OBJ_NPC, IPP_OF(pTarget)))
				return false;

			if(stricmp(pszField, "lookface") == 0)
			{
				//if(strcmp(szOpt, "=") == 0)
				{
					pTarget->SetInt(NPCDATA_LOOKFACE, nData, bUpdate);
					return true;
				}
			}
		}
		break;
	case	MAPGROUP_DELTRANSNPC:
		{
			DEADLOOP_CHECK(PID, "MAPGROUP_DELTRANSNPC")
			OBJID idMastNpc = *(OBJID*)buf;
			MapGroup(PID)->QueryIntraMsg()->DelTransNpc(idMastNpc, false);		// false: no brocast to other mapgroup
		}
		break;
	case MAPGROUP_NEW_SYN_TIME:
		if(m_pMsgPort->GetSize()-1 != PID)			//�������ͼ�������һ�ŵ�ͼ������������һ����ͼ��
		{
			char cTemp ='w';
			m_pMsgPort->Send( PID+1,MAPGROUP_NEW_SYN_TIME,VAR_TYPE( cTemp ),&cTemp );
		}
		//�������
 		if( PID == MSGPORT_MAPGROUP_FIRST )
 		{
			CSynManager* pSynManager = SynManager();
			assert( pSynManager );
			pSynManager->ProcChackSynWar();
 		}		
		break;
	case MAPGROUP_NEWDAY:	//���0321 ��һ��ĸ������ʱֻ��˰�ʱ��
		{
		}
		break;

		//20070123����:�л���ͼ��ʱ,���½�ɫ��
		//-------------------------------
	case MAPGROUP_NOWUPDATE_CALCULATED_PROPERTY:
		{
			OBJID*	pUserID = (OBJID*)buf;
			DEADLOOP_CHECK(PID, "MAPGROUP_NOWUPDATE_CALCULATED_PROPERTY: ")

			CUserPtr pUser = UserManager()->GetUser(*pUserID);
			if(pUser)
			{
				//ˢ�½���
				CUserJinMeiSysMgr* pMgr = MapGroup(PID)->GetUserJinmeiSysMgr();
				if(pMgr)
					pMgr->UpdateChange(pUser);

				pUser->m_idNowEudemon = pUser->m_idEudemonCur;

				pUser->UpdateCalculatedProperty();

				CMsgUserAttrib	msg;
				if(msg.Create(pUser->GetID(), _USERATTRIB_SPEED, pUser->AdjustSpeed(pUser->GetSpeed())))
					pUser->BroadcastRoomMsg(&msg, EXCLUDE_SELF);//�ų��Լ�
// 				CMsgUserInfo msg;
// 				if(msg.Create(pUser))
// 					MapGroup(PID)->QueryIntraMsg()->SendClientMsg(/*idSocket*/pUser->GetSocketID(), &msg);
			}
		}
		break;
	case WORLD_TEAMINVITE:
		{
			ST_TEAM_INVITE* pInfo = (ST_TEAM_INVITE*)buf;
			ProcessTeamInviteMsg(pInfo);
		}
		break;
	case WORLD_FRIENDAPPLY:
		{
			ST_FRIEND_APPLY* pInfo = (ST_FRIEND_APPLY*)buf;
			ProcessFriendApply(pInfo);
		}
		break;
	case MAPGROUP_AWARDINFO:
		{
			CLIENTMSG_PACKET0* pMsg = (CLIENTMSG_PACKET0*)buf;
			
			DEBUG_TRY
				CMsgAward::MSG_Info*	pMsgAward = (CMsgAward::MSG_Info*)pMsg;
			if(	pMsgAward->unMsgType == _MSG_GAINAWARDS)
			{
				// 				CUserPtr pUser = UserManager()->GetUser(pMsgAward->idUser);
				// 				if(pUser)
				// 				{
				switch(pMsgAward->nAction)
				{
					// 					case _GAINAWARD_INIT:
					// 						{
					// 							pUser->AddAwardExState(pMsgAward->gaInfo, pMsgAward->nAmount);
					// 						}
					// 						break;
				case _GAINAWARD_SPECILLINE:
					{
						g_SpecilLine=1;
// 						DEBUG_TRY
// 						//����Npc��������ר�� --jay
// 						CMsgTalk	msg;
// 						if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, REPLAY_OK_STR, NULL, _COLOR_WHITE, _TXTATR_SPECILLINE))
// 							MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(ID_NONE, &msg);
// 						DEBUG_CATCH("����Npc������!")

						MapManager()->CreateSpecilLineMap();
						NpcManager()->CreateSpecilLineNpc();
						//��ȡ���ɵ�ͼ//ֻ�ڵ�ͼ��һ --jay
						DEBUG_TRY
						if (m_idProcess==MSGPORT_MAPGROUP_FIRST)
						{
							if(WargameManager())
								WargameManager()->LoadMap();

							CSynSet* pSynSet=SynManager()->QuerySynSet();
							if (pSynSet)
							{
								for (int i=0;i<pSynSet->GetAmount();i++)
								{
									if(pSynSet->GetObjByIndex(i))
										pSynSet->GetObjByIndex(i)->CreateSynMap();
									else
										LogSave("ERROR:  pSynSet->GetObjByIndex(i) ��ȡ����ʧ��!");
								}	
							}
							else
								LogSave("ERROR: SynManager()->QuerySynSet() ��ȡ����!");
						}
// 						else
// 						{
// 							CSynSet* pSynSet=SynManager()->QuerySynSet();
// 							if (pSynSet)
// 							{
// 								for (int i=0;i<pSynSet->GetAmount();i++)
// 								{
// 									if(pSynSet->GetObjByIndex(i))
// 										pSynSet->GetObjByIndex(i)->SetInSynMapID();
// 									else
// 										LogSave("ERROR:  pSynSet->GetObjByIndex(i) ��ȡ����ʧ��!");
// 								}	
// 							}
// 							else
// 								LogSave("ERROR: SynManager()->QuerySynSet() ��ȡ����!");
// 						}
						DEBUG_CATCH("��ȡ���ɵ�ͼ����!!")
					}
					break;
				default:
					break;
				}
			}
			DEBUG_CATCH("CMapGroupKernel::MAPGROUP_AWARDINFO");
		}
		break;
	case MAPGROUP_VASMSG:
		{
			TRANSMSG_PACKET0*	pMsg = (TRANSMSG_PACKET0*)buf;
			
			DEBUG_TRY
			CMsgVAS::MSG_Info*	pMsgVAS = (CMsgVAS::MSG_Info*)pMsg->buf;
			CUser* pUser = UserManager()->GetUser(pMsgVAS->PlayerID);
			if (!pUser)
			{
				//��¼��ϸ��־
				if (pMsgVAS->usAction==_MSGVAS_BUY_SUC || pMsgVAS->usAction==_MSGVAS_POINT_BUY_SUC || pMsgVAS->usAction==_MSGVAS_BUY_ASALE_SUC)
				{
					for (int i=0; i<pMsgVAS->usGoodSize; i++)
						::LogSave("MAPGROUP_VASMSG�̳ǹ���,��Ҳ����� user=%u,itemtype=%u,amount=%d", pMsgVAS->PlayerID, pMsgVAS->nGoods[i].itemID, pMsgVAS->nGoods[i].itemAmount);
				}
				if (pMsgVAS->usAction==_MSGVAS_ADD_VAS_SUC)
				{
					CMsgVAS msg;
					if (msg.Create(pMsgVAS->nGoods[0].itemAmount, pMsgVAS->AccountID, pMsgVAS->PlayerID, _MSGVAS_SPEND_VAS, 0, 0, 0))
					{
						msg.m_pInfo->nGoods[0].dwData = 0;
						msg.m_pInfo->nGoods[0].itemAmount = 0;
						msg.m_pInfo->nGoods[0].itemID = 0;
						msg.m_pInfo->usGoodSize = 1;
						MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );
					}
					::LogSave("MAPGROUP_VASMSG ���ͨ��Ԫ��Ʊ��ȡ��Ԫ������δ���ҵ�����Ҳ�ɾ��Ԫ��Ʊ����Ԫ���ۻ� user=%u,itemtype=%u,amount=%d", pMsgVAS->PlayerID, pMsgVAS->nGoods[0].itemID, pMsgVAS->nGoods[0].itemAmount);
				}
				if (pMsgVAS->usAction==_MSGVAS_SPEND_VAS_SUC)
				{
					if (pMsgVAS->nGoods[0].itemID==_SPCITEM_VASEXCHANGE)
						::LogSave("MAPGROUP_VASMSG ���Ԫ��Ʊʱ�Ҳ������(userid:%u,amount:%u)",pMsgVAS->PlayerID, pMsgVAS->nGoods[0].dwData);
				}
				return true;
			}

			switch(pMsgVAS->usAction)
			{
			case _MSGVAS_BUY_SUC:
				{
					if (!pUser)
						return true;
// 					if (!pUser->EraseCheckArr(pMsgVAS->dwRandCheck)) 
// 					{
// 						::LogSave("***�յ����ɵ��̳ǹ�����Ϣ,�����������ͼˢ��Ʒ,�û�ID:%u,��ɫ��:%s", pUser->GetID(), pUser->GetName());
// 						return false;
// 					}
					int nCostVas = pUser->GetMoneyByType(5)-pMsgVAS->usVASPoint;//���ѵ�Ԫ������
					pUser->SetMoneyByType(pMsgVAS->usVASPoint, 5, false);//���ͬ�����û�������

					for (int i=0; i<pMsgVAS->usGoodSize; i++)
					{
						CItemTypeData* pItemTypeData = ItemType()->QueryItemType( pMsgVAS->nGoods[i].itemID );
						if (!pItemTypeData)
							return true;
						if (pMsgVAS->nGoods[i].dwData==_PAY_TYPE_VAS)//Ԫ��֧��,Ǯ�Ѿ����˺ŷ������۹���,ֱ�Ӹ���Ʒ
						{
							CItem* pItem = NULL;
							int nAmountLimit = pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT);
							int nPosition = pUser->QueryPackage()->GetAvailablePosition(pMsgVAS->nGoods[i].itemID, false);
							if (nPosition==-1)
							{
								pUser->SendSysMsg(_TXTATR_TOPSHOW, "�����ռ䲻��");
								return true;
							}
							if (CItem::IsCountable(pMsgVAS->nGoods[i].itemID))
							{
								int nItemCount = pMsgVAS->nGoods[i].itemAmount;
								while (nItemCount>0)
								{
									if (nItemCount>nAmountLimit)
									{
										if (pUser->QueryPackage()->GetIndexOrderAmount(nPosition)>0)
											pUser->QueryPackage()->SetIndexOrderAmount(nPosition, -1);
										pItem = pUser->AwardItem(pMsgVAS->nGoods[i].itemID, true, false, true, true, nAmountLimit);
										nItemCount-=nAmountLimit;
										if (!pItem)
											::LogSave("�̳ǹ������Ʒʧ��(1) user=%u,itemtype=%u,amount=%d", pUser->GetID(), pMsgVAS->nGoods[i].itemID, nAmountLimit);
									}
									else
									{
										if (pUser->QueryPackage()->GetIndexOrderAmount(nPosition)>0)
											pUser->QueryPackage()->SetIndexOrderAmount(nPosition, -1);
										pItem = pUser->AwardItem(pMsgVAS->nGoods[i].itemID, true, false, true, true, nItemCount);
										if (!pItem)
											::LogSave("�̳ǹ������Ʒʧ��(2) user=%u,itemtype=%u,amount=%d", pUser->GetID(), pMsgVAS->nGoods[i].itemID, nItemCount);
										nItemCount=0;
									}
								}
							}
							else
							{
								for (int j=0; j<pMsgVAS->nGoods[i].itemAmount; j++)
								{
									if (pUser->QueryPackage()->GetIndexOrderAmount(nPosition)>0)
										pUser->QueryPackage()->SetIndexOrderAmount(nPosition, -1);
									pItem = pUser->AwardItem(pMsgVAS->nGoods[i].itemID, true, false, true, true, pMsgVAS->nGoods[i].itemAmount);
									if (!pItem)
										::LogSave("�̳ǹ������Ʒʧ��(3) user=%u,itemtype=%u,amount=%d", pUser->GetID(), pMsgVAS->nGoods[i].itemID, pMsgVAS->nGoods[i].itemAmount);
								}
							}
							//pUser->QueryPackage()->SetIndexOrderAmount(nPosition, (-1)*nOrderAmount);
							pUser->SendSysMsg(_TXTATR_TOPSHOW, "����%dԪ��������%d��[%s]", nCostVas, pMsgVAS->nGoods[i].itemAmount, pItemTypeData->GetStr(ITEMTYPEDATA_NAME));
							if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
 								::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from mart by %d gold", pUser->GetID(), pUser->GetName(), pMsgVAS->nGoods[i].itemAmount, pMsgVAS->nGoods[i].itemID, nCostVas);
							//�����̳���Ʒ�ɹ��ˣ����̳�������Ʒ���⴦�� --jay
							pUser->UpdateMissionMsgByItemType(pMsgVAS->nGoods[i].itemID+SPECIALITEMBASE);

							vasLimitItem* pLimitItem = VASItem()->GetLimitItemByType(pMsgVAS->nGoods[i].itemID);
							if(pLimitItem)
							{
								int nAmount = max(0, pLimitItem->aSaleCount - pMsgVAS->nGoods[i].itemAmount);
								VASItem()->SetLimitItemAmount(pMsgVAS->nGoods[i].itemID, nAmount, true);
								pUser->SendLimitItem();
								CMsgVipList msg;//�������Э��㲥��������
								if (msg.Create(pUser->GetID(), VIPLIST_LINEBORAD, VIPLINEBORAD_VASLIMIT))
								{
									msg.Append(pMsgVAS->nGoods[i].itemID, nAmount, 0, 0, 0, pUser->GetName());
									MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
								}
							}
						}
					}
					CMsgUserAttrib	msg;
					if(msg.Create(pMsgVAS->PlayerID, _USERATTRIB_MONEY5, pMsgVAS->usVASPoint))
					{
						msg.Append(_USERATTRIB_MONEY4, pUser->GetMoneyByType(4));
						msg.Append(_USERATTRIB_MONEY3, pUser->GetMoneyByType(3) + pUser->GetGoldPoint());
						pUser->SendMsg(&msg);
					}
				}
				break;
			case _MSGVAS_POINT_BUY_SUC:
			{
				if (!pUser)
					return false;
				int nCostVas = pUser->GetGoldPoint()-pMsgVAS->usVASPoint;//���ѵĵ�ȯ����
				pUser->SetGoldPoint(pMsgVAS->usVASPoint);//���ͬ�����û�������

				for (int i=0; i<pMsgVAS->usGoodSize; i++)
				{
					CItemTypeData* pItemTypeData = ItemType()->QueryItemType( pMsgVAS->nGoods[i].itemID );
					if (!pItemTypeData)
						return false;
					if (pMsgVAS->nGoods[i].dwData%10==_PAY_TYPE_POINT)//��ȯ֧��
					{
						CItem* pItem = NULL;
	// 					if(pUser->SpendMoneyByType(m_pInfo->nGoods[i].dwData/10, 3, false))
	// 					{
							int nAmountLimit = pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT);
							int nPosition = pUser->QueryPackage()->GetAvailablePosition(pMsgVAS->nGoods[i].itemID, false);
							if (nPosition==-1)
							{
								pUser->SendSysMsg(_TXTATR_TOPSHOW, "�����ռ䲻��");
								return false;
							}
							if (CItem::IsCountable(pMsgVAS->nGoods[i].itemID))
							{
								int nItemCount = pMsgVAS->nGoods[i].itemAmount;
								while (nItemCount>0)
								{
									if (nItemCount>nAmountLimit)
									{
										if (pUser->QueryPackage()->GetIndexOrderAmount(nPosition)>0)
											pUser->QueryPackage()->SetIndexOrderAmount(nPosition, -1);
										pItem = pUser->AwardItem(pMsgVAS->nGoods[i].itemID, true, false, true, true, nAmountLimit, false, true, false, 1);
										nItemCount-=nAmountLimit;
										if (!pItem)
											::LogSave("�̳ǹ������Ʒʧ��(4) user=%u,itemtype=%u,amount=%d", pUser->GetID(), pMsgVAS->nGoods[i].itemID, nAmountLimit);
									}
									else
									{
										if (pUser->QueryPackage()->GetIndexOrderAmount(nPosition)>0)
											pUser->QueryPackage()->SetIndexOrderAmount(nPosition, -1);
										pItem = pUser->AwardItem(pMsgVAS->nGoods[i].itemID, true, false, true, true, nItemCount, false, true, false, 1);
										if (!pItem)
											::LogSave("�̳ǹ������Ʒʧ��(5) user=%u,itemtype=%u,amount=%d", pUser->GetID(), pMsgVAS->nGoods[i].itemID, nItemCount);
										nItemCount=0;
									}
								}
							}
							else
							{
								for (int j=0; j<pMsgVAS->nGoods[i].itemAmount; j++)
								{
									if (pUser->QueryPackage()->GetIndexOrderAmount(nPosition)>0)
										pUser->QueryPackage()->SetIndexOrderAmount(nPosition, -1);
									pItem = pUser->AwardItem(pMsgVAS->nGoods[i].itemID, true, false, true, true, pMsgVAS->nGoods[i].itemAmount, false, true, false, 1);
									if (!pItem)
										::LogSave("�̳ǹ������Ʒʧ��(6) user=%u,itemtype=%u,amount=%d", pUser->GetID(), pMsgVAS->nGoods[i].itemID, pMsgVAS->nGoods[i].itemAmount);
								}
							}
							
							pUser->SendSysMsg(_TXTATR_TOPSHOW, "����%d��ȯ������%d��[%s]", pMsgVAS->nGoods[i].dwData/10, pMsgVAS->nGoods[i].itemAmount, pItemTypeData->GetStr(ITEMTYPEDATA_NAME));
							if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
 								::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from mart by %d ticket", pUser->GetID(), pUser->GetName(), pMsgVAS->nGoods[i].itemAmount, pMsgVAS->nGoods[i].itemID, pMsgVAS->nGoods[i].dwData/10);

							vasLimitItem* pLimitItem = VASItem()->GetLimitItemByType(pMsgVAS->nGoods[i].itemID);
							if(pLimitItem)
							{
								int nAmount = max(0, pLimitItem->aSaleCount - pMsgVAS->nGoods[i].itemAmount);
								VASItem()->SetLimitItemAmount(pMsgVAS->nGoods[i].itemID, nAmount, true);
								pUser->SendLimitItem();
								CMsgVipList msg;//�������Э��㲥��������
								if (msg.Create(pUser->GetID(), VIPLIST_LINEBORAD, VIPLINEBORAD_VASLIMIT))
								{
									msg.Append(pMsgVAS->nGoods[i].itemID, nAmount, 0, 0, 0, pUser->GetName());
									MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg,OBJID(0));
								}
							}
//					}
					}
				}
				CMsgUserAttrib	msg;
				if(msg.Create(pMsgVAS->PlayerID, _USERATTRIB_MONEY5, pUser->GetMoneyByType(5)))
				{
					msg.Append(_USERATTRIB_MONEY4, pUser->GetMoneyByType(4));
					msg.Append(_USERATTRIB_MONEY3, pUser->GetMoneyByType(3)+pUser->GetGoldPoint());
					pUser->SendMsg(&msg);
				}
			}
			break;
			case _MSGVAS_ACTIVITY_STATES:
				{
					pUser->SetActivityAward(pMsgVAS->usVASPoint / 1000 ,pMsgVAS->usVASPoint % 1000, false);
				}
			break;

			case _MSGVAS_MOBCHARGE_STATE:
				{
					pUser->SetMobChargeAward(pMsgVAS->usVASPoint / 1000 ,pMsgVAS->usVASPoint % 1000, false);
				}
			break;
			case _MSGVAS_BUY_LOTTERY_SUC://���Ʊ�ɹ�
				{
					if (pUser->QueryTreasure())
					{
						int nCostVas = nCostVas = pUser->QueryTreasure()->GetTreasurePrice(pMsgVAS->nGoods[0].itemAmount, pMsgVAS->nGoods[0].itemID);//�˴ο����ӻ���Ԫ������
						pUser->SetTreasureCost(pUser->GetTreasureCost()+nCostVas);
						if (!pUser->QueryTreasure()->HasEnoughSpace(pMsgVAS->nGoods[0].itemID))
						{
							::LogSave("��ҿ����ӿ���Ԫ�����ֿռ䲻�� user=%u,times=%d,group=%d", pUser->GetID(),pMsgVAS->nGoods[0].itemID, pMsgVAS->nGoods[0].itemAmount);
							return true;
						}
						pUser->QueryTreasure()->AwardLotteryItem(pMsgVAS->nGoods[0].itemID, pMsgVAS->nGoods[0].itemAmount);
					}
					else
						::LogSave("��ҿ�����δ�����Ʒ user=%u,times=%d,group=%d", pUser->GetID(), pMsgVAS->nGoods[0].itemID, pMsgVAS->nGoods[0].itemAmount);

					CMsgUserAttrib	msg;//ͬ�����ͻ���
					if(msg.Create(pUser->GetID(), _USERATTRIB_MONEY5, pMsgVAS->usVASPoint))
					{
						pUser->SendMsg(&msg);
					}
				}
				break;
			case _MSGVAS_BUY_ASALE_SUC:
				{
					if (!pUser)
						return true;
					int nCostVas = pUser->GetMoneyByType(5)-pMsgVAS->usVASPoint;//���ѵ�Ԫ������
					pUser->SetMoneyByType(pMsgVAS->usVASPoint, 5, false);//���ͬ�����û�������
					
					for (int i=0; i<pMsgVAS->usGoodSize; i++)
					{
						CItemTypeData* pItemTypeData = ItemType()->QueryItemType( pMsgVAS->nGoods[i].itemID );
						if (!pItemTypeData)
						{
							::LogSave("��Ҵ��ؼ��̳ǹ�����һ�������ڵ���Ʒ,user=%u,itemtype=%u,amount=%d",pUser->GetID(), pMsgVAS->nGoods[i].itemID, pMsgVAS->nGoods[i].itemAmount);
							return true;
						}
						vasItem* pVasItem = VASItem()->GetASaleItemByType(pMsgVAS->nGoods[i].itemID);
						if (pMsgVAS->nGoods[i].dwData==_PAY_TYPE_VAS)//Ԫ��֧��,Ǯ�Ѿ����˺ŷ������۹���,ֱ�Ӹ���Ʒ
						{
							CItem* pItem = NULL;
							int nAmountLimit = pItemTypeData->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT);
							int nPosition = pUser->QueryPackage()->GetAvailablePosition(pMsgVAS->nGoods[i].itemID, false);
							if (nPosition==-1)
							{
								pUser->SendSysMsg(_TXTATR_TOPSHOW, "�����ռ䲻��");
								::LogSave("�̳Ǹ��ؼ���Ʒʱ��ұ������� user=%u,itemtype=%u,amount=%d",pUser->GetID(), pMsgVAS->nGoods[i].itemID, pMsgVAS->nGoods[i].itemAmount);
								return true;
							}
							if (CItem::IsCountable(pMsgVAS->nGoods[i].itemID))
							{
								int nItemCount = pMsgVAS->nGoods[i].itemAmount;
								while (nItemCount>0)
								{
									if (nItemCount>nAmountLimit)
									{
										if (pUser->QueryPackage()->GetIndexOrderAmount(nPosition)>0)
											pUser->QueryPackage()->SetIndexOrderAmount(nPosition, -1);
										pItem = pUser->AwardItem(pMsgVAS->nGoods[i].itemID, true, false, true, true, nAmountLimit, false, true, false, 1);
										nItemCount-=nAmountLimit;
										if (!pItem)
											::LogSave("�̳ǹ�����ؼ���Ʒʧ��(4) user=%u,itemtype=%u,amount=%d", pUser->GetID(), pMsgVAS->nGoods[i].itemID, nAmountLimit);
									}
									else
									{
										if (pUser->QueryPackage()->GetIndexOrderAmount(nPosition)>0)
											pUser->QueryPackage()->SetIndexOrderAmount(nPosition, -1);
										pItem = pUser->AwardItem(pMsgVAS->nGoods[i].itemID, true, false, true, true, nItemCount, false, true, false, 1);
										if (!pItem)
											::LogSave("�̳ǹ�����ؼ���Ʒʧ��(5) user=%u,itemtype=%u,amount=%d", pUser->GetID(), pMsgVAS->nGoods[i].itemID, nItemCount);
										nItemCount=0;
									}
								}
							}
							else
							{
								for (int j=0; j<pMsgVAS->nGoods[i].itemAmount; j++)
								{
									if (pUser->QueryPackage()->GetIndexOrderAmount(nPosition)>0)
										pUser->QueryPackage()->SetIndexOrderAmount(nPosition, -1);
									pItem = pUser->AwardItem(pMsgVAS->nGoods[i].itemID, true, false, true, true, pMsgVAS->nGoods[i].itemAmount, false, true, false, 1);
									if (!pItem)
										::LogSave("�̳ǹ�����ؼ���Ʒʧ��(6) user=%u,itemtype=%u,amount=%d", pUser->GetID(), pMsgVAS->nGoods[i].itemID, pMsgVAS->nGoods[i].itemAmount);
								}
							}
							//pUser->QueryPackage()->SetIndexOrderAmount(nPosition, (-1)*nOrderAmount);
							pUser->SendSysMsg(_TXTATR_TOPSHOW, "����%dԪ��������%d��[%s]", nCostVas, pMsgVAS->nGoods[i].itemAmount, pItemTypeData->GetStr(ITEMTYPEDATA_NAME));
							if (pItem && pItem->IsCostlyItem())//goto ������Ʒ��¼
								::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from mart by %d gold", pUser->GetID(), pUser->GetName(), pMsgVAS->nGoods[i].itemAmount, pMsgVAS->nGoods[i].itemID, nCostVas);
							//��������Ѿ�������ؼ���Ʒ��¼
							if (pItem && pVasItem)
							{
								int nTemp = 1;
								for (int nCount = 0; nCount<ASALE_ITEM_AMOUNT; nCount++)
								{
									if (pUser->GetASales()/nTemp % 100 == 0)
									{
										pUser->SetASales(pUser->GetASales()+(pVasItem->id-10000)*nTemp);
										break;
									}
									nTemp *= 100;
								}
								VASItem()->SetASaleItemAmount(pVasItem->itemType, pVasItem->aSaleCount-pMsgVAS->nGoods[i].itemAmount, true);
								CMsgMainServer msg2;//ͬ����������·
								msg2.Create(4,pVasItem->itemType,true);
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(&msg2, (OBJID)0);
								pUser->SendASalesItem();
							}
						}
					}
					CMsgUserAttrib	msg;
					if(msg.Create(pMsgVAS->PlayerID, _USERATTRIB_MONEY5, pMsgVAS->usVASPoint))
					{
						pUser->SendMsg(&msg);
					}
				}
				break;
			case _MSGVAS_ADD_VAS_SUC:
				{
					pUser->SetMoneyByType(pMsgVAS->usVASPoint, 5, false);//���ͬ�����û�������

					pUser->SendSysMsg("��ȡ�ɹ�");

					CMsgUserAttrib	msg;
					if(msg.Create(pMsgVAS->PlayerID, _USERATTRIB_MONEY5, pMsgVAS->usVASPoint))
						pUser->SendMsg(&msg);

					if (pMsgVAS->nGoods[0].itemID==_SPCITEM_VASEXCHANGE)
					{
						CItem* pItem = pUser->GetItem(pMsgVAS->nGoods[0].dwData);
						if (!pItem)
						{
							CMsgVAS msg;
							if (msg.Create(pMsgVAS->nGoods[0].itemAmount, pMsgVAS->AccountID, pMsgVAS->PlayerID, _MSGVAS_SPEND_VAS, 0, 0, 0))
								MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg( &msg );
							::LogSave("�Ҳ���Ҫɾ����Ԫ��Ʊ,�ۻ�Ԫ��(userid:%u,amount:%u)",pUser->GetID(),pMsgVAS->nGoods[0].itemAmount);
							return true;
						}
						ASSERT(pUser->EraseItem(pItem->GetID(), true));
					}
				}
				break;
			case _MSGVAS_ADD_VAS_SUC_CPST:
				{
					pUser->SetMoneyByType(pMsgVAS->usVASPoint, 5, false);//���ͬ�����û�������
					
					pUser->SendSysMsg("��ȡ�ɹ�");
					
					CMsgUserAttrib	msg;
					if(msg.Create(pMsgVAS->PlayerID, _USERATTRIB_MONEY5, pMsgVAS->usVASPoint))
						pUser->SendMsg(&msg);
				}
				break;
			case _MSGVAS_SPEND_VAS_SUC:
				{
					pUser->SetMoneyByType(pMsgVAS->usVASPoint, 5, false);//���ͬ�����û�������
					
					CMsgUserAttrib	msg;
					if(msg.Create(pMsgVAS->PlayerID, _USERATTRIB_MONEY5, pMsgVAS->usVASPoint))
						pUser->SendMsg(&msg);

					if (pMsgVAS->nGoods[0].itemID==_SPCITEM_VASEXCHANGE)
					{
						if(!pUser->QueryPackage())
						{
							::LogSave("MAPGROUP_VASMSG 1���Ԫ��Ʊʧ��(userid:%u,amount:%u)",pUser->GetID(), pMsgVAS->nGoods[0].dwData);
							return true;
						}
						int nPosition = pUser->QueryPackage()->GetAvailablePosition(pMsgVAS->nGoods[0].itemID, false);
						if (nPosition==-1)
						{
							pUser->SendSysMsg(_TXTATR_TOPSHOW, "�����ռ䲻��");
							::LogSave("MAPGROUP_VASMSG �������������Ԫ��Ʊʧ��(userid:%u,amount:%u)",pUser->GetID(), pMsgVAS->nGoods[0].dwData);
							return true;
						}
						if (pUser->QueryPackage()->GetIndexOrderAmount(nPosition)>0)
							pUser->QueryPackage()->SetIndexOrderAmount(nPosition, -1);
						CItem* pItem = pUser->AwardItem(_SPCITEM_VASEXCHANGE, true,false,false,false,pMsgVAS->nGoods[0].dwData);
						if (!pItem)
						{
							::LogSave("MAPGROUP_VASMSG 2���Ԫ��Ʊʧ��(userid:%u,amount:%u)",pUser->GetID(), pMsgVAS->nGoods[0].dwData);
							return true;
						}
						pUser->SetVasTicketAmount(pUser->GetVasTicketAmount()+pMsgVAS->nGoods[0].dwData);
					}
				}
				break;
			default:
				{
					SOCKET_ID	idSocket= pMsg->idSocket;
					OBJID		idNpc	= pMsg->idNpc;
					OBJID	idMsg	= pMsg->idPacket;
					char*	pBuf	= pMsg->buf;
					int		nMsgLen	= pMsg->nSize;
					int		nTrans	= pMsg->nTrans;
					
					DEADLOOP_CHECK(PID, "MAPGROUP_TRANSMITMSG: ")
					if(idSocket != SOCKET_NONE)
					{
						ProcessClientMsg(idSocket, idMsg, pBuf, nMsgLen, nTrans);
					}
				}
				break;
			}
			DEBUG_CATCH("CMapGroupKernel::MAPGROUP_VASMSG");
		}
		break;
	default:
		LOGWARNING("CWorldKernel::ProcessMsg() idPacket = %u",idPacket);
	}

	return true;		// return false : ��Ϣ�����쳣������رա�
}

//////////////////////////////////////////////////////////////////////
bool CMapGroupKernel::OnTimer(time_t tCurr)
{
	DEBUG_TRY
	ReconnectDB();
	DEBUG_CATCH("CMapGroupKernel::OnTimer.ReconnectDB");

	// TODO: ���ڴ����ʱ�Ӵ������
	MapGroup(PID)->OnTimer(tCurr);

	return true;		// return false : ��Ϣ�����쳣������رա�
}

//////////////////////////////////////////////////////////////////////
bool CMapGroupKernel::ProcessClientMsg(SOCKET_ID idSocket, OBJID idMsg, const char *pbufMsg, int nSize, int nTrans)
{
	// TODO: ���ڴ���ӿͻ����ϴ���Ϣ�Ĵ������
	if(!pbufMsg || nSize <= 0 || nSize >_MAX_MSGSIZE)
        return false;
	DEADLOOP_CHECK(PID, "ProcessClientMsg: ")
//#define MSG_AMOUNT_CHECK
#ifdef MSG_AMOUNT_CHECK
	static int nCheckMsgAmount = 0;
#endif
	CNetMsg* pMsg	=CNetMsg::CreateClientMsg(m_idProcess, idSocket, idMsg, pbufMsg, nSize, nTrans);
	if(pMsg)
	{
		try {
			clock_t tStart = clock();

			// msg count
			CUserPtr pUser = UserManager()->GetUser(pMsg);
			if (pUser)
				pUser->m_dwMsgCount++;

			UINT nTickCount=GetTickCount();///��ʼ��ʱ
			// msg process
			pMsg->Process(this);

#ifdef MSG_AMOUNT_CHECK
			nCheckMsgAmount ++;
			m_setCheckMsg[idMsg]++;
			
			if(nCheckMsgAmount > 30)
			{
				LOGERROR("msgCheck begain!!!!");
				map<int,int>::iterator it = m_setCheckMsg.begin();
				for(; it != m_setCheckMsg.end() ; it++)
				{
					LOGERROR("msg%u:%u",it->first,it->second);
				}
				LOGERROR("msgCheck end!!!!");
				nCheckMsgAmount = 0;
			}
#endif

#define MSG_TIME_CHECK
#ifdef MSG_TIME_CHECK
			nTickCount=GetTickCount()-nTickCount;
			if(nTickCount>100)///����ʱ����־
			{
				pMsg->DumpMsg("ProcessClientMsg overtime(%dms)",nTickCount);
			}
// 			if(pMsg->GetSize()>800)
// 			{
// 				pMsg->DumpMsg("ProcessClientMsg oversize(%dbyte)",pMsg->GetSize());
// 			}
#endif
			clock_t tUsed = clock()-tStart;
			extern struct STAT_STRUCT	g_stat;
			if(tUsed > g_stat.setDebug[0])
			{
				InterlockedExchange(&g_stat.setDebug[0], tUsed);		// debug
//				InterlockedExchange(&g_stat.setDebug[3], idMsg);		// debug
			}
		}
		catch(...)
		{
// 			CNetMsg::DumpMsg(pMsg);
			UINT		usAction=0;
			memcpy(&usAction,pMsg->GetBuf(),sizeof(UINT));
			//ASSERT(!"catch");
			::LogSave("exception catch at CGameSocket::ProcessClientMsg()! MsgType:%d, SocketID:%u,action:%u", idMsg, idSocket,usAction);
			::LogSave("nSize = %d,||pbufMsg = %s",nSize,pbufMsg);//20061025����.������.�Լ��ֶ����⿪����
		}
		SAFE_DELETE(pMsg);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CMapGroupKernel::BroadcastMapGroupMsg(SOCKET_ID idSocket, OBJID idMsg, const char *pbufMsg, int nSize)
{
	if(!pbufMsg || nSize <= 0 || nSize >_MAX_MSGSIZE)
        return false;
	DEADLOOP_CHECK(PID, "BroadcastMapGroupMsg: ")

	int nTransmit = 0;
	CNetMsg* pMsg	=CNetMsg::CreateClientMsg(m_idProcess, idSocket, idMsg, pbufMsg, nSize, nTransmit);
	if(pMsg)
	{
		/*
		try {
			RoleManager()->BroadcastMapGroupMsg(pMsg);
		}
		catch(...)
		{
//			CNetMsg::DumpMsg(pMsg);
			ASSERT(!"catch");
			::LogSave("exception catch at CGameSocket::ProcessMsg()! MsgType:%d, SocketID:%u", idMsg, idSocket);
		}
		*/
		UINT		usAction=0;
		//DEBUG_TRY
		try
		{
			memcpy(&usAction,pMsg->GetBuf(),sizeof(UINT));
			RoleManager()->BroadcastMapGroupMsg(pMsg);
		}
		catch(...)
		{
			LogSave("CMapGroupKernel::BroadcastMapGroupMsg  crash()! msgtype:%d,action:%d",pMsg->GetType(),usAction);
			if(pMsg->GetType()==_MSG_TALK)
			{
				///debug
				CMsgTalk::Dump((char*)pMsg->GetBuf(),pMsg->GetSize());
			}

		}
//		DEBUG_CATCH4("exception catch at CMapGroupKernel::BroadcastMapGroupMsg! MsgType:%d, SocketID:%u,action:%u", idMsg, idSocket,usAction)
		SAFE_DELETE(pMsg);
	}

	return true;
}

bool CMapGroupKernel::BroadcastPhyleMapGroupMsg( int nPhyle,SOCKET_ID idSocket, OBJID idMsg, const char *pbufMsg, int nSize )
{
	if( !nPhyle || !pbufMsg || nSize <= 0 || nSize >_MAX_MSGSIZE)
        return false;
	DEADLOOP_CHECK(PID, "BroadcastPhyleMapGroupMsg: ")

	int nTransmit = 0;
	CNetMsg* pMsg	=CNetMsg::CreateClientMsg(m_idProcess, idSocket, idMsg, pbufMsg, nSize, nTransmit);
	if(pMsg)
	{
		/*
		try
		{
			UserManager()->BroadcastPhyleMapGroupMsg( pMsg,nPhyle );
		}
		catch(...)
		{
//			CNetMsg::DumpMsg(pMsg);
			ASSERT(!"catch");
			::LogSave("exception catch at CGameSocket::ProcessMsg()! MsgType:%d, SocketID:%u", idMsg, idSocket);
		}
		*/
//-------���---2007/05/23-----
		UINT		usAction=0;
		DEBUG_TRY
			memcpy(&usAction,pMsg->GetBuf(),sizeof(UINT));
			UserManager()->BroadcastPhyleMapGroupMsg( pMsg,nPhyle );
		DEBUG_CATCH4("exception catch at CMapGroupKernel::BroadcastPhyleMapGroupMsg! MsgType:%d, SocketID:%u,action:%u", idMsg, idSocket,usAction)
		SAFE_DELETE(pMsg);
	}

	return true;
}
//////////////////////////////////////////////////////////////////////
bool CMapGroupKernel::SendMsg(CNetMsg* pNetMsg)
{
	ASSERT(pNetMsg);
	if(!pNetMsg)
		return false;

	if(!pNetMsg->IsNpcMsg())
		return SendMsg(pNetMsg->GetSocketID(), pNetMsg->GetType(), pNetMsg->GetBuf(), pNetMsg->GetSize());
// 	else
// 	{
// 		//zeng �޸� ���͸�npc��������CMsgPlayer��Ϣ���޸�
// 		if(pNetMsg->GetType()==_MSG_PLAYER)
// 		{
// 			CMsgPlayerNpc msg;
// 			msg.Create((CMsgPlayer*)pNetMsg);
// 			return SendNpcMsg(msg.GetNpcID(), msg.GetType(), msg.GetBuf(), msg.GetSize());
// 		}
// 		DWORD dwType = pNetMsg->GetType();
// 		if(dwType != 1014
// 			&& dwType != 3005
// 			&& dwType != 1010
// 			&& dwType != 20001
// 			&& dwType != 1022
// 			&& dwType != 1017
// 			&& dwType != 1064)
// 		{
// 			if(dwType != 1105
// 				&& dwType != 2049
// 				&& dwType != 2049
// 				&& dwType != 2049)
// 				LOGWARNING("need send to npc? dwType = %u",dwType);
// 			return true;
// 		}
// 		return SendNpcMsg(pNetMsg->GetNpcID(), pNetMsg->GetType(), pNetMsg->GetBuf(), pNetMsg->GetSize());
//	}
}

//////////////////////////////////////////////////////////////////////
bool CMapGroupKernel::SendMsg(SOCKET_ID idSocket, OBJID idMsg, const char *pBuf, int nMsgLen)
{
	// TODO: ���ڴ���ӿͻ����´���Ϣ�Ĵ���
	if (!pBuf)
	{
		STACK_DUMP();
		::LogSave("Error: null msg point found in CGameSocket::SendMsg.");
		return false;
	}

	if (_MSG_NONE == idMsg)
	{
		STACK_DUMP();
		::LogSave("Error: invalid msg type in CGameSocket::SendMsg().");
		return false;
	}
	if (nMsgLen > MAX_PACKETSIZE)
	{
		STACK_DUMP();
		::LogSave("Error: invalid msg size %d in CGameSocket::SendMsg().msg type:%d",nMsgLen,idMsg);
		if(idMsg==_MSG_TALK)
			CMsgTalk::Dump((char*)pBuf,nMsgLen);
		return false;
	}
/*
	try {
		char	bufPacket[MAX_MESSAGESIZE];
		SENDCLIENTMSG_PACKET0* pPacket = (SENDCLIENTMSG_PACKET0*)bufPacket;
		pPacket->idSocket	= idSocket;
		pPacket->idPacket	= idMsg;
		pPacket->nSize		= nMsgLen;
		memcpy(pPacket->buf, pBuf, nMsgLen);
		m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_SENDCLIENTMSG, STRUCT_TYPE(bufPacket), &bufPacket);
		return true;
	}
	catch(...)
	{
		ASSERT(!"catch");
		::LogSave("Error: exception catched in CGameSocket::SendMsg(%d).", idMsg);
		return false;
	}			
*/
	//-------���---2007/05/23-----
//	DEBUG_TRY 
	try{
		char	bufPacket[MAX_MESSAGESIZE];
		SENDCLIENTMSG_PACKET0* pPacket = (SENDCLIENTMSG_PACKET0*)bufPacket;
		pPacket->idSocket	= idSocket;
		pPacket->idPacket	= idMsg;
		pPacket->nSize		= nMsgLen;
		/*
#ifdef _DEBUG
		CString str;
		str.Format("idSocket:%d,idMsg:%d,nSize:%d/n",idSocket,idMsg,nMsgLen);
		::OutputDebugString(str.GetBuffer(0));
#endif
		*/
		memcpy(pPacket->buf, pBuf, nMsgLen);
		m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_SENDCLIENTMSG, STRUCT_TYPE(bufPacket), &bufPacket);
		return true;
	
	}
	catch(...)
	{
		if(idMsg==_MSG_TALK)
		{
			///debug
			CMsgTalk::Dump((char*)pBuf,nMsgLen);

		}
		LogSave("Error: exception catched in CGameSocket::SendMsg(%d).", idMsg);
		return false;
	}
// 	DEBUG_CATCHF2("Error: exception catched in CGameSocket::SendMsg(%d).", idMsg)
		
	return true;

}

//////////////////////////////////////////////////////////////////////
// bool CMapGroupKernel::ProcessNpcMsg(OBJID idNpc, OBJID idMsg, const char *pbufMsg, int nSize, int nTrans)
// {
// 	// TODO: ���ڴ����NPC�������ϴ���Ϣ�Ĵ������
// 	if(!pbufMsg || nSize <= 0 || nSize >_MAX_MSGSIZE)
//         return false;
// 	DEADLOOP_CHECK(PID, "ProcessNpcMsg: ")
// 
// 	CNetMsg* pMsg	=CNetMsg::CreateNpcMsg(m_idProcess, idNpc, idMsg, pbufMsg, nSize, nTrans);
// 	if(pMsg)
// 	{
// 		/*
// 		try {
// 			pMsg->Process(this);
// 		}
// 		catch(...)
// 		{
// //			CNetMsg::DumpMsg(pMsg);
// 			ASSERT(!"catch");
// 			::LogSave("exception catch at CGameSocket::ProcessNpcMsg()! MsgType:%d, NpcID:%u", idMsg, idNpc);
// 		}
// 		*/
// 		//-------���---2007/05/23-----
// 		DEBUG_TRY
// 			pMsg->Process(this);
// 		DEBUG_CATCH3("exception catch at CGameSocket::ProcessNpcMsg()! MsgType:%d, NpcID:%u", idMsg, idNpc)
// 		SAFE_DELETE(pMsg);
// 	}
// 
// 	return true;
// }
// 
// //////////////////////////////////////////////////////////////////////
// bool CMapGroupKernel::SendNpcMsg(OBJID idNpc, OBJID idMsg, const char *pBuf, int nMsgLen)
// {
// 	// TODO: ���ڴ����NPC�������´���Ϣ�Ĵ���
// 	if (!pBuf)
// 	{
// 		::LogSave("Error: null msg point found in CGameSocket::SendNpcMsg.");
// 		return false;
// 	}
// 
// 	if (_MSG_NONE == idMsg)
// 	{
// 		::LogSave("Error: invalid msg type in CGameSocket::SendNpcMsg().");
// 		return false;
// 	}
// /*
// 	try {
// 		char	bufPacket[MAX_MESSAGESIZE];
// 		SENDNPCMSG_PACKET0* pPacket = (SENDNPCMSG_PACKET0*)bufPacket;
// 		pPacket->idNpc		= idNpc;
// 		pPacket->idPacket	= idMsg;
// 		pPacket->nSize		= nMsgLen;
// 		memcpy(pPacket->buf, pBuf, nMsgLen);
// 		m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_SENDNPCMSG, STRUCT_TYPE(bufPacket), &bufPacket);
// 		return true;
// 	}
// 	catch(...)
// 	{
// 		ASSERT(!"catch");
// 		::LogSave("Error: exception catched in CGameSocket::SendNpcMsg().");
// 		return false;
// 	}			
// */
// 	//-------���---2007/05/23-----
// 	DEBUG_TRY
// 		
// 		char	bufPacket[MAX_MESSAGESIZE];
// 		SENDNPCMSG_PACKET0* pPacket = (SENDNPCMSG_PACKET0*)bufPacket;
// 		pPacket->idNpc		= idNpc;
// 		pPacket->idPacket	= idMsg;
// 		pPacket->nSize		= nMsgLen;
// 		memcpy(pPacket->buf, pBuf, nMsgLen);
// 		m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_SENDNPCMSG, STRUCT_TYPE(bufPacket), &bufPacket);
// 		return true;
// 	DEBUG_CATCHF("Error: exception catched in CGameSocket::SendNpcMsg().")
// 
// 	return true;
// }

//////////////////////////////////////////////////////////////////////
bool CMapGroupKernel::CloseSocket(SOCKET_ID idSocket)		// ֱ�ӹر�socket
{
	// TODO: ���ڴ���ӹرտͻ���SOCKET�Ĵ���
	m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_BREAKCONNECT, VARTYPE_INT, &idSocket);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
void CMapGroupKernel::SynchroData()
{
// 	CGameMap* pMap = MapManager()->QueryMap(SET_WHITE_SYN);
// 	if(pMap)
// 		_SynManager(PID)->QueryModify()->SetMapSynID(pMap->GetID(), pMap->GetOwnerID());
// 	 pMap = MapManager()->QueryMap(SET_BLACK_SYN);
// 	if(pMap)
// 		_SynManager(PID)->QueryModify()->SetMapSynID(pMap->GetID(), pMap->GetOwnerID());
}

///////////////////////////////////////////////////////////////////////////////////////
// global entry
///////////////////////////////////////////////////////////////////////////////////////
IMapGroup* IMapGroup::CreateNew()
{
	return new CMapGroupKernel;
}

void CMapGroupKernel::ProcessTeamInviteMsg(ST_TEAM_INVITE *pTeamInvite)
{	
	switch(pTeamInvite->unAction)
	{
	case TEAM_INVITE_ACTION_TYPE_INVITE_REQ:
		{
			if(strlen(pTeamInvite->szTargetName) < 1)
				break;

			CUserPtr pTarget = UserManager()->GetUser(pTeamInvite->szTargetName);
			if(!pTarget)
				return;
			
			ST_TEAM_INVITE st;
			memcpy(&st, pTeamInvite, sizeof(ST_TEAM_INVITE));

			CTeam* pTeam = pTarget->GetTeam();
			if(pTeam)
			{				
				st.unAction = TEAM_INVITE_ACTION_TYPE_INVITE_NANK;
				st.nRet = 16010;	//	ֱ��ʹ����Դ,�Է��Ѿ��ж�����
				MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, WORLD_TEAMINVITE, STRUCT_TYPE(ST_TEAM_INVITE), (const char*)&st);
			}

			else	//	������û�ж���
			{
				BOOL bValid = TRUE;
				int	nRet = 0;
				//	�����ͻ���
				if(!pTarget->GetIsAllowTeam())
				{	
					bValid = FALSE;
					nRet = 13029;
				}
				else if (pTarget->GetTeam())
				{
					bValid = FALSE;
					nRet = 13028;
				}
				else if (!pTarget->IsAlive())
				{
					bValid = FALSE;
					nRet = 13040;
				}
				//  [2007-9-27] ���
				else if(!pTarget->GetIsAllowTeam())
				{
					bValid = FALSE;
					nRet = 13029;
				}
				
				if(!bValid)
				{
					st.unAction = TEAM_INVITE_ACTION_TYPE_INVITE_NANK;
					st.nRet = 13029;	//	ֱ��ʹ����Դ,�Է��Ѿ��ж�����
					MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, WORLD_TEAMINVITE, STRUCT_TYPE(ST_TEAM_INVITE), (const char*)&st);	
					return;
				}

// 				int nTeamInfo;
// 				pTeamInvite->teamInfo.nTeamMode;//ȡ1��2λ
// 				pTeamInvite->teamInfo.nExpMode << 2;//ȡ3��4λ
// 				pTeamInvite->teamInfo.nItemMode << 4;//ȡ5��6��7λ
				CMsgTeam msg;
				IF_NOT(msg.Create(pTeamInvite->idPlayer, pTeamInvite->szPlayerName, _MSGTEAM_INVITE))
					return;

				pTarget->SendMsg(&msg);

				pTarget->SetApplyName(CUser::APPLY_TEAMINVIT, pTeamInvite->szPlayerName);
			}
		}
		break;
		
	case TEAM_INVITE_ACTION_TYPE_INVITE_ANK:
		{
//			if(strlen(pTeamInvite->szPlayerName) < 1)
//				break;

			CUserPtr pUser = UserManager()->GetUser(pTeamInvite->idPlayer);
			if(!pUser)
				return;
			
			CTeam* pTeam	= pUser->GetTeam();
			if (!pTeam)
			{
				pUser->SendSysMsg(g_objStrRes.GetStr(13034));	//STR_NOT_CREATE_TEAM
				return;
			}
			if (pTeam->GetMemberAmount() >= _MAX_TEAMAMOUNT)
			{
				pUser->SendSysMsg(g_objStrRes.GetStr(13035));	//STR_HIS_TEAM_FULL
				return;
			}

			if (pTeam->GetLeader() != pUser->GetID())
			{
				pUser->SendSysMsg(g_objStrRes.GetStr(13015));	//STR_NO_CAPTAIN_CLOSE
				return;
			}


			ST_TEAM_INVITE st;
			memset(&st, 0, sizeof(ST_TEAM_INVITE));
			st.idPlayer = pTeamInvite->idTarget;
			st.idSocket = 0;
			strcpy(st.szPlayerName, pTeamInvite->szTargetName);
			st.idTarget = pUser->GetID();
			st.idSocketTarget = pUser->GetSocketID();
			strcpy(st.szTargetName, pUser->GetName());

			st.nRet = 0;
			
			TeamInfoStruct info;
			pTeam->GetInfo(&info);
			st.teamInfo.id = info.id;
			st.teamInfo.idLeader = info.idLeader;
			st.teamInfo.bCloseMoney = info.bCloseMoney;
			st.teamInfo.bCloseItem = info.bCloseItem;
			st.teamInfo.nTeamMode = info.nTeamMode;
			st.teamInfo.nExpMode = info.nExpMode;
			st.teamInfo.nItemMode = info.nItemMode;
			memcpy(st.teamInfo.setMember, info.setMember, sizeof(info.setMember));
			strcpy(st.teamInfo.sTeamName, info.sTeamName);
			st.teamInfo.m_nLev = info.m_nLev;
			st.teamInfo.m_nTarget = info.m_nTarget;
			strcpy(st.teamInfo.m_sLeadName, info.m_sLeadName);
			strcpy(st.teamInfo.m_sTeamText, info.m_sTeamText);
			
			st.unAction = TEAM_INVITE_ACTION_TYPE_JOIN_ANK;
			MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, WORLD_TEAMINVITE, STRUCT_TYPE(ST_TEAM_INVITE), (const char*)&st);	
			return;
		}
		break;

	case TEAM_INVITE_ACTION_TYPE_INVITE_ANK_RETURN:
		{
			if(strlen(pTeamInvite->szTargetName) < 1)
				break;

			CUserPtr	pTarget = UserManager()->GetUser(pTeamInvite->szTargetName);
			if(!pTarget)
				return;

			if(pTarget->GetTeam())
				return;
			
			if(pTeamInvite->nMemberInfoCount < 1)
				return;

			if(pTeamInvite->teamInfo.id == ID_NONE)
				return;
			TeamInfoStruct info;
			info.id				= pTeamInvite->teamInfo.id;
			info.idLeader		= pTeamInvite->teamInfo.idLeader;
			info.bCloseMoney	= pTeamInvite->teamInfo.bCloseMoney;
			info.bCloseItem		= pTeamInvite->teamInfo.bCloseItem;
			info.nTeamMode		= pTeamInvite->teamInfo.nTeamMode;
			info.nExpMode		= pTeamInvite->teamInfo.nExpMode;
			info.nItemMode		= pTeamInvite->teamInfo.nItemMode;
			memcpy(info.setMember, pTeamInvite->teamInfo.setMember, sizeof(pTeamInvite->teamInfo.setMember));
			strcpy(info.sTeamName, pTeamInvite->teamInfo.sTeamName);

			info.m_nLev         = pTeamInvite->teamInfo.m_nLev;
			info.m_nTarget      = pTeamInvite->teamInfo.m_nTarget;
			strcpy(info.m_sLeadName, pTeamInvite->teamInfo.m_sLeadName);
			strcpy(info.m_sTeamText, pTeamInvite->teamInfo.m_sTeamText);
			
			CTeam* pTeam = CTeam::CreateNew(PID, &info);
			CHECK(pTeam);
			pTarget->SetTeam(pTeam);

			for(int i = 0; i < pTeamInvite->nMemberInfoCount; i++)
			{
				CMsgTeamMember msg;
				TeamMemberInfo teamMemberInfo;
				teamMemberInfo.id			= pTeamInvite->teamMemberInfo[i].id;
				teamMemberInfo.dwLookFace	= pTeamInvite->teamMemberInfo[i].dwLookFace;
				teamMemberInfo.dwLev        = pTeamInvite->teamMemberInfo[i].dwLev;
// 				teamMemberInfo.usHp			= pTeamInvite->teamMemberInfo[i].usHp;
// 				teamMemberInfo.usMaxHp		= pTeamInvite->teamMemberInfo[i].usMaxHp;
// 				teamMemberInfo.usMp			= pTeamInvite->teamMemberInfo[i].usMp;
// 				teamMemberInfo.usMaxMp		= pTeamInvite->teamMemberInfo[i].usMaxMp;
// 				teamMemberInfo.usPosX		= pTeamInvite->teamMemberInfo[i].usPosX;
// 				teamMemberInfo.usPosY		= pTeamInvite->teamMemberInfo[i].usPosY;
// 				teamMemberInfo.usMapID      = pTeamInvite->teamMemberInfo[i].usMapID;
				teamMemberInfo.usProfession = pTeamInvite->teamMemberInfo[i].usProfession;

				strcpy(teamMemberInfo.szName, pTeamInvite->teamMemberInfo[i].szName);
			    IF_NOT(msg.Create(_MSG_TEAMMEMBER_ADDMEMBER, teamMemberInfo))
					continue;
				
				pTarget->SendMsg(&msg);
			}

// 			CMsgTeam msg;
// 			if(msg.Create(pTeamInvite->teamMemberInfo[0].id, _MSGTEAM_CHG_LEADER_APPLY, pTeamInvite->teamMemberInfo[i].usMapID))
// 			{
// 				//��������
// 				pTarget->SendMsg(&msg);
// 			}
		}
		break;

	case TEAM_INVITE_ACTION_TYPE_INVITE_NANK:
		{
			if(strlen(pTeamInvite->szPlayerName) < 1)
				break;

			CUserPtr pUser = UserManager()->GetUser(pTeamInvite->szPlayerName);
			if(!pUser)
				return;

			if(pUser->FetchApplyName(CUser::APPLY_TEAMINVIT, pTeamInvite->szTargetName))
				pUser->SetApplyName(CUser::APPLY_TEAMINVIT, NULL);

			pUser->SendSysMsg(g_objStrRes.GetStr(pTeamInvite->nRet));
		}
		break;

	case TEAM_INVITE_ACTION_TYPE_JOIN_REQ:
		{
			if(strlen(pTeamInvite->szTargetName) < 1)
				break;

			CUserPtr pTarget = UserManager()->GetUser(pTeamInvite->szTargetName);
			if(!pTarget)
				return;
			
			ST_TEAM_INVITE st;
			memcpy(&st, pTeamInvite, sizeof(ST_TEAM_INVITE));

			int nRet;
			BOOL bIsValid = TRUE;
			
			CTeam* pTeam = pTarget->GetTeam();
			if(pTeam)
			{		
				
				if(pTeam->GetMemberAmount() > _MAX_TEAMAMOUNT)
				{
					nRet = 13029;	
					bIsValid = FALSE;
				}
				else if (pTeam->GetMemberAmount() >= _MAX_TEAMAMOUNT)
				{
					nRet = 13035;	
					bIsValid = FALSE;
				}
				
				else if (pTeam->GetLeader() != pTarget->GetID())
				{
					nRet = 13015;	
					bIsValid = FALSE;
				}
			}
			
			//  [2007-9-27] ���
			if(!pTarget->GetIsAllowTeam())
			{
				bIsValid = FALSE;
				nRet = 13029;
			}
			
			
			if(!bIsValid)
			{
				st.nRet = nRet;
				st.unAction = TEAM_INVITE_ACTION_TYPE_JOIN_NANK;
				MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, WORLD_TEAMINVITE, STRUCT_TYPE(ST_TEAM_INVITE), (const char*)&st);	
				return;
			}
			
			pTarget->SetApplyName(CUser::APPLY_TEAMINVIT, pTeamInvite->szPlayerName);
	
			// By Minsky
// 			int nTeamInfo;
// 			nTeamInfo |= pTeamInvite->teamInfo.nTeamMode;//ȡ1��2λ
// 			nTeamInfo |= (pTeamInvite->teamInfo.nExpMode << 2);//ȡ3��4λ
// 			nTeamInfo |= (pTeamInvite->teamInfo.nItemMode << 4);//ȡ5��6��7λ
			CMsgTeam msg;
			IF_NOT(msg.Create(pTeamInvite->idPlayer, pTeamInvite->szPlayerName, _MSGTEAM_APPLYJOIN))
				return;
			
			pTarget->SendMsg(&msg);
		}
		break;
		
	case TEAM_INVITE_ACTION_TYPE_JOIN_ANK:
		{
//			if(strlen(pTeamInvite->szPlayerName) < 1)
//				break;

			CUserPtr pUser = UserManager()->GetUser(pTeamInvite->idPlayer);
			if(!pUser)
				return;

			//	�Ѿ��ж���
			if(pUser->GetTeam())
			{	
				return;
			}

 			pUser->AppendTeam(&(pTeamInvite->teamInfo));

			CTeam *pTeam = pUser->GetTeam();
			if(pTeam)
			{
				pTeam->AddMember(pTeamInvite->idPlayer, false);

				//�ӳ���������
                CMsgTeam msgLeader;
				if(msgLeader.Create(pTeam->GetLeader(), _MSGTEAM_CHG_LEADER_APPLY))
				{
					pUser->SendMsg(&msgLeader);
				}

	 			pUser->SendShow(pUser);

				//  ���Լ��㲥�������Ա
				// By Minsky
// 				UINT teamInfo = 0;
// 				teamInfo |= pTeam->GetTeamMode();//ȡ1��2λ
// 				teamInfo |= (pTeam->GetExpMode()<<2);//ȡ3��4λ
// 				teamInfo |= (pTeam->GetItemMode()<<4);//ȡ5��6��7λ
				CMsgTeam msgRoom;
				if(msgRoom.Create(pUser->GetID(), (char*)pUser->GetName(), _MSGTEAM_JOINTEAM))
				{
					pUser->BroadcastRoomMsg(&msgRoom,true);
				}
			}

			return;
		}
		break;

	case TEAM_INVITE_ACTION_TYPE_JOIN_NANK:
		{
			if(strlen(pTeamInvite->szPlayerName) < 1)
				break;

			CUserPtr pUser = UserManager()->GetUser(pTeamInvite->szPlayerName);
			if(!pUser)
				return;

			if(pUser->FetchApplyName(CUser::APPLY_TEAMINVIT, pTeamInvite->szTargetName))
				pUser->SetApplyName(CUser::APPLY_TEAMINVIT, NULL);

			pUser->SendSysMsg(g_objStrRes.GetStr(pTeamInvite->nRet));
		}
		break;

	case TEAM_INVITE_ACTION_TYPE_JOIN_ANK_RETURN:
		{
			if(strlen(pTeamInvite->szTargetName) < 1)
				break;

			CUserPtr	pTarget = UserManager()->GetUser(pTeamInvite->szTargetName);
			if(!pTarget)
				return;

			CTeam* pTeam = pTarget->GetTeam();
			if(pTeam)	//	�ж�
			{
				if(pTeam->GetMemberAmount() >= _MAX_TEAMAMOUNT)
					return;

				if(pTeam->GetLeader() != pTarget->GetID())
					return;
			}
			else		//	�޶�
			{
				//	����һ��
				if (pTarget->CreateTeam())
				{
					CMsgTeam msg;
					if(msg.Create(pTarget->GetID(), _MSGTEAM_CREATE, pTarget->GetMapID()))
						pTarget->SendMsg(&msg);
					
					//int teamInfo = 20;//Ĭ��С������
					char teamName[64];//Ĭ��С����
					sprintf(teamName,g_objStrRes.GetStr(17011),pTarget->GetName());//"%s��СС��"

					pTeam = pTarget->GetTeam();
					pTeam->SetTeamMode(0);//ȡ1��2λ
					pTeam->SetExpMode(1);//ȡ3��4λ
					pTeam->SetItemMode(1);//ȡ5��6��7λ
					pTeam->SetTeamName(teamName);
// 					if(teamInfo & 0x2)
// 						pTeam->Close();
				
					if(msg.Create(pTarget->GetID(), (char*)pTarget->GetName(), _MSGTEAM_JOINTEAM))
					{
						pTarget->BroadcastRoomMsg(&msg, true);
					}
					//�ӳ�����Ҫ����.---------------------
					if(msg.Create(pTeam->GetLeader(), _MSGTEAM_CHG_LEADER_APPLY, 1001))
					{
						//�����Լ�
						pTeam->BroadcastTeamMsg(&msg);
					}
					//-------------------------------------------------
				}
				pTeam	= pTarget->GetTeam();
			}
			
			pTarget->AddRemoteTeamMember(pTeamInvite, TRUE);
			
			pTarget->UpdateBroadcastSet();
		
			ST_TEAM_INVITE st;
			memcpy(&st, pTeamInvite, sizeof(ST_TEAM_INVITE));
			st.nRet = 0;
			st.unAction = TEAM_INVITE_ACTION_TYPE_JOIN_FINAL;
			MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, WORLD_TEAMINVITE, STRUCT_TYPE(ST_TEAM_INVITE), (const char*)&st);	
			return;
			
		}
		break;

	case TEAM_INVITE_ACTION_TYPE_JOIN_FINAL:
		{
			if(strlen(pTeamInvite->szPlayerName) < 1)
				break;

			CUserPtr	pTarget = UserManager()->GetUser(pTeamInvite->szPlayerName);
			if(!pTarget)
				return;

			if(pTarget->GetTeam())
				return;
			
			if(pTeamInvite->nMemberInfoCount < 1)
				return;

			if(pTeamInvite->teamInfo.id == ID_NONE)
				return;
			TeamInfoStruct info;
			info.id				= pTeamInvite->teamInfo.id;
			info.idLeader		= pTeamInvite->teamInfo.idLeader;
			info.bCloseMoney	= pTeamInvite->teamInfo.bCloseMoney;
			info.bCloseItem		= pTeamInvite->teamInfo.bCloseItem;
			info.nTeamMode		= pTeamInvite->teamInfo.nTeamMode;
			info.nExpMode		= pTeamInvite->teamInfo.nExpMode;
			info.nItemMode		= pTeamInvite->teamInfo.nItemMode;
			memcpy(info.setMember, pTeamInvite->teamInfo.setMember, sizeof(pTeamInvite->teamInfo.setMember));
			strcpy(info.sTeamName, pTeamInvite->teamInfo.sTeamName);
			
			CTeam* pTeam = CTeam::CreateNew(PID, &info);
			CHECK(pTeam);
			pTarget->SetTeam(pTeam);
			

			for(int i = 0; i < pTeamInvite->nMemberInfoCount; i++)
			{
				CMsgTeamMember msg;
				TeamMemberInfo teamMemberInfo;
				teamMemberInfo.id			= pTeamInvite->teamMemberInfo[i].id;
				teamMemberInfo.dwLookFace	= pTeamInvite->teamMemberInfo[i].dwLookFace;
				teamMemberInfo.dwLev        = pTeamInvite->teamMemberInfo[i].dwLev;
// 				teamMemberInfo.usHp			= pTeamInvite->teamMemberInfo[i].usHp;
// 				teamMemberInfo.usMaxHp		= pTeamInvite->teamMemberInfo[i].usMaxHp;
// 				teamMemberInfo.usMp			= pTeamInvite->teamMemberInfo[i].usMp;
// 				teamMemberInfo.usMaxMp		= pTeamInvite->teamMemberInfo[i].usMaxMp;
// 				teamMemberInfo.usPosX		= pTeamInvite->teamMemberInfo[i].usPosX;
// 				teamMemberInfo.usPosY		= pTeamInvite->teamMemberInfo[i].usPosY;
// 				teamMemberInfo.usMapID      = pTeamInvite->teamMemberInfo[i].usMapID;
				teamMemberInfo.usProfession = pTeamInvite->teamMemberInfo[i].usProfession;
				strcpy(teamMemberInfo.szName, pTeamInvite->teamMemberInfo[i].szName);
			    IF_NOT(msg.Create(_MSG_TEAMMEMBER_ADDMEMBER, teamMemberInfo))
					continue;
				
				pTarget->SendMsg(&msg);
			}

// 			CMsgTeam msg;
// 			if(msg.Create(pTeamInvite->teamMemberInfo[0].id, _MSGTEAM_CHG_LEADER_APPLY, pTeamInvite->teamMemberInfo[i].usMapID))
// 			{
// 				//��������
// 				pTarget->SendMsg(&msg);
// 			}

	// 		pTarget->UpdateBroadcastSet();
		}
		break;

	case TEAM_INVITE_ACTION_TYPE_WAIT:
		{
			if(strlen(pTeamInvite->szTargetName) < 1)
				break;

			CUserPtr pTarget = UserManager()->GetUser(pTeamInvite->szTargetName);
			if(!pTarget)
				return;
			
			ST_TEAM_INVITE st;
			memcpy(&st, pTeamInvite, sizeof(ST_TEAM_INVITE));

			CTeam* pTeam = pTarget->GetTeam();
			if(pTeam)
			{	
				if(pTeamInvite->teamInfo.idLeader != 0) //�������ж�
				{
					pTarget->DenyTeamInvite(pTeamInvite->idPlayer, "�Է��Ѿ��ж�����");
				}
				else
				{
					CMsgTeam msg;
					if(msg.CreateMemInfo(_MSGTEAM_INVITE_WAIT, pTeamInvite->idPlayer, pTeamInvite->szPlayerName, pTeamInvite->teamInfo.m_nLev))
					{
						pTarget->SendMsg(&msg);
					}

					pTarget->SendSysMsg("��������������Ķ��� ");
				}

			}
			else	//	������û�ж���
			{
				if(pTeamInvite->teamInfo.idLeader != 0) //�������ж�
				{
					CMsgTeam msg;
					if(msg.Create(_MSGTEAM_INVITE_WAIT,
						          pTeamInvite->teamInfo.m_nTarget,
								  pTeamInvite->teamInfo.m_nLev,
								  pTeamInvite->teamInfo.idLeader,
								  pTeamInvite->teamInfo.m_sLeadName,
								  pTeamInvite->teamInfo.nLeaderlev,
								  pTeamInvite->nMemberInfoCount))
					{
						pTarget->SendMsg(&msg);
					}

					pTarget->SendSysMsg("�ж����������ļ��� ");
				}
				else
				{
					pTarget->DenyTeamInvite(pTeamInvite->idPlayer, "���ͶԷ���û�ж��飬�����������");
				}
			}
		}
		break;

	default:
		break;
	}
}

void CMapGroupKernel::ProcessFriendApply(ST_FRIEND_APPLY *pInfo)
{
	switch(pInfo->unAction)
	{
	case FRIEND_APPLY_ACTION_TYPE_APPLY_REQ:
		{
			CUserPtr pTarget = UserManager()->GetUser(pInfo->szTargetName);
			if(!pTarget)
				return;

			BOOL bIsValid = TRUE;
			int nRet = 0;
			if(pTarget->GetFriendAmount() >= _MAX_USERFRIENDSIZE)
			{
				nRet = 12073;	//STR_FRIEND_LIST_FULL
				bIsValid = FALSE;
			}
			else if(pTarget->GetFriendByName(pInfo->szPlayerName))
			{
				nRet = 12074;//STR_YOUR_FRIEND_ALREADY
				bIsValid = FALSE;
			}
			else if(!pTarget->GetIsAllowFriend())
			{
				nRet = 12075;//STR_NOTALLOWFRIEND
				bIsValid = FALSE;
			}
			
			if(!bIsValid)
			{
				ST_FRIEND_APPLY st;
				memcpy(&st, pInfo, sizeof(ST_FRIEND_APPLY));
				st.nRet = nRet;
				st.unAction = FRIEND_APPLY_ACTION_TYPE_APPLY_NANK;
				MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, WORLD_FRIENDAPPLY, STRUCT_TYPE(ST_FRIEND_APPLY), (const char*)&st);
				return;
			}

			if(!pTarget->FetchApplyName(CUser::APPLY_FRIEND, pInfo->szPlayerName))
			{
				// ����Է�δ�����Լ������������������
				
				pTarget->SetApplyName(CUser::APPLY_FRIEND, pInfo->szPlayerName);
				CMsgFriend msg;
				IF_OK(msg.Create(_FRIEND_APPLY, pInfo->idPlayer, pInfo->szPlayerName, true))
					pTarget->SendMsg(&msg);

				pTarget->SendSysMsg(g_objStrRes.GetStr(13056), pInfo->szPlayerName);//STR_TO_MAKE_FRIEND
				return;
			}	

			/*
			else
			{
				// ����Է��Ѿ����Լ�����������ֱ��Ϊ˫���Ӻ���			
				
				ST_FRIEND_APPLY st;
				memcpy(&st, pInfo, sizeof(ST_FRIEND_APPLY));
				st.unAction = FRIEND_APPLY_ACTION_TYPE_APPLY_ANK;
				st.nTargetLev = pTarget->GetLev();
				st.nTargetLookFace = pTarget->GetLookFace();
				st.nTargetProfession = pTarget->GetProfession();

				if (pTarget->AddFriend(pInfo->idPlayer, pInfo->szPlayerName))
				{
					//20061020����bug�¼Ӻ���ʱְҵ��ʾerro.
					//create�¼�����������------------------------------------
					CMsgFriend msg;
					if(msg.Create(_FRIEND_GETINFO, pInfo->idPlayer, pInfo->szPlayerName, 
						1, pInfo->nLookFace, pInfo->nLev, pInfo->nProfession) )
						pTarget->SendMsg(&msg);

					MSGBUF	szMsg;
					sprintf(szMsg, STR_MAKE_FRIEND, pInfo->szPlayerName, pInfo->szTargetName);
					pTarget->BroadcastRoomMsg(szMsg, true);

					MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, WORLD_FRIENDAPPLY, STRUCT_TYPE(ST_FRIEND_APPLY), (const char*)&st);
				}
			}
			*/
		}
		break;
	case FRIEND_APPLY_ACTION_TYPE_APPLY_ANK:
		{
			CUserPtr pUser = UserManager()->GetUser(pInfo->szPlayerName);
			if(!pUser)
				return;
			
			BOOL bIsValid = TRUE;
			int nRet = 0;
			if(pUser->GetFriendAmount() >= _MAX_USERFRIENDSIZE)
			{
				nRet = 12073;	//STR_FRIEND_LIST_FULL
				bIsValid = FALSE;
			}
			else if(pUser->GetFriendByName(pInfo->szTargetName))
			{
				nRet = 12074;//STR_YOUR_FRIEND_ALREADY
				bIsValid = FALSE;
			}
			else if(!pUser->GetIsAllowFriend())
			{
				nRet = 12075;//STR_NOTALLOWFRIEND
				bIsValid = FALSE;
			}
			
			if(!bIsValid)
			{
				pUser->SendSysMsg(g_objStrRes.GetStr(nRet));
				return;
			}
			

			if (pUser->AddFriend(pInfo->idTarget, pInfo->szTargetName))
			{
				//20061020����bug�¼Ӻ���ʱְҵ��ʾerro.
				//create�¼�����������------------------------------------
				CMsgFriend msg;
				IF_OK(msg.Create(_FRIEND_GETINFO, pInfo->idTarget, pInfo->szTargetName, 
					1, pInfo->nTargetLookFace, pInfo->nTargetLev, pInfo->nTargetProfession, 0, pInfo->nSex,pInfo->nFriendType))
					pUser->SendMsg(&msg);
				
				MSGBUF	szMsg;
				sprintf(szMsg, STR_MAKE_FRIEND, pInfo->szPlayerName, pInfo->szTargetName);
				pUser->BroadcastRoomMsg(szMsg, true);

				pInfo->nLookFace = pUser->GetLookFace();	//	д������Ϣ
				pInfo->nProfession = pUser->GetProfession();
				pInfo->nLev = pUser->GetLev();
				pInfo->nSex = pUser->GetSex();
				pInfo->unAction = FRIEND_APPLY_ACTION_TYPE_APPLY_FIN;
				MapGroup(PID)->QueryMsgPort()->Send(MSGPORT_WORLD, WORLD_FRIENDAPPLY, STRUCT_TYPE(ST_FRIEND_APPLY), (const char*)pInfo);
			}
		}
		break;
	case FRIEND_APPLY_ACTION_TYPE_APPLY_FIN:
		{
			CUserPtr pTarget = UserManager()->GetUser(pInfo->szTargetName);
			if(!pTarget)
				return;

			if (pTarget->AddFriend(pInfo->idPlayer, pInfo->szPlayerName))
			{
				//20061020����bug�¼Ӻ���ʱְҵ��ʾerro.
				//create�¼�����������------------------------------------
				CMsgFriend msg;
				IF_OK(msg.Create(_FRIEND_GETINFO, pInfo->idPlayer, pInfo->szPlayerName, 
					1, pInfo->nLookFace, pInfo->nLev, pInfo->nProfession, 0, pInfo->nSex,pInfo->nFriendType))
					pTarget->SendMsg(&msg);
				
				MSGBUF	szMsg;
				sprintf(szMsg, STR_MAKE_FRIEND, pInfo->szPlayerName, pInfo->szTargetName);
				pTarget->BroadcastRoomMsg(szMsg, true);
			}
		}
		break;
	case FRIEND_APPLY_ACTION_TYPE_APPLY_NANK:
		{
			CUserPtr pUser = UserManager()->GetUser(pInfo->szPlayerName);
			if(!pUser)
				return;

			pUser->SendSysMsg(g_objStrRes.GetStr(pInfo->nRet));
		}
		break;
	}
}

bool CMapGroupKernel::ReconnectDB()
{
if(!g_bEnableDatabaseReconnect)
		return true;

	SYSTEMTIME curTime;
	::GetLocalTime( &curTime );
	if( curTime.wDay != m_stLastDatabaseReconnectTime.wDay 
		&& curTime.wHour == 4)	
	{
		// ��ʼ�����ݿ�
		char	DB_IP	[DBSTR_SIZE] = "";
		char	DB_USER	[DBSTR_SIZE] = "";
		char	DB_PW	[DBSTR_SIZE] = "";
		char	DB_DB	[DBSTR_SIZE] = "";
		
		CIniFile	ini(CONFIG_FILENAME, "Database");
		ini.GetString(DB_IP,	"DB_IP",	DBSTR_SIZE);
		ini.GetString(DB_USER,	"DB_USER",	DBSTR_SIZE);
		ini.GetString(DB_PW,	"DB_PW",	DBSTR_SIZE);
		ini.GetString(DB_DB,	"DB_DB",	DBSTR_SIZE);
	//	m_pDb	= ::CreateDatabase(DB_IP, DB_USER, DB_PW, DB_DB);
		BOOL bSuccess	= ::ReconnectDatabase(m_pDb, DB_IP, DB_USER, DB_PW, DB_DB);
		
		
		if(!bSuccess)
		{
			LOGERROR("���ݿ���������ʧ��!, m_idProcess = %d", m_idProcess);
			return false;
		}
		
		LOGMSG("���ݿ����ӳɹ�, m_idProcess = %d", m_idProcess);

		::GetLocalTime(&m_stLastDatabaseReconnectTime);
	}

	return true;
}
