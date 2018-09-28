// SynShell.h: interface for the CSyndicateWorld class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNSHELL_H__8147FFBF_85B9_43E4_9D3A_B847BF31F088__INCLUDED_2)
#define AFX_SYNSHELL_H__8147FFBF_85B9_43E4_9D3A_B847BF31F088__INCLUDED_2

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define WINVER 0x0501		// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0510 // ��������Ϊ�ʺ� Windows Me ����߰汾����Ӧֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 6.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0500	// ����ֵ����Ϊ��Ӧ��ֵ���������� IE �������汾��ֵ��
#endif

#include "../share/define.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "protocol.h"
#include "AutoPtr.h"
#include "MessageBoard.h"
#include <vector>
#include <string>
using namespace std;

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
		SYNFUNC_UP_LEVEL,
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
enum SYNDATA{
		SYNDATA_ID=0,			// ������INSERT
		SYNDATA_NAME=1,			// ������
		SYNDATA_ANNOUNCE,		// ���ɹ���
		SYNDATA_LEADERID,		// ����ID
		SYNDATA_LEADERNAME,		// ��������
//		SYNDATA_SUBLEADERID,	// ������ID
		SYNDATA_SUBLEADERNAME,	// ����������
		SYNDATA_CREATETIME,		//����ʱ��
		SYNDATA_STATE,			//״̬1����Ӧ��2��������3�ǽ�ɢ
		SYNDATA_LEVEL,			//���õȼ�
		SYNDATA_SUBLEV1,		
		SYNDATA_SUBLEV2,
		SYNDATA_SUBLEV3,
		SYNDATA_SUBLEV4,
		SYNDATA_GROW,			//���ý���
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
};
char	szID[];
char	szSynTable[];
typedef	CGameData<SYNDATA, szSynTable, szID>	CSynWorldBase;

//////////////////////////////////////////////////////////////////////
class ISynModify
{
public:
	virtual bool	Demise(OBJID idOldLeader,int nOldSynMemberLevel,OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave)		PURE_VIRTUAL_FUNCTION_0
	virtual bool	SetTenet(LPCTSTR szTenet, bool bSave)													PURE_VIRTUAL_FUNCTION_0
	virtual bool	SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave)										PURE_VIRTUAL_FUNCTION_0
	virtual bool	SetMemberTitle(LPCTSTR szMemberTitle, bool bSave)										PURE_VIRTUAL_FUNCTION_0
	virtual bool	AddData(SYNDATA idx, int nData, bool bSave)												PURE_VIRTUAL_FUNCTION_0
	virtual bool	SetData(SYNDATA idx, int nData, bool bSave)												PURE_VIRTUAL_FUNCTION_0
	virtual bool	PresentMoney(OBJID idSyn, int nMoney, bool bSave)										PURE_VIRTUAL_FUNCTION_0
	virtual bool	SetSynName(LPCTSTR szSynName, bool bSave=true)											PURE_VIRTUAL_FUNCTION_0
};

