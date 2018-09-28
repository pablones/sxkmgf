package GameUI.Modules.AutoPlay.mediator
{
	/** 离线挂机UI */
	import Controller.TerraceController;
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.AutoPlay.Data.AutoPlayData;
	import GameUI.Modules.AutoPlay.command.AutoPlayEventList;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Maket.Data.MarketEvent;
	import GameUI.Modules.PrepaidLevel.Data.PrepaidUIData;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.UseItem;
	
	import Net.ActionSend.PlayerActionSend;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	public class OffLinePlayMediator extends Mediator
	{
		public static const NAME:String = "OffLinePlayMediator";
		
		private var panelBase:PanelBase;
		private var offLineView:MovieClip;
		private var dataProxy:DataProxy;
		
		private var playerExp:int;				//玩家的一倍经验
		public function OffLinePlayMediator()
		{
			super(NAME);
		}
		public override function listNotificationInterests():Array
		{
			return	[
						EventList.INITVIEW,
						AutoPlayEventList.SHOW_OFFLINEPLAY,
						AutoPlayEventList.CLOSE_OFFLINEPLAY,
						AutoPlayEventList.GET_OFFLINEPLAYDATA,
						AutoPlayEventList.GET_OFFLINEAWARD,
						AutoPlayEventList.SHOW_OFFLINEPLAY_FROM_PREPAID
					];
		}
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
//					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					offLineView = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("OffLineView");
					offLineView.mouseEnabled = false;
					this.panelBase = new PanelBase(this.offLineView,this.offLineView.width+10,this.offLineView.height+12);
					
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_med_off_hand_1" ]);//"离线经验"
				break;
				case AutoPlayEventList.SHOW_OFFLINEPLAY:
					if(AutoPlayData.offLineIsOpen == false)
					{
						initGrid();
						showView();
						addLis();
						showExp();
						btnRadioInit(0);
	//					//计算碎银
						countMoney();
						//如果离线时间小于1分钟(用按钮点开面板)
						if(AutoPlayData.offLineTime  < 1) offLineView.btnConfirm.visible = false;
						else offLineView.btnConfirm.visible = true;
					}
					else gcAll();
				break;
				case AutoPlayEventList.CLOSE_OFFLINEPLAY:
					gcAll();
				break;
				case AutoPlayEventList.GET_OFFLINEPLAYDATA:
					AutoPlayData.dataIsSendOver = true;										//数据传送完毕
					var data:int = notification.getBody() as Number;
					AutoPlayData.offLineTime = data;
//					AutoPlayData.offLineTime = 1;/////////////////////////测试用
					if(AutoPlayData.offLineTime  < 1) return;
//					facade.sendNotification(AutoPlayEventList.SHOW_OFFLINEPLAY);			//打开离线挂机面板
//					PrepaidLevelNet.sendPrepaidDemand(1); 
//					PrepaidUIData.openFrom = "offline";
					sendNotification( PrepaidUIData.SHOW_OFFLINE_VIEW );
				break;
				case AutoPlayEventList.GET_OFFLINEAWARD:									//领取经验是否失败成功
					if(notification.getBody() == true)
					{
						if(AutoPlayData.offLineIsOpen == true)
						{
							gcAll();
						} 
						AutoPlayData.offLineTime = 0;	//领取成功后，下线时间清0
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_off_hand_2" ], color:0xffff00});//"离线经验领取成功"
						sendNotification( PrepaidUIData.UPDATE_OFFLINE_VIEW );
					}
					else
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_off_hand_3" ], color:0xffff00});//"离线经验领取失败"
					}
				break;
				case AutoPlayEventList.SHOW_OFFLINEPLAY_FROM_PREPAID:
					showOffLine( notification.getBody() as uint );
					break;
			}
		}
		
		private function showOffLine( type:uint ):void
		{
			if(AutoPlayData.offLineIsOpen == true) gcAll();
			
			AutoPlayData.offLineCurrentType = type;
			sure();
		}
		
		public function getPanelBase():PanelBase
		{
			return this.panelBase;
		}
		
		/** 初始化格子 */
		private function initGrid():void
		{
			//快速购买
			for(var j:int = 0; j < 1; j++) {
				if(!(UIConstData.MarketGoodList[29] as Array)[j]) continue;
				var good:Object = (UIConstData.MarketGoodList[29] as Array)[j];
				var unit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				unit.x = 283;
				unit.y = 12;
				unit.name = "goodQuickBuy"+j+"_"+good.type;;
				offLineView.addChild(unit);
				
				var useItem:UseItem = new UseItem(j, good.type, offLineView);
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
				
				offLineView.addChild(useItem);
				
//				offLineView["txtGoodNamePet_"+j].text = good.Name;
				offLineView.txtPriceIn.text = good.PriceIn; //+ MarketConstData.payWayStrList[good.PayType[0]];
//				ShowMoney.ShowIcon(offLineView["mcMoney_"+j], offLineView["mcMoney_"+j].txtMoney, true);
//				offLineView["txtGoodNamePet_"+j].mouseEnabled = false;
//				offLineView["mcMoney_"+j].mouseEnabled = false;
				offLineView["btnBuy"].addEventListener(MouseEvent.CLICK, buyHandler);
			}
		}
		
		private function showView():void
		{
//			dataProxy.OfflinePlayViewIsOpen = true;
		    if( GameCommonData.fullScreen == 2 )
		    {
		    	panelBase.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - panelBase.width)/2;
		    	panelBase.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - panelBase.height)/2;
	        }else{
				panelBase.x = 350;
				panelBase.y = 180;
	        }
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			AutoPlayData.offLineIsOpen = true;
		}
		
		private function panelCloseHandler(e:Event):void
		{
			gcAll();
		}
		private function addLis():void
		{
			for(var i:int = 0;i < 4;i++)
			{
				offLineView["mcRadio_"+i].addEventListener(MouseEvent.CLICK , setExpNumHandler);
			}
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			offLineView.btnConfirm.addEventListener(MouseEvent.CLICK , getExpHandler);
			
		}
		private function gcAll():void
		{
			AutoPlayData.offLineIsOpen = false;
//			dataProxy.OfflinePlayViewIsOpen == false;
			panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
			GameCommonData.GameInstance.GameUI.removeChild(panelBase); 
			
			for(var i:int = 0;i < 4;i++)
			{
				offLineView["mcRadio_"+i].removeEventListener(MouseEvent.CLICK , setExpNumHandler);
			}
			offLineView.btnConfirm.removeEventListener(MouseEvent.CLICK , getExpHandler);
		}
		/** 计算碎银 */
		private function countMoney():void
		{
			AutoPlayData.offLineMoney = (140 * GameCommonData.Player.Role.Level + 70) * AutoPlayData.offLineTime;
		}
		/** 显示可领取的经验 和 时间 */
		private function showExp():void
		{
			var x:int = GameCommonData.Player.Role.Level;
			AutoPlayData.offLineExp = AutoPlayData.offLineTime *int(-1.06 * 0.01 * Math.pow(x ,3) + 3.26 * Math.pow(x , 2) + 32.7 * x + 384);
			offLineView.txtGetExp.text = AutoPlayData.offLineExp;
			offLineView.txtNum.text = Math.pow(2 , AutoPlayData.offLineCurrentType);
			offLineView.txtTime.text = AutoPlayData.offLineTime + GameCommonData.wordDic[ "mod_med_showExp" ];//"个小时）"
			offLineView.txtTime.textColor = 0xCC0033;
		}
		/** 点击购买逍遥丹 */
		private function buyHandler(e:MouseEvent):void
		{
			var index:uint = uint(String(e.target.name).split("_")[1]);
			for(var i:int = 0; i < offLineView.numChildren; i++) {
				if(offLineView.getChildAt(i).name.indexOf("goodQuickBuy"+index+"_") == 0) {
					var type:uint = uint(offLineView.getChildAt(i).name.split("_")[1]);
					sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:type});
				}
			}
		}
		/** 点击领取经验按钮 */
		private function getExpHandler(e:MouseEvent):void
		{
			if(GameCommonData.Player.Role.Level < 15)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_getExp_1" ], color:0xffff00});//"15级以下不能领取离线经验"
				return;
			}
			//如果经验已满，则提示，不准领取
			if(GameCommonData.Player.Role.Exp >= (UIConstData.ExpDic[GameCommonData.Player.Role.Level] * 4))
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_getExp_9" ], color:0xffff00});//"您的经验已满，请手动升级"
				return;
			}
			var type:int = AutoPlayData.offLineCurrentType;
			var info:String;
			var money:int = AutoPlayData.offLineMoney ;
			if(type == 0) 
			{
				sure();
				return;
			}
			else if(type == 1)
			{
				var jin:String =  money / Math.pow(100 , 2) > 0 ? (int(money / Math.pow(100 , 2))+GameCommonData.wordDic[ "often_used_gold" ]).toString():"";//"金" 
				var yin:String = int(money % 10000) / Math.pow(100 , 1) > 0 ? (int(int(money % 10000) / Math.pow(100 , 1))+GameCommonData.wordDic[ "often_used_sliver" ]).toString():"";//"银"
				var tong:String = int(money % 100) / Math.pow(100 , 0) > 0 ? (int(int(money % 100) / Math.pow(100 , 0))+GameCommonData.wordDic[ "often_used_copper" ]).toString():"0";//"铜" 
				info = jin + yin + tong + "<font color='#FFFFFF'>"+GameCommonData.wordDic[ "mod_med_getExp_3" ]+"</font>" + int(AutoPlayData.offLineExp * 2);//"领取"
			}
			else if(type == 2 || type == 3)  info = (type -1) + GameCommonData.wordDic[ "mod_med_getExp_4" ] + "<font color='#FFFFFF'>"+GameCommonData.wordDic[ "mod_med_getExp_3" ]+"</font>" + int(AutoPlayData.offLineExp * Math.pow(2 , type));// "个逍遥丹"  "领取"
			info = "<font color='#00FF00'>" + info + "</font>";
			facade.sendNotification(EventList.SHOWALERT, {comfrim:sure, cancel:cancel, isShowClose:false, info: GameCommonData.wordDic[ "mod_med_getExp_5" ]+info+GameCommonData.wordDic[ "mod_med_getExp_6" ], title:GameCommonData.wordDic[ "often_used_tip" ], comfirmTxt:GameCommonData.wordDic[ "often_used_confim" ], cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});//"确定要消耗"	"经验吗？"	"提示"	"确定"	"取消"	
			
		}
		/** 点击不同倍数挂机 */
		private function setExpNumHandler(e:MouseEvent):void
		{
			var type:int = (e.currentTarget.name as String).split("_")[1];
			if(AutoPlayData.offLineCurrentType == type) return;
			selectOneOffLine(type);
		}
		/** 发送领取经验的请求方法 */
		private function sendRequest():void
		{
			var num:int = Math.pow(2 , AutoPlayData.offLineCurrentType);
			PlayerActionSend.PlayerAction({type:1010,data:[0,0,0,0,0,num,278,0,0]});
		}
		/** 选取离线挂机的种类的提示 */
		private function selectOneOffLine(type:int):void
		{
			
			//碎银
			if(type == 1)
			{
				if(int(GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney) < AutoPlayData.offLineMoney)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_select_1" ], color:0xffff00});//"你身上的银两不足"
					return;
				}
			}
			//逍遥丹
			else if(type == 2||type == 3)
			{
				if(BagData.hasItemNum(AutoPlayData.XIAOYAODANTYPE)<int(type-1))
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_getExp_2" ], color:0xffff00});//"你身上的逍遥丹不足"
					return;
				}
			}
			AutoPlayData.offLineCurrentType = type;
			btnRadioInit(type);
			offLineView.txtNum.text = Math.pow(2 , type);
		}
		/** 单选按钮 */
		private function btnRadioInit(type:int):void
		{
			for(var i:int = 0;i < 4;i++)
			{
				offLineView["mcRadio_"+i].gotoAndStop(1);
			}
			offLineView["mcRadio_"+type].gotoAndStop(2);
		}
		/** 确定领取*/
		private function sure():void
		{
			
			sendRequest();//如果请求失败，则重新请求
		}
		/** 取消领取 */
		private function cancel():void
		{
			
		}
	}
}