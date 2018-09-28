# Microsoft Developer Studio Project File - Name="MapGroupKernel" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=MapGroupKernel - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MapGroupKernel.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MapGroupKernel.mak" CFG="MapGroupKernel - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MapGroupKernel - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "MapGroupKernel - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "MapGroupKernel - Win32 AshRelease" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/MapGroupKernel", OHAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MapGroupKernel - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\Bin\Release\MapGroupKernel"
# PROP Intermediate_Dir "..\..\..\..\..\Bin\Release\MapGroupKernel"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GX /O2 /I "NetWork" /I "..\..\GAMEBASECODEMT" /I "." /I "..\..\common" /I "..\..\comm" /I "..\share" /I "..\..\..\..\..\luaplus5\include" /D "MAPGROUP_KERNEL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\..\Bin\Releaselib\server\MapGroupKernel.lib"

!ELSEIF  "$(CFG)" == "MapGroupKernel - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\Bin\Debug\MapGroupKernel"
# PROP Intermediate_Dir "..\..\..\..\..\Bin\Debug\MapGroupKernel"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I ".\NetWork" /I "..\..\common\BaseSupport" /I "..\..\GameBaseCodeMT" /I "." /I "..\..\common" /I "..\..\comm" /I "..\share" /I "..\..\..\..\..\luaplus5\include" /D "MAPGROUP_KERNEL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "ZLONG_DEBUG" /Fr /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\..\Bin\Debuglib\server\MapGroupKernel_d.lib"

!ELSEIF  "$(CFG)" == "MapGroupKernel - Win32 AshRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MapGroupKernel___Win32_AshRelease"
# PROP BASE Intermediate_Dir "MapGroupKernel___Win32_AshRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\Bin\AshRelease\MapGroupKernel"
# PROP Intermediate_Dir "..\..\..\..\..\Bin\AshRelease\MapGroupKernel"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MT /W3 /GX /O2 /I "." /I "NetWork" /I "..\..\GameBaseCode" /I "..\..\common" /I "..\..\comm" /I "..\share" /D "_ASHRELEASE" /D "MAPGROUP_KERNEL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MT /W3 /GX /O2 /I "NetWork" /I "..\..\GAMEBASECODEMT" /I "." /I "..\..\common" /I "..\..\comm" /I "..\share" /I "..\..\..\..\..\luaplus5\include" /D "_ASHRELEASE" /D "MAPGROUP_KERNEL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /Zm200 /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\..\Bin\Lib\MapGroupKernel.lib"
# ADD LIB32 /nologo /out:"..\..\..\..\..\Bin\AshReleaselib\server\MapGroupKernel_ashrelease.lib"

!ENDIF 

# Begin Target

# Name "MapGroupKernel - Win32 Release"
# Name "MapGroupKernel - Win32 Debug"
# Name "MapGroupKernel - Win32 AshRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AdjustRate.cpp
# End Source File
# Begin Source File

SOURCE=.\Agent.cpp
# End Source File
# Begin Source File

SOURCE=.\AINpc.cpp
# End Source File
# Begin Source File

SOURCE=.\Announce.cpp
# End Source File
# Begin Source File

SOURCE=.\AnnounceData.cpp
# End Source File
# Begin Source File

SOURCE=.\AnnounceInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Antiplug.cpp
# End Source File
# Begin Source File

SOURCE=.\AnyBuyData.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoFightData.cpp
# End Source File
# Begin Source File

SOURCE=.\AwardExp.cpp
# End Source File
# Begin Source File

SOURCE=.\AwardHonor.cpp
# End Source File
# Begin Source File

SOURCE=.\AwardMoney.cpp
# End Source File
# Begin Source File

SOURCE=.\BetOutside.cpp
# End Source File
# Begin Source File

SOURCE=.\Booth.cpp
# End Source File
# Begin Source File

SOURCE=.\CDManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ChannelMsgManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CityWare.cpp
# End Source File
# Begin Source File

SOURCE=.\CityWarMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\CombGod.cpp
# End Source File
# Begin Source File

SOURCE=.\CombItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Compensation.cpp
# End Source File
# Begin Source File

SOURCE=.\CompensationManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CompensationMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomInstance.cpp
# End Source File
# Begin Source File

SOURCE=.\DataInspectManage.cpp
# End Source File
# Begin Source File

SOURCE=.\DropRule.cpp
# End Source File
# Begin Source File

SOURCE=.\DropRuleGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\DropRuleMap.cpp
# End Source File
# Begin Source File

SOURCE=.\EconomySystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Enemy.cpp
# End Source File
# Begin Source File

SOURCE=.\EquipCreate.cpp
# End Source File
# Begin Source File

SOURCE=.\EventPack.cpp
# End Source File
# Begin Source File

SOURCE=.\FatiGueManager.cpp
# End Source File
# Begin Source File

