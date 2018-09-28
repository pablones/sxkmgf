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
///softworms-��չ�����ݿ����-����֧�ּ򵥵õ��������ݵķ���
char	szDummyDataObjectTable[];
typedef	CGameData<LPCSTR, szDummyDataObjectTable, szDummyDataObjectTable>	CDataObjectData;///�����ģ�巽ʽ,��������,Ӧ�ú���Ĳ���Ӧ�ñ�ɺ�������.
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
