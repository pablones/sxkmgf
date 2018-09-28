// SpecilLineInfo.h: interface for the CSpecilLineInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPECILLINEINFO_H__C5DDC78E_0AAC_4EE7_B635_83BCF1781404__INCLUDED_)
#define AFX_SPECILLINEINFO_H__C5DDC78E_0AAC_4EE7_B635_83BCF1781404__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "GameObj.h"
#include "../../GameBaseCodeMT/GameData.h"

enum SPECILLINESTATE
{
	STATE_SPECILLINE_OUT	 = -1,
	STATE_SPECILLINE_REQUEST =  0,
	STATE_SPECILLINE_SEC	 =	1,
	STATE_SPECILLINE_NORMAL	 =	2,
};

enum CHANGELINE
{
	CHANGELINE_BEGIN=0,
	RANDLINE=0,//随机
	LINEONE=1,//1线
	LINETWO=2,//2线
	LINETHREE=3,//3线
	LINEFOR=4,//4线
	LINEFIVE=5,//5线
	LINESIX=6,//6线
	CHANGELINE_END,
	SPECILLINE=20,//专线
};//枚举线路

enum SPECILLINEINFO
{
	SPECILLINEINFO_ID,
	SPECILLINEINFO_USERID,
	SPECILLINEINFO_MAPID,
	SPECILLINEINFO_POSX,
	SPECILLINEINFO_POSY,
	SPECILLINEINFO_STATE,
};

struct STSpecilLine
{
	OBJID id;
	OBJID idUser;
	OBJID idMap;
	int   nPosX;
	int	  nPosY;
	int	  nState;
};

char    szSpecilLineTable[];
char	szUsersid[];
typedef CGameData<SPECILLINEINFO,szSpecilLineTable,szUsersid>     CSpecilLineData;

class CSpecilLineInfo : public CGameObj
{
public:
	CSpecilLineInfo(CUser* pUser);
	virtual ~CSpecilLineInfo();

private:
	CSpecilLineInfo();

public:
	static CSpecilLineInfo* CreateNew(CUser* pUser);
	ULONG	ReleaseByOwner();

	bool Create();
	bool Update();

	void	ClearUpdateFlags()	{ CHECK(m_pData); m_pData->ClearUpdateFlags(); }
	OBJID	InsertRecord()		{ CHECKF(m_pData); return m_pData->InsertRecord(); }
	bool	DeleteRecord()		{ CHECKF(m_pData); return m_pData->DeleteRecord(); }
	int		GetInt(SPECILLINEINFO nDataName);
	void	SetInt(SPECILLINEINFO idx, int nData, bool bUpdate=false);

	bool	InsertNewInfo(OBJID idUser);

	void	SetState(int nData, bool bUpdate=false);
	void	SetRecordPos(OBJID idMap, int nPosX, int nPosY, bool bUpdate=false);

	bool	GetInfo(STSpecilLine* pInfo);
	bool	AppendInfo(const STSpecilLine* pInfo);

	bool	ProcessLogin(OBJID& idMap, int& nPosX, int& nPosY);
	bool	NeedChangeLine();
	bool	ChangeLineMap(OBJID MapID,int nPosx,int nPosy,CHANGELINE nLine);
private:
	CSpecilLineData *m_pData;
	PROCESS_ID m_idProcess;
	CUser* m_pUser;
};

#endif // !defined(AFX_SPECILLINEINFO_H__C5DDC78E_0AAC_4EE7_B635_83BCF1781404__INCLUDED_)
