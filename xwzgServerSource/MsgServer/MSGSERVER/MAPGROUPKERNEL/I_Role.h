// RoleManager.h: interface for the CRoleManager class.
// 
//////////////////////////////////////////////////////////////////////

#if !defined(I_ROLE_HEADER)
#define I_ROLE_HEADER

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include "../share/define.h"
#include "GameObjSet.h"
#include "TimeOut.h"

enum { POSE_STAND=0, };

const int	ADJUST_PERCENT			= 30000;						// ADJUSTʱ��>=30000 ��ʾ�ٷ���
const int	ADJUST_SET				= -30000;						// ADJUSTʱ��<=-30000 ��ʾ����(-1*num - 30000)
const int	ADJUST_FULL				= -32768;						// ADJUSTʱ��== -32768 ��ʾ����
const int	DEFAULT_DEFENCE2		= 10000;						// ���ݿ�ȱʡֵ

//���ɼ���
//06.4.24�������
const int SYNRANK_SIX	= 6;
const int SYNRANK_FIVE	= 5;
const int SYNRANK_FOUR	= 4;
const int SYNRANK_THREE	= 3;
const int SYNRANK_TWO	= 2;
const int SYNRANK_ONE	= 1;

const DWORD TIME_FOOTPRINT	= 1000;

const int	TEAM_STATUS_REQ_ROLES	= 3;		// ���״̬��Ч��Ҫ�������������

const int	MAX_FRENZY_LIFE_PERCENT	= 70;		// ������������ٷֱȾͻ�ȡ����״̬


///////////////////////////////////////
// �޸�ħ��ϵͳ��Ҫ��
const int	ARROW_FLY_ALT					= 60;							// ��֧���и߶�


//---����---begin
//////////////////////////////////////////////////////////////////////
//06.4.21�����޸�
enum {
		//////////////////////////
		//ͨ����λ�����жϽ�ɫ��������
		/////////////////////////
		RANK_LEADER			= 100,			//�����ų�		
		RANK_SUBLEADER		= 90,			//���ų�
		RANK_FEELLEADER		= 85,			//��Ӣ
		RANK_NORMAL			= 10,			//��ͨ��Ա		
	//////////////////////--jay���·���
		RANK_FEELLEADER1	= 81,			//����1
		RANK_FEELLEADER2	= 82,			//����2
		RANK_FEELLEADER3	= 83,			//����3
		RANK_FEELLEADER4	= 84,			//����4
		RANK_SUBFEELLEADER1	= 71,			//������1
		RANK_SUBFEELLEADER2	= 72,			//������2
		RANK_SUBFEELLEADER3	= 73,			//������3
		RANK_SUBFEELLEADER4	= 74,			//������4
		RANK_FEELMENBER1	= 61,			//����1��Ӣ
		RANK_FEELMENBER2	= 62,			//����2��Ӣ
		RANK_FEELMENBER3	= 63,			//����3��Ӣ
		RANK_FEELMENBER4	= 64,			//����4��Ӣ
		RANK_NORMAL1		= 51,			//����1����
		RANK_NORMAL2		= 52,			//����2����
		RANK_NORMAL3		= 53,			//����3����
		RANK_NORMAL4		= 54,			//����4����

		RANK_SHOP			= 20,			//��ũ

		RANK_NONE			= 0,
};

enum
{
	SYNMEM_LEVEL_ONE		= 1,
	SYNMEM_LEVEL_TWO		= 2,
	SYNMEM_LEVEL_THREE		= 3,
	SYNMEM_LEVEL_FOUR		= 4,
	SYNMEM_LEVEL_FIVE		= 5,		
};

//---����---end
//////////////////////////////////////////////////////////////////////
enum 
{
		DIE_NORMAL			= 1,
		DIE_CRITICALHIT		= 2,
		DIE_MAGIC			= 3,
		DIE_FLOAT			= 4,
		DIE_ARROWSHOOT		= 5,
};

//////////////////////////////////////////////////////////////////////
enum { OWNER_NONE, OWNER_USER, OWNER_SYN, OWNER_NPC,OWNER_RACE/*softworms-2007-08-11-����ӵ�õ�ͼ*/ };

//************************************************************************************************************************************************
// �ⲿ״̬����Ҫͨ��MsgPlayer֪ͨ������ҵ�״̬����ͻ���ͬ��
const I64		KEEPEFFECT_NORMAL						=	0x0000000000000000; //	������״̬
const I64		KEEPEFFECT_DIE							=	0x0000000000000001;	//	����
const I64		KEEPEFFECT_CRIME						=	0x0000000000000002;	//	����
const I64		KEEPEFFECT_ORANGE						=	0x0000000000000004;	//	�õ���ʱ��֮�ᡱ//����
const I64		KEEPEFFECT_TEAMLEADER					=	0x0000000000000008;	//	�ӳ�
const I64		KEEPEFFECT_PK_PROTECT					=	0x0000000000000010;	//	PK����
const I64		KEEPEFFECT_DISAPPEARING					=	0x0000000000000020; // ʬ����ʧ״̬
const I64		KEEPEFFECT_ATTRACTMONSTER				=	0x0000000000000040;	//��������

const I64		KEEPEFFECT_DOT_POISONING				=	0x0000000000000080;	//�ж�
const I64		KEEPEFFECT_DOT_LIFEMISSING				=	0x0000000000000100;	//��Ѫ
const I64		KEEPEFFECT_DOT_MANAMISSING				=	0x0000000000000200;	//������ʧ
const I64		KEEPEFFECT_DOT_STRENGTHMISSING			=	0x0000000000000400;	//������ʧ
const I64		KEEPEFFECT_DOT_BURNING					=	0x0000000000000800;	//����

const I64		KEEPEFFECT_FRENZY						=	0x0000000000001000;	//��	
const I64		KEEPEFFECT_SHIELD						=	0x0000000000002000;	//����
const I64		KEEPEFFECT_FIRMNESS_MAGIC				=	0x0000000000004000;	//�ᶨʩ��
const I64		KEEPEFFECT_HIDDEN						=	0x0000000000008000;	//����
const I64		KEEPEFFECT_RETORT						=	0x0000000000010000;	//����
const I64		KEEPEFFECT_LURK							=	0x0000000000020000;	//Ǳ��
const I64		KEEPEFFECT_IMMUNE_ALL					=	0x0000000000040000;	//�޵�
const I64		KEEPEFFECT_SLEEP						=	0x0000000000080000;	//ս�����ܣ��������ɴ�ϣ�
const I64		KEEPEFFECT_DREAMING						=	0x0000000000100000;	//ս�����ܣ����������ɴ�ϣ�
const I64		KEEPEFFECT_PALSY						=	0x0000000000200000;	//�������(ֻ�ǲ����ƶ�)
const I64		KEEPEFFECT_SILENCE						=	0x0000000000400000;	//��Ĭ
const I64		KEEPEFFECT_CONFUSION					=	0x0000000000800000;	//����
const I64		KEEPEFFECT_STOPSAY						=	0x0000000001000000;	//����
const I64		KEEPEFFECT_RED							=	0x0000000002000000;	// ����


const I64		KEEPEFFECT_PHYSICALATTACKUP				=	0x0000000004000000;						//������������
const I64		KEEPEFFECT_PHYSICALATTACKDOWN			=	0x0000000008000000;						//���������½�
const I64		KEEPEFFECT_MAGICATTACKUP				=	0x0000000010000000;						//ħ������������
const I64		KEEPEFFECT_MAGICATTACKDOWN				=	0x0000000020000000;						//ħ���������½�
const I64		KEEPEFFECT_ATTACKUP						=	0x0000000040000000;						//����������
const I64		KEEPEFFECT_ATTACKDOWN					=	0x0000000080000000;						//�������½�
const I64		KEEPEFFECT_PHYSICALDEFENCEUP			=	0x0000000100000000;						//�����������
const I64		KEEPEFFECT_PHYSICALDEFENCEDOWN			=	0x0000000200000000;						//��������½�
const I64		KEEPEFFECT_MAGICDEFENCEUP				=	0x0000000400000000;						//ħ����������
const I64		KEEPEFFECT_MAGICDEFENCEDOWN				=	0x0000000800000000;						//ħ�������½�
const I64		KEEPEFFECT_DEFENCEUP					=	0x0000001000000000;						//����������
const I64		KEEPEFFECT_DEFENCEDOWN					=	0x0000002000000000;						//�������½�
const I64		KEEPEFFECT_HITRATEUP					=	0x0000004000000000;						//����������
const I64		KEEPEFFECT_HITRATEDOWN					=	0x0000008000000000;						//�������½�
const I64		KEEPEFFECT_DODGERATEUP					=	0x0000010000000000;						//���������
const I64		KEEPEFFECT_DODGERATEDOWN				=	0x0000020000000000;						//������½�
const I64		KEEPEFFECT_CRITRATEUP					=	0x0000040000000000;						//��������һ������
const I64		KEEPEFFECT_CRITRATEDOWN					=	0x0000080000000000;						//��������һ������
const I64		KEEPEFFECT_MAXLIFEUP					=	0x0000100000000000;						//�������ֵ����
const I64		KEEPEFFECT_MAXLIFEDOWN					=	0x0000200000000000;						//�������ֵ����
const I64		KEEPEFFECT_MAXMANAUP					=	0x0000400000000000;						//���ħ��ֵ����
const I64		KEEPEFFECT_MAXMANADOWN					=	0x0000800000000000;						//���ħ��ֵ����

const I64		KEEPEFFECT_MAXSPUP						=	0x0001000000000000;		//�������ֵ����
const I64		KEEPEFFECT_MAXSPDOWN					=	0x0002000000000000;		//��������ֵ����
const I64		KEEPEFFECT_MOVESPEEDUP					=	0x0004000000000000;		//�ƶ��ٶ�����
const I64		KEEPEFFECT_MOVESPEEDDOWN				=	0x0008000000000000;		//�ƶ��ٶȼ���
const I64		KEEPEFFECT_ATKSPEEDUP					=	0x0010000000000000;		//�����ٶ�����
const I64		KEEPEFFECT_ATKSPEEDDOWN					=	0x0020000000000000;		//�����ٶȼ���
const I64		KEEPEFFECT_PHYSICALDAMAGEUP				=	0x0040000000000000;		//�����˺�����
const I64		KEEPEFFECT_PHYSICALDAMAGEDOWN			=	0x0080000000000000;		//�����˺���С
const I64		KEEPEFFECT_MAGICDAMAGEUP				=	0x0100000000000000;		//ħ���˺�����
const I64		KEEPEFFECT_MAGICDAMAGEDOWN				=	0x0200000000000000;		//ħ���˺���С
const I64		KEEPEFFECT_DAMAGEUP						=	0x0400000000000000;		//�˺�����
const I64		KEEPEFFECT_DAMAGEDOWN					=	0x0800000000000000;		//�˺���С
const I64		KEEPEFFECT_GETEXPUP						=	0x1000000000000000;		//����������
const I64		KEEPEFFECT_GETEXPDOWN					=	0x2000000000000000;		//�����ü���
const I64		KEEPEFFECT_BROWN						=	0x4000000000000000;		//Ǳ�ܱ���  //����
const I64		KEEPEFFECT_BLACK						=	0x8000000000000000;		//��˪��	//����


//////////////////////////////////////////////////////////////////////
// �ۺ�״̬
//enum {
const I64		KEEPEFFECT_NOT_CLEAR		=KEEPEFFECT_TEAMLEADER|KEEPEFFECT_RED|KEEPEFFECT_GETEXPUP;// ������������״̬


const I64		SECONDKEEPEFFECT_CRAZE							=	0x0000000000000001;	//	����
const I64		SECONDKEEPEFFECT_BEATING						=	0x0000000000000002;	//  ���DOT
const I64		SECONDKEEPEFFECT_SUFFERING						=	0x0000000000000004;	// ����dot
const I64		SECONDKEEPEFFECT_DETECTION						=	0x0000000000000008;	//���ܣ�������ʶ��
const I64		SECONDKEEPEFFECT_RESOURCEWAR_PROTECT			=	0x0000000000000010;	//��Դս����״̬
const I64		SECONDKEEPEFFECT_DOT_ADDLIFE					=	0x0000000000000020;	//��ѪDOT
const I64		SECONDKEEPEFFECT_DOT_ADDMANA					=	0x0000000000000040;	//����DOT
const I64		SECONDKEEPEFFECT_ENIMYFIRE						=	0x0000000000000080;	//����-softworms-2007-08-13
const I64       SECONDKEEPEFFECT_LIFEUP                         =   0x0000000000000100;// ��buf��HP������� [1/30/2008 �½���]
const I64       SECONDKEEPEFFECT_COMB_MAIN                      =   0x0000000000000200; //������buff
const I64       SECONDKEEPEFFECT_COMB_SECONDARY                 =   0x0000000000000400; //�����buff
const I64       SECONDKEEPEFFECT_DA_BAO			                =   0x0000000000000800;//[��;���� 2009.03.18]��
const I64       SECONDKEEPEFFECT_ZHONG_BAO		                =   0x0000000000001000;//[��;���� 2009.03.18]�б�
const I64       SECONDKEEPEFFECT_XIAOBAO						=   0x0000000000002000;//[��;���� 2009.03.18]С��
const I64       SECONDKEEPEFFECT_CITYWAR_WEAPON					=   0x0000000000004000; //��ս����
const I64       SECONDKEEPEFFECT_SNEER      					=   0x0000000000008000; //������Ч
const I64       SECONDKEEPEFFECT_LIFESTONE      				=   0x0000000000010000; //Ѫ��ʯ��Ч
const I64       SECONDKEEPEFFECT_CITYATTACk      				=   0x0000000000020000; //��ս������־
const I64       SECONDKEEPEFFECT_CITYDEFENCE      				=   0x0000000000040000; //��ս�ط���־
const I64       SECONDKEEPEFFECT_BASEDOWN      					=   0x0000000000080000; //�������Լ���
const I64       SECONDKEEPEFFECT_PVP     						=   0x0000000000100000; //pvp
const I64       SECONDKEEPEFFECT_YABIAO     					=   0x0000000000200000; //Ѻ��
const I64       SECONDKEEPEFFECT_JIEBIAO     					=   0x0000000000400000; //����
const I64       SECONDKEEPEFFECT_LIANGONG     					=   0x0000000000800000;
const I64       SECONDKEEPEFFECT_ACTIONEND     					=   0x0000000001000000;
const I64       SECONDKEEPEFFECT_EUDEMONEXPUP   				=   0x0000000002000000;

//************************************************************************************************************************************************
//
//״̬ID�ķ���
//ID��5λ
//��1λ��״̬�������	buff	1;
//						debuff	2;
//						dot 	3;
//						����	4.
//��2λ����ֵ����ı�ʶ	�ٷֱ�				1,
//						��ֵ				2,
//						����ϵ				3
//						����				4
//��3,4,5���������壬���......
const int	BIGTYPE_BUFF	= 1;
const int	BIGTYPE_DEBUFF	= 2;
const int	BIGTYPE_DOT		= 3;
const int	BIGTYPE_OTHER	= 4;

