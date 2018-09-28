// ���ݿ�ײ��ͨ�ýӿ�
// �ɽ���, 2002.9.27
//////////////////////////////////////////////////////////////////


#ifndef	I_MYDB_H
#define	I_MYDB_H

#include "..\COMMON\common.h"

#define PURE_VIRTUAL_DECLARATION_DB		PURE_VIRTUAL_FUNCTION
#define PURE_VIRTUAL_DECLARATION_DB_0	PURE_VIRTUAL_FUNCTION_0

/*
	˵����Ϊ���ݾ�ϵͳ�����������������ϵͳ��ͬ��
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

	// ע��Ŀǰֻ��CMyRecord֧����InsertRecord()�����UpdateRecord()��
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
	// ע����ʱ����Ҫ���ء���ȡ��д���Ĺ��򣬴�����ʵ�ָı��ɲ����˹���

public:
	// ��ģʽ�Ľӿڣ����ڴ���Ӧ�ó����DATA�㡣
	virtual	_int64   GetInt64	(int idx)							PURE_VIRTUAL_DECLARATION_DB_0
	virtual _int64	GetInt64	(LPCTSTR szField)					PURE_VIRTUAL_DECLARATION_DB_0
	virtual void	SetInt64	(int nIdx,			_int64 i64Data)			PURE_VIRTUAL_DECLARATION_DB
	virtual void	SetInt64	(LPCTSTR szField,	_int64 i64Data)			PURE_VIRTUAL_DECLARATION_DB
	virtual double	GetDouble	(int nIdx)							PURE_VIRTUAL_DECLARATION_DB_0
	virtual int		GetInt		(int nIdx)							PURE_VIRTUAL_DECLARATION_DB_0
	virtual int     GetInt		(LPCTSTR szField)					PURE_VIRTUAL_DECLARATION_DB_0 ///������������չ,�����ǹ�
	virtual	LPCTSTR		GetStr(LPCTSTR szField)							PURE_VIRTUAL_DECLARATION_DB_0 ///ͬ��
	virtual LPCTSTR	GetStr		(int nIdx)							PURE_VIRTUAL_DECLARATION_DB_0
	virtual void	SetInt		(int nIdx, int nData)				PURE_VIRTUAL_DECLARATION_DB
	virtual void	SetStr		(int nIdx, LPCTSTR szData, int nSize)			PURE_VIRTUAL_DECLARATION_DB
	virtual	void	SetStr		(LPCTSTR szField,LPCTSTR szData, int nSize)	PURE_VIRTUAL_DECLARATION_DB
	virtual void	ClearUpdateFlags	()							PURE_VIRTUAL_DECLARATION_DB

	virtual bool	AddInt(LPCTSTR szField, int nData)			PURE_VIRTUAL_DECLARATION_DB_0		// ����д�⡣���޹����ͻ���ɲ��ṩ�ýӿ�
};

class	IRecordset : public IRecord
{
protected:
	virtual ~IRecordset() {}
public:
	virtual IRecord*	CreateNewConstRecord(DWORD idNewKey = NEWKEY_NONE)	PURE_VIRTUAL_DECLARATION_DB_0
//	virtual IRecord*	GetRecordInterface()	PURE_VIRTUAL_DECLARATION_DB_0		// ȡIRecord
	virtual UINT		RecordCount()			PURE_VIRTUAL_DECLARATION_DB_0		// return -1: error
	virtual bool		MoveNext()				PURE_VIRTUAL_DECLARATION_DB_0			// return false: no more data
	virtual void		MoveFirst()				PURE_VIRTUAL_DECLARATION_DB			// [12/21/2007 ���]	�ƶ�����һ����¼
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


// SetDatabase()��SetTableName()�ݲ��ṩ�ӿڣ���ʱֱ�ӴӶ�����в�����

extern IDatabase*	CreateDatabase(const char* szDBServer = NULL, const char* szLoginName = NULL, const char* szPassword = NULL, const char* szDBName = NULL/*,const char* szDBCharset = "CHS"*/);
//  [1/29/2008 ���]
extern BOOL	ReconnectDatabase(IDatabase* pDb, const char* szDBServer /*= NULL*/, const char* szLoginName /*= NULL*/, const char* szPassword /*= NULL*/, const char* szDBName /*= NULL*/);

/////////////////////////////////////////////////////////////////////////////
// ���ݿ��������
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
#define     _TBL_AUTOFIGHTSET       _WBTBL_PREHEAD "autofight" //�Զ��һ����ñ�
#define     _TBL_INSTANCEUSETIME    _WBTBL_PREHEAD "instanceusetime" //����ʹ�ô�����
#define		_TBL_WARGAME			_WBTBL_PREHEAD "wargame"	//ս�����
#define		_TBL_WARGAMEMAP			_WBTBL_PREHEAD "wargamemap"	//ս�����ͼ
#define		_TBL_USERWARGAME		_WBTBL_PREHEAD "userwargame"	//���ս����¼��
#define     _TBL_USERKEY            _WBTBL_PREHEAD "key"   //�û�����
#define		_TBL_TITLE				_WBTBL_PREHEAD "title"		//�ƺű�
#define		_TBL_TITLETYPE			_WBTBL_PREHEAD "titletype"	//�ƺű�
#define		_TBL_TREASURE			_WBTBL_PREHEAD "treasure"		//��Ʒ��
#define		_TBL_TREASURETYPE		_WBTBL_PREHEAD "treasuretype"	//��Ʒ���ͱ�
#define		_TBL_ONLINEGIFT			_WBTBL_PREHEAD "onlinegift"//����ף����
#define		_TBL_CPSATION			_WBTBL_PREHEAD "compensationdata"//������
#define		_TBL_CPSATIONMSG		_WBTBL_PREHEAD "compensationmsg"//������Ϣ
#define		_TBL_PETEGG				_WBTBL_PREHEAD "petegg"		//���ﵰ
#define    _TBL_CHANGENAME			_WBTBL_PREHEAD "changename"//����
#define    _TBL_SPECILLINE_INFO		_WBTBL_PREHEAD "specilline_info"//ר�߱�
#define		_TBL_VERIFICATION_RANDSHOP _WBTBL_PREHEAD "verification_randshop"//������֤��
//modify code by �ֵ�Ԩ begin 2011.5.30
#define		_TBL_VASLV				_WBTBL_PREHEAD "vaslv"//��ֵ�ȼ���
//modify code by �ֵ�Ԩ end

#endif // I_MYDB_H
