// MsgItem.h: interface for the CMsgItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGITEM_H__9AB1BCEA_F6D1_4413_A39D_8AC35C9FF655__INCLUDED_)
#define AFX_MSGITEM_H__9AB1BCEA_F6D1_4413_A39D_8AC35C9FF655__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

WORLDKERNEL_BEGIN
enum {	ITEMACT_NONE			=0, 
		ITEMACT_BUY,					// to server, id: idNpc, data: idItemType
		ITEMACT_SELL,	 				// to server, id: idNpc, data: idItem
		ITEMACT_DROP,					// to server, x, y
		ITEMACT_USE,					// to server, data: position
		ITEMACT_EQUIP			=5,	    // to client
		ITEMACT_UNEQUIP,				// to server, data: position
		ITEMACT_REQUEST_SPLITITEM,				// to server, data: num
		ITEMACT_REQUEST_COMBINEITEM,			// to server, data: id
		ITEMACT_QUERYMONEYSAVED,		// to server/client, id: idNpc, data: nMoney
		ITEMACT_SAVEMONEY		=10,	// to server, id: idNpc, data: nMoney
		ITEMACT_DRAWMONEY,				// to server, id: idNpc, data: nMoney
		ITEMACT_DROPMONEY,	 			// to server, x, y
		ITEMACT_SPENDMONEY,				// �����壬�ݱ���
		ITEMACT_REPAIR,					// to server, return MsgItemInfo
		ITEMACT_REPAIRALL		=15,	// to server, return ITEMACT_REPAIRAll, or not return if no money
		ITEMACT_IDENT,					// to server, return MsgItemInfo
		ITEMACT_DURABILITY,				// to client, update durability
		ITEMACT_DROPEQUIPMENT,			// to client, delete equipment
		ITEMACT_IMPROVE,				// to server, improve equipment
		ITEMACT_UPLEV			=20,	// to server, upleve equipment
		ITEMACT_BOOTH_QUERY,			// to server, open booth, data is npc id
		ITEMACT_BOOTH_ADD,				// to server/client(broadcast in table), id is idItem, data is money
		ITEMACT_BOOTH_DEL,				// to server/client(broadcast), id is idItem, data is npc id
		ITEMACT_BOOTH_BUY,				// to server, id is idItem, data is npc id
		ITEMACT_SYNCHRO_AMOUNT	=25,	// to client(data is amount)
		ITEMACT_FIREWORKS,				
		
		ITEMACT_COMPLETE_TASK,			// to server, ���Ӷ������, id=����ID, dwData=�ύ����Ʒ�����û����ΪID_NONE
		ITEMACT_EUDEMON_EVOLVE,			// to server, ���޽���, id=������ƷID��dwData=�����ɵ����ͣ�ȡֵ��Χ1~2
		ITEMACT_EUDEMON_REBORN,			// to server, ���޸��id=������ƷID, data=��ʯID
		ITEMACT_EUDEMON_ENHANCE	=30,	// to server, ����ǿ����id=������ƷID, data=��ʯID
		ITEMACT_CALL_EUDEMON,			// to server, �ٻ����ޣ�id=������ƷID
		ITEMACT_KILL_EUDEMON,			// to server, �ջػ��ޣ�id=������ƷID
										// to client, �ջػ��޵Ĺ�Ч���֣�id=������ƷID, dwData=����ID
		ITEMACT_EUDEMON_EVOLVE2,		// to server, ���޶��ν���, id=������ƷID��dwData=�����ɵ����ͣ�ȡֵ��Χ1~8
		ITEMACT_EUDEMON_ATKMODE,		// to server, ���û��޹���ģʽ, id=������ƷID��dwData=����ģʽ��ȡֵ��Χ1~3
		ITEMACT_ATTACH_EUDEMON	=35,	// �������˺���, id=������ƷID
		ITEMACT_DETACH_EUDEMON,			// ����������˺���״̬, id=������ƷID
		ITEMACT_EUDEMON_EVO_CHECKIN,	// �����������npc, to server,client id=NPCid dwData=������Ʒid
		ITEMACT_EUDEMON_EVO_CHECKOUT,	// �ӽ���NPC��ȡ��, to server id=NPCid dwData=������Ʒid	
		ITEMACT_EXCHANGEPOSOK	=39,	//	06.10.30�����޸�,to client,�ı���Ʒ�ڲ�ͬ�����е�λ��,�ɹ�.
		ITEMACT_SAVEDIAMOND		= 40,			//����ʯ
		ITEMACT_DRAWDIAMOND		= 41,			//ȡ��ʯ
//		ITEMACT_ICON_DROP_IN_SAME_PACKAGE = 42,	//20060921����:ͬһ����iconDrop.
		ITEMACT_REQUEST_TRADEPACKIDX	= 43,		//06.10.31�������,to server,���󻥻�������Ʒ��λ��(��ֻ֧��ͬһ�����в���)
		ITEMACT_REQUEST_CHGPACKIDX		= 44,		//06.10.28�������,to server,����ı���Ʒ��package_index����,��ͬһ������
		ITEMACT_SYNCHRO_PACKAGEINDEX	= 45,		//06.10.28�������,to client( data is packageIndex )
		ITEMACT_REQUEST_CHGPOSANDIDX	= 46,		//06.10.30�������,to server,����ı���Ʒ��position and package_index
		ITEMACT_SYNCHRO_UNLOCK			= 47,		//06.10.30�������,to client,����ͬ���ͻ���,������Ʒ 
		ITEMACT_SYNTHESIS_REQUEST		= 48,		//20061103����.C2S�ϳ�����
		ITEMACT_SYNTHESIS_SUCCESS		= 49,		//20061103����.S2C�ϳɳɹ�
		ITEMACT_SYNTHESIS_FAIL			= 50,		//20061103����.S2C�ϳ�ʧ��
		ITEMACT_STRENGTHAN				= 51,		//06.12.26�������,to server,����װ��ǿ��
		ITEMACT_UPDATE_CALCULATED_PROPERTY = 52,	//20070110����.S2C.���¿ͻ���hero����.�ͻ��˵�������.
		ITEMACT_TEAMPICKUP				=53,			//softworms,���ʰȡ��Ʒ
		ITEMACT_SYNCHRO_AMOUNTLIMIT	        =54,    //  [4/29/2007]�½������ to client(data is amountLIMIT)
		ITEMACT_SYNCHRO_GEM		        =55, //2007828����:S2Cͬ����������.
		ITEMACT_SYNCHRO_UNENCHANT		=56, //2007828����:S2C�����ħ.
		ITEMACT_USE_OK					=57, //[��;���� 2008/10/24]��Ʒʹ�óɹ�

		ITEMACT_SAVEITEMMONEYGET        =59, //�����ҵ���,��Ǯ���
		ITEMACT_SAVEITEMWSAGET          =60, //����ֵ�������
		ITEMACT_SAVEITEMOK              =61, //�����ȵ��߳ɹ�
		ITEMACT_BOOTHBUY_ADD			=62,	//[��;���� 2008/12/19]��̯�չ���Ʒ
		ITEMACT_BOOTH_BUYBUY			=63,//[��;���� 2008/12/24]�չ���Ʒ
		ITEMACT_BOOTH_DELBUY			=64,//[��;���� 2008/12/24]ɾ���չ���Ʒ
		ITEMACT_VIEWITEMINFO			=65, //[������� 2008/12/03]�鿴���촰������Ʒ��Ϣ
		ITEMACT_VIEWITEMAMOUNT          =66, //�鿴�����Ʒ�Ĳ���
		ITEMACT_REQUEST_TRADEPOSANDIDX  =67, //[2009.3.26--�Ŷ�]���������Ʒ
		ITEMACT_SAVEITEMGIVEUP          =68, //������ص���
		ITEMACT_PET_EXCHANGE            =69, //���ｻ��

		
		ITEMACT_ADD						=100, 
		ITEMACT_GETINFO					=101,
		ITEMACT_ORDER					=102,
		ITEMACT_BOOTH_CHGMONEY          =103,
		ITEMACT_BOOTH_CLRLEAVEWORD      =104,
		ITEMACT_GETEUDEMONINFO			=105,
		ITEMACT_GETEUDEMONLIST			=106,

		
		ITEMACT_BOOTH_ADDEUDE			=107,
		ITEMACT_BOOTH_DELEUDE			=108,
		ITEMACT_BOOTH_BUYEUDE			=109,
		ITEMACT_BOOTH_CHGEUDE			=110,
		ITEMACT_QUERYEUDEMONINFO		=111,///�����ѯ������Ϣ
		ITEMACT_QUERYITEMINFO			=112,///�����ѯ������Ϣ

};

class CMsgItem : public world_kernel::CNetMsg  
{
public:
	CMsgItem();
	virtual ~CMsgItem();

	BOOL	Create		(USHORT nAction,DWORD dwData, OBJID idTarget=ID_NONE,USHORT usPosX = 0,USHORT usPosY = 0, OBJID id = ID_NONE, OBJID	typeId = ID_NONE, DWORD	nAmount = 1, DWORD	nMaxAmount = 1, UCHAR	ucPosition = 0, UCHAR	ucBind = 0, USHORT	usIndex = 0,DWORD dwMoney = 0);
	BOOL	Add			(OBJID id,OBJID typeId,DWORD	nAmount,DWORD nMaxAmount, UCHAR ucPosition, UCHAR ucBind, USHORT	usIndex,DWORD dwMoney = 0);
public:	
	BOOL	Create		(char* pMsgBuf, DWORD dwSize);
	void	Process		(void* pInfo);

//protected:
	struct itemMiniInfo
	{
		OBJID	id;
		OBJID	typeId;
		DWORD	nAmount;
		DWORD	nMaxAmount;
		UCHAR	ucPosition;
		UCHAR	ucBind;
		USHORT	usIndex;
		DWORD	dwMoney;
	};
	typedef struct {
		MSGHEAD_DEFINE
		USHORT		usAction;
		USHORT		usAmount;
		DWORD		dwData;
		OBJID		idTarget;
		USHORT		usPosX;
		USHORT		usPosY;
		char        szTargetName[16];
		itemMiniInfo m_itemInfo[1];
	}MSG_Info;

	MSG_Info*	m_pInfo;
};
WORLDKERNEL_END
#endif // !defined(AFX_MSGITEM_H__9AB1BCEA_F6D1_4413_A39D_8AC35C9FF655__INCLUDED_)
