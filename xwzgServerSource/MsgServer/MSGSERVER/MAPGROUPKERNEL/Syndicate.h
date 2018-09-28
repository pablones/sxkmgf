// Syndicate.h: interface for the CSyndicate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNDICATE_H__78C1D98F_7DA8_482D_9819_22AD24402CB6__INCLUDED_)
#define AFX_SYNDICATE_H__78C1D98F_7DA8_482D_9819_22AD24402CB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "../share/NetWorkDef.h"
#include "../share/protocol.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"
#include <vector>
#include "MsgSynInfo.h"
#include "TimeOut.h"
#include "math.h"
#include "..\SHARE\define.h"	// Added by ClassView
using namespace std;

const int	SYN_MONEY_BASELINE		= 0;					// ���ɻ������ڸ�ֵ�������ư��ɹ���

//////////////////////////////////////////////////////////////////////
enum {
		SYNFUNC_DEMISE=1,
		SYNFUNC_SETTENET,
		SYNFUNC_SETLEADERTITLE,
		SYNFUNC_SETMEMBERTITLE,
		SYNFUNC_ADDDATA,
		SYNFUNC_SETDATA,
		SYNFUNC_PRESENT,
		SYNFUNC_ADD_MEMBER,
		SYNFUNC_DEL_MEMBER,
		SYNFUNC_UP_LEVEL,			//���ų�Ա����ȼ�����
		SYNFUNC_SETSYNNAME,
		SYNFUNC_SET_RANK,
		SYNFUNC_PAYOFF,//20070414����:������
		SYNFUNC_SET_MEMBERPROFFER,//20070417����:ͬ�������̳߳�Ա����ֵ
		SYNFUNC_LOGIN,//[��;���� 2008/9/25]��ҵ�½ͨ��
		SYNFUNC_LOGOUT,//[��;���� 2008/9/25]����Ƴ�ͨ��
		SYNFUNC_LEAVERANK,
		SYNFUNC_SYNATTACKID,
		SYNFUNC_SYNDEFENCEID,
		SYNWAR_DEC,
		SYNWAR_JOIN,
		SYNWAR_RELULT,
		SYNWAR_CLEAR,
};

//////////////////////////////////////////////////////////////////////
const int	MAX_SYNENEMYSIZE		= 5;		// һ��5���ж԰��ɿռ�
const int	MAX_SYNALLYSIZE			= 5;		// һ��5�����˰��ɿռ�
enum SYNDATA{
		SYNDATA_ID=0,			// ������INSERT
		SYNDATA_NAME=1,			// ������
		SYNDATA_ANNOUNCE,		// ���ɹ���
		SYNDATA_LEADERID,		// ����ID
		SYNDATA_LEADERNAME,		// ��������
		SYNDATA_CREATELEADERNAME,	// ������������
		SYNDATA_CREATETIME,
		SYNDATA_STATE,
		SYNDATA_LEVEL,			//���õȼ�
		SYNDATA_SUBLEV1,//����һ�ȼ�
		SYNDATA_SUBLEV2,//���ö��ȼ�
		SYNDATA_SUBLEV3,//�������ȼ�
		SYNDATA_SUBLEV4,//�����ĵȼ�
		SYNDATA_GROW,
		SYNDATA_SUBGROW1,
		SYNDATA_SUBGROW2,
		SYNDATA_SUBGROW3,
		SYNDATA_SUBGROW4,
		SYNDATA_MONEY,
		SYNDATA_AMOUNT,
 		SYNDATA_ACTIVEAMOUNT,
		SYNDATA_DATA1,
		SYNDATA_SUB1_SKILL1,
		SYNDATA_SUB1_SKILL2,
		SYNDATA_SUB1_SKILL3,
		SYNDATA_SUB2_SKILL1,
		SYNDATA_SUB2_SKILL2,
		SYNDATA_SUB2_SKILL3,
		SYNDATA_SUB3_SKILL1,
		SYNDATA_SUB3_SKILL2,
		SYNDATA_SUB3_SKILL3,
		SYNDATA_SUB4_SKILL1,
		SYNDATA_SUB4_SKILL2,
		SYNDATA_SUB4_SKILL3,
		SYNDATA_SUB1_BUILDER,
		SYNDATA_SUB2_BUILDER,
		SYNDATA_SUB3_BUILDER,
		SYNDATA_SUB4_BUILDER,
		SYNDATA_SUB1_KONGFU,
		SYNDATA_SUB2_KONGFU,
		SYNDATA_SUB3_KONGFU,
		SYNDATA_SUB4_KONGFU,
		SYNDATA_SUB1_BUSMAN,
		SYNDATA_SUB2_BUSMAN,
		SYNDATA_SUB3_BUSMAN,
		SYNDATA_SUB4_BUSMAN,
		SYNDATA_SUB1_START,
		SYNDATA_SUB2_START,
		SYNDATA_SUB3_START,
		SYNDATA_SUB4_START,
		SYNDATA_PAY,
		SYNDATA_MAPID,
};
char	szSynTable[];
typedef	CGameData<SYNDATA, szSynTable, szID>	CSynBase;

