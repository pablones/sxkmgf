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

///举例:id=123+name=abc;;
///基于串协议的封包
class CVASStringPacket  
{
public:
	CVASStringPacket();
	~CVASStringPacket();

	void Clear();///清空
	BOOL ParseString(CString& packet);///解析串流,生成fields列表
	BOOL AddField(const CString& fieldname,const CString& fieldvalue);///向fields中插入一个字段
	BOOL AddField(const CString& fieldname,ULONG fieldvalue);///重载的数值版本
	BOOL GetFieldValue(const CString& fieldname,CString& fieldvalue);///取得字段值
	ULONG  GetFieldValue(const CString& fieldname);///取得字段数据值,数值版本
	BOOL PacketString(CString& strStream);///对fields打包,得到协议包串流.
private:
	map<CString,CString> m_fields;///包的数据型模式,第一个参数是key,第二个是value,例如id<key>=1234<value>
};

#endif // !defined(AFX_VASSTRINGPACKET_H__2914DED8_F426_4996_A612_654044D8C260__INCLUDED_)
