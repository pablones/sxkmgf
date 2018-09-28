// MsgRelay.h: interface for the CMsgRelay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGRELAY_H__94B6EC05_0B53_4CBC_B0B8_3C0579419356__INCLUDED_)
#define AFX_MSGRELAY_H__94B6EC05_0B53_4CBC_B0B8_3C0579419356__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "NetMsg.h"

enum MsgRelayAction
{
	raNone,
	raRoleOnline,///�������
	raRoleOffline,///�������
	raQueryRole_Friend,///��ѯ��Һ���
	raQueryRole_Syndicate,///��ѯ��Ҿ���
	raQueryRole_Marry,///��ѯ��һ���
};

///��Ϸ���������ϵ��������ͨר����Ϣ��Ӧ���ʹ��ͬһ��Ϣ����С�����ģ
class CMsgRelay : public CNetMsg  
{
public:
	CMsgRelay();
	virtual ~CMsgRelay();
	
public:	
	BOOL Create(char* pMsgBuf,DWORD dwSize);///���
	
	//[2010-07-26 goto]
	typedef struct tag_MSG_Info
	{
		unsigned short	unMsgSize;
		unsigned short	unMsgType;
	
		USHORT			usAction;///��Ϣ�ڲ�����,ƥ�� MsgRelayAction
		OBJID			idRole;///��ɫID
		OBJID			dstRoleID;///��ѯĿ���ɫID

		union
		{
			char	szUserName[_MAX_NAMESIZE];///��ɫ��,��¼ʱʹ��
			char	szServerName[_MAX_NAMESIZE];///��·������ѯ��������ɫʱ���أ������ɫ������			
		};

		///�������ݣ��ݲ�ʹ��
		USHORT			nRoleLevel;///��ҵ�ǰ�ȼ�
		USHORT			nRoleMapID;///������ڵ�ͼID

	}MSG_Info;
	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGRELAY_H__94B6EC05_0B53_4CBC_B0B8_3C0579419356__INCLUDED_)
