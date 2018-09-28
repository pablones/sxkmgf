package GameUI.Modules.PrepaidLevel.Mediator
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.AutoResize.Data.AutoSizeData;
	import GameUI.Modules.PrepaidLevel.Data.PrepaidUIData;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class TravelHelpMediator extends Mediator
	{
		public static const NAME:String = "travelHelpMediator";
		private var panelBase:PanelBase;
		
		public function TravelHelpMediator()
		{
			super(NAME, viewComponent);
		}
		
		private function get travelHelp():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					PrepaidUIData.SHOW_TRAVELHELP_VIEW,
					PrepaidUIData.CLOSE_TRAVELHELP_VIEW
					];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case PrepaidUIData.SHOW_TRAVELHELP_VIEW:
					showView();
					PrepaidUIData.travelIsOpen = true;
					break;
				case PrepaidUIData.CLOSE_TRAVELHELP_VIEW:
					closeUI(null);
					break;
			}
		}
		
		private function showView():void
		{
			if( panelBase == null )
			{
				this.viewComponent = new ( PrepaidUIData.travelHelp ) as MovieClip;
				panelBase = new PanelBase(travelHelp, 378, travelHelp.height+12);
				panelBase.SetTitleTxt("帮助");
			}
			panelBase.addEventListener( Event.CLOSE, closeUI );
			(travelHelp.close_btn as SimpleButton).addEventListener( MouseEvent.CLICK, closeUI );
			AutoSizeData.setStartPoint( panelBase, UIConstData.DefaultPos2.x, UIConstData.DefaultPos2.y, 3 );
			GameCommonData.GameInstance.GameUI.addChild( panelBase );
		}
		
		private function closeUI( e:Event ):void
		{
			panelBase.removeEventListener( Event.CLOSE, closeUI );
			(travelHelp.close_btn as SimpleButton).removeEventListener( MouseEvent.CLICK, closeUI );
			GameCommonData.GameInstance.GameUI.removeChild( panelBase );
			PrepaidUIData.travelIsOpen = false;
		}
	}
}