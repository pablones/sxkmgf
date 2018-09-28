package GameUI.Modules.IdentifyTreasure.Data
{
	import GameUI.Modules.IdentifyTreasure.UI.TreaGridUnit;
	
	public class TreasureData
	{
		public static var TreaResourceLoaded:Boolean = false;								//资源是否加载
		public static var isStartLoading:Boolean = false;											//是否开始加载资源
		
		public static var LOAD_TREASURE_RES:String = "LOAD_TREASURE_RES";			//加载资源
		public static var TREA_RES_LOAD_COM:String = "TREA_RES_LOAD_COM";		//资源加载完成
		public static var SHOW_TREASURE_UI:String = "SHOW_TREASURE_UI";				//打开面板
		public static var CLOSE_TREASURE_UI:String = "CLOSE_TREASURE_UI";			//关闭面板
		public static var UPDATE_TREA_PACKAGE_SPACE:String = "UPDATE_TREA_PACKAGE_SPACE";					//更新包裹容量
		
		public static var CREATE_TREA_GRID:String = "CREATE_TREA_GRID";				//创建奖励格子
		public static var CHG_TREA_GRID_DATA:String = "CHG_TREA_GRID_DATA";		//更改格子数据
		
		public static var INIT_TREA_PACKAGE:String = "INIT_TREA_PACKAGE";									//初始化我的宝物包裹
		public static var OPEN_MY_TREA_PACKAGE:String = "OPEN_MY_TREA_PACKAGE";				//打开我的宝物包裹
		public static var CLOSE_MY_TREA_PACKAGE:String = "CLOSE_MY_TREA_PACKAGE";			//关闭我的宝物包裹
		public static var CHANGE_TREA_PACK_DATA:String = "CHANGE_TREA_PACK_DATA";			//更改包裹物品数据
		
		//奖励面板
		public static var SHOW_TREA_AWARD_PANEL:String = "SHOW_TREA_AWARD_PANEL";
		public static var CLOSE_TREA_AWARD_PANEL:String = "CLOSE_TREA_AWARD_PANEL";
		
		//点击单元格
		public static var CLICK_SINGLE_GRID:String = "CLICK_SINGLE_GRID";
		public static var DOUBLE_CLICK_GRID:String = "DOUBLE_CLICK_GRID";
		
		//显示文本区域
		public static var SHOW_HIGHT_HAND_GIVES:String = "SHOW_HIGHT_HAND_GIVES";							//显示高手馈赠内容
		public static var SHOW_RIVERS_HEARD:String = "SHOW_RIVERS_HEARD";											//显示江湖惊闻
		
		public static var RECEIVE_AWARD_TREA:String = "RECEIVE_AWARD_TREA";										//收到奖励
		
		public static var TreasureGrid:Class;
		public static var BlackRectGrid:Class;
		
		public static var selectGrid:TreaGridUnit;						//选中的格子
		
		//奖励
		public static var aJiantuAward:Array;
		public static var aJiankeAward:Array;
		public static var aJianhaoAward:Array;
		
		//包裹数据
		public static var packageDateArr:Array = [];
	}
}