// MsgEudemonAttrib.h: interface for the CMsgEudemonAttrib class.
//			_MSG_EUDEMONATTRIB	=_MSG_GENERAL+1037
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGEUDATTR_H__8564AAFC_105E_49B7_9B62_12E1D65F5019__INCLUDED_)
#define AFX_MSGEUDATTR_H__8564AAFC_105E_49B7_9B62_12E1D65F5019__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"


enum {
	EUDATTR_NULL,	
	EUDATTR_LIFE,	
	EUDATTR_HAPPY,
	EUDATTR_EXP,
	EUDATTR_LEVEL,
	EUDATTR_ADDPOINT,
	EUDATTR_STR,
	EUDATTR_INT,
	EUDATTR_STA,
	EUDATTR_SPI,
	EUDATTR_AGI,
	EUDATTR_MAXLIFE,
	EUDATTR_ATK,			// ������
	EUDATTR_MAGICATK,		// ħ������
	EUDATTR_DEF,			// �������
	EUDATTR_MAGICDEF,		// ħ������
	EUDATTR_HIT,
	EUDATTR_DOGE,
	EUDATTR_CRIT,
	EUDATTR_TOUGH,
	EUDATTR_LIFESPAN,
	EUDATTR_STATE,
	EUDATTR_SEX,			//�Ա�
	EUDATTR_BORN,			//��ֳ����
	EUDATTR_LAST,
};

typedef struct {
	DWORD	usType;
	DWORD	usData;
} ST_EUDEMON_ATTRIB;

class CMsgEudemonAttrib : public CNetMsg  
{
public:
	CMsgEudemonAttrib();
	virtual ~CMsgEudemonAttrib();

public:		//�ӿں���
	BOOL	Create(OBJID idEudemon, int nType, DWORD dwData);
	BOOL	Append(int nType, DWORD dwData);

	//BOOL	Create(USHORT usAction, OBJID idEudemon, int nAttribNum, ST_EUDEMON_ATTRIB setEUDATTR[]);

public:
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(CGameSocket* pSocket);

private:
	typedef struct
	{
		MSGHEAD_DEFINE

		OBJID				idEudemonItem;
		DWORD				dwAttrNum;
		ST_EUDEMON_ATTRIB	setAttrib[1];			// �䳤
	}MSG_Info;
	
	MSG_Info*	m_pInfo;

};

#endif // !defined(AFX_MSGEUDATTR_H__8564AAFC_105E_49B7_9B62_12E1D65F5019__INCLUDED_)
