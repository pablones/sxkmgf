// UserData.h: interface for the CUserData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERDATA_H__6C3EB549_7325_4A50_82DF_FD07A49E1300__INCLUDED_)
#define AFX_USERDATA_H__6C3EB549_7325_4A50_82DF_FD07A49E1300__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "../share/define.h"
#include "GameObj.h"
#include "windows.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "..\SHARE\define.h"	// Added by ClassView

const int		MEDAL_BUFSIZE		= 64+1;
const int		TITLE_BUFSIZE		= 64+1;

//////////////////////////////////////////////////////////////////////
enum USERDATA{
	USERDATA_NAME=1,
};

char	szUserTable[];

//时空之泪玩家表修改如下：
struct	UserInfoStruct
{
	OBJID			id;
	OBJID			idAccount;
	DWORD			dwLast_login;
	DWORD			dwCreateTime;
	DWORD			dwExitTime;
	DWORD			dwAllOnlineTime;
	DWORD			dwAllAutoTime;
	DWORD			dwAllDoubleTime;
	NAMESTR			szName;
	OBJID			idMate;	
	UCHAR			sex;
	UCHAR			dwLookface;
	char			szFeel[24];//[2010-06-04 goto]玩家心情
	_int64			i64HoldTitle;
	UINT			iUseTitle;
	USHORT          Vit;
	USHORT          MaxVit;
	USHORT          Ene;
	USHORT          MaxEne;
	USHORT			usLevel;
	_int64			i64Exp;
	DWORD			nLife;
	DWORD			nMana;
	USHORT          nSp;
	UCHAR           CurJob;
	USHORT          FirJob;
	USHORT          FirJobLev;
	USHORT          FirJobPhase;	
	DWORD          FirJobStr;
	DWORD          FirJobInt;
	DWORD          FirJobSta;
	DWORD          FirJobSpi;
	DWORD          FirJobAgi;
	int          FirJobPot;
	DWORD           FirJobCon;
	USHORT          SecJob;
	USHORT          SecJobLev;
	USHORT          SecJobPhase;
	DWORD          SecJobStr;
	DWORD          SecJobInt;
	DWORD          SecJobSta;
	DWORD          SecJobSpi;
	DWORD          SecJobAgi;
	int          SecJobPot;
	DWORD           SecJobCon;
	OBJID			idRecordmap;
	USHORT			usRecordX;
	USHORT			usRecordY;
	OBJID			idFamily;
	OBJID			idSyn;
	OBJID			idNation;
	DWORD			GoodEvil;
	DWORD			nPk;
	UCHAR			ucPkMode;
/*	OBJID			ucPkMode;*/
	DWORD			FamilyCon;
	DWORD			SynCon;
	DWORD			NationCon;
	DWORD			SceCon;
	DWORD			ucTutorLev;
	OBJID			idTutor;
	DWORD			nPackageLev;
	DWORD			nBankLev;	
	DWORD           money1;
	DWORD           money2;
	DWORD           money3;
	DWORD           money4;
	DWORD           money5;
	DWORD           moneySave;
	char            depotCode[20];
	_int64			i64TaskMask;
	_int64			i64DayTaskMask;
	_int64			i64DayTaskMask2;
	_int64			i64WeekTaskMask;
	_int64          i64NewGiveMask;
	_int64          i64NewGiveMask2;
	UCHAR           InfoState;
	char            InfoSex[16];
	char            InfoAge[16];
	char            InfoSign[16];
	char            InfoStar[16];
	char            InfoNation[16];
	char            InfoPro[16];
	char            InfoCity[16];
	char            InfoJob[16];
	char			description[ 128 ];

	DWORD           dwMaxScore;
	DWORD           dwMaxPhyatt;
	DWORD           dwMaxMagatt;
	DWORD           dwMaxMaxlife;
	DWORD           dwMaxCrit;
	DWORD           dwMaxHitrate;


