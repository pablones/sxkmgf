// UserWarGame.h: interface for the CUserWarGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERWARGAME_H__FCA75641_8D08_4DDA_83FB_D874CC4AEF55__INCLUDED_)
#define AFX_USERWARGAME_H__FCA75641_8D08_4DDA_83FB_D874CC4AEF55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "../../GameBaseCodeMT/GameData.h"

typedef struct{
	OBJID	idUser;
	UINT	nArenaScore;//竞技场积分
	UINT	nArenaDayMask;//竞技场天掩码
}UserWarGameInfo;

class CUserWarGame  
{
	enum WARGAME_MASK
	{
		WARGAME_MASK_ARENA		= 0x0001
	};

public:
	static	CUserWarGame*	CreateNew(PROCESS_ID idProcess, CUser* pUser);
	ULONG	ReleaseByOwner()	{ delete this; return 0; }
	CUserWarGame(PROCESS_ID idProcess, CUser* pUser);
	virtual ~CUserWarGame();

private:
	CUserWarGame();
protected:
	bool	Create					(IDatabase* pDb);
	bool	InsertNewRecord			(IDatabase* pDb);
	bool	LoadInfo				();
	bool	SaveInfo				();

public:
	UINT	GetArenaScore()			{ return m_Info.nArenaScore; }
	void	SetArenaScore(UINT nScore, bool bUpdate=false);

	void	SetWarGameMask(UINT nData, bool bUpdate=false);
	void	AddWarGameMask(int nType);
	bool	HasArenaMask()			{ return (m_Info.nArenaDayMask & WARGAME_MASK_ARENA) != 0; }

	UserWarGameInfo* GetInfo()		{ return &m_Info; }
	void	AppendInfo(UserWarGameInfo* pInfo);
protected:
	UserWarGameInfo		m_Info;
	IRecord*			m_pRes;
	bool				m_bNeedUpdate;
	CUser*				m_pUser;
	PROCESS_ID			m_idProcess;
	friend class CUser;
};

#endif // !defined(AFX_USERWARGAME_H__FCA75641_8D08_4DDA_83FB_D874CC4AEF55__INCLUDED_)
