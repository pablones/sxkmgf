#ifndef _ALLMSG_H
#define _ALLMSG_H

#define	_WINSOCKAPI_		// ��ֹ����winsock.h
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
#include "MsgWaitQueue.h"	//  [2/13/2007 Halei] ����Ŷӵȴ���Ϣ
#include "MsgStopAccount.h"//  [8/15/2007 Add By HaLei] GM���
#include "MsgNewPlayerExam.h"//  [9/22/2007 Administrator]
//#include "MsgChatRoom.h"

#include "MsgCentalMonitor.h"

#include "MsgShop.h"//[��;���� 2009.01.06]����ϵͳ��Ϣ�������߳�
#include "MsgSyndicate.h"
#include "MsgVASItem.h"
#include "MsgInfoNpcUserExit.h"
#include "MsgBroadcast.h"///[2009.3.2--�Ŷ�]ϵͳ�㲥��Ϣ
#include "MsgFightTeamAction.h"
#include "MsgNewCard.h"
#include "MsgFatigue.h"
#include "MsgTeamPos.h"
#include "MsgSchoolMember.h"
#include "MsgAward.h"
//modify code by �ֵ�Ԩ begin 2011.5.30
#include "MsgVasLv1.h"
//modify code by �ֵ�Ԩ end
#endif
