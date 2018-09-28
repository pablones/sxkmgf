package GameUI.Modules.Task.Model
{
	import GameUI.Modules.Task.Mediator.TaskMediator;
	import GameUI.UICore.UIFacade;
	
	import flash.utils.Dictionary;
	
	/**
	 *  任务数据模型代理
	 * @author felix
	 * 
	 */	
	
	public class TaskProxy
	{
		private static var _instance:TaskProxy;
		private var taskMediator:TaskMediator;
		public var linkDesDic:Dictionary=new Dictionary();
		public var taskAnimalDic:Dictionary=new Dictionary();
		
		public function TaskProxy()
		{
			linkDesDic[1]='<a href="event:1023,21,54,@,2301">'+GameCommonData.wordDic[ "mod_task_mod_taskp_tas_1" ]+'\\fx</a>';          //唐门：唐研
			linkDesDic[2]='<a href="event:1021,42,73,@,2101">'+GameCommonData.wordDic[ "mod_task_mod_taskp_tas_2" ]+'\\fx</a>';        //全真：王重阳
			linkDesDic[4]='<a href="event:1022,44,78,@,2201">'+GameCommonData.wordDic[ "mod_task_mod_taskp_tas_3" ]+'\\fx</a>';          //峨嵋：鸿陵
			linkDesDic[8]='<a href="event:1024,29,57,@,2401">'+GameCommonData.wordDic[ "mod_task_mod_taskp_tas_4" ]+'\\fx</a>';        //丐帮：金不换
			linkDesDic[16]='<a href="event:1025,22,49,@,2501">'+GameCommonData.wordDic[ "mod_task_mod_taskp_tas_5" ]+'\\fx</a>';         //少林：玄慈
			linkDesDic[32]='<a href="event:1020,25,48,@,2001">'+GameCommonData.wordDic[ "mod_task_mod_taskp_tas_6" ]+'\\fx</a>';	      //点苍：柳沧樱
			
			taskAnimalDic[10001033]={type:501};	//兔子
			taskAnimalDic[10001034]={type:519};	//狸猫
			taskAnimalDic[10001035]={type:505};	//松鼠
			taskAnimalDic[10001048]={type:502};	//鸭子
			taskAnimalDic[10001049]={type:506};	//青蛙
			taskAnimalDic[10001050]={type:504};	//野猪
			taskAnimalDic[10001063]={type:522};	//云雀
			taskAnimalDic[10001064]={type:536};	//野猪
			taskAnimalDic[10001078]={type:534};	//蝎子
			taskAnimalDic[10001079]={type:524};	//狼
			taskAnimalDic[10001093]={type:515};	//鹿
			taskAnimalDic[10001094]={type:539};	//山羊
			taskAnimalDic[10001108]={type:530};	//猴子
			taskAnimalDic[10001109]={type:513};	//猫 
			
			
			//后来新加
		    taskAnimalDic[20001073]={type:501};	//兔子
			taskAnimalDic[20001074]={type:519};	//狸猫
			taskAnimalDic[20001075]={type:505};	//松鼠
			taskAnimalDic[20001076]={type:501};	//兔子
			taskAnimalDic[20001077]={type:519};	//狸猫
			taskAnimalDic[20001078]={type:505};	//松鼠
			taskAnimalDic[20001079]={type:502};	//鸭子
			taskAnimalDic[20001080]={type:506};	//青蛙
			taskAnimalDic[20001081]={type:504};	//野猪
			taskAnimalDic[20001082]={type:502};	//鸭子
			taskAnimalDic[20001083]={type:506};	//青蛙
            taskAnimalDic[20001084]={type:504};	//野猪
			taskAnimalDic[20001085]={type:522};	//云雀
			taskAnimalDic[20001086]={type:536};	//野狗
			taskAnimalDic[20001087]={type:534};	//蝎子 
			taskAnimalDic[20001088]={type:522};	//云雀
			taskAnimalDic[20001089]={type:536};	//野狗
			taskAnimalDic[20001090]={type:534};	//蝎子
			taskAnimalDic[20001091]={type:524};	//狼
			taskAnimalDic[20001092]={type:515};	//鹿
			taskAnimalDic[20001093]={type:539};	//山羊
			taskAnimalDic[20001094]={type:524};	//狼
			taskAnimalDic[20001095]={type:515};	//鹿
			taskAnimalDic[20001096]={type:539};	//山羊
			taskAnimalDic[4105]={type:501};	//兔子
		}
		
		public static function getInstance():TaskProxy{
			if(_instance==null){
				_instance=new TaskProxy();
			}
			return _instance;
		}
		
		/**
		 * 获得Npc显示任务标志的类型（头上的标志） 
		 * @param npcId :Npc的Id号
		 * @return  0：无 1：未完成 2：未接 3：完成
		 * 
		 */		 	
		public function getNpcShowTaskType(npcId:uint):uint{
			var realNpcId:uint=npcId;
			var mapName:String=GameCommonData.GameInstance.GameScene.GetGameScene.name;
			switch(mapName){
				case "1034":
				case "1035":
					realNpcId-=3300;
					break;
					
				case "1036":
				case "1037":
					realNpcId-=3500;
					
					break;
				case "1038":
				case "1039":
					realNpcId-=3700;
					break;
				case "1040":
				case "1041":
					realNpcId-=3900;
					break;
			}
			
			taskMediator=UIFacade.UIFacadeInstance.retrieveMediator(TaskMediator.NAME) as TaskMediator;
			var type:uint=0;
			var accpetedArr:Array=[];
			var accpetArr:Array=[];
			if(taskMediator!=null){
				accpetedArr=taskMediator.taskTree.dataProvider;
				accpetArr=taskMediator.accTaskTree.dataProvider;
				type=this.getAccpetedType(accpetedArr,realNpcId);
				type=Math.max(this.getAccpetType(accpetArr,realNpcId),type);
			}
			return type;
		}
		
		/**
		 * 从已接任务数组中返回优先级高的类型 
		 * @param arr
		 * @return 
		 * 
		 */		
		private function getAccpetedType(arr:Array,npcId:uint):uint{
			var type:uint=0;
			
			if(arr==null || arr.length==0)return type;
			
			for each(var group:TaskGroupStruct in arr){
				var dic:Dictionary=group.taskDic;
				for(var id:* in dic){
					if(GameCommonData.TaskInfoDic[id]!=null){
						var taskInfo:TaskInfoStruct=GameCommonData.TaskInfoDic[id];
						if(taskInfo.taskCommitNpcId==npcId){  //已经接了的任务，讨论其提交NPC的类型
							if(taskInfo.status==3){
								type=3;
							}else{
								type=Math.max(1,type);
							}
						}
					}
				}
			}
			
			return type;
		}
		
		/**
		 * 从可接任务数组中返回优先级高的类型 
		 * @param arr
		 * @return 
		 * 
		 */	
		private function getAccpetType(arr:Array,npcId:uint):uint{
			var type:uint=0;
			if(arr==null || arr.length==0)return type;
			for each(var group:TaskGroupStruct in arr){
				var dic:Dictionary=group.taskDic;
				for(var id:* in dic){
					if(GameCommonData.TaskInfoDic[id]!=null){
						var taskInfo:TaskInfoStruct=GameCommonData.TaskInfoDic[id];
						if(taskInfo.taskNpcId==npcId){  //还未接，讨论其接爱NPC的类型
							type=2;
							break;
						}
					}
				}
			}
			
			return type;
		}
	}
}