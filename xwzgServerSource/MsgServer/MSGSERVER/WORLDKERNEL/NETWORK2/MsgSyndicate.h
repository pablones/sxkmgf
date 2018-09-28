// MsgBetOutside.h: interface for the CMsgAtk class.
// �ɽ��ޣ�2002.1.8
//////////////////////////////////////////////////////////////////////

#ifndef	MSGSYNDICATE_H
#define MSGSYNDICATE_H

#include "NetMsg.h"
#include "NetStringPacker.h"

// enum{
// 	ACTION_NONE = 0,				// ����ֵ
// 
// 	APPLY_JOIN			= 1,			// �����������, id
// 	INVITE_JOIN			= 2,			// �����������, id
// 	LEAVE_SYN			= 3,			// ��������
// 	KICKOUT_MEMBER		= 4,			// ����������Ա, name
// 	QUERY_SYN_NAME		= 6,			// ��ѯ��������
// 	SET_ALLY			= 7,			// ����				// to client, npc(npc_id is syn_id, same follow)
// 	CLEAR_ALLY			= 8,			// �������			// to client, npc
// 	SET_ANTAGONIZE		= 9,			// ����				// to client, npc
// 	CLEAR_ANTAGONIZE	= 10,			// �������			// to client, npc
// 	DONATE_MONEY		= 11,			// ���ھ�Ǯ
// 	QUERY_SYNATTR		= 12,			// ��ѯ������Ϣ		// to server
// 	SET_SYN				= 14,			// ��Ӱ���ID		// to client
// 	//UNITE_SUBSYN		= 15,			// �ϲ��ÿ� // to client // dwData���ϲ�, target������
// 	//UNITE_SYN			= 16,			// �ϲ����� // to client // dwData���ϲ�, target������
// 	SET_WHITE_SYN		= 17,			// �װ����ID // δ��ռ���뷢ID_NONE
// 	SET_BLACK_SYN		= 18,			// �ڰ����ID // δ��ռ���뷢ID_NONE
// 	DESTROY_SYN			= 19,			// ����㲥��ɾ�����ɡ�
// 	SET_MANTLE          = 20,            // ����㲥������ 
// 	_APPLY_ALLY			= 21,			// �������			// to server&client, idTarget=SynLeaderID
// 	_CLEAR_ALLY			= 22,			// �������			// to server
// 	_SET_ANTAGONIZE		= 23,			//���� client to server
// 	_CLEAR_ANTAGONIZE   = 24,			//������� client to server
// 	INVITE_JOIN_NAME	= 25,			//[��;���� 2008/10/6]����������
// 	INVITE_RETIRE		= 26,			//[��;���� 2008/10/7]��ְ
// 	NPCMSG_CREATE_SYN	= 101,			// ֪ͨNPC��������Ӱ���	// to npc
// 	NPCMSG_DESTROY_SYN	= 102,			// ֪ͨNPC������ɾ������	// to npc
// 	KICKOUT_MEMBER_INFO_QUERY	= 110,	//������ѯ���뿪���ĳ�Ա
// 	KICKOUT_MEMBER_AGREE		= 111,	//����ͬ�⿪����Ա
// 	KICKOUT_MEMBER_NOAGREE		= 112,	//������ͬ�⿪����Ա
// 	SYNMEMBER_ASSIGN		= 113,			//���ɳ�Ա����
// 	SYN_CHANGE_NAME			= 114,			// ���ɸ���
// 	SYN_CHANGE_SUBNAME		=114,		//���Ÿ���
// 	SYN_CHANGE_SUBSUBNAME	=115,		//�ֶӸ���
// 	SYN_DEMISE				=116,		//����
// 	SYN_SET_ASSISTANT		=117,		//[��;���� 2008/9/27]�԰�����ְ
// 	SYN_SET_TEAMLEADER		=118,		//���ð��ɶӳ�
// 	SYN_SET_PUBLISHTIME		=119,		//���ù���ʱ��
// 	SYN_SET_BUS				=120,		//����ְ����
// 	SYN_PAYOFF				=121,		//20070413����:C2S ���ų�������
// 	SYN_WAR                 =122,       //Լս
// 	SYN_BUY_UP              =123,
// 	SYN_MAIN_UP				=124,
// 	SYN_TASK_UP				=125,
// 	SYN_CHG_UP				=126,
// 	INVITE_JOIN_RE			=127,
// 	SYN_WAR_SET             =128,       //Լս
// };

