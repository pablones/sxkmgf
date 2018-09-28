// MsgBuff.h: interface for the CMsgBuff class.
//	20060922	����
//	Buff��Ϣ
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGBUFF_H__89528D88_2C03_48C3_904A_6CE4F828568F__INCLUDED_)
#define AFX_MSGBUFF_H__89528D88_2C03_48C3_904A_6CE4F828568F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

//------------------------07.4.25����ע��--------
//enum BuffType//Buff����,��user status�޸�.�Ժ�������չ
//{
//	BUFFTYPE_NONE			=	0,
//	BUFFTYPE_HITRATE_UP = 1,			//54 ����������HIT_RATE_UP	��������d%\%,����d%��
//	BUFFTYPE_HITRATE_DOWN = 2,		
//	BUFFTYPE_DODGE_UP = 3,				//47 ��������DODGE_UPd	��������d%\%,����d%��
//	BUFFTYPE_DODGE_DOWN = 4,		
//	BUFFTYPE_POWER_UP = 5,				//12 ս��������POWER_UP	���������˺�d%\%,����d%��
//	BUFFTYPE_POWER_DWON = 6,
//	BUFFTYPE_DEFENSE_UP = 7,			//26 ����������DEFENSE_UP	�����ܵ��˺�d%\%,����d%��
//	BUFFTYPE_DEFENSE_DOWN = 8,
//	BUFFTYPE_MOVE_SPEED_UP = 9,			//51 ����FAST_MOVE	�ӿ��ƶ��ٶ�d%\%,����d%��
//	BUFFTYPE_MOVE_SPEED_DOWN = 10,		//27 ������d���ٶ� d%\%,����N��
//	BUFFTYPE_WISDOM_UP = 11,			//56 ����֮��WISDOM_UP	���ܼ��ʱ�併��d%\%,����d%��
//	BUFFTYPE_TRANSFER_SHIELD = 12,		//108 ħ����	�����˺�d%��,����d%��
//	BUFFTYPE_BLESS	=13,				//111 �������BLESS	�������,����,����,����d%\%,����d%��
//	BUFFTYPE_PK_PROTECTED = 20,			//50 pk������
//	BUFFTYPE_IMMUNE = 27,				//�޵�
//	BUFFTYPE_FIRMNESSMAGIC = 28,		//�ᶨʩ��
//	BUFFTYPE_FRENZY = 29,				//��
//	BUFFTYPE_MULTIPLE_EXP = 30,			//˫������BUF//---���-----2007/03/29---------------
//
//	BUFFTYPE_BAD=100,
//	BUFFTYPE_PALSY = BUFFTYPE_BAD+14,				//30 ���	��ʱ���,ԭ�ز����ƶ�,����d%��
//	BUFFTYPE_POISON = BUFFTYPE_BAD+15,				//3  �ж�	ÿ4���ȥ����ֵd%��,����d%��
//	BUFFTYPE_BLEB = BUFFTYPE_BAD+16,					//58 ��Ѫ	ÿ4���ȥ����ֵd%��,����d%��
//	BUFFTYPE_MANA_DRINK = BUFFTYPE_BAD+17,			//59 �ɷ� 	ÿ4���ȥħ��ֵd%��,����d%��
//	BUFFTYPE_TIRED = BUFFTYPE_BAD+18,				//60 ����   ����TIRED	ÿ4���ȥ����ֵd%��,����d%��
//	BUFFTYPE_TAKE_SKZL =BUFFTYPE_BAD+19,				//57 ���1��ʱ��֮��TAKE_SKZL ����100�������ħ������
//	BUFFTYPE_FANJI	=BUFFTYPE_BAD+	21,				//109 ����״̬
//	BUFFTYPE_FAINT	=	BUFFTYPE_BAD+22,				//101 ѣ��
//	BUFFTYPE_CONFUSION	=BUFFTYPE_BAD+ 23,			//����
//	BUFFTYPE_SILENCE	=BUFFTYPE_BAD+ 24,			//��Ĭ
//	BUFFTYPE_BILE		=BUFFTYPE_BAD+ 25,			//��ŭ
//	BUFFTYPE_BURNING	= BUFFTYPE_BAD+26,			//����
//	 
//	
//};

//struct BuffInfo 
//{
//	int					buffPower;				//������Ч����ֵ.����Ϊ��ֵ.����Ϊ�ٷֱ�
//};
//-----------------07.4.25�������------------------------------
enum
{
	ACTION_ADDSTATUS	=	1,			//���һ��״̬,buff or debuff or dot
	ACTION_DELSTATUS	=	2,			//ɾ��һ��״̬,buff or debuff or dot
	ACTION_UPDATESTATUS	=	3,			//����һ��״̬............
};
//-------------------------end-------------------------------------

class CMsgBuff : public CNetMsg  
{
public:
	CMsgBuff();
	~CMsgBuff();

	//�����ڷ����
	BOOL	Create( int nAction,OBJID idUser,DWORD dwTypeID,int nTime,int nCount,int nPower,int nParam1,int nParam2,int nParam3 );
	BOOL	Create(char* pMsgBuf, DWORD dwSize);
	void	Process(void *pInfo);

private:
	typedef struct
	{
		MSGHEAD_DEFINE
		int				nAction;
		OBJID			idUser;
		DWORD			dwTypeID;
		int				nTime;			//����ʱ����߼��ʱ��
		int				nCount;			//���״̬�Ĵ���
		
		int				nPower;
		int				nParam1;
		int				nParam2;
		int				nParam3;
		

	}MSG_Info;
	
	MSG_Info *m_pInfo;
};

#endif // !defined(AFX_MSGBUFF_H__89528D88_2C03_48C3_904A_6CE4F828568F__INCLUDED_)
//238,239,230 (#E6EFEE)
