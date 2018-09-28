package GameUI.Modules.Relive.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Maket.Data.MarketConstData;
	import GameUI.Modules.Maket.Data.MarketEvent;
	import GameUI.Modules.Relive.Data.ReliveEvent;
	import GameUI.Proxy.DataProxy;
	import GameUI.UICore.UIFacade;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ShowMoney;
	import GameUI.View.items.UseItem;
	
	import Net.ActionSend.Zippo;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	import flash.geom.Point;
	import flash.utils.Timer;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class ReliveMediator extends Mediator
	{
		public static const NAME:String  = "ReliveMediator";
		private var panelBase:PanelBase  = null;
		private var backMask:Sprite		 = null;
		private var mcType:int 			 = 0;
		private var reliveTimer:Timer	 = new Timer(1000, 5);	//高级死亡
//		private var reliveTimerTwn:Timer = new Timer(1000, 5);	//低级死亡
		private var dataProxy:DataProxy;
		
		public function ReliveMediator()
		{
			super(NAME);
		}
		
		private function get reliveView():MovieClip
		{
			return this.viewComponent as MovieClip
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				ReliveEvent.SHOWRELIVE,				//显示复活面板
				ReliveEvent.SHOWBREAK,				//显示断线面板
				ReliveEvent.REMOVERELIVE,			//移除面板
				ReliveEvent.RELIVEBACK,				//回程复活
				ReliveEvent.RELIVENOW,				//原地满血复活
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case ReliveEvent.SHOWRELIVE:									//显示面板
					if(!dataProxy) dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					if(GameCommonData.Player.Role.Level && GameCommonData.Player.Role.Level <= 20) {
						viewComponent = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ReliveNewerView");
						mcType = 1;
					} else if(GameCommonData.Player.Role.Level && GameCommonData.Player.Role.Level > 20){
						viewComponent = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ReliveOlderView");
						mcType = 2;
					}
					UIConstData.KeyBoardCanUse = false;		// 禁用快捷键
					UIFacade.GetInstance(UIFacade.FACADEKEY).closeOpenPanel();	//死亡时关闭其他面板
					sendNotification(EventList.CLOSE_NPC_ALL_PANEL);			//死亡时关闭所有NPC打开的面板
					showView();
					addLis();
					reliveTimer.reset();
					reliveTimer.start();
					break;
				case ReliveEvent.REMOVERELIVE:									//移除面板
					gc();
					break;
				case ReliveEvent.RELIVEBACK:									//回程复活
				
					break;
				case ReliveEvent.RELIVENOW:										//原地满血复活
				
					break;
				case ReliveEvent.SHOWBREAK:										//显示断线面板
					facade.sendNotification(EventList.SHOWALERT, {comfrim:applyConnect, isShowClose:false, info: GameCommonData.wordDic[ "mod_relv_med_rm_hn_1" ], comfirmTxt:GameCommonData.wordDic[ "mod_relv_med_rm_hn_2" ]});  //"你已断开连接"  "重新登录"  
					break;
			}
		}
		
		/** 重新登录 */
		private function  applyConnect():void
		{
//			facade.sendNotification(HintEvents.RECEIVEINFO, {info:"请刷新本页面", color:0xffff00});		//调用页面JavaScript返回指定页面
			try
			{
				ExternalInterface.call("function refresh(){window.location.reload();}");
			}
			catch (e:Error)
			{
				
			}
		}
		
		private function showView():void
		{
			if(mcType == 1) {	//免费复活界面
				panelBase = new PanelBase(viewComponent as MovieClip, viewComponent.width-7, viewComponent.height+13);
			} else {			//付费复活界面
				panelBase = new PanelBase(viewComponent as MovieClip, viewComponent.width+8, viewComponent.height+13);
				initGrid();
			}
			panelBase.name = "RelivePanel";
			var pos:Point = getPos(); 
			panelBase.x = pos.x;
			panelBase.y = pos.y;
			panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_relv_med_rm_sv" ]);  //"死亡复活"
			panelBase.disableClose();
			
			backMask = new Sprite();
			backMask.name = "deadMask";
			backMask.graphics.beginFill(0xffffff, 0);
			backMask.graphics.drawRect(0,0,GameCommonData.GameInstance.GameUI.width, GameCommonData.GameInstance.GameUI.height);
			backMask.graphics.endFill();
			GameCommonData.GameInstance.GameUI.addChild(backMask);
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			if(dataProxy.TradeIsOpen) {
				sendNotification(EventList.REMOVETRADE);
			}
		}
		
		private function initGrid():void
		{
			//快速购买
			for(var j:int = 0; j < 1; j++) {
				if(!(UIConstData.MarketGoodList[26] as Array)[j]) continue;
				var good:Object = (UIConstData.MarketGoodList[26] as Array)[j];
				var unit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				unit.x = 331;
				unit.y = 10;
				unit.name = "goodQuickBuy"+j+"_"+good.type;
				reliveView.addChild(unit);
				
				var useItem:UseItem = new UseItem(j, good.type, reliveView);
				if(good.type < 300000) {
					useItem.Num = 1;
				}
				else if(good.type >= 300000) {
					useItem.Num = UIConstData.getItem(good.type).amount; 
				}
				useItem.x = unit.x + 2;
				useItem.y = unit.y + 2;
				useItem.Id = UIConstData.getItem(good.type).id;
				useItem.IsBind = 0;
				useItem.Type = good.type;
				useItem.IsLock = false;
				
				reliveView.addChild(useItem);
				
				reliveView["txtGoodNamePet_"+j].text = good.Name;
				reliveView["mcMoney_"+j].txtMoney.text = good.PriceIn + MarketConstData.payWayStrList[good.PayType[0]];
				ShowMoney.ShowIcon(reliveView["mcMoney_"+j], reliveView["mcMoney_"+j].txtMoney, true);
				reliveView["txtGoodNamePet_"+j].mouseEnabled = false;
				reliveView["mcMoney_"+j].mouseEnabled = false;
				reliveView["btnBuy_"+j].addEventListener(MouseEvent.CLICK, buyHandler);
			}
			//
		}
		
		private function buyHandler(e:MouseEvent):void
		{
			var index:uint = uint(String(e.target.name).split("_")[1]);
			for(var i:int = 0; i < reliveView.numChildren; i++) {
				if(reliveView.getChildAt(i).name.indexOf("goodQuickBuy"+index+"_") == 0) {
					var type:uint = uint(reliveView.getChildAt(i).name.split("_")[1]);
					sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:type});
				}
			}
		}
		
		/** 获取位置 */
		private function getPos():Point 
		{
			var x:Number = (1000 - panelBase.width) / 2;
			var y:Number = (580 - panelBase.height) / 2;
			var p:Point  = new Point(x, y); 
			return p;
		}
		
		private function addLis():void
		{
			if(GameCommonData.Player.Role.Level && GameCommonData.Player.Role.Level <= 20) {
				reliveView.btnRelive.addEventListener(MouseEvent.CLICK, btnHandler);
			} else {
				reliveView.btnRelive.addEventListener(MouseEvent.CLICK, btnHandler);
				reliveView.btnCancel.addEventListener(MouseEvent.CLICK, btnHandler);
//				reliveView.btnBuy.addEventListener(MouseEvent.CLICK, btnHandler);
			}
//			backMask.addEventListener(MouseEvent.CLICK, btnHandler);
		}
		private function removeLis():void
		{
			if(GameCommonData.Player.Role.Level && GameCommonData.Player.Role.Level <= 20) {
				if(reliveView)
					reliveView.btnRelive.removeEventListener(MouseEvent.CLICK, btnHandler);
			} else {
				if(reliveView) {
					reliveView.btnRelive.removeEventListener(MouseEvent.CLICK, btnHandler);
					reliveView.btnCancel.removeEventListener(MouseEvent.CLICK, btnHandler);
				}
//				reliveView.btnBuy.removeEventListener(MouseEvent.CLICK, btnHandler);
			}
//			backMask.removeEventListener(MouseEvent.CLICK, btnHandler);
		}
		
		private function btnHandler(e:MouseEvent):void
		{
			switch(e.target.name)
			{
				case "btnRelive":	//原地满血复活
					if(GameCommonData.Player.Role.Level && GameCommonData.Player.Role.Level <= 20) {
//						facade.sendNotification(HintEvents.RECEIVEINFO, {info:"原地满血满魔复活了", color:0xffff00});
						//发送原地复活命令
						if(reliveTimer.running) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_relv_med_rm_bh_1" ], color:0xffff00});   //"死亡后5秒才能复活"
							return;
						}
						Zippo.PlayerRelive(1);
					} else {
						if(!BagData.isHasItem(630000)) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_relv_med_rm_bh_2" ], color:0xffff00});  //"背包中没有春鸽"
							return;
						}
						if(reliveTimer.running) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_relv_med_rm_bh_1" ], color:0xffff00});  //"死亡后5秒才能复活"
							return;
						}
						//发送原地复活命令
						Zippo.PlayerRelive(1);
					}
//					gc();
					break;
				case "btnCancel":	//回程复活	
//					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"回城复活了", color:0xffff00});
					//发送回程复活命令
					if(reliveTimer.running) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_relv_med_rm_bh_1" ], color:0xffff00});  //"死亡后5秒才能复活"
						return;
					}
					Zippo.PlayerRelive(0);
//					gc();
					break;
//				case "btnBuy":		//购买春哥
//					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"春鸽缺货", color:0xffff00});
//					return;
//					gc();
//					break;
//				case "deadMask":
//					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"死亡状态无法移动", color:0xffff00});
//					break;
			}
		}
		
		private function gc():void
		{
			removeLis();
			reliveTimer.reset();
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase)) {
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			}
			if(backMask && GameCommonData.GameInstance.GameUI.contains(backMask)) {
				GameCommonData.GameInstance.GameUI.removeChild(backMask);
			}
			viewComponent = null;
			panelBase	  = null;
			backMask	  = null;
			mcType		  = 0;
			setTimeout(useEnableKey, 1000);
		}
		
		private function useEnableKey():void
		{
			UIConstData.KeyBoardCanUse = true;		// 可用快捷键
		}
		
	}
}