SOURCE=.\FecthData.cpp
# End Source File
# Begin Source File

SOURCE=.\FriendContactHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\GameAction.cpp
# End Source File
# Begin Source File

SOURCE=.\GameObj.cpp
# End Source File
# Begin Source File

SOURCE=.\GemCreate.cpp
# End Source File
# Begin Source File

SOURCE=.\GiftCursor.cpp
# End Source File
# Begin Source File

SOURCE=.\GiftManager.cpp
# End Source File
# Begin Source File

SOURCE=.\GiftTempleteManager.cpp
# End Source File
# Begin Source File

SOURCE=.\House.cpp
# End Source File
# Begin Source File

SOURCE=.\HouseManager.cpp
# End Source File
# Begin Source File

SOURCE=.\IMarryObject.cpp
# End Source File
# Begin Source File

SOURCE=.\InstanceNPC.cpp
# End Source File
# Begin Source File

SOURCE=.\InstanceRule.cpp
# End Source File
# Begin Source File

SOURCE=.\InstanceUseTime.cpp
# End Source File
# Begin Source File

SOURCE=.\InstanceUseTimeMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemDropLimit.cpp
# End Source File
# Begin Source File

SOURCE=.\JinmeiStrengthData.cpp
# End Source File
# Begin Source File

SOURCE=.\LoverHome.cpp
# End Source File
# Begin Source File

SOURCE=.\LoverHomeManager.cpp
# End Source File
# Begin Source File

SOURCE=.\LuaScriptMachine.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicData.cpp
# End Source File
# Begin Source File

SOURCE=.\MapArena.cpp
# End Source File
# Begin Source File

SOURCE=.\MapClear.cpp
# End Source File
# Begin Source File

SOURCE=.\MapGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\MapGroupKernel.cpp
# End Source File
# Begin Source File

SOURCE=.\MapGroupObject.cpp
# End Source File
# Begin Source File

SOURCE=.\MapInstance.cpp
# End Source File
# Begin Source File

SOURCE=.\MapInstanceMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\MapTrap.cpp
# End Source File
# Begin Source File

SOURCE=.\MarryGod.cpp
# End Source File
# Begin Source File

SOURCE=.\MarryObject.cpp
# End Source File
# Begin Source File

SOURCE=.\MarryPackage.cpp
# End Source File
# Begin Source File

SOURCE=.\MineRule.cpp
# End Source File
# Begin Source File

SOURCE=.\MineRuleGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\MineRuleMap.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgAccLock.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgAdjustChange.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgAnnounceInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgAnnounceList.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgAntiplug.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgAutoFight.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgAward.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgBetOutside.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgBuff.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgCallPetInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgChangeGem.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgCityWar.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgComb.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgCompensation.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgCompensationMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgEudemon.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgEudemonAttrib.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgEudemonInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgFightTeam.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgFightTeamAction.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgFreeUnionMagicInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgGift.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgGmAction.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgGmActionLog.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgGmAdvAction.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgGmChgPsw.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgGmMac.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgHelpAction.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgHouse.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgInsMemInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgIssue.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgItemAttrib.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgJinmei.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgMagicIntone.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgMainServer.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgMapInstance.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgNewCard.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgNPCShop.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgPetegg.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgPKGameInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgPlayDetil.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgPlayerNpc.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgPlayerTask.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgRandNPCShop.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgSaveItemView.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgSchoolMember.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgShop.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgStockTrade.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgStopAccount.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgSuperItem.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgSynAttackWar.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgSyndicateList.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgTaskAction.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTaskList.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTeamPos.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTeamResultInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTitle.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgTopList.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgTopList.h
# End Source File
# Begin Source File

SOURCE=.\MsgUserKey.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgUserPos.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgUserProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgVASItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgVipList.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgVirtualItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgWalkEx.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgWargame.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgWargameSync.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgWeapensoulShow.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgWeaponSoul.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgWeekActive.cpp
# End Source File
# Begin Source File

SOURCE=.\NpcGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\NpcMagicData.cpp
# End Source File
# Begin Source File

SOURCE=.\NpcMagicManager.cpp
# End Source File
# Begin Source File

SOURCE=.\NpcScene.cpp
# End Source File
# Begin Source File

SOURCE=.\NpcTable.cpp
# End Source File
# Begin Source File

SOURCE=.\NpcType.cpp
# End Source File
# Begin Source File

SOURCE=.\OnlineGift.cpp
# End Source File
# Begin Source File

SOURCE=.\OnlineGiftData.cpp
# End Source File
# Begin Source File

SOURCE=.\Package.cpp
# End Source File
# Begin Source File

SOURCE=.\PetEgg.cpp
# End Source File
# Begin Source File

SOURCE=.\PetEggData.cpp
# End Source File
# Begin Source File

