// StrRes.h: interface for the CStrRes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRRES_H__FA2CA326_21A2_45D5_8280_7CE73027E58C__INCLUDED_)
#define AFX_STRRES_H__FA2CA326_21A2_45D5_8280_7CE73027E58C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable:4786)
//class CStrRes  
//{
//public:
//	CStrRes();
//	virtual ~CStrRes();
//
//};
#include <map>
#include "GameData.h"
#include "typedef.h"
using namespace std;
typedef map<OBJID, char*>MAP_STRRES;

const char g_szStrResError[] = "ERROR";
const char g_szResFile[] = "ini\\StrRes_ser.ini";

class CStrRes  
{
public:
	CStrRes();
	virtual ~CStrRes();
protected:
	MAP_STRRES		m_setStr;
	void			InitStrRes();
	void			Destroy();
public:
	const	char*	GetStr(OBJID idRes);
	void			Init();
};

extern CStrRes	g_objStrRes;

#endif // !defined(AFX_STRRES_H__FA2CA326_21A2_45D5_8280_7CE73027E58C__INCLUDED_)
