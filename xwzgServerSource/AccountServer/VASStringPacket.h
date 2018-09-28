// VASStringPacket.h: interface for the CVASStringPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VASSTRINGPACKET_H__2914DED8_F426_4996_A612_654044D8C260__INCLUDED_)
#define AFX_VASSTRINGPACKET_H__2914DED8_F426_4996_A612_654044D8C260__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
 
#include "map"
#include "string"
using namespace std;

///����:id=123+name=abc;;
///���ڴ�Э��ķ��
class CVASStringPacket  
{
public:
	CVASStringPacket();
	~CVASStringPacket();

	void Clear();///���
	BOOL ParseString(CString& packet);///��������,����fields�б�
	BOOL AddField(const CString& fieldname,const CString& fieldvalue);///��fields�в���һ���ֶ�
	BOOL AddField(const CString& fieldname,ULONG fieldvalue);///���ص���ֵ�汾
	BOOL GetFieldValue(const CString& fieldname,CString& fieldvalue);///ȡ���ֶ�ֵ
	ULONG  GetFieldValue(const CString& fieldname);///ȡ���ֶ�����ֵ,��ֵ�汾
	BOOL PacketString(CString& strStream);///��fields���,�õ�Э�������.
private:
	map<CString,CString> m_fields;///����������ģʽ,��һ��������key,�ڶ�����value,����id<key>=1234<value>
};

#endif // !defined(AFX_VASSTRINGPACKET_H__2914DED8_F426_4996_A612_654044D8C260__INCLUDED_)
