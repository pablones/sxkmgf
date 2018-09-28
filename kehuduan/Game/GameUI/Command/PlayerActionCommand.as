package GameUI.Command
{
	import Controller.SceneController;
	
	import GameUI.ConstData.EventList;
	import GameUI.UICore.UIFacade;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class PlayerActionCommand extends SimpleCommand
	{
		public function PlayerActionCommand()
		{
			super();
			this.initializeNotifier(UIFacade.FACADEKEY)
		}
		
		public override function execute(notification:INotification):void 
		{
			var obj:Object = notification.getBody();
		}
		
		private function playerAction(obj:Object):void
		{
			switch(obj.nAction)
			{
				case 14:
					facade.sendNotification(EventList.ENTERMAPCOMPLETE);
				break;
			}
		}
	}
}