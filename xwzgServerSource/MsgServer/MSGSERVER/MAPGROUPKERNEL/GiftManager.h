// GiftManager.h: interface for the CGiftManager class.
//	20071110:����
//	�츳������.���������츳����.
//	��ʼ����,������̬��ȡ.���ᱻ�ı�
//	ȫ�ֶ���
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GIFTMANAGER_H__2F220C7E_EC63_4A46_AFBE_A1F73BB87E1B__INCLUDED_)
#define AFX_GIFTMANAGER_H__2F220C7E_EC63_4A46_AFBE_A1F73BB87E1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#pragma warning(disable:4786)
#include "../share/define.h"
#include "../../GameBaseCodeMT/ConstGameData.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "GameObj.h"
#include "Myheap.h"
#include "T_SingleMap2.h"
#include "GiftTempleteManager.h"//�츳ģ�������
//#include <vector>
using namespace std;

//�츳��.sk_gift----------------------------
//enum GIFTDATA
//{
//	GIFTDATA_id				= 0,
//	GIFTDATA_giftType		= 1,//�츳
//	GIFTDATA_lev,//��ǰ�ȼ�
//	GIFTDATA_levUpB,//��������B.
//	GIFTDATA_temType,//�츳ģ��type
//	GIFTDATA_temID,//�츳ģ��id
//	GIFTDATA_name,//����
//	GIFTDATA_desc,//����
//};
//char	szGiftTable[];
//typedef	CGameData<GIFTDATA,szGiftTable,szID>		CGiftData;
//typedef	ISingleMap2<CGiftData>	IGiftSet;
//typedef	CSingleMap2<CGiftData>	CGiftSet;
//�츳����.sk_gift_tree---------------------
enum GIFTTREEDATA
{
	GIFTTREEDATA_id		= 0,
	GIFTTREEDATA_temType,
	GIFTTREEDATA_temID,
};
char	szGiftTreeTable[];
typedef	CGameData<GIFTTREEDATA,szGiftTreeTable,szID>	CGiftTreeData;
typedef	ISingleMap2<CGiftTreeData>	IGiftTreeSet;
typedef	CSingleMap2<CGiftTreeData>	CGiftTreeSet;
//�츳ģ���.sk_gift_templet_xxxx------------
//enum GIFTTEMPLETEDATA
//{
//};
//char	szGiftTempletTable[];
//typedef	CGameData<ITEMTYPEDATA,szGiftTempletTable,szID>	CGiftTempletData;
//typedef	ISingleMap2<CGiftTempletData>	IGiftTempletSet;
//typedef	CSingleMap2<CGiftTempletData>	CGiftTempletSet;


class CGiftManager  
{
public:
	CGiftManager();
	virtual ~CGiftManager();
public:
	static CGiftManager*	CreateNew	()		{ return new CGiftManager; }
	ULONG	Release			()				{ delete this; return 0; }
	bool	Create			(IDatabase* pDb);

	CGiftTreeData* GetGiftData(OBJID giftType);//ͨ��type��lev�õ�������Ϣ.0Ϊ�鲻��
	IRecord* GetGiftTemplete(OBJID giftType);
protected:
	IRecord* GetGiftTempleteInfo(OBJID tmpType,OBJID tmpIdx){return m_pGiftTempleteManager->GetInfo(tmpType,tmpIdx);}
protected:
	IGiftTreeSet*	m_setGiftTree;
	CGiftTempleteManager* m_pGiftTempleteManager;//�츳ģ�������
	MYHEAP_DECLARATION(s_heap)

public:
	IGiftTreeSet*	QueryGiftTreeSet()	{return m_setGiftTree;};	//  [12/18/2007 ���]	���츳������
};

//global
extern CGiftManager* g_pGiftManager;

#endif // !defined(AFX_GIFTMANAGER_H__2F220C7E_EC63_4A46_AFBE_A1F73BB87E1B__INCLUDED_)


