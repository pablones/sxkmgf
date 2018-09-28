// protocol.h
// �ɽ��ޣ�2002.10.23
//////////////////////////////////////////////////////////////////////

#ifndef	PROTOCOL_H
#define	PROTOCOL_H

#include <string.h>
#include "define.h"
#include <vector>
#ifdef MAPGROUP_KERNEL
#include "UserData.h"
#endif


enum{
		//////////////////////////////////////////////////////////////////////
		// ��ͼ������紦�����Ϣ
		KERNEL_BASE					= 1000,
		KERNEL_CLIENTMSG			= KERNEL_BASE + 1,				// CLIENTMSG_PACKET0(size:1024+12), �ͻ����ϴ�����Ϣ��
//		KERNEL_NPCMSG				= KERNEL_BASE + 2,				// NPCMSG_PACKET0(size:1024+12), NPC�������ϴ�����Ϣ��
		KERNEL_CLOSEKERNEL			= KERNEL_BASE + 3,				// INT, SOCKET_ID��֪ͨ���Ĺر�(���ı����ٴ�֪ͨSOCKET��!!!)
		KERNEL_CREATESYNDICATE		= KERNEL_BASE + 10,				// CreateSyndicateInfo
		KERNEL_DESTROYSYNDICATE		= KERNEL_BASE + 11,				// OBJID
		KERNEL_CHANGESYNDICATE		= KERNEL_BASE + 12,				// SynFuncInfo0

