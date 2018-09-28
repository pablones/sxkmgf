// MsgFriend.h: interface for the CMsgGoodFriend class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGFRIEND_H__DC85CF8B_3B54_4C8D_94EA_F68172C0A179__INCLUDED_)
#define AFX_MSGFRIEND_H__DC85CF8B_3B54_4C8D_94EA_F68172C0A179__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// include head file(s) hrere...
#include "NetMsg.h"

WORLDKERNEL_BEGIN
// define constant(s) here...
//--------------------------------------
//����������[2008.12.12]
const int _FRIEND_ADDFRIEND					= 1;//������Ϊ���� 
//const int _FRIEND_MOVETOGOODFRIENDAPPLY		= 1;//������Ϊ���� 
const int _FRIEND_MOVETOGOODFRIENDACCEPT	= 2;//ͬ����Ϊ����
const int _FRIEND_MOVEAPPLY					= 3;//�����ƶ�����(���ܰѷ�������Ϊ����)
const int _FRIEND_MOVESUCESS				= 4;//�ƶ����ѳɹ�(���ܰѷ�������Ϊ����)

const int _FRIEND_CONTACTHISTORY_APPLY		= 5;//���뽻����ϵ
const int _FRIEND_CONTACTHISTORY_GETINFO	= 6;//��ȡ������ʷ�ĺ�����Ϣ
const int _FRIEND_CONTACTHISTORY_ONLINE		= 7;//������ϵ��������
const int _FRIEND_CONTACTHISTORY_OFFLINE	= 8;//������ϵ��������

const int _FRIEND_ADDTEMP					= 9;//��Ϊ��ʱ����,����Է�ȷ��
//--------------------------------------
const int _FRIEND_APPLY			= 10;
const int _FRIEND_ACCEPT		= 11;
const int _FRIEND_ONLINE		= 12;
const int _FRIEND_OFFLINE		= 13;
const int _FRIEND_BREAK			= 14;
const int _FRIEND_GETINFO		= 15;
const int _ENEMY_ONLINE			= 16;			// to client // ���µ�����״̬ 
const int _ENEMY_OFFLINE		= 17;			// to client // ���µ�����״̬ 
const int _ENEMY_DEL			= 18;			// to server // to client // ɾ��һ������ 
const int _ENEMY_ADD			= 19;			// to client // ���һ������ 

//[2010-06-07 goto]
const int _FRIEND_ADDFAIL		= 23; //��Ӻ���ʧ�� to client
const int _FRIEND_ADDSUCCESS	= 24; //��Ӻ��ѳɹ� to client
const int _FRIEND_CHANGEGROUP	= 25; //�޸ĺ��ѷ��� to client //to server
const int _FRIEND_CHANGEFAIL	= 26; //�޸ķ���ʧ�� to client
const int _FRIEND_DELFRIEND		= 27; //ɾ������	 to client //to server
const int _FRIEND_DELFRIENDFAIL	= 28; //ɾ������ʧ�� to client
const int _FRIEND_SETFEEL		= 29; //�޸�����	 to client //to server
const int _FRIEND_SETFEELFAIL	= 30; //�޸�����ʧ�� to client
const int _FRIEND_BESETFEEL		= 31; //�����޸�����	 to client
const int _FRIEND_GETFRIENDINFO	= 32; //��ȡ���ѵ���ϸ��Ϣ	 to client //to server
const int _FRIEND_DELCONTACT	= 33; //ɾ���Ѿ�����������	 to server
const int _FRIEND_HAVETEAM		= 34; //�����Ƿ��ڶ�����	 to client //to server
const int _FRIEND_FEEL_SWITH	= 35; //���鿪��
const int _FRIEND_QUERYFRIENDINFO = 36;
const int _FRIEND_REPLYFRIENDINFO = 37;///��Ӧ��ѯ��ҵ���ϸ��Ϣ
const int _FRIEND_ADDSUCCESSREPLY	= 38; //��Ӻ��ѳɹ�����֤�����Ƿ�������Ϣ to client
const int _FRIEND_ADDSUCCESS_PASSIVE =39;
const int _FRIEND_HAVETEAM_MT	= 101;//������ͼ�鷢������Ϣ

class CMsgFriend : public CNetMsg
{
public:
	CMsgFriend();
	virtual ~CMsgFriend();

	enum { OFFLINE_STATUS, ONLINE_STATUS };
	//BOOL	Create	(int nAction, OBJID idFriend, LPCTSTR szName, bool bOnline = false, DWORD nLookFace=0, int nLevel=0,int nFriendType = 1);
	BOOL	Create	(int nAction, 
		OBJID idFriend, 
		LPCTSTR szName, 
		short bOnline = 0, 
		DWORD nLookFace=0, 
		int nLevel=0,
		unsigned int profession/*ְҵ*/=0,
		unsigned int friendship/*�����*/=0,
		unsigned int nSex = 0,
		unsigned int nFriendType = 1,
		LPCTSTR szFeel=NULL,//[2010-06-05 goto]���Ӻ�������
		int nRelation=0,//��ϵ
		LPCTSTR szFaction=NULL,//����
		LPCTSTR szTitle=NULL,//�ƺ�
		LPCTSTR szMateName=NULL,//��ż
		LPCTSTR szTeamName=NULL,//����
		OBJID idMap=0//λ��
		);

public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void* pInfo);

//private:
	typedef struct
	{
		MSGHEAD_DEFINE

		OBJID			idFriend;	
		DWORD			dwLookFace;
		unsigned short	ucAction;
		unsigned short	ucOnlineFlag;
		unsigned short	ucLevel;
		unsigned short  ucRelation;//��ϵ,ʦͽ������...
		unsigned int	ucFriendship;//20061012����.�����.Ԥ��
		unsigned int	ucProfession;//20061012����.ְҵ
		unsigned int    ucSex;
		unsigned int	ucFriendType;
		//union{
		char			szName[_MAX_NAMESIZE];				//@ ���Ż���ֻ�޸�����״̬ʱ�����������ݡ�
		//OBJID			idTransmit;				// use for transmit
		//};
		char	szBuf[1];//[2010-06-05 goto]���Ӻ�������	
	}MSG_Info;

	MSG_Info*	m_pInfo;

private:
	CNetStringPacker	m_StrPacker;
};
WORLDKERNEL_END

#endif // !defined(AFX_MSGFRIEND_H__DC85CF8B_3B54_4C8D_94EA_F68172C0A179__INCLUDED_)
