// StrRes.cpp: implementation of the CStrRes class.
//
//////////////////////////////////////////////////////////////////////
#include "afx.h"
#include "StrRes.h"
#include "stdio.h"
#include "io.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStrRes g_objStrRes;


CStrRes::CStrRes()
{
	m_setStr.clear();
}

CStrRes::~CStrRes()
{
	Destroy();
}


void CStrRes::InitStrRes()
{
	FILE* fpStr = fopen( g_szResFile,"rb" );

	if (!fpStr)
	{
		return ;
	}
	long fileSize = _filelength( fileno( fpStr ) );
	int x = ftell(fpStr);
	char szLine[2048] = "";
	char szLineCopy[2048] = "";
	const int maxRow = 32;
	int nmax = maxRow;
	char szRow[maxRow][256];

	memset ( szRow, '\0',  sizeof( char ) * maxRow * 256  ) ;

	while(ftell(fpStr) - x < fileSize)
	{	
		if (NULL == fgets(szLine, 2047, fpStr))
			break;

		//包括等于至少有一个字符
		if(strlen(szLine) < 1)
			continue;

		//分号开头是注释
		if(szLine[0] == ';')
			continue;

		//去掉末尾换行回车空格
		while(1)
		{
			int nPos = strlen(szLine)-1;
			if(nPos < 1)
			{
				szLine[1] = 0;
				break;
			}

			if(szLine[nPos] == 10)
				szLine[nPos] = '\0';
			else if(szLine[nPos] == 13)
				szLine[nPos] = '\0';
			else
				break;
		}

		if(strlen(szLine) < 1)
			continue;		
	

		OBJID id = ID_NONE;
		static char szFile[2048]	= "";
		
		if (2 == sscanf(szLine, "%u=%s", &id, szFile))
		{
			int nLength = strlen(szLine);
			int nPos = 0;
			CString strTemp;
			for(int i = 0; i < nLength; i ++)
			{
				if(szLine[i] == '=')
				{
					nPos = i+1;
					break;
				}
			}

			// 从nPos开始拷贝
			for(int j = 0; j < nLength-nPos; j ++)
			{
				if(szLine[nPos+j] == '|')//换行
				{
					szLineCopy[j]='\n';
				}
				else
					szLineCopy[j]=szLine[nPos+j];
			}
			szLineCopy[j] = '\0';

			char* pszStr = new char[strlen(szLineCopy)+1];
			strcpy(pszStr, szLineCopy);
			pair<MAP_STRRES::iterator,bool> ret  = m_setStr.insert(make_pair(id,pszStr));
			if(ret.second==false)
			{
				delete[] pszStr;
			}
			
		}
	}
	fclose(fpStr);
}

void CStrRes::Init()
{
	InitStrRes();
}


void CStrRes::Destroy()
{
	DWORD dwAmount = m_setStr.size();
	map<OBJID, char*>::iterator iter = m_setStr.begin();
	for(DWORD dwCounter = 0; dwCounter < dwAmount; dwCounter++)
	{
		char* pszStr = iter->second;
		iter++;
		if(pszStr)
		{
			try
			{
				delete[] pszStr;
			}
			catch (...)
			{
				LOGERROR("CATCH: *** delete[] crash! ***\n");
			}
			pszStr = NULL;
		}
	}
	m_setStr.clear();
}

const char*	CStrRes::GetStr(OBJID idRes)
{
	map<OBJID, char*>::iterator iter = m_setStr.find(idRes);
	if (iter != m_setStr.end())
		return iter->second;
	return g_szStrResError;
}
