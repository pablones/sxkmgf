// UserTutor.h: interface for the CUserTutor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERTUTOR_H__C5437023_BBEB_4600_A7E0_798191EE4792__INCLUDED_)
#define AFX_USERTUTOR_H__C5437023_BBEB_4600_A7E0_798191EE4792__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../share/define.h"
#include "GameObj.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "T_SingleObjSet2.h"
#include "TutorData.h"
#include "MsgSchoolMember.h"
#include "TutorList.h"

//////////////////////////////////////////////////////////////////////
typedef	ISingleObjSet2<CTutorData>		ITutorSet;
typedef	CSingleObjSet2<CTutorData>		CTutorSet;
//////////////////////////////////////////////////////////////////////
//用于地图组发送
struct TutorInfoStruct
{
	OBJID	id;
	OBJID	idTutor;
	NAMESTR	szTutorName;
	OBJID	idStudent;
	NAMESTR	szStudentName;
	DWORD	dwGiftMask;
	int		nState;
	int		nAddLev;
	int		nLine;
	OBJID	idTeam;
	stDetailInfo	detailInfo;
};

typedef struct{
	int nMapID;
	int nPosX;
	int nPosY;
	int nLine;
}BeCalledPos;

class CUserTutor  
{
public:
	CUserTutor(PROCESS_ID idProcess, CUser* pUser);
	virtual ~CUserTutor();
private:
	CUserTutor();
public:
	static  CUserTutor*		CreateNew(PROCESS_ID idProcess, CUser* pUser);
	ULONG	ReleaseByOwner()	{ delete this; return 0; }
	bool	CreateAll();
	int		SendInfoToClient(int nAmountPerPage, int nPage);
	bool	SaveInfo();

	bool	SendLeaveMsg();

	OBJID	ApplyAddTutor(ST_MEMBERINFO* pInfo, TUTORSTATE nState, ST_MEMBERINFO* pInfoExt);
	CTutorData*	AddLeaveTutor(OBJID idStu, OBJID idTUtor);
	bool	AddNewToTutorApply(OBJID id, ST_MEMBERINFO* pInfo, TUTORSTATE nState, ST_MEMBERINFO* pInfoStuExt);
	CTutorData*	AddNewToLeaveTutorApply(OBJID idStu, OBJID idTUtor);

	bool	DelStuApply(OBJID idStu);//删除师父收到的拜师请求
	bool	DelTutorApply(OBJID idTutor);//删除徒弟发出的拜师请求

	int		GetStudentAmount(bool bIncludeGraduated);
	CTutorData* FindStudentByID(OBJID idStudent);
	CTutorData* FindTutorByID(OBJID idTutor);
	CTutorData* GetTutor();//取得未出师的师父，没有时返回空

	int		GetTutorApplyAmount();//获得徒弟申请列表的数量

	bool	TutorDivorce();
	bool	DelStudentByTutor(OBJID idStudent, bool bDelDBRec=true);
	bool	DelTutorByStudent(bool bDelDBRec=true);

	DWORD	GetTutorGiftMask(OBJID idStudent);
	void	SetTutorGiftMask(OBJID idStudent, DWORD dwMask);

	bool	Graduate(OBJID idStudent);
	void	SetState(OBJID idTutor, TUTORSTATE nState);

	bool	StuRebel();//徒弟叛师
	bool	KickOutStu(OBJID idStudent);//踢出徒弟
	bool	SendAllList();//发送师徒列表

	bool	SendStuApplyList(USHORT usPage, int nPageSize);//发送徒弟申请列表
	bool	SendStuList();//发送徒弟列表
	int		SendGraduateStuList(int nPage, int nPageSize);//发送出师的徒弟列表

	int		GetGraduatedAmount();//出徒数量

	int		static GetTutorLevByImpart(UINT nImpart);//通过传授度获得传授等级
	int		static GetMaxStuAmount(UINT nImpart);//获得玩家能带的徒弟数量

	void	SendTutorOnLineInfo();//通知师徒自己上线，同时获取师徒在线信息
	void	SendTutorOffLineInfo();//通知师徒自己下线
	void	BroadCastTutorState();//通知师父和徒弟自己状态改变了

	void	ProcessGiftMask(int nLev);//徒弟升级后师父的奖励掩码处理
	bool	CanGetGift(OBJID idStu, int nLev);//是否有师徒等级奖励
	bool	AddGiftMask(OBJID idStu, int nLev);//设置指定等级的奖励掩码

	bool	HasBeCalled() { return m_bCalled; }
	void	SetBeCalled(bool bCalled) { m_bCalled = bCalled; }
	BeCalledPos* GetCalledPos()	{ return &m_stCalledPos; }
	void	SetCalledPos(int nMapID, int nPosX, int nPosY, int nLine);

	void	AddTutorAddedLev(int nLev=1);//玩家在师父的门下升级次数加1

	int		GetSetStuSize()	{ return m_setStu->GetAmount(); }
	void	ResetTutorName(const char* szName);
	void	ResetStudentName(const char* szName);

	int		GetSetTutorSize()	{ return m_setTutor->GetAmount(); }
	//用于地图组传送
	bool	GetStuInfoByIndex(TutorInfoStruct* pInfo, int idx);
	bool	AppendStu(const TutorInfoStruct* pInfo);
	bool	GetTutorInfoByIndex(TutorInfoStruct* pInfo, int idx);
	bool	AppendTutor(const TutorInfoStruct* pInfo);
protected:
	PROCESS_ID		m_idProcess;
	CUser*          m_pUser;
	ITutorSet*		m_setStu;//徒弟列表
	ITutorSet*		m_setTutor;//师父列表，师父只有一个，但可以向多个人申请拜师
	bool			m_bCalled;//是否被师父召唤了
	//被召唤的目的地
	BeCalledPos		m_stCalledPos;
};

#endif // !defined(AFX_USERTUTOR_H__C5437023_BBEB_4600_A7E0_798191EE4792__INCLUDED_)
