package GameUI.Modules.Campaign.Data
{
	import flash.display.MovieClip;
	
	public class CampaignData
	{
		public static const INIT_CAMPAIGN:String = "INIT_CAMPAIGN";
		public static const SHOW_CAMPAIGN_PANEL:String = "SHOW_CAMPAIGN_PANEL";
		public static const CLOSE_CAMPAIGN_PANEL:String = "CLOSE_CAMPAIGN_PANEL";
		
		public static const CHANGEPAGE:String = "CHANGEPAGE";		//改变页签
		public static const CLOSECLAENDARVIEW:String = "CLOSECLAENDARVIEW";
		public static const SHOWFASTFINISH:String = "SHOWFASTFINISH";	//显示快速完成面板
		public static const HANDLERDATA:String = "CAMPAIGNDATAHANDLERDATA";	//显示快速完成面板
		
		public static var aCompaignInfo:Array = null;
		public static var campaignTaskData:Array;		//日程表任务数据
		public static var campaignAwardData:Array;		//日程表奖励数据
		
		public static var main_mc:MovieClip;
		public static var yellowFrame:MovieClip;
		public static var calendarAward_mc:MovieClip;
		public static var calendarTask_mc:MovieClip;
		public static var calendarView_mc:MovieClip;
		public static var calendarPage_mc:MovieClip;
		public static var calendarGrid_mc:MovieClip;
		public static var calendarFastFinish_mc:MovieClip;	//快速完成面板
		public static var currentPageNum:int = 0;				//页签
		public static var fastFinishIsOpen:Boolean = false;	//快速完成面板是否打开
		public static var finishList:Array = [0,1,0,1,0,0,0,0,0,0,0,0];		//完成的数组
		public static var calendarViewIsOpen:Boolean = false;	//日程表面板是否打开
	}
}