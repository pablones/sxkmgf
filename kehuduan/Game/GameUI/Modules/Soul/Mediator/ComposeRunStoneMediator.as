package GameUI.Modules.Soul.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Soul.Components.DownListComponent;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Proxy.SoulProxy;
	import GameUI.Modules.Soul.View.QuickBuyComponent;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.MoneyItem;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	/**
	 * 魂魄—合成润魂石
	 * @author lh
	 * 
	 */
	public class ComposeRunStoneMediator extends Mediator
	{
		
		public static const NAME:String = "ComposeRunStoneMediator";
		public static const INITMEDIATOR:String = "initComposeRunStoneMediator";
		public static const SHOWVIEW:String = "showcomposeRunStonePanel";
		
		public static var isComposeRunStoneSend:Boolean;
		private var downList:DownListComponent;
		private var panelBase:PanelBase;
		private var stoneNum:int;	//要合成的润魂石type
		private var moneyAll:int;	//需要的金钱总数
		
		private var bindMoneyItem:MoneyItem;
		private var unBindMoneyItem:MoneyItem;
		private var needMoney:MoneyItem;
		private var quickBuy:QuickBuyComponent;//面板和快速购买
		public function ComposeRunStoneMediator(mediatorName:String=null, viewComponent:Object=null)
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
				SoulProxy.CLOSE_ALL_SOUL_PANEL,
				EventList.UPDATEMONEY,
				EventList.SOUL_STONE_COMPOSE_SUCCED,
				EventList.ONSYNC_BAG_QUICKBAR
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case INITMEDIATOR:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"composeRunStone"});
					quickBuy = new QuickBuyComponent(this.mainView,591101);
//					panelBase = new PanelBase(mainView, mainView.width-3, mainView.height + 12 );
					panelBase = new PanelBase(quickBuy, quickBuy.width - 4, quickBuy.height + 12 );
					panelBase.name = "composeRunStonePanel";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.x = UIConstData.DefaultPos1.x+200;
					panelBase.y = UIConstData.DefaultPos1.y;
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_soul_med_comRun_hand" ]);	//"合成润魂石"
					initView();
				break;
				case SHOWVIEW:
					showView();
				break;
				case EventList.SOUL_STONE_COMPOSE_SUCCED:
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
					if(int(int(notification.getBody())/10) == 59110)
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
			
			mainView.addChild( bindMoneyItem );
			mainView.addChild( unBindMoneyItem );
			mainView.addChild( needMoney );
			upDataMoney();
			upDateNeedMoney( 0 );
		}
		
		
		private function upDataMoney():void
		{
			this.bindMoneyItem.update(UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.UnBindMoney, ["\\se","\\ss","\\sc"]));
			this.unBindMoneyItem.update(UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.BindMoney, ["\\ce","\\cs","\\cc"]));
		}
		/**
		 * 所需金钱更新 
		 * @param money
		 * 
		 */		
		private function upDateNeedMoney( money:uint ):void
		{
			this.needMoney.update(UIUtils.getMoneyStandInfo( money, ["\\se","\\ss","\\sc"]) );
		}
		/**
		 *显示 
		 * 
		 */		
		private function showView():void
		{
			sendNotification(SoulProxy.CLOSE_ALL_SOUL_PANEL);
			if(GameCommonData.GameInstance.GameUI.contains(this.panelBase))
			{
				this.panelCloseHandler(null);
				return;
			}
			
			downList = new DownListComponent();
			var dArr:Array = [];
			for ( var i:uint=2; i<10; i++ )
			{
				var str:String = i + GameCommonData.wordDic[ "mod_soul_med_comRun_showView" ];	//"级润魂石";
				dArr.push( str );
			}
			downList.dataList = dArr;
			downList.x = 74;
			downList.y = 60;
			downList.clickFun = clickDownList;
			mainView.addChild( downList );
			
			(this.mainView.txt_percent as TextField).text = "100%";
			(this.mainView.txt_explain as TextField).htmlText = getPercentTxt(0);
			upDateNeedMoney(SoulData.jade[0].gold);
			dealEventListeners(true);
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
		}
		/**
		 *加事件监听 
		 * @param isAdd
		 * 
		 */		
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
		private function clickDownList( str:String ):void
		{
			var num:int = int(str.substr(0,1));
			stoneNum = 591100 + num;
			if(stoneNum <= 591104)
			{
				quickBuy.setFast(591101);
			}
			else
			{
				quickBuy.setFast(591104);
			}
			moneyAll = SoulData.jade[num-2].gold;
			upDateNeedMoney( moneyAll );
			(this.mainView.txt_explain as TextField).htmlText = getPercentTxt();
		}
		
		private function getPercentTxt(num:int = 1):String
		{
			if(num == 0) //界面打开
			{
				stoneNum = 591102;
				moneyAll = SoulData.jade[0].gold;
				moneyAll = SoulData.jade[(stoneNum-1)%10 -1].gold;
			}
			upDateNeedMoney(moneyAll);
			var toolNum:int = BagData.hasItemNum(stoneNum-1);
			var explainTxt:String = GameCommonData.wordDic[ "mod_soul_med_comRun_getPre_1" ]+'<font color="#00FF00">3</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ffff">'+(stoneNum-1)%10+GameCommonData.wordDic[ "mod_soul_med_comRun_showView" ]+'</font><br>'+GameCommonData.wordDic[ "mod_soul_med_comRun_getPre_2" ]+'<font color="#00FF00">'+ toolNum + '</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ];//需要 	个	级润魂石 		当前拥有		个';
			return explainTxt;	
		}
		
		private function dealAfterSend():void
		{
			if(isComposeRunStoneSend)
			{
				upDateNeedMoney(0);
				isComposeRunStoneSend = false;
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comRun_deal" ], color:0xffff00});	//"合成成功"
			}
			(this.mainView.txt_explain as TextField).htmlText = getPercentTxt();
			
		}
		
		private function onMouseClick(me:MouseEvent):void
		{
			switch(me.target.name)
			{
				case "btn_sure":
					if(stoneNum == 0)
					{	
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comRun_onMouseC_1" ], color:0xffff00});	//"请选择要合成的道具"
						return;
					}
					 if(BagData.hasItemNum(stoneNum-1) < 3)	//是否有足够润魂石
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comRun_onMouseC_2" ], color:0xffff00});//"没有足够的对应等级的润魂石"
						return;
					}
					 if(BagData.bagIsFull(stoneNum))
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comRun_onMouseC_3" ], color:0xffff00});//"您的背包中没有足够的空间"
						return;
					}
					 if(SoulProxy.getPlayTotalMoney() < moneyAll)
					 {
					 	facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comRun_onMouseC_4" ], color:0xffff00});//"没有足够银两"
						return;
					 }
					isComposeRunStoneSend = true;
					SoulProxy.getComposeStone(stoneNum);
				break;
				case "btn_cancel":
					this.panelCloseHandler(null);
				break;
			
			}
			
		}
		
		private function panelCloseHandler(e:Event):void
		{
			if ( downList && mainView.contains( downList ) )
			{
				downList.gc();
				mainView.removeChild( downList );
				downList = null;
			}
			isComposeRunStoneSend = false;
			dealEventListeners(false);
			GameCommonData.GameInstance.GameUI.removeChild(this.panelBase);
		}
	}
}