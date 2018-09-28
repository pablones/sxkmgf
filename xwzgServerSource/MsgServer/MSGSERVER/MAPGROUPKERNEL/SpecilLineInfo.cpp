// SpecilLineInfo.cpp: implementation of the CSpecilLineInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "afx.h"
#include "SpecilLineInfo.h"
#include "User.h"
#include "MapGroup.h"
#include "MsgSyndicate.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char    szSpecilLineTable[] = _TBL_SPECILLINE_INFO;

CSpecilLineInfo::CSpecilLineInfo(CUser* pUser)
{
	m_pUser = pUser;
	m_idProcess = pUser->GetProcessID();

	m_pData = CSpecilLineData::CreateNew();
}

CSpecilLineInfo::~CSpecilLineInfo()
{
	if(m_pData)
		m_pData->Release();
}

ULONG CSpecilLineInfo::ReleaseByOwner()
{
	delete this; 
	return 0; 
}

CSpecilLineInfo* CSpecilLineInfo::CreateNew(CUser* pUser)
{
	CHECKF(pUser);

	CSpecilLineInfo* pSpecilLineInfo = new CSpecilLineInfo(pUser);
	CHECKF(pSpecilLineInfo);

	return pSpecilLineInfo;
}

bool CSpecilLineInfo::Create()
{
	CHECKF(m_pData);
	CHECKF(m_pUser);
	
	if(!m_pData->Create(m_pUser->GetID(),Database()))
		return false;
	if(m_pData->GetInt(SPECILLINEINFO_USERID)==ID_NONE)
		return InsertNewInfo(m_pUser->GetID());
	
    return true;
}

bool CSpecilLineInfo::InsertNewInfo(OBJID idUser)
{
	CHECKF(CMapGroup::IsValidMapGroupProcessID(m_idProcess));
	CHECKF(m_pData);
	CHECKF(idUser);

	OBJID id = ID_NONE;
	DEBUG_TRY
	IF_NOT(m_pData->Create(GameDataDefault()->GetSpecilLineData(), id))
		return ID_NONE;

	m_pData->SetInt(SPECILLINEINFO_USERID, idUser);
	m_pData->SetInt(SPECILLINEINFO_MAPID, 0);
	m_pData->SetInt(SPECILLINEINFO_POSX, 0);
	m_pData->SetInt(SPECILLINEINFO_POSY, 0);
	m_pData->SetInt(SPECILLINEINFO_STATE, STATE_SPECILLINE_OUT);
	
	id = InsertRecord();

	DEBUG_CATCH("CSpecilLineInfo::InsertNewInfo(OBJID idUser)")
	return id != ID_NONE;
}

bool CSpecilLineInfo::Update()
{
	DEBUG_TRY
	CHECKF(m_pData);
	
	return m_pData->Update();
	DEBUG_CATCH("CSpecilLineInfo::Update()")
	return false;
}

int	CSpecilLineInfo::GetInt(SPECILLINEINFO nDataName)
{
	CHECKF(m_pData);
	
	return m_pData->GetInt(nDataName);
}
void CSpecilLineInfo::SetInt(SPECILLINEINFO idx, int nData, bool bUpdate)
{
	CHECK(m_pData);

	if(GetInt(idx)==nData)
		return;

	m_pData->SetInt(idx, nData);
	if(bUpdate)
		Update();
}

void CSpecilLineInfo::SetState(int nData, bool bUpdate)
{
	CHECK(m_pData);
	
	SetInt(SPECILLINEINFO_STATE, nData, bUpdate);
}

void CSpecilLineInfo::SetRecordPos(OBJID idMap, int nPosX, int nPosY, bool bUpdate)
{
	CHECK(m_pData);
	
	SetInt(SPECILLINEINFO_MAPID, idMap);
	SetInt(SPECILLINEINFO_POSX, nPosX);
	SetInt(SPECILLINEINFO_POSY, nPosY, bUpdate);
}

bool CSpecilLineInfo::GetInfo(STSpecilLine* pInfo)
{
	CHECKF(m_pData);
	CHECKF(pInfo);

	pInfo->id = m_pData->GetInt(SPECILLINEINFO_ID);
	pInfo->idUser = m_pData->GetInt(SPECILLINEINFO_USERID);
	pInfo->idMap = m_pData->GetInt(SPECILLINEINFO_MAPID);
	pInfo->nPosX = m_pData->GetInt(SPECILLINEINFO_POSX);
	pInfo->nPosY = m_pData->GetInt(SPECILLINEINFO_POSY);
	pInfo->nState = m_pData->GetInt(SPECILLINEINFO_STATE);

	return true;
}

