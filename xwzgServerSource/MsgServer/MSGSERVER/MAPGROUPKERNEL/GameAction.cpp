// GameAction.cpp: implementation of the CGameAction class.
//
//////////////////////////////////////////////////////////////////////

#include <ctype.h>
#include <afxdisp.h>
#include "../share/define.h"
#include "windows.h"
#include "I_Role.h"
#include "User.h"
#include "Item.h"
#include "GameAction.h"
#include "MapGroup.h"
#include "AllMsg.h"
#include "ActionDefine.h"
#include "WantedList.h"
#include "PoliceWanted.h"
#include "NpcLink.h"
#include "MapTrap.h"
#include "DeadLoop.h"
#include "string.h"
#include "EventPack.h"
#include "NpcTable.h"
#include "TaskDetailData.h"
#include "TaskDetail.h"
#include "EconomySystem.h"
#include "ResourceWar.h"
#include "AwardExp.h"//  [4/18/2007] halei
#include "WargameManager.h"///softworms-2007-08-28
#include "Wargame.h"
#include "SKPostOffice.h"
#include "Network/MsgNewPlayerExam.h"
#include "LuaScriptMachine.h"
//#include "SKMail.h"
#include "TopListSystem.h"
#include "MsgCityWar.h"
#include "MsgHouse.h"
#include "AwardMoney.h"
#include "LoverHome.h"
#include "ItemDropLimit.h"
#include "MapInstanceMgr.h"
//////////////////////////////////////////////////////////////////////
const int	PRESENTMONEY_LIMIT				= 10000;			// 基金划拨不少于10000
const int	ENTER_HOME_TEAM_DISTANCE		= 5;				// 组队进入家庭的距离
const int	MAX_UNLEARN_NUMBER				= 20;				// 忘却MAGIC最多支持条数
const int	SUBSYN_NAMESIZE					= 16;				// 子帮派名字长度
const int	MAX_ACCEPTSTR					= 4;				// 分析4个子串

MYHEAP_IMPLEMENTATION(CGameAction,s_heap)


const int MEMORY_CRYSTAL_UNRECORD = 930010;	//未记录的记忆水晶类型
const int MEMORY_CRYSTAL_RECORDED = 930011;	//已记录的记忆水晶类型
extern int g_SpecilLine;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////////////////////
void CGameAction::ReplaceAttrStr(char* pszTarget, const char* pszSource, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept/*=NULL*/)
{
	if(!pszTarget || !pszSource)
	{
		ASSERT(!"CJobNpc::ReplaceAttrStr");
		return;
	}
	
	if(strchr(pszSource, '%') == NULL)
	{
		SafeCopy(pszTarget, pszSource);
		return;
	}
	
	CNpc* pNpc = NULL;
	if(pRole)
		pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc));
	
	const char*	ptr = pszSource; 
	char*	ptr2 = pszTarget;
	while(*ptr)
	{
		if(*ptr == '%')
		{
			ptr++;
			
			// width
			int nWidth	= 0;
			if(isdigit(*ptr))
				nWidth	= nWidth*10 + (*(ptr++))-'0';
			if(isdigit(*ptr))
				nWidth	= nWidth*10 + (*(ptr++))-'0';		// 仅支持2位数
			const char* pNext = ptr2 + nWidth;
			
			if(*(ptr) == '%')
			{
				*ptr2	= '%';
				
				ptr	+= 2;
				ptr2++;
				// continue;
			}
			if (strnicmp(ptr, PARAM_DATE_STAMP, sizeof(PARAM_DATE_STAMP)-1) == 0)
			{
				char szString[256] = "(err)";
				sprintf(szString, "%d", ::DateStamp());
				
				strcpy(ptr2, szString);
				
				ptr += sizeof(PARAM_DATE_STAMP)-1;
				ptr2 += strlen(szString);
				// continue;
			}
			else if (strnicmp(ptr, PARAM_TIME, sizeof(PARAM_TIME)-1) == 0)
			{
				char szString[256] = "(err)";
				sprintf(szString, "%d", time(NULL));
				
				strcpy(ptr2, szString);
				
				ptr += sizeof(PARAM_TIME)-1;
				ptr2 += strlen(szString);
				// continue;
			}

			if(pUser)
			{
				if (strnicmp(ptr, PARA_AVAILABLE_FUND, sizeof(PARA_AVAILABLE_FUND)-1) == 0)
				{
					//帮派基金
					char szString[256] = "--";
					sprintf(szString, "%d", pUser->GetAvailableSynFund());
					
					strcpy(ptr2, szString);
					
					ptr += sizeof(PARA_AVAILABLE_FUND)-1;
					ptr2 += strlen(szString);
					
				}
			}
			if(pNpc)
			{
			}
			if(pItem)
			{
				if(strnicmp(ptr, PARA_ITEM_TYPE, sizeof(PARA_ITEM_TYPE)-1) == 0)
				{
					char	szNum[256] = "";
					sprintf(szNum, "%u", pItem->GetInt(ITEMDATA_TYPE));
					strcpy(ptr2, szNum);
					
					ptr += sizeof(PARA_ITEM_TYPE)-1;
					ptr2		+= strlen(szNum);
					// continue;
				}
			}
			
			// fill space
			if(nWidth)
			{
				while(ptr2 < pNext)
					*(ptr2++)	= ' ';
			}
		} // %
		else
		{
			*ptr2	= *ptr;
			
			ptr++;
			ptr2++;
		}
	} // while
	*ptr2	= 0;
}

bool CGameAction::ProcessScriptAction(OBJID idActionType,LPCTSTR szParam,int data, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept)
{
	if (idActionType == ID_NONE)
		return false;
	m_pUser	= pUser;
	m_pRole	= pRole;
	m_pItem	= pItem;
	bool	bRet = false;
	CScriptActionData sad;
	CActionData* pAction	= &sad;
	DEBUG_TRY
		// process action...
		//		const _MAX_ACTIONPRC	=64;//20070511修罗:策划需求.原32
		//		DWORD dwActionCount	=0;
		//		while (idAction != ID_NONE)
		//		{
		//			DEADLOOP_CHECK(PID, "idAction: ")
		
	sad.SetInt(ACTIONDATA_TYPE,idActionType);
	sad.SetStr(ACTIONDATA_PARAM,szParam);
	sad.SetInt(ACTIONDATA_DATA,data);
	
	char	strParam[1024];
	strcpy(strParam,pAction->GetStr(ACTIONDATA_PARAM));
//	ReplaceAttrStr(strParam, pAction->GetStr(ACTIONDATA_PARAM), m_pUser, m_pRole, m_pItem, pszAccept);
	strParam[255]	= 0;			// 截断
	
	//			CScopePtr<CScriptActionData> psad=new CScriptActionData;
	//			psad->SetInt(ACTIONDATA_TYPE,idActionType);
	//			psad->SetStr(ACTIONDATA_PARAM,szParam);
	//			psad->SetInt(ACTIONDATA_DATA,data);
	//
	//			CActionData* pAction	= &psad;
	//			if (!pAction)
	//			{
	//				::LogSave("Error: game action %u not found.", idAction);
	//				if(pUser )
	//					::LogSave("CGameAction::ProcessAction.Ln 812...pUser->GetName() =%s",pUser->GetName());
	//				break;
	//			}
	
	//			char	szParam[1024];
	//			ReplaceAttrStr(szParam, pAction->GetStr(ACTIONDATA_PARAM), m_pUser, m_pRole, m_pItem, pszAccept);
	//			szParam[255]	= 0;			// 截断
	
	//? FOR DEBUG
	//			if(m_pUser && m_pUser->IsPM())
	//			{
	//				char	szMsg[1024];
	//				sprintf(szMsg, "%d: [%d,%d]. type[%d], data[%d], param:[%s].", 
	//						pAction->GetID(),
	//						pAction->GetInt(ACTIONDATA_IDNEXT),
	//						pAction->GetInt(ACTIONDATA_IDNEXTFAIL),
	//						pAction->GetInt(ACTIONDATA_TYPE), 
	//						pAction->GetInt(ACTIONDATA_DATA),
	//						szParam);
	//// '%' in szMsg				this->SendSysMsg(_TXTATR_NORMAL, "ACTION: %s", szMsg);
	//				szMsg[255]	= 0;
	//				CMsgTalk msg;
	//				if (msg.Create(SYSTEM_NAME, m_pUser->GetName(), szMsg, NULL, 0x00ffff00, _TXTATR_NORMAL))
	//					m_pUser->SendMsg(&msg);
	//			}
	
	// process action now!
	int nActionType = pAction->GetInt(ACTIONDATA_TYPE);
	if(nActionType > ACTION_SYS_FIRST && nActionType < ACTION_SYS_LIMIT)
		bRet	= ProcessActionSys	(pAction, strParam, m_pUser, m_pRole, m_pItem, pszAccept);
	else if(nActionType > ACTION_NPC_FIRST && nActionType < ACTION_NPC_LIMIT)
		bRet	= ProcessActionNpc	(pAction, strParam, m_pUser, m_pRole, m_pItem, pszAccept);
	else if(nActionType > ACTION_MAP_FIRST && nActionType < ACTION_MAP_LIMIT)
		bRet	= ProcessActionMap	(pAction, strParam, m_pUser, m_pRole, m_pItem, pszAccept);
	else if(nActionType > ACTION_ITEMONLY_FIRST && nActionType < ACTION_ITEMONLY_LIMIT)
		bRet	= ProcessActionItemOnly	(pAction, strParam, m_pUser, m_pRole, m_pItem, pszAccept);
	else if(nActionType > ACTION_ITEM_FIRST && nActionType < ACTION_ITEM_LIMIT)
		bRet	= ProcessActionItem	(pAction, strParam, m_pUser, m_pRole, m_pItem, pszAccept);
	else if(nActionType > ACTION_SYN_FIRST && nActionType < ACTION_SYN_LIMIT)
		bRet	= ProcessActionSyn	(pAction, strParam, m_pUser, m_pRole, m_pItem, pszAccept);
	else if(nActionType > ACTION_USER_FIRST && nActionType < ACTION_USER_LIMIT)
		bRet	= ProcessActionUser	(pAction, strParam, m_pUser, m_pRole, m_pItem, pszAccept);
	else if(nActionType > ACTION_MST_FIRST && nActionType < ACTION_MST_LIMIT)
		bRet	= ProcessActionMonster (pAction, strParam, m_pUser, m_pRole, m_pItem, pszAccept);
	else if(nActionType > ACTION_MAP_FIRST && nActionType < ACTION_MAGIC_LIMIT)
		bRet	= ProcessActionMagic(pAction, strParam, m_pUser, m_pRole, m_pItem, pszAccept);
	else if(nActionType > ACTION_SCRIPT_FIRST && nActionType < ACTION_SCRIPT_LIMIT)
		bRet	= ProcessActionScript(pAction, strParam, m_pUser, m_pRole, m_pItem, pszAccept);
	else
	{
		LOGWARNING("Error action type! nActionType = %d",nActionType);
	}
	
	//			if (bRet)
	//				idAction	= pAction->GetInt(ACTIONDATA_IDNEXT);
	//			else
	//				idAction	= pAction->GetInt(ACTIONDATA_IDNEXTFAIL);
	//
	//			if (dwActionCount++ >= _MAX_ACTIONPRC)
	//			{
	//				::LogSave("Error: too many game action, last action:%u.", idAction);
	//				break;
	//			}
	//		} // while
	DEBUG_CATCH2("Error: exception catched in CGameAction::ProcessScriptActionu(%u)", idActionType)
	return bRet;
}

