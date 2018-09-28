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
	int	    nData;// �½����޸ģ���������DWORD dwDataȫ����Ϊint nData [6/12/2007]
	DWORD	dwBattleAddInfo;//  [7/10/2007] �������
}EffectRoleStruct;

class CMsgMagicEffect : public CNetMsg
{
public:
	CMsgMagicEffect();
	virtual ~CMsgMagicEffect();

public:		//�ӿں���
	BOOL	Create(OBJID idUser, int nType, int nLevel, OBJID idTarget, UCHAR ucDir);
	BOOL	Create( OBJID idUser, int nType, int nLevel, OBJID idTarget, int nData, UCHAR ucDir,DWORD dwBattleAddInfo );
	BOOL	CreateByPos(OBJID idUser, int nType, int nLevel, int x, int y, UCHAR ucDir, OBJID idTarget=ID_NONE, int nData=0);		// ��Ƭ������������
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
// By Minsky,2010-05-28��������union
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
		UCHAR				ucEffectNum;		// ����Ϊ0
		EffectRoleStruct	setEffectRole[1];			// �䳤
	}MSG_Info;

	MSG_Info*	m_pInfo;
};

#endif