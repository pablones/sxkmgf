package GameUI.Modules.voteCard.Data
{
	import GameUI.Modules.voteCard.Mediator.VoteCardMediator;
	import GameUI.UICore.UIFacade;
	import GameUI.View.Components.LoadingView;
	
	import OopsFramework.Content.Loading.BulkLoader;
	import OopsFramework.Content.Loading.ImageItem;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.ProgressEvent;
	
	public class VoteResource
	{
		private var loader:ImageItem;
		private var main_mc:MovieClip;
		public var loadComplete:Function;
		
		public function VoteResource()
		{
			loader = new ImageItem( GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.VoteCardSWF,
																			BulkLoader.TYPE_MOVIECLIP,"VoteCardView" );
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
			main_mc = new ( loader.GetDefinitionByName( "VoteCardView" ) ) as MovieClip;
			
			var mediator:VoteCardMediator = UIFacade.GetInstance( UIFacade.FACADEKEY ).retrieveMediator( VoteCardMediator.NAME ) as VoteCardMediator;
			mediator.setViewComponent( main_mc );
			VoteData.voteResoureLoaded = true;
			
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( VoteData.SHOW_VOTE_PANEL );
		}
		
		private function gc():void
		{
			loader.removeEventListener( ProgressEvent.PROGRESS,onProgress );
			loader.removeEventListener( Event.COMPLETE,onComplete );
			loader.stop();
			loader = null;
			main_mc = null;
		}

	}
}