const int   DISMISSSYN_MONEY_LIMIT  = 100000;           // ��Ǯ���ڴ������Զ���ɢ��//add huang 2004.1.5
const int	DISMISSSYN_AMOUNT_LIMIT	= 20;				// �������ڴ������Զ���ɢ  
const int   DISMISSSYN_TIME_LIMIT   = 24*60*60;         // �Զ���ɢ����ʱ�� 24Сʱ

const int	AUTOCHECK_TIME			= 30;	//��⽵�����ɢ��ʱ������,30��

const int   SYNWARTYPE_NONE         = 0;    //��
const int   SYNWARTYPE_ATTACK       = 1;    //��ս����
const int   SYNWARTYPE_DEFENCE      = 2;    //��ս����

const int   CITYSYNWAR_NONE         = 0;    //������
const int   CITYSYNWAR_CITYWAR      = 1;    //��ս����
const int   CITYSYNWAR_SYNWAR       = 2;    //��ս����

const int SPENDBUILD = 1;
const int SPENDACTIV = 2;
const int SPENDMONEY = 3;

//////////////////////////////////////////////////////////////////////
// class ISynModify
// {
// public:
// 	virtual bool	Demise(OBJID idOldLeader,int nOldSynMemberLevel,OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave=true)		PURE_VIRTUAL_FUNCTION_0
// 	virtual bool	SetTenet(LPCTSTR szTenet, bool bSave=true)													PURE_VIRTUAL_FUNCTION_0
// 	virtual bool	SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave=true)										PURE_VIRTUAL_FUNCTION_0
// 	virtual bool	SetMemberTitle(LPCTSTR szMemberTitle, bool bSave=true)										PURE_VIRTUAL_FUNCTION_0
// 	virtual bool	AddData(SYNDATA idx, int nData, bool bSave=true)											PURE_VIRTUAL_FUNCTION_0
// 	virtual bool	SetData(SYNDATA idx, int nData, bool bSave=true)											PURE_VIRTUAL_FUNCTION_0
// 	virtual bool	PresentMoney(OBJID idSyn, int nMoney, bool bSave=true)										PURE_VIRTUAL_FUNCTION_0
// 	virtual bool	SetSynName(LPCTSTR szSynName, bool bSave=true)												PURE_VIRTUAL_FUNCTION_0
// 	//20070414����:������dwMoneyΪ��Ǯ��
// 	virtual bool	PayOff(DWORD dwMoney,BOOL bSave=true)														PURE_VIRTUAL_FUNCTION_0
// 	//20070417����:ͬ�������߳��ڳ�Ա����ֵ
// 	virtual bool	SetMemberProffer(int dwProffer,OBJID idMember,BOOL bSave=true)								PURE_VIRTUAL_FUNCTION_0
// };
typedef struct	MemberInfo{
	OBJID	id;
	char	szName[16];	
	USHORT	level;
	USHORT	firJob;
	USHORT	sex;
	USHORT	rank;
	USHORT  line;
	USHORT	vip;
	OBJID   lastlogin;
	OBJID   hiscon;
	OBJID   hismoney;
	OBJID   hisgrow;
	OBJID	maxpower;

}MEMBERINFO;
enum 
{
	MemberList=5,
		ConList,
		QueryList,
		AddMember=8,
		DelMember=9,
		AddQuery=10,
		DelQuery=11,
		SetRank= 12, 
};
//////////////////////////////////////////////////////////////////////
// ���г�Ա����Ϊԭ�Ӳ�����
class CSyndicate : public CGameObj/*, ISynModify*/
{
protected:
	CSyndicate();
	virtual ~CSyndicate();
	vector<OBJID> m_SynMonsterList;
public:
	static CSyndicate* CreateNew() { ASSERT(!"CSyndicate::CreateNew()"); return NULL; }				// ����CSynPtr
	static CSyndicate* CreateNew(PROCESS_ID idProcess)	{ CSyndicate* ptr=new CSyndicate; if(ptr) ptr->m_idProcess=idProcess; return ptr; }
	virtual ULONG	ReleaseByOwner()				{ delete this; return 0; }
	bool	Create(IRecordset* pRes);
	bool    Create(CMsgSynInfo& msg);
	bool	Create(CreateSyndicateInfo* pInfo,bool bSave);
	void	CreateSynMap();
	void	SetInSynMapID();

//	ISynModify*		QueryModify()	{ return (ISynModify*)&m_obj; }
	bool			ChangeSyndicate( const SynFuncInfo0 * pFuncInfo);
	bool			ReloadData();

