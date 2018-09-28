///////////////////////////////////////////////////////////
//
//		CMsgPackage.h
//

#ifndef		MSGPACKAGE_H
#define		MSGPACKAGE_H

#include "NetMsg.h"

enum{
		MSGPACKAGE_QUERYLIST			= 0,
		MSGPACKAGE_CHECKIN				= 1,
		MSGPACKAGE_CHECKOUT				= 2,
		MSGPACKAGE_QUERYLIST2			= 3,	// �ͻ����յ�����Ϣ��ʾ׷�Ӳֿ���Ʒ�������ԭ����
		MSGPACKAGE_SYNCHRO_AMOUNT       = 4,    //ͬ���ֿ�����Ʒ������
		MSGPACKAGE_CHECKIN_MONEY        = 5,    //��Ǯ
		MSGPACKAGE_CHECKOUT_MONEY       = 6,    //ȡǮ
		MSGPACKAGE_MONEY_AMOUNT         = 7,    //���ʹ����
		MSGPACKAGE_MOVE		            = 8,    //������Ʒ�ƶ�λ��
		MSGPACKAGE_EXCHANGE	            = 9,    //������Ʒ����λ�û�ϲ� toserver
		MSGPACKAGE_CHECKIN_EUDEMON		= 10,	//����浽�ֿ�
		MSGPACKAGE_CHECKOUT_EUDEMON		= 11,	//�Ӳֿ�ȡ������
};

enum{	// type
		MSGPACKAGE_TYPE_NONE			= 0,
		MSGPACKAGE_TYPE_STORAGE			= 10,
		MSGPACKAGE_TYPE_TRUNK			= 20,
		MSGPACKAGE_TYPE_CHEST			= 30,
		MSGPACKAGE_TYPE_EUDEMON_BROODER	= 40,	// [������MsgEudPackage��]
		MSGPACKAGE_TYPE_EUDEMON_STORAGE	= 50,	// [������MsgEudPackage��]
		MSGPACKAGE_TYPE_AUCTION_STORAGE = 60,	// �������õ�����ƷLW
		MSGPACKAGE_TYPE_EUD_BROOD_PACKAGE = 70,	// ������ϵĻ��޷����� // [������MsgEudPackage��]
		PACKAGE_MAGICSTONEBOX	 = 80,	// ħ����
		MSGPACKAGE_TYPE_MARRYSTORAGE = 90,  //���ļ�԰�ֿ�
		MSGPACKAGE_TYPE_BED_ADDMATERIAL1 = 100, //��--��Ӳ���1
		MSGPACKAGE_TYPE_BED_ADDMATERIAL2 = 110, //��--��Ӳ���2
		MSGPACKAGE_TYPE_BED_ADDMATERIAL3 = 120, //��--��Ӳ���3

		MSGPACKAGE_TYPE_DEPORT_ADDMATERIAL1 = 130, //����--��Ӳ���1
		MSGPACKAGE_TYPE_DEPORT_ADDMATERIAL2 = 140, //����--��Ӳ���2
		MSGPACKAGE_TYPE_DEPORT_ADDMATERIAL3 = 150, //����--��Ӳ���3

		MSGPACKAGE_TYPE_FLOWER_ADDMATERIAL1 = 160, //����--��Ӳ���1
		MSGPACKAGE_TYPE_FLOWER_ADDMATERIAL2 = 170, //����--��Ӳ���2
		MSGPACKAGE_TYPE_FLOWER_ADDMATERIAL3 = 180, //����--��Ӳ���3

		MSGPACKAGE_TYPE_CABINET_ADDMATERIAL1 = 190, //����--��Ӳ���1
		MSGPACKAGE_TYPE_CABINET_ADDMATERIAL2 = 200, //����--��Ӳ���2
		MSGPACKAGE_TYPE_CABINET_ADDMATERIAL3 = 210, //����--��Ӳ���3
};


class CMsgPackage : public CNetMsg
{
public:
	CMsgPackage();
	virtual ~CMsgPackage();

public:		//�ӿں���
	// modified by zlong 2003-11-24 ---- Create�������ucType����ָ�����
// 	BOOL			Create(int nAction, OBJID id, const MsgPackageItemInfo* buf, int nSize, UCHAR ucType);
// 	BOOL			Create(int nAction, OBJID id, OBJID idItem, UCHAR ucType);
	BOOL			Create(int nAction, OBJID id, OBJID idType, UINT usAmount, UINT usMaxAmount, int nIndex, int nPosition=MSGPACKAGE_TYPE_STORAGE, int isBind=0);//[2010-06-11 goto]
public:
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(CGameSocket* pSocket);

private:
// 	typedef struct
// 	{
// 		MSGHEAD_DEFINE
// 
// 		OBJID		id;						// userid, npcid, trunkid
// 		UCHAR		ucAction;
// 		UCHAR		ucType;
// 		union {
// 			struct{							// to client: size
// 				USHORT		usSize;	
// 				MsgPackageItemInfo	setItemInfo[1];
// 			};
// 			OBJID		idItem;			// to client/server: action
// 		};
// 	}MSG_Info;
	typedef struct 
	{
		MSGHEAD_DEFINE

		OBJID	id;			//��ƷID
		OBJID	idType;		//��Ʒtype
		UINT	usAmount;	//����
		UINT	usMaxAmount;//��������
		UCHAR	ucPosition;	//λ��
		UCHAR	ucIsBind;	//�Ƿ��
		USHORT	usIndex;	//�ֿ��ڵ�λ��(ҳ��)
		UCHAR	ucAction;	//����
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
};

#endif // MSGPACKAGE_H