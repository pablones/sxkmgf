// PetEggData.h: interface for the CPetEggData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PETEGGDATA_H__9A2DE666_F2FE_462E_A2A3_8E3CA32EF3B5__INCLUDED_)
#define AFX_PETEGGDATA_H__9A2DE666_F2FE_462E_A2A3_8E3CA32EF3B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameObj.h"
#include "../../GameBaseCodeMT/GameData.h"

enum PETEGGDATA{
	PETEGGDATA_ID=0,
	PETEGGDATA_OWNERID,
	PETEGGDATA_ITEMID,//��Ӧ����ƷID
	PETEGGDATA_GETTYPE,//����
	PETEGGDATA_GROW,//�ɳ�
	PETEGGDATA_STRCHG,//���� ����=pow(1.06f, gettype) * chg * ���Լӳ�
	PETEGGDATA_INTCHG,//����
	PETEGGDATA_STACHG,//����
	PETEGGDATA_SPICHG,//����
	PETEGGDATA_AGICHG,//��
	PETEGGDATA_NEXTFLUSH,//�´�ˢ��ʱ��
};
//�е�ͼ����
struct EggInfoStruct
{
	OBJID	id;
	OBJID	idUser;
	OBJID	idItem;
	USHORT	usGetType;
	USHORT	usGrow;
	USHORT	usStrChg;
	USHORT	usIntChg;
	USHORT	usStaChg;
	USHORT	usSpiChg;
	USHORT	usAgiChg;
	DWORD	dwNextFlush;
};

char szPeteggTable[];
typedef	CGameData<PETEGGDATA, szPeteggTable, szID>	CGamePeteggData;

class CPetEggData  
{
public:
	CPetEggData();
	virtual ~CPetEggData();

public:
	static  CPetEggData*	CreateNew()		{ return new CPetEggData; }
	void	ReleaseByOwner()		{ delete this; }

	bool	Create(IRecordset* pRecordset);
	bool	Create(IRecordset* pDefaultRecord, OBJID idNewKey);

	OBJID	GetID()				{ CHECKF(m_pData); return m_pData->GetID(); }
	bool	Update()			{ CHECKF(m_pData); return m_pData->Update(); }
	void	ClearUpdateFlags()	{ CHECK(m_pData); m_pData->ClearUpdateFlags(); }
	OBJID	InsertRecord()		{ CHECKF(m_pData); return m_pData->InsertRecord(); }
	bool	DeleteRecord()		{ CHECKF(m_pData); return m_pData->DeleteRecord(); }

	int		GetInt(PETEGGDATA nDataName);
	void	SetInt(PETEGGDATA idx, int nData, bool bUpdate=false);

	bool	NeedFlush();
	LONG	GetNextFlushSeconds();
protected:
	CGamePeteggData*	m_pData;
};

#endif // !defined(AFX_PETEGGDATA_H__9A2DE666_F2FE_462E_A2A3_8E3CA32EF3B5__INCLUDED_)
