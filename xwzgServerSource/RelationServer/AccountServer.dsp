# Microsoft Developer Studio Project File - Name="AccountServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=AccountServer - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AccountServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AccountServer.mak" CFG="AccountServer - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AccountServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "AccountServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/AccountServer", BAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AccountServer - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../bin/Release/AccountServer"
# PROP Intermediate_Dir "../../../../bin/Release/AccountServer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\mysql\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 GameBaseCode.lib libmysql.lib winmm.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"D:\work\RServer\ReServer.exe" /libpath:"../../../../bin/Releaselib/AccServer/" /libpath:"..\..\..\..\mysql\lib\opt"

!ELSEIF  "$(CFG)" == "AccountServer - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AccountServer___Win32_Debug"
# PROP BASE Intermediate_Dir "AccountServer___Win32_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Bin\Debug\RServer"
# PROP Intermediate_Dir "..\..\..\..\Bin\Debug\RServer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /I "..\..\..\..\mysql\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Winmm.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 GameBaseCode_d.lib libmysql.lib winmm.lib /nologo /subsystem:windows /incremental:yes /debug /machine:I386 /out:"F:\StyleDev\御剑江湖\ServerRelease\RServer\Relationserver_d.exe" /libpath:"bin/Debuglib/AccServer/" /libpath:"..\..\..\..\mysql\lib\debug"

!ENDIF 

# Begin Target

# Name "AccountServer - Win32 Release"
# Name "AccountServer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AccountServer.cpp
# End Source File
# Begin Source File

SOURCE=.\AccountServer.rc
# End Source File
# Begin Source File

SOURCE=.\AccountServerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataInspectManage.cpp
# End Source File
# Begin Source File

SOURCE=.\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\IniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ListenSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\logfile.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginThread.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgAward.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgBarAccount.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgFee.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgFriend.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgMainServer.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgNewPlayerExam.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgQueryFee.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgRelay.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgSchoolMember.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgStopAccount.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgTalk.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgVAS.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgWeaponSoul.cpp
# End Source File
# Begin Source File

SOURCE=.\MyHeap.cpp
# End Source File
# Begin Source File

SOURCE=.\NetStringPacker.cpp
# End Source File
# Begin Source File

SOURCE=.\OnlineTable.cpp
# End Source File
# Begin Source File

SOURCE=.\PointThread.cpp
# End Source File
# Begin Source File

SOURCE=.\RC5_321216.cpp
# End Source File
# Begin Source File

SOURCE=.\RoleManager.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\SupplyVASService.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadBase.cpp
# End Source File
# Begin Source File

SOURCE=.\typedef.cpp
# End Source File
# Begin Source File

SOURCE=.\VASDBManager.cpp
# End Source File
# Begin Source File

SOURCE=.\VASSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\VASStringPacket.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AccountServer.h
# End Source File
# Begin Source File

SOURCE=.\AccountServerDlg.h
# End Source File
# Begin Source File

SOURCE=.\AllHeads.h
# End Source File
# Begin Source File

SOURCE=.\DataInspectManage.h
# End Source File
# Begin Source File

SOURCE=.\EncryptServer.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\hInc.h
# End Source File
# Begin Source File

SOURCE=.\IniFile.h
# End Source File
# Begin Source File

SOURCE=.\ListenSocket.h
# End Source File
# Begin Source File

SOURCE=.\logfile.h
# End Source File
# Begin Source File

SOURCE=.\LoginThread.h
# End Source File
# Begin Source File

SOURCE=.\MsgAward.h
# End Source File
# Begin Source File

SOURCE=.\MsgBarAccount.h
# End Source File
# Begin Source File

SOURCE=.\MsgFee.h
# End Source File
# Begin Source File

SOURCE=.\MsgFriend.h
# End Source File
# Begin Source File

SOURCE=.\MsgMainServer.h
# End Source File
# Begin Source File

SOURCE=.\MsgManager.h
# End Source File
# Begin Source File

SOURCE=.\MsgNewPlayerExam.h
# End Source File
# Begin Source File

SOURCE=.\MsgQueryFee.h
# End Source File
# Begin Source File

SOURCE=.\MsgRelay.h
# End Source File
# Begin Source File

SOURCE=.\MsgSchoolMember.h
# End Source File
# Begin Source File

SOURCE=.\MsgStopAccount.h
# End Source File
# Begin Source File

SOURCE=.\MsgTalk.h
# End Source File
# Begin Source File

SOURCE=.\MsgVAS.h
# End Source File
# Begin Source File

SOURCE=.\MsgWeaponSoul.h
# End Source File
# Begin Source File

SOURCE=.\MyHeap.h
# End Source File
# Begin Source File

SOURCE=.\NetStringPacker.h
# End Source File
# Begin Source File

SOURCE=.\OnlineTable.h
# End Source File
# Begin Source File

SOURCE=.\PointThread.h
# End Source File
# Begin Source File

SOURCE=.\RC5_321216.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RoleManager.h
# End Source File
# Begin Source File

SOURCE=.\ServerSocket.h
# End Source File
# Begin Source File

SOURCE=.\ServerSocket.hpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SupplyVASService.h
# End Source File
# Begin Source File

SOURCE=.\T_Index.h
# End Source File
# Begin Source File

SOURCE=.\ThreadBase.h
# End Source File
# Begin Source File

SOURCE=.\VASDBManager.h
# End Source File
# Begin Source File

SOURCE=.\VASSocket.h
# End Source File
# Begin Source File

SOURCE=.\VASStringPacket.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\AccountServer.ico
# End Source File
# Begin Source File

SOURCE=.\res\AccountServer.rc2
# End Source File
# End Group
# Begin Group "import"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Account.cpp
# End Source File
# Begin Source File

SOURCE=.\Account.h
# End Source File
# Begin Source File

SOURCE=.\Define.h
# End Source File
# Begin Source File

SOURCE=.\MsgAccount.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgAccount.h
# End Source File
# Begin Source File

SOURCE=.\MsgConnect.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgConnect.h
# End Source File
# Begin Source File

SOURCE=.\MsgLogin.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgLogin.h
# End Source File
# Begin Source File

SOURCE=.\NetMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\NetMsg.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\account.ini
# End Source File
# Begin Source File

SOURCE=.\config.ini
# End Source File
# Begin Source File

SOURCE=.\isp.ini
# End Source File
# Begin Source File

SOURCE=.\typedef.h
# End Source File
# Begin Source File

SOURCE=".\版本更新日志.txt"
# End Source File
# Begin Source File

SOURCE=".\当前工作.txt"
# End Source File
# Begin Source File

SOURCE=".\设计任务.txt"
# End Source File
# Begin Source File

SOURCE=".\设计日志.txt"
# End Source File
# Begin Source File

SOURCE=".\设计说明.txt"
# End Source File
# End Target
# End Project