		//////////////////////////////////////////////////////////////////////
		// �ɵ�ͼ����Ĵ������Ϣ
		MAPGROUP_BASE				= 10000,
		MAPGROUP_LOGIN				= MAPGROUP_BASE + 1,			// ST_LOGIN, ��ҵ�¼��
		MAPGROUP_BROADCASTMSG		= MAPGROUP_BASE + 2,			// CLIENTMSG_PACKET0(size:1024+12), ת���㲥��Ϣ��
		MAPGROUP_SOCKETUSERINFO		= MAPGROUP_BASE + 3,			// ST_USERCHANGEMAPGORUP��STRUCT(SOCKET_ID,UserInfoStruct)����һ���ͼ�顣
		MAPGROUP_SENDOBJINFO		= MAPGROUP_BASE + 4,			// CHANGEMAPGORUP_INFO0{id,type,info}
		MAPGROUP_TRANSMITMSG		= MAPGROUP_BASE + 5,			// TRANSMSG_PACKET0(size:1024+16), ת���㲥��Ϣ��
		MAPGROUP_CHANGEMAPGROUP		= MAPGROUP_BASE + 10,			// ST_CHANGEMAPGROUP����һ���ͼ�顣
		MAPGROUP_CREATENEWNPC		= MAPGROUP_BASE + 11,			// ST_CREATENEWNPC������һ����NPC��
		MAPGROUP_CHANGETEAM			= MAPGROUP_BASE + 12,			// ST_CHANGETEAM, ת��TEAM�޸�OPERATION��
		MAPGROUP_REMOTECALL			= MAPGROUP_BASE + 13,			// REMOTE_CALL0, ת����USER��
		MAPGROUP_CHANGENPC			= MAPGROUP_BASE + 14,			// int+str+str, ת��NPC�޸�OPERATION��
		MAPGROUP_DELTRANSNPC		= MAPGROUP_BASE + 15,			// int, ת��ɾ��TRANS_NPC
//		MAPGROUP_DELALLMONSTER		= MAPGROUP_BASE + 16,			// int, ɾ�����й���Ա�����¹��ͬʱ����SOCKET_KERNRL
		MAPGROUP_TRANSMITMSG_FORCHATROOM = MAPGROUP_BASE + 20,		// ��������Ϣ
		MAPGROUP_NEWDAY				= MAPGROUP_BASE + 21,			//���0321 �µ�һ��
		MAPGROUP_PHYLEMSG	        = MAPGROUP_BASE + 27,			//��������ͼ���߳�,��������������Ϣ
		MAPGROUP_NOWUPDATE_CALCULATED_PROPERTY = MAPGROUP_BASE + 28,//20070122����:�л���ͼ��ʱ,���½�ɫ��
		MAPGROUP_CHANGEMAPGROUP_REQ	= MAPGROUP_BASE + 29,			//20070329����:source->aim�л���ͼ������.
		MAPGROUP_CHANGEMAPGROUP_ACK	= MAPGROUP_BASE + 30,			//20070329����:aim->source�л���ͼ��ͬ��.
		MAPGROUP_CHANGEMAPGROUP_NACK= MAPGROUP_BASE + 31,			//20070329����:aim->source�л���ͼ��ܾ�.
		MAPGROUP_USERCLOSEONTIMER	= MAPGROUP_BASE + 32,			//20070518����:��ͼ�����ĳ�����ֹonTimer
		MAPGROUP_GMFLYPLAYER		= MAPGROUP_BASE + 33,			//------HALEI---2007/05/23----GM�����
		MAPGROUP_GMSTOPSAY			= MAPGROUP_BASE + 34,			//  [8/13/2007 Add By HaLei] GM����
		MAPGROUP_SENDMSG			= MAPGROUP_BASE + 38,			//ָ����ͼ��ת����Ϣ-������-2008-1-4
		MAPGROUP_NEW_SYN_TIME       = MAPGROUP_BASE + 40,
		MAPGROUP_LOGOUTALLUSER		= MAPGROUP_BASE + 49,			//zeng�߳������ڸõ�ͼ����������
		MAPGROUP_CHGTUTOR   		= MAPGROUP_BASE + 54,			//ʦͽ���̴߳���
		MAPGROUP_AWARDINFO   		= MAPGROUP_BASE + 59,//ר����
		MAPGROUP_MAINSERVER		    = MAPGROUP_BASE + 61,           //����Ϸ��������Ϣ
		MAPGROUP_VASMSG				= MAPGROUP_BASE + 62,			//����Ԫ������Ϣ
		MAPGROUP_MSG				= MAPGROUP_BASE + 63,			//ת������Ϣ
		//////////////////////////////////////////////////////////////////////
		// ��������Ĵ������Ϣ
		WORLD_BASE					= 20000,
		WORLD_SHELLTALK				= WORLD_BASE + 1,				// STRING, ��Ƿ���TALK��Ϣ��
		WORLD_SETPROCESSID			= WORLD_BASE + 2,				// CHANGE_USERDATA, �����µ�PROCESS_ID
		WORLD_CLOSESOCKET			= WORLD_BASE + 3,				// INT, SOCKET_ID������֪ͨSOCKET�ر�(SOCKET����֪ͨ����)
		WORLD_USERNAMEMSG			= WORLD_BASE + 4,				// USERNAMEMSG_PACKET0(size:1024+16+12), ������ת������ҡ�
		WORLD_USERIDMSG				= WORLD_BASE + 5,				// USERIDMSG_PACKET0(size:1024+4+12), ��IDת������ҡ�
		WORLD_LEVELUP				= WORLD_BASE + 6,				// OBJID, �������10����
		WORLD_QUERYFEE				= WORLD_BASE + 7,				// OBJID, ACCOUNT ID��
		WORLD_SENDTIME				= WORLD_BASE + 8,				// INT, SOCKET_ID
		WORLD_TRANSMITMSG			= WORLD_BASE + 9,				// TRANSMSG_PACKET0(size:1024+16), ת����Ϣ��
		WORLD_VAS					= WORLD_BASE + 10,				// ������ֵϵͳ����Ϣת��
		WORLD_USERCLOSEONTIMER		= WORLD_BASE + 11,				// ֪ͨ��ͼ����ĳ�����ֹontimer
		WORLD_CHGMAP				= WORLD_BASE + 12,				// ��� [5/23/2007] ֪ͨ�����̴߳���GM������Ϣ
		WORLD_STOPSAY				= WORLD_BASE + 13,				//  [8/13/2007 Add By HaLei] ֪ͨ�����ֳɴ��������Ϣ
		WORLD_TEAMINVITE			= WORLD_BASE + 14,				//  [2007-9-7] ��� �������
		WORLD_FRIENDAPPLY			= WORLD_BASE + 15,				//  [2007-9-10] ��� �������	
		WORLD_TUTOR_INFO	    	= WORLD_BASE + 20,				//ʦͽ��Ϣ
		WORLD_RELATIONMSG			= WORLD_BASE + 23,				//ת������ϵ����������
		WORLD_TRANSMITNETMSG		= WORLD_BASE + 24,				//����ת��NetMsg
		//////////////////////////////////////////////////////////////////////
		// ��SOCKET���Ĵ������Ϣ
		SOCKET_BASE					= 30000,
		SOCKET_CLOSESOCKET			= SOCKET_BASE + 1,				// INT, SOCKET_ID������֪ͨSOCKET�ر�(SOCKET����֪ͨ����)
		SOCKET_SETPROCESSID			= SOCKET_BASE + 2,				// CHANGE_USERDATA, �����µ�PROCESS_ID
		//!!! ���ķ��͸���Ϣʱ��Ҫͬʱ����CUSER�е�PROCESS_ID��־�������ڴ���KERNEL_CLIENTMSGʱ��һ��Ҫ���PROCESS_ID�Ƿ�ƥ��!!!
		SOCKET_SENDCLIENTMSG		= SOCKET_BASE + 3,				// SENDCLIENTMSG_PACKET0(size:1024+12), ����ͻ��˺�NPC����������Ϣ��
//		SOCKET_SENDNPCMSG			= SOCKET_BASE + 4,				// SENDNPCMSG_PACKET0(size:1024+12), ����ͻ��˺�NPC����������Ϣ��
//		SOCKET_SETNPCSOCKETID		= SOCKET_BASE + 5,				// INT, SOCKET_ID, ������Ľ��յ�NPC�������ĵ�¼��Ϣʱ��֪ͨSOCKET���ġ�(����ΪSOCKET_NONE)
//		SOCKET_SETNPCPROCESSID		= SOCKET_BASE + 6,				// CHANGE_NPCPROCESSID, �����µ�PROCESS_ID
		SOCKET_CHANGECODE			= SOCKET_BASE + 7,				// CHANGE_USERDATA���޸Ķ�̬���롣
		SOCKET_BREAKCONNECT			= SOCKET_BASE + 8,				// INT, SOCKET_ID, �����ʱ���ȶϿ�SOCKET���ӡ�
		SOCKET_BREAKALLCONNECT		= SOCKET_BASE + 11,				// INT, GM_SOCKET_ID, ��������ң���ֹ�������ӡ�


		//////////////////////////////////////////////////////////////////////
		// ����Ǵ������Ϣ
		SHELL_BASE					= 40000,
		SHELL_PRINTTEXT				= SHELL_BASE + 1,				// STRING, ���ķ��͵�TEXT��Ϣ
		SHELL_KERNELSTATE			= SHELL_BASE + 2,				// STRING, ���ķ��͵�STATE��������		
		SHELL_REMOTE_CLOSE			= SHELL_BASE + 3,				//  [11/28/2007 ���]	Զ�̹ر�

		//////////////////////////////////////////////////////////////////////
		// �����ݿ���Ĵ������Ϣ
		DB_BASE						= 50000,
		DB_QUERY_USERINFO			= DB_BASE + 1,

