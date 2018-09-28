package GameUI.Modules.Task.Commamd
{
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class UpdateMaskCommand extends SimpleCommand
	{
		public function UpdateMaskCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void{
			
		}
		
	}
}