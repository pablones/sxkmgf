package GameUI.Modules.Unity.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Proxy.DataProxy;
	
	import Net.ActionSend.UnityActionSend;
	
	import flash.utils.clearInterval;
	import flash.utils.setInterval;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class CreatintMediator extends Mediator
	{
		public static const NAME:String = "CreatintMediator";
		private var dataProxy:DataProxy ;
		private var respondNum:int = 1;
		private var isTimeOn:Boolean = false;
		
		/** 副本时间警告 */
		private var time:int;
		private var timeupId:uint;
		public function CreatintMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					EventList.SHOWUNITYVIEW,
					EventList.CLOSEUNITYVIEW,
					UnityEvent.UPDATECREATINGDATA,
					EventList.TIMEUP
			   		]
		} 
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.SHOWUNITYVIEW:
					UnityConstData.iscreating = int(GameCommonData.Player.Role.unityJob-1) / 100;										//如果职业除以100得到0，则创建成功，为1，则正在创建中
					if(UnityConstData.iscreating!= 0 && GameCommonData.Player.Role.unityJob == 1100)
					{
						dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
						dataProxy.UnityIsOpen = false;
						sendAction(208 , 0 , 0);																					//向服务器请求响应该帮派的人数
					}
				break;
				
				case EventList.CLOSEUNITYVIEW:
					if(UnityConstData.iscreating != 0 && GameCommonData.Player.Role.unityJob == 1100)
					{
					}
				break;
				case UnityEvent.UPDATECREATINGDATA:
				   	 respondNum = notification.getBody() as int;
				   	 facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_cim_han_1" ]+respondNum+GameCommonData.wordDic[ "mod_uni_med_cim_han_2" ], color:0xffff00}); // 你创建的帮派已有  个人响应
//					facade.sendNotification(EventList.SHOWALERT, {comfrim:applyTrade, cancel:null, isShowClose:false, info: "你创建的帮派已有"+respondNum+"个人响应", title:"提 示", comfirmTxt:"确定", cancelTxt:null});
				break;
				/** 副本时间警告*/
				case EventList.TIMEUP:
					time = (notification.getBody() as Object).taskId;
					timeupId = setInterval(timeset , 1000);
				break;
				
			}
		}
		private function applyTrade():void
		{
			dataProxy.UnityIsOpen = false;
		}
		/** 发送请求响应人数的方法 */
		private function sendAction(type:int , currentPage:int , id:int):void
		{
			if(isTimeOn == true) 	//有时间限制
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_cim_sen_1" ], color:0xffff00});  // 两次操作需要间隔10秒
				return;					
			}
			UnityConstData.unityObj.type = 1107;														//协议号
			UnityConstData.unityObj.data = [0 , 0 , type, currentPage , id];
			UnityActionSend.SendSynAction(UnityConstData.unityObj);										//发送创建请求
			isTimeOn = true;
			setTimeout(ontime , 1000 * 10);				//十秒后撤销限制
		}
		
		private function ontime():void
		{
			isTimeOn = false;
		}
		/** 副本时间提示 */
		private function timeset():void
		{
			time -= 1 ;
			facade.sendNotification(HintEvents.RECEIVEINFO, {info:String(time), color:0xffff00});
			if(time <= 0) clearInterval(timeupId);
		}
	}
}