SOURCE=.\PKWorldCupManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PoliceWanted.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableShop.cpp
# End Source File
# Begin Source File

SOURCE=.\PortableShopManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PvpActive.cpp
# End Source File
# Begin Source File

SOURCE=.\PVPManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Rename.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourceWar.cpp
# End Source File
# Begin Source File

SOURCE=.\Role.cpp
# End Source File
# Begin Source File

SOURCE=.\RoleManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SettledAction.cpp
# End Source File
# Begin Source File

SOURCE=.\ShopVAS.cpp
# End Source File
# Begin Source File

SOURCE=.\SKHonorMurderList.cpp
# End Source File
# Begin Source File

SOURCE=.\SKPostOffice.cpp
# End Source File
# Begin Source File

SOURCE=.\SpecilLineInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusManager.cpp
# End Source File
# Begin Source File

SOURCE=.\StockOrder.cpp
# End Source File
# Begin Source File

SOURCE=.\Suit.cpp
# End Source File
# Begin Source File

SOURCE=.\SynHomeBuyData.cpp
# End Source File
# Begin Source File

SOURCE=.\SynHomeChgData.cpp
# End Source File
# Begin Source File

SOURCE=.\SynHomeLevData.cpp
# End Source File
# Begin Source File

SOURCE=.\SynHomeTaskData.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskSetRand.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskSetRandChain.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskSetRandData.cpp
# End Source File
# Begin Source File

SOURCE=.\TitleAttar.cpp
# End Source File
# Begin Source File

SOURCE=.\TitleTypeData.cpp
# End Source File
# Begin Source File

SOURCE=.\TopListSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\TopListSystem.h
# End Source File
# Begin Source File

SOURCE=.\Transformation.cpp
# End Source File
# Begin Source File

SOURCE=.\Treasure.cpp
# End Source File
# Begin Source File

SOURCE=.\TreasureData.cpp
# End Source File
# Begin Source File

SOURCE=.\TreasureType.cpp
# End Source File
# Begin Source File

SOURCE=.\Tutor.cpp
# End Source File
# Begin Source File

SOURCE=.\TutorData.cpp
# End Source File
# Begin Source File

SOURCE=.\TutorList.cpp
# End Source File
# Begin Source File

SOURCE=.\User_Key.cpp
# End Source File
# Begin Source File

SOURCE=.\UserGift.cpp
# End Source File
# Begin Source File

SOURCE=.\UserJinMeiSys.cpp
# End Source File
# Begin Source File

SOURCE=.\UserJinMeiSysData.cpp
# End Source File
# Begin Source File

SOURCE=.\UserJinMeiSysMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\UserJinMeiTypeData.cpp
# End Source File
# Begin Source File

SOURCE=.\UserKey.cpp
# End Source File
# Begin Source File

SOURCE=.\UserMarryRing.cpp
# End Source File
# Begin Source File

SOURCE=.\UserMaterialItem.cpp
# End Source File
# Begin Source File

SOURCE=.\UserPackage.cpp
# End Source File
# Begin Source File

SOURCE=.\UserPet.cpp
# End Source File
# Begin Source File

SOURCE=.\UserSaveItem.cpp
# End Source File
# Begin Source File

SOURCE=.\UserSaveItemData.cpp
# End Source File
# Begin Source File

SOURCE=.\UserTitle.cpp
# End Source File
# Begin Source File

SOURCE=.\UserTitleData.cpp
# End Source File
# Begin Source File

SOURCE=.\UserTutor.cpp
# End Source File
# Begin Source File

SOURCE=.\UserWarGame.cpp
# End Source File
# Begin Source File

SOURCE=.\VariantManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Verification.cpp
# End Source File
# Begin Source File

SOURCE=.\VerificationData.cpp
# End Source File
# Begin Source File

SOURCE=.\VipList.cpp
# End Source File
# Begin Source File

SOURCE=.\WantedList.cpp
# End Source File
# Begin Source File

SOURCE=.\Wargame.cpp
# End Source File
# Begin Source File

SOURCE=.\WarGameArena.cpp
# End Source File
# Begin Source File

SOURCE=.\WargameBorder.cpp
# End Source File
# Begin Source File

SOURCE=.\WargameBorderStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\WargameManager.cpp
# End Source File
# Begin Source File

SOURCE=.\WargameStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\WargameTime.cpp
# End Source File
# Begin Source File

SOURCE=.\WeaponSoulData.cpp
# End Source File
# Begin Source File

SOURCE=.\Weather.cpp
# End Source File
# Begin Source File

SOURCE=.\WeatherRegion.cpp
# End Source File
# Begin Source File

SOURCE=.\WeekActive.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AdjustRate.h
# End Source File
# Begin Source File

SOURCE=.\Agent.h
# End Source File
# Begin Source File

SOURCE=.\AINpc.h
# End Source File
# Begin Source File

