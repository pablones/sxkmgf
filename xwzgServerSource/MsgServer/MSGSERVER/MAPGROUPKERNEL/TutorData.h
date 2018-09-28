// TutorData.h: interface for the CTutorData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TUTORDATA_H__B21C4B4C_0011_45BA_AC0E_5F6F501A7EE7__INCLUDED_)
#define AFX_TUTORDATA_H__B21C4B4C_0011_45BA_AC0E_5F6F501A7EE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include "GameObj.h"
#include "../../GameBaseCodeMT/GameData.h"

//////////////////////////////////////////////////////////////////////
enum TUTORDATA{
	TUTORDATA_ID	=	0,
	TUTORDATA_TUTORID,		//ʦ��ID
	TUTORDATA_TUTORNAME,	//ʦ������
	TUTORDATA_STUID,		//ͽ��ID
	TUTORDATA_STUNAME,		//ͽ������
	TUTORDATA_TUTORGIFTMASK,//ʦ����������
	TUTORDATA_STATE,		//ʦͽ״̬
	TUTORDATA_ADDEDLEV,		//��ʦ�����������ļ���
	//�����ֶ���������user������ʦͽ���в����޸ĺ�update����ȡ�͸�ֵ��ʹ�������szDetailInfo�ṹ��
	TUTORDATA_SEX,		//�Ա�
	TUTORDATA_LOOKFACE,	//ͷ��
	TUTORDATA_VIPFACE,	//VIPͷ��
	TUTORDATA_LEV,		//�ȼ�
	TUTORDATA_MAINJOB,	//��ְҵ
	TUTORDATA_MAINLEV,	//��ְҵ�ȼ�
	TUTORDATA_SUBJOB,	//��ְҵ
	TUTORDATA_SUBLEV,	//��ְҵ�ȼ�
	TUTORDATA_IMPART,	//���ڶ�
	TUTORDATA_MAXSCORE,	//ս����
	TUTORDATA_VIPLEV,	//VIP�ȼ�
	TUTORDATA_EXITTIME,	//����ʱ��
	TUTORDATA_MORALITY,	//ʦ��
};
struct stDetailInfo
{
	USHORT	nSex;
	USHORT	nLookFace;
	USHORT	nVipFace;
	USHORT	nLev;
	UINT	nMainJob;
	USHORT	nMainLev;
	UINT	nSubJob;
	USHORT	nSubLev;
	UINT	nImpart;
	UINT	nMaxScore;
	USHORT	nVipLev;
	DWORD	dwExitTime;
	UINT	nMorality;
};
enum TUTORSTATE//ʦͽ״̬
{
	TUTORSTATE_BEGIN = 0,
	TUTORSTATE_NORMAL = 0,//����״̬��δ��ʦ
	TUTORSTATE_GRADUATED = 1,//�ѳ�ʦ
	TUTORSTATE_APPLY	 = 2,//ͽ�ܷ���������ͽ�������б��У�����δ֪ͨʦ��
	TUTORSTATE_APPLYREF	 = 3,//��ͽ�������б��У��Ѿ�֪ͨ��ʦ��
	TUTORSTATE_STUREBEL  = 4,//ͽ���ѳ���ʦ��
	TUTORSTATE_TUTORKICKOUT = 5,//ʦ������ͽ��,δ֪ͨͽ��
	TUTORSTATE_AGREED		= 6,//ʦ��ͬ������ͽ��δ֪ͨͽ��
	TUTORSTATE_REFUSED		= 7,//��ʦ���ܾ���δ֪ͨͽ��
	TUTORSTATE_APPLYFAILED	= 8,//��ʦ���󲻳ɹ�
	TUTORSTATE_END,
};

char szTutorTable[];
typedef	CGameData<TUTORDATA, szTutorTable, szID>	CGameTutorData;
class CTutorData
{
public:
	CTutorData();
	virtual ~CTutorData();
private:
	void	LoadDetailInfo(IRecordset* pRes, int nOffset=0);
	void	LoadDetailInfo(CUser* pUser);
	void	LoadDetailInfo(stTempUser* pUser);
public:
	static	CTutorData*	CreateNew()		{ return new CTutorData; }
	void	ReleaseByOwner()		{ delete this; }

	bool	Create(IRecordset* pRes);
	bool	Create(IRecordset* pRes, OBJID idNewKey);

	bool	Create(IRecordset* pRes, CUser* pUser);
	bool	Create(IRecordset* pRes, stTempUser* pUser);

	OBJID	GetID()				{ CHECKF(m_pData); return m_pData->GetID(); }
	bool	Update()			{ CHECKF(m_pData); return m_pData->Update(); }
	void	ClearUpdateFlags()	{ CHECK(m_pData); m_pData->ClearUpdateFlags(); }
	OBJID	InsertRecord()		{ CHECKF(m_pData); return m_pData->InsertRecord(); }
	bool	DeleteRecord()		{ CHECKF(m_pData); return m_pData->DeleteRecord(); }
	int		GetInt(TUTORDATA nDataName);
	void	SetInt(TUTORDATA idx, int nData, bool bUpdate=false);
	LPCTSTR GetStr(TUTORDATA nDataName);
	void	SetStr(TUTORDATA idx, LPCTSTR szData, int nSize, bool bUpdate=false);

