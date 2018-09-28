# Microsoft Developer Studio Project File - Name="WorldKernel" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=WorldKernel - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WorldKernel.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WorldKernel.mak" CFG="WorldKernel - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WorldKernel - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "WorldKernel - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "WorldKernel - Win32 AshRelease" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/WorldKernel", QHAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WorldKernel - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\MyBin\Release\NewDataServer\WorldKernel"
# PROP Intermediate_Dir "..\..\..\..\..\MyBin\Release\NewDataServer\WorldKernel"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GX /O2 /I "." /I ".\NetWork2" /I "..\..\common" /I "..\..\comm" /I "..\..\GameBaseCodeMT" /I "..\..\share" /D "ACCOUNT_ENABLE" /D "WORLD_KERNEL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\..\MyBin\Releaselib\server\NewDataServer\WorldKernel\WorldKernel.lib"

!ELSEIF  "$(CFG)" == "WorldKernel - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\MyBin\Debug\NewDataServer\WorldKernel"
# PROP Intermediate_Dir "..\..\..\..\..\MyBin\Debug\NewDataServer\WorldKernel"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I ".\NetWork2" /I "..\..\common" /I "..\..\comm" /I "..\..\GameBaseCodeMT" /I "..\..\share" /D "ACCOUNT_ENABLE" /D "WORLD_KERNEL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\..\MyBin\Debuglib\server\NewDataServer\WorldKernel\WorldKernel_d.lib"

!ELSEIF  "$(CFG)" == "WorldKernel - Win32 AshRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WorldKernel___Win32_AshRelease"
# PROP BASE Intermediate_Dir "WorldKernel___Win32_AshRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\MyBin\AshRelease\NewDataServer\WorldKernel"
# PROP Intermediate_Dir "..\..\..\..\..\MyBin\AshRelease\NewDataServer\WorldKernel"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MT /W3 /GX /O2 /I "." /I "NetWork2" /I "..\..\GameBaseCode" /I "..\..\common" /I "..\..\comm" /I "..\share" /D "ACCOUNT_ENABLE" /D "WORLD_KERNEL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GX /O2 /I "." /I ".\NetWork2" /I "..\..\common" /I "..\..\comm" /I "..\..\GameBaseCodeMT" /I "..\..\share" /D "ACCOUNT_ENABLE" /D "WORLD_KERNEL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\..\MyBin\Lib\NewDataServer\WorldKernel\WorldKernel.lib"
# ADD LIB32 /nologo /out:"..\..\..\..\..\MyBin\AshReleaselib\server\NewDataServer\WorldKernel\WorldKernel_release.lib"

!ENDIF 

# Begin Target

# Name "WorldKernel - Win32 Release"
# Name "WorldKernel - Win32 Debug"
# Name "WorldKernel - Win32 AshRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\WorldKernel.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\WorldKernel.h
# End Source File
# End Group
# Begin Group "NetWork"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NETWORK2\AllMsg.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\NetMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\NetMsg.h
# End Source File
# End Group
# Begin Group "ChatRoom"

# PROP Default_Filter ""
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
