package GameUI.Modules.GotoCopy.Mediator
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.GotoCopy.ListContainer;
	import GameUI.Modules.GotoCopy.data.GotoCopyCommand;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.LoadingView;
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.ResourcesFactory;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class GotoCopyMediator extends Mediator
	{
		public static const NAME:String = "gotoCopyMediator";
		private var panelBase:PanelBase;
		private var POS:Point;
		private var friendData:Array;	//好友数据
		private var listContainer:ListContainer; //滚动容器
		private var scroPanell:UIScrollPane;//滚动框
		/**副本条件类表*/
		private var _data:Array;
		/**玩家列表*/
		private var _pdata:Array;
		public function GotoCopyMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME);
		}
		
		private function get mainView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		override public function listNotificationInterests():Array
		{
			return [
				GotoCopyCommand.OPEN_GOTOCOPY_VIEW
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case GotoCopyCommand.OPEN_GOTOCOPY_VIEW:
					this._pdata = notification.getBody() as Array;
					if(!panelBase)
					{
						ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/judgeCopy.swf",onLoadComplete);
						LoadingView.getInstance().showLoading();
					}
					else
					{
						showView();
					}
				break;

			}
		}
		
		private function onLoadComplete():void
		{
			LoadingView.getInstance().removeLoading();
			var gcMc:MovieClip = ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/judgeCopy.swf");
			this.setViewComponent(gcMc);
			_data = gcMc.data;
			POS = new Point(16,47);
			panelBase = new PanelBase(gcMc, gcMc.width-9, gcMc.height + 12 );
			panelBase.name = "gotoCopyPanel";
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			panelBase.x = UIConstData.DefaultPos1.x+200;
			panelBase.y = UIConstData.DefaultPos1.y;
			panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_got_med_got_onL" ] );	//"提示框"
			showView();
		}
		
		private function showView():void
		{
			if(GameCommonData.GameInstance.GameUI.contains(this.panelBase))
			{ 
				panelCloseHandler(null);
				return;
			}
			(this.mainView.btn_seeYou as SimpleButton).addEventListener(MouseEvent.CLICK,seeYouHandler);
			initContent();
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
		}
		
	
		/**
		 *	列表渲染 
		 * 
		 */		
		private function initContent():void
		{
			
//			var tempArr:Array = UIUtils.DeeplyCopy(_data) as Array;
			if(listContainer)
			{
				listContainer.update(_data[GotoCopyCommand.copyNum],_pdata);
			}
			else
			{
				listContainer = new ListContainer(_data[GotoCopyCommand.copyNum],_pdata,226,235);
				this.mainView.addChild(listContainer); 
				listContainer.x = POS.x;
				listContainer.y = POS.y;
			}
			if(_data[GotoCopyCommand.copyNum][0])
			{
				panelBase.SetTitleTxt(_data[GotoCopyCommand.copyNum][0]);
			}
		}
		
		
		private function seeYouHandler(me:MouseEvent):void
		{
			panelCloseHandler(null);
		}
		private function panelCloseHandler(e:Event):void
		{
			(this.mainView.btn_seeYou as SimpleButton).removeEventListener(MouseEvent.CLICK,seeYouHandler);
			GameCommonData.GameInstance.GameUI.removeChild(panelBase); 
		}
	}
}