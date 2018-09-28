// 数据库底层的通用接口
// 仙剑修, 2002.9.27
//////////////////////////////////////////////////////////////////


#ifndef	I_MYDB_H
#define	I_MYDB_H

#include "..\COMMON\common.h"

#define PURE_VIRTUAL_DECLARATION_DB		PURE_VIRTUAL_FUNCTION
#define PURE_VIRTUAL_DECLARATION_DB_0	PURE_VIRTUAL_FUNCTION_0

/*
	说明：为兼容旧系统，函数参数尽量与旧系统相同。
*/

class	CMyRecordset;
class	CMyRecord;
class	IRecordset;
class	IDatabase
{
protected:
	virtual ~IDatabase() {}
public:
	virtual IRecordset*	CreateNewRecordset(LPCTSTR szSQL, bool bNoEmpty = true)		PURE_VIRTUAL_DECLARATION_DB_0
	virtual	void	Release()									PURE_VIRTUAL_DECLARATION_DB
public:
	virtual bool	ExecuteSQL	(char* szSQL)				PURE_VIRTUAL_DECLARATION_DB_0
	virtual int		GetInsertId	()						PURE_VIRTUAL_DECLARATION_DB_0
	virtual int		CountRecord	(const char* szSQL)		PURE_VIRTUAL_DECLARATION_DB_0
};

const int		NEWKEY_NONE	= (DWORD)-1;
class	IRecord
{
protected:
	virtual ~IRecord() {}
public:
	virtual IRecord*	CreateNewRecord(DWORD idNewKey = NEWKEY_NONE)	PURE_VIRTUAL_DECLARATION_DB_0
	virtual	void	Release()									PURE_VIRTUAL_DECLARATION_DB

	// 注：目前只有CMyRecord支持在InsertRecord()后继续UpdateRecord()。
	virtual DWORD	InsertRecord()								PURE_VIRTUAL_DECLARATION_DB_0
	virtual bool	UpdateRecord()								PURE_VIRTUAL_DECLARATION_DB_0
	virtual bool	DeleteRecord()								PURE_VIRTUAL_DECLARATION_DB_0

	virtual int		LoadInt		(LPCTSTR szField)					PURE_VIRTUAL_DECLARATION_DB_0
	virtual void	SetInt		(LPCTSTR szField, int nData)		PURE_VIRTUAL_DECLARATION_DB
	virtual UINT	LoadUInt	(LPCTSTR szField)					PURE_VIRTUAL_DECLARATION_DB_0
	virtual void	SetUInt		(LPCTSTR szField, UINT nData)		PURE_VIRTUAL_DECLARATION_DB
	virtual void	LoadString	(char* szData, LPCTSTR szField, int nSize)		PURE_VIRTUAL_DECLARATION_DB
	virtual void	SetString	(LPCTSTR szField, LPCTSTR szData, int nSize)	PURE_VIRTUAL_DECLARATION_DB
	virtual DWORD	LoadDWord	(LPCTSTR szField)					{ return LoadUInt(szField); }
	virtual void	SetDWord	(LPCTSTR szField, DWORD nData)		{ SetUInt(szField, nData); }
	// 注：暂时还需要遵守“先取后写”的规则，待具体实现改变后可不按此规则。

public:
	// 新模式的接口，用于代替应用程序的DATA层。
	virtual	_int64   GetInt64	(int idx)							PURE_VIRTUAL_DECLARATION_DB_0
	virtual _int64	GetInt64	(LPCTSTR szField)					PURE_VIRTUAL_DECLARATION_DB_0
	virtual void	SetInt64	(int nIdx,			_int64 i64Data)			PURE_VIRTUAL_DECLARATION_DB
	virtual void	SetInt64	(LPCTSTR szField,	_int64 i64Data)			PURE_VIRTUAL_DECLARATION_DB
	virtual double	GetDouble	(int nIdx)							PURE_VIRTUAL_DECLARATION_DB_0
	virtual int		GetInt		(int nIdx)							PURE_VIRTUAL_DECLARATION_DB_0
	virtual int     GetInt		(LPCTSTR szField)					PURE_VIRTUAL_DECLARATION_DB_0 ///彭正帮做的扩展,可能是锅
	virtual	LPCTSTR		GetStr(LPCTSTR szField)							PURE_VIRTUAL_DECLARATION_DB_0 ///同上
	virtual LPCTSTR	GetStr		(int nIdx)							PURE_VIRTUAL_DECLARATION_DB_0
	virtual void	SetInt		(int nIdx, int nData)				PURE_VIRTUAL_DECLARATION_DB
	virtual void	SetStr		(int nIdx, LPCTSTR szData, int nSize)			PURE_VIRTUAL_DECLARATION_DB
	virtual	void	SetStr		(LPCTSTR szField,LPCTSTR szData, int nSize)	PURE_VIRTUAL_DECLARATION_DB
	virtual void	ClearUpdateFlags	()							PURE_VIRTUAL_DECLARATION_DB