////////////////////////////////////////////////////////////////////////////////////
bool CGameAction::ProcessAction(OBJID idAction, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept,bool bNeedEnd)
{
	if (idAction == ID_NONE)
		return false;
	
	bool bEnd = false;
	m_pUser	= pUser;
	m_pRole	= pRole;
	m_pItem	= pItem;
	/*
	try
	{
	// process action...
	const _MAX_ACTIONPRC	=64;//20070511修罗:策划需求.原32
	DWORD dwActionCount	=0;
	while (idAction != ID_NONE)
	{
	DEADLOOP_CHECK(PID, "idAction: ")
	
	  CActionData* pAction	= ActionSet()->GetObj(idAction);
	  if (!pAction)
	  {
	  ::LogSave("Error: game action %u not found.", idAction);
	  if(pUser )
	  ::LogSave("CGameAction::ProcessAction.Ln 812...pUser->GetName() =%s",pUser->GetName());
	  break;
	  }
	  
		char	szParam[1024];
		ReplaceAttrStr(szParam, pAction->GetStr(ACTIONDATA_PARAM), m_pUser, m_pRole, m_pItem, pszAccept);
		szParam[255]	= 0;			// 截断
		
		  //? FOR DEBUG
		  if(m_pUser && m_pUser->IsPM())
		  {
		  char	szMsg[1024];
		  sprintf(szMsg, "%d: [%d,%d]. type[%d], data[%d], param:[%s].", 
		  pAction->GetID(),
		  pAction->GetInt(ACTIONDATA_IDNEXT),
		  pAction->GetInt(ACTIONDATA_IDNEXTFAIL),
		  pAction->GetInt(ACTIONDATA_TYPE), 
		  pAction->GetInt(ACTIONDATA_DATA),
		  szParam);
		  // '%' in szMsg				this->SendSysMsg(_TXTATR_NORMAL, "ACTION: %s", szMsg);
		  szMsg[255]	= 0;
		  CMsgTalk msg;
		  if (msg.Create(SYSTEM_NAME, m_pUser->GetName(), szMsg, NULL, 0x00ffff00, _TXTATR_NORMAL))
		  m_pUser->SendMsg(&msg);
		  }
		  
			// process action now!
			bool	bRet = false;
			int nActionType = pAction->GetInt(ACTIONDATA_TYPE);
			if(nActionType > ACTION_SYS_FIRST && nActionType < ACTION_SYS_LIMIT)
			bRet	= ProcessActionSys	(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
			else if(nActionType > ACTION_NPC_FIRST && nActionType < ACTION_NPC_LIMIT)
			bRet	= ProcessActionNpc	(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
			else if(nActionType > ACTION_MAP_FIRST && nActionType < ACTION_MAP_LIMIT)
			bRet	= ProcessActionMap	(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
			else if(nActionType > ACTION_ITEMONLY_FIRST && nActionType < ACTION_ITEMONLY_LIMIT)
			bRet	= ProcessActionItemOnly	(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
			else if(nActionType > ACTION_ITEM_FIRST && nActionType < ACTION_ITEM_LIMIT)
			bRet	= ProcessActionItem	(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
			else if(nActionType > ACTION_NPCONLY_FIRST && nActionType < ACTION_NPCONLY_LIMIT)
			bRet	= ProcessActionNpcOnly	(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
			else if(nActionType > ACTION_SYN_FIRST && nActionType < ACTION_SYN_LIMIT)
			bRet	= ProcessActionSyn	(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
			else if(nActionType > ACTION_USER_FIRST && nActionType < ACTION_USER_LIMIT)
			bRet	= ProcessActionUser	(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
			else if(nActionType > ACTION_EVENT_FIRST && nActionType < ACTION_EVENT_LIMIT)
			bRet	= ProcessActionEvent	(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
			else if(nActionType > ACTION_TRAP_FIRST && nActionType < ACTION_TRAP_LIMIT)
			bRet	= ProcessActionEvent	(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
			else if(nActionType > ACTION_WANTED_FIRST && nActionType < ACTION_WANTED_LIMIT)
			bRet	= ProcessActionWanted	(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
			else if(nActionType > ACTION_MST_FIRST && nActionType < ACTION_MST_LIMIT)
			bRet	= ProcessActionMonster (pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
			else if(nActionType > ACTION_MAP_FIRST && nActionType < ACTION_MAGIC_LIMIT)
			bRet	= ProcessActionMagic(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
			else
			{
			ASSERT(!"Error action type!");
			}
			
			  if (bRet)
			  idAction	= pAction->GetInt(ACTIONDATA_IDNEXT);
			  else
			  idAction	= pAction->GetInt(ACTIONDATA_IDNEXTFAIL);
			  
				if (dwActionCount++ >= _MAX_ACTIONPRC)
				{
				::LogSave("Error: too many game action, last action:%u.", idAction);
				break;
				}
				} // while
				}
				catch(...)
				{
				ASSERT(!"catch");
				::LogSave("Error: exception catched in CGameAction::ProcessAction(%u)", idAction);
				}
	*/
	//-------彭峰---2007/05/23-----
	int nActionType =0;
	char	szParam[1024]="";
	DEBUG_TRY
		// process action...
	const _MAX_ACTIONPRC	=64;//20070511修罗:策划需求.原32
	DWORD dwActionCount	=0;
	while (idAction != ID_NONE)
	{
		DEADLOOP_CHECK(PID, "idAction: ")
			
			CActionData* pAction	= ActionSet()->GetObj(idAction);
		if (!pAction)
		{
			::LogSave("Error: game action %u not found.", idAction);
			if(pUser )
				::LogSave("CGameAction::ProcessAction.Ln 812...pUser->GetName() =%s",pUser->GetName());
			break;
		}
		
		strcpy(szParam,pAction->GetStr(ACTIONDATA_PARAM));
//		ReplaceAttrStr(szParam, pAction->GetStr(ACTIONDATA_PARAM), m_pUser, m_pRole, m_pItem, pszAccept);
		szParam[255]	= 0;			// 截断
		
		//? FOR DEBUG
// 		if(m_pUser && m_pUser->IsPM())
// 		{
// 			char	szMsg[1024];
// 			sprintf(szMsg, "%d: [%d,%d]. type[%d], data[%d], param:[%s].", 
// 				pAction->GetID(),
// 				pAction->GetInt(ACTIONDATA_IDNEXT),
// 				pAction->GetInt(ACTIONDATA_IDNEXTFAIL),
// 				pAction->GetInt(ACTIONDATA_TYPE), 
// 				pAction->GetInt(ACTIONDATA_DATA),
// 				szParam);
// 			// '%' in szMsg				this->SendSysMsg(_TXTATR_NORMAL, "ACTION: %s", szMsg);
// 			szMsg[255]	= 0;
// 			CMsgTalk msg;
// 			if (msg.Create(SYSTEM_NAME, m_pUser->GetName(), szMsg, NULL, 0x00ffff00, _TXTATR_NORMAL))
// 				m_pUser->SendMsg(&msg);
// 		}
		
		// process action now!
		bool	bRet = false;
		nActionType = pAction->GetInt(ACTIONDATA_TYPE);
		if(nActionType > ACTION_SYS_FIRST && nActionType < ACTION_SYS_LIMIT)
		{
			if(440000001 == pAction->GetID())
			{
				sprintf(szParam ,"%d" ,9999);
			}
			bRet	= ProcessActionSys	(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
		}
		else if(nActionType > ACTION_NPC_FIRST && nActionType < ACTION_NPC_LIMIT)
			bRet	= ProcessActionNpc	(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
		else if(nActionType > ACTION_MAP_FIRST && nActionType < ACTION_MAP_LIMIT)
			bRet	= ProcessActionMap	(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
		else if(nActionType > ACTION_ITEMONLY_FIRST && nActionType < ACTION_ITEMONLY_LIMIT)
			bRet	= ProcessActionItemOnly	(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
		else if(nActionType > ACTION_ITEM_FIRST && nActionType < ACTION_ITEM_LIMIT)
			bRet	= ProcessActionItem	(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
		else if(nActionType > ACTION_SYN_FIRST && nActionType < ACTION_SYN_LIMIT)
			bRet	= ProcessActionSyn	(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
		else if(nActionType > ACTION_USER_FIRST && nActionType < ACTION_USER_LIMIT)
			bRet	= ProcessActionUser	(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
		else if(nActionType > ACTION_MST_FIRST && nActionType < ACTION_MST_LIMIT)
			bRet	= ProcessActionMonster (pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
		else if(nActionType > ACTION_MAP_FIRST && nActionType < ACTION_MAGIC_LIMIT)
			bRet	= ProcessActionMagic(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
		else if(nActionType > ACTION_SCRIPT_FIRST && nActionType < ACTION_SCRIPT_LIMIT)
			bRet	= ProcessActionScript(pAction, szParam, m_pUser, m_pRole, m_pItem, pszAccept);
		else
		{
			ASSERT(!"Error action type!");
		}
		
		if (bRet)
			idAction	= pAction->GetInt(ACTIONDATA_IDNEXT);
		else
			idAction	= pAction->GetInt(ACTIONDATA_IDNEXTFAIL);
		
		bEnd = bRet;
		if (dwActionCount++ >= _MAX_ACTIONPRC)
		{
			::LogSave("Error: too many game action, last action:%u.", idAction);
			break;
		}
	} // while
	DEBUG_CATCH4("Error: exception catched in CGameAction::ProcessAction(%u),nActionType:%d,szParam:%s", idAction,nActionType,szParam)

	if(bNeedEnd)
		return bEnd;
	return true;
}

bool CGameAction::ProcessActionScript(CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pazAccept)
{
	bool hr=false;
	char szUserName[50]="";
	char szRoleName[50]="";
	char szItemName[50]="";
DEBUG_TRY	// VVVVVVVVVVVVVV
	CHECKF(pAction);
	//	if (pUser) 
	//	{
	//		CMonster* pMonster=NULL;
	//		CUser*	  pDstUser=NULL;
	//		CNpc*	  pNpc=NULL;
	//		if (pRole)
	//		{
	//			pRole->QueryObj(OBJ_MONSTER,IPP_OF(pMonster));
	//			pRole->QueryObj(OBJ_USER,IPP_OF(pDstUser));
	//			pRole->QueryObj(OBJ_NPC,IPP_OF(pNpc));
	//		}		
	//		LuaScriptMachine()->InitScript(pUser,pAction->GetInt(ACTIONDATA_ID_),pDstUser,pMonster,pItem);
	//		hr=LuaScriptMachine()->RunScriptFile(szParam);
	//	}
	if(pUser)
		strncpy(szUserName,pUser->GetName(),_MAX_NAMESIZE);
	if(pRole)
		strncpy(szRoleName,pRole->GetName(),_MAX_NAMESIZE);
	if(pItem)
		strncpy(szItemName,pItem->GetStr(ITEMDATA_NAME),_MAX_NAMESIZE);
	LuaScriptMachine()->InitScript(pUser,pAction->GetInt(ACTIONDATA_ID_),pRole,pItem);
	hr=LuaScriptMachine()->RunScriptFile(szParam);

	//[游途道标 2009.05.08]如果不是管理员帐号则不显示
	if (LuaScriptMachine()->IsHasError() && pUser)///如果执行脚本有错误,则显示错误在客户端
	{
		if (pUser->IsGM())
		{
			char szTemp[2048];
			sprintf(szTemp, "提示:%s脚本执行错误,错误原因为:[%s].",szParam, LuaScriptMachine()->GetLastError());
			pUser->SendSysInfo(szTemp);
		}
	}

	//	switch(pAction->GetInt(ACTIONDATA_TYPE))
	//	{
	//		case ACTION_WARGAME_CANSIGNUP:///战争活动报名条件判断
	//			hr=WargameManager()->CanSignup(pAction->GetInt(ACTIONDATA_DATA));
	//			break;
	//		case ACTION_WARGAME_SIGNUP:///战争活动报名条件判断
	//			hr=WargameManager()->Signup(pAction->GetInt(ACTIONDATA_DATA),pUser);
	//			break;
	//		case ACTION_WARGAME_QUERYRESULT:	//显示战争活动得分明细
	//			{
	//				CMsgWargame msg;
	//				msg.m_pInfo->nWargameType=WARGAME_TYPE_RESULT;
	//				msg.m_pInfo->nWargameStatus=WARGAME_TYPE_RESULT_SHOWUI;
	//				msg.m_pInfo->nPararm=0;///初始化为第0页
	//				pUser->SendMsg(&msg);
	//			}
	//			break;
	//		default:
	//			LOGERROR("错误的Action Type", pAction->GetInt(ACTIONDATA_TYPE));
	//			break;
	//	}
	//	return hr;
DEBUG_CATCH4("CGameAction::ProcessActionScript crash!szParam:%s,szUserName:%s,szRoleName:%s",szParam,szUserName,szRoleName)	// AAAAAAAAAAA
	return hr;
}

//////////////////////////////////////////////////////////////////////
bool CGameAction::ProcessActionMonster(CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept)
{
	IF_NOT (pAction)
		return false;
	
	CMonster* pMonster = NULL;
	IF_NOT (pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
		return false;
	
	switch(pAction->GetInt(ACTIONDATA_TYPE))
	{	
	default:
		LOGERROR("错误的Action Type", pAction->GetInt(ACTIONDATA_TYPE));
	}
	
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CGameAction::ProcessActionSys(CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept)
{
	CHECKF(pAction);
	
	switch(pAction->GetInt(ACTIONDATA_TYPE))
	{
	case	ACTION_CHKTIME:
		{
			if (CMapGroup::CheckTime(pAction->GetInt(ACTIONDATA_DATA), szParam))
				return true;
		}
		break;
		
	case	ACTION_MENUTEXT:
		{
			if(!pUser)
			{
				LOGERROR("Can't find user object when action[%d].", pAction->GetID());
				return false;
			}

			pUser->ClearTaskID();
			
			CMsgDialog	msg;
			CHECKF(msg.Create(MSGDIALOG_TEXT, szParam, INDEX_NONE/*, pAction->GetInt(ACTIONDATA_DATA)*/));
			pUser->SendMsg(&msg);
			return true;
		}
		break;
	case	ACTION_MENULINK:
		{
			CHECKF(pUser);
			
			PARAMBUF	szText;
			OBJID		idAction;
			OBJID		nIndex = 0;
			if(sscanf(szParam, "%s %u", szText, &idAction) < 2)
			{
				LOGERROR("ACTION %u: 错误的参数数量", pAction->GetID());
				break;
			}
			if(idAction != 0 )
				nIndex = pUser->PushTaskID(idAction) + 1;

			CMsgDialog	msg;
			CHECKF(msg.Create(MSGDIALOG_LINK, szText,nIndex/*, 0*/));
			pUser->SendMsg(&msg);
			return true;
		}
		break;
	case	ACTION_MENUSHOW:
		{
			CHECKF(pUser);

			CMsgDialog	msg;
			CHECKF(msg.Create(MSGDIALOG_SHOW, "", 0));
			pUser->SendMsg(&msg);
			return true;
		}
		break;
	case	ACTION_RAND:
		{
			int		nData1, nData2;
			if(sscanf(szParam, "%u %u", &nData1, &nData2) < 2)
			{
				LOGERROR("ACTION %u: 错误的参数数量", pAction->GetID());
				break;
			}
			
			if(::RandGet(nData2) < nData1)
				return true;
			else
				return false;
		}
		break;
		
	case ACTION_RANDACTION:
		{
			OBJID idAction[10];
			if (10 == sscanf(szParam, "%u %u %u %u %u %u %u %u %u %u", 
				&idAction[0], &idAction[1], &idAction[2], &idAction[3],
				&idAction[4], &idAction[5], &idAction[6], &idAction[7],
				&idAction[8], &idAction[9]))
			{
				ProcessAction(idAction[::RandGet(10)], pUser, pRole, pItem, pszAccept);
				return true;
			}
			else
				LOGERROR("ACTION %u: 错误的参数数量", pAction->GetID());
		}
		break;
	case ACTION_ERRORMESSAGEBOX:
		{//让客户端弹出错误操作提示对话框20090410haocq添加
			if(pUser)
			{
 				int nChannel = _TXTATR_TOPSHOW;

				if(pAction->GetInt(ACTIONDATA_DATA) > 0)
					nChannel = pAction->GetInt(ACTIONDATA_DATA);
				
				CMsgTalk	msg;
				IF_OK(msg.Create(nChannel, szParam))
					pUser->SendMsg(&msg);
				
				return true;
			}
		}
		break;

	case ACTION_CHATSYSBOX:
		{
			if(pUser)
			{
				CMsgTalk msg;
				if (msg.Create("系统", pUser->GetName(), szParam, NULL, 0xFFFFFFFF, _TXTATR_CHATWITH))
					pUser->SendMsg(&msg);
				
				return true;
			}
		}
		break;
		
	default:
		LOGERROR("不支持的ACTION[%u]类型[%u]", pAction->GetKey(), pAction->GetInt(ACTIONDATA_TYPE));
	} // switch
	
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CGameAction::ProcessActionNpc(CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept)
{
	CHECKF(pAction);
	
	switch(pAction->GetInt(ACTIONDATA_TYPE))
	{
	case ACTION_NPC_ISTTAG:
		{
			int monsterID = pAction->GetInt(ACTIONDATA_DATA);
			return pUser->checkMonsterTag(monsterID);
		}
		break;
	case	ACTION_NPC_ERASE:
		{
			CHECKF(pAction && pRole);
			
			CNpc* pNpc;
			IF_NOT(pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)))
				return false;
			
			if(pNpc->IsDeleted())
				return false;
			
			int	nType = pAction->GetInt(ACTIONDATA_DATA);
			if(nType == 0)
			{
				m_pRole	= NULL;			// 清除任务NPC
				return pNpc->DelNpc();
			}
			else
			{
				for(IRoleSet::Iter i = RoleManager()->QuerySet()->Begin(); i != RoleManager()->QuerySet()->End(); )
				{
					IRole* pRole = RoleManager()->QuerySet()->GetObjByIter(i);
					i++;		// 要删除，提前移动
					
					CNpc* pNpc;
					if(pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->GetType() == nType)
					{
						if(!pNpc->DelNpc())
							return false;
					}
				}
				return true;
			}
		}
		break;	
	default:
		LOGERROR("不支持的ACTION[%u]类型[%u]", pAction->GetKey(), pAction->GetInt(ACTIONDATA_TYPE));
	}
	
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CGameAction::ProcessActionMap(CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem_no_use, LPCTSTR pszAccept)
{
	CHECKF(pAction);
	
	switch(pAction->GetInt(ACTIONDATA_TYPE))
	{
	case	ACTION_MAP_MOVENPC:
		{
			CNpc* pNpc;
			IRole* pRole = RoleManager()->QuerySet()->GetObj(pAction->GetInt(ACTIONDATA_DATA));
			if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)))
			{
				IF_NOT(!pNpc->IsDynaNpc())
				{
					LOGERROR("ACTION_MAP_MOVENPC 仅用于固定NPC[%d]", pNpc->GetID());
					return false;
				}
				
				OBJID idMap = ID_NONE;
				int nPosX = 0, nPosY = 0;
				
				if (3 == sscanf(szParam, "%u %d %d", &idMap, &nPosX, &nPosY))
				{
					if (pNpc->ChangePos(idMap, nPosX, nPosY))
						return true;
				}
				else
					LOGERROR("ACTION %u: 错误的参数数量", pAction->GetID());
			}
		}
		break;		
	case	ACTION_MAP_MAPUSER:
		{
			char szCmd[256] = "", szOpt[256] = "";
			int nData = 0;
			if (3 == sscanf(szParam, "%s %s %d", szCmd, szOpt, &nData))
			{
				OBJID idMap = pAction->GetInt(ACTIONDATA_DATA);
				vector<CUser*> setUser;
				
				if (0 == stricmp("map_user", szCmd))
				{
					CUserManager::Iterator pUser = UserManager()->NewEnum();
					while(pUser.Next())
					{
						if (pUser && pUser->GetMapID() == idMap)
							setUser.push_back(pUser);
					}
				}
				else if (0 == stricmp("alive_user", szCmd))
				{
					CUserManager::Iterator pUser = UserManager()->NewEnum();
					while(pUser.Next())
					{
						if (pUser && pUser->GetMapID() == idMap && pUser->IsAlive())
							setUser.push_back(pUser);
					}
				}
				else
				{
					LOGERROR("ACTION %u: 错误的param参数", pAction->GetID());
					return false;
				}
				
				
				if (0 == stricmp(szOpt, "=="))
				{
					if (setUser.size() == nData)
						return true;
				}
				else if (0 == stricmp(szOpt, "<="))
				{
					if (setUser.size() <= nData)
						return true;
				}
				else if (0 == stricmp(szOpt, ">="))
				{
					if (setUser.size() >= nData)
						return true;
				}
			}
			else
				LOGERROR("ACTION %u: 错误的参数数量", pAction->GetID());
		}
		break;
		
	case	ACTION_MAP_BROCASTMSG:
		{
			CMsgTalk msg;
			IF_OK (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szParam, NULL, 0x0ffffff, _TXTATR_SYSTEM))
				UserManager()->BroadcastMapMsg(pAction->GetInt(ACTIONDATA_DATA), &msg);
			
			return true;
		}
		break;
	default:
		LOGERROR("不支持的ACTION[%u]类型[%u]", pAction->GetKey(), pAction->GetInt(ACTIONDATA_TYPE));
	}
	
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CGameAction::ProcessActionItemOnly(CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept)
{
	CHECKF(pAction && pUser/* && pItem*/);
	
	switch(pAction->GetInt(ACTIONDATA_TYPE))
	{
	case	ACTION_ITEM_DELTHIS:
		{
			if(pUser)
			{
				int nItemType = pAction->GetInt(ACTIONDATA_DATA);
				int nAmount = 0;
				
				if(sscanf(szParam, "%u", &nAmount) != 1)
					return false;
//modify code by 林德渊 begin 2011.5.23
				CItemPtr pItem1 = pUser->GetItemByType(nItemType);
				bool bIsCostly1 = false;
				int nType1 = 0;
				if( pItem1 )
				{
					bIsCostly1 = pItem1->IsCostlyItem();
					nType1 = pItem1->GetTypeID();
				}
//modify code by 林德渊 end
				bool bRet = false;
				DEBUG_TRY
				bool bHasKind = false;
				bRet = pUser->DelItemByTypeMulti(nItemType,nAmount,bHasKind);
//modify code by 林德渊 begin 2011.5.23
				if( bIsCostly1 && bRet )
					::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) lose %d %u from using", pUser->GetID(), pUser->GetName(), nAmount, nType1);
//modify code by 林德渊 end
				DEBUG_CATCH2("delitem error %u",nItemType);
				return bRet;
			}
			return false;
		}
		break;
	default:
		LOGERROR("不支持的ACTION[%u]类型[%u]", pAction->GetKey(), pAction->GetInt(ACTIONDATA_TYPE));
	}
	
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CGameAction::ProcessActionItem(CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept)
{
	//	CHECKF(pAction && pUser);//06.10.11王玉波测试
	if( !pAction || !pUser )
	{
		//int test;
		char str[] = "CGameAction::ProcessActionItem.06.10.21王玉波注释以上一句,加断点,看堆栈!";
		::LogSave(str);
		return false;
	}
	switch(pAction->GetInt(ACTIONDATA_TYPE))
	{
	case ACTION_ITEM_ADD_COUNT:
		{
			if(!pUser)
				return false;

			bool hr=false;
			int nKindnap = 0;
			OBJID idType = pAction->GetInt(ACTIONDATA_DATA);
			CItemTypeData* p = ItemType()->QueryItemType( idType );
			if(!p)
			{
				LogSave("ACTION_ITEM_ADD_COUNT %u",idType);
				return false;
			}
			UINT  nItemAmount=0;
			if(sscanf(szParam, "%u %u",&nItemAmount,&nKindnap) >= 1)
			{
				
// 				hr=pUser->AwardItemCount(idType,nItemAmount,true,0,nKindnap);
// 					pUser->AwardItemBySys(idItemType,true,false,true,bChgattribute/*false*//*true*/,nItemCountLimit,0,nJuan,nKindnap,nBossType,nPerPrice);
				CItem* pItem = pUser->AwardItem(idType,true,false,true,true,nItemAmount,true,true,false,nKindnap,0,0);
				if (pItem && idType == 500203)//首充礼包
				{
				//	CItem* pItem = pUser->GetItemByType(idType);
					pUser->IgotAnOrangeWeapen(idType, pItem);
					
				}
				hr=pItem!=NULL;
				if ((ItemType()->GetMonoly(idType)  & COSTLY_ITEM) != 0)//goto 贵重物品记录
					::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from task or system", pUser->GetID(), pUser->GetName(), nItemAmount, idType);
			}
			return hr;
		}
		break;
	case ACTION_SOUL_ADD_TYPE:
		{
			if(!pUser)
				return false;
			
			bool hr=false;
			OBJID idType = pAction->GetInt(ACTIONDATA_DATA);
			if(idType / 10000 != ITEMTYPE_WEAPONSOUL)
				return false;

			CItemTypeData* p = ItemType()->QueryItemType( idType );
			if(!p)
			{
				LogSave("ACTION_ITEM_ADD_COUNT %u",idType);
				return false;
			}
			UINT  nType=0;
			int nKindnap = 0;
			if(sscanf(szParam, "%u %u",&nType,&nKindnap) >= 1)
			{
				ST_WEAPONSOULDATA wpslInfo;
				CWeaponSoulData::GetInfoByDefault(wpslInfo,nType * 10);

				CItem* pSoulGet = pUser->CreateNewSoul(idType,wpslInfo,nKindnap);
				if(pSoulGet)
				{
					if ((ItemType()->GetMonoly(idType)  & COSTLY_ITEM) != 0)//goto 贵重物品记录
						::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) get %d %u from task or system", pUser->GetID(), pUser->GetName(), 1, idType);

					return true;
				}
			}
			return false;
		}
		break;
	case ACTION_ITEM_LOCK_CHECK:
		{
			return pUser->IsSetUseLock();
		}
		break;
	case ACTION_ITEM_ADD_PEIFANG:
		{
			return pUser->AddPeifang(pAction->GetInt(ACTIONDATA_DATA),true);
		}
		break;
	case ACTION_ITEM_DEL_PEIFANG:
		{
			return pUser->DelPeifang(pAction->GetInt(ACTIONDATA_DATA),true);
		}
		break;
	case ACTION_ITEM_HAS_PEIFANG:
		{
			return pUser->HasPeifang(pAction->GetInt(ACTIONDATA_DATA));
		}
		break;	
	case	ACTION_ITEM_CHECK:
		{
			CItemPtr pItem = NULL;
			if(pAction->GetInt(ACTIONDATA_DATA))
			{
				int nNum = __max(1,atoi(szParam));
				const int _maxSize = 120;
				int size = _maxSize;
				OBJID itemID[_maxSize];
				int userHaveAmount = pUser->GetItemByTypeMulti(pAction->GetInt(ACTIONDATA_DATA),size,itemID);
				if(userHaveAmount>=nNum)
					return true;
			}
		}
		break;
	case	ACTION_ITEM_LEAVESPACE:
		{
			int nPackType = 0;
			int nSpace = 0;
			
			OBJID idItem = NULL;
			if (sscanf(szParam, "%d %d", &nPackType, &nSpace) == 2)
			{
				return pUser->CheckPackageFree(nPackType,nSpace);
			}
		}
		break;
	default:
		LOGERROR("不支持的ACTION[%u]类型[%u]", pAction->GetKey(), pAction->GetInt(ACTIONDATA_TYPE));
	}
	
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CGameAction::ProcessActionSyn(CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept)
{
	CHECKF(pAction && pUser);
	
	int nActionType = pAction->GetInt(ACTIONDATA_TYPE);
	switch(nActionType)
	{	
	case	ACTION_SYN_DESTROY:
		{
			if(pUser->IsSetUseLock())
			{
				pUser->SendSysMsg( "人物锁已锁定，不能解散帮派！" );
				return false;
			}

			// 检查帮派
			OBJID idSyn = pUser->GetSynID();
			if(idSyn == ID_NONE)
				return false;
			
			// 检查权限
			if(pUser->GetSynRank() != RANK_LEADER)
			{
				pUser->SendSysMsg( g_objStrRes.GetStr( 11097 ) );
				//	pUser->SendSysMsg(STR_NOT_AUTHORIZED);
				return false;
			}
			
			OBJID idFealty = ID_NONE;
			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
			//[游途道标 2009.04.29]
			char szTemp[256];
			if (pSyn)
			{
				sprintf(szTemp, "『%s』解散了『%s』帮会，『%s』帮会暂时退出历史舞台。", pUser->GetName(), pSyn->GetStr(SYNDATA_NAME), pSyn->GetStr(SYNDATA_NAME));
			}

			return true;
		}
		break;
	case	ACTION_SYN_DONATE:	
		{
			if(!pszAccept)
				return false;
			
			char* pTemp;
			int nMoney = strtol(pszAccept, &pTemp, 10);
			if(nMoney <= 0 || pUser->GetMoney() < nMoney)
				return false;
			CHECKF(pUser->QuerySynAttr());
			pUser->QuerySynAttr()->DonateMoney(nMoney);
			return true;
		}
		break;
	case	ACTION_SYN_ATTR:			//军团属性
		{
			PARAMBUF	szField;
			PARAMBUF	szOpt;
			int 		nData;
			OBJID		idSyn=ID_NONE;
			if(sscanf(szParam, "%s %s %d %u", szField, szOpt, &nData, &idSyn) < 3)
			{
				LOGERROR("ACTION %u: 错误的参数数量", pAction->GetID());
				break;
			}
			
			if(idSyn == ID_NONE)
				idSyn = pUser->GetSynID();
			CHECKF(idSyn);
			
			CSyndicate* pSyn = SynManager()->QuerySyndicate(idSyn);
			CHECKF(pSyn);
			
			//军团成员属性相关
			CSynAttr* pSynatt = pUser->QuerySynAttr();
			CHECKF(pSynatt);
			
			
			if( 0 == stricmp( szField,"isbonustime" ) )
			{
				/*------------理论上这些都应该在数据库中已作了判断----------*/
				if( ID_NONE == pUser->GetSynID() )
					return false;
				if( pUser->GetSynRank() != RANK_LEADER )
					return false;
				
				/*----------------------------------------------------------*/
				SYSTEMTIME sysTime;
				::GetLocalTime( &sysTime );	//得到本地化系统时间
				const int Sunday = 0;			//周日
				if( Sunday == sysTime.wDayOfWeek && sysTime.wHour >= 19 )//周日晚7
					return true;		
				
			}
			else if (stricmp(szField, "money") == 0)//基金
			{
				if(strcmp(szOpt, "+=") == 0)
				{
//					pSyn->QueryModify()->AddData(SYNDATA_MONEY, nData, true);		// mast true
					return true;
				}
				else if(strcmp(szOpt, "<") == 0)
				{
					if(pSyn->GetInt(SYNDATA_MONEY) < nData)
						return true;
				}
				/*--------------------06.4.24王玉波添加--------------------*/
				else if( 0 == strcmp( szOpt,">=" ) )
				{
					if( pSyn->GetInt( SYNDATA_MONEY ) >= nData )
						return true;
				}
				/*----------------------end--------------------------------*/
			}
			else if (stricmp(szField, "membernum") ==0)//成员人数
			{
				
				if(strcmp(szOpt, "<") == 0)
				{
					if( pSyn->GetMemberAmount() <  nData )
						return true;					
				}
				/*--------------------06.4.24王玉波添加-----------*/
				else if( 0 == strcmp( szOpt,">=" ) )
				{
					if( pSyn->GetMemberAmount() >= nData )
						return true;
				}
				/*------------------------end---------------------*/
			}
			else if (stricmp(szField, "level") == 0)			//军团等级
			{
				if (strcmp(szOpt, "=") == 0)
				{
					IF_NOT (nData >= SYNRANK_ONE && nData <= SYNRANK_SIX)
						return false;
					
//					pSyn->QueryModify()->SetData(SYNDATA_RANK, nData, true);
					return true;
				}
				else if (strcmp(szOpt, "+=") == 0)
				{
					int nRank = pSyn->GetInt(SYNDATA_LEVEL);
					nRank += nData;
					nRank = __min(SYNRANK_SIX, __max(SYNRANK_ONE, nRank));
					
//					pSyn->QueryModify()->SetData(SYNDATA_RANK, nRank, true);		// mast true				
					return true;
				}
				else if (strcmp(szOpt, "<") == 0)
				{
					if (pSyn->GetInt(SYNDATA_LEVEL) < nData)
						return true;
				}
				/*--------------------06.4.24王玉波添加----------------*/
				else if( 0 == strcmp( szOpt,">" ) )
				{
					if( pSyn->GetInt( SYNDATA_LEVEL ) > nData )
						return true;
				}
				/*--------------------end------------------------------*/
				else if (strcmp(szOpt, "==") == 0)
				{
					if (pSyn->GetInt(SYNDATA_LEVEL) == nData)
						return true;
				}
			}
			//个人贡献度
			else if(stricmp(szField, "userproffer") == 0)
			{
				if (strcmp(szOpt, "=") == 0)
				{
					pSynatt->SetProffer(nData);//内有汇报世界线程
					return true;
				}
				else if(strcmp(szOpt, "==") == 0)
				{
					return (pSynatt->GetProffer()==nData);
				}
				else if(strcmp(szOpt, "<") == 0)
				{
					return (pSynatt->GetProffer()<nData);
				}
				else if(strcmp(szOpt, ">") == 0)
				{
					return (pSynatt->GetProffer()>nData);
				}
				else if(strcmp(szOpt, "+=") == 0)
				{
					pSynatt->AddProffer(nData);//内有汇报世界线程
					return true;
				}
				else if(strcmp(szOpt, "-=") == 0)
				{
					if(pSynatt->GetProffer()<nData)
						return false;

					pSynatt->AddProffer(-1 * nData);//内有汇报世界线程
					return true;
				}
				else
				{
					//opt error!
				}
			}
			else
			{
				//field error!
			}
			return false;
		}
		break;
	case ACTION_SYN_CHK_SYN:
		{
			OBJID		idSyn=ID_NONE;
			
			if(idSyn == ID_NONE)
				idSyn = pUser->GetSynID();
			if(idSyn == ID_NONE)
				return false;//没有军团
			else
				return true;//有军团
		}
		break;
	case ACTION_TUTOR_TEAM_ISSTU://750 goto 队友是不是徒弟
		{
			if (!pUser)
				return false;
			CTeam* pTeam = pUser->GetTeam();
			if (!pTeam || pTeam->GetLeader()!=pUser->GetID())
			{
				//pUser->SendSysMsg(_TXTATR_TOPSHOW, "请带着你要收的徒弟组队前来");
				return false;
			}
			if (pTeam->GetMemberAmount()>2)
			{
				//pUser->SendSysMsg(_TXTATR_TOPSHOW, "每次只能带一个徒弟来举行拜师仪式哦");
				return false;
			}
			// find target
			CUser* pTarget = NULL;
			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, pTeam->GetTeamMemberOnly(), IPP_OF(pTarget)))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方已不在你的附近");
				return false;
			}
			CTutorData* pStu = pUser->GetStudent(pTeam->GetTeamMemberOnly());
			if (!pStu)
				return false;
			int nState = pAction->GetInt(ACTIONDATA_DATA);
			if (pStu->GetState()!=nState)
				return false;
			return true;
		}
		break;
	case ACTION_TUTOR_STUREBEL://徒弟叛师
		{
			if (!pUser->GetTutorManager())
				return false;
			return pUser->GetTutorManager()->StuRebel();
		}
		break;
		// 师徒系统--出师 [陈剑飞10/26/2007]
	case ACTION_TUTOR_GRADUATE:
		{
			if (!pUser)
				return false;
			
			CTeam* pTeam = pUser->GetTeam();
			if (!pTeam || pTeam->GetLeader()!=pUser->GetID())
			{
				//pUser->SendSysMsg(_TXTATR_TOPSHOW, "请跟你的徒弟组队后再来");
				return false;
			}
			if (pTeam->GetMemberAmount()>2)
			{
				//pUser->SendSysMsg(_TXTATR_TOPSHOW, "每次只能带一个徒弟来举行出师仪式哦");
				return false;
			}
			
			// find target
			OBJID idTarget = pTeam->GetTeamMemberOnly();
			CUser* pStudent = NULL;
			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, idTarget, IPP_OF(pStudent)))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方已不在你的附近");
				return false;
			}
			
			return pUser->Graduate(idTarget);
		}
		break;
	case ACTION_TUTOR_ITEMWITHNAME:// goto 给玩家刻有自己名字的物品
		{
			int nItemType = pAction->GetInt(ACTIONDATA_DATA);
			if (nItemType==ID_NONE)
				return false;
			if (pUser->AwardItemByCreate(nItemType, true, false, false, 1, 0, false, true))
				return true;
			return false;
		}
		break;
	case ACTION_TUTOR_KICKOUTMSG:
		{
			if (!pUser->GetTutorManager())
				return false;
			
			pUser->GetTutorManager()->SendStuList();
		}
		break;
	case ACTION_TUTOR_REBELWIN:// 徒弟叛师前的确认框
		{
			if (!pUser->GetTutor())
				return false;

			CMsgSchoolMember msg;
			if (msg.Create(pUser->GetID(), MESSAGEBOARD_STUREBEL))
			{
				ST_MEMBERINFO info = {0};
				info.idMember = pUser->GetTutor()->GetTutorID();
				SafeCopy(info.szName, pUser->GetTutor()->GetTutorName(), _MAX_NAMESIZE);
				msg.Append(info);
				pUser->SendMsg(&msg);
			}
		}
		break;
	case ACTION_TUTOR_TUTORNAME_ITEM:// 生成刻有师父名字的物品
		{
			int nItemType = pAction->GetInt(ACTIONDATA_DATA);
			if (nItemType==ID_NONE)
				return false;
			CTutorData* pData = pUser->GetTutor();
			if (!pData)
				return false;

			if(!pUser->AwardItemByCreate(nItemType, true, false, false, 1, 0, false, true, pData->GetTutorName()))
				return false;
			return true;
		}
		break;
	case ACTION_TUTOR_STUNAME_ITEM:// 生成刻有队伍中徒弟名字的物品
		{
			int nItemType = pAction->GetInt(ACTIONDATA_DATA);
			if (nItemType==ID_NONE)
				return false;
			CTeam* pTeam = pUser->GetTeam();
			if (!pTeam)
				return false;
			if (pTeam->GetLeader()!=pUser->GetID())
				return false;
			
			int nTeamAmount = pTeam->GetMemberAmount();
			if (nTeamAmount<2)
				return false;
			CTutorData* pData = NULL;
			for(int i = 0; i < nTeamAmount; i++)
			{
				OBJID idMember = pTeam->GetMemberByIndex(i);
				CUser* pTempUser = NULL;
				pUser->FindAroundObj( OBJ_USER,idMember,( void** )&pTempUser );
				if(pTempUser && pTempUser->IsAlive() && (pTempUser->GetID() != pUser->GetID()))
				{
					pData = pUser->GetStudent(pTempUser->GetID());
					if (pData && pData->GetState()==TUTORSTATE_NORMAL)//是徒弟
					{
						if(!pUser->AwardItemByCreate(nItemType, true, false, false, 1, 0, false, true, pTempUser->GetName()))
							::LogSave("GameAction case ACTION_TUTOR_STUNAME_ITEM error!userid=%u,itemtype=%u",pTempUser->GetID(),nItemType);

					}
				}
 			}
			return true;
		}
		break;
	case ACTION_TUTOR_ADDSTUBUF:// 给队伍中的徒弟加Buf
		{
			int nBufID = 0;
			int nBufPower = 0;
			int nBufSec = 0;
			int nTimes = 0;
			int nMapID = 0;
			int nIncludeSelf = pAction->GetInt(ACTIONDATA_DATA);

			if (5 == sscanf(szParam, "%d %d %d %d %d", &nBufID, &nBufPower, &nBufSec, &nTimes, &nMapID))
			{
				if (nBufID==0 || nBufSec==0)
					return false;
				CTeam* pTeam = pUser->GetTeam();
				if (!pTeam)
					return false;
				if (pTeam->GetLeader()!=pUser->GetID())
					return false;
				
				int nTeamAmount = pTeam->GetMemberAmount();
				if (nTeamAmount<2)
					return false;

				bool bHasStu = false;
				CTutorData* pData = NULL;
				for(int i = 0; i < nTeamAmount; i++)
				{
					OBJID idMember = pTeam->GetMemberByIndex(i);
					if (idMember==pUser->GetID())
						continue;
					CUser* pTempUser = NULL;
					pUser->FindAroundObj( OBJ_USER,idMember,( void** )&pTempUser );
					if(pTempUser && pTempUser->IsAlive())
					{
						pData = pUser->GetStudent(pTempUser->GetID());
						if (pData && pData->GetState()==TUTORSTATE_NORMAL)//是徒弟
						{
							pTempUser->AttachStatus(pTempUser->QueryRole(), nBufID, nBufPower, nBufSec, nTimes, 0, nMapID);
							bHasStu = true;
						}
					}
				}
				if (bHasStu && nIncludeSelf==0)
				{
					pUser->AttachStatus(pUser->QueryRole(), nBufID, nBufPower, nBufSec, nTimes, 0, nMapID);
					return true;
				}
				else
					return false;
			}
			return false;
		}
		break;
	case ACTION_TUTOR_STUAROUND:// 队伍中徒弟是否都在附近
		{
			CTeam* pTeam = pUser->GetTeam();
			if (!pTeam)
				return false;

			return pTeam->CheckTutorActive(pUser);
		}
		break;
	case ACTION_TUTOR_ITEMWITHPOS:// 生成带坐标的物品 坐标记录在itemAddition1,itemAddition2,itemAddition3里
		{
			int nStep = 0;
			DEBUG_TRY
			OBJID nItemType = pAction->GetInt(ACTIONDATA_DATA);
			CHECKF(nItemType);

			nStep = 1;
			int nMapID = 0, nPosX = 0, nPosY = 0;
			if (3 == sscanf(szParam, "%d %d %d", &nMapID, &nPosX, &nPosY))
			{
				nStep = 2;
				CGameMap* pNewMap = MapManager()->GetGameMap(nMapID);
				if(!pNewMap || !pNewMap->IsValidPoint(nPosX, nPosY))
				{
					::LogSave("GameAction:ACTION_TUTOR_ITEMWITHPOS Invalid Point mapid=%d,posx=%d,posy=%d", nMapID, nPosX, nPosY);
					return false;
 				}

				nStep = 3;
				ItemInfoStruct info;
				memset(&info,0,sizeof(ItemInfoStruct));
				
				nStep = 4;
				CHECKF(ItemType()->GetInfo(nItemType, &info));

				nStep = 5;
				info.idOwner	= pUser->GetID();
				info.idPlayer	= pUser->GetID();
				info.AdditionAddID[0] = nMapID;
				info.AdditionAddID[1] = nPosX;
				info.AdditionAddID[2] = nPosY;

				nStep = 6;
				SafeCopy(info.szMakerName, pUser->GetName(), _MAX_NAMESIZE);
				
				nStep = 7;
				CItem* pItem = pUser->AwardItem(&info, true, false, false, false, true);
				return pItem != NULL;
			}
			DEBUG_CATCH2("ACTION_TUTOR_ITEMWITHPOS nStep = %d",nStep)
			return false;
		}
		break;
	case ACTION_TUTOR_CREATEMONSTER:// 在指定的地点刷怪
		{
			OBJID idType = pAction->GetInt(ACTIONDATA_DATA);
			if (idType==ID_NONE)
				return false;
			OBJID idMap = ID_NONE;
			int nPosX = 0, nPosY = 0;
			if (3 != sscanf(szParam, "%d %d %d", &idMap, &nPosX, &nPosY))
			{
				idMap = pUser->GetMapID();
				nPosX = pUser->GetPosX();
				nPosY = pUser->GetPosY();
			}
			ST_CREATENEWNPC	info;
			memset(&info, 0, sizeof(ST_CREATENEWNPC));
	
			if (idMap == 0 && nPosX == 0 && nPosY == 0)
			{
				idMap = pUser->GetMapID();
				nPosX = pUser->GetPosX();
				nPosY = pUser->GetPosY();
			}
			if (idMap==1048)
			{
				idMap=pUser->GetSynMapID();
			}
			info.idMap			= idMap;//pNpc->GetMapID();
			info.idData			= 0;//1745;//20080131修罗:此id保证在sk_generator中存在
			info.idOwner		= pUser->GetID();//jay 为了得到人物id，特殊地图刷怪需要//pNpc->GetInt(NPCDATA_OWNERID);		// 总帮
			info.usPosX			= nPosX;
			info.usPosY			= nPosY;

			info.usType			= idType;
			info.ucOwnerType	= 0;//pNpc->GetInt(NPCDATA_OWNERTYPE);
			info.nData			= 0;//nData;
			
			if(idMap>=SYN_MAP_DOWN && idMap < SYN_MAP_UP)
			{
				CMonsterPtr pMonster = NpcManager()->CreateNewSynBoss(&info);
				if (pMonster!=NULL)
				{
					CSynPtr pSyn=SynManager()->QuerySyndicate(pUser->GetSynID());
					if (pSyn)
					{
						pSyn->AddSynMonster(pMonster);
						return true;
					}
						return false;
				}
				return false;
			}
			else
			{
				if (g_SpecilLine==0)
				{
					NpcManager()->CreateSynPet(&info);
					return true;
				}
				else
				{
					if(pUser->IsGM())
						pUser->SendSysMsg("专线普通地图不允许刷怪");
				}
			}
			
		}
		break;
	case ACTION_TUTOR_GRADUATEWIN:// 告诉徒弟确认出师
		{
			CTeam* pTeam = pUser->GetTeam();
			if (!pTeam || pTeam->GetLeader()!=pUser->GetID())
			{
				return false;
			}
			
			// find target
			OBJID idTarget = pTeam->GetTeamMemberOnly();
			CUser* pStudent = NULL;
			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, idTarget, IPP_OF(pStudent)))
			{
				return false;
			}
			
			pUser->SetApply(CUser::APPLY_GRADUATE, idTarget);
			CMsgSchoolMember msg;
			if (msg.Create(pUser->GetID(), MESSAGEBOARD_GRADUATEAPPLY))
			{
				ST_MEMBERINFO info = {0};
				SafeCopy(info.szName, pUser->GetName(), _MAX_NAMESIZE);
				msg.Append(info);
				pStudent->SendMsg(&msg);
			}
			return true;
		}
		break;
	case ACTION_TUTOR_TEAMADD:// 组队拜师,给徒弟弹确认框
		{
			CTeam* pTeam = pUser->GetTeam();
			if (!pTeam || pTeam->GetLeader()!=pUser->GetID())
			{
				return false;
			}
			
			// find target
			OBJID idTarget = pTeam->GetTeamMemberOnly();
			CUser* pStudent = NULL;
			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, idTarget, IPP_OF(pStudent)))
			{
				return false;
			}
			if (pUser->GetLev()-pStudent->GetLev() < 10 )//必须比对方的等级要低10
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "只能收等级比你低10级的玩家为徒弟");
				return false;
			}
			if (pUser->IsStudentFull())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "你的徒弟名额已满");
				return false;
			}
			if(pStudent->GetTutor())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方已经有师父了");
				return false;
			}
			CTutorData* pData = pUser->GetStudent(idTarget);
			if(pData && pData->GetState()!=TUTORSTATE_GRADUATED)
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方已经在你的申请列表中");
				return false;
			}
			#if (!defined(_DEBUG) && !defined(_ASHRELEASE))
			if (pStudent->GetIPAddress()==pUser->GetIPAddress())
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "该玩家登陆IP与你相同，无法结成师徒关系");
				return false;
			}
			#endif
			pUser->SetApply(CUser::APPLY_TEACHERAPPLY, idTarget);
			CMsgSchoolMember msg;
			if (msg.Create(pUser->GetID(), MESSAGEBOARD_TEAMADD))
			{
				ST_MEMBERINFO info = {0};
				SafeCopy(info.szName, pUser->GetName(), _MAX_NAMESIZE);
				msg.Append(info);
				pStudent->SendMsg(&msg);
			}
			return true;
		}
		break;
	case ACTION_TUTOR_TEAM_STULEV://751 goto 判断队伍中徒弟等级
		{
			CTeam* pTeam = pUser->GetTeam();
			if (!pTeam || pTeam->GetLeader()!=pUser->GetID())
			{
				return false;
			}
			if (pTeam->GetMemberAmount()>2)//只限两人队伍
			{
				return false;
			}
			
			// find target
			OBJID idTarget = pTeam->GetTeamMemberOnly();
			CUser* pStudent = NULL;
			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, idTarget, IPP_OF(pStudent)))
			{
				pUser->SendSysMsg(_TXTATR_TOPSHOW, "对方已不在你的附近");
				return false;
			}
			PARAMBUF	szOpt;
			int	nLev;
			if(sscanf(szParam, "%s %d",szOpt,&nLev) < 1)//两个参数，操作符和数量
			{
				LOGERROR("ACTION %u: 错误的参数数量", pAction->GetID());
				break;
			}
			if(strcmp(szOpt, ">") == 0)
				return pStudent->GetLev() > nLev;
			else if(strcmp(szOpt, "<") == 0)
				return pStudent->GetLev() < nLev;
			else if(strcmp(szOpt, ">=") == 0)
				return pStudent->GetLev() >= nLev;
			else if(strcmp(szOpt, "<=") == 0)
				return pStudent->GetLev() <= nLev;
			else if(strcmp(szOpt, "==") == 0)
				return pStudent->GetLev() == nLev;
			else
			{
				LOGERROR("ACTION %u: 错误的参数szOpt", pAction->GetID());
				break;
			}
		}
		break;
	case ACTION_TUTOR_DIVORCE:// goto 协议解除师徒关系
		{
			CTeam* pTeam = pUser->GetTeam();
			if (!pTeam || pTeam->GetLeader()!=pUser->GetID())
			{
				return false;
			}
			
			// find target
			OBJID idTarget = pTeam->GetTeamMemberOnly();
			CUser* pStudent = NULL;
			if(!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, idTarget, IPP_OF(pStudent)))
			{
				return false;
			}
			
			pUser->SetApply(CUser::APPLY_LEAVETEACHER, idTarget);
			CMsgSchoolMember msg;
			if (msg.Create(pUser->GetID(), MESSAGEBOARD_DIVORCE))
			{
				ST_MEMBERINFO info = {0};
				SafeCopy(info.szName, pUser->GetName(), _MAX_NAMESIZE);
				msg.Append(info);
				pStudent->SendMsg(&msg);
			}
			return true;
		}
		break;
	case ACTION_TUTOR_TEAM_HASTUTOR:// goto 师父踢出徒弟
		{
			if (!pUser->GetTutorManager())
				return false;
			OBJID idStu = pAction->GetInt(ACTIONDATA_DATA);
			if (idStu==ID_NONE)
				return false;
			return pUser->GetTutorManager()->KickOutStu(idStu);
		}
		break;
	case ACTION_TUTOR_HASTUTOR://753  goto 判断是否有师父
		{
			if (!pUser)
				break;
			if (pUser->GetTutor())
				return true;
			else
				return false;
		}
		break;
	case ACTION_TUTOR_HASSTUDENT:// goto 判断是否有徒弟
		{
			return pUser->GetStudentAmount(false)>0;
		}
		break;
	case ACTION_TUTOR_TEACHER://徒弟判断队长是不是师傅
		{
			/*if(pUser->GetStudentAmount())
				return true;
			return false;*/
			int nType = pAction->GetInt(ACTIONDATA_DATA);//是否包括已出师的
			CTutorData* pTutor = pUser->GetTutor();
			if (!pTutor)
				return false;
			CTeam* pTeam = pUser->GetTeam();
			if (!pTeam)
				break;
			CUser* pTarget = UserManager()->GetUser(pTeam->GetLeader());
			if (!pTarget)
				break;
			if (pTarget->GetID() == pTutor->GetTutorID())
			{
				if (pTutor->IsGraduated())
				{
					if (nType==1)
						return true;
					else//已出师的不算
						return false;
				}
				else
					return true;
			}
			return false;
		}
		break;
	case ACTION_TUTOR_STUDENT://师父判断队友是不是徒弟
		{
			/*if(pUser->GetTutor())
				return true;
			return false;*/
			int nType = pAction->GetInt(ACTIONDATA_DATA);//是否包括已出师的
			CTeam* pTeam = pUser->GetTeam();
			if (!pTeam)
				break;
			if (pTeam->GetMemberAmount()>2)
				break;
			OBJID idStudent = pTeam->GetTeamMemberOnly();
			CTutorData* pTutor = pUser->GetStudent(idStudent);
			if (!pTutor)
				return false;
			if (pTutor->IsGraduated())
			{
				if (nType==1)
					return true;
				else
					return false;
			}
			else if(pTutor->GetState()==TUTORSTATE_NORMAL)
				return true;
			return false;
		}
		break;	
	default:
		LOGERROR("不支持的ACTION[%u]类型[%u]", pAction->GetKey(), pAction->GetInt(ACTIONDATA_TYPE));
	}
	
	return false;
}