	int				GetKill() { return m_warKill; }
	void			SetKill(int kill) { m_warKill = kill; }
	void	DelSynMonster();
	void	AddSynMonster(CMonsterPtr pMonster);
	void CSyndicate::Lock()
	{
		///δ����״̬ʱ��
		m_bLock=true;
	}
	
	bool CSyndicate::HasLock()
	{
		///δ����״̬ʱ��
		return m_bLock;
	}
	void CSyndicate::Unlock()
	{
		m_bLock=false;
	}
	bool CanUpLevel(int nSub,CUserPtr pUser=NULL);
	int				GetState() { return warState; }
	void			SetState(int state) { warState = state; }
	LPCTSTR			GetRankTitle(int Rank);
public: // const
	OBJID	GetID()							{ return m_pData->GetKey(); }
	int		GetInt(SYNDATA idx);
	LPCTSTR	GetStr(SYNDATA idx)				{ return m_pData->GetStr(idx); }
	void	SetInt(SYNDATA idx,int nData)	{ CHECK(m_pData);if(nData == GetInt(idx) )return;m_pData->SetInt(idx,nData);}
	void	BroadcastSynMsg(CNetMsg* pMsg, CUser* pExcludeSender=NULL);
	void	BroadcastSubSynMsg(SOCKET_ID idSocket, OBJID idNpc, LPCTSTR szMsg, CUser* pExcludeSender=NULL, LPCTSTR pszEmotion=NULL, DWORD dwColor=0xffffff, int nAttribute=_TXTATR_SYNDICATE);
	void	BroadcastLocalSynMsg(CNetMsg* pMsg, CUser* pExcludeSender=NULL);
	LPCTSTR	GetFealtyName();
	bool	IsNearSyn(OBJID idSyn);
	CSyndicate*		GetMasterSyn();
	bool	IsFriendly(OBJID idSyn, BOOL bNest=true);		// mast be true
	bool	IsHostile(OBJID idSyn, BOOL bNest=true);		// mast be true
	bool	IsNeutral(OBJID idSyn);
	bool	IsWhiteSynMapOwner();
	bool	IsBlackSynMapOwner();
	int		GetMemberAmount(bool bIncludeSub = true);
	int		GetShopMemberAmount(bool bIncludeSub = true);
	int		GetQueryMemberAmount(bool bIncludeSub = true);
	int		GetSynAmount();
	bool    ProcAutoDismissOrDestory();
	void	ProcAutoSynWar();
	//////�¹���--jay
	void	SetSynMapID(OBJID idmap){CHECK(idmap); m_nMapID=idmap;}
	OBJID	GetSynMapID(){return m_nMapID;}
	void	TransmitWorldMsg(CNetMsg* msg);
	void	SendMenberListToClient(CUser* pUser);
	void    SendConListToClient(CUser* pUser,int nPage = 1);
	void    SendQueryListToClient(CUser* pUser,int nPage = 1);
	void	GetMemberList(vector<CUserPtr> &User);
	void	ReSetSynName(const char* szName);
	void	ReSetSynMemberName(OBJID nUserid,const char* szName);
	void	SetCallTask(bool Task){m_bCallTask=Task;}
	bool	GetCallTask(){return m_bCallTask;}
private:
	OBJID m_nMapID;
protected:
	bool    CheckDismissTime();    // add huang 2004.1.7
	bool    CheckDismissSyn();  
	int		AmontStatue;
	int		MaintainSta;


public: // application
//	void	SendQueryMenberToClient(CUser* pUser,int nPage = 1);
	void	SendInfoToClient(CUser* pUser);
	void	SendAllInfoToClient(CUser* pUser);
	void	SendSynInfoToClient(CUser* pUser);
//	void	SendMenberToClient(CUser* pUser,int nPage = 1);
	void    BroadcastSynMemberInfoMsg(OBJID idMember);
	void    BroadcastSynMemberStatusChangeMsg(OBJID idMember);

