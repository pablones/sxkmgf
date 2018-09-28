package GameUI.Modules.Pk.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Friend.command.MenuEvent;
	import GameUI.Modules.Friend.view.ui.MenuItem;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Pk.Data.PkData;
	import GameUI.Modules.Pk.Data.PkEvent;
	import GameUI.Modules.PlayerInfo.Mediator.SelfInfoMediator;
	import GameUI.Proxy.DataProxy;
	
	import Net.ActionSend.PlayerActionSend;
	
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Utils.Timer;
	
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class PkMediator extends Mediator implements IUpdateable 
	{
		public static const NAME:String = "PkMediator";
		
		private var menu:MenuItem;
		private var dataProxy:DataProxy;
		private var pkSwitch:int;			//临时储存pk状态
		private var startTime:int = 0;		//第一次打开的时间
		private var endTime:int   = 0;		//结束的时间
		private var pkCDState:Boolean = true;//是否可以打开PK
		private var txt_pk_name:TextField;
		
		private var CD:int = 10;		//以分钟为单位
		
		private var timer:Timer = new Timer();		/** 定时器 */
		private var isAddTime:Boolean = true;		/** 是否可以添加心跳*/
//		private const CD:int = 1;		
//		private var cdTime:int = CD-1;
//		private var panelBase:PanelBase;
//		private var pkView:MovieClip;
		public function PkMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					EventList.INITVIEW,
					EventList.SHOWPKVIEW,
					EventList.CLOSEPKVIEW,
					PkEvent.UPDATEDATA,
					PkEvent.GETCDTIME,
					PkEvent.INITTIMER,
					PkEvent.FIGHTSWITCH,
					PkEvent.ADDUPDATE
					]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					var selfInfoMediator:SelfInfoMediator = facade.retrieveMediator( SelfInfoMediator.NAME ) as SelfInfoMediator;
					txt_pk_name = selfInfoMediator.SelfInfoUI.txt_pk_name;
					txt_pk_name.text = PkData.pkNameList[GameCommonData.Player.Role.PkState];
				break;
				
				case EventList.SHOWPKVIEW:
					showPk();
				break;
				
				case PkEvent.UPDATEDATA:
					var pkData:int = notification.getBody() as int;
					var type:String = PkData.dataArr[pkData].data.type;
					if(pkCDState == true)
					{
						if(pkData == pkSwitch)
						{
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pk_med_pkm_hn_1" ] + type, color:0xffff00});		//"你现在的PK状态是"
							GameCommonData.Player.Role.PkState = pkData;
							txt_pk_name.text = PkData.pkNameList[GameCommonData.Player.Role.PkState];
							pkSwitch = 0;
							CD = 10;
						}
						else
						{
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pk_med_pkm_hn_2" ], color:0xffff00});		//"操作失败"
						}
					}
				break;
				/** 重置冷却时间*/
				case PkEvent.INITTIMER:
					isAddTime = true;
					CD = 10;
					pkCDState = true;
					GameCommonData.GameInstance.GameUI.Elements.Remove(this);
					GameCommonData.GameInstance.GameUI.Elements.Add(this);			//添加心跳
				break;
				/** 添加PK倒计时 */
				case PkEvent.ADDUPDATE:
				    if(isAddTime == true)
					{
						timer.DistanceTime = 1000 * 60;		//循环时间为1分钟
						isAddTime = false;
						GameCommonData.GameInstance.GameUI.Elements.Add(this);			//添加心跳
					} 
				break;
				
				
//				//暂时没用
//				case EventList.CLOSEPKVIEW:
//					gcAll();
//				break;
//				
//				//暂时没用
//				case PkEvent.GETCDTIME:
//					this.cdTime = notification.getBody() as int == 0 ? CD : notification.getBody() as int;
//					var date:Date = new Date();
//					startTime = date.getTime();
//					date = null;
//				break;
				/** 打斗时 和平模式 切换成 除恶模式 */
				case PkEvent.FIGHTSWITCH:
					pkSwitch = 1;
					
					sendData(1);
				break;

			}
		}
		
		private var enabled:Boolean = true;
		public function get Enabled():Boolean
		{
			return enabled;
		}
		public function Update(gameTime:GameTime):void
		{
			if(timer.IsNextTime(gameTime))
			{
				CD -= 1;
				if(CD == 0)
				{
					isAddTime = true;
					GameCommonData.GameInstance.GameUI.Elements.Remove(this);
				}
			}
		}
		
		public function get UpdateOrder():int{return 0}			// 更新优先级（数值小的优先更新）
		public function get EnabledChanged():Function{return null};
		public function set EnabledChanged(value:Function):void{};
        public function get UpdateOrderChanged():Function{return null};
        public function set UpdateOrderChanged(value:Function):void{};
		
		private function showPk():void
		{
			if(dataProxy.isPkOpen == false)
			{
				this.menu = new MenuItem(PkData.dataArr);
				GameCommonData.GameInstance.GameUI.addChild(menu);
//				menu.x = 780;
//				menu.y = 180;
				menu.x = 20;
				menu.y = 100;
				dataProxy.isPkOpen = true;
				addLis();
				
				timer.DistanceTime = 1000 * 60;		//循环时间为1分钟
				if(isAddTime == true)
				{
					isAddTime = false;
					GameCommonData.GameInstance.GameUI.Elements.Add(this);			//添加心跳
				} 
				
			}
			else
				gcAll();
		}
		
		private function gcAll():void
		{
			dataProxy.isPkOpen = false;
			menu.stage.removeEventListener(MouseEvent.CLICK , stageHandler);	
			menu.removeEventListener(MenuEvent.Cell_Click , menuHandler);
			GameCommonData.GameInstance.GameUI.removeChild(menu);
		}
		
		private function addLis():void
		{
//			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			menu.stage.addEventListener(MouseEvent.CLICK , stageHandler);	
			menu.addEventListener(MenuEvent.Cell_Click , menuHandler); 
		}
		
