#ifndef _ALLMSG_H
#define _ALLMSG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <windows.h>

#include "NetMsg.h"
#include "MsgGmMac.h"		//  [6/19/2007] ����GM MAC��ַ
#include "MsgGmActionLog.h"//  [4/28/2007] ����GM Ȩ����־
#include "MsgGmAdvAction.h"//  [4/27/2007] ����GM �߼�Ȩ��
#include "MsgGmAction.h"   //  [4/28/2007] ����GM ��ͨȨ��
#include "MsgTalk.h"
#include "MsgMessageBoard.h"
#include "MsgAction.h"
#include "MsgUserInfo.h"
#include "MsgPlayer.h"
#include "MsgTick.h"
#include "MsgItemInfo.h"
#include "MsgItem.h"
#include "MsgFriend.h"
#include "MsgInterAct.h"
#include "MsgUserAttrib.h"
#include "MsgName.h"
#include "MsgAiNpcInfo.h"
#include "MsgWalk.h"
#include "MsgWalkEx.h"
#include "MsgMapItem.h"
#include "MsgTrade.h"
#include "MsgBattleSystem.h"
#include "MsgNpc.h"
#include "MsgNpcInfo.h"
#include "MsgPackage.h"
#include "MsgDialog.h"
#include "MsgAllot.h"
#include "MsgTeam.h"
#include "MsgTeamMember.h"
#include "MsgMagicInfo.h"
#include "MsgFlushExp.h"
#include "MsgWeaponSkill.h"
#include "MsgMagicEffect.h"
#include "MsgSynAttrInfo.h"
#include "MsgSyndicate.h"
#include "MsgFriendInfo.h"
#include "MsgWeather.h"
#include "MsgGemEmbed.h"
#include "MsgItemInfoEx.h"
#include "MsgNpcInfoEx.h"
#include "MsgMapInfo.h"
#include "MsgSynMemberInfo.h"
//#include "MsgBetOutside.h"
#include "MsgCallPetInfo.h"
//#include "MsgDataArray.h"
#include "MsgSynInfo.h"
#include "MsgEudemonAttrib.h"
#include "MsgSchoolMember.h"
#include "MsgPlayerTask.h"
#include "MsgTaskList.h"
#include "MsgAnnounceList.h"
#include "MsgAnnounceInfo.h"
//#include "MsgAuction.h"
//#include "MsgChatRoom.h"
#include "MsgItemAttrib.h"
#include "MsgMagicIntone.h"
#include "MsgEudemonInfo.h"
//#include "MsgEudPackage.h"
#include "../MsgChangeGem.h"
#include "MsgTraffic.h"
#include "../MsgCoolDown.h"
#include "../MsgTopList.h"
#include "MsgBuff.h"
#include "MsgTaskDetail.h"//20061107����
#include "MsgCompound.h"  //���1214
#include "MsgVAS.h"//���07.01.09
#include "MsgMail.h"
#include "MsgMarry.h"
#include "MsgGmChgPsw.h"//  [6/27/2007 Add By HaLei] 
#include "MsgSuperItem.h"//  [8/6/2007 Add By HaLei] 
#include "MsgStopAccount.h"//  [8/15/2007 Add By HaLei]  GM���
#include "MsgWargame.h"	//  [2007-8-29] ���
#include "MsgChangeName.h"
#include "MsgAdjustChange.h"// GM�����޸ı��� [9/12/2007 Add By Halei ]
#include "MsgNewPlayerExam.h"//  [9/22/2007 Administrator]
#include "MsgWeaponSoul.h"//20071214����:����
#include "MsgWeapensoulShow.h"//20071214����:����
#include "MsgMapInstance.h"///2008-01-05-������
#include "MsgPKGameInfo.h"	//  [1/8/2008 ���]	PK����
#include "MsgComb.h"			//������Ϣ			
#include "MsgGift.h"//20071204����:�츳
#include "MsgHelpAction.h"
#include "MsgAntiplug.h"  //Aniplug
#include "MsgCityWar.h"
#include "MsgHouse.h"
#include "MsgShop.h"//[��;���� 2009.01.05]����ϵͳ
#include "MsgStockTrade.h"		//��Ʊ��Ϣ
#include "MsgWeekActive.h"
#include "MsgUserProperty.h"
#include "MsgVASItem.h"
#include "MsgUserKey.h"
#include "MsgInfoNpcUserExit.h"		//����˳�ʱ֪ͨnpc
#include "MsgUserPos.h"				//���ͬ����Ϣ��npc������
#include "MsgTaskAction.h"
#include "MsgVirtualItem.h"
#include "MsgAutoFight.h"
#include "MsgSynAttackWar.h"
#include "MsgFightTeam.h"
#include "MsgFightTeamAction.h"
#include "MsgTeamResultInfo.h"
#include "MsgSaveItemView.h"
#include "MsgAward.h"
#include "MsgPlayDetil.h"
#include "MsgNPCShop.h"
#include "MsgEudemon.h"
#include "MsgSyndicateList.h"
#include "MsgRandNPCShop.h"
#include "MsgNewCard.h"
#include "MsgFatigue.h"
#include "MsgAccLock.h"
#include "MsgMainServer.h"
#include "MsgInsMemInfo.h"
#include "MsgVipList.h"
#include "MsgPetegg.h"
#include "MsgCompensation.h"
#include "MsgCompensationMsg.h"
#endif
