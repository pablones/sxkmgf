// ItemData.h: interface for the CItemData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMDATA_H__28794933_A926_4C9A_AACF_1A95340DA94B__INCLUDED_)
#define AFX_ITEMDATA_H__28794933_A926_4C9A_AACF_1A95340DA94B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameObj.h"
#include "ItemType.h"

//////////////////////////////////////////////////////////////////////
const int MAX_INTIMACY		= 255;
//////////////////////////////////////////////////////////////////////
enum //��ʯ������
{
	_ERROR_GEM_TYPE=-1,
	_NOT_HOLE=99999,
	_HOLE=88888,
};


enum //����Ч��
{
	_SPECIAL_NONE=-1,
	_SPECIAL_LEVEL1=1,
	_SPECIAL_LEVEL2=2,
	_SPECIAL_LEVEL3=3,
	_SPECIAL_LEVEL4=4,
	_SPECIAL_LEVEL5=5,
	_SPECIAL_LEVEL6=6,
	_SPECIAL_LEVEL7=7,
	_SPECIAL_LEVEL8=8,
	_SPECIAL_LEVEL9=9,	
    _SPECIAL_MAX_LEVEL=_SPECIAL_LEVEL9,
};

//////////////////////////////////////////////////////////////////////
//��ƷMASK��ʶ
const DWORD	EXCHANGE_DISABLE_MASK				= 0x01;				//���ɽ���
const DWORD	STORAGE_DISABLE_MASK				= 0x02;				//���ɴ�ֿ�
const DWORD	DROP_HINT_MASK						= 0x04;				//��������ʾ
const DWORD	SELL_HINT_MASK						= 0x08;				//��������ʾ
const DWORD	NEVER_DROP_WHEN_DEAD_MASK			= 0x10;				//��������
const DWORD	SELL_DISABLE_MASK					= 0x20;				//���ܳ���
const DWORD DROP_DISABLE_MASK					= 0x40;				//���ܶ���
//const DWORD POST_DISABLE_MASK					= 0x80;				//�����ʼ�		//Ŀǰ�����,��Ϊ�����ֶ�Ϊunsigned 1�ֽ�
const DWORD COSTLY_ITEM							= 0x80;				//  ������Ʒ
const DWORD USER_EXCLUSIVE_MASK					= 0x0100;			//	20070712	�û�Ψһ
const DWORD KIDNAP_EQUIP_MASK					= 0x0200;			// װ����� [9/5/2007 Add By Halei] 
const DWORD KIDNAP_PICKUP_MASK					= 0x0400;			// ʰȡ�� [9/5/2007 Add By Halei] 
const DWORD ACTIVATE_PICKUP						= 0x0800;			//  [2007-9-14] ���	��ü���
const DWORD ACTIVATE_EQUIP						= 0x1000;			//  [2007-9-14] ���	װ������
const DWORD LIFETIME_CONTINUE					= 0x2000;			//  [2007-9-14] ���	��������
const DWORD LIFETIME_NON_CONTINUE				= 0x4000;			//  [2007-9-14] ���	��������
const DWORD DROP_WHEN_DEAD_MASK                 = 0x8000;           // ������ص��� [�½���9/13/2007]
const DWORD CHANCE_DROP_EQUIP_WHEN_DEAD			= 0x00010000;		//  [1/27/2008 ���]	��������һ�����ʵ���װ��
const DWORD UNKIDNAP							= 0x00020000;	//  [1/27/2008 ���]	���ɰ�

const int	MAGIC_NONE				= 0;
const int	GEM_NONE				= 99999;
const int	GEM_HOLE				= 255;

//////////////////////////////////////////////////////////////////////
// ��Ʒ��ķ���
const int	ITEMSORT_INVALID				=	-1;
const int	IETMSORT_FINERY1				=	1;	// װ��
const int	IETMSORT_FINERY2				=	2;	// װ��
const int	ITEMSORT_EXPEND					=	3;	// �׺�Ʒ
const int	ITEMSORT_GEM					=	4;	// ��ʯ
const int	ITEMSORT_OTHER					=	5;	// action��Ʒ
const int	ITEMSORT_NORMAL					=	6;	// ����
const int	IETMSORT_EUDEMON				=	7;	// ����

