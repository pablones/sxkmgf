// MsgAction.h: interface for the CMsgAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGDIR_H__950C0770_DB2D_4D79_970C_E2863ECE9614__INCLUDED_)
#define AFX_MSGDIR_H__950C0770_DB2D_4D79_970C_E2863ECE9614__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum {	actionNone				=0, 
		actionChgDir			=1, 
		actionPosition			=2, 
		actionEmotion			=3,
		actionBroadcastPos		=4,
		actionDivorce			= 5,
		actionSelfUnfreeze		=6,
		actionChgMap			=7,			// �������
		actionFlyMap			=8,
		actionChgWeather		=9,
//		actionFireworks			= 10,//����������������ֶ�����һ��,�������Ǹ���
		actionDie				=11,
		actionQuitSyn			=12,
		actionWalk				=13,
		actionEnterMap			=14,		//֪ͨ�ͻ��˵�¼�ɹ�������Ѿ������ͼ�������! (to client: idUser is Map)
		actionGetItemSet		= 15,
		actionGetGoodFriend		=16,		// ��¼��3��
		actionForward			=17,
		actionLeaveMap			=18,		// ������->�ͻ���,idPlayer����CMsgPlayer��Ӧ����ʾ����뿪��ͼ��Ҫ��ɾ������
		actionJump				=19,
		actionRun				= 20,
		actionEquip				=21,
		actionUnequip			=22,
		actionUplev				=23,		// �������(ֻ��ʾ����)
		actionXpCLear			=24,
		actionReborn			= 25,
		actionDelRole			=26,
		actionGetEquipItemSet	=27,	
		actionGetMagicSet		=28,	
		actionSetPkMode			=29,
		actionGetSynAttr		= 30,
		actionGhost				=31,		// ���
		actionSynchro			=32,		// ����ͬ����send to client self, request client broadcast self coord if no synchro; broadcast to other client, set coord of this player
		actionQueryFriendInfo	=33,		// idTarget = friend id
		actionQueryLeaveWord	=34,
		actionChangeFace		= 35,
		actionTeamMemeberPos	=37,
		actionQueryPlayer		=38,
		actionAbordMagic		=39,
		actionMapARGB			= 40,		// to client only
		actionMapStatus			=41,		// to npc server only, idTarget = map id.
		actionQueryTeamMember	=42,
		actionCreateBooth		=43,		// ��ʼ��̯ to server - unPosX,unPosY: playerpos; unDir:dirofbooth; to client - idTarget:idnpc; 
		actionSuspendBooth		=44,		// �����̯
		actionResumeBooth		= 45,		// ������̯ to server - unPosX,unPosY: playerpos; unDir:dirofbooth; to client - idTarget:idnpc; 
		actionDestroyBooth		=46,		// ֹͣ��̯
		actionQueryCryOut		=47,		// ��ѯ��̯ߺ��
		actionPostCmd			=48,		// to client only
		actionQueryEquip		=49,		// to server //idTargetΪ��Ҫquery��PlayerID
		actionAbortTransform	= 50,		// to server
		actionCombineSubSyn		=51,		// to client, idUser-> idSubSyn, idTarget-> idTargetSyn
//		actionTakeOff			=52,		// to server, wing user take off
		actionGetMoney			=53,		// to client only // ��500�Լ�500����Ǯ��ֻ�����Լ���dwDataΪ�񵽵�Ǯ
