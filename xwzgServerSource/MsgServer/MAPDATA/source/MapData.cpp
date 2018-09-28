//----------------------------------------------------//
// InterActiveLayer.cpp
//----------------------------------------------------//
#include "typedef.h"
#include "MapData.h"
#include "TerrainObj.h"
#include "common.h"
#include "inifile.h"
#include "../I_MapData.h"
#include "LOGFILE.h"
#include <math.h>

const int	_MAX_TITLESIZE			= 128;

//----------------------------------------------------//


//----------------------------------------------------//
CMapData::CMapData()
{
//	m_setCell.clear();
	m_setPassage.clear();
}
//----------------------------------------------------//
CMapData::~CMapData()
{
	this->ClearNodeSet();
	this->ClearCell();
	this->ClearPassage();
}
//----------------------------------------------------//
CMapData* CMapData::CreateNew(LPCTSTR pszFileName, LPCTSTR pszVersion)
{
	if(!pszFileName)
		return NULL;
	FILE* fp = fopen(pszFileName, "rb");
	if(!fp)
	{
		LOGERROR("无法打开地图文件[%s]", pszFileName);
		return NULL;
	}

	CMapData* ptr = new CMapData;
	if(!ptr)
	{
		fclose(fp);
		return NULL;
	}

	if(!ptr->LoadSurfaceCellData(fp)
		|| !ptr->LoadDataPassage(fp))
	{
		LOGERROR("地图文件[%s]数据装载错误！", pszFileName);
		SAFE_DELETE(ptr);
		fclose(fp);
		return NULL;
	}

	fclose(fp);
	return ptr;
}

bool CMapData::LoadSurfaceCellData(FILE* fp)
{
	char szTemp[1024];
	int data[4];
	fgets(szTemp,1024,fp);
	sscanf(szTemp,"%d %d %d %d\r\n",&data[0],&data[1],&data[2],&data[3]);

	SIZE infoSize = {data[2], data[3]};

	m_sizeMap		= infoSize;
	IF_NOT(m_setCell.Create(infoSize.cx*infoSize.cy))
		return false;

	int i, j;
	int	idx = 0;
	for(i = 0; i < infoSize.cx; i++)
	{
		DWORD dwCheckData = 0;
		for(j = 0; j < infoSize.cy; j++)
		{
			UCHAR dwMask	= 0;
			dwMask = fgetc(fp);
			if(dwMask == 222)
			{
				return false;
			}
			dwMask -= '0';
			if(dwMask > 10)
				return false;

			IF_OK(idx < m_setCell.size())
			{
				CCell* pCell = m_setCell[idx++];
				pCell->Create(dwMask);
			}
		}
	}
	return true;
}
//----------------------------------------------------//
bool CMapData::LoadDataPassage(FILE* fp)
{
	if(!fp)
		return false;

	char szTemp[1024];
	int data[4];
	fgets(szTemp,1024,fp);

	int nAmount = 0;
	fgets(szTemp,1024,fp);
	sscanf(szTemp,"passwayAmount=%d\r\n",&nAmount);
	for(int i= 0 ; i < nAmount ; i ++)
	{
		fgets(szTemp,1024,fp);
		sscanf(szTemp,"passway%d %d %d\r\n",&data[0],&data[1],&data[2]);
		if(data[1] < m_sizeMap.cx && data[2] < m_sizeMap.cy)
		{
			PassageInfo info;
			info.nIndex = data[0];
			info.nPosX = data[1];
			info.nPosY = data[2];
			AddPassage(&info);
		}
		else
		{
			return false;
		}
	}
	return true;
}

void CMapData::ClearCell()
{
/*	int nAmount = m_setCell.size();
	for(int i = 0; i < nAmount; i++)
	{
		CCell* pCell = m_setCell[i];
		SAFE_DELETE(pCell);
	}
*/	m_setCell.clear();
}
//----------------------------------------------------//
void CMapData::ClearPassage()
{
	int nAmount =  m_setPassage.size();
	for(int i = 0; i < nAmount; i++)
	{
		PassageInfo* pInfo = m_setPassage[i];
		SAFE_DELETE(pInfo);
	}
	m_setPassage.clear();
}

