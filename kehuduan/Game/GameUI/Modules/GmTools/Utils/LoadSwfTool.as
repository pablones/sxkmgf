package GameUI.Modules.GmTools.Utils
{
	import GameUI.View.Components.LoadingView;
	
	import OopsFramework.Content.Loading.BulkLoader;
	import OopsFramework.Content.Loading.ImageItem;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.ProgressEvent;
	
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	public class LoadSwfTool
	{
		public var sendShow:Function;				/** 完成后显示UI*/
		
		private var _mediator:Mediator;
		public function LoadSwfTool(path:String , mediator:Mediator)
		{
			_mediator = mediator;
//			_isLoadedComplete = isLoadedComplete;
			var picLoader:ImageItem = new ImageItem(GameCommonData.GameInstance.Content.RootDirectory + path, 
																	BulkLoader.TYPE_MOVIECLIP ,
																	path);
			picLoader.addEventListener(ProgressEvent.PROGRESS , onProgressHandler);
			picLoader.addEventListener(Event.COMPLETE, onPicComplete);
			picLoader.load();
		}
		/** 加载中 */
		private function onProgressHandler(e:Event):void
		{
			LoadingView.getInstance().showLoading();
		}
		/** 下载项完成事件 */
		private function onPicComplete(e:Event):void 
		{
			LoadingView.getInstance().removeLoading();
			var picLoader:Object  = e.target as Object;
			var view:MovieClip = picLoader.content.content as MovieClip;
			picLoader.destroy();
			picLoader.removeEventListener(Event.COMPLETE, onPicComplete);
			if(sendShow != null) sendShow(view);
			view = null;
		}

	}
}