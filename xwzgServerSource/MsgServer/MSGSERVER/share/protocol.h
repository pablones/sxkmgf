// protocol.h
// 仙剑修，2002.10.23
//////////////////////////////////////////////////////////////////////

#ifndef	PROTOCOL_H
#define	PROTOCOL_H

#include <string.h>
#include "define.h"
#include <vector>
#ifdef MAPGROUP_KERNEL
#include "UserData.h"
#endif


enum{
		//////////////////////////////////////////////////////////////////////
		// 地图组或世界处理的消息
		KERNEL_BASE					= 1000,
		KERNEL_CLIENTMSG			= KERNEL_BASE + 1,				// CLIENTMSG_PACKET0(size:1024+12), 客户端上传的消息。
//		KERNEL_NPCMSG				= KERNEL_BASE + 2,				// NPCMSG_PACKET0(size:1024+12), NPC服务器上传的消息。
		KERNEL_CLOSEKERNEL			= KERNEL_BASE + 3,				// INT, SOCKET_ID。通知核心关闭(核心必须再次通知SOCKET层!!!)
		KERNEL_CREATESYNDICATE		= KERNEL_BASE + 10,				// CreateSyndicateInfo
		KERNEL_DESTROYSYNDICATE		= KERNEL_BASE + 11,				// OBJID
		KERNEL_CHANGESYNDICATE		= KERNEL_BASE + 12,				// SynFuncInfo0

