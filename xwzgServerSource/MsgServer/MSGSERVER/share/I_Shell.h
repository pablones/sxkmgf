// 外壳接口文件
// 仙剑修，2002.12.13

#ifndef	INTERFACE_SHELL_HEAD_FILE
#define	INTERFACE_SHELL_HEAD_FILE

/////////////////////////////////////////////////////////////////////////////
const int STAT_MAX_MAPGROUP	= 5;			//??? @@@@可能地图组数量不足，只显示这么多
struct STAT_STRUCT{
	char	szStartServer[20];
	char	szCurr[20];

	long	nMaxSocketMS;
	long	nAllSocketMS;
	long	nSocketTimes;
	long	nMaxWorldMS;
	long	nAllWorldMS;
	long	nWorldTimes;
	long	setMaxMapGroupMS[STAT_MAX_MAPGROUP];
	long	setAllMapGroupMS[STAT_MAX_MAPGROUP];
	long	setMapGroupTimes[STAT_MAX_MAPGROUP];
	long	setMapGroupTimerMS[STAT_MAX_MAPGROUP];
	long	setMapGroupMsgSizeOnTimer[STAT_MAX_MAPGROUP];//每个地图组每次onProcess内消息队列
	long	setMapGroupMsgSizeOnTimerProc[STAT_MAX_MAPGROUP];//每个地图组每次onProcess内消息队列实际处理
	long	setMapGroupCrMonTime[STAT_MAX_MAPGROUP];//20071017修罗:每个地图组创建怪物耗时
	long	setMapGroupCrMonCount[STAT_MAX_MAPGROUP];//20071017修罗:每个地图组创建怪物个数

	long	nConnectUserSize;			//connect size
	// kernel
	long	nMaxPlayers;
	long	nAllPlayers;
	long	nLoginPlayers;
	long	nLogoutPlayers;

	long	nSocketSendBytes;			//发送字节数
	long	nSocketRecvBytes;			//接收字节数
	long	nSocketLeaveBytes;			
	long	nSocketRecvPackets;			//收到多少包
	long	nSocketSendPackets;			//发了多少包
	long	nNpcSocketSendBytes;		// npc 发了多少字节
	long	nNpcSocketRecvBytes;		// npc 收了多少字节
	long	nNpcSocketLeaveBytes;
	long	nNpcSocketSendPackets;		// npc 发了多少包
	long	nNpcSocketRecvPackets;		// npc 收了多少包

	long	nPacketID[ STAT_MAX_MAPGROUP ];

	// debug
	long	setDebug[16];
};

/////////////////////////////////////////////////////////////////////////////
class IMessagePort;
class ISocketKernel
{
public:
	virtual ~ISocketKernel() {}
	static ISocketKernel* CreateNew();
	virtual bool	Create(IMessagePort* pPort,int SERVER_LISTEN_PORT,int SERVER_LISTEN_NPC_PORT)				PURE_VIRTUAL_FUNCTION_0
	virtual bool	ProcessMsg(OBJID idPacket, void* buf, int nType, int nFrom)		PURE_VIRTUAL_FUNCTION_0
	virtual bool	ProcessSocket()							PURE_VIRTUAL_FUNCTION_0
	virtual bool	OnTimer(time_t tCurr)					PURE_VIRTUAL_FUNCTION_0
	virtual bool	Release()								PURE_VIRTUAL_FUNCTION_0
};

class IMessagePort;
class IWorld
{
public:
	virtual ~IWorld() {}
	static IWorld* CreateNew();
	virtual bool	Create(IMessagePort* pPort)				PURE_VIRTUAL_FUNCTION_0
	virtual bool	ProcessMsg(OBJID idPacket, void* buf, int nType, int nFrom)		PURE_VIRTUAL_FUNCTION_0
	virtual bool	OnTimer(time_t tCurr)					PURE_VIRTUAL_FUNCTION_0
	virtual bool	Release()								PURE_VIRTUAL_FUNCTION_0
	virtual bool	ProcessSocketMsg()						PURE_VIRTUAL_FUNCTION_0
};

class IMessagePort;
class IMapGroup
{
public:
	virtual ~IMapGroup() {}
	static IMapGroup* CreateNew();
	virtual bool	Create(IMessagePort* pPort)				PURE_VIRTUAL_FUNCTION_0
	virtual void	SynchroData()							PURE_VIRTUAL_FUNCTION
	virtual bool	ProcessMsg(OBJID idPacket, void* buf, int nType, int nFrom)	PURE_VIRTUAL_FUNCTION_0
	virtual bool	OnTimer(time_t tCurr)					PURE_VIRTUAL_FUNCTION_0
	virtual bool	Release()								PURE_VIRTUAL_FUNCTION_0
};

#endif // INTERFACE_SHELL_HEAD_FILE