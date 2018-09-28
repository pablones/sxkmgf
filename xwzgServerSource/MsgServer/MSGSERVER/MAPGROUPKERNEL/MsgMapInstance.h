// MsgMapInstance.h: interface for the CMsgMapInstance class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGMAPINSTANCE_H__0AB4B13B_916A_444D_BC0F_77F6C29E2035__INCLUDED_)
#define AFX_MSGMAPINSTANCE_H__0AB4B13B_916A_444D_BC0F_77F6C29E2035__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "NetMsg.h"
const UINT _MAX_INSTANCEPLAYER=10;
class CInstanceNPC;

class CMsgMapInstance  : public CNetMsg  
{
public:
	typedef struct
	{
		OBJID	id;
		UINT	nTeamGroup;///分队标识,1,2,3,4,5
		UINT    nLookFace;///玩家头像
		UINT	nLevel;///玩家级别
		UINT	nProfession;///玩家职业
		char	szName[_MAX_NAMESIZE];///玩家姓名
	}InstancePlayerInfo;

	enum INSTANCE_MSG_ACTION
	{
		imaRequestEnterInstance=1,///请求进入副本消息
		imaReplyEnterInstance,///回应进入副本消息
		imaReplyCreateInstance,///回应创建副本消息
		imaRefreshInsatance,///更新副本信息
		imaStartInsatance,///请求服务器开始副本
		imaJoinInsatance,///请求加入副本消息
		imaNotifyInstanceClose,///通知副本关闭消息
		imaChangeTeamGroup,///请求改变玩家所在组
		imaKickOut,///房主请求踢掉某个参与者
		imaChangeOwn,///将房主移交给其它参与者	
		imaChangeRoomTitle,///改变房间标题
		imaChangeRoomPwd,///改变房间密码
		imaCheckRoomPwd,///检查房间密码
		imaExitInsatance,///请求退出副本
		imaCloseRoom,///房主关闭了房间
//		mmaReplyGotoMate,///回应瞬间移动请求,上报自己的地图ID和位置
//		mmaShowCourtshipUI,///要求客户端打开求婚UI界面
//		mmaRequestRefreshMarry,///要求其它地图组的管辖的客户端更新婚姻信息,里面再带一个参数用于标明是什么原因要求更新信息,参数mmaUnmarry表明是因为离婚
//		mmaDelete,///删除邮件
//		///mmaExchange,///取得邮件中道具和金钱,如果邮件类型为交易,则钱物交换
//		mmaNotifyNew,///通知新邮到达
//		mmaGetItem,///取得邮件中的道具
//		mmaGetMoney,///取得邮件中的金钱
//		mmaReturn,///退回邮件,退信是不收邮资的,只需要把邮件记录的收件人和发件人对调即可.
//		mmaError,///请求产生一般错误.
//		mmaError_PlayerNotFound,///请求产生错误,收件人不存在
	};
	CMsgMapInstance(PROCESS_ID idProcess);
	CMsgMapInstance();
	virtual ~CMsgMapInstance();
    void Process(void *pInfo);///处理消息函数
	BOOL CreateRequestEnterMsg(PROCESS_ID idProcess,OBJID idUser,OBJID idOwn,UINT nInstanceType,UINT nInstanceScope,UINT nDomainMapID,UINT Cellx,UINT Celly,const char* FightingTeamName);///生成一个请求进行副本消息
	BOOL CreateReplyEnterMsg(OBJID idUser,OBJID idOwn,OBJID idMap,UINT nMapType,UINT nInstanceScope,int Cellx,int Celly);///生成一个回应进行副本消息
//	BOOL CreateReplyCreateMsg(UINT nMapType,UINT nMapType);///生成一个回应副本消息
	BOOL Create(char* pMsgBuf,DWORD dwSize);
	void CreateReplyCreateMsg(UINT nInstanceType,UINT nMapType);///生成一个回应副本消息
//	void CreateRefreshClientMsg(CInstanceNPC *pNpc);///生成一个回应副本消息
	void CreateRefreshClientMsg(CInstanceNPC *pNpc,UINT nCurrentPage);///生成一个回应副本消息
	void CreateRequestStartMsg(CInstanceNPC *pNpc);///生成一个回应副本消息
	void CreateNotifyInstanceCloseMsg(UINT nInstanceType,OBJID idOwn);///生成一个回应副本消息
	void CreateRequestCloseMsg(CInstanceNPC *pNpc);///生成一个关闭副本消息
private:
	typedef struct tag_MSG_Info
	{
//		unsigned short	unMsgSize;
//		unsigned short	unMsgType;
		UINT		usAction;///操作命令。
		UINT		idOwn;///对方ID,用作通信凭据
		UINT		nInstanceType;//副本类型
//		union 
//		{	
//			///server only
//			struct 
//			{
				UINT		idUser;///对方ID,用作通信凭据
				UINT		idProcess;///进程ID,用作通信回应凭据
				UINT		nParam;///保存请求参数
				UINT		nResult;///保存返回结果
//			};
			///client only
//			struct 
//			{
//					struct 
//					{
						OBJID				idNpc;//副本NpcID.
						UINT				nMapType;///地图类型
						UINT				nPlayerAmount;
						UINT				nCurrentPage;
						UINT				nDomainMapID;		//可占领的地图ID
						UINT				nInstanceScope;		//副本规模
						UINT				nCellx;				//玩家进入副本的落脚点
						UINT				nCelly;				//玩家进入副本的落脚点
						char				szRoomName[_MAX_STRING];///副本名字
						InstancePlayerInfo	setPlayer[_MAX_INSTANCEPLAYER];
						char				FightingTeamName[32];//战队名		
//					};
//			};
//		};
	}MSG_Info;
	MSG_Info*	m_pInfo;
public:
	MSG_Info* GetInfo(){return m_pInfo;}
};

#endif // !defined(AFX_MSGMAPINSTANCE_H__0AB4B13B_916A_444D_BC0F_77F6C29E2035__INCLUDED_)
