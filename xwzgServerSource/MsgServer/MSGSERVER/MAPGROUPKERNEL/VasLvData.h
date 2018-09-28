#ifndef _VASLVDATA_H_
#define _VASLVDATA_H_

#if _MSC_VER > 1000
#pragma once
#endif
#include "../share/define.h"
#include "../../GameBaseCodeMT/ConstGameData.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"

enum VASLVDATA
{
	/*------------------VasLv±í-------begin--------------------------------------------------------------*/
	VASLVDATA_ID_=0,				// use for insert
		VASLVDATA_ACCID=1,			
		VASLVDATA_OBJID,
		VASLVDATA_AWARDITEM,
		VASLVDATA_VISCOU,
		VASLVDATA_MAXVIS,
		VASLVDATA_VISTIME,
		VASLVDATA_VISFLAG,
		VASLVDATA_QUESTIONID,
		VASLVDATA_VASCOUNT,
		/*-------------------VasLv±í---------end------------------------------------------------------------*/
};
char	szVaslvTable[];
char    szID[];
typedef	CGameData<VASLVDATA,szVaslvTable,szID>		CGameVaslvData;

typedef struct{
	OBJID	id;
	OBJID	account_id;
	OBJID	obj_id;
	UINT	awarditem;
	UINT	viscou;
	UINT	maxvis;
	UINT	vistime;
	UINT	visflag;
	UINT	questionid;
	UINT	vascount;
}UserVasLvInfo;

class IDatabase;
class IRecord;
class IRecordset;
class CVasLvData : public CGameObj  
{
public:
	CVasLvData();
	virtual ~CVasLvData();
	
public:
	static	CVasLvData* CreateNew(){ return new CVasLvData; }
	bool		Create			(OBJID accountid,OBJID objid, IDatabase* pDb);
	bool		DeleteRecord	(void);
	CVasLvData*	QueryRecord()	{ ASSERT(this); return this; }
	ULONG		ReleaseByOwner()	{ delete this; return 0; }
	bool		AppendInfo(UserVasLvInfo* pInfo);
	UserVasLvInfo* GetInfo()	{ LoadInfo();return &m_Info; }
public: // info
	bool		LoadInfo		(void);
	bool		SaveInfo		(void);
	virtual void		OnSaveInfo(){};
	int			GetInt(VASLVDATA idx);
	void		SetInt(VASLVDATA idx, int nData, bool bUpdate = false);
	
protected: // data & type
	UserVasLvInfo		m_Info;
	CGameVaslvData*		m_pData;
//	IRecord*			m_pRes;
	bool				m_bNeedUpdate;
};

#endif