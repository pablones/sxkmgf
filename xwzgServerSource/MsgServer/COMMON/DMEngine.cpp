// DMEngine.cpp: implementation of the CDMEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "DMEngine.h"
#include <stdio.h>
#include <dbghelp.h>
#include "StackWalker.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


BOOL __stdcall My_ReadProcessMemory (HANDLE, LPCVOID lpBaseAddress, LPVOID lpBuffer, DWORD nSize, LPDWORD lpNumberOfBytesRead)
{
    return ReadProcessMemory(GetCurrentProcess(), lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
}

#pragma comment( lib, "dbghelp.lib" )
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void trans_func( unsigned int u, EXCEPTION_POINTERS* pExp ) 
{
	DumpXcptCallStackV(pExp);
} 

BOOL DumpXcptCallStackV(PEXCEPTION_POINTERS pXcpt,	const char* fmt, ... )
{
	try
	{
		char szBuf[16*1024]={0,};
		if(fmt)
		{
			try		
			{
				int iRet = 0;
				va_list arglist=NULL;
				va_start(arglist, fmt);
				
				iRet=_vsnprintf( szBuf,sizeof(szBuf)-1, fmt, arglist );
				
				va_end(arglist);
			}	
			catch(...)
			{
			}	
		}

		if(pXcpt&&pXcpt->ExceptionRecord)
		{
			char szMod[1024]={0,};
			MEMORY_BASIC_INFORMATION mbi={0,};
			if(pXcpt->ExceptionRecord->ExceptionAddress)
			{
				VirtualQuery(pXcpt->ExceptionRecord->ExceptionAddress,&mbi,sizeof(mbi));
				GetModuleFileName((HMODULE)mbi.AllocationBase,szMod,sizeof(szMod)-1);
			}
			
			char szTemp[1024]="";
			sprintf(szTemp,"DumpXcptCallStackV ExceptionCode:0x%x  NumberParameters:%u ExceptionAddress:0x%x(BaseAt:0x%x szMod:%s)\n",
				pXcpt->ExceptionRecord->ExceptionCode,
				pXcpt->ExceptionRecord->NumberParameters,
				pXcpt->ExceptionRecord->ExceptionAddress,
				mbi.AllocationBase,
				szMod);		
			::OutputDebugString(szTemp);
		}

		if(pXcpt->ContextRecord)
		{
			DumpCallStackInternal(pXcpt->ContextRecord,"",pXcpt);
		}
	}catch(...)
	{

	}
	return TRUE;
}

BOOL DumpCallStackInternal( PCONTEXT	pCxt, const char*	pszTxt,PEXCEPTION_POINTERS pXcpt)
{
	char szTemp[1024]="";
	sprintf(szTemp,"DumpCallStackInternal pszLogName:%s pszTxt:%s begin. pCxt:0x%x \n","log",pszTxt,pCxt);
	::OutputDebugString(szTemp);
	


	TraceStackEx(pCxt,pXcpt);
	
	sprintf(szTemp,"DumpCallStackInternal end. pCxt:0x%x \n",pCxt);
	::OutputDebugString(szTemp);
	
	return TRUE;
}

BOOL TraceStackEx(PCONTEXT pCxtRaw,PEXCEPTION_POINTERS pXcpt)
{
	try
	{
		PCONTEXT pCxt=pCxtRaw;
		CONTEXT context={0,};
		
		if(NULL==pCxt)
		{
			context.ContextFlags = CONTEXT_FULL;
			if (!::GetThreadContext(GetCurrentThread(), &context))
			{
				char szTemp[1024]="";
				sprintf(szTemp,"critical exection:can't get current thread context.lasterror:%u \n",GetLastError());
				::OutputDebugString(szTemp);
				return FALSE;
			}
			pCxt=&context;
		}
		
		////
		TraceStackExInteral(pCxt,pXcpt);
	}
	catch(...)
	{
	}
	
	return TRUE;
}

BOOL TraceStackExInteral( CONTEXT * pctx, PEXCEPTION_POINTERS pXcpt)
{
	DumpCallStackSimple(pctx,pXcpt);

	//MyStackWalker sw;
	//sw.ShowCallstack(GetCurrentProcess(),pctx);

	try
	{
		////
		SYSTEMTIME stNow={0,};
		GetLocalTime(&stNow);		
		
		char szDumpFilePath[1024]={0,};
		CreateDirectory(TEXT(".\\dump"), NULL);
		sprintf(szDumpFilePath, ".\\dump\\%4d年%02d月%02d日%02d时%02d分%02d秒.dmp", stNow.wYear, stNow.wMonth, stNow.wDay, stNow.wHour, stNow.wMinute, stNow.wSecond);
		

		HANDLE hFile = ::CreateFile(TEXT(szDumpFilePath), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		
		if (hFile != INVALID_HANDLE_VALUE)
		{
			MINIDUMP_EXCEPTION_INFORMATION ExInfo;
			
			ExInfo.ThreadId = ::GetCurrentThreadId();
			ExInfo.ExceptionPointers = pXcpt;
			ExInfo.ClientPointers = NULL;
			
			// write the dump
			BOOL bOK = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );
			/*ret = EXCEPTION_EXECUTE_HANDLER;*/
			::CloseHandle(hFile);
		}
	}
	catch(...)
	{
	}

	return TRUE;
}

void DumpCallStackSimple(PCONTEXT pctx,PEXCEPTION_POINTERS pXcpt)
{	
	typedef struct STACK
	{
		STACK *	Ebp;
		PBYTE	Ret_Addr;
	} STACK, * PSTACK;
	
	PSTACK	Ebp=NULL;
	
	try
	{
		char szTemp[1024]="";
		LOGWARNING("-----DumpCallStackSimple begin pXcpt:0x%08x(ExceptionCode:0x%08x ExceptionFlags:0x%08x ExceptionAddress:0x%08x NumberParameters:%u). \n",
			pXcpt,
			pXcpt&&pXcpt->ExceptionRecord?pXcpt->ExceptionRecord->ExceptionCode:0,
			pXcpt&&pXcpt->ExceptionRecord?pXcpt->ExceptionRecord->ExceptionFlags:0,
			pXcpt&&pXcpt->ExceptionRecord?pXcpt->ExceptionRecord->ExceptionAddress:0,
			pXcpt&&pXcpt->ExceptionRecord?pXcpt->ExceptionRecord->NumberParameters:0
			);
		//::OutputDebugString(szTemp);
		
		
		Ebp = (PSTACK)&pctx - 1;
		
		for(long l=0;l<100;l++)
		{
			if(IsBadReadPtr(Ebp, sizeof(STACK)) || IsBadCodePtr(FARPROC(Ebp->Ret_Addr)))
				break;

			char szFilePath[1024]={0,};
			MEMORY_BASIC_INFORMATION mbi={0,};
			
			if(VirtualQuery(Ebp->Ret_Addr,&mbi,sizeof(mbi)))
			{
				GetModuleFileName((HMODULE)mbi.AllocationBase,szFilePath,sizeof(szFilePath)-1);
			};

// 			IMAGEHLP_LINE img_line={0,};			
// 			char szFileLine[1023]={0,};
// 			img_line.SizeOfStruct = sizeof(IMAGEHLP_LINE);			
// 			img_line.FileName=szFileLine;
// 			DWORD dwDisplacementOutput = 0;
// 			int n = 0;
// 			while (!SymGetLineFromAddr (GetCurrentProcess(), (DWORD)Ebp.Ret_Addr - n, &dwDisplacementOutput, &img_line))
// 			{        
// 				if (100 == ++n)
// 					break;        
// 			}
// 			
// 			strcpy(szFileLine,img_line.FileName);
// 			DWORD dwLine = img_line.LineNumber;
			
						
			char* pchToken=strrchr(szFilePath,'\\');
			
			//LOGWARNING("-----[%02u] 0x%08x BaseAt:0x%08x Mod:%-32s.FullPath:%s.",l,Ebp->Ret_Addr,mbi.AllocationBase,pchToken?pchToken:"",szFilePath);
			//::OutputDebugString(szTemp);
			
			Ebp = Ebp->Ebp;
		}
		
		LOGWARNING("-----DumpCallStackSimple end pXcpt:0x%x.",pXcpt);
		//::OutputDebugString(szTemp);
		
	}
	catch(...)
	{
	}
}

BOOL ShouldUseSETranslator()
{
	static BOOL bCalledFirstTime=TRUE;
	
	static BOOL bSouldUseSeTranslator=TRUE;
	
	if(!bCalledFirstTime)
	{
		return bSouldUseSeTranslator;
	}
	

	bCalledFirstTime=FALSE;
	
	
	//NAME_SPACE(tq) DebugLogSaveV("bSouldUseSeTranslator:%u",bSouldUseSeTranslator);
	
	return bSouldUseSeTranslator;
}

CDMEngine::CDMEngine()
{
	if(ShouldUseSETranslator())
	{
		OldFanc = _set_se_translator( trans_func );
	}
}

CDMEngine::~CDMEngine()
{
	if(ShouldUseSETranslator())
	{
		if(OldFanc)
		{
			_set_se_translator(OldFanc); 
			OldFanc=NULL;
		}
	}
}
