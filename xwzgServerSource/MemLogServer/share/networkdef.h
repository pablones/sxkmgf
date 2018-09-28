#ifndef _NETWORKDEF_H
#define _NETWORKDEF_H

#include "define.h"

// max size define
const int _MAX_MSGSIZE			= 1024 ;
const int _MAX_WORDSSIZE		= 256  ;
const int _MAX_SPEAK_WORDSSIZE  = 512  ;
const int _MAX_IDLETIME			= 600  ;	// 600s idle time

typedef	char	WORDSSTR[_MAX_WORDSSIZE];

// msg type define
const unsigned short _MSG_NONE					=0;
//client
const unsigned short _MSG_CLIENT_NONE			=10000;
const unsigned short _MSG_CLIENT_LOGIN			=_MSG_CLIENT_NONE+1;
const unsigned short _MSG_CLIENT_RELOGIN		=_MSG_CLIENT_NONE+2;


const unsigned short _MSG_SERVER_NONE			=20000;
const unsigned short _MSG_SERVER_LOGIN			=_MSG_SERVER_NONE+1;
const unsigned short _MSG_SERVER_RELOGIN		=_MSG_SERVER_NONE+2;
const unsigned short _MSG_SERVER_STATUS			=_MSG_SERVER_NONE+3;
const unsigned short _MSG_SERVER_POST			=_MSG_SERVER_NONE+4;
const unsigned short _MSG_SERVER_ACTION			=_MSG_SERVER_NONE+5;

const unsigned short _MSG_SERVER_DATAUSER		=_MSG_SERVER_NONE+1005;
const unsigned short _MSG_SERVER_DATAACC		=_MSG_SERVER_NONE+1006;

// color define
const unsigned long	_COLOR_WHITE	=0x00ffffff;

// msg style define
const unsigned short _TXT_NORMAL	=0;
const unsigned short _TXT_SCROLL	=0x0001;
const unsigned short _TXT_FLASH		=0x0002;
const unsigned short _TXT_BLAST		=0x0008;

// msg attribute define
const unsigned short _TXTATR_NORMAL		=2000;
const unsigned short _TXTATR_PRIVATE	=_TXTATR_NORMAL+1;	// 私聊
const unsigned short _TXTATR_ACTION		=_TXTATR_NORMAL+2;	// 动作
const unsigned short _TXTATR_TEAM		=_TXTATR_NORMAL+3;	// 队伍
const unsigned short _TXTATR_SYNDICATE	=_TXTATR_NORMAL+4;	// 帮派
const unsigned short _TXTATR_SYSTEM		=_TXTATR_NORMAL+5;	// 系统
const unsigned short _TXTATR_FAMILY		=_TXTATR_NORMAL+6;	// 家庭
const unsigned short _TXTATR_TALK		=_TXTATR_NORMAL+7;	// 交谈
const unsigned short _TXTATR_YELP 		=_TXTATR_NORMAL+8;	// 叫喊
const unsigned short _TXTATR_FRIEND 	=_TXTATR_NORMAL+9;	// 朋友
const unsigned short _TXTATR_GLOBAL 	=_TXTATR_NORMAL+10;	// 千里传音
const unsigned short _TXTATR_IMPORTINFO =_TXTATR_NORMAL+11;	// 重要信息,相当于以前的GM频道
const unsigned short _TXTATR_WHISPER 	=_TXTATR_NORMAL+12;	// 耳语
const unsigned short _TXTATR_GHOST	 	=_TXTATR_NORMAL+13;	// 幽灵
const unsigned short _TXTATR_MAP        =_TXTATR_NORMAL+13;//========add by cz 加入地图频道，整个地图上的人都可以看到

const unsigned short _TXTATR_SERVE		=_TXTATR_NORMAL+14; // 服务
const unsigned short _TXTATR_WORLD		=_TXTATR_NORMAL+16;//  [6/21/2007 Add By HaLei]  小喇叭
const unsigned short _TXTATR_PK			=_TXTATR_NORMAL+17;//  [8/27/2007 Add By HaLei] PK信息
const unsigned short _TXTATR_DEF		=_TXTATR_NORMAL+18;// 本地防务 halei [10/27/2007]
const unsigned short _TXTATR_CHATWITH	=_TXTATR_NORMAL+19;//独孤求败[2008/12/23]1:1聊天
const unsigned short _TXTATR_SERVEEX	=_TXTATR_NORMAL+20; // [游途道标 2009.04.29]扩展服务频道
const unsigned short _TXTATR_HOUSE		=_TXTATR_NORMAL+23;
const unsigned short _TXTATR_MYSYSTEM   =_TXTATR_NORMAL+24;

