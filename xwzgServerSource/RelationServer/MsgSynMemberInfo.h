// MsgSynMemberInfo.h: interface for the CMsgSynMemberInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSYNMEMBERINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
#define AFX_MSGSYNMEMBERINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "windows.h"
enum OnlineStatusChange
{
	SynMemberOffline,
	SynMemberOnline,
};

enum MsgSynMemberInfoAction
{
	SynMemberAdd=4,///��һ����Ա
	SynMemberRemove,///ɾһ����Ա
	SynMemberAnswerOK,///��׼���
	SynMemberAnswerNO,///�ܾ����
	SynMemberKickout,///�߳�����
	SynMemberSetRank,///�߳�����
	SynMemberSetAnnounce,///���¹���
	SynMemberOnlineStatusChange,///��Ա����״̬�ı�,����1Ϊ����,0Ϊ����
// 	raRoleOffline,///�������
// 	raQueryRole_Friend,///��ѯ��Һ���
// 	raQueryRole_Syndicate,///��ѯ��Ҿ���
// 	raQueryRole_Marry,///��ѯ��һ���
};

class CMsgSynMemberInfo : public CNetMsg  
{
public:
	CMsgSynMemberInfo();
	virtual ~CMsgSynMemberInfo();

	BOOL	Create		(USHORT nAction,USHORT nPage,USHORT nMaxAmount);
	BOOL	Append		(OBJID idUser,USHORT level,USHORT firJob,USHORT firJobLev,USHORT rank,DWORD proff,const char* szName,int nOnline);
	BOOL	Create		(char* pMsgBuf, DWORD dwSize);
	void	Process		(void* pInfo);

//protected:
	struct MemberInfo
	{
		USHORT level;
		USHORT firJob;
		USHORT firJobLev;
		USHORT rank;
		OBJID idUser;
		USHORT proff;
		USHORT online;
		char szName[16];
	};

	typedef struct {
		unsigned short	unMsgSize;
		unsigned short	unMsgType;

		UCHAR	ucActiom;
		UCHAR	ucMaxAmount;
		UCHAR	ucAmount;
		UCHAR	ucPage;
		MemberInfo memInfo[1];
	}MSG_Info;

	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGSYNMEMBERINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
