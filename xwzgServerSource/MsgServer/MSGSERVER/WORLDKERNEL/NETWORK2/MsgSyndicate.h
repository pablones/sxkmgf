// MsgBetOutside.h: interface for the CMsgAtk class.
// 仙剑修，2002.1.8
//////////////////////////////////////////////////////////////////////

#ifndef	MSGSYNDICATE_H
#define MSGSYNDICATE_H

#include "NetMsg.h"
#include "NetStringPacker.h"

// enum{
// 	ACTION_NONE = 0,				// 保留值
// 
// 	APPLY_JOIN			= 1,			// 申请加入黑社会, id
// 	INVITE_JOIN			= 2,			// 邀请加入黑社会, id
// 	LEAVE_SYN			= 3,			// 脱离黑社会
// 	KICKOUT_MEMBER		= 4,			// 开除黑社会成员, name
// 	QUERY_SYN_NAME		= 6,			// 查询帮派名字
// 	SET_ALLY			= 7,			// 结盟				// to client, npc(npc_id is syn_id, same follow)
// 	CLEAR_ALLY			= 8,			// 解除结盟			// to client, npc
// 	SET_ANTAGONIZE		= 9,			// 树敌				// to client, npc
// 	CLEAR_ANTAGONIZE	= 10,			// 解除树敌			// to client, npc
// 	DONATE_MONEY		= 11,			// 帮众捐钱
// 	QUERY_SYNATTR		= 12,			// 查询帮派信息		// to server
// 	SET_SYN				= 14,			// 添加帮派ID		// to client
// 	//UNITE_SUBSYN		= 15,			// 合并堂口 // to client // dwData被合并, target是主人
// 	//UNITE_SYN			= 16,			// 合并帮派 // to client // dwData被合并, target是主人
// 	SET_WHITE_SYN		= 17,			// 白帮帮派ID // 未被占领请发ID_NONE
// 	SET_BLACK_SYN		= 18,			// 黑帮帮派ID // 未被占领请发ID_NONE
// 	DESTROY_SYN			= 19,			// 世界广播，删除帮派。
// 	SET_MANTLE          = 20,            // 世界广播，披风 
// 	_APPLY_ALLY			= 21,			// 申请结盟			// to server&client, idTarget=SynLeaderID
// 	_CLEAR_ALLY			= 22,			// 清除结盟			// to server
// 	_SET_ANTAGONIZE		= 23,			//树敌 client to server
// 	_CLEAR_ANTAGONIZE   = 24,			//解除树敌 client to server
// 	INVITE_JOIN_NAME	= 25,			//[游途道标 2008/10/6]邀请加入帮派
// 	INVITE_RETIRE		= 26,			//[游途道标 2008/10/7]辞职
// 	NPCMSG_CREATE_SYN	= 101,			// 通知NPC服务器添加帮派	// to npc
// 	NPCMSG_DESTROY_SYN	= 102,			// 通知NPC服务器删除帮派	// to npc
// 	KICKOUT_MEMBER_INFO_QUERY	= 110,	//帮主查询申请开除的成员
// 	KICKOUT_MEMBER_AGREE		= 111,	//帮主同意开除会员
// 	KICKOUT_MEMBER_NOAGREE		= 112,	//帮主不同意开除会员
// 	SYNMEMBER_ASSIGN		= 113,			//帮派成员编制
// 	SYN_CHANGE_NAME			= 114,			// 帮派改名
// 	SYN_CHANGE_SUBNAME		=114,		//分团改名
// 	SYN_CHANGE_SUBSUBNAME	=115,		//分队改名
// 	SYN_DEMISE				=116,		//禅让
// 	SYN_SET_ASSISTANT		=117,		//[游途道标 2008/9/27]对帮众提职
// 	SYN_SET_TEAMLEADER		=118,		//设置帮派队长
// 	SYN_SET_PUBLISHTIME		=119,		//设置公告时间
// 	SYN_SET_BUS				=120,		//帮内职务变更
// 	SYN_PAYOFF				=121,		//20070413修罗:C2S 军团长发工资
// 	SYN_WAR                 =122,       //约战
// 	SYN_BUY_UP              =123,
// 	SYN_MAIN_UP				=124,
// 	SYN_TASK_UP				=125,
// 	SYN_CHG_UP				=126,
// 	INVITE_JOIN_RE			=127,
// 	SYN_WAR_SET             =128,       //约战
// };

