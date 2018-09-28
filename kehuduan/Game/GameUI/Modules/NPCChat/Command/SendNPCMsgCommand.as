package GameUI.Modules.NPCChat.Command
{
	import GameUI.Proxy.DataProxy;
	
	import Net.ActionSend.Zippo;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class SendNPCMsgCommand extends SimpleCommand
	{	
		private var dataProxy:DataProxy;
			
		public function SendNPCMsgCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void{
			var str:String=notification.getBody().toString();
			var n:uint=uint(str);		
			Zippo.SendAnswerNPC(n);
		}
		
	}
}