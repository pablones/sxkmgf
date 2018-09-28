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
	_LOTTERY_ADD	= 1,//����һ����Ʒ
	_LOTTERY_DEL	= 2,//ɾ��һ����Ʒ
	_LOTTERY_AMOUNT	= 3,//ͬ������
	_LOTTERY_SHOW	= 4,//�ͻ���չʾ��Ʒ
};
struct TreasureInfo
{
	OBJID	idTreasure;//��ƷID
	OBJID	itemType;//��Ʒ����
	USHORT	usAmount;//��Ʒ����
	USHORT	usDivide;//���
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
