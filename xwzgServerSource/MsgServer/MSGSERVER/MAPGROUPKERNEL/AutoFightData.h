// AutoFightData.h: interface for the CAutoFightData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOFIGHTDATA_H__82B4ACDE_1D3C_4FED_9B2A_50AAB439ADE8__INCLUDED_)
#define AFX_AUTOFIGHTDATA_H__82B4ACDE_1D3C_4FED_9B2A_50AAB439ADE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/protocol.h"

enum _AUTOFUGHTDATA{
		_AUTOFUGHTDATA_ID	  = 0,
		_AUTOFUGHTDATA_IDUSER = 1,
		_AUTOFUGHTDATA_SWITCH,
		_AUTOFUGHTDATA_PERCENTHP,
		_AUTOFUGHTDATA_PERCENTMP,
		_AUTOFUGHTDATA_SKILLHP,
		_AUTOFUGHTDATA_SKILLMP,
		_AUTOFUGHTDATA_SKILLTEAMHP,
		_AUTOFUGHTDATA_EUDHP,
		_AUTOFUGHTDATA_EUDHAPPY,
		_AUTOFUGHTDATA_HITRANGE,
		_AUTOFUGHTDATA_DOUBLEEXP,
		_AUTOFUGHTDATA_DOUBLEEUDEXP,
		_AUTOFUGHTDATA_LIYUCHUN,
		_AUTOFUGHTDATA_HPMED1,
		_AUTOFUGHTDATA_HPMED2,
		_AUTOFUGHTDATA_HPMED3,
		_AUTOFUGHTDATA_MPMED1,
		_AUTOFUGHTDATA_MPMED2,
		_AUTOFUGHTDATA_MPMED3,
		_AUTOFUGHTDATA_EUDHPMED1,
		_AUTOFUGHTDATA_EUDHPMED2,
		_AUTOFUGHTDATA_EUDHPMED3,
		_AUTOFUGHTDATA_EUDTOY1,
		_AUTOFUGHTDATA_EUDTOY2,

//     	_AUTOFUGHTDATA_ID = 0,
// 		_AUTOFUGHTDATA_IDUSER,
// 		_AUTOFUGHTDATA_AR_HP,
// 		_AUTOFUGHTDATA_AR_HPITEM,
// 		_AUTOFUGHTDATA_AR_HPPER,
// 		_AUTOFUGHTDATA_AR_MP,
// 		_AUTOFUGHTDATA_AR_MPITEM,
// 		_AUTOFUGHTDATA_AR_MPPER,
// 		_AUTOFUGHTDATA_AF_HP,
// 		_AUTOFUGHTDATA_AF_HPITEM,
// 		_AUTOFUGHTDATA_AF_HPPER,
// 		_AUTOFUGHTDATA_AF_MP,
// 		_AUTOFUGHTDATA_AF_MPITEM,
// 		_AUTOFUGHTDATA_AF_MPPER,
// 		_AUTOFUGHTDATA_PICKUP,
// 		_AUTOFUGHTDATA_USE_VAS,
// 		_AUTOFUGHTDATA_SHOP,
// 		_AUTOFUGHTDATA_NUM_HP,
// 		_AUTOFUGHTDATA_NUM_MP,
// 		_AUTOFUGHTDATA_NUM_FH,
// 		_AUTOFUGHTDATA_NUM_DOUBLE,
// 		_AUTOFUGHTDATA_GOAL,
// 		_AUTOFUGHTDATA_WAY,
// 		_AUTOFUGHTDATA_AUTOUSEASSIST,
// 		_AUTOFUGHTDATA_AUTOUSEMAGIC,
// 		_AUTOFUGHTDATA_SKILL1,
// 		_AUTOFUGHTDATA_SKILL2,
// 		_AUTOFUGHTDATA_SKILL3,
// 		_AUTOFUGHTDATA_SKILL4,
// 		_AUTOFUGHTDATA_SKILL5,
// 		_AUTOFUGHTDATA_SKILL6,
// 		_AUTOFUGHTDATA_SKILL7,
// 		_AUTOFUGHTDATA_AR_HPITEM2,
// 		_AUTOFUGHTDATA_AR_HPPER2,
// 		_AUTOFUGHTDATA_AR_MPITEM2,
// 		_AUTOFUGHTDATA_AR_MPPER2,
// 		_AUTOFUGHTDATA_AE_EUDEMON1,
// 		_AUTOFUGHTDATA_AE_EUDEMON2,
// 		_AUTOFUGHTDATA_AE_DRITYPE,
// 		_AUTOFUGHTDATA_AE_DRIREDID,
// 		_AUTOFUGHTDATA_AE_DRIBLUEID,
// 		_AUTOFUGHTDATA_AE_DRIREDTYPEID,
// 		_AUTOFUGHTDATA_AE_DRIBLUETYPEID,
// 		_AUTOFUGHTDATA_AE_DRIREDPRE,
// 		_AUTOFUGHTDATA_AE_DRIBLUEPRE,
};

