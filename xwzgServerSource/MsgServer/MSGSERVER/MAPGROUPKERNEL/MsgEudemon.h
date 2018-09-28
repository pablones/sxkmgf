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
	PETACTION_PetBorn		=	3,	//��ֳ
	PETACTION_LockBornPet	=	4,	//��ֳ����
	PETACTION_UnLockBornPet	=	5,	//��ֳ����
	PETACTION_BornStop		=	6,	//ֹͣ��ֳ
	PETACTION_BornBegin		=	7,	//��ʼ��ֳ
	PETACTION_StoragePet	=	10, //�ֿ����
	PETACTION_TradePet		=	11,	//���׳���
	PETACTION_CheckinPet	=	12,	//�����
	PETACTION_CheckoutPet	=	13,	//ȡ����
	PETACTION_AddBoothPet	=	14, //̯λ�ϼӳ���
	PETACTION_DelBoothPet	=	15, //̯λ�ϼ�����
	PETACTION_PetPlay		=	16, //������ˣ
	PETACTION_LockPlayPet	=	17,	//��ˣ����
	PETACTION_UnLockPlayPet	=	18,	//��ˣ����
	PETACTION_PlayStop		=	19,	//ֹͣ��ˣ
	PETACTION_PlayBegin		=	20,	//��ʼ��ˣ
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
