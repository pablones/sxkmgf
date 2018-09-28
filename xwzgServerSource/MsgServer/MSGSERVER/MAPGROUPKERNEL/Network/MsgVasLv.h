//////////////////////////////////////////////////////////////////////

#ifndef _MSGVASLV_H_
#define _MSGVASLV_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "NetMsg.h"

enum {	ACTION_NONE_			=0, 
		ACTION_QUERYVASLVTOSER	=1,	//�ͻ��˲�ѯ��ֵԪ���ȼ���ֵ
		ACTION_QUERYVASLVTOACC,	 	//msg��ѯ��ֵԪ���ȼ���ֵ
		ACTION_QUERYVASLVTOMSG,		//account���س�ֵԪ���ȼ���ֵ
		ACTION_QUERYVASLVTOCLI,		//msg���ظ��ͻ��˳�ֵԪ���ȼ���ֵ
		ACTION_GETITEM			=5,	//�ͻ��˻�ȡԪ���ȼ�����
		ACTION_UNEQU1,				//�ݱ���
		ACTION_LOGINUPDATE,				//һ����¼����Ԫ���ȼ���Ϣ
		ACTION_UNEQU3,				//���������������������Ϣ
		ACTION_UNEQU4,				//ˢ�����н�ɫԪ���ȼ���Ϣ
		ACTION_QUERYVISITTOSER	=10,//�ͻ��˲�ѯ������ֵ
		ACTION_QUERYVISITTOCLI,		//msg���ظ��ͻ���������ֵ
		ACTION_REDUCEVAS,			//��Ԫ��
		ACTION_UNEQU6,				//�ݱ���
		ACTION_REDUCEVASUPDATE,		//��Ԫ���󷵻�
		ACTION_VISITFREETOSER	=15,//�ͻ����ύ����һ��
		ACTION_VISITFREETOCLI,		//msg���ظ��ͻ���������Ϣ
		ACTION_VISITVASTOSER,		//�ͻ����ύ����ǧ��һ��
		ACTION_VISITVASTOCLI,		//msg���ظ��ͻ�������ǧ����Ϣ
		ACTION_UNEQU8,				//�ݱ���
		ACTION_SENDANSWER		=20,//�ͻ����ύ�����
};
#pragma pack(1)
class CMsgVasLv2 : public CNetMsg
{
public:
	CMsgVasLv2();
	virtual ~CMsgVasLv2();
	BOOL	Create(unsigned short usAction,OBJID id,DWORD viscou,DWORD maxvis,DWORD vascount,OBJID questionid);
	BOOL	Create(unsigned short usAction,OBJID id,DWORD vastotal,DWORD lvtotal,DWORD dwcount,DWORD vaslv,DWORD awarditem,DWORD viscou,DWORD maxvis,DWORD vascount,OBJID questionid,DWORD questionan);
public:
	BOOL	Create(char* pMsgBuf, DWORD dwSize);
	
private:
	typedef struct {
		USHORT		unMsgSize;
		USHORT		unMsgType;
		unsigned short	usAction;//
		OBJID			id;//��ɫid
		DWORD			vastotal;//Ԫ������
		DWORD			lvtotal;//��ʾ��Ԫ������
		DWORD			dwcount;//�����ֵԪ����
		DWORD			vaslv;//Ԫ��VIP�ȼ�
		DWORD			awarditem;//��������
		
		DWORD			viscou;//ÿ����������
		DWORD			maxvis;//ÿ�������������
		DWORD			vascount;//����ǧ������Ԫ����
		
		OBJID			questionid;//���⼯id
		DWORD			questionan;//����Ĵ�
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
};

class CMsgVasLv : public CNetMsg
{
public:
	CMsgVasLv();
	virtual ~CMsgVasLv();

	BOOL	Create(unsigned short usAction,OBJID id,DWORD vastotal,DWORD lvtotal,DWORD dwcount,DWORD vaslv,DWORD awarditem,DWORD viscou,DWORD maxvis,DWORD vascount,OBJID questionid,DWORD questionan);
	BOOL	Create(unsigned short usAction,OBJID id,DWORD vastotal,DWORD lvtotal,DWORD dwcount,DWORD vaslv,DWORD awarditem);
	BOOL	Create(unsigned short usAction,OBJID id,DWORD viscou,DWORD maxvis,DWORD vascount,OBJID questionid);
	BOOL	Create(unsigned short usAction,OBJID id,OBJID questionid,DWORD questionan);
public:
	BOOL	Create(char* pMsgBuf, DWORD dwSize);
	void	Process(void* pInfo);

private:
	typedef struct {
		MSGHEAD_DEFINE
		unsigned short	usAction;//
		OBJID			id;//��ɫid
		DWORD			vastotal;//Ԫ������
		DWORD			lvtotal;//��ʾ��Ԫ������
		DWORD			dwcount;//�����ֵԪ����
		DWORD			vaslv;//Ԫ��VIP�ȼ�
		DWORD			awarditem;//��������

		DWORD			viscou;//ÿ����������
		DWORD			maxvis;//ÿ�������������
		DWORD			vascount;//����ǧ������Ԫ����

		OBJID			questionid;//���⼯id
		DWORD			questionan;//����Ĵ�
	}MSG_Info;

	MSG_Info*	m_pInfo;
};
#pragma pack()
#endif
