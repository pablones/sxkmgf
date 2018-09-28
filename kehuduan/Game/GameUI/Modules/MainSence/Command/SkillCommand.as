package GameUI.Modules.MainSence.Command
{
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;
	
	public class SkillCommand extends SimpleCommand
	{
		public static const NAME:String = "SkillCommand";
		
		public function SkillCommand()
		{
			super();
		}
		
		override public function execute(notification:INotification):void
		{
			
		}

	}
}