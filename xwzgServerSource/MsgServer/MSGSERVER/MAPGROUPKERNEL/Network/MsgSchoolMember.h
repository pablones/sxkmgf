// MsgSchoolMember.h: interface for the CMsgSchoolMember class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSCHOOLMEMBER_H__82E6220E_E387_4CA2_A66E_51466E54A627__INCLUDED_)
#define AFX_MSGSCHOOLMEMBER_H__82E6220E_E387_4CA2_A66E_51466E54A627__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "../TutorList.h"

enum {
	MESSAGEBOARD_DELMEMBER		= 1, //删除
	MESSAGEBOARD_ADDMEMBER		= 2, //添加
	MESSAGEBOARD_UPDATEMEMBER	= 3, //更新
	MESSAGEBOARD_RECALL = 4,//徒弟返回自身在线状态
	MESSAGEBOARD_APPLYDEL,	
	MESSAGEBOARD_RECOVER,
	MESSAGEBOARD_ADDDENSITY,
	MESSAGEBOARD_TUTORLOGHINT,
	MESSAGEBOARD_QUERYINFO, //9 查询详细
	MESSAGEBOARD_QUERYTOTURLIST,//10 查询师父列表
	MESSAGEBOARD_ADDTOLIST,//11
	MESSAGEBOARD_SYNLINE,//12 线路之间同步师父列表数据

	MESSAGEBOARD_APPLYADDTUTOR,//申请拜师
	MESSAGEBOARD_AGREESTU,//同意拜师请求
	MESSAGEBOARD_REFUSESTU,//拒绝拜师请求
	MESSAGEBOARD_GRADUATEAPPLY,//师父带徒弟出师
	MESSAGEBOARD_DIVORCE,//协议解除师徒关系
	MESSAGEBOARD_REQDIVORCE,//徒弟响应解除关系
	MESSAGEBOARD_STUREBEL,//徒弟叛师

	MESSAGEBOARD_LINEBROAD,//其他线路的师徒消息
	MESSAGEBOARD_QUERYSTUAPPLYLIST,//查询徒弟申请列表
	MESSAGEBOARD_TUTORKICKOUTSTU,//师父踢出徒弟
	MESSAGEBOARD_QUERYSTU,		//徒弟列表
	MESSAGEBOARD_ALLLIST,		//师徒列表

	MESSAGEBOARD_DELAPPLYLIST,	//删除徒弟申请列表中的徒弟
	MESSAGEBOARD_DELSTU,		//删除徒弟列表中的徒弟
	MESSAGEBOARD_CALLSTU,		//召唤徒弟
	MESSAGEBOARD_CALLSTUREF,	//徒弟响应召唤
	MESSAGEBOARD_GETGIFT,		//师父领取徒弟的成长奖励
	MESSAGEBOARD_HASGRADUATE,	//是否拥有出过师的徒弟
	MESSAGEBOARD_GRADUATESTU,	//请求已出师的徒弟列表
	MESSAGEBOARD_GRADUATEREF,	//徒弟响应出师

	MESSAGEBOARD_TEAMADD,		//师徒组队建立师徒关系
	MESSAGEBOARD_TEAMADDREF,	//徒弟响应拜师

	MESSAGEBOARD_QUESTION,		//师徒答题 出题
	MESSAGEBOARD_ANSWERQUESTION,//师徒答题
};

enum {
	RELATION_SELF	=	0,
	RELATION_TEACHER = 1,		// 导师
	RELATION_STUDENT,			// 徒弟
	RELATION_STUDENTOFSTUDENT,	// 徒弟的徒弟(徒孙)
	RELATION_STUDENT_GRADUATE,	// 已出师的徒弟
};