	void	SetNewSubName(LPCTSTR szName);		// ��ʱ����
	
	bool	ClearAllySyn(CSyndicate* pTargetSyn);
	bool	ClearEnemySyn(CSyndicate* pTargetSyn);

public: // link -------------------------------------
	CAutoLink<CSyndicate>&	QueryLink	()					{ return m_link; }
protected:
	CAutoLink<CSyndicate>	m_link;

public: // syn war -------------------------------------
	void	AddSynWarScore(OBJID idNpc, int nScore);
	int		GetSynWarScore(OBJID idNpc);
	void	ClearSynWarScore(OBJID idNpc);
protected:
	struct	SynWarInfo{
		OBJID	idNpc;
		int		nScore;
	};
	typedef	vector<SynWarInfo>	SYNWAR_SET;
	SYNWAR_SET		m_setSynWarScore;
	int             warState,warTurn;
	int             m_warKill;
protected: // modify, mast be atom operator. test & set
	// bSave ����Ϊtrue�����򲻻����
	virtual bool	AddData(SYNDATA idx, int nData, bool bSave);
	virtual bool	Demise(OBJID idOldLeader,int nOldSynMemberLevel,OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave);
	virtual bool	SetTenet(LPCTSTR szTenet, bool bSave);
	virtual bool	SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave);
	virtual bool	SetMemberTitle(LPCTSTR szMemberTitle, bool bSave);
	virtual bool	SetData(SYNDATA idx, int nData, bool bSave);
	virtual bool	PresentMoney(OBJID idSyn, int nMoney, bool bSave);
	virtual bool	SetSynName(LPCTSTR szSynName, bool bSave);
	virtual bool	PayOff(DWORD dwMoney,BOOL bSave/* =true */);//20070414����:������.�������ڴ�.


public:
	int GetMoney();
	void  SetSynWar(OBJID warID, bool bSave = true);
//	void  SetRepute(int data, bool bSave = true) {SetData(SYNDATA_REPUTE,data,bSave);}
	void  SetMoney(int data, bool bSave = true) {SetGrow(SPENDMONEY,data,bSave);}
	bool			SetLastBonusTime( LPCTSTR szTime );
//	LPCTSTR			GetLastBonusTime()					{ return GetStr( SYNDATA_LASTBONUSTIME ); }
	//20070331����:�����������-------------------------
	//����Set...()���������̻߳ص�ʱ����.������update.����update�����������߳����
	//��Ҫͬ��������ͼ��ʱ,����ReqSet...()����
	BOOL			ReqSetTaskPublishTime(DWORD curTime);//���������߳�,�ص�ʱ�����޸��ҽ��޸ı���ͼ���ڴ�����
