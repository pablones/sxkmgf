// InstanceNPC.h: interface for the CInstanceNPC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSTANCENPC_H__8A7A9BB2_5A21_4A87_A3BB_9943EF3B3A9A__INCLUDED_)
#define AFX_INSTANCENPC_H__8A7A9BB2_5A21_4A87_A3BB_9943EF3B3A9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//class CNpc;
//class CInstanceNPC;
///����NPC,רΪ���������,���ܳ�����е��,�����Ű�

///InstanceType,�ӵ�������õ���������
#include "InstanceRule.h"
#include "MapInstance.h"
#include "MapGroupObject.h"

class CInstanceNPC : public CNpc,public CInstanceRule,public has_slots<>
{
public:
	CInstanceNPC();
	virtual ~CInstanceNPC();
	static CInstanceNPC* CreateNew() { return new CInstanceNPC; }
	void StartInstance();
	virtual bool InitNpc(int nParam1,int nParam2);
	void RequestJoinRoom(OBJID idPlayer);///����������븱������
	BOOL ExitRoom(OBJID idPlayer);///�˳�����
	BOOL CreateRoom(OBJID idPlayer);///��������
	BOOL CloseRoom(OBJID  idPlayer);///�رշ���
	void KickPlayer(OBJID idRequest,OBJID idPlayer);///�߳����
	void ChangeOwn(OBJID idRequest,OBJID idPlayer);///�ı丱����������Ϣ
	void ChangeRoomTitle(OBJID idRequest,const CString& strRoomTitle);///�ı丱���������
	void ChangeRoomPassword(OBJID idRequest,const CString& strRoomPwd);///�ı丱����������
	void CheckRoomPassword(OBJID idRequest,const CString& strRoomPwd);///��鸱����������
	BOOL ChangeTeamGroup(OBJID idPlayer,UINT nTeamGroup);///�ı���Ҷ���
	void ReplyCreateInstance(OBJID idInstanceMap,UINT nInstanceMapType);
	void RefreshClientInstanceInfo(OBJID idPlayer,UINT nCurrentPage=1);///ˢ�¿ͻ��˸�����Ϣ
	void RefreshInstanceInfo(OBJID idPlayer);///ˢ�¿ͻ��˸�����Ϣ
	void RequestExitRoom(OBJID idRequest);///ˢ�¿ͻ��˸�����Ϣ
	void SetInstanceType(UINT nInstanceType){m_nInstanceType=nInstanceType;}
	bool IsRoomOwn(OBJID idRequest){return m_idOwn==idRequest;}
	UINT GetInstanceType(){return m_nInstanceType;}
	UINT GetInstanceMapType(){return m_nInstanceMapType;}
	void SetInstanceMapType(UINT nInstanceMapType){m_nInstanceMapType=nInstanceMapType;}
	UINT GetInstanceUserSize(){return m_InstanceUserList.size();}
	bool HasPassword(){return m_strRoomPwd.GetLength()>0;}
	OBJID	GetOwnID(){return m_idOwn;}
	void	SetOwnID(OBJID idOwd){m_idOwn=idOwd;}
	OBJID	GetInstanceMapID(){return m_idInstanceMap;}
	void	SetInstanceMapID(OBJID idInstanceMap){m_idInstanceMap=idInstanceMap;}
	CInstanceUser* GetInstanceUser(UINT nIndex);
	CInstanceUser* FindInstanceUser(OBJID idInstanceUser);
	void	StartInstance(OBJID idRequest);///����ͻ��˷������Ŀ�ʼ����������
	const CString& GetRoomTitle(){return m_strRoomTitle;}
	void	SetRoomTitle(CString strRoomTitle){m_strRoomTitle=strRoomTitle;}
	void	SetRoomPassword(CString strRoomPwd){m_strRoomPwd=strRoomPwd;}
	void	BroadcastMsg(CNetMsg& msg);///ȫ����Ա���й㲥
	signal1<OBJID>	slotPlayerEnterInstance;///�û����븱��ǰ�������¼�,ϵͳ�����ڸô���һЩ׼������,����رնԻ���.
protected:
	void CloseRoom();///�ر���������
	BOOL JoinRoom(OBJID idPlayer);///���뷿��
	void AutoCreateTeam();
	void AddPlayer(CInstanceUser& instanceuser);
	void RemovePlayer(OBJID idPlayer);
	void AddPlayer(OBJID idPlayer,UINT nTeamGroup=1);
	void TransmitAllPlayer();
	void TransmitPlayer(CInstanceUser& player);
	void OnPlayerEnterInstance(OBJID idPlayer);///�����Լ��������û����븱���¼�,׼���ر��û��ĸ����Ի���
	void NotifyUserStartInstance(OBJID idPlayer);///֪ͨ�ͻ��˸�����ʼ��
private:
	void MakeTestData();///������ٵĲ�������
	CInstanceTypeData* m_pData;///���������ֵ�����
	OBJID m_idInstanceMap;//�����ͼID
	UINT  m_nInstanceMapType;///��ͼ����
	OBJID m_idOwn;//����
	UINT  m_nInstanceType;///��������
	CString m_strRoomTitle;///�����������
	CString m_strRoomPwd;///������������
	InstanceUserList m_InstanceUserList;
};

#endif // !defined(AFX_INSTANCENPC_H__8A7A9BB2_5A21_4A87_A3BB_9943EF3B3A9A__INCLUDED_)
