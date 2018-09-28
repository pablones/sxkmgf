package GameUI.Modules.Task.Commamd
{
	import Net.ActionSend.PlayerActionSend;
	import Net.Protocol;
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class RecallTaskCommand extends SimpleCommand
	{
		public function RecallTaskCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void{
			
			var id:uint=notification.getBody()["taskID"];
			
			var step:uint;
			var taskId:uint;
			if(id>10001000){
				step=id%1000;
				taskId=Math.floor(id/1000);
			}else{
				step=id%100;
				taskId=Math.floor(id/100);
			}
			var obj:Object = new Object();
			var parm:Array = new Array;
			parm.push(notification.getBody()["equipId"]);
			parm.push(GameCommonData.Player.Role.Id);
			parm.push(0);  
			parm.push(0); 
			parm.push(step);    //任务Step
			parm.push(taskId);  //任务ID
			parm.push(notification.getBody()["type"]);	
			parm.push(notification.getBody()["petId"]);   //交宠物  与 选择提交精力档次共用字段
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);
		}
		
	}
}