SOURCE=.\Announce.h
# End Source File
# Begin Source File

SOURCE=.\AnnounceData.h
# End Source File
# Begin Source File

SOURCE=.\AnnounceInfo.h
# End Source File
# Begin Source File

SOURCE=.\Antiplug.h
# End Source File
# Begin Source File

SOURCE=.\AnyBuyData.h
# End Source File
# Begin Source File

SOURCE=.\AutoFightData.h
# End Source File
# Begin Source File

SOURCE=.\AwardExp.h
# End Source File
# Begin Source File

SOURCE=.\AwardHonor.h
# End Source File
# Begin Source File

SOURCE=.\AwardMoney.h
# End Source File
# Begin Source File

SOURCE=.\BetOutside.h
# End Source File
# Begin Source File

SOURCE=.\Booth.h
# End Source File
# Begin Source File

SOURCE=.\CDManager.h
# End Source File
# Begin Source File

SOURCE=.\ChannelMsgManager.h
# End Source File
# Begin Source File

SOURCE=.\CityWare.h
# End Source File
# Begin Source File

SOURCE=.\CityWarMgr.h
# End Source File
# Begin Source File

SOURCE=.\CombGod.h
# End Source File
# Begin Source File

SOURCE=.\CombItem.h
# End Source File
# Begin Source File

SOURCE=.\Compensation.h
# End Source File
# Begin Source File

SOURCE=.\CompensationManager.h
# End Source File
# Begin Source File

SOURCE=.\CompensationMsg.h
# End Source File
# Begin Source File

SOURCE=.\CustomInstance.h
# End Source File
# Begin Source File

SOURCE=.\DataInspectManage.h
# End Source File
# Begin Source File

SOURCE=.\DataInspectManageTest.h
# End Source File
# Begin Source File

SOURCE=.\DeadLoop.h
# End Source File
# Begin Source File

SOURCE=.\DropRule.h
# End Source File
# Begin Source File

SOURCE=.\DropRuleGroup.h
# End Source File
# Begin Source File

SOURCE=.\DropRuleMap.h
# End Source File
# Begin Source File

SOURCE=.\EconomySystem.h
# End Source File
# Begin Source File

SOURCE=.\Enemy.h
# End Source File
# Begin Source File

SOURCE=.\EquipCreate.h
# End Source File
# Begin Source File

SOURCE=.\EudPackage.h
# End Source File
# Begin Source File

SOURCE=.\EventPack.h
# End Source File
# Begin Source File

SOURCE=.\FatiGueManager.h
# End Source File
# Begin Source File

SOURCE=.\FecthData.h
# End Source File
# Begin Source File

SOURCE=.\FriendContactHistory.h
# End Source File
# Begin Source File

SOURCE=.\GameAction.h
# End Source File
# Begin Source File

SOURCE=.\GameObj.h
# End Source File
# Begin Source File

SOURCE=.\GemCreate.h
# End Source File
# Begin Source File

SOURCE=.\GiftCursor.h
# End Source File
# Begin Source File

SOURCE=.\GiftManager.h
# End Source File
# Begin Source File

SOURCE=.\GiftTempleteManager.h
# End Source File
# Begin Source File

SOURCE=.\House.h
# End Source File
# Begin Source File

SOURCE=.\HouseManager.h
# End Source File
# Begin Source File

SOURCE=..\..\GameBaseCodeMT\I_mydb.h
# End Source File
# Begin Source File

SOURCE=.\IMarryObject.h
# End Source File
# Begin Source File

SOURCE=.\InstanceNPC.h
# End Source File
# Begin Source File

SOURCE=.\InstanceRule.h
# End Source File
# Begin Source File

SOURCE=.\InstanceUseTime.h
# End Source File
# Begin Source File

SOURCE=.\InstanceUseTimeMgr.h
# End Source File
# Begin Source File

SOURCE=.\ItemDropLimit.h
# End Source File
# Begin Source File

SOURCE=.\JinmeiStrengthData.h
# End Source File
# Begin Source File

SOURCE=.\LoverHome.h
# End Source File
# Begin Source File

SOURCE=.\LoverHomeManager.h
# End Source File
# Begin Source File

SOURCE=.\LuaScriptMachine.h
# End Source File
# Begin Source File

SOURCE=.\MagicData.h
# End Source File
# Begin Source File

SOURCE=.\MapArena.h
# End Source File
# Begin Source File

SOURCE=.\MapClear.h
# End Source File
# Begin Source File

SOURCE=.\MapGroup.h
# End Source File
# Begin Source File

SOURCE=.\MapGroupKernel.h
# End Source File
# Begin Source File

SOURCE=.\MapGroupObject.h
# End Source File
# Begin Source File

SOURCE=.\MapInstance.h
# End Source File
# Begin Source File

SOURCE=.\MapInstanceMgr.h
# End Source File
# Begin Source File

