// PVPManager.h: interface for the CPVPManager class.
//
//////////////////////////////////////////////////////////////////////

#include "../share/define.h"
#include <list>
using namespace std;

#if !defined(AFX_PVPMANAGER_H__168232B4_C591_4AB7_959C_540941837F11__INCLUDED_)
#define AFX_PVPMANAGER_H__168232B4_C591_4AB7_959C_540941837F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct PVPData{
	DWORD nMode; //模式
	DWORD nBegainTime; //开始时间
	DWORD nOutTime1;
	DWORD nOutTime2;

	OBJID userID1; //玩家1
	OBJID userID2; //玩家2
	OBJID flagID; //旗帜的ID
};

typedef list<PVPData*> PVPDataSet;

class CPVPManager  
{
public:

	CPVPManager(PROCESS_ID id);
	virtual ~CPVPManager();
	void doPVP(); //地图组调用 决斗时间管理
	void EndPVP(OBJID id1,OBJID winner = 0, bool bKill = false);//结束winner是胜利者 为0时id1的对手为胜利者
	bool findPVP(OBJID id1);//找到玩家是否有决斗
	void AddPVP(OBJID id1,OBJID id2,int mode,OBJID flagID);//添加决斗
	void SetLong(OBJID id1); //设置超距离
	void ClrLong(OBJID id1); //清楚超距离
protected:
	PVPDataSet m_pAllPVP;
	DWORD thisChkTime;
	PROCESS_ID	m_idProcess;
};

#endif // !defined(AFX_PVPMANAGER_H__168232B4_C591_4AB7_959C_540941837F11__INCLUDED_)
