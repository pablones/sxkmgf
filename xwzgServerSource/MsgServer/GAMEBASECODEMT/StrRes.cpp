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
		//char	szFile[]="";    
	char szIniFile[] = "ini\\StrRes_ser.ini";
	
	FILE* fpStr = fopen( szIniFile,"rb" );

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
		if(strlen(szLine) == 0)
			continue;
		if(szLine[0] == ';')
			continue;
		if(szLine[strlen(szLine)-1] == 10)
		{
			szLine[strlen(szLine)-1] = '\0';
		}
	

		OBJID id = ID_NONE;
		static char szFile[2048]	= "";
		
		if (2 == sscanf(szLine, "%u=%s", &id, szFile))
		{
			if(szLine[strlen(szLine) -1] == 13)
			{
				szLine[strlen(szLine) -1] = '\0';
			}
			//----------------------------------
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
//-------------------------------------------------------------------
void CStrRes::Destroy()
{
	DWORD dwAmount = m_setStr.size();
	map<OBJID, char*>::iterator iter = m_setStr.begin();
	for(DWORD dwCounter = 0; dwCounter < dwAmount; dwCounter++)
	{
		char* pszStr = iter->second;
		iter++;
		//[2009.07.05]释放的是数组SAFE_DELETE(pszStr);
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
//-------------------------------------------------------------------
const char*	CStrRes::GetStr(OBJID idRes)
{
	map<OBJID, char*>::iterator iter = m_setStr.find(idRes);
	if (iter != m_setStr.end())
		return iter->second;
	return g_szStrResError;
}
//-------------------------------------------------------------------
