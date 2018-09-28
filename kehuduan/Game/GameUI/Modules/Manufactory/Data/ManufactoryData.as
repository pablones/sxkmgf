package GameUI.Modules.Manufactory.Data
{
	import flash.utils.Dictionary;
	
	public class ManufactoryData
	{
		public static const INIT_MANUFACTORY_UI:String = "INIT_MANUFACTORY_UI";				//初始化加载资源
		public static const SHOW_MANUFACTORY_UI:String = "SHOW_MANUFACTORY_UI";				//打开界面
		public static const CLOSE_MANUFACTORY_UI:String = "CLOSE_MANUFACTORY_UI";			//关闭界面
		public static const CLICK_MANU_SKIRT_CELL:String = "CLICK_MANU_SKIRT_CELL";				//单击下拉列表
		public static const SELECT_EQUIP_MANUFA:String = "SELECT_EQUIP_MANUFA";					//单击选中装备
		public static const SELECT_APPEND_CELL:String = "SELECT_APPEND_CELL";						//单击选中附加材料
		public static const CHANGE_SUCESS_RATE:String = "CHANGE_SUCESS_RATE";					//改变合成概率
		public static const MANUFACTORY_SUCEED:String = "MANUFACTORY_SUCEED";					//打造成功
		public static const IS_MANU_ING:String = "IS_MANU_ING";														//正在打造中
		public static const CLOSE_MANU_ING:String = "CLOSE_MANU_ING";										//终止打造
		
		public static const RESOURCE_FORM_TOOLTIP:String = "RESOURCE_FORM_TOOLTIP";				//资源已被悬浮框加载完成
		
		public static var ResourseIsLoaded:Boolean = false;									//资源是否加载完成
		public static var isReadingBar:Boolean = false;											//是否在读条
		public static var ManufatoryCount:uint = 1;													//打造的装备数量
		public static var curTitleBtn:uint = 0;
		public static var selectAppendType:uint = 0;												//是否选中附加材料，初始为0	
		public static var selectScenoType:uint = 0;													//选中的配方id
		public static var clickScenoType:uint = 0;													//点击的配方
		public static var clickAppendType:uint = 0;													//点击的附加材料
		public static var scenographyList:Array = [];												//已学配方id	
		public static var allInfoDic:Dictionary = new Dictionary();
		public static var isRequestToolTip:Boolean = false;
		public static var isStartLoadSource:Boolean = false;								//是否开始加载资源 
		
		public static var aLifeSkillFam:Array = [ "120","300","600","1250","2500","5000","10000","20000" ];
		
		//筛选限制
		public static var limitType:String;
		public static var limitLevel:String;
	}
}