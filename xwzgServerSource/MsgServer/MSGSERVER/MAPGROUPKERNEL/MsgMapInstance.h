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
		UINT	nTeamGroup;///�ֶӱ�ʶ,1,2,3,4,5
		UINT    nLookFace;///���ͷ��
		UINT	nLevel;///��Ҽ���
		UINT	nProfession;///���ְҵ
		char	szName[_MAX_NAMESIZE];///�������
	}InstancePlayerInfo;

	enum INSTANCE_MSG_ACTION
	{
		imaRequestEnterInstance=1,///������븱����Ϣ
		imaReplyEnterInstance,///��Ӧ���븱����Ϣ
		imaReplyCreateInstance,///��Ӧ����������Ϣ
		imaRefreshInsatance,///���¸�����Ϣ
		imaStartInsatance,///�����������ʼ����
		imaJoinInsatance,///������븱����Ϣ
		imaNotifyInstanceClose,///֪ͨ�����ر���Ϣ
		imaChangeTeamGroup,///����ı����������
		imaKickOut,///���������ߵ�ĳ��������
		imaChangeOwn,///�������ƽ�������������	
		imaChangeRoomTitle,///�ı䷿�����
		imaChangeRoomPwd,///�ı䷿������
		imaCheckRoomPwd,///��鷿������
		imaExitInsatance,///�����˳�����
		imaCloseRoom,///�����ر��˷���
//		mmaReplyGotoMate,///��Ӧ˲���ƶ�����,�ϱ��Լ��ĵ�ͼID��λ��
//		mmaShowCourtshipUI,///Ҫ��ͻ��˴����UI����
//		mmaRequestRefreshMarry,///Ҫ��������ͼ��Ĺ�Ͻ�Ŀͻ��˸��»�����Ϣ,�����ٴ�һ���������ڱ�����ʲôԭ��Ҫ�������Ϣ,����mmaUnmarry��������Ϊ���
//		mmaDelete,///ɾ���ʼ�
//		///mmaExchange,///ȡ���ʼ��е��ߺͽ�Ǯ,����ʼ�����Ϊ����,��Ǯ�ｻ��
//		mmaNotifyNew,///֪ͨ���ʵ���
//		mmaGetItem,///ȡ���ʼ��еĵ���
//		mmaGetMoney,///ȡ���ʼ��еĽ�Ǯ
//		mmaReturn,///�˻��ʼ�,�����ǲ������ʵ�,ֻ��Ҫ���ʼ���¼���ռ��˺ͷ����˶Ե�����.
//		mmaError,///�������һ�����.
//		mmaError_PlayerNotFound,///�����������,�ռ��˲�����
	};
	CMsgMapInstance(PROCESS_ID idProcess);
	CMsgMapInstance();
	virtual ~CMsgMapInstance();
    void Process(void *pInfo);///������Ϣ����
	BOOL CreateRequestEnterMsg(PROCESS_ID idProcess,OBJID idUser,OBJID idOwn,UINT nInstanceType,UINT nInstanceScope,UINT nDomainMapID,UINT Cellx,UINT Celly,const char* FightingTeamName);///����һ��������и�����Ϣ
	BOOL CreateReplyEnterMsg(OBJID idUser,OBJID idOwn,OBJID idMap,UINT nMapType,UINT nInstanceScope,int Cellx,int Celly);///����һ����Ӧ���и�����Ϣ
//	BOOL CreateReplyCreateMsg(UINT nMapType,UINT nMapType);///����һ����Ӧ������Ϣ
	BOOL Create(char* pMsgBuf,DWORD dwSize);
	void CreateReplyCreateMsg(UINT nInstanceType,UINT nMapType);///����һ����Ӧ������Ϣ
//	void CreateRefreshClientMsg(CInstanceNPC *pNpc);///����һ����Ӧ������Ϣ
	void CreateRefreshClientMsg(CInstanceNPC *pNpc,UINT nCurrentPage);///����һ����Ӧ������Ϣ
	void CreateRequestStartMsg(CInstanceNPC *pNpc);///����һ����Ӧ������Ϣ
	void CreateNotifyInstanceCloseMsg(UINT nInstanceType,OBJID idOwn);///����һ����Ӧ������Ϣ
	void CreateRequestCloseMsg(CInstanceNPC *pNpc);///����һ���رո�����Ϣ
private:
	typedef struct tag_MSG_Info
	{
//		unsigned short	unMsgSize;
//		unsigned short	unMsgType;
		UINT		usAction;///�������
		UINT		idOwn;///�Է�ID,����ͨ��ƾ��
		UINT		nInstanceType;//��������
//		union 
//		{	
//			///server only
//			struct 
//			{
				UINT		idUser;///�Է�ID,����ͨ��ƾ��
				UINT		idProcess;///����ID,����ͨ�Ż�Ӧƾ��
				UINT		nParam;///�����������
				UINT		nResult;///���淵�ؽ��
//			};
			///client only
//			struct 
//			{
//					struct 
//					{
						OBJID				idNpc;//����NpcID.
						UINT				nMapType;///��ͼ����
						UINT				nPlayerAmount;
						UINT				nCurrentPage;
						UINT				nDomainMapID;		//��ռ��ĵ�ͼID
						UINT				nInstanceScope;		//������ģ
						UINT				nCellx;				//��ҽ��븱������ŵ�
						UINT				nCelly;				//��ҽ��븱������ŵ�
						char				szRoomName[_MAX_STRING];///��������
						InstancePlayerInfo	setPlayer[_MAX_INSTANCEPLAYER];
						char				FightingTeamName[32];//ս����		
//					};
//			};
//		};
	}MSG_Info;
	MSG_Info*	m_pInfo;
public:
	MSG_Info* GetInfo(){return m_pInfo;}
};

#endif // !defined(AFX_MSGMAPINSTANCE_H__0AB4B13B_916A_444D_BC0F_77F6C29E2035__INCLUDED_)
