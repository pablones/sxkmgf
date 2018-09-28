// VASStringPacket.cpp: implementation of the CVASStringPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "accountserver.h"
#include "VASStringPacket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVASStringPacket::CVASStringPacket()
{

}

CVASStringPacket::~CVASStringPacket()
{

}

void CVASStringPacket::Clear()///清空
{
	m_fields.clear();
}

BOOL CVASStringPacket::ParseString(CString& packet)///解析串流
{
	Clear();///解析流之前先清空列表
	int i=0;
	CString strBuffer;
	CString strFieldName,strFieldValue;
	if (packet.Find("id")!=-1 && packet.Find(";;")!=-1)///id是包头,;;是包尾,如果没有这两部分就是非法包
		packet.Replace(";;","\0");///去掉结束符
	else
		return false;///非法的包
	while(1)///进入拆包循环
	{
		if(AfxExtractSubString(strBuffer,packet,i++,'+')==false)///拆出一个字段
			break;
		AfxExtractSubString(strFieldName,strBuffer,0,'=');///拆出键域
		AfxExtractSubString(strFieldValue,strBuffer,1,'=');///拆出值域
		m_fields[strFieldName]=strFieldValue;///放入数据模型中
	}
	return m_fields.size();
}

BOOL CVASStringPacket::AddField(const CString& fieldname,const CString& fieldvalue)
{
	m_fields[fieldname]=fieldvalue;///加字段
	return m_fields.size();
}

BOOL CVASStringPacket::AddField(const CString& fieldname,ULONG fieldvalue)///加字段内容
{
	CString tmpStr;
	tmpStr.Format("%u",fieldvalue);///数值版本需要先转成串型再添加
	return AddField(fieldname,tmpStr);
}

BOOL CVASStringPacket::PacketString(CString& strStream)///打包,得到串流
{
	///枚举数据模型,将所有字段整包
	for(map<CString,CString>::iterator it=m_fields.begin();it!=m_fields.end();++it)
		strStream+=it->first+"="+it->second+"+";
	strStream+="!";///去掉最后的+做准备
	strStream.Replace("+!",";;");///生成结束符
	return strStream.GetLength();
}

BOOL CVASStringPacket::GetFieldValue(const CString& fieldname,CString& fieldvalue)///取得字段值
{
	fieldvalue=m_fields[fieldname];
	return m_fields.size();
}

ULONG CVASStringPacket::GetFieldValue(const CString& fieldname)///取得字段数据值
{
	return atol(m_fields[fieldname]);///取得字段的值域,数值版本
}