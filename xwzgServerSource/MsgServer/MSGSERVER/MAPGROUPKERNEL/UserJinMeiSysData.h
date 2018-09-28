// UserJinMeiSysData.h: interface for the CUserJinMeiSysData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERJINMEISYSDATA_H__2DA44E8A_BBA8_481A_A1F7_D15153C8A101__INCLUDED_)
#define AFX_USERJINMEISYSDATA_H__2DA44E8A_BBA8_481A_A1F7_D15153C8A101__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include "GameObj.h"
#include "../../GameBaseCodeMT/GameData.h"

enum USERJINMEISYSDATA
{
	USERJINMEISYSDATA_ID = 0,
	USERJINMEISYSDATA_IDUSER,
	USERJINMEISYSDATA_TYPE,
	USERJINMEISYSDATA_LEV, 
	USERJINMEISYSDATA_STENGTHLEV,
	USERJINMEISYSDATA_STARTTIME,
	USERJINMEISYSDATA_USETIME,
	USERJINMEISYSDATA_LEARNSTATE,
	USERJINMEISYSDATA_ADDQUEUETIME,
};

struct JinmeiInfoStruct
{
	OBJID id;
	OBJID idUser;
	USHORT nType;
	USHORT nLev;
	USHORT nStrLev;
	DWORD dwStartTime;
	DWORD dwUserTime;
	USHORT nLeanState;
	DWORD addQueueTime;
};

char szUserJinmeidataTable[];
typedef	CGameData<USERJINMEISYSDATA, szUserJinmeidataTable, szID>	CGameUserJinmeiData;

class CUserJinMeiSysData  
{
public:
	CUserJinMeiSysData();
	virtual ~CUserJinMeiSysData();

public:
	static CUserJinMeiSysData*	CreateNew()		{ return new CUserJinMeiSysData; }
	void	ReleaseByOwner()		{ delete this; }
	
	bool	Create(IRecordset* pRecordset);
	bool	Create(IRecordset* pDefaultRecord, OBJID idNewKey);
	bool	Create(IRecordset* pDefaultRecord,JinmeiInfoStruct* pInfo,bool bInsert = true);
	OBJID	GetID()				{ CHECKF(m_pData); return m_pData->GetID(); }
	bool	Update()			{ CHECKF(m_pData); return m_pData->Update(); }
	void	ClearUpdateFlags()	{ CHECK(m_pData); m_pData->ClearUpdateFlags(); }
	OBJID	InsertRecord()		{ CHECKF(m_pData); return m_pData->InsertRecord(); }
	bool	DeleteRecord()		{ CHECKF(m_pData); return m_pData->DeleteRecord(); }
	int		GetInt(USERJINMEISYSDATA nDataName);
	void	SetInt(USERJINMEISYSDATA idx, int nData);
	bool    GetInfo(JinmeiInfoStruct& info);

protected:
	CGameUserJinmeiData*	m_pData;
};

#endif // !defined(AFX_USERJINMEISYSDATA_H__2DA44E8A_BBA8_481A_A1F7_D15153C8A101__INCLUDED_)