//////////////////////////////////////////////////////////////////////
// ITEMSORT_FINERY �����Ʒ
const int	ITEMTYPE_INVALID		=	-1;	  // �Ƿ�
const int	ITEMTYPE_SHOULDER		=	11;	// ����
const int	ITEMTYPE_ARMOR			=	12;	// ����
const int	ITEMTYPE_HELMET			=	13;	// ͷ��
const int	ITEMTYPE_WEAPONR		=	14;	// ����
const int	ITEMTYPE_NECKLACE		=	15;	// ����
const int	ITEMTYPE_BANGLE			=	16;	// ����
const int	ITEMTYPE_GLOVE			=	17;	// ����
const int	ITEMTYPE_CAESTUS		=	18;	// Ҫ��
const int	ITEMTYPE_SHOES			=	19;	// Ь��
const int	ITEMTYPE_MOUNT			=	20;	// ����
const int	ITEMTYPE_RING			=	21;	// ��ָ
const int	IETMTYPE_ADORN			=	22;	// ��Ʒ
const int   ITEMTYPE_DRESS			=   23; // ��װ
const int   ITEMTYPE_TREASURE		=   24; // ����
const int   ITEMTYPE_WEAPONSOUL		=   25; // ���
const int   ITEMTYPE_SOULBOOK       =   43; // ������


//////////////////////////////////////////////////////////////////////
//const int	ITEMPOSITION_NONE		= 0;
const int	ITEMPOSITION_EQUIPBEGIN	=	1;
const int	ITEMPOSITION_SHOULDER	=	1;	// ����
const int	ITEMPOSITION_ARMOR		=	2;	// ����
const int	ITEMPOSITION_HELMET		=	3;	// ͷ��
const int	ITEMPOSITION_WEAPONR	=	4;	// ����
const int	ITEMPOSITION_NECKLACE	=	5;	// ����
const int	ITEMPOSITION_BANGLE		=	6;	// ����
const int	ITEMPOSITION_GLOVE		=	7;	// ����
const int	ITEMPOSITION_CAESTUS	=	8;	// Ҫ��
const int	ITEMPOSITION_SHOES		=	9;	// Ь��
const int   ITEMPOSITION_MOUNT      =	10; // ����
const int	ITEMPOSITION_RINGT		=	11;	// �Ͻ�ָ	
const int	ITEMPOSITION_RINGB		=	12;	// �½�ָ
const int	ITEMPOSITION_ADORNT		=	13;	// ����Ʒ	
const int	ITEMPOSITION_ADORNB		=	14;	// ����Ʒ
const int   ITEMPOSITION_DRESS		=	15; //��װ
const int   ITEMPOSITION_WEAPONSOUL	=	16; //���
const int	ITEMPOSITION_EQUIPEND	=	17;//

const int	ITEMPOSITION_PACK_BEGIN			= 47;
const int	ITEMPOSITION_BACKPACK1			= 47;	// ��ͨ��Ʒ����1
const int	ITEMPOSITION_BACKPACK2			= 48;	// ��ͨ��Ʒ����2
const int	ITEMPOSITION_BACKPACK3			= 49;	// ��ͨ��Ʒ����3
const int	ITEMPOSITION_PACK_END			= 50;
const int	ITEMPOSITION_PETPACK			= 99;
const int	ITEMPOSITION_PACK_LIMIT			= 100;

//////////////////////////////////////////////////////////////////////
const int	ITEMPOSITION_STORAGE	= 201;			
const int	ITEMPOSITION_TRUNK		= 202;
const int	ITEMPOSITION_CHEST		= 203;
const int	ITEMPOSITION_PLAYERTASK	= 204;			// Ӷ������Ʒ
const int   ITEMPOSITION_SENDBOX	= 205;			// �ʾַ�����
const int	ITEMPOSITION_GROUND		= 254;			// �������ݿ�
const int	ITEMPOSITION_NONE		= 255;			// �Ƿ�λ��
const int   ITEMPOSITION_MARRYSTORAGE = 208;        //�����ֿ�

const int   ITEMPOSITION_MARRY_BED_MATERIAL1 = 209;        //������1
const int   ITEMPOSITION_MARRY_BED_MATERIAL2 = 210;        //������2
const int   ITEMPOSITION_MARRY_BED_MATERIAL3 = 211;        //������3

const int   ITEMPOSITION_MARRY_DEPORT_MATERIAL1 = 212;        //���Ӳ���1
const int   ITEMPOSITION_MARRY_DEPORT_MATERIAL2 = 213;        //���Ӳ���2
const int   ITEMPOSITION_MARRY_DEPORT_MATERIAL3 = 214;        //���Ӳ���3