	virtual bool	AddInt(LPCTSTR szField, int nData)			PURE_VIRTUAL_DECLARATION_DB_0		// 立即写库。如无共享冲突，可不提供该接口
};

class	IRecordset : public IRecord
{
protected:
	virtual ~IRecordset() {}
public:
	virtual IRecord*	CreateNewConstRecord(DWORD idNewKey = NEWKEY_NONE)	PURE_VIRTUAL_DECLARATION_DB_0
//	virtual IRecord*	GetRecordInterface()	PURE_VIRTUAL_DECLARATION_DB_0		// 取IRecord
	virtual UINT		RecordCount()			PURE_VIRTUAL_DECLARATION_DB_0		// return -1: error
	virtual bool		MoveNext()				PURE_VIRTUAL_DECLARATION_DB_0			// return false: no more data
	virtual void		MoveFirst()				PURE_VIRTUAL_DECLARATION_DB			// [12/21/2007 朱斌]	移动到第一个记录
	virtual BOOL		SetThisKey(LPCTSTR szField)				PURE_VIRTUAL_DECLARATION_DB_0
};

template<TYPENAME T>
class	IGameData
{
protected:
	virtual ~IGameData() {}
public:
	virtual	void		Release()											PURE_VIRTUAL_DECLARATION_DB
	virtual	DWORD		GetID()												PURE_VIRTUAL_DECLARATION_DB_0
public:
	virtual	int			GetInt(T nDataName)									PURE_VIRTUAL_DECLARATION_DB_0
	virtual	LPCTSTR		GetStr(T nDataName)									PURE_VIRTUAL_DECLARATION_DB_0
	virtual	void		SetInt(T nDataName, int nData)						PURE_VIRTUAL_DECLARATION_DB
	virtual	void		SetStr(T nDataName, LPCTSTR szData, int nSize)		PURE_VIRTUAL_DECLARATION_DB
public:
	virtual	bool		Create(DWORD id, IDatabase* pDb)					PURE_VIRTUAL_DECLARATION_DB_0
	virtual	bool		Create(IRecordset* pRecordset)						PURE_VIRTUAL_DECLARATION_DB_0
	virtual	bool		Create(IRecord* pDefaultRes, DWORD id)				PURE_VIRTUAL_DECLARATION_DB_0
	virtual	bool		Update()											PURE_VIRTUAL_DECLARATION_DB_0
	virtual	DWORD		InsertRecord()										PURE_VIRTUAL_DECLARATION_DB_0
	virtual	bool		DeleteRecord()										PURE_VIRTUAL_DECLARATION_DB_0
	virtual	void		ClearUpdateFlags()									PURE_VIRTUAL_DECLARATION_DB

	virtual _int64			GetInt64(T nDataName)							PURE_VIRTUAL_DECLARATION_DB_0
	virtual	void			SetInt64(T nDataName, __int64 i64Data)			PURE_VIRTUAL_DECLARATION_DB
};


// SetDatabase()、SetTableName()暂不提供接口，暂时直接从对象进行操作。

extern IDatabase*	CreateDatabase(const char* szDBServer = NULL, const char* szLoginName = NULL, const char* szPassword = NULL, const char* szDBName = NULL/*,const char* szDBCharset = "CHS"*/);
//  [1/29/2008 朱斌]
extern BOOL	ReconnectDatabase(IDatabase* pDb, const char* szDBServer /*= NULL*/, const char* szLoginName /*= NULL*/, const char* szPassword /*= NULL*/, const char* szDBName /*= NULL*/);