		DB_MSG_BASEINFO				= DB_BASE + 101,
		DB_MSG_FRIENDS				= DB_BASE + 102,
		DB_MSG_ENEMYS				= DB_BASE + 103,
		DB_MSG_ITEMS				= DB_BASE + 104,
		DB_MSG_WEAPON_SKILL			= DB_BASE + 105,
		DB_MSG_MAGICS				= DB_BASE + 106,
		DB_MSG_SYNATTR				= DB_BASE + 107,
};

struct CLIENTMSG_PACKET0			// ���յ�CLIENT��Ϣ
{
	SOCKET_ID		idSocket;			// SOCKET_ID
	OBJID	idPacket;
	int		nSize;
	ULONG	nIP;
	char	buf[1];				// ���ڽṹָ�룬����Ҫ�ռ�
};

struct NPCMSG_PACKET0			// ���յ�NPC��Ϣ
{
	OBJID	idNpc;
	OBJID	idPacket;
	int		nSize;
	char	buf[1];				// ���ڽṹָ�룬����Ҫ�ռ�
};

struct SENDCLIENTMSG_PACKET0
{
	SOCKET_ID		idSocket;			// SOCKET_ID
	OBJID	idPacket;
	int		nSize;
	char	buf[1];				// ���ڽṹָ�룬����Ҫ�ռ�
};

// struct SENDNPCMSG_PACKET0
// {
// 	OBJID	idNpc;				// �㲥��Ϣ��BCAST_NPCID
// 	OBJID	idPacket;
// 	int		nSize;
// 	char	buf[1];				// ���ڽṹָ�룬����Ҫ�ռ�
// };

struct TRANSMSG_PACKET0			// ת�����������̴������Ϣ
{
	SOCKET_ID		idSocket;			// SOCKET_ID
	OBJID			idNpc;				// ͬʱ֧��NPC����Ϣ
	OBJID	idPacket;
	int		nSize;
	int		nTrans;				// ��������
	char	buf[1];				// ���ڽṹָ�룬����Ҫ�ռ�
};

struct TRANSPHYMSG_PACK0 //ת����������ͼ��,����������Ϣ
{
	int				nPhyle;				//����
	SOCKET_ID		idSocket;			// SOCKET_ID
	OBJID			idPacket;			//��ID
	int				nSize;				//��С
	char			buf[ 1 ];			//����
};

struct USERNAMEMSG_PACKET0			// ��WORLDת������Ϣ��������ת��
{
	SOCKET_ID		idSocket;			// SOCKET_ID
	OBJID	idPacket;
	int		nSize;
	char	szName[_MAX_NAMESIZE];		// ת���������
	char	buf[1];				// ���ڽṹָ�룬����Ҫ�ռ�
};

struct USERIDMSG_PACKET0			// ��WORLDת������Ϣ����IDת��
{
	SOCKET_ID		idSocket;			// SOCKET_ID
	OBJID	idPacket;
	int		nSize;
	OBJID	idUser;				// ת���������
	char	buf[1];				// ���ڽṹָ�룬����Ҫ�ռ�
};


struct CHANGE_USERDATA
{
	SOCKET_ID	idSocket;
	int			nData;
};

struct CHANGE_NPCPROCESSID
{
	SOCKET_ID	idNpc;
	PROCESS_ID	idProcess;		// ΪSOCKET_NONEʱΪɾ��
};

struct ST_LOGIN
{
	SOCKET_ID	idSocket;
	OBJID		idUser;
	ULONG		nIP;
};

#ifdef MAPGROUP_KERNEL
struct ST_USERCHANGEMAPGORUP
{
	SOCKET_ID	idSocket;
	UserInfoStruct		info;
	ULONG		nIP;
};
#endif

#ifdef MAPGROUP_KERNEL
struct ST_NEWPLAYERSTATECHANGEMAPGORUP
{
	OBJID		idUser;
	char		newPlayerState[_MAX_NEWPLAYEREXAMSTATE];
};
#endif
	
#ifdef MAPGROUP_KERNEL
struct ST_PRESENT_NUMCHANGEMAPGORUP
{
	OBJID		idUser;
	int		nPresentNum[_MAX_PRESENT_NUM];
};
#endif
	
struct ST_TUTORSTATECHANGEMAPGORUP
{
	OBJID	idByStudent[3];
	OBJID	idUserByStudent[3];
	OBJID	idTutorByStudent[3];
	NAMESTR	szUserNameByStudent[3];
	NAMESTR	szTutorNameByStudent[3];

	OBJID	id;
	OBJID	idUser;
	OBJID	idTutor;
	NAMESTR	szUserName;
	NAMESTR	szTutorName;

	DWORD		m_dwExpToTutor;


};


struct CHANGEMAPGORUP_INFO0
{
	OBJID		idUser;
	int			nInfoType;
	char		info[1];
};

struct CHANGE_MAPGROUP
{
	OBJID		idUser;
	PROCESS_ID	idProcess;
};

struct ST_CHANGEMAPGROUP
{
	DWORD		idSourceProgress;//20070329����:��ǰ��ͼ��id.���ڷ��� 
	OBJID		idUser;
	OBJID		idMap;
	int			nPosX;
	int			nPosY;
	void		*lpBooth;//[��;���� 2009.05.12]
};

struct ST_CREATENEWNPC
{
	OBJID			id;
	OBJID			idMap;
	OBJID			idData;
	OBJID			idOwner;
	int				nData;
	unsigned short	usPosX;
	unsigned short	usPosY;
	unsigned short	usAction;
	unsigned short	usType;
	unsigned char	ucOwnerType;
	UINT            unCitywarType;
};