SOURCE=.\MapTrap.h
# End Source File
# Begin Source File

SOURCE=.\MarryGod.h
# End Source File
# Begin Source File

SOURCE=.\MarryObject.h
# End Source File
# Begin Source File

SOURCE=.\MarryPackage.h
# End Source File
# Begin Source File

SOURCE=.\MineRule.h
# End Source File
# Begin Source File

SOURCE=.\MineRuleGroup.h
# End Source File
# Begin Source File

SOURCE=.\MineRuleMap.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgAccLock.h
# End Source File
# Begin Source File

SOURCE=.\MsgAdjustChange.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgAnnounceInfo.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgAnnounceList.h
# End Source File
# Begin Source File

SOURCE=.\MsgAntiplug.h
# End Source File
# Begin Source File

SOURCE=.\MsgAutoFight.h
# End Source File
# Begin Source File

SOURCE=.\MsgAward.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgBetOutside.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgBuff.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgCallPetInfo.h
# End Source File
# Begin Source File

SOURCE=.\MsgChangeGem.h
# End Source File
# Begin Source File

SOURCE=.\MsgCityWar.h
# End Source File
# Begin Source File

SOURCE=.\MsgComb.h
# End Source File
# Begin Source File

SOURCE=.\MsgCompensation.h
# End Source File
# Begin Source File

SOURCE=.\MsgCompensationMsg.h
# End Source File
# Begin Source File

SOURCE=.\MsgEudemon.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgEudemonAttrib.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgEudemonInfo.h
# End Source File
# Begin Source File

SOURCE=.\MsgFightTeam.h
# End Source File
# Begin Source File

SOURCE=.\MsgFightTeamAction.h
# End Source File
# Begin Source File

SOURCE=.\MsgFreeUnionMagicInfo.h
# End Source File
# Begin Source File

SOURCE=.\MsgGift.h
# End Source File
# Begin Source File

SOURCE=.\MsgGmAction.h
# End Source File
# Begin Source File

SOURCE=.\MsgGmActionLog.h
# End Source File
# Begin Source File

SOURCE=.\MsgGmAdvAction.h
# End Source File
# Begin Source File

SOURCE=.\MsgGmChgPsw.h
# End Source File
# Begin Source File

SOURCE=.\MsgGmMac.h
# End Source File
# Begin Source File

SOURCE=.\MsgHelpAction.h
# End Source File
# Begin Source File

SOURCE=.\MsgHouse.h
# End Source File
# Begin Source File

SOURCE=.\MsgInsMemInfo.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgIssue.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgItemAttrib.h
# End Source File
# Begin Source File

SOURCE=.\MsgJinmei.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgLottery.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgMagicIntone.h
# End Source File
# Begin Source File

SOURCE=.\MsgMainServer.h
# End Source File
# Begin Source File

SOURCE=.\MsgMapInstance.h
# End Source File
# Begin Source File

SOURCE=.\MsgNewCard.h
# End Source File
# Begin Source File

SOURCE=.\MsgNPCShop.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgPetegg.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgPKGameInfo.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgPlayDetil.h
# End Source File
# Begin Source File

SOURCE=.\MsgPlayerNpc.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgPlayerTask.h
# End Source File
# Begin Source File

SOURCE=.\MsgRandNPCShop.h
# End Source File
# Begin Source File

SOURCE=.\MsgSaveItemView.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgSchoolMember.h
# End Source File
# Begin Source File

SOURCE=.\MsgShop.h
# End Source File
# Begin Source File

SOURCE=.\MsgStockTrade.h
# End Source File
# Begin Source File

SOURCE=.\MsgStopAccount.h
# End Source File
# Begin Source File

SOURCE=.\MsgSuperItem.h
# End Source File
# Begin Source File

SOURCE=.\MsgSynAttackWar.h
# End Source File
# Begin Source File

SOURCE=.\MsgSyndicateList.h
# End Source File
# Begin Source File

SOURCE=.\MsgTaskAction.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTaskList.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTeamPos.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTeamResultInfo.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTitle.h
# End Source File
# Begin Source File

SOURCE=.\MsgUserKey.h
# End Source File
# Begin Source File

SOURCE=.\MsgUserPos.h
# End Source File
# Begin Source File

SOURCE=.\MsgUserProperty.h
# End Source File
# Begin Source File

SOURCE=.\MsgVASItem.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgVipList.h
# End Source File
# Begin Source File

SOURCE=.\MsgVirtualItem.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgWalkEx.h
# End Source File
# Begin Source File

SOURCE=.\MsgWargame.h
# End Source File
# Begin Source File

SOURCE=.\MsgWargameSync.h
# End Source File
# Begin Source File

SOURCE=.\MsgWeapensoulShow.h
# End Source File
# Begin Source File

SOURCE=.\MsgWeaponSoul.h
# End Source File
# Begin Source File

