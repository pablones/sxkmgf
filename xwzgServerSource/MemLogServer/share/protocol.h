// protocol.h
// �ɽ��ޣ�2002.10.23
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
		// ���紦�����Ϣ
		WORLD_BASE					= 1000,
		WORLD_CLIENTMSG				= WORLD_BASE + 1,				//��������Ŀͻ�����Ϣ
		WORLD_SOCKETMSG				= WORLD_BASE + 2,				//��������Ĵ�����Ϣ
		WORLD_SERVERMSG				= WORLD_BASE + 3,				//���Կͻ����ķ���˶���Ϣ
		WORLD_CLIENTKERMSG			= WORLD_BASE + 4,				//���Կͻ����Ĵ�����Ϣ

		//////////////////////////////////////////////////////////////////////
		//��SOCKET���Ĵ������Ϣ
		SOCKET_BASE					= 2000,
		SOCKET_SOCKETWORLDACTION	= SOCKET_BASE + 1,				// ֪ͨ����ACTION		
		SOCKET_SENDCLIENTMSG		= SOCKET_BASE + 2,				// ֪ͨ����MSG
		
		//����Ǵ������Ϣ
		SHELL_BASE					= 3000,
		SHELL_PRINTTEXT				= SHELL_BASE + 1,				// TEXT��Ϣ
		SHELL_KERNELSTATE			= SHELL_BASE + 2,				// STRING, ���ķ��͵�STATE��������		
		SHELL_REMOTE_CLOSE			= SHELL_BASE + 3,				// Զ�̹ر�

		//��client�������Ϣ
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

//�ͻ��˽��̵��������
struct CLIENTTOWORLDMSG_PACKET0
{
	OBJID	idPacket;
	int		nSize;
	char	buf[1];				// ���ڽṹָ�룬����Ҫ�ռ�
};
struct CLIENTTOWORLDMSG_ACTION
{
	int		nAction;
};

//SOCKET���̵��������
struct SOCKETTOWORLDMSG_PACKET0
{
	OBJID	idPacket;
	OBJID	idSocket;
	int		nSize;
	char	buf[1];				// ���ڽṹָ�룬����Ҫ�ռ�
};
struct SOCKETTOWORLDMSG_ACTION
{
	int		nSocket;
	int		nAction;
};

//������̵��ͻ��˽���
struct WORLDTOCLIENTMSG_PACKET0
{
	OBJID	idPacket;
	int		nSize;
	char	buf[1];				// ���ڽṹָ�룬����Ҫ�ռ�
};
struct WORLDTOCLIENTMSG_ACTION
{
	int		nAction;
};

//������̵�SOCKET����
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


struct SERVERMSG_PACKET0			// ���յ�CLIENT��Ϣ
{
	unsigned short	usMsgSize;
	unsigned short	usMsgType;	
	char	buf[1];				// ���ڽṹָ�룬����Ҫ�ռ�
};

struct CLIENTMSG_PACKET0			// ���յ�CLIENT��Ϣ
{
	SOCKET_ID		idSocket;			// SOCKET_ID
	OBJID	idPacket;
	int		nSize;
	char	buf[1];				// ���ڽṹָ�룬����Ҫ�ռ�
};


struct SENDCLIENTMSG_PACKET0
{
	SOCKET_ID		idSocket;			// SOCKET_ID
	OBJID	idPacket;
	int		nSize;
	char	buf[1];				// ���ڽṹָ�룬����Ҫ�ռ�
};


struct TRANSMSG_PACKET0			// ת�����������̴������Ϣ
{
	SOCKET_ID		idSocket;			// SOCKET_ID
	OBJID			idNpc;				// ͬʱ֧��NPC����Ϣ
	OBJID	idPacket;
	int		nSize;
	int		nTrans;				// ��������
	char	buf[1];				// ���ڽṹָ�룬����Ҫ�ռ�
};

#define _MAX_TEAMMEMBER 6


// �ò��ֳ������������������������ʹ��
const int	PROTOCOL_MSG_HEADSIZE		= 4;
struct CProtocolMsgStruct{
	unsigned short	nMsgSize;
	unsigned short	idPacket;
	char			pMsg[1];
};


// �ֽ���Ϣ��
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




// �ϳ���Ϣ��
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
// C/S Э���NPC����	

// �ò��ֳ������������������������ʹ��
const int	PROTOCOL_NPCMSG_HEADSIZE		= 8;
struct CProtocolNpcMsgStruct{
	unsigned short	nMsgSize;
	unsigned short	idPacket;
	unsigned long	idNpc;
	char			pMsg[1];
};

// �ֽ���Ϣ��
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

// �ϳ���Ϣ��
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

const int    MALE_ASSIST		=	0x00000001;//�и���	1  ����
const int    MALE_MASTER	    =	0x00000002;//�з�ʦ	2  ȫ��
const int    MALE_PASTOR	  	=	0x00000004;//����ʦ	4  ��ü
const int    MALE_ASSISSIN		=	0x00000008;//�д̿�	8  ؤ��
const int    MALE_SOLDIER	    =	0x00000010;//��սʿ	16 ����
const int    MALE_SWORD  		=	0x00000020;//�й���	32 ���

const int    FEMALE_ASSIST		=	0x00000040;//Ů����	64
const int    FEMALE_MASTER	    =	0x00000080;//Ů��ʦ	128
const int    FEMALE_PASTOR  	=	0x00000100;//Ů��ʦ	256
const int    FEMALE_ASSISSIN	=	0x00000200;//Ů�̿�	512
const int    FEMALE_SOLDIER		=	0x00000400;//Ůսʿ	1024
const int    FEMALE_SWORD		=	0x00000800;//Ů����	2048

const int    MALE_NEWHAND		=	0x00001000;//������	4096
const int    FEMALE_NEWHAND	    =	0x00002000;//Ů����	8192

const int	PROF_MALE		=	MALE_ASSIST + MALE_MASTER + MALE_PASTOR + MALE_ASSISSIN + MALE_SOLDIER + MALE_SWORD + MALE_NEWHAND;//��
const int	PROF_FEMALE		=	FEMALE_ASSIST + FEMALE_MASTER + FEMALE_PASTOR + FEMALE_ASSISSIN + FEMALE_SOLDIER + FEMALE_SWORD + FEMALE_NEWHAND;//Ů	


const int	PROF_ASSIST		=	MALE_ASSIST + FEMALE_ASSIST;//����
const int	PROF_MASTER	    =	MALE_MASTER + FEMALE_MASTER;//��ʦ
const int	PROF_PASTOR	    =	MALE_PASTOR + FEMALE_PASTOR ;//��ʦ
const int	PROF_ASSISSIN	=	MALE_ASSISSIN + FEMALE_ASSISSIN;//�̿�
const int	PROF_SOLDIER	=	MALE_SOLDIER + FEMALE_SOLDIER;//սʿ
const int	PROF_SWORD	    =	MALE_SWORD + FEMALE_SWORD ;//����
const int	PROF_NEWHAND	=	MALE_NEWHAND + FEMALE_NEWHAND;//����
#endif // PROTOCOL_H
