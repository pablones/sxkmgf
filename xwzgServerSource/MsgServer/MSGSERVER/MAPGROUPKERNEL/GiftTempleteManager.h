// GiftTempleteManager.h: interface for the CGiftTempleteManager class.
//	20071127:����
//	�츳ģ�������
//	ͨ��sk_gift_templet_manager��ȡ��ģ���
//	��ģ�����ֶβ�һ��
//	ÿһ��ģ������һ��recordset��.������ʱ,��ʹ��getint,getstr�ȼ򵥺���.
//	��recordset������getint�Ĳ����ĺϷ���
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GIFTTEMPLETEMANAGER_H__E9D042EB_BBA2_4402_8CFF_057D4FD27299__INCLUDED_)
#define AFX_GIFTTEMPLETEMANAGER_H__E9D042EB_BBA2_4402_8CFF_057D4FD27299__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "../share/define.h"
//#include "ConstGameData.h"
#include "GameData.h"
// #include "GameObj.h"
// #include "T_SingleMap2.h"
//#include "Myheap.h"
//#include "T_SingleMap2.h"
#include <map>
using namespace std;


const int _GIFTTEMPLETETABLENAME = 64;//�츳ģ�������
const int _GIFTTEMPLETEDESC = 256;//�츳ģ�������

typedef map<OBJID,IRecord*>		MAP_GIFTTEMP_TYPE;		//���001��008��ģ���¼���ͣ�ÿ��ģ��һ��map����

struct ST_GIFTTEMPLETEDATA
{
	OBJID id;
	MAP_GIFTTEMP_TYPE* m_pGiftTempMap;
};
typedef map<OBJID,ST_GIFTTEMPLETEDATA*> MAP_GIFTTEMPLETE;//�츳ģ�弯��



enum EM_GIFTTEMPLETEMNGDATA//sk_gift_templet_manager���ֶ�
{
	EM_GIFTTEMPLETEMNGDATA_id	=0,
	EM_GIFTTEMPLETEMNGDATA_tableName,
	EM_GIFTTEMPLETEMNGDATA_desc,
};

//��ģ����ֶ�ö��////////////////////////////////
//1001
//һ�����ʴ���һ��buff�����Լ���Է�����
enum EM_GIFTTEMPLATE_1001
{
	EM_GIFTTEMPLETE_1001_id	= 0,
	EM_GIFTTEMPLETE_1001_name,
	EM_GIFTTEMPLETE_1001_triggerType,
	EM_GIFTTEMPLETE_1001_triggerID,
	EM_GIFTTEMPLETE_1001_chance,
	EM_GIFTTEMPLETE_1001_target,
	EM_GIFTTEMPLETE_1001_buffID,
	EM_GIFTTEMPLETE_1001_pow,
	EM_GIFTTEMPLETE_1001_sec,
	EM_GIFTTEMPLETE_1001_times,
};
enum EM_GFTTMP1001_TARGET
{
	EM_GFTTMP1001_TARGET_self	= 1,//���Լ�ʩ��
	EM_GFTTMP1001_TARGET_target	= 2,//�ԶԷ�ʩ��
	EM_GFTTMP1001_TARGET_selfAndTarget = EM_GFTTMP1001_TARGET_self&EM_GFTTMP1001_TARGET_target,
};
enum EM_GFTTMP1001_TRIGGERTYPE
{
	EM_GFTTMP1001_TRIGGERTYPE_magicAtk	= 1,//ħ������ʱ
	EM_GFTTMP1001_TRIGGERTYPE_physicAtk	= 2,//������ʱ
	EM_GFTTMP1001_TRIGGERTYPE_bePhysicAtk	= 3,//�ܵ�������ʱ
	EM_GFTTMP1001_TRIGGERTYPE_beMagicAtk	= 4,//�ܵ�ħ������ʱ
	EM_GFTTMP1001_TRIGGERTYPE_beCritical	= 5,//�ܵ�����ʱ
	EM_GFTTMP1001_TRIGGERTYPE_beFightDisable	= 6,//�ܵ�������ս������ʱ
};

enum EM_GIFTTEMPLATE_1002
{
	EM_GIFTTEMPLATE_1002_id = 0,
	EM_GIFTTEMPLATE_1002_name,
	EM_GIFTTEMPLATE_1002_magicType1,
	EM_GIFTTEMPLATE_1002_magicType2,
	EM_GIFTTEMPLATE_1002_magicType3,
	EM_GIFTTEMPLATE_1002_type,
	EM_GIFTTEMPLATE_1002_value,
	EM_GIFTTEMPLATE_1002_chance,
};

