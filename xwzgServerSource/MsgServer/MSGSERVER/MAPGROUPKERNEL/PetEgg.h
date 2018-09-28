// PetEgg.h: interface for the CPetEgg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PETEGG_H__0645F0B2_2EFB_4058_8A9A_245E0F2863E9__INCLUDED_)
#define AFX_PETEGG_H__0645F0B2_2EFB_4058_8A9A_245E0F2863E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "GameObj.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "T_SingleObjSet2.h"
#include "PetEggData.h"

const int PETEGG_ONTIMER_INTEVAL = 1000;
//////////////////////////////////////////////////////////////////////
typedef	ISingleObjSet2<CPetEggData>		IPetEggSet;
typedef	CSingleObjSet2<CPetEggData>		CPetEggSet;
//////////////////////////////////////////////////////////////////////

class CPetEgg  
{
public:
	CPetEgg(PROCESS_ID idProcess, CUser* pUser);
	virtual ~CPetEgg();
private:
	CPetEgg();
public:
	static  CPetEgg*	CreateNew(PROCESS_ID idProcess, CUser* pUser);
	ULONG	ReleaseByOwner()	{ delete this; return 0; }
	bool	CreateAll();
	bool	SaveInfo();

	OBJID	AddNewPetData(CItem* pItem, int nGrow, int nNextFlush);
	bool	FlushPetData(OBJID idData, CItem* pItem, int nGrow, int nNextFlush);
	bool	FlushItemAllPetData(CItem* pItem, bool bUseProp=false);
	CItem*	GetItem(CPetEggData* pData);
	void	OnTimer(time_t tCurr);
	bool	CreatePetByEgg(OBJID idData, CItem* pEgg, int nGetType, int nGrow);
	void	DelPetEgg(OBJID idItem);
	void	SendToClient(CItem* pItem, int nType);

	int		GetAmount()	{ return m_setData->GetAmount(); }
	bool	GetInfoByIndex(EggInfoStruct* pInfo, int idx);
	bool	AppendPetData(const EggInfoStruct* pInfo);
protected:
	PROCESS_ID		m_idProcess;
	CUser*          m_pUser;
	IPetEggSet*		m_setData;
	clock_t			m_tLastOnTimer;
};

#endif // !defined(AFX_PETEGG_H__0645F0B2_2EFB_4058_8A9A_245E0F2863E9__INCLUDED_)
