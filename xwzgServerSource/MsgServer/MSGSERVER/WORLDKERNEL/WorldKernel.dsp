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
# PROP Output_Dir "..\..\..\..\..\Bin\Release\WorldKernel"
# PROP Intermediate_Dir "..\..\..\..\..\Bin\Release\WorldKernel"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GX /O2 /I "." /I "NetWork2" /I "..\..\GAMEBASECODEMT" /I "..\..\common" /I "..\..\comm" /I "..\share" /D "ACCOUNT_ENABLE" /D "WORLD_KERNEL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\..\Bin\Releaselib\server\WorldKernel.lib"

!ELSEIF  "$(CFG)" == "WorldKernel - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\Bin\Debug\WorldKernel"
# PROP Intermediate_Dir "..\..\..\..\..\Bin\Debug\WorldKernel"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I ".\NetWork2" /I "..\..\common" /I "..\..\common\BaseSupport" /I "..\..\comm" /I "..\..\GameBaseCodeMT" /I "..\share" /D "ACCOUNT_ENABLE" /D "WORLD_KERNEL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\..\Bin\Debuglib\server\WorldKernel_d.lib"

!ELSEIF  "$(CFG)" == "WorldKernel - Win32 AshRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WorldKernel___Win32_AshRelease"
# PROP BASE Intermediate_Dir "WorldKernel___Win32_AshRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\Bin\AshRelease\WorldKernel"
# PROP Intermediate_Dir "..\..\..\..\..\Bin\AshRelease\WorldKernel"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MT /W3 /GX /O2 /I "." /I "NetWork2" /I "..\..\GameBaseCode" /I "..\..\common" /I "..\..\comm" /I "..\share" /D "ACCOUNT_ENABLE" /D "WORLD_KERNEL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GX /O2 /I "." /I "NetWork2" /I "..\..\GAMEBASECODEMT" /I "..\..\common" /I "..\..\comm" /I "..\share" /D "ACCOUNT_ENABLE" /D "WORLD_KERNEL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\..\Bin\Lib\WorldKernel.lib"
# ADD LIB32 /nologo /out:"..\..\..\..\..\Bin\AshReleaselib\server\WorldKernel_release.lib"

!ENDIF 

# Begin Target

# Name "WorldKernel - Win32 Release"
# Name "WorldKernel - Win32 Debug"
# Name "WorldKernel - Win32 AshRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Arena.cpp
# End Source File
# Begin Source File

SOURCE=.\ArenaManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectUser.cpp
# End Source File
# Begin Source File

SOURCE=.\MapList.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgAward.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgBroadcast.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgEudemonInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgFightTeamAction.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgInfoNpcUserExit.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgIssue.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgItem.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgItemInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgNewCard.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgRelay.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgSchoolMember.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgShop.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgStopAccount.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgSyndicate.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgSynMemberInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgTeamPos.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgVASItem.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgWeaponSoul.cpp
# End Source File
# Begin Source File

SOURCE=.\Shop.cpp
# End Source File
# Begin Source File

SOURCE=.\ShopItem.cpp
# End Source File
# Begin Source File

SOURCE=.\ShopManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ShopVAS.cpp
# End Source File
# Begin Source File

SOURCE=.\SupermanList.cpp
# End Source File
# Begin Source File

SOURCE=.\SynWorldManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SynWorldShell.cpp
# End Source File
# Begin Source File

SOURCE=.\TutorWorld.cpp
# End Source File
# Begin Source File

SOURCE=.\UserList.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldKernel.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Arena.h
# End Source File
# Begin Source File

SOURCE=.\ArenaManager.h
# End Source File
# Begin Source File

SOURCE=.\ClientSocket.h
# End Source File
# Begin Source File

SOURCE=..\share\Cn_DDRes.h
# End Source File
# Begin Source File

SOURCE=.\ConnectUser.h
# End Source File
# Begin Source File

SOURCE=.\EncryptClient.h
# End Source File
# Begin Source File

SOURCE=.\MapList.h
# End Source File
# Begin Source File

SOURCE=.\MessageBoard.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgAward.h
# End Source File
# Begin Source File

SOURCE=.\MsgBroadcast.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgEudemonInfo.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgFightTeamAction.h
# End Source File
# Begin Source File

SOURCE=.\MsgInfoNpcUserExit.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgIssue.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgItem.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgItemInfo.h
# End Source File
# Begin Source File

SOURCE=.\MsgNewCard.h
# End Source File
# Begin Source File

SOURCE=.\MsgRelay.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgSchoolMember.h
# End Source File
# Begin Source File

SOURCE=.\MsgShop.h
# End Source File
# Begin Source File

SOURCE=.\MsgStopAccount.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgSyndicate.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgSynMemberInfo.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgTeamPos.h
# End Source File
# Begin Source File

SOURCE=.\MsgVASItem.h
# End Source File
# Begin Source File

SOURCE=.\MsgWeaponSoul.h
# End Source File
# Begin Source File

SOURCE=.\Shop.h
# End Source File
# Begin Source File

SOURCE=.\ShopItem.h
# End Source File
# Begin Source File

SOURCE=.\ShopManager.h
# End Source File
# Begin Source File

SOURCE=.\ShopVAS.h
# End Source File
# Begin Source File

SOURCE=.\SupermanList.h
# End Source File
# Begin Source File

SOURCE=.\SynWorldManager.h
# End Source File
# Begin Source File

SOURCE=.\SynWorldShell.h
# End Source File
# Begin Source File

SOURCE=.\TutorWorld.h
# End Source File
# Begin Source File

SOURCE=.\UserList.h
# End Source File
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

SOURCE=.\NETWORK2\MsgAccount_A.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgAiNpcInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgAiNpcInfo.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgCentalMonitor.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgCentalMonitor.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgConnect.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgConnect.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgConnect_A.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgDialog.h
# End Source File
# Begin Source File

SOURCE=.\MsgFatigue.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgFatigue.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgFee_A.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgFriend.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgFriend.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgLogin.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgLogin.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgLogin_A.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgMessageBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgMessageBoard.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgName.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgName.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgNewPlayerExam.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgNewPlayerExam.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgQueryFee_A.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgRegister.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgRegister.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgTalk.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgTalk.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgTime.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgTime.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgVAS.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgVAS.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgVasLv1.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgVasLv1.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgWaitQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\MsgWaitQueue.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\NetMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\NetMsg.h
# End Source File
# Begin Source File

SOURCE=.\NETWORK2\NetworkDef.h
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
