// GiftManager.h: interface for the CGiftManager class.
//	20071110:修罗
//	天赋管理器.管理所有天赋数据.
//	初始化后,仅被静态读取.不会被改变
//	全局对像
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
#include "GiftTempleteManager.h"//天赋模板管理器
//#include <vector>
using namespace std;

//天赋表.sk_gift----------------------------
//enum GIFTDATA
//{
//	GIFTDATA_id				= 0,
//	GIFTDATA_giftType		= 1,//天赋
//	GIFTDATA_lev,//当前等级
//	GIFTDATA_levUpB,//升级所需B.
//	GIFTDATA_temType,//天赋模板type
//	GIFTDATA_temID,//天赋模板id
//	GIFTDATA_name,//名称
//	GIFTDATA_desc,//描述
//};
//char	szGiftTable[];
//typedef	CGameData<GIFTDATA,szGiftTable,szID>		CGiftData;
//typedef	ISingleMap2<CGiftData>	IGiftSet;
//typedef	CSingleMap2<CGiftData>	CGiftSet;
//天赋树表.sk_gift_tree---------------------
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
//天赋模板表.sk_gift_templet_xxxx------------
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

	CGiftTreeData* GetGiftData(OBJID giftType);//通过type和lev得到具体信息.0为查不到
	IRecord* GetGiftTemplete(OBJID giftType);
protected:
	IRecord* GetGiftTempleteInfo(OBJID tmpType,OBJID tmpIdx){return m_pGiftTempleteManager->GetInfo(tmpType,tmpIdx);}
protected:
	IGiftTreeSet*	m_setGiftTree;
	CGiftTempleteManager* m_pGiftTempleteManager;//天赋模板管理器
	MYHEAP_DECLARATION(s_heap)

public:
	IGiftTreeSet*	QueryGiftTreeSet()	{return m_setGiftTree;};	//  [12/18/2007 朱斌]	得天赋树表集合
};

//global
extern CGiftManager* g_pGiftManager;

#endif // !defined(AFX_GIFTMANAGER_H__2F220C7E_EC63_4A46_AFBE_A1F73BB87E1B__INCLUDED_)


