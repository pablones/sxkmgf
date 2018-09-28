#include "MsgTopList.h"
#include "RoleManager.h"
#include "MapGroup.h"
//FileName	:MsgTopList.cpp
//Time		:06.9.9
//Author	:王玉波
//说明		:玩家查看排行榜的消息类

///////////////////////////////////////////////////////////////////////////////////////////////
CMsgTopList::CMsgTopList()
{
	Init();
	m_pInfo	=( MSG_Info* )m_bufMsg;
}
//////////////////////////////////////////////////////////////////////////////////////////////
CMsgTopList::~CMsgTopList()
{

}
//////////////////////////////////////////////////////////////////////////////////////////////
//为接收消息的Crate函数
BOOL CMsgTopList::Create( char* pbufMsg, DWORD dwMsgSize )
{
	if ( !CNetMsg::Create( pbufMsg, dwMsgSize ) )
		return false;

	if( _MSG_TOPLIST != this->GetType() )
		return false;

	return true;
}

BOOL CMsgTopList::Create ( USHORT usType,toplistMemData* pData,int nSize,USHORT usPage,USHORT myPos)
{
	this->Init();
	m_pInfo->usAction = usType;
	m_pInfo->usPage = usPage;
	m_pInfo->usAmount = nSize;
	m_pInfo->usData = myPos;

	if(nSize > 0)
	{
		m_unMsgSize	= sizeof( MSG_Info ) + sizeof( toplistMemData ) * ( nSize - 1 );
		if(m_unMsgSize > MAX_PACKETSIZE)
			return false;

		memcpy( m_pInfo->topData,pData,sizeof(toplistMemData) * nSize );
	}
	else
		m_unMsgSize = sizeof( MSG_Info ) - sizeof( toplistMemData );

	m_unMsgType	= _MSG_TOPLIST;
	return true;
}

void CMsgTopList::Process(void *pInfo)
{
	IRole* pRole = RoleManager()->QueryRole( this );
	if ( pRole )
	{
		CUser* pUser = NULL;
		pRole->QueryObj( OBJ_USER,( void** )&pUser );
		if( !pUser )
			return;
		if(m_pInfo->usAction >= MSGTOPLIST_QUERYTOTAL)
		pUser->QueryTopList( m_pInfo->usAction - MSGTOPLIST_QUERYTOTAL, m_pInfo->usPage );	
	}
}