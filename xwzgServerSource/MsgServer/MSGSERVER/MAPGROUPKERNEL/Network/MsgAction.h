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
		actionChgMap			=7,			// 无需参数
		actionFlyMap			=8,
		actionChgWeather		=9,
//		actionFireworks			= 10,//发现这个变量下面又定义了一次,用下面那个吧
		actionDie				=11,
		actionQuitSyn			=12,
		actionWalk				=13,
		actionEnterMap			=14,		//通知客户端登录成功，玩家已经进入地图。仅玩家! (to client: idUser is Map)
		actionGetItemSet		= 15,
		actionGetGoodFriend		=16,		// 登录第3步
		actionForward			=17,
		actionLeaveMap			=18,		// 服务器->客户端,idPlayer。与CMsgPlayer对应，表示玩家离开地图，要求删除对象。
		actionJump				=19,
		actionRun				= 20,
		actionEquip				=21,
		actionUnequip			=22,
		actionUplev				=23,		// 玩家升级(只显示动画)
		actionXpCLear			=24,
		actionReborn			= 25,
		actionDelRole			=26,
		actionGetEquipItemSet	=27,	
		actionGetMagicSet		=28,	
		actionSetPkMode			=29,
		actionGetSynAttr		= 30,
		actionGhost				=31,		// 变鬼
		actionSynchro			=32,		// 坐标同步。send to client self, request client broadcast self coord if no synchro; broadcast to other client, set coord of this player
		actionQueryFriendInfo	=33,		// idTarget = friend id
		actionQueryLeaveWord	=34,
		actionChangeFace		= 35,
		actionTeamMemeberPos	=37,
		actionQueryPlayer		=38,
		actionAbordMagic		=39,
		actionMapARGB			= 40,		// to client only
		actionMapStatus			=41,		// to npc server only, idTarget = map id.
		actionQueryTeamMember	=42,
		actionCreateBooth		=43,		// 开始摆摊 to server - unPosX,unPosY: playerpos; unDir:dirofbooth; to client - idTarget:idnpc; 
		actionSuspendBooth		=44,		// 收起摆摊
		actionResumeBooth		= 45,		// 继续摆摊 to server - unPosX,unPosY: playerpos; unDir:dirofbooth; to client - idTarget:idnpc; 
		actionDestroyBooth		=46,		// 停止摆摊
		actionQueryCryOut		=47,		// 查询摆摊吆喝
		actionPostCmd			=48,		// to client only
		actionQueryEquip		=49,		// to server //idTarget为需要query的PlayerID
		actionAbortTransform	= 50,		// to server
		actionCombineSubSyn		=51,		// to client, idUser-> idSubSyn, idTarget-> idTargetSyn
//		actionTakeOff			=52,		// to server, wing user take off
		actionGetMoney			=53,		// to client only // 捡到500以及500以上钱，只传给自己，dwData为捡到的钱
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
		actionStudentApply		=72,		// 师父申请招收徒弟 // to server/client idUser为师父ID dwData为徒弟ID
		actionTeacherApply		=73,		// 徒弟申请拜师 // to server/client idUser为徒弟ID dwData为师父ID goto 改为师父带徒弟出师 idUser为师父
		actionAgreeStudentApply	=74,		// 同意拜师 // to server idUser为师父ID dwData为徒弟ID
		actionAgreeTeacherApply	= 75,		// 同意招收学生 // to server idUser为徒弟ID dwData为师父ID
		actionDismissStudent	=76,		// 师父单方删除学生// to server idUser为师父ID dwData为徒弟ID
		actionLeaveTeacher		=77,		// 徒弟删除师父 // to server idUser为徒弟ID dwData为师父ID
		actionQuerySchoolMember	=78,		// 查询师徒列表 //to server //
		actionTeacherRequest    =79,        // 在新手工会中发拜师申请 goto 改为徒弟响应出师
		
		////////////////
		// 佣兵任务相关
		actionQueryPlayerTaskAcceptedList		=80, // 查询玩家悬赏的已接任务列表 // to server // dwData为上一次查询的最后一个ID
		actionQueryPlayerTaskUnacceptedList		=81, // 查询玩家悬赏的未接任务列表 // to server // dwData为上一次查询的最后一个ID
