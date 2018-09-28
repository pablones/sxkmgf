// Team.h: interface for the CTeam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEAM_H__096C275F_D6F9_40F1_8B39_471D69EBA7CC__INCLUDED_)
#define AFX_TEAM_H__096C275F_D6F9_40F1_8B39_471D69EBA7CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include <windows.h>
#include "Myheap.h"

#include <vector>
#include <algorithm>
#include "../share/protocol.h"
using namespace std;


const int   TUTOR_TEAM_ADD_EXP_PERSENT   	= 1;


const int	_MAX_TEAMAMOUNT			= 6;			//-------���---2007/04/24-----�޸ĳ��������Ϊ5

const int	_MAX_TEAMAMOUNTOUT		= 8;// ���������// By Minsky,2010-05-28

//[��;���� 2009.03.09]�޸����ɆT�ж�������40��
//const int	_RANGE_EXPSHARE			= 32 + 360;		//  [2007-8-16] ��� ��32��Ϊ52,//  [2007-8-21] ��� ��ӵ���Ч��Χ�����л���������20������20*18=360�񣩡�
const int	_RANGE_EXPSHARE			= 40;	
const int	_RANGE_TEAM_STATUS		= 12;			// ���״̬Ӱ��ķ�Χ�뾶
const int	MAX_TEAM_EXP_TIMES		= 360;			// ��Ӿ���ֵ�ĵȼ������
//---jinggy---����ϵͳ---begin
//const int	TEAMLEADER_WITHNEWBIE_REPUTEINC = 100;		//���ͼ���������1Сʱ��������100
//---jinggy---����ϵͳ---end

enum {
	CHANGETEAM_ADD=1,
	CHANGETEAM_DEL,
	CHANGETEAM_DISMISS,
	CHANGETEAM_INFO,
	CHANGETEAM_FLY,
	CHANGETEAM_CHANGENAME,//20070202����:С�Ӹ���
	CHANGETEAM_CHANGLEADER,//20070205����:�ӳ����
	CHANGETEAM_CHANGLEADER_CLSEFFECT,//20070205����:�ӳ����.ȥ��ԭ�ӳ���Ч
	CHANGETEAM_CHANGLEADER_SETEFFECT,//20070205����:�ӳ����.����¶ӳ���Ч
	CHANGETEAM_CHANGCONFIG,//20070207����:С�����ñ��.
	CHANGETEAM_UPDATE_BROADCAST_SET,	//  [2007-9-27] ���	����״̬
	CHANGETEAM_QUERY_MEMINFO,      //��ѯ��Ա��Ϣ
	CHANGETEAM_TEAMTEXT,           //���Ķ�����Ϣ
	CHANGETEAM_PICKMONEY,          //�ֵ����Ѽ�Ľ�Ǯ
	CHANGETEAM_CREATE,
	CHANGETEAM_DES,
	CHANGETEAM_ONLINESTATUSCHANGE,
	CHANGETEAM_SETLOCATION,//��Ա����״̬�ı�
	CHANGETEAM_REQUESTCHANGLEADER,//�������������ӳ����,ֻ���ڶӳ����ڵ�ͼ�������˵�����²ŷ���
	CHANGETEAM_CLRLOCATION
};

//20070206����:����ṹ��.����ͼ��
//int							m_nTeamMode;
//int							m_nExpMode;
//int							m_nItemMode;
//char						m_sTeamName[24];
//struct TeamInfoStruct{
//	OBJID	id;
//	OBJID	idLeader;
//	OBJID	setMember[_MAX_TEAMAMOUNT];
//	BOOL	bCloseMoney;
//	BOOL	bCloseItem;
//	int     nLeaderlev;
//	//------------------
//	int							nTeamMode;
//	int							nExpMode;
//	int							nItemMode;
//	char						sTeamName[24];
//	//------------------
//
//	//------------------
//	char                        m_sLeadName[16];
//	int                         m_nLev;
//	int                         m_nTarget;
//	char                        m_sTeamText[96];
//};

