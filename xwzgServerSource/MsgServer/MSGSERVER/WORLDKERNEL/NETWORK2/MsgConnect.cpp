// MsgConnect.cpp: implementation of the CMsgConnect class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "UserList.h"
#include "logfile.h"
using namespace world_kernel;
#include "WorldKernel.h"
#include "MsgFee_A.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgConnect::CMsgConnect()
{
	Init();
	m_pInfo	=(MSG_Info *)m_bufMsg;

}

CMsgConnect::~CMsgConnect()
{

}

//////////////////////////////////////////////////////////////////////
//06.5.5王玉波修改
BOOL CMsgConnect::Create(OBJID idAccount, DWORD dwData, char* pszInfo)
{																							//halei  外装           武器
	// param check
	if (!pszInfo || strlen(pszInfo) >= _MAX_NAMESIZE)
		return false;
	
	// init
	this->Init();

	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_CONNECT;

	m_pInfo->idAccount		=idAccount;
	m_pInfo->dwData			=dwData;
	strcpy(m_pInfo->szInfo, pszInfo);
	m_pInfo->nRoleCount = 0;
	return true;
}

bool  CMsgConnect::Add(DWORD id,USHORT level,USHORT photoIndex,char* pszInfo,USHORT firJob,USHORT firLev,USHORT secJob,USHORT secLev,DWORD firCoat,DWORD firWapon,DWORD firMount,DWORD secCoat,DWORD secWapon,DWORD secMount)
{
	if(m_pInfo->nRoleCount > 8)
		return false;

	m_pInfo->snapInfo[m_pInfo->nRoleCount].userid = id;
	m_pInfo->snapInfo[m_pInfo->nRoleCount].Lev = level;
	m_pInfo->snapInfo[m_pInfo->nRoleCount].PhotoIndex = photoIndex;
	m_pInfo->snapInfo[m_pInfo->nRoleCount].firJob = firJob;
	m_pInfo->snapInfo[m_pInfo->nRoleCount].firLev = firLev;
	m_pInfo->snapInfo[m_pInfo->nRoleCount].secJob = secJob;
	m_pInfo->snapInfo[m_pInfo->nRoleCount].secLev = secLev;
	m_pInfo->snapInfo[m_pInfo->nRoleCount].firCoat = firCoat;
	m_pInfo->snapInfo[m_pInfo->nRoleCount].firWapon = firWapon;
	m_pInfo->snapInfo[m_pInfo->nRoleCount].firMount = firMount;
	m_pInfo->snapInfo[m_pInfo->nRoleCount].secCoat = secCoat;
	m_pInfo->snapInfo[m_pInfo->nRoleCount].secWapon = secWapon;
	m_pInfo->snapInfo[m_pInfo->nRoleCount].secMount = secMount;
	strcpy(m_pInfo->snapInfo[m_pInfo->nRoleCount].szName,pszInfo);
	m_pInfo->nRoleCount ++;

	m_unMsgSize = sizeof(MSG_Info) + (m_pInfo->nRoleCount - 1) * sizeof(UserSnapInfo) ;

	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgConnect::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_CONNECT != this->GetType())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgConnect::Process(void* pSocket)
{
#ifdef _MYDEBUG_X
	::LogSave("Process CMsgConnect, idAccount:%u, Data:%d, Info:%s",
				m_pInfo->idAccount,
				m_pInfo->dwData,
				m_pInfo->szInfo);
#endif

	if (!pSocket || m_pInfo->idAccount == ID_NONE)
		return;

	if(m_pInfo->dwData!=0)		//李京哲0308 客户端第一次连接
	{
		double Version = atof( m_pInfo->szInfo );
//		if (1 != sscanf(m_pInfo->szInfo, "%.8f", &Version))
//		{
//			CMsgTalk msg;
//			if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, STR_ERROR_VERSION, NULL, _COLOR_WHITE, _TXTATR_ENTRANCE))
//				SendMsg(&msg);
//			return;
//		}
// 		if ( Version < _VERSION )
// 		{
// 			CMsgTalk msg;
// 			if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, STR_ERROR_LOWER_VERSION, NULL, _COLOR_WHITE, _TXTATR_ENTRANCE))
// 				SendMsg(&msg);
// 			return;
// 		}

		
		extern DWORD g_dwMaxPlayer;
		if(GameWorld()->QueryUserList()->GetUserAmount() > g_dwMaxPlayer && m_pInfo->idAccount > MAX_GM_ID)
		{
			CMsgTalk msg;
			if(msg.Create(SYSTEM_NAME, ALLUSERS_NAME, STR_ERROR_SERVER_FULL, NULL, _COLOR_WHITE, _TXTATR_ENTRANCE))
				SendMsg(&msg);
			return;
		}
		
		// 接收完了MSG_CONNECT后，用认证号重置密码。
		//20071016修罗:更换密钥----------------
		//DWORD dwEncyptCode = (m_pInfo->idAccount+m_pInfo->dwData)^0x4321;
		//DWORD dwEncyptCode = (m_pInfo->idAccount+m_pInfo->dwData)^0x1116;
		//-------------------------------------
		//GameWorld()->ChangeCode(GetSocketID(), m_pInfo->dwData^dwEncyptCode);

		GameWorld()->ClientConnect(m_pInfo->idAccount, m_pInfo->dwData, m_pInfo->szInfo, this->GetSocketID(),CLIENTOK,0,this->GetULongIP());
		GameWorld()->SendFee(m_pInfo->idAccount,CMsgFee_A::FEE_TICK);///告诉帐号服务器,这个客户端已经使用这张票登录过游戏服务器了
	}
	else
		GameWorld()->ClientConnect(m_pInfo->idAccount,0, m_pInfo->szInfo, this->GetSocketID(),JOIN,0,this->GetULongIP());


	//{
	//	CPlayer* pPlayer	= g_UserList.GetPlayerByAccountID(m_pInfo->idAccount);
	//	ASSERT(pPlayer);
	//	if(pPlayer)
	//		UserList()->LoginToMapGroup(pPlayer);
	//}
}
