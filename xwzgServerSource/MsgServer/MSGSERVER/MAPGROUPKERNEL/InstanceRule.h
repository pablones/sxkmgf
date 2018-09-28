// InstanceRule.h: interface for the CInstanceRule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSTANCERULE_H__80D46F79_A140_4876_A6D2_AA169D97B2C8__INCLUDED_)
#define AFX_INSTANCERULE_H__80D46F79_A140_4876_A6D2_AA169D97B2C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "windows.h"
#include "map"
#include "string"
using namespace std;

typedef map<string,string> InstanceRuleList;///������Ϣ�б�,�������ڴ������,��������,�������Ƶ���Ϣ,�ɲ߻���action��ű�ʹ��
///����������-softworms-2008-01-25
class CInstanceRule  
{
public:
	CInstanceRule();
	virtual ~CInstanceRule();
	void AddRule();
	void RemoveRule();
	const string& GetRuleInfo(LPCSTR strRuleName)
	{
		return m_RuleList[strRuleName];
	}
	void SetRuleInfo(LPCSTR strRuleName,LPCSTR strRuleValue)
	{
		m_RuleList[strRuleName]=strRuleValue;
	}
	void Reset()
	{
		m_RuleList.clear();
	}
private:
	InstanceRuleList m_RuleList;///�����Զ������,���������Ա㸱������ʱ��ѯ
};

#endif // !defined(AFX_INSTANCERULE_H__80D46F79_A140_4876_A6D2_AA169D97B2C8__INCLUDED_)
