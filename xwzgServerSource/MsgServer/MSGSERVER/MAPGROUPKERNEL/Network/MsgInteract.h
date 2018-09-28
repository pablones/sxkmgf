// MsgInteract.h: interface for the CMsgInteract class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSTEAL_H__05C99CA2_5829_11D5_A212_0000E8E7F527__INCLUDED_)
#define AFX_MSGSTEAL_H__05C99CA2_5829_11D5_A212_0000E8E7F527__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum {	INTERACT_NONE =0, 
		INTERACT_STEAL, 
		INTERACT_ATTACK, 
		INTERACT_HEAL, 
		INTERACT_POISON, 
		INTERACT_ASSASSINATE,
		INTERACT_FREEZE,
		INTERACT_UNFREEZE,
		INTERACT_COURT,
		INTERACT_MARRY,
		INTERACT_DIVORCE			= 10,
		INTERACT_PRESENTMONEY,
		INTERACT_PRESENTITEM,
		INTERACT_SENDFLOWERS,
		INTERACT_KILL				= 14,
		INTERACT_JOINSYN,
		INTERACT_ACCEPTSYNMEMBER,
		INTERACT_KICKOUTSYNMEMBER,
		INTERACT_PRESENTPOWER,
		INTERACT_QUERYINFO,
		INTERACT_RUSHATK			= 20,
		INTERACT_MAGICATTACK		= 21, 
		INTERACT_ABORTMAGIC,
		INTERACT_REFLECTWEAPON,
		INTERACT_BUMP,
		INTERACT_SHOOT				= 25,
		INTERACT_REFLECTMAGIC,
		INTERACT_CUTMEAT,					// ��ʱû�ã� ���� idSender:HeroID, iDTarget:Target ID, usPosX HeroPosX, usPosY HeroPosY
		INTERACT_MINE,                      // �ڿ� idSender:HeroID, iDTarget:Target ID, usPosX HeroPosX, usPosY HeroPosY 
		INTERACT_QUARRY,					// ��ʱû�ã� ��ʯ idSender:HeroID, iDTarget:Target ID, usPosX HeroPosX, usPosY HeroPosY
		INTERACT_CHOP				= 30,	// ��ʱû�ã� ��ľ idSender:HeroID, iDTarget:Target ID, usPosX HeroPosX, usPosY HeroPosY
		INTERACT_HUSTLE,					// ��ʱû�ã� ���ˣ�dwDataΪ��һ���ķ���(ע�⣺�������ͬʱ��÷����ƶ�һ��)
		INTERACT_SOUL,						// ���������ȡ������Ǳ���
		INTERACT_OPENWARSKILL,		//33,	սʿ�������� //06.3.21����
		INTERACT_CLOSEWARSKILL,		//34	սʿ���ܹر�
		INTERACT_INTONESCROLL,		//35,    ����070324�����ʹ�þ���״̬
		INTERACT_OPENSAVEITEM,      //�򿪵������
		ITEMACT_SAVEITEMSHOW,        //������ʾ
		INTERACT_MAYYRTYPE,         //��������
		//20080108����:AI���--------------------
		INTERACT_SynEnmity	= 201,		//msg 2 npc ͬ������б�
		//---------------------------------------
		INTERACT_MOUNT	=	300,	//[2010-08-09 goto]����
		INTERACT_MOUNT_ON	=	301,//[2010-08-09 goto]������
		INTERACT_MOUNT_OFF	=	302,//[2010-08-09 goto]������
};

class CMsgInteract : public CNetMsg  
{
public:
	CMsgInteract();
	virtual ~CMsgInteract();

	BOOL	Create2(USHORT unType, OBJID idSender, OBJID idTarget, USHORT unPosX, USHORT unPosY,DWORD dwData = 0,DWORD dwBattleAddInfo=0);
	BOOL	Create(USHORT unType, OBJID idSender, OBJID idTarget, USHORT unPosX, USHORT unPosY, USHORT usMagicType, USHORT usMagicLev, DWORD nIntoneTime=0);

public:	
	BOOL		Create		(char* pMsgBuf, DWORD dwSize);
	void		Process		(void* pInfo);
	
private:
	typedef struct{
		MSGHEAD_DEFINE
		OBJID	idSender;
		OBJID	idTarget;

		USHORT	unType;
		DWORD	dwData;
		USHORT	usPosX;
		USHORT	usPosY;
		USHORT	usMagicType;
		USHORT	usMagicLev;
		DWORD dwBattleAddInfo;
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGSTEAL_H__05C99CA2_5829_11D5_A212_0000E8E7F527__INCLUDED_)