	DWORD			vipState;
	DWORD			vipLev;
	DWORD			vipTime;
	//[2010-08-27 goto]特殊大红大蓝的增量，最后一位表示类型
	DWORD			dwHpAdd;
	DWORD			dwMpAdd;
	DWORD			dwPetAdd;

	DWORD			dwLeaveExp;
	

	DWORD			dwStatus;
	_int64			i64Effect;
	_int64			i64Effect2;
	BOOL			bIsAllowTeam;
	BOOL			bClosing;

	USHORT          usSpeed;
	int				newKardState[5];
	DWORD           onlineaward;
	int				Identity;
	int				Fatigue;
	int				FatiTime;
	OBJID			Vipface;	// vip脸
	OBJID			VipfaceTime;//vip脸的时间
	DWORD			toMapid;
	DWORD			idTeam;//专为副本系统便利准备的
	DWORD			MonsterKill;
	int				EverydayOL;

	char			szItemLockPwd[24];
	_int64			i64BuyASales;//购买过的特价商品
	int             nJinmeiValue;
//	int				Worktimes;
	int				nMorality;//师德
};

class IRecord;
class IDatabase;
class CUserData
{
public:
	CUserData();
	virtual ~CUserData();
//如果要实现在DEBUG时才能用，把函数体加上编译控制。
	void			SetName			(const char* pszName);
///////////////////////////////////////////////////////////////////////////////////
protected:
	bool	Create					(OBJID idUser, IDatabase* pDb);
	bool	LoadInfo				(void);
	bool	SaveInfo				(void);

public: 
	void SetFati(int nData);
	OBJID GetVipfaceTime();
	void SetVipfaceTime(OBJID nTime,bool bUpdate = false);
	void SetVipface(int nData);
	DWORD GetVipface()				{return m_Info.Vipface;}

	void SetSynCon(int nData, int bUpdate = false);
	DWORD GetSynCon();
	DWORD GetAwardTime();
	void SetOnlineAward(int ntime,bool bUpdate = false);
	void SetFatiTime(int FatiTime, bool bUpdate = false);
	int	 GetFatiTime() {return m_Info.FatiTime;}

	int getFati();
	void setFati(int iFatigue);
	UserInfoStruct&		GetInfo()	{ return m_Info; }

	_int64 GetHoldTitle() const {return m_Info.i64HoldTitle;}//[游途道标 2008/10/13]获取当前拥有的称号
	void SetHoldTitle(UINT nPower, BOOL bUpdate = true, IDatabase* pDb = NULL);//[游途道标 2008/10/13]设置当前拥有的称号
	void AddHoldTitle(UINT nPower, BOOL bUpdate = true, IDatabase* pDb = NULL);
	void SubHoldTitle(UINT nPower, BOOL bUpdate = true, IDatabase* pDb = NULL);
	int CmpHoldTitle(UINT nPower);
	UINT GetUseTitle() const {return m_Info.iUseTitle;}//[游途道标 2008/10/13]获取当前使用的称号
	void SetUseTitle(int nPower, BOOL bUpdate = false);//[游途道标 2008/10/13]设置当前使用的称号
// 	void			SetGiftPoint(int nGiftPoint,BOOL bUpdate = false);
// 	int				GetGiftPoint(){return m_Info.giftpoint;}

	//通用数据
	int				GetAccountID()							{ return m_Info.idAccount; }
	int				GetID()									{ return m_Info.id; }

	DWORD			GetLastLogin()		{ return m_Info.dwLast_login; }
	void			SetLastLogin(int nTime = 0 );

	DWORD			GetLastLogout()		{ return m_Info.dwExitTime; }
	void			SetLastLogout();

	DWORD			GetLeaveExp()		{ return m_Info.dwLeaveExp; }
	void			SetLeaveExp(DWORD dwData);

	void			SetCreateTime( DWORD dwData, BOOL bUpdate = false );
	DWORD			GetCreateTime ()	{ return m_Info.dwCreateTime; }

	void			SetExitTime( DWORD dwData, BOOL bUpdate = false );
	DWORD			GetExitTime ()	{ return m_Info.dwExitTime; }
	
