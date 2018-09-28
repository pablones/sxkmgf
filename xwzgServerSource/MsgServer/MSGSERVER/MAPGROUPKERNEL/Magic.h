// Magic.h: interface for the CMagic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGIC_H__B2689767_419D_487F_B0F9_FBB04C4619AA__INCLUDED_)
#define AFX_MAGIC_H__B2689767_419D_487F_B0F9_FBB04C4619AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "GameObj.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "UserTable.h"
#include "MagicType.h"
#include "T_SingleObjSet2.h"
#include "Myheap.h"
#include "MagicData.h"
#include "StrRes.h"


#pragma warning(disable:4786)
#include <vector>


enum EM_GIFT_MAGIC_FLAG
{
	EM_NOT_GIFT_MAGIC = 0,	//	��ͨ����
	EM_GIFT_MAGIC,			//	�츳����
	EM_GIFT_MAGIC_INVALID,	//  ������Ч
	EM_GIFT_MAGIC_REMOVE,	//	����ɾ��(����)
};

char	szMagicTable[];

enum MAGIC_TYPE
{
	MAGIC_PASSIVITY_AVENGER				 =2410,///����-������-������
	MAGIC_PASSIVITY_NATURALAFFINITY		 =2411,///����-������-��Ȼ�׺�
	MAGIC_PASSIVITY_NATURETHINK			 =2210,///����-������-���˺�һ
	MAGIC_PASSIVITY_EASYTAKE			 =2110,///����-������-�ֵ�����
	MAGIC_PASSIVITY_BEASTRECOVERY		 =2310,///����-������-Ұ�޻ָ�
	MAGIC_PASSIVITY_PAINSUPPRESS 		 =2211,///����-������-ʹ��ѹ��
	MAGIC_PASSIVITY_INNATENESSWARRIOR  	 =2111,///����-������-����սʿ
	MAGIC_PASSIVITY_MANNAAGILITY  		 =2311,///����-������-�������

	MAGIC_PASSIVITY_DCZ  = 1401,///����-����--������
	MAGIC_PASSIVITY_SLBN = 1402,///����-����--ɱ¾����
    MAGIC_PASSIVITY_YSLL = 1111,///����-����--սʿ-ԭʼ����-
	MAGIC_PASSIVITY_ZLFK = 1112,///����-����--սʿ--��������
	MAGIC_PASSIVITY_LLGR = 1311,///����-����--����--��������
	MAGIC_PASSIVITY_XLSH = 1312,///����-����--����--�������
	MAGIC_PASSIVITY_BMLH = 1211,///����-����--��ʦ--��������
	MAGIC_PASSIVITY_GGZH = 1212,///����-����--��ʦ--ب���ǻ�

	MAGIC_PASSIVITY_ZLZ  = 3401,///����-����--������
	MAGIC_PASSIVITY_DXJH = 3402,///����-����--��л����
    MAGIC_PASSIVITY_JYTZ = 3111,///����-����--սʿ--��������-
	MAGIC_PASSIVITY_TZBD = 3112,///����-����--սʿ--���ֲ���
	MAGIC_PASSIVITY_GCMB = 3311,///����-����--ǹ��--�ᴩĿ��
	MAGIC_PASSIVITY_DWXF = 3312,///����-����--ǹ��--�����鷢
	MAGIC_PASSIVITY_TNQH = 3211,///����-����--��ʦ--ͷ��ǿ��
	MAGIC_PASSIVITY_JSMX = 3212,///����-����--��ʦ--��˼ڤ��

//----------------����������--------------------------------------------------------------------------
	MAGIC_MZWQ = 4111,//  ��������--ħ������[12/14/2007 �½���]warriorWARRIOR
	MAGIC_JSZD = 4112,//  ��������--�������[12/14/2007 �½���]
	MAGIC_SZSJ = 4113,// ��������--��ս�پ� [12/14/2007 �½���]
	MAGIC_CSZJ = 4114,//��ʤ׷��
	MAGIC_WG   = 4115,// ��������--�ȹ� [12/14/2007 �½���]
	MAGIC_KZ   = 4116,//����
	MAGIC_GMXN = 4117,//��������
	MAGIC_JSLX = 4118,//��������
	MAGIC_DMKJ = 4119,//��ĥ����	4119	����	��õ������л�������֮�͵������ħ����
	MAGIC_MZWQ2 = 4311,//ħ������	4311	����	�����ͨ������10���ļ��ʸ�����������ֵ��ħ���˺���
	MAGIC_CTSJ = 4312,//��͸���	4312	����	�����������5���ļ�������Ŀ�����������
	MAGIC_SDFY = 4313,//���練Ӧ	4313	����	����5���ļ��ʺ��������˺���
	MAGIC_DKMF = 4314,//�ֿ�ħ��	4314	����	����5���ļ��ʺ���ħ���˺���
	MAGIC_FSCT = 4209,//������͸	4209	����	��ķ���������10���ļ�������Ŀ��ħ��
	MAGIC_BLFS = 4210,//������ʦ	4210	����	�����������10��
	MAGIC_XS   = 4211,//����	4211	����	��15���ļ������ܵ������˺�ʱ�ظ����ڸ��˺�20����HP��
	MAGIC_QHXS = 4212,//ǿ������	4212	����	���ջ�������20���ļ������ܵ�ħ���˺�ʱ�ظ����ڸ��˺�20����MP��
	MAGIC_SHFT = 4402,//�˺�����	4402	����	����5���ļ��ʷ����ܵ����˺���
	MAGIC_HB   = 4403,//�ر�	4403	����	�������ܵ��ķ�Χ�����˺�10����
	MAGIC_JR   = 4404,//����	4404	����	�������ܵ������ļ���10����
	MAGIC_SL   = 4405,//����	4405	����	�����Թ�����ɵ��˺������300��
	MAGIC_JH   = 4120,//����	4120	����	��ն/����/������10���ļ���ʹĿ��ѣ��1�루ս�����ܣ��˺�����ϣ���
	MAGCI_MZTB = 4315,//��׼�Ȳ�	4315	����	ǿ����/���ټ�/������10���ļ���ʹĿ�궨��2�롣
	MAGIC_HM   = 4213,//����	4213	����	���������10���ļ���ʹĿ�겻��ս�����ܵ��˺�����ʧ��������3�롣
	MAGIC_ZQ   = 4214,//եȡ	4214	����	��������15���ļ���Ϊ��ظ�HP��Ч���൱������˺���10����
	MAGIC_QHJHS = 4316,//ǿ��������	4316	����	���������ԶԵ���ʹ�ã�����Ŀ�������������״̬������״̬��Buff��
	MAGIC_QHQTJHS = 4317,//ǿ��Ⱥ�徻����	4317	����	ǿ��Ⱥ�徻�����ԶԵ���ʹ�ã�����Ŀ����Χ���˵�1������״̬����Χ���Ϊ20������״̬��Buff��
	MAGIC_JTMS = 4215,//��ͨĨɷ��	4215	����	Ĩɷ�����ʹ��ظ�HP��Ч�����Ϊ����˺���25����
	MAGIC_SXWB = 4216,//˲Ϣ���	4216	����	�ܵ����������������Т����ȴʱ�䡣
	MAGIC_QHSY = 4217,//ǿ��˲���ƶ�	4217	����	˲���ƶ����Խ��ս������״̬��

//---------------------------------------------------------------------------------------------------------------