enum SyndicateAction{
	ACTION_NONE = 0,				// 保留值
		
		APPLY_JOIN			= 1,			// 申请加入黑社会, id
		INVITE_JOIN			= 2,			// 邀请加入黑社会, id
		LEAVE_SYN			= 3,			// 脱离黑社会
		KICKOUT_MEMBER		= 4,			// 开除黑社会成员, name
		QUERY_SYN_NAME		= 6,			// 查询帮派名字
		SET_ALLY			= 7,			// 结盟				// to client, npc(npc_id is syn_id, same follow)
		CLEAR_ALLY			= 8,			// 解除结盟			// to client, npc
		SET_ANTAGONIZE		= 9,			// 树敌				// to client, npc
		CLEAR_ANTAGONIZE	= 10,			// 解除树敌			// to client, npc
		DONATE_MONEY		= 11,			// 帮众捐钱
		QUERY_SYNATTR		= 12,			// 查询帮派信息		// to server
		
		SET_SYN				= 14,			// 添加帮派ID		// to client
		//UNITE_SUBSYN		= 15,			// 合并堂口 // to client // dwData被合并, target是主人
		//UNITE_SYN			= 16,			// 合并帮派 // to client // dwData被合并, target是主人
		SET_WHITE_SYN		= 17,			// 白帮帮派ID // 未被占领请发ID_NONE
		SET_BLACK_SYN		= 18,			// 黑帮帮派ID // 未被占领请发ID_NONE
		DESTROY_SYN			= 19,			// 世界广播，删除帮派。
		SET_MANTLE          = 20,            // 世界广播，披风 // add huang 2004.1.1       
		
		_APPLY_ALLY			= 21,			// 申请结盟			// to server&client, idTarget=SynLeaderID
		_CLEAR_ALLY			= 22,			// 清除结盟			// to server
		
		_SET_ANTAGONIZE		= 23,			//树敌 client to server
		_CLEAR_ANTAGONIZE   = 24,			//解除树敌 client to server
		
		INVITE_JOIN_NAME	= 25,			//[游途道标 2008/10/6]邀请加入帮派
		INVITE_RETIRE	= 26,			//[游途道标 2008/10/7]辞职
		
		NPCMSG_CREATE_SYN	= 101,			// 通知NPC服务器添加帮派	// to npc
		NPCMSG_DESTROY_SYN	= 102,			// 通知NPC服务器删除帮派	// to npc
		
		KICKOUT_MEMBER_INFO_QUERY	= 110,	//帮主查询申请开除的成员
		KICKOUT_MEMBER_AGREE		= 111,	//帮主同意开除会员
		KICKOUT_MEMBER_NOAGREE		= 112,	//帮主不同意开除会员
		
		SYNMEMBER_ASSIGN	= 113,			//帮派成员编制	
		