const unsigned short _TXTATR_WORLDSERVER		=_TXTATR_NORMAL+30;//小喇叭
const unsigned short _TXTATR_BOOTHNAME			=_TXTATR_NORMAL+31;//摊位名字
const unsigned short _TXTATR_BOOTHDES			=_TXTATR_NORMAL+32;//摊位信息
const unsigned short _TXTATR_BOOTHLEAVEWORD		=_TXTATR_NORMAL+33;//摊位留言
const unsigned short _TXTATR_TOPSHOW			=_TXTATR_NORMAL+34;//top show
const unsigned short _TXTATR_ANNOUNCE			=_TXTATR_NORMAL+35;//系统公告
const unsigned short _TXTATR_TOPANNOUNCE		=_TXTATR_NORMAL+36;//滚屏公告
const unsigned short _TXTATR_HELP				=_TXTATR_NORMAL+37;//帮助
const unsigned short _TXTATR_FATI				=_TXTATR_NORMAL+38;//防沉迷
const unsigned short _TXTATR_BIGHORN			=_TXTATR_NORMAL+39;//大喇叭

const unsigned short _TXTATR_MAYBEMAIL			=_TXTATR_NORMAL+40;
const unsigned short _TXTATR_PRO				=_TXTATR_NORMAL+41; 
const unsigned short _TXTATR_SECPRO				=_TXTATR_NORMAL+42; 

const unsigned short _TXTATR_PRIVATE_OTHERLINE	=_TXTATR_NORMAL+43;//其他线传过来的私聊消息
const unsigned short _TXTATR_FORGMTOOL			=_TXTATR_NORMAL+44;//GM工具用




//------add by cz-----------
const char strLeoTalk[]="LeoTalk";//--------如果_TXTATR_WORLD消息的接收者是strLeoTalk表示这是一个狮子吼否则是一个世界消息

const unsigned short _TXTATR_REGISTER	=_TXTATR_NORMAL+100;
const unsigned short _TXTATR_ENTRANCE	=_TXTATR_NORMAL+101;
const unsigned short _TXTATR_SHOP		=_TXTATR_NORMAL+102;
const unsigned short _TXTATR_PETTALK	=_TXTATR_NORMAL+103;
const unsigned short _TXTATR_CRYOUT		=_TXTATR_NORMAL+104;
const unsigned short _TXTATR_WEBPAGE	=_TXTATR_NORMAL+105;

const unsigned short _TXTATR_NEWMESSAGE	=_TXTATR_NORMAL+106;
const unsigned short _TXTATR_TASK		=_TXTATR_NORMAL+107;
const unsigned short _TXTATR_SYNWAR_FIRST	=_TXTATR_NORMAL+108;	// 帮派战绩
const unsigned short _TXTATR_SYNWAR_NEXT	=_TXTATR_NORMAL+109;	// 帮派战绩

const unsigned short _TXTATR_LEAVEWORD		=_TXTATR_NORMAL+110;	// 留言系统
const unsigned short _TXTATR_SYNANNOUNCE	=_TXTATR_NORMAL+111;	// 帮派公告
const unsigned short _TXTATR_MESSAGEBOX		=_TXTATR_NORMAL+112;	// 对话框
const unsigned short _TXTATR_REJECT			=_TXTATR_NORMAL+113;	// 驳回
const unsigned short _TXTATR_SYNTENET		=_TXTATR_NORMAL+114;	// 帮派宗旨
const unsigned short _TXTATR_USERSNAP		=_TXTATR_NORMAL+115;	//李京哲0307 玩家登陆时的简单信息
const unsigned short _TXTATR_ERRORMESSAGEBOX=_TXTATR_NORMAL+116;	// 20090410haocq错误操作提示对话框

const unsigned short _TXTATR_MSG_TRADE	=_TXTATR_NORMAL+201;	// 交易留言板
const unsigned short _TXTATR_MSG_FRIEND	=_TXTATR_NORMAL+202;	// 交友留言板
const unsigned short _TXTATR_MSG_TEAM	=_TXTATR_NORMAL+203;	// 组队留言板
const unsigned short _TXTATR_MSG_SYN	=_TXTATR_NORMAL+204;	// 帮派留言板 
const unsigned short _TXTATR_MSG_OTHER	=_TXTATR_NORMAL+205;	// 其它留言板
const unsigned short _TXTATR_MSG_SYSTEM	=_TXTATR_NORMAL+206;	// 其它留言板
const unsigned short _TXTATR_MSG_ABOUTLOGIN	= _TXTATR_NORMAL + 207;//在开始游戏时用到..
const unsigned short _TXTATR_MSG_SERVERLIST	= _TXTATR_NORMAL + 208;//在开始游戏时用到..
const unsigned short _TXTATR_DIALOG_BEGIN	=_TXTATR_NORMAL+500;	// 对话框第一个
// ... 中间的保留用于通知客户端打开界面
const unsigned short _TXTATR_DIALOG_END		=_TXTATR_NORMAL+600;	// 对话框最后一个 + 1
#endif
