// UserMaterialItem.cpp: implementation of the CUserMaterialItem class.
//
//////////////////////////////////////////////////////////////////////

#include "UserMaterialItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUserMaterialItem* CUserMaterialItem::s_Instance = NULL;

CUserMaterialItem::CUserMaterialItem()
{

}

CUserMaterialItem::~CUserMaterialItem()
{	
	m_setLoverHomeBed.clear();
	m_setLoverHomeStorage.clear();
	m_setLoverHomeFlower.clear();
	m_setLoverHomeCabinet.clear();
}

CUserMaterialItem* CUserMaterialItem::CreateNew()
{
	if(!s_Instance)
	{
		s_Instance = new CUserMaterialItem;
		if(!s_Instance)
			return NULL;
	}
	return s_Instance;
}

void CUserMaterialItem::Release()
{
	if(s_Instance)
	{
		delete s_Instance;
		s_Instance = NULL;
	}
}

bool CUserMaterialItem::Init()
{
	char szIniFile[] = "ini\\LoverHomeMaterial.ini";
    
// 	char szLine[1024];
// 	int size;
	int dwData[4];
	char szTitle[32];

	FILE* fp = fopen( szIniFile, "r");
	//ASSERT(fp != NULL); 
	if(!fp)
	{
		::LogSave("缺少LoverHomeMaterial.ini文件。");//Error: null msg point found in CGameSocket::SendNpcMsg.
		return false;
	}

    
	while(!feof(fp))
	{
		LoverHomeMaterial info;
		memset( &info,0,sizeof( info ) );

		int nResult = fscanf(fp, "%u %s %u %u\n",&dwData[0],szTitle,&dwData[2],&dwData[3]);

		if (nResult != 4)
		{
			::LogSave("LoverHomeMaterial.ini字段数非法错误!");
			return false;
		}
		
		info.mat = dwData[2];
		info.num = dwData[3];
		_snprintf(info.szTitle,sizeof(char)*32,"%s",szTitle);

        switch (dwData[0])
		{
		case 1:  //床
		    m_setLoverHomeBed.push_back(info);
			break;
		case 2:  //柜子
            m_setLoverHomeStorage.push_back(info);
			break;
		case 3: //花盆
			m_setLoverHomeFlower.push_back(info);
			break;
		case 4: //厨房
			m_setLoverHomeCabinet.push_back(info);
			break;
		}
	}
	fclose(fp);
	return true;
}