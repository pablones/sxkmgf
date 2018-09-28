// UserMaterialItem.h: interface for the CUserMaterialItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERMATERIALITEM_H__440C7D63_0C93_434E_876B_9659E96014D0__INCLUDED_)
#define AFX_USERMATERIALITEM_H__440C7D63_0C93_434E_876B_9659E96014D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//include "User.h"
#include <vector>
#include "GameData.h"
#include "..\MSGSERVER\typedef.h"
using namespace std;

class CUserMaterialItem  
{
protected:
	CUserMaterialItem();
	virtual ~CUserMaterialItem();
public:
	static CUserMaterialItem* CreateNew();
	void               Release();
	bool               Init();
public:
	static CUserMaterialItem* s_Instance;
//protected:
	
typedef struct
{
	OBJID mat;  //材料名字
	OBJID num;  //材料数量
	char szTitle[32];//材料名称 
}LoverHomeMaterial;

	vector<LoverHomeMaterial> m_setLoverHomeBed;
    vector<LoverHomeMaterial> m_setLoverHomeStorage;
	vector<LoverHomeMaterial> m_setLoverHomeFlower;
    vector<LoverHomeMaterial> m_setLoverHomeCabinet;
};

#endif // !defined(AFX_USERMATERIALITEM_H__440C7D63_0C93_434E_876B_9659E96014D0__INCLUDED_)
