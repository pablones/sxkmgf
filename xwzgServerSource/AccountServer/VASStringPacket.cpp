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

void CVASStringPacket::Clear()///���
{
	m_fields.clear();
}

BOOL CVASStringPacket::ParseString(CString& packet)///��������
{
	Clear();///������֮ǰ������б�
	int i=0;
	CString strBuffer;
	CString strFieldName,strFieldValue;
	if (packet.Find("id")!=-1 && packet.Find(";;")!=-1)///id�ǰ�ͷ,;;�ǰ�β,���û���������־��ǷǷ���
		packet.Replace(";;","\0");///ȥ��������
	else
		return false;///�Ƿ��İ�
	while(1)///������ѭ��
	{
		if(AfxExtractSubString(strBuffer,packet,i++,'+')==false)///���һ���ֶ�
			break;
		AfxExtractSubString(strFieldName,strBuffer,0,'=');///�������
		AfxExtractSubString(strFieldValue,strBuffer,1,'=');///���ֵ��
		m_fields[strFieldName]=strFieldValue;///��������ģ����
	}
	return m_fields.size();
}

BOOL CVASStringPacket::AddField(const CString& fieldname,const CString& fieldvalue)
{
	m_fields[fieldname]=fieldvalue;///���ֶ�
	return m_fields.size();
}

BOOL CVASStringPacket::AddField(const CString& fieldname,ULONG fieldvalue)///���ֶ�����
{
	CString tmpStr;
	tmpStr.Format("%u",fieldvalue);///��ֵ�汾��Ҫ��ת�ɴ��������
	return AddField(fieldname,tmpStr);
}

BOOL CVASStringPacket::PacketString(CString& strStream)///���,�õ�����
{
	///ö������ģ��,�������ֶ�����
	for(map<CString,CString>::iterator it=m_fields.begin();it!=m_fields.end();++it)
		strStream+=it->first+"="+it->second+"+";
	strStream+="!";///ȥ������+��׼��
	strStream.Replace("+!",";;");///���ɽ�����
	return strStream.GetLength();
}

BOOL CVASStringPacket::GetFieldValue(const CString& fieldname,CString& fieldvalue)///ȡ���ֶ�ֵ
{
	fieldvalue=m_fields[fieldname];
	return m_fields.size();
}

ULONG CVASStringPacket::GetFieldValue(const CString& fieldname)///ȡ���ֶ�����ֵ
{
	return atol(m_fields[fieldname]);///ȡ���ֶε�ֵ��,��ֵ�汾
}