//		actionCancelKeepBow		=54,		// to server, cancel keep_bow status
		actionQueryEnemyInfo	= 55,		// idTarget = enemy id	// to server
		actionMoveStop			=56,		// to target client, data=milli secs.
		actionKickBack			=57,		// to client	idUser is Player ID, unPosX unPosY is Player pos
		actionDropMagic			=58,		// to client only, data is magic type
		actionDropSkill			=59,		// to client only, data is weapon skill type
		actionSoundEffect		= 60,		// to client only, client play sound effect, dwData is monster type
		actionQueryMedal		=61,		// to server idUser is Player ID, dwData is medal
		actionDelMedal			=62,		// to server idUser is Player ID, dwData is medal
		actionAddMedal			=63,		// to client idUser is Player ID, dwData is medal
		actionSelectMedal		=64,		// to server idUser is Player ID, dwData is medal
		actionQueryHonorTitle	= 65,		// to server idUser is Player ID, dwData is title
		actionDelHonorTitle		=66,		// to server idUser is Player ID, dwData is title
		actionAddHonorTitle		=67,		// to client idUser is Player ID, dwData is title
		actionSelectHonorTitle	=68,		// to server idUser is Player ID, dwData is title
		actionOpenDialog		=69,		// to client only, open a dialog, dwData is id of dialog
		actionFlashStatus		= 70,		// broadcast to client only, team member only. dwData is dwStatus
		actionQuerySynInfo		=71,		// to server, return CMsgSynInfo. dwData is target syn id.

		///////////////
		actionStudentApply		=72,		// ʦ����������ͽ�� // to server/client idUserΪʦ��ID dwDataΪͽ��ID
		actionTeacherApply		=73,		// ͽ�������ʦ // to server/client idUserΪͽ��ID dwDataΪʦ��ID goto ��Ϊʦ����ͽ�ܳ�ʦ idUserΪʦ��
		actionAgreeStudentApply	=74,		// ͬ���ʦ // to server idUserΪʦ��ID dwDataΪͽ��ID
		actionAgreeTeacherApply	= 75,		// ͬ������ѧ�� // to server idUserΪͽ��ID dwDataΪʦ��ID
		actionDismissStudent	=76,		// ʦ������ɾ��ѧ��// to server idUserΪʦ��ID dwDataΪͽ��ID
		actionLeaveTeacher		=77,		// ͽ��ɾ��ʦ�� // to server idUserΪͽ��ID dwDataΪʦ��ID
		actionQuerySchoolMember	=78,		// ��ѯʦͽ�б� //to server //
		actionTeacherRequest    =79,        // �����ֹ����з���ʦ���� goto ��Ϊͽ����Ӧ��ʦ
		
		////////////////
		// Ӷ���������
		actionQueryPlayerTaskAcceptedList		=80, // ��ѯ������͵��ѽ������б� // to server // dwDataΪ��һ�β�ѯ�����һ��ID
		actionQueryPlayerTaskUnacceptedList		=81, // ��ѯ������͵�δ�������б� // to server // dwDataΪ��һ�β�ѯ�����һ��ID
//		actionQueryPlayerTaskCompletedList		=82, // ��ѯ������͵�����������б� // to server // dwDataΪ��һ�β�ѯ�����һ��ID
		actionQueryPlayerMyTaskList				=83, // ��ѯ������͵��ҵ������б� // to server // dwDataΪ��һ�β�ѯ�����һ��ID
		actionQueryPlayerTaskDetail				=84, // ��ѯ�������������ϸ���� // to server // dwDataΪ��ѯ����ID

		actionAcceptPlayerTask					=88, // �������� // to server // dwDataΪ����ID
//		actionCompletePlayerTask				=89, // ������� // to server // dwDataΪ����ID	// <== ����CMsgItem
		actionCancelPlayerTask					=90, // �������� // to server // dwDataΪ����ID

		actionLockUser							=91, // �����ͻ��˲�ͬ�����귽��
		actionUnlockUser						=92, // �����ͻ��˲�ͬ�����귽��
		actionMagicTrack						=93, // ͬ�����귽��
		
		actionQueryStudentsOfStudent			=94, // ��ѯͽ���б�idTarget = ��Ҫ��ѯͽ���б��ͽ��ID
		
		actionBurstXp			=95,		// XP����������ʹ��XP����
		actionTransPos			=96,		// ֪ͨ�ͻ��˽�����ƶ��������λ��
		actionDisappear			=97,		// ֪ͨ�ͻ���Ŀ�곬�����߷�Χ��ʧ, idUserΪĿ��ID��dwData=0��ʾ��ɫ��dwData=1��ʾ����MapItem
		actionNotAllowLook		=98,		// ������鿴װ��
		actionClearLockTarget	=99,		//20061027����.�������Ŀ��.����lockAtk
		actionProcHonorKill		= 100,		//20070411����: S2C����������ɱ
		actionProcDishonorKill	= 101,		//20070411����: S2C�����������ɱ
		actionLockAtkBegin		= 102,		//20070424����:	S2C lockAtk��ʼ
		actionLockAtkEnd		= 103,		//20070424����:	S2C lockAtk����
		actionLogoutReq			= 104,		//20070601����: C2S �˳�����
		actionLogoutAck			= 105,		//20070601����: S2C �˳���ȷ��Ӧ
		actionLogoutNack		= 106,		//20070601����: S2C �˳��ܾ�.�ж�
		actionRebornUseScroll	= 107,		//07.6.15���񲨣�C2S,Ҫ��ʹ�ø�����Ḵ��
		actionRequestRate		= 108,		// ��� [6/18/2007] ����õ����µ�VAS�һ�����
