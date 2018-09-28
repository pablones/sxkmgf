// Compensation.h: interface for the CCompensation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPENSATION_H__B2AC7872_D577_4708_99D0_9287B6DEE021__INCLUDED_)
#define AFX_COMPENSATION_H__B2AC7872_D577_4708_99D0_9287B6DEE021__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "../../GameBaseCodeMT/ConstGameData.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"

enum COMPENSATIONTYPE
{
	COMPENSATIONTYPE_ITEM = 1,
	COMPENSATIONTYPE_EUDEMON,
	COMPENSATIONTYPE_GOLD1, //����
	COMPENSATIONTYPE_GOLD2, //����
	COMPENSATIONTYPE_GOLD3, //�鱦
	COMPENSATIONTYPE_GOLD5, //Ԫ��
	COMPENSATIONTYPE_PORFFER, //����
	COMPENSATIONTYPE_SYNPORFFER, //�ﹱ
	COMPENSATIONTYPE_EXP, //����
};

enum COMPENSATIONSTATUS{
	COMPENSATIONSTATUS_NEW,
	COMPENSATIONSTATUS_WAITE,
};

enum COMPENSATIONDATA{
	COMPENSATIONDATA_ID = 0,
	COMPENSATIONDATA_USERID,
	COMPENSATIONDATA_TYPE, 
	COMPENSATIONDATA_STATUS,
	COMPENSATIONDATA_TYPEID, //�������Ʒ����
	COMPENSATIONDATA_NUM, //����
	COMPENSATIONDATA_WRITETIME,
	COMPENSATIONDATA_PARAM, //��ϸ����
};
//��Ʒ "kidnap quality star addlev itembase1 itembase1 itemAddition1 itemAddition2 itemAddition3 itemAddition4 itemAddition5 itemAddition6 itemAddition7 itemAddition8 itemGem1 itemGem2 itemGem3"
//���� "level exp sex gettype understand habit gift borntime grow born strchg intchg stachg spichg agichg skill1 skill2 skill3 skill4 skill5 skill6 skill7"

char	szCompensationTable[];
typedef		CGameData<COMPENSATIONDATA, szCompensationTable, szID>	CCompensationData;

class CCompensation  
{
public:
	CCompensation();
	virtual ~CCompensation();

	static CCompensation* CreateNew() { CCompensation* ptr=new CCompensation; return ptr;}
	
	bool	Create(IRecordset* pRes);
	ULONG	Release()				{ delete this; return 0; }
	
//	void    GetInfo(InstanceUseTimeInfo* pInfo);
	void    Destroy();
	void    Update()  {if(m_pData) m_pData->Update();}

public:
	int		GetInt(COMPENSATIONDATA idx)                  { return m_pData->GetInt(idx); }
	LPCTSTR	GetStr(COMPENSATIONDATA idx)		   	      { return m_pData->GetStr(idx); }
	void SetInt(COMPENSATIONDATA idx,int nData)			  { m_pData->SetInt(idx,nData);m_pData->Update();}

private:	
	CCompensationData*    m_pData;
};

#endif // !defined(AFX_COMPENSATION_H__B2AC7872_D577_4708_99D0_9287B6DEE021__INCLUDED_)
