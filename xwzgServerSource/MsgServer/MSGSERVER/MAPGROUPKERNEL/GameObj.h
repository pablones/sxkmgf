// GameObj.h: interface for the CGameObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEOBJ_H__0E59B91E_AF23_466F_B7FC_63963D268670__INCLUDED_)
#define AFX_GAMEOBJ_H__0E59B91E_AF23_466F_B7FC_63963D268670__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <stdio.h>

#include "../share/define.h"
#include "basefunc.h"
#include "../../common/common.h"

char	szID[];

struct stTempUser
{
	OBJID idUser;
	OBJID idAcc;
	USHORT lookface;
	USHORT level;
	USHORT Fir_Job;
	USHORT Fir_JobLv;
	USHORT Sec_Job;
	USHORT Sec_JobLv;
	USHORT use_title;
	USHORT vipface;
	USHORT sex;
	USHORT Tutor_Lv;
	USHORT viplev;
	DWORD Exit_Time;
	DWORD max_score;
	DWORD TutorMorality;
	DWORD timeCheck;
	OBJID Syn_ID;
	NAMESTR name;
	NAMESTR Feel;
};

class CGameObj  
{
public:
	CGameObj();
	virtual ~CGameObj();
	
	virtual	OBJID	GetID()						PURE_VIRTUAL_FUNCTION_0

	virtual int		GetObjType()			{return m_nObjType;}
	virtual void	SetObjType(int nType)	{m_nObjType=nType;}

	static BOOL SafeCheck	(CGameObj* pObj);

private:
	int		m_nObjType;
};


#endif // !defined(AFX_GAMEOBJ_H__0E59B91E_AF23_466F_B7FC_63963D268670__INCLUDED_)