//	BOOL			SetTaskPublishTime(DWORD curTime,BOOL bUpdate=UPDATE_NEVER);//�����̻߳ص�,���þ������񷢲�ʱ��
//	DWORD			GetTaskPublishTime(){return GetInt(SYNDATA_TASKPUBLISHTIME);}//��þ������񷢲�ʱ��

	BOOL			ReqSetActionID(DWORD actionID);//���������߳�,�ص�ʱ�����޸��ҽ��޸ı���ͼ���ڴ�����
//	BOOL			SetActionID(DWORD actionID,BOOL bUpdate=UPDATE_NEVER);//�����̻߳ص�,���þ�������action
//	DWORD			GetActionID(){return GetInt(SYNDATA_ACTIONID);}//��þ���action

	//20070412����:���Ź�����չ
	BOOL			ReqSetSynProffer(int synProffer);//���������߳����þ��Ź���ֵ
//	int				GetSynProffer(){return GetInt(SYNDATA_SYNPROFFER);}//��þ��Ź���ֵ

	BOOL			ReqPayOff(DWORD dwMoney);//���������̷߳�����
	BOOL			ReportMemberProfferToWolrd(int dwProffer,OBJID idMember);//�㱨��Ա����ֵ�������߳�.
	//--------------------------------------------------

	BOOL			ReqSetSynName(LPCTSTR szNewSynName);//20070825����:���������߳̾��Ÿ���
private:
	CSynBase*	m_pData;
	bool		m_bLock;///1��ʾ����,0��ʾδ��
protected: // ctrl
	PROCESS_ID		m_idProcess;
	CTimeOut t_Maintain;
	CTimeOut t_Update;
	CTimeOut t_PayEmploy;

	MYHEAP_DECLARATION(s_heap)
//protected:
// 	class	XSynModify : public ISynModify
// 	{
// 	public:
// 		virtual bool	Demise(OBJID idOldLeader,int nOldSynMemberLevel,OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave);
// 		virtual bool	SetTenet(LPCTSTR szTenet, bool bSave);
// 		virtual bool	SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave);
// 		virtual bool	SetMemberTitle(LPCTSTR szMemberTitle, bool bSave);
// 		virtual bool	AddData(SYNDATA idx, int nData, bool bSave);
// 		virtual bool	SetData(SYNDATA idx, int nData, bool bSave);
// 		virtual bool	PresentMoney(OBJID idSyn, int nMoney, bool bSave);
// 		virtual bool	SetSynName(LPCTSTR szSynName, bool bSave);
// 		//20070414����:�����������󷢹���
// 		virtual bool	PayOff(DWORD dwMoney,BOOL bSave);//bSave=true
// 		virtual bool	SetMemberProffer(int dwProffer,OBJID idMember,BOOL bSave/* =true */);//20070417����:ͬ�������߳��ڳ�Ա����ֵ
// 	protected:
// 		CSyndicate*		This()		{ return m_pOwner; }
// 		CSyndicate*		m_pOwner;
// 		friend class CSyndicate;
// 	} m_obj;
public:
	int GetLeaderID();
	int GetSubLeaderID();
	int GetSubLeader2ID();
	int GetLeaderWifeID();
	void SetSubLeaderID(OBJID id);	
	void SetSubLeaderID2(OBJID id);
	void SetLeaderWifeID(OBJID id);
	void	SynchroInfo();

	OBJID	GetMasterSynID();
	bool	FallLevel();
	bool	IsCanFallLevel();
	bool	IsSubSubSyn();
	bool	IsSubSyn();
	int		GetSubSubSynNum();
	bool	IfCanCreateSubSubSyn();
	bool	IsMasterSyn()				{ return false; }
	//����ÿ�ܻ���ͳһ�ĵط����ű��ܾ��������һ������Ϊ��Ա����
	bool	AllotBonusForMember(CUserPtr pUser,DWORD dwBonus);
	bool	ApplyKickoutMember(CUserPtr pLeader,CUserPtr pTarget);
	int		GetSubSynNum();
//	friend class CSyndicate::XSynModify;
public:
	typedef	vector<MemberInfo>	MEMBER_SET;
private:
	MEMBER_SET m_setMember;
	MEMBER_SET m_setQueryMember;
	OBJID idLeader;
	OBJID idSubLeader;
	OBJID idSubLeader2;
	OBJID idFeelLeader;
	bool  m_bCallTask;
