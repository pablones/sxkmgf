// DataObject.h: interface for the CDataObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAOBJECT_H__3F44B40B_A03B_401F_801A_0C0FF4B506AF__INCLUDED_)
#define AFX_DATAOBJECT_H__3F44B40B_A03B_401F_801A_0C0FF4B506AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum DATAOBJECT{
		DATAOBJECT_ID_=0,
};
///softworms-扩展的数据库对象-尝试支持简单得到表中数据的方法
char	szDummyDataObjectTable[];
typedef	CGameData<LPCSTR, szDummyDataObjectTable, szDummyDataObjectTable>	CDataObjectData;///错误的模板方式,真是无聊,应该后面的参数应该变成函数参数.
//template<LPCSTR TBL>
class CDataObject  : public CDataObjectData
{
public:
	CDataObject();
	virtual ~CDataObject();
//	ULONG	ReleaseByOwner	()				{ delete this; return 0; }
	static  CDataObject*	CreateNew	()	{ return new CDataObject; }
//private:
//	CDataObjectData* m_dataptr;
};

typedef	ISingleObjSet2<CDataObject>		IDataObjectSet;
typedef	CSingleObjSet2<CDataObject>		CDataObjectSet;

#endif // !defined(AFX_DATAOBJECT_H__3F44B40B_A03B_401F_801A_0C0FF4B506AF__INCLUDED_)