//----------------------------------------------------//
CCell*  CMapData::GetCell(int nIndex)
{
	int nAmount = m_setCell.size();
	if((nIndex < 0) || (nIndex >= nAmount))
		return NULL;
	return m_setCell[nIndex];
}
//----------------------------------------------------//
CCell*  CMapData::GetCell(int nCellX, int nCellY)
{
	return GetCell(POS2INDEX(nCellX, nCellY, m_sizeMap.cx, m_sizeMap.cy));
}
//----------------------------------------------------//
void CMapData::AddPassage(const PassageInfo* pInfo)
{
	CHECK(pInfo);

	POINT	posCell;
	posCell.x	= pInfo->nPosX;
	posCell.y	= pInfo->nPosY;
	AddPassage(posCell, pInfo->nIndex);
}
//----------------------------------------------------//
void CMapData::AddPassage(POINT posMap, int nIndex)
{
	// 兼容于地图编辑器
	{
		int nAmount =  m_setPassage.size();
		for(int i = 0; i < nAmount; i++)
		{
			PassageInfo* pInfo = m_setPassage[i];
			if(pInfo)
			{
				if((pInfo->nPosX == posMap.x) && (pInfo->nPosY == posMap.y))
				{
					pInfo->nIndex = nIndex;
					return;
				}
			}
		}
	}

	PassageInfo* pInfo = new PassageInfo;
	if(pInfo)
	{
		pInfo->nPosX = posMap.x;
		pInfo->nPosY = posMap.y;
		pInfo->nIndex = nIndex;
		m_setPassage.push_back(pInfo);
	}
}
//----------------------------------------------------//
void CMapData::DelPassage(POINT posMap)
{
	int nAmount =  m_setPassage.size();
	for(int i = 0; i < nAmount; i++)
	{
		PassageInfo* pInfo = m_setPassage[i];
		if(pInfo)
		{
			if((pInfo->nPosX == posMap.x) && (pInfo->nPosY == posMap.y))
			{
				SAFE_DELETE(pInfo);
				m_setPassage.erase(m_setPassage.begin()+i);
				// 兼容于编辑器	return ;
			}
		}
	}
}
//----------------------------------------------------//
int  CMapData::GetExitIndex(POINT posCell)			// return -1 : error
{
	int nAmount =  m_setPassage.size();
	for(int i = 0; i < nAmount; i++)
	{
		PassageInfo* pInfo = m_setPassage[i];
		if(pInfo)
		{
			if((pInfo->nPosX == posCell.x) && (pInfo->nPosY == posCell.y))
			{
				return pInfo->nIndex;
			}
		}
	}
	return -1;
}

///////////////////////////////////////////////////////////////////////////////////////
int  CMapData::GetMapWidth()
{
	return m_sizeMap.cx;
}

///////////////////////////////////////////////////////////////////////////////////////
int	 CMapData::GetMapHeight()
{
	return m_sizeMap.cy;
}

///////////////////////////////////////////////////////////////////////////////////////
CCell* CMapData::QueryCell(int nCellX, int nCellY)
{
	CHECKF(nCellX >= 0 && nCellX < m_sizeMap.cx);
	CHECKF(nCellY >= 0 && nCellY < m_sizeMap.cy);

	return GetCell(nCellX, nCellY);
}

///////////////////////////////////////////////////////////////////////////////////////
CCell* CMapData::QueryCell(POINT posCell)
{
	return QueryCell(posCell.x, posCell.y);
}

///////////////////////////////////////////////////////////////////////////////////////
int CMapData::GetPassage(int x, int y)
{
	for(int i = 0; i < m_setPassage.size(); i++)
	{
		if(m_setPassage[i]->nPosX == x && m_setPassage[i]->nPosY == y)
		{
			return m_setPassage[i]->nIndex;
		}
	}

	return PASSAGE_NONE;
}

///////////////////////////////////////////////////////////////////////////////////////
// global entry
///////////////////////////////////////////////////////////////////////////////////////
IMapData* IMapData::CreateNew(int nMapDoc, LPCTSTR pszVersion/*=0*/)
{
	CHECKF(nMapDoc != ID_NONE);

	char	szField[256];
	sprintf(szField, "Map%d", nMapDoc);
	CIniFile	ini("ini\\gamemap.ini", szField);
	char	szFileName[256];
	if(!ini.GetString(szFileName, "File", 256))
		return NULL;

	IMapData*	pObj = CMapData::CreateNew(szFileName, pszVersion);
	if(!pObj)
		return false;

	return pObj;
}

