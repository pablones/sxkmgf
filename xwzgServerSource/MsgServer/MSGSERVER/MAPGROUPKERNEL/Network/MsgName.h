// MsgName.h: interface for the CMsgName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGNAME_H__62DB03E2_7185_4BDF_B9B0_5AE64B03DFDB__INCLUDED_)
#define AFX_MSGNAME_H__62DB03E2_7185_4BDF_B9B0_5AE64B03DFDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "NetStringPacker.h"

enum {	NAMEACT_NONE				= 0, 
		NAMEACT_FIREWORKS			= 1,
		NAMEACT_CREATE_SYN			= 2, 				// �ĵ�TALK��
		NAMEACT_SYNDICATE			= 3, 				// �޷�ʵ���޸İ����������´�
		NAMEACT_CHANGE_SYNTITLE		= 4, 
		NAMEACT_DELROLE				= 5,				// ������
		NAMEACT_MATE				= 6,
		NAMEACT_QUERY_NPC			= 7,				// to server��to client
		NAMEACT_WANTED				= 8,				// to client
		NAMEACT_MAPEFFECT			= 9,				// to client
		NAMEACT_ROLEEFFECT			= 10,
		NAMEACT_MEMBERLIST			= 11,				// to server/client, dwData is index
		MANEACT_KICKOUT_SYNMEM		= 12,
		NAMEACT_QUERY_WANTED		= 13,
		NAMEACT_QUERY_POLICEWANTED	= 14,
		NAMEACT_POLICEWANTED		= 15,
		NAMEACT_QUERY_MATE			= 16,
		NAMEACT_ADDDICE_PLAYER		= 17,			//BcastClient(INCLUDE_SELF) �����������// dwDataΪ����̯ID // To Server ���� ��Ҫԭ��Ϣ����
		NAMEACT_DELDICE_PLAYER		= 18,			//BcastClient(INCLUDE_SELF) ɾ���������// dwDataΪ����̯ID // To Server �뿪 ��Ҫԭ��Ϣ����
		NAMEACT_DICE_BONUS			= 19,			//BcastClient �����ӽ��� ���// dwDataΪMoney
		NAMEACT_PLAYER_WAVE			= 20,			// ����Ч // ���usPosX = usPosY = 0; �ǵ�ͼ��Ч // ѹ����ַ���Ϊ��Ч�����·�� 
		NAMEACT_MEMBERLIST_SPECIFYSYN = 21,			//��ѯָ�����ɵĳ�Ա�б�
		NAMEACT_PLAYERTASKLIST		= 23,			// Ӷ�������ѯ����
		NAMEACT_CHANGE_EUDEMON_NAME	= 24,			// ���޸�����idTarget=������ƷID����һ���ַ���Ϊ����������
		NAMEACT_CHANGE_DESC			= 25,			// �����ҽ���..
		NAMEACT_REBORN_MSG			= 26,			//����ĳ�˶���ʹ���˸�����
		NAMEACT_REBORN_ACCEPT		= 27,			//�ͻ��˷��͹���ͬ�⸴��.
		NAMEACT_CHANGE_EXPSCALE		= 28,			//�ͻ��˷��͹�������������þ������
		NAMEACT_CHANGE_EXPSCALEOK	= 29,			//���ĳɹ��������ͻ���.
		NAMEACT_LOOK_DESC			= 30,			//�鿴���˵Ľ���
		NAMEACT_REBORN_MSG_SR		= 31,	//2007828����:struggle relive���غ���.���Ҹ���
		NAMEACT_CHANGE_WPSLEXPSCALE		= 32,			//�ͻ��˷��͹����������������þ������
		NAMEACT_CHANGE_WPSLEXPSCALEOK	= 33,			//���ĳɹ��������ͻ���.

		NAMEACT_CHANGE_QQ               = 34,            //����qq
		NAMEACT_CHANGE_PROV             = 35,            //ʡ
		NAMEACT_CHANGE_CITY             = 36,            //��
		NAMEACT_CHANGE_OLD              = 37,             //����
		NAMEACT_CHANGE_STAR             = 38,            //����
		NAMEACT_CHANGE_BTSEX            = 39,            //�Ա�̬

		NAMEACT_CHANGE_TITLE			= 40,			//[��;���� 2008/10/13]�ͻ��˷��͹������ĳƺ�
		NAMEACT_CHANGE_TITLEOK			= 41,			//[��;���� 2008/10/13]�ƺŸ��ĳɹ��������ͻ���.

		NAMEACT_CHANGE_HOLDTITLEOK		= 42,			//[��;���� 2008/10/14]ӵ�гƺŸ��ĳɹ��������ͻ���.
		NAMEACT_CHANGE_RECORDKEY        = 43,           //�����ݼ��ͼ�����
		NAMEACT_HANDUP_BUG              = 44,           //�ύ����
		NAMEACT_SETLINE					= 45,//[2009.07.06]
		NAMEACT_COPY_SYNNAME            = 46,           //���ư�����
};

class CMsgName : public CNetMsg   
{
public:
	CMsgName();
	virtual ~CMsgName();

	BOOL	Create(int nType, const char* pszName, __int64 dwData=0);
	BOOL	Create(int nType, const char* pszName, USHORT usPosX, USHORT usPosY);
	BOOL	AppendStr(const char* pszName);

public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void* pInfo);
	
private:
	typedef struct{
		MSGHEAD_DEFINE

		DWORD	dwData;
		DWORD	dwHData;
		union {
			OBJID	idTarget;
			struct{
				USHORT  usPosX, usPosY;
			};
		};
		UCHAR	ucType;
		DWORD	dwSynData;	
		char	szBuf[1];
	}MSG_Info;
	
	MSG_Info*	m_pInfo;

private:
	CNetStringPacker	m_StrPacker;
};

#endif // !defined(AFX_MSGNAME_H__62DB03E2_7185_4BDF_B9B0_5AE64B03DFDB__INCLUDED_)