	int		GetState()			{ CHECKF(m_pData); return m_pData->GetInt(TUTORDATA_STATE); }
	void	SetState(TUTORSTATE nState, bool bUpdate=true) { SetInt(TUTORDATA_STATE, nState, bUpdate); }

	bool	IsGraduated()		{ CHECKF(m_pData); return m_pData->GetInt(TUTORDATA_STATE)==TUTORSTATE_GRADUATED; }
	OBJID	GetStuID()			{ CHECKF(m_pData); return m_pData->GetInt(TUTORDATA_STUID); }

	OBJID	GetTutorID()		{ CHECKF(m_pData); return m_pData->GetInt(TUTORDATA_TUTORID); }
	LPCTSTR	GetTutorName()		{ CHECKF(m_pData); return m_pData->GetStr(TUTORDATA_TUTORNAME); }

	DWORD	GetTutorGiftMask()				{ CHECKF(m_pData); return m_pData->GetInt(TUTORDATA_TUTORGIFTMASK); }
	void	SetTutorGiftMask(DWORD dwMask, bool bUpdate=false)	{ SetInt(TUTORDATA_TUTORGIFTMASK, dwMask, bUpdate); }

	LPCTSTR	GetStuName()			{ CHECKF(m_pData); return m_pData->GetStr(TUTORDATA_STUNAME); }

	int		GetLookFace();

	int		GetLine()		{ return m_nLine; }
	void	SetLine(LPCTSTR szLine);
	void	SetLine(UCHAR ucLine)	{ m_nLine=ucLine; }

	bool	HasTeam()		{ return m_nTeam>0; }
	void	SetTeam(OBJID idTeam);

	stDetailInfo*	GetDetailInfo()	{ return &m_stDetailInfo; }
	void			SetDetailInfo(const stDetailInfo* pInfo);
protected:
	CGameTutorData* m_pData;
	stDetailInfo	m_stDetailInfo;
	int				m_nLine;//������·
	OBJID			m_nTeam;//�Ƿ��ڶ�����
};
//////////////////////////////////////////////////////////////////////////
// class CTutorData : public CGameObj  
// {
// public:
// 	CTutorData();
// 	virtual ~CTutorData();
// 
// public:
// 	bool	Create			(IRecordset* pRes);
// 	bool	Create			(OBJID id, IDatabase* pDb);
// 	TutorInfoStruct*	GetInfo	(void)		{return &m_Info;}
// 
// 	bool	LoadInfo		(IRecord* pRes);
// 
// public:	// get
// 	OBJID	GetID()				{ return m_Info.id; }
// 	OBJID	GetUserID()			{ return m_Info.idUser; }
// 	OBJID	GetTutorID()		{ return m_Info.idTutor; }
// 	LPCTSTR	GetUserName()		{ return m_Info.szUserName; }
// 	LPCTSTR	GetTutorName()		{ return m_Info.szTutorName; }
// 	UINT	GetImpart()			{ return m_Info.nImpart; }
// 	bool	IsGraduated()		{ return m_Info.nIsGraduated>0; }
// 	bool	HasGift()			{ return m_Info.nHasGift>0; }
// 	DWORD	GetGiftMask()		{ return m_Info.nGiftMask; }
// 	DWORD	GetTutorMask()		{ return m_Info.nTutorMask; }
// 	UINT	GetStuLev()			{ return m_Info.nStuLev; }
// 	UINT	GetTutorLev()		{ return m_Info.nTutorLev; }
// 	UINT	GetTutorImpartLv()	{ return m_Info.nTutorImpartLv; }
// //	UINT    GetDensity()        { return m_Info.nDensity; }
// //	UINT    GetDelApplyType()   { return m_Info.nApplyType; }
// // 	LPCTSTR GetQuitTime()       { return m_Info.strQuitTime; }
// // 	int     GetStudentPro()     { return m_Info.nStudentPro; }
// // 	int     GetTutorPro()       { return m_Info.nTutorPro; } 
// 
// //	void    SetApplyType(int nApplyType)   { m_Info.nApplyType = nApplyType; }
// //	void    SetDelTime(const char* pStr)   { strcpy(m_Info.strQuitTime, pStr); }
// 
// public:	// add del
// 	OBJID	CreateRecord(const TutorInfoStruct* info, IDatabase* pDb);
// 	bool	DeleteRecord(IDatabase* pDb);
// 	bool	AddImpart(int nImpart, IDatabase* pDb, bool bUpdate=true);
// 	bool	SetGiftMask(int nGiftMask, IDatabase* pDb, bool bUpdate=true);
// 	bool	SetTutorMask(int nGiftMask, IDatabase* pDb, bool bUpdate=true);
// 	bool	SetGraduated(int nGraduated, IDatabase* pDb, bool bUpdate=true);
// 	bool	SetHasGiftFlag(int nFlag, IDatabase* pDb, bool bUpdate=true);//����ʦ��������ʶ
// //	bool    AddDensity(int nDensity);
// 
// protected:
// 	TutorInfoStruct		m_Info;
// };

#endif // !defined(AFX_TUTORDATA_H__B21C4B4C_0011_45BA_AC0E_5F6F501A7EE7__INCLUDED_)
