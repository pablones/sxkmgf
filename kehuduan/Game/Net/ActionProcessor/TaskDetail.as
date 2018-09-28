package Net.ActionProcessor
{
	import Controller.PlayerController;
	
	import GameUI.Modules.Task.Commamd.TaskCommandList;
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	import GameUI.Modules.Task.Model.TaskProxy;
	import GameUI.MouseCursor.RepeatRequest;
	
	import Net.GameAction;
	
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import OopsFramework.Debug.Logger;
	
	import flash.utils.ByteArray;

	public class TaskDetail extends GameAction
	{
		public function TaskDetail(isUsePureMVC:Boolean=true)
		{			
			super(isUsePureMVC);	
		}
		
		public override function Processor(bytes:ByteArray):void
		{
			bytes.position = 4;
			var dwAction:uint		 = bytes.readUnsignedShort();
			var dwNum:uint  		 = bytes.readUnsignedShort(); 
			var taskID:uint		     = 0;
			var taskPhase:uint		 = 0;
			var lifeTime:uint		 = 0;
			var data1:uint		 = 0;
			var data2:uint		 = 0;
			var data3:uint		 = 0;
			var data4:uint		 = 0;
			var data5:uint		 = 0;
			var data6:uint		 = 0;
			var isRec:uint		 = 0;
			var isFollow:uint    = 0;
			var loopStep:uint    = 0;
			
			RepeatRequest.getInstance().taskCount+=dwNum;
			for(var i:int=0 ; i < dwNum ; i ++)
			{ 
				taskID = bytes.readUnsignedInt();
				taskPhase = bytes.readUnsignedInt();
				lifeTime = bytes.readUnsignedInt();    //99999:不需要时间
				data1 = bytes.readUnsignedShort();
				data2 = bytes.readUnsignedShort();
				data3 = bytes.readUnsignedShort();
				data4 = bytes.readUnsignedShort();
				data5 = bytes.readUnsignedShort();
				data6 = bytes.readUnsignedShort();
				isRec = bytes.readUnsignedShort();     //0:可接任务 1：已接任务 3：已经完成任务  
				isFollow = bytes.readUnsignedShort();   //0:不追踪   1：追踪 
				loopStep=bytes.readUnsignedInt();       //跑环任务步骤
//				loopStep=0;       //跑环任务步骤
				
				var id:uint;
				var taskInfo:TaskInfoStruct;
				var animal:GameElementAnimal;
				var animalCommit:GameElementAnimal;
				var type:uint;
				var key:String;
				
				var npc9046:GameElementAnimal;
				
				if(taskID>10000){
					id=taskID*1000+taskPhase;	
				}else{
					id=taskID*100+taskPhase;
				}
				taskInfo=GameCommonData.TaskInfoDic[id] as TaskInfoStruct;
				if(id>101 && id<200){
					GameCommonData.isNewTaskEnd=true;
				}
				
				if(taskInfo==null)continue;
				if(dwAction == 12)
				{
					if(isRec == 0){	
						Logger.Info(this,"Processor",GameCommonData.wordDic[ "net_ap_td_proc_1" ] + taskID + taskPhase);    //"放弃掉任务或可接的任务"
						if(taskInfo.status!=0){
							taskInfo.status=0;
							sendNotification(TaskCommandList.UPDATETASKTREE,{type:2,id:id});        //取消一个已接的任务
							sendNotification(TaskCommandList.REMOVE_TASK_FOLLOW,taskInfo);
						}
						if(id<10001000)
						{
							sendNotification(TaskCommandList.ADD_ACCEPT_TASK,id);
						}
						
						if ((taskInfo.id > 20001000) && (taskInfo.taskNpcId == 9046)) // 特殊处理：帮派任务，而且与长老有关
						{
							npc9046 = null;
							
							for (key in GameCommonData.SameSecnePlayerList)
							{
								if (PlayerController.GetPlayer(int(key)).Role.MonsterTypeID == 9046)
								{
									npc9046 = GameCommonData.SameSecnePlayerList[key];
								}
							}
							
							animal=npc9046;
							
							if (animal)
							{
								type=TaskProxy.getInstance().getNpcShowTaskType(taskInfo.taskNpcId);
								animal.SetMissionPrompt(type);
							}
						}
						else
						{
							animal=GameCommonData.SameSecnePlayerList[taskInfo.taskNpcId];
							if(animal!=null)
							{
								type=TaskProxy.getInstance().getNpcShowTaskType(taskInfo.taskNpcId);
								animal.SetMissionPrompt(type);
							}
						}
					}	
					if(isRec == 1){
						if(taskInfo.status==1){                         //更新状态
							Logger.Info(this,"Processor",GameCommonData.wordDic[ "net_ap_td_proc_2" ] + taskID + taskPhase + taskInfo.isFollow);            //"更新任务"
						}else if(taskInfo.status==0){                   //接收任务
							Logger.Info(this,"Processor",GameCommonData.wordDic[ "net_ap_td_proc_3" ] + taskID + taskPhase + taskInfo.isFollow);           //"接收任务"
							taskInfo.isFollow=1;
							taskInfo.status=1;
							if(id>10001000){
								taskInfo.loopStep=loopStep;
								taskInfo.toChangeDesByJob();
								taskInfo.toName();
								taskInfo.toPrizeExp();
								taskInfo.toPrizeMoney();
								sendNotification(TaskCommandList.UPDATETASKTREE,{type:1,id:id});
							}else{
								sendNotification(TaskCommandList.UPDATETASKTREE,{type:1,id:id});
								sendNotification(TaskCommandList.REMOVE_ACCEPT_TASK,id);
							}
							
							if ((taskInfo.id > 20001000) && ((taskInfo.taskNpcId == 9046) || (taskInfo.taskCommitNpcId == 9046))) // 特殊处理：帮派任务，而且与长老有关
							{
								npc9046 = null;
								
								for (key in GameCommonData.SameSecnePlayerList)
								{
									if (PlayerController.GetPlayer(int(key)).Role.MonsterTypeID == 9046)
									{
										npc9046 = GameCommonData.SameSecnePlayerList[key];
									}
								}
								
								if (taskInfo.taskNpcId == 9046)
								{
									animal=npc9046;
									
									if (animal)
									{
										type=TaskProxy.getInstance().getNpcShowTaskType(taskInfo.taskNpcId);
										animal.SetMissionPrompt(type);
									}
								}
								else
								{
									animalCommit=npc9046;
									if(animalCommit!=null)
									{
										type = TaskProxy.getInstance().getNpcShowTaskType(taskInfo.taskCommitNpcId);
										animalCommit.SetMissionPrompt(type);
									}
								}
							}
							else
							{
								animal=GameCommonData.SameSecnePlayerList[taskInfo.taskNpcId];
								if(animal!=null)
								{
									type=TaskProxy.getInstance().getNpcShowTaskType(taskInfo.taskNpcId);
									animal.SetMissionPrompt(type);
								}
								animalCommit=GameCommonData.SameSecnePlayerList[taskInfo.taskCommitNpcId];
								if(animalCommit!=null)
								{
									type = TaskProxy.getInstance().getNpcShowTaskType(taskInfo.taskCommitNpcId);
									animalCommit.SetMissionPrompt(type);
								}
							}
							sendNotification(TaskCommandList.SHOW_TASKSPECIFIC_COMMAND,id,"1");		
						}
						//第一个任务等级信息特殊处理
						if(id==101){
							data4=Math.min(10,GameCommonData.Player.Role.Level);
						}	
						sendNotification(TaskCommandList.UPDATE_TASK_PROCESS,{id:id,dataArr:[data1,data2,data3,data4,data5,data6]});
					}
					if(isRec == 2){
						Logger.Info(this,"Processor",GameCommonData.wordDic[ "net_ap_td_proc_4" ] + taskID + taskPhase);             //"完成任务"
						//如果为0就是没有接的。。。。就是服务端脚本错误
						if(taskInfo.status==0)continue;
						sendNotification(TaskCommandList.UPDATETASKTREE,{type:5,id:id});
						taskInfo.status=2;		// taskInfo为空就是脚本问题
						
						if ((taskInfo.id > 20001000) && (taskInfo.taskCommitNpcId == 9046)) // 特殊处理：帮派任务，而且与长老有关
						{
							npc9046 = null;
							
							for (key in GameCommonData.SameSecnePlayerList)
							{
								if (PlayerController.GetPlayer(int(key)).Role.MonsterTypeID == 9046)
								{
									npc9046 = GameCommonData.SameSecnePlayerList[key];
								}
							}
							
							animal=npc9046;
							
							if (animal)
							{
								type=TaskProxy.getInstance().getNpcShowTaskType(taskInfo.taskCommitNpcId);
								animal.SetMissionPrompt(type);
							}
						}
						else
						{
							animal=GameCommonData.SameSecnePlayerList[taskInfo.taskCommitNpcId];
							if(animal!=null){
								//查看是否还有其它的任务
								type=TaskProxy.getInstance().getNpcShowTaskType(taskInfo.taskCommitNpcId);
								animal.SetMissionPrompt(type);			
							}
						}					
							
						sendNotification(TaskCommandList.SHOW_TASKSPECIFIC_COMMAND,id,"2");		
					}
				}
				if(dwAction == 13)
				{
					Logger.Info(this,"Processor",GameCommonData.wordDic[ "net_ap_td_proc_5" ] + taskID);         //"任务删除"
					if(id<10001000){
						delete GameCommonData.TaskInfoDic[id];
					}
				}
			}
		}	
	}
}