SOURCE=.\MsgWeekActive.h
# End Source File
# Begin Source File

SOURCE=.\MultiObjSet.h
# End Source File
# Begin Source File

SOURCE=.\Network\NetMsg.h
# End Source File
# Begin Source File

SOURCE=.\NpcGenerator.h
# End Source File
# Begin Source File

SOURCE=.\NpcMagicData.h
# End Source File
# Begin Source File

SOURCE=.\NpcMagicManager.h
# End Source File
# Begin Source File

SOURCE=.\NpcManager.h
# End Source File
# Begin Source File

SOURCE=.\NpcScene.h
# End Source File
# Begin Source File

SOURCE=.\NpcTable.h
# End Source File
# Begin Source File

SOURCE=.\NpcType.h
# End Source File
# Begin Source File

SOURCE=.\OnlineGift.h
# End Source File
# Begin Source File

SOURCE=.\OnlineGiftData.h
# End Source File
# Begin Source File

SOURCE=.\Package.h
# End Source File
# Begin Source File

SOURCE=.\PetEgg.h
# End Source File
# Begin Source File

SOURCE=.\PetEggData.h
# End Source File
# Begin Source File

SOURCE=.\PKWorldCupManager.h
# End Source File
# Begin Source File

SOURCE=.\PoliceWanted.h
# End Source File
# Begin Source File

SOURCE=.\PortableShop.h
# End Source File
# Begin Source File

SOURCE=.\PortableShopManager.h
# End Source File
# Begin Source File

SOURCE=.\PvpActive.h
# End Source File
# Begin Source File

SOURCE=.\PVPManager.h
# End Source File
# Begin Source File

SOURCE=.\Rename.h
# End Source File
# Begin Source File

SOURCE=.\ResourceWar.h
# End Source File
# Begin Source File

SOURCE=.\Role.h
# End Source File
# Begin Source File

SOURCE=.\RoleManager.h
# End Source File
# Begin Source File

SOURCE=.\SettledAction.h
# End Source File
# Begin Source File

SOURCE=.\ShopVAS.h
# End Source File
# Begin Source File

SOURCE=.\SingleMap64.h
# End Source File
# Begin Source File

SOURCE=.\SKHonorMurderList.h
# End Source File
# Begin Source File

SOURCE=.\SKPostOffice.h
# End Source File
# Begin Source File

SOURCE=.\SKPostOfficeTest.h
# End Source File
# Begin Source File

SOURCE=.\SpecilLineInfo.h
# End Source File
# Begin Source File

SOURCE=.\StatusManager.h
# End Source File
# Begin Source File

SOURCE=.\StockOrder.h
# End Source File
# Begin Source File

SOURCE=.\Suit.h
# End Source File
# Begin Source File

SOURCE=.\SynHomeBuyData.h
# End Source File
# Begin Source File

SOURCE=.\SynHomeChgData.h
# End Source File
# Begin Source File

SOURCE=.\SynHomeLevData.h
# End Source File
# Begin Source File

SOURCE=.\SynHomeTaskData.h
# End Source File
# Begin Source File

SOURCE=.\T_SingleMap2.h
# End Source File
# Begin Source File

SOURCE=.\T_SingleObjSet2.h
# End Source File
# Begin Source File

SOURCE=.\TaskSetRand.h
# End Source File
# Begin Source File

SOURCE=.\TaskSetRandChain.h
# End Source File
# Begin Source File

SOURCE=.\TaskSetRandData.h
# End Source File
# Begin Source File

SOURCE=.\TitleAttar.h
# End Source File
# Begin Source File

SOURCE=.\TitleTypeData.h
# End Source File
# Begin Source File

SOURCE=.\Transformation.h
# End Source File
# Begin Source File

SOURCE=.\Treasure.h
# End Source File
# Begin Source File

SOURCE=.\TreasureData.h
# End Source File
# Begin Source File

SOURCE=.\TreasureType.h
# End Source File
# Begin Source File

SOURCE=.\Tutor.h
# End Source File
# Begin Source File

SOURCE=.\TutorData.h
# End Source File
# Begin Source File

SOURCE=.\TutorList.h
# End Source File
# Begin Source File

SOURCE=.\UnitTestSelf.h
# End Source File
# Begin Source File

SOURCE=.\UserGift.h
# End Source File
# Begin Source File

SOURCE=.\UserJinMeiSys.h
# End Source File
# Begin Source File

SOURCE=.\UserJinMeiSysData.h
# End Source File
# Begin Source File

SOURCE=.\UserJinMeiSysMgr.h
# End Source File
# Begin Source File

SOURCE=.\UserJinMeiTypeData.h
# End Source File
# Begin Source File

SOURCE=.\UserKey.h
# End Source File
# Begin Source File

SOURCE=.\UserMarryRing.h
# End Source File
# Begin Source File

