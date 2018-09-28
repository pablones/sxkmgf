package GameUI.Modules.Campaign.Mediator.UI
{
	import GameUI.Modules.Campaign.Data.CampaignData;
	import GameUI.Modules.Campaign.Event.CampaignEvent;
	import GameUI.Modules.Task.View.TaskText;
	import GameUI.View.Components.LoadingView;
	import GameUI.View.Components.UIScrollPane;
	
	import OopsFramework.Content.Loading.BulkLoader;
	import OopsFramework.Content.Loading.ImageItem;
	
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.ProgressEvent;
	
	public class CampaignMainView
	{
//		private var main_mc:MovieClip;
		private var cell_mc:MovieClip;
		private var bg_mc:MovieClip;
//		private var calendarAward_mc:MovieClip;
//		private var calendarTask_mc:MovieClip;
//		private var calendarView_mc:MovieClip;
		private var loader:ImageItem;
		private var aCell:Array;
		private var cellContainer:Sprite;
//		private var yellowFrame:MovieClip;
		private var contentTxt:TaskText;
//		private var scroll:UIScrollPane;
		private var width:Number = 695;
		private var height:Number = 297;
		
		public function CampaignMainView()
		{
			loader = new ImageItem( GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.CampaignSWF,
																			BulkLoader.TYPE_MOVIECLIP,"CampaignMainView" );
			loader.addEventListener( ProgressEvent.PROGRESS,onProgress );
			loader.addEventListener( Event.COMPLETE,onComplete );
			loader.load();
		}
		
		private function onProgress(evt:ProgressEvent):void
		{
			LoadingView.getInstance().showLoading();
		}
		
		private function onComplete(evt:Event):void
		{
			LoadingView.getInstance().removeLoading();
			initData();
			initUI();
			GameCommonData.UIFacadeIntance.sendNotification(CampaignData.SHOW_CAMPAIGN_PANEL,{Main:CampaignData.calendarPage_mc});
		}
		
		private function initData():void
		{
			CampaignData.aCompaignInfo = loader.content.content.campaignArr;
			CampaignData.campaignTaskData = loader.content.content.calendarData;		//日程表数据
			CampaignData.campaignAwardData = loader.content.content.awardData;
		}
		
		private function initUI():void
		{
			CampaignData.main_mc = new (loader.GetDefinitionByName("CampaignPanel")) as MovieClip;
			CampaignData.yellowFrame = new (loader.GetDefinitionByName("YellowFrameSelect")) as MovieClip;
			CampaignData.calendarAward_mc = new (loader.GetDefinitionByName("CalendarAward")) as MovieClip;
			CampaignData.calendarTask_mc = new (loader.GetDefinitionByName("CalendarTask")) as MovieClip;
			CampaignData.calendarView_mc = new (loader.GetDefinitionByName("CalendarView")) as MovieClip;
			CampaignData.calendarPage_mc	=  new (loader.GetDefinitionByName("CalendarPage")) as MovieClip;
			CampaignData.calendarGrid_mc	= new (loader.GetDefinitionByName("CalendarGrid")) as MovieClip;
			CampaignData.calendarFastFinish_mc	= new (loader.GetDefinitionByName("CalendarFastFinish")) as MovieClip;
			this.contentTxt = new TaskText( 675 );
			contentTxt.x = 22;
//			contentTxt.y = 322;
			contentTxt.y = 355;
			CampaignData.main_mc.addChild(contentTxt);
			creatCells();
		}
		
		private function creatCells():void
		{
			aCell = [];
			cellContainer = new Sprite();
			
			for ( var i:uint=0; i<CampaignData.aCompaignInfo.length; i++ )
			{
				this.bg_mc = new (loader.GetDefinitionByName("BgGray")) as MovieClip;
				this.cell_mc = new (loader.GetDefinitionByName("CampaignCell")) as MovieClip;
			
				var cell:CampaignCell = new CampaignCell( cell_mc,bg_mc,CampaignData.aCompaignInfo[i] );
				cell.x = 0;
				cell.y =1+16.5*i;
				aCell.push(cell);
				cellContainer.addChild(cell);
				cell.addEventListener( CampaignEvent.CLICK_CELL,onCellClick );
			}
			cellContainer.graphics.beginFill( 0xff0000,0 );
			cellContainer.graphics.drawRect(0,0,width,height);
			cellContainer.graphics.endFill();
			
			var scroll:UIScrollPane = new UIScrollPane(cellContainer);
			scroll.x = 10;
			scroll.y = 33.5;
			scroll.width = width;
			scroll.height = height;
			scroll.refresh();
			CampaignData.main_mc.addChild( scroll );
		}
		
		private function onCellClick(evt:CampaignEvent):void
		{
			contentTxt.tfText = evt.remark;
			( evt.target as DisplayObjectContainer ).addChild( CampaignData.yellowFrame );
		}
		
		public function activeIsOpen():void
		{
//			trace ("小时是： "+GameCommonData.gameHour,"   分钟  "+GameCommonData.gameMinute);
			for ( var i:uint=0; i<aCell.length; i++ )
			{
				aCell[i].checkTime();
			}
		}
//		/**
//		 * 切换页签 
//		 */
//		public static function set page(vaule:int):void
//		{
//			if(vaule == 2) GameCommonData.UIFacadeIntance.sendNotification(CampaignData.SHOW_CAMPAIGN_PANEL,{Main:main_mc});
//			if(vaule == 1) GameCommonData.UIFacadeIntance.sendNotification(CampaignData.SHOWCLAENDARVIEW,{Main:calendarView_mc , Task:calendarTask_mc , Award:calendarAward_mc});
//		}
	}
}