struct ST_CREATENEWNPCNEW
{
	OBJID			idMap;
	unsigned short	usPosX;
	unsigned short	usPosY;
	unsigned short	usType;
	OBJID			idGen;
};

struct DukDeleteObject
{
	template <typename T>
	void operator()(T* ptr)
	{
	    delete ptr;
	}
};


struct InstanceUseTimeInfo
{
	UINT  id;
	UINT  nUserId;
	UINT  nInstanceType;
	UINT  nUserTime;
	char  szCreateTime[32];
	UINT  nCanUser;
	UINT  nFinishTimes;
};

struct CAutoFightInfo
{
//	MSGHEAD_DEFINE
	UINT nAction;
	
	OBJID id  ;                 //�Զ�����������       
	OBJID idUser ;              //�Զ��һ��������õ����id
	//����
	OBJID AutoSwitch;		   //���п���
	unsigned short PercentHp;
	unsigned short PercentMp;
	unsigned short SkillHp;
	unsigned short SkillMp;
	unsigned short SkillTeamhp;
	unsigned short EudHp;
	unsigned short EudHappy;
	unsigned short HitRange;
	unsigned short DoubleExp;
	unsigned short DoubleEudExp;
	unsigned short LIYUCHUN;
	unsigned short temp;
	OBJID Hpmed1;
	OBJID Hpmed2;
	OBJID Hpmed3;
	OBJID Mpmed1;
	OBJID Mpmed2;
	OBJID Mpmed3;
	OBJID EudHpmed1;
	OBJID EudHpmed2;
	OBJID EudHpmed3;
	OBJID EudToy1;
	OBJID EudToy2;
	//  ����Ϊ�Զ��ָ������ֶΣ�
// 	UINT autoRestoration_hp ;  //�Ƿ����Զ��ָ������е�"������������"������Ϊ"1","2"��δ����Ϊ"0"��
// 	char aRhp_item[_MAX_NAMESIZE] ;           //ʹ�ú��ֵ������Զ��ָ�Ѫ������д����ID��
// 	char aRhp_item2[_MAX_NAMESIZE] ;          //ʹ�ú��ֵ������Զ��ָ�Ѫ������д����ID��
// 	UINT aRhpPercentage ;      //��Ѫ���������ֵ�ٷ�֮����ʱ�������Զ���Ѫ����д�ٷ�����
// 	UINT aRhpPercentage2 ;      //��Ѫ���������ֵ�ٷ�֮����ʱ�������Զ���Ѫ����д�ٷ�����
// 	UINT autoRestoration_mp;   //�Ƿ����Զ��ָ������е�"����ħ��ֵ����"������Ϊ"1","2"��δ����Ϊ"0"��
// 	char aRmp_Item[_MAX_NAMESIZE];            //ʹ�ú��ֵ������Զ��ָ�ħ��������д����ID��
// 	char aRmp_Item2[_MAX_NAMESIZE];            //ʹ�ú��ֵ������Զ��ָ�ħ��������д����ID��
// 	UINT armpPercentage ;      //��ħ�����������ֵ�ٷ�֮����ʱ�������Զ���ħ����д�ٷ�����
// 	UINT armpPercentage2 ;      //��ħ�����������ֵ�ٷ�֮����ʱ�������Զ���ħ����д�ٷ�����
// 
// // 	UINT autoRestoration_hp ;  //�Ƿ����Զ��ָ������е�"������������"������Ϊ"1"��δ����Ϊ"0"��
// // 	char aRhp_item[_MAX_NAMESIZE] ;           //ʹ�ú��ֵ������Զ��ָ�Ѫ������д����ID��
// // 	UINT aRhpPercentage ;      //��Ѫ���������ֵ�ٷ�֮����ʱ�������Զ���Ѫ����д�ٷ�����
// // 	UINT autoRestoration_mp;   //�Ƿ����Զ��ָ������е�"����ħ��ֵ����"������Ϊ"1"��δ����Ϊ"0"��
// // 	char aRmp_Item[_MAX_NAMESIZE];            //ʹ�ú��ֵ������Զ��ָ�ħ��������д����ID��
// // 	UINT armpPercentage ;      //��ħ�����������ֵ�ٷ�֮����ʱ�������Զ���ħ����д�ٷ�����
// 	// ����Ϊ�Զ��һ������ֶΣ�
// 	UINT AutoAttack_hp  ;      //�Ƿ����Զ��һ��е�"������������"������Ϊ"1"��δ����Ϊ"0"��
// 	char AAhp_item[_MAX_NAMESIZE] ;           //ʹ�ú��ֵ������Զ��ָ�Ѫ������д����ID��
// 	UINT AahpPercentage ;      //��Ѫ���������ֵ�ٷ�֮����ʱ�������Զ���Ѫ����д�ٷ�����
// 	UINT AutoAttack_mp ;      //�Ƿ����Զ��һ��е�"����ħ��ֵ����"������Ϊ"1"��δ����Ϊ"0"��
// 	char AAmp_item[_MAX_NAMESIZE]   ;        //ʹ�ú��ֵ������Զ��ָ�ħ��������д����ID��
// 	UINT AampPercentage ;     //��Ѫ���������ֵ�ٷ�֮����ʱ�������Զ���ħ����д�ٷ�����
// 	UINT Pickup ;             //�Զ�ʰȡ���ã�1�����Զ�ʰȡ��0δ�����Զ�ʰȡ��2ʰȡҩƷ��4ʰȡ���ϣ�8ʰȡ��ʯ��16ʰȡ�����飬32ʰȡ����Ʒ�����ϣ�64ʰȡ����Ʒ�����ϣ�128ʰȡ׿ԽƷ�����ϡ�
// 	UINT autousevas  ;         //�Զ�ʹ����ֵ���ߣ�����Ϊ"1"��δ����Ϊ"0"��2Ϊ�Զ�ʹ�ø���� 4Ϊ�Զ�ʹ���񽫴���Ϊ�Զ�ʹ��˫������ҩ��
// 	UINT autoshopping ;       //�Զ��������ã�1Ϊ�����Զ�����0Ϊδ������2Ϊ�Զ�����죬4Ϊ�Զ���������8Ϊ�Զ����򸴻�����ֶ���ֵΪ���ϸ��趨ֵ���������
// 	UINT num_hp ;             //�Զ������ҩ��������
// 	UINT num_mp ;             //�Զ�������ҩ��������
// 	UINT num_fh  ;            //�Զ����򸴻���������
// 	UINT num_double  ;        //�Զ�ʹ��˫������ҩ������
// 	UINT goal  ;              //�Զ����Ŀ���ѡ��0Ϊ����Ĺ֣�1Ϊ���ڹ����Լ��Ĺ֡�
// 	UINT way  ;               //�Զ���ַ�ʽ��ѡ��0Ϊ��ͨ������1Ϊ���ܹ�����
// 	UINT autoUseAssit;        //�Զ�ʹ�ø���ħ��
// 	UINT autoUseAttack;       //�Զ�ʹ�ù���ħ��
// 
// 	//���ﲿ��
// 	OBJID	dwEudemon1;
// 	OBJID	dwEudemon2;
// 	DWORD	dwEudemonDrinkType;				
// 	DWORD	dwEudemonDrinkRedID;			
// 	DWORD	dwEudemonDrinkBlueID;			
// 	DWORD	dwEudemonDrinkRedTypeID;			
// 	DWORD	dwEudemonDrinkBlueTypeID;			
// 	DWORD	dwEudemonPercentRed;			
// 	DWORD	dwEudemonPercentBlue;
// 
// 	char skills1[_MAX_NAMESIZE]   ;          //�Զ����ʹ�ü����趨��skills1~3Ϊ���漼�ܣ�skills4~7Ϊ�������ܣ���д����ID��
// 	char skills2[_MAX_NAMESIZE];
// 	char skills3[_MAX_NAMESIZE];
// 	char skills4[_MAX_NAMESIZE];
// 	char skills5[_MAX_NAMESIZE];
// 	char skills6[_MAX_NAMESIZE];
// 	char skills7[_MAX_NAMESIZE];
};

