package GameUI.Modules.Task.Commamd
{
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class ReceiveTaskCommand extends SimpleCommand
	{
		public function ReceiveTaskCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void{
			var id:uint=notification.getBody()["id"];	
			//跑环任务
			if(id>10001000){
				var taskInfo:TaskInfoStruct=GameCommonData.TaskInfoDic[id] as TaskInfoStruct;
				if(taskInfo==null)return;
				if(taskInfo.status==0){
					taskInfo.toName();
					taskInfo.toChangeDesByJob();                //将描述信息与链接改一下
				}
			}
			this.sendNotification(TaskCommandList.SHOW_TASKINFO_UI,{id:id});
		}
		
	}
}