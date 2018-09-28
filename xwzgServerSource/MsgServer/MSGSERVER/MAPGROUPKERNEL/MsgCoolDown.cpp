
//FileName:	MsgCoolDown.cpp
//Time:		2006.9.4
//Author:	����
//About:	���ڼ�����ȴʱ��ӵ���Ϣ��

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
//����:��������CDʱ���֪ͨ,�����ͻ���
//����:idΪ���ID,usMagicType���ܱ��,usAction�Ǳ�ʶ�ǿ�ʼ���߽���,dwPrivateCDTimeĳ���ܵ�CDʱ��.dwPublicCDTime����CDʱ��
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