/////////////////////////////////////////////////////////////////////////////
// 数据库表名定义
//
#define		_WBTBL_PREHEAD			"wb_"
#define		_TBL_DELUSER			_WBTBL_PREHEAD "deluser"
#define		_TBL_MONSTERTYPE		_WBTBL_PREHEAD "monstertype"
#define		_TBL_USER				_WBTBL_PREHEAD "user"
#define		_TBL_USERSTATUS			_WBTBL_PREHEAD "userstatus"
#define		_TBL_ITEM				_WBTBL_PREHEAD "item"
#define		_TBL_ITEMTYPE			_WBTBL_PREHEAD "itemtype"
#define		_TBL_ITEMADDITION		_WBTBL_PREHEAD "itemaddition"
#define		_TBL_LEVEXP				_WBTBL_PREHEAD "levexp"
#define		_TBL_DROPITEMRULE		_WBTBL_PREHEAD "dropitemrule"
#define		_TBL_FRIEND				_WBTBL_PREHEAD "friend"
#define		_TBL_LEAVEWORD			_WBTBL_PREHEAD "leaveword"
#define		_TBL_ACTION				_WBTBL_PREHEAD "action"
#define		_TBL_MAP				_WBTBL_PREHEAD "map"
#define		_TBL_PASSWAY			_WBTBL_PREHEAD "passway"
#define		_TBL_NPC				_WBTBL_PREHEAD "npc"
#define		_TBL_GOODS				_WBTBL_PREHEAD "goods"
#define		_TBL_TASKDETAIL			_WBTBL_PREHEAD "taskdetail"
#define     _TBL_RANDTASKSET        _WBTBL_PREHEAD "taskset"
#define		_TBL_EUDEMON			_WBTBL_PREHEAD "eudemon"
#define		_TBL_EUDEMONTYPE		_WBTBL_PREHEAD "eudemontype"
#define		_TBL_PEIFANG			_WBTBL_PREHEAD "peifang"
#define		_TBL_PEIFANGTYPE		_WBTBL_PREHEAD "peifangtype"
#define		_TBL_EUDEMONMAGIC		_WBTBL_PREHEAD "eudemonskill" 
#define		_TBL_SOULMAGIC			_WBTBL_PREHEAD "weapensoulkill" 
#define		_TBL_SYNATTR			_WBTBL_PREHEAD "synattr"
#define		_TBL_SYNDICATE			_WBTBL_PREHEAD "syndicate"
#define		_TBL_INSTANCETYPE		_WBTBL_PREHEAD "instancetype"
#define		_TBL_DYNAMAP			_WBTBL_PREHEAD "dynamap"
#define		_TBL_SYNMAP				_WBTBL_PREHEAD "synmap"
#define		_TBL_DYNANPC			_WBTBL_PREHEAD "dynanpc"
#define		_TBL_GENERATOR			_WBTBL_PREHEAD "generator"
#define     _TBL_SUIT				_WBTBL_PREHEAD "Suit"
#define		_TBL_GMMAC				_WBTBL_PREHEAD "gmmac"
#define		_TBL_WEAPONSOUL			_WBTBL_PREHEAD "weapensoul"
#define		_TBL_MONTERMAGIC		_WBTBL_PREHEAD "monstermagic"
#define		_TBL_GIFTUSER			_WBTBL_PREHEAD "gift_user"
#define		_TBL_GIFTTREE			_WBTBL_PREHEAD "gift_tree"
#define		_TBL_GIFTTEMPLETMANAGER	_WBTBL_PREHEAD "gift_templet_manager"
#define     _TBL_SHOPVAS			_WBTBL_PREHEAD "goods_vas"
#define     _TBL_VASASALEITEM		_WBTBL_PREHEAD "vas_asaleitem"
#define     _TBL_VASLIMITITEM		_WBTBL_PREHEAD "vas_limititem"

#define		_TBL_MAGIC				_WBTBL_PREHEAD "magic"
#define		_TBL_MAGICTYPE			_WBTBL_PREHEAD "magictype"
#define		_TBL_TUTOR				_WBTBL_PREHEAD "tutor"
#define     _TBL_JINMEITYPE         _WBTBL_PREHEAD "jinmei_template"
#define     _TBL_JINMEIUSER         _WBTBL_PREHEAD "jinmei_user"
#define     _TBL_JINMEISTRENGTHTYPE _WBTBL_PREHEAD "jinmeistrength_template"
#define     _TBL_JINMEIEFFCTTYPE    _WBTBL_PREHEAD "jinmeieffect_template"
#define     _TBL_AUTOFIGHTSET       _WBTBL_PREHEAD "autofight" //自动挂机设置表
#define     _TBL_INSTANCEUSETIME    _WBTBL_PREHEAD "instanceusetime" //副本使用次数表
#define		_TBL_WARGAME			_WBTBL_PREHEAD "wargame"	//战争活动表
#define		_TBL_WARGAMEMAP			_WBTBL_PREHEAD "wargamemap"	//战争活动地图
#define		_TBL_USERWARGAME		_WBTBL_PREHEAD "userwargame"	//玩家战争记录表
#define     _TBL_USERKEY            _WBTBL_PREHEAD "key"   //用户按键
#define		_TBL_TITLE				_WBTBL_PREHEAD "title"		//称号表
#define		_TBL_TITLETYPE			_WBTBL_PREHEAD "titletype"	//称号表
#define		_TBL_TREASURE			_WBTBL_PREHEAD "treasure"		//奖品表
#define		_TBL_TREASURETYPE		_WBTBL_PREHEAD "treasuretype"	//奖品类型表
#define		_TBL_ONLINEGIFT			_WBTBL_PREHEAD "onlinegift"//上线祝福表
#define		_TBL_CPSATION			_WBTBL_PREHEAD "compensationdata"//补偿表
#define		_TBL_CPSATIONMSG		_WBTBL_PREHEAD "compensationmsg"//补偿信息
#define		_TBL_PETEGG				_WBTBL_PREHEAD "petegg"		//宠物蛋
#define    _TBL_CHANGENAME			_WBTBL_PREHEAD "changename"//改名
#define    _TBL_SPECILLINE_INFO		_WBTBL_PREHEAD "specilline_info"//专线表
#define		_TBL_VERIFICATION_RANDSHOP _WBTBL_PREHEAD "verification_randshop"//跑商验证表
//modify code by 林德渊 begin 2011.5.30
#define		_TBL_VASLV				_WBTBL_PREHEAD "vaslv"//充值等级表
//modify code by 林德渊 end

#endif // I_MYDB_H