enum SyndicateAction{
	ACTION_NONE = 0,				// ����ֵ
		
		APPLY_JOIN			= 1,			// �����������, id
		INVITE_JOIN			= 2,			// �����������, id
		LEAVE_SYN			= 3,			// ��������
		KICKOUT_MEMBER		= 4,			// ����������Ա, name
		QUERY_SYN_NAME		= 6,			// ��ѯ��������
		SET_ALLY			= 7,			// ����				// to client, npc(npc_id is syn_id, same follow)
		CLEAR_ALLY			= 8,			// �������			// to client, npc
		SET_ANTAGONIZE		= 9,			// ����				// to client, npc
		CLEAR_ANTAGONIZE	= 10,			// �������			// to client, npc
		DONATE_MONEY		= 11,			// ���ھ�Ǯ
		QUERY_SYNATTR		= 12,			// ��ѯ������Ϣ		// to server
		
		SET_SYN				= 14,			// ��Ӱ���ID		// to client
		//UNITE_SUBSYN		= 15,			// �ϲ��ÿ� // to client // dwData���ϲ�, target������
		//UNITE_SYN			= 16,			// �ϲ����� // to client // dwData���ϲ�, target������
		SET_WHITE_SYN		= 17,			// �װ����ID // δ��ռ���뷢ID_NONE
		SET_BLACK_SYN		= 18,			// �ڰ����ID // δ��ռ���뷢ID_NONE
		DESTROY_SYN			= 19,			// ����㲥��ɾ�����ɡ�
		SET_MANTLE          = 20,            // ����㲥������ // add huang 2004.1.1       
		
		_APPLY_ALLY			= 21,			// �������			// to server&client, idTarget=SynLeaderID
		_CLEAR_ALLY			= 22,			// �������			// to server
		
		_SET_ANTAGONIZE		= 23,			//���� client to server
		_CLEAR_ANTAGONIZE   = 24,			//������� client to server
		
		INVITE_JOIN_NAME	= 25,			//[��;���� 2008/10/6]����������
		INVITE_RETIRE	= 26,			//[��;���� 2008/10/7]��ְ
		
		NPCMSG_CREATE_SYN	= 101,			// ֪ͨNPC��������Ӱ���	// to npc
		NPCMSG_DESTROY_SYN	= 102,			// ֪ͨNPC������ɾ������	// to npc
		
		KICKOUT_MEMBER_INFO_QUERY	= 110,	//������ѯ���뿪���ĳ�Ա
		KICKOUT_MEMBER_AGREE		= 111,	//����ͬ�⿪����Ա
		KICKOUT_MEMBER_NOAGREE		= 112,	//������ͬ�⿪����Ա
		
		SYNMEMBER_ASSIGN	= 113,			//���ɳ�Ա����	
		
