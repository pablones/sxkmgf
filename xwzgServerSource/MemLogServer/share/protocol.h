// protocol.h
// 仙剑修，2002.10.23
//////////////////////////////////////////////////////////////////////

#ifndef	PROTOCOL_H
#define	PROTOCOL_H

#include <string.h>
#include "define.h"
#include <vector>
#ifdef MAPGROUP_KERNEL
//#include "UserData.h"
#endif


enum{
		//////////////////////////////////////////////////////////////////////
		// 世界处理的消息
		WORLD_BASE					= 1000,
		WORLD_CLIENTMSG				= WORLD_BASE + 1,				//来自网络的客户端消息
		WORLD_SOCKETMSG				= WORLD_BASE + 2,				//来自网络的处理消息
		WORLD_SERVERMSG				= WORLD_BASE + 3,				//来自客户网的服务端端消息
		WORLD_CLIENTKERMSG			= WORLD_BASE + 4,				//来自客户网的处理消息

		//////////////////////////////////////////////////////////////////////
		//由SOCKET核心处理的消息
		SOCKET_BASE					= 2000,
		SOCKET_SOCKETWORLDACTION	= SOCKET_BASE + 1,				// 通知网络ACTION		
		SOCKET_SENDCLIENTMSG		= SOCKET_BASE + 2,				// 通知网络MSG
		
		//由外壳处理的消息
		SHELL_BASE					= 3000,
		SHELL_PRINTTEXT				= SHELL_BASE + 1,				// TEXT消息
		SHELL_KERNELSTATE			= SHELL_BASE + 2,				// STRING, 核心发送的STATE串，多行		
		SHELL_REMOTE_CLOSE			= SHELL_BASE + 3,				// 远程关闭

		//由client处理的消息
		CLIENT_BASE					= 4000,
		CLIENT_SENDSERVERMSG		= CLIENT_BASE + 1,
		CLIENT_WORLDMSG				= CLIENT_BASE + 2,

		ALL_END						= 5000,
};

enum{
	CLIENTTOWORLDACTION_NONE = 0,
	CLIENTTOWORLDACTION_CREATEOK,
};

enum{
	SOCKETTOWORLDACTION_NONE = 0,
	SOCKETTOWORLDACTION_CLOSE,
};

enum{
	WORLDTOSOCKETACTION_NONE = 0,
	WORLDTOSOCKETACTION_CLOSED,
	WORLDTOSOCKETACTION_CLICK,
	WORLDTOSOCKETACTION_ALL,
};

enum{
	WORLDTOCLIENTACTION_NONE = 0,
	WORLDTOCLIENTACTION_CONNECT,
};

//客户端进程到世界进程
struct CLIENTTOWORLDMSG_PACKET0
{
	OBJID	idPacket;
	int		nSize;
	char	buf[1];				// 用于结构指针，不需要空间
};
struct CLIENTTOWORLDMSG_ACTION
{
	int		nAction;
};

//SOCKET进程到世界进程
struct SOCKETTOWORLDMSG_PACKET0
{
	OBJID	idPacket;
	OBJID	idSocket;
	int		nSize;
	char	buf[1];				// 用于结构指针，不需要空间
};
struct SOCKETTOWORLDMSG_ACTION
{
	int		nSocket;
	int		nAction;
};

//世界进程到客户端进程
struct WORLDTOCLIENTMSG_PACKET0
{
	OBJID	idPacket;
	int		nSize;
	char	buf[1];				// 用于结构指针，不需要空间
};
struct WORLDTOCLIENTMSG_ACTION
{
	int		nAction;
};

//世界进程到SOCKET进程
struct WORLDTOSOCKETMSG_PACKET0
{
	OBJID		idPacket;
	SOCKET_ID	idSocket;
	int			nSize;
	char		buf[1];
};
struct WORLDTOSOCKETMSG_ACTION
{
	int		nAction;
};


struct SERVERMSG_PACKET0			// 接收的CLIENT消息
{
	unsigned short	usMsgSize;
	unsigned short	usMsgType;	
	char	buf[1];				// 用于结构指针，不需要空间
};

struct CLIENTMSG_PACKET0			// 接收的CLIENT消息
{
	SOCKET_ID		idSocket;			// SOCKET_ID
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


struct TRANSMSG_PACKET0			// 转发给其它进程处理的消息
{
	SOCKET_ID		idSocket;			// SOCKET_ID
	OBJID			idNpc;				// 同时支持NPC的消息
	OBJID	idPacket;
	int		nSize;
	int		nTrans;				// 附加数据
	char	buf[1];				// 用于结构指针，不需要空间
};

#define _MAX_TEAMMEMBER 6


// 该部分常量、定义仅由以下两个函数使用
const int	PROTOCOL_MSG_HEADSIZE		= 4;
struct CProtocolMsgStruct{
	unsigned short	nMsgSize;
	unsigned short	idPacket;
	char			pMsg[1];
};


// 分解消息包
// return: netpacket size
inline int	SplitPacket(char* pBuf, int nBufLen, OBJID* pidPacket, char** ppMsg, int* pMsgSize)
{
	if(nBufLen > PROTOCOL_MSG_HEADSIZE)
	{
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
#endif // PROTOCOL_H
