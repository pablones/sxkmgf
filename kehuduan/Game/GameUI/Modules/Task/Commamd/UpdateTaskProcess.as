package GameUI.Modules.Task.Commamd
{
	import Controller.PlayerController;
	
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	
	/**
	 * 更新任务状态 
	 * @author net
	 * 
	 */	
	public class UpdateTaskProcess extends SimpleCommand
	{
		public function UpdateTaskProcess()
		{
			super();
		}
		
		public override function execute(notification:INotification):void{
			var taskInfo:TaskInfoStruct=GameCommonData.TaskInfoDic[notification.getBody()["id"]];
			if(taskInfo==null)return;
			var dataArr:Array=notification.getBody()["dataArr"] as Array;
			var mask:uint;
			if(taskInfo.taskProcess1!=""){
				taskInfo.taskProcess1=this.process(taskInfo.taskProcess1,dataArr[0]);
				if(this.checkIsFinish(taskInfo.taskProcess1)){
					mask+=1;
				}	
			}
			if(taskInfo.taskProcess2!=""){
				taskInfo.taskProcess2=this.process(taskInfo.taskProcess2,dataArr[1]);
				if(this.checkIsFinish(taskInfo.taskProcess2)){
					mask+=2;
				}	
			}
			if(taskInfo.taskProcess3!=""){
				taskInfo.taskProcess3=this.process(taskInfo.taskProcess3,dataArr[2]);
				if(this.checkIsFinish(taskInfo.taskProcess3)){
					mask+=4;
				}	
			}
			if(taskInfo.taskProcess4!=""){
				taskInfo.taskProcess4=this.process(taskInfo.taskProcess4,dataArr[3]);
				if(this.checkIsFinish(taskInfo.taskProcess4)){
					mask+=8;
				}	
			}
			if(taskInfo.taskProcess5!=""){
				taskInfo.taskProcess5=this.process(taskInfo.taskProcess5,dataArr[4]);
				if(this.checkIsFinish(taskInfo.taskProcess5)){
					mask+=16;
				}	
			}
			if(taskInfo.taskProcess6!=""){
				taskInfo.taskProcess6=this.process(taskInfo.taskProcess6,dataArr[5]);
				if(this.checkIsFinish(taskInfo.taskProcess6)){
					mask+=32;
				}	
			}
			if(taskInfo.processMask==mask){
				taskInfo.status=3;
				//任务完成状态时，通知新手指导
				if(NewerHelpData.newerHelpIsOpen)//新手指导开启状态
					sendNotification(NewerHelpEvent.TASK_UPDATE_NOCICE_NEWER_HELP,{id:taskInfo.id});
			}else{
				taskInfo.status=1;
			}
			if(taskInfo.processMask==0){
				taskInfo.status=3;
			}
			if (taskInfo.taskCommitNpcId == 9046)
			{
				var npc9046:GameElementAnimal; // 帮派长老
				
				for (var key:String in GameCommonData.SameSecnePlayerList)
				{
					if (PlayerController.GetPlayer(int(key)).Role.MonsterTypeID == 9046)
					{
						npc9046 = GameCommonData.SameSecnePlayerList[key];
					}
				}
				
				if (npc9046)
				{
					npc9046.SetMissionPrompt(taskInfo.status);
				}
			}
			else
			{
				var animal:GameElementAnimal=GameCommonData.SameSecnePlayerList[taskInfo.taskCommitNpcId]
				if(taskInfo.status==3 && animal!=null){
					animal.SetMissionPrompt(3);	
				}
			}
			sendNotification(TaskCommandList.UPDATE_TASK_PROCESS_VIEW,taskInfo);
		}
		
		protected function process(source:String,data:String):String{
			if(source==null || source.length==0)return "";
			var tempArr:Array=[];	
			tempArr=source.split("$");
			if(uint(data)>=uint(tempArr[3])){
				tempArr[1]=tempArr[3];
			}else{
				tempArr[1]=data;
			}
			var str:String=""
			for each(var s:String in tempArr){
				str+=s+"$";
			}
			str=str.substr(0,str.length-1);
			return str;
		}
		
		protected function checkIsFinish(source:String):Boolean{
			var tempArr:Array=[];	
			tempArr=source.split("$");
			if(uint(tempArr[1])>=uint(tempArr[3])){
				return true;
			}else {
				return false
			}
				
		}
		
	}
}