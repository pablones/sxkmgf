// MapGroupObject.h: interface for the CMapGroupObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPGROUPOBJECT_H__CEC9F4C6_5F02_48B1_96F3_3A895CD46110__INCLUDED_)
#define AFX_MAPGROUPOBJECT_H__CEC9F4C6_5F02_48B1_96F3_3A895CD46110__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMapGroupObject  
{
public:
	CMapGroupObject();
	virtual ~CMapGroupObject();
	void SetProcessID(OBJID idPorcess){m_idProcess=idPorcess;}
	OBJID GetProcessID(){return m_idProcess;}
protected:
	PROCESS_ID			m_idProcess;
};

#endif // !defined(AFX_MAPGROUPOBJECT_H__CEC9F4C6_5F02_48B1_96F3_3A895CD46110__INCLUDED_)