//		actionAtonement         = 109,      // Ҫ��ʹ������� [�½��� 7/10/2007]
//		actionReturnZero        = 110,      // Ҫ��ʹ�ù�Ԫ�� [�½��� 7/10/2007]
		actiondance				= 120,		//  [8/17/2007 Add By HaLei] ���鶯��
		actionExpPercent        = 121,     // ����ӳ� [�½���9/25/2007]
		actionGameStart			= 122,		// ������ʽ��½��Ϸʱ��toClient
		WPSL_LIVE				= 123,		//���鼤�������Ϣ
		WPSL_DOWN				= 124,		//��������
		actionAddFireWorks      = 126,      // �̻� [1/12/2008 �½���]

		actionStandBy			= 125,		//  [1/15/2008 ���]	������������������ͼ������ սʿ,������!	

		actionGiftLearn			= 127,		// ѧϰ�츳 [12/4/2007 dio]
		actionRebornWithOutScroll = 128,		//08.9.17�³���,30���������3���ԭ�ظ���,���ø����
		actionRebornInstance    = 129,		//��������

		actionMoveComplete		=130,
        
		//����ϵͳ���
        actionQueryJinmeiInfo   = 140,   // ��ѯ������Ϣ
        actionUpJinmeiLev       = 141,   // ���������ȼ�
        actionUpjinmeiLevQuick  = 142,   // �������
        actionStartLearnJinmei  = 143,   // ��ʼ��������
        actionStopLearnJinmei   = 144,   // ֹͣ��������
        actionStrengthJinmei    = 145,   // ǿ������
        actionAddJinmeiWaitQueue= 146,   // ���뾭����������
        actionMoveJinmeiWaitQueue = 147, // �Ƴ���������������
        actionJinmeiNeedUpLev   = 148,   // ֪ͨ�ͻ����о�����Ҫ����
        actionJinmeiAutoLearn   = 149,   // ֪ͨ�ͻ����о����Զ���ʼ����
        actionJinmeiOpratRet    = 150,   // ֪ͨ�ͻ��˾����������
		actionJinmeiWaitLearnNum= 151,   // ֪ͨ�ͻ��˿ɲ�������Ŀ
		actionAddJinmeiValue    = 152,   // ��ȡ��Ԫ
	    actionUseAllZhenyuanDan = 153,   // �������е���Ԫ��
		// for ai server
		actionSuperChgMap		= 200,		// to game server, data=idMap
		actionFullItem			= 201,		// to game server, data=itemtype
		actionChangeMapDoc		= 202,//no use		// to ai server, idPlayer=idMap, data=idDoc
		actionAddTerrainObj		= 203,		// to ai server, idUser=idMap, data=idOwner, npc_id=idTerrainObj
		actionDelTerrainObj		= 204,		// to ai server, idUser=idMap, data=idOwner

		actionResumeMoveSpeed	= 205,		// to NPC server,�ָ�ĳ�����ƶ��ٶ�06.9.25�������
        actionAbordMine         = 206,      //ֹͣ�ɿ�
       	actionGetMine           = 207,      //��ÿ�ʯ
		atcionPkProcRed         = 208,       //��������
		actionPkProcWhile       = 209,       //��������
		atcionPkProcSyn			= 109,		//���ɱ���

		actionDoAction          = 210,
		actionDoScript          = 211,       //�����ű�
		actionDoGenScript       = 212,       //������������ű�
		actionRequireMarry      = 213,       //�������

		actionNoEudemon		    = 214,
		actionLoverStorageLevUp = 215,       //���Ĺ��ӵ�����
		actionLoverFlowerLevUp  = 216,       //���Ļ��������
		actionLoverCabinetLevUp = 217,       //���ĳ���������
		actionTaskOnTime        = 218,       //��ʱ��ɽű�
		actionForSuggust		= 219,
		actionLeanMagic			= 220,
		actionUseLock			= 221,
		actionQueryServerAlive	= 222,
		actionMarryBed			= 223,		//�ָ���ҵ�Ѫ����
		actionSetBooth			= 224,		//����̯λ
	
		//actionCancelDisStudent	= 225,		// ȡ������ѧ��//
		actionTeamLeaveTeacher	= 225,		//goto Э����ʦͽ��ϵ