const int	DATAIDENT_PERCENT	= 1;
const int	DATAIDENT_NUMBER	= 2;
const int	DATAIDENT_CONTROL	= 3;
const int	DATAIDENT_OTHER		= 4;
//************************************************************************************************************
//״̬��Ź���ID����λ,
//��һλ��BigType��ϸ��buff 1,debuff 2,dot 3,���� 4
//�ڶ�λ����ֵ��ʶ,�ٷֱȣ�1 ��ֵ2 ����ϵ 3 ���� 4
//�������ġ���λ�����
//���������buff�� Cuser::IsBadlyStatus������һλΪ 2��3�� BUFF ����PK��������������ӵ�~~��
//////////////////////////////////////////////////////////////////////
// �ڲ�״̬������Ҫͨ��MsgPlayer֪ͨ������ҵ�״̬���������ڲ�ʹ��(cq_magictypeʹ��)
enum {
		STATUS_NORMAL		=	44001,								//��ͨ״̬	
		STATUS_DIE			=	44002,								//����
		STATUS_DABAO		=	44017,								//[��;���� 2009.03.18]��
		STATUS_ZHONGBAO		=	44018,								//[��;���� 2009.03.18]�б�
		STATUS_XIAOBAO		=	44019,								//[��;���� 2009.03.18]С��
		STATUS_CRIME		=	54003,								//����״̬
//07.4.28���񲨣���STATUS_SKZL ID�ĵ�BUFF����Է����ͻ���
//		STATUS_SKZL			=	44004,								//���ʱ��֮�ᣬ������ħ������100	
		STATUS_TEAMLEADER	=	44005,								//�ӳ�,
		STATUS_PKVALUE		=	44006,								//PKֵ
		STATUS_DETACH_BADLY	=	44007,								//ȥ�����в���״̬						//������SetStatus(),ClsStatus()�г���
		STATUS_DETACH_ALL	=	44008,								//ȥ������״̬��������"����״̬"		//������SetStatus(),ClsStatus()�г���
		STATUS_DETACH_SOMEBAD = 44009,                             // ȥ��һЩ����״̬ [12/26/2007 �½���]
	
//07.4.28���񲨣���STATUS_PK_PROTECT ID�ĵ�BUFF����Է����ͻ���
//		STATUS_PK_PROTECT	=	44009,								//PK����״̬
		STATUS_DISAPPEARING	=	44010,								//ʬ����ʧ״̬
		STATUS_ATTRACTMONSTER	= 44011,								//�������ֻ����Ҳ��У�
		STATUS_PARRY			= 44012,								//�мܣ����ˣ�������������			//������SetStatus(),ClsStatus()�г���
		STATUS_RECOVER			= 44013,								//�ָ�����ȥ���ж�����Ѫ��				//������SetStaus(),ClsStatus()�г���
		STATUS_GLARE			= 44014,								//ѣĿ֮��(���Է��ӷ������٣����м���)	//������SetStaus(),ClsStatus()�г���
		STATUS_AZRAEL_HAND		= 44015,								//����֮��								//������SetStaus(),ClsStatus()�г���
		STATUS_ATTACKMORE		= 44016,								//����֮��								//������SetStaus(),ClsStatus()�г���
		STATUS_FORTITUDE_WILL   = 41016,                             // ������־ [12/25/2007 �½���]
		STATUS_VERIFICATION_WAIT	=44100,								//������֤��
		STATUS_VERIFICATION_RUNNING=44101,								//������
		STATUS_DETACH_STOPMOVE = 41017,								// �� [2/1/2008 �½���]
		STATUS_BIZ_DEAD		   = 41018,								// goto �����������buff
		STATUS_TUTOR_BOSS	   = 35001,								// ʦͽ��������������Ҫ��Buff

 		STATUS_DOT_POISONING		=	34001,							//�ж�DOT			,��hp
 		STATUS_DOT_LIFEMISSING		=	34002,							//��ѪDOT			,��hp
// 		STATUS_DOT_MANAMISSING		=	34003,							//������ʧDOT		,��mp
// 		STATUS_DOT_STRENGTHMISSING	=	34004,							//������ʧDOT		,��sp
 		STATUS_DOT_BURNING			=	34005,							//����DOT			,��hp
 		STATUS_DOT_SUFFERING		=	34006,							//ʹ�����			,��hp
// 		STATUS_DOT_BEATING			=	34007,							//�����			 ��hp
 		STATUS_DOT_ADDLIFE			=	34008,							//��ѪDOT
 		STATUS_DOT_ADDMANA			=	34009,							//����DOT
// 		STATUS_DOT_ADDLIFE_RB		=	34010,							//20070827����:��ѪDOT.����֮��regeneration blow ��RB��
// 		STATUS_DOT_ADDMANA_MC		=	34011,							//20070827����:����DOT.������mind concentrade ��MC��
// 		STATUS_DOT_ENMITY           =   34012,                          //ǿ�Ƴ��DOT
		STATUS_DOT_LIFEMISS1		=	34013,							//��ѪDOT����hp,Ϊ�˿��Ե��Ӷ��¼ӵ�״̬
// 		
// 		STATUS_DOT_BEATINGPRE       =   31012,                          //�½��� �������2���԰ٷֱȵ�Ѫ
// 		STATUS_DOT_SUFFERING_PRE    =   31013,                          // ʹ�����2���ٷֱȵ�Ѫ [12/13/2007 �½���]
// 		STATUS_DOT_POISON_ATK       =   31014,                          // ��Ϯ,����������ö�ϮЧ�� [12/13/2007 �½���]
// 		STATUS_DOT_QUICK_ADDLIFE    =   32015,                         // ���ٻָ� [12/25/2007 �½���]
// 		STATUS_DOT_ADDLIFE_L		=	34016,							//��ѪDOT
// 		STATUS_DOT_ADDMANA_L		=	34017,							//����DOT
		STATUS_DOT_REDMANA			=	34018,					//����DOT
		STATUS_DOT_REDSP			=	34019,					//��ŭ��DOT
		STATUS_DOT_WARGAMEEXP		=	34995,					//�������Ӿ���buff
		STATUS_DOT_ADDEXP			=	34996,					//�ר�üӾ���buff( m_nParam2 �������Ǹ���ͼ�о��� ,0 ��ȫ��ͼ)
		STATUS_DOT_TUTOREXP			=	34997,					//ʦͽ�Ȳ�buf
		STATUS_ACTIVATE_ADDEXP		=	34998,					//�漣ʢ��buff��ÿ���ӻ��һ������
		STATUS_BOOTH_ADDEXP			=	34999,					//��̯ʱ�Զ���þ���BUFF
		STATUS_ITEM_ADDEXP			=	35000,					//��Ʒ���ļӾ���buff
		STATUS_DOT_SYNEXP			=	35002,					//���ɼӾ���buff
//		STATUS_SYNWORKTIME	        =	36000,					//���ɴ�ʱ��buff

		STATUS_FRENZY				=	11019,					//��	
		STATUS_ERUPTPOT_PERCENT		=	11020,					//Ǳ�ܱ���
		STATUS_SHIELD				=	12001,					//һ��ʱ���ڿɵ���N������	����
		STATUS_FIRMNESS_MAGIC		=	12002,					//һ��ʱ��������ħ��������ϵĻ���	�ᶨʩ��
		STATUS_HIDDEN				=	14001,					//����	
		STATUS_RETORT				=	14002,					//����	
		STATUS_LURK					=	14003,					//NPC�������Լ����������Ч	Ǳ��	
		STATUS_IMMUNE_ALL			=	14004,					//����һ���˺�	�޵�
		STATUS_CRAZE				=	14005,					//���ȣ���MP���������˺�
		STATUS_DETECTION			=	14006,					//ʶ�ƣ����ܣ�
		STATUS_RESOURCEWAR_PROTECT	=	14007,					//��Դս����״̬���൱���޵�,�����ü���ʹ��
		STATUS_PVPMODE				=	14008,					//����buff
		STATUS_YABIAO				=	14009,					//Ѻ��buff
		STATUS_JIEBIAO				=	14013,					//����buff
		STATUS_LIANGONG				=	14014,
		STATUS_ACTIONEND			=	14015,                  //ͬ���ڽ���ʱ�ܴ���action
		STATUS_SLEEP				=	23001,					//�����ƶ������������ܡ�ʹ����Ʒ	ս�����ܣ��������ɽ����	
		STATUS_DREAMING				=	23002,					//�����ƶ������������ܡ�ʹ����Ʒ	ս�����ܣ��������������	
		STATUS_PALSY				=	23003,					//�����ƶ�	�������	
		STATUS_SILENCE				=	23004,					//����ʹ����Ҫ�����ļ���	��Ĭ	
		STATUS_CONFUSION			=	23005,					//�����ת����ʧĿ��	����	
		STATUS_STOPSAY				=	24001,					//����˵�����ǿ���ϵ	����	
		STATUS_FROSTSHIELD			=	12009,					//��˪�ף��������������������ʱ�����Ͷ����ƶ��ٶ�

		STATUS_SKZL					=	22011,								//���ʱ��֮�ᣬ����	
		STATUS_PK_PROTECT			=	14010,								//PK����״̬
		STATUS_STRUGGLE_RELIVE		=	14011,								//20070827����:���غ���struggle relive ��SR��
		STATUS_NEW_PK_PROTECT		=	14012,					//  [2/22/2008 ���]	��PK����,����Ҳ�����
		STATUS_SHIELD_1				=	12039,					//һ��ʱ���������˺�	����
		STATUS_SHIELD_2				=	12045,					//һ��ʱ���������˺�	����

		//�ٷֱ�ϵ��------begin----------------------*******************************************************
		STATUS_PHYSICALATTACKUP_PERCENT		=	11001,				//������������
		STATUS_PHYSICALATTACKDOWN_PERCENT	=	21001,				//���������½�
		STATUS_MAGICATTACKUP_PERCENT		=	11002,				//ħ������������
		STATUS_MAGICATTACKDOWN_PERCENT		=	21002,				//ħ���������½�
		STATUS_ATTACKUP_PERCENT				=	11003,				//����������
		STATUS_ATTACKDOWN_PERCENT			=	21003,				//�������½�
		STATUS_PHYSICALDEFENCEUP_PERCENT	=	11004,				//�����������
		STATUS_PHYSICALDEFENCEDOWN_PERCENT	=	21004,				//��������½�
		STATUS_MAGICDEFENCEUP_PERCENT		=	11005,				//ħ����������
		STATUS_MAGICDEFENCEDOWN_PERCENT		=	21005,				//ħ�������½�
		STATUS_DEFENCEUP_PERCENT			=	11006,				//����������
		STATUS_DEFENCEDOWN_PERCENT			=	21006,				//�������½�
		STATUS_HITRATEUP_PERCENT			=	11007,				//����������
		STATUS_HITRATEDOWN_PERCENT			=	21007,				//�������½�
		STATUS_DODGERATEUP_PERCENT			=	11008,				//���������
		STATUS_DODGERATEDOWN_PERCENT		=	21008,				//������½�	
		STATUS_CRITRATEUP_PERCENT			=	11009,				//����һ������		
		STATUS_CRITRATEDOWN_PERCENT			=	21009,				//����һ������
		STATUS_MAXLIFEUP_PERCENT			=	11010,				//�������ֵ����
		STATUS_MAXLIFEDOWN_PERCENT			=	21010,				//�������ֵ����
		STATUS_MAXMANAUP_PERCENT			=	11011,				//���ħ��ֵ����
		STATUS_MAXMANADOWN_PERCENT			=	21011,				//���ħ��ֵ����
		STATUS_MAXSPUP_PERCENT				=	11012,				//�������ֵ����
		STATUS_MAXSPDOWN_PERCENT			=	21012,				//��������ֵ����
		STATUS_MOVESPEEDUP_PERCENT			=	11013,				//�ƶ��ٶ�����
		STATUS_MOVESPEEDDOWN_PERCENT		=	21013,				//�ƶ��ٶȼ���
		STATUS_ATKSPEEDUP_PERCENT			=	11014,				//�����ٶ�����
		STATUS_ATKSPEEDDOWN_PERCENT			=	21014,				//�����ٶȼ���
		STATUS_PHYSICALDAMAGEUP_PERCENT		=	21015,				//�����˺�����
		STATUS_PHYSICALDAMAGEDOWN_PERCENT	=	11015,				//�����˺���С
		STATUS_MAGICDAMAGEUP_PERCENT		=	21016,				//ħ���˺�����
		STATUS_MAGICDAMAGEDOWN_PERCENT		=	11016,				//ħ���˺���С
		STATUS_DAMAGEUP_PERCENT				=	21017,				//�˺�����
		STATUS_DAMAGEDOWN_PERCENT			=	11017,				//�˺���С
		STATUS_GETEXPUP_PERCENT				=	11018,				//����������
		STATUS_GETEXPDOWN_PERCENT			=	21018,				//�����ü���
		STATUS_BASE_DOWN_PERSENT            =   21019,
		STATUS_PHYSICALATTACKUP_PERCENT_WS	=	11021,				//20070827����:������������.ս��֮��war song ��WS��
		STATUS_MAGICATTACKUP_PERCENT_ED		=	11022,				//20070827����:ħ������������.����רעenergy Determined ��ED��
		STATUS_DAMAGEDOWN_PERCENT_TN		=	11023,				//20070827����:�˺���С.��ǿ�ƶ�tough nail ��TN��
		STATUS_GETEXPUP_PERCENT_WT			=	11024,				//20070827����:����������.�츳����well talent ��WT��
		STATUS_PHYATTACKUP_TUTOR_PERCENT	=	11025,				//ʦͽ��Ӽӹ���
		STATUS_MAGATTACKUP_TUTOR_PERCENT    =   11026,              // ʦͽ��Ӽ�ħ������ [�½���10/27/2007]
		STATUS_SELF_DAMAGEUP				=	11027,				//����Ե��˺�����
		STATUS_SELF_DAMAGEUP_PERCENT		=	11028,				//����Ե��˺�����,������ɵ���

		STATUS_GETEXPUP_PERCENT_SYNINS		=	11090,				//���ɸ�����ô��˫��

		//����ϵ��--------begin------------------------****************************************************
		STATUS_MAGICDEFENCEUP_NUMBER		=	12003,				//ħ���������ӣ�������
		STATUS_MAGICDEFENCEUP_NUMBER_1		=	12040,				//ħ���������ӣ�������//Ϊ�˿��ܵ��ӣ��ඨ��һ��
		STATUS_MAGICDEFENCEDOWN_NUMBER		=	22003,				//ħ���������٣�������
		STATUS_PHYSICALDEFENCEUP_NUMBER		=	12004,				//����������ӣ�������
		STATUS_PHYSICALDEFENCEDOWN_NUMBER	=	22004,				//����������٣�������
		STATUS_PHYSICALATTUP_NUMBER	    	=	12019,				//���������ӣ������� //�Ŷ��޸�
		STATUS_PHYSICALATTDOWN_NUMBER   	=	22019,				//���������٣������� //�Ŷ��޸�
		STATUS_DEFENCEUP_NUMBER				=	12005,				//��������(����)
		STATUS_DEFENCEDOWN_NUMBER			=	22005,				//��������(����)
		STATUS_CRITRATEUP_NUMBER			=	12006,				//��������һ�����ʣ�������
		STATUS_CRITRATEDOWN_NUMBER			=	22006,				//��������һ�����ʣ�������
		STATUS_HITRATEUP_NUMBER				=	12007,				//����������(����)
		STATUS_HITRATEDOWN_NUMBER			=	22007,				//�����ʼ���
		STATUS_DODGERATEUP_NUMBER			=	12008,				//����������
		STATUS_DODGERATEDOWN_NUMBER			=	22008,				//�����ʼ���
		STATUS_PHYSICALDEFENCEUP_NUMBER_SF	=	12010,				//20070827����:����������ӣ�����������֮��shield of faith ��SF��
		STATUS_MAGICDEFENCEUP_NUMBER_EM		=	12011,				//20070827����:ħ���������ӣ���������������expose the myth ��EM��
		STATUS_CRITRATEUP_NUMBER_SW			=	12012,				//20070827����:��������һ�����ʣ�������Ѱ������seek weakpoints ��SW��

		STATUS_LIFE_UP                      =   12013,              // ����ֵ���� [1/30/2008 �½���]

		STATUS_BIGBLOOD_USERNOT             =   24002,               // ����ʹ����ֵҩˮ [1/15/2008 �½���]

		STATUS_MOUNTSPEED_NUMBER            =   30000,              // ����BUF [�½���8/2/2007]     

		STATUS_ENEMYFIRE					=	22009,				//����,�����ͼ�ɱ�����.
		STATUS_INVALID						=	99999,				//��Ч״̬,������AttachStatus

		STATUS_WARGAME						=	30001,				//  [2007-8-21] ��� ��Ҳμ���Ϸ���BUF

        STATUS_COMB_MAIN                    =   12015,
		STATUS_COMB_SECONDARY               =   12016,

		STATUS_FISH_FOOD                    =   12017,             //���
		STATUS_FISH_WORKER                  =   12018,             //�㹤
		STATUS_FISH_TUTOR                   =   12031,             //�����ʦ
		STATUS_CITYWAR_WEAPON               =   12032,             //��ս����
		STATUS_CITYWAR_SAFETIME             =   12033,             //��ս��ȫʱ��
		STATUS_DEAD_ADDTEAMATTR             =   12034,             //��������Ѽ�Ѫ
		STATUS_CITYWAR_ATTACKMARK           =   12035,             //������־
		STATUS_CITYWAR_DEFENCEMARK          =   12036,             //�ط���־

		STATUS_LOCK_TARGET					=	22010,				//  [12/19/2007 ���]	ֻ�ܹ���ĳһ��Ŀ��,��ϼ��� ��ս 6405
//		STATUS_BUFF_HPMP                    =   12014,              //ͬʱ��Ѫ�ͼ���
		STATUS_MAGICDEFENCEUP_NUMBER_L		=	12020,				//ħ���������ӣ�������
		STATUS_PHYSICALDEFENCEUP_NUMBER_L	=	12021,				//�����������(����)
		STATUS_STRENGUP_NUMBER				=	12022,				//��������
		STATUS_STRENGDOWN_NUMBER			=	22022,				//��������
		STATUS_INTUP_NUMBER					=	12023,				//��������
		STATUS_INTDOWN_NUMBER				=	22024,				//��������
		STATUS_MAGIC_SHIELD					=	12024,				//ħ���ܣ��ܵ��˺�ʱ�������������˿�Ѫ
		STATUS_SPENDMP_UP					=	21023,				//����������

		STATUS_STAUP_NUMBER					=	12025,				//��������
		STATUS_STADOWN_NUMBER				=	22025,				//��������
		STATUS_SPIUP_NUMBER					=	12026,				//���Ӿ���
		STATUS_SPIDOWN_NUMBER				=	22026,				//���پ���
		STATUS_AGIUP_NUMBER					=	12027,				//��������
		STATUS_AGIDOWN_NUMBER				=	22027,				//��������
		STATUS_TOUCHUP_NUMBER				=	12028,				//���Ӽ���
		STATUS_TOUGHDOWN_NUMBER				=	22028,				//���ټ���
		STATUS_SELF_DAMAGEUP_NUMBER			=	12030,				//����Ե��˺�����
		STATUS_FOURATTUP_NUMBER				=	12037,				//����������
		STATUS_FOURATTDOWN_NUMBER			=	22037,				//�����Լ���
		STATUS_BASEATTUP_NUMBER				=	12038,				//����������
		STATUS_MONSTER_ENMITY				=	22038,				//����ǿ�ƹ��﹥��ʩ���ߣ���Ҫ��NPC����ʱ����,�ó��ֵ����
		STATUS_DEFDOWN_ATKUP				=	12041,				//�Է�����������½����⹥����
		STATUS_FIREDEF_UP					=	12046,				//���
		STATUS_ICEDEF_UP					=	12047,				//����
		STATUS_MYSTICDEF_UP					=	12048,				//����
		STATUS_NOXDEF_UP					=	12049,				//����
		//goto ����װ��������Ч��
		STATUS_EQUIP_DEFUP					=	12042,				//���������	������װ��
		STATUS_EQUIP_DAMAGEUP				=	12043,				//�Ե��˺�����  ������װ��
		STATUS_EQUIP_CRIUP					=	12044,				//��������		������װ��
	
		STATUS_EQUIP_BEDAMAGEUP				=	22039,				//�ܵ��˺�����	������װ��
		STATUS_EQUIP_DEFDOWN				=	22040,				//���������	������װ��
		
		//��긽��Ч��12051--12059

		STATUS_SOUL_CRI						=	12051,				//����			���
		STATUS_SOUL_HITRATE					=	12052,				//����			���
		STATUS_SOUL_DODGE					=	12053,				//����			���
		STATUS_SOUL_OUTERATT				=	12054,				//�⹦			���
		STATUS_SOUL_INNERATT				=	12055,				//�ڹ�			���
		STATUS_SOUL_OUTERDEF				=	12056,				//���			���
		STATUS_SOUL_INNERDEF				=	12057,				//�ڷ�			���
		STATUS_SOUL_REBLOOD					=	12058,				//��Ѫ			���
		STATUS_SOUL_REMP					=	12059,				//��ħ			���


		STATUS_SOUL_REBLOOD_PER				=	12101,				//��Ѫ�ٷֱ�
//modify code by �ֵ�Ԩ begin 2011.6.22
		STATUS_RETORT_TENSEC				=	21024,				//10����תһ��
		STATUS_RETORT_LGWS					=	21025,				//�ҹ�����		A-BOSS
//		STATUS_RETORT_BACK2					=	21026,				//������		B-BOSSս����ʼ��BOSSÿ10���Ӷ���BOSS���������3*3��Χ��14*14��Χ������������һ��Ⱥ��AOE�˺����ڷ�Χ�ڵ���Ҳ��ܴ˼���Ӱ��
//		STATUS_RETORT_BACK4					=	21027,				//׷�긫		B-BOSSս����ʼ��BOSSÿ40���������һ������ͷ�һ��BUFF��BUFF����5���Ժ����һ��3*3��ķ�Χ�˺������˺��ϸ�
		STATUS_RETORT_ZHF					=	21028,				//׷�긫		B-BOSS5���Ժ����һ��3*3��ķ�Χ�˺������˺��ϸ�
		STATUS_RETORT_MDSX					=	21029,				//�Զ�����		C-BOSSǰ�ȴ�20��
		STATUS_RETORT_MDSX_BUFF2			=	21030,				//�Զ�����		C-BOSS BUFF2
		STATUS_RETORT_MDSX_1				=	21031,				//�Զ�����		C-BOSS ��ʱ��ѪBUFF
//modify code by �ֵ�Ԩ end
		//����
		STATUS_SYN_HITRATEDOWN				=   22041,				//���н���
		STATUS_SYN_SHIELD					=	12050,				//һ��ʱ���������˺�	����

		//���︽��
		STATUS_EUDEMON_SOUL                 =   12060,              //���︽�壬ȫ���Լ�10

		//��������˼ӵ�BUFF����ΪҪ���˼ӵ��ص����������ⶨ��
		STATUS_STRENGUP_NUMBER_PET			=	12722,				//��������
		STATUS_STRENGDOWN_NUMBER_PET		=	22722,				//��������
		STATUS_SPIUP_NUMBER_PET				=	12726,				//���Ӿ���
		STATUS_SPIDOWN_NUMBER_PET			=	22726,				//���پ���
		STATUS_AGIUP_NUMBER_PET				=	12727,				//��������
		STATUS_AGIDOWN_NUMBER_PET			=	22727,				//��������
		STATUS_STAUP_NUMBER_PET				=	12725,				//��������
		STATUS_STADOWN_NUMBER_PET			=	22725,				//��������
		STATUS_INTUP_NUMBER_PET				=	12723,				//��������
		STATUS_INTDOWN_NUMBER_PET			=	22724,				//��������

		STATUS_PET_DODGE					=	12724,//���︽�������
		STATUS_PET_HITRATE					=	12728,//���︽�������
};

//////////////////////////////////////////////////////////////////////
struct StatusInfoStruct;
class IStatus
{
public:
	virtual OBJID	GetID()							PURE_VIRTUAL_FUNCTION_0
	virtual ULONG	ReleaseByOwner()				PURE_VIRTUAL_FUNCTION_0

	virtual bool		IsValid()							PURE_VIRTUAL_FUNCTION_0
	virtual void		SetPower(int data)					PURE_VIRTUAL_FUNCTION
	virtual int			GetPower()							PURE_VIRTUAL_FUNCTION_0
	virtual int			GetParam1		()					PURE_VIRTUAL_FUNCTION_0
	virtual int			GetParam2		()					PURE_VIRTUAL_FUNCTION_0
	virtual bool		GetInfo(StatusInfoStruct* pInfo)	PURE_VIRTUAL_FUNCTION_0
	virtual bool		OnTimer(DWORD tCurr)				PURE_VIRTUAL_FUNCTION_0
	virtual bool		ChangeData(int nPower,int nSecs,int nTimes=0,int nParam1=0,int nParam2=0,int nParam3=0,OBJID idOwner=0)		PURE_VIRTUAL_FUNCTION_0
	virtual bool		IncTime(int nMilliSecs, int nLimit)		PURE_VIRTUAL_FUNCTION_0
	virtual bool		ToFlash()							PURE_VIRTUAL_FUNCTION_0
	virtual int			GetParam3()							PURE_VIRTUAL_FUNCTION_0	//06.11.4�������	,06.11.13�����޸�				
	virtual int			GetSec()							PURE_VIRTUAL_FUNCTION_0	//  [3/31/2008 ���]	�ó���ʱ��
	virtual int			GetSecLeft()						PURE_VIRTUAL_FUNCTION_0
	virtual void		SetFreeze()							PURE_VIRTUAL_FUNCTION
	virtual void		SetUnFreeze()						PURE_VIRTUAL_FUNCTION
};

typedef	IGameObjSet<IStatus>		IStatusSet;
typedef	CGameObjSet<IStatus>		CStatusSet;

////////////////////////////////////////////////////////////////
enum {	MOVEMODE_WALK=0,					// PathMove()��ģʽ
		MOVEMODE_RUN,
		MOVEMODE_SHIFT,						// to server only
		MOVEMODE_JUMP,
		MOVEMODE_TRANS,
		MOVEMODE_CHGMAP,
		MOVEMODE_JUMPMAGICATTCK,
		MOVEMODE_COLLIDE,
		MOVEMODE_SYNCHRO,					// to server only
		MOVEMODE_TRACK,

		MOVEMODE_RUN_DIR0 = 20,

		MOVEMODE_RUN_DIR7 = 30,
};

////////////////////////////////////////////////////////////////
const int _DELTA_X[11]={ 0, 0, 1, 1, -1, 0, 1, -1, -1 , 0 , 0};
const int _DELTA_Y[11]={ 0, -1, -1, 0, -1, 0, 1, 0, 1 , 1 , 0};
const int MAX_DIRSIZE = 11;
inline int GetDirByPos(int nFromX, int nFromY, int nToX, int nToY)		// return MAX_DIRSIZE: error
{
	if(nFromX < nToX)
	{
		if(nFromY < nToY)
			return 6;
		else if(nFromY > nToY)
			return 2;
		else
			return 3;
	}
	else if(nFromX > nToX)
	{
		if(nFromY < nToY)
			return 8;
		else if(nFromY > nToY)
			return 4;
		else
			return 7;
	}
	else // if(nFromX == nToX)
	{
		if(nFromY < nToY)
			return 9;
		else if(nFromY > nToY)
			return 1;
	}
	return MAX_DIRSIZE;
}
inline int		MaskLook(int look)							{ return look/10; }
inline int		MaskDir(int look)							{ return look%10; }
inline DWORD	MakeLookFace(int nMaskLook, int nMaskDir)	{ return (nMaskLook*10) + (nMaskDir%10); }
inline float	GetRadian(float posSourX, float posSourY, float posTargetX, float posTargetY)
{
	CHECKF(posSourX != posTargetX || posSourY != posTargetY);

	float PI = static_cast<float>(3.1415926535);
	float fDeltaX = posTargetX - posSourX;
	float fDeltaY = posTargetY - posSourY;
	float fDistance	= sqrt(fDeltaX*fDeltaX + fDeltaY*fDeltaY);
	CHECKF(fDeltaX <= fDistance && fDistance > 0);
	float fRadian = asin(fDeltaX / fDistance);

	return fDeltaY > 0 ? (PI/2 - fRadian) : (PI + fRadian + PI/2);
}

//////////////////////////////////////////////////////////////////////
class IRole;
class IMapThing
{
public: // base
	virtual void	ReleaseByOwner()							PURE_VIRTUAL_FUNCTION
	virtual OBJID	GetID()										PURE_VIRTUAL_FUNCTION_0
	virtual int 	GetObjType()								PURE_VIRTUAL_FUNCTION_0
	virtual int		GetPosX()									PURE_VIRTUAL_FUNCTION_0
	virtual int		GetPosY()									PURE_VIRTUAL_FUNCTION_0
	virtual void	SendShow(IRole* pRole)						PURE_VIRTUAL_FUNCTION

	// return true: ppv���ض���ָ��
	virtual bool	QueryObj(OBJID idObjType, void** ppv)				PURE_VIRTUAL_FUNCTION_0
	virtual bool	QueryRole(void** ppv)						PURE_VIRTUAL_FUNCTION_0
};

//20080111����:����npc��־------------
enum EM_SENDNPCFLAG
{
	EM_SENDNPCFLAG_FALSE	= 0,//�����͸�npc
		EM_SENDNPCFLAG_TRUE,//��������.�����������npc����
		EM_SENDNPCFLAG_ENFORCE,//ǿ��.���۱�������û��npc
};
//------------------------------------

/////////////////////////////////////////////////////////////////////
class IMapThing;
class IStatus;
class CTransformation;
class CTeam;
class CItem;
class CMagic;
class IRole
{
protected:
	IRole(){}
	~IRole(){}
public:
	virtual void	ReleaseByOwner		()						PURE_VIRTUAL_FUNCTION
public: // get attrib
	virtual OBJID	GetID				()						PURE_VIRTUAL_FUNCTION_0
	virtual LPCTSTR	GetName				()						PURE_VIRTUAL_FUNCTION_0
//	virtual LPCTSTR	GetMate				()						PURE_VIRTUAL_FUNCTION_0

	virtual DWORD	GetLookFace			()						PURE_VIRTUAL_FUNCTION_0
	//�Ժ�Ҫ�ֽ�����������ӿ�
	//virtual int	GetLook				()						PURE_VIRTUAL_FUNCTION_0
	//virtual int	GetFace				()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetHair				()						PURE_VIRTUAL_FUNCTION_0
	virtual int 	GetPosX				()						PURE_VIRTUAL_FUNCTION_0
	virtual int 	GetPosY				()						PURE_VIRTUAL_FUNCTION_0
	virtual int 	GetDir				()						PURE_VIRTUAL_FUNCTION_0
//	virtual	BOOL	CheckStatus			(int nStatus)			PURE_VIRTUAL_FUNCTION_0
	virtual I64		GetEffect			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetLev				()						PURE_VIRTUAL_FUNCTION_0