struct ChgInstanceUseInfo
{
	UINT nAction;
	InstanceUseTimeInfo nInfo;
};

typedef struct
{
	OBJID		idSyn;
	NAMESTR		szName;
	OBJID		idLeader;
	NAMESTR		szLeader;
	int			nMoney;
	char        szWord[1024];
}CreateSyndicateInfo;

typedef struct
{
	//[��;���� 2008/9/24]�ڿ�ͷ����idUser,nSex,nProfession������Ա
	OBJID			idUser;//��ұ�ʶ
	int				nSex;//����Ա�
	int				nProfession;//���ְҵ
	//[��;���� 2008/9/25]����bOnline
	bool			bOnline;//�Ƿ�����
	OBJID			idSyn;
	union {
		struct {
			int		nRank;				//���ɳ�Ա�ľ���
			int		nSynMemberLevel;	//���ɳ�Ա����
		};
		struct {
			DWORD	dwData[2];	
		};
	};
	unsigned short	nSize;
	unsigned char	ucFuncType;
	unsigned char	ucSaveFlag;
	union{
		long		IntParam[1];
		char		StrParam[1];
	};
}SynFuncInfo0;

struct GM_FLYPLAYER
{
	OBJID		UserID;
	int			MapData;
	int			MapX;
	int			MapY;
};//  [5/23/2007] ����  GM����ҵ�����
struct GM_STOPSAY
{
	char	szname[16];
	int		sec;
};//  [8/13/2007 Add By HaLei] 
struct ST_CHANGETEAM
{
	int				nAction;
	OBJID			idTeam;
	OBJID			idUser;
	int				nData1;
	int				nData2;
	int				nData3;
	int				nData4;
	char			teamName[24];//20070202����:С������.
};

struct	REMOTE_CALL0
{
	OBJID			idUser;
	unsigned short	nSize;
	unsigned char	ucFuncType;
	union{
		long		IntParam[2];
		char		StrParam[1];
	};
};

struct	VARIANT_SET0
{
	long		m_setLong[1];
	long&		IntParam(int idx)			{ return m_setLong[idx]; }
	char*		StrParam(int idx)			{ return (char*)&m_setLong[idx]; }
};


/////////////////////////////////////////
// DBService��	-- zlong 2004.09.06

struct	ST_QUERY_USERINFO
{
	OBJID	idUser;
};

const int		_MEDAL_BUFSIZE		= 64+1;
const int		_TITLE_BUFSIZE		= 64+1;

struct	ST_DB_USERINFO
{
	OBJID	idUser;

	// ����Ϊ��4�ֽڶ��룬û�а���ԭ����˳��
	DWORD	dwCoin_money;
	DWORD	dwLast_login;
	DWORD	dwLookface;
	DWORD	dwHair;
	char	setMedal[_MEDAL_BUFSIZE];
	UCHAR	ucLevel;
	UCHAR	ucMetempsychosis;
	UCHAR	ucNobility;
	DWORD	dwMedal_select;

	char	setTitle[_TITLE_BUFSIZE];
	UCHAR	ucProfession;
	UCHAR	ucAdditional_point;
	UCHAR	ucAuto_allot;
	DWORD	dwTitle_select;

