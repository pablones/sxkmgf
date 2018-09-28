// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__2DEF01BC_6018_41FB_9A48_4D2196F1C7C1__INCLUDED_)
#define AFX_STDAFX_H__2DEF01BC_6018_41FB_9A48_4D2196F1C7C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows XP 或更高版本的功能。
#define WINVER 0x0501		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0510 // 将它更改为适合 Windows Me 或更高版本的相应值。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 6.0 或更高版本的功能。
#define _WIN32_IE 0x0500	// 将此值更改为相应的值，以适用于 IE 的其他版本。值。
#endif

#define _WINSOCKAPI_		// 阻止加载winsock2.h

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
// #include "vld.h"

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//#define	FD_SETSIZE			600		//??? 当客户端LOGIN连接数很大时，应增加该值
#define	FD_SETSIZE			1500		//20070523修罗

#include <winsock2.h>		// MFC socket extensions	2.0版

#include "hInc.h"

#define MFC_VER_4
#define INDEX           register int
#define WINDEX          register WORD
#define LINDEX          register long
#define DWINDEX         register DWORD
typedef signed  char    INT8  ;
typedef unsigned char   UINT8 ;
typedef signed  short   INT16 ;
typedef unsigned short  UINT16;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2DEF01BC_6018_41FB_9A48_4D2196F1C7C1__INCLUDED_)
