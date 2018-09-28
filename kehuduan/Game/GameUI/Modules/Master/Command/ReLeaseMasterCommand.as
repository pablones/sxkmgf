package GameUI.Modules.Master.Command
{
	import GameUI.ConstData.EventList;
	
	import Net.ActionSend.PlayerActionSend;
	import Net.Protocol;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	//解除师徒关系
	public class ReLeaseMasterCommand extends SimpleCommand
	{
		public static const NAME:String = "ReLeaseMasterCommand";
		private var masterName:String;
		
		public function ReLeaseMasterCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			masterName = notification.getBody().sName;
			var info:String = "你的师傅<font color = '#ffff00'>"+masterName+"</font>要与你解除师徒关系，是否同意？";
			facade.sendNotification(EventList.SHOWALERT, {comfrim:releaseMaster, cancel:cancelClose, info:info, title:"提 示",comfirmTxt:"同 意",cancelTxt:"拒 绝"});
		}
		
		private function releaseMaster():void
		{
			sendInfo(1);
		}
		
		private function cancelClose():void
		{
			sendInfo(2);
		}
		
		private function sendInfo(index:uint):void
		{
			var obj:Object = new Object();
			var parm:Array = [];
			parm.push(0);
			parm.push(GameCommonData.Player.Role.Id);
			parm.push(index);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(226);							//解除师徒关系
			parm.push(0);
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);
		}
		
	}
}