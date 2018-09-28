// MsgVAS.cpp: implementation of the CMsgVAS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsgVAS.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgVAS::CMsgVAS()
{
	Init();///��ʼ����
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgVAS::~CMsgVAS()
{

}

///����һ����ֵ�����
// BOOL CMsgVAS::Create(const MsgTraffic& mtExtraInfo,int usVASPoint,OBJID AccountID,OBJID PlayerID)///������ֵ����������Ϣ
// {
// 	// init
// 	this->Init();
// 
// 	// fill info now
// 	m_unMsgSize	=sizeof( MSG_Info );
// 	m_unMsgType	=_MSG_VAS;
// 
// 	m_pInfo->mtExtraInfo=mtExtraInfo;///���빺����Ϣ��
// 	m_pInfo->usVASPoint=usVASPoint;///��ֵ����
// 	m_pInfo->AccountID=AccountID;///�ʺ�ID
// 	m_pInfo->PlayerID=PlayerID;///��ɫID
// 
// 	return true;
// }
BOOL CMsgVAS::Create( int usVASPoint, OBJID AccountID, OBJID PlayerID, USHORT usAction, OBJID itemID, int itemAmount, DWORD dwData/*=0*/)
{
	this->Init();
	
	m_pInfo->usVASPoint = usVASPoint;
	m_pInfo->AccountID = AccountID;
	m_pInfo->PlayerID = PlayerID;
	m_pInfo->usAction = usAction;
	
	if(itemID == ID_NONE)
		m_pInfo->usGoodSize			= 0;
	else
	{
		m_pInfo->usGoodSize			= 1;
		m_pInfo->nGoods[0].itemID=itemID;
		m_pInfo->nGoods[0].itemAmount=itemAmount;
		m_pInfo->nGoods[0].dwData=dwData;
	}
	
	m_unMsgType	=_MSG_VAS;
	m_unMsgSize	=sizeof(MSG_Info);
	
	return TRUE;
}
///�������
BOOL CMsgVAS::Create(char* pMsgBuf,DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_VAS != this->GetType())
		return false;

	return true;
}