// MsgCompound.h: interface for the CMsgCompound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGCOMPOUND_H__9FE3B9EF_9DD9_4F14_902B_8E7003F5B884__INCLUDED_)
#define AFX_MSGCOMPOUND_H__9FE3B9EF_9DD9_4F14_902B_8E7003F5B884__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NETWORK\NetMsg.h"

const int COMP_CREAT = 8001;
const int COMP_STRENG = 8002;
const int COMP_MAGIC = 8003;
const int COMP_DICK = 8004;
const int COMP_GEN = 8005;
const int COMP_SUC_LEV1 = 20;//1��ǿ����ʯǿ���ɹ��� 20%
const int COMP_SUC_LEV2 = 8;//2��ǿ����ʯǿ���ɹ��� 8%
const int COMP_SUC_LEV3 = 3;//3��ǿ����ʯǿ���ɹ��� 3%
const int COMP_SUC_LEV4 = 1;//4��ǿ����ʯǿ���ɹ��� 1%
const int COMP_SUC_FAIL_RATE = 10;//ǿ�����߼���ʧ�ܺ�ÿ��ǿ����ʯ�ĳɹ��ʼӳ� 0.01%
const int COMP_MAX_EQUIPLEV	 = 10;//ǿ�����߼��ǵ���ߵȼ�

const int COMP_MAKEHOLE_COST	= 30000;//װ����׵���������
const int COMP_EMBEDGEM_COST	= 5000;//װ����Ƕ����������
const int COMP_TAKEOUTGEM_COST	= 5000;//װ��ȡ����ʯ����������

//VIP�ɹ��ʼӳ�
const int COMP_VIP_SUC_ADD2		= 1;//2��VIP
const int COMP_VIP_SUC_ADD3		= 2;

enum
{
	MSGCOMPOUND_EQUIPCREATE		= 5,
	MSGCOMPOUND_EMBEDEQUIP		= 6,
	MSGCOMPOUND_EMBED_SUC		= 9,
	MSGCOMPOUND_EMBED_FAL		= 10,
	MSGCOMPOUND_EQUIPCREATE_SUC  = 14,
	MSGCOMPOUND_MAKEHOLE			= 23,
	MSGCOMPOUND_MAKEHOLE_SUC		= 24,
	MSGCOMPOUND_MAKEHOLE_FAL		= 25,
	MSGCOMPOUND_EQUIPCREATE_FAL		= 26,
	MSGCOMPOUND_EQUIP_DECOMPOUND		= 60,
	MSGCOMPOUND_EQUIP_TRANS				= 61,
	//[2010-07-19 goto]�������
	MSGCOMPOUND_EQUIP_LEVUP				= 62,//[2010-07-19 goto] װ��ǿ��		C-S
	MSGCOMPOUND_EQUIP_LEVUP_SUC			= 63,//[2010-07-19 goto] װ��ǿ���ɹ�	S-C
	MSGCOMPOUND_EQUIP_LEVUP_FAIL		= 64,//[2010-07-19 goto] װ��ǿ��ʧ��	S-C
	
	MSGCOMPOUND_EQUIP_ADDSTAR			= 65,//[2010-07-19 goto] װ������		C-S
	MSGCOMPOUND_EQUIP_ADDSTAR_SUC		= 66,//[2010-07-19 goto] װ�����ǳɹ�	S-C
	MSGCOMPOUND_EQUIP_ADDSTAR_FAIL		= 67,//[2010-07-19 goto] װ������ʧ��	S-C

	MSGCOMPOUND_PUT_EQUIP				= 68,//[2010-07-20 goto] �û�����Ҫǿ����װ������ʱ���ͻ��˷���װ����ʧ�ܼӳ�
	MSGCOMPOUND_EQUIP_FAILRATE			= 69,//[2010-07-20 goto] װ����ʧ�ܼӳ�

