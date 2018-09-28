// MsgVAS.h: interface for the CMsgVAS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGVAS_H__372D264E_A294_471D_B7B9_CBE4716A2B5A__INCLUDED_)
#define AFX_MSGVAS_H__372D264E_A294_471D_B7B9_CBE4716A2B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
//[2010-07-26 goto]action����
enum
{
	_MSGVAS_QUERY_BALANCE	=	1001,//��ѯ��� C-S
	_MSGVAS_ADD_VAS			=	1002,//����ӻƽ� MS-AS
	_MSGVAS_BUY_ITEM		=	1003,//�����̳���Ʒ C-S	
	_MSGVAS_SPEND_VAS		=	1004,//���ٻƽ� MS-AS
	_MSGVAS_QUERY_CARD		=	1005,//��ѯ�� C-S
	_MSGVAS_POINT_BUY		=	1007,//��ȯ������Ʒ
	_MSGVAS_OPRATE_CARD		=	1006,//������ C-S
	_MSGVAS_QUERY_ACTIVITY	=	1008,//��ѯ�����״̬
	_MSGVAS_OPRATE_ACTIVITY	=	1009,//���������״̬
	_MSGVAS_BUY_LOTTERY		=	1010,//���Ʊ,������
	_MSGVAS_BUY_ASALE		=	1011,//���ؼ���Ʒ
	_MSGVAS_QUERY_MOBCHARGE =   1012,//��ѯ�ֻ���ֵ���
	_MSGVAS_GET_MOBCHARGE   =   1013,//����ֻ���ֵ���
	_MSGVAS_ADD_VAS_CPST	=	1014,
		
	_MSGVAS_RETURN_BALANCE	=	2002,//�˺ŷ������������
	_MSGVAS_BUY_SUC			=	2004,//����ɹ�
	_MSGVAS_BUY_FAIL		=	2005,//����ʧ��
	_MSGVAS_ADD_VAS_SUC		=	2006,//����ӻƽ� AS-MS
	_MSGVAS_ADD_VAS_FAIL	=	2007,//����ӻƽ� AS-MS
	_MSGVAS_SPEND_VAS_SUC	=	2008,//���ٻƽ� AS-MS
	_MSGVAS_SPEND_VAS_FAIL	=	2009,//���ٻƽ� AS-MS
	_MSGVAS_CARD_STATES     =   2010,//���ؿ�״̬
	_MSGVAS_POINT_BUY_SUC	=	2011,//��ȯ������Ʒ�ɹ�
	_MSGVAS_POINT_BUY_FAIL	=	2012,//��ȯ������Ʒʧ��
	_MSGVAS_ACTIVITY_STATES	=	2013,//���ػ����״̬
	_MSGVAS_BUY_LOTTERY_SUC	=	2014,//���Ʊ�ɹ�
	_MSGVAS_BUY_LOTTERY_FAIL=	2015,//���Ʊʧ��
	_MSGVAS_BUY_ASALE_SUC	=	2016,//���ؼ���Ʒ�ɹ�
	_MSGVAS_BUY_ASALE_FAIL	=	2017,//���ؼ���Ʒʧ��
	_MSGVAS_MOBCHARGE_STATE   = 2018,
	_MSGVAS_ADD_VAS_FAIL_CPST = 2019,
	_MSGVAS_ADD_VAS_SUC_CPST = 2020,
};
//  [2007-9-15] ���
enum
{
	_MSGVAS_REQUEST_ITEM_LIFE_TIME_CONTINUE = 300,	//	�ͻ�����������
	_MSGVAS_REQUEST_ITEM_LIFE_TIME_CONTINUE_SUC = 301,	//	���ѳɹ�
	_MSGVAS_REQUEST_ITEM_LIFE_TIME_CONTINUE_FAL = 302,	//	����ʧ��
	_MSGVAS_CHG_SUC = 303,
	_MSGVAS_CHG_FAL = 304,
	_MSGVAS_CHG_TRADE = 305,//[��;���� 2008/12/3]�޸���ֵ������
	_MSGVAS_CHG_TRADE_SUC = 306,//[��;���� 2008/12/3]�޸���ֵ�����
	_MSGVAS_CHG_TRADE_FAL = 307,//[��;���� 2008/12/3]�޸���ֵ�ҳ���
	_MSGVAS_CHG = 308,//[��;���� 2009.01.08]ֱ���޸���ֵ������
	_MSGVAS_SERVER_SUC = 309,
	_MSGVAS_SERVER_FAL = 310,

	_MSGVAS_DAYMASK_ACC_GET = 311,
	_MSGVAS_DAYMASK_ACC_SET = 312,
	_MSGVAS_DAYMASK_ACC_RETURN = 313,
};


///��ֵ������Ϣ
class CMsgVAS : public CNetMsg  
{
public:
	CMsgVAS();
	virtual ~CMsgVAS();

// 	typedef struct tag_MsgTraffic
// 	{
// //		MSGHEAD_DEFINE
// 		OBJID		id;
// 		USHORT		usAction;
// 		OBJID		itemID;//20060915����
// 		int			itemAmount;//20060915����
	// 		DWORD		dwData;	//  [2/16/2008 ���]	��������
	// 	}MsgTraffic;
	//[2010-07-26 goto]
	typedef struct tag_MsgTraffic
	{
		OBJID		itemID;//20060915����
		int			itemAmount;//20060915����
		DWORD		dwData;	//  [2/16/2008 ���]	�������� ֧�ַ�ʽ:Ԫ�����鱦����ȯ
	}MsgTraffic;

//	BOOL Create(const MsgTraffic& mtExtraInfo,int usVASPoint,OBJID AccountID,OBJID PlayerID);///������ֵ����������Ϣ
	BOOL Create( int usVASPoint, OBJID AccountID, OBJID PlayerID, USHORT usAction, OBJID itemID=ID_NONE, int itemAmount=0, DWORD dwData=0);
public:	
	BOOL Create(char* pMsgBuf,DWORD dwSize);///���
//	void			Process		(CGameSocket* pSocket);

//private:


// 	typedef struct tag_MSG_Info
// 	{
// 		unsigned short	unMsgSize;
// 		unsigned short	unMsgType;
// 		MsgTraffic		mtExtraInfo;///���ӽ��װ���Ϣ
// 		int				usVASPoint;///��ֵ����
// 		OBJID			AccountID;///�ʺ�ID
// 		OBJID			PlayerID;///��ɫID
// 		int				nJuan;///����
// 		int				nRealID;///����
// 		char			szRegard[1];	//  [2007-9-21] ���	������
	// 	}MSG_Info;
	//[2010-07-26 goto]
	typedef struct tag_MSG_Info
	{
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
		
		ULONG			usVASPoint;///��ֵ����
		OBJID			AccountID;///�ʺ�ID
		OBJID			PlayerID;///��ɫID
		
		USHORT			usAction;
		USHORT			usGoodSize;//���ﳵ����Ʒ����
		MsgTraffic		nGoods[1];//��Ʒ
	}MSG_Info;
	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGVAS_H__372D264E_A294_471D_B7B9_CBE4716A2B5A__INCLUDED_)