		SYN_CHANGE_NAME		= 114,			// 帮派改名
		
		
		
		
		SYN_CHANGE_SUBNAME		=114,		//分团改名
		SYN_CHANGE_SUBSUBNAME	=115,		//分队改名
		
		
		SYN_SET_ASSISTANT		=117,		//[游途道标 2008/9/27]对帮众提职
		SYN_SET_TEAMLEADER		=118,		//设置帮派队长
		SYN_SET_PUBLISHTIME		=119,		//设置公告时间
		SYN_SET_BUS				=120,		//帮内职务变更
		SYN_PAYOFF				=121,		//20070413修罗:C2S 军团长发工资
		SYN_WAR                 =122,       //约战
		SYN_BUY_UP              =123,
		SYN_MAIN_UP				=124,
		SYN_TASK_UP				=125,
		SYN_CHG_UP				=126,
		INVITE_JOIN_RE			=127,
		SYN_WAR_SET             =128,       //约战
		SYN_WAR_SEND            =129,       //约战
		
		
		SYN_CREATE              =201,
		SYN_CREATE_OK           =202,
		SYN_CREATE_ERROR        =203,
		SYN_QUERY_CREATE        =204,
		SYN_JOIN				=205,
		SYN_JOIN_QUERY			=206,
		SYN_QUERY_LIST          =207,
		SYN_QUERY_INFO          =208,
		SYN_QUERY_MEMBER        =209,
		SYN_QUERY_QUERYMEMBER   =210,
		SYN_ANSERR_OK           =211,
		SYN_ANSERR_NO           =212,
		SYN_KICKOUT	            =213,
		SYN_LEAVER	            =214,
		SYN_SETRANK	            =215,
		SYN_UPDATE	            =216,
		SYN_DEMISE				=217,		//禅让
		SYN_ANNONCE				=218,
		SYN_CREATECOMPLETE		=219,
		SYN_ANNONCE_OTHERLINE	=220,
		SYN_EVERYSUB			=221,		//分堂信息
		SYN_BUILDER				=222,
		SYN_BUILDER_OTHERLINE	=223,		//工匠修改
		SYN_UPLEV				=224,		//帮会升级
 		SYN_UPSKILLSUB			=225,		//分堂建造
		SYN_UPLEV_OTHERLINE		=226,
		SYN_UPSKILLSUB_OTHERLINE=227,
		SYN_SYNWORK				=228,		//打工
		SYN_WORKOVER			=229,
		SYN_CONTRIBUTE			=230,		//捐献
		SYN_SETBUILDOTHERLINE	=231,
		SYN_REDUCELEV			=232,
		SYN_INVITE				=233,		//邀请入帮
		SYN_INVITE_OTHERLINE	=234,
		SYN_CHANGELINE			=335,		//换线--jay
		SYN_IMPACH				=336,		//弹劾--jay
		SYN_IMPACHOTHERLINE		=337,		//其他线发来的弹劾
		SYN_MEMBERLIST			=301,		//成员列表
		SYN_CONLIST				=302,		//帮贡列表
		SYN_QUERYLIST			=303,		//申请列表
		SYN_QUERYSYNLIST		=304,		//帮派列表
		SYN_SYNINFO				=305,		//帮派信息
		SYN_SCIINFO				=306,		//科技信息
		SYN_LEVELUP				=310,		//帮派升级
		SYN_ACTION_ENTERSYN		=320,		//快速回帮
		SYN_SUB1_ACTION			=321,		//青龙
		SYN_SUB2_ACTION			=322,		//白虎
		SYN_SUB3_ACTION			=323,		//玄武
		SYN_SUB4_ACTION			=324,		//朱雀
		SYN_ACTION_WELFARE		=325,		//领取福利
		SYN_ACTION_SHOP			=326,		//商店
		SYN_SYNTALKMSG			=334,		//帮派群发消息
		SYN_UPDATEOTHERNLINE	=335,		//更新
		SYN_SETLEADER			=338,		//任命帮主
		SYN_SETSYNMAPID			=340,		//更新帮派地图id
};

WORLDKERNEL_BEGIN
class CMsgSyndicate : public CNetMsg
{
public:
	CMsgSyndicate();
	virtual ~CMsgSyndicate();
public:	

	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void *pInfo);

//protected:
	typedef struct {
		MSGHEAD_DEFINE

		unsigned short	usAction;
		unsigned short	dwData;
		OBJID	idTarget;
		char	szBuf[1];
		
// 		unsigned short	usAction;
// 		union
// 		{
// 			OBJID	idTarget;
// 			DWORD	dwData;
// 		};
// 		union {
// 			OBJID	idFealty;				// 上级帮派ID
// 			OBJID	idUser;					// 仅用于修改帮派编制时
// 		};
// 		char	szBuf[1];
	}MSG_Info;
	
	MSG_Info*	m_pInfo;

	CNetStringPacker m_StrPacker;
	
};
WORLDKERNEL_END

#endif // MSGSYNDICATE_H
