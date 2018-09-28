// MsgWargame.h: interface for the CMsgWargame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGWARGAME_H__0B8223DA_7D96_430F_9C0D_27BFA4ED4A84__INCLUDED_)
#define AFX_MSGWARGAME_H__0B8223DA_7D96_430F_9C0D_27BFA4ED4A84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NETWORK\NetMsg.h"

// enum	//	ʤ����־
// {
// 	WARGAME_WIN_FLAG_NONE = 0,	//	��Ч
// 	WARGAME_WIN_FLAG_WIN =  1,	//	ʤ��
// 	WARGAME_WIN_FLAG_LOST = 2,	//	ʧ��
// 	WARGAME_WIN_FLAG_DRAW = 3,	//	ƽ��
// };
enum
{
	WARGAME_NONE	=	0,
	WARGAME_LINEBORAD,	//������·�㲥
	WARGAME_SIGNIN,		//����
	WARGAME_SCORERANK,	//���ְ�
	WARGAME_ENTER,		//֪ͨ�ͻ��˿��Խ���
	WARGAME_CAMPSCORE,	//����Ӫ����
};

enum
{
	WARGAMELINEBORAD_NONE	=	0,
	WARGAMELINEBORAD_LOADMAP,
	WARGAMELINEBORAD_START,		//֪ͨ��������ר��֪ͨ
	WARGAMELINEBORAD_SIGNIN,	//����
	WARGAMELINEBORAD_ENTER,		//����
	WARGAMELINEBORAD_LEAVE		//����
};

typedef struct  
{
	OBJID	idPlayer;
	USHORT	usRank;		//����
	USHORT	usLev;		//�ȼ�
	int		nScore;		//��ǰ����
	int		nAwardScore;//��������
	USHORT	usKilled;	//ɱ����
	UCHAR	ucCamp;		//��Ӫ
	UCHAR	ucVipLev;	//vip�ȼ�
	DWORD	dwProfession;//ְҵ
	NAMESTR	szName;		//����
}WARGAME_PLAYERSCORE;	//	ͳ����Ŀ

class CMsgWargame : public CNetMsg  
{
public:
	CMsgWargame();
	virtual ~CMsgWargame();

	BOOL Create(char* pbufMsg, DWORD dwMsgSize);
	BOOL Create(OBJID idUser, USHORT usAction, UCHAR ucPage);

	BOOL Append(WARGAME_PLAYERSCORE info);
	BOOL Append(OBJID idUser, USHORT usLev, int nScore, UCHAR ucCamp, UCHAR ucVipLev, DWORD dwProfession, LPCTSTR szName);

	void Process(void* pInfo);
	void ProcessLineMsg(void *pInfo);

	int  GetAmount()	{ return m_pInfo->ucAmount; }

public:
	typedef struct {
		MSGHEAD_DEFINE

		OBJID	idUser;
		USHORT	usAction;
		UCHAR	ucPage;
		UCHAR	ucAmount;
		WARGAME_PLAYERSCORE	stUserList[1];
	}MSG_Info;

	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGWARGAME_H__0B8223DA_7D96_430F_9C0D_27BFA4ED4A84__INCLUDED_)
