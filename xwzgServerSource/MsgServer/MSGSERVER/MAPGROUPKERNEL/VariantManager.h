// VariantManager.h: interface for the CVariantManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VARIANTMANAGER_H__93CBFBC5_2066_47CE_BC0B_B639752296A5__INCLUDED_)
#define AFX_VARIANTMANAGER_H__93CBFBC5_2066_47CE_BC0B_B639752296A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InstanceRule.h"
#include "../share/define.h"

#define SPIRIT_LIMIT "spirit_limit"

class CVariantManager  
{
public:
	typedef CInstanceRule CVariantSet;
	typedef map<string,CVariantSet> CVariantList;
	CVariantManager();
	CVariantManager(PROCESS_ID idProcess)
	{
		
	}
	virtual ~CVariantManager();

	LPCSTR GetVariantValue(LPCSTR strVariantSetName,LPCSTR strVariantName)
	{
		EnterCriticalSection(&m_sect);
		CVariantSet& vs=GetVariantSet(strVariantSetName);
		LPCSTR VariantValue=vs.GetRuleInfo(strVariantName).c_str();
		LeaveCriticalSection(&m_sect);
		return VariantValue;
	}

	int GetVariantInt(LPCSTR strVariantSetName,LPCSTR strVariantName)
	{
		return atoi(GetVariantValue(strVariantSetName,strVariantName));
	}

	void SetVariantValue(LPCSTR strVariantSetName,LPCSTR strVariantName,LPCSTR strVariantValue)
	{
		EnterCriticalSection(&m_sect);
		CVariantSet& vs=GetVariantSet(strVariantSetName);
		vs.SetRuleInfo(strVariantName,strVariantValue);
		LeaveCriticalSection(&m_sect);
	}

	void SetVariantInt(LPCSTR strVariantSetName,LPCSTR strVariantName,int nVariantValue)
	{
		char strVariantValue[256]={0};
		sprintf(strVariantValue,"%d",nVariantValue);
		SetVariantValue(strVariantSetName,strVariantName,strVariantValue);
	}

	void AddVariantInt(LPCSTR strVariantSetName,LPCSTR strVariantName,int nVariantValue)
	{
		int nCurrentVariantValue=GetVariantInt(strVariantSetName,strVariantName);
		SetVariantInt(strVariantSetName,strVariantName,nCurrentVariantValue+nVariantValue);
	}
	
	void AddVariantInt(LPCSTR strVariantSetName,OBJID idVariant,int nVariantValue)
	{
		char strVariantName[256]={0};
		sprintf(strVariantName,"%d",idVariant);
		AddVariantInt(strVariantSetName,strVariantName,nVariantValue);
	}

	int GetVariantInt(LPCSTR strVariantSetName,OBJID idVariant)
	{
		char strVariantName[256]={0};
		sprintf(strVariantName,"%d",idVariant);
		return GetVariantInt(strVariantSetName,strVariantName);
	}
	void ResetVariantSet(LPCSTR strVariantSetName)
	{
		EnterCriticalSection(&m_sect);
		CVariantSet& vs=GetVariantSet(strVariantSetName);
		vs.Reset();		
		LeaveCriticalSection(&m_sect);
	}
protected:
	CVariantSet& GetVariantSet(LPCSTR strVariantSetName)
	{
		return m_VariantList[strVariantSetName];
	}
private:
	CRITICAL_SECTION m_sect;
	CVariantList m_VariantList;
};

extern CVariantManager	g_VariantManager;// 彭正帮[6/18/2007] 临时变量表

#endif // !defined(AFX_VARIANTMANAGER_H__93CBFBC5_2066_47CE_BC0B_B639752296A5__INCLUDED_)
