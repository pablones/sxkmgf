// TaskDetailData.h: interface for the CTaskDetailData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKDETAILDATA_H__87F05AAE_F2F6_422E_83D0_6D911519C5A2__INCLUDED_)
#define AFX_TASKDETAILDATA_H__87F05AAE_F2F6_422E_83D0_6D911519C5A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable:4786)
#include "../share/define.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"
#include "MyHeap.h"
#include "ItemData.h"
#include "NetWork/MsgPlayerTask.h"
#include <vector>

#define COMPLETENUM_LIMIT 65535
//20070511修罗:PHASE_LIMIT策划需求.原255
#define PHASE_LIMIT 65535 
//////////////////////////////////////////////////////////////////////
enum TASKDETAILDATA 
{
	TASKDETAILDATA_ID = 0,			// 任务ID
	TASKDETAILDATA_USERID,
	TASKDETAILDATA_TASKID,
	TASKDETAILDATA_TASKPHASE,
	TASKDETAILDATA_TASKBEGINTIME,
	TASKDETAILDATA_DATA1,
	TASKDETAILDATA_DATA2,
	TASKDETAILDATA_DATA3,
	TASKDETAILDATA_DATA4,
	TASKDETAILDATA_DATA5,
	TASKDETAILDATA_DATA6,
	TASKDETAILDATA_ISREC,
	TASKDETAILDATA_RANDSTEP,
	TASKDETAILDATA_SPENDENETYPE,//goto 消耗精力类型
};
//////////////////////////////////////////////////////////////////////
struct ST_TASKDETAILDATA
{
	OBJID idUser;
	OBJID idTask;
	int	  nTaskPhase;
	DWORD dwBegainTime;
	short	data1;
	short   data2;
	short   data3;
	short   data4;
	short   data5;
	short   data6;
	short   isRec;
	DWORD   randStep;
	short	spendEneType;//goto 消耗精力类型
	OBJID   id;
};
char	szTaskDetailTable[];
typedef CGameData<TASKDETAILDATA, szTaskDetailTable, szID>		CGameTaskDetailData;

//////////////////////////////////////////////////////////////////////
class IDatabase;
class CItem;

class CTaskDetailData  : public CGameObj
{
public:
	bool Create(IRecordset* pRes, IDatabase* pDb);
	bool DeleteRecord()	{ return m_pData->DeleteRecord(); }
public:
	OBJID	GetID()		{return m_pData->GetKey();}
	int		GetInt(TASKDETAILDATA idx)	{ return m_pData->GetInt(idx); }
	LPCTSTR	GetStr(TASKDETAILDATA idx)	{ return m_pData->GetStr(idx); }
	void	SetInt(TASKDETAILDATA idx, int nData, bool bUpdate = false);
	void	SetStr(TASKDETAILDATA idx, LPCTSTR szData, int nSize, bool bUpdate = false);
	bool	Release()				{ delete this; return true; }
	void    UpdateData();
	bool	GetInfo(ST_TASKDETAILDATA& info);
public:
	bool CreateRecord( IRecord* pDefault, const ST_TASKDETAILDATA* pInfo ,bool bInset = true);//06.11.21王玉波添加
	bool CreateRecord(ST_TASKDETAILDATA * pInfo,IDatabase * pDb);
	CTaskDetailData();
	virtual ~CTaskDetailData();
	static CTaskDetailData* CreateNew(){return new CTaskDetailData;}
protected:
	bool					m_bNeedUpdate;
	CGameTaskDetailData*	m_pData;
	MYHEAP_DECLARATION(s_heap)
};

class CUserPetData  : public CGameObj
{
public:
	bool Create(IRecordset* pRes, IDatabase* pDb);
	bool DeleteRecord()	{ return m_pData->DeleteRecord(); }
public:
	void	SetLock(bool bLock)     { m_bLock = bLock; }
	bool    isLocked()				{ return m_bLock; }
	OBJID	GetID()		{return m_pData->GetKey();}
	int		GetInt(EUDEMONDATA idx)	{ return m_pData->GetInt(idx); }
	LPCTSTR	GetStr(EUDEMONDATA idx)	{ return m_pData->GetStr(idx); }
	void	SetInt(EUDEMONDATA idx, int nData, bool bUpdate = false);//	{ m_pData->SetInt(idx, nData); if (bUpdate) m_pData->Update(); }
	void	SetStr(EUDEMONDATA idx, LPCTSTR szData, int nSize, bool bUpdate = false)	{ m_pData->SetStr(idx, szData, nSize); if (bUpdate) m_pData->Update(); }
	bool	Release()				{ delete this; return true; }

	bool	Create(IRecord* pDefaultRes, OBJID id);
public:
	bool CreateRecord( IRecord* pDefault, const EudemonData* pInfo );//06.11.21王玉波添加
	int	 ProcGift1001(int nType, int oldValue,int valueMask /*= valueMask_ALL*/);
//	bool CreateRecord(EudemonData * pInfo,IDatabase * pDb);
	bool GetData(EudemonData& data);
	bool SetData(EudemonData& data,bool bInset = true);
	void SaveData()   { m_pData->Update();}
	bool IsSoulState();//是否是附体状态
	bool SetSkill(int nIndex, int nData, bool bUpdate=false);
	CUserPetData();
	virtual ~CUserPetData();
	static CUserPetData* CreateNew(){return new CUserPetData;}
protected:
	CEudemonData*	m_pData;
	bool            m_bLock;
	MYHEAP_DECLARATION(s_heap)
};


class CUserPeifangData  : public CGameObj
{
public:
	bool Create(IRecordset* pRes, IDatabase* pDb);
	bool DeleteRecord()	{ return m_pData->DeleteRecord(); }
public:
	OBJID	GetID()		{return m_pData->GetKey();}
	int		GetInt(PEIFANGDATA idx)	{ return m_pData->GetInt(idx); }
	LPCTSTR	GetStr(PEIFANGDATA idx)	{ return m_pData->GetStr(idx); }
	void	SetInt(PEIFANGDATA idx, int nData, bool bUpdate = false)	{ m_pData->SetInt(idx, nData); if (bUpdate) m_pData->Update(); }
	void	SetStr(PEIFANGDATA idx, LPCTSTR szData, int nSize, bool bUpdate = false)	{ m_pData->SetStr(idx, szData, nSize); if (bUpdate) m_pData->Update(); }
	bool	Release()				{ delete this; return true; }
	bool	Create(IRecord* pDefaultRes, OBJID id);
	void	ClearUpdateFlags()						{ m_pData->ClearUpdateFlags(); }
	bool    GetInfo(ST_PEIFANGDATA& info);
	
public:
	bool CreateRecord( IRecord* pDefault, OBJID idUser , OBJID idPeifang );
	CUserPeifangData();
	virtual ~CUserPeifangData();
	static CUserPeifangData* CreateNew(){return new CUserPeifangData;}
protected:
	CPeifangData*	m_pData;
	MYHEAP_DECLARATION(s_heap)
};


#endif // !defined(AFX_TASKDETAILDATA_H__87F05AAE_F2F6_422E_83D0_6D911519C5A2__INCLUDED_)
