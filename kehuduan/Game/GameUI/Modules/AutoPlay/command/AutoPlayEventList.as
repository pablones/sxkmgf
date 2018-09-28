package GameUI.Modules.AutoPlay.command
{
	public class AutoPlayEventList
	{
		/** 显示自动挂机界面 */
		public static const SHOW_AUTOPLAY_UI:String="SHOW_AUTOPLAY_UI";
		/** 隐藏自动挂机界面 */
		public static const HIDE_AUTOPLAY_UI:String="hide_autoPlay_UI";	
		/** 添加物品到自动挂机界面 */
		public static const ADD_ITEM_AUTOPLAYUI:String="add_item_autoPlay";
		/** 和背包同步数量*/
		public static const ONSYN_BAG_NUM:String="ONSYN_BAG_NUM";
		/** 人物或宠物的某些属性状态更新（当低于某个值时要自动加++）*/
		public static const ATT_CHANGE_EVENT:String="att_change_event";
		/** 取消自动挂机 */
		public static const CANCEL_AUTOPLAY_EVENT:String="cancel_autoPlay_event";
		/** 显示离线挂机界面 */
		public static const SHOW_OFFLINEPLAY:String = "SHOWOFFLINEPLAY";
		/** 关闭离线挂机界面 */
		public static const CLOSE_OFFLINEPLAY:String = "CLOSE_OFFLINEPLAY";
		/** 得到离线挂机数据 */
		public static const GET_OFFLINEPLAYDATA:String = "GET_OFFLINEPLAYDATA";
		/**挂机消息处理Command*/
		public static const AUTOPLAY_PROCESS_COMMAND:String="autoplay_process_command";
		/**收到服务器消息*/
		public static const AUTO_PLAY_RECEIVE_SERVER:String = "AUTO_PLAY_RECEIVE_SERVER";
		
		/** 禁止拖动自动挂机面板 */
		public static const STOP_DRAG_AUTOPLAY:String = "stop_drag_autoplay";
		/**离线经验是否领取成功*/
		public static const GET_OFFLINEAWARD:String = "GET_OFFLINEAWARD";
		/**开始挂机*/
		public static const START_AUTO_PLAY:String = "START_AUTO_PLAY";
		
		/**从充值等级界面领取离线经验*/
		public static const SHOW_OFFLINEPLAY_FROM_PREPAID:String = "show_offLineplay_from_prepaid";

	}
}