	//��ն����������ն
	MAGIC_LONGZ = 1102 ,
	MAGIC_LANGY = 2102,
	MAGIC_LEIZ = 3102,

	//������ѣ�������,�������,Ĩɷ2
	MAGIC_TBDL = 9022,
	MAGIC_YJHL = 10000,
	MAGIC_KRLG2 = 4208,
	MAGIC_MOS2 = 6432,

	//ǿ����/���ټ�/����
	MAGIC_QLN = 1302,
	MAGIC_JSJ = 2302,
	MAGIC_SUS = 3302,
	
	//˳��ն��������
	MAGIC_SSZ  = 4105,
	MAGIC_FJCY = 6701,


	//���䡢ǿ���������׼���
	MAGIC_SS   = 3302,
	MAGIC_QLSJ = 4307,
	MAGIC_MZSJ = 6452,


	//���ܵ�������ͻϮ�����񹥻�
	MAGIC_CND  = 3203,
	MAGIC_JNTX = 3204,
	MAGIC_JSGJ = 6726,




	//ħ������������������������Ǳ�ܱ���
	MAGIC_MLZF = 4201,
	MAGIC_FSBH = 6827,
	MAGIC_FL   = 6427,
	MAGIC_QNBF = 6829,

	//��־����ֲ���С��̶���Х�������⻷���мܡ�ͭǽ����
	MAGIC_DZAY = 4101,
	MAGIC_KBHJ = 4102,
	MAGIC_CEJX = 6803,
	MAGIC_YQGH = 6804,
	MAGIC_ZJ   = 4107,
	MAGIC_TQTB = 6811,
	//Ԥ��Σ�ա���սŮ�ס�����Ƥ��

	MAGIC_YJWX = 1301,
	MAGIC_TZLW = 6551,
	MAGIC_JYPF = 6552,
	

	MAGIC_BARB			 =2101,///����
	MAGIC_DEATHCHAINS	 =4104,///��������
	MAGIC_DEATHHANDSHAKE =6807,///����֮��
	MAGIC_HEALMAGIC =4301,///������
	MAGIC_COLONYHEAL =6851,///Ⱥ��������
	MAGIC_ENERGYSHIELD =2202,///��Ȼ֮��
	MAGIC_CRAZEAFFLATUS =6826,///�������

	//satan 
	MAGIC_SSQY = 1119,//ѳ��ȡ��
	//goto ���������б䶯,���Ա��Ϊ׼,����ע���е�����Ϊ׼
	MAGIC_JGBH	=9001,//��ղ���
	MAGIC_JGBRZ	=9002,//��հ�����
	MAGIC_TBS	=9003,//������
	MAGIC_LY	=1124,//����
	MAGIC_ZYHT	=9004,//��Ԫ����
	MAGIC_WJJ	=9008,//�򽣾�,����Ŀ��
	MAGIC_BACK	=9000,//�س�
	MAGIC_BACKSYN=2501,//�ذ�
//modify code by �ֵ�Ԩ begin 2011.6.22
	MAGIC_LGWS  =4010,//�ҹ�����
	MAGIC_FJCY_2=4011,//������
	MAGIC_ZHF   =4012,//׷�긫
	MAGIC_MDSX  =4013,//�Զ�����
//modify code by �ֵ�Ԩ end
	//���＼��
	MAGIC_XX	=8001,//��Ѫ
	MAGIC_GJXX	=8002,//�߼���Ѫ
	MAGIC_XQ	=8003,//����
	MAGIC_GJXQ	=8004,//�߼�����
	MAGIC_DN	=8005,//��ŭ
	MAGIC_GJDN	=8006,//�߼���ŭ
	MAGIC_LJ	=8007,//����
	MAGIC_GJLJ	=8008,//�߼�����
};

enum MAGIC_GROUP_TYPE
{
	MAGICGROUP_NONE	=0,
	MAGICGROUP_ATTACK_NOMOVE,	     // �ϻ�1������ԭ�ع���
	MAGICGROUP_ATTACK_MOVE,		     // �ϻ�2��������λ����(��λ����)
	MAGICGROUP_ATTACK_RAND,		     // ���޹�����ҺϷ���Χ�ڵ�ĳһ�������
	MAGICGROUP_ATTACK_SAME,		     // ���޹����������ͬ��Ŀ��
	MAGICGROUP_STANDAR_UINON_ATTACK, // ����ù̶���ϼ����� tcp2005.9.21 add
	MAGICGROUP_FREE_UINON_ATTACK,    // �����������ϼ����� tcp2005.9.21 add	
};
//////////////////////////////////////////////////////////////////////
enum {
		MAGICSORT_ATTACK			= 1,
		MAGICSORT_RECRUIT			= 2,			// support auto active.
		MAGICSORT_CROSS				= 3,
		MAGICSORT_FAN				= 4,			// support auto active(random).
		MAGICSORT_BOMB				= 5,
		MAGICSORT_ATTACHSTATUS		= 6,
		MAGICSORT_DETACHSTATUS		= 7,
		MAGICSORT_SQUARE			= 8,
		MAGICSORT_FLYSTRIKE		= 9,				// move, a-lock
		MAGICSORT_RANDOMTRANS		= 10,			// move, a-lock
		MAGICSORT_DISPATCHXP		= 11,
		MAGICSORT_COLLIDE			= 12,			// move, a-lock & b-synchro	
		MAGICSORT_SERIALCUT			= 13,			// auto active only.
		MAGICSORT_LINE				= 14,			// support auto active(random).
		MAGICSORT_ATKRANGE			= 15,			// auto active only, forever active.
		MAGICSORT_ATKSTATUS			= 16,			// support auto active, random active.
		MAGICSORT_CALLTEAMMEMBER	= 17,
		MAGICSORT_RECORDTRANSSPELL	= 18,
		MAGICSORT_TRANSFORM			= 19,
		MAGICSORT_ADDMANA			= 20,			// support self target only.
		MAGICSORT_LAYTRAP			= 21,
		MAGICSORT_DANCE				= 22,			// ����(only use for client)
		MAGICSORT_CALLPET			= 23,			// �ٻ���
		MAGICSORT_VAMPIRE			= 24,			// ��Ѫ��power is percent award. use for call pet
		MAGICSORT_INSTEAD			= 25,			// ����. use for call pet
		MAGICSORT_DECLIFE			= 26,			// ��Ѫ(��ǰѪ�ı���)
		MAGICSORT_GROUNDSTING		= 27,			// �ش�
		MAGICSORT_REBORN			= 28,			// ���� -- zlong 2004.5.14
		MAGICSORT_TEAM_MAGIC		= 29,			// ���ħ������ ��MAGICSORT_ATTACHSTATUS��ͬ����
													//				�����������ֻ��Ϊ�˷���ͻ���ʶ��
		MAGICSORT_BOMB_LOCKALL		= 30,			// ��MAGICSORT_BOMB������ͬ��ֻ������ȫ��Ŀ��
		MAGICSORT_SORB_SOUL			= 31,			// ����ħ��
		MAGICSORT_STEAL				= 32,			// ͵���������Ŀ������͵ȡpower����Ʒ
		MAGICSORT_LINE_PENETRABLE	= 33,			// �����߹켣���Դ��˵����Թ���

		//////////////////////////////////////////////
		// ��������ħ������
		MAGICSORT_BLAST_THUNDER			= 34,			// ħ��
		MAGICSORT_MULTI_ATTACHSTATUS	= 35,			// Ⱥ��ʩ��״̬
		MAGICSORT_MULTI_DETACHSTATUS	= 36,			// Ⱥ����״̬
		MAGICSORT_MULTI_CURE			= 37,			// Ⱥ�岹Ѫ
		MAGICSORT_STEAL_MONEY			= 38,			// ͵Ǯ
		MAGICSORT_KO					= 39,			// ��ɱ����Ŀ��ѪС��15%�Զ�����
		MAGICSORT_ESCAPE				= 40,			// ����/����
//		MAGICSORT_FLASH_ATTACK			= 41,			// ���λ�λ	
//		MAGICSORT_ATTRACK_MONSTER		= 42,			// ��������
		MAGICSORT_PASS_THROUGH			= 43,			// ���

		MAGICSORT_PRECONDITION			= 44,
		MAGICSORT_SPACETIMEDOOR			= 55,	//ʱ����

		MAGICSORT_MONSTER_POINTATTACK	= 56,	//����ĵ㹥������
		MAGICSORT_MONSTER_CIRCLEATTACK	= 57,	//����ķ�Χ���ܹ���,�Թ�����ΪԲ��,һ���뾶��Բ��Χ

		MAGICSORT_STOPTRAP				= 58,	//��ֹ����
		MAGICSORT_DRAGONHEART			= 59,	//��֮��
		MAGICSORT_BELLOW				= 60,	//ǿ������
		MAGICSORT_CONGEAL_SPRITGRID		= 61,	//ԭ������
		MAGICSORT_DIMENSION_DOOR		= 62,	//˲���ƶ� Dimension Door: Teleports you short distance.
		MAGICSORT_SWITCHSTATUS		    = 63,	///softworms-2007-08-13����״̬����,���״̬����,��ȥ����״̬,��֮����Ӹ�״̬.
		MAGICSORT_CLEARSKILLCD          = 64,   // �������CD [12/21/2007 �½���]
		MAGICSORT_SUMMON				= 65,	//20080202����:�ٻ�.
		MAGICSORT_TARGET_BACK			= 66,	//����Ŀ��
		MAGICSORT_RANDOM_MULTI			= 67,	//����������Ŀ��
		MAGICSORT_TIMESATK				= 68,	//����
		MAGICSORT_WITHDRAWATK			= 69,	//���˲�����
		MAGICSORT_BACK_CITY				= 90,	//�س�
		MAGICSORT_EUDEMON_SOUL			= 98,	//���︽�屻������
		MAGICSORT_BACK_SYN				= 91,	//�ذ�
};
enum MAGICLEVADDTYPE
{
	MAGICLEVADDTYPE_POW		=	0,//���������ӳ�
	MAGICLEVADDTYPE_MP		=	1,//�����ӳ�
	MAGICLEVADDTYPE_PRIVATECD	=	2,//˽��CD�ӳ�
	MAGICLEVADDTYPE_PUBLICCD	=	3,//����CD�ӳ�
	MAGICLEVADDTYPE_STEPSEC		=	4,//����ʱ��ӳ�
	MAGICLEVADDTYPE_TIMES		=	5,//���ô���
	MAGICLEVADDTYPE_TARGET		=	6,//����Ŀ������
	MAGICLEVADDTYPE_POW2		=	7,//��������2�ӳ�
	MAGICLEVADDTYPE_STATUS_POW	=	8,//����״̬�����ӳ�
};
//////////////////////////////////////////////////////////////////////
const int	WEAPONMAGIC_OFFSET				= 10000;	// �����ϴ���ħ�������IDֵ��ͬʱ��ITEMTYPE��MAGICx�ֶμӴ�ƫ�Ƶ���MAGICTYPE��ID
const int	WEAPONMAGIC_MIN					= WEAPONMAGIC_OFFSET;
const int	WEAPONMAGIC_MAX					= WEAPONMAGIC_OFFSET+256;
const int	WEAPONMAGIC_LEVEL	= 0;					// ȱʡ������ħ���ȼ�
const int	WEAPONR_MAGICID		= 4200000001;			// ����������ħ��ID
const int	WEAPONL_MAGICID		= 4200000002;			// ����������ħ��ID

//////////////////////////////////////////////////////////////////////
const int	MAGICDAMAGE_ALT					= 26;		// ����Ч���ĸ߲�����
const int	AUTOLEVELUP_EXP					= -1;		// �Զ������ı�־
const int	DISABLELEVELUP_EXP				= 0;		// �������ı�־
const int	AUTOMAGICLEVEL_PER_USERLEVEL	= 10;		// ÿ10���������ȼ���һ��
const int	USERLEVELS_PER_MAGICLEVEL		= 10;		// ��ҵȼ������Ƿ����ȼ���10��

const int	KILLBONUS_PERCENT				= 5;		// ɱ������Ķ���EXP����
const int	KILLBONUS_PERCENT_TEAM			= 20;		//20060905����:С��ɱ������Ķ���exp����
const int	HAVETUTORBONUS_PERCENT			= 10;		// �е�ʦ������¶���EXP����
const int	WITHTUTORBONUS_PERCENT			= 20;		// �͵�ʦһ�����������EXP����

const int	MAGIC_DELAY						= 1000;		// ħ��DELAY
const int	MAGIC_DECDELAY_PER_LEVEL		= 100;		// ÿ���������ȼ������ٵ�ħ��DELAY
const int	RANDOMTRANS_TRY_TIMES			= 10;		// ���˲�Ƶĳ��Դ���
const int	DISPATCHXP_NUMBER				= 20;		// ��XP������
const int	COLLIDE_POWER_PERCENT			= 80;		// ��ײ���ӹ������İٷֱ�
const int	COLLIDE_SHIELD_DURABILITY		= 3;		// ��ײҪ���ٵĶ�������
const int	LINE_WEAPON_DURABILITY			= 2;		// ֱ�߹���Ҫ���ٵ���������
const int	MAX_SERIALCUTSIZE				= 10;		// ˳��ն������
const int	AWARDEXP_BY_TIMES				= 1;		// �������Ӿ���ֵ
const int	AUTO_MAGIC_DELAY_PERCENT		= 150;		// ����ħ������ʱ���ӵ�DELAY
const int	BOW_SUBTYPE						= 500;		// ����SUBTYPE
const int	POISON_MAGIC_TYPE				= 1501;		// use for more status
const int	DEFAULT_MAGIC_FAN				= 120;		// 
//const int	STUDENTBONUS_PERCENT			= 5;		// ��ʦɱ��һֻ����ͽ�ܵõ��ľ���ٷֱ�

const int	MAGIC_KO_LIFE_PERCENT			= 15;		// ��ɱ����ȥѪ����
const int	MAGIC_ESCAPE_LIFE_PERCENT		= 15;		// ���ܼ�����Ч����������

		
const int	PERISH_TAGLIFE_PERCENT		= 20;		//���ܲ���������Ŀ��Ѫ��С�ڴ˰ٷֱȣ�����ʹ��
//�꼼
const int	SPECIALSKILL_TYPE_PERISH1	= 6005;		//���ܲ���������
const int	SPECIALSKILL_TYPE_PERISH2	= 6102;		//��������
const int	SPECIALSKILL_TYPE_PERISH3	= 6302;		//��˳����

const int	SPECIALSKILL_TYPE_BURNSOUL	= 6802;		//���ܷٻ����

const int	SPECIALSILL_TYPE_BURNMANA	= 8001;		//����ȼħ������ǻ꼼

const int	SPECIALSKILL_TYPE_BURNSOUL_MAX_POWER = 29999;// �ٻ��������˺� [11/5/2007]
const int	SPECIALSILL_TYPE_BURNMANA_MAX_POWER = 10000;// ȼħ�������˺�

const int	SPECIALSKILL_TYPE_THROWICEBALL = 8002;

const int   OPPOSE_TAGLIFE_PERCENT = 50;            // ���ܿ�����Ѫ��С�ڴ˰ٷֱ� [12/14/2007 �½���]
const int   SPECIALSKILL_TYPE_OPPOSE = 4116;       // ������Ѫ��С�ڰٷֱ�ʱ�����˺� [12/14/2007 �½���]

//VIPѧϰ�����ۿ�
const int	MAGIC_VIP_LEARN1	= 95;//1��VIP
const int	MAGIC_VIP_LEARN2	= 90;//2��VIP
const int	MAGIC_VIP_LEARN3	= 85;//3��VIP
//////////////////////////////////////////////////////////////////////
typedef	CGameData<MAGICDATA, szMagicTable, szID>	CMagicDataBase;
class	CMagicData : public IMagicData
{
protected:
	CMagicData()			{ m_pData = NULL;needsend = 0; m_nAddLev = 0;m_nLearn = 0;m_nIndex=-1;m_bNeedUpdate = false;}
	virtual ~CMagicData();
public: // overload
	static IMagicData* CreateNew()		{ return new CMagicData; }
	virtual int		GetMagicLevel();
	virtual void	ReleaseByOwner()	{ delete this; }
	virtual bool	Create(IRecordset* pRecordset/*,IDatabase* pDb = NULL*/);
	virtual bool	Create(IRecordset* pDefaultRecord, OBJID idNewKey);
	virtual bool	Create(ST_MAGIC_DATA* pMagicData);
	virtual int		GetInt(MAGICDATA nDataName);
	virtual void	SetInt(MAGICDATA idx, int nData);
	virtual void	SetStr(MAGICDATA idx, char* szData,int nSize);
	virtual bool	FindType();		//? ��������Ҫ���ô˺����ֹ�ͬ��

	virtual OBJID	GetID()				{ CHECKF(m_pData); return m_pData->GetID(); }
	virtual bool	Update();
	virtual void	ClearUpdateFlags()	{ CHECK(m_pData); m_pData->ClearUpdateFlags(); }
	virtual OBJID	InsertRecord()		{ CHECKF(m_pData); return m_pData->InsertRecord(); }
	virtual bool	DeleteRecord()		{ CHECKF(m_pData); return m_pData->DeleteRecord(); }
	virtual int		NeedSend()	;
	virtual int		GetIndex()			{ return m_nIndex; }
	virtual void	SetIndex(int nIndex){ m_nIndex = nIndex; }
		
protected:
	CMagicDataBase*	m_pData;
	bool m_bNeedUpdate;
	int  needsend;
	int m_nAddLev;
	int m_nLearn;
	int m_nIndex;//�����±꣬����ר��

protected: // ctrl
	MYHEAP_DECLARATION(s_heap)
};

