package GameUI.Modules.Equipment.command
{
	import Net.ActionSend.EquipSend;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class ComposeCommand extends SimpleCommand
	{
		public static const NAME:String = "ComposeCommand";
		public function ComposeCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			var num:uint = notification.getBody().idStoneArr.length;
			var data:Array=[];
			data.push(num);
			for(var i:uint = 0;i < num;i++)
			{
				data.push(notification.getBody().idStoneArr[i]);
			}
			data.push(73);
			data.push(notification.getBody().idFu);
			data.push(num);
			trace("发的是什么 " +data);
			EquipSend.createMsgCompound(data);
		}
		
	}
}