package GameUI.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.View.LibsGameComponent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class LibsComponentMediator extends Mediator
	{
		public static const NAME:String = "LibsComponentMediator";
			
		public function LibsComponentMediator()
		{
			super(NAME, new LibsGameComponent(GameCommonData.GameInstance));
		}
		
		private function get libsComponent():LibsGameComponent
		{
			return viewComponent as LibsGameComponent;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.GETRESOURCE
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.GETRESOURCE:
					var obj:Object = notification.getBody();
					libsComponent.GetLibrary(obj.type, obj.mediator, obj.name);
				break;
			}
		}
	}
}