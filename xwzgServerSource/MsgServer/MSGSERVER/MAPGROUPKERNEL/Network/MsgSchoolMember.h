// MsgSchoolMember.h: interface for the CMsgSchoolMember class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSCHOOLMEMBER_H__82E6220E_E387_4CA2_A66E_51466E54A627__INCLUDED_)
#define AFX_MSGSCHOOLMEMBER_H__82E6220E_E387_4CA2_A66E_51466E54A627__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "../TutorList.h"

enum {
	MESSAGEBOARD_DELMEMBER		= 1, //ɾ��
	MESSAGEBOARD_ADDMEMBER		= 2, //���
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

	MESSAGEBOARD_DELAPPLYLIST,	//ɾ��ͽ�������б��е�ͽ��
	MESSAGEBOARD_DELSTU,		//ɾ��ͽ���б��е�ͽ��
	MESSAGEBOARD_CALLSTU,		//�ٻ�ͽ��
	MESSAGEBOARD_CALLSTUREF,	//ͽ����Ӧ�ٻ�
	MESSAGEBOARD_GETGIFT,		//ʦ����ȡͽ�ܵĳɳ�����
	MESSAGEBOARD_HASGRADUATE,	//�Ƿ�ӵ�г���ʦ��ͽ��
	MESSAGEBOARD_GRADUATESTU,	//�����ѳ�ʦ��ͽ���б�
	MESSAGEBOARD_GRADUATEREF,	//ͽ����Ӧ��ʦ

	MESSAGEBOARD_TEAMADD,		//ʦͽ��ӽ���ʦͽ��ϵ
	MESSAGEBOARD_TEAMADDREF,	//ͽ����Ӧ��ʦ

	MESSAGEBOARD_QUESTION,		//ʦͽ���� ����
	MESSAGEBOARD_ANSWERQUESTION,//ʦͽ����
};

enum {
	RELATION_SELF	=	0,
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
	LINEBROAD_APPLYBYNAMEREF= 13,//���Ͱ�ʦ����ķ���
};
// typedef struct
// {
// 	OBJID	idTutor;	// ʦ��ID
// 	OBJID	idMember;				// ��ԱID
// 	char	szName[_MAX_NAMESIZE];	// ��Ա����
// 	char	szMate[_MAX_NAMESIZE];	// ��ż����
// 	UCHAR	ucLevel;				// �ȼ�
// 	UCHAR	ucProfession;			// ְҵ���
// 	USHORT	usPkValue;				// PKֵ
// 	UCHAR	ucNobilityRank;			// ��ݣ���λ��
// 	UCHAR	ucRelation;				// ����ҹ�ϵ
// 	UCHAR	ucStatus;				// ״̬ ���� �����ߡ�����
// 	DWORD	dwSynID_Rank;			// ����ID & ְλ (��MsgPlayerһ������8λΪְλ��ţ���24λΪ����ID)
// 	UINT    ucSex;
// 	UINT    uApplyType;             //��������
// 	int     nDensity;               //���ܶ�
// 	char    delTime[32];            //����ɾ��ʱ��
// } ST_MEMBERINFO;
//goto �޸Ľṹ
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
	BOOL	Create	(OBJID idUser, UCHAR ucAction, ST_MEMBERINFO* pMember=NULL, UCHAR ucAmount=0, UCHAR ucPage=0);
// 	BOOL	Append	(UCHAR ucRelation, UCHAR ucStatus, OBJID idMember, const char* pszName = NULL,OBJID idTutor = ID_NONE);
// 	BOOL	Append	(UCHAR ucRelation, UCHAR ucStatus, OBJID idMember, UCHAR ucProfession,UCHAR	ucLevel, UINT ucSex, const char* pszName = NULL);
	BOOL	Append(ST_MEMBERINFO stMember);
	BOOL	Append(TUTORLIST stInfo);
	BOOL	Append	(UCHAR ucRelation, CUser* pUser);
	BOOL	Create	(char* pMsgBuf, DWORD dwSize);
	void	Process	(CGameSocket* pSocket);

public:
	typedef struct
	{
		MSGHEAD_DEFINE

		OBJID		idUser;
		USHORT		ucAction;			// action
		UCHAR		ucAmount;			// ����			//ÿҳ��ʾ����
		UCHAR		ucPage;
		ST_MEMBERINFO	setMember[1];	// ��Ա�б�
	}MSG_Info;
	MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGSCHOOLMEMBER_H__82E6220E_E387_4CA2_A66E_51466E54A627__INCLUDED_)