enum {
	MEMBERSTATUS_OFFLINE = 0,	// 离线状态
	MEMBERSTATUS_ONLINE,		// 上线状态
	MEMBERSTATUS_RECALL,//徒弟返回自身在线状态
};
enum {
	SYNTUTOR_NONE	= 0,
	SYNTUTOR_ONLINE = 1,//师父列表中玩家上线
	SYNTUTOR_OFFLINE = 2,//师父列表中玩家下线
	SYNTUTOR_ADD	 = 3,//师父列表增加
	SYNTUTOR_DEL	 = 4,//师父列表删除
};
enum {
	LINEBROAD_STUREBEL = 0,//徒弟叛师
	LINEBROAD_TUTORKICKOUT = 1,//师父踢出徒弟
	LINEBROAD_APPLYTUTOR	= 2,//拜师请求
	LINEBROAD_STATE			= 3,//设置状态
	LINEBROAD_AGREEAPPLY	= 4,//接受拜师
	LINEBROAD_REFUSEAPPLY	= 5,//拒绝拜师
	LINEBROAD_ONLINESTATE	= 6,//在线状态
	LINEBROAD_ONLINESTATEREF= 7,//返回在线状态
	LINEBROAD_SYNGIFTMASK   = 8,//同步奖励掩码
	LINEBROAD_CALLSTU		= 9,//召唤徒弟
	LINEBROAD_CALLSTUREF	= 10,//徒弟响应召唤
	LINEBROAD_ADDEDLEV		= 11,//带徒弟升过的等级
	LINEBROAD_APPLYBYNAME	= 12,//按名字拜师
	LINEBROAD_APPLYBYNAMEREF= 13,//发送拜师申请的返回
};
// typedef struct
// {
// 	OBJID	idTutor;	// 师傅ID
// 	OBJID	idMember;				// 成员ID
// 	char	szName[_MAX_NAMESIZE];	// 成员名字
// 	char	szMate[_MAX_NAMESIZE];	// 配偶名字
// 	UCHAR	ucLevel;				// 等级
// 	UCHAR	ucProfession;			// 职业编号
// 	USHORT	usPkValue;				// PK值
// 	UCHAR	ucNobilityRank;			// 身份（爵位）
// 	UCHAR	ucRelation;				// 和玩家关系
// 	UCHAR	ucStatus;				// 状态 —— 如在线、离线
// 	DWORD	dwSynID_Rank;			// 帮派ID & 职位 (与MsgPlayer一样，高8位为职位编号，低24位为帮派ID)
// 	UINT    ucSex;
// 	UINT    uApplyType;             //申请类型
// 	int     nDensity;               //亲密度
// 	char    delTime[32];            //申请删除时间
// } ST_MEMBERINFO;
//goto 修改结构
typedef struct
{
	OBJID	idMember;				//成员ID
	union{
		DWORD	usMainJob;			//主职业
		DWORD	dwMaxScore;			//战斗力
	};
	union{
		DWORD	usSubJob;			//副职业
		DWORD	dwExitTime;			//离线时间
	};
	USHORT	usLev;					//等级
	USHORT	usMainLev;				//主职业等级
	USHORT	usSubLev;				//副职业等级
	USHORT	usLookFace;				//头像
	UINT	nImpart;				//传授度/奖励掩码
	UCHAR	ucVipLev;				//VIP等级
	UCHAR	ucInTeam;				//是否有队伍/性别
	UCHAR	ucLine;					//线路
	UCHAR	ucRelation;				//和玩家关系
	char	szName[_MAX_NAMESIZE];	//成员名字
} ST_MEMBERINFO;

class CMsgSchoolMember : public CNetMsg  
{
public:
	CMsgSchoolMember();
	virtual ~CMsgSchoolMember();

public:	
	BOOL	Create	(OBJID idUser, UCHAR ucAction, ST_MEMBERINFO* pMember=NULL, UCHAR ucAmount=0, UCHAR ucPage=0);
// 	BOOL	Append	(UCHAR ucRelation, UCHAR ucStatus, OBJID idMember, const char* pszName = NULL,OBJID idTutor = ID_NONE);
// 	BOOL	Append	(UCHAR ucRelation, UCHAR ucStatus, OBJID idMember, UCHAR ucProfession,UCHAR	ucLevel, UINT ucSex, const char* pszName = NULL);
	BOOL	Append(ST_MEMBERINFO stMember);
	BOOL	Append(TUTORLIST stInfo);
	BOOL	Append	(UCHAR ucRelation, CUser* pUser);
	BOOL	Create	(char* pMsgBuf, DWORD dwSize);
	void	Process	(CGameSocket* pSocket);

public:
	typedef struct
	{
		MSGHEAD_DEFINE

		OBJID		idUser;
		USHORT		ucAction;			// action
		UCHAR		ucAmount;			// 数量			//每页显示数量
		UCHAR		ucPage;
		ST_MEMBERINFO	setMember[1];	// 成员列表
	}MSG_Info;
	MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGSCHOOLMEMBER_H__82E6220E_E387_4CA2_A66E_51466E54A627__INCLUDED_)
