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
	Init();///初始化包
	m_pInfo	=(MSG_Info *)m_bufMsg;
}

CMsgVAS::~CMsgVAS()
{

}

///生成一个增值服务包
// BOOL CMsgVAS::Create(const MsgTraffic& mtExtraInfo,int usVASPoint,OBJID AccountID,OBJID PlayerID)///生成增值服务请求消息
// {
// 	// init
// 	this->Init();
// 
// 	// fill info now
// 	m_unMsgSize	=sizeof( MSG_Info );
// 	m_unMsgType	=_MSG_VAS;
// 
// 	m_pInfo->mtExtraInfo=mtExtraInfo;///放入购买信息包
// 	m_pInfo->usVASPoint=usVASPoint;///增值点数
// 	m_pInfo->AccountID=AccountID;///帐号ID
// 	m_pInfo->PlayerID=PlayerID;///角色ID
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
///解包函数
BOOL CMsgVAS::Create(char* pMsgBuf,DWORD dwSize)
{
	if (!CNetMsg::Create(pMsgBuf, dwSize))
		return false;

	if(_MSG_VAS != this->GetType())
		return false;

	return true;
}