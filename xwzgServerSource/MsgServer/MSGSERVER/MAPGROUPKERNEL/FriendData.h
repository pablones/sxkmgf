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
	UINT		nFriendShip;//[2010-06-07 goto]�Ѻö�
	FEELSTR		szFeel;//����
	UCHAR		dwLookface;//ͷ��
	USHORT		usLevel;//����ȼ�
	USHORT      FirJob;//��ְҵ
	USHORT      FirJobLev;//��ְҵ�ȼ�
	USHORT      SecJob;//��ְҵ
	USHORT      SecJobLev;//��ְҵ�ȼ�
	DWORD		SynCon;//����
	UINT		iUseTitle;//�ƺ�
	NAMESTR		szMateName;//��ż����
	UCHAR		iRelation;//��ϵ
	NAMESTR		szUserName;//�Լ�������
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
	LPCTSTR		GetFriendFeel()				{ return m_Info.szFeel; }		//[2010-06-07 goto]��������
	UINT		GetFriendShip()				{ return m_Info.nFriendShip; }	//[2010-06-07 goto]�Ѻö�
	USHORT		GetFriendType()				{ return m_Info.nFriendType; }	//�����
	UINT		GetFriendLookFace()			{ return m_Info.dwLookface; }	//����ͷ��
	USHORT		GetFriendLevel()			{ return m_Info.usLevel; }		//��������ȼ�
	USHORT		GetFriendFirJob()			{ return m_Info.FirJob; }		//������ְҵ
	USHORT		GetFriendFirJobLev()		{ return m_Info.FirJobLev; }	//������ְҵ�ȼ�
	USHORT		GetFriendSecJob()			{ return m_Info.SecJob; }		//���Ѹ�ְҵ
	USHORT		GetFriendSecJobLev()		{ return m_Info.SecJobLev; }	//���Ѹ�ְҵ�ȼ�
	DWORD		GetFriendSynID()			{ return m_Info.SynCon; }		//���Ѱ���ID
	UINT		GetFriendTitle()			{ return m_Info.iUseTitle; }	//���ѳƺ�
	UINT		GetFriendRelation()			{ return m_Info.iRelation; }	//����ѵĹ�ϵ
	LPCTSTR		GetFriendMateName()			{ return m_Info.szMateName; }	//������ż������
	//----------------------[ADD BY�������2008/12/17]------------------------
public:
	void		SetRecord(OBJID id, OBJID idUser, OBJID idFriend, LPCTSTR szFriendName,USHORT nFriendType, LPCTSTR szUserName, UINT nFriendShip = 0 ,LPCTSTR szFriendMateName = "��");
	void		SetFriendType(WORD nFriendType){ m_Info.nFriendType = nFriendType; }
	void		SetFriendID(OBJID idFriend){ m_Info.idFriend = idFriend; }
	void		SetFriendName(LPCTSTR szFriendName){ SafeCopy(m_Info.szFriendName, szFriendName, _MAX_NAMESIZE);}
	void		SetID(OBJID id){m_Info.id = id; }
	void		SetUserID(OBJID idUser){ m_Info.idUser = idUser; }
	void		SetFriendFeel(LPCTSTR szFriendFeel){ SafeCopy(m_Info.szFeel, szFriendFeel, _MAX_FEELSIZE);}
	bool		SetFriendShip(OBJID id, OBJID idFriend, IDatabase* pDb,int nFriendShip);
	//-----------------------------------------------------------------------

public: // add del
	OBJID		CreateRecord(OBJID idUser, OBJID idFriend, LPCTSTR szFriendName,  IDatabase* pDb,USHORT nFriendType = 1, UINT nFriendShip = 0 ,LPCTSTR szFriendMateName = "��");
	bool		DeleteRecord(IDatabase* pDb);

protected:
	FriendInfoStruct	m_Info;

public:	// ��ģ��
	void		SetFriendFace(int nFace);
	static bool			Init(IDatabase* pDb);
	static void			Final();
protected:
//	static IRecord*		m_pDefaultRecord;
};


#endif // !defined(AFX_FRIENDDATA_H__B256E96D_8D10_4E17_8572_0726F0249539__INCLUDED_)
