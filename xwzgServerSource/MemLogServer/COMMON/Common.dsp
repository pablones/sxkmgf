# Microsoft Developer Studio Project File - Name="Common" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Common - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Common.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Common.mak" CFG="Common - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Common - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Common - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Common - Win32 AshRelease" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Common", TGAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Common - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\MyBin\Release\NewDataServer\Common"
# PROP Intermediate_Dir "..\..\..\..\..\MyBin\Release\NewDataServer\Common"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /I "..\GameBaseCodeMT" /I "..\share" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\..\MyBin\Releaselib\server\NewDataServer\Common\Common.lib"

!ELSEIF  "$(CFG)" == "Common - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\MyBin\Debug\NewDataServer\Common"
# PROP Intermediate_Dir "..\..\..\..\..\MyBin\Debug\NewDataServer\Common"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "..\GameBaseCodeMT" /I "..\share" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\..\MyBin\Debuglib\server\NewDataServer\Common\Common_d.lib"

!ELSEIF  "$(CFG)" == "Common - Win32 AshRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Common___Win32_AshRelease"
# PROP BASE Intermediate_Dir "Common___Win32_AshRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\MyBin\AshRelease\NewDataServer\Common"
# PROP Intermediate_Dir "..\..\..\..\..\MyBin\AshRelease\NewDataServer\Common"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /I "..\GameBaseCodeMT" /I "..\share" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Common.lib"
# ADD LIB32 /nologo /out:"..\..\..\..\..\MyBin\AshReleaselib\server\NewDataServer\Common\Common_ashrelease.lib"

!ENDIF 

# Begin Target

# Name "Common - Win32 Release"
# Name "Common - Win32 Debug"
# Name "Common - Win32 AshRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BaseFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\common.cpp
# End Source File
# Begin Source File

SOURCE=.\Console.cpp
# End Source File
# Begin Source File

SOURCE=.\IniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\logfile.cpp
# End Source File
# Begin Source File

SOURCE=.\MyCriticalSection.cpp
# End Source File
# Begin Source File

SOURCE=.\MyHeap.cpp
# End Source File
# Begin Source File

SOURCE=.\NetStringPacker.cpp
# End Source File
# Begin Source File

SOURCE=.\StackWalker.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeOut.cpp
# End Source File
# Begin Source File

SOURCE=.\VarType.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Array.h
# End Source File
# Begin Source File

SOURCE=.\AutoLink.h
# End Source File
# Begin Source File

SOURCE=.\AutoLinkSet.h
# End Source File
# Begin Source File

SOURCE=.\AutoPtr.h
# End Source File
# Begin Source File

SOURCE=.\BaseFunc.h
# End Source File
# Begin Source File

SOURCE=.\basetype.h
# End Source File
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=.\Console.h
# End Source File
# Begin Source File

SOURCE=.\En_Res.h
# End Source File
# Begin Source File

SOURCE=.\GameObjArray.h
# End Source File
# Begin Source File

SOURCE=.\GameObjSet.h
# End Source File
# Begin Source File

SOURCE=.\Index.h
# End Source File
# Begin Source File

SOURCE=.\IniFile.h
# End Source File
# Begin Source File

SOURCE=.\logfile.h
# End Source File
# Begin Source File

SOURCE=.\MyArray.h
# End Source File
# Begin Source File

SOURCE=.\mycom.h
# End Source File
# Begin Source File

SOURCE=.\MyCriticalSection.h
# End Source File
# Begin Source File

SOURCE=.\MyHeap.h
# End Source File
# Begin Source File

SOURCE=.\MyStack.h
# End Source File
# Begin Source File

SOURCE=.\MyString.h
# End Source File
# Begin Source File

SOURCE=.\NetStringPacker.h
# End Source File
# Begin Source File

SOURCE=.\StackWalker.h
# End Source File
# Begin Source File

SOURCE=.\T_MyQueue.h
# End Source File
# Begin Source File

SOURCE=.\T_SafePtr.h
# End Source File
# Begin Source File

SOURCE=.\T_SingleMap.h
# End Source File
# Begin Source File

SOURCE=.\ThreadBase.h
# End Source File
# Begin Source File

SOURCE=.\TickOver.h
# End Source File
# Begin Source File

SOURCE=.\TimeOut.h
# End Source File
# Begin Source File

SOURCE=.\VarType.h
# End Source File
# End Group
# End Target
# End Project