const int   ITEMPOSITION_MARRY_FLOWER_MATERIAL1 = 215;        //������1
const int   ITEMPOSITION_MARRY_FLOWER_MATERIAL2 = 216;        //������2
const int   ITEMPOSITION_MARRY_FLOWER_MATERIAL3 = 217;        //������3

const int   ITEMPOSITION_MARRY_CABINET_MATERIAL1 = 218;        //��������1
const int   ITEMPOSITION_MARRY_CABINET_MATERIAL2 = 219;        //��������2
const int   ITEMPOSITION_MARRY_CABINET_MATERIAL3 = 220;        //��������3

//////////////////////////////////////////////////////////////////////
//06.12.29�������
//��ʯ����Ƕ��װ���е��ĸ�λ��
const USHORT	EMBEDPOS_INVALID	= 0x0000;//��Ч
const USHORT	EMBEDPOS_HELMET		= 0x0001;//ͷ��
const USHORT	EMBEDPOS_NECKLACE	= 0x0002;//����
const USHORT	EMBEDPOS_ARMOR		= 0x0004;//�·�
const USHORT	EMBEDPOS_WEAPON		= 0x0008;//����
const USHORT	EMBEDPOS_RING		= 0x0010;//��ָ
const USHORT	EMBEDPOS_GLOVE		= 0x0020;//����
const USHORT	EMBEDPOS_BANGLE		= 0x0040;//����
const USHORT	EMBEDPOS_CAESTUS	= 0x0080;//����
const USHORT	EMBEDPOS_SHOES		= 0x0100;//Ь��

const int CITYWAR_ITEM_TYPEID = 520000;      //��ս��ս��Ʒid
enum ITEMDATA
{
/*------------------item��-------begin--------------------------------------------------------------*/
		ITEMDATA_ID_=0,				// use for insert
		ITEMDATA_TYPE=1,			
		ITEMDATA_OWNERID,
		ITEMDATA_PLAYERID,
		ITEMDATA_AMOUNT,
		ITEMDATA_AMOUNTLIMIT,
		ITEMDATA_POSITION,
		ITEMDATA_PACKAGE_INDEX,
		ITEMDATA_KIDNAP,
		ITEMDATA_NEEDCHECK,
		ITEMDATA_LAST_CHECK_TIME,
		ITEMDATA_LIFE_TIME,
		ITEMDATA_QUALITY,
		ITEMDATA_STAR,
		ITEMDATA_ADDLEV,
		ITEMDATA_BASE1,
		ITEMDATA_BASE2,
		ITEMDATA_BASIC_ADDITION1,
		ITEMDATA_BASIC_ADDITION2,
		ITEMDATA_BASIC_ADDITION3,
		ITEMDATA_BASIC_ADDITION4,
		ITEMDATA_BASIC_ADDITION5,
		ITEMDATA_BASIC_ADDITION6,
		ITEMDATA_BASIC_ADDITION7,
		ITEMDATA_BASIC_ADDITION8,
		ITEMDATA_GEM1,
		ITEMDATA_GEM2,
		ITEMDATA_GEM3,
		ITEMDATA_MAKERNAME,
		ITEMDATA_ADDLEV_FAILRATE,//[2010-07-19 goto]ǿ��ʧ�ܺ�ļӳɳɹ��� 100%=10000
		ITEMDATA_ADDSTAR_FAILRATE,//[2010-07-19 goto]����ʧ�ܺ�ļӳɳɹ��� 100%=10000
		ITEMDATA_COLOR,
		ITEMDATA_BUYPRICE,
		ITEMDATA_GEM4,
		ITEMDATA_SOULSTATE,
/*---------------item��------end---------------------------------------------------------------------*/
/*---------------itemtype��-------begin--------------------------------------------------------------*/
		ITEMTYPEDATA_OFFSET=100,
		ITEMDATA_NAME=101,
		ITEMDATA_REQ_PROF,
		ITEMDATA_REQ_USERLEVEL,	
		ITEMDATA_REQ_JOBLEVEL,
		ITEMDATA_REQ_SEX,
		ITEMDATA_PRICE,
		ITEMDATA_PRICEOUT,
		ITEMDATA_ACTION,
		ITEMDATA_AMOUNT_INIT,
		ITEMDATA_AMOUNT_LIMIT_INIT,
		ITEMDATA_CAPABILITY_INIT,
		ITEMDATA_INIT_LIFE_TIME,
		ITEMDATA_LIFE,
		ITEMDATA_MANA,
		ITEMDATA_BASE1_INIT,
		ITEMDATA_BASE2_INIT,
		ITEMDATA_BASIC_ADDITION1_INIT,
		ITEMDATA_BASIC_ADDITION2_INIT,
		ITEMDATA_BASIC_ADDITION3_INIT,
		ITEMDATA_BASIC_ADDITION4_INIT,
		ITEMDATA_BASIC_ADDITION5_INIT,
		ITEMDATA_BASIC_ADDITION6_INIT,
		ITEMDATA_BASIC_ADDITION7_INIT,
		ITEMDATA_BASIC_ADDITION8_INIT,
		ITEMDATA_GEM1_INIT,
		ITEMDATA_GEM2_INIT,
		ITEMDATA_GEM3_INIT,
		ITEMDATA_SUIT_ID,
		ITEMDATA_MONOPOLY,
		ITEMDATA_DES,
		ITEMDATA_ITEMCD,
		ITEMDATA_COLOR_INIT,
		ITEMDATA_STAR_INIT,
/*-------------------itemtype��---------end------------------------------------------------------------*/
};
char	szItemTable[];
typedef	CGameData<ITEMDATA,szItemTable,szID>		CGameItemData;

