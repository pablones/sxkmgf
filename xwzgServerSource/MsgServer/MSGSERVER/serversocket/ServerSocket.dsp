# Microsoft Developer Studio Project File - Name="ServerSocket" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ServerSocket - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ServerSocket.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ServerSocket.mak" CFG="ServerSocket - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ServerSocket - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ServerSocket - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ServerSocket - Win32 AshRelease" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ServerSocket", KHAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ServerSocket - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\Bin\Release\ServerSocket"
# PROP Intermediate_Dir "..\..\..\..\..\Bin\Release\ServerSocket"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GX /O2 /I "..\..\GAMEBASECODEMT" /I "..\..\common" /I "..\..\comm" /I "..\share" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\..\Bin\Releaselib\server\ServerSocket.lib"

!ELSEIF  "$(CFG)" == "ServerSocket - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\Bin\Debug\ServerSocket"
# PROP Intermediate_Dir "..\..\..\..\..\Bin\Debug\ServerSocket"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\common" /I "..\..\common\BaseSupport" /I "..\..\comm" /I "..\share" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\..\Bin\Debuglib\server\ServerSocket_d.lib"

!ELSEIF  "$(CFG)" == "ServerSocket - Win32 AshRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ServerSocket___Win32_AshRelease"
# PROP BASE Intermediate_Dir "ServerSocket___Win32_AshRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\Bin\AshRelease\ServerSocket"
# PROP Intermediate_Dir "..\..\..\..\..\Bin\AshRelease\ServerSocket"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MT /W3 /GX /O2 /I "..\..\GameBaseCode" /I "..\..\common" /I "..\..\comm" /I "..\share" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GX /O2 /I "..\..\GAMEBASECODEMT" /I "..\..\common" /I "..\..\comm" /I "..\share" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\..\Bin\Lib\ServerSocket.lib"
# ADD LIB32 /nologo /out:"..\..\..\..\..\Bin\AshReleaselib\server\ServerSocket_ashrelease.lib"

!ENDIF 

# Begin Target

# Name "ServerSocket - Win32 Release"
# Name "ServerSocket - Win32 Debug"
# Name "ServerSocket - Win32 AshRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Acceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\IOCPServer.cpp
# End Source File
# Begin Source File

SOURCE=.\IoHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\MsWinsockUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\Session.cpp
# End Source File
# Begin Source File

SOURCE=.\SessionList.cpp
# End Source File
# Begin Source File

SOURCE=.\SessionPool.cpp
# End Source File
# Begin Source File

SOURCE=.\SSocket.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Acceptor.h
# End Source File
# Begin Source File

SOURCE=.\CircuitQueue.h
# End Source File
# Begin Source File

SOURCE=.\EncryptServer.h
# End Source File
# Begin Source File

SOURCE=.\IOCPServer.h
# End Source File
# Begin Source File

SOURCE=.\IoHandler.h
# End Source File
# Begin Source File

SOURCE=.\MsWinsockUtil.h
# End Source File
# Begin Source File

SOURCE=.\NetBase.h
# End Source File
# Begin Source File

SOURCE=.\RecvBuffer.h
# End Source File
# Begin Source File

SOURCE=.\SendBuffer.h
# End Source File
# Begin Source File

SOURCE=.\ServerSocket.h
# End Source File
# Begin Source File

SOURCE=.\Session.h
# End Source File
# Begin Source File

SOURCE=.\SessionList.h
# End Source File
# Begin Source File

SOURCE=.\SessionPool.h
# End Source File
# Begin Source File

SOURCE=.\SSocket.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\share\define.h
# End Source File
# Begin Source File

SOURCE=..\share\I_Shell.h
# End Source File
# Begin Source File

SOURCE=..\share\protocol.h
# End Source File
# End Target
# End Project
