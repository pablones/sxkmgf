package GameUI.Modules.Task.Commamd
{
	import Controller.PlayerController;
	import Controller.TransferSceneController;
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	import GameUI.UICore.UIFacade;
	
	import Net.ActionSend.PlayerActionSend;
	
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	import flash.geom.Point;

	public class MoveToCommon extends EventDispatcher
	{
		public function MoveToCommon(target:IEventDispatcher=null)
		{
			super(target);
		}
		
		/**
		 * 自动寻路 
		 * @param mapId
		 * @param tileX
		 * @param tileY
		 * @param taskId:为0就是不用管任务有没有接
		 * @param npcId：为0就是到没有NPC的地方
		 * 
		 */		
		public static function MoveTo(mapId:uint,tileX:uint,tileY:uint,taskId:int,npcId:int):void{
			if(!GameCommonData.Scene.IsFirstLoad)return;
			 
			if(npcId<0){
				GameCommonData.autoPlayAnimalType=-npcId;
				npcId=0;
			}
			 
			if(GameCommonData.Player.IsAutomatism){
				PlayerController.EndAutomatism();
			}
			if(GameCommonData.GameInstance.GameScene.GetGameScene.name != GameCommonData.GameInstance.GameScene.GetGameScene.MapId && GameCommonData.GameInstance.GameScene.GetGameScene.name!="2002")
			{
				if ( int(GameCommonData.GameInstance.GameScene.GetGameScene.MapId) <9000 || int(GameCommonData.GameInstance.GameScene.GetGameScene.MapId)>=10000 )
				{
					UIFacade.UIFacadeInstance.showPrompt( GameCommonData.wordDic[ "mod_task_com_mov_mov_1" ] ,0xffff00);       //副本中不能使用自动寻路
					return;	
				}
			}
			if(GameCommonData.GameInstance.GameScene.GetGameScene.name=="1026"){
				UIFacade.UIFacadeInstance.showPrompt( GameCommonData.wordDic[ "mod_task_com_mov_mov_2" ] ,0xffff00);        //地府中不能使用自动寻路
				return;
			}
			
			if(GameCommonData.GameInstance.GameScene.GetGameScene.name=="1027"){
				UIFacade.UIFacadeInstance.showPrompt( GameCommonData.wordDic[ "mod_task_com_mov_mov_3" ] ,0xffff00);       //公共监狱中不能使用自动寻路
				return;
			}
			if(taskId!=0){
				var taskInfo:TaskInfoStruct=GameCommonData.TaskInfoDic[taskId];
				if(taskInfo==null)return;
				if(taskInfo.status==0 ){
					UIFacade.UIFacadeInstance.showPrompt( GameCommonData.wordDic[ "mod_task_com_mov_mov_4" ] ,0xffff00);          //你还未接收任务
					return;
				}
				//点击任务追踪、任务面板的自动寻路, 通知新手指导
				if(NewerHelpData.newerHelpIsOpen)//新手指导开启状态
					UIFacade.UIFacadeInstance.sendNotification(NewerHelpEvent.TASK_FIND_PATH_NOTICE_NEWER_HELP,{id:taskId}); 
			}
			var mapName:String=GameCommonData.GameInstance.GameScene.GetGameScene.name;
			var realNpcId:uint=npcId;
			var realMapId:uint=mapId;
			switch(mapName){
				case "1034":
				case "1035":
					realNpcId+=3300;
					realMapId+=33;
					break;
				case "1036":
				case "1037":
					realNpcId+=3500;
					realMapId+=35;
					break;
				case "1038":
				case "1039":
					realNpcId+=3700;
					realMapId+=37;
					break;
				case "1040":
				case "1041":
					realNpcId+=3900;
					realMapId+=39;
					break;		
			}
			GameCommonData.isAutoPath=true;
			GameCommonData.targetID=realNpcId;
			var autoHelpObj:Object = NewerHelpData.getAutoPathData();
			if(autoHelpObj != null) {
				NewerHelpData.autoPathData = null;
			}
			
			if(taskId == 311 || taskId == 101) {										//在皇陵副本中寻路
				if(GameCommonData.GameInstance.GameScene.GetGameScene.name == "2002") {
					if(tileX == 77 && tileY == 88) {		//寻路张择端
					    GameCommonData.targetID = 2147483647;
						tileX = 32;
						tileY = 20; 
						GameCommonData.Scene.MapPlayerTitleMove(new Point(tileX,tileY),2,"",autoHelpObj);
						return;
					}
					if(tileX == 124 && tileY == 98) {		//寻路荆轲
						tileX = 32;
						tileY = 43;
						GameCommonData.autoPlayAnimalType = 3091;
						GameCommonData.Scene.MapPlayerTitleMove(new Point(tileX,tileY),2,"",autoHelpObj);
						return;
					} else if(tileX == 124 && tileY == 99) {	//寻路刺客
						tileX = 38;
						tileY = 42;
						GameCommonData.autoPlayAnimalType = 3061;
						GameCommonData.Scene.MapPlayerTitleMove(new Point(tileX,tileY),2,"",autoHelpObj);
						return;
					}
				} else {
					GameCommonData.autoPlayAnimalType = 0;
				}
			}
			
			GameCommonData.Scene.MapPlayerTitleMove(new Point(tileX,tileY),2,String(realMapId),autoHelpObj);
		}
		
		/**
		 * 飞行道具
		 * @param mapId
		 * @param tileX
		 * @param tileY
		 * @param taskId ：为0就是不用管任务有没有接
		 * @param npcId
		 * 
		 */		
		public static function FlyTo(mapId:uint,tileX:uint,tileY:uint,taskId:uint,npcId:int):void{
			if(!GameCommonData.Scene.IsFirstLoad)return;
			if(GameCommonData.Player.IsAutomatism)return;
			
//			if(npcId<0){
//				GameCommonData.autoPlayAnimalType=-npcId;
//				npcId=0;
//			}
			
			if(UIConstData.IS_BUSINESSING){
				UIFacade.UIFacadeInstance.showPrompt( GameCommonData.wordDic[ "mod_task_com_mov_fly_1" ] ,0xffff00);       //跑商中不能使用小飞鞋
				return;
			}
			if(GameCommonData.GameInstance.GameScene.GetGameScene.name != GameCommonData.GameInstance.GameScene.GetGameScene.MapId)
			{ 
				if ( int(GameCommonData.GameInstance.GameScene.GetGameScene.MapId) <9000 || int(GameCommonData.GameInstance.GameScene.GetGameScene.MapId)>=10000 )
				{
					UIFacade.UIFacadeInstance.showPrompt( GameCommonData.wordDic[ "mod_task_com_mov_fly_2" ] ,0xffff00);       //副本中不能使用小飞鞋
					return;	
				}
			}
			
			if(GameCommonData.GameInstance.GameScene.GetGameScene.name=="1026"){
				UIFacade.UIFacadeInstance.showPrompt( GameCommonData.wordDic[ "mod_task_com_mov_fly_3" ] ,0xffff00);        //地府中不能使用小飞鞋
				return;
			}
			
			if(GameCommonData.GameInstance.GameScene.GetGameScene.name=="1027"){
				UIFacade.UIFacadeInstance.showPrompt( GameCommonData.wordDic[ "mod_task_com_mov_fly_4" ] ,0xffff00);         //公共监狱中不能使用小飞鞋
				return;
			}
			if(StallConstData.stallSelfId>0 || UIFacade.UIFacadeInstance.isLookStall() ){
				UIFacade.UIFacadeInstance.showPrompt( GameCommonData.wordDic[ "mod_task_com_mov_fly_5" ] ,0xffff00);         //摆摊或查看摊位时不能使用小飞鞋
				return;
			}
			
			if(UIFacade.UIFacadeInstance.isTrading()){		
				UIFacade.UIFacadeInstance.showPrompt( GameCommonData.wordDic[ "mod_task_com_mov_fly_6" ] ,0xffff00);           //交易中不能使用小飞鞋
				return;
			}
				
			if(taskId!=0){
				var taskInfo:TaskInfoStruct=GameCommonData.TaskInfoDic[taskId];
				if(taskInfo==null)return;
				if(taskInfo.status==0 && taskId!=0){
					UIFacade.UIFacadeInstance.showPrompt( GameCommonData.wordDic[ "mod_task_com_mov_mov_4" ] ,0xffff00);            //你还未接收任务
					return;
				}
			}
			if(!BagData.isHasItem(630006) && GameCommonData.Player.Role.VIP==0){
				UIFacade.UIFacadeInstance.showPrompt( GameCommonData.wordDic[ "mod_task_com_mov_fly_7" ] ,0xffff00);             //你没有小飞鞋
				return;
			}
			
			var mapName:String=GameCommonData.GameInstance.GameScene.GetGameScene.name;
			var realNpcId:uint=npcId;
			var realMapId:uint=mapId;
			switch(mapName){
				case "1034":
				case "1035":
					realNpcId+=3300;
					realMapId+=33;
					break;
					
				case "1036":
				case "1037":
					realNpcId+=3500;
					realMapId+=35;
					break;
				case "1038":
				case "1039":
					realNpcId+=3700;
					realMapId+=37;
					break;
				case "1040":
				case "1041":
					realNpcId+=3900;
					realMapId+=39;
					break;
			}
			
			if(TransferSceneController.IsNeedCheckPKScene(String(mapId))){
				UIFacade.UIFacadeInstance.sendNotification(EventList.SHOWALERT,{comfrim:MoveToCommon.sendFlyToCommand,cancel:new Function(),info: GameCommonData.wordDic[ "mod_task_com_mov_fly_8" ] +getShoeStr(),params:{mapId:realMapId,tileX:tileX,tileY:tileY,taskId:taskId,npcId:realNpcId}});         //即将使用一只小飞鞋到达可强制PK场景<br>
			}else{
				if(GameCommonData.Player.Role.IsAttack){
//					UIFacade.UIFacadeInstance.showPrompt("该状态暂不能使用小飞鞋，请稍候再试",0xffff00);
					//使用小飞鞋改成倒计时模式，如果是战斗状态则开始5秒倒计时，倒计时结束即可使用小飞鞋    2011.1.24  冯
					UIFacade.UIFacadeInstance.sendNotification(EventList.SHOWALERT,{comfrim:MoveToCommon.sendFlyToCommand,cancel:new Function(),info: GameCommonData.wordDic[ "mod_task_com_mov_fly_9" ] +getShoeStr(),params:{mapId:realMapId,tileX:tileX,tileY:tileY,taskId:taskId,npcId:realNpcId},extendsFn:MoveToCommon.noticeNewerHelp,doExtends:0,canDrag:false,countDown:5}); //请确认使用一只小飞鞋到达目标点<br>
				} else {
					UIFacade.UIFacadeInstance.sendNotification(EventList.SHOWALERT,{comfrim:MoveToCommon.sendFlyToCommand,cancel:new Function(),info: GameCommonData.wordDic[ "mod_task_com_mov_fly_9" ] +getShoeStr(),params:{mapId:realMapId,tileX:tileX,tileY:tileY,taskId:taskId,npcId:realNpcId},extendsFn:MoveToCommon.noticeNewerHelp,doExtends:0,canDrag:false});    //请确认使用一只小飞鞋到达目标点<br>
				}
				if(NewerHelpData.newerHelpIsOpen) UIFacade.UIFacadeInstance.sendNotification(NewerHelpEvent.CLICK_FLY_SHOE, 0); 	//通知新手引导
			}
			
		}
		
		/** 根据VIP拼出小飞鞋提示字符串 */
		private static function getShoeStr():String
		{
			var res:String = "";
			var vipLev:uint = GameCommonData.Player.Role.VIP;
			switch(vipLev) {
				case 0:
					var num:uint = BagData.hasItemNum(630006);
					if(num > 0) {
						
						res = "<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_task_com_mov_get_1" ]+"</font><font color='#ffff00'>"+num+"<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_task_com_mov_get_2" ]+"</font>"    //注：剩余小飞鞋     只
					} else {
						//
						res = "<font color='#ff0000'>"+GameCommonData.wordDic[ "mod_task_com_mov_get_3" ]+"</font>"     //注：背包中没有小飞鞋
					}
					break;
				case 1:
					res = "<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_task_com_mov_get_4" ]+"</font><font color='#0098FF'>[VIP]</font><font color='#00ff00'>"+GameCommonData.wordDic[ "mod_task_com_mov_get_5" ]+"</font>"  //注：你是     玩家不会扣除小飞鞋
					break;
				case 2:
					res = "<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_task_com_mov_get_4" ]+"</font><font color='#9727FF'>[VIP]</font><font color='#00ff00'>"+GameCommonData.wordDic[ "mod_task_com_mov_get_5" ]+"</font>"  //注：你是     玩家不会扣除小飞鞋
					break;
				case 3:
					res = "<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_task_com_mov_get_4" ]+"</font><font color='#FF6532'>[VIP]</font><font color='#00ff00'>"+GameCommonData.wordDic[ "mod_task_com_mov_get_5" ]+"</font>"  //注：你是     玩家不会扣除小飞鞋
					break;
				case 4:
					res = "<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_task_com_mov_get_4" ]+"</font><font color='#FF6532'>[VIP]</font><font color='#00ff00'>"+GameCommonData.wordDic[ "mod_task_com_mov_get_5" ]+"</font>"  //注：你是     玩家不会扣除小飞鞋
			}
			return res;
		}
		
		/** 确定使用小飞鞋时，清除当先画面显示的UI面板，防止数据错误 */
		private static function clearUIPanels():void
		{
			UIFacade.UIFacadeInstance.sendNotification(EventList.CLOSE_NPC_ALL_PANEL);
		}
		
		/**
		 * 
		 * @param obj
		 * 
		 */		
		public static function sendFlyToCommand(o:Object):void{
			if(!GameCommonData.Scene.IsSceneLoaded)return;
//			if(GameCommonData.Player.Role.IsAttack){										使用小飞鞋改成倒计时模式，如果是战斗状态则开始5秒倒计时，倒计时结束即可使用小飞鞋    2011.1.24  冯
//				UIFacade.UIFacadeInstance.showPrompt("人物处于交战状态，不能传送",0xffff00);
//				return;
//			} 
			
			clearUIPanels();
			var param:Array=[];
			var obj:Object={};
			obj.type=1010;
			param.push(0);
			param.push(GameCommonData.Player.Role.Id);
			param.push(o.tileX);
			param.push(o.tileY);
			param.push(o.mapId);
			param.push(0);
			param.push(264);
			param.push(0);
			param.push("0");
			obj.data=param;
			PlayerActionSend.PlayerAction(obj);
		}
		
		private static function noticeNewerHelp():void
		{
			if(NewerHelpData.newerHelpIsOpen) UIFacade.UIFacadeInstance.sendNotification(NewerHelpEvent.CLICK_FLY_SHOE, 1); 	//通知新手引导
		}
		
	}
}