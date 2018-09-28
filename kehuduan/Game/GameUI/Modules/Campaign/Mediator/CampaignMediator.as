package GameUI.Modules.Campaign.Mediator
{
	import GameUI.Modules.Campaign.Data.CampaignData;
	import GameUI.Modules.Campaign.Mediator.UI.CampaignMainView;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class CampaignMediator extends Mediator
	{
		public static const NAME:String = "CampaignMediator";
		private var mainView:CampaignMainView = null;
		private var page_mc:MovieClip;
		private var dataProxy:DataProxy;
		private var panelBase:PanelBase;
		
		public function CampaignMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public override function listNotificationInterests():Array
		{
			return [ CampaignData.INIT_CAMPAIGN,
						 CampaignData.SHOW_CAMPAIGN_PANEL,
						 CampaignData.CHANGEPAGE,
						 CampaignData.CLOSE_CAMPAIGN_PANEL
						 ];
						 
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case CampaignData.INIT_CAMPAIGN:
					initUI();
				break;
				
				case CampaignData.SHOW_CAMPAIGN_PANEL:
					try
					{
						if ( (this.page_mc == null) && (notification.getBody().Main != null) )
						{
							page_mc = notification.getBody().Main;
						}
						showUI();
					}
					catch ( e:Error )
					{
//						trace ("资源未加进来");
					}
				break;
				
				case CampaignData.CHANGEPAGE://切换页签
					if(CampaignData.currentPageNum == 1)
					{
						facade.sendNotification(CampaignData.CLOSECLAENDARVIEW);
						page_mc.addChild(CampaignData.main_mc);
						mainView.activeIsOpen();
					}
				break;
				
				case CampaignData.CLOSE_CAMPAIGN_PANEL:
					panelCloseHandler(null);
				break;
			}
		}
		
		private function initUI():void
		{
			dataProxy = facade.retrieveProxy( DataProxy.NAME ) as DataProxy;
			if ( dataProxy.CampaignPanIsOpen )
			{
				panelCloseHandler(null);
			}
			else
			{
				if ( mainView==null )
				{
					mainView = new CampaignMainView();
				}
				else
				{
					sendNotification( CampaignData.SHOW_CAMPAIGN_PANEL );
				}
			}
		}
		
		private function showUI():void
		{
			if ( panelBase==null )
			{
//				panelBase = new PanelBase(page_mc, page_mc.width+8, page_mc.height+15);
				panelBase = new PanelBase( page_mc, 719, 505 ); 
				panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
				panelBase.x = 170;
				panelBase.y = 10;
				page_mc.y = 45;
				panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_cam_med_cam" ]);//"活  动"
			}
			panelBase.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - panelBase.width) / 2;
			panelBase.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - panelBase.height) / 2;
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			dataProxy.CampaignPanIsOpen = true; 
			addLis();
			for(var i:int = 0; i < 2; i++)
			{
				page_mc["mcPage_" + i].gotoAndStop(2);
			}
			page_mc["mcPage_" + CampaignData.currentPageNum].gotoAndStop(1);
			facade.sendNotification(CampaignData.CHANGEPAGE , page_mc);
		}
		private function addLis():void
		{
			for(var i:int = 0; i < 2; i++)
			{
				page_mc["mcPage_" + i].addEventListener(MouseEvent.CLICK , changePageHandler);
			}
			
		}
		private function changePageHandler(e:MouseEvent):void
		{
			var page:int = e.target.name.split("_")[1];
			if(CampaignData.currentPageNum == page) return;
			for(var i:int = 0; i < 2; i++)
			{
				page_mc["mcPage_" + i].gotoAndStop(2);
			}
			e.target.gotoAndStop(1);
			CampaignData.currentPageNum = page;
			page_mc.removeChildAt(page_mc.numChildren - 1);
			facade.sendNotification(CampaignData.CHANGEPAGE , page_mc);
		}
		private function panelCloseHandler(evt:Event):void
		{
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase) )
			{
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			}
			dataProxy.CampaignPanIsOpen = false;
			for(var i:int = 0; i < 2; i++)
			{
				page_mc["mcPage_" + i].removeEventListener(MouseEvent.CLICK , changePageHandler);
			}
			facade.sendNotification(CampaignData.CLOSECLAENDARVIEW);			//关闭日程表
		}
		
	}
}