	DWORD	dwMoney;
	DWORD	dwMoney_saved;
	USHORT	usRecordX;
	USHORT	usRecordY;
	//OBJID	id;
	OBJID	idAccount;
	OBJID	idRecordmap;
	OBJID	idSyndicate;
	int		nDeed;
	int		nExp;

	_int64	i64TaskMask;
	_int64	i64DayTaskMask;

	DWORD	dwVirtue;
	OBJID	idHome;
	DWORD	dwLockKey;
	//DWORD	dwCurrLockKey;
	char	szMate[_MAX_NAMESIZE];
	char	szName[_MAX_NAMESIZE];
	USHORT	usForce;
	USHORT	usConstitution;
	USHORT	usHealth;
	int		usLife;
	USHORT	usMana;
	USHORT	usSoul;
	USHORT	usSpeed;
	SHORT	sPk;
};

struct	ST_DB_FRIEND_INFO
{
		OBJID	id;
		OBJID	idUser;
		OBJID	idFriend;
		char	szFriendName[_MAX_NAMESIZE];
};

struct	ST_DB_FRIEND
{
	OBJID	idUser;

	int		nAmount;
	ST_DB_FRIEND_INFO	setFriends[1];
};

struct	ST_DB_ENEMY
{
	OBJID	idUser;
};

struct	ST_DB_ITEMINFO
{
	OBJID	id;

};
struct	ST_DB_ITEM
{
	OBJID	idUser;

	int		nAmount;
	ST_DB_ITEMINFO	setItems[1];
};

struct	ST_DB_WEAPON_SKILL
{
	OBJID	idUser;
};

struct	ST_DB_MAGIC
{
	OBJID	idUser;
};

struct	ST_DB_SYNATTR
{
	OBJID	idUser;
};

enum
{
	FRIEND_APPLY_ACTION_TYPE_APPLY_REQ,
	FRIEND_APPLY_ACTION_TYPE_APPLY_ANK,
	FRIEND_APPLY_ACTION_TYPE_APPLY_FIN,
	FRIEND_APPLY_ACTION_TYPE_APPLY_NANK,
};

//////////////////////////////////////////////////////////////////////////
//  [2007-9-10] ��� ���ͼ��Ӻ���
typedef struct  
{
	unsigned short	unAction;
	OBJID			idPlayer;
	NAMESTR			szPlayerName;
	int				nLookFace;
	int				nLev;
	int				nProfession;
	OBJID			idTarget;
	NAMESTR			szTargetName;
	int				nTargetLookFace;
	int				nTargetLev;
	int				nTargetProfession;
	int				nRet;
	int             nSex;
	int				nFriendType;
}ST_FRIEND_APPLY;

enum
{
	TEAM_INVITE_ACTION_TYPE_INVITE_REQ,
	TEAM_INVITE_ACTION_TYPE_INVITE_ANK,
	TEAM_INVITE_ACTION_TYPE_INVITE_NANK,
	TEAM_INVITE_ACTION_TYPE_INVITE_ANK_RETURN,
	TEAM_INVITE_ACTION_TYPE_JOIN_REQ,
	TEAM_INVITE_ACTION_TYPE_JOIN_ANK,
	TEAM_INVITE_ACTION_TYPE_JOIN_ANK_RETURN,
	TEAM_INVITE_ACTION_TYPE_JOIN_FINAL,
	TEAM_INVITE_ACTION_TYPE_JOIN_NANK,
	TEAM_INVITE_ACTION_TYPE_WAIT,
};

#define _MAX_TEAMMEMBER 6

struct AwardInfoStruct
{
	int nTime;
	int nLeave;
};

struct TeamInfoStruct
{
	OBJID	id;
	OBJID	idLeader;
	OBJID	setMember[_MAX_TEAMMEMBER];
		//--------add by cz �ӳ���ְҵ
	DWORD   setProfession[_MAX_TEAMMEMBER];
	BOOL	bCloseMoney;
	BOOL	bCloseItem;
	int     nLeaderlev;
	//------------------
	int							nTeamMode;
	int							nExpMode;
	int							nItemMode;
	char						sTeamName[24];
	//------------------
	//------------------
	char                        m_sLeadName[16];
	int                         m_nLev;
	int                         m_nTarget;
	char                        m_sTeamText[96];

};

typedef struct TeamMemberInfo_Tag
{
	char			szName[_MAX_NAMESIZE];
	OBJID			id;
	DWORD			dwLookFace;
	DWORD           dwLev;
	USHORT			usProfession;
	USHORT			usProLev;
	BOOL			bOnlineStatus;///��ʶ�˶�Ա������״̬,1������,0�ǲ�����
	OBJID			idMap;///����ʱ�˶�Ա���ڵĵ�ͼid
	USHORT			usPosX;///����ʱ���ڵ�X����
	USHORT			usPosY;///����ʱ���ڵ�Y����
	TeamMemberInfo_Tag()
	{
		::memset(this,0x0,sizeof(this));
		bOnlineStatus=true;
		idMap=0;///����ʱ�˶�Ա���ڵĵ�ͼid
		usPosX=0;///����ʱ���ڵ�X����
		usPosY=0;///����ʱ���ڵ�Y����
	}
}TeamMemberInfo;

//////////////////////////////////////////////////////////////////////////
//  [2007-9-7] ��� ���ͼ���������
typedef struct  
{
	unsigned short	unAction;
	OBJID			idPlayer;
	NAMESTR			szPlayerName;
	SOCKET_ID		idSocket;
	OBJID			idTarget;
	NAMESTR			szTargetName;
	SOCKET_ID		idSocketTarget;	
	int				nRet;	//	����
							/*
							��1λΪ�Ƿ��Զ����
							��2λΪ�Ƿ���������
							��3λΪ�������ȡ��
							��4λΪ����ֵƽ������
							��5λΪ��Ʒ�Ƿ����ȡ��
							��6λΪ��Ʒ�Ƿ�����ʰȡ
							��7λΪ��Ʒ�Ƿ��������
	*/
	
	TeamInfoStruct		teamInfo;
	
	int				nMemberInfoCount;
	TeamMemberInfo	teamMemberInfo[_MAX_TEAMMEMBER];

}ST_TEAM_INVITE;