		//////////////////////////////////////////////////////////////////////
		// 由地图组核心处理的消息
		MAPGROUP_BASE				= 10000,
		MAPGROUP_LOGIN				= MAPGROUP_BASE + 1,			// ST_LOGIN, 玩家登录。
		MAPGROUP_BROADCASTMSG		= MAPGROUP_BASE + 2,			// CLIENTMSG_PACKET0(size:1024+12), 转发广播消息。
		MAPGROUP_SOCKETUSERINFO		= MAPGROUP_BASE + 3,			// ST_USERCHANGEMAPGORUP。STRUCT(SOCKET_ID,UserInfoStruct)，玩家换地图组。
		MAPGROUP_SENDOBJINFO		= MAPGROUP_BASE + 4,			// CHANGEMAPGORUP_INFO0{id,type,info}
		MAPGROUP_TRANSMITMSG		= MAPGROUP_BASE + 5,			// TRANSMSG_PACKET0(size:1024+16), 转发广播消息。
		MAPGROUP_CHANGEMAPGROUP		= MAPGROUP_BASE + 10,			// ST_CHANGEMAPGROUP。玩家换地图组。
		MAPGROUP_CREATENEWNPC		= MAPGROUP_BASE + 11,			// ST_CREATENEWNPC。创建一个新NPC。
		MAPGROUP_CHANGETEAM			= MAPGROUP_BASE + 12,			// ST_CHANGETEAM, 转发TEAM修改OPERATION。
		MAPGROUP_REMOTECALL			= MAPGROUP_BASE + 13,			// REMOTE_CALL0, 转发给USER。
		MAPGROUP_CHANGENPC			= MAPGROUP_BASE + 14,			// int+str+str, 转发NPC修改OPERATION。
		MAPGROUP_DELTRANSNPC		= MAPGROUP_BASE + 15,			// int, 转发删除TRANS_NPC
//		MAPGROUP_DELALLMONSTER		= MAPGROUP_BASE + 16,			// int, 删除所有怪物，以便接收新怪物。同时发到SOCKET_KERNRL
		MAPGROUP_TRANSMITMSG_FORCHATROOM = MAPGROUP_BASE + 20,		// 聊天室消息
		MAPGROUP_NEWDAY				= MAPGROUP_BASE + 21,			//李京哲0321 新的一天
		MAPGROUP_PHYLEMSG	        = MAPGROUP_BASE + 27,			//给其它地图组线程,处理种族聊天信息
		MAPGROUP_NOWUPDATE_CALCULATED_PROPERTY = MAPGROUP_BASE + 28,//20070122修罗:切换地图组时,更新角色卡
		MAPGROUP_CHANGEMAPGROUP_REQ	= MAPGROUP_BASE + 29,			//20070329修罗:source->aim切换地图组请求.
		MAPGROUP_CHANGEMAPGROUP_ACK	= MAPGROUP_BASE + 30,			//20070329修罗:aim->source切换地图组同意.
		MAPGROUP_CHANGEMAPGROUP_NACK= MAPGROUP_BASE + 31,			//20070329修罗:aim->source切换地图组拒绝.
		MAPGROUP_USERCLOSEONTIMER	= MAPGROUP_BASE + 32,			//20070518修罗:地图组控制某玩家终止onTimer
		MAPGROUP_GMFLYPLAYER		= MAPGROUP_BASE + 33,			//------HALEI---2007/05/23----GM飞玩家
		MAPGROUP_GMSTOPSAY			= MAPGROUP_BASE + 34,			//  [8/13/2007 Add By HaLei] GM禁言
		MAPGROUP_SENDMSG			= MAPGROUP_BASE + 38,			//指定地图组转发消息-彭正帮-2008-1-4
		MAPGROUP_NEW_SYN_TIME       = MAPGROUP_BASE + 40,
		MAPGROUP_LOGOUTALLUSER		= MAPGROUP_BASE + 49,			//zeng踢出隶属于该地图组的所有玩家
		MAPGROUP_CHGTUTOR   		= MAPGROUP_BASE + 54,			//师徒跨线程处理
		MAPGROUP_AWARDINFO   		= MAPGROUP_BASE + 59,//专线用
		MAPGROUP_MAINSERVER		    = MAPGROUP_BASE + 61,           //主游戏服务器消息
		MAPGROUP_VASMSG				= MAPGROUP_BASE + 62,			//关于元宝的消息
		MAPGROUP_MSG				= MAPGROUP_BASE + 63,			//转发的消息
		//////////////////////////////////////////////////////////////////////
		// 由世界核心处理的消息
		WORLD_BASE					= 20000,
		WORLD_SHELLTALK				= WORLD_BASE + 1,				// STRING, 外壳发送TALK消息。
		WORLD_SETPROCESSID			= WORLD_BASE + 2,				// CHANGE_USERDATA, 设置新的PROCESS_ID
		WORLD_CLOSESOCKET			= WORLD_BASE + 3,				// INT, SOCKET_ID，核心通知SOCKET关闭(SOCKET不再通知核心)
		WORLD_USERNAMEMSG			= WORLD_BASE + 4,				// USERNAMEMSG_PACKET0(size:1024+16+12), 按名字转发给玩家。
		WORLD_USERIDMSG				= WORLD_BASE + 5,				// USERIDMSG_PACKET0(size:1024+4+12), 按ID转发给玩家。
		WORLD_LEVELUP				= WORLD_BASE + 6,				// OBJID, 玩家升到10级。
		WORLD_QUERYFEE				= WORLD_BASE + 7,				// OBJID, ACCOUNT ID。
		WORLD_SENDTIME				= WORLD_BASE + 8,				// INT, SOCKET_ID
		WORLD_TRANSMITMSG			= WORLD_BASE + 9,				// TRANSMSG_PACKET0(size:1024+16), 转发消息。
		WORLD_VAS					= WORLD_BASE + 10,				// 处理增值系统的消息转发
		WORLD_USERCLOSEONTIMER		= WORLD_BASE + 11,				// 通知地图组内某玩家终止ontimer
		WORLD_CHGMAP				= WORLD_BASE + 12,				// 彭峰 [5/23/2007] 通知世界线程处理GM飞人消息
		WORLD_STOPSAY				= WORLD_BASE + 13,				//  [8/13/2007 Add By HaLei] 通知世界现成处理禁言消息
		WORLD_TEAMINVITE			= WORLD_BASE + 14,				//  [2007-9-7] 朱斌 申请组队
		WORLD_FRIENDAPPLY			= WORLD_BASE + 15,				//  [2007-9-10] 朱斌 申请好友	
		WORLD_TUTOR_INFO	    	= WORLD_BASE + 20,				//师徒信息
		WORLD_RELATIONMSG			= WORLD_BASE + 23,				//转发到关系服务器处理
		WORLD_TRANSMITNETMSG		= WORLD_BASE + 24,				//世界转发NetMsg
		//////////////////////////////////////////////////////////////////////
		// 由SOCKET核心处理的消息
		SOCKET_BASE					= 30000,
		SOCKET_CLOSESOCKET			= SOCKET_BASE + 1,				// INT, SOCKET_ID，核心通知SOCKET关闭(SOCKET不再通知核心)
		SOCKET_SETPROCESSID			= SOCKET_BASE + 2,				// CHANGE_USERDATA, 设置新的PROCESS_ID
		//!!! 核心发送该消息时，要同时更新CUSER中的PROCESS_ID标志。核心在处理KERNEL_CLIENTMSG时，一定要检查PROCESS_ID是否匹配!!!
		SOCKET_SENDCLIENTMSG		= SOCKET_BASE + 3,				// SENDCLIENTMSG_PACKET0(size:1024+12), 发向客户端和NPC服务器的消息。
//		SOCKET_SENDNPCMSG			= SOCKET_BASE + 4,				// SENDNPCMSG_PACKET0(size:1024+12), 发向客户端和NPC服务器的消息。
//		SOCKET_SETNPCSOCKETID		= SOCKET_BASE + 5,				// INT, SOCKET_ID, 世界核心接收到NPC服务器的登录消息时，通知SOCKET核心。(可能为SOCKET_NONE)
//		SOCKET_SETNPCPROCESSID		= SOCKET_BASE + 6,				// CHANGE_NPCPROCESSID, 设置新的PROCESS_ID
		SOCKET_CHANGECODE			= SOCKET_BASE + 7,				// CHANGE_USERDATA。修改动态密码。
		SOCKET_BREAKCONNECT			= SOCKET_BASE + 8,				// INT, SOCKET_ID, 踢玩家时，先断开SOCKET连接。
		SOCKET_BREAKALLCONNECT		= SOCKET_BASE + 11,				// INT, GM_SOCKET_ID, 踢所有玩家，禁止所有连接。


		//////////////////////////////////////////////////////////////////////
		// 由外壳处理的消息
		SHELL_BASE					= 40000,
		SHELL_PRINTTEXT				= SHELL_BASE + 1,				// STRING, 核心发送的TEXT消息
		SHELL_KERNELSTATE			= SHELL_BASE + 2,				// STRING, 核心发送的STATE串，多行		
		SHELL_REMOTE_CLOSE			= SHELL_BASE + 3,				//  [11/28/2007 朱斌]	远程关闭

