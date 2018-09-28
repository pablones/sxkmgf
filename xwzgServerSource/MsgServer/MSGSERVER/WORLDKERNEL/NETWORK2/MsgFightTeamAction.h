// MsgFightTeamAction.h: interface for the CMsgFightTeamAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGFIGHTTEAMACTION_H__49F9023E_2AE8_4B39_9BDD_4F28F600F45F__INCLUDED_)
#define AFX_MSGFIGHTTEAMACTION_H__49F9023E_2AE8_4B39_9BDD_4F28F600F45F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AllMsg.h"

WORLDKERNEL_BEGIN
enum FightTeamAction
{
	FightTeamAction_Create,        //����ս��
		FightTeamAction_Destroy,       //ɾ��ս��
		FightTeamAction_InviteMember,  //�����Ա
		FightTeamAction_Refruse,       //����ܾ�
		FightTeamAction_OK,            //����ɹ�
		FightTeamAction_ChangeLeader,  //�����ӳ�
		FightTeamAction_DelMember,     //ɾ������
		FightTeamAction_ExitTeam,      //�˳�����
		FightTeamAction_OnLine,        //�Ƿ�����
		FightTeamAction_Query,         //��ȡս����Ϣ
		FightTeamAction_InviteEnterInstace,  //�������FB
		FightTeamAction_ExitInstace,		//�˳�������
		FightTeamAction_IsInArena,
		FightTeamAction_UpdateTeamInfo,
		FightTeamAction_LeaveTeamOK,
};

class CMsgFightTeamAction : public CNetMsg  
{
public:
	CMsgFightTeamAction();
	virtual ~CMsgFightTeamAction();

	BOOL	Create(int nType, const char* pszFightTeamName, const char* pszMemberName,DWORD dwData,DWORD dwType=0);
	
public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void* pInfo);
	
private:
	typedef struct{
		MSGHEAD_DEFINE
		

		unsigned short ucFightTeamType; //��������
        DWORD  dwData;  //ս�����ͻ����û�ID
        DWORD  dwType;
        char szBuf[1]; //���ڼ�¼ս�����ֺͳ�Ա����
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
private:
	CNetStringPacker	m_StrPacker;
};
WORLDKERNEL_END
#endif // !defined(AFX_MSGFIGHTTEAMACTION_H__49F9023E_2AE8_4B39_9BDD_4F28F600F45F__INCLUDED_)
