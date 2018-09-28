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
//���ڵ�ͼ�鷢��
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

	bool	DelStuApply(OBJID idStu);//ɾ��ʦ���յ��İ�ʦ����
	bool	DelTutorApply(OBJID idTutor);//ɾ��ͽ�ܷ����İ�ʦ����

	int		GetStudentAmount(bool bIncludeGraduated);
	CTutorData* FindStudentByID(OBJID idStudent);
	CTutorData* FindTutorByID(OBJID idTutor);
	CTutorData* GetTutor();//ȡ��δ��ʦ��ʦ����û��ʱ���ؿ�

	int		GetTutorApplyAmount();//���ͽ�������б������

	bool	TutorDivorce();
	bool	DelStudentByTutor(OBJID idStudent, bool bDelDBRec=true);
	bool	DelTutorByStudent(bool bDelDBRec=true);

	DWORD	GetTutorGiftMask(OBJID idStudent);
	void	SetTutorGiftMask(OBJID idStudent, DWORD dwMask);

	bool	Graduate(OBJID idStudent);
	void	SetState(OBJID idTutor, TUTORSTATE nState);

	bool	StuRebel();//ͽ����ʦ
	bool	KickOutStu(OBJID idStudent);//�߳�ͽ��
	bool	SendAllList();//����ʦͽ�б�

	bool	SendStuApplyList(USHORT usPage, int nPageSize);//����ͽ�������б�
	bool	SendStuList();//����ͽ���б�
	int		SendGraduateStuList(int nPage, int nPageSize);//���ͳ�ʦ��ͽ���б�

	int		GetGraduatedAmount();//��ͽ����

	int		static GetTutorLevByImpart(UINT nImpart);//ͨ�����ڶȻ�ô��ڵȼ�
	int		static GetMaxStuAmount(UINT nImpart);//�������ܴ���ͽ������

	void	SendTutorOnLineInfo();//֪ͨʦͽ�Լ����ߣ�ͬʱ��ȡʦͽ������Ϣ
	void	SendTutorOffLineInfo();//֪ͨʦͽ�Լ�����
	void	BroadCastTutorState();//֪ͨʦ����ͽ���Լ�״̬�ı���

	void	ProcessGiftMask(int nLev);//ͽ��������ʦ���Ľ������봦��
	bool	CanGetGift(OBJID idStu, int nLev);//�Ƿ���ʦͽ�ȼ�����
	bool	AddGiftMask(OBJID idStu, int nLev);//����ָ���ȼ��Ľ�������

	bool	HasBeCalled() { return m_bCalled; }
	void	SetBeCalled(bool bCalled) { m_bCalled = bCalled; }
	BeCalledPos* GetCalledPos()	{ return &m_stCalledPos; }
	void	SetCalledPos(int nMapID, int nPosX, int nPosY, int nLine);

	void	AddTutorAddedLev(int nLev=1);//�����ʦ������������������1

	int		GetSetStuSize()	{ return m_setStu->GetAmount(); }
	void	ResetTutorName(const char* szName);
	void	ResetStudentName(const char* szName);

	int		GetSetTutorSize()	{ return m_setTutor->GetAmount(); }
	//���ڵ�ͼ�鴫��
	bool	GetStuInfoByIndex(TutorInfoStruct* pInfo, int idx);
	bool	AppendStu(const TutorInfoStruct* pInfo);
	bool	GetTutorInfoByIndex(TutorInfoStruct* pInfo, int idx);
	bool	AppendTutor(const TutorInfoStruct* pInfo);
protected:
	PROCESS_ID		m_idProcess;
	CUser*          m_pUser;
	ITutorSet*		m_setStu;//ͽ���б�
	ITutorSet*		m_setTutor;//ʦ���б�ʦ��ֻ��һ���������������������ʦ
	bool			m_bCalled;//�Ƿ�ʦ���ٻ���
	//���ٻ���Ŀ�ĵ�
	BeCalledPos		m_stCalledPos;
};

#endif // !defined(AFX_USERTUTOR_H__C5437023_BBEB_4600_A7E0_798191EE4792__INCLUDED_)