		//////////////////////////////////////////////////////////////////////
		// 由数据库核心处理的消息
		DB_BASE						= 50000,
		DB_QUERY_USERINFO			= DB_BASE + 1,

		DB_MSG_BASEINFO				= DB_BASE + 101,
		DB_MSG_FRIENDS				= DB_BASE + 102,
		DB_MSG_ENEMYS				= DB_BASE + 103,
		DB_MSG_ITEMS				= DB_BASE + 104,
		DB_MSG_WEAPON_SKILL			= DB_BASE + 105,
		DB_MSG_MAGICS				= DB_BASE + 106,
		DB_MSG_SYNATTR				= DB_BASE + 107,
};

struct CLIENTMSG_PACKET0			// 接收的CLIENT消息
{
	SOCKET_ID		idSocket;			// SOCKET_ID
	OBJID	idPacket;
	int		nSize;
	ULONG	nIP;
	char	buf[1];				// 用于结构指针，不需要空间
};

struct NPCMSG_PACKET0			// 接收的NPC消息
{
	OBJID	idNpc;
	OBJID	idPacket;
	int		nSize;
	char	buf[1];				// 用于结构指针，不需要空间
};

struct SENDCLIENTMSG_PACKET0
{
	SOCKET_ID		idSocket;			// SOCKET_ID
	OBJID	idPacket;
	int		nSize;
	char	buf[1];				// 用于结构指针，不需要空间
};

// struct SENDNPCMSG_PACKET0
// {
// 	OBJID	idNpc;				// 广播消息用BCAST_NPCID
// 	OBJID	idPacket;
// 	int		nSize;
// 	char	buf[1];				// 用于结构指针，不需要空间
// };

struct TRANSMSG_PACKET0			// 转发给其它进程处理的消息
{
	SOCKET_ID		idSocket;			// SOCKET_ID
	OBJID			idNpc;				// 同时支持NPC的消息
	OBJID	idPacket;
	int		nSize;
	int		nTrans;				// 附加数据
	char	buf[1];				// 用于结构指针，不需要空间
};

struct TRANSPHYMSG_PACK0 //转发给其它地图组,种族聊天信息
{
	int				nPhyle;				//种族
	SOCKET_ID		idSocket;			// SOCKET_ID
	OBJID			idPacket;			//包ID
	int				nSize;				//大小
	char			buf[ 1 ];			//内容
};

struct USERNAMEMSG_PACKET0			// 让WORLD转发的消息，按名字转发
{
	SOCKET_ID		idSocket;			// SOCKET_ID
	OBJID	idPacket;
	int		nSize;
	char	szName[_MAX_NAMESIZE];		// 转发给该玩家
	char	buf[1];				// 用于结构指针，不需要空间
};

struct USERIDMSG_PACKET0			// 让WORLD转发的消息，按ID转发
{
	SOCKET_ID		idSocket;			// SOCKET_ID
	OBJID	idPacket;
	int		nSize;
	OBJID	idUser;				// 转发给该玩家
	char	buf[1];				// 用于结构指针，不需要空间
};


struct CHANGE_USERDATA
{
	SOCKET_ID	idSocket;
	int			nData;
};

struct CHANGE_NPCPROCESSID
{
	SOCKET_ID	idNpc;
	PROCESS_ID	idProcess;		// 为SOCKET_NONE时为删除
};

struct ST_LOGIN
{
	SOCKET_ID	idSocket;
	OBJID		idUser;
	ULONG		nIP;
};

#ifdef MAPGROUP_KERNEL
struct ST_USERCHANGEMAPGORUP
{
	SOCKET_ID	idSocket;
	UserInfoStruct		info;
	ULONG		nIP;
};
#endif

#ifdef MAPGROUP_KERNEL
struct ST_NEWPLAYERSTATECHANGEMAPGORUP
{
	OBJID		idUser;
	char		newPlayerState[_MAX_NEWPLAYEREXAMSTATE];
};
#endif
	
#ifdef MAPGROUP_KERNEL
struct ST_PRESENT_NUMCHANGEMAPGORUP
{
	OBJID		idUser;
	int		nPresentNum[_MAX_PRESENT_NUM];
};
#endif
	
struct ST_TUTORSTATECHANGEMAPGORUP
{
	OBJID	idByStudent[3];
	OBJID	idUserByStudent[3];
	OBJID	idTutorByStudent[3];
	NAMESTR	szUserNameByStudent[3];
	NAMESTR	szTutorNameByStudent[3];

	OBJID	id;
	OBJID	idUser;
	OBJID	idTutor;
	NAMESTR	szUserName;
	NAMESTR	szTutorName;

	DWORD		m_dwExpToTutor;


};


struct CHANGEMAPGORUP_INFO0
{
	OBJID		idUser;
	int			nInfoType;
	char		info[1];
};

struct CHANGE_MAPGROUP
{
	OBJID		idUser;
	PROCESS_ID	idProcess;
};

