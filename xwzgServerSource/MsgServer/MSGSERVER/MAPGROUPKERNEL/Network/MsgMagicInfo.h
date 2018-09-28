// MsgMagicInfo.h: interface for the CMsgMagicInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGMAGICINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
#define AFX_MSGMAGICINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum MAGICINFO{_MAGICINFO_NORMAL = 0,
                _MAGICINFO_SNEER,
				_MAGICINFO_DROPEQUIP,
			    _MAGICINFO_CHANGELEV,
				_MAGICINFO_LEARNNEW,//学习新技能
				_MAGICINFO_AWARDEXP,//增加技能熟练度
};

class CMsgMagicInfo : public CNetMsg  
{
public:
	CMsgMagicInfo();
	virtual ~CMsgMagicInfo();

	BOOL    Create      (UINT nAction, OBJID idTarget );
	BOOL	Create		(OBJID idOwner, int nType, int nLevel, int nExp ,int action=_MAGICINFO_NORMAL);
	BOOL	Create		(char* pMsgBuf, DWORD dwSize);
	void	Process		(void* pInfo);

protected:
	typedef struct {
		MSGHEAD_DEFINE
        UINT        nAction;
		OBJID		idOwner;
		DWORD		dwExp;
		USHORT		usType;
		USHORT		usLevel;
	}MSG_Info;

	MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGMAGICINFO_H__683B9A1E_9C3D_41E1_B90D_8DF5ABE79632__INCLUDED_)
