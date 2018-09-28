# Microsoft Developer Studio Project File - Name="Comm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Comm - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Comm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Comm.mak" CFG="Comm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Comm - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Comm - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Comm - Win32 AshRelease" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Comm", WCAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Comm - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\Bin\Release\Comm"
# PROP Intermediate_Dir "..\..\..\..\..\Bin\Release\Comm"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\common" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\..\Bin\Releaselib\server\Comm.lib"

!ELSEIF  "$(CFG)" == "Comm - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\Bin\Debug\Comm"
# PROP Intermediate_Dir "..\..\..\..\..\Bin\Debug\Comm"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\common" /I "." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\..\Bin\Debuglib\server\Comm_d.lib"

!ELSEIF  "$(CFG)" == "Comm - Win32 AshRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Comm___Win32_AshRelease"
# PROP BASE Intermediate_Dir "Comm___Win32_AshRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\Bin\AshRelease\Comm"
# PROP Intermediate_Dir "..\..\..\..\..\Bin\AshRelease\Comm"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\common" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\common" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Comm.lib"
# ADD LIB32 /nologo /out:"..\..\..\..\..\Bin\AshReleaselib\server\Comm_ashrelease.lib"

!ENDIF 

# Begin Target

# Name "Comm - Win32 Release"
# Name "Comm - Win32 Debug"
# Name "Comm - Win32 AshRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\source\InternetPort.cpp
# End Source File
# Begin Source File

SOURCE=.\source\messageport.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\I_MessagePort.h
# End Source File
# Begin Source File

SOURCE=.\InternetPort.h
# End Source File
# Begin Source File

SOURCE=.\MessagePort.h
# End Source File
# End Group
# Begin Source File

SOURCE=".\–ﬁ∏ƒ»’÷æ.txt"
# End Source File
# End Target
# End Project