SOURCE=.\UserMaterialItem.h
# End Source File
# Begin Source File

SOURCE=.\UserPackage.h
# End Source File
# Begin Source File

SOURCE=.\UserPet.h
# End Source File
# Begin Source File

SOURCE=.\UserSaveItem.h
# End Source File
# Begin Source File

SOURCE=.\UserSaveItemData.h
# End Source File
# Begin Source File

SOURCE=.\UserTitle.h
# End Source File
# Begin Source File

SOURCE=.\UserTitleData.h
# End Source File
# Begin Source File

SOURCE=.\UserTutor.h
# End Source File
# Begin Source File

SOURCE=.\UserWarGame.h
# End Source File
# Begin Source File

SOURCE=.\VariantManager.h
# End Source File
# Begin Source File

SOURCE=.\Verification.h
# End Source File
# Begin Source File

SOURCE=.\VerificationData.h
# End Source File
# Begin Source File

SOURCE=.\VipList.h
# End Source File
# Begin Source File

SOURCE=.\WantedList.h
# End Source File
# Begin Source File

SOURCE=.\Wargame.h
# End Source File
# Begin Source File

SOURCE=.\WarGameArena.h
# End Source File
# Begin Source File

SOURCE=.\WargameBorder.h
# End Source File
# Begin Source File

SOURCE=.\WargameBorderStatus.h
# End Source File
# Begin Source File

SOURCE=.\WargameManager.h
# End Source File
# Begin Source File

SOURCE=.\WargameStatus.h
# End Source File
# Begin Source File

SOURCE=.\WargameTime.h
# End Source File
# Begin Source File

SOURCE=.\WeaponSoulData.h
# End Source File
# Begin Source File

SOURCE=.\Weather.h
# End Source File
# Begin Source File

SOURCE=.\WeatherRegion.h
# End Source File
# Begin Source File

SOURCE=.\WeekActive.h
# End Source File
# End Group
# Begin Group "NetWork"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Network\MsgAction.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgAction.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgAiNpcInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgAiNpcInfo.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgAllot.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgAllot.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgBattleSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgBattleSystem.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgChangeName.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgChangeName.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgCompound.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgCompound.h
# End Source File
# Begin Source File

SOURCE=.\MsgCoolDown.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgCoolDown.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgDialog.h
# End Source File
# Begin Source File

SOURCE=.\MsgFatigue.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgFatigue.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgFlushExp.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgFlushExp.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgFriend.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgFriend.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgFriendInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgFriendInfo.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgGemEmbed.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgGemEmbed.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgInteract.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgInteract.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgItem.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgItemInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgItemInfo.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgItemInfoEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgItemInfoEx.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgLottery.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgLottery.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgMagicEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgMagicEffect.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgMagicInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgMagicInfo.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgMail.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgMail.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgMapInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgMapInfo.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgMapItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgMapItem.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgMarry.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgMarry.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgMessageBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgMessageBoard.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgName.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgName.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgNewPlayerExam.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgNewPlayerExam.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgNpc.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgNpc.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgNpcInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgNpcInfo.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgNpcInfoEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgNpcInfoEx.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgPackage.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgPackage.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgPlayer.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgSynAttrInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgSynAttrInfo.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgSyndicate.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgSyndicate.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgSynInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgSynInfo.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgSynMemberInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgSynMemberInfo.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTalk.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTalk.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTaskDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTaskDetail.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTeam.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTeam.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTeamMember.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTeamMember.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTick.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTick.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTrade.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTrade.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTraffic.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgTraffic.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgUserAttrib.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgUserAttrib.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgUserInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgUserInfo.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgVAS.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgVAS.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgVasLv.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgVasLv.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgWalk.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgWalk.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgWeaponSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgWeaponSkill.h
# End Source File
# Begin Source File

SOURCE=.\Network\MsgWeather.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\MsgWeather.h
# End Source File
# Begin Source File

SOURCE=.\SKMail.cpp
# End Source File
# Begin Source File

SOURCE=.\SKMail.h
# End Source File
# End Group
# Begin Group "User"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BattleSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleSystem.h
# End Source File
# Begin Source File

SOURCE=.\Friend.cpp
# End Source File
# Begin Source File

SOURCE=.\Friend.h
# End Source File
# Begin Source File

SOURCE=.\FriendData.cpp
# End Source File
# Begin Source File

SOURCE=.\FriendData.h
# End Source File
# Begin Source File

SOURCE=.\LeaveWord.cpp
# End Source File
# Begin Source File

SOURCE=.\LeaveWord.h
# End Source File
# Begin Source File

SOURCE=.\NpcManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SquareDeal.cpp
# End Source File
# Begin Source File

SOURCE=.\SquareDeal.h
# End Source File
# Begin Source File

SOURCE=.\Team.cpp
# End Source File
# Begin Source File

