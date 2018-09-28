package GameUI.Modules.VipHeadIcon.view.ui
{
	import GameUI.View.Components.LoadingView;
	
	import OopsFramework.Content.Loading.BulkLoader;
	import OopsFramework.Content.Loading.ImageItem;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.ProgressEvent;
	
	public class VhiLoadTool
	{
		private var loader:ImageItem;
		public var sendShow:Function;//加载完成后的回调函数
		public function VhiLoadTool(path:String)
		{
			loader = new ImageItem(GameCommonData.GameInstance.Content.RootDirectory + path, 	
											BulkLoader.TYPE_MOVIECLIP ,	path);
			loader.addEventListener(ProgressEvent.PROGRESS,progerssHandler);		
			loader.addEventListener(Event.COMPLETE,completeHander);
			loader.load();					
		}
		/**加载过程中*/
		private function progerssHandler(pe:ProgressEvent):void
		{
			LoadingView.getInstance().showLoading();
		}
		/**加载完成*/
		private function completeHander(e:Event):void
		{
			LoadingView.getInstance().removeLoading();
			var view:MovieClip = loader.content.content as MovieClip;
			var gridClass:Class = loader.GetDefinitionByName("headGrid");
			loader.destroy();
			loader.removeEventListener(ProgressEvent.PROGRESS,progerssHandler);
			loader.removeEventListener(Event.COMPLETE, completeHander);
			if(sendShow != null) 
			sendShow(view,gridClass);
		}


	}
}