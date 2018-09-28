// MsgAutoFight.h: interface for the CMsgAutoFight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGAUTOFIGHT_H__176F519E_5D50_4759_A1CA_ECEB5C92D4BB__INCLUDED_)
#define AFX_MSGAUTOFIGHT_H__176F519E_5D50_4759_A1CA_ECEB5C92D4BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NETWORK\NetMsg.h"
#include "../share/protocol.h"

enum _AutoFight{
	_AutoFight_WRITE	= 0,
	_AutoFight_READ,
	_AutoFight_PICKUP,
	_AutoFight_OUTOFRANGE,
};

class CMsgAutoFight :public CNetMsg 
{
public:
	CMsgAutoFight();
	virtual ~CMsgAutoFight();
	
	virtual BOOL Create		(char* pMsgBuf, DWORD dwSize);
	virtual void Process	(void *pInfo);
	
	
	BOOL Create(int nAction, struct AutoFightInfo &pInfo);
	BOOL Create(_AutoFight nAction, int nData = 0);
private:
	typedef struct
	{
		MSGHEAD_DEFINE
		UINT nAction;
		
//		OBJID id  ;                 //�Զ�����������       
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

// 		//  ����Ϊ�Զ��ָ������ֶΣ�
// 		UINT autoRestoration_hp ;  //�Ƿ����Զ��ָ������е�"������������"������Ϊ"1","2"��δ����Ϊ"0"��
// 		char aRhp_item[_MAX_NAMESIZE] ;           //ʹ�ú��ֵ������Զ��ָ�Ѫ������д����ID��
// 		char aRhp_item2[_MAX_NAMESIZE] ;          //ʹ�ú��ֵ������Զ��ָ�Ѫ������д����ID��
// 		UINT aRhpPercentage ;      //��Ѫ���������ֵ�ٷ�֮����ʱ�������Զ���Ѫ����д�ٷ�����
// 		UINT aRhpPercentage2 ;      //��Ѫ���������ֵ�ٷ�֮����ʱ�������Զ���Ѫ����д�ٷ�����
// 		UINT autoRestoration_mp;   //�Ƿ����Զ��ָ������е�"����ħ��ֵ����"������Ϊ"1","2"��δ����Ϊ"0"��
// 		char aRmp_Item[_MAX_NAMESIZE];            //ʹ�ú��ֵ������Զ��ָ�ħ��������д����ID��
// 		char aRmp_Item2[_MAX_NAMESIZE];            //ʹ�ú��ֵ������Զ��ָ�ħ��������д����ID��
// 		UINT armpPercentage ;      //��ħ�����������ֵ�ٷ�֮����ʱ�������Զ���ħ����д�ٷ�����
// 		UINT armpPercentage2 ;      //��ħ�����������ֵ�ٷ�֮����ʱ�������Զ���ħ����д�ٷ�����
// 
// 
// // 		UINT autoRestoration_hp ;  //�Ƿ����Զ��ָ������е�"������������"������Ϊ"1"��δ����Ϊ"0"��
// // 		char aRhp_item[_MAX_NAMESIZE] ;           //ʹ�ú��ֵ������Զ��ָ�Ѫ������д����ID��
// // 		UINT aRhpPercentage ;      //��Ѫ���������ֵ�ٷ�֮����ʱ�������Զ���Ѫ����д�ٷ�����
// // 		UINT autoRestoration_mp;   //�Ƿ����Զ��ָ������е�"����ħ��ֵ����"������Ϊ"1"��δ����Ϊ"0"��
// // 		char aRmp_Item[_MAX_NAMESIZE];            //ʹ�ú��ֵ������Զ��ָ�ħ��������д����ID��
// // 		UINT armpPercentage ;      //��ħ�����������ֵ�ٷ�֮����ʱ�������Զ���ħ����д�ٷ�����
// 		// ����Ϊ�Զ��һ������ֶΣ�
// 		UINT AutoAttack_hp  ;      //�Ƿ����Զ��һ��е�"������������"������Ϊ"1"��δ����Ϊ"0"��
// 		char AAhp_item[_MAX_NAMESIZE] ;           //ʹ�ú��ֵ������Զ��ָ�Ѫ������д����ID��
// 		UINT AahpPercentage ;      //��Ѫ���������ֵ�ٷ�֮����ʱ�������Զ���Ѫ����д�ٷ�����
// 		UINT AutoAttack_mp ;      //�Ƿ����Զ��һ��е�"����ħ��ֵ����"������Ϊ"1"��δ����Ϊ"0"��
// 		char AAmp_item[_MAX_NAMESIZE]   ;        //ʹ�ú��ֵ������Զ��ָ�ħ��������д����ID��
// 		UINT AampPercentage ;     //��Ѫ���������ֵ�ٷ�֮����ʱ�������Զ���ħ����д�ٷ�����
// 		UINT Pickup ;             //�Զ�ʰȡ���ã�1�����Զ�ʰȡ��0δ�����Զ�ʰȡ��2ʰȡҩƷ��4ʰȡ���ϣ�8ʰȡ��ʯ��16ʰȡ�����飬32ʰȡ����Ʒ�����ϣ�64ʰȡ����Ʒ�����ϣ�128ʰȡ׿ԽƷ�����ϡ�
// 		UINT autousevas  ;         //�Զ�ʹ����ֵ���ߣ�����Ϊ"1"��δ����Ϊ"0"��2Ϊ�Զ�ʹ�ø���� 4Ϊ�Զ�ʹ���񽫴���Ϊ�Զ�ʹ��˫������ҩ��
// 		UINT autoshopping ;       //�Զ��������ã�1Ϊ�����Զ�����0Ϊδ������2Ϊ�Զ�����죬4Ϊ�Զ���������8Ϊ�Զ����򸴻�����ֶ���ֵΪ���ϸ��趨ֵ���������
// 		UINT num_hp ;             //�Զ������ҩ��������
// 		UINT num_mp ;             //�Զ�������ҩ��������
// 		UINT num_fh  ;            //�Զ����򸴻���������
// 		UINT num_double  ;        //�Զ�ʹ��˫������ҩ����
// 		UINT goal  ;              //�Զ����Ŀ���ѡ��0Ϊ����Ĺ֣�1Ϊ���ڹ����Լ��Ĺ֡�
// 		UINT way  ;               //�Զ���ַ�ʽ��ѡ��0Ϊ��ͨ������1Ϊ���ܹ�����
// 		UINT autoUseAssit;        //�Զ�ʹ�ø���ħ��
//      	UINT autoUseAttack;       //�Զ�ʹ�ù���ħ��
// 
// 		//���ﲿ��
// 		OBJID	dwEudemon1;
// 		OBJID	dwEudemon2;
// 		DWORD	dwEudemonDrinkType;				
// 		DWORD	dwEudemonDrinkRedID;			
// 		DWORD	dwEudemonDrinkBlueID;			
// 		DWORD	dwEudemonDrinkRedTypeID;			
// 		DWORD	dwEudemonDrinkBlueTypeID;			
// 		DWORD	dwEudemonPercentRed;			
// 		DWORD	dwEudemonPercentBlue;
// 
// 		char skills1[_MAX_NAMESIZE];          //�Զ����ʹ�ü����趨��skills1~3Ϊ���漼�ܣ�skills4~7Ϊ�������ܣ���д����ID��
// 		char skills2[_MAX_NAMESIZE];
// 		char skills3[_MAX_NAMESIZE];
// 		char skills4[_MAX_NAMESIZE];
// 		char skills5[_MAX_NAMESIZE];
// 		char skills6[_MAX_NAMESIZE];
// 		char skills7[_MAX_NAMESIZE];
		
	}MSG_Info;
	
	MSG_Info*	m_pInfo;
	
};

#endif // !defined(AFX_MSGAUTOFIGHT_H__176F519E_5D50_4759_A1CA_ECEB5C92D4BB__INCLUDED_)
