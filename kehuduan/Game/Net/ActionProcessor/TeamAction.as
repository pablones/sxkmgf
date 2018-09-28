package Net.ActionProcessor
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Team.Datas.TeamEvent;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class TeamAction extends GameAction
	{
		public static const _MSGTEAM_CREATE:uint			=	0;	//TS	//TC	创建
		public static const _MSGTEAM_APPLYJOIN:uint			=	1;  //TS			申请加入
		public static const _MSGTEAM_LEAVE:uint				=	2;	//TS	//TC	某人离开某队
		public static const _MSGTEAM_ACCEPTINVITE:uint		=	3;	//TS	
		public static const _MSGTEAM_INVITE:uint			=	4;	//TS	//TC 	邀请
		public static const _MSGTEAM_AGREEJOIN:uint			=	5;  //TS			同意申请
		public static const _MSGTEAM_DISMISS:uint			=	6;	//TS	//TC
		public static const _MSGTEAM_KICKOUT:uint			=	7;	//TS	//TC	踢出
		public static const _MSGTEAM_CLOSETEAM:uint			=	8;	//TS	//TC
		public static const _MSGTEAM_OPENTEAM:uint			=	9;	//TS    //TC
		public static const _MSGTEAM_JOINTEAM:uint			=	10;	//TS	某人加入某队
		public static const _MSGTEAM_LEAVETEAM:uint			=   11;	//		某人离开某队
		public static const _MSGTEAM_NOTALLOWTEAM:uint		=	12;	//		设置自己为不允许组队
//		public static const _MSGTEAM_OFLEADER:uint			=	13;	//		移交队长
		public static const _MSGTEAM_CHG_LEADER_APPLY:uint  =   18  //				队长变更
		public static const _MSGTEAM_ASKINFO:uint			=	24;	//				全队信息
		public static const _MSGTEAM_DENY_INVITE:uint		=   31;	//				拒绝邀请
		public static const _MSGTEAM_DENY_APPLY:uint		=   32;	//				拒绝申请
		public static const _MSGTEAM_MEMMBER_APPLY:uint     =   33; //				发出     表示向队长提出某人加入组队，收到表示队员向队长提出要求
		public static const _MSGTEAM_TARGET_INTEAM:uint		=   35;	//		简单的返回，用于查询
		public static const _MSGTEAM_USER_INTEAM:uint		=	36;	//		返回你已经组队，不能再申请加入别人的队伍
		
		public static const _MSGTEAM_INVEITE_INFO:uint		=	37;	//C-S   邀请的队伍信息
		
		public static const _MSGTEAM_REFUSE_INVITE_SOMEONT:uint	 =	38;	//	C-S	队长拒绝 某队员邀请某人
		
		public static const _MSGTEAM_SUPER_MAKE_TEAM:uint		 = 39;	//超级组队模式
		
		public static const _MSGTEAM_UPDATA_TEAMMEMBER_ONLINESTATE:uint = 40;	//更新队伍成员上下线状态  10/23/2010
		
		/////////////////////////// 
		
		public function TeamAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void
		{
			bytes.position  = 4;
			var obj:Object  = new Object();
			
			obj.unAction = bytes.readUnsignedShort();			// Action	
			
			obj.usLev = bytes.readUnsignedShort();				// usLev	(s下发给对长的)		玩家等级   A队长 ，B队员，B邀请C入队，Server发给A的消息中包含C的等级
			obj.usPro = bytes.readUnsignedShort();				// usPro	(s下发给对长的)		玩家职业   A队长 ，B队员，B邀请C入队，Server发给A的消息中包含C的职业
			obj.usProLev = bytes.readUnsignedShort();			//职业等级 	(s下发给对长的)		玩家职业等级
			obj.idPlayer = bytes.readUnsignedInt();				// idPlayer						玩家ID
			obj.idMap = bytes.readUnsignedInt();				// idMap		   地图ID
			obj.idTarget = bytes.readUnsignedInt();				// idTarget		1：队伍ID：		目标ID
			obj.szPlayerName = bytes.readMultiByte(16,GameCommonData.CODE);	// szPlayerName[16]	玩家姓名	   （用于游戏显示，和UI无关）
			obj.szTargetName = bytes.readMultiByte(16,GameCommonData.CODE);	//玩家姓名						目标玩家名

			switch(obj.unAction)
			{	
				case _MSGTEAM_CREATE:														//创建了队伍
//					trace("1");
					sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:1});
					sendNotification(EventList.ASKTEAMINFO, obj);
					break;
				case _MSGTEAM_APPLYJOIN:													//收到申请
//					trace("2");
					sendNotification(EventList.ASKTEAMINFO, obj);
					break;
				case _MSGTEAM_LEAVE:														//离开
//					trace("3");
					sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:4, data:obj});
					sendNotification(EventList.ASKTEAMINFO, obj);
					break;
				case _MSGTEAM_ACCEPTINVITE:													//接收邀请
//					trace("4");
					sendNotification(EventList.ASKTEAMINFO, obj);
					break;
				case _MSGTEAM_INVITE:														//被邀请
//					trace("5");
//					trace(obj.idPlayer);
//					trace(obj.idTarget);
//					trace(obj.szPlayerName);
//					trace(obj.szTargetName);
					sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:10, data:obj});
					sendNotification(TeamEvent.HAVAINVITE, obj);
					break;
				case _MSGTEAM_AGREEJOIN:													//自己的申请被接受了
//					trace("6");
					sendNotification(EventList.ASKTEAMINFO, obj);
					break;
				case _MSGTEAM_DISMISS:
//					trace("7");
					sendNotification(EventList.ASKTEAMINFO, obj);
					break;
				case _MSGTEAM_KICKOUT:														//踢出
