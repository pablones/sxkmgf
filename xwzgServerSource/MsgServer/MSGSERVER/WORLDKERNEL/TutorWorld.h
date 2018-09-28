// TutorWorld.h: interface for the CTutorWorld class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TUTORWORLD_H__BF555F23_4A75_4E15_A930_439880DFD88A__INCLUDED_)
#define AFX_TUTORWORLD_H__BF555F23_4A75_4E15_A930_439880DFD88A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "protocol.h"
#include "../../GameBaseCodeMT/I_mydb.h"

// enum TUTORDATA
// {
// 	TUTORDATA_ID = 0,
// 	TUTORDATA_USERID,
// 	TUTORDATA_NAME,
// 	TUTORDATA_TUTORID,
// 	TUTORDATA_TUTORNAME,
// 	TUTORDATA_DENSITY,
// 	TUTORDATA_APPLYTYPE,
// 	TUTORDATA_DELTIME,
// };

const int SUB_DENSITY = 20;  //24��������ܶ�

class CTutorWorld  
{
public:
	CTutorWorld();
	virtual ~CTutorWorld();

	ULONG Release()				{ delete this; return 0; }
	bool  Create(IDatabase* pDb);
	void  OnTimer();
	bool  ProcessTutorInfo(const ChgTutorInfo *info);     //����ʦͽ�����Ϣ

	MYHEAP_DECLARATION(s_heap)

private:
	void UpdateOnTime();                                          //ʦͽ������ش��� clock 24
	bool RequestDelTutor(const ChgTutorInfo *info, bool bTutor);  //����ɾ��ʦ��
	bool RecoverTutor(const ChgTutorInfo *info, bool bTutor);     //�ָ�ʦ��
	bool AddDensity(const ChgTutorInfo *info);                    //�������ܶ�
	bool CancelLeaveTutor(const ChgTutorInfo *info);              //ȡ���뿪ʦ��
	bool CancelDelStudent(const ChgTutorInfo *info);              //ȡ��ɾ��ѧ��

	IDatabase* m_pDb;
	bool       m_bUpdate;
};

#endif // !defined(AFX_TUTORWORLD_H__BF555F23_4A75_4E15_A930_439880DFD88A__INCLUDED_)
