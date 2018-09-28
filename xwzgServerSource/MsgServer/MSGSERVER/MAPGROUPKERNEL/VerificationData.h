// VerificationData.h: interface for the CVerificationData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERIFICATIONDATA_H__9E579305_6D0D_4AA9_B1C2_1C6C83C8AC3F__INCLUDED_)
#define AFX_VERIFICATIONDATA_H__9E579305_6D0D_4AA9_B1C2_1C6C83C8AC3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../share/define.h"
#include "GameObj.h"
#include "../../GameBaseCodeMT/GameData.h"
#include "T_SingleObjSet2.h"
#include "Myheap.h"
#include <map>
class CVerificationData  
{
public:
	CVerificationData();
	virtual ~CVerificationData();
	static CVerificationData*	CreateNew	()		{ return new CVerificationData; }

public:
	bool	Create(IDatabase* pDb);
	string  GetCodeById(OBJID nid){return m_idcodmap[nid];}
	int     GetListSize(){return m_idlist.size();}
	OBJID   GetIdByIndex(int nindex){if(nindex>GetListSize())return 0;	return m_idlist[nindex];}
private:
	typedef map<OBJID,string> IDCODE_SET;
	IDCODE_SET m_idcodmap;
	vector<OBJID>m_idlist;
};

#endif // !defined(AFX_VERIFICATIONDATA_H__9E579305_6D0D_4AA9_B1C2_1C6C83C8AC3F__INCLUDED_)