class CNetMsg;
class IRole;
class CTeam  
{
public:
	CTeam(PROCESS_ID idProcess);
	virtual ~CTeam();

public:
	static	CTeam*		CreateNew		(PROCESS_ID idProcess, OBJID m_idTeam, OBJID idLeader,bool bSend);
	static	CTeam*		CreateNew		(PROCESS_ID idProcess, TeamInfoStruct* pInfo);
	ULONG	ReleaseByOwner()	{ delete this; return 0; }
	OBJID	GetID()				{ return m_id; }
	void	GetInfo(TeamInfoStruct* pInfo);
	void	Destroy				();

public:
	BOOL	AddMember			(OBJID idMember, bool bOwnerMap = true);
	BOOL	DelMember			(OBJID idMember);
	void	Dismiss				(OBJID idLeader);
	void    SetOnlineStatus(OBJID idMember,BOOL bStatus,BOOL bTransmitMsg=false);///�ı��Ա����״̬,0Ϊ����,1Ϊ����
	void    SetLocation(OBJID idMember,OBJID idMap,int x,int y,BOOL bTransmitMsg=false);///����ʱ����һ�¶�Ա��λ��
	void	ResetLocation(BOOL bTransmitMsg);
	void    ResetOfflineLocation(BOOL bTransmitMsg=false);///���������߶�Ա�ı���ĸ�����ͼλ������Ϊ0
	//20070202����:С�Ӹ���
	void	ChangeTeamName(char *teamName);
	//20070203����:�ӳ����
	void	ChangeTeamLeader(OBJID leaderID);
	//20070205����:�ӳ�����.�����öӳ�.��������
	void	SetTeamLeaderOnly(OBJID leaderID);
	void    SetTeamLeaderName(const char *leaderName);
	//20070207����:�޸�����
	void	ChangeTeamInfo(int teamInfo);
	void	ChangeTeamInfoOnly(int teamInfo);//�����޸�
	void    ChangeTeamText(char *pText);

	void	Open				();
	void	Close				();
	BOOL	IsClosed			();
	void	OpenMoney			();
	void	CloseMoney			();
	BOOL	IsCloseMoney		()			{ return m_bCloseMoney; }
	void	SetCloseMoney		(bool flag)	{ m_bCloseMoney = flag; }
	void	OpenItem			();
	void	CloseItem			();
	BOOL	IsCloseItem			()			{ return m_bCloseItem; }
	void	SetCloseItem		(bool flag)	{ m_bCloseItem = flag; }
	void	OpenGemAccess		();
	void	CloseGemAccess		();
	BOOL	IsCloseGemAccess	()			{ return m_bCloseGemAccess; }
	void	SetCloseGemAccess	(bool flag)	{ m_bCloseGemAccess = flag; }

	OBJID	GetMemberByIndex	(int nIndex);
	int		GetMemberAmount		();
	int		GetOnlineMemberAmount(int nSex = 2 /* 2��������������0���У�1��Ů*/);
	bool	GetMemberInfo		(TeamMemberInfo& menberInfo,OBJID idUser);
	bool    IsMemberOnline(OBJID idUser);
	bool	UpdateMemberInfo();

	void	DelBuffByTutor( OBJID UserID );// pengfeng ���ʦͽ��BUFF [10/26/2007]
	void	AddBuffByTutor( OBJID UserID );// pengfeng ���ʦͽ��BUFF [10/26/2007]
	void	DelBuffByTutorOnDisband();//�ӳ��Ѷ����ɢɾ���ض�BUFF// pengfeng [10/26/2007]
	OBJID   GetTeamMemberOnly();///���õ���Ա�Ƕӳ�
	OBJID   GetOnlineTeamMemberOnly();///���õ����߳�Ա�Ƕӳ�
	OBJID   GetNextTeamLeader();///���õ����߷Ƕӳ���Ա,�����ڸ�����
	BOOL	IsTeamMember		(OBJID idMember);
	BOOL	IsTeamWithNewbie	(OBJID idKiller, IRole* pTarget);
	BOOL	IsTeamWithTutor		(OBJID idKiller, IRole* pTarget);
	BOOL	IsAccountTeamMember	(OBJID idAccount);///[2009.2.12--�Ŷ�]���˺ŵ��û��Ƿ��Ƕ�Ա

	void	SetLeader			(OBJID idLeader);
	OBJID	GetLeader			();

