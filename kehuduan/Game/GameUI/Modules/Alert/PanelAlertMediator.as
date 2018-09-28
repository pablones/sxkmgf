package GameUI.Modules.Alert
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Alert.Data.AlertData;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class PanelAlertMediator extends Mediator
	{
		public static const NAME:String = "PanelAlertMediator";
		
		private var main_mc:MovieClip;
		private var panelBase:PanelBase;
		private var hint_txt:TextField;
		
		public function PanelAlertMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							AlertData.SHOW_PANEL_ALERT_VIEW
						];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case AlertData.SHOW_PANEL_ALERT_VIEW:
					var info:String = notification.getBody().info;
					var title:String = notification.getBody().title;
					showPanel( info,title );
				break;
			}
		}
		
		private function showPanel( info:String,title:String ):void
		{
			if ( !main_mc )
			{
				main_mc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("NPCChat");
				panelBase = new PanelBase( main_mc,main_mc.width+8,main_mc.height+12 );
				panelBase.SetTitleTxt( title );
				hint_txt = new TextField();
			}
			
			var format:TextFormat = new TextFormat();
			format.size = 12;
			format.font = "宋体";
			format.leading = 3;
//			format.color = 0xffffff;
			
			hint_txt.htmlText = info;
			hint_txt.mouseEnabled = false;
			hint_txt.x = 17;
			hint_txt.y = 20;
			hint_txt.width = 230;
			hint_txt.multiline = true;
			hint_txt.autoSize = TextFieldAutoSize.LEFT;
			hint_txt.wordWrap = true;
			hint_txt.setTextFormat( format );
			main_mc.addChild( hint_txt );
			
			panelBase.x = UIConstData.DefaultPos1.x;
			panelBase.y = UIConstData.DefaultPos1.y;
			panelBase.addEventListener( Event.CLOSE,panelClose );
			GameCommonData.GameInstance.GameUI.addChild( panelBase );
		}
		
		private function panelClose( evt:Event ):void
		{
			panelBase.removeEventListener( Event.CLOSE,panelClose );
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				GameCommonData.GameInstance.GameUI.removeChild( panelBase );
			}
			main_mc = null;
			panelBase = null;
			hint_txt = null;
			facade.removeMediator( NAME );
		}
	}
}