//goto �޸�ʦͽΪ���½ṹ
// struct TutorInfoStruct
// {
// 	OBJID	id;
// 	OBJID	idUser;		//ͽ��ID
// 	OBJID	idTutor;	//ʦ��ID
// 	NAMESTR	szUserName;	//ͽ������
// 	NAMESTR	szTutorName;//ʦ������
// 	UINT	nImpart;	//���ڶ�
// 	short	nIsGraduated;//�Ƿ��ѳ�ʦ
// 	short	nHasGift;	//ʦ���Ƿ����ѳ�ʦ��ͽ�ܵĽ���
// 	DWORD	nGiftMask;	//ͽ�ܽ�������
// 	DWORD	nTutorMask;	//ʦ����������
// 	UINT	nStuLev;	//ͽ�ܵȼ�
// 	UINT	nTutorLev;	//ʦ���ȼ�
// 	UINT	nTutorImpartLv;//ʦ��ʦ�µȼ�
// };
enum CHGTUTOR_DATA
{
	CHGTUTORDATA_CLEARTUTOR,
	CHGTUTORDATA_REQDELTUTOR,
	CHGTUTORDATA_REQDELSTUDENT,
	CHGTUTORDATA_RECOVERTUTOR,
	CHGTUTORDATA_RECOVERSTUDENT,
	CHGTUTORDATA_REQADDDENSITY,
	CHGTUTORDATA_CANCELLEAVETUTOR,
	CHGTUTORDATA_CANCELDELSTUDENT,
};

const int APPLYDEL_TUTOR   = 1;     //ѧ������ɾ����ʦ
const int APPLYDEL_STUDENT = 2;     //��ʦ����ɾ��ѧ��

const int MAX_DENSITY      = 10000;  //������ܶ�

struct ChgTutorInfo
{
	UINT  nAction;
	OBJID idUser;
	OBJID idTutor;
	OBJID idStudent;
	OBJID id;
	int   nTutorImpart;
};

//////////////////////////////////////////////////////////////////////
// C/S Э��Ŀͻ��˲���	

// �ò��ֳ������������������������ʹ��
const int	PROTOCOL_MSG_HEADSIZE		= 4;
struct CProtocolMsgStruct{
	unsigned short	nMsgSize;
	unsigned short	idPacket;
	char			pMsg[1];
};

struct ST_SHOP
{
	int				nAction;
	OBJID			idUser;
	OBJID			idItem;
	UINT			uState;
	OBJID			idOwer;
	UINT			uPrice;
	OBJID			idShop;
	UINT			uNumber;
	OBJID			idShopItem;
};

//////////////////////////////////////////////////////////////////////////
//-----�޸ĵ�ͼ���û���buffer
struct CHANGE_USER_STATE
{
	OBJID m_userid;
	DWORD m_Stateid;
	DWORD m_power;
	int  m_nSecs; 
    int m_nTimes; 
	int m_nParam1;
	int m_nParam2;
	int m_nParam3;
	bool m_IsAttach;
};

extern BYTE g_cbEncryptKey;	//���ܼ�ֵ

#define ENCRYPTPACKET

#ifdef ENCRYPTPACKET

#define ENCRYPT_KEY 0x3e
inline void EncryptPacket(char* pBuf,int nBufLen)
{
	CHECK(pBuf && nBufLen);
	if(g_cbEncryptKey==0)///�����ֽ�ֵΪ0ʱ��ʾ����������ʱ�������
		return;
	short nMsgType=0;
	try
	{
		///����������ֻ���ܳ���Ϣ�������������ݣ�������Ϣ����
		pBuf+=2;
		nBufLen-=2;
		
		memcpy(&nMsgType,pBuf,sizeof(short));
		if(nMsgType==1002) 
			return;

		for(int i=0;i<nBufLen;++i)
		{
			pBuf[i]^=g_cbEncryptKey;
		}
	}
	catch(...)
	{
		LOGERROR("EncryptPacket ������,���찡!");
	}
}
#else
#define EncryptPacket
#endif

// �ֽ���Ϣ��
// return: netpacket size
inline int	SplitPacket(char* pBuf, int nBufLen, OBJID* pidPacket, char** ppMsg, int* pMsgSize)
{
	if(nBufLen > PROTOCOL_MSG_HEADSIZE)
	{
 		EncryptPacket(pBuf,nBufLen);
		CProtocolMsgStruct*	pMsgPtr = (CProtocolMsgStruct*)pBuf;
		if(pMsgPtr->nMsgSize <= nBufLen)
		{
			if(pMsgPtr->nMsgSize < 4 || pMsgPtr->nMsgSize > MAX_PACKETSIZE)
				return 0;

			*pidPacket	= pMsgPtr->idPacket;
			*pMsgSize	= pMsgPtr->nMsgSize - PROTOCOL_MSG_HEADSIZE;
			*ppMsg		= pMsgPtr->pMsg;
			return pMsgPtr->nMsgSize;
		}
	}
	return 0;
}