//		actionCancelLevTeacher	= 226,		// ȡ������ʦ��//
		actionStudentLeave		= 226,		//goto ͽ����Ӧ���ʦͽ��ϵ
		actionAgrResortStudent	= 227,		// ͬ��ָ�ѧ��//
		actionAgrResortTeacher	= 228,		// ͬ��ָ���ʦ//
		actionResetGift         = 229,      //�����츳��
		actionLearnMagicLearn   = 230,       //����ħ��ѧϰAction
		actionEudemonChoose     = 232,
		actionQueryEudemon		= 233,

		actionGetPlayDetil		= 235,
		actionClientInitOK		= 236,

		//����ڶ�ְҵ
		actionGetSecInfo		= 237,
		actionUpUserlev			= 238,
		actionAddPoint			= 239,
		actionActiveNPC			= 240,

		actionAcceptTask		= 241,
		actionGiveupTask		= 242,
		actionEndTask			= 243,

		actionEudemonChgName	= 244,
		actionEudemonEat		= 245,
		actionEudemonHappy		= 246,
		actionEudemonGiveup		= 247,
		actionAddEudemonPoint   = 248,
		actionEudemonError		= 249,
		actionEudemonBornAdd	= 250,
		actionEudemonBornLock	= 251,
		actionEudemonBorn		= 252,
		actionEudemonBornCancel	= 253,
		actionEudemonBaby		= 254,
		actionEudemonUnderUp	= 255,
		actionEudemonMix        = 256,
		actionEudemonLean       = 257,
		actionEudemonKillUp     = 258,
		actionEudemonCall		= 259,
		actionEudemonReset      = 260,
		actionEudSkillUnlean    = 261,
		actionTaskSelectItem    = 262,
		actionTaskAnswerOk	    = 263,
		actionFlyMapByItem	    = 264,
		actionPKProcSyn			= 265, 
		actionGotojial_Public	= 266,
		actionFeePoint			= 267, //����
		actionSychroVas			= 268,//ͬ��Ԫ��
		actionQueryEudemonList  = 269,
		actionQuerySkillList	= 270,
		actionQueryUserkeyList	= 271,
		actionQueryCDList		= 272,
		actionQueryMissList		= 273,
		actionQueryMoneyAdd		= 274,
		actionQueryBuffAdd      = 275,
		actionLifeSpanadd       = 276,
		actionQueryLeaveExp     = 277,
		actionGetLeaveExp		= 278,

		// goto Ԫ��Ʊ�;��鵤
		actionVasToCheck		= 279,//Ԫ����Ʊ
		actionEXPToCheck		= 281,//���黻��

		//�ɼ�
		actionMine				= 282,
		actionStopMine			= 283,        

		//�������ֳɾ���Ϣ
		actionQueryNewGive		= 284,
		actionGetNewGive		= 285,

		actionQueryOtherInfo    =286,//����������������Ϣ��
		actionGetAward          =287, //����ʱ�佱��
		actionStopAutoAtk		=288,//����Զ�����Ŀ��
		actionGetEudemonMagicCD =289,//��ȡ���＼��CD
		actionGetTeamMemPos		=290,//��ȡ���ѵ�����
		actionSendPosToTeam		=291,//�����Լ���λ�ø�����

		actionQueryAuto			=292,//����һ���Ϣ

		actionClientInitComplete =293,//�ͻ��˳�ʼ��ȫ�����
		actionQueryTitleList	 =294,//����ƺ�
		actionSetUseTitle		 =295,//���õ�ǰ�ƺ�

		actionFireWorks			 =296,//goto ���̻� ������
		actionSetDress			=299,//����ʱװ�Ƿ���ʾ
		actionQueryDress		=300,//�����Ƿ���ʾʱװ
		actionQueryTreasure		=301,//goto �����Ʊ����Ʒ
		actionLottery			=302,//goto ��Ʊ������
		actionGetLeaveExpSuc		=303,
		actionGetLeaveExpFai		=304,
		actionGetTreasure			=305,//ȡ����Ʒ���еĽ�Ʒ������
		actionDropTreasure			=306,//������Ʒ���е�һ����Ʒ

		actionEveryDayWorkPresent	=307,//�ճ̱�
		actionQuickDayWork			=308,
		actionQueryInsInfo			=309,