//////////////////////////////////////////////////////////////////////
enum EUDEMONDATA{
	EUDEMONDATA_id = 0,			
	EUDEMONDATA_owner_id,
	EUDEMONDATA_typeid,
	EUDEMONDATA_playtime,//EUDEMONDATA_itemid,//����������ֶ���Ϊ��ˣ����
	EUDEMONDATA_name,
	EUDEMONDATA_life,
	EUDEMONDATA_level,					
	EUDEMONDATA_exp,				
	EUDEMONDATA_sex,	
	EUDEMONDATA_gettype,
	EUDEMONDATA_happy,
	EUDEMONDATA_understand,
	EUDEMONDATA_bone, 
	EUDEMONDATA_habit,
	EUDEMONDATA_lifespan,
	EUDEMONDATA_gift,
    EUDEMONDATA_borntime,
	EUDEMONDATA_grow,
	EUDEMONDATA_addpoint,
	EUDEMONDATA_firstr,
	EUDEMONDATA_stradd,
	EUDEMONDATA_strchg,
	EUDEMONDATA_firint,
	EUDEMONDATA_intadd,
	EUDEMONDATA_intchg,
	EUDEMONDATA_firsta,
	EUDEMONDATA_staadd,
	EUDEMONDATA_stachg,
	EUDEMONDATA_firspi,
	EUDEMONDATA_spiadd,
	EUDEMONDATA_spichg,
	EUDEMONDATA_firagi,
	EUDEMONDATA_agiadd,
	EUDEMONDATA_agichg,
	EUDEMONDATA_skill_1,
	EUDEMONDATA_skill_2,
	EUDEMONDATA_skill_3,
	EUDEMONDATA_skill_4,
	EUDEMONDATA_skill_5,
	EUDEMONDATA_skill_6,
	EUDEMONDATA_skill_7,
	EUDEMONDATA_skill_8,
	EUDEMONDATA_skill_9,
	EUDEMONDATA_skill_10,
	EUDEMONDATA_state,
	EUDEMONDATA_lastborn,
	EUDEMONDATA_failtime,
	EUDEMONDATA_MaxScore,
	EUDEMONDATA_skill1_lastuse,
	EUDEMONDATA_lastplay,//EUDEMONDATA_skill2_lastuse,//�ĳ���ˣʱ���ֶ�
	EUDEMONDATA_fusion,//Ĭ��
	EUDEMONDATA_intelligence,//����
};

struct EudemonData{
	//��̬��
	OBJID id;				
	OBJID ownerId;
	DWORD typeId;
	DWORD playtimes;	
	char name[64];
	DWORD life;	
	DWORD level;					
	DWORD exp;			
	DWORD sex;
	DWORD fusion;//Ĭ��
	DWORD intelligence;//����
	DWORD gettype;
	DWORD happy;
	DWORD understand;
	DWORD bone;
	DWORD habit;
	DWORD lifespan;
	DWORD gift;
	DWORD borntime;
	DWORD grow;	
	DWORD addpoint;