	void			SetAllOnlineTime( DWORD nAllOnlineTime, BOOL bUpdate = false );
	DWORD			GetAllOnlineTime ()	{ return m_Info.dwAllOnlineTime; }
	
	void			SetAllAddExpTime( DWORD nAllAddExpTime, BOOL bUpdate = false );
	DWORD			GetAllAddExpTime ()				{ return m_Info.dwAllDoubleTime; }

	void			SetMate			(OBJID idMate, BOOL bUpdate = false);
	OBJID			GetMate			()	{ return m_Info.idMate; }
	
	void			SetDepotCode	(LPCTSTR szDepotCode, BOOL bUpdate = false);	// 很少使用，默认直接修改数据库
	char*			GetDepotCode	()	{ return m_Info.depotCode; }
	
	void			SetPk			(int nPk, BOOL bUpdate = false);	
	int				GetPk			()	{return m_Info.nPk;}

	void			SetMonsterkill	(DWORD nMonsterkill, BOOL bUpdate = false);	
	DWORD			GetMonsterkill	()	{return m_Info.MonsterKill;}

	void			SetNationCon	(int nPk, BOOL bUpdate = false);	
	int				GetNationCon	()	{return m_Info.NationCon;}

	void			SetGoodEvil		(int nData, BOOL bUpdate = false);	
	int				GetGoodEvil		()	{return m_Info.GoodEvil;}

	void			SetLev			( int nLev, BOOL bUpdate = false );
	USHORT			GetLev			()	{ return m_Info.usLevel; }

	void			SetExp			( _int64 i64Exp, BOOL bUpdate = false );
	_int64			GetExp()		{ return m_Info.i64Exp; }

	void			SetCurJob	(int curJob, BOOL bUpdate = false);
	DWORD			GetCurJob	()	{ return m_Info.CurJob; }	

	void			SetVit	(int nVit, BOOL bUpdate = false);
	DWORD			GetVit	()	{ return m_Info.Vit; }
	DWORD			GetMaxVit	()	{ return m_Info.MaxVit; }

	void			SetEne	(int nEne, BOOL bUpdate = false);
	DWORD			GetEne	()	{ return m_Info.Ene; }
	DWORD			GetMaxEne	()	{ return m_Info.MaxEne; }

	void			SetMaxScore	(DWORD dwMaxScore, BOOL bUpdate = false);
	DWORD			GetMaxScore	()	{ return m_Info.dwMaxScore; }

	void			SetMaxPhyatt	(DWORD dwMaxPhyatt, BOOL bUpdate = false);
	DWORD			GetMaxPhyatt	()	{ return m_Info.dwMaxPhyatt; }

	void			SetMaxMagatt	(DWORD dwMaxMagatt, BOOL bUpdate = false);
	DWORD			GetMaxMagatt	()	{ return m_Info.dwMaxMagatt; }

	void			SetMaxMaxlife	(DWORD dwMaxMaxlife, BOOL bUpdate = false);
	DWORD			GetMaxMaxlife	()	{ return m_Info.dwMaxMaxlife; }

	void			SetMaxCrit	(DWORD dwMaxCrit, BOOL bUpdate = false);
	DWORD			GetMaxCrit	()	{ return m_Info.dwMaxCrit; }

	void			SetMaxHitrate	(DWORD dwMaxHitrate, BOOL bUpdate = false);
	DWORD			GetMaxHitrate	()	{ return m_Info.dwMaxHitrate; }

	void			SetMoney		( int Money,int moneyTye = 1, BOOL bUpdate = false);	// 很常使用，默认不直接修改数据库
	void			SetMoneySaved	( int nMoney, BOOL bUpdate = false);	// 很常使用，默认不直接修改数据库
	DWORD			GetMoney		( int moneyTye);
	DWORD			GetMoney1()		{ return m_Info.money1;		}
	DWORD			GetMoney2()		{ return m_Info.money2;		}
	DWORD			GetMoney3()		{ return m_Info.money3;		}
	DWORD			GetMoney4()		{ return m_Info.money4;		}
	DWORD			GetMoney5()		{ return m_Info.money5;		}
	DWORD			GetMoneySaved()	{ return m_Info.moneySave;	}
	
	
	OBJID			GetSynID()		{ return m_Info.idSyn; }
	void			SetSynID( OBJID id,BOOL bUpdate = false );

