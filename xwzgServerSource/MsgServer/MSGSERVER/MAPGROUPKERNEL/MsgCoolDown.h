
#ifndef _MSGCOOLDWON_H
#define _MSGCOOLDWON_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
enum
{

	MAGICCOOLDOWN_BEGIN			=	0,		//��ʼ������ȴʱ��
	MAGICCOOLDOWN_END			=	1,		//����������ȴʱ��
};
enum INFO_TYPE
{
	MAGIC_INFO	= 0,								//����
	ITEM_INFO	= 1,								//��Ʒ
};
//FileName:	MsgCoolDown.h
//Time:		2006.9.4
//Author:	����
//About:	���ڼ�����ȴʱ��ӵ���Ϣ��


class CMsgCoolDown:public CNetMsg
{
	
public:
	CMsgCoolDown();
	virtual ~CMsgCoolDown();
	
	BOOL	Create		( OBJID id,USHORT usAction,USHORT usInfoType,OBJID idObjType,DWORD dwPrivateCDTime,DWORD dwPublicCDTime );
	void	Process		( void* pInfo );
	
protected:
	typedef struct
	{
		MSGHEAD_DEFINE

		OBJID		idUser;							//user id
		USHORT		usAction;						//action
		USHORT		usInfoType;						// ĿǰΪħ������Ʒ
		OBJID		idObjType;						//���ܱ�Ż�����ƷTYPE ID
		DWORD		dwPrivateCDTime;				//�˼��ܵ�CDʱ��
		DWORD		dwPublicCDTime;					//����CDʱ��
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
};











#endif // _MSGCOOLDWON_H