		SYN_CHANGE_NAME		= 114,			// ���ɸ���
		
		
		
		
		SYN_CHANGE_SUBNAME		=114,		//���Ÿ���
		SYN_CHANGE_SUBSUBNAME	=115,		//�ֶӸ���
		
		
		SYN_SET_ASSISTANT		=117,		//[��;���� 2008/9/27]�԰�����ְ
		SYN_SET_TEAMLEADER		=118,		//���ð��ɶӳ�
		SYN_SET_PUBLISHTIME		=119,		//���ù���ʱ��
		SYN_SET_BUS				=120,		//����ְ����
		SYN_PAYOFF				=121,		//20070413����:C2S ���ų�������
		SYN_WAR                 =122,       //Լս
		SYN_BUY_UP              =123,
		SYN_MAIN_UP				=124,
		SYN_TASK_UP				=125,
		SYN_CHG_UP				=126,
		INVITE_JOIN_RE			=127,
		SYN_WAR_SET             =128,       //Լս
		SYN_WAR_SEND            =129,       //Լս
		
		
		SYN_CREATE              =201,
		SYN_CREATE_OK           =202,
		SYN_CREATE_ERROR        =203,
		SYN_QUERY_CREATE        =204,
		SYN_JOIN				=205,
		SYN_JOIN_QUERY			=206,
		SYN_QUERY_LIST          =207,
		SYN_QUERY_INFO          =208,
		SYN_QUERY_MEMBER        =209,
		SYN_QUERY_QUERYMEMBER   =210,
		SYN_ANSERR_OK           =211,
		SYN_ANSERR_NO           =212,
		SYN_KICKOUT	            =213,
		SYN_LEAVER	            =214,
		SYN_SETRANK	            =215,
		SYN_UPDATE	            =216,
		SYN_DEMISE				=217,		//����
		SYN_ANNONCE				=218,
		SYN_CREATECOMPLETE		=219,
		SYN_ANNONCE_OTHERLINE	=220,
		SYN_EVERYSUB			=221,		//������Ϣ
		SYN_BUILDER				=222,
		SYN_BUILDER_OTHERLINE	=223,		//�����޸�
		SYN_UPLEV				=224,		//�������
 		SYN_UPSKILLSUB			=225,		//���ý���
		SYN_UPLEV_OTHERLINE		=226,
		SYN_UPSKILLSUB_OTHERLINE=227,
		SYN_SYNWORK				=228,		//��
		SYN_WORKOVER			=229,
		SYN_CONTRIBUTE			=230,		//����
		SYN_SETBUILDOTHERLINE	=231,
		SYN_REDUCELEV			=232,
		SYN_INVITE				=233,		//�������
		SYN_INVITE_OTHERLINE	=234,
		SYN_CHANGELINE			=335,		//����--jay
		SYN_IMPACH				=336,		//����--jay
		SYN_IMPACHOTHERLINE		=337,		//�����߷����ĵ���
		SYN_MEMBERLIST			=301,		//��Ա�б�
		SYN_CONLIST				=302,		//�ﹱ�б�
		SYN_QUERYLIST			=303,		//�����б�
		SYN_QUERYSYNLIST		=304,		//�����б�
		SYN_SYNINFO				=305,		//������Ϣ
		SYN_SCIINFO				=306,		//�Ƽ���Ϣ
		SYN_LEVELUP				=310,		//��������
		SYN_ACTION_ENTERSYN		=320,		//���ٻذ�
		SYN_SUB1_ACTION			=321,		//����
		SYN_SUB2_ACTION			=322,		//�׻�
		SYN_SUB3_ACTION			=323,		//����
		SYN_SUB4_ACTION			=324,		//��ȸ
		SYN_ACTION_WELFARE		=325,		//��ȡ����
		SYN_ACTION_SHOP			=326,		//�̵�
		SYN_SYNTALKMSG			=334,		//����Ⱥ����Ϣ
		SYN_UPDATEOTHERNLINE	=335,		//����
		SYN_SETLEADER			=338,		//��������
		SYN_SETSYNMAPID			=340,		//���°��ɵ�ͼid
};

WORLDKERNEL_BEGIN
class CMsgSyndicate : public CNetMsg
{
public:
	CMsgSyndicate();
	virtual ~CMsgSyndicate();
public:	

	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void *pInfo);

//protected:
	typedef struct {
		MSGHEAD_DEFINE

		unsigned short	usAction;
		unsigned short	dwData;
		OBJID	idTarget;
		char	szBuf[1];
		
// 		unsigned short	usAction;
// 		union
// 		{
// 			OBJID	idTarget;
// 			DWORD	dwData;
// 		};
// 		union {
// 			OBJID	idFealty;				// �ϼ�����ID
// 			OBJID	idUser;					// �������޸İ��ɱ���ʱ
// 		};
// 		char	szBuf[1];
	}MSG_Info;
	
	MSG_Info*	m_pInfo;

	CNetStringPacker m_StrPacker;
	
};
WORLDKERNEL_END

#endif // MSGSYNDICATE_H
