package GameUI.Modules.CompensateStorage.mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.CompensateStorage.data.CompensateStorageConst;
	import GameUI.Modules.CompensateStorage.data.CompensateStorageData;
	import GameUI.Modules.CompensateStorage.view.CompensateStorageViewManager;
	
	import flash.display.Loader;
	import flash.events.Event;
	import flash.net.URLRequest;
	import flash.system.ApplicationDomain;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class CompensateStorageMediator extends Mediator
	{
		public static const NAME:String="CompenstateStorageMediator";
		
		private var loader:Loader;
		
		/** true：正在加载中 */
		private var loading:Boolean = false;
		/** true：加载成功 */
		private var loadSucceed:Boolean = false;
		/** 存放临时数据 */
		private var data:Object = null;
		
		private var viewManager:CompensateStorageViewManager;
		
		public function CompensateStorageMediator()
		{
			super(NAME);
			viewManager = new CompensateStorageViewManager();
		}

		public override function listNotificationInterests():Array
		{
			return [
				CompensateStorageConst.SHOW_COMPENSATESTORGE_VIEW,
				CompensateStorageConst.CLOSE_COMPENSATESTORGE_VIEW,
				CompensateStorageConst.SHOW_COMPENSATESTORGE_DETAILS_VIEW,
				CompensateStorageConst.CLOSE_COMPENSATESTORGE_DETAILS_VIEW,
				CompensateStorageConst.GET_COMPENSATESTORGE_ITEM,
				EventList.CLOSE_NPC_ALL_PANEL,
				];
		}

		public override function handleNotification(notification:INotification):void
		{
			switch (notification.getName())
			{
				case CompensateStorageConst.SHOW_COMPENSATESTORGE_VIEW:
					show(notification.getBody());
					
					break;
				case CompensateStorageConst.CLOSE_COMPENSATESTORGE_VIEW:
					viewManager.close();
					break;
				case CompensateStorageConst.SHOW_COMPENSATESTORGE_DETAILS_VIEW:
					viewManager.showDetails();
					break;
				case CompensateStorageConst.CLOSE_COMPENSATESTORGE_DETAILS_VIEW:
					viewManager.closeDetails();
					break;
				case CompensateStorageConst.GET_COMPENSATESTORGE_ITEM:
					CompensateStorageData.getOut();
					break;
				case EventList.CLOSE_NPC_ALL_PANEL:
					viewManager.close();
					break;
			}
		}
		
		private function show(body:Object = null):void
		{
			if(!loadSucceed)
			{
				load();
				data = body;
			} 
			else
			{
				viewManager.show(body);
			}
		}
		
		/* 加载资源 */
		private function load():void
		{
			if( loading || loadSucceed )	return;
			loading = true;
			loader = new Loader();
			var request:URLRequest = new URLRequest();
			request.url = GameCommonData.GameInstance.Content.RootDirectory + CompensateStorageData.resourcePath;
			
			loader.contentLoaderInfo.addEventListener( Event.COMPLETE, onComplete );
			loader.load( request );
		}
		
		private function onComplete( event:Event ):void
		{
			CompensateStorageData.domain = event.target.applicationDomain as ApplicationDomain;
			
			CompensateStorageData.petListView=loader.contentLoaderInfo.content["petListView"] as Array;
			CompensateStorageData.otherSiteList = loader.contentLoaderInfo.content["otherSiteList"] as Array;
			
			loader.contentLoaderInfo.removeEventListener( Event.COMPLETE, onComplete );
			loader = null;
			loading = false;
			loadSucceed = true;
			show(data);
		}
	}
}