package GameUI.Modules.Equipment.command
{
	import GameUI.Modules.Equipment.mediator.EquipStrengen;
	import GameUI.View.items.UseItem;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class AddEquipDoubleCommand extends SimpleCommand
	{
		
		public function AddEquipDoubleCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void{
			
		}
	}
}