/*
1��pow����
2����������
3����������
4����Χ����
5��CD����
6����������
7�����ļ���
8������ʱ������
*/
enum EM_GTTMP1002_TYPE
{
	EM_GFTTMP1002_power = 1,	
	EM_GFTTMP1002_change,
	EM_GFTTMP1002_distance,
	EM_GFTTMP1002_range,
	EM_GFTTMP1002_cd,
	EM_GFTTMP1002_intonate,
	EM_GFTTMP1002_comsume,
	EM_GFTTMP1002_duration,
};

//////////////////////////////////////////////////////////////////////////
// 1003

enum EM_GIFTTEMPLATE_1003
{
	EM_GIFTTEMPLATE_1003_id = 0,
	EM_GIFTTEMPLATE_1003_name,
	EM_GIFTTEMPLATE_1003_magicType,
	EM_GIFTTEMPLATE_1003_lev,
};

// end 1003
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// 1005
enum EM_GIFTTEMPLATE_1005
{
	EM_GIFTTEMPLATE_1005_id = 0,
	EM_GIFTTEMPLATE_1005_name,
	EM_GIFTTEMPLATE_1005_giftSort,
	EM_GIFTTEMPLATE_1005_giftType,
	EM_GIFTTEMPLATE_1005_gene,
	EM_GIFTTEMPLATE_1005_targetAtt,
};

enum EM_GTTMP1005_TARGETATT
{
	EM_GTTMP1005_TARGETATT_power = 1,
	EM_GTTMP1005_TARGETATT_dexterity,
	EM_GTTMP1005_TARGETATT_soul,
	EM_GTTMP1005_TARGETATT_health,
	EM_GTTMP1005_TARGETATT_loveliness,
};

// end 1005
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//	1006
enum EM_GIFTEMPLATE_1006
{
	EM_GIFTTEMPLATE_1006_id = 0,
	EM_GIFTTEMPLATE_1006_name,
	EM_GIFTTEMPLATE_1006_type,
	EM_GIFTTEMPLATE_1006_power,
};

enum EM_GTTMP1006_TYPE
{	
	EM_GTTMP1006_TYPE_power = 1,	//1������
	EM_GTTMP1006_TYPE_health,			//2������
	EM_GTTMP1006_TYPE_dexterity,	//3������
	EM_GTTMP1006_TYPE_soul,			//4���ǻ�
	EM_GTTMP1006_TYPE_loveliness,	//5������
	EM_GTTMP1006_TYPE_power_armet,		// 6��ͨ��ͷ�������������Ч���ӳ�
	EM_GTTMP1006_TYPE_power_bangle,		// 7��ͨ����������������Ч���ӳ�
	EM_GTTMP1006_TYPE_power_caestus,	// 8��ͨ�����������������Ч���ӳ�
	EM_GTTMP1006_TYPE_power_weapon,		// 9��ͨ�����������������Ч���ӳ�
	EM_GTTMP1006_TYPE_health_necklace,		//	10��ͨ�����������������Ч���ӳ�
	EM_GTTMP1006_TYPE_health_glove,			//	11��ͨ�����׻����������Ч���ӳ�
	EM_GTTMP1006_TYPE_health_shors,			//	12��ͨ��սѥ�����������Ч���ӳ�
	EM_GTTMP1006_TYPE_health_armor,			//	13��ͨ�����׻����������Ч���ӳ�
	EM_GTTMP1006_TYPE_dexterity_armet,	// 14��ͨ��ͷ�������������Ч���ӳ�
	EM_GTTMP1006_TYPE_dexterity_bangle,	// 15��ͨ����������������Ч���ӳ�
	EM_GTTMP1006_TYPE_dexterity_caestus,// 16��ͨ�����������������Ч���ӳ�
	EM_GTTMP1006_TYPE_dexterity_weapon,	// 17��ͨ�����������������Ч���ӳ�
	EM_GTTMP1006_TYPE_soul_necklace,	//	18��ͨ��������������ǻ�Ч���ӳ�
	EM_GTTMP1006_TYPE_soul_glove,		//	19��ͨ�����׻�������ǻ�Ч���ӳ�
	EM_GTTMP1006_TYPE_soul_shoes,		//	20��ͨ��սѥ��������ǻ�Ч���ӳ�
	EM_GTTMP1006_TYPE_soul_armor,		//	21��ͨ�����׻�������ǻ�Ч���ӳ�
	EM_GTTMP1006_TYPE_soul_armet,		//	22��ͨ��ͷ����������ǻ�Ч���ӳ�
	EM_GTTMP1006_TYPE_soul_bangle,		//	23��ͨ�������������ǻ�Ч���ӳ�
	EM_GTTMP1006_TYPE_soul_caestus,		//	24��ͨ��������������ǻ�Ч���ӳ�
	EM_GTTMP1006_TYPE_soul_weapon,		//	25��ͨ��������������ǻ�Ч���ӳ�
};

