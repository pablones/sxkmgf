package GameUI.Modules.GotoCopy.data
{
	import Net.ActionProcessor.PlayerAction;
	import Net.ActionSend.PlayerActionSend;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class GotoCopyCommand extends SimpleCommand
	{
		public static const NAME:String = "gotoCopyCommand";
		public static const OPEN_GOTOCOPY_VIEW:String = "openGotoCopyView";
		public static var copyNum:int;//副本编号
		public function GotoCopyCommand()
		{
			super();
		}
		
		override public function execute(noti:INotification):void
		{
			copyNum = int(noti.getBody());
			
			var palyerId:int = GameCommonData.Player.Role.Id;
			var obj:Object={type:1010};
			
			obj.data = [0,palyerId,0,0,0,copyNum,PlayerAction.SEND_GOTOCOPY_INFO,0];//	SEND_GOTOCOPY_INFO == 309
			
			PlayerActionSend.PlayerAction(obj); 
		}
	}
}