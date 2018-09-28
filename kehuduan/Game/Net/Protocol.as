package Net
{
	import GameUI.Mediator.UiNetAction;
	
	/** 网络协议 */
	public class Protocol
	{		
		/** 登录账号服务器  */
		public static const LOGIN_ACCSERVER:int  = 1051;
		/** 登录游戏服务器  */
		public static const LOGIN_GSERVER:int    = 1052;
		/** 登录成功 */
		public static const GAMESERVER_INFO:int  = 1052;
		/** 玩家操作 */
		public static const PLAYER_ACTION:int    = 1010;
		/** 玩家发言 */
		public static const PLAYER_CHAT:int      = 1004;
		/** 玩家信息 */
		public static const USER_INFO:int 	     = 1006;
		public static const PLAYER_INFO:int      = 1014;
		/** 玩家使用快捷键 */
		public static const USER_KEY:int 		 = 1066;
//		/** 玩家使用快捷键 */
//		public static const EUDE_INFO:int 		 = 1066;
		/** 走路消息 */
		public static const PLAYER_WALK:int 	 = 3005;
		/** 地图掉落物品（打怪掉落）  */
		public static const PLAYER_MAPITEM:int 	 = 1101;
		/** 自动挂机 */
		public static const PLAYER_AUTOFIGHT:int = 2079;
		/** 组队操作 */
		public static const TEAM_ACTION:int		 = 1023; 
		/** 队伍详细信息 */
		public static const TEAM_INFO:int		 = 1026;
		/** 战斗相互信息 */
		public static const MSG_INTERACT:int	 = 1022;
		
		public static const OPERATE_ITEMS:int 	 = 1009;
		/** 交易 */
		public static const TRADE_INFO:int       = 1056;
		/** 好友  */
		public static const FRIEND_ACTION:int    = 1019;
		/** 游戏属性  */
		public static const USER_ATT:uint 	 = 1017;		
		/** 仓库 */
		public static const DEPOT_OPERATE:int   = 1102;
		 /**人物角色的详细属性  */
		public static const MSG_PLAYDETAIL:uint = 2091;
		/**发送人物技能信息*/
		public static const MSG_S_GAMESKILL:uint = 1010;
		/**接收人物信息列表*/
		public static const MSG_A_GAMESKILL:uint = 1103;
		/**NPC对话*/
		public static const MSG_NPCDIALOG:uint = 2032;	
		/**获取技能信息协议**/
		public static const MSG_MAGICEFFECT:uint = 1105;
		/** 物品详细信息  */
		public static const MSG_ITEMINFO:uint = 1008;
		/** NPC商店 */
		public static const MSG_NPCSHOP:uint = 2092;
		/** NPC跑商商店 */
		public static const MSG_NPCBUSINESS:uint = 2094;
		/** 任务 */
		public static const MSG_TASK:uint = 2050;
		/** 宠物快照 */
		public static const MSG_PET_EUDEMON:uint = 1118;
		/** 宠物详细信息 */
		public static const MSG_PET_INFO_ALL:uint = 1119;
		/** 宠物信息更新 */
		public static const MSG_PET_ATT:uint = 2037;
		/** 商城 */
		public static const MSG_MARKET:uint = 2052;
		
		/**技能与物品cd */
		public static const MSG_COOLDOWN:uint = 2047;
		/**技能BUFF效果*/
		public static const MSG_BUFF:uint = 2049;
		/** 排行榜数据 */
		public static const MSG_TOPLIST:uint = 2048;
		/** 装备通信*/
		public static const MSG_EQUIP:uint=2051;
		/** 创建帮派的操作 */
		public static const MSG_UNITY:uint=1107;
		/** 响应帮派得到的数据 */
		public static const MSG_GETDATAUNITY:uint=2093;
		/** 请求帮派成员信息数据 */
		public static const MSG_GETMENBERLIST:uint=1112;
		/** 请求帮派信息数据 */
		public static const MSG_GETINFOLIST:uint=1114;
		/** 快捷协议 */
		public static const MSG_QUICKSWITCH:uint=1066;
		/** 师徒协议 */
		public static const MASTER_LIST_ACTION:uint=2038;
		/** 帮派分堂数据协议*/
		public static const MSG_GETINFOSUB:uint= 1120;
		/** 队伍位置消息  */
		public static const MSG_TEAM_POS:uint=1029;
		/** 账号保护密码协议 */
		public static const MSG_ENCRYPT:uint = 1068;
		/** 创建角色广播通告协议 */
		public static const MSG_REGISTER:uint = 1001;
		/** 自动挂机协议 */
		public static const MSG_AUTO_PLAY:uint = 2079;  
		/** 称号协议 */ 
		public static const MSG_DESIGNATION:uint = 1030;
		/** 开箱子协议 */
		public static const MSG_OPEN_TREASURE:uint = 1031;
		/**副本验证协议 */
		public static const MSG_OPEN_GOTOCOPY:uint = 1121;
		/**经脉协议 */
		public static const MSG_MERIDIANS:uint = 1205;
		/**武魂详细信息 */
		public static const MSG_SOUL_DETAIL:uint = 2070;
		/**武魂快照 */
		public static const MSG_SOUL_PICTURE:uint = 2072;
		/**vip */
		public static const MSG_VIP_SHOW:uint = 1069; 
		/**宠物蛋 */
		public static const MSG_PET_EGG:uint = 1206; 
		/**竞技场比分 */
		public static const ARENA_SCORE:uint = 2064;
		/**充值等级*/
		public static const MSG_GENERAL:uint = 1300;

		/** 补偿仓库物品列表 */
		public static const MSG_COMPENSATE_LIST:uint = 2095;
		/** 补偿仓库日志 */
		public static const MSG_COMPENSATE_LOG:uint = 2096;
	}
}