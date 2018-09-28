// MsgIssue.h: interface for the CMsgIssue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGISSUE_H__06764C89_F374_42A1_BD8A_FDED275BE356__INCLUDED_)
#define AFX_MSGISSUE_H__06764C89_F374_42A1_BD8A_FDED275BE356__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// #include "winsock.h"
#include "NetMsg.h"
WORLDKERNEL_BEGIN

const int _MAX_ISSUETITLESIZE =100;///50���ֱ���
const int _MAX_ISSUETEXTSIZE =800;///400��������

enum IssueType
{
	itNone=-1,///δָ��
	itRealtime,///��ʱҪ��
	itItemReplicate,///���߸���
	itValueExplode,///����ˢ��ֵ
	itAssware,///���
	itServerUnstead,///������״̬�쳣
	itValueExcept,///�ؼ������쳣
	itBug,///�ش�Bug(��Ϸ�ж�)
	itOther,///�����ش�����쳣
	it1v1chat,///��������һ��
};

enum 
{
	ISSUEACT_NONE=-1, 
	ISSUEACT_SENDISSUE,				// �ͻ��˷�������
	ISSUEACT_REQUESTREPLYCHAT,		// GM�������1V1�������Ӧ��,��Result����ʾ���
 	ISSUEACT_SYSMAIL,				// ����ϵͳר�õ�ϵͳ�ʼ�
 	ISSUEACT_CHATTEXT, // �����������װ��// id is user id
};

class CMsgIssue : public world_kernel::CNetMsg  
{
public:
	CMsgIssue();
	virtual ~CMsgIssue();

	///�������
	//BOOL	Create	(const char* pszAccount, const char* pszPsw, const char* pszServer);
public:	
	BOOL	SendIssue(LPCTSTR strPlayerName,UINT nPlayerID,LPCTSTR strTitle,IssueType itType,LPCTSTR strText);

	///�������
	BOOL	Create		(char* pMsgBuf, DWORD dwSize);
	void	Process		(void* pInfo);

protected:
	BOOL	CreateIssue(LPCTSTR strPlayerName,UINT nPlayerID,LPCTSTR strTitle,IssueType itType,LPCTSTR strText);

	typedef struct {
// 		unsigned short	unMsgSize;
// 		unsigned short	unMsgType;
		UINT		usAction;			//��Ϣ����,�������,��Ӧ�Ӵ�,����
		UINT		usPlayerID;			//���playerid
		UINT		usRequestID;		///������id
		UINT		usType;				//��������
		UINT		usResult;			//������
		union
		{
			struct
			{
				char		usTitle[_MAX_ISSUETITLESIZE];			//����
				char		usText[_MAX_ISSUETEXTSIZE];			//����
				char		usPlayerName[_MAX_NAMESIZE];	//�����
			};
			char	usChatText[_MAX_ISSUETITLESIZE+_MAX_ISSUETEXTSIZE+_MAX_NAMESIZE];	//�����
		};
	}MSG_Info;

	MSG_Info*	m_pInfo;

};

WORLDKERNEL_END
#endif // !defined(AFX_MSGISSUE_H__06764C89_F374_42A1_BD8A_FDED275BE356__INCLUDED_)