//////////////////////////////////////////////////////////////////////
typedef	ISingleObjSet2<IMagicData>		IMagicSet;
typedef	CSingleObjSet2<IMagicData>		CMagicSet;

//////////////////////////////////////////////////////////////////////
struct MagicInfoStruct
{
	OBJID	id;
	OBJID	idOwner;
	int		nType;
	int		nLevel;
	DWORD	dwExp;
	clock_t	updateClock;	// ���� [5/30/2007]��ת����ͼ���õ�.�˼���CD�ĸ���ʱ��
	int		nHabit;//Ӱ��CD���Ը�
	int		nAffect;//Ӱ�����ֵ,������ʾ���٣�������ʾ����
	USHORT	usCDIndex;//CD�±꣬����ר��
};
//[2010-08-03 goto]Ŀ�������������ܹ�������ʱ������������ȡ����С��Ŀ��
struct BeAttackSort 
{
	IRole* pRole;
	int	   nDistance;
};
////////////////////////////////////////////////////////////////
enum
{
	TYPE_MAGIC		= 0, // ħ��
	TYPE_XPSKILL	= 1, // xp��
	TYPE_KONGFU		= 2, // �书��ʽ
};

//////////////////////////////////////////////////////////////////////
enum
{
	MAGICSTATE_NONE		= 0,		// ��״̬
	MAGICSTATE_INTONE	= 1,		// ����״̬
	MAGICSTATE_LAUNCH	= 2,		// ʩչ״̬
	MAGICSTATE_DELAY	= 3,		// �ӳ�״̬
	PHYSICAL_ATT        = 4,        // ������
};

//////////////////////////////////////////////////////////////////////
enum AUTO_ACTIVE{
	AUTOACTIVE_KILL			= 0x01,
	AUTOACTIVE_FOREVER		= 0x02,
	AUTOACTIVE_RANDOM		= 0x04,
	AUTOACTIVE_HIDDEN		= 0x08,		// �����������ħ������ʾ�ڿͻ���
	AUTOACTIVE_EXPDISABLE	= 0x10,		// �����������ħ��ʹ�ù����в��ܵõ�����
};
enum MASK_BIT{
	MAGICMASKBIT_IGNOREPKDIABLE = 0x01,//�ɻ����ͼPK����
};
//////////////////////////////////////////////////////////////////////
enum THROW_WEAPON{
	THROW_WERPONR_TARGET			= 1,
};

//////////////////////////////////////////////////////////////////////
inline bool		IsWeaponMagic(int nType)			{ return nType >= WEAPONMAGIC_MIN && nType < WEAPONMAGIC_MAX; }
inline int		MagicType(int idx)					{ int nType = idx + WEAPONMAGIC_OFFSET; CHECKF(IsWeaponMagic(nType)); return nType; }
inline OBJID	POSITION2MAGIC_ID(int nPosition)	{ if(nPosition == ITEMPOSITION_WEAPONR) return WEAPONR_MAGICID; else return WEAPONL_MAGICID; }
void	DDALine(int x0, int y0, int x1, int y1, int nRange, vector<POINT>& vctPoint);

//////////////////////////////////////////////////////////////////////
typedef struct				// �ϻ�ʱ,�������λ��
{
	DWORD	idEudemon;	
	USHORT	usOffsetX, usOffsetY;
} EudemonPos_ST;

// dwData [0][0][5][1][0][6] ����(+05, -06)
//////////////////////////////////////////////////////////////////////
class CMagic  
{
protected:
	CMagic(PROCESS_ID idProcess, IRole* pRole);
	virtual ~CMagic();

private:
	CMagic() { m_bAutoAttack = false;}

public:
	static CMagic* CreateNew(PROCESS_ID idProcess, IRole* pRole);
	ULONG	ReleaseByOwner()	{ delete this; return 0; }
	void	OnTimer(DWORD tCurr);
	void	ClearMagicCD();//�������CD ���
public: // login
	bool	CreateAll();
	int		CreateAllCd();
	int		SendInfoToClient();

public: // MapGroup
	bool	SaveInfo();
	bool	GetInfoByIndex(MagicInfoStruct* pInfo, int idx);
	bool	AppendMagic(const MagicInfoStruct* pInfo);

public: // application
	IMagicSet*	QuerySet()		{ CHECKF(m_setData); return m_setData; }
	bool		MagicAttack(int nType, OBJID idTarget, int x, int y, UCHAR ucAutoActive = 0,bool needLearn = false,int learnLev = 0);		// return false: NO BROADCAST
	bool		AbortMagic(bool bSynchro=true/*false*/);// halei [5/25/2007] ������ֹħ��Ӧ�úͿͻ�������ͬ��
	bool		TestMagicCastEnable(OBJID idMagicType,OBJID idTarget,int x,int y);