// end 1006
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//	1007
enum EM_GIFTEMPLATE_1007
{
	EM_GIFTTEMPLATE_1007_id = 0,
	EM_GIFTTEMPLATE_1007_addhp,
	EM_GIFTTEMPLATE_1007_addmp,
	EM_GIFTTEMPLATE_1007_addcrihit,
	EM_GIFTTEMPLATE_1007_addhit,
	EM_GIFTTEMPLATE_1007_adddodge,
	EM_GIFTTEMPLATE_1007_addTough,
	EM_GIFTTEMPLATE_1007_addphyatt,
	EM_GIFTTEMPLATE_1007_addmagatt,
	EM_GIFTTEMPLATE_1007_addphydef,
	EM_GIFTTEMPLATE_1007_addmagdef,	
	EM_GIFTTEMPLATE_1007_addstr,
	EM_GIFTTEMPLATE_1007_addint,
	EM_GIFTTEMPLATE_1007_addsta,
	EM_GIFTTEMPLATE_1007_addspi,
	EM_GIFTTEMPLATE_1007_addagi,	
	EM_GIFTTEMPLATE_1007_addAtt1,
	EM_GIFTTEMPLATE_1007_addAtt2,
	EM_GIFTTEMPLATE_1007_addAtt3,
	EM_GIFTTEMPLATE_1007_addAtt4,
	EM_GIFTTEMPLATE_1007_addDef1,
	EM_GIFTTEMPLATE_1007_addDef2,
	EM_GIFTTEMPLATE_1007_addDef3,
	EM_GIFTTEMPLATE_1007_addDef4,
};

enum EM_GTTMP1007_TARGETTYPE
{	
	EM_GTTMP1007_TARGETTYPE_ME = 1,	
	EM_GTTMP1007_TARGETTYPE_USER,	
	EM_GTTMP1007_TARGETTYPE_MONSTER,
	EM_GTTMP1007_TARGETTYPE_ALL,	
};

enum EM_GTTMP1007_DOTYPE
{	
	EM_GTTMP1007_DOTYPE_HP = 1,	
	EM_GTTMP1007_DOTYPE_MP,	
	EM_GTTMP1007_DOTYPE_DEL,
	EM_GTTMP1007_DOTYPE_ALL,	
};

// end 1007
//////////////////////////////////////////////////////////////////////////

enum EM_NEWGTTMP1001_DOTYPE
{
	DOTYPE_addhp = 1,
	DOTYPE_addmp,
	DOTYPE_addcrihit,
	DOTYPE_addhit,
	DOTYPE_adddodge,
	DOTYPE_addTough,
	DOTYPE_addphyatt,
	DOTYPE_addmagatt,
	DOTYPE_addphydef,
	DOTYPE_addmagdef,
	DOTYPE_addTreat,
	DOTYPE_addstr,
	DOTYPE_addint,
	DOTYPE_addsta,
	DOTYPE_addspi,
	DOTYPE_addagi,	
	DOTYPE_addAtt1,
	DOTYPE_addAtt2,
	DOTYPE_addAtt3,
	DOTYPE_addAtt4,
	DOTYPE_addDef1,
	DOTYPE_addDef2,
	DOTYPE_addDef3,
	DOTYPE_addDef4,
	DOTYPE_addAllAtt,
	DOTYPE_addAllDef,
	DOTYPE_addHANDD,
	DOTYPE_addPANDMDEF,
	//goto ����4�����ɵ��ر𱻶�����
	DOTYPE_petAddAtk,
	DOTYPE_petAddMagAtk,
	DOTYPE_petAddCrihit,
	DOTYPE_petAddMaxLife,
};