bool CSpecilLineInfo::AppendInfo(const STSpecilLine* pInfo)
{
	CHECKF(pInfo);
	CHECKF(m_pData);
	CHECKF(CMapGroup::IsValidMapGroupProcessID(m_idProcess));

	DEBUG_TRY
	IF_NOT(m_pData->Create(GameDataDefault()->GetSpecilLineData(), pInfo->id))
		return false;
	
	m_pData->SetInt(SPECILLINEINFO_ID, pInfo->id);
	m_pData->SetInt(SPECILLINEINFO_USERID, pInfo->idUser);
	m_pData->SetInt(SPECILLINEINFO_MAPID, pInfo->idMap);
	m_pData->SetInt(SPECILLINEINFO_POSX, pInfo->nPosX);
	m_pData->SetInt(SPECILLINEINFO_POSY, pInfo->nPosY);
	m_pData->SetInt(SPECILLINEINFO_STATE, pInfo->nState);

	m_pData->ClearUpdateFlags();
	DEBUG_CATCH("CSpecilLineInfo::AppendInfo")

	return true;
}

bool CSpecilLineInfo::NeedChangeLine()
{
	CHECKF(m_pUser);
	CHECKF(m_pData);

	OBJID idSpeMap = GetInt(SPECILLINEINFO_MAPID);
	int   nSpeState = GetInt(SPECILLINEINFO_STATE);

	if(idSpeMap==ID_NONE)
		return false;

	bool bNeedChange = false;

	if (nSpeState == STATE_SPECILLINE_REQUEST)
	{
		if (g_SpecilLine == 1)//当前服务器是专线
		{
			bNeedChange = false;
		}
		else//普通线
		{
			bNeedChange = true;
		}
	}
	else if(nSpeState == STATE_SPECILLINE_SEC)//已经尝试过一次切线了
	{
		bNeedChange = false;
	}
	else//不是请求进专线
	{
		if (g_SpecilLine==1)//我是专线！可能是错误进入
		{
			bNeedChange = true;
		}
		else
			bNeedChange = false;
	}

	return bNeedChange;
}

bool CSpecilLineInfo::ProcessLogin(OBJID& idMap, int& nPosX, int& nPosY)
{
	CHECKF(m_pUser);
	CHECKF(m_pData);

	CGameMap* pMap = NULL;
	OBJID idSpeMap = GetInt(SPECILLINEINFO_MAPID);
	int	  nSpePosX = GetInt(SPECILLINEINFO_POSX);
	int   nSpePosY = GetInt(SPECILLINEINFO_POSY);
	int   nSpeState = GetInt(SPECILLINEINFO_STATE);

	if(idSpeMap==ID_NONE)
		return false;

	bool bNeedChange = false;
	DEBUG_TRY
	if (nSpeState >= STATE_SPECILLINE_REQUEST)//请求到专线的处理 --jay
	{
		if (g_SpecilLine == 1)//当前服务器是专线
		{
			if (idSpeMap == m_pUser->GetSynID())//是进帮派
			{
				CSynPtr pSyn = SynManager()->QuerySyndicate(m_pUser->GetSynID());
				if(pSyn)//找到帮
				{
					pMap	= MapManager()->QueryMap(pSyn->GetSynMapID());
					nPosX	= nSpePosX;
					nPosY	= nSpePosY;
					if (pMap)
					{
						idMap = pSyn->GetSynMapID();
						SetState(STATE_SPECILLINE_NORMAL);//正常进入
					}
					else
					{
						LOGWARNING("找不到帮派地图 m_SpecilMapID = %u ,玩家[%s]被踢出",idSpeMap,m_pUser->GetName());
						ChangeLineMap(m_pUser->GetRecordMapID(),m_pUser->GetRecordPosX(),m_pUser->GetRecordPosY(),RANDLINE);//任意线
						bNeedChange = true;
					}
				}
				else//没有帮,T走 T_T
				{
					ChangeLineMap(m_pUser->GetRecordMapID(),m_pUser->GetRecordPosX(),m_pUser->GetRecordPosY(),RANDLINE);//任意线
					bNeedChange = true;
				}
			}
			else if(idSpeMap > 20000 && idSpeMap < 30000)//竞技场
			{
				OBJID idWarGame = idSpeMap % 10;
				if(idWarGame==0)
				{
					LOGWARNING("Enter WarGame error ! idWarGame==0, m_SpecilMapID = %u, userid=%u",idSpeMap, m_pUser->GetID());
					SetState(STATE_SPECILLINE_OUT);
					ChangeLineMap(m_pUser->GetRecordMapID(),m_pUser->GetRecordPosX(),m_pUser->GetRecordPosY(),RANDLINE);//任意线
					bNeedChange = true;
				}
				else
				{
					pMap = WargameManager()->GetWarGameMap(idWarGame, m_pUser, nPosX, nPosY);
					
					if (pMap && WargameManager()->CanUserEnter(idWarGame))
					{
						idMap = pMap->GetID();
						SetState(STATE_SPECILLINE_NORMAL);
					}
					else
					{
						LOGWARNING("Can not found WarGame map! m_SpecilMapID = %u, userid=%u",idSpeMap, m_pUser->GetID());
						SetState(STATE_SPECILLINE_OUT);
						ChangeLineMap(m_pUser->GetRecordMapID(),m_pUser->GetRecordPosX(),m_pUser->GetRecordPosY(),RANDLINE);//任意线
						bNeedChange = true;
					}
				}
			}
			else//其他，把他T到其他线
			{
				pMap = MapManager()->GetGameMap(idSpeMap);
				if (pMap)
				{
					if(pMap->GetType()==3)
					{
						SetState(STATE_SPECILLINE_NORMAL);//正常进入
						idMap = pMap->GetID();
						nPosX	= nSpePosX;
						nPosY	= nSpePosY;
					}
					else
					{
						ChangeLineMap(m_pUser->GetRecordMapID(),m_pUser->GetRecordPosX(),m_pUser->GetRecordPosY(),RANDLINE);//任意线
						bNeedChange = true;
					}
				}
				else
				{
					ChangeLineMap(m_pUser->GetRecordMapID(),m_pUser->GetRecordPosX(),m_pUser->GetRecordPosY(),RANDLINE);//任意线
					bNeedChange = true;
				}
			}
		}
		else//普通线
		{
			if(nSpeState == STATE_SPECILLINE_REQUEST)
			{
				if(idSpeMap > 20000 && idSpeMap < 30000)//竞技场
				{
					DWORD nExitTime = m_pUser->GetExitTime();
					if (nExitTime != 0)
					{
						int nYear = 2000 + nExitTime/100000000;
						int nMonth = (nExitTime/1000000) % 100;
						int nDay = (nExitTime/10000) % 100;
						int nHour = (nExitTime/100) % 100;
						int nMin = nExitTime % 100;
						
						CTime timecount(nYear, nMonth, nDay, nHour, nMin, 0);
						CTime timeCurr(CTime::GetCurrentTime());
						CTimeSpan spanTime = timeCurr - timecount;
						int nPassHour = spanTime.GetTotalHours();
						if(nPassHour == 0)//下线1小时以内才尝试返回竞技场,否则有可能是昨天在竞技场下线的玩家，却巧合进了今天的竞技场
						{
							ChangeLineMap(idSpeMap, nSpePosX, nSpePosY, SPECILLINE);
							bNeedChange = true;
						}
					}
				}
				else
				{
					ChangeLineMap(idSpeMap, nSpePosX, nSpePosY, SPECILLINE);//专线
					bNeedChange = true;
				}
				SetState(STATE_SPECILLINE_SEC);
			}
		}
	}
	else//不是请求进专线
	{
		if (g_SpecilLine==1)//我是专线！可能是错误进入！把他T走
		{
			ChangeLineMap(m_pUser->GetRecordMapID(),m_pUser->GetRecordPosX(),m_pUser->GetRecordPosY(),RANDLINE);
			bNeedChange = true;
		}
	}
	DEBUG_CATCH3("CSpecilLineInfo::ProcessLogin iduser=%u , idmap=%u",m_pUser->GetID(),idSpeMap)
	return bNeedChange;
}

