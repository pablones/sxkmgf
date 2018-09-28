package GameUI.Modules.Task.Commamd
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.AutoPlay.Data.AutoPlayData;
	import GameUI.Modules.NPCChat.Proxy.DialogConstData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	
	import Net.ActionProcessor.PlayerAction;
	import Net.ActionProcessor.TaskDetail;
	import Net.Protocol;
	
	import OopsFramework.Debug.Logger;
	
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	/**
	 *  任务服务器模拟器
	 * @author felix
	 */	
	public class TaskSeverMediator extends Mediator
	{
		private var taskDetailProxy:TaskDetail;
		private var playerAction:PlayerAction;
		private var bytes:ByteArray;
		public function TaskSeverMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(mediatorName, viewComponent);
		}
		
		
		public override function listNotificationInterests():Array{
			return [EventList.ENTERMAPCOMPLETE,
					EventList.PLAYER_TASK_LEVEL,
					EventList.DONE_FIRST_TIP
					];	
		}
		
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case EventList.ENTERMAPCOMPLETE:
					this.taskDetailProxy=GameCommonData.GameNets.Gam.ActionList[Protocol.MSG_TASK];
					break;	
				case EventList.PLAYER_TASK_LEVEL:
					var type:uint=uint(notification.getBody());
					if(type==1){
						 if(GameCommonData.Player.Role.Level==2){
						 	//启用 新手帮助中的2级提示 加了图片帮助
						 	if(NewerHelpData.newerHelpIsOpen)
								sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 17);
						 }
						 else if(GameCommonData.Player.Role.Level==8){
						 	sendNotification(EventList.DO_FIRST_TIP, {comfrim:noticeNewer, info:DialogConstData.getInstance().getTipDesByType(6), title:GameCommonData.wordDic[ "often_used_smallTip" ], comfirmTxt:GameCommonData.wordDic[ "mod_task_com_taskse_han_1" ], width:260}); //小提示   我知道了
						 }else if(GameCommonData.Player.Role.Level==10){
						 	sendNotification(EventList.DO_FIRST_TIP, {comfrim:null, info:DialogConstData.getInstance().getTipDesByType(7), title:GameCommonData.wordDic[ "often_used_smallTip" ], comfirmTxt:GameCommonData.wordDic[ "mod_task_com_taskse_han_1" ], width:330}); //小提示   我知道了
						 }else if(GameCommonData.Player.Role.Level==15) { 
							if(AutoPlayData.offLineTime <= 12) {	//第一个成就已过期 则 不提示
								sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 37)
							}
						 }
					}	
					this.taskDetailProxy=GameCommonData.GameNets.Gam.ActionList[Protocol.MSG_TASK];
					var mask:uint=GameCommonData.MaskLow & 2;
					if(mask>0 ||GameCommonData.isNewTaskEnd ){
						return;
					}
					bytes=new ByteArray();
					bytes.endian=Endian.LITTLE_ENDIAN;
					bytes.position=4;
					bytes.writeShort(12);     //功能号
					bytes.writeShort(1);      //结构数量
					
					bytes.writeUnsignedInt(1);  //任务ID
					bytes.writeUnsignedInt(1);  //任务步数
					bytes.writeUnsignedInt(99999);
					//process
					bytes.writeShort(0);
					bytes.writeShort(0);
					bytes.writeShort(0);
					bytes.writeShort(GameCommonData.Player.Role.Level);
					bytes.writeShort(0);
					bytes.writeShort(0);
					// end process
					bytes.writeShort(1);      //已经接任务
					bytes.writeShort(1);      //任务追踪
					bytes.writeUnsignedInt(1);
					taskDetailProxy.Processor(bytes);
	
					Logger.Info(this,"handleNotification",GameCommonData.wordDic[ "mod_task_com_taskse_han_2" ]);    //发初始任务
					break;	
				//第一次进入游戏提示 	
				case EventList.DONE_FIRST_TIP:
					if(GameCommonData.Player.Role.Level>1)return;
					playerAction=GameCommonData.GameNets.Gam.ActionList[Protocol.PLAYER_ACTION];
					bytes=new ByteArray();
					bytes.endian=Endian.LITTLE_ENDIAN;
					
					bytes.writeInt(0);
					bytes.writeInt(1);
					bytes.writeInt(2);
					bytes.writeInt(3);
					bytes.writeInt(4);
					bytes.writeInt(27);
					bytes.writeShort(69);
					bytes.writeShort(0);
					bytes.writeUnsignedInt(1);
					playerAction.Processor(bytes);
					break;
						
			}
		}
			
		/** 通知新手指导10级需手动升级 */
		private function noticeNewer():void
		{
			if(NewerHelpData.newerHelpIsOpen) {
				sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 41);
			}
		}
		
	}
}