package GameUI.Modules.Task.Commamd
{
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class UpdateLevelTaskCommand extends SimpleCommand
	{
		public function UpdateLevelTaskCommand()
		{
			super();
		}
		
		
		/**
		 * 人物等级的更新对环任务的影响 
		 * @param notification
		 * 
		 */		
		public override function execute(notification:INotification):void{
			for(var id:* in GameCommonData.TaskInfoDic){
				if(id>10001000){
					var taskInfo:TaskInfoStruct=GameCommonData.TaskInfoDic[id] as TaskInfoStruct;
					if(taskInfo && (taskInfo.status==1 || taskInfo.status==3)){
						taskInfo.toPrizeExp();
						taskInfo.toPrizeMoney();
						taskInfo.toChangeDesByJob();
						sendNotification(TaskCommandList.UPDATE_TASK_PROCESS_VIEW,taskInfo);
					}
				}
			}
		}
		
	}
}