bool CSpecilLineInfo::ChangeLineMap(OBJID MapID,int nPosx,int nPosy,CHANGELINE nLine)//向指定的线路换地图--jay
{
	CHECKF(m_pUser);

	if (nLine < 0)
		return false;

	if(MapID==ID_NONE)
	{
		if(nLine==SPECILLINE)
		{
			::LogSave("CSpecilLineInfo::ChangeLineMap specil line map id error!");
			SetState(STATE_SPECILLINE_OUT);
			return false;
		}
		MapID = DEFAULT_LOGIN_MAPID;
		nPosx = DEFAULT_LOGIN_POSX;
		nPosy = DEFAULT_LOGIN_POSY;
	}

	//专线的还往专线切
	if(nLine == SPECILLINE && g_SpecilLine==1)
		return false;

	//普通线还往普通线切
	if(nLine == RANDLINE && g_SpecilLine!=1)
		return false;

	if (nLine == SPECILLINE)
	{
		if(m_pUser->GetMapID()==ID_NONE)
			m_pUser->SetRecordPos(m_pUser->GetRecordMapID(), m_pUser->GetRecordPosX(), m_pUser->GetRecordPosY());
		else
			m_pUser->SetRecordPos(m_pUser->GetMapID(), m_pUser->GetPosX(), m_pUser->GetPosY());
		SetRecordPos(MapID, nPosx, nPosy);
		SetState(STATE_SPECILLINE_REQUEST);
	}
	else
	{
		m_pUser->SetRecordPos(MapID, nPosx, nPosy);
		SetRecordPos(GetInt(SPECILLINEINFO_MAPID), GetInt(SPECILLINEINFO_POSX), GetInt(SPECILLINEINFO_POSY));
		SetState(STATE_SPECILLINE_OUT);
	}
	CMsgSyndicate msg;
	if(msg.Create(SYN_CHANGELINE,nLine))
		m_pUser->SendMsg(&msg);

	m_pUser->SetChangeLine(true);

	return true;
}