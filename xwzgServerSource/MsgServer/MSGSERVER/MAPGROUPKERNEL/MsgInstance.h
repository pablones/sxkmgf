// MsgInstance.h: interface for the CMsgInstance class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGINSTANCE_H__7F96DEBD_E496_4389_B20B_58194E41FCE2__INCLUDED_)
#define AFX_MSGINSTANCE_H__7F96DEBD_E496_4389_B20B_58194E41FCE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NETWORK\NetMsg.h"

///副本消息
class CMsgInstance : public CNetMsg  
{
public:
	CMsgInstance();
	virtual ~CMsgInstance();

};

#endif // !defined(AFX_MSGINSTANCE_H__7F96DEBD_E496_4389_B20B_58194E41FCE2__INCLUDED_)
