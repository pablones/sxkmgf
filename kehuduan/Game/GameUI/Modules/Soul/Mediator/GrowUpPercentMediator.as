package GameUI.Modules.Soul.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Proxy.SoulProxy;
	import GameUI.Modules.Soul.View.QuickBuyComponent;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.MoneyItem;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	/**
	 * 魂魄-提升成长率完成
	 * @author lh
	 * 
	 */
	public class GrowUpPercentMediator extends Mediator
	{
		public static const NAME:String = "GorwUpPercentMediator";
		public static const INITMEDIATOR:String = "initGorwUpPercentMediator";
		public static const SHOWVIEW:String = "showGorwUpPercentPanel";
		public static const DEAL_AFTER_SEND_GROW_UP_PERCENT:String = "dealAfterSendGrowUpPercent";		
		
		public static var isGrowUpPercentSend:Boolean;
		public var panelBase:PanelBase;
		private var tempGrowUp:Number;
		private var bindMoneyItem:MoneyItem;
		private var unBindMoneyItem:MoneyItem;
		private var needMoney:MoneyItem;
		private var moneyAll:int;	//需要的金钱总数
		private var quickBuy:QuickBuyComponent;//面板和快速购买
		public function GrowUpPercentMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		private function get mainView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		override public function listNotificationInterests():Array
		{ 
			return [
				INITMEDIATOR,
				SHOWVIEW,
				DEAL_AFTER_SEND_GROW_UP_PERCENT,
				SoulProxy.CLOSE_ALL_SOUL_PANEL,
				EventList.UPDATEMONEY,
				EventList.ONSYNC_BAG_QUICKBAR
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case INITMEDIATOR:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"gorwUpPercent"});
					quickBuy = new QuickBuyComponent(this.mainView,591002,"GorwUpPercentPanel");
//					panelBase = new PanelBase(mainView, mainView.width-15, mainView.height + 12 );
					panelBase = new PanelBase(quickBuy, quickBuy.width-8, quickBuy.height + 12 );
					panelBase.name = "GorwUpPercentPanel";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.x = UIConstData.DefaultPos1.x+200;
					panelBase.y = UIConstData.DefaultPos1.y;
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_soul_med_growUp_hand" ]);	//"提升成长率"
					initView();
				break;
				case SHOWVIEW:
					showView();
				break;
				case DEAL_AFTER_SEND_GROW_UP_PERCENT:
					dealAfterSend();
				break;
				case SoulProxy.CLOSE_ALL_SOUL_PANEL:
					if(GameCommonData.GameInstance.GameUI.contains(this.mainView))
					{
						panelCloseHandler(null);
					}
				break;
				case EventList.UPDATEMONEY:
					upDataMoney();
				break;
				case EventList.ONSYNC_BAG_QUICKBAR:
					if(int(notification.getBody()) == 591002)
					{
						if(GameCommonData.GameInstance.GameUI.contains(this.mainView))
						{
							dealAfterSend();
						}
					}
				break;
			}
		}
		
		private function initView():void
		{
			(this.mainView.txt_percent as TextField).mouseEnabled = false;
			(this.mainView.txt_percent as TextField).autoSize = TextFieldAutoSize.CENTER;
			(this.mainView.txt_beforeGrow as TextField).mouseEnabled = false;
			(this.mainView.txt_afterGrow as TextField).mouseEnabled = false;
//			(this.mainView.txt_describe as TextField).mouseEnabled = false;
			(this.mainView.txt_explain as TextField).mouseEnabled = false;
		
			bindMoneyItem = new MoneyItem();
			bindMoneyItem.x = 20;
			bindMoneyItem.y = 238;
			unBindMoneyItem = new MoneyItem();
			unBindMoneyItem.x = 20;
			unBindMoneyItem.y = 260;
			needMoney = new MoneyItem();
			needMoney.x = 20;
			needMoney.y = 216;
			mainView.addChild(bindMoneyItem);
			mainView.addChild(unBindMoneyItem);
			mainView.addChild(needMoney);
			upDataMoney();
			upDateNeedMoney( 0 );
		}
		
		private function upDataMoney():void
		{
			this.bindMoneyItem.update(UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.UnBindMoney, ["\\se","\\ss","\\sc"]));
			this.unBindMoneyItem.update(UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.BindMoney, ["\\ce","\\cs","\\cc"]));
		}
		
		private function upDateNeedMoney( money:uint ):void
		{
			this.needMoney.update(UIUtils.getMoneyStandInfo( money, ["\\se","\\ss","\\sc"]) );
		}
		
		private function showView():void
		{
			sendNotification(SoulProxy.CLOSE_ALL_SOUL_PANEL);
			if(SoulMediator.soulVO.growPercent == 1000)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_growUp_showView" ], color:0xffff00});	//"成长率已满，无需提升"
				return;
			}
			sendNotification(SoulProxy.CLOSE_ALL_SOUL_PANEL);
			if(GameCommonData.GameInstance.GameUI.contains(this.panelBase))
			{
				this.panelCloseHandler(null);
				return;
			}
			tempGrowUp = SoulMediator.soulVO.growPercent;
			dealEventListeners(true);
			initTxt();
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
		}
		
		private function dealEventListeners(isAdd:Boolean):void
		{
			if(isAdd)
			{
				(this.mainView.btn_sure as DisplayObject).addEventListener(MouseEvent.CLICK,onMouseClick);
				(this.mainView.btn_cancel as DisplayObject).addEventListener(MouseEvent.CLICK,onMouseClick);
			}
			else
			{
				(this.mainView.btn_sure as DisplayObject).removeEventListener(MouseEvent.CLICK,onMouseClick);
				(this.mainView.btn_cancel as DisplayObject).removeEventListener(MouseEvent.CLICK,onMouseClick);
			}
		}
		private function initTxt():void
		{
			var infoObj:Object = getPercentTxt();
			(this.mainView.txt_percent as TextField).htmlText = infoObj.pTxt;
 			(this.mainView.txt_beforeGrow as TextField).htmlText = infoObj.aTxt;
			(this.mainView.txt_afterGrow as TextField).htmlText = infoObj.bTxt; 
			(this.mainView.txt_explain as TextField).htmlText = infoObj.eTxt;
			
		}
		/**
		 * 获得成长率
		 * @return 
		 * 
		 */		
		private function getPercentTxt():Object
		{
			
			var gp:int = SoulMediator.soulVO.growPercent;
			if(gp == 0)
			{
				gp = 500;
			}
			var obj:Object = getDescribeStr(gp,1);
			var percentTxt:String = obj.growUp+"%";
			if(obj.growUp == 100)
			{
				(this.mainView.txt_percent as TextField).textColor = 0x00FF00;
			}
			else
			{
				(this.mainView.txt_percent as TextField).textColor = 0xFF0000;
			}
			var afterStr:String = obj.str;
			var beforeStr:String = getDescribeStr(gp+10).str;
			
			var toolNum:int = BagData.hasItemNum(591002);//凝炼珠
			var explainTxt:String = GameCommonData.wordDic[ "mod_soul_med_comRun_getPre_1" ]+'<font color="#00FF00">1</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ffff">'+GameCommonData.wordDic[ "mod_soul_med_growUp_getP_1" ]+'</font><br>'+GameCommonData.wordDic[ "mod_soul_med_comRun_getPre_2" ]+'<font color="#00FF00">'+ toolNum + '</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ];	//需要	个	凝炼精华	当前拥有	个
			return {pTxt:percentTxt,aTxt:afterStr,bTxt:beforeStr,eTxt:explainTxt};
		}
		private function getDescribeStr(growStr:int,num:int = 0):Object
		{
			var descStr:String;
			if(500 <= growStr)
			{
				if(growStr <= 649)
				{
					descStr = '<font color="'+IntroConst.itemColors[0]+'">'+growStr+'('+GameCommonData.wordDic[ "mod_pet_med_petu_sho_16" ]+')</font>';	//普通
				}
				else if(growStr <= 749)
				{
					descStr = '<font color="'+IntroConst.itemColors[2]+'">'+growStr+'('+GameCommonData.wordDic[ "mod_pet_med_petu_sho_15" ]+')</font>';	//优秀
				}
				else if(growStr <= 849)
				{
					descStr = '<font color="'+IntroConst.itemColors[3]+'">'+growStr+'('+GameCommonData.wordDic[ "mod_pet_med_petu_sho_14" ]+')</font>';	//杰出
				}
				else if(growStr <= 949)
				{
					descStr = '<font color="'+IntroConst.itemColors[4]+'">'+growStr+'('+GameCommonData.wordDic[ "mod_pet_med_petu_sho_13" ]+')</font>';	//卓越
				}
				else if(growStr <= 1000)
				{
					descStr = '<font color="'+IntroConst.itemColors[5]+'">'+growStr+'('+GameCommonData.wordDic[ "mod_pet_med_petu_sho_12" ]+')</font>';	//完美
				}
			}
			if(num == 1)
			{
				moneyAll = SoulData.getGrowth(growStr).gold;
				upDateNeedMoney(moneyAll);
			}
			return {str:descStr,growUp:SoulData.getGrowth(growStr).compound};
		}
		
		private function dealAfterSend():void
		{
			if(isGrowUpPercentSend)
			{
				isGrowUpPercentSend = false;
				if(SoulMediator.soulVO.growPercent > tempGrowUp)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_growUp_deal" ], color:0xffff00});	//"成长率提升成功"
				}
				tempGrowUp = SoulMediator.soulVO.growPercent;
			}
			this.initTxt();
		}
		
		private function onMouseClick(me:MouseEvent):void
		{
			
			switch(me.target.name)
			{
				case "btn_sure":
					if(!RolePropDatas.ItemList[15])
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_growUp_onMouseC_1" ], color:0xffff00});	//"您当前没有准备魂魄"
						return;
					}
					if(BagData.hasItemNum(591002) == 0)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_growUp_onMouseC_2" ], color:0xffff00});	//"您的背包中没有凝炼精华"
						return;
					}
					if(SoulProxy.getPlayTotalMoney() < moneyAll)
					{
					 	facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comRun_onMouseC_4" ], color:0xffff00});//"没有足够银两"
						return;
					}
					if(SoulMediator.soulVO.growPercent == 1000)
					{
					 	facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_growUp_onMouseC_3" ], color:0xffff00});//"成长率已满，无需提升"
						return;
					}
					
					isGrowUpPercentSend = true;
					SoulProxy.getGrowUp();
				break;
				case "btn_cancel":
					this.panelCloseHandler(null);
				break;
			}
		}
		
		
		
		private function panelCloseHandler(e:Event):void
		{
			isGrowUpPercentSend = false;
			dealEventListeners(false);
			GameCommonData.GameInstance.GameUI.removeChild(this.panelBase);
		}
	}
}