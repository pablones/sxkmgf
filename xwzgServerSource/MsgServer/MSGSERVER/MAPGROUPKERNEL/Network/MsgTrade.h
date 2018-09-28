// MsgTrade.h: interface for the CMsgTrade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGTRADE_H__F3900087_7C9F_4D0D_906A_10E4EB5B5E40__INCLUDED_)
#define AFX_MSGTRADE_H__F3900087_7C9F_4D0D_906A_10E4EB5B5E40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

const unsigned short _TRADE_APPLY			= 1;		// ������		to client/server
const unsigned short _TRADE_QUIT			= 2;		// ȡ������		to server
const unsigned short _TRADE_OPEN			= 3;		// �򿪽��״�	to client
const unsigned short _TRADE_SUCCESS			= 4;		// ���׳ɹ�(ͬʱ�ر�)		to client
const unsigned short _TRADE_FALSE			= 5;		// ����ʧ��(ͬʱ�ر�)		to client
const unsigned short _TRADE_ADDITEM			= 6;		// ����Ʒ		to client/server(�Է��յ�MSGITEMINFO��Ϣ)
const unsigned short _TRADE_ADDMONEY		= 7;		// ��Ǯ			to server
const unsigned short _TRADE_MONEYALL		= 8;		// �Է�Ǯ����	to client(�Է��յ���Ǯ����)
const unsigned short _TRADE_SELFMONEYALL	= 9;		// �Լ�Ǯ��Ǯ	to client(��Ǯ����)
const unsigned short _TRADE_OK				= 10;		// ͬ�⽻��		to client/server
const unsigned short _TRADE_ADDITEMFAIL		= 11;		// ����Ʒʧ��	to client
const unsigned short _TRADE_NOTALLOW		= 12;		//�Լ���������to server
const unsigned short _TRADE_BACK_WU		    = 13;		//����Ʒ		to client/server
const unsigned short _TRADE_BACK_MONEY		= 14;
const unsigned short _TRADE_LOCK    		= 15;
const unsigned short _TRADE_ADDVAS	    	= 16;       //��vas
const unsigned short _TRADE_VASOTHER		= 17;		// �Է�vas
const unsigned short _TRADE_VASSELF     	= 18;		// �Լ�vas
const unsigned short _TRADE_REFUSE      	= 19;		//[2010-06-01 goto] �ܾ�����	to client/server
const unsigned short _TRADE_DELITEMFAIL		= 20;		//[2010-06-01 goto] ����Ʒʧ��	to client
const unsigned short _TRADE_ADDITEMOP		= 21;		//[2010-06-01 goto] �Է�������Ʒ	to client
const unsigned short _TRADE_BACK_WUOP		= 22;		//[2010-06-01 goto] �Է�������Ʒ	to client
const unsigned short _TRADE_UNLOCK			= 23;		//[2010-06-16 goto] �Լ��������	to client
const unsigned short _TRADE_UNLOCK_OP		= 24;		//[2010-06-16 goto] �Է��������	to client
const unsigned short _TRADE_TARGETTRADEING	= 25;		//[2010-06-22 goto] �Է����ڽ�����	to client

const unsigned short _TRADE_ADDMONSTER		= 26;		//[2010-07-30 goto] ���ӳ���		to server
const unsigned short _TEADE_ADDMONSTER_OP	= 27;		//[2010-07-30 goto] �Է����ӳ���	to client //δʹ�ã�ֱ�Ӹ��ͻ��˷��������
const unsigned short _TRADE_ADDMONSTERSUC	= 28;		//[2010-07-30 goto] �ӳ���ɹ�		to client
const unsigned short _TRADE_ADDMONSTERFAIL	= 29;		//[2010-07-30 goto] �ӳ���ʧ��		to client
const unsigned short _TRADE_DELMONSTER		= 30;		//[2010-07-30 goto] ���ٳ���		to server
const unsigned short _TRADE_DELMONSTERSUC	= 31;		//[2010-07-30 goto] ���ٳ���ɹ�	to client
const unsigned short _TRADE_DELMONSTERFAIL	= 32;		//[2010-07-30 goto] ���ٳ���ʧ��	to client
const unsigned short _TRADE_DELMONSTER_OP	= 33;		//[2010-07-30 goto] �Է����ٳ���	to client

const unsigned short _TEADE_ACCEPT_APPLY	= 34;		//[2010-08-26 goto] ͬ�⽻������,Ϊ����������ͬʱ���������׳��ֵ�BUG��Ӵ�action

class CMsgTrade : public CNetMsg   
{
public:
	CMsgTrade();
	virtual ~CMsgTrade();

public:
	BOOL	Create		(unsigned short usAction, OBJID id, int nType = 0);

	BOOL	Create		(char* pbufMsg, DWORD dwMsgSize);
	BOOL	Create		(CItem* pItem, unsigned short nAction=_TRADE_ADDITEM);//[2010-05-31 goto] ���ؽ�����Ʒ��Ϣ
	void	Process		(CGameSocket* pSocket);

private:
	typedef struct 
	{
		MSGHEAD_DEFINE

		OBJID			id;
		DWORD			nData;
		OBJID           idType;
		
		UINT type;
		unsigned short	usAction;
	}MSG_Info;

	MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGTRADE_H__F3900087_7C9F_4D0D_906A_10E4EB5B5E40__INCLUDED_)