//					trace("8");
					sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:7, data:obj});
					sendNotification(EventList.ASKTEAMINFO, obj);
					break;
				case _MSGTEAM_CLOSETEAM:
//					trace("9");
					sendNotification(EventList.ASKTEAMINFO, obj);
					break;
				case _MSGTEAM_OPENTEAM:
//					trace("10");
					sendNotification(EventList.ASKTEAMINFO, obj);
					break;
				case _MSGTEAM_JOINTEAM:		
//					trace("11");
					sendNotification(EventList.ASKTEAMINFO, obj);
					break;
				case _MSGTEAM_LEAVETEAM:
//					trace("12");
//					trace(obj.szPlayerName);
					sendNotification(EventList.ASKTEAMINFO, obj);
					break;
				case _MSGTEAM_DENY_APPLY:													//我的申请被拒绝了
//					trace("我的申请被拒");
//					trace(obj.idPlayer);//拒绝者的ID
//					trace(obj.szPlayerName);
					sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:18, data:obj});
					sendNotification(EventList.ASKTEAMINFO, obj);
					break;
				case _MSGTEAM_CHG_LEADER_APPLY:
//					trace("13");
					sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:5, data:obj});
//					trace("-------"+GameCommonData.Player.Role.Id);
					trace("team leader changed, id:", obj.idPlayer, obj.idTarget);
					trace("-----------------------");
					sendNotification(EventList.ASKTEAMINFO, obj);
					break;
				case _MSGTEAM_MEMMBER_APPLY:												//队员向我(我是队长)提出（某人被邀请加入队伍）
//					trace("14");					
					sendNotification(TeamEvent.MEMBERINVITESOMEONE, obj);
					break;
				case _MSGTEAM_DENY_INVITE:													//自己的邀请被拒绝了
//					trace("15");
//					trace(obj.idPlayer);//拒绝者的ID
//					trace(obj.szPlayerName);
					sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:13, data:obj});
					sendNotification(EventList.ASKTEAMINFO, obj);
					break;
				case _MSGTEAM_TARGET_INTEAM:
//					trace("队伍已满");
					sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:12});
					sendNotification(EventList.ASKTEAMINFO, obj);
					break;
				case _MSGTEAM_USER_INTEAM:
//					trace("你已经有队伍了,不能再申请加入其它队伍");
					sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:14});
					sendNotification(EventList.ASKTEAMINFO, obj);
					break;
				case _MSGTEAM_UPDATA_TEAMMEMBER_ONLINESTATE:								//更新队伍成员上下线状态  10/23/2010
					sendNotification(EventList.MEMBER_ONLINE_STATUS_TEAM, {id:obj.idPlayer, state:obj.idMap});
//					trace("someone change:", obj.idPlayer, obj.idMap); 
					break; 
				case _MSGTEAM_ASKINFO:						
//					trace("16");
					sendNotification(EventList.ASKTEAMINFO, obj);
					break;
				default:
//					trace("其他命令-", obj.unAction);
					break;
			}
		}
//		_MSGTEAM_CREATE				=	0,	//TS	//TC
//		_MSGTEAM_APPLYJOIN			=	1,  //TS	
//		_MSGTEAM_LEAVE				=	2,	//TS	//TC
//		_MSGTEAM_ACCEPTINVITE		=	3,	//TS	
//	
//		_MSGTEAM_INVITE				=	4,	//TS	//TC
//		_MSGTEAM_AGREEJOIN			=	5,  //TS	
//		_MSGTEAM_DISMISS			=	6,	//TS	//TC
//		_MSGTEAM_KICKOUT			=	7,	//TS	//TC
//		_MSGTEAM_CLOSETEAM			=	8,	//TS	//TC
//		_MSGTEAM_OPENTEAM			=	9,	//TS    //TC
//	
//		_MSGTEAM_JOINTEAM			=	10,	//TS	某人加入某队
//		_MSGTEAM_LEAVETEAM			=   11,	//		某人离开某队
//		_MSGTEAM_NOTALLOWTEAM		=	12,	//		设置自己为不允许组队
//		_MSGTEAM_OFLEADER			=	13,	//移交队长
//		//20070201修罗:小队换名----------------------
//		_MSGTEAM_CHG_NAME_REQ		=	14,//C2S
//		_MSGTEAM_CHG_NAME_APPLY		=	15,//S2C
//		_MSGTEAM_CHG_NAME_ROOMBROADCAST_REQ	=	16,//C2S
//		_MSGTEAM_CHG_NAME_ROOMBROADCAST_APPLY	=	17,//S2C
//		//-------------------------------------------
//		//20070205修罗:队长变更
//		_MSGTEAM_CHG_LEADER_APPLY	= 18,//S2C
//		//20070207修罗:小队设置变更全队通知
//		_MSGTEAM_CHG_CONFIG			= 19,//S2C
//		
//		//20070307softworms:小队中某人共享状态改变通知
//		_MSGTEAM_CHG_SHARESTATUS_IN			= 20,//进入共享区域
//		_MSGTEAM_CHG_SHARESTATUS_OUT		= 21,//离开共享区域
//		_MSGTEAM_SEARCH_TEAM                = 22,
//		_MSGTEAM_SEARCH_ROLE                = 23,
//		_MSGTEAM_SEARCH_BEGIN,
//		_MSGTEAM_SEARCH_END,
//		_MSGTEAM_TEAM_INFO,
//		_MSGTEAM_DISCRIPT,
//		_MSGTEAM_JOIN_WAIT,
//		_MSGTEAM_INVITE_WAIT,
//		_MSGTEAM_CHANGE_TEXT,
//		_MSGTEAM_DENY_INVITE,
//		_MSGTEAM_MEMADDLIFE_EFFECT,	
		
	}
}