struct ST_CHANGEMAPGROUP
{
	DWORD		idSourceProgress;//20070329修罗:当前地图组id.用于反馈 
	OBJID		idUser;
	OBJID		idMap;
	int			nPosX;
	int			nPosY;
	void		*lpBooth;//[游途道标 2009.05.12]
};

struct ST_CREATENEWNPC
{
	OBJID			id;
	OBJID			idMap;
	OBJID			idData;
	OBJID			idOwner;
	int				nData;
	unsigned short	usPosX;
	unsigned short	usPosY;
	unsigned short	usAction;
	unsigned short	usType;
	unsigned char	ucOwnerType;
	UINT            unCitywarType;
};

struct ST_CREATENEWNPCNEW
{
	OBJID			idMap;
	unsigned short	usPosX;
	unsigned short	usPosY;
	unsigned short	usType;
	OBJID			idGen;
};

struct DukDeleteObject
{
	template <typename T>
	void operator()(T* ptr)
	{
	    delete ptr;
	}
};


struct InstanceUseTimeInfo
{
	UINT  id;
	UINT  nUserId;
	UINT  nInstanceType;
	UINT  nUserTime;
	char  szCreateTime[32];
	UINT  nCanUser;
	UINT  nFinishTimes;
};

struct CAutoFightInfo
{
//	MSGHEAD_DEFINE
	UINT nAction;
	
	OBJID id  ;                 //自动增长的索引       
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
	//  以下为自动恢复设置字段：
// 	UINT autoRestoration_hp ;  //是否开启自动恢复保护中的"开启生命保护"，开启为"1","2"，未开启为"0"。
// 	char aRhp_item[_MAX_NAMESIZE] ;           //使用何种道具以自动恢复血量。填写道具ID。
// 	char aRhp_item2[_MAX_NAMESIZE] ;          //使用何种道具以自动恢复血量。填写道具ID。
// 	UINT aRhpPercentage ;      //当血量低至最大值百分之多少时，触发自动回血。填写百分数。
// 	UINT aRhpPercentage2 ;      //当血量低至最大值百分之多少时，触发自动回血。填写百分数。
// 	UINT autoRestoration_mp;   //是否开启自动恢复保护中的"开启魔法值保护"，开启为"1","2"，未开启为"0"。
// 	char aRmp_Item[_MAX_NAMESIZE];            //使用何种道具以自动恢复魔法量。填写道具ID。
// 	char aRmp_Item2[_MAX_NAMESIZE];            //使用何种道具以自动恢复魔法量。填写道具ID。
// 	UINT armpPercentage ;      //当魔法量低至最大值百分之多少时，触发自动回魔。填写百分数。
// 	UINT armpPercentage2 ;      //当魔法量低至最大值百分之多少时，触发自动回魔。填写百分数。
// 
// // 	UINT autoRestoration_hp ;  //是否开启自动恢复保护中的"开启生命保护"，开启为"1"，未开启为"0"。
// // 	char aRhp_item[_MAX_NAMESIZE] ;           //使用何种道具以自动恢复血量。填写道具ID。
// // 	UINT aRhpPercentage ;      //当血量低至最大值百分之多少时，触发自动回血。填写百分数。
// // 	UINT autoRestoration_mp;   //是否开启自动恢复保护中的"开启魔法值保护"，开启为"1"，未开启为"0"。
// // 	char aRmp_Item[_MAX_NAMESIZE];            //使用何种道具以自动恢复魔法量。填写道具ID。
// // 	UINT armpPercentage ;      //当魔法量低至最大值百分之多少时，触发自动回魔。填写百分数。
// 	// 以下为自动挂机设置字段：
// 	UINT AutoAttack_hp  ;      //是否开启自动挂机中的"开启生命保护"，开启为"1"，未开启为"0"。
// 	char AAhp_item[_MAX_NAMESIZE] ;           //使用何种道具以自动恢复血量。填写道具ID。
// 	UINT AahpPercentage ;      //当血量低至最大值百分之多少时，触发自动回血。填写百分数。
// 	UINT AutoAttack_mp ;      //是否开启自动挂机中的"开启魔法值保护"，开启为"1"，未开启为"0"。
// 	char AAmp_item[_MAX_NAMESIZE]   ;        //使用何种道具以自动恢复魔法量。填写道具ID。
// 	UINT AampPercentage ;     //当血量低至最大值百分之多少时，触发自动回魔。填写百分数。
// 	UINT Pickup ;             //自动拾取设置，1开启自动拾取，0未开启自动拾取，2拾取药品，4拾取材料，8拾取宝石，16拾取技能书，32拾取优秀品质以上，64拾取完美品质以上，128拾取卓越品质以上。
// 	UINT autousevas  ;         //自动使用增值道具，开启为"1"，未开启为"0"，2为自动使用复活卷 4为自动使用神将锤，为自动使用双倍经验药。
// 	UINT autoshopping ;       //自动购买设置，1为开启自动购买，0为未开启，2为自动购买红，4为自动购买蓝，8为自动购买复活卷。该字段填值为以上各设定值相加总数。
// 	UINT num_hp ;             //自动购买红药的数量。
// 	UINT num_mp ;             //自动购买蓝药的数量。
// 	UINT num_fh  ;            //自动购买复活卷的数量。
// 	UINT num_double  ;        //自动使用双倍经验药数量。
// 	UINT goal  ;              //自动打怪目标的选择，0为最近的怪，1为正在攻击自己的怪。
// 	UINT way  ;               //自动打怪方式的选择，0为普通攻击，1为技能攻击。
// 	UINT autoUseAssit;        //自动使用辅助魔法
// 	UINT autoUseAttack;       //自动使用攻击魔法
// 
// 	//宠物部分
// 	OBJID	dwEudemon1;
// 	OBJID	dwEudemon2;
// 	DWORD	dwEudemonDrinkType;				
// 	DWORD	dwEudemonDrinkRedID;			
// 	DWORD	dwEudemonDrinkBlueID;			
// 	DWORD	dwEudemonDrinkRedTypeID;			
// 	DWORD	dwEudemonDrinkBlueTypeID;			
// 	DWORD	dwEudemonPercentRed;			
// 	DWORD	dwEudemonPercentBlue;
// 
// 	char skills1[_MAX_NAMESIZE]   ;          //自动打怪使用技能设定，skills1~3为增益技能，skills4~7为攻击技能，填写技能ID。
// 	char skills2[_MAX_NAMESIZE];
// 	char skills3[_MAX_NAMESIZE];
// 	char skills4[_MAX_NAMESIZE];
// 	char skills5[_MAX_NAMESIZE];
// 	char skills6[_MAX_NAMESIZE];
// 	char skills7[_MAX_NAMESIZE];
};

