// BattleSystem.h: interface for the CBattleSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLESYSTEM_H__291473E1_45B6_4A0A_8567_548852B93C33__INCLUDED_)
#define AFX_BATTLESYSTEM_H__291473E1_45B6_4A0A_8567_548852B93C33__INCLUDED_

#include "T_SingleObjSet2.h"

//////////////////////////////////////////////////////////////////////
enum LEVUPEXPDATA{
		LEVUPEXPDATA_LEVEL		=0,
		LEVUPEXPDATA_EXP,
		LEVUPEXPDATA_STU_EXP,	// ��ʦɱ���֣�������ѧԱ����ȼ���߿��Եõ��ľ��� --  zlong 2004.10.08
		LEVUPEXPDATA_EVOLUTION,//-----��� 2007/03/03--- ���������Ժ�任������
		LEVUPEXPDATA_REQLEVEL,//20070319����:����ȼ���ֵ
		LEVUPEXPDATA_UNKNOW,
		LEVUPEXPDATA_SYNSKILL_EXP,		//�����о��ȶ�Ӧ�ĵȼ�
};
char	szZcxKey[];
char	szLevupexpTable[];
typedef	CGameData<LEVUPEXPDATA, szLevupexpTable, szZcxKey>	CLevupexpData;
typedef	ISingleObjSet2<CLevupexpData>	ILevupexpSet;
typedef	CSingleObjSet2<CLevupexpData>	CLevupexpSet;

const int	_EXP_TYPE	= 1000;
enum {
	EXP_TYPE_USER		= 0,	// ������������
	EXP_TYPE_USER_FIRJOB,
	EXP_TYPE_USER_SECJOB,
	EXP_TYPE_EUDEMON,
	EXP_TYPE_SKILLEXP,
	EXP_TYPE_SYNMONEY=5,
	EXP_TYPE_SYNSKILLCON = 7,
	EXP_TYPE_WEAPONSOUL = 8,

	EXP_TYPE_TUTOR=9,//ʦͽ
	EXP_TYPE_SKILLMONEY,
};
//07.1.30�����޸�
enum ATK_TYPE
{
	ATKTYPE_PHYSICAL_COMMON		=	0,	//������ͨ����
	ATKTYPE_PHYSICAL_SKILL		=	1,	//�����ܹ���
	ATKTYPE_MAGIC_SKILL			=	2,	//ħ�����ܹ���
};
enum
{
	ATKTYPEMODE_NONE	=	0,//��Ч
	ATKTYPEMODE_COMMON	=	1,//�⹥
	ATKTYPEMODE_INNNER	=	2,//�ڹ�
	ATKTYPEMODE_ICE		=	3,//����
	ATKTYPEMODE_FIRE	=	4,//��
	ATKTYPEMODE_NOXIOUS	=	5,//����
	ATKTYPEMODE_MYSTERY	=	6,//����
};
//07.1.20�������,ƫб����,�˺��̶�
enum DAMAGE_PITCH
{
	DAMAGE_LOW	= 1,//����
	DAMAGE_HALT	= 2,//����
	DAMAGE_HIGH	= 3,//����
};

enum {
	MAGICTYPE_NONE		= 0,	// ����Ϊ0(������falseת������)
	MAGICTYPE_NORMAL	= 1,	// ����Ϊ1(������trueת������)
};

class IRole;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBattleSystem  
{
public:
	CBattleSystem(PROCESS_ID idProcess, IRole* pOwner);
	virtual ~CBattleSystem();

public:
	// һ�ν�ս��ֹ�����ʱ����
	void		ResetBattle				();
	BOOL		CreateBattle			(OBJID idTarget);
	IRole*		GetOwner				()					{return m_pOwner;}
	BOOL		checkMonsterTag			(int monsterID);

	BOOL		ProcAttack_Hand2Hand	();
	BOOL		IsBattleMaintain		();
	OBJID		GetTargetID				()					{return m_idTarget;}
	BOOL		IsActived				()					{return m_idTarget!=ID_NONE;}

public: // static
	static int	AdjustDrop( int nDrop,int nAtkLev,int nDefLev );
	static int	AdjustExp( int nExp,int nAtkLev,int nDefLev );
	//�ⲿ�ӿ�
	static int				CalcDamage( DWORD &dwBattleAddInfoRef,const ATK_TYPE& type,IRole* pAtker,IRole* pTargeter,int nPower = 0,int nSkillHitRate= 0, int nPower2=0, int nAtackType=100);

	//�ڲ�����
	//�ȼ������
	static float			GetLevelDifference( int nAtkLevel,int nTagLevel );
	//�Ƿ񴥷�������һ��,�������Ƿ�������,�������㷨��ͬ
	static bool				IsCriticalHit( IRole* pRole,bool bMagic ,IRole* pTarget = NULL);
	//----------------��ͨ���������--------------------------------------------------------------------------
	//�Ƿ�����
	static bool				IsHitRatePhysicalCommon( IRole* pAtker,IRole* pTargeter );
	//----------------�����ܹ������-------------------------------------------------------------------------
	//�Ƿ�����
	bool static				IsHitRatePhysicalSkill( IRole* pAtker,IRole* pTargeter,int nPercentValue );

	
	//----------------ħ�����ܹ������------------------------------------------------------------------------
	//������
	static int				CalcDamage_PhysicalCommon( IRole* pAtker,IRole* pTargeter,DWORD& dwBattleAddInfoRef );//������ͨ�������˺�
	static int				CalcDamage_MagicSkill( IRole* pAtker,IRole* pTargeter,int nMagicSkillPower,DWORD& dwBattleAddInfoRef, int nPower2=0, int nAtackType=100);//����ħ�������˺�

	static int				Adjust_Damage_MagicPassivity(IRole* pAtker,IRole* pTargeter,int nDamage );		//����Ӱ�챻��ħ���˺��ĺ�����softworms-2007-09-05
	static float			CheckATKType(DWORD atkType,DWORD defType);


	OBJID			m_idTarget;
	IRole*			m_pOwner;

	PROCESS_ID		m_idProcess;

public: // ctrl
	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_BATTLESYSTEM_H__291473E1_45B6_4A0A_8567_548852B93C33__INCLUDED_)
