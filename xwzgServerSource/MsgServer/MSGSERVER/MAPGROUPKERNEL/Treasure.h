// Treasure.h: interface for the CTreasure class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREASURE_H__CFA8EE15_5BA9_41F7_AEF6_3EDE657F3026__INCLUDED_)
#define AFX_TREASURE_H__CFA8EE15_5BA9_41F7_AEF6_3EDE657F3026__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "GameObj.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "T_SingleObjSet2.h"
#include "Myheap.h"
#include "TreasureData.h"
#include "TreasureType.h"
#include "MsgLottery.h"
//////////////////////////////////////////////////////////////////////
typedef	ISingleObjSet2<CTreasureData>		ITreasureSet;
typedef	CSingleObjSet2<CTreasureData>		CTreasureSet;
//////////////////////////////////////////////////////////////////////
const USHORT _MAXTREASURE_COUNT	= 200;//彩票包裹空间上限
const UINT	 _TREASURE_PRICE[3][3] = {{5,45,200},{10,90,400},{20,180,800}};//[剑徒,剑客,剑豪][1次,10次,50次]
//用于地图组发送
struct TreasureInfoStruct
{
	OBJID	id;
	OBJID	idOwner;
	OBJID	itemType;
	int		nAmount;
	int		nDivide;
};

class CTreasure  
{
public:
	CTreasure(PROCESS_ID idProcess, CUser* pUser);
	virtual ~CTreasure();
private:
	CTreasure();
public:
	static  CTreasure*		CreateNew(PROCESS_ID idProcess, CUser* pUser);
	static  CTreasureType*	FindTreasureType(int nGroup, int nRand);
	ULONG	ReleaseByOwner()	{ delete this; return 0; }
	bool	CreateAll();
	int		SendInfoToClient();
	bool	SaveInfo();
	OBJID	AddTreasure(int nType, int nAmount, int nDivide, CMsgLottery* msgAdd, CMsgLottery* msgSynchor);
	CTreasureData* FindTreasureData(OBJID idTreasure);
	int		GetFreeSpaceCount();
	bool	HasEnoughSpace(int nLotteryCount);
	bool	AwardLotteryItem(int nAmount, int nGroup);
	bool	GetTreasure(OBJID idTreasure);
	bool	DelTreasure(OBJID idTreasure);
	void	BroadCastTreasure(LPCTSTR szUserName, OBJID itemType, int nGroup);
	int		GetTreasurePrice(int nGroup, int nAmount);
	CTreasureType* FindTreasureTypeByType(OBJID nType, int nGroup);
	int		GetAmount()	{ return m_setData->GetAmount(); }
	bool	GetInfoByIndex(TreasureInfoStruct* pInfo, int idx);
	bool	AppendTreasure(const TreasureInfoStruct* pInfo);
protected:
	typedef std::vector<CTreasureData*>	TREASURE_SET;
	bool	CollectTreasureSet(TREASURE_SET& setTreasure, OBJID itemType);
protected:
	PROCESS_ID		m_idProcess;
	CUser*          m_pUser;
	ITreasureSet*	m_setData;
};

#endif // !defined(AFX_TREASURE_H__CFA8EE15_5BA9_41F7_AEF6_3EDE657F3026__INCLUDED_)