enum EM_NEWGIFTEMPLATE_1001
{
	EM_NEWGIFTTEMPLATE_1001_id = 0,
	EM_NEWGIFTTEMPLATE_1001_dotype,
	EM_NEWGIFTTEMPLATE_1001_dodata,
	EM_NEWGIFTTEMPLATE_1001_dodatainc,
};

enum EM_NEWGTTMP1002_DOTYPE
{
	DOTYPE1002_decphydef = 1,
	DOTYPE1002_decmagdef,
	DOTYPE1002_decDef1,
	DOTYPE1002_decDef2,
	DOTYPE1002_decDef3,
	DOTYPE1002_decDef4,
};

enum EM_NEWGIFTEMPLATE_1002
{
	EM_NEWGIFTTEMPLATE_1002_id = 0,
	EM_NEWGIFTTEMPLATE_1002_dotype,
	EM_NEWGIFTTEMPLATE_1002_dodata,
	EM_NEWGIFTTEMPLATE_1002_dodatainc,
};


enum EM_NEWGTTMP1003_DOTYPE
{
	DOTYPE1003_buffcrit = 12051,
	DOTYPE1003_buffhitrate,
	DOTYPE1003_buffdodge,
	DOTYPE1003_buffphyatt,
	DOTYPE1003_buffmagatt,
	DOTYPE1003_buffphydef,
	DOTYPE1003_buffmagdef,
	DOTYPE1003_bufflife,
	DOTYPE1003_buffmana,
};

enum EM_NEWGIFTEMPLATE_1003
{
	EM_NEWGIFTTEMPLATE_1003_id = 0,
	EM_NEWGIFTTEMPLATE_1003_dobuff,
	EM_NEWGIFTTEMPLATE_1003_dochange,
	EM_NEWGIFTTEMPLATE_1003_dochangeinc,
	EM_NEWGIFTTEMPLATE_1003_dopow,
	EM_NEWGIFTTEMPLATE_1003_dopowinc,
	EM_NEWGIFTTEMPLATE_1003_dosec,
	EM_NEWGIFTTEMPLATE_1003_dotime,
};

enum EM_NEWGTTMP1004_DOTYPE
{
	DOTYPE1004_phgdem = 1,
	DOTYPE1004_magdem,
	DOTYPE1004_Att1,
	DOTYPE1004_Att2,
	DOTYPE1004_Att3,
	DOTYPE1004_Att4,
};

enum EM_NEWGIFTEMPLATE_1004
{
	EM_NEWGIFTTEMPLATE_1004_id = 0,
	EM_NEWGIFTTEMPLATE_1004_dotype,
	EM_NEWGIFTTEMPLATE_1004_dochange,
	EM_NEWGIFTTEMPLATE_1004_dochangeinc,
	EM_NEWGIFTTEMPLATE_1004_dodata,
	EM_NEWGIFTTEMPLATE_1004_dodatainc,
};

//////////////////////////////////////////////////////////////////////////
//	1008
enum EM_GIFTTEMPLATE_1008
{
	EM_GIFTTEMPLATE_1008_id = 0,
	EM_GIFTTEMPLATE_1008_name,
	EM_GIFTTEMPLATE_1008_magicType1,
	EM_GIFTTEMPLATE_1008_magicType2,
	EM_GIFTTEMPLATE_1008_magicType3,
	EM_GIFTTEMPLATE_1008_type,
	EM_GIFTTEMPLATE_1008_value,
	EM_GIFTTEMPLATE_1008_targetAtt,
};

enum EM_GTTMP1008_TYPE
{
	EM_GFTTMP1008_power = 1,	
	EM_GFTTMP1008_change,
	EM_GFTTMP1008_distance,
	EM_GFTTMP1008_range,
	EM_GFTTMP1008_cd,
	EM_GFTTMP1008_intonate,
	EM_GFTTMP1008_comsume,
	EM_GFTTMP1008_duration,
};

enum EM_GTTMP1008_TARGETATT_TYPE
{
	EM_GFTMP1008_TargetAtt_phyAttack = 1,
	EM_GFTMP1008_TargetAtt_magicAttack,
	EM_GFTMP1008_TargetAtt_power,
	EM_GFTMP1008_TargetAtt_dexterity,
	EM_GFTMP1008_TargetAtt_soul,
	EM_GFTMP1008_TargetAtt_damage,
	EM_GFTMP1008_TargetAtt_soulAndHealth,
};

//��ģ����ֶ�ö��end////////////////////////////