	virtual DWORD	GetLife				()						PURE_VIRTUAL_FUNCTION_0
//	virtual DWORD	GetMana				()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetMaxLife			()						PURE_VIRTUAL_FUNCTION_0
//	virtual DWORD	GetMaxMana			()						PURE_VIRTUAL_FUNCTION_0

	virtual DWORD	GetAtk				()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetMgcAtk			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetAttack			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetAtkHitRate		()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetAddAttByType     (int nType)				PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetAtkType			()						PURE_VIRTUAL_FUNCTION_0

	virtual DWORD	GetDef				()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetDefence			()						PURE_VIRTUAL_FUNCTION_0

	virtual DWORD	GetDex				()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetDexterity		()						PURE_VIRTUAL_FUNCTION_0

	virtual DWORD	GetDdg				()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetDodge			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetCrit				()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetTough			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD 	AdjustDamage(DWORD nDamage,int nAttribType) PURE_VIRTUAL_FUNCTION_0

//	virtual DWORD	GetMagicAtk			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetMagicDef			()						PURE_VIRTUAL_FUNCTION_0

	virtual	DWORD	GetInterAtkRate		()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetIntervalAtkRate	()						PURE_VIRTUAL_FUNCTION_0

	virtual int		GetHelmetTypeID		()						PURE_VIRTUAL_FUNCTION_0
	virtual int		GetArmorTypeID		()						PURE_VIRTUAL_FUNCTION_0
	virtual int		GetWeaponRTypeID	()						PURE_VIRTUAL_FUNCTION_0
	virtual int 	GetWeaponLTypeID	()						PURE_VIRTUAL_FUNCTION_0
	virtual int     GetDressTypeID		()						PURE_VIRTUAL_FUNCTION_0
	virtual int     GetWingTypeID		()						PURE_VIRTUAL_FUNCTION_0//���
//	virtual int		GetNecklaceTypeID	()						PURE_VIRTUAL_FUNCTION_0
//	virtual int 	GetRingRTypeID		()						PURE_VIRTUAL_FUNCTION_0
//	virtual int 	GetRingLTypeID		()						PURE_VIRTUAL_FUNCTION_0
//	virtual int 	GetShoesTypeID		()						PURE_VIRTUAL_FUNCTION_0
	virtual int 	GetMountTypeID		()						PURE_VIRTUAL_FUNCTION_0
	virtual int		GetMantleTypeID		()						PURE_VIRTUAL_FUNCTION_0
	virtual int		GetWeaponSpecialView()						PURE_VIRTUAL_FUNCTION_0

//	virtual DWORD 	GetMedalSelect		()						PURE_VIRTUAL_FUNCTION_0
//	virtual DWORD	GetTitleSelect		()						PURE_VIRTUAL_FUNCTION_0

	virtual int		GetDistance			(int x, int y)			PURE_VIRTUAL_FUNCTION_0
	virtual int		GetDistance			(IMapThing* pTarget)	PURE_VIRTUAL_FUNCTION_0

	virtual CGameMap*	GetMap			()						PURE_VIRTUAL_FUNCTION_0

	virtual DWORD	GetSynID			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetSynRank			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetSynRankShow		()						PURE_VIRTUAL_FUNCTION_0

	virtual bool	IsSimpleMagicAtk	()						PURE_VIRTUAL_FUNCTION_0
	virtual int 	GetSimpleMagicHitRate()						PURE_VIRTUAL_FUNCTION_0

	virtual int		GetTutorExp			()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetTutorLevel		()						PURE_VIRTUAL_FUNCTION_0
	virtual UCHAR	GetMercenaryRank	()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetMercenaryExp		()						PURE_VIRTUAL_FUNCTION_0
	virtual UCHAR	GetNobilityRank		()						PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetExploit			()						PURE_VIRTUAL_FUNCTION_0
public: // const

//	virtual int		GetSex				()						PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsAlive				()						PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsTalkEnable		()						PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsGM				()						PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsPM				()						PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsMonster			()						PURE_VIRTUAL_FUNCTION_0
	//20070316����
	virtual BOOL	IsUser				()						PURE_VIRTUAL_FUNCTION_0
//	virtual bool	IsScene				()						PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsDelThis			()						PURE_VIRTUAL_FUNCTION_1
	virtual bool	IsVirtuous()								PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsEvil()									PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsBowman()									PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsShieldEquip()								PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsWing()									PURE_VIRTUAL_FUNCTION_0
	// return true: ppv���ض���ָ��
	virtual bool	QueryObj			(OBJID idObjType, void** ppv)	PURE_VIRTUAL_FUNCTION_0
	virtual IMapThing*	QueryMapThing	()						PURE_VIRTUAL_FUNCTION_0
	virtual IRole*	FindAroundRole		(OBJID id)				PURE_VIRTUAL_FUNCTION_0
	virtual CUser*	QueryOwnerUser		()						PURE_VIRTUAL_FUNCTION_0

public:	// modify attrib ------------------------------
	virtual void	SetDir				(int nDir)							PURE_VIRTUAL_FUNCTION
	virtual	void	SetStatus			(int nStatus, bool bSynchro=true,int nPower=0)						PURE_VIRTUAL_FUNCTION
	virtual	void	ClsStatus			(int nStatus, bool bSynchro=true)						PURE_VIRTUAL_FUNCTION

	virtual	void	AddBuff( int nStatus,int nTime,int nTimes,int nPower,int nParam1,int nParam2,int nParam3 )		PURE_VIRTUAL_FUNCTION//07.4.26�������
	virtual void	UpdateBuff( int nStatus,int nTime,int nTimes,int nPower,int nParam1,int nParam2,int nParam3 )	PURE_VIRTUAL_FUNCTION//07.4.26�������

	virtual void    ClsBuff				(int nStatus)						PURE_VIRTUAL_FUNCTION
	virtual bool	AddAttrib			(int idxAttr, __int64 i64Data, int bSynchro)					PURE_VIRTUAL_FUNCTION_0
	virtual void	ProcessOnMove		(int nMoveMode)						PURE_VIRTUAL_FUNCTION
	virtual void	ProcessAfterMove	()									PURE_VIRTUAL_FUNCTION
	virtual void	GetFootPrint		(int& x, int& y)					PURE_VIRTUAL_FUNCTION

	virtual bool	SpendArrow			()									PURE_VIRTUAL_FUNCTION_0

	virtual	IStatus*	QueryStatus(OBJID idType)										PURE_VIRTUAL_FUNCTION_0
	virtual	IStatusSet*	QueryStatusSet()												PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	AwardBattleExp		(int nExp, bool bGemEffect=true, bool bIncludeOwner=true)		PURE_VIRTUAL_FUNCTION_0
	virtual void	SetFightPause		(int nInterval)									PURE_VIRTUAL_FUNCTION
	virtual void	SaveInfo			()												PURE_VIRTUAL_FUNCTION

public: // money --------------------------------
//	virtual bool	SpendMoney			(DWORD nMoney, bool bSynchro)			PURE_VIRTUAL_FUNCTION_0
//	virtual bool	GainMoney			(DWORD nMoney, bool bSynchro)			PURE_VIRTUAL_FUNCTION_0

public: // interactive
//	virtual bool	AddItem				(CItem* pItem)								PURE_VIRTUAL_FUNCTION_0
//	virtual bool	IsItemFull			()											PURE_VIRTUAL_FUNCTION_0

public: // system
	//20080111����:
	//-------------------
	//virtual void	BroadcastRoomMsg(CNetMsg* pMsg, bool bSendSelf = false)	PURE_VIRTUAL_FUNCTION
	virtual void	BroadcastRoomMsg(CNetMsg* pMsg, 
		bool bSendSelf = false,
		EM_SENDNPCFLAG sendNpc = EM_SENDNPCFLAG_TRUE)	PURE_VIRTUAL_FUNCTION
	//-------------------
	virtual void	BroadcastRoomMsg(LPCTSTR szMsg, bool bSendSelf = false)	PURE_VIRTUAL_FUNCTION
//	virtual void	BroadcastMapMsg(CNetMsg* pMsg, bool bSendSelf = false)	PURE_VIRTUAL_FUNCTION
//	virtual void	BroadcastMapMsg(LPCTSTR szMsg, bool bSendSelf = false)	PURE_VIRTUAL_FUNCTION
	virtual bool	SendMsg(CNetMsg* pMsg)									PURE_VIRTUAL_FUNCTION_0
	virtual bool	SendSysMsg(const char* fmt, ...)						PURE_VIRTUAL_FUNCTION_0
	virtual bool	SendSysMsg(int nChannel, const char* fmt, ...)			PURE_VIRTUAL_FUNCTION_0
	virtual void	OnTimer(time_t tCurr)									PURE_VIRTUAL_FUNCTION

public: // map
	virtual int		MoveToward(int nDir, bool bSync = true)					PURE_VIRTUAL_FUNCTION_0
	virtual void	TransPos(int nPosX, int nPosY)							PURE_VIRTUAL_FUNCTION
	virtual void	JumpPos(int nPosX, int nPosY)							PURE_VIRTUAL_FUNCTION
	virtual bool	SyncTrackTo(int nPosX, int nPosY, int nDir, DWORD dwAction)			PURE_VIRTUAL_FUNCTION_0
	virtual void	ActiveTransPos(int nPosX, int nPosY)					PURE_VIRTUAL_FUNCTION
	virtual bool	SendSelfToBlock()										PURE_VIRTUAL_FUNCTION_0

