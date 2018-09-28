// MsgEudemonInfo.h: interface for the CMsgEudemonInfo class.
//	幻兽INFO (_MSG_EUDEMONINFO	=_MSG_GENERAL+116)
//  从 MsgPlayer 分离出来的，就是幻兽的 MsgPlayer
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MsgEudemonInfo_H__298DE32A_21B9_41E0_8728_CD895F36A959__INCLUDED_)
#define AFX_MsgEudemonInfo_H__298DE32A_21B9_41E0_8728_CD895F36A959__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "NetStringPacker.h"
#include "ItemData.h"

class CMsgEudemonInfo : public CNetMsg  
{
public:
	CMsgEudemonInfo();
	virtual ~CMsgEudemonInfo();

public:	
	BOOL			Create		(CMonster* pMonster);
	BOOL			Create		(EudemonData data);

	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void *pInfo);

protected:
	typedef struct {
		MSGHEAD_DEFINE
		//显示固有
		OBJID id;
		
		//特殊
		OBJID ownerId;
		DWORD typeId;
		DWORD needLev;

		//动态
		DWORD life;					
		DWORD exp;
		USHORT dwLookFace;
		USHORT addpoint;
		USHORT gift;
		USHORT grow;
		USHORT understand;
		USHORT level;		
		USHORT gettype;
		USHORT happy;
		USHORT habit;
		UCHAR  bronTime;
		UCHAR  playTime;
		USHORT state;
		USHORT sex;
		USHORT fusion;
		USHORT intelligence;
		DWORD score;
		DWORD lifespan;
		DWORD maxlifespan;

		//计算属性
		DWORD firStrChgMax;
		DWORD firIntChgMax;
		DWORD firStaChgMax;
		DWORD firSpiChgMax;
		DWORD firAgiChgMax;
		DWORD firStrChg;
		DWORD firIntChg;
		DWORD firStaChg;
		DWORD firSpiChg;
		DWORD firAgiChg;
		DWORD StrChg;
		DWORD IntChg;
		DWORD StaChg;
		DWORD SpiChg;
		DWORD AgiChg;
		DWORD dwStr;
		DWORD dwInt;
		DWORD dwSta;
		DWORD dwSpi;
		DWORD dwAgi;
		DWORD PhyAttack;		
		DWORD MagicAttack;		
		DWORD PhyDef;			
		DWORD MagicDef;		
		DWORD Hit;				    
		DWORD Hide;			
		DWORD Crit;			
		DWORD Toughness;		
		DWORD HpMax;					

		DWORD skill[10];
		char szBuf[1];
	}MSG_Info;
	MSG_Info*	m_pInfo;

private:
	CNetStringPacker	m_StrPacker;

};

#endif // !defined(AFX_MsgEudemonInfo_H__298DE32A_21B9_41E0_8728_CD895F36A959__INCLUDED_)