//char	szGiftTemp001Table[];
//char	szGiftTemp002Table[];
//char	szGiftTemp003Table[];
//char	szGiftTemp005Table[];
//char	szGiftTemp006Table[];
//char	szGiftTemp007Table[];
//char	szGiftTemp008Table[];
//
//typedef	CGameData<EM_GIFTTEMPLATE_1001,szGiftTemp001Table,szID>	CGiftTemp001Data;
//typedef	ISingleMap2<CGiftTemp001Data>	IGiftTemp001Set;
//typedef	CSingleMap2<CGiftTemp001Data>	CGiftTemp001Set;
//
//typedef	CGameData<EM_GIFTTEMPLATE_1002,szGiftTemp002Table,szID>	CGiftTemp002Data;
//typedef	ISingleMap2<CGiftTemp002Data>	IGiftTemp002Set;
//typedef	CSingleMap2<CGiftTemp002Data>	CGiftTemp002Set;
//
//typedef	CGameData<EM_GIFTTEMPLATE_1003,szGiftTemp003Table,szID>	CGiftTemp003Data;
//typedef	ISingleMap2<CGiftTemp003Data>	IGiftTemp003Set;
//typedef	CSingleMap2<CGiftTemp003Data>	CGiftTemp003Set;
//
//typedef	CGameData<EM_GIFTTEMPLATE_1005,szGiftTemp005Table,szID>	CGiftTemp005Data;
//typedef	ISingleMap2<CGiftTemp005Data>	IGiftTemp005Set;
//typedef	CSingleMap2<CGiftTemp005Data>	CGiftTemp005Set;
//
//typedef	CGameData<EM_GIFTEMPLATE_1006,szGiftTemp006Table,szID>	CGiftTemp006Data;
//typedef	ISingleMap2<CGiftTemp006Data>	IGiftTemp006Set;
//typedef	CSingleMap2<CGiftTemp006Data>	CGiftTemp006Set;
//
//typedef	CGameData<EM_GIFTEMPLATE_1007,szGiftTemp007Table,szID>	CGiftTemp007Data;
//typedef	ISingleMap2<CGiftTemp007Data>	IGiftTemp007Set;
//typedef	CSingleMap2<CGiftTemp007Data>	CGiftTemp007Set;
//
//typedef	CGameData<EM_GIFTTEMPLATE_1008,szGiftTemp008Table,szID>	CGiftTemp008Data;
//typedef	ISingleMap2<CGiftTemp008Data>	IGiftTemp008Set;
//typedef	CSingleMap2<CGiftTemp008Data>	CGiftTemp008Set;



class CGiftTempleteManager  
{
//public:
private:
	CGiftTempleteManager();
	virtual ~CGiftTempleteManager();
public:
	static CGiftTempleteManager*	CreateNew	()		{ return new CGiftTempleteManager; }
	ULONG	Release			()				{ delete this; return 0; }
	bool	Create			(IDatabase* pDb);

	IRecord* GetInfo(OBJID tmpType,OBJID tmpIdx);//ͨ��ģ��type��idx�ҵ����м�¼.[��Ŀ]����㱣֤�ͷ�IRecord!
protected:
	MAP_GIFTTEMPLETE m_mGiftTemplete;

//	MAP_GIFTTEMP_TYPE	m_GiftTemp001Map;
//	MAP_GIFTTEMP_TYPE	m_GiftTemp002Map;
//	MAP_GIFTTEMP_TYPE	m_GiftTemp003Map;
//	MAP_GIFTTEMP_TYPE	m_GiftTemp005Map;
//	MAP_GIFTTEMP_TYPE	m_GiftTemp006Map;
//	MAP_GIFTTEMP_TYPE	m_GiftTemp007Map;
//	MAP_GIFTTEMP_TYPE	m_GiftTemp008Map;
//	IGiftTemp001Set*	m_pGiftTemp001Set;
//	IGiftTemp002Set*	m_pGiftTemp002Set;
//	IGiftTemp003Set*	m_pGiftTemp003Set;
//	IGiftTemp005Set*	m_pGiftTemp005Set;
//	IGiftTemp006Set*	m_pGiftTemp006Set;
//	IGiftTemp007Set*	m_pGiftTemp007Set;
//	IGiftTemp008Set*	m_pGiftTemp008Set;
};

#endif // !defined(AFX_GIFTTEMPLETEMANAGER_H__E9D042EB_BBA2_4402_8CFF_057D4FD27299__INCLUDED_)
