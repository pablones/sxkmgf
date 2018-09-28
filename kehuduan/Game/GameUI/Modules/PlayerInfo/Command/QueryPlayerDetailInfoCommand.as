package GameUI.Modules.PlayerInfo.Command
{
	import GameUI.Modules.PlayerInfo.Mediator.CounterWorkerInfoMediator;
	import GameUI.Modules.PlayerInfo.Mediator.PlayerDetailInfoMediator;
	
	import Net.ActionSend.PlayerActionSend;
	import Net.Protocol;
	
	import OopsEngine.Role.GameRole;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class QueryPlayerDetailInfoCommand extends SimpleCommand
	{
		public function QueryPlayerDetailInfoCommand()
		{
			super();
		}
		
		override public function execute(notification:INotification):void{
			var id:*=notification.getBody()["id"];
			var name:String=notification.getBody()["name"];
			
			var counter:CounterWorkerInfoMediator=facade.retrieveMediator(CounterWorkerInfoMediator.NAME) as CounterWorkerInfoMediator;
			if(id==null){
				id=0;
				PlayerDetailInfoMediator.Role=new GameRole();
				PlayerDetailInfoMediator.Role.Name=name;
			}
			if(name==null){
				PlayerDetailInfoMediator.Role=counter.role;
				name="0";
			}			
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = new Array();
			obj.data.push(0);
			obj.data.push(id);         //查询的ID号
 			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(235);          //功能号    //查询其他玩家信息  返回协议号:2091
			obj.data.push(0);
			obj.data.push(name);
			PlayerActionSend.PlayerAction(obj);
		}
		
	}
}