	// ���ڽ��ϵͳ��
	void	AttachMemberStatus	(int nStatus, int nPower, int nSecs, int nTimes, OBJID idExclude);
	void	AdjustMemberStatus	(int nOldAmount, int nNewAmount);
	int		ShareDamage			(int nDamage, OBJID idExclude);
	bool	CheckActive(CUser* pUser);
// 	void	BroadcastTeamMsg	(CNetMsg* pMsg, OBJID idExclude=ID_NONE);
	void	BroadcastTeamMsg	(CNetMsg* pMsg, CUser* pSender=NULL,bool bSendOtherLine = true);
	void	BroadcastMemberLife	(CUser* pMember, bool bMaxLife=false);
	void	BroadcastMemberMana(CUser* pMember, bool bMaxMana);
	void	BroadcastMemberLev(CUser* pMember, bool bLev=true);
	//void	BordcastTeamMsg		(int nChannel,const char* pMsg);
	OBJID	GetTeamMap() const { return m_idTeamMap; }
	void	SetTeamMap(OBJID val,BOOL bTransmitMsg=false); 
public: // call by transmit from other mapgroup
	int     GetTeamAddExp(int nExp, int nMemberCount);
	bool    DevolveOnLeader(int newLeaID);
//	void DealTeamLeaderInfo();
	void	AddMemberOnly		(TeamMemberInfo menberInfo);
	void	DelMemberOnly		(OBJID idMember);
	//061124hunter
	void    SetTeamTarget(int nTarget){ m_nTarget = nTarget;}
	int     GetTeamTarget(){ return m_nTarget;}
	void    SetTeamLev(int nLev){ m_nLev = nLev;}
	int     GetteamLev(){ return m_nLev;}
	void    SetTeamText(char *text){ strcpy(m_sTeamText, text);}
	char*   GetTeamText(){ return m_sTeamText;}
	void	SetTeamMode(int mode){	m_nTeamMode = mode; }
	int		GetTeamMode()	{	return	m_nTeamMode; }
	int		GetMemberLev();
	void	SetExpMode(int mode){	m_nExpMode = mode; }
	int		GetExpMode()	{	return	m_nExpMode; }
	void	SetItemMode(int mode){	m_nItemMode = mode; }
	int		GetItemMode()	{	return	m_nItemMode; }
	void	SetTeamName(char*name){strcpy(m_sTeamName,name);}
	char*	GetTeamName()	{return m_sTeamName;}
	char*   GetTeamLeaderName() {return m_sLeadName;}
	map<OBJID,bool>& GetShareStatusSet(){return m_setUserStatus;}
	void    AwardBufForTeacher();// ����ӵ�ʦͽ��BUF [�½���10/26/2007]
	TeamMemberInfo              m_TeamIn[6];
private:
	BOOL	Create				(OBJID idLeader,OBJID m_idTeam,bool bSend);
	void	Init				();
	CTeam*	CloneNew();

protected:
	typedef	vector<OBJID>		USERIDSET;
	USERIDSET					m_setIdUser;
	USERIDSET					m_setAccountIdUser;///[2009.2.12--�Ŷ�]�����Ա���˺�ID
	map<OBJID,bool>				m_setUserStatus;///ָʾ������Ƿ��ھ��鹲����

private:
	OBJID						m_id;
	OBJID						m_idLeader;
	PROCESS_ID					m_idProcess;
	BOOL						m_fClosed;
	BOOL						m_bCloseMoney;
	BOOL						m_bCloseItem;
	BOOL						m_bCloseGemAccess;
	int							m_nTeamMode;
	int							m_nExpMode;
	int							m_nItemMode;
	char						m_sTeamName[24];
	CTimeOut					m_tGuidingTime;  //����ʱ��
	char                        m_sLeadName[16];
	int                         m_nLev;
	int                         m_nTarget;
	char                        m_sTeamText[96];

	bool                        m_bEudemonBron;
	OBJID                       m_bronUser[2];
	OBJID                       m_bronEudemon[2];
	bool                        m_bEudemonBronLock[2];

	bool                        m_bEudemonPlay;
	OBJID                       m_playUser[2];
	OBJID                       m_playEudemon[2];
	bool                        m_bEudemonPlayLock[2];

	OBJID						m_idTeamMap;///�ӳ����ڵ�ͼid
//	TeamMemberInfo              m_TeamOut[6];

protected: // ctrl
	MYHEAP_DECLARATION(s_heap)

// By Minsky,2010-05-28������Ϊ����ӵ���ӵ������Ա�Ĺ���
private:
	USERIDSET					m_setIdApply;// By Minsky,2010-05-30,�����еĶ�Ա���б�
public:
	void	AddMemberApply		(OBJID idMember);
	bool	DelMemberApply		(OBJID idMember);
	OBJID	GetMemberApplyByIndex(int nIndex);
	int		GetMemberApplyAmount		();

	bool    initEudemonBron();
	bool    initEudemonPlay();
	bool    EudemonBronChange(OBJID idUser,OBJID idEudmon);
	bool    EudemonBronChange(OBJID idUser,bool idLock);
	bool    EudemonBron();
	bool    EudemonCancel();
	bool	EudemonPlayChange(OBJID idUser,OBJID idEudmon);
	bool	SendEudemonBronInfo(OBJID idUser,OBJID idUserTarget = ID_NONE);
	bool	SendEudemonPlayInfo(OBJID idUser,OBJID idUserTarget = ID_NONE);
	bool	EudemonPlayChange(OBJID idUser,bool idLock);
	bool	EudemonPlayCancel();
	bool	EudemonPlay();
	bool	CheckTutorActive(CUser* pLeader);//�鿴������ͽ���Ƿ��ڸ���
	bool    CheckIsSameSyn(OBJID idsyn);
};

#endif // !defined(AFX_TEAM_H__096C275F_D6F9_40F1_8B39_471D69EBA7CC__INCLUDED_)