	OBJID			GetFamilyID()	{ return m_Info.idFamily; }
	void			SetFamilyID( OBJID id,BOOL bUpdate = false );

	OBJID			GetNationID()	{ return m_Info.idNation; }
	void			SetNationID( OBJID id,BOOL bUpdate = false );

	void			SetRecordPos	(OBJID idMap, DWORD dwX, DWORD dwY, BOOL bUpdate = false);
	OBJID			GetRecordMap	()	{return m_Info.idRecordmap;}
	DWORD			GetRecordX		()	{return m_Info.usRecordX;}
	DWORD			GetRecordY		()	{return m_Info.usRecordY;}

	void			SetSpeed		(DWORD nData, BOOL bUpdate = false);
	DWORD			GetSpeed		()	{return m_Info.usSpeed;}

	void			SetLookFace		(DWORD dwLookFace, BOOL fUpdate = false);		//很少使用
	DWORD			GetLookFace		()	{if(!m_Info.Vipface) {return m_Info.dwLookface;} else {return m_Info.Vipface;}}

	//[2010-06-04 goto] 用户心情，在好友中显示
	void			SetFeel		(char* szFeel, BOOL bUpdate = false);
	char*			GetFeel		()	{return m_Info.szFeel;}

	void			SetSex			( UINT nSex,BOOL bUpdate = false );	
	UINT			GetSex			()	{ return m_Info.sex; }	
	
	bool			SetNewGiveMask		(_int64 i64Mask);	
	_int64			GetNewGiveMask		()	{ return m_Info.i64NewGiveMask;}

	bool			SetNewGiveMask2		(_int64 i64Mask);	
	_int64			GetNewGiveMask2		()	{ return m_Info.i64NewGiveMask2;}

	void			SetTaskMask		(_int64 i64Mask);	
	_int64			GetTaskMask		()	{ return m_Info.i64TaskMask;}
	
	void			SetDayTaskMask		(_int64 i64DayMask);	
	_int64			GetDayTaskMask		()	{ return m_Info.i64DayTaskMask; }	

	void			SetDayTaskMask2		(_int64 i64DayMask2);	
	_int64			GetDayTaskMask2		()	{ return m_Info.i64DayTaskMask2; }	

	void			SetWeekTaskMask		(_int64 i64WeekMask);
	_int64			GetWeekTaskMask		()	{ return m_Info.i64WeekTaskMask; }

	char*			GetDescription()	{ return m_Info.description; }			
	void			SetDescription( char* desc,BOOL bUpdate = false );

	DWORD			GetTutorLevel	()		{ return m_Info.ucTutorLev; }
	void			SetTutorLevel	( DWORD ucLevel, BOOL bUpdate = false );

	UCHAR			GetTutorMask	()		{ return m_Info.idTutor; }
	void			SetTutorMask	( int nMask, BOOL bUpdate = false );
	
	DWORD			GetPackLev()		{ return m_Info.nPackageLev; }
	void			SetPackLev( DWORD ucLevel,bool bUpdate = false);
	
	DWORD			GetBankLev		()	{ return m_Info.nBankLev; }	
	void			SetBankLev		(DWORD ucLevel, BOOL bUpdate = false);

	int 			GetNewKardState(int index){return m_Info.newKardState[index];}
	void 			SetNewKardState(int data,int index){m_Info.newKardState[index] = data;}
	
	int             GetPkProc(){return m_Info.ucPkMode;}
	void            SetPkProc(int data,BOOL bUpdate = false);

	//职业数据
	void			SetProfession(DWORD nProff,int nType = 0, BOOL bUpdate = false);
	DWORD			GetJobByType	(DWORD nType);

