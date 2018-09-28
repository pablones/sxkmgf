// UserTitle.h: interface for the CUserTitle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERTITLE_H__0F49760B_66EC_40C8_9719_417927A5C549__INCLUDED_)
#define AFX_USERTITLE_H__0F49760B_66EC_40C8_9719_417927A5C549__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "GameObj.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "T_SingleObjSet2.h"
#include "Myheap.h"
#include "UserTitleData.h"
#include "TitleTypeData.h"

const int _TITLE_VIP = 7001;
const int _TITLE_SYNDICATE = 5001;
//////////////////////////////////////////////////////////////////////
typedef	ISingleObjSet2<CUserTitleData>		ITitleSet;
typedef	CSingleObjSet2<CUserTitleData>		CTitleSet;
//////////////////////////////////////////////////////////////////////

class CUserTitle  
{
protected:
	CUserTitle(PROCESS_ID idProcess, CUser* pUser);
	virtual ~CUserTitle();
	
private:
	CUserTitle();
public:
	static CUserTitle* CreateNew(PROCESS_ID idProcess, CUser* pUser);
	static CTitleTypeData* FindTitleType(int nType);
	ULONG	ReleaseByOwner()	{ delete this; return 0; }
	bool	CreateAll();
	int		SendInfoToClient();
	bool	SaveInfo();

	CUserTitleData* FindTitle(int nType);
	OBJID			AddTitle(int nType, int nLev=1);
	bool			TitleUpLevel(int nType);
	bool			DelTitleByType(int nType);
	int				GetTitleLev(int nType);
	bool			IsValidTitle(int nType, int nLev);
	bool			ChangeTitleLevel(int nType, int nLev);
	int				GetAmount()	{ return m_setData->GetAmount(); }
	bool			GetInfoByIndex(ST_TITLE_DATA* pInfo, int idx);
	bool			AppendTitle(const ST_TITLE_DATA* pInfo);

	//goto 操作离线用户的称号,用于帮派等
	static bool AddOrSetTitle(OBJID idUser, int nType, int nLev, IDatabase* pDb);
	static bool DelOffLineUserTitle(OBJID idUser, int nType, IDatabase* pDb);
protected:
	PROCESS_ID		m_idProcess;
	CUser*          m_pUser;
	ITitleSet*		m_setData;
};

#endif // !defined(AFX_USERTITLE_H__0F49760B_66EC_40C8_9719_417927A5C549__INCLUDED_)
