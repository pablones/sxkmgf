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

const int SUB_DENSITY = 20;  //24点减少亲密度

class CTutorWorld  
{
public:
	CTutorWorld();
	virtual ~CTutorWorld();

	ULONG Release()				{ delete this; return 0; }
	bool  Create(IDatabase* pDb);
	void  OnTimer();
	bool  ProcessTutorInfo(const ChgTutorInfo *info);     //处理师徒相关信息

	MYHEAP_DECLARATION(s_heap)

private:
	void UpdateOnTime();                                          //师徒定点相关处理 clock 24
	bool RequestDelTutor(const ChgTutorInfo *info, bool bTutor);  //请求删除师傅
	bool RecoverTutor(const ChgTutorInfo *info, bool bTutor);     //恢复师傅
	bool AddDensity(const ChgTutorInfo *info);                    //增加亲密度
	bool CancelLeaveTutor(const ChgTutorInfo *info);              //取消离开师傅
	bool CancelDelStudent(const ChgTutorInfo *info);              //取消删除学生

	IDatabase* m_pDb;
	bool       m_bUpdate;
};

#endif // !defined(AFX_TUTORWORLD_H__BF555F23_4A75_4E15_A930_439880DFD88A__INCLUDED_)
