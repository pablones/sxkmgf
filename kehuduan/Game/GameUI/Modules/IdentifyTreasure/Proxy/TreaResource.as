package GameUI.Modules.IdentifyTreasure.Proxy
{
	import GameUI.Modules.IdentifyTreasure.Data.TreasureData;
	import GameUI.UICore.UIFacade;
	import GameUI.View.Components.LoadingView;
	
	import OopsFramework.Content.Loading.BulkLoader;
	import OopsFramework.Content.Loading.ImageItem;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.ProgressEvent;
	
	public class TreaResource
	{
		private var loader:ImageItem;
		
		public function TreaResource()
		{
			if ( TreasureData.isStartLoading ) return;
			TreasureData.isStartLoading = true;
			loader = new ImageItem( GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.OpenTreaSWF,
																			BulkLoader.TYPE_MOVIECLIP,"OpenTreasure" );
			loader.addEventListener( ProgressEvent.PROGRESS,onProgress );
			loader.addEventListener( Event.COMPLETE,onComplete );
			loader.load();
		}
		
		private function onProgress( evt:ProgressEvent ):void
		{
			LoadingView.getInstance().showLoading();	
		}
		
		private function onComplete( evt:Event ):void
		{
			LoadingView.getInstance().removeLoading();
			initData();
		}
		
		//获取外部数据
		private function initData():void
		{
			var obj:Object = new Object();
			obj.needMoneyArr = loader.content.content.needMoneyArr;
			obj.bestAwardArr = loader.content.content.bestAwardArr;
			obj.main_mc = new ( loader.GetDefinitionByName( "MainTearuser" ) ) as MovieClip;
			obj.package_mc = new ( loader.GetDefinitionByName( "TreaPackage" ) ) as MovieClip;				//乾坤袋
			obj.showAward_mc = new ( loader.GetDefinitionByName( "ShowTreasureAward" ) ) as MovieClip;				//奖品显示
			
			//有花边的格子
			TreasureData.TreasureGrid = loader.GetDefinitionByName( "TreasureBox" );
			//没花边的格子
			TreasureData.BlackRectGrid = loader.GetDefinitionByName( "BlackRectBox" );
			
			TreasureData.aJiantuAward = loader.content.content.aJiantuAward;
			TreasureData.aJiankeAward = loader.content.content.aJiankeAward;
			TreasureData.aJianhaoAward = loader.content.content.aJianhaoAward;
			
			TreasureData.TreaResourceLoaded = true;
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( TreasureData.TREA_RES_LOAD_COM,obj );
			
			loader.removeEventListener( ProgressEvent.PROGRESS,onProgress );
			loader.removeEventListener( Event.COMPLETE,onComplete );
			loader.stop();
			loader = null;
		}

	}
}