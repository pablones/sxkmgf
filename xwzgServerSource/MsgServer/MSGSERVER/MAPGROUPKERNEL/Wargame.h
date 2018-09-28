// Wargame.h: interface for the CWargame class.
//
//////////////////////////////////////////////////////////////////////
/********************************************************************
	created:	2007/08/27
	created:	27:8:2007   17:21
	filename: 	E:\mySVN\MSGServer\branch\TEST-activitySYS\MSGSERVER\MAPGROUPKERNEL\Wargame.h
	file path:	E:\mySVN\MSGServer\branch\TEST-activitySYS\MSGSERVER\MAPGROUPKERNEL
	file base:	Wargame
	file ext:	h
	author:		�粻Ϣ
	
	purpose:	ս���
*********************************************************************/
#if !defined(AFX_WARGAME_H__97C41ECC_03E6_4811_B5A9_87A9EFC503DE__INCLUDED_)
#define AFX_WARGAME_H__97C41ECC_03E6_4811_B5A9_87A9EFC503DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "WargameStatus.h" 
#include "I_Role.h"
#include "MsgWargame.h"
#include <map>

const int	WARGAME_ONTIMER_INTEVAL	=	1000;

enum	//	ս�������
{
	WARGAME_TYPE_ARENA	=	1,	//������
};

enum	//	��ǰ�״̬
{
	WARGAME_STATUS_STOP		=	0,	//	ֹͣ��
	WARGAME_STATUS_PREPARE	=	1,	//	׼��
	WARGAME_STATUS_WORK		=	2,	//	������
};


typedef struct	//	���Ϣ
{
	OBJID	id;				//id
	USHORT	nType;			//����
	USHORT	nCamps;			//��Ӫ����
	int		nSinginBegin;	//������ʼʱ��
	int		nSinginEnd;		//��������ʱ��
	int		nStartTime;		//���ʼʱ��
	int		nEndTime;		//�����ʱ��
	USHORT	nLevBegin;		//��ȼ�����
	USHORT	nLevEnd;		//��ȼ�����
	USHORT	nMaxPlayer;		//��������
	USHORT	nBaseScore;		//���ֻ�ֵ
	USHORT	nScoreParam;	//���ֲ���
} WARGAME_INFO;

typedef struct	//	���Ϣ
{
	OBJID	id;				//id
	UCHAR	ucCamp;			//������Ӫ
	UCHAR	ucVipLev;		//vip�ȼ�
	USHORT	usLev;			//�ȼ�
	DWORD	dwPro;			//ְҵ
	UINT	nKilled;		//ɱ�˴���
	UINT	nBeKilled;		//��������
	UINT	nScore;			//��������
	UINT	nAwardScore;	//��������
	OBJID	idLastKilled;	//ɱ�������ID
	UINT	nKillTime;		//�ϴ�ɱ�˵�ʱ��
	NAMESTR	szName;			//����
	clock_t tDieTime;		//����ʱ��
	bool	bEntered;		//�Ƿ����
} WARGAME_PLAYER;

typedef struct	//	���Ϣ
{
	OBJID	id;				//id
	USHORT	nType;			//�����
	OBJID	idMap;			//��ͼID
	int		nBorn_X;//����������
	int		nBorn_Y;
	int		nReBorn_X;//���������
	int		nReBorn_Y;
} WARGAMEMAP_INFO;

typedef struct
{
	int	nScore;
	int nPlayerAmount;	//ֻ��¼���뾺����������
	WARGAMEMAP_INFO mapInfo[1];
} WARGAME_CAMPS;

extern int g_SpecilLine;

class CWargame  
{ 
public:
	CWargame();
	virtual ~CWargame();

	static CWargame* CreateNew(int nType);
	bool CreateWargame(WARGAME_INFO &info, int idProcess);	//	����
	void Release();									//	��ʩ��ʱ������
	int  LoadMap();
	void Stop();
public:
	virtual void Init();							//	��ʼ��,��ʼ������״̬
	virtual void OnTimer(time_t tCurr);
	virtual void OnProcess();						//	������ʱ��OnTimer����
	virtual bool CanSignup();						//�Ƿ��ڱ���ʱ��
	virtual bool EnterWar(CUser* pUser);
	virtual void LeaveWar(CUser* pUser);
	virtual void OnPrepare();
	virtual void OnStart();
	virtual void OnStop();
	virtual void OnPlayerKill(IRole* pAtker, IRole* pTarget);//ɱ�˺�Ĵ���
	virtual void ProcessReborn(OBJID idUser);
	virtual void BroadCaseEnterMsg();

public:
	int GetCurrentStatus();
	int GetExpectStatusByTime();						//��ǰʱ���״̬
	
	//int GetType();									//�û����
	OBJID	GetWarGameID();								//�õ����ΨһID

	bool Signup(CUser* pUser, bool bSynchro=true);		//��ұ���
	bool Signup(WARGAME_PLAYERSCORE* pInfo);
	
	
	int	 AllocCamp();//��ѡ�ֻ�����Ӫ
	int  SendScoreRank(CUser* pUser);//����ս�����ְ�����,��NULL��ʾ�������ڹ㲥
	bool  SendCampScore(CUser* pUser=NULL);

	bool IsMaxPlayer();
	bool IsMaxSigninPlayer();
	bool CanUserEnter();
	bool AddWarGamePlayer(WARGAME_PLAYER info);			//δ����ظ����
	bool GetRebornPos(OBJID idUser, int &idMap, int &nPosX, int &nPosY);

	void BroadCaseWarGameMsg(CMsgWargame* pMsg);
	
	int  GetWinCamp(int &nMaxScore);//ȡ�û�ʤ��Ӫ������0��ʾƽ��

	WARGAME_CAMPS*	GetCampsInfo(int nCamps);
	WARGAME_PLAYER* GetWarGamePlayer(OBJID idUser);

	void SynchroPlayerAmount(WARGAME_PLAYERSCORE* pInfo, int nAmount);

	CGameMap* GetWarGameMap(CUser* pUser, int& nPosX, int& nPosY);

	int GetPlayerInWar();
	void PlayerLogout(CUser* pUser);

	static bool IsWarGameMap(OBJID idMap);

	void BroadCastCampMsg(OBJID idSender, CNetMsg* pMsg);//��Ӫ����

protected:
	WARGAME_INFO	m_wargameInfo;				//	���Ϣ
	vector<WARGAMEMAP_INFO>		m_vetMapInfo;	//��ͼ��Ϣ
	vector<WARGAME_PLAYER>		m_vetPlayerInfo;//�����Ϣ
	map<int, WARGAME_CAMPS>		m_mapCampScore;	//��Ӫ��Ϣ
	int m_nStatus;
	int m_idProcess;					//	��ͼ��ID
	clock_t m_tLastOnTimer;
	clock_t m_tStopTime;//����ʱ��
	//int m_nProfession;
};

#endif // !defined(AFX_WARGAME_H__97C41ECC_03E6_4811_B5A9_87A9EFC503DE__INCLUDED_)
