package GameUI.Command
{
	import Net.ActionSend.AccLogin;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;
	
	public class AccLoginCommand extends SimpleCommand
	{
		public function AccLoginCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void 
		{
			
		} 

	}
}