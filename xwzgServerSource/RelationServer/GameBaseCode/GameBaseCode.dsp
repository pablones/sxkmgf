# Microsoft Developer Studio Project File - Name="GameBaseCode" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=GameBaseCode - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GameBaseCode.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GameBaseCode.mak" CFG="GameBaseCode - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GameBaseCode - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "GameBaseCode - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/GameBaseCode", XFBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GameBaseCode - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "bin/Release/ACCGameBaseCode"
# PROP Intermediate_Dir "bin/Release/ACCGameBaseCode"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /I "..\..\..\..\mysql\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../bin/Releaselib/AccServer/GameBaseCode.lib"

!ELSEIF  "$(CFG)" == "GameBaseCode - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin/Debug/ACCGameBaseCode"
# PROP Intermediate_Dir "bin/Debug/ACCGameBaseCode"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\..\..\..\mysql\include" /D "_COLOR_ENABLE" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../bin/Debuglib/AccServer/GameBaseCode_d.lib"

!ENDIF 

# Begin Target

# Name "GameBaseCode - Win32 Release"
# Name "GameBaseCode - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "DataBase"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MyDatabase.cpp
# End Source File
# Begin Source File

SOURCE=.\mydatabase.h
# End Source File
# Begin Source File

SOURCE=.\MyField.cpp
# End Source File
# Begin Source File

SOURCE=.\MyField.h
# End Source File
# Begin Source File

SOURCE=.\MyRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\myrecord.h
# End Source File
# Begin Source File

SOURCE=.\MyRecordset.cpp
# End Source File
# Begin Source File

SOURCE=.\myrecordset.h
# End Source File
# End Group
# Begin Group "MsgBase"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MsgPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgPacket.h
# End Source File
# Begin Source File

SOURCE=.\NetStringPacker.cpp
# End Source File
# Begin Source File

SOURCE=.\NetStringPacker.h
# End Source File
# End Group
# Begin Group "MemBase"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PrcHeap.cpp
# End Source File
# Begin Source File

SOURCE=.\PrcHeap.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\BaseFunc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BaseFunc.h
# End Source File
# Begin Source File

SOURCE=.\I_mydb.h
# End Source File
# Begin Source File

SOURCE=.\mydb.h
# End Source File
# End Group
# Begin Source File

SOURCE=".\–ﬁ∏ƒ»’÷æ.txt"
# End Source File
# End Target
# End Project