struct ChgInstanceUseInfo
{
	UINT nAction;
	InstanceUseTimeInfo nInfo;
};

typedef struct
{
	OBJID		idSyn;
	NAMESTR		szName;
	OBJID		idLeader;
	NAMESTR		szLeader;
	int			nMoney;
	char        szWord[1024];
}CreateSyndicateInfo;

typedef struct
{
	//[游途道标 2008/9/24]在开头新增idUser,nSex,nProfession三个成员
	OBJID			idUser;//玩家标识
	int				nSex;//玩家性别
	int				nProfession;//玩家职业
	//[游途道标 2008/9/25]增加bOnline
	bool			bOnline;//是否在线
	OBJID			idSyn;
	union {
		struct {
			int		nRank;				//帮派成员的军衔
			int		nSynMemberLevel;	//帮派成员级别
		};
		struct {
			DWORD	dwData[2];	
		};
	};
	unsigned short	nSize;
	unsigned char	ucFuncType;
	unsigned char	ucSaveFlag;
	union{
		long		IntParam[1];
		char		StrParam[1];
	};
}SynFuncInfo0;

struct GM_FLYPLAYER
{
	OBJID		UserID;
	int			MapData;
	int			MapX;
	int			MapY;
};//  [5/23/2007] 哈雷  GM飞玩家的数据
struct GM_STOPSAY
{
	char	szname[16];
	int		sec;
};//  [8/13/2007 Add By HaLei] 
struct ST_CHANGETEAM
{
	int				nAction;
	OBJID			idTeam;
	OBJID			idUser;
	int				nData1;
	int				nData2;
	int				nData3;
	int				nData4;
	char			teamName[24];//20070202修罗:小队名字.
};

struct	REMOTE_CALL0
{
	OBJID			idUser;
	unsigned short	nSize;
	unsigned char	ucFuncType;
	union{
		long		IntParam[2];
		char		StrParam[1];
	};
};

struct	VARIANT_SET0
{
	long		m_setLong[1];
	long&		IntParam(int idx)			{ return m_setLong[idx]; }
	char*		StrParam(int idx)			{ return (char*)&m_setLong[idx]; }
};


/////////////////////////////////////////
// DBService用	-- zlong 2004.09.06

struct	ST_QUERY_USERINFO
{
	OBJID	idUser;
};

const int		_MEDAL_BUFSIZE		= 64+1;
const int		_TITLE_BUFSIZE		= 64+1;

struct	ST_DB_USERINFO
{
	OBJID	idUser;

	// 以下为了4字节对齐，没有按照原来的顺序
	DWORD	dwCoin_money;
	DWORD	dwLast_login;
	DWORD	dwLookface;
	DWORD	dwHair;
	char	setMedal[_MEDAL_BUFSIZE];
	UCHAR	ucLevel;
	UCHAR	ucMetempsychosis;
	UCHAR	ucNobility;
	DWORD	dwMedal_select;

	char	setTitle[_TITLE_BUFSIZE];
	UCHAR	ucProfession;
	UCHAR	ucAdditional_point;
	UCHAR	ucAuto_allot;
	DWORD	dwTitle_select;

	DWORD	dwMoney;
	DWORD	dwMoney_saved;
	USHORT	usRecordX;
	USHORT	usRecordY;
	//OBJID	id;
	OBJID	idAccount;
	OBJID	idRecordmap;
	OBJID	idSyndicate;
	int		nDeed;
	int		nExp;

	_int64	i64TaskMask;
	_int64	i64DayTaskMask;

	DWORD	dwVirtue;
	OBJID	idHome;
	DWORD	dwLockKey;
	//DWORD	dwCurrLockKey;
	char	szMate[_MAX_NAMESIZE];
	char	szName[_MAX_NAMESIZE];
	USHORT	usForce;
	USHORT	usConstitution;
	USHORT	usHealth;
	int		usLife;
	USHORT	usMana;
	USHORT	usSoul;
	USHORT	usSpeed;
	SHORT	sPk;
};