//		actionQueryPlayerTaskCompletedList		=82, // 查询玩家悬赏的已完成任务列表 // to server // dwData为上一次查询的最后一个ID
		actionQueryPlayerMyTaskList				=83, // 查询玩家悬赏的我的任务列表 // to server // dwData为上一次查询的最后一个ID
		actionQueryPlayerTaskDetail				=84, // 查询玩家悬赏任务详细内容 // to server // dwData为查询任务ID

		actionAcceptPlayerTask					=88, // 接受任务 // to server // dwData为任务ID
//		actionCompletePlayerTask				=89, // 完成任务 // to server // dwData为任务ID	// <== 改用CMsgItem
		actionCancelPlayerTask					=90, // 撤销任务 // to server // dwData为任务ID

		actionLockUser							=91, // 锁定客户端并同步坐标方向
		actionUnlockUser						=92, // 解锁客户端并同步坐标方向
		actionMagicTrack						=93, // 同步坐标方向
		
		actionQueryStudentsOfStudent			=94, // 查询徒孙列表，idTarget = 需要查询徒孙列表的徒弟ID
		
		actionBurstXp			=95,		// XP爆发，允许使用XP技能
		actionTransPos			=96,		// 通知客户端将玩家移动到随机的位置
		actionDisappear			=97,		// 通知客户端目标超出视线范围消失, idUser为目标ID，dwData=0表示角色，dwData=1表示其他MapItem
		actionNotAllowLook		=98,		// 不允许查看装备
		actionClearLockTarget	=99,		//20061027修罗.清除锁定目标.不再lockAtk
		actionProcHonorKill		= 100,		//20070411修罗: S2C处理荣誉击杀
		actionProcDishonorKill	= 101,		//20070411修罗: S2C处理非荣誉击杀
		actionLockAtkBegin		= 102,		//20070424修罗:	S2C lockAtk开始
		actionLockAtkEnd		= 103,		//20070424修罗:	S2C lockAtk结束
		actionLogoutReq			= 104,		//20070601修罗: C2S 退出请求
		actionLogoutAck			= 105,		//20070601修罗: S2C 退出正确响应
		actionLogoutNack		= 106,		//20070601修罗: S2C 退出拒绝.中断
		actionRebornUseScroll	= 107,		//07.6.15王玉波，C2S,要求使用复活卷轴复活
		actionRequestRate		= 108,		// 彭峰 [6/18/2007] 请求得到最新的VAS兑换比率