////////////////////////////����ף��//////////////////////////////////////////////
		actionWeapensoulMixSuc		=312,
		actionGift					=311,
		actionAcceptGift			=310,		
		actionChoseTimeBox			=313,//����ʱ�ޱ���
		actionVipGetVas				=314,//vip��ȡ�鱦
		actionQueryGift				=315,
		actionEudemonTranse         =316,
		actionEudemonIntelUp        =317,
		actionEudemonFusionUp       =318,
		actionEudemonSoul           =320,
		actionRemoveEudemonSoul     =321,
		actionEudemonChangeSex		=322,//�������
		actionEudemonPlayAdd		= 323,//ѡ����ˣ����
		actionEudemonPlayLock		= 324,//����
		actionEudemonPlay			= 325,//ȷ����ˣ
		actionEudemonPlayCancel		= 326,//ȡ����ˣ
	
		//20080110����:AI���-----------------------
		MSGACTION_CHGTARGET_SUGGEST	= 1001,		//Msg 2 Npc ת��Ŀ�꽨��.npc�������,Msg������
		MSGACTION_CHGTARGET_ACK,			//Npc 2 Msg ת��Ŀ��ͬ��.Msg���ݴ���ϢҲ�ı�Ŀ��
		MSGACTION_RESET_REQ,				//Npc 2 Msg ��������.֮��msg�������ô���.������ɺ�.���Ӧ[Ӧ��]
		MSGACTION_RESET_ACK,				//Msg 2 Npc ����Ӧ��.�ڴ�֮ǰ.�������ö����Ѿ��ɱ����·����npc
		MSGACTION_CLSENMITYLIST,			//Msg 2 Npc ��ճ���б�
		//AI���------------------------------------
		
	
};

//enum { PKMODE_FREE=0, PKMODE_SAFE, PKMODE_TEAM, PKMODE_ARRESTMENT, PKMODE_SYNDICATE };    //change huang 2004.1.15
const int _ACTION_DANCE1		=001;
const int _ACTION_STANDBY		=100;
const int _ACTION_LAUGH			=150;
const int _ACTION_GUFFAW		=151;
const int _ACTION_FURY          =160;
const int _ACTION_SAD	        =170;
const int _ACTION_EXCITEMENT	=180;
const int _ACTION_SAYHELLO		=190;
const int _ACTION_SALUTE		=200;
const int _ACTION_GENUFLECT     =210;
const int _ACTION_KNEEL			=220;
const int _ACTION_COOL			=230;
const int _ACTION_SWIM			=240;
const int _ACTION_SITDOWN       =250;
const int _ACTION_ZAZEN			=260;
const int _ACTION_FAINT	        =270;
const int _ACTION_LIE	        =271;
const int _ACTION_NOINCSP		=272;
//const int _ACTION_WALK		= 272;
typedef struct{
	OBJID Gift1   ; 
	OBJID Gift2   ;
	OBJID Gift3	  ;
	OBJID Gift4   ; 
	USHORT usAction;
	USHORT DayAmount ;
	USHORT Num;
}MSG_GIFT;
 
class CMsgAction : public CNetMsg  
{
public:

	CMsgAction();
	virtual ~CMsgAction();

	BOOL	Create	(OBJID idPlayer, int nPosX      , int nPosY, USHORT usDir, USHORT usAction, DWORD dwData=0);
	BOOL	Create	(OBJID idPlayer, int nPosX      , int nPosY, USHORT usDir, USHORT usAction, USHORT usTargetPosX, USHORT usTargetPosY);
	BOOL    Create  (USHORT usDir  , USHORT usAction, DWORD dwData=0);// Ϊ�㲥����ӳ�����һ������ [�½���9/26/2007]
	BOOL    Create  (OBJID idPlayer, int nPosX      , int nPosY, USHORT usDir, USHORT usAction, DWORD dwData,OBJID idNpc,BOOL bShow);
	BOOL	Create  (OBJID idPlayer, int nPosX      , int nPosY, USHORT usDir, USHORT usAction, USHORT usTargetPosX, USHORT usTargetPosY, const char* szTargetName);
	BOOL    Create  (MSG_GIFT cGift);
public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void *pInfo);
	virtual	void	DumpMsg(LPCTSTR szReasion,...);///����Ϊ���������Ϣ��ԭ��

protected:

	typedef struct {
		MSGHEAD_DEFINE

		DWORD		dwTimeStamp;

		OBJID		idUser;
		USHORT		unPosX, unPosY;
		USHORT		unDir;
		union {
			OBJID		idTarget;
			DWORD		dwData;
			INT			iData;
			BOOL		bSucDone;
			struct {
				USHORT usTargetPosX;
				USHORT usTargetPosY;
			};
		};
		USHORT		usAction;
		OBJID       idNpc;
		char		szTargetName[16];
	}MSG_Info;

	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGDIR_H__950C0770_DB2D_4D79_970C_E2863ECE9614__INCLUDED_)