	virtual void	AddToBCRoleSet			(OBJID idRole, bool bSendMsg)	PURE_VIRTUAL_FUNCTION
	virtual void	RemoveFromBCRoleSet		(OBJID idRole,bool bClrBigBc = false)					PURE_VIRTUAL_FUNCTION
	virtual void	AddToBCMapItemSet		(OBJID idMapItem)				PURE_VIRTUAL_FUNCTION
	virtual void	RemoveFromBCMapItemSet	(OBJID idMapItem)				PURE_VIRTUAL_FUNCTION
	
public: // fight ------------------------------
	virtual void	ClrAttackTarget		()									PURE_VIRTUAL_FUNCTION
	virtual bool	SetAttackTarget(IRole* pTarget = NULL)					PURE_VIRTUAL_FUNCTION_0
	virtual bool	AutoSkillAttack(IRole* pTarget)							PURE_VIRTUAL_FUNCTION_0
	virtual int		Attack( IRole* pTarget,DWORD& dwBattleAddInfoRef )		PURE_VIRTUAL_FUNCTION_0
	virtual bool	BeAttack(bool bMagic, IRole* pTarget, int nPower,int addi = 0, bool bReflectEnable=true)					PURE_VIRTUAL_FUNCTION_0
	virtual bool	BeEnmityAttack(bool bMagic, IRole* pTarget,int addi = 0, bool bReflectEnable=true)					PURE_VIRTUAL_FUNCTION_0
    virtual bool	MyEnmityClr(void)			PURE_VIRTUAL_FUNCTION_0
	virtual bool	ReleaseSleep(void)			PURE_VIRTUAL_FUNCTION_0
	virtual bool	MagicAttack(int nType, OBJID idTarget, int x, int y,bool needLearn = false,int learnLev = 0)	PURE_VIRTUAL_FUNCTION_0
	virtual void	Kill(IRole* pTarget, DWORD dwDieWay)									PURE_VIRTUAL_FUNCTION
	virtual void	BeKill(IRole* pTarget = NULL)							PURE_VIRTUAL_FUNCTION
	virtual bool	IsAtkable(IRole* pTarget, bool bSendHint=false)			PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsBeAtkable()											PURE_VIRTUAL_FUNCTION_0
	virtual int		GetAttackRange(int nTargetSizeAdd)						PURE_VIRTUAL_FUNCTION_0
	virtual int		GetSizeAdd()											PURE_VIRTUAL_FUNCTION_0
	virtual void	SendDamageMsg( OBJID idTarget, int nDamage,DWORD dwBattleAddInfo )				PURE_VIRTUAL_FUNCTION
public:	// Ϊ���޸�ħ��ϵͳ�����ӵĲ��֣���ħ��ϵͳ�޸�����������
	virtual void	AwardSynWarScore(CNpc* pNpc, int nScore)				PURE_VIRTUAL_FUNCTION
	virtual bool	IsImmunity(IRole* pRole)								PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetProfession()											PURE_VIRTUAL_FUNCTION_0
	virtual bool	SpendEquipItem(int nSubType, int nNum, bool bSynchro)	PURE_VIRTUAL_FUNCTION_0
	virtual bool	MagicSpendItem(int nSubType, int nNum, bool bSynchro)	PURE_VIRTUAL_FUNCTION_0
	virtual int		AdjustExp(IRole* pTarget, int nRawExp, bool bNewbieBonusMsg=false)		PURE_VIRTUAL_FUNCTION_0
	virtual bool	DecEquipmentDurability(bool bBeAttack, bool bMagic, int bDurValue=1)	PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsEmbedGemType(int nGemType)							PURE_VIRTUAL_FUNCTION_0

	virtual DWORD	GetMana()												PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetMaxMana()											PURE_VIRTUAL_FUNCTION_0

	virtual int		GetEnergy()												PURE_VIRTUAL_FUNCTION_0
	virtual bool	CheckWeaponSubType(int nSubType, int nNum = 0)			PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetPotential()											PURE_VIRTUAL_FUNCTION_0
	virtual CTransformation* QueryTransformation()							PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsArrowPass(int x, int y, int nAlt=ARROW_FLY_ALT)		PURE_VIRTUAL_FUNCTION_0
	virtual CTeam*	GetTeam()												PURE_VIRTUAL_FUNCTION_0
	virtual CItem*	GetEquipItemByPos(int nPosition)						PURE_VIRTUAL_FUNCTION_0
	virtual bool	CheckCrime(IRole* pRole)								PURE_VIRTUAL_FUNCTION_0
	virtual void	SetDelay(int nDelay)									PURE_VIRTUAL_FUNCTION

	virtual CItem*	GetWeaponR			()									PURE_VIRTUAL_FUNCTION_0
	virtual CItem*	GetWeaponL			()									PURE_VIRTUAL_FUNCTION_0

	virtual bool	DropItem(OBJID idItem, int x, int y)					PURE_VIRTUAL_FUNCTION_0
	virtual DWORD	GetSoulSum()											PURE_VIRTUAL_FUNCTION_0
	virtual void	AddEquipmentDurability(int nPosition, int nAddValue)	PURE_VIRTUAL_FUNCTION
	virtual bool	SynPosition(int nPosX, int nPosY, int nMaxDislocation = 8)						PURE_VIRTUAL_FUNCTION_0
	virtual bool	Transform(DWORD dwType, int nKeepSecs, bool bSynchro = true)					PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsInFan(POINT pos, POINT posSource, int nRange, int nWidth, POINT posCenter)	PURE_VIRTUAL_FUNCTION_0
	virtual bool	AddItem (CItem* pItem, bool bSynchro, bool bUpdate = true)						PURE_VIRTUAL_FUNCTION_0
	virtual bool	Synchro()												PURE_VIRTUAL_FUNCTION_0
	virtual int		AdjustData(int nData, int nAdjust, int nMaxData=0)		PURE_VIRTUAL_FUNCTION_0
	virtual CItem*	FindSpaceTransSpell()									PURE_VIRTUAL_FUNCTION_0

	virtual	CMagic*	QueryMagic()											PURE_VIRTUAL_FUNCTION_0
	virtual bool	IgnoreTargetDodge()										PURE_VIRTUAL_FUNCTION_0	
	virtual bool	IsMustCritHit()											PURE_VIRTUAL_FUNCTION_0
};




#endif // !defined(I_ROLE_HEADER)