	MSGCOMPOUND_EMBED_TAKEOUTGEM		= 70,//[2010-07-21 goto] ȡ����ʯ
	MSGCOMPOUND_EMBED_TAKEOUTGEM_SUC	= 71,//[2010-07-21 goto] ȡ����ʯ�ɹ�
	MSGCOMPOUND_EMBED_TAKEOUTGEM_FAIL	= 72,//[2010-07-21 goto] ȡ����ʯʧ��
	
	MSGCOMPOUND_GEM_LEVUP				= 73,//[2010-08-04 goto] ��ʯ�ϳɣ�3-5���ͼ���ʯ�ϳ�һ���߼���ʯ
	MSGCOMPOUND_GEM_LEVUP_SUC			= 74,//[2010-08-04 goto] ��ʯ�ϳɳɹ�
	MSGCOMPOUND_GEM_LEVUP_FAIL			= 75,//[2010-08-04 goto] ��ʯ�ϳ�ʧ��

	MSGCOMPOUND_EQUIP_SOULMARK			= 76,//goto ��ӡ
	MSGCOMPOUND_EQUIP_SOULMARK_SUC		= 77,
	MSGCOMPOUND_EQUIP_SOULMARK_FAIL		= 78,

	MSGCOMPOUND_GEM_CUTWORK				= 79,//��ʯ����
	MSGCOMPOUND_GEM_CUTWORK_SUC			= 80,
	MSGCOMPOUND_GEM_CUTWORK_FAIL		= 81,

	MSGCOMPOUND_EQUIP_SOULADD				= 82,//����
	MSGCOMPOUND_EQUIP_SOULADD_SUC			= 83,
	MSGCOMPOUND_EQUIP_SOULADD_FAIL		= 84,
	
	MSGCOMPOUND_EQUIP_SOULEXCHANGE				= 85,//����ת��
	MSGCOMPOUND_EQUIP_SOULEXCHANGE_SUC			= 86,
	MSGCOMPOUND_EQUIP_SOULEXCHANGE_FAIL		= 87,
//modify code by �ֵ�Ԩ begin 2011.7.13
	MSGCOMPOUND_ARTIFIC					= 88,//װ������
	MSGCOMPOUND_ARTIFIC_SUC				= 89,
	MSGCOMPOUND_ARTIFIC_FAIL			= 90,
	
	MSGCOMPOUND_EQUIP_MOVE				= 91,//ǿ��ת��
	MSGCOMPOUND_EQUIP_MOVE_SUC			= 92,
	MSGCOMPOUND_EQUIP_MOVE_FAIL			= 93,
//modify code by �ֵ�Ԩ end

	MSGCOMPOUND_SENDPEIFANG				= 101,
	MSGCOMPOUND_PEIFANGADD              = 102,
	MSGCOMPOUND_PEIFANGDEL              = 103,
};
class CMsgCompound : public CNetMsg  
{
public:
	CMsgCompound();
	virtual ~CMsgCompound();

	BOOL	Create(char* pbufMsg, DWORD dwMsgSize);
	BOOL	Create(int nAction,OBJID idItem, int data=0);
	void	Process(void *pInfo);
	bool	CheckSameItemID (int* array,int ArraySize);
	BOOL    AppendPeifang(OBJID idPeifang);
	typedef struct
	{
		int nItemID;
		int nAmount;
	}MATERIAL,LPMATERIAL;
private:
	typedef struct
	{
		MSGHEAD_DEFINE;
		int nAction;
		int nItemTypeID;//���в�����װ��ID������type
		int nMaterialSize;//MATERIAL����.����MSG_Info������Ǹ� [2010-07-19 goto] �޸�Ϊǿ��ʯ������������װ������
		MATERIAL Material[1]; //ǿ��������ʱ�ĳ�ǿ�����������Ƿ��Զ����
	}MSG_Info;
	
	MSG_Info *m_pInfo;

};

#endif // !defined(AFX_MSGCOMPOUND_H__9FE3B9EF_9DD9_4F14_902B_8E7003F5B884__INCLUDED_)