	bool		UserKillTarget(IRole* pTarget);
	bool		AutoAttackTarget(IRole* pTarget);
	IMagicData*	FindMagic(int nType);
	int         GetAtkRange();
	bool		AwardExp(int nType, int nExp = AWARDEXP_BY_TIMES);
	bool		AwardExp(int nType, int nBattleExp, int nExp);
//	bool		AwardExp(int nBattleExp, int nExp = AWARDEXP_BY_TIMES, bool bIgnoreFlag=false);		// ͬʱ֧������ã���bAddBattleExpӦ��Ϊ0�����ڱ�����
	void		BreakAutoAttack();
	void		DebugReloadAll(){
					for(int i = 0; i < m_setData->GetAmount(); i++)
						m_setData->GetObjByIndex(i)->FindType();
				}

	bool		IsIntone()		{ return m_nMagicState == MAGICSTATE_INTONE; }	//return (m_tIntone.IsActive() && !m_tIntone.IsTimeOut()); }
	bool		IsInLaunch()	{ return m_nMagicState == MAGICSTATE_LAUNCH; }	//return (m_tApply.IsActive() && !m_tApply.IsTimeOut()); }
	bool		IsActive()		{ return (m_nMagicState != MAGICSTATE_NONE); }
	
	//-----���---2007/06/08----��Userħ��CDʱ��û��ʹ��ħ��ʱ����ʹ��������
	BOOL		IsCanPhysicalAttack()		{ if( m_nMagicState == MAGICSTATE_DELAY || m_nMagicState == MAGICSTATE_NONE) return true; return false;}
	
	///[2009.5.19--�Ŷ�]��ȡ����CD
	DWORD GetPublicCDTime() { return m_nPublicCDTime;}
	DWORD GetPublicCDInterval() { return m_nPublicCDInterval;}
	int	  GetMagicScore();

public: // static
	static		CMagicTypeData*		FindMagicType	(int nType/*, int nLevel*/);
	static		int					CalcMagicPower	(IRole* pAtker, IRole* pTarget, int* pAdjustAtk=0);

protected:
	void		ShowMiss();
	int			Attack(int nType, IRole* pRole);		// return lose life
	int			Attack(int nType, int x, int y);		// return lose life
	int			Launch();
	bool		UpLevel(IMagicData* pMagic,int nLev, bool bSynchro = true);
	bool		CheckCondition(IMagicData* pData, 
		OBJID idTarget, 
		int x, 
		int y, 
		bool bSpiritSkill,
		char *szError=NULL);//20070614����:�������������Ϣ
	bool		CheckCrime(const vector<IRole*>* pSet);
	bool		CheckCrime(IRole* pRole);

	bool		LockTarget(bool bLock);
public: // const
	enum { HIT_BY_MAGIC, HIT_BY_WEAPON, HIT_BY_ITEM };
	bool		IsImmunity(IRole* pRole, bool bSendMsg=true);
	bool		HitByMagic();
	bool		HitByWeapon();
	int			GetDieMode();
	bool		IsKeepBow()						{ return false; }	//m_pMagic && m_pMagic->GetInt(MAGICDATA_SORT) == MAGICSORT_ATTACHSTATUS && m_pMagic->GetInt(MAGICDATA_STATUS) == STATUS_KEEPBOW; }
	int			GetPower(int nStepIndex = -1);
	OBJID		GetType();
//	int			GetApplyMS(int nStepIndex = -1);
	ATK_TYPE	GetAtkType() const;
public: // task
	bool		CheckType(int nType);
	bool		CheckLevel(int nType, int nLevel);

public: // create
	void		ResetDelay();
//	OBJID		LearnMagic(int nType, int nLevel=0, bool bSaveMagic=true);
	OBJID		LearnMagic(int nType, 
		                   int nLevel=0,
						   bool bSaveMagic=true,
						   int nGiftFlag = EM_NOT_GIFT_MAGIC,
						   DWORD dwAddLev = NULL,
						   DWORD dwLearn = NULL);
	void        ClearDelay();// �������CD [12/21/2007 �½���]

public:
//	bool		IsGroupMagic(IMagicData* pData)	{ return pData->GetInt(MAGICDATA_GROUP_TYPE) != MAGICGROUP_NONE; }
protected: // process
	bool		ProcessAttack();
	bool		ProcessAtkStatus();
	bool		ProcessRecruit();
	bool		ProcessCross();
	bool		ProcessLine();
	bool		ProcessFan();
	bool		ProcessBomb();
	bool		ProcessBombLockAll();
	bool		ProcessSquare();
	bool		ProcessAttach();
	bool		ProcessSwitchStatus();///״̬���ش�����,softworms2007-08-13
	bool		ProcessDetach();
	bool		ProcessJumpAttackFaild(int x, int y);
	bool		ProcessFlyStrike();								//������������
	bool		ProcessDragonHeart();							//��֮��
	bool		ProcessRandomTrans();
	bool		ProcessDispatchXP();
	bool		ProcessCollideFail(int x, int y, int nDir);
	bool		ProcessCollide();
	bool		ProcessSerialCut();
	bool		ProcessCallTeamMember();
	bool		ProcessRecordTransSpell();
	bool		ProcessTransform();
	bool		ProcessAddMana();
	bool		ProcessLayTrap();
	bool		ProcessCallPet();
	bool		ProcessDecLife();
	bool		ProcessGroundSting();
	bool		ProcessReborn();
	bool		ProcessSorbSoul();
	bool		ProcessSteal();

