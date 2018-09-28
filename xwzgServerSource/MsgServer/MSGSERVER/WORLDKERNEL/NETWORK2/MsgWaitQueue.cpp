// MsgWaitQueue.cpp: implementation of the CMsgWaitQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
using namespace world_kernel;
#include "WorldKernel.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgWaitQueue::CMsgWaitQueue()
{
	Init();
	m_pInfo	=( MSG_info * )m_bufMsg;
}

CMsgWaitQueue::~CMsgWaitQueue()
{

}
BOOL CMsgWaitQueue::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if (!CNetMsg::Create(pbufMsg, dwMsgSize))
		return false;

	if(_MSG_WAITQUEUE != this->GetType())
		return false;

	return true;
}
BOOL CMsgWaitQueue::Create( OBJID idAccount,DWORD dwEndTime,int usAction )
{
	this->Init();

	
	m_pInfo->usAction = usAction;
	m_pInfo->idAccount = idAccount;
	m_pInfo->EndWaitTime= dwEndTime;
	
	m_unMsgSize	=sizeof( MSG_info );	
	m_unMsgType = _MSG_WAITQUEUE;

	return true;
}

BOOL CMsgWaitQueue::Create(OBJID idAccount,int usAction,DWORD curpos,DWORD queuesize)
{
	this->Init();
	
	
	m_pInfo->usAction = usAction;
	m_pInfo->idAccount = idAccount;
	m_pInfo->CurPos = curpos;
	m_pInfo->QueueSize = queuesize;
	
	m_unMsgSize	=sizeof( MSG_info );	
	m_unMsgType = _MSG_WAITQUEUE;
	
	return true;
}

void CMsgWaitQueue::Process( void* pInfo )
{
	switch( m_pInfo->usAction )
	{
	
	case NETWORK_LOGIN:
		{
			//�����յ�LOGIN��Ϣ���޸�Ϊ������ֱ���ж����Ƿ���Ҫ�Ŷӡ��ͻ��˻��ж��Ŷӻ��Ŷӡ�
		}
		break;
	case NETWORK_WAITSUSSECS:
		{
			BOOL FindInQueue = false;
			int  isEndTime  = false;
			//�ͻ����Ŷ���Ϻ�ᷢ�ʹ�ACTION

			int infosize = GameWorld()->WaitQueue.size();
			for (int i = 0;i<infosize;i++)
			{
				if (GameWorld()->WaitQueue[i].idAccount == m_pInfo->idAccount)
				{
					FindInQueue = true;
					isEndTime = GameWorld()->WaitQueue[i].EndWaitTime-GetTickCount()/1000;
					if ( isEndTime <= 0 )
					{
						CMsgWaitQueue msg;
						msg.Create(m_pInfo->idAccount,0,NETWORK_WAITSUSSECS);
						GameWorld()->SendClientMsg( GameWorld()->WaitQueue[i].idSocket,&msg );
						
						CConnectUser* pConnect = GameWorld()->m_setConnectClient->GetObj( m_pInfo->idAccount );
						if( pConnect )
						{

							int tempidAccount  = GameWorld()->WaitQueue[ i ].idAccount;
							int tempidSocket   = GameWorld()->WaitQueue[ i ].idSocket;
							ConnectInfo_ST tempInfo;
							tempInfo.idAccount	= tempidAccount;
							tempInfo.idSocket	= tempidSocket;

							if( !GameWorld()->LoginUserSnap( tempInfo ) )		//halei
							{
								GameWorld()->m_setConnectClient->DelObj( m_pInfo->idAccount );
							}
							pConnect->SetLoginState( JOIN );

							GameWorld()->WaitQueue.erase(GameWorld()->WaitQueue.begin()+i);

						}									
					}
					else
					{
						CMsgWaitQueue msg;			//�Ƿ��Ŷ�
						msg.Create(m_pInfo->idAccount,60,NETWORK_LOGIN);	//���Ҫ������Ŷ�״̬
						GameWorld()->SendClientMsg( GameWorld()->WaitQueue[i].idSocket,&msg );
					}
					break;//XXXXXX
				}
				else
				{
					

				}
				
			}
			
		}
		break;
	default:
		break;
	}
}