typedef struct AutoFightInfo
{      
	OBJID idUser ;              //自动挂机保存设置的玩家id
	//开关
	OBJID AutoSwitch;		   //所有开关
	unsigned short PercentHp;
	unsigned short PercentMp;
	unsigned short SkillHp;
	unsigned short SkillMp;
	unsigned short SkillTeamhp;
	unsigned short EudHp;
	unsigned short EudHappy;
	unsigned short HitRange;
	unsigned short DoubleExp;
	unsigned short DoubleEudExp;
	unsigned short LIYUCHUN;
	unsigned short temp;
	OBJID Hpmed1;
	OBJID Hpmed2;
	OBJID Hpmed3;
	OBJID Mpmed1;
	OBJID Mpmed2;
	OBJID Mpmed3;
	OBJID EudHpmed1;
	OBJID EudHpmed2;
	OBJID EudHpmed3;
	OBJID EudToy1;
	OBJID EudToy2;
	OBJID id;
}AUTOF;

const int PICKMODE_NO = 0;                      //0未开启自动拾取，
const int PICKMODE_YES = 1;                     //1开启自动拾取
const int PICKMODE_POISON = 2;                  //2拾取药品
const int PICKMODE_STUFF = 4;                   //4拾取材料
const int PICKMODE_GEM = 8;                     //8拾取宝石
const int PICKMODE_BOOK = 16;                   //16拾取技能书
const int PICKMODE_GOOD = 32;                   //32拾取优秀品质以上
const int PICKMODE_PERFECT = 64;                //64拾取完美品质以上
const int PICKMODE_EXCELENT = 128;              //128拾取卓越品质以上

char    szAutoTable[];
char    szUsersid[];
typedef CGameData<_AUTOFUGHTDATA,szAutoTable,szUsersid>     CAutoFight;

class CAutoFightData   : public CGameObj
{
public:
	CAutoFightData();
	virtual ~CAutoFightData();

	CAutoFightData* CreateNew(){ return new CAutoFightData; }
//	static CAutoFightData* CreateNew();
	ULONG	ReleaseByOwner();				

	bool Create(IDatabase* pDb,CUser* pUser,bool bLogin,IRecordset* pRec);
	bool AutoFightWriteDB(struct AutoFightInfo &pInfo,bool bUpdate,IDatabase* pDB = NULL,bool bInsert = true); //写入数据库
	bool AutoFightReadDB(CUser* pUser, IDatabase* pDB = NULL, const char* pSkillName=NULL);	///从数据库读出信息
	bool Update();
	bool bAutof;


	void	SendObjInfo	(CUser* pUser ,PROCESS_ID idProcess);
	void	SetObjInfo(AUTOF* pStInfo);
private:
    AUTOF m_pAutoFightInfo;
	CAutoFight *m_AutoFight;
	bool m_bNeedUpdate;
	
};

#endif // !defined(AFX_AUTOFIGHTDATA_H__82B4ACDE_1D3C_4FED_9B2A_50AAB439ADE8__INCLUDED_)
