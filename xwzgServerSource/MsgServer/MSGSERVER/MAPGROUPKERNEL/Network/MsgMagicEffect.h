///////////////////////////////////////////////////////////
//
//		CMsgMagicEffect.h
//

#ifndef		_MSGMAGICEFFECT_H_
#define		_MSGMAGICEFFECT_H_

#include "NetMsg.h"


///////////////////////////////////////////////////////////
typedef struct 
{
	DWORD	idRole;
	int	    nData;// 陈剑飞修改，该类所以DWORD dwData全部改为int nData [6/12/2007]
	DWORD	dwBattleAddInfo;//  [7/10/2007] 王玉波添加
}EffectRoleStruct;

class CMsgMagicEffect : public CNetMsg
{
public:
	CMsgMagicEffect();
	virtual ~CMsgMagicEffect();

public:		//接口函数
	BOOL	Create(OBJID idUser, int nType, int nLevel, OBJID idTarget, UCHAR ucDir);
	BOOL	Create( OBJID idUser, int nType, int nLevel, OBJID idTarget, int nData, UCHAR ucDir,DWORD dwBattleAddInfo );
	BOOL	CreateByPos(OBJID idUser, int nType, int nLevel, int x, int y, UCHAR ucDir, OBJID idTarget=ID_NONE, int nData=0);		// 成片攻击都传坐标
	BOOL	CreateCollide(OBJID idUser, int nType, int nLevel, OBJID idTarget, int nData, int nCollideDir, UCHAR ucDir);
	BOOL	AppendRole( OBJID idRole, int nData,DWORD dwBattleAddInfo );

public:
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(CGameSocket* pSocket);

private:
	typedef struct
	{
		MSGHEAD_DEFINE

		OBJID				idUser;
// By Minsky,2010-05-28，不适用union
//		union{
			OBJID				idTarget;
			struct{
				USHORT				usPosX;
				USHORT				usPosY;
			};
			UCHAR				ucCollideDir;			// 8: no move
//		};
		USHORT				usType;
		USHORT				usLevel;
		UCHAR				ucDir;
		UCHAR				ucEffectNum;		// 可能为0
		EffectRoleStruct	setEffectRole[1];			// 变长
	}MSG_Info;

	MSG_Info*	m_pInfo;
};

#endif