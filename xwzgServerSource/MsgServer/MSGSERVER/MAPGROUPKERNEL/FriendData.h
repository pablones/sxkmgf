// FriendData.h: interface for the CFriendData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRIENDDATA_H__B256E96D_8D10_4E17_8572_0726F0249539__INCLUDED_)
#define AFX_FRIENDDATA_H__B256E96D_8D10_4E17_8572_0726F0249539__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameObj.h"
//#include "UserManager.h"
#include "../../GameBaseCodeMT/I_mydb.h"

struct FriendInfoStruct
{
	OBJID		id;
	OBJID		idUser;
	OBJID		idFriend;
	NAMESTR		szFriendName;
	WORD		nFriendType;
	UINT		nFriendShip;//[2010-06-07 goto]友好度
	FEELSTR		szFeel;//心情
	UCHAR		dwLookface;//头像
	USHORT		usLevel;//人物等级
	USHORT      FirJob;//主职业
	USHORT      FirJobLev;//主职业等级
	USHORT      SecJob;//副职业
	USHORT      SecJobLev;//副职业等级
	DWORD		SynCon;//帮派
	UINT		iUseTitle;//称号
	NAMESTR		szMateName;//配偶名字
	UCHAR		iRelation;//关系
	NAMESTR		szUserName;//自己的名字
};

class CFriendData : public CGameObj  
{
protected:
	CFriendData();
	virtual ~CFriendData();

public:
	bool Create(IRecordset* pRes, CUser* pUser, LPCTSTR szMyName);
	bool Create(IRecordset* pRes, stTempUser* pUser, LPCTSTR szMyName);

	bool	Create			(OBJID id, CUser* pTarget, LPCTSTR szMyName);
	bool	Create			(OBJID id, stTempUser* pTarget, LPCTSTR szMyName);

	FriendInfoStruct*	GetInfo	(void)		{return &m_Info;}
	bool	LoadInfo		(IRecord* pRes);
public: // get
	OBJID		GetID()						{ return m_Info.id; }
	OBJID		GetUserID()					{ return m_Info.idUser; }
	LPCTSTR		GetUserName()				{ return m_Info.szUserName; }
	OBJID		GetFriendID()				{ return m_Info.idFriend; }
	LPCTSTR		GetFriendName()				{ return m_Info.szFriendName; }
	LPCTSTR		GetFriendFeel()				{ return m_Info.szFeel; }		//[2010-06-07 goto]好友心情
	UINT		GetFriendShip()				{ return m_Info.nFriendShip; }	//[2010-06-07 goto]友好度
	USHORT		GetFriendType()				{ return m_Info.nFriendType; }	//分组号
	UINT		GetFriendLookFace()			{ return m_Info.dwLookface; }	//好友头像
	USHORT		GetFriendLevel()			{ return m_Info.usLevel; }		//好友人物等级
	USHORT		GetFriendFirJob()			{ return m_Info.FirJob; }		//好友主职业
	USHORT		GetFriendFirJobLev()		{ return m_Info.FirJobLev; }	//好友主职业等级
	USHORT		GetFriendSecJob()			{ return m_Info.SecJob; }		//好友副职业
	USHORT		GetFriendSecJobLev()		{ return m_Info.SecJobLev; }	//好友副职业等级
	DWORD		GetFriendSynID()			{ return m_Info.SynCon; }		//好友帮派ID
	UINT		GetFriendTitle()			{ return m_Info.iUseTitle; }	//好友称号
	UINT		GetFriendRelation()			{ return m_Info.iRelation; }	//与好友的关系
	LPCTSTR		GetFriendMateName()			{ return m_Info.szMateName; }	//好友配偶的名字
	//----------------------[ADD BY独孤求败2008/12/17]------------------------
public:
	void		SetRecord(OBJID id, OBJID idUser, OBJID idFriend, LPCTSTR szFriendName,USHORT nFriendType, LPCTSTR szUserName, UINT nFriendShip = 0 ,LPCTSTR szFriendMateName = "无");
	void		SetFriendType(WORD nFriendType){ m_Info.nFriendType = nFriendType; }
	void		SetFriendID(OBJID idFriend){ m_Info.idFriend = idFriend; }
	void		SetFriendName(LPCTSTR szFriendName){ SafeCopy(m_Info.szFriendName, szFriendName, _MAX_NAMESIZE);}
	void		SetID(OBJID id){m_Info.id = id; }
	void		SetUserID(OBJID idUser){ m_Info.idUser = idUser; }
	void		SetFriendFeel(LPCTSTR szFriendFeel){ SafeCopy(m_Info.szFeel, szFriendFeel, _MAX_FEELSIZE);}
	bool		SetFriendShip(OBJID id, OBJID idFriend, IDatabase* pDb,int nFriendShip);
	//-----------------------------------------------------------------------

public: // add del
	OBJID		CreateRecord(OBJID idUser, OBJID idFriend, LPCTSTR szFriendName,  IDatabase* pDb,USHORT nFriendType = 1, UINT nFriendShip = 0 ,LPCTSTR szFriendMateName = "无");
	bool		DeleteRecord(IDatabase* pDb);

protected:
	FriendInfoStruct	m_Info;

public:	// 表模板
	void		SetFriendFace(int nFace);
	static bool			Init(IDatabase* pDb);
	static void			Final();
protected:
//	static IRecord*		m_pDefaultRecord;
};


#endif // !defined(AFX_FRIENDDATA_H__B256E96D_8D10_4E17_8572_0726F0249539__INCLUDED_)
