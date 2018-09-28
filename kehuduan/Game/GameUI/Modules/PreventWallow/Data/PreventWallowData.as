package GameUI.Modules.PreventWallow.Data
{
	public class PreventWallowData
	{
		public static var PreventWallowIsOpen:Boolean = false;
		
		/** 五小时以上的邮件信息 */
		// 亲爱的玩家您好：您上线时间已经超过5小时，由于您的账号没有通过防沉迷验证，非常抱歉的通知您，您接下来不会获得任何收益！
		public static const sixHours:String = GameCommonData.wordDic[ "mod_pre_dat_pre_1" ];
		/** 3小时到5小时的邮件信息 */
		//  亲爱的玩家您好：您上线时间已经超过3小时，由于您的账号没有通过防沉迷验证，接下来您在御剑江湖中的收益会减半。
		public static const threeHours:String = GameCommonData.wordDic[ "mod_pre_dat_pre_2" ];
		/** 填写信息成功的邮件信息 */
		//  亲爱的玩家您好，您已经通过御剑江湖的防沉迷验证，请您在享受江湖之旅的同时，不要忘了注意休息。祝您游戏愉快^^
		public static const wallowsucceed:String = GameCommonData.wordDic[ "mod_pre_dat_pre_3" ];
		/** 填写信息失败的邮件信息 */
		//  亲爱的玩家您好，由于您填写的资料不符合国家认证的防沉迷标准，所以非常抱歉的通知您，您没有通过御剑江湖的防沉迷验证。超过健康游戏时间后，您将不会获得任何游戏经验。你可以通过点击人物头像旁的防沉迷按钮，进行再一次防沉迷资料填写，以便通过防沉迷验证。
		public static const wallowFail:String = GameCommonData.wordDic[ "mod_pre_dat_pre_4" ];
		/** 是否让按钮闪 */
		public static var mcIsPlay:Boolean = false;
		/** 是否正在提交防沉迷信息 */
		public static var isPosting:Boolean = false;
		/** 心跳单例 */
		public static var PwTimerSingle:Boolean = false;
		
		/**防沉迷公告*/
		public static var PREVENT_CHAT_DATA:Array = [
			"<9_抵制不良游戏 拒绝盗版游戏 注意自我保护				谨防上当受骗 适度游戏益脑 沉迷游戏伤身				合理安排时间 享受健康生活_13>",
			"<9_您累计在线时间已满1小时_13>",
			"<9_您累计在线时间已满2小时_13>"
		];
		
//		public static var PREVENT_CHAT_DATA:Array = [
//			"<9_抵制不良游戏 拒绝盗版游戏 注意自我保护\r谨防上当受骗 适度游戏益脑 沉迷游戏伤身\r合理安排时间 享受健康生活_13>",
//			"<9_您累计在线时间已满1小时_13>",
//			"<9_您累计在线时间已满2小时_13>"
//		];


	}
}