	bool		ProcessDimensionDoor();
	bool		ProcessSummon();//20080202����:�ٻ�
	bool		ProcessPreCondition();
	bool        ProcessClearCD();//  [12/21/2007 �½���]
	////////////////////////////////////////////////////////
	// ��������ħ������
	bool		ProcessBlastThunder();
	bool		ProcessMultiAttach();
	bool		ProcessMultiDetach();
	bool		ProcessMultiCure();
	bool		ProcessStealMoney();
	bool		ProcessKO();
	bool		ProcessEscape();
//	bool		ProcessFlashAttack();
//	bool		ProcessAttrackMonster();
	bool		ProcessPassThrough();
	bool		Process_StopTrap();
	bool		Process_Bellow();


	int			AdjustMagicPower(int nPower,int nMagicType, int nlev=1, MAGICLEVADDTYPE nAddType=MAGICLEVADDTYPE_POW);///�������ħ��ֵǿ��ֵ,softworms-2007-09-05
//	int			AdditionPower(int nPower1,int nPower2);//��Powerֵ���мӳ�.softworms-2007-09-05

	typedef std::vector<IRole*>	ROLE_SET;
	enum { LOCK_NONE, LOCK_ONE, LOCK_ALL };
	bool		CollectTargetSet_Bomb(ROLE_SET& setRole, POINT& pos, int nLockType = LOCK_NONE );

	bool		CollectTargetSet_Team(ROLE_SET& setRole);
	bool		CollectTargetSetByPos(ROLE_SET& setRole, POINT& pos);
	
protected: // spawn id
	static OBJID	SpawnMagicID()						{ return InterlockedIncrement(&m_lockNextID); }

protected: // data
	IMagicSet*		m_setData;

protected: // intone
	IMagicData*		m_pMagic;
	OBJID			m_idTarget;
	POINT			m_pos;
	int				m_nData;
	bool			m_bAutoAttack;
// ����ע�� [5/30/2007]---------begin-----------------------------------------------
//	int				m_nRawDelay;		// fast delay for trans map
//	int				m_nDelay;			// normal delay
//--------------------------------end-----------------------------------------------

	int				m_nMagicState;		// ħ��״̬	-- zlong 2004-11-30

	CTimeOutMS		m_tIntone;			// ������ʱ
	CTimeOutMS		m_tApply;			// ÿ��Ч����ʱ
	int				m_nApplyTimes;		// ����Ч������
	CTimeOutMS		m_tDelay;			// ʩչ������ʱ

	vector<OBJID>	m_setTargetLocked;	// ħ���������й켣��Ŀ�꼯
	bool			m_bTargetLocked;
	vector<EudemonPos_ST>	m_setEudemonInfo;		// �ϻ�2ʱ�������λ��
	int						m_nAttackSameAmount;	// �ϻ�4 ��������
private: // cheat --------
	OBJID			m_cheatTargetID;
	int				m_cheatCount;
	bool			m_bIsSpendMp;
	int				nSpendMp;
	DWORD           m_nPublicCDTime;     //�ϴι���CDʱ��
	DWORD           m_nPublicCDInterval; //����CDʱ��
	int				m_nSpendAng;		 //����ŭ��
	bool			m_bIsSpendAng;
// XP add by arhun
public:
//	int GetMagicLevel(IMagicData* pData);
	bool MyProcessDo();
	BOOL HasSameWargameStatus(IRole* pRole);
	BOOL CanRecurit(IRole* pRole);	//  [2007-9-29] ���	�ɷ��Ѫ

	bool Process_SpaceTimeDoor();
//	bool Process_CongealSpiritStone();
	bool ProcessAtkBack();
	bool ProcessBackCity();
	bool ProcessBackSyn();
	bool ProcessRandomMultiAtk();
	bool ProcessTimesAtk();
	bool ProcessWithDrawAtk();
	int  ProcessSynMagic(CUser* pUser, IRole* pTargeter, int nSynRank=100);//���ɼ���,ְλĬ��Ϊ100����Ϊ����Ҳ��ѧϰ���а��ɼ���
	int	 ProcessSoulEudemonMagic(CUser* pUser, IRole* pTargeter);

//modify code by �ֵ�Ԩ begin 2011.6.22
	int ProcessBuffBomb(IRole *m_pOwner,int flag);
	bool CollectTargetSet_BuffBomb(ROLE_SET& setRole, POINT& pos, int nLockType,int irange);
//modify code by �ֵ�Ԩ end
	void        setMagicState(int state) { m_nMagicState = state;	}
	int         getMagicState(void) { return m_nMagicState;	}
	bool		CheckMagicLevAmount(int nLev, int nAmount, int nProf);
protected: // ctrl
	PROCESS_ID		m_idProcess;
	IRole*			m_pOwner;
	static long		m_lockNextID;
	CUser*          m_pUser;
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_MAGIC_H__B2689767_419D_487F_B0F9_FBB04C4619AA__INCLUDED_)