//		actionAtonement         = 109,      // 要求使用赎罪卷 [陈剑飞 7/10/2007]
//		actionReturnZero        = 110,      // 要求使用归元丹 [陈剑飞 7/10/2007]
		actiondance				= 120,		//  [8/17/2007 Add By HaLei] 表情动作
		actionExpPercent        = 121,     // 经验加成 [陈剑飞9/25/2007]
		actionGameStart			= 122,		// 人物正式登陆游戏时间toClient
		WPSL_LIVE				= 123,		//泪灵激活或者休息
		WPSL_DOWN				= 124,		//泪灵拿下
		actionAddFireWorks      = 126,      // 烟火 [1/12/2008 陈剑飞]

		actionStandBy			= 125,		//  [1/15/2008 朱斌]	用于死亡后随机复活地图复活用 战士,起来吧!	

		actionGiftLearn			= 127,		// 学习天赋 [12/4/2007 dio]
		actionRebornWithOutScroll = 128,		//08.9.17郝超庆,30级以下玩家3秒后原地复活,不用复活卷
		actionRebornInstance    = 129,		//副本复活

		actionMoveComplete		=130,
        
		//经脉系统相关
        actionQueryJinmeiInfo   = 140,   // 查询经脉信息
        actionUpJinmeiLev       = 141,   // 提升经脉等级
        actionUpjinmeiLevQuick  = 142,   // 立即完成
        actionStartLearnJinmei  = 143,   // 开始修炼经脉
        actionStopLearnJinmei   = 144,   // 停止经脉修炼
        actionStrengthJinmei    = 145,   // 强化经脉
        actionAddJinmeiWaitQueue= 146,   // 加入经脉修炼队列
        actionMoveJinmeiWaitQueue = 147, // 移除出经脉修炼队列
        actionJinmeiNeedUpLev   = 148,   // 通知客户端有经脉需要升级
        actionJinmeiAutoLearn   = 149,   // 通知客户端有经脉自动开始修炼
        actionJinmeiOpratRet    = 150,   // 通知客户端经脉操作结果
		actionJinmeiWaitLearnNum= 151,   // 通知客户端可操作的数目
		actionAddJinmeiValue    = 152,   // 领取真元
	    actionUseAllZhenyuanDan = 153,   // 消耗所有的真元丹
		// for ai server
		actionSuperChgMap		= 200,		// to game server, data=idMap
		actionFullItem			= 201,		// to game server, data=itemtype
		actionChangeMapDoc		= 202,//no use		// to ai server, idPlayer=idMap, data=idDoc
		actionAddTerrainObj		= 203,		// to ai server, idUser=idMap, data=idOwner, npc_id=idTerrainObj
		actionDelTerrainObj		= 204,		// to ai server, idUser=idMap, data=idOwner

		actionResumeMoveSpeed	= 205,		// to NPC server,恢复某怪物移动速度06.9.25王玉波添加
        actionAbordMine         = 206,      //停止采矿
       	actionGetMine           = 207,      //获得矿石
		atcionPkProcRed         = 208,       //红名保护
		actionPkProcWhile       = 209,       //白名保护
		atcionPkProcSyn			= 109,		//帮派保护

		actionDoAction          = 210,
		actionDoScript          = 211,       //触发脚本
		actionDoGenScript       = 212,       //触发产生任务脚本
		actionRequireMarry      = 213,       //申请婚礼

		actionNoEudemon		    = 214,
		actionLoverStorageLevUp = 215,       //爱心柜子的升级
		actionLoverFlowerLevUp  = 216,       //爱心花盆的升级
		actionLoverCabinetLevUp = 217,       //爱心厨房的升级
		actionTaskOnTime        = 218,       //及时完成脚本
		actionForSuggust		= 219,
		actionLeanMagic			= 220,
		actionUseLock			= 221,
		actionQueryServerAlive	= 222,
		actionMarryBed			= 223,		//恢复玩家的血和蓝
		actionSetBooth			= 224,		//设置摊位
	
		//actionCancelDisStudent	= 225,		// 取消开除学生//
		actionTeamLeaveTeacher	= 225,		//goto 协议解除师徒关系
