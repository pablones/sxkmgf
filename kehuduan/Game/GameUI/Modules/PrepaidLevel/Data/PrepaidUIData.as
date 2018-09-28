package GameUI.Modules.PrepaidLevel.Data
{
	import GameUI.Modules.AutoPlay.Data.AutoPlayData;
	
	import flash.display.MovieClip;
	
	public class PrepaidUIData
	{
		public static const SHOW_PREPAID_VIEW:String = "show_prepaid_view";
		
		public static const SHOW_WISH_VIEW:String = "show_wish_view";
		
		public static const UPDATE_OFFLINE_VIEW:String = "update_offline_view";
		
		public static const UPDATE_TRAVEL_VIEW:String = "update_travel_view";
		
		public static const UPDATE_PREPAID_VIEW:String = "updata_prepaid_view";
		
		public static const SHOW_ADDXIAOYAO:String = "show_addxiaoyao";
		
		public static const INIT_ADDXIAOYAO:String = "init_addxiaoyao";
		
		public static const UPDATE_ADDXIAOYAO:String = "update_addxiaoyao";
		
		public static const CLOSE_ADDXIAOYAO:String = "close_addxiaoyao";
		
		public static const SHOW_LOADINGVIEW:String = "show_loadingView";
		
		public static const SHOW_OFFLINE_VIEW:String = "show_offLine_view";
		
		public static const SHOW_TRAVELHELP_VIEW:String = "show_travelHelp_view";
		
		public static const CLOSE_TRAVELHELP_VIEW:String = "close_travelHelp_view";
		
		public static const CLOSE_PREPAID_VIEW:String = "close_prepaid_view";
		
		public static const stateArr:Array = ["新兵", "下士", "中士", "高阶军士", "士官长", "骑士队长", "军团长", "司令官", "统帅", "帝国将军"];
		
		public static var giftBtn:MovieClip;          //领取礼包按钮
		
		public static var gift:MovieClip;             //礼包
		
		public static var travelHelp:Class;           //游历帮助界面
		
		public static var travelIsOpen:Boolean = false;      //游历帮助界面是否打开
		
		public static var isFirst:Boolean = true;            //是否第一次打开充值连接
		
		/** 计算碎银 */
		public static function countMoney():String
		{
			var offLineExp:uint = AutoPlayData.offLineTime *int(-1.06 * 0.01 * Math.pow(GameCommonData.Player.Role.Level ,3) + 3.26 * Math.pow(GameCommonData.Player.Role.Level , 2) + 32.7 * GameCommonData.Player.Role.Level + 384);
			var money:uint = (140 * GameCommonData.Player.Role.Level + 70) * AutoPlayData.offLineTime;
			var jin:String =  money / Math.pow(100 , 2) > 0 ? ("<font color='#00ff00'>"+int(money / Math.pow(100 , 2))+"</font>\\se"):"";
			var yin:String = int(money % 10000) / Math.pow(100 , 1) > 0 ? ("<font color='#00ff00'>"+int(int(money % 10000) / Math.pow(100 , 1))+"</font>\\ss"):"";
			var tong:String = int(money % 100) / Math.pow(100 , 0) > 0 ? ("<font color='#00ff00'>"+int(int(money % 100) / Math.pow(100 , 0))+"</font>\\sc"):"0";
			var info:String = "<font color='#E2CCA5'>确定要消耗"+jin + yin + tong+"，领取<font color='#FFFF00'>"+ offLineExp * 2 +"</font>经验吗？</font>";
			return info;
		}
	}
}