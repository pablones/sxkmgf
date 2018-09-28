package GameUI.Modules.ChangeLine.Command
{
	import Net.ActionProcessor.PlayerAction;
	import Net.ActionSend.PlayerActionSend;
	import Net.Protocol;
	
	import flash.utils.getTimer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;
	
	public class ChgLineSendCommand extends SimpleCommand
	{
		public static const NAME:String = "ChgLineSendCommand";
		
		public function ChgLineSendCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = new Array();
			obj.data.push(0);
			obj.data.push(GameCommonData.Player.Role.Id);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(PlayerAction.HEARTPOINT);
			obj.data.push(0);
			PlayerActionSend.PlayerAction(obj);
			GameCommonData.netDelayStartTime = getTimer();				//开始延迟计时
		}
	}
}