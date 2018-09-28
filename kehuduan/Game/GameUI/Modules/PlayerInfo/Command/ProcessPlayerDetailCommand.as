package GameUI.Modules.PlayerInfo.Command
{
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class ProcessPlayerDetailCommand extends SimpleCommand
	{
		public function ProcessPlayerDetailCommand()
		{
			super();
		}
		
		override public function execute(notification:INotification):void{
			var obj:Object=notification.getBody();
			sendNotification(PlayerInfoComList.SHOW_PLAYER_DETAILINFO_UI,obj);
		}
		
	}
}