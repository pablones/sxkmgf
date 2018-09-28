// MsgItemAttrib.h: interface for the CMsgItemAttrib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGITEMATTRIB_H__256183C8_E4BC_4A5D_8A8A_F9EC4EB9117A__INCLUDED_)
#define AFX_MSGITEMATTRIB_H__256183C8_E4BC_4A5D_8A8A_F9EC4EB9117A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum  {
	_ITEMATTRIB_AMOUNT,			_ITEMATTRIB_FIDELITY		=_ITEMATTRIB_AMOUNT,
	_ITEMATTRIB_AMOUNT_LIMIT,	_ITEMATTRIB_EUDEMON_LEVEL	=_ITEMATTRIB_AMOUNT_LIMIT,
	_ITEMATTRIB_WARGHOSTEXP,	_ITEMATTRIB_EUDEMON_LIFE	=_ITEMATTRIB_WARGHOSTEXP,
	_ITEMATTRIB_GEMTYPE,		_ITEMATTRIB_EUDEMON_EXP		=_ITEMATTRIB_GEMTYPE,
	_ITEMATTRIB_PHYSICAL_DEF,	//----------06.12.26����----begin---------	//�������
	_ITEMATTRIB_MAGIC_DEF,														//ħ������
	_ITEMATTRIB_PHYSICAL_MINATTACK,												//��С������
	_ITEMATTRIB_PHYSICAL_MAXATTACK,												//���������
	_ITEMATTRIB_MAGIC_MINATTACK,												//��Сħ������
	_ITEMATTRIB_MAGIC_MAXATTACK,												//���ħ������
	_ITEMATTRIB_STRENGTHAN_COUNT,//---------06.12.26����-----end-------------	//ǿ������( +�� )
	_ITEMATTRIB_GEM1,			//----------07.1.8����----begin---------		
	_ITEMATTRIB_GEM2,
	_ITEMATTRIB_GEM3,
	_ITEMATTRIB_GEM4,
	_ITEMATTRIB_GEM5,
	_ITEMATTRIB_GEM6,			//----------07.1.8����----end--------
	_ITEMATTRIB_BASIC_ADDITION1,//[��;���� 2008/11/13]
	_ITEMATTRIB_BASIC_ADDITION2,//[��;���� 2008/11/13]
	_ITEMATTRIB_BASIC_ADDITION3,//[��;���� 2008/11/13]
//	_ITEMATTRIB_WEAPON_UP,		//��� 2007/02/28 ����������Ϣ
	_ITEMATTRIB_KIDNAP,			//-------���---2007/04/10-----װ����
	_ITEMATTRIB_MAX_ACTGROW,// ����ɳ����� [�½���9/4/2007]
	_ITEMATTRIB_MIN_ACTGROW,
	_ITEMATTRIB_MAX_MAGICACTGROW,

	_ITEMATTRIB_IS_ACTIVATE,	//  [2007-9-15] ���	�Ƿ񼤻�
	_ITEMATTRIB_LIFE_TIME,		//  [2007-9-15] ���	��������

	_ITEMATTRIB_WPSL_STATE,
	_ITEMATTRIB_WPSL_ID1,		//���ID
	_ITEMATTRIB_WPSL_ID2,		//���ID
	_ITEMATTRIB_WPSL_ID3,		//���ID
	_ITEMATTRIB_WPSL_ID4,		//���ID
	_ITEMATTRIB_NONE = -1,
};

typedef struct 
{
	DWORD	ucAttributeType;
	DWORD	dwAttributeData;
}ST_ITEMATTRIB;


class CMsgItemAttrib : public CNetMsg  
{
public:
	CMsgItemAttrib();
	virtual ~CMsgItemAttrib();

	BOOL	Create(OBJID idItem, int nType, DWORD dwData);
	BOOL	Append(int nType, DWORD dwData);

public:
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(CGameSocket* pSocket);

protected:
	typedef struct
	{
		MSGHEAD_DEFINE

		OBJID			idItem;
		DWORD			dwAttributeNum;
		ST_ITEMATTRIB	setItemAttrib[1];			// �䳤
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGITEMATTRIB_H__256183C8_E4BC_4A5D_8A8A_F9EC4EB9117A__INCLUDED_)