//////////////////////////////////////////////////////////////////////
// ���г�Ա����Ϊԭ�Ӳ�����
struct CPlayer;
class CSyndicateWorld : ISynModify
{
protected:
	CSyndicateWorld();
	virtual ~CSyndicateWorld();
public:
	static CSyndicateWorld* CreateNew()	{ return new CSyndicateWorld; }
	virtual ULONG	ReleaseByOwner()				{ delete this; return 0; }
	bool	Create(IRecordset* pRes);
	bool	Create(IRecordset* pRes, const CreateSyndicateInfo* pInfo, bool bSave);
	ISynModify*		QueryModify()					{ return (ISynModify*)this; }
	CMessageBoard*	QueryMessageBoard()				{ ASSERT(m_ptrMsgBd); return m_ptrMsgBd; }
	bool			ChangeSyndicate(/*const*/ SynFuncInfo0* pFuncInfo);
	BOOL			PayOff(DWORD dwMoney,BOOL bSave);//20070414����:������.�����޸����ݿ�.�Ҵ����Ա�ַ�
	BOOL			SetMemberProffer(int dwProffer,OBJID idMember,BOOL bSave);//20070417����:��Ա����ֵ
	void			WriteOfflineTime(OBJID userID);
public:
	OBJID	GetID()									{ return m_pData->GetKey(); }
	int		GetInt(SYNDATA idx)						{ return m_pData->GetInt(idx); }
	LPCTSTR	GetStr(SYNDATA idx)						{ return m_pData->GetStr(idx); }

protected: // syn member list --------------------------
	typedef struct	MemberInfo{
		//[��;���� 2008/9/24]�԰����Ϣ��ԭ���ĸ����䵽�߸�
		//[��;���� 2008/9/25]���Ӷ�������Ա�ĵȼ�
		MemberInfo(int id, LPCTSTR str, int nUserRank, int level, int proffer, int sex, int Profession, bool Online,int OfflineTime = 0) 
		{
			nId			=	id;
			szName		=	str;
			nRank		=	nUserRank;
			nLevel		=	level;
			nProffer	=	proffer;
			nSex		=	sex;
			nProfession	=	Profession;
			bOnline		=	Online;
			nOfflineTime = OfflineTime;
		}
		int		nId;		//��ұ�ʶ
		string	szName;		//�����
		int		nRank;		//���ְλ
		int		nLevel;		//��ҵȼ�
		int     nProffer;	//�������
		int		nSex;		//����Ա�
		int		nProfession;//���ְҵ
		bool	bOnline;	//�Ƿ�����
		int     nOfflineTime;//����ʱ�� *������ߵ�¼����ֵ����1000000 �������ּ�¼������ʵʱ��
	}MEMBERINFO;
	typedef	vector<MemberInfo>	MEMBER_SET;
public:
	bool DelMember(int id);
	int GetMemberProffer(OBJID id);
	int		GetSynRankShow(int nRank);
    const char* GetMemberName(OBJID id);
//	void	SendMemberListSpecifySyn(CPlayer* pUser, int idxLast);
	CSyndicateWorld* GetMasterSyn();
	//[��;���� 2008/9/24]��AddMember�������¼���������
	//[��;���� 2008/9/25]��AddMember�������¼�bOnline��¼�������
	bool	AddMember(int id,LPCTSTR szName, int nUserRank,int level,int proffer, int nSex, int nProfession, bool bOnline);
	bool	DelMember(LPCTSTR szName);
	bool	SetMemberRank(const char* pszName, int nRank);
	int		GetMemberAmount()				{ return m_setMember.size(); }
	void	SendMemberList(CPlayer* pUser,DWORD dwTime );
	void	CombineMemberList(const MEMBER_SET& setMember);
	MEMBER_SET&	QueryMemberList()						{ return m_setMember; }

// 	OBJID   GetSynAttackID();
// 	OBJID   GetSynDefenceID();
// 	void    SetSynAttackID(OBJID idAttack, bool bUpdate=false);
// 	void    SetSynDefenceID(OBJID idDefence, bool bUpdate=false);
	bool    Update();

protected:
//------------06.10.26����ע�Ͳ��޸�-------------begin----------------------------------------
//	int		AppendMember(class CMsgName& msg, int idx, int idxFirst, int& nCount);
	void		AppendMember(class CMsgName& msg,int idxFirst,int idxTime );

//-------------------------------------------------end---------------------------------------
//	int		AppendOnlineMember(class CMsgName& msg, int idxFirst);
//	int		AppendOnlineMemberForAllMember(class CMsgName& msg, int idxFirst);
//	int		AppendOfflineMember(class CMsgName& msg, int idxFirst, int nCurrCount=0);
//	int		AppendOfflineMemberForAllMember(class CMsgName& msg, int idx, int nCount=0);
protected:
	MEMBER_SET	m_setMember;
//	MEMBER_SET	m_setMemberTmp;

protected: // modify, mast be atom operator. test & set
	// bSave ����Ϊtrue�����򲻻����
	//[��;���� 2008/9/24]��Demise���������¼�nOldSex��nOldProfession��������
	virtual bool	Demise(OBJID idOldLeader,int nOldSynMemberLevel,OBJID idNewLeader, LPCTSTR szNewLeader, int nNewSynMemberLevel, OBJID idBackSyn, bool bSave, int nOldSex, int nOldProfession);
	virtual bool	SetTenet(LPCTSTR szTenet, bool bSave);
	virtual bool	SetLeaderTitle(LPCTSTR szLeaderTitle, bool bSave);
	virtual bool	SetMemberTitle(LPCTSTR szMemberTitle, bool bSave);
	virtual bool	AddData(SYNDATA idx, int nData, bool bSave);
	virtual bool	SetData(SYNDATA idx, int nData, bool bSave);
	virtual bool	PresentMoney(OBJID idSyn, int nMoney, bool bSave);
	virtual bool	SetSynName(LPCTSTR szSynName, bool bSave);
//06.10.25�������---------begin--------------------
	bool	SetMemberLevel( const char* pTargetName,int nLevel );//����Ҳ�Ǹ���synattr��,�����������
	DWORD	m_dwTimeLastUpdateMemberInfo;//��������һ�θ��³�Ա��Ϣ��ʱ��,�������ж��Ƿ�Ҫ�ط���Ա��Ϣ
	DWORD	m_dwTimeFirstUpdateMemberOffline;
//---------------------------end----------------------
private:
	CSynWorldBase*	m_pData;

protected: // msgbd
	typedef	CAutoPtr<CMessageBoard>	MSGBD_PTR;
	MSGBD_PTR		m_ptrMsgBd;

protected: // ctrl
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_SYNSHELL_H__8147FFBF_85B9_43E4_9D3A_B847BF31F088__INCLUDED_2)
