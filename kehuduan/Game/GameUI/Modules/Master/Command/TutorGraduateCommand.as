package GameUI.Modules.Master.Command
{
	import GameUI.ConstData.EventList;
	
	import Net.ActionSend.PlayerActionSend;
	import Net.Protocol;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	//徒弟出师
	public class TutorGraduateCommand extends SimpleCommand
	{
		public static const NAME:String = "TutorGraduateCommand";
		
		public function TutorGraduateCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			facade.sendNotification(EventList.SHOWALERT, {comfrim:onComfrim, cancel:cancelClose, info:"是否要现在出师？", title:"提 示"});
		}
		
		private function onComfrim():void
		{
//			RequestTutor.requestTutorAction(79);								//发送徒弟确认
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
			parm.push(79);							//请求师徒列表
			parm.push(0);
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);
		}
		
	}
}