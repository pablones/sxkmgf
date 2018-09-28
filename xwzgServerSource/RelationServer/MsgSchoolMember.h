// MsgSchoolMember.h: interface for the CMsgSchoolMember class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSCHOOLMEMBER_H__9F1EABD9_F12D_45F7_A088_F99E49BACEB4__INCLUDED_)
#define AFX_MSGSCHOOLMEMBER_H__9F1EABD9_F12D_45F7_A088_F99E49BACEB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum {
	MESSAGEBOARD_DELMEMBER		= 1, //ɾ��
	MESSAGEBOARD_ADDMEMBER		= 2, //����
	MESSAGEBOARD_UPDATEMEMBER	= 3, //����
	MESSAGEBOARD_RECALL = 4,//ͽ�ܷ�����������״̬
	MESSAGEBOARD_APPLYDEL,	
	MESSAGEBOARD_RECOVER,
	MESSAGEBOARD_ADDDENSITY,
	MESSAGEBOARD_TUTORLOGHINT,
	MESSAGEBOARD_QUERYINFO, //9 ��ѯ��ϸ
	MESSAGEBOARD_QUERYTOTURLIST,//10 ��ѯʦ���б�
	MESSAGEBOARD_ADDTOLIST,//11
	MESSAGEBOARD_SYNLINE,//12 ��·֮��ͬ��ʦ���б�����

	MESSAGEBOARD_APPLYADDTUTOR,//�����ʦ
	MESSAGEBOARD_AGREESTU,//ͬ���ʦ����
	MESSAGEBOARD_REFUSESTU,//�ܾ���ʦ����
	MESSAGEBOARD_GRADUATEAPPLY,//ʦ����ͽ�ܳ�ʦ
	MESSAGEBOARD_DIVORCE,//Э����ʦͽ��ϵ
	MESSAGEBOARD_REQDIVORCE,//ͽ����Ӧ�����ϵ
	MESSAGEBOARD_STUREBEL,//ͽ����ʦ
	
	MESSAGEBOARD_LINEBROAD,//������·��ʦͽ��Ϣ
	MESSAGEBOARD_QUERYSTUAPPLYLIST,//��ѯͽ�������б�
	MESSAGEBOARD_TUTORKICKOUTSTU,//ʦ���߳�ͽ��
	MESSAGEBOARD_QUERYSTU,		//ͽ���б�
	MESSAGEBOARD_ALLLIST,		//ʦͽ�б�
	
	MESSAGEBOARD_DELAPPLYLIST,	//ɾ��ͽ���б��е�ͽ��
};

enum {
	RELATION_TEACHER = 1,		// ��ʦ
	RELATION_STUDENT,			// ͽ��
	RELATION_STUDENTOFSTUDENT,	// ͽ�ܵ�ͽ��(ͽ��)
	RELATION_STUDENT_GRADUATE,	// �ѳ�ʦ��ͽ��
};

enum {
	MEMBERSTATUS_OFFLINE = 0,	// ����״̬
	MEMBERSTATUS_ONLINE,		// ����״̬
	MEMBERSTATUS_RECALL,//ͽ�ܷ�����������״̬
};
enum {
	SYNTUTOR_NONE	= 0,
	SYNTUTOR_ONLINE = 1,//ʦ���б����������
	SYNTUTOR_OFFLINE = 2,//ʦ���б����������
	SYNTUTOR_ADD	 = 3,//ʦ���б�����
	SYNTUTOR_DEL	 = 4,//ʦ���б�ɾ��
};
enum {
	LINEBROAD_STUREBEL = 0,//ͽ����ʦ
	LINEBROAD_TUTORKICKOUT = 1,//ʦ���߳�ͽ��
	LINEBROAD_APPLYTUTOR	= 2,//��ʦ����
	LINEBROAD_STATE			= 3,//����״̬
	LINEBROAD_AGREEAPPLY	= 4,//���ܰ�ʦ
	LINEBROAD_REFUSEAPPLY	= 5,//�ܾ���ʦ
	LINEBROAD_ONLINESTATE	= 6,//����״̬
	LINEBROAD_ONLINESTATEREF= 7,//��������״̬
	LINEBROAD_SYNGIFTMASK   = 8,//ͬ����������
	LINEBROAD_CALLSTU		= 9,//�ٻ�ͽ��
	LINEBROAD_CALLSTUREF	= 10,//ͽ����Ӧ�ٻ�
	LINEBROAD_ADDEDLEV		= 11,//��ͽ�������ĵȼ�
	LINEBROAD_APPLYBYNAME	= 12,//�����ְ�ʦ
};
typedef struct
{
	OBJID	idMember;				//��ԱID
	union{
		DWORD	usMainJob;			//��ְҵ
		DWORD	dwMaxScore;			//ս����
	};
	union{
		DWORD	usSubJob;			//��ְҵ
		DWORD	dwExitTime;			//����ʱ��
	};
	USHORT	usLev;					//�ȼ�
	USHORT	usMainLev;				//��ְҵ�ȼ�
	USHORT	usSubLev;				//��ְҵ�ȼ�
	USHORT	usLookFace;				//ͷ��
	UINT	nImpart;				//���ڶ�/��������
	UCHAR	ucVipLev;				//VIP�ȼ�
	UCHAR	ucInTeam;				//�Ƿ��ж���/�Ա�
	UCHAR	ucLine;					//��·
	UCHAR	ucRelation;				//����ҹ�ϵ
	char	szName[_MAX_NAMESIZE];	//��Ա����
} ST_MEMBERINFO;

class CMsgSchoolMember : public CNetMsg  
{
public:
	CMsgSchoolMember();
	virtual ~CMsgSchoolMember();
public:
	typedef struct
	{
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
			
		OBJID		idUser;
		USHORT		ucAction;			// action
		UCHAR		ucAmount;			// ����			//ÿҳ��ʾ����
		UCHAR		ucPage;
		ST_MEMBERINFO	setMember[1];	// ��Ա�б�
	}MSG_Info;
	MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGSCHOOLMEMBER_H__9F1EABD9_F12D_45F7_A088_F99E49BACEB4__INCLUDED_)