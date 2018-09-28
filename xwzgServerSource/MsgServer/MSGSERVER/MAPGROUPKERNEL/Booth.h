// Booth.h: interface for the CBooth class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOOTH_H__8DDAB97F_6B97_43F3_9EA8_8DC3E505968B__INCLUDED_)
#define AFX_BOOTH_H__8DDAB97F_6B97_43F3_9EA8_8DC3E505968B__INCLUDED_

#include "I_Role.h"
#include "../share/NetworkDef.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int	BOOTH_LOOK		= 407;					// look for booth

class CGameMap;
class CBooth : IMapThing
{
protected:
	CBooth();
	virtual ~CBooth();
public:
	static CBooth*	CreateNew(PROCESS_ID idProcess, CUser* pUser, int nPosX, int nPosY, int nDir);
protected:
	int		FindIndex();
	void		SetIndex(int index,bool data);
	bool	Create(PROCESS_ID idProcess, CUser* pUser, int nPosX, int nPosY, int nDir);
	IMapThing*	QueryMapThing()									{ return (IMapThing*)this; }

public:
	bool	AddEudemon(OBJID idEudemon, int nMoney);
	bool	ChgEudemon(OBJID idEudemon, int nMoney);
	bool	AddItem(OBJID idItem, int nMoney, DWORD dwType = 0);
	bool	ChgItem(OBJID idItem, int nMoney, DWORD dwType = 0);
	bool 	DelItem(OBJID idItem, DWORD dwNumber = 0, CUser *pTarget = NULL);
	bool	DelEudemon(OBJID idEudemon);
	bool	BuyEudemon(CUser* pTarget, OBJID idEudemon, DWORD dwMoney);
	bool	BuyItem(CUser* pTarget, OBJID idItem, DWORD dwMoney, DWORD dwNumber = NULL);//[游途道标 2008/12/23]新增一个数量
	void	SendGoods(CUser* pTarget);
	void	EnterMap(int nPosX, int nPosY, int nDir);
	void	LeaveMap();

	void	SetCryOut(LPCTSTR szWords,bool bSend = false);
	void	SendCryOut(CUser* pUser);
	void	ChangeName(LPCTSTR szName,bool bSend = false);
	void	ClrLeaveWord();
	void	AddLeaveWord(LPCTSTR szSender,LPCTSTR szWord,bool bSend = false,CUser* pNeed = NULL);
	void	SendLeaveWord(CUser* pUser);
	void	SendInfo(CUser* pUser);

public: // IMapThing
	void Clear();
	bool IsHaveItem(OBJID idItem);
	void SetUserPoint(CUser *pUser);
	virtual void	ReleaseByOwner()							{ delete this;}
	virtual OBJID	GetID()										{ return m_id; }
	virtual OBJID	GetIDLock()									{ return m_idLook; }
	virtual int 	GetObjType()								{ return OBJ_BOOTH; }
	virtual int		GetPosX()									{ return m_nPosX; }
	virtual int		GetPosY()									{ return m_nPosY; }
	virtual void	SendShow(IRole* pRole);

	// return true: ppv返回对象指针
	virtual bool	QueryObj(OBJID idObjType, void** ppv)		{ *ppv = NULL; if(idObjType == OBJ_BOOTH) return *ppv=this,true; return false; }
	virtual bool	QueryRole(void** ppv)						{ *ppv = NULL; return false; }

protected:
	struct	GOODS_ST{
		OBJID	idItem;
		int		nType;
		int		nMoney;
		int		nindex;
		DWORD	dwNumber;
		DWORD	dwColor;
	};
	struct	EUDEMON_ST{
		OBJID	idEudemon;
		DWORD	nMoney;
		USHORT	gettype;		
		USHORT	lookface;
		UCHAR	needlev;
		UCHAR	failtime;
		UCHAR	sex;
		UCHAR	habit;
		UCHAR	lev;
		UCHAR	gift;
		UCHAR	bronTime;
		UCHAR	understand;
		char	szName[16];
	};
	typedef	vector<GOODS_ST>	GOODS_SET;
	typedef	vector<EUDEMON_ST>	EUDEMON_SET;
	GOODS_SET		m_setGoods;
	EUDEMON_SET     m_setEudemon;
	bool bHasItem[24];

protected:
	CUser*			m_pUser;
	OBJID			m_id;
	CGameMap*		m_pMap;
	int				m_nPosX;
	int				m_nPosY;
	int				m_nDir;

	char			m_szName[24];
	char			m_szCryOut[1024];
	char			m_szLeaveWord[20][1024];
	int				m_LeaveTop;
	int				m_LeaveEnd;

	OBJID			m_idLook;

protected: // ctrl
	PROCESS_ID		m_idProcess;
	bool			m_bEnterMap;

protected: // ctrl
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_BOOTH_H__8DDAB97F_6B97_43F3_9EA8_8DC3E505968B__INCLUDED_)