struct	ST_DB_FRIEND_INFO
{
		OBJID	id;
		OBJID	idUser;
		OBJID	idFriend;
		char	szFriendName[_MAX_NAMESIZE];
};

struct	ST_DB_FRIEND
{
	OBJID	idUser;

	int		nAmount;
	ST_DB_FRIEND_INFO	setFriends[1];
};

struct	ST_DB_ENEMY
{
	OBJID	idUser;
};

struct	ST_DB_ITEMINFO
{
	OBJID	id;

};
struct	ST_DB_ITEM
{
	OBJID	idUser;

	int		nAmount;
	ST_DB_ITEMINFO	setItems[1];
};

struct	ST_DB_WEAPON_SKILL
{
	OBJID	idUser;
};

struct	ST_DB_MAGIC
{
	OBJID	idUser;
};

struct	ST_DB_SYNATTR
{
	OBJID	idUser;
};

enum
{
	FRIEND_APPLY_ACTION_TYPE_APPLY_REQ,
	FRIEND_APPLY_ACTION_TYPE_APPLY_ANK,
	FRIEND_APPLY_ACTION_TYPE_APPLY_FIN,
	FRIEND_APPLY_ACTION_TYPE_APPLY_NANK,
};

//////////////////////////////////////////////////////////////////////////
//  [2007-9-10] 朱斌 跨地图组加好友
typedef struct  
{
	unsigned short	unAction;
	OBJID			idPlayer;
	NAMESTR			szPlayerName;
	int				nLookFace;
	int				nLev;
	int				nProfession;
	OBJID			idTarget;
	NAMESTR			szTargetName;
	int				nTargetLookFace;
	int				nTargetLev;
	int				nTargetProfession;
	int				nRet;
	int             nSex;
	int				nFriendType;
}ST_FRIEND_APPLY;

enum
{
	TEAM_INVITE_ACTION_TYPE_INVITE_REQ,
	TEAM_INVITE_ACTION_TYPE_INVITE_ANK,
	TEAM_INVITE_ACTION_TYPE_INVITE_NANK,
	TEAM_INVITE_ACTION_TYPE_INVITE_ANK_RETURN,
	TEAM_INVITE_ACTION_TYPE_JOIN_REQ,
	TEAM_INVITE_ACTION_TYPE_JOIN_ANK,
	TEAM_INVITE_ACTION_TYPE_JOIN_ANK_RETURN,
	TEAM_INVITE_ACTION_TYPE_JOIN_FINAL,
	TEAM_INVITE_ACTION_TYPE_JOIN_NANK,
	TEAM_INVITE_ACTION_TYPE_WAIT,
};

#define _MAX_TEAMMEMBER 6

struct AwardInfoStruct
{
	int nTime;
	int nLeave;
};

struct TeamInfoStruct
{
	OBJID	id;
	OBJID	idLeader;
	OBJID	setMember[_MAX_TEAMMEMBER];
		//--------add by cz 队长的职业
	DWORD   setProfession[_MAX_TEAMMEMBER];
	BOOL	bCloseMoney;
	BOOL	bCloseItem;
	int     nLeaderlev;
	//------------------
	int							nTeamMode;
	int							nExpMode;
	int							nItemMode;
	char						sTeamName[24];
	//------------------
	//------------------
	char                        m_sLeadName[16];
	int                         m_nLev;
	int                         m_nTarget;
	char                        m_sTeamText[96];

};

typedef struct TeamMemberInfo_Tag
{
	char			szName[_MAX_NAMESIZE];
	OBJID			id;
	DWORD			dwLookFace;
	DWORD           dwLev;
	USHORT			usProfession;
	USHORT			usProLev;
	BOOL			bOnlineStatus;///标识此队员的在线状态,1是在线,0是不在线
	OBJID			idMap;///下线时此队员所在的地图id
	USHORT			usPosX;///下线时所在的X坐标
	USHORT			usPosY;///下线时所在的Y坐标
	TeamMemberInfo_Tag()
	{
		::memset(this,0x0,sizeof(this));
		bOnlineStatus=true;
		idMap=0;///下线时此队员所在的地图id
		usPosX=0;///下线时所在的X坐标
		usPosY=0;///下线时所在的Y坐标
	}
}TeamMemberInfo;

//////////////////////////////////////////////////////////////////////////
//  [2007-9-7] 朱斌 跨地图组申请组队
typedef struct  
{
	unsigned short	unAction;
	OBJID			idPlayer;
	NAMESTR			szPlayerName;
	SOCKET_ID		idSocket;
	OBJID			idTarget;
	NAMESTR			szTargetName;
	SOCKET_ID		idSocketTarget;	
	int				nRet;	//	返回
							/*
							第1位为是否自动组队
							第2位为是否锁定队伍
							第3位为经验各自取得
							第4位为经验值平均分配
							第5位为物品是否各自取得
							第6位为物品是否自由拾取
							第7位为物品是否随机分配
	*/
	
	TeamInfoStruct		teamInfo;
	
	int				nMemberInfoCount;
	TeamMemberInfo	teamMemberInfo[_MAX_TEAMMEMBER];

}ST_TEAM_INVITE;

