// LuaScriptMachine.h: interface for the CLuaScriptMachine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LUASCRIPTMACHINE_H__BABC696D_9DFB_4EEF_BD87_3A5993DC9B35__INCLUDED_)
#define AFX_LUASCRIPTMACHINE_H__BABC696D_9DFB_4EEF_BD87_3A5993DC9B35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define LUAPLUS_LIB

#include "LuaPlus.h"
using namespace LuaPlus;

//template <typename Callee, typename Func>
//void RegisterObjectDirect(LuaStateOwner& state,const char* objname,const char* classname,const char* funname, Callee* callee, Func func);

///lua�ű������,���ӵļһ�,Ϊ������Ϸ���Ҹ��Ұ�.
class CLuaScriptMachine  
{
public:
	CLuaScriptMachine();
	CLuaScriptMachine(PROCESS_ID idProcess);
	virtual ~CLuaScriptMachine();
	bool InitScript(CUser *pScrUser,int nContext=0,IRole* pRole=NULL,CItem* pItem=NULL);///�ҽӶ����еĺ���
	bool InitMapOntimerScript(int nContext=0);
//	bool DeInit();///���ע��
	bool RunScriptFile(const char* szFileName);///���нű��ļ�
	bool  RunScriptFun(const char* szFileName,const char* szFunName);///���нű��ļ��еĺ���
	bool RunScript(const char* txtScript);///���нű�����
	LPCSTR GetLastError(){return m_strLastError.c_str();}
	BOOL  IsHasError(){return  m_strLastError.size();}
	LuaStateOwner& GetState(){return m_LuaState;}
	bool LoadScriptFile(const char* szFileName);///���нű��ļ�
	bool InitVar(int nIndex); 
//	LuaFunction GetFunObj(const char* szFileName)
	template <typename Callee, typename Func>
	void RegisterObjectDirect(const char* objname,const char* classname,const char* funname, Callee* callee, Func func)
	{
		LuaObject mt=m_LuaState->GetGlobals().GetByName(classname);
		if(mt.IsNil())
		{
			mt= m_LuaState->GetGlobals().CreateTable(classname);
			mt.SetMetaTable(mt);
			mt.SetObject("__index", mt);
		}
		mt.RegisterObjectDirect(funname, (Callee*)0, func);
		///����ű��еĶ���
		LuaObject luaObject=m_LuaState->GetGlobals().GetByName(objname);
		if(luaObject.IsNil())
		{
			luaObject.AssignNewTable(m_LuaState);
			luaObject.SetMetaTable(m_LuaState->GetGlobal(classname));
		}
		luaObject.SetLightUserData("__object",callee);
		m_LuaState->GetGlobals().SetObject(objname, luaObject);
//		::RegisterObjectDirect(m_LuaState,objname,classname,funname,callee,func);
	}
protected:
	bool InitScript(CUser *pScrUser,int nContext=0,CUser *pDstUser=NULL,CNpc *pNpc=NULL,CMonster* pMonster=NULL,CItem* pItem=NULL);///�ҽӶ����еĺ���
private:
	BOOL HookMapMgr();///�ҵ�ͼ������
	BOOL HookNpcMgr();///��NPC������
	void HookVarMgr();///�ұ���������
	LuaStateOwner m_LuaState;
	PROCESS_ID	  m_idProcess;
	string m_strLastError;
};

#endif // !defined(AFX_LUASCRIPTMACHINE_H__BABC696D_9DFB_4EEF_BD87_3A5993DC9B35__INCLUDED_)