//////////////////////////////////////////////////////////////////////
bool CGameAction::ProcessActionUser(CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pszAccept)
{
	if( !pAction || !pUser )
	{
		char str[] = "CGameAction::ProcessActionUser";
		::LogSave(str);
		return false;
	}
	switch(pAction->GetInt(ACTIONDATA_TYPE))
	{
	case ACTION_USER_CHECKINSFINISHTIME:
		{
			int nInstanceType1;
			int nInstanceType2;
			char szOptIns[128];
			int nData = pAction->GetInt(ACTIONDATA_DATA);
			if (3 == sscanf(szParam, "%u %s %u",&nInstanceType1, szOptIns,&nInstanceType2))
			{
				if(0 == stricmp(szOptIns,">"))
				{
					if(0 == nData)
						return pUser->GetInstanceFinishTime(nInstanceType1) > pUser->GetInstanceFinishTime(nInstanceType2);
					else if(1 == nData)
						return pUser->GetInstanceTime(nInstanceType1) > pUser->GetInstanceTime(nInstanceType2);
				}
			}
		}
		break;
	case ACTION_USER_TASK_MANAGER:	//玩家任务管理
		{			
			int nData = pAction->GetInt(ACTIONDATA_DATA);
			DEBUG_TRY
			char szItem[128] = "";
			char szOpt[128] = "";
			int paramData = 0;
			
			CTaskDetail * pTask = pUser->GetTaskDetail();
			IF_NOT(pTask)
				return false;
			
			if (3 == sscanf(szParam, "%s %s %d",szItem, szOpt,&paramData))
			{
				if(0 == stricmp(szItem,"task"))
				{
					if(0 == stricmp(szOpt,"new"))
					{
						int taskId = nData;
						int nStep = paramData;
						if (pTask->QueryData(pUser->GetID(), taskId))
							return false;
					
						CTaskSetRandData* pSetData = TaskSetRand()->GetDataStruct(taskId, nStep);
						if(!pSetData)
							return false;

						ST_TASKDETAILDATA stData;
						stData.idTask = taskId;
						stData.idUser = pUser->GetID();
						stData.nTaskPhase = nStep;
						stData.dwBegainTime = 0;
						stData.data1 = 0;
						stData.data2 = 0;
						stData.data3 = 0;
						stData.data4 = 0;
						stData.data5 = 0;
						stData.data6 = 0;
						stData.isRec = 0;
						stData.randStep = 1;
						stData.spendEneType = 0;
						
						if(pTask->CreateNewTaskDetailData(&stData))
						{
// 							TaskDetailInfoForMsg taskData;
// 							taskData.taskID = stData.idTask;
// 							taskData.taskPhase = stData.nTaskPhase;
// 							taskData.lifeTime = 0;
// 							taskData.data1 = stData.data1;
// 							taskData.data2 = stData.data2;
// 							taskData.data3 = stData.data3;
// 							taskData.data4 = stData.data4;
// 							taskData.data5 = stData.data5;
// 							taskData.data6 = stData.data6;
// 							taskData.isRec = stData.isRec;
// 
// 							CMsgTaskDetail msg;
// 							if(msg.Create(_TASKHINT_UPDATE,&taskData,1))
// 								pUser->SendMsg(&msg);
							return true;
						}
						return false;
					}
					else if(0 == stricmp(szOpt,"accept"))
					{
						int taskId = nData;
						int nStep = paramData;
						CTaskDetailData* pTaskDetail = pTask->QueryData(pUser->GetID(), taskId);
						if (!pTaskDetail)
							return false;
						if(pTaskDetail->GetInt(TASKDETAILDATA_TASKPHASE) != nStep)
							return false;
						if(pTaskDetail->GetInt(TASKDETAILDATA_ISREC) == 1)
							return false;
						CTaskSetRandData* pSetData = TaskSetRand()->GetDataStruct(taskId, nStep);
						if(!pSetData)
							return false;
						
						TaskDetailInfoForMsg taskData;

						if(pSetData->TaskData.setTime == 0)
							taskData.lifeTime = 99999;
						else
							taskData.lifeTime = pSetData->TaskData.setTime;

						
						taskData.taskID = taskId;
						taskData.taskPhase = nStep;						
						taskData.data1 = 0;
						if(pSetData->TaskData.monster1 > 100000)
						{
							if(pSetData->TaskData.monster1 == 100001)
							{
								taskData.data1 = pUser->GetFriendAllAmount();
							}
							else if(pSetData->TaskData.monster1 == 100002)
							{
								taskData.data1 = pUser->GetLev();
							}
							else if(pSetData->TaskData.monster1 > 200000 && pSetData->TaskData.monster1 < 300000)
							{
								OBJID idSkill = pSetData->TaskData.monster1 - 200000;
								if(pUser->QueryMagic(idSkill))
									taskData.data1 = pUser->QueryMagic(idSkill)->GetMagicLevel();
							}
						}
						taskData.data2 = 0;
						if(pSetData->TaskData.monster2 > 100000)
						{
							if(pSetData->TaskData.monster2 == 100001)
							{
								taskData.data2 = pUser->GetFriendAllAmount();
							}
							else if(pSetData->TaskData.monster2 == 100002)
							{
								taskData.data2 = pUser->GetLev();
							}
							else if(pSetData->TaskData.monster2 > 200000 && pSetData->TaskData.monster2 < 300000)
							{
								OBJID idSkill = pSetData->TaskData.monster2 - 200000;
								if(pUser->QueryMagic(idSkill))
									taskData.data2 = pUser->QueryMagic(idSkill)->GetMagicLevel();
							}
						}
						taskData.data3 = 0;
						if(pSetData->TaskData.monster3 > 100000)
						{
							if(pSetData->TaskData.monster3 == 100001)
							{
								taskData.data3 = pUser->GetFriendAllAmount();
							}
							else if(pSetData->TaskData.monster3 == 100002)
							{
								taskData.data3 = pUser->GetLev();
							}
							else if(pSetData->TaskData.monster3 > 200000 && pSetData->TaskData.monster3 < 300000)
							{
								OBJID idSkill = pSetData->TaskData.monster3 - 200000;
								if(pUser->QueryMagic(idSkill))
									taskData.data3 = pUser->QueryMagic(idSkill)->GetMagicLevel();
							}
						}
						int nMaxAmount = 0;
						taskData.data4 = 0;
						if(pSetData->TaskData.item1)
						{
							if(pSetData->TaskData.item1 == _SPCITEM_MONEYFORSHOP)
								taskData.data4 = 0;
							else if(pSetData->TaskData.item1 > 99999)
								{
									if(pSetData->TaskData.item1 < SPECIALITEMBASE)
										taskData.data4 = pUser->GetItemByTypeMulti(pSetData->TaskData.item1,nMaxAmount,NULL);	
								}
							else
								taskData.data4 = pUser->GetEudemonAmountByType(pSetData->TaskData.item1);
						}
						else
							taskData.data4 = 0;

						nMaxAmount = 0;
						taskData.data5 = 0;
						if(pSetData->TaskData.item2)
						{
							if(pSetData->TaskData.item2 == _SPCITEM_MONEYFORSHOP)
								taskData.data5 = 0;
							else if(pSetData->TaskData.item2 > 99999)
							{
								if(pSetData->TaskData.item2 < SPECIALITEMBASE)
									taskData.data5 = pUser->GetItemByTypeMulti(pSetData->TaskData.item2,nMaxAmount,NULL);	
							}
							else
								taskData.data5 = pUser->GetEudemonAmountByType(pSetData->TaskData.item2);
						}
						else
							taskData.data5 = 0;

						nMaxAmount = 0;
						taskData.data6 = 0;
						if(pSetData->TaskData.item3)
						{
							if(pSetData->TaskData.item3 == _SPCITEM_MONEYFORSHOP)
								taskData.data6 = 0;
							else if(pSetData->TaskData.item3 > 99999)
							{
								if(pSetData->TaskData.item3 < SPECIALITEMBASE)
									taskData.data6 = pUser->GetItemByTypeMulti(pSetData->TaskData.item3,nMaxAmount,NULL);	
							}
							else
								taskData.data6 = pUser->GetEudemonAmountByType(pSetData->TaskData.item3);
						}
						else
							taskData.data6 = 0;

						taskData.isRec = 1;

						pTaskDetail->SetInt(TASKDETAILDATA_TASKBEGINTIME,TimeGet(TIME_SECOND));
						pTaskDetail->SetInt(TASKDETAILDATA_DATA1,taskData.data1);
						pTaskDetail->SetInt(TASKDETAILDATA_DATA2,taskData.data2);
						pTaskDetail->SetInt(TASKDETAILDATA_DATA3,taskData.data3);
						pTaskDetail->SetInt(TASKDETAILDATA_DATA4,taskData.data4);
						pTaskDetail->SetInt(TASKDETAILDATA_DATA5,taskData.data5);
						pTaskDetail->SetInt(TASKDETAILDATA_DATA6,taskData.data6);
						pTaskDetail->SetInt(TASKDETAILDATA_ISREC,1);

						taskData.randStep = pTaskDetail->GetInt(TASKDETAILDATA_RANDSTEP);
						
						CMsgTaskDetail msg;
						if(msg.Create(_TASKHINT_UPDATE,&taskData,1))
								pUser->SendMsg(&msg);
						return true;
					}
					else if(0 == stricmp(szOpt,"state"))
					{
						int taskId = nData;
						CTaskDetailData* pTaskDetail = pTask->QueryData(pUser->GetID(), taskId);
						if (!pTaskDetail)
							return false;

						CTaskSetRandData* pSetData = TaskSetRand()->GetDataStruct(taskId, pTaskDetail->GetInt(TASKDETAILDATA_TASKPHASE));
						if(!pSetData)
							return false;

						if(paramData == 3)
						{
							if((pTaskDetail->GetInt(TASKDETAILDATA_ISREC) == 2) || (pTaskDetail->GetInt(TASKDETAILDATA_ISREC) == 0))
								return false;								
							if((pSetData->TaskData.monsterNum1 != 0)  && (pTaskDetail->GetInt(TASKDETAILDATA_DATA1) < pSetData->TaskData.monsterNum1))
								return false;
							if((pSetData->TaskData.monsterNum2 != 0)  && (pTaskDetail->GetInt(TASKDETAILDATA_DATA2) < pSetData->TaskData.monsterNum2))
								return false;
							if((pSetData->TaskData.monsterNum3 != 0)  && (pTaskDetail->GetInt(TASKDETAILDATA_DATA3) < pSetData->TaskData.monsterNum3))
								return false;
							if((pSetData->TaskData.itemNum1 != 0 ) && (pTaskDetail->GetInt(TASKDETAILDATA_DATA4) < pSetData->TaskData.itemNum1))
								return false;
							if((pSetData->TaskData.itemNum2 != 0)  && (pTaskDetail->GetInt(TASKDETAILDATA_DATA5) < pSetData->TaskData.itemNum2))
								return false;
							if((pSetData->TaskData.itemNum3 != 0)  && (pTaskDetail->GetInt(TASKDETAILDATA_DATA6) < pSetData->TaskData.itemNum3))
								return false;

							return true;
						}
						else
							return pTaskDetail->GetInt(TASKDETAILDATA_ISREC) == paramData;
					}
					else if(0 == stricmp(szOpt,"endnogive"))
					{
						int taskId = nData;
						int nStep = paramData;
						CTaskDetailData* pTaskDetail = pTask->QueryData(pUser->GetID(), taskId);
						if (!pTaskDetail)
							return false;
						if(pTaskDetail->GetInt(TASKDETAILDATA_TASKPHASE) != nStep)
							return false;
						if(pTaskDetail->GetInt(TASKDETAILDATA_ISREC) != 1)
							return false;
						
						pTaskDetail->SetInt(TASKDETAILDATA_ISREC,2);
						
						TaskDetailInfoForMsg taskData;
						taskData.taskID = taskId;
						taskData.taskPhase = nStep;	
						taskData.lifeTime = 0;
						taskData.data1 = 0;
						taskData.data2 = 0;
						taskData.data3 = 0;
						taskData.data4 = 0;
						taskData.data5 = 0;
						taskData.data6 = 0;
						taskData.isRec = 2;
						
						taskData.randStep = pTaskDetail->GetInt(TASKDETAILDATA_RANDSTEP);
						
						CMsgTaskDetail msg;
						if(msg.Create(_TASKHINT_UPDATE,&taskData,1))
							pUser->SendMsg(&msg);

						return true;
					}
					else if(0 == stricmp(szOpt,"end"))
					{
						int taskId = nData;
						int nStep = paramData;
						CTaskDetailData* pTaskDetail = pTask->QueryData(pUser->GetID(), taskId);
						if (!pTaskDetail)
							return false;
						if(pTaskDetail->GetInt(TASKDETAILDATA_TASKPHASE) != nStep)
							return false;
						if(pTaskDetail->GetInt(TASKDETAILDATA_ISREC) != 1)
							return false;

						CTaskSetRandData* pSetData = TaskSetRand()->GetDataStruct(taskId, pTaskDetail->GetInt(TASKDETAILDATA_TASKPHASE));
						if(!pSetData)
							return false;
						
						//宠物任务扣去宠物
						//1.找到需不要要扣
						OBJID idEudemonType = ID_NONE;
						OBJID idEudemon = ID_NONE;
						if(pSetData->TaskData.item1 < 100000)
							idEudemonType = pSetData->TaskData.item1;
						else if(pSetData->TaskData.item2 < 100000)
							idEudemonType = pSetData->TaskData.item2;
						else if(pSetData->TaskData.item3 < 100000)
							idEudemonType = pSetData->TaskData.item3;

						EudemonData eData;
						if(idEudemonType != ID_NONE)
						{
							idEudemon = pUser->GetLastTaskEudemon();
							if(idEudemon != ID_NONE)
							{
								pUser->LoadEudemon(idEudemon,eData,1);
								if(eData.typeId != idEudemonType)
									return false;

								pUser->SetLastTaskEudemon(ID_NONE);
							}
							else
								return false;

							if(pUser->IsLockedEudemon(idEudemon))
							{
								pUser->SendSysMsg("当前宠物不能提交");
								return false;
							}

							pUser->EudemonGiveup(idEudemon, true);
						}

						pTaskDetail->SetInt(TASKDETAILDATA_ISREC,2);

						TaskDetailInfoForMsg taskData;
						taskData.taskID = taskId;
						taskData.taskPhase = nStep;	
						taskData.lifeTime = 0;
						taskData.data1 = 0;
						taskData.data2 = 0;
						taskData.data3 = 0;
						taskData.data4 = 0;
						taskData.data5 = 0;
						taskData.data6 = 0;
						taskData.isRec = 2;
						
						taskData.randStep = pTaskDetail->GetInt(TASKDETAILDATA_RANDSTEP);

						CMsgTaskDetail msg;
						if(msg.Create(_TASKHINT_UPDATE,&taskData,1))
							pUser->SendMsg(&msg);

						if(nData > 10000) //环任务奖励写死
						{
							int nMaxDouble = 10;
							if(pUser->GetProfession(2) != 0)
								nMaxDouble = 20;

							int nChangeExp = 1;
							int nChangeMoney = 0;
							if(taskData.randStep <= nMaxDouble)
							{
								nChangeExp = 10;
								nChangeMoney = 5;
							}

							int nRand = taskData.randStep % 10;
							if(nRand == 0)
								nRand = 10;

							DWORD dwRandExp = (pSetData->TaskData.uRandExp + nRand * 5 * pUser->GetLev()) * nChangeExp;
							DWORD dwRandMoney = (pSetData->TaskData.uRandMoney + nRand * 5 * pUser->GetLev()) * nChangeMoney;

							DWORD  nExpUaer = pUser->AwardBattleExp( dwRandExp,false,false );
							if(nExpUaer > 0)
							{
								pUser->SendSysMsg("你获得了%u经验",dwRandExp);
							}
							if (nData == 20001)//帮派环任务
							{
							}
							else
							{
								if(dwRandMoney > 0)
								{
									pUser->GainMoney(dwRandMoney,true,2);
								}

								int nOldJobCon = pUser->GetJobCon();
								int nAdd = 1;
								if(nRand > 4 && nRand <= 7)
									nAdd = 2;
								else if(nRand > 7 && nRand <= 9)
									nAdd = 3;
								else if(nRand > 9)
									nAdd = 4;

								pUser->SetJobCon(0,max(0,nOldJobCon + nAdd));
								if(pUser->GetCurJob() == 1)
									pUser->SendSysMsg("您获得了%d点主职业门派贡献度",nAdd);
								else
									pUser->SendSysMsg("您获得了%d点副职业门派贡献度",nAdd);
							}
						}
						if(nData == 29)
						{
							DWORD dwRandMoney = pUser->GetRandShopMoney();
						}
						return true;
					}
					else if(0 == stricmp(szOpt,"delete"))
					{
						int taskId = nData;
						int nStep = paramData;
						CTaskDetailData* pTaskDetail = pTask->QueryData(pUser->GetID(), taskId);
						if (!pTaskDetail)
						{
							return false;
						}

						TaskDetailInfoForMsg taskData;
						taskData.taskID = taskId;
						taskData.taskPhase = nStep;	
						CMsgTaskDetail msg;
						if(msg.Create(_TASKHINT_DELETE,&taskData,1))
							pUser->SendMsg(&msg);

						pTask->DeleteData(pUser->GetID(), taskId);
						return true;
					}
					else if(0 == stricmp(szOpt,"has"))
					{
						int taskId = nData;
						if (pTask->QueryData(pUser->GetID(), taskId))
							return true;
						return false;
					}
				}
				else if(0 == stricmp(szItem,"randstep"))
				{
					if(nData < 10001)
						return false;
					
					int taskId = nData;
					CTaskDetailData* pTaskDetail = pTask->QueryData(pUser->GetID(), taskId);
					if (!pTaskDetail)
						return false;
					
					int oldStep = pTaskDetail->GetInt(TASKDETAILDATA_RANDSTEP);
					
					if(0 == stricmp(szOpt,">="))
						return pTaskDetail->GetInt(TASKDETAILDATA_RANDSTEP) >= paramData;
					else if(0 == stricmp(szOpt,"<"))
						return pTaskDetail->GetInt(TASKDETAILDATA_RANDSTEP) < paramData;
					else if(0 == stricmp(szOpt,"=="))
						return pTaskDetail->GetInt(TASKDETAILDATA_RANDSTEP) == paramData;
					else if(0 == stricmp(szOpt,"="))
					{
						pTaskDetail->SetInt(TASKDETAILDATA_RANDSTEP,paramData);
						return true;
					}
					else if(0 == stricmp(szOpt,"+="))
					{
						pTaskDetail->SetInt(TASKDETAILDATA_RANDSTEP,oldStep + paramData);
						return true;
					}
					else if(0 == stricmp(szOpt,"%"))
					{
						pTaskDetail->SetInt(TASKDETAILDATA_RANDSTEP,oldStep % paramData);
						return true;
					}
				}
				else if(0 == stricmp(szItem,"step"))
				{
					int taskId = nData;
					CTaskDetailData* pTaskDetail = pTask->QueryData(pUser->GetID(), taskId);
					if (!pTaskDetail)
						return false;

					if(0 == stricmp(szOpt,">="))
						return pTaskDetail->GetInt(TASKDETAILDATA_TASKPHASE) >= paramData;
					else if(0 == stricmp(szOpt,"<"))
						return pTaskDetail->GetInt(TASKDETAILDATA_TASKPHASE) < paramData;
					else if(0 == stricmp(szOpt,"=="))
						return pTaskDetail->GetInt(TASKDETAILDATA_TASKPHASE) == paramData;
					else if(0 == stricmp(szOpt,"+="))
					{
						if(pTaskDetail->GetInt(TASKDETAILDATA_ISREC) != 2)
							return false;
						paramData += pTaskDetail->GetInt(TASKDETAILDATA_TASKPHASE);

						CTaskSetRandData* pSetData = TaskSetRand()->GetDataStruct(taskId, paramData);
						if(!pSetData)
							return false;
						pTaskDetail->SetInt(TASKDETAILDATA_TASKPHASE,paramData);
						pTaskDetail->SetInt(TASKDETAILDATA_TASKBEGINTIME,0);
						pTaskDetail->SetInt(TASKDETAILDATA_DATA1,0);
						pTaskDetail->SetInt(TASKDETAILDATA_DATA2,0);
						pTaskDetail->SetInt(TASKDETAILDATA_DATA3,0);
						pTaskDetail->SetInt(TASKDETAILDATA_DATA4,0);
						pTaskDetail->SetInt(TASKDETAILDATA_DATA5,0);
						pTaskDetail->SetInt(TASKDETAILDATA_DATA6,0);
						pTaskDetail->SetInt(TASKDETAILDATA_ISREC,0);
						
						TaskDetailInfoForMsg taskData;
						taskData.taskID = taskId;
						taskData.taskPhase = paramData;						
						taskData.data1 = 0;
						taskData.data2 = 0;
						taskData.data3 = 0;
						taskData.data4 = 0;
						taskData.data5 = 0;
						taskData.data6 = 0;
						taskData.isRec = 0;

						taskData.randStep = pTaskDetail->GetInt(TASKDETAILDATA_RANDSTEP);
						
						CMsgTaskDetail msg;
						if(msg.Create(_TASKHINT_UPDATE,&taskData,1))
							pUser->SendMsg(&msg);
					}
					else if(0 == stricmp(szOpt,"="))
					{
						CTaskSetRandData* pSetData = TaskSetRand()->GetDataStruct(taskId, paramData);
						if(!pSetData)
							return false;
						pTaskDetail->SetInt(TASKDETAILDATA_TASKPHASE,paramData);
						pTaskDetail->SetInt(TASKDETAILDATA_TASKBEGINTIME,0);
						pTaskDetail->SetInt(TASKDETAILDATA_DATA1,0);
						pTaskDetail->SetInt(TASKDETAILDATA_DATA2,0);
						pTaskDetail->SetInt(TASKDETAILDATA_DATA3,0);
						pTaskDetail->SetInt(TASKDETAILDATA_DATA4,0);
						pTaskDetail->SetInt(TASKDETAILDATA_DATA5,0);
						pTaskDetail->SetInt(TASKDETAILDATA_DATA6,0);
						pTaskDetail->SetInt(TASKDETAILDATA_ISREC,0);

						TaskDetailInfoForMsg taskData;
						taskData.taskID = taskId;
						taskData.taskPhase = paramData;						
						taskData.data1 = 0;
						taskData.data2 = 0;
						taskData.data3 = 0;
						taskData.data4 = 0;
						taskData.data5 = 0;
						taskData.data6 = 0;
						taskData.isRec = 0;

						taskData.randStep = pTaskDetail->GetInt(TASKDETAILDATA_RANDSTEP);
						
						CMsgTaskDetail msg;
						if(msg.Create(_TASKHINT_UPDATE,&taskData,1))
								pUser->SendMsg(&msg);
						return true;
					}
					else if(0 == stricmp(szOpt,"has"))
					{
						CTaskSetRandData* pSetData = TaskSetRand()->GetDataStruct(taskId, paramData);
						if(!pSetData)
							return false;
						return true;
					}
					
				}
				else if(0 == stricmp(szItem,"mask"))
				{
					if(paramData < 0 || paramData > 63 )
						return false;
					if(0 == stricmp(szOpt,"+="))
					{
						pUser->AddTaskMask(paramData);
						return true;
					}
					else if(0 == stricmp(szOpt,"=="))
					{
						return pUser->ChkTaskMask(paramData);
					}
					else if(0 == stricmp(szOpt,"!="))
					{
						return !(pUser->ChkTaskMask(paramData));
					}
					else if(0 == stricmp(szOpt,"-="))
					{
						pUser->ClrTaskMask(paramData);
						return true;
					}
				}
				else if(0 == stricmp(szItem,"daymask"))
				{
					if(paramData < 0 || paramData > 63 )
						return false;
					if(0 == stricmp(szOpt,"+="))
					{
						pUser->AddDayTaskMask(paramData);
						return true;
					}
					else if(0 == stricmp(szOpt,"=="))
					{
						return pUser->ChkDayTaskMask(paramData);
					}
					else if(0 == stricmp(szOpt,"!="))
					{
						return !(pUser->ChkDayTaskMask(paramData));
					}
					else if(0 == stricmp(szOpt,"-="))
					{
						pUser->ClrDayTaskMask(paramData);
						return true;
					}
				}
				else if(0 == stricmp(szItem,"daymask2"))
				{
					if(paramData < 0 || paramData > 63 )
						return false;
					if(0 == stricmp(szOpt,"+="))
					{
						pUser->AddDayTaskMask2(paramData);
						return true;
					}
					else if(0 == stricmp(szOpt,"=="))
					{
						return pUser->ChkDayTaskMask2(paramData);
					}
					else if(0 == stricmp(szOpt,"!="))
					{
						return !(pUser->ChkDayTaskMask2(paramData));
					}
					else if(0 == stricmp(szOpt,"-="))
					{
						pUser->ClrDayTaskMask2(paramData);
						return true;
					}
				}
				else if(0 == stricmp(szItem,"weekmask"))
				{
					if(paramData < 0 || paramData > 63 )
						return false;
					if(0 == stricmp(szOpt,"+="))
					{
						pUser->AddWeekTaskMask(paramData);
						return true;
					}
					else if(0 == stricmp(szOpt,"=="))
					{
						return pUser->ChkWeekTaskMask(paramData);
					}
					else if(0 == stricmp(szOpt,"-="))
					{
						pUser->ClrWeekTaskMask(paramData);
						return true;
					}
				}
			}
			DEBUG_CATCH4("ACTION_USER_TASK_MANAGER ERROR USERID=%u,data=%u,param='%s'",pUser->GetID(),nData,szParam)
		}
		break;
	case ACTION_USER_DELCURNPC:
		{
			if(pUser)
			{
				pUser->DelCurNpc();
			}
			return true;
		}
		break;
	case ACTION_TASKSELECTITEM:
		{
			if(pUser)
			{
				if(0 == stricmp(szParam,"clr"))
				{
					pUser->SetTaskSelect(ID_NONE);
					return true;
				}
				else if(0 == stricmp(szParam,"check"))
				{
					return pUser->IsSelecetItem(pAction->GetInt(ACTIONDATA_DATA));
				}
			}
			return false;
		}
		break;
	case ACTION_USER_SETJOB:
		{
			OBJID FIRAUTOSKILL = 0;
			OBJID SECAUTOSKILL = 0;
			int nData = pAction->GetInt(ACTIONDATA_DATA);
			if(!pUser)
				return false;

			char szOpt[256]		= "";
			DWORD dwProff = 0;
			if (2 != sscanf(szParam, "%s %u", szOpt,&dwProff))
				return false;

			if(nData == 1 || nData == 2)
			{
				char szMsg[256] = "";
				char sztemp[3][128];
				char szServer[32];
				strcpy(szServer, MapGroup(PID)->GetServerName());
				sprintf(sztemp[0], "%d_[%s]_%d", SNAPSHOTUSER, pUser->GetName(),COLORYELLOW);
				sprintf(sztemp[2], "%d_天下惊闻_%d", SNAPSHOTNOTHING, COLORPINK);
				if (0 == stricmp(szOpt, "="))
				{
					pUser->SetProfession(nData,dwProff);
					if (nData == 1)
					{
// 						if (MALE_PASTOR == dwProff)
// 						{
// 							sprintf(sztemp[1], "%d_峨嵋派弟子_%d", SNAPSHOTNOTHING , COLORPINK);
// 							sprintf(szMsg, "<%s>心怀仁慈，在峨眉山救助伤患时经峨眉掌门提拔，成为了一名<%s>！", sztemp[0], sztemp[1]);
// 							UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, pUser, 0, 0, "" , ALLUSERS_NAME);
// 							//SetUserKey(95010, true);
// 						}
 						/*else*/ if (MALE_SOLDIER == dwProff)
 						{
							FIRAUTOSKILL = 2147483383;					
// 							sprintf(sztemp[1], "%d_少林派弟子_%d", SNAPSHOTNOTHING , COLORPINK);
// 							sprintf(szMsg, "<%s>舍己为人，在少室山潜心修炼时经少林方丈赏识，成为了一名<%s>！", sztemp[0], sztemp[1]);
// 							//UserManager()->BroadcastMsg(szMsg, NULL, NULL, 0, _TXTATR_ANNOUNCE);
// 							UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, pUser, 0, 0, "" , ALLUSERS_NAME);
// 							//SetUserKey(95020, true);
 						}
 						else if (MALE_MASTER == dwProff)
 						{
							FIRAUTOSKILL = 2147483631;
// 							sprintf(sztemp[1], "%d_全真教弟子_%d", SNAPSHOTNOTHING , COLORPINK);
// 							sprintf(szMsg, "<%s>至情重义，在终南山行侠仗义时被全真掌门选中，成为了一名<%s>！",  sztemp[0], sztemp[1]);
// 							//UserManager()->BroadcastMsg(szMsg, NULL, NULL, 0, _TXTATR_ANNOUNCE);
// 							UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, pUser, 0, 0, "" , ALLUSERS_NAME);
// 							//SetUserKey(95030, true);
 						}
// 						else if (MALE_SWORD == dwProff)
// 						{
// 							sprintf(sztemp[1], "%d_点苍派弟子_%d", SNAPSHOTNOTHING , COLORPINK);
// 							sprintf(szMsg, "<%s>箭术非凡，在苍山习弓练箭时经点苍掌门提拔，成为了一名<%s>！",  sztemp[0], sztemp[1]);
// 							//UserManager()->BroadcastMsg(szMsg, NULL, NULL, 0, _TXTATR_ANNOUNCE);
// 							UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, pUser, 0, 0, "" , ALLUSERS_NAME);
// 							//SetUserKey(95040, true);
// 						}
 						else if (MALE_ASSISSIN == dwProff)
						{
							FIRAUTOSKILL = 2147483631;
// 							sprintf(sztemp[1], "%d_丐帮弟子_%d", SNAPSHOTNOTHING , COLORPINK);
// 							sprintf(szMsg, "<%s>路见不平，在君山拔刀相助时被丐帮帮主相中，成为了一名<%s>！",  sztemp[0], sztemp[1]);
// 							UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, pUser, 0, 0, "" , ALLUSERS_NAME);
// 							//SetUserKey(95040, true);	
 						}
// 						else if (MALE_ASSIST == dwProff)
// 						{
// 							sprintf(sztemp[1], "%d_唐门弟子_%d", SNAPSHOTNOTHING , COLORPINK);
// 							sprintf(szMsg, "<%s>深藏不露，在唐家堡以少胜多时被唐门门主选中，成为了一名<%s>！",  sztemp[0], sztemp[1]);
// 							UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, pUser, 0, 0, "" , ALLUSERS_NAME);
// 							//SetUserKey(95050, true);
// 						}
						pUser->m_pUserKey.SetAutoSkill(FIRAUTOSKILL, 0, 0, pUser);
					}
					if (nData == 2)
					{
							
						if (MALE_PASTOR == dwProff)
						{
							sprintf(sztemp[1], "%d_峨嵋派_%d", SNAPSHOTNOTHING , COLORPINK);
							sprintf(szMsg, "<%s>：<%s>德才兼备，经过了重重考验，加入了第二门派<%s>，其名声威震江湖！", sztemp[2], sztemp[0], sztemp[1]);
							UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, pUser, 0, 0, "" , ALLUSERS_NAME);
						}
						else if (MALE_SOLDIER == dwProff)
						{
							SECAUTOSKILL = 2147483383;
							sprintf(sztemp[1], "%d_少林派_%d", SNAPSHOTNOTHING , COLORPINK);
							sprintf(szMsg, "<%s>：<%s>勇闯险关，经过了重重考验，加入了第二门派<%s>，其名声如雷贯耳！", sztemp[2], sztemp[0], sztemp[1]);
							UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, pUser, 0, 0, "" , ALLUSERS_NAME);
						}
						else if (MALE_MASTER == dwProff)
						{
							SECAUTOSKILL = 2147483631;
							sprintf(sztemp[1], "%d_全真教_%d", SNAPSHOTNOTHING , COLORPINK);
							sprintf(szMsg,"<%s>：<%s>卓尔不群，经过了重重考验，加入了第二门派<%s>，其名声无出其右！",  sztemp[2], sztemp[0], sztemp[1]);
							UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, pUser, 0, 0, "" , ALLUSERS_NAME);
						}
						else if (MALE_SWORD == dwProff)
						{
							sprintf(sztemp[1], "%d_点苍派_%d", SNAPSHOTNOTHING , COLORPINK);
							sprintf(szMsg, "<%s>：<%s>超群绝伦，经过了重重考验，加入了第二门派<%s>，其名声盖世无双！",  sztemp[2], sztemp[0], sztemp[1]);
							UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, pUser, 0, 0, "" , ALLUSERS_NAME);
						}
						else if (MALE_ASSISSIN == dwProff)
						{
							SECAUTOSKILL = 2147483631;
							sprintf(sztemp[1], "%d_丐帮_%d", SNAPSHOTNOTHING , COLORPINK);
							sprintf(szMsg, "<%s>：<%s>智勇双全，经过了重重考验，加入了第二门派<%s>，其名声举世闻名！",  sztemp[2], sztemp[0], sztemp[1]);
							UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, pUser, 0, 0, "" , ALLUSERS_NAME);
						}
						else if (MALE_ASSIST == dwProff)
						{
							sprintf(sztemp[1], "%d_唐门_%d", SNAPSHOTNOTHING , COLORPINK);
							sprintf(szMsg, "<%s>：<%s>足智多谋，经过了重重考验，加入了第二门派<%s>，其名声闻名遐迩！",  sztemp[2], sztemp[0], sztemp[1]);
							UserManager()->BroadCastMsgAndReceiveName(szMsg, NULL, NULL, 0x0099ff, _TXTATR_ANNOUNCE, pUser, 0, 0, "" , ALLUSERS_NAME);
						}
						pUser->m_pUserKey.SetAutoSkill(0, SECAUTOSKILL, 0, pUser);
					}

					return true;
				}
				else if(0 == stricmp(szOpt, "=="))
				{
					return pUser->GetProfession(nData) == dwProff;
				}
			}

			return false;
		}
		break;
	case	ACTION_USER_SEX:
		{
			return ( pUser->GetSex() );//0,男,1女..
		}
		break;
	case	ACTION_USER_LOG:
		{
			::GmLogSave(szParam);
			return true;
		}
		break;
	case	ACTION_USER_ATTR:
		{
			PARAMBUF	szAttr;
			PARAMBUF	szOpt;
			PARAMBUF	szData;
			if(sscanf(szParam, "%s %s %s", szAttr, szOpt, szData) < 3)
			{
				LOGERROR("ACTION %u: 错误的参数数量", pAction->GetID());
				break;
			}
			int		nData = atoi(szData);
			bool	bUpdate = UPDATE_FALSE;
			
			
			if(stricmp(szAttr,"vip")==0)
			{
				if(strcmp(szOpt, "+=") == 0)
				{					
					return pUser->AddVipTime(nData);
				}
				else if(strcmp(szOpt, "==") == 0)
				{
					if(!pUser->IsVipActive())
						return (0 == nData);
					else
						return (pUser->GetVipLev() == nData);
				}
				else if(strcmp(szOpt, "-=") == 0)
				{
					pUser->ClrVipTime();
					return true;
				}
			}
			if(stricmp(szAttr,"vit")==0)
			{
				if(strcmp(szOpt, "==") == 0)
				{					
					return (pUser->GetVit() == nData);
				}
				else if(strcmp(szOpt, "+=") == 0)
				{
					pUser->AddVit(nData);
					return true;
				}
				else if(strcmp(szOpt, "-=") == 0)
				{
					pUser->AddVit(-1 * nData);
					return true;
				}
				else if(strcmp(szOpt, "=") == 0)
				{
					pUser->SetVit(nData);
					return true;
				}
				else if(strcmp(szOpt, ">=") == 0)
				{
					return (pUser->GetVit() >= nData);
				}
				else if(strcmp(szOpt, "<") == 0)
				{
					return (pUser->GetVit() < nData);
				}
				else if(strcmp(szOpt, "full") == 0)
				{
					return (pUser->GetVit() == pUser->GetMaxVit());
				}
			}
			if(stricmp(szAttr,"givemask2")==0)
			{
				if(nData < 0 || nData > 63 )
					return false;
				if(0 == stricmp(szOpt,"+="))
				{
					pUser->AddNewGiveMask2(nData);
					return true;
				}
				else if(0 == stricmp(szOpt,"=="))
				{
					return pUser->ChkNewGiveMask2(nData);
				}
				else if(0 == stricmp(szOpt,"!="))
				{
					return !(pUser->ChkNewGiveMask2(nData));
				}
				else if(0 == stricmp(szOpt,"-="))
				{
					pUser->ClrNewGiveMask2(nData);
					return true;
				}
			}
			if(stricmp(szAttr,"ene")==0)
			{
				if(strcmp(szOpt, "==") == 0)
				{					
					return (pUser->GetEne() == nData);
				}
				else if(strcmp(szOpt, "+=") == 0)
				{
					pUser->AddEne(nData);
					return true;
				}
				else if(strcmp(szOpt, "-=") == 0)
				{
					pUser->AddEne(-1 * nData);
					return true;
				}
				else if(strcmp(szOpt, "=") == 0)
				{
					pUser->SetEne(nData);
					return true;
				}
				else if(strcmp(szOpt, ">=") == 0)
				{
					return (pUser->GetEne() >= nData);
				}
				else if(strcmp(szOpt, "<") == 0)
				{
					return (pUser->GetEne() < nData);
				}
				else if(strcmp(szOpt, "full") == 0)
				{
					return (pUser->GetEne() == pUser->GetMaxEne());
				}
			}
			if(stricmp(szAttr, "packagelev") == 0)
			{
				int nType = pAction->GetInt(ACTIONDATA_DATA);
				if(strcmp(szOpt, ">=") == 0)
				{
					return pUser->GetPacklev(nType)>=nData;
				}
				else if(strcmp(szOpt, "<") == 0)
				{
					return pUser->GetPacklev(nType)<nData;
				}
				else if(strcmp(szOpt, "=") == 0)
				{
					return pUser->SetPacklev(nType,nData);
				}
			}
			if(stricmp(szAttr, "Profession") == 0)
			{
				if(strcmp(szOpt, "==") == 0)
				{
					return pUser->GetProfession(0) == nData;
				}
			}
			if(stricmp(szAttr, "curjob") == 0)
			{
				if(strcmp(szOpt, "==") == 0)
				{
					return pUser->GetCurJob() == nData;
				}
				else if(strcmp(szOpt, "=") == 0)
				{
					return pUser->SetCurJob(nData);
				}
			}
			if(stricmp(szAttr, "banklev") == 0)
			{
				int nType = pAction->GetInt(ACTIONDATA_DATA);
				if(strcmp(szOpt, ">=") == 0)
				{
					return pUser->GetBanklev(nType)>=nData;
				}
				else if(strcmp(szOpt, "<") == 0)
				{
					return pUser->GetBanklev(nType)<nData;
				}
				else if(strcmp(szOpt, "=") == 0)
				{
					return pUser->SetBanklev(nType,nData);
				}
			}
			else if(stricmp(szAttr, "life") == 0)
			{
				if(strcmp(szOpt, "+=") == 0)
				{
					return pUser->AddAttrib(_USERATTRIB_LIFE, nData, SYNCHRO_TRUE);
				}
				else if(strcmp(szOpt, "==") == 0)
				{
					return (pUser->GetLife() == nData);
				}
				else if(strcmp(szOpt, "<") == 0)
				{
					return (pUser->GetLife() < nData);
				}
			}
			else if(stricmp(szAttr, "jobcon") == 0)
			{
				int nType = pAction->GetInt(ACTIONDATA_DATA);
				if(strcmp(szOpt, "<") == 0)
				{
					return (pUser->GetJobCon(nType) < nData);
				}
				else if(strcmp(szOpt, ">") == 0)
				{
					return (pUser->GetJobCon(nType) > nData);
				}
				else if(strcmp(szOpt, "==") == 0)
				{
					return (pUser->GetJobCon(nType) == nData);
				}
				else if(strcmp(szOpt, "=") == 0)
				{
					int nOldJobCon = pUser->GetJobCon(nType);
					pUser->SetJobCon(nType,nData);
					return true;
				}
				else if(strcmp(szOpt, "+=") == 0)
				{
					int nOldJobCon = pUser->GetJobCon(nType);
					pUser->SetJobCon(nType,max(0,nOldJobCon + nData));
					return true;
				}
				else if(strcmp(szOpt, "-=") == 0)
				{
					int nOldJobCon = pUser->GetJobCon(nType);
					pUser->SetJobCon(nType,max(0,nOldJobCon - nData));
					return true;
				}
			}
			else if(stricmp(szAttr, "Syncon") == 0)
			{
				if (!pUser->GetSyndicate())
					return false;
				if(strcmp(szOpt, "<") == 0)
				{
					return (pUser->GetSynCon() < nData);
				}
				else if(strcmp(szOpt, ">") == 0)
				{
					return (pUser->GetSynCon() > nData);
				}
				else if(strcmp(szOpt, "==") == 0)
				{
					return (pUser->GetSynCon() == nData);
				}
				else if(strcmp(szOpt, "=") == 0)
				{
					pUser->SetSynCon(nData);
					return true;
				}
				else if(strcmp(szOpt, "+=") == 0)
				{
					int nOldJobCon = pUser->GetSynCon();
					pUser->SetSynCon(max(0,nOldJobCon + nData));
					return true;
				}
				else if(strcmp(szOpt, "-=") == 0)
				{
					int nOldJobCon = pUser->GetSynCon();
					pUser->SetSynCon(max(0,nOldJobCon - nData));
					if (nData >= 100)
						::MyLogSave(VALUABLESLOG_FILENAME, "%u(%s) oldcon = %u ,lost %u from lua ",pUser->GetID(), pUser->GetName(), nOldJobCon, nData);
					return true;
				}
			}
			else if(stricmp(szAttr, "Synmoney") == 0)
			{
				//				int nType = pAction->GetInt(ACTIONDATA_DATA);
				if(strcmp(szOpt, "<") == 0)
				{
					return (pUser->GetSynMoney() < nData);
				}
				else if(strcmp(szOpt, ">") == 0)
				{
					return (pUser->GetSynMoney() > nData);
				}
				else if(strcmp(szOpt, "==") == 0)
				{
					return (pUser->GetSynMoney() == nData);
				}
				else if(strcmp(szOpt, "=") == 0)
				{
					pUser->SetSynMoney(nData);
					return true;
				}
				else if(strcmp(szOpt, "+=") == 0)
				{
					pUser->SetSynMoney(max(0,pUser->GetSynMoney() + nData));
					return true;
				}
				else if(strcmp(szOpt, "-=") == 0)
				{
					pUser->SetSynMoney(max(0,pUser->GetSynMoney() - nData));
					return true;
				}
			}
			else if(stricmp(szAttr, "Syngrow") == 0)
			{
				//				int nType = pAction->GetInt(ACTIONDATA_DATA);
				if(strcmp(szOpt, "<") == 0)
				{
					return (pUser->GetSynGrow() < nData);
				}
				else if(strcmp(szOpt, ">") == 0)
				{
					return (pUser->GetSynGrow() > nData);
				}
				else if(strcmp(szOpt, "==") == 0)
				{
					return (pUser->GetSynGrow() == nData);
				}
				else if(strcmp(szOpt, "=") == 0)
				{
					pUser->SetSynGrow(nData);
					return true;
				}
				else if(strcmp(szOpt, "+=") == 0)
				{
					pUser->SetSynGrow(max(0,pUser->GetSynGrow() + nData));
					return true;
				}
				else if(strcmp(szOpt, "-=") == 0)
				{
					pUser->SetSynGrow(max(0, pUser->GetSynGrow()- nData));
					return true;
				}
			}
			//--------------------------------------------------------------//
			else if(stricmp(szAttr, "last_logout") == 0)
			{
				if(strcmp(szOpt, "<") == 0)
				{
					return (pUser->GetLastLogin() < nData);
				}
				else if(strcmp(szOpt, ">") == 0)
				{
					return (pUser->GetLastLogin() > nData);
				}
				else if(strcmp(szOpt, "==") == 0)
				{
					return (pUser->GetLastLogin() == nData);
				}				
			}
			else if (stricmp(szAttr, "tutor_level") == 0)
			{
				if (strcmp(szOpt, ">=") == 0)
				{
					return (pUser->GetTutorLevel() >= nData);
				}
				else if (strcmp(szOpt, "-=") == 0)
				{
					pUser->AddAttrib(_USERATTRIB_TUTOR_LEVEL, nData * -1, SYNCHRO_TRUE);
					return true;
				}
			}
			else if(stricmp(szAttr, "mana") == 0)
			{
				if(strcmp(szOpt, "+=") == 0)
				{
					return pUser->AddAttrib(_USERATTRIB_MANA, nData, SYNCHRO_TRUE);
				}
				else if(strcmp(szOpt, "==") == 0)
				{
					return (pUser->GetMana() == nData);
				}
				else if(strcmp(szOpt, "<") == 0)
				{
					return (pUser->GetMana() < nData);
				}
			}
			else if(stricmp(szAttr, "money") == 0)
			{
				if(strcmp(szOpt, "+=") == 0)
				{
					return pUser->AddAttrib(_USERATTRIB_MONEY + pAction->GetInt(ACTIONDATA_DATA) - 1, nData, SYNCHRO_TRUE);
				}
				else if( 0 == stricmp( szOpt,"-=" ) )
				{
					DWORD dwMoney = pUser->GetMoneyByType(pAction->GetInt(ACTIONDATA_DATA));
					if( dwMoney < nData )
						return false;
					else
					{
						pUser->SetAttrib( _USERATTRIB_MONEY + pAction->GetInt(ACTIONDATA_DATA) - 1,dwMoney - nData, SYNCHRO_TRUE );
						return true;
					}
				}
				else if(strcmp(szOpt, "==") == 0)
				{
					return (pUser->GetMoneyByType(pAction->GetInt(ACTIONDATA_DATA)) == nData);
				}
				else if(strcmp(szOpt, "<") == 0)
				{
					return (pUser->GetMoneyByType(pAction->GetInt(ACTIONDATA_DATA)) < nData);
				}
			}
			else if (stricmp(szAttr, "jinmeivalue") == 0)
			{
				if (0 == stricmp(szOpt, "+="))
				{
					return pUser->IncJinmeiValue(nData);
				}
				else if (0 == stricmp(szOpt, "=="))
				{
					return pUser->GetJinmeiValue() == nData;
				}
				else if (0 == stricmp(szOpt, "-="))
				{
					return pUser->SpendJinmeiValue(nData);
				}
			}
			else if( stricmp(szAttr,"weapon_type") == 0 )
			{
				if( !pUser->GetWeaponR())
				{
					return false;
				}
				else if( 0 == stricmp( szOpt,"==" ) )
				{
					return ( pUser->GetWeaponR()->GetTypeID() == nData );
				}
			}
			else if(stricmp(szAttr, "exp") == 0)
			{
				__int64	i64Data = _atoi64(szData);
				if(strcmp(szOpt, "+=") == 0)
				{
					DWORD dwExp = pUser->AwardBattleExp(nData, false, false,true);
					if(dwExp > 0)
					{
						pUser->SendSysMsg("你获得了%u经验",dwExp);
						return true;
					}
					else
						return false;
					
				}
				else if(strcmp(szOpt, "==") == 0)
				{
					return (pUser->GetExp() == i64Data);
				}
				else if(strcmp(szOpt, "<") == 0)
				{
					return (pUser->GetExp() < i64Data);
				}
				else if( strcmp( szOpt, "=" ) == 0 )// 使用归元丹，经验归零 [陈剑飞 7/10/2007]
				{
					pUser->SetExp( i64Data );
					  return true;
				}
			}
			else if(stricmp(szAttr, "petexp") == 0)
			{
				if(pUser->m_pWarPet)
				{
					__int64	i64Data = _atoi64(szData);
					if(strcmp(szOpt, "+=") == 0)
					{
						DWORD dwExp = pUser->AwardEudemonExp(nData);
						if(dwExp > 0)
						{
							pUser->SendSysMsg("你的%s获得%u经验",pUser->m_pWarPet->GetName(),dwExp);
						}
						return true;
					}
					else if(strcmp(szOpt, "==") == 0)
					{
						return (pUser->m_pWarPet->GetExp() == i64Data);
					}
					else if(strcmp(szOpt, "<") == 0)
					{
						return (pUser->m_pWarPet->GetExp() < i64Data);
					}
				}
			}
			else if(stricmp(szAttr, "weapenexp") == 0)
			{
				__int64	i64Data = _atoi64(szData);
				if(strcmp(szOpt, "+=") == 0)
				{
					return pUser->AwardWeaponExp(i64Data);
				}
			}
			else if(stricmp(szAttr, "pk") == 0)
			{
				if(strcmp(szOpt, "+=") == 0)
				{
					pUser->AddPk(nData);
				}
				else if(strcmp(szOpt, "==") == 0)
				{
					if(pUser->GetPk() == nData)
						return true;
				}
				else if(strcmp(szOpt, "<") == 0)
				{
					if(pUser->GetPk() < nData)
						return true;
				}
				else if( strcmp( szOpt, "=" ) ==0 )// 使用赎罪卷，PK值清零 [陈剑飞 7/10/2007]
				{
				     pUser->SetPk(nData);
					 return true;
				}
				else if( strcmp( szOpt, "-=" ) ==0 )
				{
					int nOld = pUser->GetPk();
					if(nOld > 0)
					{
						pUser->SetPk(nOld - 1);
						return true;
					}
				}
				else if( strcmp(szOpt,">" ) == 0)
				{
					if(pUser->GetPk() > nData)
						return true;
				}
			}
			
			else if(stricmp(szAttr, "level") == 0)
			{
				if( 0 ==  stricmp( szOpt,">" ) )
				{
					if( pUser->GetLev() > nData )
						return true;
				}
				else if( 0 ==  stricmp( szOpt,">=" ) )
				{
					if( pUser->GetLev() >= nData )
						return true;
				}
				else if(strcmp(szOpt, "==") == 0)
				{
					if(pUser->GetLev() == nData)
						return true;
				}
				else if(strcmp(szOpt, "<") == 0)
				{
					if(pUser->GetLev() < nData)
						return true;
				}
				else if(strcmp(szOpt, "+=") == 0)
				{
					pUser->IncLev(nData);
					return true;
				}
				
				return false;
			}
			else if(stricmp(szAttr, "crime") == 0)
			{
				if (strcmp(szOpt, "==") == 0)
				{
					if (nData == 0)
						return (pUser->QueryStatus(STATUS_CRIME) == NULL);
					else
						return (pUser->QueryStatus(STATUS_CRIME) != NULL);
				}
				else if (strcmp(szOpt, "set") == 0)
				{
					if (nData == 0)
					{
						CRole::DetachStatus(pUser->QueryRole(), STATUS_CRIME);
	
						return true;
					}
					else
					{
						pUser->SetCrimeStatus();
						return true;
					}
				}
			}			
			else if (stricmp(szAttr, "tutor_exp") == 0)
			{
				if (strcmp(szOpt, "==") == 0)
				{
					return (pUser->GetTutorExp() == nData);
				}
				else if (strcmp(szOpt, "<") == 0)
				{
					return (pUser->GetTutorExp() < nData);
				}
				else if (strcmp(szOpt, "+=") == 0)
				{
					pUser->AddAttrib(_USERATTRIB_TUTOR_EXP, nData, SYNCHRO_TRUE);
					return true;
				}
				else if (strcmp(szOpt, "=") == 0)
				{
					pUser->SetAttrib(_USERATTRIB_TUTOR_EXP, nData, SYNCHRO_TRUE);
					return true;
				}
			}
			else if (stricmp(szAttr, "syn_proffer") == 0) 
			{		
				if (!pUser->GetSyndicate())
					return false;
				if (strcmp(szOpt, "<") == 0)
				{
					return (pUser->QuerySynProffer() < nData);
				}
				else if (strcmp(szOpt, "+=") == 0)
				{
					if (pUser->QuerySynAttr())
					{
						pUser->QuerySynAttr()->SetProffer(pUser->QuerySynProffer() + nData);
						return true;
					}
						return false;
				}		
				else if (strcmp(szOpt, "=") == 0)
				{
					if (pUser->QuerySynAttr())
					{
						pUser->QuerySynAttr()->SetProffer(nData);
						return true;
					}
						return false;
				}
				else if( 0 == strcmp( szOpt,">=" ) )
				{
					return ( pUser->QuerySynProffer() >= nData );
				}
				else if (0 == strcmp(szOpt, "=="))
				{
					return(pUser->QuerySynProffer() == nData);
				}
				else if (0 == strcmp(szOpt, "<="))
				{
					return(pUser->QuerySynProffer() <= nData);
				}
				else if (0 == strcmp(szOpt, ">"))
				{
					return(pUser->QuerySynProffer() > nData);
				}
				else if (0 == strcmp(szOpt, "!="))
				{
					return(pUser->QuerySynProffer() != nData);
				}
				else if (0 == strcmp(szOpt, "-="))
				{
					if(pUser->QuerySynAttr())
					{
						pUser->QuerySynAttr()->SetProffer(pUser->QuerySynProffer() - nData);
						return true;
					}
					return false;
				}
			}
			
			else if(  0 == stricmp( szAttr, "presentEx" ) ) //地推礼物值
			{
				if( 0 == strcmp( szOpt,"==" ) )
				{
					return pUser->GetCardState(pAction->GetInt(ACTIONDATA_DATA)) == nData;
				}
				if( 0 == strcmp( szOpt,"=" ) )
				{
					pUser->SetCardState(pAction->GetInt(ACTIONDATA_DATA),nData,true);
					return true;
				}
			}
			else if(  0 == stricmp( szAttr, "topListAward" ) ) //排行榜礼物值
			{
				if( 0 == strcmp( szOpt,"==" ) )
				{
					return pUser->GetActivityAward(pAction->GetInt(ACTIONDATA_DATA)) == nData;
				}
				if( 0 == strcmp( szOpt,"=" ) )
				{
					pUser->SetActivityAward(pAction->GetInt(ACTIONDATA_DATA), nData, true);
					return true;
				}
			}
			else if( 0== stricmp( szAttr, "getMobChargeP" ))
			{
				if( 0 == strcmp( szOpt,">" ) )
				{
					return pUser->GetActivityAward(pAction->GetInt(ACTIONDATA_DATA))>nData;
				}
				
				if( 0 == strcmp( szOpt,"-=" ) )
				{
				    pUser->SetMobChargeAward(pAction->GetInt(ACTIONDATA_DATA), nData, true);
					return true;
				}

			}

			else if (stricmp(szAttr, "warscore") == 0) //竞技场积分
			{
				if (strcmp(szOpt, "==") == 0)
				{
					return (pUser->GetArenaScore() == nData);
				}
				else if (strcmp(szOpt, "<") == 0)
				{
					return (pUser->GetArenaScore() < nData);
				}
				else if (strcmp(szOpt, "+=") == 0)
				{
					pUser->AddAttrib(_USERATTRIB_ARENA_SCORE, nData, SYNCHRO_TRUE);
					return true;
				}
				else if (strcmp(szOpt, "-=") == 0)
				{
					pUser->AddAttrib(_USERATTRIB_ARENA_SCORE, -1 * nData, SYNCHRO_TRUE);
					return true;
				}
				else if (strcmp(szOpt, "=") == 0)
				{
					pUser->SetAttrib(_USERATTRIB_ARENA_SCORE, nData, SYNCHRO_TRUE);
					return true;
				}
			}
			
			else if(  0 == stricmp( szAttr, "natCon" ) ) //大宋荣誉
			{
				if(strcmp(szOpt, "+=") == 0)
				{
					pUser->AddNationCon(nData);
				}
				else if(strcmp(szOpt, "==") == 0)
				{
					if(pUser->GetNationCon() == nData)
						return true;
				}
				else if(strcmp(szOpt, "<") == 0)
				{
					if(pUser->GetNationCon() < nData)
						return true;
				}
				else if( strcmp( szOpt, "=" ) ==0 )
				{
					pUser->SetNationCon(nData);
					return true;
				}
				else if( strcmp( szOpt, "-=" ) ==0 )
				{
					ASSERT(nData);
					int nOld = pUser->GetNationCon();
					if(nOld - nData >= 0)
					{
						nOld = nOld - nData;
					}
					else
						nOld = 0;

					pUser->SetNationCon(nOld);
					return true;
				}
				else if( strcmp(szOpt,">" ) == 0)
				{
					if(pUser->GetNationCon() > nData)
						return true;
				}
			}

			else if(  0 == stricmp( szAttr, "chiCon" ) ) //江湖侠义
			{
				if(strcmp(szOpt, "+=") == 0)
				{
					pUser->AddChivalrous(nData);
				}
				else if(strcmp(szOpt, "==") == 0)
				{
					if(pUser->GetChivalrous() == nData)
						return true;
				}
				else if(strcmp(szOpt, "<") == 0)
				{
					if(pUser->GetChivalrous() < nData)
						return true;
				}
				else if( strcmp( szOpt, "=" ) ==0 )
				{
					pUser->SetChivalrous(nData);
					return true;
				}
				else if( strcmp( szOpt, "-=" ) ==0 )
				{
					ASSERT(nData);
					int nOld = pUser->GetChivalrous();
					if(nOld - nData >= 0)
					{
						nOld = nOld - nData;
					}
					else
						nOld = 0;
					
					pUser->SetChivalrous(nOld);
					return true;
				}
				else if( strcmp(szOpt,">" ) == 0)
				{
					if(pUser->GetChivalrous() > nData)
						return true;
				}
			}

			else if(0 == stricmp(szAttr, "user_holdtitle")
				|| 0 == stricmp(szAttr, "userholdtitle"))//[游途道标 2008/10/14]已拥有称号修改
			{
				if(0 == strcmp(szOpt, "+="))
				{
					pUser->AddHoldTitle(nData, pAction->GetInt(ACTIONDATA_DATA));
					return true;
				}
				else if(0 == strcmp(szOpt, "-="))
				{
					pUser->DelHoldTitle(nData);
					return true;
				}
				else if(0 == strcmp(szOpt, "=="))//称号查找
				{
					return pUser->FindTitleByType(nData) != NULL;
				}
				else if(0 == strcmp(szOpt, "++"))//称号升级
				{
					pUser->LevUpTitle(pAction->GetInt(ACTIONDATA_DATA));
					return true;
				}
				else if(0 == strcmp(szOpt, "="))//设置称号等级
				{
					pUser->SetTitleLevel(pAction->GetInt(ACTIONDATA_DATA), nData);
					return true;
				}
				else if(0 == strcmp(szOpt, ">="))//判断称号等级
				{
					return pUser->GetTitleLev(pAction->GetInt(ACTIONDATA_DATA)) >= nData;
				}
			}
			else
			{	
				LOGERROR("ACTION %u: szParam %s", pAction->GetID(),szParam);
				ASSERT(!"ACTION_USER_ATTR");
			}
		}
		break;
	case	ACTION_USER_FULL:
		{
			if (0 == stricmp(szParam, "life"))
			{
				return pUser->AddAttrib(_USERATTRIB_LIFE, pUser->GetMaxLife(), SYNCHRO_TRUE);
			}
			else if (0 == stricmp(szParam, "mana"))
			{
				return pUser->AddAttrib(_USERATTRIB_MANA, pUser->GetMaxMana(), SYNCHRO_TRUE);
			}
			else if( 0 == stricmp( szParam,"petlife" ) )
			{
				if(pUser->m_pWarPet)
					return pUser->m_pWarPet->AddAttrib( _USERATTRIB_LIFE,pUser->m_pWarPet->GetMaxLife(),SYNCHRO_TRUE );
				return false;
			}
		}
		break;
	case	ACTION_USER_CHGMAP:
		{
			OBJID idMap = ID_NONE;
			int nMapX=0, nMapY=0;
			BOOL bImmediacy = false;

			if (sscanf(szParam, "%u %d %d %d", &idMap, &nMapX, &nMapY, &bImmediacy) < 3)
			{
				LOGERROR("ACTION %u: 错误的参数数量", pAction->GetID());
				break;
			}

			if(idMap == 0 && nMapX == 0 && nMapY == 0)
			{
				idMap = pUser->GetRecordMapID();
				nMapX = pUser->GetRecordPosX();
				nMapY = pUser->GetRecordPosY();
			}

			if (!pUser->GetMap()->IsTeleportDisable() || bImmediacy)
			{
				pUser->ProcessOnMove(MOVEMODE_CHGMAP);
				if (idMap==1048)
				{
					CSynPtr pSyn=MapGroup(MSGPORT_MAPGROUP_FIRST)->GetSynManager()->QuerySyndicate(pUser->GetSynID());
					if (pSyn)
					{
						idMap=pSyn->GetSynMapID();
					}
				}
				int ret = pUser->FlyMap(idMap, nMapX, nMapY);
				if(ret == FLYMAP_NORMAL)
					return true;
				else if(ret == FLYMAP_MAPGROUP)
				{

					m_pUser	= NULL;					// 清除任务玩家
					return true;
				}
				else if(ret == FLYMAP_ERROR)
				{
					return false;
				}
			}
		}
		break;
	case ACTION_USER_CHECKWEAPENSOUL:
		{
			CItem* pItem = pUser->GetEquipItemByPos(ITEMPOSITION_WEAPONSOUL);
			if(!pItem)
				return false;
			if(!pItem->IsWeaponSoul())
				return false;
			return true;
		}
		break;
	case ACTION_USER_CHGTO_MAINMAP: //到主地图的复活点复活
		{
			return pUser->FlyToMainMap();
		}
		break;	
	case ACTION_USER_DEPOT_CODE:
		{
			if(0 == strcmp("无", pUser->GetDepotCode()))
				return false;
			else
				return true;
		}
		break;
	case ACTION_USER_ADDEUDEMON:
		{
			EudemonData eudData;
			return pUser->AwardEudemon(pAction->GetInt(ACTIONDATA_DATA),eudData);
		}
		break;
	case ACTION_USER_EUDEMON:
		{
			int data  = 0;
			char szText1[36] = {0};
			char szText2[36] = {0};
			if (3 == sscanf(szParam, "%s %s %d", szText1, szText2, &data))
			{
				if(strcmp(szText1,"born") == 0)
				{
					if(strcmp(szText2,"cando") == 0)
					{
						return pUser->CanEudemonBorn();
					}
					else if(strcmp(szText2,"init") == 0)
					{
						if(pUser->GetTeam())
							return pUser->GetTeam()->initEudemonBron();
						return false;
					}
					else if(strcmp(szText2,"get") == 0)
					{
						return pUser->EudemonBronGet();
						return false;
					}
				}
				else if(strcmp(szText1,"play") == 0)
				{
					if(strcmp(szText2,"cando") == 0)
					{
						return pUser->CanEudemonPlay();
					}
					else if(strcmp(szText2,"init") == 0)
					{
						if(pUser->GetTeam())
							return pUser->GetTeam()->initEudemonPlay();
						return false;
					}
					else if(strcmp(szText2,"get") == 0)
					{
						return pUser->EudemonPlayGet();
						return false;
					}
				}
			}
			return false;
		}
	case ACTION_USER_EUDEMON_DEAD:
		{
			pUser->EudemonDead(/*pAction->GetInt(ACTIONDATA_DATA)*/);
			return true;
		}
		break;
	case ACTION_USER_CHGTO_RANDOMPOS:
		{
			return pUser->FlyToRandomPos();
		}
		break;
	case	ACTION_USER_RECORDPOINT:
		{
			OBJID idMap = ID_NONE;
			int nMapX=0, nMapY=0;
			if (3 == sscanf(szParam, "%u %d %d", &idMap, &nMapX, &nMapY))
			{
				pUser->SetRecordPos(idMap, nMapX, nMapY, false);
				return true;
			}
		}
		break;
	case	ACTION_USER_RESETPOINT:
		{
			if(pAction->GetInt(ACTIONDATA_DATA) < 0 || pAction->GetInt(ACTIONDATA_DATA) > 5 )
				return false;
			int nData = ::atoi(szParam);
			return pUser->ReformPoint(pAction->GetInt(ACTIONDATA_DATA),nData);
		}
		break;
	case ACTION_USER_CHK_ITEMAMOUNT:
		{
			int nData = ::atoi(szParam);
			return pUser->GetItemNumByType(pAction->GetInt(ACTIONDATA_DATA)) >= nData;
		}
		break;

	case ACTION_USER_RANDSHOPFULL:
		{
			CItem* pItemMoney = pUser->GetItemByType(_SPCITEM_MONEYFORSHOP);
			if(!pItemMoney)
				return false;
			
			int nHasMoney = pItemMoney->GetItemAmount();
						
			if(pUser->GetLev() < 50)
			{
				if(nHasMoney >= 50000)
					return true;
			}
			else if(pUser->GetLev() < 70)
			{
				if(nHasMoney >= 100000)
					return true;
			}
			else if(pUser->GetLev() < 90)
			{
				if(nHasMoney >= 150000)
					return true;
			}
			else
			{
				if(nHasMoney >= 180000)
					return true;
			}
			return false;
		}
		break;
	case    ACTION_USER_INSTANCETIMES_OPERATE:
		{
			int nInstanceType = pAction->GetInt(ACTIONDATA_DATA);

			char  szKey[16];
			char  szSign[16];
			DWORD dwExtra1 = 0;

			if (3 == sscanf(szParam, "%s %s %u", szKey, szSign, &dwExtra1))
			{
				int nTime = InstanceUseMgr()->CheckInstanceUse(pUser->GetID(), nInstanceType);
				if(nTime == -1)
					nTime = 0;
				

				if(0 == stricmp("time", szKey))
				{
					if (0 == stricmp("+", szSign))
					{
						return InstanceUseMgr()->AddNewInstanceUse(pUser->GetID(), nInstanceType,dwExtra1);
					}
					else if (0 == stricmp(">", szSign))
					{
						return nTime > dwExtra1;
					}
					else if (0 == stricmp("<", szSign))
					{
						return nTime < dwExtra1;
					}
					else if (0 == stricmp("==", szSign))
					{
						return nTime == dwExtra1;
					}
				}
				else if(0 == stricmp("using", szKey))
				{
					if (0 == stricmp("==", szSign))
					{
						if(InstanceUseMgr()->CheckInstanceUse(pUser->GetID(), nInstanceType) == -1)
							return 0 == dwExtra1;
						else
							return (InstanceUseMgr()->CheckUseInstance(pUser->GetID(), nInstanceType) == dwExtra1);
					}
					else if (0 == stricmp("-", szSign))
					{
						pUser->ClearUsingInstance();
						return true;
						//return (InstanceUseMgr()->UseInstance(pUser->GetID(), nInstanceType,dwExtra1));
					}
				}
				else if(0 == stricmp("finishtime", szKey))
				{
					if (0 == stricmp("+", szSign))
					{
						return InstanceUseMgr()->FinishInstance(pUser->GetID(), nInstanceType,dwExtra1);
					}
				}
			}
			return false;
		}
		break;

	case	ACTION_USER_MAGIC:
		{
			if (!pUser->QueryMagic())
				return false;

			PARAMBUF	szCmd;
			int			nType;
			int			nData=0;
			bool		bSave=true;
			int	nNum = 0;
			if((nNum=sscanf(szParam, "%s %u %u %u", szCmd, &nType, &nData, &bSave)) < 2)
			{
				LOGERROR("ACTION %u: 错误的参数数量", pAction->GetID());
				break;
			}

			if(stricmp(szCmd, "check") == 0)
			{
				if(nNum == 3)
					return pUser->QueryMagic()->CheckLevel(nType, nData);
				else
					return pUser->QueryMagic()->CheckType(nType);
			}
			else if(stricmp(szCmd, "learn") == 0)
			{
				return pUser->QueryMagic()->LearnMagic(nType, nData, bSave) != ID_NONE;
			}
// 			else if(stricmp(szCmd, "uplevel") == 0)
// 			{
// 				return pUser->QueryMagic()->UpLevelByTask(nType);
// 			}
			else if(stricmp(szCmd, "addexp") == 0)
			{
				return pUser->QueryMagic()->AwardExp(nType, 0, nData);
			}
			else
			{
				ASSERT(!"ACTION_USER_MAGIC 错误的参数类型!");
			}
			return true;
		}
		break;
	case ACTION_USER_CHECKINSTANCE:
		{
			if(pUser && pUser->GetMap())
				return pUser->GetMap()->IsDynaMap();
			return false;
		}
		break;
	case ACTION_SEND_NOTICE:
		{
			if (int n = strlen(szParam))
			{
				char szTemp[1024];
				szTemp[0] = '\0';
				StrCpyN(szTemp, szParam, n);
				pUser->SendNotice(szTemp);
			}
		}
		break;
	case ACTION_USER_OPEN_DIALOG:
		{
			OBJID idNpc = 0;

			DEBUG_TRY
			if(pAction->GetInt(ACTIONDATA_DATA) == 2)
			{
				int idNpc = 0;
				if(sscanf(szParam,"%u",&idNpc) == 1)
				{
					DEBUG_TRY
						pUser->ActivateShopNpc(idNpc);
					DEBUG_CATCH2("pUser->ActivateShopNpc(%u);",idNpc)
						return true;
				}
				return false;
			}
			if(pAction->GetInt(ACTIONDATA_DATA) == 27 || pAction->GetInt(ACTIONDATA_DATA) == 42 )
			{
				if(sscanf(szParam,"%u",&idNpc) != 1)
				{
					return false;
				}
			}
			if(pAction->GetInt(ACTIONDATA_DATA) == 28  || pAction->GetInt(ACTIONDATA_DATA) == 30)
			{
				if(sscanf(szParam,"%u",&idNpc) != 1)
				{
					return false;
				}
			}
			else if(pAction->GetInt(ACTIONDATA_DATA) == 3 || pAction->GetInt(ACTIONDATA_DATA) == 29 )
			{
				idNpc = 0;

				int taskID;
				int taskStep;
				if(sscanf(szParam,"%d %d",&taskID,&taskStep) == 2)
				{
					if(taskID > 10000)
						idNpc = taskID*1000 + taskStep;
					else
						idNpc = taskID*100 + taskStep;
				}
			}
			else if(pAction->GetInt(ACTIONDATA_DATA) == 33)
			{
				idNpc = 0;
				
				int taskID;
				int taskStep;
				if(sscanf(szParam,"%d %d",&taskID,&taskStep) == 2)
				{
					idNpc = taskID*100000 + taskStep;
				}
			}
			else if(pAction->GetInt(ACTIONDATA_DATA) == 82)
			{
				int cur = 0;
				int begain = 0;
				int end = 0;
				sscanf(szParam,"%d %d %d",&cur,&begain,&end);
				idNpc = cur * 10000 + begain * 100 + end;
			}
			else if(pAction->GetInt(ACTIONDATA_DATA) == 52)
			{
				OBJID id = 0;
				sscanf(szParam,"%d",&id);
				if(id)
					idNpc = id;
			}
			else if(pAction->GetInt(ACTIONDATA_DATA) == 15 || pAction->GetInt(ACTIONDATA_DATA) == 16)//强化和升星之前同步元宝
			{
			}
			else if(pAction->GetInt(ACTIONDATA_DATA)==54 || pAction->GetInt(ACTIONDATA_DATA)==55 || pAction->GetInt(ACTIONDATA_DATA)==56)//时限宝盒
			{
				OBJID id = 0;
				sscanf(szParam,"%u",&id);
				if(id)
					idNpc = id;
			}
			DEBUG_CATCH2("ACTION_USER_OPEN_DIALOG %u",pAction->GetInt(ACTIONDATA_DATA))
			CMsgAction	msg;
			IF_OK(msg.Create(pUser->GetID(), pUser->GetPosX(), pUser->GetPosY(), pUser->GetDir(), actionOpenDialog, pAction->GetInt(ACTIONDATA_DATA),idNpc,0))
				pUser->SendMsg(&msg);
	

			return true;
		}
		break;
	case ACTION_USER_CHGMAP_REBORN:
		{
			OBJID	idRebornMap = ID_NONE;
			POINT	pos;
			IF_OK(pUser->GetMap()->GetRebornMap(&idRebornMap, &pos))
				pUser->FlyMap(idRebornMap, pos.x, pos.y);
			return true;
		}
		break;

	case ACTION_USER_TAKESTUDENTEXP:
		{
			if (pszAccept && strlen(pszAccept) > 0 && pUser)
			{
				int nExpTake = atoi(pszAccept);
				if (nExpTake > 0)
					return pUser->TakeExpFromStudent(nExpTake);
				else
					return false;
			}
		}
		break;
	case ACTION_TEAM_BROADCAST:
		{
			CTeam* pTeam = pUser->GetTeam();
			if(!pTeam /*|| pUser->GetID() != pTeam->GetLeader() */|| pTeam->GetMemberAmount() <= 1)
				return false;

			for(int i = 0; i < pTeam->GetMemberAmount(); i++)
			{
				CUser* pMember = pUser->FindAroundUser(pTeam->GetMemberByIndex(i));
				if(pMember /*&& pMember->GetID() != pUser->GetID()*/)
				{
					pMember->SendSysMsg(_TXTATR_TEAM, szParam);
				}
			}
			return true;
		}
		break;
	case ACTION_TEAM_CHK_ISINTEAM:
		{
			CTeam* pTeam = pUser->GetTeam();
			if(!pTeam || pTeam->GetMemberAmount() <= 1)
				return false;
			return true;
		}
		break;
	case ACTION_TEAM_ISTUTOR:// pengfeng [10/26/2007]
		{
			CTeam* pTeam = pUser->GetTeam();
			if(!pTeam || pTeam->GetMemberAmount() <= 1)
				return false;

			PARAMBUF	szField, szOpt="(none)";
			int		nData=0;
			if (sscanf(szParam, "%s %s %d", szField, szOpt, &nData) < 1)
			{
				LOGERROR("ACTION %u: 错误的参数数量", pAction->GetID());
				return false;
			}

			if(strcmp(szField, "hastutor")==0)
			{
				CTutorData* pTutor = pUser->GetTutor();
				if(NULL == pTutor)
					return false;

				for(int i = 0; i < pTeam->GetMemberAmount(); i++)
				{
					OBJID idTeamMem = pTeam->GetMemberByIndex(i);
					if(pTutor->GetTutorID() == idTeamMem)
						return true;
				}
				return false;
			}
			else if(strcmp(szField, "hasstudent")==0)
			{
				if(0 == pUser->GetStudentAmount(false))
					return false;

				CTutorData* pData = NULL;
				for(int i = 0; i < pTeam->GetMemberAmount(); i++)
				{
					OBJID idMember = pTeam->GetMemberByIndex(i);
					
					pData = pUser->GetStudent(idMember);
					if (pData && pData->GetState()==TUTORSTATE_NORMAL)
						return true;
				}

				return false;
			}
			
			return false;
		}
		break;
		//---------------------------------
	case	ACTION_TEAM_ATTR:
		{
			CTeam* pTeam = pUser->GetTeam();
			if(!pTeam)
				return false;

			PARAMBUF	szField, szOpt="(none)";
			int		nData=0;
			if (sscanf(szParam, "%s %s %d", szField, szOpt, &nData) < 1)
			{
				LOGERROR("ACTION %u: 错误的参数数量", pAction->GetID());
				return false;
			}

			if(strcmp(szField, "count")==0)
			{
				if(strcmp(szOpt, "<")==0)
				{
					if(!( pTeam->GetOnlineMemberAmount() < nData ))
						return false;
				}
				if(strcmp(szOpt, ">")==0)
				{
					if(!( pTeam->GetOnlineMemberAmount() > nData ))
						return false;
				}
				else if(strcmp(szOpt, "==")==0)
				{
					if(!( pTeam->GetOnlineMemberAmount() == nData ))
						return false;
				}
				else
				{
					return false;
				}
				return true;
			}
			else if(strcmp(szField, "countmale")==0)
			{
				if(strcmp(szOpt, "<")==0)
				{
					if(!( pTeam->GetOnlineMemberAmount(0) < nData ))
						return false;
				}
				if(strcmp(szOpt, ">")==0)
				{
					if(!( pTeam->GetOnlineMemberAmount(0) > nData ))
						return false;
				}
				else if(strcmp(szOpt, "==")==0)
				{
					if(!( pTeam->GetOnlineMemberAmount(0) == nData ))
						return false;
				}
				else
				{
					return false;
				}
				return true;
			}
			else if(strcmp(szField, "countfemale")==0)
			{
				if(strcmp(szOpt, "<")==0)
				{
					if(!( pTeam->GetOnlineMemberAmount(1) < nData ))
						return false;
				}
				if(strcmp(szOpt, ">")==0)
				{
					if(!( pTeam->GetOnlineMemberAmount(1) > nData ))
						return false;
				}
				else if(strcmp(szOpt, "==")==0)
				{
					if(!( pTeam->GetOnlineMemberAmount(1) == nData ))
						return false;
				}
				else
				{
					return false;
				}
				return true;
			}
			else if(strcmp(szField, "inteam")==0)
			{
				return true;
			}
			else if(strcmp(szField, "teamlead")==0)
			{
				return pUser->GetID() == pTeam->GetLeader();
			}
			else if(strcmp(szField, "active")==0)
			{
				return pTeam->CheckActive(pUser);
			}
			return true;
		}
		break;
	case ACTION_TEAM_ACTIONFORMR_DO:
		{
			CHECKF(pUser->GetMap());

			CTeam* pTeam = pUser->GetTeam();
			if(!pTeam)
				return false;
			
			int nData = pAction->GetInt(ACTIONDATA_DATA);
			IF_NOT(nData>0)
				return false;
			
			CUser* pMember = NULL;

			for(int i = 0 ;i<_MAX_TEAMAMOUNT ; i++)
			{
				if(pTeam->m_TeamIn[i].id == ID_NONE)///无队员id不处理
					continue;
				pMember = UserManager()->GetUser(pTeam->m_TeamIn[i].id);
				if(pMember==NULL)///队员为空不处理
					continue;

				if(pMember->GetMap()==NULL)///队员地图为空不处理
					continue;

				if(pUser->GetMapID() != pMember->GetMapID())///发起者与队员不在同一地图不处理
					continue;

// 				if(pUser->GetDistance(pMember->GetPosX(),pMember->GetPosY()) > 24)///发起者与队员距离超值则不处理
// 					continue;

				ProcessAction(nData,pMember,pRole,pItem,pszAccept,true);
			}
			return true;
		}
		break;
	case ACTION_TEAM_SAME_IP://判断队伍中是否有N个人IP相同
		{
			CTeam* pTeam = pUser->GetTeam();
			if(!pTeam)
				return false;

			int nAmount = pAction->GetInt(ACTIONDATA_DATA);

			map<DWORD, USHORT> mapIP;
			int nTeamAmount = pTeam->GetMemberAmount();

			if (nAmount<=0 || nTeamAmount < nAmount)
				return false;

			map<DWORD, USHORT>::iterator it;
			for(int i = 0; i < nTeamAmount; i++)
			{
				CUser* pMem = UserManager()->GetUser(pTeam->GetMemberByIndex(i));
				if (!pMem)
					continue;
				it = mapIP.find(pMem->GetIPAddress());
				if(it!=mapIP.end())
					it->second++;
				else
					mapIP.insert(make_pair(pMem->GetIPAddress(), 1));

			}
			it = mapIP.begin();
			for(; it!=mapIP.end(); it++)
			{
				if( it->second >= nAmount )
					return true;
			}
			return false;
		}
		break;
	case ACTION_TEAM_DIFFENT_IP:
		{
			CTeam* pTeam = pUser->GetTeam();
			if(!pTeam)
				return false;
			
			int nAmount = pAction->GetInt(ACTIONDATA_DATA);
			
			map<DWORD, USHORT> mapIP;
			int nTeamAmount = pTeam->GetMemberAmount();
			
			if (nAmount<=0 || nTeamAmount < nAmount)
				return false;
			
			map<DWORD, USHORT>::iterator it;
			for(int i = 0; i < nTeamAmount; i++)
			{
				CUser* pMem = UserManager()->GetUser(pTeam->GetMemberByIndex(i));
				if (!pMem)
					continue;
				it = mapIP.find(pMem->GetIPAddress());
				if(it!=mapIP.end())
					it->second++;
				else
					mapIP.insert(make_pair(pMem->GetIPAddress(), 1));
				
			}
			if( mapIP.size() >= nAmount )
					return true;
			return false;
		} 
		break;
	case ACTION_TEAM_SAMEIP_SELF://判断队伍中有是否有N个人IP与自己相同
		{
			CTeam* pTeam = pUser->GetTeam();
			if(!pTeam)
				return false;
			
			int nAmount = pAction->GetInt(ACTIONDATA_DATA);
			
			int nTeamAmount = pTeam->GetMemberAmount();
			
			if (nAmount<=0 || nTeamAmount < nAmount)
				return false;
			
			int nResult = 0;
			ULONG uMyIp = pUser->GetIPAddress();
			for(int i = 0; i < nTeamAmount; i++)
			{
				CUser* pMem = UserManager()->GetUser(pTeam->GetMemberByIndex(i));
				if (pMem && pMem->GetID() != pUser->GetID())
				{
					if(pMem->GetIPAddress() == uMyIp)
						nResult++;
				}
			}
			return nResult>=nAmount;
		}
		break;
	case ACTION_TEAM_ACTION_DO:
		{
			CTeam* pTeam = pUser->GetTeam();
			if(!pTeam)
				return false;
			
			int nData = pAction->GetInt(ACTIONDATA_DATA);
			IF_NOT(nData>0)
				return false;
			
			OBJID idTeamLeader = pTeam->GetLeader();
			//队长先
			CUser* pMember = UserManager()->GetUser(idTeamLeader);
			if(pMember)
			{
				if(!this->ProcessAction(nData,pMember,pRole,pItem,pszAccept,true))
				{
//					LogSave("ACTION_TEAM_ACTION_DO idTeamLeader ProcessAction fail!");
					return false;
				}
			}
			else
			{
//				LogSave("ACTION_TEAM_ACTION_DO UserManager()->GetUser(idTeamLeader)==NULL!");
				return false;
			}

			for(int i = 0 ;i<_MAX_TEAMAMOUNT ; i++)
			{
				if(pTeam->m_TeamIn[i].id == ID_NONE)///无队员id不处理
					continue;
				if(pTeam->m_TeamIn[i].id == idTeamLeader)///队长不处理，因为前面处理了
					continue;
				pMember = UserManager()->GetUser(pTeam->m_TeamIn[i].id);
				if(pMember==NULL)///队员为空不处理
					continue;
				
				if(pMember->GetMap()==NULL)///队员地图为空不处理
					continue;
				
// 				if(pUser->GetMapID() != pMember->GetMapID())///发起者与队员不在同一地图不处理
// 					continue;
				
				// 				if(pUser->GetDistance(pMember->GetPosX(),pMember->GetPosY()) > 24)///发起者与队员距离超值则不处理
				// 					continue;
				
				if(!ProcessAction(nData,pMember,pRole,pItem,pszAccept,true))
				{
//					LogSave("ACTION_TEAM_ACTION_DO ProcessAction fail!");
					return false;
				}
			}

			return true;
		}
		break;
	case ACTION_TEAM_CHK_ISLEADER:	
		{
			CTeam* pTeam = pUser->GetTeam();
			if (pTeam && pTeam->GetLeader() == pUser->GetID())
				return true;
		}
		break;
		case ACTION_USER_ADD_TIME_STATUS:
			{ 
				DEBUG_TRY
				int nStatusID = 0;
				int nPower = 0; 
				PARAMBUF	szDate; 
				PARAMBUF	szTime;
				if(sscanf(szParam, "%d %d %s %s", &nStatusID, &nPower, szDate, szTime) < 4)
				{
					LOGERROR("ACTION %u: 错误的参数数量", pAction->GetID());
					break;
				} 
				strcat(szDate, " "); 
				strcat(szDate, szTime);
				COleDateTime startTime;
				startTime.ParseDateTime(szDate);
				COleDateTime nowTime = COleDateTime::GetCurrentTime();
				COleDateTimeSpan span = startTime - nowTime;
				int nMinute = span.GetTotalSeconds();
				if(nMinute > 0)
				{
					CRole::AttachStatus(pUser->QueryRole(), nStatusID, nPower, nMinute);
				}		
				return true;
				DEBUG_CATCH2("CGameAction::ProcesssActionUser ACTION_USER_ADD_TIME_STATUS param=%s", szParam);
			}
			break;
		case ACTION_USER_CHK_IN_RANGE	:	//  [1/27/2008 朱斌]		新增action，可以判断玩家所处的地图坐标
			{
				int nRange = pAction->GetInt(ACTIONDATA_DATA);
				if(nRange <= 0)
				{
					LOGERROR("ACTION %u: 错误的data:%d", pAction->GetID(), nRange);
					break;
				}
				int nMapID = 0;
				int x = 0, y = 0;
				if(sscanf(szParam, "%d %d %d", &nMapID, &x, &y) < 3)
				{
					LOGERROR("ACTION %u: 错误的参数数量", pAction->GetID());
					break;
				}
				
				CGameMap* pMap = pUser->GetMap();
				if(pMap == NULL)
					break;

				if(pMap->GetID() != nMapID)
					break;

				if(pMap->Distance(pUser->GetPosX(), pUser->GetPosY(), x, y) <= nRange)
					return true;
			}
			break;
	default:
		LOGERROR("不支持的ACTION[%u]类型[%u]", pAction->GetKey(), pAction->GetInt(ACTIONDATA_TYPE));
	}

	return false;
}
//////////////////////////////////////////////////////////////////////
bool CGameAction::ProcessActionMagic(CActionData* pAction, LPCTSTR szParam, CUser* pUser, IRole* pRole, CItem* pItem, LPCTSTR pazAccept)
{
	CHECKF(pAction && pUser);

	switch(pAction->GetInt(ACTIONDATA_TYPE))
	{
	case ACTION_DUBLEEXP_FREE:
		{
			if (pUser)
			{
				if(!pUser->CheckFreezeDoubleExp(pAction->GetInt(ACTIONDATA_DATA)))
					return pUser->RestoreDoubleExp(pAction->GetInt(ACTIONDATA_DATA));
				return false;
			}
		}
		break;
	case ACTION_DUBLEEXP_LOCK:
		{
			if (pUser)
			{
				if(pUser->CheckFreezeDoubleExp(pAction->GetInt(ACTIONDATA_DATA)))
					return pUser->FreezeDoubleExp(pAction->GetInt(ACTIONDATA_DATA));
				return false;
			}
		}
		break;
	case ACTION_DUBLEEXP_CHECK:
		{
			if (pUser)
			{
				return pUser->CheckFreezeDoubleExp(pAction->GetInt(ACTIONDATA_DATA));
			}
		}
		break;
	case ACTION_MAGIC_ATTACHSTATUS:
		{
			if (pUser)
			{
				int nStatus, nPower, nSecs, nTimes = 0,nMap = 0;
				int nParmCount = 0;
				nParmCount = sscanf(szParam, "%d %d %d %d %u", &nStatus, &nPower, &nSecs, &nTimes,&nMap);
				
				if(nParmCount < 4)
					return false;
				else if(nParmCount == 4)
					nMap = 0;

				if(pAction->GetInt(ACTIONDATA_DATA) == 0 )
				{

					if(nStatus != STATUS_GETEXPUP_PERCENT && nStatus != STATUS_GETEXPUP_PERCENT_WT && nStatus != STATUS_GETEXPUP_PERCENT_SYNINS)
					{
						IStatus* pStatus = pUser->QueryStatus(nStatus);
						//if(pStatus && pStatus->GetSec() >= nSecs)
						int allTimes = max(1,nTimes) * nSecs;
						if(pStatus)
						{
							if(pStatus->GetPower() > nPower)
							{
								pUser->SendSysMsg("你已经拥有更强大的同类状态");
								return false;
							}
							else if((pStatus->GetSecLeft() >= allTimes) && (pStatus->GetPower() == nPower))//goto 改为取剩余时间，这样可以更新buf时间
							{
								pUser->SendSysMsg("你已经拥有更强大的同类状态");
								return false;
							}
						}
						
					}
				

					if (CRole::AttachStatus(pUser, nStatus, nPower, nSecs, nTimes,0,nMap))
					{
						return true;
					}
				}
				else
				{
					if(pUser->m_pWarPet)
					{
						IStatus* pStatus = pUser->m_pWarPet->QueryStatus(nStatus);
						//if(pStatus && pStatus->GetSec() >= nSecs)
						if(pStatus && pStatus->GetSecLeft() >= nSecs)//goto 改为取剩余时间，这样可以更新buf时间
						{
							return true;
						}
						
						
						if (CRole::AttachStatus(pUser->m_pWarPet, nStatus, nPower, nSecs, nTimes,0,nMap))
						{
							return true;
						}
					}
				}
			}
		}
		break;
	case	ACTION_CHECK_MAGICANDLEV:
		{
			int nType = pAction->GetInt(ACTIONDATA_DATA);
			int nLevel = 0;
			sscanf(szParam, "%u", &nLevel);
			IMagicData* pMagic = pUser->QueryMagic(nType);
			if(!pMagic)
				return false;
			if(pMagic->GetMagicLevel() >= nLevel)
				return true;
			return false;
		}
		break;
	case ACTION_CHECK_BUF:
		{
			int nStatus, nPower = 0;
			if (2 != sscanf(szParam, "%d %d ", &nStatus, &nPower))
				return false;
			if( pUser )
			{
				IStatus* pStatus = pUser->QueryStatus(nStatus);
				if( pStatus )
				{
					if(nPower == 0)
						return true;
					if( pStatus->GetPower() == nPower )
						return true;
				}
			}
			return false;
		}
		break;
	case ACTION_DETACH_BUF:
		{
			int nStatus, nPower = 0;
			if (2 != sscanf(szParam, "%d %d ", &nStatus, &nPower))
				return false;
			
			if( pUser )
			{
				IStatus* pStatus = pUser->QueryStatus(nStatus);
				if( pStatus )
				{
					if( (nPower == 0) || (pStatus->GetPower() == nPower) )
					{
						CRole::DetachStatus( pUser->QueryRole(),nStatus );
						return true;
					}
				}
			}
			return false;
		}
		break;
	default:
	break;
	}

	return false;
}
