# Microsoft Developer Studio Project File - Name="MsgServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MsgServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MsgServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MsgServer.mak" CFG="MsgServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MsgServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MsgServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "MsgServer - Win32 AshRelease" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/DDServer", XFAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MsgServer - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Bin\Release\MsgServer"
# PROP Intermediate_Dir "..\..\..\..\Bin\Release\MsgServer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\GAMEBASECODEMT" /I "..\common" /I "..\comm" /I "..\..\..\..\luaplus5\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 nafxcw.lib libcmt.lib ws2_32.lib libmysql.lib winmm.lib /nologo /subsystem:windows /pdb:"D:\work\msgServer_t\msg_out\MsgServer_Release.pdb" /debug /machine:I386 /out:"F:\StyleDev\��������\ServerRelease\msg_line1\MsgServer_Release.exe" /libpath:"..\..\..\..\mysql\lib\opt" /libpath:"..\..\..\..\luaplus5\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "MsgServer - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\Bin\Debug\MsgServer"
# PROP Intermediate_Dir "..\..\..\..\Bin\Debug\MsgServer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\mysql\include" /I "..\GameBaseCodeMT" /I "..\GameBaseCodeMT\Source" /I "..\common\BaseSupport" /I "share" /I "..\MapData" /I "..\common" /I "..\comm" /I "..\..\..\..\luaplus5\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "X_DEBUG_MULTITHREAD" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib libmysql.lib winmm.lib nafxcwd.lib libcmtd.lib /nologo /subsystem:windows /pdb:"D:\work\msgServer_t\msg_line1\MsgServer_d.pdb" /debug /machine:I386 /out:"F:\StyleDev\��������\ServerRelease\msg_line1\MsgServer_d.exe" /pdbtype:sept /libpath:"..\..\..\..\mysql\lib\debug" /libpath:"..\..\..\..\luaplus5\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "MsgServer - Win32 AshRelease"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MsgServer___Win32_AshRelease0"
# PROP BASE Intermediate_Dir "MsgServer___Win32_AshRelease0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Bin\AshRelease\MsgServer"
# PROP Intermediate_Dir "..\..\..\..\Bin\AshRelease\MsgServer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\common" /I "..\comm" /I "..\GameBaseCode" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\GAMEBASECODEMT" /I "..\common" /I "..\comm" /I "..\..\..\..\luaplus5\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 nafxcw.lib libcmt.lib ws2_32.lib libmysql.lib winmm.lib /nologo /subsystem:windows /machine:I386 /out:"E:\ʱ��\server\MsgServer.exe"
# ADD LINK32 nafxcw.lib libcmt.lib ws2_32.lib libmysql.lib winmm.lib /nologo /subsystem:windows /machine:I386 /out:"D:\work\msgServer_t\msg_out\MsgServer_ashrelease.exe" /libpath:"..\..\..\..\mysql\lib\opt" /libpath:"..\..\..\..\luaplus5\lib"

!ENDIF 

# Begin Target

# Name "MsgServer - Win32 Release"
# Name "MsgServer - Win32 Debug"
# Name "MsgServer - Win32 AshRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DlgConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\mapgroupkernel\Magic_Process.cpp
# End Source File
# Begin Source File

SOURCE=.\MapGroupThread.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgServer.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgServer.rc
# End Source File
# Begin Source File

SOURCE=.\MsgServerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\mapgroupkernel\Package.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketThread.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\mapgroupkernel\User.cpp
# End Source File
# Begin Source File

SOURCE=.\mapgroupkernel\User_Item.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldThread.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\WORLDKERNEL\NETWORK2\CMsgMainServer.h
# End Source File
# Begin Source File

SOURCE=.\share\define.h
# End Source File
# Begin Source File

SOURCE=.\DlgConfig.h
# End Source File
# Begin Source File

SOURCE=.\share\I_Shell.h
# End Source File
# Begin Source File

SOURCE=.\libInc.h
# End Source File
# Begin Source File

SOURCE=.\MapGroupThread.h
# End Source File
# Begin Source File

SOURCE=.\WORLDKERNEL\NETWORK2\MsgMainServer.h
# End Source File
# Begin Source File

SOURCE=.\MsgServer.h
# End Source File
# Begin Source File

SOURCE=.\MsgServerDlg.h
# End Source File
# Begin Source File

SOURCE=.\share\protocol.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SocketThread.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\WorldThread.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\MsgServer.ico
# End Source File
# Begin Source File

SOURCE=.\res\MsgServer.rc2
# End Source File
# Begin Source File

SOURCE=".\res\��Ϸ.ico"
# End Source File
# End Group
# Begin Source File

SOURCE=.\cq_tables.sql
# End Source File
# Begin Source File

SOURCE=.\cq_taskall.sql
# End Source File
# Begin Source File

SOURCE=.\cq_user.sql
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\shell.ini
# End Source File
# Begin Source File

SOURCE=.\typedef.h
# End Source File
# End Target
# End Project
