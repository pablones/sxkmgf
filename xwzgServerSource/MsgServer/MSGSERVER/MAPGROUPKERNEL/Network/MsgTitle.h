// MsgTitle.h: interface for the CMsgTitle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGTITLE_H__D2ECFEDA_1636_4F9A_9808_6EDB98D7E278__INCLUDED_)
#define AFX_MSGTITLE_H__D2ECFEDA_1636_4F9A_9808_6EDB98D7E278__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

const int _TITLE_INIT	= 1;		//ͬ���ƺŵ��ͻ���
const int _TITLE_ADD	= 2;		//���һ���³ƺ�
const int _TITLE_CHGLEV	= 3;		//�ƺ�����
const int _TITLE_DELTE	= 4;		//ɾ���ƺ�

struct TitleInfo
{
	UINT	uType;	//�ƺ�����
	UINT	uLevel;	//�ƺŵȼ�
};
class CMsgTitle : public CNetMsg  
{
public:
	CMsgTitle();
	virtual ~CMsgTitle();
public:
	BOOL	Create(char* pbufMsg, DWORD dwMsgSize);
    BOOL	Create(int nAction, OBJID idUser);
	BOOL	Append(UINT uType, UINT uLevel);
	
	void	Process(void* pInfo);
	
private:
	typedef struct 
	{
		MSGHEAD_DEFINE
		OBJID               idUser;
		UINT				uAction;
		UINT        		uAmount;
		TitleInfo			titleInfo[1];
	}MSG_Info;
	
	MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGTITLE_H__D2ECFEDA_1636_4F9A_9808_6EDB98D7E278__INCLUDED_)
