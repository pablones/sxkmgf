#ifndef _ALLMSG_H
#define _ALLMSG_H

#define	_WINSOCKAPI_		// 阻止加载winsock.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <windows.h>

#include "NetMsg.h"

using namespace world_kernel;		// all msg use namespace world_kernel!!!
#include "MsgTalk.h"
#include "MsgMessageBoard.h"
#include "MsgRegister.h"
#include "MsgLogin.h"
#include "MsgAiNpcInfo.h"
#include "MsgConnect.h"
#include "MsgTime.h"
#include "MsgName.h"
#include "MsgFriend.h"
#include "MsgDialog.h"
#include "MsgVAS.h"
#include "MsgWaitQueue.h"	//  [2/13/2007 Halei] 玩家排队等待消息
#include "MsgStopAccount.h"//  [8/15/2007 Add By HaLei] GM封号
#include "MsgNewPlayerExam.h"//  [9/22/2007 Administrator]
//#include "MsgChatRoom.h"

#include "MsgCentalMonitor.h"

#include "MsgShop.h"//[游途道标 2009.01.06]店铺系统消息于世界线程
#include "MsgSyndicate.h"
#include "MsgVASItem.h"
#include "MsgInfoNpcUserExit.h"
#include "MsgBroadcast.h"///[2009.3.2--张冬]系统广播消息
#include "MsgFightTeamAction.h"
#include "MsgNewCard.h"
#include "MsgFatigue.h"
#include "MsgTeamPos.h"
#include "MsgSchoolMember.h"
#include "MsgAward.h"
//modify code by 林德渊 begin 2011.5.30
#include "MsgVasLv1.h"
//modify code by 林德渊 end
#endif
