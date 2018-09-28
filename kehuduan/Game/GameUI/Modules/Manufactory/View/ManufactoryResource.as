package GameUI.Modules.Manufactory.View
{
	import GameUI.Modules.Manufactory.Data.ManufactoryData;
	import GameUI.Modules.Manufactory.Proxy.ManufatoryProxy;
	import GameUI.UICore.UIFacade;
	import GameUI.View.Components.LoadingView;
	
	import OopsFramework.Content.Loading.BulkLoader;
	import OopsFramework.Content.Loading.ImageItem;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.ProgressEvent;
	import flash.utils.Dictionary;
	
	public class ManufactoryResource
	{
		private var loader:ImageItem;
		private var manufactoryProxy:ManufatoryProxy;
		private var main_mc:MovieClip;
		public var LoadDataComplete:Function;
		
		public function ManufactoryResource()
		{
//			trace( "开始加载打造资源" );
			if ( ManufactoryData.isStartLoadSource ) return;
			ManufactoryData.isStartLoadSource = true;
			loader = new ImageItem( GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.ManufatorySWF,
																			BulkLoader.TYPE_MOVIECLIP,"ManufactoryResource" );
			loader.addEventListener( ProgressEvent.PROGRESS,onProgress );
			loader.addEventListener( Event.COMPLETE,onComplete );
			loader.load();
		}
		
		private function onProgress(evt:ProgressEvent):void
		{
			if ( LoadDataComplete == null )
			{
				LoadingView.getInstance().showLoading();		
			}
		}
		
		private function onComplete(evt:Event):void
		{
			LoadingView.getInstance().removeLoading();
			initData();
			initUI();
			if ( LoadDataComplete != null )
			{
				LoadDataComplete();
				UIFacade.UIFacadeInstance.sendNotification( ManufactoryData.RESOURCE_FORM_TOOLTIP,{view:this.main_mc} );
			}
			else
			{	
				UIFacade.UIFacadeInstance.sendNotification( ManufactoryData.SHOW_MANUFACTORY_UI,{view:this.main_mc} );
			}
			gc();
		}
		
		private function initData():void					//读取数据
		{
			if ( !UIFacade.GetInstance( UIFacade.FACADEKEY ).hasProxy( ManufatoryProxy.NAME ) )
			{
				UIFacade.GetInstance( UIFacade.FACADEKEY ).registerProxy( new ManufatoryProxy () );
			}
			manufactoryProxy = UIFacade.GetInstance( UIFacade.FACADEKEY ).retrieveProxy( ManufatoryProxy.NAME ) as ManufatoryProxy;
			manufactoryProxy.allInfoDic = loader.content.content.manufatoryDic;
			ManufactoryData.allInfoDic = loader.content.content.manufatoryDic;									
//			for ( var key:* in manufactoryProxy.allInfoDic )
//			{
//				if ( manufactoryProxy.allInfoDic[key].classTpye == "stock" )
//				{
//					manufactoryProxy.aStockInfo.push( manufactoryProxy.allInfoDic[key] );
//				}
//				else if ( manufactoryProxy.allInfoDic[key].classTpye == "leather" )
//				{
//					manufactoryProxy.aLeatherInfo.push( manufactoryProxy.allInfoDic[key] );
//				}
//				else if ( manufactoryProxy.allInfoDic[key].classTpye == "refinement" )
//				{
//					manufactoryProxy.aRefinementInfo.push( manufactoryProxy.allInfoDic[key] );
//				}
//			}
			
			//获取附加材料数据
			var appendDic:Dictionary = new Dictionary();
			appendDic = loader.content.content.appendManuDic;
			manufactoryProxy.aAllAppendDic = appendDic;
			for( var appendKey:* in appendDic )
			{
				if ( appendDic[appendKey].kind == "stock" )
				{
					manufactoryProxy.aStockAppend.push( appendDic[appendKey] );
				}
				else if ( appendDic[appendKey].kind == "leather" )
				{
					manufactoryProxy.aLeartherAppend.push( appendDic[appendKey] );
				}
				else if ( appendDic[appendKey].kind == "refinement" )
				{
					manufactoryProxy.aRefinementAppend.push( appendDic[appendKey] );
				}
			}
			ManufactoryData.ResourseIsLoaded = true;
			manufactoryProxy.aStockAppend.sortOn( "level",Array.NUMERIC );
			manufactoryProxy.aLeartherAppend.sortOn( "level",Array.NUMERIC );
			manufactoryProxy.aRefinementAppend.sortOn( "level",Array.NUMERIC );
		}
		
		private function initUI():void
		{
			main_mc = new ( loader.GetDefinitionByName( "ManufatoryMainMC" ) ) as MovieClip;
			manufactoryProxy.skirtListBack = new ( loader.GetDefinitionByName( "SkirtListBackground" ) ) as MovieClip;
			manufactoryProxy.ratioCircle = loader.GetDefinitionByName( "RatioCircle" );
			manufactoryProxy.readingBar = loader.GetDefinitionByName( "YellowReadingBar" );
		}
		
		private function gc():void
		{
			loader.removeEventListener( ProgressEvent.PROGRESS,onProgress );
			loader.removeEventListener( Event.COMPLETE,onComplete );
			loader.stop();
			loader = null;
			main_mc = null;
			manufactoryProxy = null;
		}

	}
}