SOURCE=.\Team.h
# End Source File
# Begin Source File

SOURCE=.\User.cpp
# End Source File
# Begin Source File

SOURCE=.\User.h
# End Source File
# Begin Source File

SOURCE=.\User_Item.cpp
# End Source File
# Begin Source File

SOURCE=.\User_Task.cpp
# End Source File
# Begin Source File

SOURCE=.\UserData.cpp
# End Source File
# Begin Source File

SOURCE=.\UserData.h
# End Source File
# Begin Source File

SOURCE=.\UserManager.cpp
# End Source File
# Begin Source File

SOURCE=.\UserManager.h
# End Source File
# Begin Source File

SOURCE=.\UserTable.h
# End Source File
# Begin Source File

SOURCE=.\UserTeam.cpp
# End Source File
# Begin Source File

SOURCE=.\UserWeaponSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\UserWeaponSkill.h
# End Source File
# End Group
# Begin Group "Map"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GameBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\GameBlock.h
# End Source File
# Begin Source File

SOURCE=.\GameMap.cpp
# End Source File
# Begin Source File

SOURCE=.\GameMap.h
# End Source File
# Begin Source File

SOURCE=.\MapItem.cpp
# End Source File
# Begin Source File

SOURCE=.\MapItem.h
# End Source File
# Begin Source File

SOURCE=.\MapManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MapManager.h
# End Source File
# Begin Source File

SOURCE=.\VasLvData.cpp
# End Source File
# Begin Source File

SOURCE=.\VasLvData.h
# End Source File
# End Group
# Begin Group "Npc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Npc.cpp
# End Source File
# Begin Source File

SOURCE=.\Npc.h
# End Source File
# Begin Source File

SOURCE=.\NpcLink.cpp
# End Source File
# Begin Source File

SOURCE=.\NpcLink.h
# End Source File
# Begin Source File

SOURCE=.\NpcShop.cpp
# End Source File
# Begin Source File

SOURCE=.\NpcShop.h
# End Source File
# Begin Source File

SOURCE=.\NpcStorage.cpp
# End Source File
# Begin Source File

SOURCE=.\NpcStorage.h
# End Source File
# Begin Source File

SOURCE=.\NpcTask.cpp
# End Source File
# Begin Source File

SOURCE=.\NpcTask.h
# End Source File
# End Group
# Begin Group "Syn"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SynAttr.cpp
# End Source File
# Begin Source File

SOURCE=.\SynAttr.h
# End Source File
# Begin Source File

SOURCE=.\Syndicate.cpp
# End Source File
# Begin Source File

SOURCE=.\Syndicate.h
# End Source File
# Begin Source File

SOURCE=.\SynManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SynManager.h
# End Source File
# End Group
# Begin Group "Magic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Magic.cpp
# End Source File
# Begin Source File

SOURCE=.\Magic.h
# End Source File
# Begin Source File

SOURCE=.\Magic_Process.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicType.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicType.h
# End Source File
# Begin Source File

SOURCE=.\StatusMore.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusMore.h
# End Source File
# Begin Source File

SOURCE=.\StatusOnce.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusOnce.h
# End Source File
# End Group
# Begin Group "Item"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Item.cpp
# End Source File
# Begin Source File

SOURCE=.\Item.h
# End Source File
# Begin Source File

SOURCE=.\ItemAddition.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemAddition.h
# End Source File
# Begin Source File

SOURCE=.\ItemData.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemData.h
# End Source File
# Begin Source File

SOURCE=.\ItemType.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemType.h
# End Source File
# End Group
# Begin Group "TaskDetail"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TaskDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskDetail.h
# End Source File
# Begin Source File

SOURCE=.\TaskDetailData.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskDetailData.h
# End Source File
# End Group
# Begin Group "chat"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\ActionDefine.h
# End Source File
# Begin Source File

SOURCE=.\Network\AllMsg.h
# End Source File
# Begin Source File

SOURCE=..\share\Cn_DDRes.h
# End Source File
# Begin Source File

SOURCE=..\share\define.h
# End Source File
# Begin Source File

SOURCE=.\I_MapData.h
# End Source File
# Begin Source File

SOURCE=.\I_Role.h
# End Source File
# Begin Source File

SOURCE=..\share\I_Shell.h
# End Source File
# Begin Source File

SOURCE=.\Network\NetMsg.cpp
# End Source File
# Begin Source File

SOURCE=..\share\networkdef.h
# End Source File
# Begin Source File

SOURCE=..\share\protocol.h
# End Source File
# Begin Source File

SOURCE=".\怪物掉物品规则设计.txt"
# End Source File
# Begin Source File

SOURCE=".\回合战斗系统设计日志.txt"
# End Source File
# Begin Source File

SOURCE=".\设计日志.txt"
# End Source File
# End Target
# End Project