//goto 修改师徒为以下结构
// struct TutorInfoStruct
// {
// 	OBJID	id;
// 	OBJID	idUser;		//徒弟ID
// 	OBJID	idTutor;	//师父ID
// 	NAMESTR	szUserName;	//徒弟名字
// 	NAMESTR	szTutorName;//师父名字
// 	UINT	nImpart;	//传授度
// 	short	nIsGraduated;//是否已出师
// 	short	nHasGift;	//师父是否有已出师的徒弟的奖励
// 	DWORD	nGiftMask;	//徒弟奖励掩码
// 	DWORD	nTutorMask;	//师父奖励掩码
// 	UINT	nStuLev;	//徒弟等级
// 	UINT	nTutorLev;	//师父等级
// 	UINT	nTutorImpartLv;//师父师德等级
// };
enum CHGTUTOR_DATA
{
	CHGTUTORDATA_CLEARTUTOR,
	CHGTUTORDATA_REQDELTUTOR,
	CHGTUTORDATA_REQDELSTUDENT,
	CHGTUTORDATA_RECOVERTUTOR,
	CHGTUTORDATA_RECOVERSTUDENT,
	CHGTUTORDATA_REQADDDENSITY,
	CHGTUTORDATA_CANCELLEAVETUTOR,
	CHGTUTORDATA_CANCELDELSTUDENT,
};

const int APPLYDEL_TUTOR   = 1;     //学生申请删除老师
const int APPLYDEL_STUDENT = 2;     //老师申请删除学生

const int MAX_DENSITY      = 10000;  //最大亲密度

struct ChgTutorInfo
{
	UINT  nAction;
	OBJID idUser;
	OBJID idTutor;
	OBJID idStudent;
	OBJID id;
	int   nTutorImpart;
};

//////////////////////////////////////////////////////////////////////
// C/S 协议的客户端部分	

// 该部分常量、定义仅由以下两个函数使用
const int	PROTOCOL_MSG_HEADSIZE		= 4;
struct CProtocolMsgStruct{
	unsigned short	nMsgSize;
	unsigned short	idPacket;
	char			pMsg[1];
};

struct ST_SHOP
{
	int				nAction;
	OBJID			idUser;
	OBJID			idItem;
	UINT			uState;
	OBJID			idOwer;
	UINT			uPrice;
	OBJID			idShop;
	UINT			uNumber;
	OBJID			idShopItem;
};

//////////////////////////////////////////////////////////////////////////
//-----修改地图组用户的buffer
struct CHANGE_USER_STATE
{
	OBJID m_userid;
	DWORD m_Stateid;
	DWORD m_power;
	int  m_nSecs; 
    int m_nTimes; 
	int m_nParam1;
	int m_nParam2;
	int m_nParam3;
	bool m_IsAttach;
};

extern BYTE g_cbEncryptKey;	//加密键值

#define ENCRYPTPACKET

#ifdef ENCRYPTPACKET

#define ENCRYPT_KEY 0x3e
inline void EncryptPacket(char* pBuf,int nBufLen)
{
	CHECK(pBuf && nBufLen);
	if(g_cbEncryptKey==0)///加密字节值为0时表示服务器启动时不想加密
		return;
	short nMsgType=0;
	try
	{
		///下面两行是只加密除消息长度外所有数据，包括消息类型
		pBuf+=2;
		nBufLen-=2;
		
		memcpy(&nMsgType,pBuf,sizeof(short));
		if(nMsgType==1002) 
			return;

		for(int i=0;i<nBufLen;++i)
		{
			pBuf[i]^=g_cbEncryptKey;
		}
	}
	catch(...)
	{
		LOGERROR("EncryptPacket 崩溃了,灵异啊!");
	}
}
#else
#define EncryptPacket
#endif

// 分解消息包
// return: netpacket size
inline int	SplitPacket(char* pBuf, int nBufLen, OBJID* pidPacket, char** ppMsg, int* pMsgSize)
{
	if(nBufLen > PROTOCOL_MSG_HEADSIZE)
	{
 		EncryptPacket(pBuf,nBufLen);
		CProtocolMsgStruct*	pMsgPtr = (CProtocolMsgStruct*)pBuf;
		if(pMsgPtr->nMsgSize <= nBufLen)
		{
			if(pMsgPtr->nMsgSize < 4 || pMsgPtr->nMsgSize > MAX_PACKETSIZE)
				return 0;

			*pidPacket	= pMsgPtr->idPacket;
			*pMsgSize	= pMsgPtr->nMsgSize - PROTOCOL_MSG_HEADSIZE;
			*ppMsg		= pMsgPtr->pMsg;
			return pMsgPtr->nMsgSize;
		}
	}
	return 0;
}




