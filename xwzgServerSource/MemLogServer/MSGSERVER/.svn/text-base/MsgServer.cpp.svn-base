// MsgServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MsgServer.h"
#include "MsgServerDlg.h"

#include <dbgHelp.h>
#pragma comment(lib, "DbgHelp.lib")

/////////////////////////////////////////////////////////////////////////////
// CMsgServerApp

BEGIN_MESSAGE_MAP(CMsgServerApp, CWinApp)
	//{{AFX_MSG_MAP(CMsgServerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgServerApp construction

CMsgServerApp::CMsgServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMsgServerApp object

CMsgServerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMsgServerApp initialization

LONG WINAPI TopLevelFilter( struct _EXCEPTION_POINTERS *pExceptionInfo )
{
    LONG ret = EXCEPTION_CONTINUE_SEARCH;
	
    time_t  nowtime;
    time(&nowtime);
    struct tm *pTime = localtime(&nowtime);
    char szFile[256] = {0};
	CreateDirectory(TEXT(".\\dump"), NULL);
    sprintf(szFile, ".\\dump\\%4d年%02d月%02d日%02d时%02d分%02d秒.dmp", pTime->tm_year+1900, pTime->tm_mon+1, pTime->tm_mday, pTime->tm_hour, pTime->tm_min, pTime->tm_sec);
	
    HANDLE hFile = ::CreateFile(TEXT(szFile), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
    if (hFile != INVALID_HANDLE_VALUE)
    {
        MINIDUMP_EXCEPTION_INFORMATION ExInfo;
		
        ExInfo.ThreadId = ::GetCurrentThreadId();
        ExInfo.ExceptionPointers = pExceptionInfo;
        ExInfo.ClientPointers = NULL;
		
        // write the dump
        BOOL bOK = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );
        /*ret = EXCEPTION_EXECUTE_HANDLER;*/
        ::CloseHandle(hFile);
    }
    return ret;
}

BOOL CMsgServerApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	//设置dump
	::SetUnhandledExceptionFilter(TopLevelFilter);
	
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CMsgServerDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