//		actionCancelLevTeacher	= 226,		// 取消背叛师门//
		actionStudentLeave		= 226,		//goto 徒弟响应解除师徒关系
		actionAgrResortStudent	= 227,		// 同意恢复学生//
		actionAgrResortTeacher	= 228,		// 同意恢复老师//
		actionResetGift         = 229,      //重置天赋点
		actionLearnMagicLearn   = 230,       //触发魔法学习Action
		actionEudemonChoose     = 232,
		actionQueryEudemon		= 233,

		actionGetPlayDetil		= 235,
		actionClientInitOK		= 236,

		//请求第二职业
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
		actionFeePoint			= 267, //心跳
		actionSychroVas			= 268,//同步元宝
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

		// goto 元宝票和经验丹
		actionVasToCheck		= 279,//元宝换票
		actionEXPToCheck		= 281,//经验换丹

		//采集
		actionMine				= 282,
		actionStopMine			= 283,        

		//请求新手成就信息
		actionQueryNewGive		= 284,
		actionGetNewGive		= 285,

		actionQueryOtherInfo    =286,//玩家人物面板其他信息栏
		actionGetAward          =287, //在线时间奖励
		actionStopAutoAtk		=288,//清空自动攻击目标
		actionGetEudemonMagicCD =289,//获取宠物技能CD
		actionGetTeamMemPos		=290,//获取队友的坐标
		actionSendPosToTeam		=291,//发送自己的位置给队友

		actionQueryAuto			=292,//请求挂机信息

		actionClientInitComplete =293,//客户端初始化全部完成
		actionQueryTitleList	 =294,//请求称号
		actionSetUseTitle		 =295,//设置当前称号

		actionFireWorks			 =296,//goto 放烟花 仅发送
		actionSetDress			=299,//设置时装是否显示
		actionQueryDress		=300,//请求是否显示时装
		actionQueryTreasure		=301,//goto 请求彩票包物品
		actionLottery			=302,//goto 彩票开箱子
		actionGetLeaveExpSuc		=303,
		actionGetLeaveExpFai		=304,
		actionGetTreasure			=305,//取出奖品包中的奖品到背包
		actionDropTreasure			=306,//丢弃奖品包中的一个物品

		actionEveryDayWorkPresent	=307,//日程表
		actionQuickDayWork			=308,
		actionQueryInsInfo			=309,
////////////////////////////上线祝福//////////////////////////////////////////////
		actionWeapensoulMixSuc		=312,
		actionGift					=311,
		actionAcceptGift			=310,		
		actionChoseTimeBox			=313,//开启时限宝盒
		actionVipGetVas				=314,//vip领取珠宝
		actionQueryGift				=315,
		actionEudemonTranse         =316,
		actionEudemonIntelUp        =317,
		actionEudemonFusionUp       =318,
		actionEudemonSoul           =320,
		actionRemoveEudemonSoul     =321,
		actionEudemonChangeSex		=322,//宠物变性
		actionEudemonPlayAdd		= 323,//选择玩耍宠物
		actionEudemonPlayLock		= 324,//锁定
		actionEudemonPlay			= 325,//确定玩耍
		actionEudemonPlayCancel		= 326,//取消玩耍
	
		//20080110修罗:AI相关-----------------------
		MSGACTION_CHGTARGET_SUGGEST	= 1001,		//Msg 2 Npc 转换目标建议.npc采纳与否,Msg不关心
		MSGACTION_CHGTARGET_ACK,			//Npc 2 Msg 转换目标同意.Msg根据此消息也改变目标
		MSGACTION_RESET_REQ,				//Npc 2 Msg 重置请求.之后msg会做重置处理.重置完成后.会回应[应答]
		MSGACTION_RESET_ACK,				//Msg 2 Npc 重置应答.在此之前.所有重置动作已经由别的线路传回npc
		MSGACTION_CLSENMITYLIST,			//Msg 2 Npc 清空仇恨列表
		//AI相关------------------------------------
		
	
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
	BOOL    Create  (USHORT usDir  , USHORT usAction, DWORD dwData=0);// 为广播经验加成新添一条创建 [陈剑飞9/26/2007]
	BOOL    Create  (OBJID idPlayer, int nPosX      , int nPosY, USHORT usDir, USHORT usAction, DWORD dwData,OBJID idNpc,BOOL bShow);
	BOOL	Create  (OBJID idPlayer, int nPosX      , int nPosY, USHORT usDir, USHORT usAction, USHORT usTargetPosX, USHORT usTargetPosY, const char* szTargetName);
	BOOL    Create  (MSG_GIFT cGift);
public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void *pInfo);
	virtual	void	DumpMsg(LPCTSTR szReasion,...);///参数为导出这个消息的原因

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
