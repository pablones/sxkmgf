// MsgUserInfo.h: interface for the CMsgUserInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGUSERINFO_H__DFC94B21_4F65_11D5_A212_0000E8E7F527__INCLUDED_)
#define AFX_MSGUSERINFO_H__DFC94B21_4F65_11D5_A212_0000E8E7F527__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

struct UserInfoStruct;
class  ISocket;
class CMsgUserInfo : public CNetMsg
{
public:
	CMsgUserInfo();
	virtual ~CMsgUserInfo();

public:	
	BOOL			Create		(CUser* pUser);
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void *pInfo);

protected:
	typedef struct {
		MSGHEAD_DEFINE

		OBJID id;				//id标识
		UCHAR ucLookFace;		//肖像索引
		UCHAR ucFeel;
		UCHAR ucCurJob;
		UCHAR  nSex;
		DWORD usHp;
		DWORD usMaxHp;
		DWORD usMp;
		DWORD usMaxMp;
		USHORT usSp;
		USHORT usMaxSp;	

		USHORT usLevel;		//等级
		USHORT usFirJob;
		USHORT usFirJobLev;
		USHORT usFirJobPhase;
		USHORT usSecJob;
		USHORT usSecJobLev;
		USHORT usSecJobPhase;
		USHORT Vit;
		USHORT MaxVit;
		USHORT Ene;
		USHORT MaxEne;
		USHORT Canrename;
		DWORD nDress;
		DWORD nWeapen;
		DWORD nMount;

		UINT uHoldTitle;//玩家拥有的称号
		UINT uUseTitle;//玩家当前使用的称号
		UINT uExp;
		UINT nPk;
		UINT PkProc;
		DWORD nMoney1;
		DWORD nMoney2;
		DWORD nMoney3;
		DWORD nMoney4;
		DWORD nMoney5;

		DWORD dwBagLev;
		DWORD dwBankLev;

		DWORD taskMaskHi;
		DWORD taskMaskLow;
		DWORD DaytaskMaskHi;
		DWORD DaytaskMaskLow;

		DWORD idSyn;
		DWORD nSynRank;


		DWORD dwIsVip;
        DWORD onlineaward;

		DWORD dwIsDress;
		DWORD conSyn;

		DWORD DaytaskMaskLow2;
		DWORD DaytaskMaskHi2;

		char szBuf[1];
	}MSG_Info;
	MSG_Info*	m_pInfo;

private:
	CNetStringPacker	m_StrPacker;
};

#endif // !defined(AFX_MSGUSERINFO_H__DFC94B21_4F65_11D5_A212_0000E8E7F527__INCLUDED_)
