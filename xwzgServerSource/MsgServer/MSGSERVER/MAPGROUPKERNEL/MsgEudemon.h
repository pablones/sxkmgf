// MsgEudemon.h: interface for the CMsgEudemon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGEUDEMON_H__EA513CF7_FFE9_4368_90AF_77B1A4FC0E1B__INCLUDED_)
#define AFX_MSGEUDEMON_H__EA513CF7_FFE9_4368_90AF_77B1A4FC0E1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "ItemData.h"

enum PETACTION
{
	PETACTION_PetInfo		=	1,
	PETACTION_DelPet		=	2,
	PETACTION_PetBorn		=	3,	//繁殖
	PETACTION_LockBornPet	=	4,	//繁殖加锁
	PETACTION_UnLockBornPet	=	5,	//繁殖解锁
	PETACTION_BornStop		=	6,	//停止繁殖
	PETACTION_BornBegin		=	7,	//开始繁殖
	PETACTION_StoragePet	=	10, //仓库宠物
	PETACTION_TradePet		=	11,	//交易宠物
	PETACTION_CheckinPet	=	12,	//存宠物
	PETACTION_CheckoutPet	=	13,	//取宠物
	PETACTION_AddBoothPet	=	14, //摊位上加宠物
	PETACTION_DelBoothPet	=	15, //摊位上减宠物
	PETACTION_PetPlay		=	16, //宠物玩耍
	PETACTION_LockPlayPet	=	17,	//玩耍加锁
	PETACTION_UnLockPlayPet	=	18,	//玩耍解锁
	PETACTION_PlayStop		=	19,	//停止玩耍
	PETACTION_PlayBegin		=	20,	//开始玩耍
};

class CMsgEudemon : public CNetMsg 
{
public:
	CMsgEudemon();
	virtual ~CMsgEudemon();
public:	
	BOOL			Create		(USHORT usAction,EudemonData* data = NULL,OBJID idOwner = ID_NONE);
	BOOL			Append		(EudemonData data,int nMoney = 0);
	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void *pInfo);
	
protected:
	struct eudemonShow
	{
		DWORD id;		
		UCHAR gettype;
		UCHAR playTime;
		USHORT lookface;
		UCHAR needlev;
		UCHAR failtime;
		UCHAR sex;
		UCHAR habit;
		UCHAR lev;
		UCHAR gift;
		UCHAR bronTime;
		UCHAR understand;
		DWORD money;
		UCHAR nGrow;
		UCHAR usStatus;
		UCHAR fusion;
		UCHAR intell;
		char szName[16];
	};

	typedef struct {
		MSGHEAD_DEFINE
		USHORT usAction;
		USHORT usAmount;
		OBJID idOwner;
		eudemonShow data[1];
	}MSG_Info;
	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGEUDEMON_H__EA513CF7_FFE9_4368_90AF_77B1A4FC0E1B__INCLUDED_)