	void			SetLife			(int nLife, BOOL bUpdate = false);
	DWORD			GetLife			();
	
	void			SetMana			(int nPower, BOOL bUpdate = false);
	DWORD			GetMana			();
	
	DWORD			GetSp			();
	void			SetSp			( int data,bool bUpdate = false );

	void			SetJobLev		(int nData,int type = 0, BOOL bUpdate = false);
	DWORD			GetJobLev		(int type = 0);

	void			SetJobCon		(int nData,int type = 0, BOOL bUpdate = false);
	DWORD			GetJobCon		(int type = 0);

	void			AddStr		    (int nData,int type = 0, BOOL bUpdate = false);
	void			SetStr		    (int nData,int type = 0, BOOL bUpdate = false);
	DWORD			GetStr			(int type = 0);

	void			AddIntex		(int nData,int type = 0, BOOL bUpdate = false);
	void			SetIntex		(int nData,int type = 0, BOOL bUpdate = false);
	DWORD			GetIntex		(int type = 0);

	void			AddSta		    (int nData,int type = 0, BOOL bUpdate = false);
	void			SetSta		    (int nData,int type = 0, BOOL bUpdate = false);
	DWORD			GetSta		    (int type = 0);

	void			AddSpi		    (int nData,int type = 0, BOOL bUpdate = false);
	void			SetSpi		    (int nData,int type = 0, BOOL bUpdate = false);
	DWORD			GetSpi		    (int type = 0);

	void			AddAgi		    (int nData,int type = 0, BOOL bUpdate = false);
	void			SetAgi		    (int nData,int type = 0, BOOL bUpdate = false);
	DWORD			GetAgi		    (int type = 0);

	void			AddPot		    (int nData,int type = 0, BOOL bUpdate = false);
	void			SetPot		    (int nData,int type = 0, BOOL bUpdate = false);
	DWORD			GetPot		    (int type = 0);

	void            SetVipTime      (DWORD dwMin);
	void			SetVipLev		(DWORD dwMin);

	DWORD           GetVipTime      () { return m_Info.vipTime; }
	DWORD			GetVipLev		() { return m_Info.vipLev; }

	void			SetEverydayOL	(int nTime, BOOL bUpdate = false);
	int				GetEverydayOL		()	{return m_Info.EverydayOL;}

	void			SetIdentity		(int IdNum);
	void            ActiveVipTime   ();
	void            ClrVipTime      ();
	bool            IsVipActive     ()  { return (m_Info.vipState == 1); }
	//[2010-08-27 goto]特殊大红大蓝的增量，最后一位表示类型
	DWORD			GetHpAdd		()	{ return m_Info.dwHpAdd; }
	DWORD			GetMpAdd		()	{ return m_Info.dwMpAdd; }
	DWORD			GetPetAdd		()	{ return m_Info.dwPetAdd; };
	void			SetHpAdd		(DWORD dwHpAdd, bool bUpdate=false);
	void			SetMpAdd		(DWORD dwMpAdd, bool bUpdate=false);
	void			SetPetAdd		(DWORD dwPetAdd, bool bUpdate=false);
	char*			GetItemLockPwd	()	{ return m_Info.szItemLockPwd; }
	void			SetItemLockPwd	(char* szPwd, bool bUpdate=false);//涉及到物品安全，所以实时保存

	_int64			GetASales();//购买的特价商品
	void			SetASales(_int64 nData, bool bUpdate = false);
	void            SetJinmeiValue(int nData, bool bUpdate = false);
	int             GetJinmeiValue();

	int				GetMorality();//师德
	void			SetMorality(int nData, bool bUpdate = false);
protected:
	UserInfoStruct		m_Info;		
	IRecord*			m_pRes;
	bool				m_bNeedUpdate;
	friend class CUser;		// 所有内容由CUser操作
};

#endif // !defined(AFX_USERDATA_H__6C3EB549_7325_4A50_82DF_FD07A49E1300__INCLUDED_)
