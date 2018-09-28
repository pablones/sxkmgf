// Treasure.h: interface for the CTreasure class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREASURE_H__CFA8EE15_5BA9_41F7_AEF6_3EDE657F302R__INCLUDED_)
#define AFX_TREASURE_H__CFA8EE15_5BA9_41F7_AEF6_3EDE657F302R__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "GameObj.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "T_SingleObjSet2.h"
#include "Myheap.h"
#include "OnlineGiftData.h"

//用于地图组发送
struct OnlineGiftInfoStruct
{
	OBJID	id;
	OBJID   userid;
	OBJID	gift1;
	OBJID	gift2;
	OBJID	gift3;
	OBJID	gift4;
	OBJID	gift5;
	OBJID	gift6;
	int		taskmask;
	int		date;
	int     amount;
};
class COnlineGift  
{
public:
	COnlineGift(PROCESS_ID idProcess, CUser* pUser);
	virtual ~COnlineGift();
private:
	COnlineGift();
public:
	//用于地图组传送
	bool	AppendOnlinegiftInfo(const OnlineGiftInfoStruct* pInfo);
	bool	GetOnlinegiftInfo(OnlineGiftInfoStruct* pInfo);
	static  COnlineGift*		CreateNew(PROCESS_ID idProcess, CUser* pUser);
	ULONG	ReleaseByOwner()	{ delete this; return 0; }
	

	bool	CreateAll();

	bool	SaveInfo();

	bool AddGifts();
	void SetTaskMask(int nTaskMask);
	int  GetTaskMask();
	int GetDate();
	void SetGift(OBJID Gift1ID, OBJID Gift2ID, OBJID Gift3ID, OBJID Gift4ID, OBJID Gift5ID,OBJID Gift6ID);
	void ClnGifts();
	OBJID GetGiftID(int idx);
	void SetDate(int nDate);
	bool HasRecord();
	void AddDayAmount();
	int GetDayAmount();
	void SetDayAmount(int nData);
	void AddDayAmount(int nData);
protected:
	PROCESS_ID		m_idProcess;
	CUser*          m_pUser;
	COnlineGiftData*	m_setData;
};

#endif // !defined(AFX_TREASURE_H__CFA8EE15_5BA9_41F7_AEF6_3EDE657F302R__INCLUDED_)
