package GameUI.Modules.CompensateStorage.view
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.CompensateStorage.data.CompensateStorageConst;
	import GameUI.Modules.CompensateStorage.data.CompensateStorageData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.CompensateStorageSend;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	public class CompensateStorageViewManager implements CSViewManager
	{
		public static var MAXPAGE:int = 3;
		
		private var panelBase:PanelBase;
		private var mainView:MovieClip;
		
		private var currentViewManager:CSViewManager;
		private var itemViewManager:CompensateStorageItemViewManager;
		private var petViewManager:CompensateStoragePetViewManager;
		private var otherViewManager:CompensateStorageOtherViewManager;
		private var viewManagerList:Array = new Array();
		private var detailsViewManager:CompensateStorageDetailsViewManager;
		
		public function CompensateStorageViewManager()
		{
			
		}

		public function init():void
		{
			var i:int;
			if( CompensateStorageData.domain.hasDefinition( "CompensateStorage" ) )
			{
				var CompensateStorage:Class = CompensateStorageData.domain.getDefinition( "CompensateStorage" ) as Class;
				mainView = new CompensateStorage();
			}
			panelBase = new PanelBase( mainView,mainView.width + 24 ,mainView.height + 12 );
			panelBase.SetTitleTxt( CompensateStorageData.CompensateStorageName );
			panelBase.name = "CompensateStorageView";
			itemViewManager = new CompensateStorageItemViewManager(mainView);
			petViewManager = new CompensateStoragePetViewManager(mainView);
			otherViewManager = new CompensateStorageOtherViewManager(mainView);
			detailsViewManager = new CompensateStorageDetailsViewManager(mainView);
			viewManagerList[0] = itemViewManager;
			viewManagerList[1] = petViewManager;
			viewManagerList[2] = otherViewManager
			
			currentViewManager = itemViewManager;
			
			for(i = 0;i < MAXPAGE;i++)
			{
				(mainView["CompensateStorage_mcPage_"+i] as MovieClip).buttonMode    = true;
				(mainView["CompensateStorage_mcPage_"+i] as MovieClip).useHandCursor = true;
			}
			selectPage(0);
			CompensateStorageData.isInitView = true;
		}
		
		public function show(body:Object = null):void
		{
			if(!CompensateStorageData.isInitView) init();
			if(body is int)
			{
				var index:int = body as int;
				selectPage(index);
				CompensateStorageData.currentPage = index;
				if(currentViewManager != viewManagerList[index])
				{
					currentViewManager.close();
					currentViewManager = viewManagerList[index];
				}
				CompensateStorageData.openPageIndex = index;
			}
			
			currentViewManager.show();

			if(!CompensateStorageData.isShowView)
			{
				GameCommonData.GameInstance.GameUI.addChild( panelBase );
				addLis();
				CompensateStorageData.isShowView = true;
				if (GameCommonData.fullScreen == 2)
				{
					panelBase.x=UIConstData.DefaultPos1.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth) / 2 + 160;
					panelBase.y=UIConstData.DefaultPos1.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight) / 2 + 20;
				}
				else
				{
					panelBase.x=UIConstData.DefaultPos1.x + 160;
					panelBase.y=UIConstData.DefaultPos1.y + 25;
				}
			}
			panelBase
		}
		
		private function onClick(event:MouseEvent):void
		{
			switch( event.target.name )
			{
				case "CompensateStorage_details":
					if(CompensateStorageData.isShowDetailsView)
					{
						closeDetails();
					}
					else
					{
						showDetails();
					}
					break;	
				case "CompensateStorage_getOut":
					if(CompensateStorageData.openPageIndex == 2)
					{
						var i:int = 0;
						for(i = 0; i < CompensateStorageData.dataList.length; i++)
						{
							if(CompensateStorageData.dataList[i].type >= 30000000)
							{
								CompensateStorageData.selectedId = CompensateStorageData.dataList[i].id;
								break;
							}
						}
					}
					if(CompensateStorageData.openPageIndex == 0 && CompensateStorageData.selectedId == 0)
					{
						GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:CompensateStorageData.word11/**"请先点选一样道具"*/, color:0xffff00});
					}
					if(CompensateStorageData.openPageIndex == 1 && CompensateStorageData.selectedId == 0)
					{
						GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:CompensateStorageData.word12/**"请先点选一个宠物"*/, color:0xffff00});
					}
					if(CompensateStorageData.openPageIndex == 2 && CompensateStorageData.selectedId == 0)
					{
						GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:CompensateStorageData.word13/**"当前没有可以领取的东西"*/, color:0xffff00});
					}
					CompensateStorageData.getOut();
					currentViewManager.show();
					break;	
			}
		}
		
		public function showDetails():void
		{
			if(CompensateStorageData.isEmptyOrUpdata)
			{
				CompensateStorageData.clearLog();
				CompensateStorageSend.logSend(null);
			}else
			{
				detailsViewManager.show();
			}
		}
		
		public function closeDetails():void
		{
			detailsViewManager.close();
		}
		
		private function onSelectPage(event:MouseEvent):void
		{
			var index:uint = uint(event.target.name.split("_")[2]);
			show(index);
		}
		
		private function selectPage(index:uint):void
		{
			if(index == CompensateStorageData.currentPage) return;
			
			for(var i:int = 0; i < MAXPAGE; i++) {
				mainView["CompensateStorage_mcPage_"+i].gotoAndStop(2);
			}
			mainView["CompensateStorage_mcPage_"+index].gotoAndStop(1);
			CompensateStorageData.currentPage = index;
		}
		
		private function addLis():void
		{
			var i:int;
			panelBase.addEventListener( Event.CLOSE, close );
			
			mainView.CompensateStorage_details.addEventListener(MouseEvent.CLICK,onClick);
			mainView.CompensateStorage_getOut.addEventListener(MouseEvent.CLICK,onClick);
			for(i = 0;i < MAXPAGE;i++)
			{
				(mainView["CompensateStorage_mcPage_"+i] as MovieClip).addEventListener(MouseEvent.CLICK,onSelectPage);
			}
		}
		
		private function removeLis():void
		{
			var i:int;
			panelBase.removeEventListener( Event.CLOSE, close );
			mainView.CompensateStorage_details.removeEventListener(MouseEvent.CLICK,onClick);
			mainView.CompensateStorage_getOut.removeEventListener(MouseEvent.CLICK,onClick);
			for(i = 0;i < MAXPAGE;i++)
			{
				(mainView["CompensateStorage_mcPage_"+i] as MovieClip).removeEventListener(MouseEvent.CLICK,onSelectPage);
			}
		}
		
		public function close(event:Event = null):void
		{
			if(CompensateStorageData.isShowView)
			{
				GameCommonData.GameInstance.GameUI.removeChild( panelBase )
				removeLis();
				CompensateStorageData.isShowView = false;
			} 
			if(CompensateStorageData.isShowDetailsView)
			{
				GameCommonData.UIFacadeIntance.sendNotification(CompensateStorageConst.CLOSE_COMPENSATESTORGE_DETAILS_VIEW);
			}
		}
	}
}