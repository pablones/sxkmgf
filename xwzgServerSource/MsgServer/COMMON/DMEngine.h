// DMEngine.h: interface for the CDMEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DMENGINE_H__3CB4DF96_A2F8_4FEC_9CA9_076B044EACCF__INCLUDED_)
#define AFX_DMENGINE_H__3CB4DF96_A2F8_4FEC_9CA9_076B044EACCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <eh.h> 
#include <windows.h>

#pragma warning(disable: 4200)	//nonstandard extension used : zero-sized array in struct/union
#pragma optimize( "y", off )	// 保证CALL FRAME不会被优化掉


void trans_func( unsigned int u, EXCEPTION_POINTERS* pExp );
BOOL ShouldUseSETranslator();
BOOL DumpXcptCallStackV(PEXCEPTION_POINTERS pXcpt=NULL,	const char* fmt=NULL, ... );
BOOL DumpCallStackInternal( PCONTEXT		pCxt, const char*	pszTxt="",PEXCEPTION_POINTERS pXcpt=NULL);
BOOL TraceStackEx(PCONTEXT pCxtRaw=NULL,PEXCEPTION_POINTERS pXcpt=NULL);
BOOL TraceStackExInteral( CONTEXT * pctx, PEXCEPTION_POINTERS pXcpt);
void DumpCallStackSimple(PCONTEXT pctx,PEXCEPTION_POINTERS pXcpt);

class CDMEngine  
{
public:
	CDMEngine();
	virtual ~CDMEngine();

private:
	_se_translator_function OldFanc;
};

#endif // !defined(AFX_DMENGINE_H__3CB4DF96_A2F8_4FEC_9CA9_076B044EACCF__INCLUDED_)