	DWORD firstr;
	DWORD stradd;
	DWORD strchg;
	DWORD firint;
	DWORD intadd;
	DWORD intchg;
	DWORD firsta;
	DWORD staadd;
	DWORD stachg;
	DWORD firspi;
	DWORD spiadd;
	DWORD spichg;
	DWORD firagi;
	DWORD agiadd;
	DWORD agichg;
	DWORD skill[10];
	DWORD failtime;

	//��̬��
	DWORD dwLookface;
	DWORD dwNeedlev;
	DWORD initgrow;
	DWORD maxstrchg;
	DWORD maxintchg;
	DWORD maxstachg;
	DWORD maxspichg;
	DWORD maxagichg;
	DWORD maxlifespan;

	//2������
	DWORD dwMaxLife;
	DWORD dwScore;
	DWORD dwState;
	DWORD dwfirStrChg;
	DWORD dwfirIntChg;
	DWORD dwfirStaChg;
	DWORD dwfirSpiChg;
	DWORD dwfirAgiChg;
	DWORD dwStrChg;
	DWORD dwIntChg;
	DWORD dwStaChg;
	DWORD dwSpiChg;
	DWORD dwAgiChg;
	DWORD dwStr;
	DWORD dwInt;
	DWORD dwSta;
	DWORD dwSpi;
	DWORD dwAgi;
	DWORD dwPhyAtt;
	DWORD dwMagAtt;
	DWORD dwPhyDef;
	DWORD dwMagDef;
	DWORD dwhit;
	DWORD dwDoge;
	DWORD dwCrit;
	DWORD dwTough;
};

char	szEudemonTable[];
typedef		CGameData<EUDEMONDATA, szEudemonTable, szID>	CEudemonData;


enum PEIFANGDATA{
	PEIFANGDATA_id = 0,			
	PEIFANGDATA_owner_id,
	PEIFANGDATA_peifangid,
};

struct ST_PEIFANGDATA
{
	OBJID id;
	OBJID owner_id;
	OBJID idPeifang;
};
char	szPeifangTable[];
typedef		CGameData<PEIFANGDATA, szPeifangTable, szID>	CPeifangData;

//////////////////////////////////////////////////////////////////////
#include "T_SingleObjSet2.h"
#include "T_SingleMap2.h"
#include "UserTable.h"
#include "ItemAddition.h"

//////////////////////////////////////////////////////////////////////
class IDatabase;
class IRecord;
class IRecordset;
class CItemData : public CGameObj  
{
protected:
	CItemData();
	virtual ~CItemData();

public:
 	bool		Create			(IDatabase* pDb, IRecord* pDefault, const ItemInfoStruct* pInfo, IRecord* pDefaultEudemonData, bool bInsert=true, OBJID idNew=ID_NONE );
	bool		CreateSend			(IDatabase* pDb, IRecord* pDefault, const ItemInfoStructSend* pInfo, IRecord* pDefaultEudemonData, bool bInsert=true, OBJID idNew=ID_NONE );	
//	bool		Create			(IRecord* pDefault, OBJID idType, OBJID idUser, int nPosition, IRecord* pDefaultEudemonData=NULL);//06.12.29����ע��
	bool		Create			(OBJID idItem, IDatabase* pDb);
	bool		Create			(IRecordset* pRes, IDatabase* pDb);
	bool		DeleteRecord	(IDatabase* pDb, bool bDelEudemon=true);
//	static bool		DeleteItemRecord	(OBJID id, IDatabase* pDb);

public: // info
	bool		GetInfo	(ItemInfoStruct* pInfo);
	bool		GetInfo	(ItemInfoStructSend* pInfo);
	bool		SaveInfo		(void);
	virtual void		OnSaveInfo(){};

public: // get set
	void		AdjustItemData();
	//bool		IsCanUpLevel();20070714����:�õ�CItem
	OBJID		GetID()					{ return m_pData->GetKey(); }

	int			GetInt(ITEMDATA idx);
	LPCTSTR		GetStr(ITEMDATA idx);
	void		SetInt(ITEMDATA idx, int nData, bool bUpdate = false);
	void		SetStr(ITEMDATA idx, LPCTSTR szData, int nSize, bool bUpdate = false);

	_int64		GetInt64(ITEMDATA idx);
	void		SetInt64(ITEMDATA idx, __int64 i64Data, bool bUpdate = false);
protected: // data & type
	CGameItemData*		m_pData;
	CItemTypeData*		m_pType;
	bool				m_bNeedUpdate;

};

#endif // !defined(AFX_ITEMDATA_H__28794933_A926_4C9A_AACF_1A95340DA94B__INCLUDED_)
