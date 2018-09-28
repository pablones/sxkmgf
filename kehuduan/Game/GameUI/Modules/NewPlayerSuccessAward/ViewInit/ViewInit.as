package GameUI.Modules.NewPlayerSuccessAward.ViewInit
{
	import GameUI.Modules.NewPlayerSuccessAward.Data.NewAwardEvent;
	import GameUI.Modules.NewPlayerSuccessAward.Mediator.NewAwardMediator;
	import GameUI.View.Components.LoadingView;
	
	import OopsFramework.Content.Loading.BulkLoader;
	import OopsFramework.Content.Loading.ImageItem;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.ProgressEvent;

	public class ViewInit
	{
		private var view:MovieClip;
		public function ViewInit()
		{
			var picLoader:ImageItem = new ImageItem(GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.NewPlayerAward, 
																	BulkLoader.TYPE_MOVIECLIP ,
																	"ViewInit");
			picLoader.addEventListener(ProgressEvent.PROGRESS , onProgressHandler);
			picLoader.addEventListener(Event.COMPLETE, onPicComplete);
			picLoader.load();
		}
		/** 加载中 */
		private function onProgressHandler(e:Event):void
		{
			LoadingView.getInstance().showLoading();
			(GameCommonData.UIFacadeIntance.retrieveMediator(NewAwardMediator.NAME) as NewAwardMediator).isLoadedComplete(false);
		}
		/** 下载项完成事件 */
		private function onPicComplete(e:Event):void 
		{
			(GameCommonData.UIFacadeIntance.retrieveMediator(NewAwardMediator.NAME) as NewAwardMediator).isLoadedComplete(true);
			LoadingView.getInstance().removeLoading();
			var picLoader:Object  = e.target as Object;
			view = picLoader.content.content as MovieClip;
			picLoader.destroy();
			picLoader.removeEventListener(Event.COMPLETE, onPicComplete);
			GameCommonData.UIFacadeIntance.sendNotification(NewAwardEvent.SHOWNEWPLAYERAWARD , view);				//显示新手成就面板
		}
		
	}
}