// �ϳ���Ϣ��
// return: netpacket size
inline int	UnitePacket(char* pBuf, int nBufLen, OBJID idPacket, const char* pMsg, int nMsgSize, unsigned int& nMsgType)
{
	if(nBufLen >= nMsgSize + PROTOCOL_MSG_HEADSIZE)
	{
		CProtocolMsgStruct*	pMsgPtr = (CProtocolMsgStruct*)pBuf;
		pMsgPtr->idPacket		= (unsigned short)idPacket;
		nMsgType = pMsgPtr->idPacket;
		pMsgPtr->nMsgSize		= nMsgSize + PROTOCOL_MSG_HEADSIZE;
		memcpy(pMsgPtr->pMsg,	pMsg, (size_t)nMsgSize);

		EncryptPacket((char*)pMsgPtr,pMsgPtr->nMsgSize);///�Գ��˰�ͷ֮������ݽ��������
		return pMsgPtr->nMsgSize;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////
// C/S Э���NPC����	

// �ò��ֳ������������������������ʹ��
const int	PROTOCOL_NPCMSG_HEADSIZE		= 8;
struct CProtocolNpcMsgStruct{
	unsigned short	nMsgSize;
	unsigned short	idPacket;
	unsigned long	idNpc;
	char			pMsg[1];
};

// �ֽ���Ϣ��
// return: netpacket size
inline int	SplitNpcPacket(const char* pBuf, int nBufLen, OBJID* pidPacket, char** ppMsg, int* pMsgSize, OBJID* pidNpc)
{
	if(nBufLen > PROTOCOL_NPCMSG_HEADSIZE)
	{
		CProtocolNpcMsgStruct*	pMsgPtr = (CProtocolNpcMsgStruct*)pBuf;
		if(pMsgPtr->nMsgSize <= nBufLen)
		{
			if(pMsgPtr->nMsgSize < 4 || pMsgPtr->nMsgSize > MAX_PACKETSIZE)
				return 0;

			*pidPacket	= pMsgPtr->idPacket;
			*pMsgSize	= pMsgPtr->nMsgSize - PROTOCOL_NPCMSG_HEADSIZE;
			*pidNpc		= pMsgPtr->idNpc;
			*ppMsg		= pMsgPtr->pMsg;
			return pMsgPtr->nMsgSize;
		}
	}
	return 0;
}

// �ϳ���Ϣ��
// return: netpacket size
inline int	UniteNpcPacket(char* pBuf, int nBufLen, OBJID idPacket, const char* pMsg, int nMsgSize, OBJID idNpc)
{
	if(nBufLen >= nMsgSize + PROTOCOL_NPCMSG_HEADSIZE)
	{
		CProtocolNpcMsgStruct*	pMsgPtr = (CProtocolNpcMsgStruct*)pBuf;
		pMsgPtr->idPacket		= (unsigned short)idPacket;
		pMsgPtr->nMsgSize		= nMsgSize + PROTOCOL_NPCMSG_HEADSIZE;
		pMsgPtr->idNpc			= idNpc;
		memcpy(pMsgPtr->pMsg,	pMsg, (size_t)nMsgSize);

		return pMsgPtr->nMsgSize;
	}
	return 0;
}

const int    MALE_ASSIST		=	0x00000001;//�и���	1  ����
const int    MALE_MASTER	    =	0x00000002;//�з�ʦ	2  ȫ��
const int    MALE_PASTOR	  	=	0x00000004;//����ʦ	4  ��ü
const int    MALE_ASSISSIN		=	0x00000008;//�д̿�	8  ؤ��
const int    MALE_SOLDIER	    =	0x00000010;//��սʿ	16 ����
const int    MALE_SWORD  		=	0x00000020;//�й���	32 ���

const int    FEMALE_ASSIST		=	0x00000040;//Ů����	64
const int    FEMALE_MASTER	    =	0x00000080;//Ů��ʦ	128
const int    FEMALE_PASTOR  	=	0x00000100;//Ů��ʦ	256
const int    FEMALE_ASSISSIN	=	0x00000200;//Ů�̿�	512
const int    FEMALE_SOLDIER		=	0x00000400;//Ůսʿ	1024
const int    FEMALE_SWORD		=	0x00000800;//Ů����	2048

const int    MALE_NEWHAND		=	0x00001000;//������	4096
const int    FEMALE_NEWHAND	    =	0x00002000;//Ů����	8192

const int	PROF_MALE		=	MALE_ASSIST + MALE_MASTER + MALE_PASTOR + MALE_ASSISSIN + MALE_SOLDIER + MALE_SWORD + MALE_NEWHAND;//��
const int	PROF_FEMALE		=	FEMALE_ASSIST + FEMALE_MASTER + FEMALE_PASTOR + FEMALE_ASSISSIN + FEMALE_SOLDIER + FEMALE_SWORD + FEMALE_NEWHAND;//Ů	


const int	PROF_ASSIST		=	MALE_ASSIST + FEMALE_ASSIST;//����
const int	PROF_MASTER	    =	MALE_MASTER + FEMALE_MASTER;//��ʦ
const int	PROF_PASTOR	    =	MALE_PASTOR + FEMALE_PASTOR ;//��ʦ
const int	PROF_ASSISSIN	=	MALE_ASSISSIN + FEMALE_ASSISSIN;//�̿�
const int	PROF_SOLDIER	=	MALE_SOLDIER + FEMALE_SOLDIER;//սʿ
const int	PROF_SWORD	    =	MALE_SWORD + FEMALE_SWORD ;//����
const int	PROF_NEWHAND	=	MALE_NEWHAND + FEMALE_NEWHAND;//����

const int   ASALE_ITEM_AMOUNT = 2;//ÿ���ؼ���Ʒ����,Ҫ����ʱֱ�Ӹ�������������4��

const int _SPCITEM_VASEXCHANGE	=351001;//Ԫ��Ʊ
const int _SPCITEM_EXPEXCHANGE	=351002;//���鵤
#endif // PROTOCOL_H