//		private function panelCloseHandler(event:Event):void
//		{
//			gcAll();
//		}
		private function handTime():void
		{
			if(CD > 0)
			{
				 pkCDState = false;
				 facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pk_med_pkm_ht_1" ] + CD +GameCommonData.wordDic[ "mod_pk_med_pkm_ht_2" ], color:0xffff00});		//"切换到该模式还需要等待"		"分钟,任何攻击其他玩家的行为均会重置冷却时间"
			}
			else pkCDState = true;
			
//			var date:Date = new Date();
//			if(startTime == 0)
//			{
//				startTime = date.getTime();
//			}
//			endTime = date.getTime();
//			date = null;
//			var passTime:int = int(endTime - startTime)/1000;
//			if(passTime > this.cdTime)
//			{
//				pkCDState = true;
//				this.cdTime = CD;
//				startTime = endTime;
//				endTime = 0;
//			}
//			else 
//			{
//				pkCDState = false;
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:int((this.cdTime - passTime)/60 + 1) + "分钟后才能切换PK模式", color:0xffff00});
//			}
		}
		private function menuHandler(e:MenuEvent):void
		{
			dataProxy.isPkOpen = false;
			switch(e.cell.data.type)
			{
				case GameCommonData.wordDic[ "mod_pk_dat_pkd_1" ]:		//"和平模式"
					if(GameCommonData.Player.Role.Level < 20)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pk_med_pkm_mh" ], color:0xffff00});		//"你等级未到20级，不能PK"
						return;
					}
					pkSwitch = 0;
					
					sendData(0);
				break;
				
				case GameCommonData.wordDic[ "mod_pk_dat_pkd_2" ]:		//"除恶模式"
					if(GameCommonData.Player.Role.Level < 20)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pk_med_pkm_mh" ], color:0xffff00});		//"你等级未到20级，不能PK"
						return;
					}
					pkSwitch = 1;
					
					sendData(1);
				break;
				
				case GameCommonData.wordDic[ "mod_pk_dat_pkd_3" ]:		//"自由模式"
					if(GameCommonData.Player.Role.Level < 20)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pk_med_pkm_mh" ], color:0xffff00});		//"你等级未到20级，不能PK"
						return;
					}
					pkSwitch = 2;
					
					sendData(2);
				break;
				
				case GameCommonData.wordDic[ "mod_pk_dat_pkd_4" ]:		//"帮派模式"
					if(GameCommonData.Player.Role.Level < 20)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pk_med_pkm_mh" ], color:0xffff00});		//"你等级未到20级，不能PK"
						return;
					}
					pkSwitch = 3;
					
					sendData(3);
				break;
				
				case GameCommonData.wordDic[ "mod_pk_dat_pkd_5" ]:		//"关于PK"
					facade.sendNotification(PkEvent.ABOUTPK);
				break;
			}
		}
		
		/** 点击舞台 */
		private function stageHandler(e:MouseEvent):void
		{
			gcAll();
		}
		/** 发送数据 */
		private function sendData(pkSwidth:int):void
		{
			if(pkSwidth == GameCommonData.Player.Role.PkState) 
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pk_med_pkm_sd" ], color:0xffff00});		//"你已经选择了该模式"
				return;
			}
			if(matchNoCd(pkSwidth) == false) 	//如果没有冷却时间，就调用CD方法
			{
				handTime();						//操作CD时间
				if(pkCDState == false)
				{
					if(isAddTime == true)
					{
						isAddTime = false;
						GameCommonData.GameInstance.GameUI.Elements.Add(this);			//添加心跳
					} 
					return;
				} 
			}
			isAddTime = true;
			CD = 0;
			pkCDState = true;
			GameCommonData.GameInstance.GameUI.Elements.Remove(this);
			
			var obj:Object = new Object();
			obj.type = 1010;
			obj.data =          [GameCommonData.Player.Role.Id , 0 , pkSwidth , 0 , 0 , 0 , 208, 0 , 0];
			PlayerActionSend.PlayerAction(obj);
		}
		/** 是否匹配无冷却时间的表*/
		private function matchNoCd(type:int):Boolean
		{
			var isNoCd:Boolean = false;
			if(GameCommonData.Player.Role.PkState == 0) return true;	//和平模式无冷却时间
			for(var i:int = 0; i<PkData.pkStateList.length ; i++)
			{
				if(i == GameCommonData.Player.Role.PkState)
				{
					if(PkData.pkChangeStateList[i] == type)			//已匹配
					{
						isNoCd = true;
					}
				}
			}
			return isNoCd;
		}
	}
}