// 合成消息包
// return: netpacket size
inline int	UnitePacket(char* pBuf, int nBufLen, OBJID idPacket, const char* pMsg, int nMsgSize, unsigned int& nMsgType)
{
	if(nBufLen >= nMsgSize + PROTOCOL_MSG_HEADSIZE)
	{
		CProtocolMsgStruct*	pMsgPtr = (CProtocolMsgStruct*)pBuf;
		pMsgPtr->idPacket		= (unsigned short)idPacket;
		nMsgType = pMsgPtr->idPacket;
		pMsgPtr->nMsgSize		= nMsgSize + PROTOCOL_MSG_HEADSIZE;
		memcpy(pMsgPtr->pMsg,	pMsg, (size_t)nMsgSize);

		EncryptPacket((char*)pMsgPtr,pMsgPtr->nMsgSize);///对除了包头之外的内容进行异或处理
		return pMsgPtr->nMsgSize;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////
// C/S 协议的NPC部分	

// 该部分常量、定义仅由以下两个函数使用
const int	PROTOCOL_NPCMSG_HEADSIZE		= 8;
struct CProtocolNpcMsgStruct{
	unsigned short	nMsgSize;
	unsigned short	idPacket;
	unsigned long	idNpc;
	char			pMsg[1];
};

// 分解消息包
// return: netpacket size
inline int	SplitNpcPacket(const char* pBuf, int nBufLen, OBJID* pidPacket, char** ppMsg, int* pMsgSize, OBJID* pidNpc)
{
	if(nBufLen > PROTOCOL_NPCMSG_HEADSIZE)
	{
		CProtocolNpcMsgStruct*	pMsgPtr = (CProtocolNpcMsgStruct*)pBuf;
		if(pMsgPtr->nMsgSize <= nBufLen)
		{
			if(pMsgPtr->nMsgSize < 4 || pMsgPtr->nMsgSize > MAX_PACKETSIZE)
				return 0;

			*pidPacket	= pMsgPtr->idPacket;
			*pMsgSize	= pMsgPtr->nMsgSize - PROTOCOL_NPCMSG_HEADSIZE;
			*pidNpc		= pMsgPtr->idNpc;
			*ppMsg		= pMsgPtr->pMsg;
			return pMsgPtr->nMsgSize;
		}
	}
	return 0;
}

// 合成消息包
// return: netpacket size
inline int	UniteNpcPacket(char* pBuf, int nBufLen, OBJID idPacket, const char* pMsg, int nMsgSize, OBJID idNpc)
{
	if(nBufLen >= nMsgSize + PROTOCOL_NPCMSG_HEADSIZE)
	{
		CProtocolNpcMsgStruct*	pMsgPtr = (CProtocolNpcMsgStruct*)pBuf;
		pMsgPtr->idPacket		= (unsigned short)idPacket;
		pMsgPtr->nMsgSize		= nMsgSize + PROTOCOL_NPCMSG_HEADSIZE;
		pMsgPtr->idNpc			= idNpc;
		memcpy(pMsgPtr->pMsg,	pMsg, (size_t)nMsgSize);

		return pMsgPtr->nMsgSize;
	}
	return 0;
}

const int    MALE_ASSIST		=	0x00000001;//男辅助	1  唐门
const int    MALE_MASTER	    =	0x00000002;//男法师	2  全真
const int    MALE_PASTOR	  	=	0x00000004;//男牧师	4  峨眉
const int    MALE_ASSISSIN		=	0x00000008;//男刺客	8  丐帮
const int    MALE_SOLDIER	    =	0x00000010;//男战士	16 少林
const int    MALE_SWORD  		=	0x00000020;//男弓手	32 点仓

const int    FEMALE_ASSIST		=	0x00000040;//女辅助	64
const int    FEMALE_MASTER	    =	0x00000080;//女法师	128
const int    FEMALE_PASTOR  	=	0x00000100;//女牧师	256
const int    FEMALE_ASSISSIN	=	0x00000200;//女刺客	512
const int    FEMALE_SOLDIER		=	0x00000400;//女战士	1024
const int    FEMALE_SWORD		=	0x00000800;//女弓手	2048

const int    MALE_NEWHAND		=	0x00001000;//男新手	4096
const int    FEMALE_NEWHAND	    =	0x00002000;//女新手	8192

const int	PROF_MALE		=	MALE_ASSIST + MALE_MASTER + MALE_PASTOR + MALE_ASSISSIN + MALE_SOLDIER + MALE_SWORD + MALE_NEWHAND;//男
const int	PROF_FEMALE		=	FEMALE_ASSIST + FEMALE_MASTER + FEMALE_PASTOR + FEMALE_ASSISSIN + FEMALE_SOLDIER + FEMALE_SWORD + FEMALE_NEWHAND;//女	


const int	PROF_ASSIST		=	MALE_ASSIST + FEMALE_ASSIST;//辅助
const int	PROF_MASTER	    =	MALE_MASTER + FEMALE_MASTER;//法师
const int	PROF_PASTOR	    =	MALE_PASTOR + FEMALE_PASTOR ;//牧师
const int	PROF_ASSISSIN	=	MALE_ASSISSIN + FEMALE_ASSISSIN;//刺客
const int	PROF_SOLDIER	=	MALE_SOLDIER + FEMALE_SOLDIER;//战士
const int	PROF_SWORD	    =	MALE_SWORD + FEMALE_SWORD ;//弓手
const int	PROF_NEWHAND	=	MALE_NEWHAND + FEMALE_NEWHAND;//新手

const int   ASALE_ITEM_AMOUNT = 2;//每天特价商品种类,要增加时直接改这个变量，最多4个

const int _SPCITEM_VASEXCHANGE	=351001;//元宝票
const int _SPCITEM_EXPEXCHANGE	=351002;//经验丹
#endif // PROTOCOL_H
