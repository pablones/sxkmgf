// MsgSynAttrInfo.h: interface for the CMsgSynInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGSYNINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
#define AFX_MSGSYNINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "../Syndicate.h"
#include "NetStringPacker.h"


class CMsgSynInfo : public CNetMsg  
{
public:
	bool CreateSub(CSyndicate* pSyn);
	CMsgSynInfo();
	virtual ~CMsgSynInfo();

	BOOL	Create		(CSyndicate* pSyn);
	BOOL	CreateNew		(CSyndicate* pSyn,CUserPtr pUser);
	BOOL	CreateSci	(CSyndicate* pSyn);
	BOOL	CreateInfo(CSyndicate *pSyn);
	BOOL	Create		(char* pMsgBuf, DWORD dwSize);
	void	Process		(void* pInfo);
	BOOL	CreateUpdate(OBJID nAction,OBJID nValue,OBJID nValue2=0,OBJID nValue3=0);
protected:
	typedef struct {
		MSGHEAD_DEFINE
		USHORT		level;
		USHORT      amount;
		USHORT      shopAmount;
		USHORT      nMove;
		OBJID       nBuild;
		OBJID		affluent;
		OBJID		idSyn;
		DWORD		dwMoney;
		DWORD		dwCreateTime;
		USHORT		state;
		USHORT		onlineUser;

		char		szBuf[1];
	}MSG_Info;

protected:
	typedef struct {

	MSGHEAD_DEFINE
	USHORT		sub1_lev;
	USHORT		sub2_lev;
	USHORT		sub3_lev;
	USHORT		sub4_lev;


	OBJID		sub1_Growing;
	OBJID		sub2_Growing;
	OBJID		sub3_Growing;
	OBJID		sub4_Growing;
	////////////////////////////////////////////
	OBJID		sub1_skill1;
	OBJID		sub1_skill2;
	OBJID		sub1_skill3;
	
	OBJID		sub2_skill1;
	OBJID		sub2_skill2;
	OBJID		sub2_skill3;
	
	OBJID		sub3_skill1;
	OBJID		sub3_skill2;
	OBJID		sub3_skill3;
	
	OBJID		sub4_skill1;
	OBJID		sub4_skill2;
	OBJID		sub4_skill3;
	/////////////////////////////////////////////
	OBJID		sub1_Kongfu;
	OBJID		sub2_Kongfu;
	OBJID		sub3_Kongfu;
	OBJID		sub4_Kongfu;
	
	OBJID		sub1_Busman;
	OBJID		sub2_Busman;
	OBJID		sub3_Busman;
	OBJID		sub4_Busman;
	
	OBJID		sub1_Builder;
	OBJID		sub2_Builder;
	OBJID		sub3_Builder;
	OBJID		sub4_Builder;

	OBJID		sub_Stop;
	OBJID		Pay;
	}SUBMSG_INFO;

protected:
	typedef struct {
		MSGHEAD_DEFINE
		USHORT Level;//��ȼ�
		USHORT Rank;//ְλ
		USHORT Online;//��������
		USHORT Amount;//�������
		OBJID Synid;//��id
		OBJID HisCon;//��ʷ�ﹱ
		OBJID NowCon;//���ڰﹱ
		OBJID Money;//����ʽ�
		OBJID Contribut;//��Ὠ��
		char		szBuf[1];
	}NEWMSG_INFO;
protected:
	typedef struct {
		MSGHEAD_DEFINE//1.���� 2.�׻� 4.��ȸ 3.����
		OBJID sub1;//
		OBJID sub2;//
		OBJID sub3;//
		OBJID sub4;//
		
	}SCIMSG_INFO;

	MSG_Info*		m_pInfo;
	SUBMSG_INFO*	m_pSubInfo;
	NEWMSG_INFO*    m_pNewInfo;
	SCIMSG_INFO*	m_pSciInfo;
	CNetStringPacker m_StrPacker;
	CNetStringPacker m_StrPacker2;
};

	

#endif // !defined(AFX_MSGSYNINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