// 	OBJID idFeelLeader1;
// 	OBJID idFeelLeader2;
// 	OBJID idFeelLeader3;
// 	OBJID idFeelLeader4;
// 	OBJID idSubFeelLeader1;
// 	OBJID idSubFeelLeader2;
// 	OBJID idSubFeelLeader3;
// 	OBJID idSubFeelLeader4;
// 	int nAmount;
//	int nShpAmount;
// 	int nOnlineAmount;
	DWORD dwLastUpdate;

public:

	DWORD GetSkillExpDeadline(int nSub);
	bool IsStopSub(int nSub);
	int GetSkillLev(int nSub, int nSkill);
	void SetSkill(int nSub, int nData, bool bSendtoOtherline = true);
	UINT SetGrow(int nType, UINT nData, bool bSendtoOtherline = true);//(�����ʽ�,����)�ᷢ�Ϳͻ��˼�ʱ����
	bool UpSubSkill(int nSub, int nSkill, char* szUserName = "");
	bool CheckSkillDeadline(int nSub);
	int GetSubAmount();
	UINT GetSkillExp(int nSub, int nSkill = 0);
	bool SpendMoney(int Syndata, UINT nData);
	bool CheckCondition(/*CUserPtr pUser, */int nSub);
	bool UpLevel(CUserPtr pUser, int nSub);
	void SetPay(int nPay);
	void SetDeadline();	
	void UpdateInfo(int nThing,OBJID nData,USHORT nSub=0);//0�ǽ��裬1���ʽ�2�ǵȼ�
	void UpdateLevel(int sub=0);//
	const __int64 GetDeadline(int nThing, int Sub = 0);//1�Ƿ��ٽ����ʽ� 2�Ǽ��� 3�ǰ��ɵȼ� 4�Ǹ��ֹ������� 5���ʽ�
	void ReSetStopsub(int nSub);
	void SetStopsub(int nSub);
	void SetLev(int nSub = 0, int nLev = 0);
	UINT GetLev(int nSub = 0);
	void ReduceLevel();
	void OnTimer(time_t tCur);
	bool PayBasic();
	bool NoPay();
//	UINT GetBusMan();
//	UINT GetBuilder();
//	bool PayEmployer();
	bool MaintainSynData();
	UINT GetGrow();
	LPCTSTR GetSubNameByID(OBJID sub);
//	bool SetBusMan(OBJID SubId, int Data1, int Data2, int Data3, const char* szName, int Money);//������ʿ����
//	UINT GetBusMan(OBJID SubId);
//	void SetKONGFU(OBJID SubId, int Data);
//	UINT GetKONGFU(OBJID SubId);
//	void SetBuider(OBJID SubId, int Data);
//	UINT GetBuilder(OBJID SubId);
	void SendSubToC(CUserPtr pUser);
	void SendSciToC(CUserPtr pUser);

	MemberInfo* GetMemberFromList(OBJID idUser,int nType);

	bool Hasmember(OBJID idUser);
	bool HasQuerymember(OBJID idUser);
	void SetOnline(OBJID idUser,UCHAR cLine);
// 	void incOnline() { nOnlineAmount ++ ;}
// 	void DecOnline() { nOnlineAmount -- ; if(nOnlineAmount < 0) nOnlineAmount = 0;}
 	int GetOnlineAmount();
	int AddMemberList(MEMBERINFO info,int nType = 1); //1 ����Ա 2.�����б�
	int ChgMemberRank(OBJID idUser,int nRank = 10,bool bSend = true,CUser* pUser = NULL);
	bool DelMemberList(OBJID idUser,int nType = 1);
	bool AllowMenber(OBJID idUser,CUser* pUser);
	void SetStr(SYNDATA idx, LPCTSTR szData,int nSize,bool bSave = false)				{ m_pData->SetStr(idx,szData,nSize); if(bSave)  m_pData->Update();}
};

#endif // !defined(AFX_SYNDICATE_H__78C1D98F_7DA8_482D_9819_22AD24402CB6__INCLUDED_)
