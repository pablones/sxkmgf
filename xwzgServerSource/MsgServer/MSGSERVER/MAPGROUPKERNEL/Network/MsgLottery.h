// MsgLottery.h: interface for the CMsgLottery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGLOTTERY_H__E9265FB2_27C1_4D2A_8370_617F88A07C15__INCLUDED_)
#define AFX_MSGLOTTERY_H__E9265FB2_27C1_4D2A_8370_617F88A07C15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
enum _LOTTERY
{
	_LOTTERY_ADD	= 1,//增加一个奖品
	_LOTTERY_DEL	= 2,//删除一个奖品
	_LOTTERY_AMOUNT	= 3,//同步数量
	_LOTTERY_SHOW	= 4,//客户端展示奖品
};
struct TreasureInfo
{
	OBJID	idTreasure;//奖品ID
	OBJID	itemType;//奖品类型
	USHORT	usAmount;//奖品数量
	USHORT	usDivide;//类别
};
class CMsgLottery : public CNetMsg  
{
public:
	CMsgLottery();
	virtual ~CMsgLottery();
public:
	BOOL	Create(char* pbufMsg, DWORD dwMsgSize);
    BOOL	Create(int nAction, OBJID idUser);
	BOOL	Append(OBJID idTreasure, OBJID itemType, USHORT usAmount, USHORT usDivide);
	
	void	Process(void* pInfo);
	
public:
	typedef struct 
	{
		MSGHEAD_DEFINE
		OBJID               idUser;
		UINT				uAction;
		UINT        		uAmount;
		TreasureInfo		treasureInfo[1];
	}MSG_Info;
	
	MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGLOTTERY_H__E9265FB2_27C1_4D2A_8370_617F88A07C15__INCLUDED_)
