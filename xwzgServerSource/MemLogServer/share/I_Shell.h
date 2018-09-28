// ��ǽӿ��ļ�
// �ɽ��ޣ�2002.12.13

#ifndef	INTERFACE_SHELL_HEAD_FILE
#define	INTERFACE_SHELL_HEAD_FILE

/////////////////////////////////////////////////////////////////////////////
const int STAT_MAX_MAPGROUP	= 5;			//??? @@@@���ܵ�ͼ���������㣬ֻ��ʾ��ô��
struct STAT_STRUCT{
	//ʱ��
	char	szStartServer[20];  //��ʼʱ��
	char	szCurr[20];			//��ǰʱ��
	
	long	nMaxPlayers;
	long	nAllPlayers;
	
	//�ڲ���Ϣ client
	long	nMaxSocketMS;
	long	nAllSocketMS;
	long	nMaxSocketProcessSize;
	long	nMaxSocketSize;
	
	long	nMaxClientMS;
	long	nAllClientMS;
	long	nMaxClientProcessSize;
	long	nMaxClientSize;
	
	long	nMaxWorldMS;
	long	nAllWorldMS;
	long	nMaxWorldProcessSize;
	long	nMaxWorldSize;
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
};

#endif // INTERFACE_SHELL_HEAD_FILE