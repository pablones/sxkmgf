
//FileName:	MsgCoolDown.cpp
//Time:		2006.9.4
//Author:	王玉波
//About:	关于技能冷却时间加的消息类

#include "MsgCoolDown.h"

/////////////////////////////////////////////////////////////////////////////
CMsgCoolDown::CMsgCoolDown()
{
	Init();
	m_pInfo	=( MSG_Info * )m_bufMsg;
}
/////////////////////////////////////////////////////////////////////////////
CMsgCoolDown::~CMsgCoolDown()
{


}
/////////////////////////////////////////////////////////////////////////////
//功能:创建技能CD时间的通知,发往客户端
//参数:id为玩家ID,usMagicType技能编号,usAction是标识是开始或者结束,dwPrivateCDTime某技能的CD时间.dwPublicCDTime公共CD时间
BOOL CMsgCoolDown::Create( OBJID id,USHORT usAction,USHORT usInfoType,OBJID idObjType,DWORD dwPrivateCDTime,DWORD dwPublicCDTime )
{ 
	// init
	this->Init();
	// fill info now
	m_unMsgSize	=sizeof( MSG_Info );
	m_unMsgType	=_MSG_COOLDOWN;
	
	m_pInfo->idUser				= id;
	m_pInfo->usAction			= usAction;
	m_pInfo->usInfoType			= usInfoType;
	m_pInfo->idObjType			= idObjType;
	m_pInfo->dwPrivateCDTime	= dwPrivateCDTime;
	m_pInfo->dwPublicCDTime		= dwPublicCDTime;

	return true;
}
////////////////////////////////////////////////////////////////////////////////
void CMsgCoolDown::Process( void* pInfo )
{
#ifdef _MSGDEBUG
	::LogSave( "Process MsgCoolDown,idUser:%d, 
				usMagicType:%d,
				usAction:%d", 
				m_pInfo->idUser,
				m_pInfo->usMagicType,
				m_pInfo->usAction );
#endif
}
